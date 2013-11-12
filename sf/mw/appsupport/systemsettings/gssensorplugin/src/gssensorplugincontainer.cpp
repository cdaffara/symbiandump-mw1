/*
* Copyright (c) 2006 Nokia Corporation and/or its subsidiary(-ies). 
* All rights reserved.
* This component and the accompanying materials are made available
* under the terms of "Eclipse Public License v1.0"
* which accompanies this distribution, and is available
* at the URL "http://www.eclipse.org/legal/epl-v10.html".
*
* Initial Contributors:
* Nokia Corporation - initial contribution.
*
* Contributors:
*
* Description:  Container for the sensor settings
*
*/



// INCLUDE FILES
#include "gssensorplugincontainer.h"
#include "gssensormskobserver.h"
#include "gssensorpluginmodel.h"
#include "gssensorplugin.hrh"
#include "gssensorplugin_debug.h"

#include <aknlists.h>
#include <csxhelp/cp.hlp.hrh>
#include <gsfwviewuids.h>     // for KUidGS
#include <gssensorpluginrsc.rsg>
#include <gslistbox.h>
#include <gsplugininterface.h>

// ========================== MEMBER FUNCTIONS ================================

// ---------------------------------------------------------------------------
// CGSSensorPluginContainer::ConstructL
// This constructL is not used
// ---------------------------------------------------------------------------
//
void CGSSensorPluginContainer::ConstructL( const TRect& /*aRect*/ )
    {
    TRACE_( "[GSSensorPlugin] CGSSensorPluginContainer::ConstructL() - deprecated" );
    User::Leave( KErrArgument );
    }

// ---------------------------------------------------------------------------
// CGSSensorPluginContainer::ConstructL
// ---------------------------------------------------------------------------
//
void CGSSensorPluginContainer::ConstructL( 
    const TRect& aRect,
    CAknViewAppUi* aAppUi,
    CArrayPtrFlat<CGSPluginInterface>* aPluginArray )
    {
    TRACE_( "[GSSensorPlugin] CGSSensorPluginContainer::ConstructL()" );

    iAppUi = aAppUi;
    iPluginArray = aPluginArray;

    // Create list box and parent container
    iListBox = new ( ELeave ) CAknSettingStyleListBox;
    BaseConstructL( aRect, R_GS_SEN_VIEW_TITLE, R_GS_SEN_DOUBLE_LBX );

    TRACE_( "[GSSensorPlugin] CGSSensorPluginContainer::ConstructL() - return" );  
    }

// ---------------------------------------------------------------------------
// CGSSensorPluginContainer::~CGSSensorPluginContainer
// ---------------------------------------------------------------------------
//
CGSSensorPluginContainer::~CGSSensorPluginContainer()
    {
    TRACE_( "[GSSensorPlugin] CGSSensorPluginContainer::~CGSSensorPluginContainer()" );

    if ( iListboxItemArray )
        {
        delete iListboxItemArray;
        }

     if ( iListBoxTextArray )
        {
        iListBoxTextArray->Reset();
        delete iListBoxTextArray;
        }
    
    if ( iSensorStatusText )
        {
        iSensorStatusText->Reset();
        delete iSensorStatusText;
        }

    if ( iListItems )
        {
        iListItems->Reset();
        delete iListItems;
        }

    iModel = NULL;
    iAppUi = NULL;
    iPluginArray = NULL;

    TRACE_( "[GSSensorPlugin] CGSSensorPluginContainer::~CGSSensorPluginContainer() - return" );
    }

// ---------------------------------------------------------------------------
// CGSSensorPluginContainer::CGSSensorPluginContainer
// ---------------------------------------------------------------------------
//
CGSSensorPluginContainer::CGSSensorPluginContainer( 
    CGSSensorPluginModel* aModel,
    MGSSensorMskObserver& aMskObserver )
    : iModel ( aModel ),
      iMskObserver( aMskObserver )
    {
    TRACE_( "[GSSensorPlugin] CGSSensorPluginContainer::CGSSensorPluginContainer()" );
    }

// ---------------------------------------------------------------------------
// CGSSensorPluginContainer::ConstructListBoxL
// ---------------------------------------------------------------------------
//
void CGSSensorPluginContainer::ConstructListBoxL( TInt aResLbxId )
    {
    TRACE_( "[GSSensorPlugin] CGSSensorPluginContainer::ConstructListBoxL()" );
    iListBox->ConstructL( this, EAknListBoxSelectionList );
    iListboxItemArray = CGSListBoxItemTextArray::NewL( aResLbxId, *iListBox, *iCoeEnv );
    iListBoxTextArray = static_cast<CDesCArray*>( iListBox->Model()->ItemTextArray() );
    iListBox->Model()->SetOwnershipType( ELbmDoesNotOwnItemArray );

    // Get sensors activation status and set text 'On' or 'Off' to list view below 
    // Sensors list item
    iSensorStatusText = iCoeEnv->ReadDesC16ArrayResourceL( R_GS_SENSOR_ACTIVE_STATUS );
    iListItems = iCoeEnv->ReadDesC16ArrayResourceL( R_GS_SEN_DOUBLE_LBX_TEXT );
    // Create and make visible list box items
    CreateListBoxItemsL();

    iListBox->HandleItemAdditionL();
    TRACE_( "[GSSensorPlugin] CGSSensorPluginContainer::ConstructListBoxL() - return" );
    }

// ---------------------------------------------------------------------------
// CGSSensorPluginContainer::CreateListBoxItemsL
// ---------------------------------------------------------------------------
//
void CGSSensorPluginContainer::CreateListBoxItemsL()
    {
    TRACE_( "[GSSensorPlugin] CGSSensorPluginContainer::CreateListBoxItemsL()" );

    iGeneralItemCount = 0;

    const TUint32 tapVariation = iModel->VariationListL( EGSSenTapping );
    const TUint32 turnVariation = iModel->VariationListL( EGSSenTurning );

    if ( tapVariation ||turnVariation )
        {
        // Make Sensors active status visible if there is at least one enabled use case
        MakeSensorsItemL();
        iGeneralItemCount++;
        
        if ( !tapVariation )
            {
            iListboxItemArray->SetItemVisibilityL( EGSTapCtrlListItemId,
                CGSListBoxItemTextArray::EInvisible );
            }
        else
            {
            MakeTapCtrlItemL();
            iGeneralItemCount++;
            }
        
        if ( !turnVariation )
            {
            iListboxItemArray->SetItemVisibilityL( EGSTurnCtrlListItemId,
                CGSListBoxItemTextArray::EInvisible );
            }
        else
            {
            MakeTurnCtrlItemL();
            iGeneralItemCount++;
            }
        }

    if ( iPluginArray )
        {
        MakeExtendedPluginsItemL();
        }


    TRACE_( "[GSSensorPlugin] CGSSensorPluginContainer::CreateListBoxItemsL() - return" );
    }

// ---------------------------------------------------------------------------
// CGSSensorPluginContainer::UpdateListBoxL
// ---------------------------------------------------------------------------
//
void CGSSensorPluginContainer::UpdateListBoxL( TInt aFeatureId )
    {
    TRACE_1( "[GSSensorPlugin] CGSSensorPluginContainer::UpdateListBoxL() - feature: %i", 
        aFeatureId );
    switch ( aFeatureId )
        {
        case EGSSensorsListItemId:
            {
            // Delete current text and then create new
            iListBoxTextArray->Delete( EGSSensorsListItemId );
            MakeSensorsItemL();
            break;
            }
        case EGSExtPluginsListItemId:
            MakeExtendedPluginsItemL();
            break;
        case EGSTapCtrlListItemId:
        case EGSTurnCtrlListItemId:
        case EGSOrientationListItemId:
        default:
            break;
        }

    // Draw listbox again
    iListBox->HandleItemAdditionL();
    TRACE_( "[GSSensorPlugin] CGSSensorPluginContainer::UpdateListBoxL() - return" );
    }

// ---------------------------------------------------------------------------
// CGSSensorPluginContainer::MakeSensorsItemL
// ---------------------------------------------------------------------------
//
void CGSSensorPluginContainer::MakeSensorsItemL()
    {
    TRACE_( "[GSSensorPlugin] CGSSensorPluginContainer::MakeSensorsItemL()" );
    const TInt sensorsStatus = iModel->SensorActiveStatusL();
    SetSensorStatusTextL( sensorsStatus );
    TRACE_( "[GSSensorPlugin] CGSSensorPluginContainer::MakeSensorsItemL() - return" );
    }

// ---------------------------------------------------------------------------
// CGSSensorPluginContainer::MakeTapCtrlItemL
// ---------------------------------------------------------------------------
//
void CGSSensorPluginContainer::MakeTapCtrlItemL()
    {
    TRACE_( "[GSSensorPlugin] CGSSensorPluginContainer::MakeTapCtrlItemL()" );
    AppendListItemL( EGSTapCtrlListItemId, 
                     iListItems->operator[]( EGSTapCtrlListItemId ), 
                     KNullDesC16 );
    TRACE_( "[GSSensorPlugin] CGSSensorPluginContainer::MakeTapCtrlItemL() - return" );
    }

// ---------------------------------------------------------------------------
// CGSSensorPluginContainer::MakeTurnCntrItemL
// ---------------------------------------------------------------------------
//
void CGSSensorPluginContainer::MakeTurnCtrlItemL()
    {
    TRACE_( "[GSSensorPlugin] CGSSensorPluginContainer::MakeTurnCtrlItemL()" );
    AppendListItemL( EGSTurnCtrlListItemId, 
                     iListItems->operator[]( EGSTurnCtrlListItemId ), 
                     KNullDesC16 );
    TRACE_( "[GSSensorPlugin] CGSSensorPluginContainer::MakeTurnCtrlItemL() - return" );
    }

// ---------------------------------------------------------------------------
// CGSSensorPluginContainer::MakeOrientationItemL
// ---------------------------------------------------------------------------
//
void CGSSensorPluginContainer::MakeOrientationItemL()
    {
    TRACE_( "[GSSensorPlugin] CGSSensorPluginContainer::MakeOrientationItemL()" );
    User::Leave( KErrNotSupported );
    TRACE_( "[GSSensorPlugin] CGSSensorPluginContainer::MakeOrientationItemL() - return" );
    }

// ---------------------------------------------------------------------------
// CGSSensorPluginContainer::MakeExtendedPluginsItemL
// ---------------------------------------------------------------------------
//
void CGSSensorPluginContainer::MakeExtendedPluginsItemL()
    {
    TRACE_( "[GSSensorPlugin] CGSSensorPluginContainer::MakeExtendedPluginsItemL()" );
    // Create list item for all child plug-ins
    const TInt pluginCount( iPluginArray->Count() );
    CGSPluginInterface* plugin = NULL;

    const TUint32 tapVariation = iModel->VariationListL( EGSSenTapping );
    const TUint32 turnVariation = iModel->VariationListL( EGSSenTurning );

    if ( ( pluginCount == 1 ) && ( !tapVariation && !turnVariation ) )
        {
        ActivateExtendedPluginL();
        }
    else
        {
        // 1) Read item descriptor from plugin:
        TBuf<KGSBufSize128> caption;

        for ( TInt i = 0; i < pluginCount; i++ )
            {
            plugin = iPluginArray->operator[]( i );
            plugin->GetCaptionL( caption );
            // Append text to list
            AppendListItemL( KErrNotFound, caption, KNullDesC16 );
            }
        }
    TRACE_( "[GSSensorPlugin] CGSSensorPluginContainer::MakeExtendedPluginsItemL() - return" );
    }

// ---------------------------------------------------------------------------
// CGSSensorPluginContainer::ActivateExtendedPluginL
// ---------------------------------------------------------------------------
//
void CGSSensorPluginContainer::ActivateExtendedPluginL()
    {
    TRACE_( "[GSSensorPlugin] CGSSensorPluginContainer::ActivateExtendedPluginL()" );
    // Not local view, try plugins
    if ( iPluginArray->Count() == 1)
        {
        CGSPluginInterface* selectedPlugin = iPluginArray->operator[]( 0 );
    
        switch( selectedPlugin->ItemType() )
            {
            // In these cases the plugin is a view:
            case EGSItemTypeSingleLarge:
            case EGSItemTypeSetting:
            case EGSItemTypeSettingIcon:
                {
                CleanupStack::PushL( selectedPlugin );
                iAppUi->ActivateLocalViewL( selectedPlugin->Id() );
                CleanupStack::Pop( selectedPlugin );
                break;
                }
                
            // In these cases the plugin is a dialog:
            case EGSItemTypeSettingDialog:
            case EGSItemTypeSingleLargeDialog:
                 selectedPlugin->HandleSelection( EGSSelectionByMenu );
                 break;
            default:
                break;
            }
        }
    TRACE_( "[GSSensorPlugin] CGSSensorPluginContainer::ActivateExtendedPluginL() - return" );
    }


// ---------------------------------------------------------------------------
// CGSSensorPluginContainer::AppendListItem
// ---------------------------------------------------------------------------
//
void CGSSensorPluginContainer::AppendListItemL( 
    const TInt aPos,
    const TDesC16& aFirstLabel, 
    const TDesC16& aSecondLabel )
    {
    TBuf<KGSBufSize128> itemText;
    _LIT( KSpaceAndTab, " \t" );
    _LIT( KTab, "\t" );

    // The format is: " \tFirstLabel\tSecondLabel\t0\t0"
    // First, append space and tab
    itemText.Append( KSpaceAndTab );
    itemText.Append( aFirstLabel );
    itemText.Append( KTab );
    itemText.Append( KTab );
    // Currently this is used only to show sensors active status (on/off)
    itemText.Append( aSecondLabel ); 

    // No need to add last tabs
    if ( ( KErrNotFound != aPos ) && ( aPos <= iListBoxTextArray->Count() ) )
        {
        iListBoxTextArray->InsertL( aPos, itemText );
        }
    else
        {
        iListBoxTextArray->AppendL( itemText );
        }
    }

// -----------------------------------------------------------------------------
// CGSSensorPluginContainer::SelectedPlugin
// Returns pointer to selected plugin
// -----------------------------------------------------------------------------
//
CGSPluginInterface* CGSSensorPluginContainer::SelectedPluginL()
    {
    TRACE_( "[GSSensorPlugin] CGSSensorPluginContainer::SelectedPlugin()" );
    CGSPluginInterface* plugin = NULL;

    // 1) Decrease index with the count
    TInt index = iListBox->CurrentItemIndex() - iGeneralItemCount;
    
    // 2) Check that new index is in range of plugins
    if ( index >= 0 && index < iPluginArray->Count() )
        {
        // 3) Get correct plugin
        plugin = iPluginArray->operator[]( index );
        }

    // Leave if not found
    if ( !plugin )
        {
        User::Leave( KErrNotFound );
        }

    TRACE_1( "[GSSensorPlugin] CGSSensorPluginContainer::SelectedPlugin() - return: 0x%x", plugin );
    return plugin;
    }

// ---------------------------------------------------------------------------
// CGSSensorPluginContainer::CurrentFeatureId
// ---------------------------------------------------------------------------
//
TInt CGSSensorPluginContainer::CurrentFeatureId() const
    {
    TRACE_( "[GSSensorPlugin] CGSSensorPluginContainer::CurrentFeatureId()" );
    return iListboxItemArray->CurrentFeature();
    }

// ---------------------------------------------------------------------------
// CGSSensorPluginContainer::OfferKeyEventL
// ---------------------------------------------------------------------------
//
TKeyResponse CGSSensorPluginContainer::OfferKeyEventL( 
    const TKeyEvent& aKeyEvent, TEventCode aType )
    {
    TRACE_( "[GSSensorPlugin] CGSSensorPluginContainer::OfferKeyEventL()" );

    TKeyResponse returnValue( EKeyWasNotConsumed );

    switch ( aKeyEvent.iCode )
        {
        case EKeyUpArrow:
        case EKeyDownArrow:
            {
            // Send key event to Listbox
            returnValue = iListBox->OfferKeyEventL( aKeyEvent, aType );
            // Change MSK label if needs to changed
            iMskObserver.CheckMiddleSoftkeyLabelL();
            break;
            }
        case EKeyLeftArrow:
        case EKeyRightArrow:
            {
            // Listbox takes all events even if it doesn't use them.
            // Stick with the default return value
            break;
            }
        default:
            {
            // Forward key event to listbox
            returnValue = iListBox->OfferKeyEventL( aKeyEvent, aType );
            break;
            }
        }

    TRACE_( "[GSSensorPlugin] CGSSensorPluginContainer::OfferKeyEventL() - return" );
    return returnValue;
    }

// ---------------------------------------------------------------------------
// CGSSensorPluginContainer::GetHelpContext
// ---------------------------------------------------------------------------
//
void CGSSensorPluginContainer::GetHelpContext( TCoeHelpContext& aContext ) const
    {
    TRACE_( "[GSSensorPlugin] CGSSensorPluginContainer::GetHelpContext()" );
    aContext.iMajor = KUidGS;
    aContext.iContext = KSSET_HLP_MAIN_VIEW;
    TRACE_( "[GSSensorPlugin] CGSSensorPluginContainer::GetHelpContext() - return" );
    }

// ---------------------------------------------------------------------------
// CGSSensorPluginContainer::SetSensorStatusTextL
// Setting Sensors status Text based on the value sent,
// '0' meams "Off" & '1' Means 'On'
// ---------------------------------------------------------------------------
//
void CGSSensorPluginContainer::SetSensorStatusTextL( TInt aValue )
    {
    TRACE_( "[GSSensorPlugin] CGSSensorPluginContainer::SetSensorStatusTextL()" );
    AppendListItemL( 
        EGSSensorsListItemId, 
        iListItems->operator[]( EGSSensorsListItemId ), 
        ( *iSensorStatusText )[aValue] );
    TRACE_( "[GSSensorPlugin] CGSSensorPluginContainer::SetSensorStatusTextL() - return" );
    }

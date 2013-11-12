/*
* Copyright (c) 2006-2010 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Base container for multiselection containers
*
*/


// INCLUDE FILES
#include "gssenbasecontainer.h" 
#include "gssensorpluginmodel.h"
#include "gssensorplugin.hrh"
#include "gssensorplugin_debug.h"
#include "gssensormskobserver.h"

#include <aknlists.h>
#include <gslistbox.h>
#include <gssensorpluginrsc.rsg>
#include <akncheckboxsettingpage.h>
#include <AknIconArray.h>
#include <aknconsts.h>
#include <avkon.mbg>
#include <gulicon.h>
#include <eiklbv.h>

// ========================= MEMBER FUNCTIONS ================================

// ---------------------------------------------------------------------------
// CGSSenBaseContainer::CGSSenBaseContainer
// Default constructor
// ---------------------------------------------------------------------------
//
CGSSenBaseContainer::CGSSenBaseContainer( MGSSensorMskObserver& aMskObserver ) : 
    iMskObserver( aMskObserver )
    {
    TRACE_( "[GSSensorPlugin] CGSSenBaseContainer::CGSSenBaseContainer()" );
    }

// ---------------------------------------------------------------------------
// CGSSenBaseContainer::~CGSSenBaseContainer
// Destructor
// ---------------------------------------------------------------------------
//
CGSSenBaseContainer::~CGSSenBaseContainer()
    {
    TRACE_( "[GSSensorPlugin] CGSSenBaseContainer::~CGSSenBaseContainer()" );
    if ( iSelectionItems )
        {
        delete iSelectionItems;
        }
    if ( iListboxItemArray )
        {
        delete iListboxItemArray;
        }

    iModel = NULL;
    TRACE_( "[GSSensorPlugin] CGSSenBaseContainer::~CGSSenBaseContainer() - return" );
    }

// ---------------------------------------------------------------------------
// CGSSenBaseContainer::ConstructListBoxL
// ---------------------------------------------------------------------------
//
void CGSSenBaseContainer::ConstructListBoxL( TInt aResLbxId, TInt aResMsId )
    {
    TRACE_( "[GSSensorPlugin] CGSSenBaseContainer::ConstructListBoxL()" );
    iListBox->ConstructL( this, EAknListBoxMultiselectionList );
    iListboxItemArray = CGSListBoxItemTextArray::NewL( 
        aResLbxId, *iListBox, *iCoeEnv );
    iListBox->Model()->SetItemTextArray( iListboxItemArray );
    iListBox->Model()->SetOwnershipType( ELbmDoesNotOwnItemArray );

    iSelectionItems = iCoeEnv->ReadDesC16ArrayResourceL(
                            aResMsId );


    const TInt interactionId = InteractionId();

    // Get current selections from CenRep
    iSelections = iModel->SelectionListL( interactionId );
    const TUint32 variation = iModel->VariationListL( interactionId );

    TUint bitmaskpos = 0;

    // Loop selections and delete disabled items
    for ( TInt i = 0; i < iSelectionItems->Count(); i++ )
        {
        TUint status( 0 );
        TUint mask( 0x01 << bitmaskpos++ );
    
        // Get value from member variable bitmask. It is updated when this view is launched
        // and will be stored when view is closed
        status = variation & mask;
        
        if (!status)
            {
            // Remove disabled item from iSelectionItems
            iSelectionItems->Delete( i );
            i--; // Decrease index
            }
        }

    // Add items to list box
    iListBox->Model()->SetItemTextArray( iSelectionItems );
    iListBox->HandleItemAdditionL();

    // Create checkbox icons
    AddCheckboxIconsL();

    // Update checkbox icons according to previous settings
    UpdateCheckboxIconsL();
    TRACE_( "[GSSensorPlugin] CGSSenBaseContainer::ConstructListBoxL() - return" );
    }

// ---------------------------------------------------------------------------
// CGSSenBaseContainer::AddCheckboxIconsL
// ---------------------------------------------------------------------------
//
void CGSSenBaseContainer::AddCheckboxIconsL()
    {
    TRACE_( "[GSSensorPlugin] CGSSenBaseContainer::AddCheckboxIconsL()" );
    CAknIconArray* iconArray = new( ELeave ) CAknIconArray( 1 );
    CleanupStack::PushL( iconArray );
    CFbsBitmap* checkboxOnBitmap = NULL;
    CFbsBitmap* checkboxOnBitmapMask = NULL;
    CFbsBitmap* checkboxOffBitmap = NULL;
    CFbsBitmap* checkboxOffBitmapMask = NULL;

    //CListItemDrawer is using this logical color as default for its marked icons
    TRgb defaultColor;
    defaultColor = ( ( CEikonEnv* )iCoeEnv )->Color( EColorControlText );

    // Create 'ON' checkbox icon
    AknsUtils::CreateColorIconLC( AknsUtils::SkinInstance(),
    			KAknsIIDQgnIndiCheckboxOn,
    			KAknsIIDQsnIconColors,
    			EAknsCIQsnIconColorsCG13,
    			checkboxOnBitmap,
    			checkboxOnBitmapMask,
    			KAvkonBitmapFile,
    			EMbmAvkonQgn_indi_checkbox_on,
    			EMbmAvkonQgn_indi_checkbox_on_mask,
    			defaultColor
    			);

    CGulIcon* checkboxOnIcon = CGulIcon::NewL( checkboxOnBitmap, checkboxOnBitmapMask );
    // cleanup checkboxOnBitmap, checkboxOnBitmapMask
    CleanupStack::Pop( checkboxOnBitmapMask );
    CleanupStack::Pop( checkboxOnBitmap ); 

    CleanupStack::PushL( checkboxOnIcon );
    iconArray->AppendL( checkboxOnIcon );

    // Create 'OFF' checkbox icon
    AknsUtils::CreateColorIconLC( AknsUtils::SkinInstance(),
    			KAknsIIDQgnIndiCheckboxOff,
    			KAknsIIDQsnIconColors,
    			EAknsCIQsnIconColorsCG13,
    			checkboxOffBitmap,
    			checkboxOffBitmapMask,
    			KAvkonBitmapFile,
    			EMbmAvkonQgn_indi_checkbox_off,
    			EMbmAvkonQgn_indi_checkbox_off_mask,
    			defaultColor
    			);

    CGulIcon* checkboxOffIcon = CGulIcon::NewL( checkboxOffBitmap, checkboxOffBitmapMask );
    // cleanup checkboxOffBitmap, checkboxOffBitmapMask
    CleanupStack::Pop( checkboxOffBitmapMask );
    CleanupStack::Pop( checkboxOffBitmap ); 
    

    CleanupStack::PushL( checkboxOffIcon );
    iconArray->AppendL( checkboxOffIcon );

    CAknColumnListBox* castedListbox = static_cast<CAknColumnListBox*>( iListBox );
    castedListbox->ItemDrawer()->ColumnData()->SetIconArray( iconArray );

    // cleanup checkboxOnIcon, checkboxOffIcon, iconArray
    CleanupStack::Pop( checkboxOffIcon );
    CleanupStack::Pop( checkboxOnIcon );
	
	if(iconArray)
		{
		CleanupStack::Pop( iconArray );
		}

    TRACE_( "[GSSensorPlugin] CGSSenBaseContainer::AddCheckboxIconsL() - return" );
    }

// ---------------------------------------------------------------------------
// CGSSenBaseContainer::OfferKeyEventL
// ---------------------------------------------------------------------------
//
TKeyResponse CGSSenBaseContainer::OfferKeyEventL( 
    const TKeyEvent& aKeyEvent, TEventCode aType )
    {
    TRACE_( "[GSSensorPlugin] CGSSenBaseContainer::OfferKeyEventL()" );

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

    TRACE_( "[GSSensorPlugin] CGSSenBaseContainer::OfferKeyEventL() - return" );
    return returnValue;
    }

// ---------------------------------------------------------------------------
// CGSSenBaseContainer::UpdateCheckboxIconL
// ---------------------------------------------------------------------------
//
void CGSSenBaseContainer::UpdateCheckboxIconL( TInt aSelectedItem )
    {
    TRACE_1( "[GSSensorPlugin] CGSSenBaseContainer::UpdateCheckboxIconL() - Item: %i", aSelectedItem );
    // Range of selected item is already checked by caller of this function
    if ( SelectionStatus( aSelectedItem ) )
        {
        iListBox->View()->SelectItemL( aSelectedItem );
        }
    else
        {
        iListBox->View()->DeselectItem( aSelectedItem );
        }
    TRACE_( "[GSSensorPlugin] CGSSenBaseContainer::UpdateCheckboxIconL() - return" );
    }

// ---------------------------------------------------------------------------
// CGSSenBaseContainer::StoreSelectionsL
// Store current selection list to CenRep
// ---------------------------------------------------------------------------
//
void CGSSenBaseContainer::StoreSelectionsL()
    {
    TRACE_( "[GSSensorPlugin] CGSSenBaseContainer::StoreSelectionsL()" );
    // Send current selection list to be stored by model
    iModel->SetSelectionListL( InteractionId(), iSelections );
    TRACE_( "[GSSensorPlugin] CGSSenBaseContainer::StoreSelectionsL() - return" );
    }

// ---------------------------------------------------------------------------
// CGSSenBaseContainer::CurrentFeatureId
// ---------------------------------------------------------------------------
//
TInt CGSSenBaseContainer::CurrentFeatureId( ) const
    {
    TRACE_1( "[GSSensorPlugin] CGSSenBaseContainer::CurrentFeatureId() - %i", 
        iListboxItemArray->CurrentFeature() );
    return iListboxItemArray->CurrentFeature();
    }

// ----------------------------------------------------------------------------
// CGSSenBaseContainer::SelectionStatus
// ----------------------------------------------------------------------------
//
TInt CGSSenBaseContainer::SelectionStatus( TInt aPosition )
    {
    TRACE_1( "[GSSensorPlugin] CGSSenBaseContainer::SelectionStatus() - Position: %i", aPosition );
    TUint status( 0 );
    TUint mask( 0x01 << aPosition );
    
    // Get value from member variable bitmask. It is updated when this view is launched
    // and will be stored when view is closed
    status = iSelections & mask;
    TRACE_1( "[GSSensorPlugin] CGSSenBaseContainer::SelectionStatus() - return: %i", status );
    return status;
    }

// ----------------------------------------------------------------------------
// CGSSenBaseContainer::ChangeSelectionStatus
// ----------------------------------------------------------------------------
//
void CGSSenBaseContainer::ChangeSelectionStatus( TInt aPosition)
    {
    TRACE_1( "[GSSensorPlugin] CGSSenBaseContainer::ChangeSelectionStatus() - Position: %i", 
        aPosition );
    // Set position bit to 1
    TUint bit( 0x01 << aPosition );
    // Get current status
    const TBool status( SelectionStatus( aPosition ) );

    // Set value only to member variable bitmask. Bitmask is updated to CenRep when all
    // selections are approved by user

    if ( !status )
        {
        // Set selected bit on
        iSelections = iSelections | bit;    
        }
    else
        {
        // Set selected bit off. In example, second and fourth are selected. Second needs to set off
        // First take bitwice from selections, for example         0000 1010 -> 1111 0101
        // Make OR operand to bitwise with selected bit (second)   1111 0101 -> 1111 0111
        // Make bitwise again to set selections to original form   1111 0111 -> 0000 1000
        TUint bitwise( ~iSelections );
        iSelections = ~( bitwise | bit );
        }
    TRACE_( "[GSSensorPlugin] CGSSenBaseContainer::ChangeSelectionStatus() - return" );
    }

// ----------------------------------------------------------------------------
// CGSSenBaseContainer::CheckSelections
// ----------------------------------------------------------------------------
//
void CGSSenBaseContainer::CheckSelections()
    {
    TRACE_( "[GSSensorPlugin] CGSSenBaseContainer::CheckSelections()");

    // This function is needed purely for this component to work in S60 3.1 as it does not support MSK 
    // as well as S60 3.2 and onwards. The reason for this function is that in S60 3.1 MSK presses does 
    // not end up for this component, but the checking / unchecking is done in AVKON side and hence 
    // this component does not know the changes other way than with this function

    TUint status( 0 );
    CArrayFix<TInt>* selections = const_cast<CArrayFix<TInt>*> ( iListBox->View()->SelectionIndexes() );
    
    if ( selections )
        {
        for ( TInt i = 0; i < selections->Count(); i++ )
            {
            // Set selected bit to 1
            TUint bit( 0x01 << selections->At( i ) );
            // Set selected bit on
            status = status | bit;
            }
        }
#ifdef _DEBUG
    TRACE_2( "[GSSensorPlugin] CGSSenBaseContainer::CheckSelections() - iSelections: %i listbox selections: %i", 
             iSelections, status );
#endif
    iSelections = status;
    TRACE_( "[GSSensorPlugin] CGSSenBaseContainer::CheckSelections() - return" );
    }

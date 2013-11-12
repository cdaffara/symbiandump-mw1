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
* Description:  Sensors activation status view container
*
*/


// INCLUDE FILES
#include "gssensensorscontainer.h" 
#include "gssensorpluginmodel.h"
#include "gssensorplugin.hrh"
#include "gssensorplugin_debug.h"

#include <aknlists.h>
#include <csxhelp/cp.hlp.hrh>
#include <GSFwViewUIDs.h>     // for KUidGS
#include <GSListBox.h>
#include <gssensorpluginrsc.rsg>


// CONSTANTS

// ========================= MEMBER FUNCTIONS ================================

// ---------------------------------------------------------------------------
// CGSSenSensorsContainer::CGSSenSensorsContainer
// Default constructor
// ---------------------------------------------------------------------------
//
CGSSenSensorsContainer::CGSSenSensorsContainer( CGSSensorPluginModel* aModel ) :
    iModel ( aModel )
    {
    TRACE_( "[GSSensorPlugin] CGSSenSensorsContainer::CGSSenSensorsContainer()" );
    }

// ---------------------------------------------------------------------------
// CGSSenSensorsContainer::ConstructL
// Symbian OS two phased constructor
// ---------------------------------------------------------------------------
//
void CGSSenSensorsContainer::ConstructL( const TRect& aRect )
    {
    TRACE_( "[GSSensorPlugin] CGSSenSensorsContainer::ConstructL()" );
    // Has to create listbox before calling BaseConstructL because base class
    // uses iListBox
    iListBox = new( ELeave ) CAknSettingStyleListBox;
    BaseConstructL( aRect, R_GS_SENSORS_VIEW_TITLE, R_GS_SENSORS_STATUS_LBX );
    TRACE_( "[GSSensorPlugin] CGSSenSensorsContainer::ConstructL() - return" );
    }

// ---------------------------------------------------------------------------
// CGSSenSensorsContainer::~CGSSenSensorsContainer()
// Destructor
// ---------------------------------------------------------------------------
//
CGSSenSensorsContainer::~CGSSenSensorsContainer()
    {
    TRACE_( "[GSSensorPlugin] CGSSenSensorsContainer::~CGSSenSensorsContainer()" );
    if ( iSensorStatusItems )
        {
        delete iSensorStatusItems;
        }
    if ( iListboxItemArray )
        {
        delete iListboxItemArray;
        }

    iModel = NULL;
    TRACE_( "[GSSensorPlugin] CGSSenSensorsContainer::~CGSSenSensorsContainer() - return" );
    }

// ---------------------------------------------------------------------------
// CGSSenSensorsContainer::ConstructListBoxL()
// ---------------------------------------------------------------------------
//
void CGSSenSensorsContainer::ConstructListBoxL( TInt aResLbxId )
    {
    TRACE_( "[GSSensorPlugin] CGSSenSensorsContainer::ConstructListBoxL()" );
    iListBox->ConstructL( this, EAknListBoxSelectionList );
    iListboxItemArray = CGSListBoxItemTextArray::NewL( 
        aResLbxId, *iListBox, *iCoeEnv );
    iListBox->Model()->SetItemTextArray( iListboxItemArray );
    iListBox->Model()->SetOwnershipType( ELbmDoesNotOwnItemArray );
    
    iSensorStatusItems = iCoeEnv->ReadDesC16ArrayResourceL(
                            R_GS_SENSOR_ACTIVE_STATUS );

    CreateListBoxItemsL();
    TRACE_( "[GSSensorPlugin] CGSSenSensorsContainer::ConstructListBoxL() - return" );
    }

// ---------------------------------------------------------------------------
// CGSSenSensorsContainer::CreateListBoxItemsL
// ---------------------------------------------------------------------------
//
void CGSSenSensorsContainer::CreateListBoxItemsL()
    {
    TRACE_( "[GSSensorPlugin] CGSSenSensorsContainer::CreateListBoxItemsL()" );
    MakeSensorsSelectionItemL();
    TRACE_( "[GSSensorPlugin] CGSSenSensorsContainer::CreateListBoxItemsL() - return" );
    }

// ---------------------------------------------------------------------------
// CGSSenSensorsContainer::UpdateListBoxL
// ---------------------------------------------------------------------------
//
void CGSSenSensorsContainer::UpdateListBoxL( TInt aFeatureId )
    {
    TRACE_( "[GSSensorPlugin] CGSSenSensorsContainer::UpdateListBoxL()" );
    switch( aFeatureId )
        {
        case KGSSenorsActive:
            MakeSensorsSelectionItemL();
            break;
        default:
            break;
        }
    iListBox->HandleItemAdditionL();
    TRACE_( "[GSSensorPlugin] CGSSenSensorsContainer::UpdateListBoxL() - return" );
    }

// ---------------------------------------------------------------------------
// CGSSenSensorsContainer::MakeSensorsSelectionItemL
// ---------------------------------------------------------------------------
//
void CGSSenSensorsContainer::MakeSensorsSelectionItemL()
    {
    TRACE_( "[GSSensorPlugin] CGSSenSensorsContainer::MakeSensorsSelectionItemL()" );
    HBufC* dynamicText = HBufC::NewLC( KGSBufSize128 );
    TPtr ptrBuffer ( dynamicText->Des() );

    // Get sensors status and set corresponding text
    const TInt active = iModel->SensorActiveStatusL();
    ptrBuffer = ( *iSensorStatusItems )[active];

    // Finally, set the dynamic text
    iListboxItemArray->SetDynamicTextL( KGSSenorsActive, ptrBuffer );

    CleanupStack::PopAndDestroy( dynamicText );

    // And add to listbox
    iListboxItemArray->SetItemVisibilityL( KGSSenorsActive, 
        CGSListBoxItemTextArray::EVisible );
    TRACE_( "[GSSensorPlugin] CGSSenSensorsContainer::MakeSensorsSelectionItemL() - return" );
    }

// ---------------------------------------------------------------------------
// CGSSenSensorsContainer::GetHelpContext
// Gets Help context
// ---------------------------------------------------------------------------
//
void CGSSenSensorsContainer::GetHelpContext( TCoeHelpContext& aContext ) const
    {
    TRACE_( "[GSSensorPlugin] CGSSenSensorsContainer::GetHelpContext()" );
    aContext.iMajor = KUidGS;
    aContext.iContext = KSSET_HLP_MAIN_VIEW;
    TRACE_( "[GSSensorPlugin] CGSSenSensorsContainer::GetHelpContext() - return" );
    }

// ---------------------------------------------------------------------------
// CGSSenSensorsContainer::CurrentFeatureId
// ---------------------------------------------------------------------------
//
TInt CGSSenSensorsContainer::CurrentFeatureId( ) const
    {
    TRACE_( "[GSSensorPlugin] CGSSenSensorsContainer::CurrentFeatureId()" );
    return iListboxItemArray->CurrentFeature( );
    }

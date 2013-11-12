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
* Description:  Sensors turn control view
*
*/


// INCLUDE FILES
#include "gssenturnctrlview.h"
#include "gssenturnctrlcontainer.h"
#include "gssensorplugin.hrh"
#include "gssensorpluginlocalviewids.h"
#include "gssenturnctrlcontainer.h"
#include "gssensorpluginmodel.h"
#include "gssensorplugin_debug.h"

#include <aknViewAppUi.h>
#include <gssensorpluginrsc.rsg>
#include <gscommon.hrh>
#include <eikbtgpc.h>


// ========================= MEMBER FUNCTIONS ================================

// ---------------------------------------------------------------------------
// CGSSenTurnCtrlView::CGSSenTurnCtrlView
// Default constructor
// ---------------------------------------------------------------------------
//
CGSSenTurnCtrlView::CGSSenTurnCtrlView( CGSSensorPluginModel* aModel ) :
    iModel( aModel )
    {
    TRACE_( "[GSSensorPlugin] CGSSenTurnCtrlView::CGSSenTurnCtrlView()" );
    }

// ---------------------------------------------------------------------------
// CGSSenTurnCtrlView::NewL
// Symbian OS two-phased constructor
// ---------------------------------------------------------------------------
CGSSenTurnCtrlView* CGSSenTurnCtrlView::NewL( CGSSensorPluginModel* aModel )
    {
    TRACE_( "[GSSensorPlugin] CGSSenTurnCtrlView::NewL()" );
    CGSSenTurnCtrlView* self = NewLC( aModel );
    CleanupStack::Pop( self );
    TRACE_( "[GSSensorPlugin] CGSSenTurnCtrlView::NewL() - return" );
    return self;
    }

// ---------------------------------------------------------------------------
// CGSSenTurnCtrlView::NewLC
// Symbian OS two-phased constructor
// ---------------------------------------------------------------------------
CGSSenTurnCtrlView* CGSSenTurnCtrlView::NewLC( CGSSensorPluginModel* aModel )
    {
    TRACE_( "[GSSensorPlugin] CGSSenTurnCtrlView::NewLC()" );
    CGSSenTurnCtrlView* self = new( ELeave ) CGSSenTurnCtrlView( aModel );
    CleanupStack::PushL( self );
    self->ConstructL();
    TRACE_( "[GSSensorPlugin] CGSSenTurnCtrlView::NewLC() - return" );
    return self;
    }

// ---------------------------------------------------------------------------
// CGSSenTurnCtrlView::~CGSSenTurnCtrlView()
// Destructor
// ---------------------------------------------------------------------------
CGSSenTurnCtrlView::~CGSSenTurnCtrlView()
    {
    TRACE_( "[GSSensorPlugin] CGSSenTurnCtrlView::~CGSSenTurnCtrlView()" );
    iModel = NULL;
    }

// ---------------------------------------------------------------------------
// CGSSenTurnCtrlView::ConstructL()
// Symbian OS two-phased constructor
// ---------------------------------------------------------------------------
void CGSSenTurnCtrlView::ConstructL()
    {
    TRACE_( "[GSSensorPlugin] CGSSenTurnCtrlView::ConstructL()" );
    BaseConstructL( R_GS_CHECKBOX_VIEW );
    TRACE_( "[GSSensorPlugin] CGSSenTurnCtrlView::ConstructL() - return" );
    }

// ---------------------------------------------------------------------------
// TUid CGSSenTurnCtrlView::Id()
// ---------------------------------------------------------------------------
TUid CGSSenTurnCtrlView::Id() const
    {
    TRACE_1( "[GSSensorPlugin] CGSSenTurnCtrlView::Id() - ID: %i", KSenTurnCtrlViewId );
    return KSenTurnCtrlViewId;
    }

// ---------------------------------------------------------------------------
// CGSSenTurnCtrlView::HandleCommandL()
// Command handling for sensor activation status view
// ---------------------------------------------------------------------------
void CGSSenTurnCtrlView::HandleCommandL( TInt aCommand )
    {   
    TRACE_1( "[GSSensorPlugin] CGSSenTurnCtrlView::HandleCommandL() - command: %i",
        aCommand );

    switch ( aCommand )
        {
        case EGSSenMskGeneral:
            {
            // Update checkbox and selections
            HandleListBoxSelectionL();
            // Update MSK label
            CheckMiddleSoftkeyLabelL();
            break;
            }
        case EGSCmdOk:
            // Check selections
            Container()->CheckSelections();
            // Store changes and show previous view
            Container()->StoreSelectionsL();
            // cont. to next case
        case EAknSoftkeyBack:
            {
            // activate previous view
            iAppUi->ActivateLocalViewL( KGSSensorPluginUid );
            break;
            }
        default:
            iAppUi->HandleCommandL( aCommand );
            break;
        }
    TRACE_( "[GSSensorPlugin] CGSSenTurnCtrlView::HandleCommandL() - return" );
    }

// ---------------------------------------------------------------------------
// CGSSenTurnCtrlView::Container
// Returns call container item
// ---------------------------------------------------------------------------
//
CGSSenTurnCtrlContainer* CGSSenTurnCtrlView::Container()
    {
    TRACE_( "[GSSensorPlugin] CGSSenTurnCtrlView::Container()" );
    return static_cast<CGSSenTurnCtrlContainer*>( iContainer );
    }

// ---------------------------------------------------------------------------
// CGSSenTurnCtrlView::NewContainerL()
// Creates new iContainer.
// ---------------------------------------------------------------------------
//
void CGSSenTurnCtrlView::NewContainerL()
    {
    TRACE_( "[GSSensorPlugin] CGSSenTurnCtrlView::NewContainerL()" );
    iContainer = new ( ELeave ) CGSSenTurnCtrlContainer( iModel, *this );
    TRACE_( "[GSSensorPlugin] CGSSenTurnCtrlView::NewContainerL() - return" );
    }

// ---------------------------------------------------------------------------
// CGSSenTurnCtrlView::HandleListBoxSelectionL()
// ---------------------------------------------------------------------------
void CGSSenTurnCtrlView::HandleListBoxSelectionL() 
    {
    TRACE_( "[GSSensorPlugin] CGSSenTurnCtrlView::HandleListBoxSelectionL()" );
    const TInt currentItem = Container()->CurrentFeatureId();
    Container()->ChangeSelectionStatus( currentItem );
    Container()->UpdateCheckboxIconL( currentItem );
    TRACE_( "[GSSensorPlugin] CGSSenTurnCtrlView::HandleListBoxSelectionL() - return" );
    }

// ---------------------------------------------------------------------------
// CGSSenTurnCtrlView::CheckMiddleSoftkeyLabelL
// ---------------------------------------------------------------------------
//
void CGSSenTurnCtrlView::CheckMiddleSoftkeyLabelL() 
    {
    TRACE_( "[GSSensorPlugin] CGSSenTurnCtrlView::CheckMiddleSoftkeyLabelL()" );
    // First, resolve current item
    const TInt currentItem = Container()->CurrentFeatureId();

    // First, remove current label
    RemoveCommandFromMSK();

    // Resolve is currently selected item checked or unchecked and change MSK label accordingly
    if ( Container()->SelectionStatus( currentItem ) )
        {
        // Change MKS to 'Unmark'
        SetMiddleSoftKeyLabelL( R_GS_MKS_LABEL_UNMARK, EGSSenMskGeneral );
        }
    else
        {
        // Change MSK to 'Mark'
        SetMiddleSoftKeyLabelL( R_GS_MKS_LABEL_MARK, EGSSenMskGeneral );
        }  

    TRACE_( "[GSSensorPlugin] CGSSenTurnCtrlView::CheckMiddleSoftkeyLabelL() - return" );
    }

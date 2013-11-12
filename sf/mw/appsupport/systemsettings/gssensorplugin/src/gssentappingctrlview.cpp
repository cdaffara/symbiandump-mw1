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
* Description:  Sensors tapping control view
*
*/


// INCLUDE FILES
#include "gssentappingctrlview.h"
#include "gssentappingctrlcontainer.h"
#include "gssensorplugin.hrh"
#include "gssensorpluginlocalviewids.h"
#include "gssensorpluginmodel.h"
#include "gssensorplugin_debug.h"

#include <aknViewAppUi.h>
#include <gssensorpluginrsc.rsg>
#include <gscommon.hrh>


// ========================= MEMBER FUNCTIONS ================================

// ---------------------------------------------------------------------------
// CGSSenTappingCtrlView::CGSSenTappingCtrlView
// Default constructor
// ---------------------------------------------------------------------------
//
CGSSenTappingCtrlView::CGSSenTappingCtrlView( CGSSensorPluginModel* aModel ) :
    iModel( aModel )
    {
    TRACE_( "[GSSensorPlugin] CGSSenTappingCtrlView::CGSSenTappingCtrlView()" );
    }

// ---------------------------------------------------------------------------
// CGSSenTappingCtrlView::NewL
// Symbian OS two-phased constructor
// ---------------------------------------------------------------------------
CGSSenTappingCtrlView* CGSSenTappingCtrlView::NewL( CGSSensorPluginModel* aModel )
    {
    TRACE_( "[GSSensorPlugin] CGSSenTappingCtrlView::NewL()" );
    CGSSenTappingCtrlView* self = NewLC( aModel );
    CleanupStack::Pop( self );
    TRACE_( "[GSSensorPlugin] CGSSenTappingCtrlView::NewL() - return" );
    return self;
    }

// ---------------------------------------------------------------------------
// CGSSenTappingCtrlView::NewLC
// Symbian OS two-phased constructor
// ---------------------------------------------------------------------------
CGSSenTappingCtrlView* CGSSenTappingCtrlView::NewLC( CGSSensorPluginModel* aModel )
    {
    TRACE_( "[GSSensorPlugin] CGSSenTappingCtrlView::NewLC()" );
    CGSSenTappingCtrlView* self = new( ELeave ) CGSSenTappingCtrlView( aModel );
    CleanupStack::PushL( self );
    self->ConstructL();
    TRACE_( "[GSSensorPlugin] CGSSenTappingCtrlView::NewLC() - return" );
    return self;
    }

// ---------------------------------------------------------------------------
// CGSSenTappingCtrlView::~CGSSenTappingCtrlView()
// Destructor
// ---------------------------------------------------------------------------
CGSSenTappingCtrlView::~CGSSenTappingCtrlView()
    {
    TRACE_( "[GSSensorPlugin] CGSSenTappingCtrlView::~CGSSenTappingCtrlView()" );
    iModel = NULL;
    }

// ---------------------------------------------------------------------------
// CGSSenTappingCtrlView::ConstructL()
// Symbian OS two-phased constructor
// ---------------------------------------------------------------------------
void CGSSenTappingCtrlView::ConstructL()
    {
    TRACE_( "[GSSensorPlugin] CGSSenTappingCtrlView::ConstructL()" );
    BaseConstructL( R_GS_CHECKBOX_VIEW );
    TRACE_( "[GSSensorPlugin] CGSSenTappingCtrlView::ConstructL() - return" );
    }

// ---------------------------------------------------------------------------
// TUid CGSSenTappingCtrlView::Id()
// ---------------------------------------------------------------------------
TUid CGSSenTappingCtrlView::Id() const
    {
    TRACE_1( "[GSSensorPlugin] CGSSenTappingCtrlView::Id() - ID: %i", KSenTapCtrlViewId );
    return KSenTapCtrlViewId;
    }

// ---------------------------------------------------------------------------
// CGSSenTappingCtrlView::HandleCommandL()
// Command handling for sensor activation status view
// ---------------------------------------------------------------------------
void CGSSenTappingCtrlView::HandleCommandL( TInt aCommand )
    {
    TRACE_1( "[GSSensorPlugin] CGSSenTappingCtrlView::HandleCommandL() - command: %i",
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
    TRACE_( "[GSSensorPlugin] CGSSenTappingCtrlView::HandleCommandL() - return" );
    }

// ---------------------------------------------------------------------------
// CGSSenTappingCtrlView::Container
// Returns call container item
// ---------------------------------------------------------------------------
//
CGSSenTappingCtrlContainer* CGSSenTappingCtrlView::Container()
    {
    TRACE_( "[GSSensorPlugin] CGSSenTappingCtrlView::Container()" );
    return static_cast<CGSSenTappingCtrlContainer*>( iContainer );
    }

// ---------------------------------------------------------------------------
// CGSSenTappingCtrlView::NewContainerL
// Creates new iContainer.
// ---------------------------------------------------------------------------
//
void CGSSenTappingCtrlView::NewContainerL()
    {
    TRACE_( "[GSSensorPlugin] CGSSenTappingCtrlView::NewContainerL()" );
    iContainer = new ( ELeave ) CGSSenTappingCtrlContainer( iModel, *this );
    TRACE_( "[GSSensorPlugin] CGSSenTappingCtrlView::NewContainerL() - return" );
    }

// ---------------------------------------------------------------------------
// CGSSenTappingCtrlView::HandleListBoxSelectionL
// ---------------------------------------------------------------------------
void CGSSenTappingCtrlView::HandleListBoxSelectionL() 
    {
    TRACE_( "[GSSensorPlugin] CGSSenTappingCtrlView::HandleListBoxSelectionL()" );
    const TInt currentItem = Container()->CurrentFeatureId();
    Container()->ChangeSelectionStatus( currentItem );
    Container()->UpdateCheckboxIconL( currentItem );
    TRACE_( "[GSSensorPlugin] CGSSenTappingCtrlView::HandleListBoxSelectionL() - return" );
    }

// ---------------------------------------------------------------------------
// CGSSenTappingCtrlView::CheckMiddleSoftkeyLabelL
// ---------------------------------------------------------------------------
//
void CGSSenTappingCtrlView::CheckMiddleSoftkeyLabelL() 
    {
    TRACE_( "[GSSensorPlugin] CGSSenTappingCtrlView::CheckMiddleSoftkeyLabelL()" );
    // First, resolve current item
    const TInt currentItem = Container()->CurrentFeatureId();

    // Remove current label
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

    TRACE_( "[GSSensorPlugin] CGSSenTappingCtrlView::CheckMiddleSoftkeyLabelL() - return" );
    }

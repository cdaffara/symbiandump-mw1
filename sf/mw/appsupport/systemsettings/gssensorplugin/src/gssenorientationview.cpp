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
* Description:  Sensors orientation view
*
*/


// INCLUDE FILES
#include "gssenorientationview.h"
#include "gssenorientationcontainer.h"
#include "gssensorplugin.hrh"
#include "gssensorpluginlocalviewids.h"
#include "gssenorientationcontainer.h"
#include "gssensorpluginmodel.h"
#include "gssensorplugin_debug.h"

#include <AknViewAppUi.h>
#include <gssensorpluginrsc.rsg>
#include <GSCommon.hrh>


// ========================= MEMBER FUNCTIONS ================================

// ---------------------------------------------------------------------------
// CGSSenOrientationView::CGSSenOrientationView
// Default constructor
// ---------------------------------------------------------------------------
//
CGSSenOrientationView::CGSSenOrientationView( CGSSensorPluginModel* aModel ) :
    iModel( aModel )
    {
    TRACE_( "[GSSensorPlugin] CGSSenOrientationView::CGSSenOrientationView()" );
    }

// ---------------------------------------------------------------------------
// CGSSenOrientationView::NewL
// Symbian OS two-phased constructor
// ---------------------------------------------------------------------------
CGSSenOrientationView* CGSSenOrientationView::NewL( CGSSensorPluginModel* aModel )
    {
    TRACE_( "[GSSensorPlugin] CGSSenOrientationView::NewL()" );
    CGSSenOrientationView* self = NewLC( aModel );
    CleanupStack::Pop( self );
    TRACE_( "[GSSensorPlugin] CGSSenOrientationView::NewL() - return" );
    return self;
    }

// ---------------------------------------------------------------------------
// CGSSenOrientationView::NewLC
// Symbian OS two-phased constructor
// ---------------------------------------------------------------------------
CGSSenOrientationView* CGSSenOrientationView::NewLC( CGSSensorPluginModel* aModel )
    {
    TRACE_( "[GSSensorPlugin] CGSSenOrientationView::NewLC()" );
    CGSSenOrientationView* self = new( ELeave ) CGSSenOrientationView( aModel );
    CleanupStack::PushL( self );
    self->ConstructL();
    TRACE_( "[GSSensorPlugin] CGSSenOrientationView::NewLC() - return" );
    return self;
    }

// ---------------------------------------------------------------------------
// CGSSenOrientationView::~CGSSenOrientationView()
// Destructor
// ---------------------------------------------------------------------------
CGSSenOrientationView::~CGSSenOrientationView()
    {
    TRACE_( "[GSSensorPlugin] CGSSenOrientationView::~CGSSenOrientationView()" );
    iModel = NULL;
    }

// ---------------------------------------------------------------------------
// CGSSenOrientationView::ConstructL()
// Symbian OS two-phased constructor
// ---------------------------------------------------------------------------
void CGSSenOrientationView::ConstructL()
    {
    TRACE_( "[GSSensorPlugin] CGSSenOrientationView::ConstructL()" );
    BaseConstructL( R_GS_CHECKBOX_VIEW );
    TRACE_( "[GSSensorPlugin] CGSSenOrientationView::ConstructL() - return" );
    }

// ---------------------------------------------------------------------------
// TUid CGSSenOrientationView::Id()
// ---------------------------------------------------------------------------
TUid CGSSenOrientationView::Id() const
    {
    TRACE_1( "[GSSensorPlugin] CGSSenOrientationView::Id() - ID: %i", KSenOrientationViewId );
    return KSenOrientationViewId;
    }

// ---------------------------------------------------------------------------
// CGSSenOrientationView::HandleCommandL()
// Command handling for sensor activation status view
// ---------------------------------------------------------------------------
void CGSSenOrientationView::HandleCommandL( TInt aCommand )
    {   
    TRACE_1( "[GSSensorPlugin] CGSSenOrientationView::HandleCommandL() - command: %i",
        aCommand );

    switch ( aCommand )
        {
        case EGSMSKCmdAppChange:
            {
            // Update checkbox and selections
            HandleListBoxSelectionL();
            break;
            }
        case EGSCmdOk:
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
    TRACE_( "[GSSensorPlugin] CGSSenOrientationView::HandleCommandL() - return" );
    }

// ---------------------------------------------------------------------------
// CGSSenOrientationView::Container
// Returns call container item
// ---------------------------------------------------------------------------
//
CGSSenOrientationContainer* CGSSenOrientationView::Container()
    {
    TRACE_( "[GSSensorPlugin] CGSSenOrientationView::Container()" );
    return static_cast<CGSSenOrientationContainer*>( iContainer );
    }

// ---------------------------------------------------------------------------
// CGSSenOrientationView::NewContainerL()
// Creates new iContainer.
// ---------------------------------------------------------------------------
//
void CGSSenOrientationView::NewContainerL()
    {
    TRACE_( "[GSSensorPlugin] CGSSenOrientationView::NewContainerL()" );
    iContainer = new ( ELeave ) CGSSenOrientationContainer( iModel );
    TRACE_( "[GSSensorPlugin] CGSSenOrientationView::NewContainerL() - return" );
    }

// ---------------------------------------------------------------------------
// CGSSenOrientationView::HandleListBoxSelectionL()
// ---------------------------------------------------------------------------
void CGSSenOrientationView::HandleListBoxSelectionL() 
    {
    TRACE_( "[GSSensorPlugin] CGSSenOrientationView::HandleListBoxSelectionL()" );
    const TInt currentItem = Container()->CurrentFeatureId();
    Container()->ChangeSelectionStatus( currentItem );
    Container()->UpdateCheckboxIconL( currentItem );
    TRACE_( "[GSSensorPlugin] CGSSenOrientationView::HandleListBoxSelectionL() - return" );
    }

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
* Description:  Base view in GS Sensor Plugin.
*
*/


// INCLUDE FILES
#include "gssensorbaseview.h"
#include "gssensorplugin_debug.h"
#include "gssensormskobserver.h"
#include "gssensorplugin.hrh"

#include <aknViewAppUi.h>
#include <gsbasecontainer.h>
#include <StringLoader.h>
#include <eikbtgpc.h>

// CONSTANTS
_LIT( KGSDoActivateError, "DoActivateL" );

// ========================= MEMBER FUNCTIONS ================================

// ---------------------------------------------------------------------------
// CGSSensorBaseView::CGSSensorBaseView
// C++ constructor
// ---------------------------------------------------------------------------
//
CGSSensorBaseView::CGSSensorBaseView()
    {
    TRACE_( "[GSSensorPlugin] CGSSensorBaseView::CGSSensorBaseView()" );
    iAppUi = iAvkonViewAppUi;
    TRACE_( "[GSSensorPlugin] CGSSensorBaseView::CGSSensorBaseView() - return" );
    }


// ---------------------------------------------------------------------------
// CGSSensorBaseView::~CGSSensorBaseView
// Destructor
// ---------------------------------------------------------------------------
//
CGSSensorBaseView::~CGSSensorBaseView()
    {
    TRACE_( "[GSSensorPlugin] CGSSensorBaseView::~CGSSensorBaseView()" );
    if ( iContainer && iAppUi )
        {
        iAppUi->RemoveFromViewStack( *this, iContainer );
        delete iContainer;
        }
    TRACE_( "[GSSensorPlugin] CGSSensorBaseView::~CGSSensorBaseView() - return" );
    }

// ---------------------------------------------------------------------------
// CGSSensorBaseView::SetCurrentItem
// ---------------------------------------------------------------------------
//
void CGSSensorBaseView::SetCurrentItem( TInt aIndex )
    {
    TRACE_1( "[GSSensorPlugin] CGSSensorBaseView::SetCurrentItem() - %i", aIndex );
    iCurrentItem = aIndex;
    TRACE_( "[GSSensorPlugin] CGSSensorBaseView::SetCurrentItem() - return" );
    }

// ---------------------------------------------------------------------------
// CGSSensorBaseView::HandleListBoxEventL
// ---------------------------------------------------------------------------
//
void CGSSensorBaseView::HandleListBoxEventL( 
    CEikListBox* /*aListBox*/, 
    TListBoxEvent aEventType )
    {
    TRACE_( "[GSSensorPlugin] CGSSensorBaseView::HandleListBoxEventL()" );
    switch ( aEventType )
        {
        case EEventEnterKeyPressed:
        case EEventItemSingleClicked:
            HandleListBoxSelectionL();
            break;
        default:
           break;
        }
    TRACE_( "[GSSensorPlugin] CGSSensorBaseView::HandleListBoxEventL() - return" );
    }

// ---------------------------------------------------------------------------
// CGSSensorBaseView::HandleClientRectChange
// ---------------------------------------------------------------------------
//
void CGSSensorBaseView::HandleClientRectChange()
    {
    TRACE_( "[GSSensorPlugin] CGSSensorBaseView::HandleClientRectChange()" );
    if ( iContainer && iContainer->iListBox )
        {
        iContainer->SetRect( ClientRect() );
        }
    TRACE_( "[GSSensorPlugin] CGSSensorBaseView::HandleClientRectChange() - return" );
    }

// ---------------------------------------------------------------------------
// CGSSensorBaseView::DoActivateL
// Activates the view.
// ---------------------------------------------------------------------------
//
void CGSSensorBaseView::DoActivateL( 
    const TVwsViewId& /*aPrevViewId*/,
    TUid /*aCustomMessageId*/, 
    const TDesC8& /*aCustomMessage*/ )
    {
    TRACE_( "[GSSensorPlugin] CGSSensorBaseView::DoActivateL()" );
    if ( iContainer )
        {
        iAppUi->RemoveFromViewStack( *this, iContainer );
        delete iContainer;
        iContainer = NULL;
        }

    CreateContainerL();

    iAppUi->AddToViewStackL( *this, iContainer );
    iContainer->iListBox->SetListBoxObserver( this );
    // Update MSK label
    CheckMiddleSoftkeyLabelL();

    TRACE_( "[GSSensorPlugin] CGSSensorBaseView::DoActivateL() - return" );
    }

// ---------------------------------------------------------------------------
// CGSSensorBaseView::DoDeactivate()
// ---------------------------------------------------------------------------
//
void CGSSensorBaseView::DoDeactivate()
    {
    TRACE_( "[GSSensorPlugin] CGSSensorBaseView::DoDeactivate()" );
    if ( iContainer )
        {
        iAppUi->RemoveFromViewStack( *this, iContainer );
        delete iContainer;
        iContainer = NULL;
        }
    TRACE_( "[GSSensorPlugin] CGSSensorBaseView::DoDeactivate() - return" );
    }

// ---------------------------------------------------------------------------
// CGSSensorBaseView::CreateContainerL()
// ---------------------------------------------------------------------------
//
void CGSSensorBaseView::CreateContainerL()
    {
    TRACE_( "[GSSensorPlugin] CGSSensorBaseView::CreateContainerL()" );
    NewContainerL();
    __ASSERT_ALWAYS( iContainer, User::Panic( KGSDoActivateError, EGSViewPanicNullPtr ) );
    iContainer->SetMopParent( this );

    TRAPD( error, iContainer->ConstructL( ClientRect() ) );

    if ( error )
        {
        TRACE_1( "[GSSensorPlugin] CGSSensorBaseView::CreateContainerL() - Error: %i", error );
        delete iContainer;
        iContainer = NULL;
        User::Leave( error );
        }
    TRACE_( "[GSSensorPlugin] CGSSensorBaseView::CreateContainerL() - return" );
    }

// -----------------------------------------------------------------------------
// CGSSensorBaseView::RemoveCommandFromMSK
// This method should do nothing but MSK issues.
// -----------------------------------------------------------------------------
//
void CGSSensorBaseView::RemoveCommandFromMSK()
    {
    TRACE_( "[GSSensorPlugin] CGSSensorBaseView::RemoveCommandFromMSK()" );
    CEikButtonGroupContainer* cbaGroup = Cba();
    if ( cbaGroup )
        {
        // Remove command from stack
        cbaGroup->RemoveCommandFromStack( KGSSenPluginMSKControlID, EGSSenMskGeneral );
        }
    TRACE_( "[GSSensorPlugin] CGSSensorBaseView::RemoveCommandFromMSK() - return" );
    }

// -----------------------------------------------------------------------------
// CGSSensorBaseView::SetMiddleSoftKeyLabelL
// Sets middle softkey label.
// -----------------------------------------------------------------------------
//
void CGSSensorBaseView::SetMiddleSoftKeyLabelL( 
    const TInt aResourceId, const TInt aCommandId )
    {
    TRACE_( "[GSSensorPlugin] CGSSensorBaseView::SetMiddleSoftKeyLabelL()" );
    CEikButtonGroupContainer* cbaGroup = Cba();
    if ( cbaGroup )
        {
        HBufC* middleSKText = StringLoader::LoadLC( aResourceId );
        TPtr mskPtr = middleSKText->Des();
        cbaGroup->AddCommandToStackL( 
            KGSSenPluginMSKControlID, 
            aCommandId, 
            mskPtr );
        CleanupStack::Pop( middleSKText );
        delete middleSKText;
        }
    TRACE_( "[GSSensorPlugin] CGSSensorBaseView::SetMiddleSoftKeyLabelL() - return" );
    }

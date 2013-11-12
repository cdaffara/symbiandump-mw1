/*
* Copyright (c) 2005-2008 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Declaration of CGSAccTextPhoneView class
*
*/


#include <e32cmn.h> // For accessoriescrkeys.h
#include <accessoriescrkeys.h>
#include <gsaccessoryplugin.rsg>
#include <gscommon.hrh>

#include "acclocalviewids.h"
#include "gsaccessoryplugin.hrh"
#include "gsacctextphonecontainer.h"
#include "gsacctextphoneview.h"
#include "trace.h"

// ========================= MEMBER FUNCTIONS ================================

// ---------------------------------------------------------------------------
// CGSAccTextPhoneView::NewLC()
// ---------------------------------------------------------------------------
//
CGSAccTextPhoneView* CGSAccTextPhoneView::NewLC(
    CGSAccessoryPluginModel& aModel )
    {
    CGSAccTextPhoneView* self = new ( ELeave ) CGSAccTextPhoneView( aModel );
    CleanupStack::PushL( self );
    self->ConstructL();
    return self;
    }


// ---------------------------------------------------------------------------
// CGSAccTextPhoneView::~CGSAccTextPhoneView()
// ---------------------------------------------------------------------------
//
CGSAccTextPhoneView::~CGSAccTextPhoneView()
    {
    FUNC_LOG;
    }


// ---------------------------------------------------------------------------
// TUid CGSAccTextPhoneView::Id()
// ---------------------------------------------------------------------------
//
TUid CGSAccTextPhoneView::Id() const
    {
    return KAccTextPhoneViewId;
    }


// ---------------------------------------------------------------------------
// CGSAccTextPhoneView::HandleCommandL()
// ---------------------------------------------------------------------------
//
void CGSAccTextPhoneView::HandleCommandL( TInt aCommand )
    {
    FUNC_LOG;

    switch ( aCommand )
        {
        case EGSCmdAppChange:
            {
            // Act as user had pressed the selection key
            HandleListBoxSelectionL( iContainer->CurrentFeatureId() );
            break;
            }
        default:
            CGSAccBaseView::HandleCommandL( aCommand );
            break;
        }
    }


// ---------------------------------------------------------------------------
// CGSAccTextPhoneView::NewContainerL()
// ---------------------------------------------------------------------------
//
void CGSAccTextPhoneView::NewContainerL()
    {
    FUNC_LOG;

    // Base class owns the container.
    iContainer = new ( ELeave ) CGSAccTextPhoneContainer( iModel );
    }


// ---------------------------------------------------------------------------
// CGSAccTextPhoneView::HandleListBoxSelectionL()
// ---------------------------------------------------------------------------
//
void CGSAccTextPhoneView::HandleListBoxSelectionL( TInt aSelectedItem )
    {
    FUNC_LOG;

    // User has pressed selection key. If the selected item has just two
    // possible values, flip the value to the other option. If the item has
    // more possible values, show a selection dialog.
    if ( aSelectedItem == KGSSettIdTextPhoneDP )
        {
        ChangeDefaultProfileL(
            KSettingsTTYDefaultProfile, KGSSettIdTextPhoneDP );
        }
    }


// ---------------------------------------------------------------------------
// CGSAccTextPhoneView::CGSAccTextPhoneView()
// ---------------------------------------------------------------------------
//
CGSAccTextPhoneView::CGSAccTextPhoneView( CGSAccessoryPluginModel& aModel )
  : CGSAccBaseView( aModel )
    {
    FUNC_LOG;
    }


// ---------------------------------------------------------------------------
// CGSAccTextPhoneView::ConstructL()
// ---------------------------------------------------------------------------
//
void CGSAccTextPhoneView::ConstructL()
    {
    FUNC_LOG;

    BaseConstructL( R_ACC_TTY_VIEW );
    }

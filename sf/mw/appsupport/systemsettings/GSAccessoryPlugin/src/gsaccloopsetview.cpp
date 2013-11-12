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
* Description:  Implementation of CGSAccLoopsetView class
*
*/


#include <e32cmn.h> // For accessoriescrkeys.h
#include <accessoriescrkeys.h>
#include <gsaccessoryplugin.rsg>
#include <gscommon.hrh>

#include "acclocalviewids.h"
#include "gsaccessoryplugin.hrh"
#include "gsaccloopsetcontainer.h"
#include "gsaccloopsetview.h"
#include "trace.h"

// ========================= MEMBER FUNCTIONS ================================

// ---------------------------------------------------------------------------
// CGSAccLoopsetView::NewLC()
// ---------------------------------------------------------------------------
//
CGSAccLoopsetView* CGSAccLoopsetView::NewLC( CGSAccessoryPluginModel& aModel )
    {
    CGSAccLoopsetView* self = new ( ELeave ) CGSAccLoopsetView( aModel );
    CleanupStack::PushL( self );
    self->BaseConstructL( R_ACC_LOOPSET_VIEW );
    return self;
    }


// ---------------------------------------------------------------------------
// CGSAccLoopsetView::~CGSAccLoopsetView()
// ---------------------------------------------------------------------------
//
CGSAccLoopsetView::~CGSAccLoopsetView()
    {
    FUNC_LOG;
    }


// ---------------------------------------------------------------------------
// TUid CGSAccLoopsetView::Id()
// ---------------------------------------------------------------------------
//
TUid CGSAccLoopsetView::Id() const
    {
    return KAccLoopsetViewId;
    }

// ---------------------------------------------------------------------------
// CGSAccLoopsetView::HandleCommandL()
// ---------------------------------------------------------------------------
//
void CGSAccLoopsetView::HandleCommandL( TInt aCommand )
    {
    FUNC_LOG;

    if ( aCommand == EGSCmdAppChange )
        {
        const TInt currentFeatureId = iContainer->CurrentFeatureId();
        if ( currentFeatureId == KGSSettIdLSAA )
            {
            // Change automatic answer mode setting through selection dialog
            ChangeAutoAnswerModeSettingL(
                KSettingsLoopsetAutomaticAnswer,
                KGSSettIdLSAA,
                ETrue );
            }
        else
            {
            // Act as user had pressed the selection key
            HandleListBoxSelectionL( currentFeatureId );
            }
        }
    else
        {
        CGSAccBaseView::HandleCommandL( aCommand );
        }
    }


// ---------------------------------------------------------------------------
// CGSAccLoopsetView::NewContainerL()
// ---------------------------------------------------------------------------
//
void CGSAccLoopsetView::NewContainerL()
    {
    iContainer = new ( ELeave ) CGSAccLoopsetContainer( iModel );
    }


// ---------------------------------------------------------------------------
// CGSAccLoopsetView::HandleListBoxSelectionL()
// ---------------------------------------------------------------------------
//
void CGSAccLoopsetView::HandleListBoxSelectionL( TInt aSelectedItem )
    {
    FUNC_LOG;

    // User has pressed selection key. If the selected item has just two
    // possible values, flip the value to the other option. If the item has
    // more possible values, show a selection dialog.
    switch ( aSelectedItem )
        {
        case KGSSettIdLSDP:
            // Show selection dialog for the accessory default profile
            ChangeDefaultProfileL(
                KSettingsLoopsetDefaultProfile, KGSSettIdLSDP );
            break;
        case KGSSettIdLSAA:
            // Change automatic answer mode setting without selection dialog
            ChangeAutoAnswerModeSettingL(
                KSettingsLoopsetAutomaticAnswer,
                KGSSettIdLSAA,
                EFalse );
            break;
       default:
            break;
        }
    }


// ---------------------------------------------------------------------------
// CGSAccLoopsetView::CGSAccLoopsetView()
// ---------------------------------------------------------------------------
//
CGSAccLoopsetView::CGSAccLoopsetView( CGSAccessoryPluginModel& aModel )
  : CGSAccBaseView( aModel )
    {
    FUNC_LOG;
    }

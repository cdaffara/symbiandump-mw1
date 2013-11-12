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
* Description:  Implementation of CGSAccHeadsetView class
*
*/


#include <e32cmn.h> // For accessoriescrkeys.h
#include <accessoriescrkeys.h>
#include <gsaccessoryplugin.rsg>
#include <gscommon.hrh>

#include "acclocalviewids.h"
#include "gsaccessoryplugin.hrh"
#include "gsaccheadsetview.h"
#include "gsaccheadsetcontainer.h"
#include "trace.h"

// ========================= MEMBER FUNCTIONS ================================

// ---------------------------------------------------------------------------
// CGSAccHeadsetView::NewLC()
// ---------------------------------------------------------------------------
//
CGSAccHeadsetView* CGSAccHeadsetView::NewLC( CGSAccessoryPluginModel& aModel )
    {
    CGSAccHeadsetView* self = new ( ELeave ) CGSAccHeadsetView( aModel );
    CleanupStack::PushL( self );
    self->BaseConstructL( R_ACC_HEADSET_VIEW );
    return self;
    }


// ---------------------------------------------------------------------------
// CGSAccHeadsetView::~CGSAccHeadsetView()
// ---------------------------------------------------------------------------
//
CGSAccHeadsetView::~CGSAccHeadsetView()
    {
    FUNC_LOG;
    }


// ---------------------------------------------------------------------------
// TUid CGSAccHeadsetView::Id()
// ---------------------------------------------------------------------------
//
TUid CGSAccHeadsetView::Id() const
    {
    return KAccHeadsetViewId;
    }


// ---------------------------------------------------------------------------
// CGSAccHeadsetView::HandleCommandL()
// ---------------------------------------------------------------------------
//
void CGSAccHeadsetView::HandleCommandL( TInt aCommand )
    {
    FUNC_LOG;

    if ( aCommand == EGSCmdAppChange )
        {
        const TInt currentFeatureId = iContainer->CurrentFeatureId();
        if ( currentFeatureId == KGSSettIdHSAA )
            {
            // Change automatic answer mode setting through selection dialog
            ChangeAutoAnswerModeSettingL(
                KSettingsHeadsetAutomaticAnswer,
                KGSSettIdHSAA,
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
// CGSAccHeadsetView::NewContainerL()
// ---------------------------------------------------------------------------
//
void CGSAccHeadsetView::NewContainerL()
    {
    iContainer = new ( ELeave ) CGSAccHeadsetContainer ( iModel );
    }


// ---------------------------------------------------------------------------
// CGSAccHeadsetView::HandleListBoxSelectionL()
// ---------------------------------------------------------------------------
//
void CGSAccHeadsetView::HandleListBoxSelectionL( TInt aSelectedItem )
    {
    FUNC_LOG;

    // User has pressed selection key. If the selected item has just two
    // possible values, flip the value to the other option. If the item has
    // more possible values, show a selection dialog.
    switch ( aSelectedItem )
        {
        case KGSSettIdHSDP:
            // Show selection dialog for the accessory default profile
            ChangeDefaultProfileL(
                KSettingsHeadsetDefaultProfile, KGSSettIdHSDP );
            break;
        case KGSSettIdHSAA:
            // Change automatic answer mode setting without selection dialog
            ChangeAutoAnswerModeSettingL(
                KSettingsHeadsetAutomaticAnswer,
                KGSSettIdHSAA,
                EFalse );
            break;
       default:
            break;
        }
    }


// ---------------------------------------------------------------------------
// CGSAccHeadsetView::CGSAccHeadsetView()
// ---------------------------------------------------------------------------
//
CGSAccHeadsetView::CGSAccHeadsetView( CGSAccessoryPluginModel& aModel )
  : CGSAccBaseView( aModel )
    {
    FUNC_LOG;
    }

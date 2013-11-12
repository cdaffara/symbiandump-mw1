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
* Description:  Implementation of CGSAccMusicStandView class
*
*/


#include <e32cmn.h> // For accessoriescrkeys.h
#include <accessoriescrkeys.h>
#include <AccSettingsDomainCRKeys.h>
#include <gsaccessoryplugin.rsg>
#include <gscommon.hrh>

#include "acclocalviewids.h"
#include "gsaccessoryplugin.hrh"
#include "gsaccmusicstandcontainer.h"
#include "gsaccmusicstandview.h"
#include "trace.h"

// ========================= MEMBER FUNCTIONS ================================

// ---------------------------------------------------------------------------
// CGSAccMusicStandView::NewLC()
// ---------------------------------------------------------------------------
//
CGSAccMusicStandView* CGSAccMusicStandView::NewLC(
    CGSAccessoryPluginModel& aModel )
    {
    CGSAccMusicStandView* self =
        new ( ELeave ) CGSAccMusicStandView( aModel );
    CleanupStack::PushL( self );
    self->BaseConstructL( R_ACC_MUSIC_STAND_VIEW );
    return self;
    }


// ---------------------------------------------------------------------------
// CGSAccMusicStandView::~CGSAccMusicStandView()
// ---------------------------------------------------------------------------
//
CGSAccMusicStandView::~CGSAccMusicStandView()
    {
    FUNC_LOG;
    }


// ---------------------------------------------------------------------------
// TUid CGSAccMusicStandView::Id()
// ---------------------------------------------------------------------------
//
TUid CGSAccMusicStandView::Id() const
    {
    return KAccMusicStandViewId;
    }


// ---------------------------------------------------------------------------
// CGSAccMusicStandView::HandleCommandL()
// ---------------------------------------------------------------------------
//
void CGSAccMusicStandView::HandleCommandL( TInt aCommand )
    {
    FUNC_LOG;

    if ( aCommand == EGSCmdAppChange )
        {
        const TInt currentFeatureId = iContainer->CurrentFeatureId();
        if ( currentFeatureId == KGSSettIdMSAA )
            {
            // Change automatic answer mode setting through selection dialog
            ChangeAutoAnswerModeSettingL(
                KSettingsMusicStandAutomaticAnswer,
                KGSSettIdMSAA,
                ETrue );
            }
        else if ( currentFeatureId == KGSSettIdMSL )
            {
            // Change lights mode setting through selection dialog
            ChangeLightModeSettingL(
                EAccModeMusicStand,
                KAccServerMusicStandLights,
                KGSSettIdMSL,
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
// CGSAccMusicStandView::NewContainerL()
// ---------------------------------------------------------------------------
//
void CGSAccMusicStandView::NewContainerL()
    {
    iContainer = new ( ELeave ) CGSAccMusicStandContainer( iModel );
    }


// ---------------------------------------------------------------------------
// CGSAccMusicStandView::HandleListBoxSelectionL()
// ---------------------------------------------------------------------------
//
void CGSAccMusicStandView::HandleListBoxSelectionL( TInt aSelectedItem )
    {
    FUNC_LOG;

    // User has pressed selection key. If the selected item has just two
    // possible values, flip the value to the other option. If the item has
    // more possible values, show a selection dialog.
    switch ( aSelectedItem )
        {
        case KGSSettIdMSDP:
            // Show selection dialog for the accessory default profile
            ChangeDefaultProfileL(
                KSettingsMusicStandDefaultProfile, KGSSettIdMSDP );
            break;
        case KGSSettIdMSAA:
            // Change automatic answer mode setting without selection dialog
            ChangeAutoAnswerModeSettingL(
                KSettingsMusicStandAutomaticAnswer,
                KGSSettIdMSAA,
                EFalse );
            break;
        case KGSSettIdMSL:
            // Change lights mode setting without selection dialog
            ChangeLightModeSettingL(
                EAccModeMusicStand,
                KAccServerMusicStandLights,
                KGSSettIdMSL,
                EFalse );
            break;
       default:
            break;
        }
    }


// ---------------------------------------------------------------------------
// CGSAccMusicStandView::CGSAccMusicStandView()
// ---------------------------------------------------------------------------
//
CGSAccMusicStandView::CGSAccMusicStandView( CGSAccessoryPluginModel& aModel )
  : CGSAccBaseView( aModel )
    {
    FUNC_LOG;
    }

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
* Description:  View for Wired Carkit sub-view
*
*/


#include <e32cmn.h> // For accessoriescrkeys.h
#include <accessoriescrkeys.h>
#include <AccSettingsDomainCRKeys.h>
#include <gsaccessoryplugin.rsg>
#include <gscommon.hrh>

#include "acclocalviewids.h"
#include "gsaccessoryplugin.hrh"
#include "gsaccwiredcarkitview.h"
#include "gsaccwiredcarkitcontainer.h"
#include "trace.h"

// ========================= MEMBER FUNCTIONS ================================

// ---------------------------------------------------------------------------
// CGSAccWiredCarkitView::NewLC()
// ---------------------------------------------------------------------------
//
CGSAccWiredCarkitView* CGSAccWiredCarkitView::NewLC(
    CGSAccessoryPluginModel& aModel )
    {
    CGSAccWiredCarkitView* self =
        new ( ELeave ) CGSAccWiredCarkitView( aModel );
    CleanupStack::PushL( self );
    self->BaseConstructL( R_ACC_WIRED_CARKIT_VIEW );
    return self;
    }


// ---------------------------------------------------------------------------
// CGSAccWiredCarkitView::~CGSAccWiredCarkitView()
// ---------------------------------------------------------------------------
//
CGSAccWiredCarkitView::~CGSAccWiredCarkitView()
    {
    FUNC_LOG;
    }


// ---------------------------------------------------------------------------
// TUid CGSAccWiredCarkitView::Id()
// ---------------------------------------------------------------------------
//
TUid CGSAccWiredCarkitView::Id() const
    {
    return KAccWiredCarkitViewId;
    }


// ---------------------------------------------------------------------------
// CGSAccWiredCarkitView::HandleCommandL()
//
//
// ---------------------------------------------------------------------------
void CGSAccWiredCarkitView::HandleCommandL( TInt aCommand )
    {
    FUNC_LOG;

    if ( aCommand == EGSCmdAppChange )
        {
        const TInt currentFeatureId = iContainer->CurrentFeatureId();

        if ( currentFeatureId == KGSSettIdCKAA )
            {
            // Change automatic answer mode setting through selection dialog
            ChangeAutoAnswerModeSettingL(
                KSettingsCarKitAutomaticAnswer,
                KGSSettIdCKAA,
                ETrue );
            }
        else if ( currentFeatureId == KGSSettIdCKL )
            {
            // Change lights mode setting through selection dialog
            ChangeLightModeSettingL(
                EAccModeWiredCarKit,
                KAccServerWiredCarKitLights,
                KGSSettIdCKL,
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
// CGSAccWiredCarkitView::NewContainerL()
// ---------------------------------------------------------------------------
//
void CGSAccWiredCarkitView::NewContainerL()
    {
    iContainer = new ( ELeave ) CGSAccWiredCarkitContainer( iModel );
    }


// ---------------------------------------------------------------------------
// CGSAccWiredCarkitView::HandleListBoxSelectionL()
// ---------------------------------------------------------------------------
//
void CGSAccWiredCarkitView::HandleListBoxSelectionL( TInt aSelectedItem )
    {
    FUNC_LOG;

    // User has pressed selection key. If the selected item has just two
    // possible values, flip the value to the other option. If the item has
    // more possible values, show a selection dialog.
    switch ( aSelectedItem )
        {
        case KGSSettIdCKDP:
            // Show selection dialog for the accessory default profile
            ChangeDefaultProfileL(
                KSettingsCarKitDefaultProfile, KGSSettIdCKDP );
            break;
        case KGSSettIdCKAA:
            // Change automatic answer mode setting without selection dialog
            ChangeAutoAnswerModeSettingL(
                KSettingsCarKitAutomaticAnswer,
                KGSSettIdCKAA,
                EFalse );
            break;
        case KGSSettIdCKL:
            // Change lights mode setting without selection dialog
            ChangeLightModeSettingL(
                EAccModeWiredCarKit,
                KAccServerWiredCarKitLights,
                KGSSettIdCKL,
                EFalse );
            break;
       default:
            break;
        }
    }


// ---------------------------------------------------------------------------
// CGSAccWiredCarkitView::CGSAccWiredCarkitView()
// ---------------------------------------------------------------------------
//
CGSAccWiredCarkitView::CGSAccWiredCarkitView( CGSAccessoryPluginModel& aModel )
  : CGSAccBaseView( aModel )
    {
    FUNC_LOG;
    }

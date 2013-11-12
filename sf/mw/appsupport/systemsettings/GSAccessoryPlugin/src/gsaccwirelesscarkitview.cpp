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
* Description:  Implementation of CGSAccWirelessCarkitView class
*
*/


#include <e32cmn.h> // For accessoriescrkeys.h
#include <accessoriescrkeys.h>
#include <gsaccessoryplugin.rsg>
#include <gscommon.hrh>

#include "acclocalviewids.h"
#include "gsaccessoryplugin.hrh"
#include "gsaccwirelesscarkitcontainer.h"
#include "gsaccwirelesscarkitview.h"
#include "trace.h"

// ========================= MEMBER FUNCTIONS ================================

// ---------------------------------------------------------------------------
// CGSAccWirelessCarkitView::NewLC()
// ---------------------------------------------------------------------------
//
CGSAccWirelessCarkitView* CGSAccWirelessCarkitView::NewLC(
    CGSAccessoryPluginModel& aModel )
    {
    CGSAccWirelessCarkitView* self =
        new ( ELeave ) CGSAccWirelessCarkitView( aModel );
    CleanupStack::PushL( self );
    self->BaseConstructL( R_ACC_WIRELESS_CARKIT_VIEW );
    return self;
    }


// ---------------------------------------------------------------------------
// CGSAccWirelessCarkitView::~CGSAccWirelessCarkitView()
// ---------------------------------------------------------------------------
//
CGSAccWirelessCarkitView::~CGSAccWirelessCarkitView()
    {
    FUNC_LOG;
    }


// ---------------------------------------------------------------------------
// TUid CGSAccWirelessCarkitView::Id()
// ---------------------------------------------------------------------------
//
TUid CGSAccWirelessCarkitView::Id() const
    {
    return KAccWirelessCarkitViewId;
    }


// ---------------------------------------------------------------------------
// CGSAccWirelessCarkitView::HandleCommandL()
// ---------------------------------------------------------------------------
//
void CGSAccWirelessCarkitView::HandleCommandL( TInt aCommand )
    {
    FUNC_LOG;

    if ( aCommand == EGSCmdAppChange )
        {
        const TInt currentFeatureId = iContainer->CurrentFeatureId();

        if ( currentFeatureId == KGSSettIdWCKAA )
            {
            // Change automatic answer mode setting through selection dialog
            ChangeAutoAnswerModeSettingL(
                KSettingsWirelessCarkitAutomaticAnswer,
                KGSSettIdWCKAA,
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
// CGSAccWirelessCarkitView::NewContainerL()
// ---------------------------------------------------------------------------
//
void CGSAccWirelessCarkitView::NewContainerL()
    {
    iContainer = new ( ELeave ) CGSAccWirelessCarkitContainer( iModel );
    }


// ---------------------------------------------------------------------------
// CGSAccWirelessCarkitView::HandleListBoxSelectionL()
// ---------------------------------------------------------------------------
//
void CGSAccWirelessCarkitView::HandleListBoxSelectionL(
    TInt aSelectedItem )
    {
    FUNC_LOG;

    // User has pressed selection key. If the selected item has just two
    // possible values, flip the value to the other option. If the item has
    // more possible values, show a selection dialog.
    switch ( aSelectedItem )
        {
        case KGSSettIdWCKDP:
            // Show selection dialog for the accessory default profile
            ChangeDefaultProfileL(
                KSettingsWirelessCarkitDefaultProfile, KGSSettIdWCKDP );
            break;
        case KGSSettIdWCKAA:
            // Change automatic answer mode setting without selection dialog
            ChangeAutoAnswerModeSettingL(
                KSettingsWirelessCarkitAutomaticAnswer,
                KGSSettIdWCKAA,
                EFalse );
            break;
       default:
            break;
        }
    }


// ---------------------------------------------------------------------------
// CGSAccWirelessCarkitView::CGSAccWirelessCarkitView()
// ---------------------------------------------------------------------------
//
CGSAccWirelessCarkitView::CGSAccWirelessCarkitView(
    CGSAccessoryPluginModel& aModel )
  : CGSAccBaseView( aModel )
    {
    FUNC_LOG;
    }

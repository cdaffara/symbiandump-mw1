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
* Description:  Implementation of CGSAccHeadphonesView class
*
*/


#include <e32cmn.h> // For accessoriescrkeys.h
#include <accessoriescrkeys.h>
#include <gsaccessoryplugin.rsg>
#include <gscommon.hrh>

#include "acclocalviewids.h"
#include "gsaccessoryplugin.hrh"
#include "gsaccheadphonescontainer.h"
#include "gsaccheadphonesview.h"
#include "trace.h"

// ========================= MEMBER FUNCTIONS ================================

// ---------------------------------------------------------------------------
// CGSAccHeadphonesView::NewLC()
// ---------------------------------------------------------------------------
//
CGSAccHeadphonesView* CGSAccHeadphonesView::NewLC(
    CGSAccessoryPluginModel& aModel )
    {
    CGSAccHeadphonesView* self = new ( ELeave ) CGSAccHeadphonesView( aModel );
    CleanupStack::PushL( self );
    self->BaseConstructL( R_ACC_HEADPHONES_VIEW );
    return self;
    }


// ---------------------------------------------------------------------------
// CGSAccHeadphonesView::~CGSAccHeadphonesView()
// ---------------------------------------------------------------------------
//
CGSAccHeadphonesView::~CGSAccHeadphonesView()
    {
    FUNC_LOG;
    }


// ---------------------------------------------------------------------------
// TUid CGSAccHeadphonesView::Id()
// ---------------------------------------------------------------------------
//
TUid CGSAccHeadphonesView::Id() const
    {
    return KAccHeadphonesViewId;
    }


// ---------------------------------------------------------------------------
// CGSAccHeadphonesView::HandleCommandL()
// ---------------------------------------------------------------------------
//
void CGSAccHeadphonesView::HandleCommandL( TInt aCommand )
    {
    FUNC_LOG;

    switch ( aCommand )
        {
        case EGSCmdAppChange:
            // Act as user had pressed the selection key
            HandleListBoxSelectionL( iContainer->CurrentFeatureId() );
            break;
        default:
            CGSAccBaseView::HandleCommandL( aCommand );
            break;
        }
    }


// ---------------------------------------------------------------------------
// CGSAccHeadphonesView::NewContainerL()
// ---------------------------------------------------------------------------
//
void CGSAccHeadphonesView::NewContainerL()
    {
    iContainer = new ( ELeave ) CGSAccHeadphonesContainer ( iModel );
    }


// ---------------------------------------------------------------------------
// CGSAccHeadphonesView::HandleListBoxSelectionL()
// ---------------------------------------------------------------------------
//
void CGSAccHeadphonesView::HandleListBoxSelectionL( TInt aSelectedItem )
    {
    FUNC_LOG;

    // User has pressed selection key. If the selected item has just two
    // possible values, flip the value to the other option. If the item has
    // more possible values, show a selection dialog.
    if ( aSelectedItem == KGSSettIdHPDP )
        {
        ChangeDefaultProfileL(
            KSettingsHeadphonesDefaultProfile, KGSSettIdHPDP );
        }
    }


// ---------------------------------------------------------------------------
// CGSAccHeadphonesView::CGSAccHeadphonesView()
// ---------------------------------------------------------------------------
//
CGSAccHeadphonesView::CGSAccHeadphonesView( CGSAccessoryPluginModel& aModel )
  : CGSAccBaseView( aModel )
    {
    FUNC_LOG;
    }

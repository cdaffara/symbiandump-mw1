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
* Description:  Implementation of CGSAccHACView class
*
*/


#include <gsaccessoryplugin.rsg>
#include <gscommon.hrh>

#include "acclocalviewids.h"
#include "gsaccessoryplugin.hrh"
#include "gsaccessorypluginmodel.h"
#include "gsacchaccontainer.h"
#include "gsacchacview.h"
#include "trace.h"

// ========================= MEMBER FUNCTIONS ================================

// ---------------------------------------------------------------------------
// CGSAccHACView::NewLC()
// ---------------------------------------------------------------------------
//
CGSAccHACView* CGSAccHACView::NewLC( CGSAccessoryPluginModel& aModel )
    {
    CGSAccHACView* self = new ( ELeave ) CGSAccHACView( aModel );
    CleanupStack::PushL( self );
    self->BaseConstructL( R_ACC_HAC_VIEW );
    return self;
    }


// ---------------------------------------------------------------------------
// CGSAccHACView::~CGSAccHACView()
// ---------------------------------------------------------------------------
//
CGSAccHACView::~CGSAccHACView()
    {
    FUNC_LOG;
    }


// ---------------------------------------------------------------------------
// TUid CGSAccHACView::Id()
// ---------------------------------------------------------------------------
//
TUid CGSAccHACView::Id() const
    {
    return KAccHACViewId;
    }


// ---------------------------------------------------------------------------
// CGSAccHACView::HandleCommandL()
// ---------------------------------------------------------------------------
//
void CGSAccHACView::HandleCommandL( TInt aCommand )
    {
    FUNC_LOG;

    if ( aCommand == EGSCmdAppChange )
        {
        const TInt currentFeatureId = iContainer->CurrentFeatureId();
        if ( currentFeatureId == KGSSettIdHACMode )
            {
            // Change HAC mode setting through selection dialog
            ChangeHACModeSettingL( ETrue );
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
// CGSAccHACView::NewContainerL()
// ---------------------------------------------------------------------------
//
void CGSAccHACView::NewContainerL()
    {
    iContainer = new ( ELeave ) CGSAccHACContainer( iModel );
    }


// ---------------------------------------------------------------------------
// CGSAccHACView::HandleListBoxSelectionL()
// ---------------------------------------------------------------------------
//
void CGSAccHACView::HandleListBoxSelectionL( TInt aSelectedItem )
    {
    FUNC_LOG;

    // User has pressed selection key. If the selected item has just two
    // possible values, flip the value to the other option. If the item has
    // more possible values, show a selection dialog.
    if ( aSelectedItem == KGSSettIdHACMode )
        {
        // Change HAC mode setting without selection dialog
        ChangeHACModeSettingL( EFalse );
        }
    }


// ---------------------------------------------------------------------------
// CGSAccHACView::CGSAccHACView()
// ---------------------------------------------------------------------------
//
CGSAccHACView::CGSAccHACView( CGSAccessoryPluginModel& aModel )
  : CGSAccBaseView( aModel )
    {
    FUNC_LOG;
    }


// ---------------------------------------------------------------------------
// CGSAccHACView::ChangeHACModeSettingL()
// ---------------------------------------------------------------------------
//
void CGSAccHACView::ChangeHACModeSettingL( TBool aUseSettingPage )
    {
    FUNC_LOG;

    TInt currentValue = iModel.HacMode();
    TBool updateValue( EFalse );

    // Value is updated either by setting page or simply switching to other
    // value (On<->Off).
    if ( aUseSettingPage )
        {
        updateValue = ShowRadioButtonSettingsPageL(
            R_ACC_HAC_MODE_SETTING_PAGE,
            R_ACC_HAC_MODE_SETTING_PAGE_LBX,
            currentValue );
        }
    else
        {
        // User pressed selection key instead of opening setting page.
        // Do not ask user input from dialog - just flip the value:
        // 0 -> 1, 1 -> 0.
        iModel.FlipValue( currentValue );
        updateValue = ETrue;
        }

    if ( updateValue ) // If value was changed, store it.
        {
        iModel.SetHacModeL( currentValue );
        UpdateListBoxL( KGSSettIdHACMode );
        }
    }

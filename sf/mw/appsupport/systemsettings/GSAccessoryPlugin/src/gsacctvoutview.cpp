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
* Description:  Implementation of CGSAccTvoutView class
*
*/


#include <e32cmn.h> // For accessoriescrkeys.h
#include <accessoriescrkeys.h>
#include <AknQueryDialog.h>
#include <avkon.hrh>
#include <gsaccessoryplugin.rsg>
#include <gscommon.hrh>
#include <GSServerEngine.h>
#include <StringLoader.h>

#include "acclocalviewids.h"
#include "gsaccessoryplugin.hrh"
#include "gsaccessorypluginmodel.h"
#include "gsacctvoutcontainer.h"
#include "gsacctvoutview.h"
#include "trace.h"

// ========================= MEMBER FUNCTIONS ================================

// ---------------------------------------------------------------------------
// CGSAccTvoutView::NewLC()
// ---------------------------------------------------------------------------
//
CGSAccTvoutView* CGSAccTvoutView::NewLC( CGSAccessoryPluginModel& aModel )
    {
    CGSAccTvoutView* self = new ( ELeave ) CGSAccTvoutView( aModel );
    CleanupStack::PushL( self );
    self->ConstructL();
    return self;
    }


// ---------------------------------------------------------------------------
// CGSAccTvoutView::~CGSAccTvoutView()
// ---------------------------------------------------------------------------
//
CGSAccTvoutView::~CGSAccTvoutView()
    {
    FUNC_LOG;

    delete iServerEngine;
    }


// ---------------------------------------------------------------------------
// TUid CGSAccTvoutView::Id()
// ---------------------------------------------------------------------------
//
TUid CGSAccTvoutView::Id() const
    {
    return KAccTvoutViewId;
    }


// ---------------------------------------------------------------------------
// CGSAccTvoutView::HandleCommandL()
// ---------------------------------------------------------------------------
//
void CGSAccTvoutView::HandleCommandL( TInt aCommand )
    {
    FUNC_LOG;

    switch ( aCommand )
        {
        case EGSCmdAppChange:
            {
            const TInt currentFeatureId = iContainer->CurrentFeatureId();
            if ( currentFeatureId == EGSSettIdTvAspectRatio )
                {
                // Change aspect ratio setting through selection dialog
                ChangeAspectRatioSettingL( ETrue );
                }
            else if ( currentFeatureId == EGSSettIdTvFlickerFilter )
                {
                // Change flicker filter setting through selection dialog
                ChangeFlickerFilterSettingL( ETrue );
                }
            else
                {
                // Act as user had pressed the selection key
                HandleListBoxSelectionL( currentFeatureId );
                }
            }
            break;
        case EAknSoftkeyBack: // Fall through
        case EAknCmdExit:
            if ( iSettingChanged )
                {
                DisplaySettingsChangeNoteL();
                } // Fall through
        default:
            CGSAccBaseView::HandleCommandL( aCommand );
            break;
        }
    }

// ---------------------------------------------------------------------------
// CGSAccTvoutView::NewContainerL()
// ---------------------------------------------------------------------------
//
void CGSAccTvoutView::NewContainerL()
    {
    iContainer = new ( ELeave ) CGSAccTvoutContainer( iModel, *iServerEngine );
    }


// ---------------------------------------------------------------------------
// CGSAccTvoutView::HandleListBoxSelectionL()
// ---------------------------------------------------------------------------
//
void CGSAccTvoutView::HandleListBoxSelectionL( TInt aSelectedItem )
    {
    FUNC_LOG;

    // User has pressed selection key. If the selected item has just two
    // possible values, flip the value to the other option. If the item has
    // more possible values, show a selection dialog.
    switch ( aSelectedItem )
        {
        case EGSSettIdTvoutDP:
            // Show selection dialog for the accessory default profile
            ChangeDefaultProfileL(
                KSettingsTvOutDefaultProfile, EGSSettIdTvoutDP );
            break;
        case EGSSettIdTvAspectRatio:
            // Change aspect ratio setting without selection dialog
            ChangeAspectRatioSettingL( EFalse );
            break;
        case EGSSettIdTvSystem:
            // Show selection dialog for the TV system
            ChangeTvSystemSettingL();
            break;
        case EGSSettIdTvFlickerFilter:
            // Change flicker filter setting without selection dialog
            ChangeFlickerFilterSettingL( EFalse );
            break;
       default:
            break;
        }
    }


// ---------------------------------------------------------------------------
// CGSAccTvoutView::CGSAccTvoutView()
// ---------------------------------------------------------------------------
//
CGSAccTvoutView::CGSAccTvoutView( CGSAccessoryPluginModel& aModel )
  : CGSAccBaseView( aModel ),
    iSettingChanged( EFalse )
    {
    FUNC_LOG;
    }


// ---------------------------------------------------------------------------
// CGSAccTvoutView::ConstructL()
// ---------------------------------------------------------------------------
//
void CGSAccTvoutView::ConstructL()
    {
    FUNC_LOG;

    BaseConstructL( R_ACC_TVOUT_VIEW );

    iServerEngine = CGSServerEngine::NewL();
    }


// ---------------------------------------------------------------------------
// CGSAccTvoutView::ChangeAspectRatioSettingL()
// ---------------------------------------------------------------------------
//
void CGSAccTvoutView::ChangeAspectRatioSettingL( TBool aSettingPage )
    {
    FUNC_LOG;

    TInt currentValue = iServerEngine->AspectRatioL();
    TBool updateValue( EFalse );

    if ( aSettingPage )
        {
        updateValue = ShowRadioButtonSettingsPageL(
            R_ACC_ASPECT_RATIO_SETTING_PAGE,
            R_ACC_ASPECT_RATIO_SETTING_PAGE_LBX,
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
        iServerEngine->SetAspectRatioL( currentValue );
        UpdateListBoxL( EGSSettIdTvAspectRatio );
        iSettingChanged = ETrue;
        }
    }


// ---------------------------------------------------------------------------
// CGSAccTvoutView::ChangeFlickerFilterSettingL()
// ---------------------------------------------------------------------------
//
void CGSAccTvoutView::ChangeFlickerFilterSettingL( TBool aSettingPage )
    {
    FUNC_LOG;

    TInt currentValue = iServerEngine->FlickerFilterL();
    TBool updateValue( EFalse );

    if ( aSettingPage )
        {
        updateValue = ShowRadioButtonSettingsPageL(
            R_ACC_FLICKER_FILTER_SETTING_PAGE,
            R_ACC_FLICKER_FILTER_SETTING_PAGE_LBX,
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
        iServerEngine->SetFlickerFilterL( currentValue );
        UpdateListBoxL( EGSSettIdTvFlickerFilter );
        iSettingChanged = ETrue;
        }
    }


// ---------------------------------------------------------------------------
// CGSAccTvoutView::ChangeTvSystemSettingL()
// ---------------------------------------------------------------------------
//
void CGSAccTvoutView::ChangeTvSystemSettingL()
    {
    FUNC_LOG;

    TInt currentValue = iServerEngine->TvSystemL();

    // If PALM is not supported, index correction
    if( !iModel.PalmSupport() && currentValue )
        {
        currentValue--;
        }

    if ( ShowRadioButtonSettingsPageL(
            R_ACC_TV_SYSTEM_SETTING_PAGE,
            iModel.PalmSupport() ?
                R_ACC_TV_SYSTEM_SETTING_PAGE_LBX :
                R_ACC_TV_SYSTEM_SETTING_PAGE_NO_PALM_LBX,
            currentValue ) )
        {
        if( !iModel.PalmSupport() && currentValue )
            {
            //In case PALM support is missing fix the NTSC value index
            currentValue++;
            }

        iServerEngine->SetTvSystemL( currentValue );
        UpdateListBoxL( EGSSettIdTvSystem );
        iSettingChanged = ETrue;
        }
    }


// ---------------------------------------------------------------------------
// CGSAccTvoutView::DisplaySettingsChangeNoteL
// ---------------------------------------------------------------------------
//
void CGSAccTvoutView::DisplaySettingsChangeNoteL()
    {
    FUNC_LOG;

    HBufC* buf = StringLoader::LoadLC( R_ACC_TV_SETTINGS_CHANGE_STR, iCoeEnv );

    CAknQueryDialog* query =
        new( ELeave ) CAknQueryDialog( CAknQueryDialog::ENoTone );
    query->PrepareLC( R_ACC_TV_SETTINGS_CHANGE_QUERY );
    query->SetPromptL( *buf );

    query->RunLD();

    // No need to display note until setting is changed again
    iSettingChanged = EFalse;

    CleanupStack::PopAndDestroy( buf );
    }


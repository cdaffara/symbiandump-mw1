/*
* Copyright (c) 2002-2008 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Declaration of CGSAccBaseView class
*
*/


#include <aknradiobuttonsettingpage.h>
#include <aknViewAppUi.h>
#include <featmgr.h>
#include <gsaccessoryplugin.rsg>
#include <gscommon.hrh>
#include <hlplch.h> // For HlpLauncher

#include "gsaccbasecontainer.h"
#include "gsaccbaseview.h"
#include "gsaccessorypluginconst.hrh"
#include "gsaccessorypluginmodel.h"
#include "gsaccprofilelist.h"
#include "trace.h"

// ========================= MEMBER FUNCTIONS ================================

// ---------------------------------------------------------------------------
// CGSAccBaseView::~CGSAccBaseView
// ---------------------------------------------------------------------------
//
CGSAccBaseView::~CGSAccBaseView()
    {
    FUNC_LOG;

    delete iContainer;
    }


// ---------------------------------------------------------------------------
// CGSAccBaseView::HandleCommandL()
// ---------------------------------------------------------------------------
//
void CGSAccBaseView::HandleCommandL( TInt aCommand )
    {
    FUNC_LOG;

    switch ( aCommand )
        {
        case EGSMSKCmdAppChange:
            // Forward command to sub-class.
            HandleListBoxSelectionL( iContainer->CurrentFeatureId() );
            break;
        case EAknSoftkeyBack:
            // Go back to accessories main view.
            AppUi()->ActivateLocalViewL( TUid::Uid( KGSAccessoryPluginUid ) );
            break;
        case EAknCmdHelp:
            {
            // Launch help if it is supported.
            if ( FeatureManager::FeatureSupported( KFeatureIdHelp ) )
                {
                HlpLauncher::LaunchHelpApplicationL(
                    iEikonEnv->WsSession(), AppUi()->AppHelpContextL() );
                }
            break;
            }
        default:
            // Let framework handle other commands.
            AppUi()->HandleCommandL( aCommand );
            break;
        }
    }


// ---------------------------------------------------------------------------
// CGSAccBaseView::DoActivateL
// ---------------------------------------------------------------------------
//
void CGSAccBaseView::DoActivateL(
    const TVwsViewId& /*aPrevViewId*/,
    TUid /*aCustomMessageId*/,
    const TDesC8& /*aCustomMessage*/ )
    {
    FUNC_LOG;

    DestroyContainer();
    CreateContainerL();

    AppUi()->AddToViewStackL( *this, iContainer );
    iContainer->iListBox->SetListBoxObserver( this );
    }


// ---------------------------------------------------------------------------
// CGSAccBaseView::DoDeactivate()
// ---------------------------------------------------------------------------
//
void CGSAccBaseView::DoDeactivate()
    {
    FUNC_LOG;

    DestroyContainer();
    }


// ----------------------------------------------------------------------------
// CGSAccBaseView::DynInitMenuPaneL()
// ----------------------------------------------------------------------------
//
void CGSAccBaseView::DynInitMenuPaneL(
    TInt aResourceId,
    CEikMenuPane* aMenuPane )
    {
    FUNC_LOG;

    if ( aResourceId == R_GS_MENU_ITEM_HELP )
        {
        User::LeaveIfNull( aMenuPane );
        // Dim help item unless help feature is supported.
        aMenuPane->SetItemDimmed(
            EAknCmdHelp,
            !FeatureManager::FeatureSupported( KFeatureIdHelp ) );
        }
    }


// ---------------------------------------------------------------------------
// CGSAccBaseView::HandleListBoxEventL
// ---------------------------------------------------------------------------
//
void CGSAccBaseView::HandleListBoxEventL(
    CEikListBox* /*aListBox*/,
    TListBoxEvent aEventType )
    {
    FUNC_LOG;

    switch ( aEventType )
        {
        case EEventEnterKeyPressed:
        case EEventItemSingleClicked:
            HandleListBoxSelectionL( iContainer->CurrentFeatureId() );
            break;
        default:
           break;
        }
    }


// ---------------------------------------------------------------------------
// CGSAccBaseView::CGSAccBaseView
// ---------------------------------------------------------------------------
//
CGSAccBaseView::CGSAccBaseView( CGSAccessoryPluginModel& aModel )
  : iModel( aModel )
    {
    FUNC_LOG;
    }


// ---------------------------------------------------------------------------
// CGSAccBaseView::UpdateListBoxL
// ---------------------------------------------------------------------------
//
void CGSAccBaseView::UpdateListBoxL( TInt aItemId )
    {
    FUNC_LOG;
    ASSERT_TRACE( iContainer );

    iContainer->UpdateListBoxL( aItemId );
    }


// ---------------------------------------------------------------------------
// CGSAccBaseView::ShowRadioButtonSettingsPageL()
// ---------------------------------------------------------------------------
//
TBool CGSAccBaseView::ShowRadioButtonSettingsPageL(
    TInt aDlgResourceID,
    TInt aItemsResourceID,
    TInt& aCurrentSelectionIndex ) const
    {
    FUNC_LOG;

    CDesCArrayFlat* items =
        iCoeEnv->ReadDesC16ArrayResourceL( aItemsResourceID );
    CleanupStack::PushL( items );

    TBool ret = ShowRadioButtonSettingsPageL(
        aDlgResourceID, *items, aCurrentSelectionIndex );

    CleanupStack::PopAndDestroy( items );
    return ret;
    }


// ---------------------------------------------------------------------------
// CGSAccBaseView::ShowRadioButtonSettingsPageL()
// ---------------------------------------------------------------------------
//
TBool CGSAccBaseView::ShowRadioButtonSettingsPageL(
    TInt aDlgResourceID,
    const MDesCArray& aItemArray,
    TInt& aCurrentSelectionIndex ) const
    {
    FUNC_LOG;

    CAknRadioButtonSettingPage* dlg =
        new ( ELeave ) CAknRadioButtonSettingPage(
            aDlgResourceID,
            aCurrentSelectionIndex,
            &aItemArray );

    TBool ret = dlg->ExecuteLD( CAknSettingPage::EUpdateWhenChanged );

    INFO_1( "CAknRadioButtonSettingPage::ExecuteLD returned %d", ret );
    return ret;
    }


// ---------------------------------------------------------------------------
// CGSAccBaseView::ChangeDefaultProfileL()
// ---------------------------------------------------------------------------
//
void CGSAccBaseView::ChangeDefaultProfileL(
    TInt aProfileKey,
    TInt aListBoxId )
    {
    FUNC_LOG;

    // Read current value
    TInt current = iModel.ProfileList().IndexByIdL(
        iModel.DefaultProfileByKey( aProfileKey ), iCoeEnv );

    // Query new value from user
    TBool update = ShowRadioButtonSettingsPageL(
        R_ACC_DEFAULT_PROFILE_SETTING_PAGE,
        iModel.ProfileList().NameListL( iCoeEnv ),
        current );

    if ( update )
        {
        // Store new value
        iModel.SetDefaultProfileByKeyL(
            aProfileKey,
            iModel.ProfileList().IdByIndexL( current, iCoeEnv ) );
        UpdateListBoxL( aListBoxId );
        }
    }


// ---------------------------------------------------------------------------
// CGSAccBaseView::ChangeAutoAnswerModeSettingL()
// ---------------------------------------------------------------------------
//
void CGSAccBaseView::ChangeAutoAnswerModeSettingL(
    TInt aAutoAnswerKey,
    TInt aListBoxId,
    TBool aUseSettingPage )
    {
    FUNC_LOG;

    TInt currentValue = iModel.AutoAnswerModeByKey( aAutoAnswerKey );
    TBool updateValue( EFalse );

    // Value is updated either by setting page or simply switching to other
    // value (On<->Off).
    if ( aUseSettingPage )
        {
        updateValue = ShowRadioButtonSettingsPageL(
            R_ACC_AUTO_ANSWER_SETTING_PAGE,
            R_ACC_AUTO_ANSWER_SETTING_PAGE_LBX,
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
        iModel.SetAutoAnswerModeByKeyL( aAutoAnswerKey, currentValue );
        UpdateListBoxL( aListBoxId );
        }
    }


// ---------------------------------------------------------------------------
// CGSAccBaseView::ChangeLightModeSettingL()
// ---------------------------------------------------------------------------
//
void CGSAccBaseView::ChangeLightModeSettingL(
    TAccMode aAccMode,
    TInt aAccSetting,
    TInt aListBoxId,
    TBool aSettingPage )
    {
    FUNC_LOG;

    TInt currentValue = iModel.LightMode( aAccMode, aAccSetting );
    TBool updateValue( EFalse );

    // Value is updated either by setting page or simply switching to other
    // value (On<->Off).
    if ( aSettingPage )
        {
        updateValue = ShowRadioButtonSettingsPageL(
            R_ACC_LIGHTS_SETTING_PAGE,
            R_ACC_LIGHTS_SETTING_PAGE_LBX,
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

    if ( updateValue )
        {
        iModel.SetLightModeL( aAccMode, aAccSetting, currentValue );
        UpdateListBoxL( aListBoxId );
        }
    }


// ---------------------------------------------------------------------------
// CGSAccBaseView::CreateContainerL()
// ---------------------------------------------------------------------------
//
void CGSAccBaseView::CreateContainerL()
    {
    FUNC_LOG;

    NewContainerL(); // Derived class should assign object to iContainer.
    ASSERT_TRACE( iContainer );

    iContainer->SetMopParent( this );
    TRAPD( error, iContainer->ConstructL( ClientRect() ) );
    ERROR( error, "Failed to create container" );

    if ( error )
        {
        delete iContainer;
        iContainer = NULL;
        User::Leave( error );
        }
    }


// ---------------------------------------------------------------------------
// CGSAccBaseView::DestroyContainer()
// ---------------------------------------------------------------------------
//
void CGSAccBaseView::DestroyContainer()
    {
    FUNC_LOG;

    if ( iContainer )
        {
        AppUi()->RemoveFromViewStack( *this, iContainer );
        delete iContainer;
        iContainer = NULL;
        }
    }

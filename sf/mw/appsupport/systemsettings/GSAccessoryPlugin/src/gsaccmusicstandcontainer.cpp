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
* Description:  Implementation of CGSAccMusicStandContainer class
*
*/


#include <e32cmn.h> // For accessoriescrkeys.h
#include <accessoriescrkeys.h>
#include <AccSettingsDomainCRKeys.h>
#include <gsaccessoryplugin.rsg>
#include <gsfwviewuids.h> // for KUidGS
#include <gslistbox.h>
#include <csxhelp/cp.hlp.hrh>

#include "gsaccessoryplugin.hrh"
#include "gsaccessorypluginmodel.h"
#include "gsaccmusicstandcontainer.h"
#include "gsaccprofilelist.h"
#include "trace.h"

// ========================= MEMBER FUNCTIONS ================================

// ---------------------------------------------------------------------------
// CGSAccMusicStandContainer::CGSAccMusicStandContainer()
// ---------------------------------------------------------------------------
//
CGSAccMusicStandContainer::CGSAccMusicStandContainer(
    CGSAccessoryPluginModel& aModel )
  : CGSAccBaseContainer( aModel )
    {
    FUNC_LOG;
    }


// ---------------------------------------------------------------------------
// CGSAccMusicStandContainer::~CGSAccMusicStandContainer()
// ---------------------------------------------------------------------------
//
CGSAccMusicStandContainer::~CGSAccMusicStandContainer()
    {
    FUNC_LOG;

    delete iLightModeItems;
    delete iAutoAnswerItems;
    }


// ---------------------------------------------------------------------------
// CGSAccMusicStandContainer::UpdateListBoxL( TInt aFeatureId )
// ---------------------------------------------------------------------------
//
void CGSAccMusicStandContainer::UpdateListBoxL( TInt aFeatureId )
    {
    FUNC_LOG;

    switch( aFeatureId )
        {
        case KGSSettIdMSDP:
            RefreshDefaultProfileL();
            break;
        case KGSSettIdMSAA:
            RefreshAutoAnswerL();
            break;
        case KGSSettIdMSL:
            RefreshLightModeL();
            break;
        default:
            break;
        }

    iListBox->HandleItemAdditionL();
    }


// ---------------------------------------------------------------------------
// CGSAccMusicStandContainer::ConstructL()
// ---------------------------------------------------------------------------
//
void CGSAccMusicStandContainer::ConstructL( const TRect& aRect )
    {
    FUNC_LOG;

    CreateListBoxL();
    BaseConstructL( aRect, R_ACC_MUSIC_STAND_TITLE, R_ACC_MUSIC_STAND_LBX ); // Needs the listbox
    }


// ---------------------------------------------------------------------------
// CGSAccMusicStandContainer::ConstructListBoxL()
// ---------------------------------------------------------------------------
//
void CGSAccMusicStandContainer::ConstructListBoxL( TInt aResLbxId )
    {
    FUNC_LOG;

    CGSAccBaseContainer::ConstructListBoxL( aResLbxId );

    iAutoAnswerItems =
        iCoeEnv->ReadDesC16ArrayResourceL( R_ACC_AUTO_ANSWER_SETTING_PAGE_LBX );
    iLightModeItems =
        iCoeEnv->ReadDesC16ArrayResourceL( R_ACC_LIGHTS_SETTING_PAGE_LBX );

    RefreshDefaultProfileL();
    MakeItemVisibleL( KGSSettIdMSDP );

    RefreshAutoAnswerL();
    MakeItemVisibleL( KGSSettIdMSAA );

    RefreshLightModeL();
    MakeItemVisibleL( KGSSettIdMSL );
    }


// ---------------------------------------------------------------------------
// CGSAccMusicStandContainer::GetHelpContext() const
// ---------------------------------------------------------------------------
//
void CGSAccMusicStandContainer::GetHelpContext(
    TCoeHelpContext& aContext ) const
    {
    FUNC_LOG;

    aContext.iMajor = KUidGS;
    aContext.iContext = KSET_HLP_ACCESSORIES_MUSIC_ST;
    }


// ---------------------------------------------------------------------------
// CGSAccMusicStandContainer::RefreshDefaultProfileL()
// ---------------------------------------------------------------------------
//
void CGSAccMusicStandContainer::RefreshDefaultProfileL()
    {
    FUNC_LOG;

    TInt profile =
        iModel.DefaultProfileByKey( KSettingsMusicStandDefaultProfile );
    // Set default profile item text.
    SetItemTextL(
        KGSSettIdMSDP,
        iModel.ProfileList().NameByIdL( profile, iCoeEnv ) );
    }


// ---------------------------------------------------------------------------
// CGSAccMusicStandContainer::RefreshAutoAnswerL()
// ---------------------------------------------------------------------------
//
void CGSAccMusicStandContainer::RefreshAutoAnswerL()
    {
    FUNC_LOG;

    TInt mode = iModel.AutoAnswerModeByKey( KSettingsMusicStandAutomaticAnswer );
    if ( iAutoAnswerItems && mode >= 0 && mode < iAutoAnswerItems->Count() )
        {
        SetItemTextL( KGSSettIdMSAA, ( *iAutoAnswerItems )[ mode ] );
        }
    }


// ---------------------------------------------------------------------------
// CGSAccMusicStandContainer::RefreshLightModeL()
// ---------------------------------------------------------------------------
//
void CGSAccMusicStandContainer::RefreshLightModeL()
    {
    FUNC_LOG;

    TInt mode = iModel.LightMode( EAccModeMusicStand, KAccServerMusicStandLights );
    if ( iLightModeItems && mode >= 0 && mode < iLightModeItems->Count() )
        {
        SetItemTextL( KGSSettIdMSL, ( *iLightModeItems )[ mode ] );
        }
    }

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
* Description:  Implementation of CGSAccWiredCarkitContainer class
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
#include "gsaccprofilelist.h"
#include "gsaccwiredcarkitcontainer.h"
#include "trace.h"

// ========================= MEMBER FUNCTIONS ================================

// ---------------------------------------------------------------------------
// CGSAccWiredCarkitContainer::CGSAccWiredCarkitContainer()
// ---------------------------------------------------------------------------
//
CGSAccWiredCarkitContainer::CGSAccWiredCarkitContainer(
    CGSAccessoryPluginModel& aModel )
  : CGSAccBaseContainer( aModel )
    {
    FUNC_LOG;
    }


// ---------------------------------------------------------------------------
// CGSAccWiredCarkitContainer::~CGSAccWiredCarkitContainer()
// ---------------------------------------------------------------------------
//
CGSAccWiredCarkitContainer::~CGSAccWiredCarkitContainer()
    {
    FUNC_LOG;

    delete iLightModeItems;
    delete iAutoAnswerItems;
    }


// ---------------------------------------------------------------------------
// CGSAccWiredCarkitContainer::UpdateListBoxL()
// ---------------------------------------------------------------------------
//
void CGSAccWiredCarkitContainer::UpdateListBoxL( TInt aFeatureId )
    {
    FUNC_LOG;

    switch ( aFeatureId )
        {
        case KGSSettIdCKDP:
            RefreshDefaultProfileL();
            break;
        case KGSSettIdCKAA:
            RefreshAutoAnswerL();
            break;
        case KGSSettIdCKL:
            RefreshLightModeL();
            break;
        default:
            break;
        }

    iListBox->HandleItemAdditionL();
    }


// ---------------------------------------------------------------------------
// CGSAccWiredCarkitContainer::ConstructL()
// ---------------------------------------------------------------------------
//
void CGSAccWiredCarkitContainer::ConstructL( const TRect& aRect )
    {
    FUNC_LOG;

    CreateListBoxL();
    BaseConstructL( aRect, R_ACC_WIRED_CARKIT_TITLE, R_ACC_WIRED_CARKIT_LBX ); // Needs the listbox
    }


// ---------------------------------------------------------------------------
// CGSAccWiredCarkitContainer::ConstructListBoxL()
// ---------------------------------------------------------------------------
//
void CGSAccWiredCarkitContainer::ConstructListBoxL( TInt aResLbxId )
    {
    FUNC_LOG;

    CGSAccBaseContainer::ConstructListBoxL( aResLbxId );

    iAutoAnswerItems =
        iCoeEnv->ReadDesC16ArrayResourceL( R_ACC_AUTO_ANSWER_SETTING_PAGE_LBX );
    iLightModeItems =
        iCoeEnv->ReadDesC16ArrayResourceL( R_ACC_LIGHTS_SETTING_PAGE_LBX );

    RefreshDefaultProfileL();
    MakeItemVisibleL( KGSSettIdCKDP );

    RefreshAutoAnswerL();
    MakeItemVisibleL( KGSSettIdCKAA );

    RefreshLightModeL();
    MakeItemVisibleL( KGSSettIdCKL );
    }


// ---------------------------------------------------------------------------
// CGSAccWiredCarkitContainer::GetHelpContext() const
// ---------------------------------------------------------------------------
//
void CGSAccWiredCarkitContainer::GetHelpContext(
    TCoeHelpContext& aContext ) const
    {
    FUNC_LOG;

    aContext.iMajor = KUidGS;
    aContext.iContext = KSET_HLP_ACCESS_WIRED_CK;
    }


// ---------------------------------------------------------------------------
// CGSAccWiredCarkitContainer::RefreshDefaultProfileL()
// ---------------------------------------------------------------------------
//
void CGSAccWiredCarkitContainer::RefreshDefaultProfileL()
    {
    FUNC_LOG;

    TInt profile =
        iModel.DefaultProfileByKey( KSettingsCarKitDefaultProfile );
    // Set default profile item text.
    SetItemTextL(
        KGSSettIdCKDP,
        iModel.ProfileList().NameByIdL( profile, iCoeEnv ) );
    }


// ---------------------------------------------------------------------------
// CGSAccWiredCarkitContainer::RefreshAutoAnswerL()
// ---------------------------------------------------------------------------
//
void CGSAccWiredCarkitContainer::RefreshAutoAnswerL()
    {
    FUNC_LOG;

    TInt mode = iModel.AutoAnswerModeByKey( KSettingsCarKitAutomaticAnswer );
    if ( iAutoAnswerItems && mode >= 0 && mode < iAutoAnswerItems->Count() )
        {
        SetItemTextL( KGSSettIdCKAA, ( *iAutoAnswerItems )[ mode ] );
        }
    }


// ---------------------------------------------------------------------------
// CGSAccWiredCarkitContainer::RefreshLightModeL()
// ---------------------------------------------------------------------------
//
void CGSAccWiredCarkitContainer::RefreshLightModeL()
    {
    FUNC_LOG;

    TInt mode =
        iModel.LightMode( EAccModeWiredCarKit, KAccServerWiredCarKitLights );
    if ( iLightModeItems && mode >= 0 && mode < iLightModeItems->Count() )
        {
        SetItemTextL( KGSSettIdCKL, ( *iLightModeItems )[ mode ] );
        }
    }

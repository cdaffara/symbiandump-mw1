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
* Description:  Implementation of CGSAccTvoutContainer class
*
*/


#include <e32cmn.h> // For accessoriescrkeys.h
#include <accessoriescrkeys.h>
#include <gsaccessoryplugin.rsg>
#include <gsfwviewuids.h> // for KUidGS
#include <gslistbox.h>
#include <GSServerEngine.h>
#include <csxhelp/cp.hlp.hrh>

#include "gsaccessoryplugin.hrh"
#include "gsaccessorypluginmodel.h"
#include "gsaccprofilelist.h"
#include "gsacctvoutcontainer.h"
#include "trace.h"

// ========================= MEMBER FUNCTIONS ================================

// ---------------------------------------------------------------------------
// CGSAccTvoutContainer::CGSAccTvoutContainer()
//
// Default constructor
// ---------------------------------------------------------------------------
//
CGSAccTvoutContainer::CGSAccTvoutContainer(
    CGSAccessoryPluginModel& aModel,
    CGSServerEngine& aServerEngine )
  : CGSAccBaseContainer( aModel ),
    iServerEngine ( aServerEngine )
    {
    FUNC_LOG;
    }


// ---------------------------------------------------------------------------
// CGSAccTvoutContainer::~CGSAccTvoutContainer()
// ---------------------------------------------------------------------------
//
CGSAccTvoutContainer::~CGSAccTvoutContainer()
    {
    FUNC_LOG;

    delete iTvSystemItems;
    delete iAspectItems;
    delete iFlickerItems;
    }


// ---------------------------------------------------------------------------
// CGSAccTvoutContainer::UpdateListBoxL( TInt aFeatureId )
// ---------------------------------------------------------------------------
//
void CGSAccTvoutContainer::UpdateListBoxL( TInt aFeatureId )
    {
    FUNC_LOG;

    switch ( aFeatureId )
        {
        case EGSSettIdTvAspectRatio:
            RefreshAspectRatioL();
            break;
        case EGSSettIdTvSystem:
            RefreshTvSystemL();
            break;
        case EGSSettIdTvoutDP:
            RefreshDefaultProfileL();
            break;
        case EGSSettIdTvFlickerFilter:
            RefreshFlickerFilterL();
            break;
        default:
            break;
        }

    iListBox->HandleItemAdditionL();
    }


// ---------------------------------------------------------------------------
// CGSAccTvoutContainer::ConstructL()
// ---------------------------------------------------------------------------
//
void CGSAccTvoutContainer::ConstructL( const TRect& aRect )
    {
    FUNC_LOG;

    CreateListBoxL();
    BaseConstructL( aRect, R_ACC_TVOUT_TITLE, R_ACC_TVOUT_LBX ); // Needs the listbox
    }


// ---------------------------------------------------------------------------
// CGSAccTvoutContainer::ConstructListBoxL()
// ---------------------------------------------------------------------------
//
void CGSAccTvoutContainer::ConstructListBoxL( TInt aResLbxId )
    {
    FUNC_LOG;

    CGSAccBaseContainer::ConstructListBoxL( aResLbxId );

    RefreshDefaultProfileL();
    MakeItemVisibleL( EGSSettIdTvoutDP );

    iAspectItems =
        iCoeEnv->ReadDesC16ArrayResourceL( R_ACC_ASPECT_RATIO_SETTING_PAGE_LBX );
    iFlickerItems =
        iCoeEnv->ReadDesC16ArrayResourceL( R_ACC_FLICKER_FILTER_SETTING_PAGE_LBX );
    iTvSystemItems =
        iCoeEnv->ReadDesC16ArrayResourceL(
            iModel.PalmSupport() ?
                R_ACC_TV_SYSTEM_SETTING_PAGE_LBX :
                R_ACC_TV_SYSTEM_SETTING_PAGE_NO_PALM_LBX );

    RefreshAspectRatioL();
    MakeItemVisibleL( EGSSettIdTvAspectRatio );

    RefreshTvSystemL();
    MakeItemVisibleL( EGSSettIdTvSystem );

    RefreshFlickerFilterL();
    MakeItemVisibleL( EGSSettIdTvFlickerFilter );
    }


// ---------------------------------------------------------------------------
// CGSAccTvoutContainer::GetHelpContext() const
// ---------------------------------------------------------------------------
//
void CGSAccTvoutContainer::GetHelpContext( TCoeHelpContext& aContext ) const
    {
    FUNC_LOG;

    aContext.iMajor = KUidGS;
    aContext.iContext = KSET_HLP_ACCESSORIES_TVOUT;
    }


// ---------------------------------------------------------------------------
// CGSAccTvoutContainer::RefreshDefaultProfileL()
// ---------------------------------------------------------------------------
//
void CGSAccTvoutContainer::RefreshDefaultProfileL()
    {
    FUNC_LOG;

    TInt profile =
        iModel.DefaultProfileByKey( KSettingsTvOutDefaultProfile );
    // Set default profile item text.
    SetItemTextL(
        EGSSettIdTvoutDP,
        iModel.ProfileList().NameByIdL( profile, iCoeEnv ) );
    }


// ---------------------------------------------------------------------------
// CGSAccTvoutContainer::RefreshAspectRatioL()
// ---------------------------------------------------------------------------
//
void CGSAccTvoutContainer::RefreshAspectRatioL()
    {
    FUNC_LOG;

    TInt value = iServerEngine.AspectRatioL();
    if ( iAspectItems && value >= 0 && value < iAspectItems->Count() )
        {
        SetItemTextL( EGSSettIdTvAspectRatio, ( *iAspectItems )[ value ] );
        }
    }


// ---------------------------------------------------------------------------
// CGSAccTvoutContainer::RefreshTvSystemL()
// ---------------------------------------------------------------------------
//
void CGSAccTvoutContainer::RefreshTvSystemL()
    {
    FUNC_LOG;

    TInt value = iServerEngine.TvSystemL();

    // If PALM is not supported, index correction
    if ( !iModel.PalmSupport() && value )
        {
        value--;
        }

    if ( iTvSystemItems && value >= 0 && value < iTvSystemItems->Count() )
        {
        SetItemTextL( EGSSettIdTvSystem, ( *iTvSystemItems )[ value ] );
        }
    }


// ---------------------------------------------------------------------------
// CGSAccTvoutContainer::RefreshFlickerFilterL()
// ---------------------------------------------------------------------------

void CGSAccTvoutContainer::RefreshFlickerFilterL()
    {
    FUNC_LOG;

    TInt value = iServerEngine.FlickerFilterL();
    if ( iFlickerItems && value >= 0 && value < iFlickerItems->Count() )
        {
        SetItemTextL( EGSSettIdTvFlickerFilter, ( *iFlickerItems )[ value ] );
        }
    }

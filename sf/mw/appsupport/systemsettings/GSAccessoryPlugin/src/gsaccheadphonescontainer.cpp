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
* Description:  Implementation of CGSAccHeadphonesContainer class
*
*/


#include <e32cmn.h> // For accessoriescrkeys.h
#include <accessoriescrkeys.h>
#include <gsaccessoryplugin.rsg>
#include <gsfwviewuids.h> // for KUidGS
#include <gslistbox.h>
#include <csxhelp/cp.hlp.hrh>

#include "gsaccessoryplugin.hrh"
#include "gsaccessorypluginmodel.h"
#include "gsaccprofilelist.h"
#include "gsaccheadphonescontainer.h"
#include "trace.h"

// ========================= MEMBER FUNCTIONS ================================

// ---------------------------------------------------------------------------
// CGSAccHeadphonesContainer::CGSAccHeadphonesContainer()
// ---------------------------------------------------------------------------
//
CGSAccHeadphonesContainer::CGSAccHeadphonesContainer(
    CGSAccessoryPluginModel& aModel )
  : CGSAccBaseContainer( aModel )
    {
    FUNC_LOG;
    }


// ---------------------------------------------------------------------------
// CGSAccHeadphonesContainer::~CGSAccHeadphonesContainer()
// ---------------------------------------------------------------------------
//
CGSAccHeadphonesContainer::~CGSAccHeadphonesContainer()
    {
    FUNC_LOG;
    }


// ---------------------------------------------------------------------------
// CGSAccHeadphonesContainer::UpdateListBoxL( TInt aFeatureId )
// ---------------------------------------------------------------------------
//
void CGSAccHeadphonesContainer::UpdateListBoxL( TInt aFeatureId )
    {
    FUNC_LOG;

    if ( aFeatureId == KGSSettIdHPDP )
        {
        RefreshDefaultProfileL();
        }

    iListBox->HandleItemAdditionL();
    }


// ---------------------------------------------------------------------------
// CGSAccHeadphonesContainer::ConstructL()
// ---------------------------------------------------------------------------
//
void CGSAccHeadphonesContainer::ConstructL( const TRect& aRect )
    {
    FUNC_LOG;

    CreateListBoxL();
    BaseConstructL( aRect, R_ACC_HEADPHONES_TITLE, R_ACC_HEADPHONES_LBX ); // Needs the listbox
    }


// ---------------------------------------------------------------------------
// CGSAccHeadphonesContainer::ConstructListBoxL()
// ---------------------------------------------------------------------------
//
void CGSAccHeadphonesContainer::ConstructListBoxL( TInt aResLbxId )
    {
    FUNC_LOG;

    CGSAccBaseContainer::ConstructListBoxL( aResLbxId );

    RefreshDefaultProfileL();
    MakeItemVisibleL( KGSSettIdHPDP );
    }


// ---------------------------------------------------------------------------
// CGSAccHeadphonesContainer::GetHelpContext() const
// ---------------------------------------------------------------------------
//
void CGSAccHeadphonesContainer::GetHelpContext( TCoeHelpContext& aContext ) const
    {
    aContext.iMajor = KUidGS;
    aContext.iContext = KSET_HLP_ACCESSORIES_HEADPHONE;
    }


// ---------------------------------------------------------------------------
// CGSAccHeadphonesContainer::RefreshDefaultProfileL()
// ---------------------------------------------------------------------------
//
void CGSAccHeadphonesContainer::RefreshDefaultProfileL()
    {
    FUNC_LOG;

    TInt profile =
        iModel.DefaultProfileByKey( KSettingsHeadphonesDefaultProfile );
    // Set default profile item text.
    SetItemTextL(
        KGSSettIdHPDP,
        iModel.ProfileList().NameByIdL( profile, iCoeEnv ) );
    }

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
* Description:  Implementation of CGSAccTextPhoneContainer class
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
#include "gsacctextphonecontainer.h"
#include "trace.h"

// ========================= MEMBER FUNCTIONS ================================

// ---------------------------------------------------------------------------
// CGSAccTextPhoneContainer::CGSAccTextPhoneContainer()
// ---------------------------------------------------------------------------
//
CGSAccTextPhoneContainer::CGSAccTextPhoneContainer(
    CGSAccessoryPluginModel& aModel )
  : CGSAccBaseContainer( aModel )
    {
    FUNC_LOG;
    }


// ---------------------------------------------------------------------------
// CGSAccTextPhoneContainer::~CGSAccTextPhoneContainer()
// ---------------------------------------------------------------------------
//
CGSAccTextPhoneContainer::~CGSAccTextPhoneContainer()
    {
    FUNC_LOG;
    }


// ---------------------------------------------------------------------------
// CGSAccTextPhoneContainer::UpdateListBoxL()
// ---------------------------------------------------------------------------
//
void CGSAccTextPhoneContainer::UpdateListBoxL( TInt aFeatureId )
    {
    FUNC_LOG;

    if ( aFeatureId == KGSSettIdTextPhoneDP )
        {
        RefreshDefaultProfileL();
        }

    iListBox->HandleItemAdditionL();
    }


// ---------------------------------------------------------------------------
// CGSAccTextPhoneContainer::ConstructL()
// ---------------------------------------------------------------------------
//
void CGSAccTextPhoneContainer::ConstructL( const TRect& aRect )
    {
    FUNC_LOG;

    CreateListBoxL();
    BaseConstructL( aRect, R_ACC_TTY_TITLE, R_ACC_TTY_LBX ); // Needs the listbox
    }


// ---------------------------------------------------------------------------
// CGSAccTextPhoneContainer::ConstructListBoxL()
// ---------------------------------------------------------------------------
//
void CGSAccTextPhoneContainer::ConstructListBoxL( TInt aResLbxId )
    {
    FUNC_LOG;

    CGSAccBaseContainer::ConstructListBoxL( aResLbxId );

    RefreshDefaultProfileL();
    MakeItemVisibleL( KGSSettIdTextPhoneDP );
    }


// ---------------------------------------------------------------------------
// CGSAccTextPhoneContainer::GetHelpContext() const
// ---------------------------------------------------------------------------
//
void CGSAccTextPhoneContainer::GetHelpContext(
    TCoeHelpContext& aContext ) const
    {
    FUNC_LOG;

    aContext.iMajor = KUidGS;
    aContext.iContext = KSET_HLP_ACCESSORIES_TTY;
    }


// ---------------------------------------------------------------------------
// CGSAccTextPhoneContainer::RefreshDefaultProfileL()
// ---------------------------------------------------------------------------
//
void CGSAccTextPhoneContainer::RefreshDefaultProfileL()
    {
    FUNC_LOG;

    TInt profile =
        iModel.DefaultProfileByKey( KSettingsTTYDefaultProfile );
    // Set default profile item text.
    SetItemTextL(
        KGSSettIdTextPhoneDP,
        iModel.ProfileList().NameByIdL( profile, iCoeEnv ) );
    }

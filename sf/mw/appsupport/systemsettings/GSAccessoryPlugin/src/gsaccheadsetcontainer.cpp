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
* Description:  Implementation of CGSAccHeadsetContainer class
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
#include "gsaccheadsetcontainer.h"
#include "gsaccprofilelist.h"
#include "trace.h"

// ========================= MEMBER FUNCTIONS ================================

// ---------------------------------------------------------------------------
// CGSAccHeadsetContainer::CGSAccHeadsetContainer()
// ---------------------------------------------------------------------------
//
CGSAccHeadsetContainer::CGSAccHeadsetContainer(
    CGSAccessoryPluginModel& aModel )
  : CGSAccBaseContainer( aModel )
    {
    FUNC_LOG;
    }


// ---------------------------------------------------------------------------
// CGSAccHeadsetContainer::~CGSAccHeadsetContainer()
// ---------------------------------------------------------------------------
//
CGSAccHeadsetContainer::~CGSAccHeadsetContainer()
    {
    FUNC_LOG;

    delete iAutoAnswerItems;
    }


// ---------------------------------------------------------------------------
// CGSAccHeadsetContainer::UpdateListBoxL( TInt aFeatureId )
// ---------------------------------------------------------------------------
//
void CGSAccHeadsetContainer::UpdateListBoxL( TInt aFeatureId )
    {
    FUNC_LOG;

    switch( aFeatureId )
        {
        case KGSSettIdHSDP:
            RefreshDefaultProfileL();
            break;
        case KGSSettIdHSAA:
            RefreshAutoAnswerL();
            break;
        default:
            break;
        }

    iListBox->HandleItemAdditionL();
    }


// ---------------------------------------------------------------------------
// CGSAccHeadsetContainer::ConstructL()
// ---------------------------------------------------------------------------
//
void CGSAccHeadsetContainer::ConstructL( const TRect& aRect )
    {
    FUNC_LOG;

    CreateListBoxL();
    BaseConstructL( aRect, R_ACC_HEADSET_TITLE, R_ACC_HEADSET_LBX ); // Needs the listbox
    }


// ---------------------------------------------------------------------------
// CGSAccHeadsetContainer::ConstructListBoxL()
// ---------------------------------------------------------------------------
//
void CGSAccHeadsetContainer::ConstructListBoxL( TInt aResLbxId )
    {
    FUNC_LOG;

    CGSAccBaseContainer::ConstructListBoxL( aResLbxId );

    iAutoAnswerItems =
        iCoeEnv->ReadDesC16ArrayResourceL( R_ACC_AUTO_ANSWER_SETTING_PAGE_LBX );

    RefreshDefaultProfileL();
    MakeItemVisibleL( KGSSettIdHSDP );

    RefreshAutoAnswerL();
    MakeItemVisibleL( KGSSettIdHSAA );
    }


// ---------------------------------------------------------------------------
// CGSAccHeadsetContainer::GetHelpContext() const
// ---------------------------------------------------------------------------
//
void CGSAccHeadsetContainer::GetHelpContext( TCoeHelpContext& aContext ) const
    {
    FUNC_LOG;

    aContext.iMajor = KUidGS;
    aContext.iContext = KSET_HLP_ACCESSORIES_HEADSET;
    }


// ---------------------------------------------------------------------------
// CGSAccHeadsetContainer::RefreshDefaultProfileL()
// ---------------------------------------------------------------------------
//
void CGSAccHeadsetContainer::RefreshDefaultProfileL()
    {
    FUNC_LOG;

    TInt profile =
        iModel.DefaultProfileByKey( KSettingsHeadsetDefaultProfile );
    // Set default profile item text.
    SetItemTextL(
        KGSSettIdHSDP,
        iModel.ProfileList().NameByIdL( profile, iCoeEnv ) );
    }


// ---------------------------------------------------------------------------
// CGSAccHeadsetContainer::RefreshAutoAnswerL()
// ---------------------------------------------------------------------------
//
void CGSAccHeadsetContainer::RefreshAutoAnswerL()
    {
    FUNC_LOG;

    TInt mode = iModel.AutoAnswerModeByKey( KSettingsHeadsetAutomaticAnswer );
    if ( iAutoAnswerItems && mode >= 0 && mode < iAutoAnswerItems->Count() )
        {
        SetItemTextL( KGSSettIdHSAA, ( *iAutoAnswerItems )[ mode ] );
        }
    }

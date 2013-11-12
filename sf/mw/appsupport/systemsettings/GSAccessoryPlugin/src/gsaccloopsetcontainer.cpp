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
* Description:  Implementation of CGSAccLoopsetContainer class
*
*/


#include <e32cmn.h> // For accessoriescrkeys.h
#include <accessoriescrkeys.h>
#include <gsaccessoryplugin.rsg>
#include <gsfwviewuids.h> // for KUidGS
#include <csxhelp/cp.hlp.hrh>

#include "gsaccessoryplugin.hrh"
#include "gsaccessorypluginmodel.h"
#include "gsaccloopsetcontainer.h"
#include "gsaccprofilelist.h"
#include "trace.h"

// ========================= MEMBER FUNCTIONS ================================

// ---------------------------------------------------------------------------
// CGSAccLoopsetContainer::CGSAccLoopsetContainer()
// ---------------------------------------------------------------------------
//
CGSAccLoopsetContainer::CGSAccLoopsetContainer(
    CGSAccessoryPluginModel& aModel )
  : CGSAccBaseContainer( aModel )
    {
    FUNC_LOG;
    }


// ---------------------------------------------------------------------------
// CGSAccLoopsetContainer::~CGSAccLoopsetContainer()
// ---------------------------------------------------------------------------
//
CGSAccLoopsetContainer::~CGSAccLoopsetContainer()
    {
    FUNC_LOG;

    delete iAutoAnswerItems;
    }


// ---------------------------------------------------------------------------
// CGSAccLoopsetContainer::UpdateListBoxL()
// ---------------------------------------------------------------------------
//
void CGSAccLoopsetContainer::UpdateListBoxL( TInt aFeatureId )
    {
    FUNC_LOG;

    switch( aFeatureId )
        {
        case KGSSettIdLSDP:
            RefreshDefaultProfileL();
            break;
        case KGSSettIdLSAA:
            RefreshAutoAnswerL();
            break;
        default:
            break;
        }

    iListBox->HandleItemAdditionL();
    }


// ---------------------------------------------------------------------------
// CGSAccLoopsetContainer::ConstructL()
// ---------------------------------------------------------------------------
//
void CGSAccLoopsetContainer::ConstructL( const TRect& aRect )
    {
    FUNC_LOG;

    CreateListBoxL();
    BaseConstructL( aRect, R_ACC_LOOPSET_TITLE, R_ACC_LOOPSET_LBX ); // Needs the listbox
    }


// ---------------------------------------------------------------------------
// CGSAccLoopsetContainer::ConstructListBoxL()
// ---------------------------------------------------------------------------
//
void CGSAccLoopsetContainer::ConstructListBoxL( TInt aResLbxId )
    {
    FUNC_LOG;

    CGSAccBaseContainer::ConstructListBoxL( aResLbxId );

    iAutoAnswerItems =
        iCoeEnv->ReadDesC16ArrayResourceL( R_ACC_AUTO_ANSWER_SETTING_PAGE_LBX );

    RefreshDefaultProfileL();
    MakeItemVisibleL( KGSSettIdLSDP );

    RefreshAutoAnswerL();
    MakeItemVisibleL( KGSSettIdHSAA );
    }


// ---------------------------------------------------------------------------
// CGSAccLoopsetContainer::GetHelpContext() const
// ---------------------------------------------------------------------------
//
void CGSAccLoopsetContainer::GetHelpContext( TCoeHelpContext& aContext ) const
    {
    FUNC_LOG;

    aContext.iMajor = KUidGS;
    aContext.iContext = KSET_HLP_ACCESSORIES_LOOPSET;
    }


// ---------------------------------------------------------------------------
// CGSAccLoopsetContainer::RefreshDefaultProfileL()
// ---------------------------------------------------------------------------
//
void CGSAccLoopsetContainer::RefreshDefaultProfileL()
    {
    FUNC_LOG;

    TInt profile =
        iModel.DefaultProfileByKey( KSettingsLoopsetDefaultProfile );
    // Set default profile item text.
    SetItemTextL(
        KGSSettIdLSDP,
        iModel.ProfileList().NameByIdL( profile, iCoeEnv ) );
    }


// ---------------------------------------------------------------------------
// CGSAccLoopsetContainer::RefreshAutoAnswerL()
// ---------------------------------------------------------------------------
//
void CGSAccLoopsetContainer::RefreshAutoAnswerL()
    {
    FUNC_LOG;

    TInt mode = iModel.AutoAnswerModeByKey( KSettingsLoopsetAutomaticAnswer );
    if ( iAutoAnswerItems && mode >= 0 && mode < iAutoAnswerItems->Count() )
        {
        SetItemTextL( KGSSettIdLSAA, ( *iAutoAnswerItems )[ mode ] );
        }
    }

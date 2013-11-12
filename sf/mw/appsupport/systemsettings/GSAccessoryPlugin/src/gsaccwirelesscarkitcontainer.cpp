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
* Description:  Implementation of CGSAccWirelessCarkitContainer class
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
#include "gsaccwirelesscarkitcontainer.h"
#include "trace.h"

// ========================= MEMBER FUNCTIONS ================================

// ---------------------------------------------------------------------------
// CGSAccWirelessCarkitContainer::CGSAccWirelessCarkitContainer()
// ---------------------------------------------------------------------------
//
CGSAccWirelessCarkitContainer::CGSAccWirelessCarkitContainer(
    CGSAccessoryPluginModel& aModel )
  : CGSAccBaseContainer( aModel )
    {
    FUNC_LOG;
    }


// ---------------------------------------------------------------------------
// CGSAccWirelessCarkitContainer::~CGSAccWirelessCarkitContainer()
// ---------------------------------------------------------------------------
//
CGSAccWirelessCarkitContainer::~CGSAccWirelessCarkitContainer()
    {
    FUNC_LOG;

    delete iAutoAnswerItems;
    }


// ---------------------------------------------------------------------------
// CGSAccWirelessCarkitContainer::UpdateListBoxL()
// ---------------------------------------------------------------------------
//
void CGSAccWirelessCarkitContainer::UpdateListBoxL( TInt aFeatureId )
    {
    FUNC_LOG;

    switch ( aFeatureId )
        {
        case KGSSettIdWCKDP:
            RefreshDefaultProfileL();
            break;
        case KGSSettIdWCKAA:
            RefreshAutoAnswerL();
            break;
        default:
            break;
        }

    iListBox->HandleItemAdditionL();
    }


// ---------------------------------------------------------------------------
// CGSAccWirelessCarkitContainer::ConstructL()
// ---------------------------------------------------------------------------
//
void CGSAccWirelessCarkitContainer::ConstructL( const TRect& aRect )
    {
    FUNC_LOG;

    CreateListBoxL();
    BaseConstructL(
        aRect, R_ACC_WIRELESS_CARKIT_TITLE, R_ACC_WIRELESS_CARKIT_LBX ); // Needs the listbox
    }


// ---------------------------------------------------------------------------
// CGSAccWirelessCarkitContainer::ConstructListBoxL()
// ---------------------------------------------------------------------------
//
void CGSAccWirelessCarkitContainer::ConstructListBoxL( TInt aResLbxId )
    {
    FUNC_LOG;

    CGSAccBaseContainer::ConstructListBoxL( aResLbxId );

    iAutoAnswerItems =
        iCoeEnv->ReadDesC16ArrayResourceL( R_ACC_AUTO_ANSWER_SETTING_PAGE_LBX );

    RefreshDefaultProfileL();
    MakeItemVisibleL( KGSSettIdWCKDP );

    RefreshAutoAnswerL();
    MakeItemVisibleL( KGSSettIdWCKAA );
    }


// ---------------------------------------------------------------------------
// CGSAccWirelessCarkitContainer::GetHelpContext() const
// ---------------------------------------------------------------------------
//
void CGSAccWirelessCarkitContainer::GetHelpContext(
    TCoeHelpContext& aContext ) const
    {
    FUNC_LOG;

    aContext.iMajor = KUidGS;
    aContext.iContext = KSET_HLP_ACCESS_WIRELESS_CK;
    }


// ---------------------------------------------------------------------------
// CGSAccWirelessCarkitContainer::RefreshDefaultProfileL()
// ---------------------------------------------------------------------------
//
void CGSAccWirelessCarkitContainer::RefreshDefaultProfileL()
    {
    FUNC_LOG;

    TInt profile =
        iModel.DefaultProfileByKey( KSettingsWirelessCarkitDefaultProfile );
    // Set default profile item text.
    SetItemTextL(
        KGSSettIdWCKDP,
        iModel.ProfileList().NameByIdL( profile, iCoeEnv ) );
    }


// ---------------------------------------------------------------------------
// CGSAccWirelessCarkitContainer::RefreshAutoAnswerL()
// ---------------------------------------------------------------------------
//
void CGSAccWirelessCarkitContainer::RefreshAutoAnswerL()
    {
    FUNC_LOG;

    TInt mode =
        iModel.AutoAnswerModeByKey( KSettingsWirelessCarkitAutomaticAnswer );
    if ( iAutoAnswerItems && mode >= 0 && mode < iAutoAnswerItems->Count() )
        {
        SetItemTextL( KGSSettIdWCKAA, ( *iAutoAnswerItems )[ mode ] );
        }
    }

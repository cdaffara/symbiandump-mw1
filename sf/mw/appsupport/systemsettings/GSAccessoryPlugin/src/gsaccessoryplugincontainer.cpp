/*
* Copyright (c) 2005-2009 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Implementation of CGSAccessoryPluginContainer class
*
*/


#include <aknlists.h>
#include <gsaccessoryplugin.rsg>
#include <gsfwviewuids.h> // for KUidGS
#include <gslistbox.h>
#include <csxhelp/cp.hlp.hrh>
#include <StringLoader.h>

#include "gsaccessoryplugin.hrh"
#include "gsaccessorypluginmodel.h"
#include "gsaccessoryplugincontainer.h"
#include "trace.h"

// ========================== MEMBER FUNCTIONS ===============================

// ---------------------------------------------------------------------------
// CGSAccessoryPluginContainer::CGSAccessoryPluginContainer()
// ---------------------------------------------------------------------------
//
CGSAccessoryPluginContainer::CGSAccessoryPluginContainer(
    CGSAccessoryPluginModel& aModel )
  : iModel( aModel )
    {
    FUNC_LOG;
    }


// ---------------------------------------------------------------------------
// CGSAccessoryPluginContainer::~CGSAccessoryPluginContainer()
// ---------------------------------------------------------------------------
//
CGSAccessoryPluginContainer::~CGSAccessoryPluginContainer()
    {
    FUNC_LOG;

    delete iListboxItemArray;
    delete iDefaultText;
    }


// ---------------------------------------------------------------------------
// CGSAccessoryPluginContainer::CurrentFeatureId()
// ---------------------------------------------------------------------------
//
TInt CGSAccessoryPluginContainer::CurrentFeatureId() const
    {
    FUNC_LOG;

    return iListboxItemArray->CurrentFeature();
    }


// ---------------------------------------------------------------------------
// CGSAccessoryPluginContainer::ChangeDefaultItemL()
// ---------------------------------------------------------------------------
//
void CGSAccessoryPluginContainer::ChangeDefaultItemL(
    const TInt aNewDefaultItem,
    const TInt aOldDefaultItem )
    {
    FUNC_LOG;

    INFO_2( "Default accessory changed from %d to %d", aOldDefaultItem, aNewDefaultItem );

    ShowLabelL( aOldDefaultItem, KNullDesC );
    ShowLabelL( aNewDefaultItem, *iDefaultText );
    }


// ---------------------------------------------------------------------------
// CGSAccessoryPluginContainer::ConstructL()
// ---------------------------------------------------------------------------
//
void CGSAccessoryPluginContainer::ConstructL( const TRect& aRect )
    {
    FUNC_LOG;

    iListBox = new ( ELeave ) CAknDoubleStyleListBox;
    BaseConstructL( aRect, R_ACC_MAIN_VIEW_TITLE, R_ACC_MAIN_VIEW_LBX ); // Needs the listbox
    }


// ---------------------------------------------------------------------------
// CGSAccessoryPluginContainer::ConstructListBoxL()
// ---------------------------------------------------------------------------
//
void CGSAccessoryPluginContainer::ConstructListBoxL( TInt aResLbxId )
    {
    FUNC_LOG;

    iListBox->ConstructL( this, EAknListBoxSelectionList );
    iListboxItemArray = CGSListBoxItemTextArray::NewL(
        aResLbxId, *iListBox, *iCoeEnv );
    iListBox->Model()->SetItemTextArray( iListboxItemArray );
    iListBox->Model()->SetOwnershipType( ELbmDoesNotOwnItemArray );

    MakeVisibleIfSupportedL( EGSHeadset, EGSHeadsetListItemId );
    MakeVisibleIfSupportedL( EGSHeadphones, EGSHeadphonesListItemId );
    MakeVisibleIfSupportedL( EGSMusicStand, EGSMusicStandListItemId );
    MakeVisibleIfSupportedL( EGSTextPhone, EGSTextPhoneListItemId );
    MakeVisibleIfSupportedL( EGSTvout, EGSTvOutListItemId );
    MakeVisibleIfSupportedL( EGSWiredCarkit, EGSWiredCarKitListItemId );
    MakeVisibleIfSupportedL( EGSWirelessCarkit, EGSWirelessCarKitListItemId );
    MakeVisibleIfSupportedL( EGSLoopset, EGSLoopsetListItemId );
    MakeVisibleIfSupportedL( EGSHac, EGSHACListItemId );

    iDefaultText = StringLoader::LoadL( R_ACC_DEFAULT_ACCESSORY );
    ShowLabelL( iModel.DefaultAccessory(), *iDefaultText );
    }


// ---------------------------------------------------------------------------
// CGSAccessoryPluginContainer::GetHelpContext() const
// ---------------------------------------------------------------------------
//
void CGSAccessoryPluginContainer::GetHelpContext(
    TCoeHelpContext& aContext ) const
    {
    FUNC_LOG;

    aContext.iMajor = KUidGS;
    aContext.iContext = KSET_HLP_ACCESSORIES_LIST;
    }


// ---------------------------------------------------------------------------
// CGSAccessoryPluginContainer::MakeVisibleIfSupportedL()
//
// ---------------------------------------------------------------------------
//
void CGSAccessoryPluginContainer::MakeVisibleIfSupportedL(
    TInt aAccessory,
    TInt aItemId )
    {
    FUNC_LOG;

    if ( iModel.AccessorySupportedL( aAccessory ) )
        {
        iListboxItemArray->SetItemVisibilityL(
            aItemId, CGSListBoxItemTextArray::EVisible );
        }
    }


// ---------------------------------------------------------------------------
// CGSAccessoryPluginContainer::ShowLabelL()
// ---------------------------------------------------------------------------
//
void CGSAccessoryPluginContainer::ShowLabelL(
    TInt aFeatureId,
    const TDesC& aLabel )
    {
    FUNC_LOG;

    iListboxItemArray->SetDynamicTextDoublePaneL( aFeatureId, aLabel );
    }

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
* Description:  Implementation of CGSAccessoryPluginModel class
*
*/


#include <centralrepository.h>
#include <featmgr.h>
#include <HWRMDomainCRKeys.h> // HAC feature
#include <ProfileEngineDomainCRKeys.h> // HAC feature
#include <accessoriescrkeys.h>

#include "gsaccessoryplugin.hrh"
#include "gsaccessorypluginconst.hrh"
#include "gsaccessorypluginmodel.h"
#include "gsaccprofilelist.h"
#include "trace.h"

// ========================= MEMBER FUNCTIONS ================================

// ---------------------------------------------------------------------------
// CGSAccessoryPluginModel::NewL
// ---------------------------------------------------------------------------
//
CGSAccessoryPluginModel* CGSAccessoryPluginModel::NewL()
    {
    CGSAccessoryPluginModel* self = new( ELeave ) CGSAccessoryPluginModel;
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );
    return self;
    }


// ---------------------------------------------------------------------------
// CGSAccessoryPluginModel::~CGSAccessoryPluginModel
// ---------------------------------------------------------------------------
//
CGSAccessoryPluginModel::~CGSAccessoryPluginModel()
    {
    FUNC_LOG;

    delete iProfileList;
    delete iAccessoryRepository;
    delete iHwSettingsRepository;
    delete iProfVariationRepository;
    delete iProfileRepository;
    iAccessorySettings.CloseSubSession();
    iAccServer.Disconnect();
    }


// ---------------------------------------------------------------------------
// CGSAccessoryPluginModel::AccessorySupportedL
// ---------------------------------------------------------------------------
//
TBool CGSAccessoryPluginModel::AccessorySupportedL( TInt aAccessory ) const
    {
    FUNC_LOG;

    TBool supported( EFalse );
    switch ( aAccessory )
        {
        case EGSHeadset:    // Fall through
        case EGSHeadphones: // Fall through
        case EGSMusicStand:
            supported = ETrue;
            break;
        case EGSWiredCarkit:
            User::LeaveIfError( iAccessoryRepository->Get(
                KSettingsAccWiredCarkitSupported, supported ) );
            break;
        case EGSWirelessCarkit:
            User::LeaveIfError( iAccessoryRepository->Get(
                KSettingsAccWirelessCarkitSupported, supported ) );
            break;
        case EGSTvout:
            supported = FeatureManager::FeatureSupported( KFeatureIdTvOut );
            break;
        case EGSLoopset:
            User::LeaveIfError( iAccessoryRepository->Get(
                KSettingsAccLoopsetSupported, supported ) );
            break;
        case EGSTextPhone:
            supported = FeatureManager::FeatureSupported( KFeatureIdPhoneTty );
            break;
        case EGSHac:
            {
            TInt localFeatures( 0 );
            User::LeaveIfError( iProfVariationRepository->Get(
                KProEngTonesViewLVFlags, localFeatures ) );
            supported = localFeatures & KTonesViewFeatureIdHac;
            }
            break;
        default:
            break;
        }

    INFO_2( "Accessory %d support status = %d", aAccessory, supported );
    return supported;
    }


// ---------------------------------------------------------------------------
// CGSAccessoryPluginModel::DefaultAccessory
// ---------------------------------------------------------------------------
//
TInt CGSAccessoryPluginModel::DefaultAccessory() const
    {
    FUNC_LOG;

    TUint32 value( 0 );
    TInt error = iAccessorySettings.GetHWDeviceSettings( value );
    ERROR( error, "Failed to get default accessory" );
    return ( error == KErrNone ? value : error );
    }


// ---------------------------------------------------------------------------
// CGSAccessoryPluginModel::SetDefaultAccessory()
// ---------------------------------------------------------------------------
//
void CGSAccessoryPluginModel::SetDefaultAccessoryL( TInt aValue )
    {
    FUNC_LOG;

    iAccessorySettings.SetHWDeviceSettingsL( aValue );
    }


// ---------------------------------------------------------------------------
// CGSAccessoryPluginModel::DefaultProfileByKey
// ---------------------------------------------------------------------------
//
TInt CGSAccessoryPluginModel::DefaultProfileByKey( TInt aKey ) const
    {
    FUNC_LOG;

    // Feature flagging should be checked in the UI for configurable accessories.
    TInt profile( 0 );
    TInt error = iProfileRepository->Get( aKey, profile );
    ERROR_1( error, "Failed to read key %d value from profile repository", aKey );
    INFO_2( "Default profile key %d value is %d", aKey, profile );
    return ( error == KErrNone ? profile : error );
    }


// ---------------------------------------------------------------------------
// CGSAccessoryPluginModel::SetDefaultProfileByKeyL
// ---------------------------------------------------------------------------
//
void CGSAccessoryPluginModel::SetDefaultProfileByKeyL(
    TInt aKey,
    TInt aNewProfile )
    {
    FUNC_LOG;

    User::LeaveIfError( iProfileRepository->Set( aKey, aNewProfile ) );
    INFO_2( "Default profile key %d set to %d", aKey, aNewProfile );
    }


// ---------------------------------------------------------------------------
// CGSAccessoryPluginModel::AutoAnswerModeByKey
// ---------------------------------------------------------------------------
//
TInt CGSAccessoryPluginModel::AutoAnswerModeByKey( TInt aKey ) const
    {
    FUNC_LOG;

    TInt mode;
    TInt error = iAccessoryRepository->Get( aKey, mode );
    ERROR_1( error, "Failed to read key %d value from accessory repository", aKey );
    INFO_2( "Auto answer mode key %d value is %d", aKey, mode );
    if ( error == KErrNone )
        {
        return ( mode == 1 ? KIndexOn : KIndexOff ); // Map to list box indexes
        }
    return error;
    }


// ---------------------------------------------------------------------------
// CGSAccessoryPluginModel::SetAutoAnswerModeByKeyL
// ---------------------------------------------------------------------------
//
void CGSAccessoryPluginModel::SetAutoAnswerModeByKeyL(
    TInt aKey,
    TInt aMode ) const
    {
    FUNC_LOG;

    TInt val = ( aMode == KIndexOn ? 1 : 0 );
    INFO_1( "Setting automatic answer mode to %d", val );
    TInt error = iAccessoryRepository->Set( aKey, val );
    ERROR( error, "Failed to set automatic answer mode" );
    User::LeaveIfError( error );
    }


// ---------------------------------------------------------------------------
// CGSAccessoryPluginModel::LightModeL
// ---------------------------------------------------------------------------
//
TInt CGSAccessoryPluginModel::LightMode(
    TAccMode aAccMode,
    TInt aAccSetId ) const
    {
    FUNC_LOG;

    TAccModeSetting setting;
    setting.SetId( aAccSetId );

    TInt error = iAccessorySettings.GetAccessoryModeSetting( aAccMode, setting );
    ERROR_2( error, "Failed to read mode for accessory %d setting %d", aAccMode, aAccSetId );

    TInt mode( KErrNone );
    setting.GetTInt( mode );

    if ( error == KErrNone )
        {
        return ( mode == EAccLightsOn ? KIndexOn : KIndexAuto ); // Map to list box indexes
        }
    return error;
    }


// ---------------------------------------------------------------------------
// CGSAccessoryPluginModel::SetLightModeL
// ---------------------------------------------------------------------------
//
void CGSAccessoryPluginModel::SetLightModeL(
    TAccMode aAccMode,
    TInt aAccSetId,
    TInt aLightMode )
    {
    FUNC_LOG;

    TAccModeSetting setting;
    setting.SetId( aAccSetId );
    TInt val = ( aLightMode == KIndexOn ? EAccLightsOn : EAccLightsAutomatic );
    INFO_3( "Setting light mode for accessry %d setting %d to %d", aAccMode, aAccSetId, val );
    setting.SetTInt( val );
    iAccessorySettings.SetAccessoryModeSettingL( aAccMode, setting );
    }


// ---------------------------------------------------------------------------
// CGSAccessoryPluginModel::HacMode()
// ---------------------------------------------------------------------------
//
TInt CGSAccessoryPluginModel::HacMode()
    {
    FUNC_LOG;

    TInt mode;
    TInt error = iHwSettingsRepository->Get( KHWSettingsHearingAidMode, mode );
    ERROR( error, "Failed to read HAC mode" );
    INFO_1( "HAC mode = %d", mode );
    if ( error == KErrNone )
        {
        return ( mode == 1 ? KIndexOn : KIndexOff ); // Map to list box indexes
        }
    return error;
    }


// ---------------------------------------------------------------------------
// CGSAccessoryPluginModel::SetHACModeL()
// ---------------------------------------------------------------------------
//
void CGSAccessoryPluginModel::SetHacModeL( TInt aMode )
    {
    FUNC_LOG;

    TInt val = ( aMode == KIndexOn ? 1 : 0 );
    INFO_1( "Setting HAC mode to %d", val );
    TInt error = iHwSettingsRepository->Set( KHWSettingsHearingAidMode, val );
    ERROR( error, "Failed to set HAC mode" );
    User::LeaveIfError( error );
    }


// ---------------------------------------------------------------------------
// CGSAccessoryPluginModel::PalmSupport
// ---------------------------------------------------------------------------
//
TBool CGSAccessoryPluginModel::PalmSupport()
    {
    return iPalmSupport;
    }


// ---------------------------------------------------------------------------
// CGSAccessoryPluginModel::ProfileList
// ---------------------------------------------------------------------------
//
CGSAccProfileList& CGSAccessoryPluginModel::ProfileList() const
    {
    return *iProfileList;
    }


// ---------------------------------------------------------------------------
// CGSAccessoryPluginModel::FlipValue
// ---------------------------------------------------------------------------
//
void CGSAccessoryPluginModel::FlipValue( TInt& aValue )
    {
    FUNC_LOG;

    if ( aValue == 0 )
        {
        aValue = 1;
        }
    else
        {
        aValue = 0;
        }
    }


// ---------------------------------------------------------------------------
// CGSAccessoryPluginModel::CGSAccessoryPluginModel
// ---------------------------------------------------------------------------
//
CGSAccessoryPluginModel::CGSAccessoryPluginModel()
    {
    FUNC_LOG;
    }


// ---------------------------------------------------------------------------
// CGSAccessoryPluginModel::ConstructL
// ---------------------------------------------------------------------------
//
void CGSAccessoryPluginModel::ConstructL()
    {
    FUNC_LOG;

    iAccessoryRepository = CRepository::NewL( KCRUidAccessorySettings );
    iProfileRepository = CRepository::NewL( KCRUidProfileSettings );
    iHwSettingsRepository = CRepository::NewL( KCRUidHWSettings );
    iProfVariationRepository = CRepository::NewL( KCRUidProfilesLV );
    iPalmSupport = EFalse;

    CRepository* tvRepository = CRepository::NewL( KCRUidTvoutSettings );
    CleanupStack::PushL( tvRepository );
    tvRepository->Get( KSettingsTvoutPalmVisibility, iPalmSupport ); // Ignore error
    CleanupStack::PopAndDestroy( tvRepository );

    iProfileList = CGSAccProfileList::NewL();

    // connect to Accessory server
    TInt error = iAccServer.Connect();
    ERROR( error, "Failed to connect to Accessory Server" );
    if ( !error )
        {
        // create a session to accessory settings
        error = iAccessorySettings.CreateSubSession( iAccServer );
        ERROR( error, "Failed to create Accessory Settings subsession" );
        }

    User::LeaveIfError( error );
    }


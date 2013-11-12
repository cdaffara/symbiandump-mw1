/*
* Copyright (c) 2002 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Implementation of the CProfileEngineImpl.
*
*/



// INCLUDE FILES
#include "CProfileEngineImpl.h"
#include "CProfilesNamesArrayImpl.h"
#include "CProfileImpl.h"
#include "CProfileNameImpl.h"
#include "ProfileEngUtils.h"
#include "ProfileEngPanic.h"
#include "CProfileTiming.h"
#include "ProfileEnginePrivateCRKeys.h"
#include "ProfilesDebug.h"
#include <bautils.h> // BaflUtils
#include <barsc.h> // RResoureFile
#include <featmgr.h>
#include <bldvariant.hrh>
#include <DRMCommon.h>
#include <sysutil.h>

#include <ProfileEng.rsg>
#include <TProfileToneSettings.h>
#include <MProfileTones.h>
#include <MProfileSetName.h>
#include <ProfileEng.hrh>
#include <ProfileEngineConstants.h>
#include <MProfileSetTones.h>
#include <MProfileSetExtraTones.h>
#include <MProfileExtraSettings.h>
#include <MProfileFeedbackSettings.h>
#include <MProfilesLocalFeatures.h>
#include <MProfileUtilitySingleton.h>
#include <ProfilesVariant.hrh> // KProEngFeatureIdVTRingingTone
#include <centralrepository.h>
#include <ProfileEnginePrivatePSKeys.h>
#include <hwrmvibrasdkcrkeys.h>
#include <data_caging_path_literals.hrh>

#include <psmsettings.h>
#include <psmsrvdomaincrkeys.h>

#include <MProfileExtraTones2.h>
#include <MProfileVibraSettings.h>


// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CProfileEngineImpl::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CProfileEngineImpl* CProfileEngineImpl::NewL()
    {
    CProfileEngineImpl* self = CProfileEngineImpl::NewLC();
    CleanupStack::Pop();
    return self;
    }


// -----------------------------------------------------------------------------
// CProfileEngineImpl::NewLC
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CProfileEngineImpl* CProfileEngineImpl::NewLC()
    {
    CProfileEngineImpl* self = new ( ELeave ) CProfileEngineImpl;

    CleanupStack::PushL( self );
    self->ConstructL();

    return self;
    }


// -----------------------------------------------------------------------------
// CProfileEngineImpl::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CProfileEngineImpl* CProfileEngineImpl::NewL( RFs* aFs )
    {
    CProfileEngineImpl* self = CProfileEngineImpl::NewLC( aFs );
    CleanupStack::Pop();
    return self;
    }


// -----------------------------------------------------------------------------
// CProfileEngineImpl::NewLC
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CProfileEngineImpl* CProfileEngineImpl::NewLC( RFs* aFs )
    {
    CProfileEngineImpl* self = new ( ELeave ) CProfileEngineImpl;

    CleanupStack::PushL( self );
    self->ConstructL( aFs );

    return self;
    }


// -----------------------------------------------------------------------------
// CProfileEngineImpl::ConstructL
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CProfileEngineImpl::ConstructL()
    {
    iFs = new ( ELeave ) RFs;
    iFsOwned = ETrue;
    User::LeaveIfError( iFs->Connect() );
    CommonConstructL();
    }

// -----------------------------------------------------------------------------
// CProfileEngineImpl::ConstructL
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CProfileEngineImpl::ConstructL( RFs* aFs )
    {
    iFs = aFs;
    CommonConstructL();
    }

// -----------------------------------------------------------------------------
// CProfileEngineImpl::CommonConstructL
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CProfileEngineImpl::CommonConstructL()
    {
    PRODEBUG( "Profiles Engine CommonConstructL" );

    FeatureManager::InitializeLibL();
    iSideVolumeKeys = FeatureManager::FeatureSupported( KFeatureIdSideVolumeKeys );
    

    iCenRep = CRepository::NewL( KCRUidProfileEngine );
    iVibraCenRep = CRepository::NewL( KCRUidVibraCtrl );

    // Profile Utility must be released in destructor:
    iFeatures = &( ProfileUtilityInstanceL().ProfilesLocalFeatures() );
    iProfileLocalisedNames = CProfilesNamesArrayImpl::NewL();
    ReadProfilesNamesFromResourceL();

    iProfileTiming = CProfileTiming::NewL( *iCenRep, *iFeatures );

    // Open/Create mutex
    TInt error( KErrNotFound );
    while( error == KErrNotFound )
        {
        error = iMutex.CreateGlobal( KProfileMutexName );
        if( error != KErrAlreadyExists )
            {
            break;
            }
        error = iMutex.OpenGlobal( KProfileMutexName );
        }
    User::LeaveIfError( error );
    PRODEBUG( "Profiles Engine CommonConstructL end" );
    }

// Destructor
CProfileEngineImpl::~CProfileEngineImpl()
    {
    iMutex.Close();
    iProperty.Close();
    delete iProfileTiming;
    delete iProfileLocalisedNames;
    
    if ( iFeatures )
        {
        ReleaseProfileUtility();    
        }
        
    delete iCenRep;
    delete iVibraCenRep;
    if( iFsOwned )
        {
        iFs->Close();
        delete iFs;
        }
    
    FeatureManager::UnInitializeLib();
    }

// -----------------------------------------------------------------------------
// CProfileEngineImpl::ActiveProfileLC
//
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
MProfile* CProfileEngineImpl::ActiveProfileLC()
    {
    TInt id( User::LeaveIfError( ActiveProfileId() ) );
    MProfileExtended* profile = ProfileLC( id );
    return profile;
    }

// -----------------------------------------------------------------------------
// CProfileEngineImpl::ActiveProfileL
//
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
MProfile* CProfileEngineImpl::ActiveProfileL()
    {
    MProfile* profile = ActiveProfileLC();
    CleanupStack::Pop();    // profile
    return profile;
    }

// -----------------------------------------------------------------------------
// CProfileEngineImpl::ProfileLC
//
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
MProfileExtended* CProfileEngineImpl::ProfileLC(
    TInt aId )
    {
    return Profile2LC( aId );
    /*
    CProfileImpl* profile = NULL;

    iMutex.Wait();
    TInt err( CheckProfileIdL( aId ) );

    if( !err )
        {
        TRAP( err, profile = GetProfileL( aId ) );
        }
    iMutex.Signal();

    User::LeaveIfError( err );

    CleanupStack::PushL( profile );

    if( iSideVolumeKeys )
        {
        // Because SetTemp*VolumeL() functions update only Shared Data those
        // values must be read from SD and set to the MProfile instance
        // returned to the caller:
        TInt activeId( User::LeaveIfError( ActiveProfileId() ) );
        if( activeId == aId )
            {
            TProfileToneSettings& settings =
                    profile->ProfileSetTones().SetToneSettings();
            settings.iRingingVolume = TempRingingVolumeL();
            settings.iMediaVolume = TempMediaVolumeL();
            }
        }

    if( !profile->IsProfileNameChanged() )
        {
        if( IsDefaultProfile( aId ) )
            {
            profile->SetLocalizedProfileNameL( *iProfileLocalisedNames );
            }
        else
            {
            SetLocalizedNameForDynamicProfileL( *profile );
            }
        }

    return profile;
    */
    }

// -----------------------------------------------------------------------------
// CProfileEngineImpl::ProfileL
//
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
MProfileExtended* CProfileEngineImpl::ProfileL(
    TInt aId )
    {
    return Profile2L( aId );
    /*
    MProfileExtended* profile = ProfileLC( aId );
    CleanupStack::Pop();    // profile
    return profile;
    */
    }


// -----------------------------------------------------------------------------
// CProfileEngineImpl::SetActiveProfileL
//
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CProfileEngineImpl::SetActiveProfileL( TInt aId )
    {
    PRODEBUG1( "SetActiveProfileL( %d )", aId );
    CreatePubSubKeysIfNeededL(); 

    iMutex.Wait();
    TInt err( CheckProfileIdL( aId ) );

    if( !err )
        {
        TRAP( err, DoSetActiveProfileL( aId ) );
        }
    else
        {
        // Timed profile was deleted while time expired
        // So pass event to ActiveIdle2 to deal with time icon.
        iProfileTiming->CancelTimedProfileL();
        PublishChangeL( KProEngActiveProfileModified );    
        }
    iMutex.Signal();

    PRODEBUG1( "SetActiveProfileL err=%d", err );
    User::LeaveIfError( err );
    PRODEBUG1( "SetActiveProfileL( %d ) end", aId );
    }

// -----------------------------------------------------------------------------
// CProfileEngineImpl::IsActiveProfileTimedL
//
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TBool CProfileEngineImpl::IsActiveProfileTimedL()
    {
    return iProfileTiming->IsTimingActiveL();
    }

// -----------------------------------------------------------------------------
// CProfileEngineImpl::SetActiveProfileTimedL
//
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CProfileEngineImpl::SetActiveProfileTimedL( TInt aId, TTime aTime )
    {
    if( !iFeatures->IsFeatureSupported( KProEngFeatureIdTimedProfiles ) )
        {
        User::Leave( KErrNotSupported );
        }

    CreatePubSubKeysIfNeededL(); 

    iMutex.Wait();
    TInt err( CheckProfileIdL( aId ) );

    if( !err )
        {
        TRAP( err, DoSetActiveProfileL( aId, &aTime ) );
        }
    iMutex.Signal();

    User::LeaveIfError( err );
    }

// -----------------------------------------------------------------------------
// CProfileEngineImpl::ReadResourceInBufferLC
//
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CProfileEngineImpl::ReadResourceInBufferLC( TInt aResourceId,
        TResourceReader& aReader )
    {
    TParse* fp = new ( ELeave ) TParse();
    fp->Set( KProfileEngineResourceDriveAndDir, &KDC_RESOURCE_FILES_DIR, NULL );
    iFileName.Copy( fp->FullName() );
    delete fp;

    BaflUtils::NearestLanguageFile( *iFs, iFileName );

    RResourceFile resourceFile;
    resourceFile.OpenL( *iFs, iFileName );
    CleanupClosePushL( resourceFile );

    resourceFile.ConfirmSignatureL( PROFILES_ENGINE_RESOURCE_SIGNATURE );

    HBufC8* dataBuffer = resourceFile.AllocReadLC( aResourceId );

    CleanupStack::Pop(); // dataBuffer
    CleanupStack::PopAndDestroy(); // resourceFile.Close()
    CleanupStack::PushL( dataBuffer );

    aReader.SetBuffer( dataBuffer );
    }

// -----------------------------------------------------------------------------
// CProfileEngineImpl::ReadProfilesNamesFromResourceL
//
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CProfileEngineImpl::ReadProfilesNamesFromResourceL()
    {
    TResourceReader rsReader;
    ReadResourceInBufferLC( R_DEFAULT_PROFILES, rsReader );

    TInt numOfElements( rsReader.ReadInt16() );

    for( TInt i( 0 ) ; i < numOfElements ; ++i )
        {
        TInt profileId( rsReader.ReadInt32() );
        TPtrC profileName( rsReader.ReadTPtrC() );
        TPtrC shortName( rsReader.ReadTPtrC() );
        TPtrC naviName( rsReader.ReadTPtrC() );
        TPtrC titleName( rsReader.ReadTPtrC() );
        CProfileNameImpl* pn =
            CProfileNameImpl::NewLC( profileId, profileName, shortName,
                                     naviName, titleName );
        User::LeaveIfError( iProfileLocalisedNames->Append( pn ) );
        CleanupStack::Pop(); // pn
        }

    CleanupStack::PopAndDestroy(); // ReadResourceInBufferLC()
    }

// -----------------------------------------------------------------------------
// CProfileEngineImpl::Release
//
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CProfileEngineImpl::Release()
    {
    delete this;
    }

// -----------------------------------------------------------------------------
// CProfileEngineImpl::SendPresenceSettingsL
//
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CProfileEngineImpl::SendPresenceSettingsL(
    const MProfilePresence& /* aPresence */ )
    {
    }

// -----------------------------------------------------------------------------
// CProfileEngineImpl::IsDefaultProfile
//
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TBool CProfileEngineImpl::IsDefaultProfile( TInt aId )
    {
    return ( iProfileLocalisedNames->FindById( aId ) != KErrNotFound );
    }

// -----------------------------------------------------------------------------
// CProfileEngineImpl::LocalFeaturesL
//
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
MProfilesLocalFeatures& CProfileEngineImpl::LocalFeatures()
    {
    return *iFeatures;
    }

// -----------------------------------------------------------------------------
// CProfileEngineImpl::IsFeatureSupported
//
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TBool CProfileEngineImpl::IsFeatureSupported(
        TProfileFeatureId aFeatureId ) const
    {
    switch( aFeatureId )
        {
        case EProfileFeatureVTRingingTone:
            {
            return iFeatures->IsFeatureSupported(
                    KProEngFeatureIdVTRingingTone );
            // no break after return
            }
        default:
            {
            return EFalse;
            // no break after return
            }
        }
    }

// -----------------------------------------------------------------------------
// CProfileEngineImpl::ProfileUtilityObject
//
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CProfileEngineImpl::GetProfileUtilityObject(
        TProfileUtilityObjectType /* aType */, TAny*& /* aImpl */ )
    {
    }

// -----------------------------------------------------------------------------
// CProfileEngineImpl::LeaveIfReadOnlyL
//
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CProfileEngineImpl::LeaveIfReadOnlyL( TProfileFlags aFlag )
    {
    MProfileExtended* profile = ProfileLC( ActiveProfileId() );
    TUint32 flags = profile->ModifiableFlags();
    CleanupStack::PopAndDestroy(); // profile
    if( !( flags & aFlag ) )
        {
        User::Leave( KErrAccessDenied );
        }
    }

// -----------------------------------------------------------------------------
// CProfileEngineImpl::ActiveProfileId
//
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt CProfileEngineImpl::ActiveProfileId()
    {
    TInt profileId( 0 );
    TInt error( iCenRep->Get( KProEngActiveProfile, profileId ) );
    if( error )
        {
        return error;
        }
    return profileId;
    }

// -----------------------------------------------------------------------------
// CProfileEngineImpl::CommitChangeL
//
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CProfileEngineImpl::CommitChangeL(
    MProfileExtended& aProfile )
    {
    CreatePubSubKeysIfNeededL(); 
    if( SysUtil::FFSSpaceBelowCriticalLevelL( iFs ) )
        {
        User::Leave( KErrDiskFull );
        }

    TInt profileId( aProfile.ProfileName().Id() );
    // write profile settings
    iMutex.Wait();
    TUint32 ignore( 0 );
    TRAPD( error,
           TInt activeId( User::LeaveIfError( ActiveProfileId() ) );
           User::LeaveIfError( iCenRep->StartTransaction(
                   CRepository::EReadWriteTransaction ) );
           iCenRep->CleanupCancelTransactionPushL();
           aProfile.ExternalizeL( *iCenRep );

           if( profileId == activeId )
               {
               MProfileExtended2 *activeProfile = Profile2LC(activeId);
               UpdateActiveProfileSettingsL( *activeProfile );
               CleanupStack::PopAndDestroy(); // activeProfile
               }

           // write settings to Cenrep:
           User::LeaveIfError( iCenRep->CommitTransaction( ignore ) );
           CleanupStack::PopAndDestroy(); // CleanupCancelTransactionPushL()

           if( profileId == activeId )
               {
               //Send event through P&S
               PublishChangeL( KProEngActiveProfileModified );
               }
         );
    iMutex.Signal();
    User::LeaveIfError( error );
    }

// -----------------------------------------------------------------------------
// CProfileEngineImpl::ProfilesNamesArrayLC
//
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
MProfilesNamesArray* CProfileEngineImpl::ProfilesNamesArrayLC()
    {
    CProfilesNamesArrayImpl* nameArray = CProfilesNamesArrayImpl::NewLC();

    iMutex.Wait();
    TRAPD( error, ReadProfilesNamesL( nameArray ) );
    iMutex.Signal();
    User::LeaveIfError( error );

    return nameArray;
    }

// -----------------------------------------------------------------------------
// CProfileEngineImpl::PublishChangeL
//
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CProfileEngineImpl::PublishChangeL( TUint32 aPSKey )
    {
    TInt flag( 0 );
    User::LeaveIfError( iProperty.Get( KPSUidProfileEngine,
                                       aPSKey, flag ) );
    User::LeaveIfError( iProperty.Set( KPSUidProfileEngine,
                                       aPSKey, ~flag ) );
    }

// -----------------------------------------------------------------------------
// CProfileEngineImpl::UpdateActiveProfileSettingsL
//
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CProfileEngineImpl::UpdateActiveProfileSettingsL( MProfileExtended2& aProfile,
        TBool aSetId )
    {
    const MProfileTones& profileTones = aProfile.ProfileTones();
    const MProfileExtraTones2& profileExtraTones = aProfile.ProfileExtraTones2();
    const MProfileName& profileName = aProfile.ProfileName();
    const TProfileToneSettings& settings = profileTones.ToneSettings();
   // const MProfileVibraSettings &vibraSettings = aProfile.ProfileVibraSettings();

    // These cannot fail because this method is called only in between
    // StartTransaction() and CommitTransaction() calls (except vibra)
    iCenRep->Set( KProEngActiveRingingType, settings.iRingingType );
   
    //Removed. only master volue used
    // iCenRep->Set( KProEngActiveRingingVolume, settings.iRingingVolume );
    iCenRep->Set( KProEngActiveMessageAlert, !( aProfile.IsSilent() ) );
    
    //Since 10.1
    iCenRep->Set( KProEngActiveRingTone,profileTones.RingingTone1() );
    iCenRep->Set( KProEngActiveMessageTone,profileTones.MessageAlertTone() );
    iCenRep->Set( KProEngActiveReminderTone,profileExtraTones.ReminderTone() );
    iCenRep->Set( KProEngActiveClockAlarmTone,profileExtraTones.ClockAlarmTone() );
    
    /*
    if ( MasterVibraL() )
        {
        iCenRep->Set( KProEngActiveAlertVibra,vibraSettings.AlertVibra() );
        }
    else
        {
        iCenRep->Set( KProEngActiveAlertVibra,0 );
        }
    */
    
    if ( !PowerSaveMode() )
        {
    User::LeaveIfError( iVibraCenRep->Set(
            KVibraCtrlProfileVibraEnabled, /*vibraSettings.AlertVibra() != 0 &&*/ MasterVibraL() ) );

#ifdef RD_TACTILE_FEEDBACK
	const MProfileExtraSettings& extraSettings = aProfile.ProfileExtraSettings();
	const MProfileFeedbackSettings& feedbackSettings = 
			extraSettings.ProfileFeedbackSettings();
    User::LeaveIfError( iVibraCenRep->Set( KVibraCtrlProfileFeedbackEnabled, 
    		( feedbackSettings.TactileFeedback() ? 1 : 0 ) ) );
    iCenRep->Set( KProEngActiveTactileFeedback, feedbackSettings.TactileFeedback() );
    iCenRep->Set( KProEngActiveAudioFeedback, feedbackSettings.AudioFeedback() );
#endif

    iCenRep->Set( KProEngActiveKeypadVolume, settings.iKeypadVolume );
        }

    iCenRep->Set( KProEngActiveWarningTones, settings.iWarningAndGameTones );
    if( aSetId )
        {
        iCenRep->Set( KProEngActiveProfile, profileName.Id() );
        }
    }

// -----------------------------------------------------------------------------
// CProfileEngineImpl::UpdateContactGroupsValueL
//
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CProfileEngineImpl::UpdateContactGroupsValueL(
    MProfile& /* aProfile */ )
    {
    }

// -----------------------------------------------------------------------------
// CProfileEngineImpl::GetProfileL
//
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
CProfileImpl* CProfileEngineImpl::GetProfileL(
    TInt aId )
    {
    CProfileImpl* profile = CProfileImpl::NewLC( *iFs );
    profile->InternalizeL( *iCenRep, aId );
    CleanupStack::Pop(); // profile
    return profile;
    }

// -----------------------------------------------------------------------------
// CProfileEngineImpl::ReadProfilesNamesL
//
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CProfileEngineImpl::ReadProfilesNamesL(
        CProfilesNamesArrayImpl* aProfilesNames )
    {
    __ASSERT_DEBUG( aProfilesNames,
        ProfileEngPanic::Panic( EProfileEngPanicNullPointer ) );

    TInt len( iProfileLocalisedNames->MdcaCount() );
    CProfileNameImpl* profileName = CProfileNameImpl::NewL();
    CleanupStack::PushL( profileName );

    for( TInt i( 0 ); i < len; ++i )
        {
        TInt id( iProfileLocalisedNames->ProfileName( i )->Id() );
        profileName->InternalizeL( *iCenRep, id );
        if( !profileName->IsProfileNameChanged() )
            {
            profileName->SetLocalizedProfileNameL( *iProfileLocalisedNames );
            }
        CProfileNameImpl* nameImpl =
            CProfileNameImpl::NewLC( *profileName );

        User::LeaveIfError( aProfilesNames->InsertInOrder( nameImpl ) );
        CleanupStack::Pop();    // nameImpl
        }

    // Since 10.1, No dynamic profiles in TB 10.1
 //   ReadDynamicProfilesNamesL( aProfilesNames, *profileName );
    CleanupStack::PopAndDestroy();  // profileName
    }

// -----------------------------------------------------------------------------
// CProfileEngineImpl::DoSetActiveProfileL
//
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CProfileEngineImpl::DoSetActiveProfileL(
    TInt aId, TTime* aTime )
    {
    MProfileExtended2* profile = Profile2LC( aId );

    User::LeaveIfError( iCenRep->StartTransaction(
            CRepository::EReadWriteTransaction ) );
    iCenRep->CleanupCancelTransactionPushL();
    // Cancel first any pending timings (the logic is the same both in new
    // timing and in plain activation):
    TInt previousId( iProfileTiming->CancelTimedProfileL() );
    if( aTime )
        {
        TInt activeId( ( previousId != KErrNotFound )
                       ? previousId
                       : User::LeaveIfError( ActiveProfileId() ) );
        iProfileTiming->SetTimedProfileL( activeId, *aTime );
        }

    TRAPD( error, UpdateActiveProfileSettingsL( *profile, ETrue ) );
    TUint32 ignore( 0 );
    if( !error )
        {
        error = iCenRep->CommitTransaction( ignore );
        }

    if( error )
        {
        if( aTime )
            {
            iProfileTiming->RollbackTimedProfile();
            }
        User::Leave( error );
        }

    PublishChangeL( KProEngActiveProfileChanged );// send P&S event

    CleanupStack::PopAndDestroy( 2 ); // profile, CleanupCancelTransactionPushL
    }

// -----------------------------------------------------------------------------
// CProfileEngineImpl::SetTempRingingVolumeL
//
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CProfileEngineImpl::SetTempRingingVolumeL(
        TProfileRingingVolume aVolume )
    {
    LeaveIfReadOnlyL( EProfileFlagRingingVolume );
    CreatePubSubKeysIfNeededL(); 
    iMutex.Wait();
    TRAPD( error,
           User::LeaveIfError( iCenRep->StartTransaction(
                   CRepository::EReadWriteTransaction ) );
           iCenRep->CleanupCancelTransactionPushL();
           SetTempVolumeL( *iCenRep, KProEngActiveRingingVolume, aVolume );
           TUint32 ignore( 0 );
           User::LeaveIfError( iCenRep->CommitTransaction( ignore ) );
           CleanupStack::PopAndDestroy(); // CleanupCancelTransactionPushL()
           PublishChangeL( KProEngActiveProfileModified );
        );
    iMutex.Signal();
    User::LeaveIfError( error );
    }

// -----------------------------------------------------------------------------
// CProfileEngineImpl::TempRingingVolumeL
//
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TProfileRingingVolume CProfileEngineImpl::TempRingingVolumeL() const
    {
    return TempVolumeL( *iCenRep, KProEngActiveRingingVolume );
    }

// -----------------------------------------------------------------------------
// CProfileEngineImpl::SetTempMediaVolumeL
//
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CProfileEngineImpl::SetTempMediaVolumeL(
        TProfileRingingVolume /* aVolume */ )
    {
    // Media volume is not used anymore
    }

// -----------------------------------------------------------------------------
// CProfileEngineImpl::TempMediaVolumeL
//
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TProfileRingingVolume CProfileEngineImpl::TempMediaVolumeL() const
    {
    return TempRingingVolumeL();
    // Media volume is not used anymore
    }

// -----------------------------------------------------------------------------
// CProfileEngineImpl::SetTempVolumeL
//
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CProfileEngineImpl::SetTempVolumeL(
        CRepository& aCenRep, TUint32 aCenRepKey,
        TProfileRingingVolume aVolume )
    {
    // level 0 is only for CDMA version:
    User::LeaveIfError( aCenRep.Set( aCenRepKey, aVolume ) );
    }


// -----------------------------------------------------------------------------
// CProfileEngineImpl::TempVolumeL
//
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TProfileRingingVolume CProfileEngineImpl::TempVolumeL(
        CRepository& aCenRep, TUint32 aCenRepKey ) const
    {
    TInt volume( 0 );
    User::LeaveIfError( aCenRep.Get( aCenRepKey, volume ) );
    TProfileRingingVolume vol = TProfileRingingVolume( volume );
    return vol;
    }

// -----------------------------------------------------------------------------
// CProfileEngineImpl::CheckProfileIdL
//
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt CProfileEngineImpl::CheckProfileIdL( TInt aId )
    {
    if( !IsDefaultProfile( aId ) && ProfileEngUtils::FindValueFromBinaryKeyL(
            *iCenRep, KProEngUsedIds, aId ) == KErrNotFound )
        {
        return KErrNotFound;
        }
    return KErrNone;
    }

// -----------------------------------------------------------------------------
// CProfileEngineImpl::CreatePubSubKeysIfNeededL
//
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CProfileEngineImpl::CreatePubSubKeysIfNeededL()
    {
    if( iPubSubKeysCreated ) return;

    _LIT_SECURITY_POLICY_PASS( KProfileEngReadPolicy );
    const TSecurityPolicy KProfileEngWritePolicy( ECapabilityWriteDeviceData );

    TInt error( RProperty::Define( KPSUidProfileEngine,
                                   KProEngActiveProfileModified,
                                   RProperty::EInt,
                                   KProfileEngReadPolicy,
                                   KProfileEngWritePolicy ) );
    if( error && error != KErrAlreadyExists )
        {
        User::Leave( error );
        }

    error = RProperty::Define( KPSUidProfileEngine,
                               KProEngActiveProfileChanged,
                               RProperty::EInt,
                               KProfileEngReadPolicy,
                               KProfileEngWritePolicy );
    if( error && error != KErrAlreadyExists )
        {
        User::Leave( error );
        }

    iPubSubKeysCreated = ETrue;
    }

// ========================== OTHER EXPORTED FUNCTIONS =========================

// -----------------------------------------------------------------------------
// CreateProfileEngineL create profiles engine and return public interface
// of the profiles engine.
// Returns: Profiles Engine public interface
// -----------------------------------------------------------------------------
//
EXPORT_C MProfileEngine* CreateProfileEngineL()
    {
    return CProfileEngineImpl::NewL();
    }

// -----------------------------------------------------------------------------
// CreateProfileEngineExtendedL create profiles engine and
// return internal interface of the profiles engine
// Returns: Profiles Engine internal interface
// -----------------------------------------------------------------------------
//
EXPORT_C MProfileEngineExtended* CreateProfileEngineExtendedL()
    {
    return CProfileEngineImpl::NewL();
    }

// -----------------------------------------------------------------------------
// CreateProfileEngineExtendedL create profiles engine and
// return internal interface of the profiles engine
// Returns: Profiles Engine internal interface
// -----------------------------------------------------------------------------
//
EXPORT_C MProfileEngineExtended2* CreateProfileEngineExtended2L()
    {
    return CProfileEngineImpl::NewL();
    }

// -----------------------------------------------------------------------------
// CreateProfileEngineL create profiles engine and return public interface
// of the profiles engine.
// Returns: Profiles Engine public interface
// -----------------------------------------------------------------------------
//
EXPORT_C MProfileEngine* CreateProfileEngineL( RFs* aFs )
    {
    return CProfileEngineImpl::NewL( aFs );
    }

// -----------------------------------------------------------------------------
// CreateProfileEngineExtendedL create profiles engine and
// return internal interface of the profiles engine
// Returns: Profiles Engine internal interface
// -----------------------------------------------------------------------------
//
EXPORT_C MProfileEngineExtended* CreateProfileEngineExtendedL( RFs* aFs )
    {
    return CProfileEngineImpl::NewL( aFs );
    }

// -----------------------------------------------------------------------------
// CreateProfileEngineExtendedL create profiles engine and
// return internal interface of the profiles engine
// Returns: Profiles Engine internal interface
// -----------------------------------------------------------------------------
//
EXPORT_C MProfileEngineExtended2* CreateProfileEngineExtended2L( RFs* aFs )
    {
    return CProfileEngineImpl::NewL( aFs );
    }

// -----------------------------------------------------------------------------
// CProfileEngineImpl::PowerSaveMode
//
// -----------------------------------------------------------------------------
//
TBool CProfileEngineImpl::PowerSaveMode()
    {
    TBool psm = EFalse;
    
    TRAPD( err, psm = PowerSaveModeL() );
    
    if ( err != KErrNone )
        {
        return EFalse;
        }
        
    return psm;
    }

    
// -----------------------------------------------------------------------------
// CProfileEngineImpl::PowerSaveModeL
//
// -----------------------------------------------------------------------------
//
TBool CProfileEngineImpl::PowerSaveModeL()
    {
    if( !FeatureManager::FeatureSupported( KFeatureIdPowerSave ) )
        {
        return EFalse;
        }

    TInt psm = EPsmsrvModeNormal;
    
    // KProEngPsmChanged PubSub key is set by GSProfilesPlugin when power save
    // mode changes. If the key cannot be read (doesn't exist), PSM hasn't
    // changed since the last boot and the mode is read from PSM CenRep.
    if( RProperty::Get( KPSUidProfileEngine, KProEngPsmChanged, psm ) != KErrNone )
        {
        CRepository* cenrep = CRepository::NewLC( KCRUidPowerSaveMode );
        User::LeaveIfError( cenrep->Get( KPsmCurrentMode, psm ) );
        CleanupStack::PopAndDestroy( cenrep );
        }

    if ( psm == EPsmsrvModePowerSave )
        {
        return ETrue;
        }
        
    return EFalse;
    }



// -----------------------------------------------------------------------------
// CProfileEngineImpl::Profile2LC
//
// -----------------------------------------------------------------------------
//
MProfileExtended2* CProfileEngineImpl::Profile2LC( TInt aId )
    {
    CProfileImpl* profile = NULL;

    iMutex.Wait();
    TInt err( CheckProfileIdL( aId ) );

    if( !err )
        {
        TRAP( err, profile = GetProfileL( aId ) );
        }
    iMutex.Signal();

    User::LeaveIfError( err );

    CleanupStack::PushL( profile );

    if( iSideVolumeKeys )
        {
        // Because SetTemp*VolumeL() functions update only Shared Data those
        // values must be read from SD and set to the MProfile instance
        // returned to the caller:
        TInt activeId( User::LeaveIfError( ActiveProfileId() ) );
        if( activeId == aId )
            {
            TProfileToneSettings& settings =
                    profile->ProfileSetTones().SetToneSettings();
            settings.iRingingVolume = TempRingingVolumeL();
            settings.iMediaVolume = TempMediaVolumeL();
            }
        }

    if( !profile->IsProfileNameChanged() )
        {
        if( IsDefaultProfile( aId ) )
            {
            profile->SetLocalizedProfileNameL( *iProfileLocalisedNames );
            }
        else
            {
            SetLocalizedNameForDynamicProfileL( *profile );
            }
        }

    return profile;
    }

// -----------------------------------------------------------------------------
// CProfileEngineImpl::Profile2L
//
// -----------------------------------------------------------------------------
//
MProfileExtended2* CProfileEngineImpl::Profile2L( TInt aId )
    {
    MProfileExtended2* profile = Profile2LC( aId );
    CleanupStack::Pop();    // profile
    return profile;
    }

// -----------------------------------------------------------------------------
// CProfileEngineImpl::CommitChange2L
//
// -----------------------------------------------------------------------------
//
void CProfileEngineImpl::CommitChange2L( MProfileExtended2& aProfile )
    {
    CommitChangeL (aProfile);
    }


// -----------------------------------------------------------------------------
// CProfileEngineImpl::MasterVolumeL
//
// -----------------------------------------------------------------------------
//
TInt CProfileEngineImpl::MasterVolumeL() const
    {
    TInt masterVolume( 0 );
    User::LeaveIfError( iCenRep->Get( KProEngMasterVolume, masterVolume ) );
    return masterVolume;
    }

// -----------------------------------------------------------------------------
// CProfileEngineImpl::SetMasterVolumeL
//
// -----------------------------------------------------------------------------
//
void CProfileEngineImpl::SetMasterVolumeL( TInt aMasterVolume )
    {
    User::LeaveIfError( iCenRep->Set( KProEngMasterVolume, aMasterVolume ) );
    User::LeaveIfError( iCenRep->Set( KProEngActiveRingingVolume, aMasterVolume ) );
    
    //Send event through P&S
    CreatePubSubKeysIfNeededL();
    PublishChangeL( KProEngActiveProfileModified );
    }

// -----------------------------------------------------------------------------
// CProfileEngineImpl::MasterVibraL
//
// -----------------------------------------------------------------------------
//
TBool CProfileEngineImpl::MasterVibraL() const
    {
    TInt masterVibra( 0 );
    User::LeaveIfError( iCenRep->Get( KProEngMasterVibra, masterVibra ) );
    return ( masterVibra != 0 );
    }

// -----------------------------------------------------------------------------
// CProfileEngineImpl::SetMasterVibraL
//
// -----------------------------------------------------------------------------
//
void CProfileEngineImpl::SetMasterVibraL( TBool aMasterVibra )
    {
    User::LeaveIfError( iCenRep->Set( KProEngMasterVibra, aMasterVibra ? 1 : 0 ) );
    User::LeaveIfError ( iCenRep->Set( KProEngActiveAlertVibra, aMasterVibra ? 1 : 0 ) );
    
    /*
    MProfileExtended2* activeProfile = Profile2LC( ActiveProfileId() );
    const MProfileVibraSettings &vibraSettings = activeProfile->ProfileVibraSettings();
    if (activeProfile)
        {
        if (aMasterVibra)
            {
            iCenRep->Set( KProEngActiveAlertVibra,vibraSettings.AlertVibra() );
            }
        else
            {
            iCenRep->Set( KProEngActiveAlertVibra, 0 );
            }        
        }
    CleanupStack::PopAndDestroy(); // activeProfile
    */
            
    if ( !PowerSaveMode() )
        {
        User::LeaveIfError( iVibraCenRep->Set(
                KVibraCtrlProfileVibraEnabled, aMasterVibra ? 1 : 0) );
        }
    
    //Send event through P&S
    CreatePubSubKeysIfNeededL();
    PublishChangeL( KProEngActiveProfileModified );
    }

// -----------------------------------------------------------------------------
// CProfileEngineImpl::SilenceModeL
//
// -----------------------------------------------------------------------------
//
TBool CProfileEngineImpl::SilenceModeL() const
    {
    TInt silenceMode( 0 );
    User::LeaveIfError( iCenRep->Get( KProEngSilenceMode, silenceMode ) );
    return ( silenceMode != 0 );
    }

// -----------------------------------------------------------------------------
// CProfileEngineImpl::SetSilenceModeL
//
// -----------------------------------------------------------------------------
//
void CProfileEngineImpl::SetSilenceModeL( TBool aSilenceMode )
    {
    User::LeaveIfError( iCenRep->Set( KProEngSilenceMode, aSilenceMode ? 1 : 0 ) );
    
    //Send event through P&S
    CreatePubSubKeysIfNeededL();
    PublishChangeL( KProEngActiveProfileModified );
    }

//  End of File

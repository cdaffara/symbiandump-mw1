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
* Description: Implementation of the CProfileEngineImpl.
*
*/



// INCLUDE FILES
#include "CProfileEngineImpl.h"
#include "CProfilesNamesArrayImpl.h"
#include "CProfileImpl.h"
#include "CProfileNameImpl.h"
#include "ProfileEngUtils.h"
#include "ProfileEng.hrh"
#include "ProfileEngPanic.h"
#include "ProfileEnginePrivateCRKeys.h"
#include <bautils.h>
#include <barsc.h>
#include <featmgr.h>
#include <settingsinternalcrkeys.h>
#include <sysutil.h>
#include <accessoriescrkeys.h>

#include <ProfileEng.rsg>
#include <TProfileToneSettings.h>
#include <MProfileTones.h>
#include <MProfileSetName.h>
#include <ProfileEngineConstants.h>

namespace
    {
    const TInt KProfileGeneratedNameTailLength( 4 );
    _LIT( KFirstParenthesis, "(" );
    }

// ============================= LOCAL FUNCTIONS ===============================

void GenerateProfileName( TDes& aProfileName, const MProfilesNamesArray& aProfileNames )
    {
    _LIT( KEndParenthesis, ")" );
    _LIT( KZero, "0" );
    TInt counter( 1 );
    TBuf<KProfileMaxNameLength> rootName( aProfileName );
    while( aProfileNames.FindByName( aProfileName ) != KErrNotFound &&
           counter <= KProfilesMaxNumberOfDynamicProfiles )
        {
        aProfileName.Copy( rootName );
        aProfileName.Append( KFirstParenthesis );
        if( counter < 10 )
            {
            aProfileName.Append( KZero );
            }
        TBuf<KProfilesMaxTIntLength> number;
        number.Num( counter );
        aProfileName.Append( number );
        aProfileName.Append( KEndParenthesis );
        ++counter;
        }
    }

// -----------------------------------------------------------------------------
// HandleProfileRemoved
//
// Updates the given central repository value from the given file, when a
// profile is removed. This is used for those central repository keys that
// contain a profile id as a value, and if a deleted profile is
// currently assigned to the central repository value, it is resetted to the
// given default value.
// -----------------------------------------------------------------------------
//
void HandleProfileRemoved( const TInt aRemovedProfileId,
                            const TInt aValuePlusDelta,
                            CRepository& aCenRep,
                            const TUint aKey,
                            const TInt aDefaultValue )
    {
    if ( ( aValuePlusDelta ) == aRemovedProfileId )
        {
        aCenRep.Set( aKey, aDefaultValue );
        }
    }

// -----------------------------------------------------------------------------
// CreateIntSettingForNewProfileL
//
// Creates a new integer Central Repository key for the new profile and
// initialises it by copying the default value to it.
//
// -----------------------------------------------------------------------------
//
void CreateIntSettingForNewProfileL( CRepository& aRepository, TInt aSettingId,
                                     TInt aProfileId )
    {
    TInt val;
    User::LeaveIfError(
            aRepository.Get( 
            ProfileEngUtils::ResolveKey( aSettingId,
            KProEngDefaultProfileId ), val ) );
    TInt err(
            aRepository.Create( 
            ProfileEngUtils::ResolveKey( aSettingId, aProfileId ),
            val ) );
    if( ( err != KErrNone ) && ( err != KErrAlreadyExists ) )
        {
        User::Leave( err );
        }
    }

// -----------------------------------------------------------------------------
// CreateStringSettingForNewProfileL
//
// Creates a new string Central Repository key for the new profile and
// initialises it by copying the default value to it.
//
// -----------------------------------------------------------------------------
//
void CreateStringSettingForNewProfileL( CRepository& aRepository,
                                        TInt aSettingId, TInt aProfileId,
                                        const TDesC& aNewProfileName,
                                        TPtr& aStringPtr ) 
    {
    if( aSettingId != KProEngProfileName )
        {
        User::LeaveIfError(
                aRepository.Get( 
                ProfileEngUtils::ResolveKey( aSettingId,
                KProEngDefaultProfileId ), aStringPtr ) );
        }
    else
        {
        aStringPtr = aNewProfileName;
        }
    TInt err(
            aRepository.Create( 
            ProfileEngUtils::ResolveKey( aSettingId, aProfileId ),
            aStringPtr ) );
    if( ( err != KErrNone ) && ( err != KErrAlreadyExists ) )
        {
        User::Leave( err );
        }
    }

// -----------------------------------------------------------------------------
// CreateBinSettingForNewProfileL
//
// Creates a new binary Central Repository key for the new profile and
// initialises it by copying the default value to it.
//
// -----------------------------------------------------------------------------
//
void CreateBinSettingForNewProfileL( CRepository& aRepository, TInt aSettingId,
                                     TInt aProfileId, TPtr8& aBinPtr )
    {
    User::LeaveIfError(
            aRepository.Get( 
            ProfileEngUtils::ResolveKey( aSettingId,
            KProEngDefaultProfileId ), aBinPtr ) );
    TInt err(
            aRepository.Create( 
            ProfileEngUtils::ResolveKey( aSettingId, aProfileId ),
            aBinPtr ) );
    if( ( err != KErrNone ) && ( err != KErrAlreadyExists ) )
        {
        User::Leave( err );
        }
    }

// -----------------------------------------------------------------------------
// GetSettingsForProfile
//
// Gets all the settings of the profile with given id from Central Repository.
// -----------------------------------------------------------------------------
//
TInt GetSettingsForProfile( CRepository& aRepository,
                            RArray<TUint32>& aSettings,
                            TInt aId )
    {
    const TUint32 KId( aId << 24 );
    const TUint32 KMask( 0xFF000000 );
    TInt error( 0 );
    TRAP( error, error = aRepository.FindL( KId, KMask, aSettings ) );
    return error;
    }

// -----------------------------------------------------------------------------
// DeleteSettingsForProfile
//
// Deletes all the settings of the profile with given id from Central Repository
// Tries to delete all without checking if a deletion fails.
// -----------------------------------------------------------------------------
//
void DeleteSettingsForProfile( CRepository& aRepository,
                               RArray<TUint32>& aSettings )
    {
    for( TInt i( aSettings.Count() - 1 ); i >= 0; --i )
        {
        aRepository.Delete( aSettings[i] ); // ignore errors
        }
    }

// -----------------------------------------------------------------------------
// CreateSettingsForNewProfileL
//
// Creates new Central Repository keys for the new profile and initialises them
// by copying the default values to them.
//
// -----------------------------------------------------------------------------
//
void CreateSettingsForNewProfileL( CRepository& aRepository, TInt aProfileId,
                                   const TDesC& aNewProfileName,
                                   TResourceReader& aReader )
    {
    User::LeaveIfError( aRepository.StartTransaction(
                        CRepository::EReadWriteTransaction ) );

    // This only be used after normal RFS(restore factory settings), If there was
    // user created profile before normal RFS, and the normal RFS only restore
    // KProEngUsedIds, the settings of user created profile are in CenRep still.
    // So we should delete the remain settings and create new one.
    RArray<TUint32> settings;
    if ( !GetSettingsForProfile( aRepository, settings, aProfileId ) )
    	{
    	DeleteSettingsForProfile( aRepository, settings );
    	}
    settings.Close();

    HBufC* stringVal = HBufC::NewL(
            NCentralRepositoryConstants::KMaxUnicodeStringLength );
    CleanupStack::PushL( stringVal );
    TPtr stringValPtr = stringVal->Des();
    HBufC8* binVal = HBufC8::NewL(
            NCentralRepositoryConstants::KMaxBinaryLength );
    CleanupStack::PushL( binVal );
    TPtr8 binValPtr = binVal->Des();

    TInt numOfElements( aReader.ReadInt16() );

    for( TInt i( 0 ) ; i < numOfElements ; ++i )
        {
        TInt settingId( aReader.ReadInt32() );
        TInt keyType( aReader.ReadInt32() );
        switch( keyType )
            {
            case EProfileSettingKeyTypeInt:
                {
                CreateIntSettingForNewProfileL( aRepository, settingId,
                                                aProfileId );
                break;
                }
            case EProfileSettingKeyTypeString:
                {
                CreateStringSettingForNewProfileL( aRepository, settingId,
                                                   aProfileId,
                                                   aNewProfileName,
                                                   stringValPtr );
                break;
                }
            case EProfileSettingKeyTypeBin:
                {
                CreateBinSettingForNewProfileL( aRepository, settingId,
                                                aProfileId, binValPtr );
                                                
                break;
                }
            case EProfileSettingKeyTypeReal: // no settings of this type
            default:
                {
                break;
                }
            }
        }

    CleanupStack::PopAndDestroy( 2 ); // stringVal, binVal
 
    // Update the used ids value (this is done last so that we can get the
    // repository back in a legal state (by deleting the settings just 
    // created)).
    TBuf8< KProfilesMaxNumberOfDynamicProfiles > idBuf;
    ProfileEngUtils::GetBinarySettingL( aRepository, KProEngUsedIds, idBuf );
    idBuf.Append( aProfileId );
    User::LeaveIfError( aRepository.Set( KProEngUsedIds, idBuf ) );

    TUint32 ignore( 0 );
    User::LeaveIfError( aRepository.CommitTransaction( ignore ) );
    }

// -----------------------------------------------------------------------------
// DeleteProfileIdFromList
//
// -----------------------------------------------------------------------------
//
void DeleteProfileIdFromList( CRepository& aRepository, TInt aId, TDes8& aIdBuf )
    {
    TInt numOfIds = aIdBuf.Length();
    for( TInt idx( 0 ); idx < numOfIds; ++idx )
        {
        if( aIdBuf[ idx ] == aId )
            {
            aIdBuf.Delete( idx, 1 );
            aRepository.Set( KProEngUsedIds, aIdBuf );
            break;
            }
        }
    }

void DoDeleteProfileL( CRepository& aCenRep, TInt aId )
    {
    RArray<TUint32> settings;
    User::LeaveIfError( GetSettingsForProfile( aCenRep, settings, aId ) );

    TBuf8< 4 * KProfilesMaxNumberOfDynamicProfiles > idBuf; // One id is 4-bytes long
    ProfileEngUtils::GetBinarySettingL( aCenRep, KProEngUsedIds, idBuf );

    User::LeaveIfError( aCenRep.StartTransaction(
                        CRepository::EReadWriteTransaction ) );
    DeleteSettingsForProfile( aCenRep, settings );
    settings.Reset();
    DeleteProfileIdFromList( aCenRep, aId, idBuf );

    TInt headsetVal( 0 );
    TInt loopsetVal( 0 );
    TInt ttyVal( 0 );
    TInt btVal( 0 );
    TUint32 ignore( 0 );
    TRAPD( error,
          CRepository* cr = CRepository::NewL( KCRUidProfileSettings );
          CleanupStack::PushL( cr );
          User::LeaveIfError( cr->Get( KSettingsHeadsetDefaultProfile, headsetVal ) );
          User::LeaveIfError( cr->Get( KSettingsLoopsetDefaultProfile, loopsetVal ) );
          User::LeaveIfError( cr->Get( KSettingsTTYDefaultProfile, ttyVal ) );
          User::LeaveIfError( cr->Get( KSettingsBTDefaultProfile, btVal ) );
          User::LeaveIfError( cr->StartTransaction(
                              CRepository::EReadWriteTransaction ) );
          // -1 is a mystery, consult general settings team:
          // 0 ( = general profile ) is the default profile used with all the
          // accessories:
          HandleProfileRemoved( aId, headsetVal-1, *cr,
                                KSettingsHeadsetDefaultProfile, 0 );
          HandleProfileRemoved( aId, loopsetVal-1, *cr,
                                KSettingsLoopsetDefaultProfile, 0 );
          HandleProfileRemoved( aId, ttyVal-1, *cr,
                                KSettingsTTYDefaultProfile, 0 );
          HandleProfileRemoved( aId, btVal-1, *cr,
                                KSettingsBTDefaultProfile, 0 );
          User::LeaveIfError( cr->CommitTransaction( ignore ) );

          CleanupStack::PopAndDestroy(); // delete cr:
        );

    if( error )
        {
        aCenRep.RollbackTransaction(); // ignore error
        User::Leave( error );
        }

    User::LeaveIfError( aCenRep.CommitTransaction( ignore ) );
    }

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CProfileEngineImpl::DeleteProfileL
//
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt CProfileEngineImpl::DeleteProfileL( TInt aId )
    {
    iMutex.Wait();
    TInt activeProfileId( ActiveProfileId() );

    if( activeProfileId < 0 )
        {
        iMutex.Signal();
        User::Leave( activeProfileId );
        }

    // Do not allow delete active profile
    if( aId == activeProfileId )
        {
        iMutex.Signal();
        return KErrAbort;
        }
    // Do not allow delete default profile
    if( IsDefaultProfile( aId ) )
        {
        iMutex.Signal();
        return KErrAbort;
        }

    TRAPD( error, DoDeleteProfileL( *iCenRep, aId ) );
    iMutex.Signal();
    User::LeaveIfError( error );
    return KErrNone;
    }

// -----------------------------------------------------------------------------
// CProfileEngineImpl::CreateProfileL
//
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
MProfileExtended* CProfileEngineImpl::CreateProfileL()
    {
    if( SysUtil::FFSSpaceBelowCriticalLevelL( iFs ) )
        {
        User::Leave( KErrDiskFull );
        }

    // Create profile ID
    CProfilesNamesArrayImpl* profileNameArray = CProfilesNamesArrayImpl::NewLC();

    iMutex.Wait();
    TRAPD( error, ReadProfilesNamesL( profileNameArray ) );
    if( error )
        {
        iMutex.Signal();
        User::Leave( error );
        }

    if( profileNameArray->MdcaCount() -         // current profile count
        iProfileLocalisedNames->MdcaCount() >=  // default profile count
        KProfilesMaxNumberOfDynamicProfiles )   // max dynamic profiles
        {
        // Max number of dynamic profiles
        CleanupStack::PopAndDestroy();  // profileNameArray
        iMutex.Signal();
        return NULL;
        }

    TInt profileId( KErrNotFound );
    // Find first free id
    TInt lastDynamicProfileId( KProfileBaseDynamicProfileId + 
                               KProfilesMaxNumberOfDynamicProfiles );

    for( TInt i( KProfileBaseDynamicProfileId ); i < lastDynamicProfileId; ++i )
        {
        TInt index( profileNameArray->FindById( i ) );
        if( index == KErrNotFound )
            {
            profileId = i;
            break;
            }
        }

    if( profileId == KErrNotFound )
        {
        CleanupStack::PopAndDestroy();  // profileNameArray
        iMutex.Signal();
        return NULL;
        }

    TBuf<KProfileMaxNameLength> profileName;
    TRAP( error,
          ReadNewProfileNameFromResourceL( R_NEW_PROFILE, profileName ) );
    if( error )
        {
        iMutex.Signal();
        User::Leave( error );
        }

    GenerateProfileName( profileName, *profileNameArray );
    CleanupStack::PopAndDestroy();  // profileNameArray

    TResourceReader rsReader;
    CProfileImpl* newProfile = NULL;
    TRAP( error,
          ReadResourceInBufferLC( R_PROFILE_SETTINGS, rsReader );
          CreateSettingsForNewProfileL( *iCenRep, profileId,
                                        profileName, rsReader );
          CleanupStack::PopAndDestroy();  // ReadResourceInBufferLC

          // Create and store new profile
          newProfile = CProfileImpl::NewLC( *iFs );
          newProfile->InternalizeL( *iCenRep, profileId );
                
          CleanupStack::Pop();    // newProfile
          );
 
    iMutex.Signal();
    User::LeaveIfError( error );

    return newProfile;
    }

// -----------------------------------------------------------------------------
// CProfileEngineImpl::ReadNewProfileNameFromResourceL
//
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CProfileEngineImpl::ReadNewProfileNameFromResourceL( TInt aResourceId,
                                                          TDes& aProfileName )
    {
    TResourceReader rsReader;
    ReadResourceInBufferLC( aResourceId, rsReader );

    TPtrC profileName( rsReader.ReadTPtrC() );
    aProfileName.Copy( profileName );

    CleanupStack::PopAndDestroy(); // ReadResourceInBufferLC
    }

// -----------------------------------------------------------------------------
// CProfileEngineImpl::ReadNewProfileNameFromResourceL
//
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
CProfileNameImpl* CProfileEngineImpl::ReadNewProfileNameFromResourceLC()
    {
    TBuf<KProfileMaxNameLength> profileName;
    ReadNewProfileNameFromResourceL( R_NEW_PROFILE, profileName ); 

    TBuf<KProfileMaxNameLength> shortName;
    ReadNewProfileNameFromResourceL( R_NEW_PROFILE_SHORT, shortName ); 

    TBuf<KProfileMaxNameLength> naviName;
    ReadNewProfileNameFromResourceL( R_NEW_PROFILE_NAVI, naviName ); 

    TBuf<KProfileMaxNameLength> titleName;
    ReadNewProfileNameFromResourceL( R_NEW_PROFILE_TITLE, titleName );

    CProfileNameImpl* localizedName =
        CProfileNameImpl::NewLC( 0, profileName, shortName,
                                 naviName, titleName );

    return localizedName;
    }

// -----------------------------------------------------------------------------
// CProfileEngineImpl::ReadDynamicProfilesNamesL
//
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CProfileEngineImpl::ReadDynamicProfilesNamesL(
        CProfilesNamesArrayImpl* aProfilesNames,
        CProfileNameImpl& aNameTemplate )
    {
    TBuf8< NCentralRepositoryConstants::KMaxBinaryLength > idBuf;
    ProfileEngUtils::GetBinarySettingL( *iCenRep, KProEngUsedIds, idBuf );
    TInt len = idBuf.Length();

    if( len == 0 )
        {
        return; // No user-created profiles, just return
        }

    CProfileNameImpl* localizedName = ReadNewProfileNameFromResourceLC();

    for( TInt i( 0 ); i < len; ++i )
        {
        aNameTemplate.InternalizeL( *iCenRep, idBuf[i] );
        if( !aNameTemplate.IsProfileNameChanged() )
            {
            TInt idxOfParenthesis(
                    aNameTemplate.Name().Find( KFirstParenthesis ) );
            if( idxOfParenthesis != KErrNotFound )
                {
                TBuf<KProfileGeneratedNameTailLength> uniquePart =
                        aNameTemplate.Name().Mid( idxOfParenthesis,
                        KProfileGeneratedNameTailLength );
                aNameTemplate.SetLocalizedProfileNameL( *localizedName,
                                                        uniquePart );
                }
            else
                {
                aNameTemplate.SetLocalizedProfileNameL( *localizedName,
                                                        KNullDesC() );
                }
            }

        CProfileNameImpl* nameImpl =
            CProfileNameImpl::NewLC( aNameTemplate );

        User::LeaveIfError( aProfilesNames->InsertInOrder( nameImpl ) );
        CleanupStack::Pop();    // nameImpl
        }

    CleanupStack::PopAndDestroy( localizedName );
    }

// -----------------------------------------------------------------------------
// CProfileEngineImpl::SetLocalizedNameForDynamicProfileL
//
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CProfileEngineImpl::SetLocalizedNameForDynamicProfileL(
        CProfileImpl& aProfile )
    {
    CProfileNameImpl* localizedName = ReadNewProfileNameFromResourceLC();

    TInt idxOfParenthesis(
            aProfile.ProfileName().Name().Find( KFirstParenthesis ) );
    if( idxOfParenthesis != KErrNotFound )
        {
        TBuf<KProfileGeneratedNameTailLength> uniquePart =
                aProfile.ProfileName().Name().Mid( idxOfParenthesis,
                KProfileGeneratedNameTailLength );
        aProfile.SetLocalizedProfileNameL( *localizedName, uniquePart );
        }
    else
        {
        aProfile.SetLocalizedProfileNameL( *localizedName,
                                           KNullDesC() );
        }

    CleanupStack::PopAndDestroy( localizedName );
    }

//  End of File


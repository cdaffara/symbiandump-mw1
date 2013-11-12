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
* Description:  Implementation of the CProfileImpl.
*
*/



// INCLUDE FILES
#include "CProfileImpl.h"
#include "CProfileNameImpl.h"
#include "CProfileTonesImpl.h"
#include "CProfileExtraTonesImpl.h"
#include "CProfileExtraSettingsImpl.h"
#include "CProfilePresenceImpl.h"
#include "ProfileEngUtils.h"
#include "ProfileEnginePrivateCRKeys.h"
#include <e32svr.h>
#include <cntitem.h>
#include <cntdb.h>
#include <cntdef.h>
#include <TProfileToneSettings.h>
#include <MProfilePttSettings.h>
#include <featmgr.h>
#include <bldvariant.hrh>
#include <rsssettings.h>
#include <pathinfo.h>
#include <MProfilesLocalFeatures.h>
#include <MProfileUtilitySingleton.h>
#include <ProfilesVariant.hrh>
#include <ProfileEngineDomainConstants.h>
#include "CProfileVibraSettingsImpl.h"

// CONSTANTS
// Max. number of Alert for groups:
const TInt KProfilesMaxAlertForGroups = 512;
const TInt KProfilesMaxFlagsLength = 4;

// FORWARD DECLARATIONS
class CRepository;

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CProfileImpl::CProfileImpl
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
CProfileImpl::CProfileImpl( RFs& aFs )
    : iFs( aFs )
    {
    }

// -----------------------------------------------------------------------------
// CProfileImpl::ConstructL
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CProfileImpl::ConstructL()
    {
    CommonConstructL();
    iProfileName = CProfileNameImpl::NewL();
    iProfileTones = CProfileTonesImpl::NewL();
    iProfileExtraTones = CProfileExtraTonesImpl::NewL();
    iProfilePresence = CProfilePresenceImpl::NewL();
    // ProfileUtility must be released in destructor:
    iFeatures = &( ProfileUtilityInstanceL().ProfilesLocalFeatures() );
    iProfileExtraSettings = CProfileExtraSettingsImpl::NewL();
    iProfileVibraSettings = CProfileVibraSettingsImpl::NewL();
    }

// -----------------------------------------------------------------------------
// CProfileImpl::ConstructL
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CProfileImpl::ConstructL(
    const MProfileExtended2& aProfile,
    TInt aId )
    {
    iProfileName = CProfileNameImpl::NewLC( aId, KNullDesC );
    CleanupStack::Pop();    // iProfileName
    CommonConstructL();
    iProfileTones = CProfileTonesImpl::NewL( aProfile.ProfileTones() );
    iProfileExtraTones = CProfileExtraTonesImpl::NewL(
        aProfile.ProfileExtraTones2() );

    iFeatures = &( ProfileUtilityInstanceL().ProfilesLocalFeatures() );

    iProfileExtraSettings = CProfileExtraSettingsImpl::NewL();
    iProfilePresence = CProfilePresenceImpl::NewL(
        aProfile.ProfilePresence() );
    iVisibleFlags = aProfile.VisibleFlags();
    iModifiableFlags = aProfile.ModifiableFlags();
    
    iProfileVibraSettings = CProfileVibraSettingsImpl::NewL();
    
    }

// -----------------------------------------------------------------------------
// CProfileImpl::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CProfileImpl* CProfileImpl::NewL(
    RFs& aFs )
    {
    CProfileImpl* self = NewLC( aFs );
    CleanupStack::Pop();

    return self;
    }

// -----------------------------------------------------------------------------
// CProfileImpl::NewLC
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CProfileImpl* CProfileImpl::NewLC(
    RFs& aFs )
    {
    CProfileImpl* self = new( ELeave ) CProfileImpl( aFs );

    CleanupStack::PushL( self );
    self->ConstructL();

    return self;
    }

// -----------------------------------------------------------------------------
// CProfileImpl::NewLC
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CProfileImpl* CProfileImpl::NewLC(
    RFs& aFs,
    const MProfileExtended2& aProfile,
    TInt aId )
    {
    CProfileImpl* self = new( ELeave ) CProfileImpl( aFs );

    CleanupStack::PushL( self );
    self->ConstructL( aProfile, aId );

    return self;
    }

// Destructor
CProfileImpl::~CProfileImpl()
    {
    delete iSilentTone;
    iAlertFor.Close();

    delete iProfileExtraSettings;
    
    if ( iFeatures )
        {
        ReleaseProfileUtility();  
        }
        
    delete iProfileName;
    delete iProfileTones;
    delete iProfileExtraTones;
    delete iProfilePresence;
    
    delete iProfileVibraSettings;
    }

// -----------------------------------------------------------------------------
// CProfileImpl::IsProfileNameChanged
//
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TBool CProfileImpl::IsProfileNameChanged() const
    {
    return iProfileName->IsProfileNameChanged();
    }

// -----------------------------------------------------------------------------
// CProfileImpl::SetLocalizedProfileNameL
//
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CProfileImpl::SetLocalizedProfileNameL(
    const MProfilesNamesArray& aNameArray )
    {
    iProfileName->SetLocalizedProfileNameL( aNameArray );
    }

// -----------------------------------------------------------------------------
// CProfileImpl::SetLocalizedProfileNameL
//
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CProfileImpl::SetLocalizedProfileNameL( const CProfileNameImpl& aNameImpl,
                                             const TDesC& aUniquePart )
    {
    iProfileName->SetLocalizedProfileNameL( aNameImpl, aUniquePart );
    }

// -----------------------------------------------------------------------------
// CProfileImpl::AlertForL
//
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
const TArray<TContactItemId> CProfileImpl::AlertForL()
    {
    TInt contactIdListCount( iAlertFor.Count() );


    return iAlertFor.Array();
    }

// -----------------------------------------------------------------------------
// CProfileImpl::IsSilent
//
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TBool CProfileImpl::IsSilent() const
    {
	//Since 10.1, firstly check maste silence mode
	TInt silenceMode( 0 );
	CRepository* cenrep = CRepository::NewLC( KCRUidProfileEngine  );
    cenrep->Get( KProEngSilenceMode , silenceMode );
    CleanupStack::PopAndDestroy( cenrep );
    if ( silenceMode ) 
    	{
		return ETrue;
    	}
	
    TProfileRingingType ringType( iProfileTones->ToneSettings().iRingingType );
    if ( ringType == EProfileRingingTypeSilent )
        {
        // If ringing type is silent, the whole profile is considered as silent:
        return ETrue;
        }

    if ( ringType == EProfileRingingTypeBeepOnce )
        {
        // If ringing type is beep once, the profile can not be silent:
        return EFalse;
        }

    TBool isSilent( EFalse );
    TInt als( ESSSettingsAlsNotSupported );
    ProfileEngUtils::GetAlternateLineService( als );

    switch( als )
        {
        case ESSSettingsAlsAlternate:
            {
            const TDesC& ringingTone2 = iProfileTones->RingingTone2();
            isSilent = ( ( ringingTone2.CompareF( KNullDesC ) == 0 ) ||
                         ( ringingTone2.CompareF( *iSilentTone ) == 0 ) );
                         
            break;
            }
        case ESSSettingsAlsPrimary:  // Fall through
        default: // = ESSSettingsAlsNotSupported
            {
            const TDesC& ringingTone1 = iProfileTones->RingingTone1();
            isSilent = ( ( ringingTone1.CompareF( KNullDesC ) == 0 ) ||
                         ( ringingTone1.CompareF( *iSilentTone ) == 0 ) );
                         
            break;
            }
        }

    isSilent &= ( iProfileTones->MessageAlertTone().CompareF
                        ( *iSilentTone ) == 0 );

    if( iAlwaysOnLineEmail )
        {
        // Email is supported
        const TDesC& emailTone = iProfileExtraTones->EmailAlertTone();
        isSilent &= ( ( emailTone.CompareF( KNullDesC ) == 0 ) ||
                      ( emailTone.CompareF( *iSilentTone ) == 0 ) );
        }
    if( iFeatures->IsFeatureSupported( KProEngFeatureIdVTRingingTone ) )
        {
        // VT ringing tone is supported
        const TDesC& vtTone = iProfileExtraTones->VideoCallRingingTone();
        isSilent &= ( ( vtTone.CompareF( KNullDesC ) == 0 ) ||
                     ( vtTone.CompareF( *iSilentTone ) == 0 ) );
        }
    if( iOmaPoc )
        {
        // POC ringing tone is supported
        const TDesC& pocTone = iProfileExtraSettings->ProfilePttSettings()
            .PttRingingTone();
        isSilent &= ( ( pocTone.CompareF( KNullDesC ) == 0 ) ||
                     ( pocTone.CompareF( *iSilentTone ) == 0 ) );
        }
    return isSilent;
    }

// -----------------------------------------------------------------------------
// CProfileImpl::ProfileSetName
//
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
MProfileSetName& CProfileImpl::ProfileSetName() const
    {
    return *iProfileName;
    }

// -----------------------------------------------------------------------------
// CProfileImpl::ProfileSetTones
//
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
MProfileSetTones& CProfileImpl::ProfileSetTones() const
    {
    return *iProfileTones;
    }

// -----------------------------------------------------------------------------
// CProfileImpl::ProfilePresence
//
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
const MProfilePresence& CProfileImpl::ProfilePresence() const
    {
    return *iProfilePresence;
    }

// -----------------------------------------------------------------------------
// CProfileImpl::ProfileSetPresence
//
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
MProfileSetPresence& CProfileImpl::ProfileSetPresence() const
    {
    return *iProfilePresence;
    }

// -----------------------------------------------------------------------------
// CProfileImpl::ProfileSetExtraTones
//
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
MProfileSetExtraTones& CProfileImpl::ProfileSetExtraTones() const
    {
    return *iProfileExtraTones;
    }

// -----------------------------------------------------------------------------
// CProfileImpl::ProfileSetExtraSettings
//
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
MProfileSetExtraSettings& CProfileImpl::ProfileSetExtraSettings() const
    {
    return *iProfileExtraSettings;
    }

// -----------------------------------------------------------------------------
// CProfileImpl::ProfileName
//
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
const MProfileName& CProfileImpl::ProfileName() const
    {
    return *iProfileName;
    }

// -----------------------------------------------------------------------------
// CProfileImpl::ProfileTones
//
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
const MProfileTones& CProfileImpl::ProfileTones() const
    {
    return *iProfileTones;
    }

// -----------------------------------------------------------------------------
// CProfileImpl::ProfileExtraTones
//
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
const MProfileExtraTones& CProfileImpl::ProfileExtraTones() const
    {
    return *iProfileExtraTones;
    }

// -----------------------------------------------------------------------------
// CProfileImpl::ProfileExtraSettings
//
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
const MProfileExtraSettings& CProfileImpl::ProfileExtraSettings() const
    {
    return *iProfileExtraSettings;
    }

// -----------------------------------------------------------------------------
// CProfileImpl::Release
//
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CProfileImpl::Release()
    {
    delete this;
    }

// -----------------------------------------------------------------------------
// CProfileImpl::ModifiableFlags
//
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TUint32 CProfileImpl::ModifiableFlags() const
    {
    return iModifiableFlags;
    }

// -----------------------------------------------------------------------------
// CProfileImpl::VisibleFlags
//
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TUint32 CProfileImpl::VisibleFlags() const
    {
    return iVisibleFlags;
    }

void CProfileImpl::InternalizeL( CRepository& aCenRep, TInt aProfileId )
    {
    TBuf8< KProfilesMaxFlagsLength > flags;
    // Flags
    User::LeaveIfError(
            aCenRep.Get( ProfileEngUtils::ResolveKey( KProEngModifiableFlags,
            aProfileId ), flags ) );
    ProfileEngUtils::GetUint32Presentation( iModifiableFlags, flags, 0 );

    flags.Zero();
    User::LeaveIfError(
            aCenRep.Get( ProfileEngUtils::ResolveKey(
            KProEngVisibleFlags, aProfileId ), flags ) );
    ProfileEngUtils::GetUint32Presentation( iVisibleFlags, flags, 0 );

    // Profile name and id
    iProfileName->InternalizeL( aCenRep, aProfileId );

    // Alert for
    ProfileEngUtils::GetBinarySettingL( aCenRep, ProfileEngUtils::ResolveKey(
            KProEngAlertForGroups, aProfileId ), iAlertForBuf );
            
    TInt count( iAlertForBuf.Length() );
    const TInt KProEngContactIdLength( 4 );
    const TInt KProEngThreeByteShift( 24 );
    const TInt KProEngTwoByteShift( 16 );
    const TInt KProEngOneByteShift( 8 );

    for( TInt i( 0 ); i < count ; i+=KProEngContactIdLength )
        {
        TUint32 id( ( iAlertForBuf[i] << KProEngThreeByteShift ) |
                    ( iAlertForBuf[i+1] << KProEngTwoByteShift ) |
                    ( iAlertForBuf[i+2] << KProEngOneByteShift ) |
                    ( iAlertForBuf[i+3] ) );
        User::LeaveIfError( iAlertFor.Append( TContactItemId( id ) ) );
        }
    iAlertFor.Compress();
    iAlertForBuf.Zero();

    // Profile tones and presence
    iProfileTones->InternalizeL( aCenRep, aProfileId );
    iProfileExtraTones->InternalizeL( aCenRep, aProfileId );

    iProfilePresence->InternalizeL( aCenRep, aProfileId );
    iProfileExtraSettings->InternalizeL( aCenRep, aProfileId );
    
    iProfileVibraSettings->InternalizeL( aCenRep, aProfileId );
    }

void CProfileImpl::ExternalizeL( CRepository& aCenRep )
    {
    TInt profileId = iProfileName->Id();

    // Flags aren't externalized because they should not be modified in runtime

    // Profile name and id
    iProfileName->ExternalizeL( aCenRep );

    // Alert for
    iAlertForBuf.Zero();
    TInt count( iAlertFor.Count() );
    for( TInt i( 0 ) ; i < count ; ++i )
        {
        TContactItemId id( iAlertFor[i] );
        TUint8 pieceOfId( ( iAlertFor[i] >> 24 ) & 0x000000FF );
        iAlertForBuf.Append( pieceOfId );
        pieceOfId = ( iAlertFor[i] >> 16 & 0x000000FF );
        iAlertForBuf.Append( pieceOfId );
        pieceOfId = ( iAlertFor[i] >> 8 & 0x000000FF );
        iAlertForBuf.Append( pieceOfId );
        pieceOfId = ( iAlertFor[i] & 0x000000FF );
        iAlertForBuf.Append( pieceOfId );
        }
    User::LeaveIfError(
            aCenRep.Set( ProfileEngUtils::ResolveKey( KProEngAlertForGroups,
            profileId ), iAlertForBuf ) );
    iAlertForBuf.Zero();

    // Tones and presence
    iProfileTones->ExternalizeL( aCenRep, profileId );
    iProfileExtraTones->ExternalizeL( aCenRep, profileId );
    iProfilePresence->ExternalizeL( aCenRep, profileId );
    iProfileExtraSettings->ExternalizeL( aCenRep, profileId );
    
    iProfileVibraSettings->ExternalizeL(  aCenRep, profileId );
    }

// -----------------------------------------------------------------------------
// CProfileImpl::SetAlertForL
//
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CProfileImpl::SetAlertForL( const TArray<TContactItemId>& aAlertFor )
    {
    TInt count( aAlertFor.Count() );
    if( count > KProfilesMaxAlertForGroups )
        {
        User::Leave( KErrArgument );
        }

    iAlertFor.Reset();

    for( TInt i( 0 ) ; i < count ; ++i )
        {
        User::LeaveIfError( iAlertFor.Append( aAlertFor[i] ) );
        }
    }

// -----------------------------------------------------------------------------
// CProfileImpl::CommonConstructL
//
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CProfileImpl::CommonConstructL()
    {
    RBuf toneBuf;
    toneBuf.CreateL( KMaxFileName );
    CleanupClosePushL( toneBuf );
    toneBuf.Copy( TParsePtrC( PathInfo::RomRootPath() ).Drive() );
    toneBuf.Append( KProfileNoSoundPath );

    iSilentTone = toneBuf.AllocL();
    CleanupStack::PopAndDestroy(); // toneBuf

    FeatureManager::InitializeLibL();
    iAlwaysOnLineEmail = FeatureManager::FeatureSupported( KFeatureIdAlwaysOnLineEmail );
    iOmaPoc = FeatureManager::FeatureSupported( KFeatureIdOmaPoc );
    FeatureManager::UnInitializeLib();
    }


// -----------------------------------------------------------------------------
// CProfileImpl::ProfileExtraTones2
//
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
const MProfileExtraTones2& CProfileImpl::ProfileExtraTones2() const 
    {
    return *iProfileExtraTones;
    }

// -----------------------------------------------------------------------------
// CProfileImpl::ProfileSetExtraTones2
//
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
MProfileSetExtraTones2& CProfileImpl::ProfileSetExtraTones2() 
    {
    return *iProfileExtraTones;
    }

// -----------------------------------------------------------------------------
// CProfileImpl::ProfileVibraSettings
//
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
const MProfileVibraSettings& CProfileImpl::ProfileVibraSettings() const
    {
    return  *iProfileVibraSettings;
    }

// -----------------------------------------------------------------------------
// CProfileImpl::ProfileSetVibraSettings
//
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
MProfileSetVibraSettings& CProfileImpl::ProfileSetVibraSettings()
    {
    return *iProfileVibraSettings;
    }

//  End of File

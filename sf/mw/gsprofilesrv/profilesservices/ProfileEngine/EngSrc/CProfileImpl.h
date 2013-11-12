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
* Description:  Profile implementation.
*
*/



#ifndef __CPROFILEIMPL_H__
#define __CPROFILEIMPL_H__

//  INCLUDES
#include <e32base.h>
#include <MProfileExtended2.h>
#include <centralrepository.h> // NCentralRepositoryConstants::KMaxBinaryLength

// CONSTANTS
// MACROS
// DATA TYPES
// FUNCTION PROTOTYPES

// FORWARD DECLARATIONS
class CProfileNameImpl;
class CProfileTonesImpl;
class CProfilePresenceImpl;
class CProfileExtraTonesImpl;
class CProfileExtraSettingsImpl;
class CPGFileGenerator;
class MProfilesNamesArray;
class MProfilesLocalFeatures;
struct TProfilePresenceSettings;
class CProfileVibraSettingsImpl;

// CLASS DECLARATION

/**
*  Profile implementation.
*
*  @lib ?library
*  @since 2.0
*/
NONSHARABLE_CLASS(CProfileImpl) : public CBase,
                     public MProfileExtended2
    {
    public:  // Constructors and destructor

        /**
        * Two-phased constructor.
        * @param aFs Ref. to file server session.
        */
        static CProfileImpl* NewL( RFs& aFs );

        /**
        * Two-phased constructor.
        * @param aFs Ref. to file server session.
        */
        static CProfileImpl* NewLC( RFs& aFs );

        /**
        * Two-phased constructor. Copy constructor.
        * @param aFs Ref. to file server session
        * @param aProfile Profile settings
        * @param aId Profile ID
        */
        static CProfileImpl* NewLC(
            RFs& aFs, const MProfileExtended2& aProfile, TInt aId );

        /**
        * Destructor.
        */
        virtual ~CProfileImpl();

    public: // New functions

        /**
        * Is profile names changed.
        * @since 2.0
        * @return ETrue if profile name is changed
        */
        TBool IsProfileNameChanged() const;

        /**
        * Set profile localized name.
        * @since 2.0
        * @param aNameArray Profiles names array
        */
        void SetLocalizedProfileNameL( const MProfilesNamesArray& aNameArray );

        /**
        * Set localised profile name.
        * @since 3.1
        * @param aNameImpl Profiles name impl from which the name and short name
        *        are copied into this Profile.
        * @param aUniquePart contains the part which is used to differentiate
        *        the automatically generated names from each other. This is
        *        merged in the name.
        */
        void SetLocalizedProfileNameL( const CProfileNameImpl& aNameImpl,
                                       const TDesC& aUniquePart );

        /**
        * Internalize data from the central repository.
        * @since 2.8
        * @param aCenRep Central Repository object to read the settings
        * @param aProfileId the id of the profile to be internalized
        */
        void InternalizeL( CRepository& aCenRep, TInt aProfileId );

    public: // Functions from base classes

        /**
        * From MProfile.
        */
        virtual void Release();

        /**
        * From MProfile.
        */
        virtual const TArray<TContactItemId> AlertForL();

        /**
        * From MProfile.
        */
        virtual TBool IsSilent() const;

        /**
        * From MProfile.
        */
        virtual const MProfileName& ProfileName() const;

        /**
        * From MProfile.
        */
        virtual const MProfileTones& ProfileTones() const;

        /**
        * From MProfile.
        */
        virtual const MProfileExtraTones& ProfileExtraTones() const;

        /**
        * From MProfileExtended.
        */
        virtual void SetAlertForL( const TArray<TContactItemId>& aAlertFor );

        /**
        * From MProfileExtended.
        */
        virtual MProfileSetName& ProfileSetName() const;

        /**
        * From MProfileExtended.
        */
        virtual MProfileSetTones& ProfileSetTones() const;

        /**
        * From MProfileExtended.
        */
        virtual const MProfilePresence& ProfilePresence() const;

        /**
        * From MProfileExtended.
        */
        virtual MProfileSetPresence& ProfileSetPresence() const;

        /**
        * From MProfileExtended.
        */
        virtual MProfileSetExtraTones& ProfileSetExtraTones() const;

        /**
        * From MProfileExtended.
        */
        virtual TUint32 ModifiableFlags() const;

        /**
        * From MProfileExtended.
        */
        virtual TUint32 VisibleFlags() const;

        /**
        * From MProfile.
        */
        virtual const MProfileExtraSettings& ProfileExtraSettings() const;

        /**
        * From MProfileExtended.
        */
        virtual MProfileSetExtraSettings& ProfileSetExtraSettings() const;
        
        
        /**
        * From MProfileExtended2.
        */
        virtual const MProfileExtraTones2& ProfileExtraTones2() const;
        
        /**
        * From MProfileExtended2.
        */
        virtual MProfileSetExtraTones2& ProfileSetExtraTones2();
        
        /**
        * From MProfileExtended2.
        */
        virtual const MProfileVibraSettings& ProfileVibraSettings() const;
        
        /**
        * From MProfileExtended2.
        */
        virtual MProfileSetVibraSettings& ProfileSetVibraSettings();

        /**
        * Externalize data to Central Repository.
        * @since 2.8
        * @param aCenRep Central Repository object to write the settings
        */
        void ExternalizeL( CRepository& aCenRep );

    private:

        /**
        * C++ default constructor.
        * aParam aFs Ref. to file server session
        */
        CProfileImpl( RFs& aFs );

        /**
        * By default Symbian 2nd phase constructor is private.
        */
        void ConstructL();

        /**
        * By default Symbian 2nd phase constructor is private.
        * @param aProfile Profile settings
        * @param aId Profile ID
        */
        void ConstructL( const MProfileExtended2& aProfile, TInt aId );

        /**
         * This method does the common part of the construction.
         */
        void CommonConstructL();

    private:    // Data

        // Own: Contact item array for alert for settings.
        RArray<TContactItemId> iAlertFor;

        // Own: Profile name implementation
        CProfileNameImpl* iProfileName;

        // Own: Profile tone settings implementation
        CProfileTonesImpl* iProfileTones;

        // Own: Profile extra tones settings implementation
        CProfileExtraTonesImpl* iProfileExtraTones;

        // Own: Profile extra settings implementation
        CProfileExtraSettingsImpl* iProfileExtraSettings;

        // Own: Profile presence settings implementation
        CProfilePresenceImpl* iProfilePresence;
        
        // Own: Profile vibra settings implementation
        CProfileVibraSettingsImpl *iProfileVibraSettings;

        // Own: Profile modifiable flags
        TUint32 iModifiableFlags;

        // Own: Profile visible flags
        TUint32 iVisibleFlags;

        // Ref: File Server session
        RFs& iFs;

        // Not owned: Locally variated features of Profiles:
        MProfilesLocalFeatures* iFeatures;

        // Own: Contains the path to the empty sound file:
        HBufC* iSilentTone;

        // Buffer for storing/reading alert for groups in/from CenRep:
        TBuf8< NCentralRepositoryConstants::KMaxBinaryLength > iAlertForBuf;

        // Supported features
        TBool iAlwaysOnLineEmail;
        TBool iOmaPoc;
    };

#endif      // __CPROFILEIMPL_H__

// End of File

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
* Description:  Profile engine implementation.
*
*/



#ifndef __CPROFILEENGINEIMPL_H__
#define __CPROFILEENGINEIMPL_H__

//  INCLUDES
#include <e32property.h>
#include <f32file.h>            // RFs
#include <Profile.hrh>          // TProfileRingingVolume 
#include <MProfileEngineExtended2.h> // MProfileEngineExtended2
#include <MProfileExtended.h> // MProfileExtended

// FORWARD DECLARATIONS
class CProfileNameImpl;
class CProfileImpl;
class CProfilesNamesArrayImpl;
class DRMCommon;
class MProfilesLocalFeatures;
class CRepository;
class TResourceReader;
class CProfileTiming;

// CLASS DECLARATION

/**
*  Profile engine implementation.
*
*  @lib ProfileEng
*  @since 2.0
*/
NONSHARABLE_CLASS(CProfileEngineImpl) : public CBase,
                           public MProfileEngineExtended2
    {
    public:  // Constructors and destructor

        /**
        * Two-phased constructor.
        */
        static CProfileEngineImpl* NewL();

        /**
        * Two-phased constructor.
        */
        static CProfileEngineImpl* NewLC();

        /**
        * Two-phased constructor.
        * @param aFs An open file server session. Ownership is NOT transferred.
        */
        static CProfileEngineImpl* NewL( RFs* aFs );

        /**
        * Two-phased constructor.
        * @param aFs An open file server session. Ownership is NOT transferred.
        */
        static CProfileEngineImpl* NewLC( RFs* aFs );

        /**
        * Destructor.
        */
        virtual ~CProfileEngineImpl();

    public: // Functions from base classes

        /**
        * From MProfileEngine.
        */
        virtual void Release();

        /**
        * From MProfileEngine.
        */
        virtual MProfile* ActiveProfileLC();

        /**
        * From MProfileEngine.
        */
        virtual MProfile* ActiveProfileL();

        /**
        * From MProfileEngine.
        */
        virtual TInt ActiveProfileId();

        /**
        * From MProfileEngine.
        */
        virtual void SetActiveProfileL( TInt aId );

        /**
        * From MProfileEngine.
        */
        virtual MProfilesNamesArray* ProfilesNamesArrayLC();

        /**
        * From MProfileEngineExtended.
        */
        virtual MProfileExtended* ProfileLC( TInt aId );

        /**
        * From MProfileEngineExtended.
        */
        virtual MProfileExtended* ProfileL( TInt aId );

        /**
        * From MProfileEngineExtended.
        */
        virtual void CommitChangeL( MProfileExtended& aProfile );

        /**
        * From MProfileEngineExtended.
        */
        virtual void SendPresenceSettingsL(
            const MProfilePresence& aPresence );

        /**
        * From MProfileEngineExtended.
        */
        virtual TInt DeleteProfileL( TInt aId );

        /**
        * From MProfileEngineExtended.
        */
        virtual MProfileExtended* CreateProfileL();

        /**
        * From MProfileEngineExtended.
        */
        virtual TBool IsDefaultProfile( TInt aId );

        /**
        * From MProfileEngine.
        */
        void SetTempRingingVolumeL( TProfileRingingVolume aVolume );

        /**
        * From MProfileEngine.
        */
        TProfileRingingVolume TempRingingVolumeL() const;

        /**
        * From MProfileEngine.
        */
        void SetTempMediaVolumeL( TProfileRingingVolume aVolume );

        /**
        * From MProfileEngine.
        */
        TProfileRingingVolume TempMediaVolumeL() const;

        /**
        * From MProfileEngineExtended.
        */
        MProfilesLocalFeatures& LocalFeatures();

        /**
        * From MProfileEngine.
        */
        TBool IsFeatureSupported( TProfileFeatureId aFeatureId ) const;

        /**
        * From MProfileEngineExtended.
        */
        void GetProfileUtilityObject( TProfileUtilityObjectType aType, TAny*& aImpl );

        /**
        * From MProfileEngine.
        */
        TBool IsActiveProfileTimedL();

        /**
        * From MProfileEngineExtended2.
        */
        void SetActiveProfileTimedL( TInt aId, TTime aTime );
        
        
        /**
        * From MProfileEngineExtended2.
        */
        virtual MProfileExtended2* Profile2LC( TInt aId );

        /**
        * From MProfileEngineExtended2.
        */
        virtual MProfileExtended2* Profile2L( TInt aId );

        /**
        * From MProfileEngineExtended2.
        */
        virtual void CommitChange2L( MProfileExtended2& aProfile );
        
        
        /**
        * From MProfileEngineExtended2.
        */
        virtual TInt MasterVolumeL() const;
        
        /**
        * From MProfileEngineExtended2.
        */
        virtual void SetMasterVolumeL( TInt aMasterVolume );
        
        /**
        * From MProfileEngineExtended2.
        */
        virtual TBool MasterVibraL() const;
        
        /**
        * From MProfileEngineExtended2.
        */
        virtual void SetMasterVibraL( TBool aMasterVibra );
        
        /**
        * From MProfileEngineExtended2.
        */
        virtual TBool SilenceModeL() const;
        
        /**
        * From MProfileEngineExtended2.
        */
        virtual void SetSilenceModeL( TBool aSilenceMode );

    protected:  // New functions

        /**
        * Reads the resource with the given id from Profiles Engine resource
        * file.
        * @param aResourceId the id of the resource to be read
        * @param aReader the resource reader object in which the buffer
        * containing the resource is set
        */
        void ReadResourceInBufferLC( TInt aResourceId,
                                     TResourceReader& aReader );

        /**
        * Read default profiles names from resource.
        */
        void ReadProfilesNamesFromResourceL();

        /**
        * Write profile settings to Central Repository.
        * @since 2.8
        * @param aProfile Profile
        */
        void WriteProfileL( MProfileExtended& aProfile );

        /**
        * Send an event indicating that a setting in the active profile was
        * modified through Publish & Subscribe.
        * @param aPSKey the P&S key which value should be changed
        * @since 3.0
        */
        void PublishChangeL( TUint32 aPSKey );

        /**
        * Update Active Profile settings in Central Repository.
        * @since 2.8
        * @param aProfile Profile
        * @param aSetId sets the id of the active profile to Central Repository
        * only if this is ETrue
        */
        void UpdateActiveProfileSettingsL( MProfileExtended2& aProfile,
                                           TBool aSetId = EFalse );

        /**
         * Updates the alert for groups shared data value.
         * @since 2.6
         * @param aProfile settings containing the new alert for groups.
         */
        void UpdateContactGroupsValueL( MProfile& aProfile );
            
        /**
        * Get profile settings.
        * @since 2.0
        * @param aId Profile ID
        * @return Profile instance
        */
        CProfileImpl* GetProfileL( TInt aId );

        /**
        * Read profiles names.
        * @since 2.0
        * @param aProfilePath Profile path where profiles readed
        * @param aProfilesNames Profile names array
        */
        void ReadProfilesNamesL( CProfilesNamesArrayImpl* aProfilesNames );

        /**
        * Read new profile default name from resource.
        * @param aResourceId resource id containing the profile name or
        *        short name.
        * @param aProfileName Buffer for default profile name.
        * @since 2.1
        */
        void ReadNewProfileNameFromResourceL( TInt aResourceId,
                                              TDes& aProfileName );

        /**
         * Helper function for setting the active profile. Wraps several
         * leaving calls inside.
         * @since 2.5
         * @param aId the id of the profile to be set as active profile.
         * @param aTime the time when the new profile should be deactivated
         */
        void DoSetActiveProfileL( TInt aId, TTime* aTime = NULL );

        /**
         * Helper method for SetTempRingingVolumeL() and SetTempMediaVolumeL()
         * to set the needed volume value to Central Repository.
         * @since 2.8
         * @param aCenRep the central repository object to write the new
         *        temporary volume
         * @param aCenRepKey the key to the volume value in CentralRepository
         * @param aVolume the new value to be set for the volume
         */
        void SetTempVolumeL( CRepository& aCenRep, TUint32 aCenRepKey,
                             TProfileRingingVolume aVolume );

        /**
         * Helper method for TempRingingVolumeL() and TempMediaVolumeL() to
         * get the needed volume value from Central Repository.
         * @since 2.6
         * @param aCenRep reference to the CenRep instance containing the data
         * @param aCenRepKey the key to the volume value in Central Repository
         */
        TProfileRingingVolume TempVolumeL( CRepository& aCenRep,
                                           TUint32 aCenRepKey ) const;

        /**
         * Helper method for SetTempMediaVolumeL() and SetTempRingingVolumeL()
         * to check and leave with KErrAccessDenied if the corresponding
         * setting item is read-only.
         * @since 2.8
         * @param aFlag the flag corresponding to the setting item to be
         * checked for modifiability.
         */
        void LeaveIfReadOnlyL( TProfileFlags aFlag );

        /**
         * Helper method to check whether a client-given profile ID belongs to
         * an existing profile.
         * @since 3.0
         * @param aId profile ID to check
         * @return KErrNotFound if a profile with the given ID is not found,
         *         KErrNone otherwise.
         */

        TInt CheckProfileIdL( TInt aId );

        /**
         * Creates the Publish & Subscribe keys needed for notifying about
         * active profile changes if the keys aren't already created.
         * @since 3.1
         */
        void CreatePubSubKeysIfNeededL();

        /**
         * Reads names of user-created profiles.
         * @param aProfilesNames Profiles Names Array to be updated with the
         *        user-created Profile Names.
         * @param aNameTemplate CProfileNameImpl to be reused when reading name
         *        data from Profiles Central Repository.
         * @since 3.1
         */
        void ReadDynamicProfilesNamesL( CProfilesNamesArrayImpl* aProfilesNames,
                                        CProfileNameImpl& aNameTemplate );

        /**
         * Reads both the normal and short versions of the name of a new profile
         * from the resources.
         * @return Pointer to CProfileNameImpl wrapping the normal and short
         *         names of a new profile.
         * @since 3.1
         */
        CProfileNameImpl* ReadNewProfileNameFromResourceLC();

        /**
         * Sets the localized name for a dynamic (= user-created) profile.
         * @param aProfile the profile object into which the localized name
         *        should be set.
         * @since 3.1
         */
        void SetLocalizedNameForDynamicProfileL( CProfileImpl& aProfile );
             
    private:
        TBool PowerSaveMode();
        TBool PowerSaveModeL();
        
    private:

        /**
        * By default Symbian 2nd phase constructor is private.
        */
        void ConstructL();

        /**
        * By default Symbian 2nd phase constructor is private.
        */
        void ConstructL( RFs* aFs );

        /**
        * Common part used by both ConstructL() methods above.
        */
        void CommonConstructL();

    private:    // Data
        // Own: resource file name is stored here (to avoid stack overflow)
        TFileName iFileName;

        // Own or Ref: File server session
        RFs* iFs;

        // Indicates whether the file server session is owned or not
        TBool iFsOwned;

        // Own: Central Repository handle for Profiles repository
        CRepository* iCenRep;

        // Own: Central Repository handle for VibraCntrl repository
        CRepository* iVibraCenRep;

        // Own: Publish & Subscribe client:
        RProperty iProperty;

        // Own: Localised profile names
        CProfilesNamesArrayImpl* iProfileLocalisedNames;

        // Own: Handle to a mutex
        RMutex iMutex;

        // Not owned: Locally variated features of Profiles:
        MProfilesLocalFeatures* iFeatures;

        // Indicates whether the Publish & Subscribe keys has been created:
        TBool iPubSubKeysCreated;

        // Own: functionality for timing of profiles
        CProfileTiming* iProfileTiming;

        // Supported features
        TBool iSideVolumeKeys;
    };

#endif      //  __CPROFILEENGINEIMPL_H__

// End of File

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
* Description:  Profile Engine interface.
*
*/



#ifndef __MPROFILEENGINE_H__
#define __MPROFILEENGINE_H__

// INCLUDES
#include <e32std.h>
#include <Profile.hrh>          // TProfileRingingVolume 

// DATA TYPES
enum TProfileFeatureId
    {
    EProfileFeatureVTRingingTone,
    EProfileFeatureWmaDemoPlay  // if this is true, demo-play is DISABLED
    };

// FUNCTION PROTOTYPES
class MProfileEngine;
class RFs;

/**
* Create profile engine, ownership transfered.
*
*  @lib ProfileEng.lib
*  @since 2.0
*/
IMPORT_C MProfileEngine* CreateProfileEngineL();

/**
* Create profile engine, ownership transfered.
*
*  @param aFs An open file server session, ownership is NOT transferred.
*  aFs must remain connected for the lifetime of the returned object.
*  @lib ProfileEng.lib
*  @since 3.0
*/
IMPORT_C MProfileEngine* CreateProfileEngineL( RFs* aFs );

// FORWARD DECLARATIONS
class MProfile;
class MProfilesNamesArray;

// CLASS DECLARATION

/**
*  Profile engine interface.
*  This class offers methods to get active profile, active profile ID,
*  profile names array and set active profile.
*  How to use:
*  Create profile engine with CreateProfileEngineL();
*      MProfileEngine* profileEngine = CreateProfileEngineL();
*  Free resources with MProfileEngine::Release();
*      profileEngine->Release();
*  or if you release resources in destructor:
*      ( iProfileEngine )
*          {
*          iProfileEngine->Release();
*          }
*
*  If you put this class to CleanupStack then use void CleanupReleasePushL()
*     CleanupReleasePushL( *profileEngine );
*
*
*  @lib N/A
*  @since 2.0
*/
class MProfileEngine
    {
    protected:  // Destructor

        virtual ~MProfileEngine() {};

    public:

        /**
        * Frees resources.
        * @since 2.0
        */
        virtual void Release() = 0;

        /**
        * Return active profile, ownership transfered.
        * @since 2.0
        * @return Instance of the MProfile
        */
        virtual MProfile* ActiveProfileLC() = 0;

        /**
        * Return active profile, ownership transfered.
        * @since 2.0
        * @return Instance of the MProfile
        */
        virtual MProfile* ActiveProfileL() = 0;

        /**
        * Return active profile ID or system error code.
        * @since 2.0
        * @return Active profile ID or system error code.
        */
        virtual TInt ActiveProfileId() = 0;

        /**
        * Return profiles names array, ownership transfered.
        * @since 2.0
        * @return Instance of the profiles names array
        */
        virtual MProfilesNamesArray* ProfilesNamesArrayLC() = 0;

        /**
        * Set active profile if ID is incorrect then leave with KErrNotFound.
        * @since 2.0
        * @param aId Profile ID
        */
        virtual void SetActiveProfileL( TInt aId ) = 0;

        /**
         * Sets the temporary ringing volume of the current active profile to
         * the given value. Temporary means that if another profile is
         * activated and after that the original is reactivated the temporary
         * value will be lost. Leaves with KErrAccessDenied if the ringing
         * volume setting of the current profile is read-only.
         * @since 2.6
         * @param aVolume the new value for the temporary ringing volume level.
         */
        virtual void SetTempRingingVolumeL( TProfileRingingVolume aVolume ) = 0;

        /**
         * Gets the temporary ringing volume of the current active profile.
         * Temporary means that if another profile is activated and after
         * that the original is reactivated the temporary value will be lost.
         * @since 2.6
         * @return the temporary ringing volume level of the active profile.
         */
        virtual TProfileRingingVolume TempRingingVolumeL() const = 0;

        /**
         * Sets the temporary media volume of the current active profile to
         * the given value. Temporary means that if another profile is
         * activated and after that the original is reactivated the temporary
         * value will be lost. Leaves with KErrAccessDenied if the media
         * volume setting of the current profile is read-only.
         * @since 2.6
         * @param aVolume the new value for the temporary media volume level.
         */
        virtual void SetTempMediaVolumeL( TProfileRingingVolume aVolume ) = 0;

        /**
         * Gets the temporary media volume of the current active profile.
         * Temporary means that if another profile is activated and after
         * that the original is reactivated the temporary value will be lost.
         * @since 2.6
         * @return the temporary media volume level of the active profile.
         */
        virtual TProfileRingingVolume TempMediaVolumeL() const = 0;

        /**
         * Checks whether the given feature is supported by Profiles Engine.
         * @since 2.8
         * @param aFeatureId the feature to be checked.
         * @return ETrue if the given feature is supported, EFalse otherwise.
         */
        virtual TBool IsFeatureSupported(
            TProfileFeatureId aFeatureId ) const = 0;

        /**
        * Checks if the current active profile is timed or not.
        * @since 3.2
        * @return ETrue if the current active profile is timed, EFalse
        * otherwise.
        */
        virtual TBool IsActiveProfileTimedL() = 0;

    };

#endif      //  __MPROFILEENGINE_H__

// End of File

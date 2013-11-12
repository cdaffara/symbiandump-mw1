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
* Description:  Extended profile engine interface.
*
*/



#ifndef __MPROFILEENGINEEXTENDED_H__
#define __MPROFILEENGINEEXTENDED_H__

//  INCLUDES
#include <MProfileEngine.h>

// DATA TYPES
enum TProfileUtilityObjectType
    {
    EProfileUtilityObjectPresenceTool      = 0
    };

// FUNCTION PROTOTYPES
class MProfileEngineExtended;
class MProfilesLocalFeatures;

/**
* Create extended profile engine, ownership transfered.
*
*  @lib ProfileEng.lib
*  @since 2.0
*/
IMPORT_C MProfileEngineExtended* CreateProfileEngineExtendedL();

/**
* Create extended profile engine, ownership transfered.
*
*  @param aFs An open file server session, ownership is NOT transferred.
*  aFs must remain connected for the lifetime of the returned object.
*  @lib ProfileEng.lib
*  @since 3.0
*/
IMPORT_C MProfileEngineExtended* CreateProfileEngineExtendedL( RFs* aFs );

// FORWARD DECLARATIONS
class MProfileExtended;
class MProfilePresence;

// CLASS DECLARATION

/**
*  Extended profile engine interface.
*  This interface extend MProfileEngine interface and
*  offers methods to get profile settings and store profile settings.
*
*  @lib N/A
*  @since 2.0
*/
class MProfileEngineExtended : public MProfileEngine
    {
    protected:  // Destructor

        virtual ~MProfileEngineExtended() {};

    public: // New functions

        /**
        * Create extended profile, ownership transfered.
        * @since 2.0
        * @param aId Profile ID
        * @return Instance of the MProfileExtended
        */
        virtual MProfileExtended* ProfileLC( TInt aId ) = 0;

        /**
        * Create extended profile, ownership transfered.
        * @since 2.0
        * @param aId Profile ID
        * @return Instance of the MProfileExtended
        */
        virtual MProfileExtended* ProfileL( TInt aId ) = 0;

        /**
        * Store profile settings. Leaves with KErrAccessDenied if one or more
        * of the settings of the target profile is/are read-only.
        * @since 2.0
        * @param aProfile Profile
        */
        virtual void CommitChangeL( MProfileExtended& aProfile ) = 0;

        /**
        * Send presence settings.
        * @since 2.1
        * @param aProfile Profile
        */
        virtual void SendPresenceSettingsL(
            const MProfilePresence& aPresence ) = 0;

        /**
        * Delete profile.
        * @since 2.1
        * @param aId Profile ID
        * @return Error code
        */
        virtual TInt DeleteProfileL( TInt aId ) = 0;

        /**
        * Create new profile, ownership transfered.
        * @since 2.1
        * @return Instance of the MProfileExtended
        */
        virtual MProfileExtended* CreateProfileL() = 0;

        /**
        * Returns is profile default profile.
        * @since 2.0
        * @param aId Profile ID
        * @return Is profile default profile
        */
        virtual TBool IsDefaultProfile( TInt aId ) = 0;

        /**
        * Returns locally variated features of Profiles.
        * @since 2.6
        */
        virtual MProfilesLocalFeatures& LocalFeatures() = 0;

        /**
         * Returns an utility object needed for a specific task. The returned
         * object can be generally of any class and the caller must know what
         * it wants and cast the returned object to the correct class.
         * The ownership is not transferred.
         * @param aType the type of the utility object to be returned.
         * @param aImpl the utility object needed by the client on return, NULL
         * if there isn't any of the given type
         */
        virtual void GetProfileUtilityObject(
                TProfileUtilityObjectType aType, TAny*& aImpl ) = 0;

        /**
        * Set timed active profile. When the time expires the profile which was
        * active before calling this method will be reactivated.
        * If ID is incorrect then leave with KErrNotFound.
        * @since 3.2
        * @param aId Profile ID
        * @param aTime time when the profile activation expires
        */
        virtual void SetActiveProfileTimedL( TInt aId, TTime aTime ) = 0;

    };

#endif      // __MPROFILEENGINEEXTENDED_H__

// End of File

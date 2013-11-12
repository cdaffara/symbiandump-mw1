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
* Description:  Profile settings interface.
*
*/



#ifndef __MPROFILE_H__
#define __MPROFILE_H__

//  INCLUDES
#include <cntdef.h> // TContactItemId

// FORWARD DECLARATIONS
class MProfileName;
class MProfileTones;
class MProfileExtraTones;
class MProfileExtraSettings;

// CLASS DECLARATION

/**
*  Profile settigns interface.
*  MProfile offers methods to get the alert for items, profile name interface,
*  profile tones interface and profile extra tones interface.
*  How to use:
*  Get active profile with MProfileEngine::ActiveProfileL().
*     MProfile* profile = iProfileEngine->ActiveProfileL();
*  Free resources with MProfile::Release()
*     profile->Release();
*  If you release resources in destructor then:
*     if( iProfile )
*         {
*         iProfile->Release();
*         }
*
*  If you put this class to CleanupStack then use void CleanupReleasePushL()
*     CleanupReleasePushL( *profile );
*
*  @lib N/A
*  @since 2.0
*/
class MProfile
    {
    protected:  // Destructor

        virtual ~MProfile() {};

    public: // New functions

        /**
        * Free resources of the profile.
        * @since 2.0
        */
        virtual void Release() = 0;

        /**
        * Return the alert for item array.
        * @since 2.0
        * @return Alert for array
        */
        virtual const TArray<TContactItemId> AlertForL() = 0;

        /**
        * Is silent profile.
        * @since 2.0
        * @return ETrue if profile is silent
        */
        virtual TBool IsSilent() const = 0;

        /**
        * Return profile name.
        * @since 2.0
        * @return Instance of the profile name
        */
        virtual const MProfileName& ProfileName() const = 0;

        /**
        * Return tone settings.
        * @since 2.0
        * @return Instance of the MProfileTones
        */
        virtual const MProfileTones& ProfileTones() const = 0;

        /**
        * Return extra tone settings.
        * @since 2.0
        * @return Instance of the MProfileExtraTones
        */
        virtual const MProfileExtraTones& ProfileExtraTones() const = 0;

        /**
        * Return Profiles extra settings.
        * @since 3.0
        * @return Instance of the MProfileExtraSettings
        */
        virtual const MProfileExtraSettings& ProfileExtraSettings() const = 0;

    };

#endif      //  __MPROFILE_H__

// End of File

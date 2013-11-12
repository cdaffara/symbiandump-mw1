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
* Description:  Profile extra tones implementation.
*
*/



#ifndef __CPROFILEEXTRASETTINGSIMPL_H
#define __CPROFILEEXTRASETTINGSIMPL_H

//  INCLUDES
#include <e32base.h>
#include <MProfileExtraSettings.h>
#include <MProfileSetExtraSettings.h>

// FORWARD DECLARATIONS
class CRepository;
class CProfilePttSettingsImpl;
class CProfileFeedbackSettingsImpl;
class CProfile3DToneSettingsImpl;

// CLASS DECLARATION

/**
*  Profile extra tones implementation.
*
*  @lib ?library
*  @since 2.0
*/
NONSHARABLE_CLASS(CProfileExtraSettingsImpl) : public CBase,
                               public MProfileExtraSettings,
                               public MProfileSetExtraSettings
    {
    public:  // Constructors and destructor

        /**
        * Two-phased constructor.
        */
        static CProfileExtraSettingsImpl* NewL();

        /**
        * Destructor.
        */
        virtual ~CProfileExtraSettingsImpl();

    public: // New functions

        /**
        * Internalize data from the central repository.
        * @since 3.0
        * @param aCenRep Central Repository object to read the settings
        * @param aProfileId the id of the profile to be internalized
        */
        void InternalizeL( CRepository& aCenRep, TInt aProfileId );

        /**
        * Externalize data to Central Repository.
        * @since 3.0
        * @param aCenRep Central Repository object to write the settings
        */
        void ExternalizeL( CRepository& aCenRep, TInt aProfileId ) const;

    public: // Functions from base classes

        /**
        * From MProfileExtraSettings.
         */
        virtual const MProfilePttSettings& ProfilePttSettings() const;

        /**
        * From MProfileExtraSettings.
        */
        virtual const MProfileFeedbackSettings& ProfileFeedbackSettings() const;

        /**
        * From MProfileExtraSettings.
        */
        virtual const MProfile3DToneSettings& Profile3DToneSettings() const;

        /**
        * From MProfileSetExtraSettings.
        */
        virtual MProfileSetPttSettings& ProfileSetPttSettings() const;

        /**
        * From MProfileSetExtraSettings.
        */
        virtual MProfileSetFeedbackSettings& ProfileSetFeedbackSettings() const;

        /**
        * From MProfileSetExtraSettings.
        */
        virtual MProfileSet3DToneSettings& ProfileSet3DToneSettings() const;

    private:

        /**
        * By default Symbian 2nd phase constructor is private.
        */
        void ConstructL();

    private: // Data

        // Own: Push to talk settings
        CProfilePttSettingsImpl* iPttSettings;

        // Own: feedback settings
        CProfileFeedbackSettingsImpl* iFeedbackSettings;

        // Own: 3D tone settings
        CProfile3DToneSettingsImpl* i3DToneSettings;

    };

#endif      // __CPROFILEEXTRASETTINGSIMPL_H

// End of File


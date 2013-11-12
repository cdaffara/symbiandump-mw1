/*
* Copyright (c) 2007 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Profile Feedback settings implementation.
*
*/



#ifndef __CPROFILEFEEDBACKSETTINGSIMPL_H
#define __CPROFILEFEEDBACKSETTINGSIMPL_H

//  INCLUDES
#include <e32base.h>
#include <MProfileFeedbackSettings.h>
#include <MProfileSetFeedbackSettings.h>

// FORWARD DECLARATIONS
class CRepository;

// CLASS DECLARATION

/**
*  Profile feedback settings implementation.
*
*  @lib ProfileEng.lib
*  @since 5.0
*/
NONSHARABLE_CLASS(CProfileFeedbackSettingsImpl) : public CBase,
                               public MProfileFeedbackSettings,
                               public MProfileSetFeedbackSettings
    {
    public:  // Constructors and destructor

        /**
        * Two-phased constructor.
        */
        static CProfileFeedbackSettingsImpl* NewL();
        static CProfileFeedbackSettingsImpl* NewL(
                const MProfileFeedbackSettings& aProfileFeedbackSettings );

        /**
        * Destructor.
        */
        virtual ~CProfileFeedbackSettingsImpl();

    public: // New functions

        /**
        * Internalize data from the central repository.
        * @since 5.0
        * @param aCenRep Central Repository object to read the settings
        * @param aProfileId the id of the profile to be internalized
        */
        void InternalizeL( CRepository& aCenRep, TInt aProfileId );

        /**
        * Externalize data to Central Repository.
        * @since 5.0
        * @param aCenRep Central Repository object to write the settings
        */
        void ExternalizeL( CRepository& aCenRep, TInt aProfileId ) const;

    public: // Functions from base classes

        /**
        * From MProfileFeedbackSettings.
        */
        TProfileTactileFeedback TactileFeedback() const;

        /**
        * From MProfileFeedbackSettings.
        */
        TProfileAudioFeedback AudioFeedback() const;

        /**
        * From MProfileSetFeedbackSettings.
        */
        void SetTactileFeedback( TProfileTactileFeedback aTactileFeedback );

        /**
        * From MProfileSetFeedbackSettings.
        */
        void SetAudioFeedback( TProfileAudioFeedback aAudioFeedback );

    private:
        void InitPowerSaveMode();
        void ReadPowerSaveConfigL();

    private:    // Data

        // Tactile feedback
        TProfileTactileFeedback iTactileFeedback;

        // Audio feedback
        TProfileAudioFeedback iAudioFeedback;
        
        TInt iPsmConfigTactileFeedback;
        TInt iPsmConfigAudioFeedback;
        TBool iPowerSaveMode;
    };

#endif      // __CPROFILEFEEDBACKSETTINGSIMPL_H

// End of File



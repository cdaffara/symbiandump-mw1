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
* Description:  Profile tactile/audio feedback settings interface.
*
*/



#ifndef MPROFILEFEEDBACKSETTINGS_H
#define MPROFILEFEEDBACKSETTINGS_H

//  INCLUDES
#include <ProfileInternal.hrh>

// CLASS DECLARATION

/**
*  Profile feedback settings interface.
*  @since 3.1
*/
class MProfileFeedbackSettings
    {
    protected:  // Destructor

        virtual ~MProfileFeedbackSettings() {};

    public: // New functions

        /**
        * Returns Tactile feedback status of the profile.
        * @since 5.0
        * @return Tactile feedback status of the profile.
        */
        virtual TProfileTactileFeedback TactileFeedback() const = 0;

        /**
         * Returns Audio feedback status of the profile.
         * @since 5.0
         * @return Audio feedback status of the profile.
         */
        virtual TProfileAudioFeedback AudioFeedback() const = 0;

    };

#endif      //  MPROFILEFEEDBACKSETTINGS_H

// End of File


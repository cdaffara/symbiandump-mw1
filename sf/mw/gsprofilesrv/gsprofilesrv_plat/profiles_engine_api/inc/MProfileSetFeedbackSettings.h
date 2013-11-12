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
* Description:  Profile set feedback settings interface.
*
*/



#ifndef MPROFILESETFEEDBACKSETTINGS_H
#define MPROFILESETFEEDBACKSETTINGS_H

//  INCLUDES
#include <e32def.h>

// FORWARD DECLARATIONS

// CLASS DECLARATION

/**
*  Profile set feedback settings interface.
*  @since 5.0
*/
class MProfileSetFeedbackSettings
    {
    protected:  // Destructor

        virtual ~MProfileSetFeedbackSettings() {};

    public: // New functions

        /**
        * Set tactile feedback of the profile.
        * @since 5.0
        */
        virtual void SetTactileFeedback( TProfileTactileFeedback aTactileFeedback ) = 0;

        /**
         * Set audio feedback of the profile.
         * @since 5.0
         */
        virtual void SetAudioFeedback( TProfileAudioFeedback aAudioFeedback ) = 0;

    };

#endif      //  MPROFILESETFEEDBACKSETTINGS_H

// End of File




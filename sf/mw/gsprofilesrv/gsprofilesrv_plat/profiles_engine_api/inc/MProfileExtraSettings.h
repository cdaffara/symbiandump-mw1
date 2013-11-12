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



#ifndef MPROFILEEXTRASETTINGS_H
#define MPROFILEEXTRASETTINGS_H

//  INCLUDES
#include <e32def.h>

// FORWARD DECLARATIONS
class MProfilePttSettings;
class MProfileFeedbackSettings;
class MProfile3DToneSettings;

// CLASS DECLARATION

/**
*  Profile extra settings interface.
*  @since 3.1
*/
class MProfileExtraSettings
    {
    protected:  // Destructor

        virtual ~MProfileExtraSettings() {};

    public: // New functions

        /**
         * Returns MProfilePttSettings set for this profile.
         * @return MProfilePttSettings set for this profile.
         * @since 3.1
         */
        virtual const MProfilePttSettings& ProfilePttSettings() const = 0;

       /**
         * Returns MProfileFeedbackSettings set for this profile.
         * @return MProfileFeeedbackSettings set for this profile.
         * @since 5.0
         */
        virtual const MProfileFeedbackSettings& ProfileFeedbackSettings() const = 0;

       /**
         * Returns MProfile3DToneSettings set for this profile.
         * @return MProfile3DToneSettings set for this profile.
         * @since 3.1
         */
        virtual const MProfile3DToneSettings& Profile3DToneSettings() const = 0;
    };

#endif      //  MPROFILEEXTRASETTINGS_H

// End of File


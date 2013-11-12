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



#ifndef MPROFILESETEXTRASETTINGS_H
#define MPROFILESETEXTRASETTINGS_H

//  INCLUDES
#include <e32def.h>

// FORWARD DECLARATIONS
class MProfileSetPttSettings;
class MProfileSetFeedbackSettings;
class MProfileSet3DToneSettings;

// CLASS DECLARATION

/**
*  Interface for modifying Profiles extra settings.
*  @since 3.1
*/
class MProfileSetExtraSettings
    {
    protected:  // Destructor

        virtual ~MProfileSetExtraSettings() {};

    public: // New functions

        /**
         * Returns set profile ptt settings interface.
         * @return set profile ptt settings interface.
         * @since 3.1
         */
        virtual MProfileSetPttSettings& ProfileSetPttSettings() const = 0;

        /**
         * Returns set profile feedback settings interface.
         * @return set profile feedback settings interface.
         * @since 5.0
         */
        virtual MProfileSetFeedbackSettings& ProfileSetFeedbackSettings() const = 0;

        /**
         * Returns set profile 3D tone settings interface.
         * @return set profile 3D tone settings interface.
         * @since 3.1
         */
        virtual MProfileSet3DToneSettings& ProfileSet3DToneSettings() const = 0;

    };

#endif      //  MPROFILESETEXTRASETTINGS_H

// End of File



/*
* Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Extended profile interface.
*
*/



#ifndef __MPROFILEEXTENDED2_H__
#define __MPROFILEEXTENDED2_H__

//  INCLUDES
#include <MProfileExtended.h>

// DATA TYPES

// FORWARD DECLARATIONS

class MProfileExtraTones2;
class MProfileSetExtraTones2;
class MProfileVibraSettings;
class MProfileSetVibraSettings;

// CLASS DECLARATION

/**
*  Extended profile interface.
*  This interface extend MProfile interface.
*
*  @lib N/A
*  @since 10.1
*/
class MProfileExtended2 : public MProfileExtended
    {
    protected:  // Destructor

        virtual ~MProfileExtended2() {};

    public: // New functions

        /**
        * Returns profile extra Tones 2 settings interface.
        * @since 10.1
        * @return get rofile Extra Tones 2 settings
        */
        virtual const MProfileExtraTones2& ProfileExtraTones2() const = 0;
        
        /**
        * Returns profile set extra tones 2 settings interface.
        * @since 10.1
        * @return set profile extra tones 2 settings interface.
        */
        virtual MProfileSetExtraTones2& ProfileSetExtraTones2() = 0;
        
        /**
        * Returns profile vibra settings interface.
        * @since 10.1
        * @return get profile vibra settings.
        */
        virtual const MProfileVibraSettings& ProfileVibraSettings() const = 0;
        
        /**
        * Returns set vibra settings interface.
        * @since 10.1
        * @return set profile vibra settings.
        */
        virtual MProfileSetVibraSettings& ProfileSetVibraSettings() = 0;

    };

#endif      // __MPROFILEEXTENDED_H__

// End of File

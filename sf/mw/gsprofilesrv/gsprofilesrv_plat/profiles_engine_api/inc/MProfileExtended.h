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
* Description:  Extended profile interface.
*
*/



#ifndef __MPROFILEEXTENDED_H__
#define __MPROFILEEXTENDED_H__

//  INCLUDES
#include <MProfile.h>

// DATA TYPES
enum TProfileFlags
    {
    EProfileFlagRingingTone =               0x0001,
    EProfileFlagRingingType =               0x0002,
    EProfileFlagRingingVolume =             0x0004,
    EProfileFlagMessageAlertTone =          0x0008,
    EProfileFlagInstantMessageAlertTone =   0x0010,
    EProfileFlagVibratingAlert =            0x0020,
    EProfileFlagKeypadVolume =              0x0040,
    EProfileFlagWarningTones =              0x0080,
    EProfileFlagAlertFor =                  0x0100,
    EProfileFlagAvailabilityAttribute =     0x0200,
    EProfileFlagStatusMessage =             0x0400,
    EProfileFlagProfileName =               0x0800,
    EProfileFlagEmailAlertTone =            0x1000,
    EProfileFlagVideoCallRingingTone =      0x2000,
    EProfileFlagMediaVolume =               0x4000,
    EProfileFlagTextToSpeechTone =          0x8000,
//  Free slot              =               0x10000,
    EProfileFlagPttTone =                  0x20000,
    EProfileFlagPttStatus =                0x40000,
    EProfileFlag3DEffect =                 0x80000,
    EProfileFlag3DEcho =                  0x100000,
    EProfileFlagTactileFeedback =         0x200000,
    EProfileFlagAudioFeedback =           0x400000,
    EProfileFlagAll =                       0xFFFF
    };

// FORWARD DECLARATIONS
class MProfileSetName;
class MProfileSetTones;
class MProfilePresence;
class MProfileSetPresence;
class MProfileSetExtraTones;
class MProfileSetExtraSettings;
class CRepository;

// CLASS DECLARATION

/**
*  Extended profile interface.
*  This interface extend MProfile interface.
*
*  @lib N/A
*  @since 2.0
*/
class MProfileExtended : public MProfile
    {
    protected:  // Destructor

        virtual ~MProfileExtended() {};

    public: // New functions

        /**
        * This method set alert for items to the profile.
        * @since 2.0
        * @param aAlertFor Alert for item array.
        */
        virtual void SetAlertForL(
            const TArray<TContactItemId>& aAlertFor ) = 0;

        /**
        * Returns set profile name interface.
        * @since 2.0
        * @return Returns set profile name interface.
        */
        virtual MProfileSetName& ProfileSetName() const = 0;

        /**
        * Returns set profile tones interface.
        * @since 2.0
        * @return Returns set profile tones interface.
        */
        virtual MProfileSetTones& ProfileSetTones() const = 0;

        /**
        * Returns profile presence interface.
        * @since 2.0
        * @return Returns profile presence interface.
        */
        virtual const MProfilePresence& ProfilePresence() const = 0;

        /**
        * Returns set profile presence interface.
        * @since 2.0
        * @return Returns set profile presence interface.
        */
        virtual MProfileSetPresence& ProfileSetPresence() const = 0;

        /**
        * Returns set profile extra tones interface.
        * @since 2.0
        * @return Returns set profile extra tones interface.
        */
        virtual MProfileSetExtraTones& ProfileSetExtraTones() const = 0;

        /**
        * Returns profile modifiable flags. See TProfileFlags.
        * @since 2.0
        * @return Returns profile modifiable flags.
        */
        virtual TUint32 ModifiableFlags() const = 0;

        /**
        * Returns profile visible flags. See TProfileFlags.
        * @since 2.0
        * @return Returns profile visible flags.
        */
        virtual TUint32 VisibleFlags() const = 0;

        /**
        * Externalize data to Central Repository.
        * @since 2.8
        * @param aCenRep Central Repository object to write the settings
        */
        virtual void ExternalizeL( CRepository& aCenRep ) = 0;

        /**
        * Returns set profile extra settings interface.
        * @since 3.0
        * @return set profile extra settings interface.
        */
        virtual MProfileSetExtraSettings& ProfileSetExtraSettings() const = 0;

    };

#endif      // __MPROFILEEXTENDED_H__

// End of File

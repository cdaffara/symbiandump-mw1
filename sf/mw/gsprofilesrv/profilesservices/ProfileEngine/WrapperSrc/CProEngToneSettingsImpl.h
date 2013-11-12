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
* Description:  This class implements MProEngToneSettings interface.
*
*/



#ifndef CPROENGTONESETTINGSIMPL_H
#define CPROENGTONESETTINGSIMPL_H

// INCLUDES
#include <MProEngToneSettings.h>
#include <e32base.h>
#include <TProfileToneSettings.h>

// FORWARD DECLARATIONS
class MProfileTones;
class MProfileSetTones;

// CLASS DECLARATION

/**
* This class implements MProEngToneSettings interface.
*
*  @lib ProfileEngine.lib
*  @since 3.1
*/
NONSHARABLE_CLASS( CProEngToneSettingsImpl ) : public CBase,
                                               public MProEngToneSettings
    {
    public:  // Constructors and destructor

        /**
         * Two-phased constructor.
         */
        static CProEngToneSettingsImpl* NewL(
                TProfileToneSettings& aToneSettings,
                TUint32 aModifiableFlags );

        /**
         * Destructor.
         */
        virtual ~CProEngToneSettingsImpl();

    public:

        /**
         * From MProEngToneSettings
         */
        TProfileRingingType RingingType() const;

        /**
        * Returns the keypad volume.
        * @since 3.1
        * @return Returns the keypad volume.
        */
        TProfileKeypadVolume KeypadVolume() const;

        /**
        * Returns the ringing volume.
        * @since 3.1
        * @return Returns the ringing volume.
        */
        TProfileRingingVolume RingingVolume() const;

        /**
        * Returns the state of vibrating alert setting.
        * @since 3.1
        * @return Returns the state of vibrating alert setting.
        */
        TBool VibratingAlert() const;

        /**
        * Returns the state of warning and game tones setting.
        * @since 3.1
        * @return Returns the state of warning and game tones setting.
        */
        TBool WarningAndGameTones() const;

        /**
        * Returns the state of text-to-speech setting.
        * @since 3.1
        * @return Returns the state of text-to-speech setting.
        */
        TBool TextToSpeech() const;

        /**
        * Sets the ringing type.
        * @since 3.1
        * @param aRingingType The new value for ringing type.
        * @return KErrNone if succesful, KErrAccessDenied if ringing type
        *         setting of this profile is read-only.
        */
        TInt SetRingingType( TProfileRingingType aRingingType );

        /**
        * Sets the keypad volume.
        * @since 3.1
        * @param aKeypadVolume The new value for keypad tones volume.
        * @return KErrNone if succesful, KErrAccessDenied if keypad tones
        *         setting of this profile is read-only.
        */
        TInt SetKeypadVolume( TProfileKeypadVolume aKeypadVolume );

        /**
        * Sets the ringing volume.
        * @since 3.1
        * @param aRingingVolume The new value for ringing volume.
        * @return KErrNone if succesful, KErrAccessDenied if ringing volume
        *         setting of this profile is read-only.
        */
        TInt SetRingingVolume(
                TProfileRingingVolume aRingingVolume );

        /**
        * Sets the state of vibrating alert setting.
        * @since 3.1
        * @param aVibratingAlert The new state for the vibrating alert setting.
        * @return KErrNone if succesful, KErrAccessDenied if vibrating alert
        *         setting of this profile is read-only.
        */
        TInt SetVibratingAlert( TBool aVibratingAlert );

        /**
        * Sets the state of warning and game tones setting.
        * @since 3.1
        * @param aWarningAndGameTones The new state for Warning and Game tones
        * setting.
        * @return KErrNone if succesful, KErrAccessDenied if warning and game
        *         tones setting of this profile is read-only.
        */
        TInt SetWarningAndGameTones( TBool aWarningAndGameTones );

        /**
        * Sets the state of text-to-speech setting.
        * @since 3.1
        * @param aTextToSpeech The new state for text-to-speech setting.
        * @return KErrNone if succesful, KErrAccessDenied if text-to-speech
        *         setting of this profile is read-only.
        */
        TInt SetTextToSpeech( TBool aTextToSpeech );


    private:

        /**
         * C++ default constructor.
         */
        CProEngToneSettingsImpl(
                TProfileToneSettings& aToneSettings,
                TUint32 aModifiableFlags );

        /**
        * By default Symbian 2nd phase constructor is private.
        */
        void ConstructL();

    private:    // Data

        // The real implementation of the profile tone settings
        TProfileToneSettings& iToneSettings;

        // Flags to check is modification allowed:
        TUint32 iModifiableFlags;
    };

#endif      //  CPROENGTONESETTINGSIMPL_H

// End of File


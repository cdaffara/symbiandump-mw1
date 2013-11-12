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
* Description:  Profile tones implementation.
*
*/



#ifndef __CPROFILETONESIMPL_H
#define __CPROFILETONESIMPL_H

//  INCLUDES
#include <e32base.h>
#include <MProfileTones.h>
#include <TProfileToneSettings.h>
#include <MProfileSetTones.h>

// FORWARD DECLARATIONS
class RWriteStream;
class CRepository;

// CLASS DECLARATION

/**
*  Profile tones implementation.
*  This class implements MProfileTones and MProfileSetTones interfaces.
*
*  @lib ?library
*  @since 2.0
*/
NONSHARABLE_CLASS(CProfileTonesImpl) : public CBase,
                          public MProfileTones,
                          public MProfileSetTones
    {
    public:  // Constructors and destructor

        /**
        * Two-phased constructor.
        */
        static CProfileTonesImpl* NewL();

        /**
        * Two-phased constructor. Copy constructor.
        * @param aProfileTones Profile tones
        */
        static CProfileTonesImpl* NewL( const MProfileTones& aProfileTones );

        /**
        * Destructor.
        */
        virtual ~CProfileTonesImpl();

    public: // New functions

        /**
        * Internalize data from the central repository.
        * @since 2.8
        * @param aCenRep Central Repository object to read the settings
        * @param aProfileId the id of the profile to be internalized
        */
        void InternalizeL( CRepository& aCenRep, TInt aProfileId );

        /**
        * Externalize data to Central Repository.
        * @since 2.8
        * @param aCenRep Central Repository object to write the settings
        */
        void ExternalizeL( CRepository& aCenRep, TInt aProfileId ) const;

    public: // Functions from base classes

        /**
        * From MProfileTones.
        */
        virtual const TDesC& RingingTone1() const;

        /**
        * From MProfileTones.
        */
        virtual const TDesC& RingingTone2() const;

        /**
        * From MProfileTones.
        */
        virtual const TDesC& MessageAlertTone() const;

        /**
        * From MProfileTones.
        */
        virtual const TProfileToneSettings& ToneSettings() const;

        /**
        * From MProfileSetTones.
        */
        virtual void SetRingingTone1L( const TDesC& aRingingTone );

        /**
        * From MProfileSetTones.
        */
        virtual void SetRingingTone2L( const TDesC& aRingingTone );

        /**
        * From MProfileSetTones.
        */
        virtual void SetMessageAlertToneL( const TDesC& aMessageAlertTone );

        /**
        * From MProfileSetTones.
        */
        virtual TProfileToneSettings& SetToneSettings();

    private:

        /**
        * By default Symbian 2nd phase constructor is private.
        */
        void ConstructL();

        /**
        * By default Symbian 2nd phase constructor is private.
        * @param aProfileTones Profile tones
        */
        void ConstructL( const MProfileTones& aProfileTones );

    private:
        void InitPowerSaveMode();
        void ReadPowerSaveConfigL();
        
    private:    // Data

        // Own: Line 1 Ringing tone file name
        HBufC* iRingingTone1;

        // Own: Line 2 Ringing tone file name
        HBufC* iRingingTone2;

        // Own: Message alert tone file name
        HBufC* iMessageAlertTone;

        // Own: Tone settings
        TProfileToneSettings iProfileToneSettings;
        
        TInt iPsmConfigVibra;
        TInt iPsmConfigKeypadVolume;
        TBool iPowerSaveMode;
    };

#endif      // __CPROFILETONESIMPL_H

// End of File

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



#ifndef __CPROFILEEXTRATONESIMPL_H
#define __CPROFILEEXTRATONESIMPL_H

//  INCLUDES
#include <e32base.h>
#include <MProfileExtraTones2.h>
#include <MProfileSetExtraTones2.h>

// FORWARD DECLARATIONS
class RWriteStream;
class CRepository;

// CLASS DECLARATION

/**
*  Profile extra tones implementation.
*
*  @lib ?library
*  @since 2.0
*/
NONSHARABLE_CLASS(CProfileExtraTonesImpl) : public CBase,
                               public MProfileExtraTones2,
                               public MProfileSetExtraTones2
    {
    public:  // Constructors and destructor

        /**
        * Two-phased constructor.
        */
        static CProfileExtraTonesImpl* NewL();

        /**
        * Two-phased constructor. Copy constructor.
        * @param aProfileExtraTones Profile extra tones
        */
        static CProfileExtraTonesImpl* NewL(
            const MProfileExtraTones2& aProfileExtraTones );

        /**
        * Destructor.
        */
        virtual ~CProfileExtraTonesImpl();

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
        * From MProfileExtraTones.
        */
        virtual const TDesC& IMMessageAlertTone() const;

        /**
        * From MProfileSetExtraTones.
        */
        virtual void SetIMMessageAlertToneL(
            const TDesC& aIMMessageAlertTone );

        /**
        * From MProfileExtraTones.
        */
        virtual const TDesC& EmailAlertTone() const;

        /**
        * From MProfileSetExtraTones.
        */
        virtual void SetEmailAlertToneL(
            const TDesC& aEmailAlertTone );

        /**
        * From MProfileExtraTones.
        */
        virtual const TDesC& VideoCallRingingTone() const;

        /**
        * From MProfileSetExtraTones.
        */
        virtual void SetVideoCallRingingToneL(
            const TDesC& aRingingTone );
        
        /**
        * From MProfileExtraTones2.
        */
        virtual const TDesC& ReminderTone() const;
               
        /**
        * From MProfileSetExtraTones2.
        */
        virtual void SetReminderToneL( 
                const TDesC& aReminderTone );
        
        /**
        * From MProfileExtraTones2.
        */
        virtual const TDesC& ClockAlarmTone() const;
        
        /**
        * From MProfileSetExtraTones2.
        */
        virtual void SetClockAlarmToneL( 
                const TDesC& aClockAlarmTone );

    private:

        /**
        * By default Symbian 2nd phase constructor is private.
        */
        void ConstructL();

        /**
        * By default Symbian 2nd phase constructor is private.
        * @param aProfileExtraTones Profile extra tones
        */
        void ConstructL( const MProfileExtraTones2& aProfileExtraTones );

    private:    // Data

        // Own: IM Message Alert tone file name
        HBufC* iIMMessageAlertTone;

        // Own: Email Alert tone file name
        HBufC* iEmailAlertTone;

        // Own: Video call ringing tone file name
        HBufC* iVideoCallRingingTone;
        
        //Own: Reminder tone file name
        HBufC* iReminderTone;
        
        //Own: Clock alarm tone file name
        HBufC* iClockAlarmTone;
    };

#endif      // __CPROFILEEXTRATONESIMPL_H

// End of File

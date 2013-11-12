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
* Description:  Profile PTT settings implementation.
*
*/



#ifndef __CPROFILEPTTSETTINGSIMPL_H
#define __CPROFILEPTTSETTINGSIMPL_H

//  INCLUDES
#include <e32base.h>
#include <MProfilePttSettings.h>
#include <MProfileSetPttSettings.h>

// FORWARD DECLARATIONS
class CRepository;

// CLASS DECLARATION

/**
*  Profile extra tones implementation.
*
*  @lib ProfileEng.lib
*  @since 3.1
*/
NONSHARABLE_CLASS(CProfilePttSettingsImpl) : public CBase,
                               public MProfilePttSettings,
                               public MProfileSetPttSettings
    {
    public:  // Constructors and destructor

        /**
        * Two-phased constructor.
        */
        static CProfilePttSettingsImpl* NewL();
        static CProfilePttSettingsImpl* NewL(
                const MProfilePttSettings& aProfilePttSettings );

        /**
        * Destructor.
        */
        virtual ~CProfilePttSettingsImpl();

    public: // New functions

        /**
        * Internalize data from the central repository.
        * @since 3.1
        * @param aCenRep Central Repository object to read the settings
        * @param aProfileId the id of the profile to be internalized
        */
        void InternalizeL( CRepository& aCenRep, TInt aProfileId );

        /**
        * Externalize data to Central Repository.
        * @since 3.1
        * @param aCenRep Central Repository object to write the settings
        */
        void ExternalizeL( CRepository& aCenRep, TInt aProfileId ) const;

    public: // Functions from base classes

        /**
        * From MProfilePttSettings.
        */
        const TDesC& PttRingingTone() const;

        /**
        * From MProfilePttSettings.
        */
        TProfilePttStatus PttStatus() const;

        /**
        * From MProfileSetPttSettings.
        */
        void SetPttRingingToneL( const TDesC& aPttRingingTone );

        /**
        * From MProfileSetPttSettings.
        */
        void SetPttStatus( TProfilePttStatus aPttStatus );

    private:

        /**
        * By default Symbian 2nd phase constructor is private.
        */
        void ConstructL();

        /**
        * By default Symbian 2nd phase constructor is private.
        * @param aProfileExtraTones Profile extra tones
        */
        void ConstructL( const MProfilePttSettings& aProfilePttSettings );

    private:    // Data

        // Own: PTT ringing tone
        HBufC* iPttRingingTone;

        // Own: PTT status
        TProfilePttStatus iPttStatus;
    };

#endif      // __CPROFILEPTTSETTINGSIMPL_H

// End of File


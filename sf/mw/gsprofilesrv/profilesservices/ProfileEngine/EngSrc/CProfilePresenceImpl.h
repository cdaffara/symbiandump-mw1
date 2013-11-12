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
* Description:  Profile presence settings implementation.
*
*/



#ifndef __CPROFILEPRESENCEIMPL_H
#define __CPROFILEPRESENCEIMPL_H

//  INCLUDES
#include <e32base.h>
#include <MProfilePresence.h>
#include <MProfileSetPresence.h>

// CONSTANTS
// MACROS
// DATA TYPES
// FUNCTION PROTOTYPES

// FORWARD DECLARATIONS
class CRepository;

// CLASS DECLARATION

/**
*  Profile presence settings implementation.
*
*  @lib ?library
*  @since 2.0
*/
NONSHARABLE_CLASS(CProfilePresenceImpl) : public CBase,
                             public MProfilePresence,
                             public MProfileSetPresence
    {
    public:  // Constructors and destructor

        /**
        * Two-phased constructor.
        * @param aFs Ref. to file server session.
        */
        static CProfilePresenceImpl* NewL();

        /**
        * Two-phased constructor. Copy constructor.
        * @param aFs Ref. to file server session.
        * @param aPresence Profile presence settings.
        */
        static CProfilePresenceImpl* NewL(
            const MProfilePresence& aPresence );

        /**
        * Destructor.
        */
        virtual ~CProfilePresenceImpl();

    public: // New functions

        /**
        * Internalize data from the Central Repository.
        * @since 2.0
        * @param aCenRep profiles central repository
        * @param aProfileId id of the profile which is read
        */
        void InternalizeL( CRepository& aCenRep, TInt aProfileId );

        /**
        * Externalize data to the Central Repository.
        * @since 2.0
        * @param aCenRep profiles central repository
        * @param aProfileId id of the profile which is written
        */
        void ExternalizeL( CRepository& aCenRep, TInt aProfileId ) const;

    public: // Functions from base classes

        /**
        * From MProfilePresence.
        */
        virtual TUint32 AvailabilityAttribute() const;

        /**
        * From MProfilePresence.
        */
        virtual const TDesC& StatusMessage() const;

        /**
        * From MProfileSetPresence.
        */
        virtual void SetAvailabilityAttribute(
            const TUint32 aAvailabilityAttribute );

        /**
        * From MProfileSetPresence.
        */
        virtual void SetStatusMessageL( const TDesC& aStatusMessage );

    private:

        /**
        * C++ default constructor.
        * @param aFs Ref. to file server session.
        */
        CProfilePresenceImpl();

        /**
        * By default Symbian 2nd phase constructor is private.
        */
        void ConstructL();

        /**
        * By default Symbian 2nd phase constructor is private.
        * @param aPresence Presence settings.
        */
        void ConstructL( const MProfilePresence& aPresence );

    private:    // Data

        // Own: Availability attribute
        TUint32 iAvailabilityAttribute;

        // Own: Status message
        HBufC* iStatusMessage;

    };

#endif      //  __CPROFILEPRESENCEIMPL_H

// End of File

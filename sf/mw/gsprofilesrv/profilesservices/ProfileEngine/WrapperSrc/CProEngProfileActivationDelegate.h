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
* Description:  This class listens to the changes in the Publish & Subscribe key that is used
*               by Profiles Engine to notify that the active profile has been changed.
*
*/



#ifndef CPROENGPROFILEACTIVATIONDELEGATE_H
#define CPROENGPROFILEACTIVATIONDELEGATE_H

// INCLUDES
#include "CProEngPubSubObserverBase.h"

// FORWARD DECLARATIONS
class CRepository;
class MProEngProfileActivationObserver;

// CLASS DECLARATION

/**
* This class listens to the changes in the Publish & Subscribe key that is used
* by Profiles Engine to notify that the active profile has been changed.
*
*  @lib ProfileEngine.lib
*  @since 3.1
*/
NONSHARABLE_CLASS( CProEngProfileActivationDelegate )
    : public CProEngPubSubObserverBase
    {
    public:  // Constructors and destructor

        /**
         * Two-phased constructor.
         */
        static CProEngProfileActivationDelegate* NewL(
                MProEngProfileActivationObserver& aObserver );
        
        /**
         * Destructor.
         */
        ~CProEngProfileActivationDelegate();

    private: // Functions from base classes

        /**
         * From CProEngProfileActivationDelegateBase.
         */
        void NotifyObserverL();

        /**
         * From CProEngProfileActivationDelegateBase.
         */
        void NotifyError( TInt aError );

    private:

        /**
         * C++ constructor.
         */
        CProEngProfileActivationDelegate(
                MProEngProfileActivationObserver& aObserver );

        /**
        * By default Symbian 2nd phase constructor is private.
        */
        void ConstructL();

    public: // Data:

        // Reference to the client implemented Profile Activation Observer
        MProEngProfileActivationObserver& iObserver;

        // Own: Profiles Engine Central Repository
        CRepository* iRepository;

    };

#endif      //  CPROENGPROFILEACTIVATIONDELEGATE_H

// End of File


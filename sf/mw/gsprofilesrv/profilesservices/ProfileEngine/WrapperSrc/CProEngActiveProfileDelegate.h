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
* Description:  Listen the changes in the Publish & Subscribe key
*
*/



#ifndef CPROENGACTIVEPROFILEDELEGATE_H
#define CPROENGACTIVEPROFILEDELEGATE_H

// INCLUDES
#include "CProEngPubSubObserverBase.h"

// FORWARD DECLARATIONS
class MProEngActiveProfileObserver;

// CLASS DECLARATION

/**
* This class listens to the changes in the Publish & Subscribe key that is used
* by Profiles Engine to notify that the active profile has been modified.
*
*  @lib ProfileEngine.lib
*  @since 3.1
*/
NONSHARABLE_CLASS( CProEngActiveProfileDelegate )
    : public CProEngPubSubObserverBase
    {
    public:  // constructor and destructor
        CProEngActiveProfileDelegate(
                MProEngActiveProfileObserver& aObserver );

        ~CProEngActiveProfileDelegate() {};

    private: // Functions from base classes

        /**
         * From CProEngActiveProfileDelegateBase.
         */
        void NotifyObserverL();

        void NotifyError( TInt aError );

    public: // Data:

        // Profile Activation Observer reference
        MProEngActiveProfileObserver& iObserver;

    };

#endif      //  CPROENGACTIVEPROFILEDELEGATE_H

// End of File


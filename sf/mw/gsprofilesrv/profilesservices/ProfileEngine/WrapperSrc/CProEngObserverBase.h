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
* Description:  The base class for listening to events in Publish & Subscribe or in
* 				Central Repository
*
*/



#ifndef CPROENGOBSERVERBASE_H
#define CPROENGOBSERVERBASE_H

// INCLUDES
#include <e32base.h>

// CLASS DECLARATION

/**
* The base class for listening to events in Publish & Subscribe or in
* Central Repository. This is a helper class for CProEngNotifyHandlerImpl.
*
*  @lib ProfileEngine.lib
*  @since 3.1
*/
NONSHARABLE_CLASS( CProEngObserverBase ) : public CActive
    {
    protected:  // constructor and destructor

        CProEngObserverBase( TUint32 aKey );

        virtual ~CProEngObserverBase() {};

    public: // New functions:

        /**
         * Absract method to request notifications from the source determined
         * by the subclass. This must be called once after the real observer
         * class is constructed. After that this is called by RunL() of this
         * class.
         * @return KErrNone if succeed, otherwise a system-wide error code.
         */
        virtual TInt RequestNotification() = 0;

        /**
         * Abstract method to send notification to the observer.
         */
        virtual void NotifyObserverL() = 0;

        /**
         * Abstract method to send notification about the given error to the
         * observer.
         * @param aError the error code to be delivered to the observer.
         */
        virtual void NotifyError( TInt aError ) = 0;

    private: // Functions from base classes

        /**
         * From CActive.
         */
        void RunL();

    protected: // Data:

        // The key to observe
        TUint32 iKey;

    };

#endif      //  CPROENGOBSERVERBASE_H

// End of File


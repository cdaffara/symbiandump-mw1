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
* Description:  Delegate notifications about profile modifications
*               to the observer.
*
*/



#ifndef CPROENGPROFILEEVENTDELEGATE_H
#define CPROENGPROFILEEVENTDELEGATE_H

// INCLUDES
#include "CProEngCenRepObserverBase.h"

// FORWARD DECLARATIONS
class MProEngProfileObserver;

// CLASS DECLARATION

/**
* This class delegates notifications about profile modifications to the observer
* implemented by the client of Profiles Engine Wrapper API. This is a helper
* class for CProEngNotifyHandlerImpl.
*
*  @lib ProfileEngine.lib
*  @since 3.1
*/
NONSHARABLE_CLASS( CProEngProfileEventDelegate )
    : public CProEngCenRepObserverBase
    {
    public:  // constructor and destructor

        static CProEngProfileEventDelegate* NewL(
                 TInt aProfileId,
                 MProEngProfileObserver& aObserver );

        virtual ~CProEngProfileEventDelegate() {};

    public: // Functions from base classes

        /**
         * From CProEngObserverBase.
         */
        void NotifyObserverL();

        /**
         * From CProEngObserverBase.
         */
        void NotifyError( TInt aError );


    public: // New functions

        /**
         * Returns the ID of the profile observed by this class.
         */
        TInt ProfileId() const;

    private:  // constructor

        CProEngProfileEventDelegate( TUint32 aPartialCenRepKey,
                                     TUint32 aKeyMask,
                                     MProEngProfileObserver& aObserver );

    private: // Data:

        // The client implemented observer for profile modification events:
        MProEngProfileObserver& iObserver;

    };

#endif      //  CPROENGPROFILEEVENTDELEGATE_H

// End of File


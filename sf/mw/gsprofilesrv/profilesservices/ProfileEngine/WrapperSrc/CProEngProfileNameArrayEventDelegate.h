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
* Description:  This class delegates notifications about name array
*               changes to the observer.
*
*/



#ifndef CPROENGPROFILENAMEARRAYEVENTDELEGATE_H
#define CPROENGPROFILENAMEARRAYEVENTDELEGATE_H

// INCLUDES
#include "CProEngCenRepObserverBase.h"

// FORWARD DECLARATIONS
class MProEngProfileNameArrayObserver;

// CLASS DECLARATION

/**
* This class delegates notifications about name array changes to the observer
* implemented by the client of Profiles Engine Wrapper API. This is
* a helper class for CProEngNotifyHandlerImpl.
*
*  @lib ProfileEngine.lib
*  @since 3.1
*/
NONSHARABLE_CLASS( CProEngProfileNameArrayEventDelegate )
    : public CProEngCenRepObserverBase
    {
    public:

        static CProEngProfileNameArrayEventDelegate* NewL(
                MProEngProfileNameArrayObserver& aObserver );

        virtual ~CProEngProfileNameArrayEventDelegate() {};

    public: // Functions from base classes

        /**
         * From CProEngObserverBase.
         */
        void NotifyObserverL();

        /**
         * From CProEngObserverBase.
         */
        void NotifyError( TInt aError );

    private:  // constructor

        CProEngProfileNameArrayEventDelegate(
                TUint32 aPartialCenRepKey,
                TUint32 aKeyMask,
                MProEngProfileNameArrayObserver& aObserver );

    private: // Data:

        // The client implemented observer for name array modification events:
        MProEngProfileNameArrayObserver& iObserver;

    };

#endif      //  CPROENGPROFILENAMEARRAYEVENTDELEGATE_H

// End of File


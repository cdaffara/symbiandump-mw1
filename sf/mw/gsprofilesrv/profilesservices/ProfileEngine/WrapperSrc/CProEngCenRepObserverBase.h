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
* Description:  Listening the events in Central Repository.
*
*/



#ifndef CPROENGCENREPOBSERVERBASE_H
#define CPROENGCENREPOBSERVERBASE_H

// INCLUDES
#include "CProEngObserverBase.h"

// FORWARD DECLARATIONS
class CRepository;

// CLASS DECLARATION

/**
* The base class for listening to events in Central Repository. This is
* a helper class for CProEngNotifyHandlerImpl.
*
*  @lib ProfileEngine.lib
*  @since 3.1
*/
NONSHARABLE_CLASS( CProEngCenRepObserverBase ) : public CProEngObserverBase
    {
    protected:  // constructor and destructor

        CProEngCenRepObserverBase( TUint32 aPartialCenRepKey,
                                   TUint32 aKeyMask );

        virtual ~CProEngCenRepObserverBase(); 

        virtual void ConstructL();

    public: // Functions from base classes

        /**
         * From CProEngObserverBase.
         */
        TInt RequestNotification();

        /**
         * From CActive.
         */
        void DoCancel();

    protected: // Data:

        // The mask to request notifications about several keys from CenRep
        TUint32 iKeyMask;

        // Own: Profiles Engine Central Repository
        CRepository* iRepository;

    };

#endif      //  CPROENGCENREPOBSERVERBASE_H

// End of File


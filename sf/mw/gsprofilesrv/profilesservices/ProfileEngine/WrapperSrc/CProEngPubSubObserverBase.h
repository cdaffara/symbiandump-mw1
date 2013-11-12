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
* Description:  The base class for listening to events in the 
*               Publish & Subscribe.
*
*/



#ifndef CPROENGPUBSUBOBSERVERBASE_H
#define CPROENGPUBSUBOBSERVERBASE_H

// INCLUDES
#include    "CProEngObserverBase.h"
#include    <e32property.h>  // RProperty

// CLASS DECLARATION

/**
* The base class for listening to events in the Publish & Subscribe. This is
* a helper class for CProEngNotifyHandlerImpl.
*
*  @lib ProfileEngine.lib
*  @since 3.1
*/
NONSHARABLE_CLASS( CProEngPubSubObserverBase ) : public CProEngObserverBase
    {
    protected:  // constructor and destructor

        CProEngPubSubObserverBase( TUint32 aPubSubKey );

        virtual ~CProEngPubSubObserverBase();

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

        // Profiles Engine Publish and Subscribe interface
        RProperty iProperty;
        
        TBool iAttatched;

    };

#endif      //  CPROENGPUBSUBOBSERVERBASE_H

// End of File


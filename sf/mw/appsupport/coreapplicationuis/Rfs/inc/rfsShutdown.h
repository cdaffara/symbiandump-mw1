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
* Description: 
*
*  Shutdown timer for RFS server.
*     
*
*/


#ifndef RFS_SHUTDOWN_H
#define RFS_SHUTDOWN_H

// INCLUDES
#include <e32base.h>

// CLASS DECLARATION

/**
* Active object for shutting down the RFS server after a certain time of
* inactivity.
*/
class CRfsShutdown : public CActive
    {
    public:  // Constructors and destructor

        /**
        * Two-phased constructor.
        */
        static CRfsShutdown* CRfsShutdown::NewL( TInt aPriority );

        ~CRfsShutdown();

    public:  // new functions

        void Start( TInt aTimeout );

    protected:  // From CActive

        void RunL();
        void DoCancel();

    private:

        /**
        * C++ default constructors.
        */        
        CRfsShutdown( TInt aPriority );

    private:    // Data

        RTimer iTimer;
    };

#endif      // RFS_SHUTDOWN_H   

// End of File

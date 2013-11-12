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
*		This class is used to notify RFS server that the RFS operations has
*	been completed.
*     
*
*/


#ifndef RFS_COMPLETER_H
#define RFS_COMPLETER_H

// INCLUDES
#include <e32base.h>

// CONSTANTS

const TInt KRfsCompletionSent       = 0x0001;
const TInt KRfsCompletionHandled    = 0x0002;

// FORWARD DECLARATIONS

class CRfsSession;

// CLASS DECLARATION

/**
* Active object for completing client request when RFS is done
*/
class CRfsCompleter : public CActive
    {
    public:  // Constructors and destructor

        /**
        * Two-phased constructor
        */        
        static CRfsCompleter* NewL( CRfsSession* aSession );

        ~CRfsCompleter();

    public:  // new functions

        void Start();
        void Complete( TInt aReturn );

    protected:  // From CActive

        void RunL();
        void DoCancel();

    private:

        /**
        * C++ default constructors.
        */        
        CRfsCompleter( CRfsSession* iSession );

    private:    // Data

        CRfsSession* iSession;  // not owned

        TInt iFlags;
        TInt iReturn;

    private:    // friend classes

        friend class CRfsSession;
        friend class CRfsScript;
    };

#endif      // RFS_COMPLETER_H   

// End of File

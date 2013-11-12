/*
* Copyright (c) 2002-2008 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Idle state listener
*
*/



#ifndef REMCONIDLELISTENER_H
#define REMCONIDLELISTENER_H

// INCLUDES
#include <e32base.h>
#include <e32property.h>

// CONSTANTS

// MACROS

// DATA TYPES

// FUNCTION PROTOTYPES

// FORWARD DECLARATIONS
class CRemConTspController;
// CLASS DECLARATION

/**
*  Idle state listener implementation.
*
*  RemTspController.lib
*  @since S60 3.1
*/
NONSHARABLE_CLASS( CRemConIdleListener ) : public CActive
    {
    public: // Constructors and destructor

        /**
        * Destructor.
        */
        ~CRemConIdleListener();

        /**
        * Two-phased constructor.
        */
        static CRemConIdleListener* NewL( CRemConTspController& aController );

    private: // New functions

        /**
        * C++ default constructor.
        */
        CRemConIdleListener( CRemConTspController& aController );

        /**
        * Symbian 2nd phase constructor.
        */
        void ConstructL();

        static TInt RetryCB( TAny* aPtr );

        void CreateController();

    private:  // Functions from base classes

        //from CActive
        void DoCancel();

        void RunL();

        TInt RunError( TInt aError );

    private:    
        
        // not owned
        CRemConTspController& iController;
        
        // owned    
        RProperty iProperty;
        
        // owned
        CPeriodic* iPeriodic;
        
        // not owned
        TBool* iDeleteInd;
    };

#endif // REMCONIDLELISTENER_H

// End of File

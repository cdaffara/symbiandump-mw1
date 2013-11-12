/*
* Copyright (c) 2004 Nokia Corporation and/or its subsidiary(-ies). 
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
*     This class provides Security services for RFS activation UI. 
*     
*
*/


#ifndef RFS_SECURITY_HANDLER_H
#define RFS_SECURITY_HANDLER_H

//  INCLUDES

#include <e32std.h>
// for TRfsType
#include "RfsHandler.h"

// CONSTANTS

// CLASS DECLARATION
class RTASecuritySession;

/**
*  This class provides a method to activate the rfs process including all UI.
*  (notes etc.)
*/
class CRfsSecurityHandler : public CActive
    {
    public:  // Constructors and destructor
        /**
        * Creates instance of the CRfsSecurityHandler class.
        *
        * @return Returns the instance just created.
        */
        static CRfsSecurityHandler* NewL();

        /**
        * Destructor also calls Cancel()
        */
        ~CRfsSecurityHandler();

    private:
        /**
        * Constructors are private
        */
        CRfsSecurityHandler();
        void ConstructL();

    protected:  // From CActive

        void RunL();
        void DoCancel();

    public: // New functions

        /**
        * Activates the RFS security query
        *
        * @param aMode the level of Rfs security is checking
        * @return ETrue if security check passed, EFalse otherwise
        */
        TBool AskSecurityL( TRfsType aMode );

    private:    // Data

        // for security code query when using Security Engine
        RTASecuritySession* iSecuritySession;
        // for waiting on security server response
        CActiveSchedulerWait iWait;
        // the result of our query
        TBool iResult;
        // a check to ensure we aren't out of scope
        TBool* iIsDestroyed;
    };

#endif  // RFS_SECURITY_HANDLER_H   
            
// End of File

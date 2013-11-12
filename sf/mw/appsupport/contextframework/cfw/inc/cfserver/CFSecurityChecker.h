/*
* Copyright (c) 2002-2006 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Security Checker interface for CF.
*
*/


#ifndef M_CFSECURITYCHECKER_H
#define M_CFSECURITYCHECKER_H

class RThread;

/**
* Interface for security checker.
*/
class MCFSecurityChecker
    {
    public:
    
        /**
        * Checks client thread security.
        * Normal CF implementation lets current CF thread pass all
        * security checks. Implementation for test cases can fail whenever
        * the test case requires.
        *
        * @since S60 4.0
        * @param aClientThread Action identifier.
        * @param aSecurityPolicy Security level for the thread.
        * @return ETrue if the security check is passed.
        */
        virtual TBool CheckClientSecurity( const RThread& aClientThread,
            const TSecurityPolicy& aSecurityPolicy ) = 0;

    protected:
    
        // Deny destruction through this class
        virtual ~MCFSecurityChecker(){};
    };

#endif //M_CFSECURITYCHECKER_H

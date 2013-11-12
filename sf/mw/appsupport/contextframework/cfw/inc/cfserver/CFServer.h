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
* Description:  ?Description
*
*/


#ifndef C_CFSERVER_H
#define C_CFSERVER_H


#include <e32base.h>
#include <f32file.h>

#include "cfcommon.h"
#include "cftraceconfiguration.hrh"
#include "CFSecurityChecker.h"

class CCFEngine;

/**
* Context Framework Server class.
*/
NONSHARABLE_CLASS( CCFServer ): public CServer2, public MCFSecurityChecker
    {
    public: // New methods
    
        static CCFServer* NewL();
        static CCFServer* NewLC();

        ~CCFServer();
        
    public:// New methods
    
        static TInt ThreadFunction( TAny* aThreadParms );
        
        TBool CheckClientSecurity( const RThread& aClientThread,
            const TSecurityPolicy& aSecurityPolicy );
        
        
    private: // From base classes
    
        // From CServer2
        virtual CSession2 *NewSessionL( const TVersion &aVersion,
            const RMessage2& aMessage ) const;

    private:
    
        void ConstructL();
        CCFServer( TInt aPriority );
        
    private: // New methods

        static void ThreadFunctionL(TAny* aThreadParms);
        static void PanicServer(TContextServPanic aPanic);
        
    private: // Data

        /** CF engine */    
        CCFEngine* iCFEngine;
    };

#endif

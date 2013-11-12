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


#ifndef C_CFSERVSESSION_H
#define C_CFSERVSESSION_H

#include <e32base.h>

class MCFExtendedContextInterface;
class MCFActionInterface;
class MCFScriptInterface;
class CCFContextObject;
class CCFMessageHandlerBase;
class CCFContextSubscription;
class CCFContextIndication;

/**
* Serverside client session.
*/
NONSHARABLE_CLASS( CCFServSession ): public CSession2
    {
    public:
    
        // Symbian two phased constructors
        static CCFServSession* NewL( MCFExtendedContextInterface& aCFContext,
            MCFActionInterface& aCFAction,
            MCFScriptInterface& aScriptInterface );
        static CCFServSession* NewLC( MCFExtendedContextInterface& aCFContext,
            MCFActionInterface& aCFAction,
            MCFScriptInterface& aScriptInterface );
        
        // C++ destructor
        ~CCFServSession();
        
    public:
    
        // panic the client
        static void PanicClient( const RMessage2& aMessage, TInt aPanic );
        
        // safewrite between client and server
        static void Write( const RMessage2& aMessage,TInt aParam,const TDesC8& aDes,TInt anOffset=0 );
        
    private: // From base classes
    
        // From CSession2
        void ServiceL( const RMessage2 &aMessage );
        
    private: // New methods
    
        // Dispatches message
        void DispatchMessageL( const RMessage2 &aMessage );
        
    private:
    
        // Symbian 2nd phase constructor
        void ConstructL( MCFExtendedContextInterface& aCFContext,
            MCFActionInterface& aCFAction,
            MCFScriptInterface& aScriptInterface );
        
        // C++ constrcutor
        CCFServSession();

    private:
    
        // Own: Message handlers
        RPointerArray<CCFMessageHandlerBase> iMessageHandlers;
    };

#endif // C_CFSERVSESSION_H



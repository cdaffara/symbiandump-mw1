/*
* Copyright (c) 2006-2006 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Base class for message handlers.
*
*/


#ifndef C_CFMESSAGEHANDLERBASE_H
#define C_CFMESSAGEHANDLERBASE_H


#include <e32base.h>

#include "cfcontextsubscriptionlistener.h"

class RMessage2;
class MCFExtendedContextInterface;
class MCFActionInterface;
class MCFScriptInterface;

/**
* Context Framework server session message handler base class.
*/
NONSHARABLE_CLASS( CCFMessageHandlerBase ): public CBase
    {
    public:
    
        // Message handler type
        enum TCFMessageHandler
            {
            EContextMessageHandler,
            EActionMessageHandler,
            EScriptMessageHandler
            };
        
    public:
    
        /**
        * Handles message from client. 
        *
        * @since S60 4.0
        * @param aMessage Message from client.
        * @return ETrue if message handled.
        */
        virtual TBool HandleMessageL( const RMessage2& aMessage ) = 0;
        
    protected:
    
        CCFMessageHandlerBase( MCFExtendedContextInterface& aCFContext,
            MCFActionInterface& aCFAction, 
            MCFScriptInterface& aScriptInterface );
        
    protected: // Data
    
        // Ref:
        MCFExtendedContextInterface& iCFContext;
        
        // Ref:
        MCFActionInterface& iCFAction;
        
        // Ref:
        MCFScriptInterface& iCFScriptInterface;
    };

#endif  // C_CFMESSAGEHANDLERBASE_H

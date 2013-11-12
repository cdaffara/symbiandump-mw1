/*
* Copyright (c) 2006-2008 Nokia Corporation and/or its subsidiary(-ies). 
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


#ifndef C_CFMESSAGEHANDLERACTION_H
#define C_CFMESSAGEHANDLERACTION_H

#include "CFMessageHandlerBase.h"
#include "CFActionSubscriptionListener.h"
#include "cfactionowner.h"

class CCFActionIndication;

/**
 * Action messages related handler class.
 */
NONSHARABLE_CLASS( CCFMessageHandlerAction ): public CCFMessageHandlerBase,
public MCFActionSubscriptionListener,
public MCFActionOwner
    {
public:

    // Two phased constructors
    static CCFMessageHandlerAction* NewL( MCFExtendedContextInterface& aCF,
        MCFActionInterface& aActivatorEngine,
        MCFScriptInterface& aScriptInterface );
    static CCFMessageHandlerAction* NewLC( MCFExtendedContextInterface& aCF,
        MCFActionInterface& aActivatorEngine,
        MCFScriptInterface& aScriptInterface );

    // Destructor
    ~CCFMessageHandlerAction();

protected: // From base classes

    // From CCFMessageHandlerBase
    TBool HandleMessageL( const RMessage2& aMessage );

    // From MCFContextSubscriptionListener
    void ActionIndicationL( CCFActionIndication* aIndication );
    
    // From MCFActionOwner
    TCFActionOwner ActionOwner() const;

private: // New methods

    // Checks if there is action indication waiting
    void CheckActionQueueL();

    // Stream and send action indication to client
    void IndicateCurrentActionL();

    // Streams indication from object to descriptor
    HBufC8* ExternalizeIndicationLC( CCFActionIndication& aIndication );

private:

    CCFMessageHandlerAction( MCFExtendedContextInterface& aCFContext,
        MCFActionInterface& aCFAction,
        MCFScriptInterface& aScriptInterface );
    void ConstructL();

private:

    // Own: Subscription message
    RMessage2 iSubscriptionListenMessage;

    // Own: Action indications array
    RPointerArray<CCFActionIndication> iActionQueue;

    // Own: Current action indciation
    CCFActionIndication* iCurrentAction;
    };

#endif//C_CFMESSAGEHANDLERACTION_H


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
* Description:  Message handler for contexts.
 *
*/


#ifndef C_CFMESSAGEHANDLERCONTEXT_H
#define C_CFMESSAGEHANDLERCONTEXT_H

#include "CFMessageHandlerBase.h"
#include "cfcontextobject.h"
#include "cfcontextsourceinterface.h"

class CCFContextIndicationImpl;
class CCFContextSubscription;
class CCFContextQuery;
class CCFContextObjectImpl;
class CCFContextSourceCommand;

/**
 * Context Framework server session message handler base class.
 */
NONSHARABLE_CLASS( CCFMessageHandlerContext ):
public CCFMessageHandlerBase,
public MCFContextSubscriptionListener, public MCFContextSource
    {
private:

    // Subscription error
    struct TSubscriptionError
        {
        TInt iError;
        CCFContextObjectImpl* iContext;
        };

public:

    // Two phased constructors
    static CCFMessageHandlerContext* NewL( MCFExtendedContextInterface& aCF,
        MCFActionInterface& aActivatorEngine,
        MCFScriptInterface& aScriptInterface );
    static CCFMessageHandlerContext* NewLC( MCFExtendedContextInterface& aCF,
        MCFActionInterface& aActivatorEngine,
        MCFScriptInterface& aScriptInterface );

    // Destructor
    ~CCFMessageHandlerContext();

protected: // From base classes

    // From CCFMessageHandlerBase
    TBool HandleMessageL( const RMessage2& aMessage );

    // From MCFContextSubscriptionListener
    void ContextIndicationL( CCFContextIndication* aIndication );

    // From MCFContextSubscriptionListener
    TInt Client( RThread& aThread ) const;

    // From MCFContextSubscriptionListener
    void HandleContextSubscriptionError( TInt aError,
        const TDesC& aSource,
        const TDesC& aType );

private: // from MCFContextSource

    // from MCFContextSource
    void Subscribers( const TDesC& aContextSource,
        const TDesC& aContextType );

    // from MCFContextSource
    void NoSubscribers( const TDesC& aContextSource,
        const TDesC& aContextType );

    // from MCFContextSource
    void HandleCommand( const CCFContextSourceCommand& aCommand );

private: // New methods

    // Publishes a context
    TInt PublishContext( CCFContextObject& aContext,
        RThread& aClientThread );

    // Adds a new subscription to CF
    TInt AddSubscription( CCFContextSubscription* aSubscription,
        RThread& aClientThread );

    // Removes subscription
    void RemoveSubscription(CCFContextSubscription& aSubscription);

    // Starts receiving context indication
    void ReceiveContextIndicationMessageL( const RMessage2& aMessage );

    // Cancels receive context indication message
    void CancelReceiveContextIndicationMessage();

    // Requests context
    TInt RequestContext( CCFContextQuery& aContextQuery,
        RThread& aClientThread );

    // Indicate subscription error
    void IndicateSubscriptionErrorL();

    // Get next indication from queue
    void CompleteNextIndicationFromQueueL( const RMessage2& aMessage );

    // Checks context indication queue
    void CheckContextIndicationQueueL();

    // Write context request buffer into stream
    void CompleteRequestContextL( const RMessage2& aMessage );

    // Handles error situations
    void DoHandleContextSubscriptionErrorL( TInt aError,
        const TDesC& aSource,
        const TDesC& aType );

private: // new message system

    void ReceiveMessageL( const RMessage2& aMessage );

    // Cancels receive message
    void CancelReceiveMessage();

    void CompleteNextMessageFromQueueL( const RMessage2& aMessage );

    void CheckMessageQueueL();
    
    // Leavable version from subscribers callback
    void SubscribersL( const TDesC& aContextSource, const TDesC& aContextType );

    // Leavable version from no subscribers callback
    void NoSubscribersL( const TDesC& aContextSource, const TDesC& aContextType );

    // Leavable version from handle command callback
    void HandleCommandL( const CCFContextSourceCommand& aCommand );

private:

    CCFMessageHandlerContext( MCFExtendedContextInterface& aCFContext,
        MCFActionInterface& aCFAction,
        MCFScriptInterface& aScriptInterface );
    void ConstructL();

private: // Data

    // Receive message
    RMessage2 iReceiveMessage;

    // Context request buffer
    RContextObjectArray iContextRequestBuffer;

    // Context indication queue
    RPointerArray<CCFContextIndicationImpl> iIndicationQueue;

    // Own: Subscription errors
    RArray<TSubscriptionError> iSubscriptionErrors;

    // Receive message for context source related messages
    RMessage2 iReceiveContextSourceMsg;

    // Own: array of messages to client
    RPointerArray<HBufC8> iMessageQueue;
    };

#endif

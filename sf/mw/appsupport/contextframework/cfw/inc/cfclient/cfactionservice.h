/*
* Copyright (c) 2008-2008 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  CCFActionService class declaration.
*
*/


#ifndef CFACTIONSERVICE_H
#define CFACTIONSERVICE_H

// SYSTEM INCLUDE FILES

// USER INCLUDE FILES
#include "cfservicebase.h"

// FORWARD DECLARATIONS
class RCFClientSession;
class CCFActionSubscription;
class CCFActionSubscriptionListener;

// DATA TYPES

// CLASS DECLARATION

/**
 * Services class to handle action related features.
 *
 * @lib cfclient.lib
 * @since S60 5.0
 */
NONSHARABLE_CLASS( CCFActionService ): public CCFServiceBase
    {
public:

    /**
     * Symbian two phased constructors.
     *
     * @since S60 5.0
     * @param None.
     * @return CCFActionService*
     */
    static CCFActionService* NewL( RCFClientSession& aSession,
        MCFListener& aListener );
    static CCFActionService* NewLC( RCFClientSession& aSession,
        MCFListener& aListener);

    /**
     * C++ destructor.
     */
    virtual ~CCFActionService( );

public:

    // @see CCFClient
    TInt DefineAction( const TDesC& aActionIdentifier,
        const TSecurityPolicy& aSecurityPolicy );
        
    // @see CCFClient
    void SubscribeActionL(
        CCFActionSubscription& aSubscription );
        
    // @see CCFClient
    void UnsubscribeActionL(
        CCFActionSubscription& aSubscription );

private:

    // Stream action subscription
    HBufC8* WriteStreamActionSubscriptionLC(
        CCFActionSubscription& aSubscription );

private:

    CCFActionService( RCFClientSession& aSession, MCFListener& aListener );
    void ConstructL( );

private: // Data

    /** Action subscription listener */
    CCFActionSubscriptionListener* iActionSubscriptionListener;
    };

#endif // CFACTIONSERVICE_H


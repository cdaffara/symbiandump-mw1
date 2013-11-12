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
* Description:  Action interface in CF.
*
*/


#ifndef M_CFACTIONINTERFACE_H
#define M_CFACTIONINTERFACE_H

class CCFActionSubscription;
class MCFActionSubscriptionListener;
class MCFActionOwner;
class RThread;

/**
* Interface for actions.
*/
class MCFActionInterface
    {
    public:
    
        /**
        * Defines action.
        * Action is identified by an identifier and security policy.
        * Possible error values:
        * - KErrAlreadyExists, action already defined
        *
        * @since S60 5.0
        * @param aActionIdentifier Action identifier.
        * @param aSecurityPolicy Security level for the action.
        * @param aOwnerPtr Pointer to the owner.
        * @return None
        */
        virtual TInt DefineAction( const TDesC& aActionIdentifier,
            const TSecurityPolicy& aSecurityPolicy,
            const TUid& aOwnerUid,
            MCFActionOwner* aOwner ) = 0;
        
        /**
        * Subscribes for an action. When action is triggered
        * a notification is given through MCFActionSubscriptionListner
        * interface.
        * Possible error codes:
        * - KErrNotFound, specified action not defined
        * - KErrAccessDenied, not enough capabilities for the action
        *
        * @since S60 5.0
        * @param aActionSubscription Action subscription.
        * @param aListener Action notification listener.
        * @param aClientThread Client thread.
        * @return KErrNone if no errors.
        */
        virtual TInt SubscribeAction(
            CCFActionSubscription* aActionSubscription,
            MCFActionSubscriptionListener* aListener,
            RThread& aClientThread ) = 0;
        
        /**
        * Unsubscribes an action.
        *
        * @since S60 5.0
        * @param aActionSubscription Action subscription.
        * @param aListener Action notification listener.
        * @return None
        */
        virtual void UnsubscribeAction( CCFActionSubscription& aActionSubscription,
            MCFActionSubscriptionListener& aListener ) = 0;
            
        /**
        * Removes all subscriptions from the particular subscriber.
        *
        * @since S60 5.0
        * @param aListener Subscription listener.
        * @return None.
        */
        virtual void UnsubscribeActions( MCFActionSubscriptionListener& aListener ) = 0;
        
        /**
         * Inactivates actions defined by the client when the client
         * session is closed.
         * 
         * @since S60 5.0
         * @param aOwnerPtr Matching pointer to the owner.
         * @return None.
         */
        virtual void DeregisterActions( MCFActionOwner* aOwner ) = 0;

    protected:
    
        // Deny destruction through this class
        virtual ~MCFActionInterface(){};
    };

#endif//M_CFACTIONINTERFACE_H

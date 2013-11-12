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
* Description:  CFActivatorCallBackInterface declaration.
*
*/


#ifndef M_CFACTIVATORCALLBACKINTERFACE_H
#define M_CFACTIVATORCALLBACKINTERFACE_H

class CCFActionSubscription;
class MCFActionSubscriptionListener;
class TSecurityPolicy;
class MCFActionOwner;

/**
 *  Activator engine abstract interface.
 *  Activator engine interface provides possibility to
 *  define actions from client process and
 *  subscribe actions from client process.
 *
 *  @lib -
 *  @since S60 4.0
 */
 class MCFActivatorCallBackInterface  
    {
    public:
     
        /**
        * Defines action from client process.
        * Every action is defined with identifier and security policy.
        * Possible leave codes:
        * - KErrAlreadyExists, action already defined
        *
        * @since S60 5.0
        * @param aActionIdentifier Action to define.
        * @param aSecurityPolicy Security policy for the action.
        * @param aOwnerUid Uid of the owner process.
        * @param aOwnerPtr Pointer to the owner.
        * @return None
        */
        virtual void DefineActionL( const TDesC& aActionIdentifier,
            const TSecurityPolicy& aSecurityPolicy,
            const TUid& aOwnerUid,
            MCFActionOwner* aOwner ) = 0;

        /**
        * Subscribes for an action.
        * When this action is triggered from a script listener
        * will be informed through MCFActionSubscriptionListener interface.
        * Possible leave codes:
        * - KErrAlreadyExists, action already defined
        * - KErrAccessDenied, insufficient capabilities
        * 
        * @since S60 5.0
        * @param aActionSubscription Action subcription.
        * @param aListener Callback interface.
        * @return None
        */
        virtual void SubscribeActionL(
            CCFActionSubscription* aActionSubscription,
            MCFActionSubscriptionListener* aListener,
            RThread& aClientThread ) = 0;

        /**
        * Unsubscribes action. If action is not defined nothing happens.
        *
        * @since S60 5.0
        * @param aActionSubscription Subscription to unsubscribe.
        * @param aListener Callback interface.
        * @return None
        */
        virtual void UnsubscribeAction(
            CCFActionSubscription& aActionSubscription,
            MCFActionSubscriptionListener& aListener ) = 0;
            
        /**
        * Removes all subscriptions from the particular subscriber.
        *
        * @since S60 5.0
        * @param aListener Subscription listener.
        * @return None.
        */
        virtual void RemoveSubscriptions(
            MCFActionSubscriptionListener& aListener ) = 0;
        
        /**
         * Inactivates actions defined by the client when the client
         * session is closed.
         * 
         * @since S60 5.0
         * @param aOwnerPtr Matching pointer to the owner.
         * @return None.
         */
        virtual void DeregisterActions( MCFActionOwner* aOwner ) = 0;
    };

#endif

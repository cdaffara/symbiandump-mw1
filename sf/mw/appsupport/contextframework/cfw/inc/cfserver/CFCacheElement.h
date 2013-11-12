/*
* Copyright (c) 2002-2007 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  CCFCacheElement class declaration.
*
*/


#ifndef C_CFCACHEELEMENT_H
#define C_CFCACHEELEMENT_H

#include <e32base.h>

#include "cfcontextobject.h"

class CCFContextSubscriptionImpl;
class MCFContextSource;

/**
* Encapsulates one cache element.
*/
NONSHARABLE_CLASS( CCFCacheElement ): public CBase
    {
    public:
    
        static CCFCacheElement* NewL ();
        static CCFCacheElement* NewLC ();
        
        ~CCFCacheElement();
        
    public: // New methods
    
        /**
        * Returns context object associated in this cache element.
        * 
        * @since S60 5.0
        * @param None
        * @return CCFContextObject&
        */
        CCFContextObject& Context() const;

        /**
        * Adds a new subscription in to the cache element.
        * 
        * @since S60 5.0
        * @param aSubscription New subscription.
        * @return None
        */
        void AddSubscriptionL( CCFContextSubscriptionImpl& aSubscription );

        /**
        * Removes a subscription from the cache element.
        * 
        * @since S60 5.0
        * @param aIndex Index to the subscription in the array to remove.
        * @return None
        */
        void RemoveSubscription( TInt aIndex );

        /**
        * Nullifies a subscription from the cache element by setting the pointer
        * NULL.
        * 
        * @since S60 5.0
        * @param aIndex Index to the subscription in the array to nullify.
        * @return None
        */
        void NullifySubscription( TInt aIndex );

        /**
        * Removes all NULL subscriptions from the cache element.
        * 
        * @since S60 5.0
        * @return None
        */
        void RemoveNullSubscriptions();

        /**
        * Returns subscriptions associated in this cache element.
        * 
        * @since S60 5.0
        * @param None
        * @return RPointerArray<CCFContextSubscriptionImpl>&
        */
        const RPointerArray<CCFContextSubscriptionImpl>& Subscriptions() const;
        
        /**
        * Set a read security policy associated in this cache element.
        * 
        * @since S60 5.0
        * @param aSecurityPolicy Security policy associated to
        *  this cache element.
        * @return None.
        */
        void SetReadSecurityPolicy( const TSecurityPolicy& aSecurityPolicy );
        
        /**
        * Returns read security policy associated in this cache element.
        * 
        * @since S60 5.0
        * @param None
        * @return const TSecurityPolicy&
        */
        const TSecurityPolicy& GetReadSecurityPolicy() const;

        /**
         * Set a write security policy associated in this cache element.
         * 
         * @since S60 5.0
         * @param aSecurityPolicy Security policy associated to
         *  this cache element.
         * @return None.
         */
        void SetWriteSecurityPolicy( const TSecurityPolicy& aSecurityPolicy );
         
         /**
         * Returns write security policy associated in this cache element.
         * 
         * @since S60 5.0
         * @param None
         * @return const TSecurityPolicy&
         */
         const TSecurityPolicy& GetWriteSecurityPolicy() const;

         /**
          * Sets the context source of this cache element.
          * @param aContextPublisher Publisher of the context.
          * Can also be set to NULL.
          */
         void SetContextPublisher( MCFContextSource* aContextPublisher );

         /**
          * Sets the context source of this cache element.
          * @param aContextPublisher Publisher of the context.
          * Can also be set to NULL.
          * @param aUid Context publisher's uid.
          */
         void SetContextPublisher(
                MCFContextSource* aContextPublisher, const TUid& aUid );

         /**
          * Get the context source of this cache element.
          * @return Cache element, may be NULL if not given.
          */
         const MCFContextSource* ContextPublisher() const;
         

         /**
          * Get UID of the context publisher process.
          * @return UID of the context publisher process.
          */
         const TUid& PublisherUid() const;

         
        /**
        * Compares two cache elements by source and type.
        * 
        * @since S60 5.0
        * @param aFirst First cache element.
        * @param aSecond Second cache element.
        * @return KErrNone if same.
        */
        static TInt CompareByTypeAndSource(
            const CCFCacheElement& aFirst,
            const CCFCacheElement& aSecond );

        /**
        * Compares two cache elements by type.
        * 
        * @since S60 5.0
        * @param aFirst First cache element.
        * @param aSecond Second cache element.
        * @return KErrNone if same.
        */
        static TInt CompareByType(
            const CCFCacheElement& aFirst,
            const CCFCacheElement& aSecond );
            
    private:

        CCFCacheElement();
        void ConstructL();
        
    private: // Data
    
        // Own: Context related to the cache element
        CCFContextObject* iContext;
        
        // Own: Subscriptions for the context
        RPointerArray<CCFContextSubscriptionImpl> iSubscriptions;
        
        // Own: Read security policy for the context
        TSecurityPolicy iReadSecurityPolicy;
        
        // Own: Write security policy for the context
        TSecurityPolicy iWriteSecurityPolicy;
        
        // Ref: Reference to the provider of this context
        MCFContextSource* iPublisher;
        
        // Own: UID of the context publisher;
        TUid iPublisherUid;
        
    };

#endif

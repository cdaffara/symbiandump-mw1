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


#ifndef C_CFCONTEXTSUBSCRIPTIONIMPL_H
#define C_CFCONTEXTSUBSCRIPTIONIMPL_H

#include "cfcontextsubscription.h"

// FORWARD DECLARATIONS

class MCFContextSubscriptionListener;
class CCFContextQueryImpl;
class CCFContextObject;
class CCFContextDataObject;
class RWriteStream;

/**
* Subscription for observing context value changes for certain
* context type or a subtree of context types coming from certain
* source or all sources.
* 
* Information about context changes will be sent to object implementing
* <code>MCFListener</code>-interface.
*
* @since S60 5.0
* @lib cfservices.lib
*/
NONSHARABLE_CLASS( CCFContextSubscriptionImpl ): public CCFContextSubscription
    {
    public:

        /**
        * Two phased construction.
        *
        * @since S60 5.0
        * @param None.
        * @return CCFContextSubscriptionImpl*
        */
        IMPORT_C static CCFContextSubscriptionImpl* NewL();
        IMPORT_C static CCFContextSubscriptionImpl* NewLC();

        /**
        * Two phased construction with parameters.
        *
        * @since S60 5.0
        * @param aStream Stream where to initialize.
        * @return CCFContextSubscriptionImpl*
        */
        IMPORT_C static CCFContextSubscriptionImpl* NewL (
        	RReadStream& aStream);
        IMPORT_C static CCFContextSubscriptionImpl* NewLC (
        	RReadStream& aStream);

        // Destructor.
        ~CCFContextSubscriptionImpl();

    public: // From base classes
    
        // @see CCFContextSubscription
        void SetContextTypeL( const TDesC& aContextType );
    
        // @see CCFContextSubscription
        const TDesC& ContextType() const;
    
        // @see CCFContextSubscription
        void SetContextSourceL( const TDesC& aSource );
    
        // @see CCFContextSubscription
        const TDesC& ContextSource() const;

        // @see CCFContextSubscription
		TBool SubTypeMatch() const;
		
        // @see CCFContextSubscription
		void SetSubTypeMatch( TBool aMatchSubTypes );

        // @see CCFContextSubscription
		TBool OnlyChangesIndicated() const;

        // @see CCFContextSubscription
		void SetOnlyChangesIndicated( TBool aIndicateChangesOnly );
		
        // @see CCFContextSubscription
		void SetDataObjectEnabled( TBool aEnabled );
		
        // @see CCFContextSubscription
		TBool DataObjectEnabled() const;
    
    public: // New methods

        /**
        * Internalizes object.
        *
        * @since S60 5.0
        * @param aStream Stream from which to internalize.
        * @return None.
        */
        IMPORT_C void InternalizeL( RReadStream& aStream );

        /**
        * Externalizes object.
        * 
        * @since S60 5.0
        * @param aStream Stream where to externalize.
        * @return None
        */
        IMPORT_C void ExternalizeL( RWriteStream& aStream );

        /**
        * Gets object size in bytes.
        *
        * @since S60 5.0
        * @param None
        * @return TInt
        */
        IMPORT_C TInt Size() const;

        /**
        * Returns subscriber of this subscription.
        *
        * @since S60 5.0
        * @param None.
        * @return MCFContextSubscriptionListener&
        */
        IMPORT_C MCFContextSubscriptionListener& SubscriptionListener() const;
        
        /**
        * Sets a subscriber for this subscription.
        *
        * @since S60 5.0
        * @param aListener Call back to subscriber.
        * @return None.
        */
        IMPORT_C void SetSubscriptionListener(
            MCFContextSubscriptionListener& aListener );

        /**
        * Evaluates context change event coming from data manager and sends
        * indication to the subscriber if it matches this subscription.
        *
        * @since S60 5.0
        * @param aOldContext Previous context.
        * @param aNewContext Current context.
        * @return
        */
        IMPORT_C void EvaluateContextChangeL(
            const CCFContextObject& aNewContext,
            const CCFContextObject& aOldContext, TBool aValueChanged ) const;

        /**
        * Tells whether this instance is the same as the argument.
        * Comparison is made based on client session, subscribed context source
        * and type.
        *
        * @return ETrue if this instance equals the argument. Otherwise, EFalse.
        */
        IMPORT_C TBool IsSame(
            const CCFContextSubscriptionImpl& aSubscription) const;

        /**
        * Checks if a context type matches with this subscription.
        * 
        * @since S60 5.0
        * @param aContextType Context type to match.
        * @return ETrue if a match.
        */
        IMPORT_C TBool Matches( const TDesC& aContextType ) const;

        /**
        * Checks if a context source and a context type matches
        * this subscription.
        *
        * @since S60 5.0
        * @param aContextType Context type to match.
        * @param aContextSource Context source to match.
        * @return ETrue if a match.
        */
		IMPORT_C TBool Matches( const TDesC& aContextType,
			const TDesC& aContextSource ) const;

        /**
        * Evaluates context change event coming from data manager and sends
        * indication to the subscriber if it matches this subscription.
        *
        * @since S60 5.0
        * @param aNewContext Current context.
        * @param aData Data object associated with the context.
        * @param aValueChanged Value changed.
        * @return
        */
        IMPORT_C void EvaluateContextChangeL(
            const CCFContextObject& aNewContext,
            CCFContextDataObject& aData,
            TBool aValueChanged ) const;
            
    private:
    
        // Check if a context notification is needed.
        TBool NotificationNeeded( const CCFContextObject& aNewContext,
            TBool aValueChanged ) const;

    private:
    
        // C++ constructor
        CCFContextSubscriptionImpl();
        
        // 2nd phase constructors
        void ConstructL();
        void ConstructL( RReadStream& aStream );

    private:

        /** Context query related to subscription */
        CCFContextQueryImpl* iContextQuery;

        /** A call back interface to the subscriber */
        MCFContextSubscriptionListener* iSubscriptionListener;

        /** Flag indicating whether only changes in context value are indicated */
        TBool iOnlyChangesIndicated;
        
        /** Flag indicating that subscriber wants also data objects */
        TBool iDataObjectEnabled;
    };

#endif

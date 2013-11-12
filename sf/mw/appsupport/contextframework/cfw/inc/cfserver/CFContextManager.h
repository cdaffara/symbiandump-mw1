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
* Description:  CFContextManager.
*
*/



#ifndef C_CFCONTEXTMANAGER_H
#define C_CFCONTEXTMANAGER_H

// INCLUDES
#include "cfcontextobject.h"
#include "CFSecurityChecker.h"

#include <e32base.h>
#include <f32file.h>

#include "CFContextQueryImpl.h"
#include "CFContextObjectImpl.h"


// FORWARD DECLARATIONS
//class TCFSubscription;
class CCFCacheElement;
class CCFContextSubscription;
class CCFContextSubscriptionImpl;
class MCFContextSubscriptionListener;
class MCFContextSource;
class CCFContextDataObject;

typedef TBool ( CCFContextQueryImpl::*queryMatchFunction )
( const TDesC&, const TDesC& ) const;


/**
* Front-end class for data storage access.
*  Data storage service provider for CF Server. This manager stores contexts
*  into a cache.
*
*  The cache stores the latest context for each type for fast change
*  comparison (and quick query latest response).
*/
NONSHARABLE_CLASS( CCFContextManager ): public CBase
    {
    public: // constructors and destructor
    
        static CCFContextManager* NewL( MCFSecurityChecker& aSecurityChecker );
        static CCFContextManager* NewLC( MCFSecurityChecker& aSecurityChecker );
    
        ~CCFContextManager();
        
    public: // new functions

	    /**
	     * Defines a new context in Context Framework.
	     * Every context needs to be defined before it can be published
	     * and subscribed.
	     * If the context is already defined KErrPermissionDenied is
	     * returned.
	     *
	     * @since S60 5.0
	     * @param aContextSource Source of the context.
	     * @param aContextType Type of the context.
	     * @param aReadSecurityPolicy Needed read capabilities for the context.
	     * @param aWriteSecurityPolicy Needed write capabilities for the context
	     * @return None
	     */
	    void DefineContextL( const TDesC& aContextSource,
	    	    const TDesC& aContextType,
	    	    const TSecurityPolicy& aReadSecurityPolicy,
	    	    const TSecurityPolicy& aWriteSecurityPolicy);
    
	    /**
	     * Defines a new context in Context Framework.
	     * Every context needs to be defined before it can be published
	     * and subscribed.
	     * If the context is already defined KErrPermissionDenied is
	     * returned.
	     *
	     * @since S60 5.0
	     * @param aContextSource Source of the context.
	     * @param aContextType Type of the context.
	     * @param aReadSecurityPolicy Needed read capabilities for the context.
	     * @param aWriteSecurityPolicy Needed write capabilities for the context
	     * @param aContextPublisher Interface to which the information whether
	     * this context has subscribers or not is reported, can be also NULL.
	     * @param aPublisherUid Secure UID of the process which registered
	     * this context. Only process with the same SID is able to reregister
	     * context in the purpose registering aContextPublisher.
	     * @return None
	     */
        void DefineContextL( const TDesC& aContextSource,
        	    const TDesC& aContextType,
        	    const TSecurityPolicy& aReadSecurityPolicy,
        	    const TSecurityPolicy& aWriteSecurityPolicy,
        	    MCFContextSource* aContextPublisher,
        	    const TUid& aPublisherUid );
    
        /**
         * Publishes a new context object.
         * The new context value is cached and needed indications
         * for context subscribers are sent.
         * Leaves with:
         * - KErrNotFound, context not defined
         * - KErrAccessDenied, insufficient capabilities to write the context
         * 
         * @since S60 5.0
         * @param aContext New context object.
         * @param aClientThread Handle to the current client.
         * @return None.
         */
        void PublishContextL( CCFContextObject& aContext,
            RThread& aClientThread );

        /**
         * Publishes a new context object.
         * The new context value is cached and needed indications
         * for context subscribers are sent.
         * The data object is also sent to subscribers currently
         * interested from the data.
         * 
         * @since S60 5.0
         * @param aContext New context object.
         * @param aClientThread Handle to the current client.
         * @param aData Data object associated with the context.
         * @return None.
         */
        void PublishContextL( CCFContextObject& aContext,
            RThread& aClientThread,
            CCFContextDataObject& aData );

        /**
         * Gets contexts of specified type.
         * Tries to retrieve from cache the requested contexts using the given
         * request (query) type. The requested context type can be partial
         * ontology path.
         *
         * @param aResultBuffer Storage for the requested contexts.
         * @param aContextQuery Settings for the request (query).
         */
        void RequestContextL(
            RContextObjectArray& aResultBuffer,
            CCFContextQuery& aContextQuery,
            RThread& aClientThread );
        
        /**
         * Requests specified context object.
         * @param aContextType: Specifies the context type requested
         * @param aContextSource: Specifies the context source
         * @return CCFContextObject*
         */
        CCFContextObject* RequestContextL(
            const TDesC& aContextType,
            const TDesC& aContextSource,
            RThread& aClientThread );

		/**
        *
        * @since S60 5.0
        * @param aParam1 Explanation.
        * @param aParam2 Explanation.
        * @return None
        */    
        void SubscribeContextL( CCFContextSubscription* aSubscription,
    		MCFContextSubscriptionListener* aListener,
    		RThread& aClientThread );

		/**
        *
        * @since S60 5.0
        * @param aParam1 Explanation.
        * @param aParam2 Explanation.
        * @return ETrue if cache element(s) was added to cleanup array
        *   (subscription was nullified, not removed), EFalse otherwise.
        */    
        TBool RemoveSubscription( CCFContextSubscription& aSubscription,
    		MCFContextSubscriptionListener& aListener );

		/**
        *
        * @since S60 5.0
        * @param aParam1 Explanation.
        * @param aParam2 Explanation.
        * @return ETrue if cache element(s) was added to cleanup array
        *   (subscription(s) was nullified, not removed), EFalse otherwise.
        */    
        TBool RemoveSubscriptions(
            MCFContextSubscriptionListener* aListener );

        /**
        * Cleanups context cache element's subscriptions by removing nullified
        * subscriptions for all cache elements added to the cleanup array.
        * 
        * @since S60 5.0
        * @return None.
        */
        void CleanupCacheElementSubscriptions();

        /**
 		* Gets the read security policy of particular action.
 		*
 		* @param aContextSource Context Source.
 		* @param aContextType Context Type.
 		* @param aPolicy security policy which will be set by this method.
 		* @return None.
 		*/
		void GetReadSecurityPolicyL( const TDesC& aContextSource,
            const TDesC& aContextType,
			TSecurityPolicy& aPolicy );
		
		/**
		* Gets the write security policy of particular action.
		*
		* @param aContextSource Context Source.
		* @param aContextType Context Type.
		* @param aPolicy security policy which will be set by this method.
		* @return None.
		*/
		void GetWriteSecurityPolicyL( const TDesC& aContextSource,
	            const TDesC& aContextType,
				TSecurityPolicy& aPolicy );
        
		/**
		 * Deregister context publisher for all contexts it has been
		 * registered as a publisher.
		 * This has to be called before context publisher can be deleted.
		 * @param aPublisher Publisher to be deregistered.
		 */
		void DeregisterPublisher( MCFContextSource& aPublisher );

        /**
         * Validates that the context can be published.
         * Checks that the context is defined and the client has sufficient
         * capabilities to write to the context.
         * 
         * Leaves with:
         * - KErrNotFound, context not defined
         * - KErrAccessDenied, insufficient capabilities to write the context
         * 
         * @since S60 5.0
         * @param aContext New context object.
         * @param aClientThread Handle to the current client.
         * @return None.
         */
        void ValidatePublishContextL( CCFContextObject& aContext,
            RThread& aClientThread );

        /**
         * Executes the actual publishing for the context.
         * ValidatePublishContextL() must have been called before calling this
         * because security is not checked by this function.
         * 
         * Leaves with:
         * - KErrNotFound, context not defined
         * - any other system-wide error code
         * 
         * @since S60 5.0
         * @param aContext New context object.
         * @param aData Data object associated with the context.
         * @return None.
         */
        void DoPublishContextL( CCFContextObject& aContext,
            CCFContextDataObject* aData = NULL );

    private: // Constructors

        CCFContextManager( MCFSecurityChecker& aSecurityChecker );
        void ConstructL();

    private: // New functions


        // Notifies data manager that new subscription has been added.
        void SubscriptionAddedL( CCFContextSubscriptionImpl& aSubscription,
            RThread& aClientThread );
        
        // Notifies data manager that subscription has been removed.
        // Returns value ETrue if cache element(s) was added to cleanup array,
        // EFalse otherwise.
        TBool SubscriptionRemoved( CCFContextSubscriptionImpl& aSubscription );

        // Finalizes security check.        
        TInt DoReadSecurityCheck( CCFCacheElement& aContextCache,
        	    RThread& aClientThread );
        
        // Checks if the particular client has write access to particular
        // cache element
        TInt DoWriteSecurityCheck( CCFCacheElement& aContextCache,
        	    RThread& aClientThread );

        // Gets latest context values from context cache based on the specified
        // query.
        void GetLatestFromCacheL( 
            RContextObjectArray& aLatestContexts, 
            queryMatchFunction aMatchFunction, 
            const CCFContextQueryImpl& aContextQuery,
            RThread* aClientThread );
            
        // Logs context cache
        void LogContextCache();

        // Checks if the current client is from the same process
        TBool RequestFromSameProcess( RThread& aClientThread );
        
        // Request contexts
        void DoRequestContextL( RContextObjectArray& aResultBuffer,
            CCFContextQuery& aContextQuery,
            RThread* aClientThread );
		    
        // From MCFSubscriptionManager
        void CacheElementAddedL( CCFCacheElement& aCacheElement );
        
        // Checks if context re-define is allowed
        TBool RedefineAllowed( CCFCacheElement& aElement,
            const MCFContextSource* aSource,
            const TUid& aUid ) const;

    private: // Member variables

        // Previous context instance for subscriber indications.
        CCFContextObject* iPreviousContext;

        // Context cache.
        RPointerArray<CCFCacheElement> iContextCache;

        // A "linear order" instance.
        TLinearOrder<CCFCacheElement>* iCacheOrder;

        //
        CCFCacheElement* iCacheSearchKey;
        
        // Own: Subscriptions
        RPointerArray<CCFContextSubscriptionImpl> iSubscriptions;
        
        // Security checker interface.
        MCFSecurityChecker* iSecurityChecker;

        // Signals SubscriptionRemoved() that manager is in the middle of
        // publishing context. Needed for context operations and context sources
        // removing their subscriptions based on context indication, and doing
        // it as a synchronous operation during the indication process.
        // Not owned.
        CCFCacheElement* iPublishingForCacheElement;

        // Cleanup array for cache elements having nullified subscriptions,
        // pointers are not owned.
        RPointerArray< CCFCacheElement > iCleanupCacheElements;
    };

#endif  // C_CFCONTEXTMANAGER_H

// End of File

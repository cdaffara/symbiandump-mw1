/*
* Copyright (c) 2006-2007 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  MCFContextInterface interface description.
*
*/


#ifndef M_CFCONTEXTINTERFACE_H
#define M_CFCONTEXTINTERFACE_H

#include <cfcontextquery.h>
#include <cfcontextobject.h>

class MCFContextSource;
class MCFContextSubscriptionListener;
class CCFContextSubscription;
class CCFContextDataObject;

/**
* MCFContextInterface provides possibility to use CF services related to
* context manipulation:
* - Define contexts
* - Publish contexts
* - Subscribe contexts
* - Unsubscribe contexts
* - Request contexts
* Every context source plug-in has a reference to MCFContextInterface
* by default.
*
* Define context:
* Before any context can be used in any way it must be defined.
* Context is defined by a source, type and security policy:
* @code
*  _LIT( KSource, "Source" );
*  _LIT( KType, "Type" );
*  _LIT_SECURITY_POLICY_PASS( KPassSec );
*  TInt err = iCF.DefineContext( KSource, KType, KPassSec );
*  if( err != KErrNone )
*    {
*    // Handle error...
*    }
* @endcode
*
* Publish context:
* To publish a context the context being publish needs to be
* defined. To publish a context a new context object must be configured.
* Context object is configured by setting a source, type and a new
* value for the context:
* @code
*  _LIT( KSource, "Source" );
*  _LIT( KType, "Type" );
*  _LIT( KValue, "NewValue" );
*  CCFContextObject* context = CCFContextObject::NewLC();
*  context->SetSourceL( KSource );
*  context->SetTypeL( KType );
*  context->SetValueL( KValue );
*  RThread thread;
*  CleanupClosePushL( thread );
*  TInt err = iCF.PublishContext( *context, thread );
*  if( err != KErrNone )
*    {
*    // Handle error...
*    }
*  CleanupStack::PopAndDestroy( &thread );
*  CleanupStack::PopAndDestroy( context );
* @endcode
* It is suggested that only one CCFContextObject* instance is kept
* as a member variable to save memory.
*
* Subscribe context:
* To subscribe a context the context being subscribed needs to be
* defined. A context subscription will not lead to an error
* even a context which has not been defined is subscribed.
* Context indications will be received only after the context
* subscribed is defined.
* To subscribe a context a new context subscription is made and
* passed it to Context Framework:
* @code
*  _LIT( KSource, "Source" );
*  _LIT( KType, "Type" );
*  CCFContextSubscription* subscription = CCFContextSubscription::NewLC();
*  subscription->SetContextSourceL( KSource );
*  subscription->SetContextTypeL( KType );
*  RThread thread;
*  CleanupClosePushL( thread );
*  TInt err = iCF.SubscribeContext( *subscription, this, thread );
*  if( err != KErrNone )
*    {
*    // Handle error...
*    }
*  CleanupStack::PopAndDestroy( &thread );
*  CleanupStack::PopAndDestroy( subscription );
* @endcode
* It is assumed that 'this' is derived directly from
* MCFContextSubscriptionListener interface. Context subscription
* can also be configured more to give more precise indications.
*
* Request context:
* To request a context the context being requested needs to be
* defined.
* Context is requested by defining a context query and passing it
* to Context Framework:
* @code
*  _LIT( KSource, "Source" );
*  _LIT( KType, "Type" );
*  CCFContextQuery* query = CCFContextQuery::NewLC( KSource, KType );
*  RThread thread;
*  CleanupClosePushL( thread );
*  RContextObjectArray results;
*  TInt err = iCF.RequestContext( results, *query, thread );
*  if( err == KErrNone && results.Count() )
*    {
*    // Handle request results...
*    }
*  else
*    {
*    // Handle error...
*    }
*  results.ResetAndDestroy();
*  CleanupStack::PopAndDestroy( &thread );
*  CleanupStack::PopAndDestroy( query );
* @endcode
*
* Remove subscriptions:
* If a context source plug-in has subscribed contexts it is necessary
* that also all the contexts will be unsubscribed. Unsubscription can
* be done by defining a single subscription or to remove all the
* subscriptions from the particular subscriber:
* @code
*  _LIT( KSource, "Source" );
*  _LIT( KType, "Type" );
*  CCFContextSubscription* subscription = CCFContextSubscription::NewLC();
*  subscription->SetContextSourceL( KSource );
*  subscription->SetContextTypeL( KType );
*  iCF.UnsubscribeContext( *subscription, *this );
*  CleanupStack::PopAndDestroy( subscription );
* @endcode
* @code
* iCF.UnsubscribeContexts( *this );
* @endcode
* It is assumed that 'this' is derived directly from
* MCFContextSubscriptionListener interface.
*
* @since S60 5.0
* @lib -
*/
class MCFContextInterface
    {
    public:
    
        /**
        * Defines a new context in Context Framework.
        * Every context needs to be defined before it can be published,
        * subscribed or requested.
        *
        * Security policy for reading and writing is the same.
        * 
        * If some client subscribes for this context, the contex owner
        * will not be notified. Use one of the followinf DefineContext
        * methods to receive context susbsription notifications.
        *
        * @since S60 5.0
        * @param aContextSource Source of the context.
        * @param aContextType Type of the context.
        * @param aReadWritePolicy Needed capabilities for reading and writing
        *		of the context.
        * @return Possible return values:
        * - KErrNone, no errors
        * - KErrAlreadyExists, defined context already exists.
        */
        virtual TInt DefineContext( const TDesC& aContextSource,
            const TDesC& aContextType,
            const TSecurityPolicy& aReadWritePolicy ) = 0;
        
        /**
        * Defines a new context in Context Framework.
        * Every context needs to be defined before it can be published,
        * subscribed or requested.
        *
        * Security policy for reading and writing is the same.
        * 
        * If some client subscribes for this context, the contex owner
        * will be notified through MCFContextSource interface.
        *
        * @since S60 5.0
        * @param aContextSource Source of the context.
        * @param aContextType Type of the context.
        * @param aReadWritePolicy Needed capabilities for reading and writing
        *		of the context.
        * @param aOwner Owner of the context.
        * @return Possible return values:
        * - KErrNone, no errors
        * - KErrAlreadyExists, defined context already exists.
        */
        virtual TInt DefineContext( const TDesC& aContextSource,
            const TDesC& aContextType,
            const TSecurityPolicy& aReadWritePolicy,
            MCFContextSource& aOwner ) = 0;
        
        /**
        * Defines a new context in Context Framework.
        * Every context needs to be defined before it can be published,
        * subscribed or requested.
        *
        * Security policy for reading and writing are different.
        * 
        * If some client subscribes for this context, the contex owner
        * will be notified through MCFContextSource interface.
        *
        * @since S60 5.0
        * @param aContextSource Source of the context.
        * @param aContextType Type of the context.
        * @param aReadWritePolicy Needed capabilities for reading and writing
        *		of the context.
        * @return Possible return values:
        * - KErrNone, no errors
        * - KErrAlreadyExists, defined context already exists.
        */
        virtual TInt DefineContext( const TDesC& aContextSource,
            const TDesC& aContextType,
            const TSecurityPolicy& aReadPolicy,
            const TSecurityPolicy& aWritePolicy,
            MCFContextSource& aOwner ) = 0;
        
        /**
        * Publishes a new value of a context into Context Framework.
        * Subscribed clients will be notified of the change.
        * Context must have been defined before it can be published.
        *
        * @since S60 5.0
        * @param aContext Context object to be added.
        * @return Possible return values:
        * - KErrNone, no errors
        * - KErrNotFound, context not defined
        * - KErrAccessDenied, insufficient capabilities for the context
        * - KErrArgument if context source, type and value not defined
        */
        virtual TInt PublishContext( CCFContextObject& aContext,
            RThread& aClientThread ) = 0;
            
        /**
        * Publishes a new value of a context into Context Framework.
        * Subscribed clients will be notified of the change.
        * Context can be associated with a specific data object.
        * If a subscriber has requested to receive also data objects
        * the data object is associated in the context indication.
        * Context must have been defined before it can be published.
        *
        * @since S60 5.0
        * @param aContext Context object to be added.
        * @param aData Data object associated the context.
        * @return Possible return values:
        * - KErrNone, no errors
        * - KErrNotFound, context not defined
        * - KErrAccessDenied, insufficient capabilities for the context
        * - KErrArgument if context source, type and value not defined
        * - KErrNotSupported if the context data UID is NULL
        */
        virtual TInt PublishContext( CCFContextObject& aContext,
            CCFContextDataObject& aData,
            RThread& aClientThread ) = 0;
            
        /**
        * Adds new context subscription in Context Framework.
        * Context Framework takes ownership from aSubscription.
        * Possible error codes:
        * - KErrNotFound: Published context not defined
        * - KErrAccessDenied: Insufficient capabilities for the context
        *
        * @since S60 5.0
        * @param aSubscription New subscription.
        * @param aListener Subscription listener.
        * @param aClientThread Client thread where function is called.
        * @return KErrNone if no errors
        */
        virtual TInt SubscribeContext(
            CCFContextSubscription* aSubscription,
            MCFContextSubscriptionListener* aListener,
            RThread& aClientThread ) = 0;
        
        /**
        * Remove a context subscription from Context Framework.
        *
        * @since S60 5.0
        * @param aSubscription New subscription.
        * @param aListener Subscription listener.
        * @return None
        */
        virtual void UnsubscribeContext(
            CCFContextSubscription& aSubscription,
            MCFContextSubscriptionListener& aListener ) = 0;
        
        /**
        * Remove all subscriptions for the particular listener.
		*
		* @since S60 5.0
        * @param aObserver Listener whose subscriptions will be removed.
        * @return None
        */
        virtual void UnsubscribeContexts(
            MCFContextSubscriptionListener& aObserver ) = 0;
        
        /**
        * Requests latest context.
        * - KErrNotFound: Requested context not defined
        * - KErrAccessDenied: Insufficient capabilities for the context
        *
        * @since S60 5.0
        * @param aResultBuffer Context objects.
        * @param aContextQuery Context query.
        * @param aClientThread Client thread where function is called.
        * @return None
        */
        virtual TInt RequestContext(
            RContextObjectArray& aResultBuffer,
            CCFContextQuery& aContextQuery,
            RThread& aClientThread ) = 0;
            
		/**
		* Gets the security policy of particular action.
		*
		* @param aContextSource Context Source.
		* @param aContextType Context Type.
		* @param aPolicy security policy which will be set by this method.
		* @return KErrNone if context definition found, KErrNotFound if not.
		*/
		virtual TInt GetSecurityPolicy( const TDesC& aContextSource,
            const TDesC& aContextType,
            TSecurityPolicy& aPolicy ) = 0;
			        
    protected:
    
        // Deny destruction through this class
        virtual ~MCFContextInterface(){};
    };

#endif

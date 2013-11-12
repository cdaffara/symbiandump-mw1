/*
* Copyright (c) 2002-2008 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  CCFContextManager class implementation.
*
*/


#include <cfcontextdataobject.h>

#include "CFContextManager.h"
#include "CFContextQueryImpl.h"
#include "CFContextObjectImpl.h"
#include "cfcontextsubscriptionlistener.h"
#include "CFContextSubscriptionImpl.h"
#include "CFCacheElement.h"
#include "cfutils.h"
#include "cftrace.h"
#include "CFContextDataProxy.h"
#include "cfcommon.h"

// Public Constructors and destructors
CCFContextManager* CCFContextManager::NewL(
	MCFSecurityChecker& aSecurityChecker )
    {
    FUNC_LOG;

    CCFContextManager* self = CCFContextManager::NewLC( aSecurityChecker );
    CleanupStack::Pop(self);

    return self;
    }

CCFContextManager* CCFContextManager::NewLC(
	MCFSecurityChecker& aSecurityChecker )
    {
    FUNC_LOG;

    CCFContextManager* self
    	= new (ELeave) CCFContextManager( aSecurityChecker );
    CleanupStack::PushL(self);
    self->ConstructL();

    return self;
    }

CCFContextManager::~CCFContextManager()
    {
    FUNC_LOG;

    iCleanupCacheElements.Close();
    iContextCache.ResetAndDestroy();
    iContextCache.Close();
    delete iPreviousContext;
    delete iCacheOrder;
    delete iCacheSearchKey;

    iSubscriptions.ResetAndDestroy();
    }

CCFContextManager::CCFContextManager( MCFSecurityChecker& aSecurityChecker )
    {
    FUNC_LOG;

    iSecurityChecker = &aSecurityChecker;
    }

void CCFContextManager::ConstructL()
    {
    FUNC_LOG;

    iCacheOrder = new (ELeave) TLinearOrder<CCFCacheElement>
        (CCFCacheElement::CompareByTypeAndSource);

    iCacheSearchKey = CCFCacheElement::NewL();

    iPreviousContext = CCFContextObject::NewL();
    }

// METHODS

//-----------------------------------------------------------------------------
// CCFContextManager::PublishContextL
//-----------------------------------------------------------------------------
//
void CCFContextManager::PublishContextL( CCFContextObject &aContext,
    RThread& aClientThread )
    {
    FUNC_LOG;

    ValidatePublishContextL( aContext, aClientThread );
    DoPublishContextL( aContext );
    }

//-----------------------------------------------------------------------------
// CCFContextManager::PublishContextL
//-----------------------------------------------------------------------------
//
void CCFContextManager::PublishContextL( CCFContextObject &aContext,
    RThread& aClientThread,
    CCFContextDataObject& aData )
    {
    FUNC_LOG;

    ValidatePublishContextL( aContext, aClientThread );
    DoPublishContextL( aContext, &aData );
    }

//-----------------------------------------------------------------------------
// CCFContextManager::RequestContextL
//-----------------------------------------------------------------------------
//
void CCFContextManager::RequestContextL( RContextObjectArray& aResultBuffer,
    CCFContextQuery& aContextQuery,
    RThread& aClientThread )
    {
    FUNC_LOG;

    // Accessing the context cache for the latest context values is enough
    DoRequestContextL( aResultBuffer, aContextQuery, &aClientThread );
    }


//-----------------------------------------------------------------------------
// CCFContextManager::RequestContextL
//-----------------------------------------------------------------------------
//
CCFContextObject* CCFContextManager::RequestContextL( const TDesC& aContextType,
    const TDesC& aContextSource,
    RThread& aClientThread )
    {
    FUNC_LOG;

    // check the cache
    iCacheSearchKey->Context().SetTypeL( aContextType );
    iCacheSearchKey->Context().SetSourceL( aContextSource );
    TInt index = KErrNone;
    if( iContextCache.FindInOrder( iCacheSearchKey, index,
        *iCacheOrder ) == KErrNone )
        {
        // index points to the matching object
        if( index >= 0 && index < iContextCache.Count() )
            {
            User::LeaveIfError( DoReadSecurityCheck(
                *iContextCache[index],
                aClientThread ) );
            return &iContextCache[index]->Context();
            }
        else
            {
            return NULL;
            }
        }
    else    // == KErrNotFound
        {   // index points to "the place to be"
        return NULL;
        }
    }

//-----------------------------------------------------------------------------
// CCFContextManager::SubscriptionAddedL
//-----------------------------------------------------------------------------
//
void CCFContextManager::SubscriptionAddedL(
    CCFContextSubscriptionImpl& aSubscription,
    RThread& aClientThread )
    {
    FUNC_LOG;

    TInt count = iContextCache.Count();
    CCFCacheElement* element = NULL;
    for( TInt i = 0; i < count; i++ )
        {
        element = iContextCache[i];
        if( aSubscription.Matches( element->Context().Type(),
            element->Context().Source() ) )
            {
            TInt err = DoReadSecurityCheck( *element, aClientThread );
            if (err == KErrNone)
            	{
            	element->AddSubscriptionL( aSubscription );
            	}
            else
            	{
            	aSubscription.SubscriptionListener().HandleContextSubscriptionError( err,
            		element->Context().Source(),
            		element->Context().Type() );
            	}
            }
        }
    }

//-----------------------------------------------------------------------------
// CCFContextManager::SubscriptionRemoved
//-----------------------------------------------------------------------------
//
TBool CCFContextManager::SubscriptionRemoved(
    CCFContextSubscriptionImpl& aSubscription)
    {
    FUNC_LOG;

    TBool cleanupElement( EFalse );
    for( TInt i = 0; i < iContextCache.Count(); i++ )
        {
        TInt j = iContextCache[ i ]->Subscriptions().Count() - 1;
        for( ; j >= 0; j-- )
            {
            if( &aSubscription == iContextCache[ i ]->Subscriptions()[ j ] )
                {
                CCFCacheElement* cacheElement = iContextCache[ i ];
                if ( iPublishingForCacheElement
                    && ( iPublishingForCacheElement == cacheElement ) )
                    {
                    cleanupElement = ETrue;
                    cacheElement->NullifySubscription( j );
                    TInt err = iCleanupCacheElements.InsertInAddressOrder(
                            cacheElement );
                    if ( err == KErrAlreadyExists )
                        {
                        err = KErrNone;
                        }
                    ERROR( err, "CCFContextManager::SubscriptionRemoved - Adding cache element to cleanup array failed!" );
                    }
                else
                    {
                    cacheElement->RemoveSubscription( j );
                    }
                }
            }
        }

    return cleanupElement;
    }


//----------------------------------------------------------------------------
// CCFContextManager::DefineContextL
//----------------------------------------------------------------------------
//
void CCFContextManager::DefineContextL( const TDesC& aContextSource,
    const TDesC& aContextType,
    const TSecurityPolicy& aReadSecurityPolicy,
    const TSecurityPolicy& aWriteSecurityPolicy )
    {
    FUNC_LOG;

    RThread thread;
    CleanupClosePushL( thread );

    DefineContextL( aContextSource, aContextType, aReadSecurityPolicy,
    		aWriteSecurityPolicy, NULL, thread.SecureId() );

    CleanupStack::PopAndDestroy( );
    }


//----------------------------------------------------------------------------
// CCFContextManager::DefineContextL
//----------------------------------------------------------------------------
//
void CCFContextManager::DefineContextL( const TDesC& aContextSource,
    const TDesC& aContextType,
    const TSecurityPolicy& aReadSecurityPolicy,
    const TSecurityPolicy& aWriteSecurityPolicy,
    MCFContextSource* aContextPublisher,
    const TUid& aPublisherUid )
    {
    FUNC_LOG;

    // Prepare and find context
    iCacheSearchKey->Context().SetSourceL( aContextSource );
    iCacheSearchKey->Context().SetTypeL( aContextType );
    iCacheSearchKey->Context().SetValueL( KNullDesC );
    iCacheSearchKey->SetReadSecurityPolicy( aReadSecurityPolicy );
    iCacheSearchKey->SetWriteSecurityPolicy( aWriteSecurityPolicy );
    iCacheSearchKey->SetContextPublisher( aContextPublisher, aPublisherUid );

    TInt index = KErrNone;
    TInt searchError = iContextCache.FindInOrder( iCacheSearchKey,
        index,
        *iCacheOrder );

    if ( searchError != KErrNone )
        {
        // New context type
        User::LeaveIfError(
            iContextCache.Insert( iCacheSearchKey, index ) );
        CacheElementAddedL( *iCacheSearchKey );
        iCacheSearchKey = NULL;
        iCacheSearchKey = CCFCacheElement::NewL();

        // Signalling new context type
        iPreviousContext->SetSourceL( KNullDesC );
        iPreviousContext->SetTypeL( KNullDesC );

        INFO_2( "New context defined: [%S: %S]",
            &aContextSource,
            &aContextType );
        }
    else
        {
        TInt err = KErrNone;
        CCFCacheElement* cacheElement = iContextCache[index];
        if( cacheElement->PublisherUid() == aPublisherUid )
            {
            if( RedefineAllowed( *cacheElement, aContextPublisher, aPublisherUid ) )
                {
                cacheElement->SetContextPublisher( aContextPublisher,
                    aPublisherUid );
                INFO_3( "Context [%S: %S] redefined by client [%x]",
                    &aContextSource, &aContextType, aPublisherUid.iUid );
                }
            else
                {
                err = KErrAlreadyExists;
                }
            }
        else
            {
            err = KErrAlreadyExists;
            }

        ERROR_2( err, "Context already defined: [%S: %S]",
            &aContextSource,
            &aContextType );
        User::LeaveIfError( err );
        }
    }


//-----------------------------------------------------------------------------
// CCFContextManager::GetReadSecurityPolicyL
//-----------------------------------------------------------------------------
//
void CCFContextManager::GetReadSecurityPolicyL( const TDesC& aContextSource,
            const TDesC& aContextType,
			TSecurityPolicy& aPolicy )
	{
	FUNC_LOG;

    TInt count = iContextCache.Count();
    CCFCacheElement* cacheElement = NULL;
    for( TInt i = 0; i < count; i++ )
        {
        cacheElement = iContextCache[ i ];

        if( cacheElement->Context().Source().Compare( aContextSource ) == 0
        	&& cacheElement->Context().Type().Compare( aContextType ) == 0 )
        	{
			aPolicy = cacheElement->GetReadSecurityPolicy();
			return;
        	}
        }

	ERROR_2( KErrNotFound, "Security Policy for [%S: %S] not found!",
		&aContextSource, &aContextType );
	User::Leave( KErrNotFound );
	}

//-----------------------------------------------------------------------------
// CCFContextManager::GetWriteSecurityPolicyL
//-----------------------------------------------------------------------------
//
void CCFContextManager::GetWriteSecurityPolicyL( const TDesC& aContextSource,
            const TDesC& aContextType,
			TSecurityPolicy& aPolicy )
	{
	FUNC_LOG;

    TInt count = iContextCache.Count();
    CCFCacheElement* cacheElement = NULL;
    for( TInt i = 0; i < count; i++ )
        {
        cacheElement = iContextCache[ i ];

        if( cacheElement->Context().Source().Compare( aContextSource ) == 0
        	&& cacheElement->Context().Type().Compare( aContextType ) == 0 )
        	{
			aPolicy = cacheElement->GetWriteSecurityPolicy();
			return;
        	}
        }

	ERROR_2( KErrNotFound, "Security Policy for [%S: %S] not found!",
		&aContextSource, &aContextType );
	User::Leave( KErrNotFound );
	}


//-----------------------------------------------------------------------------
// CCFContextManager::DeregisterPublisher
//-----------------------------------------------------------------------------
//
void CCFContextManager::DeregisterPublisher( MCFContextSource& aPublisher )
	{
    FUNC_LOG;

    for( TInt i = 0; i < iContextCache.Count() ; i++ )
        {
        CCFCacheElement* cacheElement = iContextCache[ i ];
        if( cacheElement->ContextPublisher() == &aPublisher )
        	{
			cacheElement->SetContextPublisher( NULL );
        	}
        }
	}


//-----------------------------------------------------------------------------
// CCFContextManager::GetLatestFromCacheL
//-----------------------------------------------------------------------------
//
void CCFContextManager::GetLatestFromCacheL(
    RContextObjectArray& aLatestContexts,
    queryMatchFunction aMatchFunction,
    const CCFContextQueryImpl& aContextQuery,
    RThread* aClientThread )
    {
    FUNC_LOG;

    TInt count = iContextCache.Count();
    CCFCacheElement* cacheElement = NULL;
    for( TInt i = 0; i < count; ++i )
        {
        cacheElement = iContextCache[ i ];
        if( cacheElement->Context().Value().Length()
            && ( aContextQuery.*aMatchFunction )(
                cacheElement->Context().Type(),
                cacheElement->Context().Source() ) )
            {
            if( !aClientThread ||
                DoReadSecurityCheck( *cacheElement, *aClientThread ) == KErrNone )
                {
                aLatestContexts.AppendL( &cacheElement->Context() );
                }
            }
        }
    }

//-----------------------------------------------------------------------------
// CCFContextManager::LogContextCache
//-----------------------------------------------------------------------------
//
void CCFContextManager::LogContextCache()
    {
#ifdef DEBUG_LOG_CONTEXT_CACHE
    INFO( "CONTEXT CACHE start:" );
    TInt cacheIndex;
    for (cacheIndex = 0; cacheIndex < iContextCache.Count(); ++cacheIndex)
        {
        iContextCache[cacheIndex]->DebugPrintContents();
        }
    INFO( "CONTEXT CACHE start:" );
    INFO( "CONTEXT CACHE BY TYPE start:" );
    for (cacheIndex = 0; cacheIndex < iContextCacheByType.Count() ;
    ++cacheIndex)
        {
        iContextCacheByType[cacheIndex]->DebugPrintContents();
        }
    INFO( "CONTEXT CACHE BY TYPE end:" );
#endif // DEBUG_LOG_CONTEXT_CACHE
    }

//-----------------------------------------------------------------------------
// CCFContextManager::DoSecurityCheck
//-----------------------------------------------------------------------------
//
TInt CCFContextManager::DoReadSecurityCheck( CCFCacheElement& aContextCache,
    RThread& aClientThread )
    {
    FUNC_LOG;

    // Security check only needed if request is from other process than CF
    TInt err = KErrNone;

    TBool securityCheckOk =
        iSecurityChecker->CheckClientSecurity( aClientThread,
            aContextCache.GetReadSecurityPolicy() );

    if( !securityCheckOk )
        {
        // Insuffcient security, indicate client
        err = KErrAccessDenied;
        }

    return err;
    }

//-----------------------------------------------------------------------------
// CCFContextManager::DoSecurityCheck
//-----------------------------------------------------------------------------
//
TInt CCFContextManager::DoWriteSecurityCheck( CCFCacheElement& aContextCache,
    RThread& aClientThread )
    {
    FUNC_LOG;

    // Security check only needed if request is from other process than CF
    TInt err = KErrNone;

    TBool securityCheckOk =
        iSecurityChecker->CheckClientSecurity( aClientThread,
            aContextCache.GetWriteSecurityPolicy() );

    if( !securityCheckOk )
        {
        // Insuffcient security, indicate client
        err = KErrAccessDenied;
        }

    return err;
    }


//-----------------------------------------------------------------------------
// CCFContextManager::DoRequestContextL
//-----------------------------------------------------------------------------
//
void CCFContextManager::DoRequestContextL( RContextObjectArray& aResultBuffer,
    CCFContextQuery& aContextQuery,
    RThread* aClientThread )
    {
    FUNC_LOG;

    CCFContextQueryImpl& queryImpl = (CCFContextQueryImpl&) aContextQuery;
    GetLatestFromCacheL( aResultBuffer,
        &CCFContextQueryImpl::Matches,
        queryImpl,
        aClientThread );
    }

//-----------------------------------------------------------------------------
// CCFContextManager::SubscribeContextL
//-----------------------------------------------------------------------------
//
void CCFContextManager::SubscribeContextL(
    CCFContextSubscription* aSubscription,
    MCFContextSubscriptionListener* aListener,
    RThread& aClientThread )
    {
    FUNC_LOG;

	CCFContextSubscriptionImpl* subscriptionImpl =
        static_cast<CCFContextSubscriptionImpl*>( aSubscription );
    subscriptionImpl->SetSubscriptionListener( *aListener );

    // Append new subscription into array
    TInt err = iSubscriptions.Append( subscriptionImpl );
    if( err != KErrNone )
        {
        User::Leave( err );
        }

    // Notify data manager that new subscription has been added
    TRAP( err,
        SubscriptionAddedL( *subscriptionImpl, aClientThread ) );
    if( err )
        {
        iSubscriptions.Remove( iSubscriptions.Count() -1 );
        User::Leave( err );
        }
    }

//-----------------------------------------------------------------------------
// CCFContextManager::RemoveSubscription
//-----------------------------------------------------------------------------
//
TBool CCFContextManager::RemoveSubscription(
    CCFContextSubscription& aSubscription,
    MCFContextSubscriptionListener& aListener )
    {
    FUNC_LOG;

    TBool cleanupElement( EFalse );
   	CCFContextSubscriptionImpl* subscriptionImpl =
   	    static_cast<CCFContextSubscriptionImpl*>( &aSubscription );

    subscriptionImpl->SetSubscriptionListener( aListener );
    TBool deleteSubscriptionImpl( EFalse );

    TInt i = iSubscriptions.Count() - 1;
    while ( i >= 0 )
        {
        if ( i >= iSubscriptions.Count() )
            {
            // Context source has removed subscriptions via synchronous call
            // e.g. from managers call to source's NoSubscribers() function.
            i = iSubscriptions.Count() - 1;
            }

        if ( i < 0 )
            {
            INFO( "All subscriptions removed" );
            break;
            }

        if ( iSubscriptions[ i ]->IsSame( *subscriptionImpl ) )
            {
            CCFContextSubscriptionImpl* subscription = iSubscriptions[ i ];
            iSubscriptions.Remove( i );
            INFO_2( "CCFContextManager::RemoveSubscription - Subscription [%S: %S] removed",
                    &subscription->ContextSource(),
                    &subscription->ContextType() );
            if ( SubscriptionRemoved( *subscription ) )
                {
                cleanupElement = ETrue;
                }
            if ( subscription != subscriptionImpl )
                {
                delete subscription;
                }
            else
                {
                // Delay deleting when the same subscription instance is used
                // internally for both subscribe and unsubscribe.
                deleteSubscriptionImpl = ETrue;
                }
            }
        --i;
        }

    if ( deleteSubscriptionImpl )
        {
        delete subscriptionImpl;
        }

    return cleanupElement;
    }

//-----------------------------------------------------------------------------
// CCFContextManager::RemoveSubscription
//-----------------------------------------------------------------------------
//
TBool CCFContextManager::RemoveSubscriptions(
    MCFContextSubscriptionListener* aListener)
    {
    FUNC_LOG;

    TBool cleanupElement( EFalse );

    TInt i = iSubscriptions.Count() - 1;
    while ( i >= 0 )
        {
        if ( i >= iSubscriptions.Count() )
            {
            // Context source has removed subscriptions via synchronous call
            // e.g. from managers call to source's NoSubscribers() function.
            i = iSubscriptions.Count() - 1;
            }

        if ( i < 0 )
            {
            INFO( "All subscriptions removed" );
            break;
            }

        CCFContextSubscriptionImpl* subscription = iSubscriptions[ i ];
        if ( &( subscription->SubscriptionListener() ) == aListener )
            {
            iSubscriptions.Remove( i );
            if ( SubscriptionRemoved( *subscription ) )
                {
                cleanupElement = ETrue;
                }
            delete subscription;
            subscription = NULL;
            }
        --i;
        }

    return cleanupElement;
    }

//-----------------------------------------------------------------------------
// CCFContextManager::CleanupCacheElementSubscriptions
//-----------------------------------------------------------------------------
//
void CCFContextManager::CleanupCacheElementSubscriptions()
    {
    FUNC_LOG;

    for ( TInt i = 0; i < iCleanupCacheElements.Count(); ++i )
        {
        CCFCacheElement* element = iCleanupCacheElements[ i ];
        element->RemoveNullSubscriptions();
        }
    iCleanupCacheElements.Reset();
    }

//-----------------------------------------------------------------------------
// CCFContextManager::CacheElementAddedL
//-----------------------------------------------------------------------------
//
void CCFContextManager::CacheElementAddedL(
    CCFCacheElement& aCacheElement )
    {
    FUNC_LOG;

    TInt count = iSubscriptions.Count();
    CCFContextSubscriptionImpl* subscription = NULL;
    for( TInt i = 0; i < count; i++ )
        {
        subscription = iSubscriptions[i];
        if( subscription->Matches( aCacheElement.Context().Type(),
            aCacheElement.Context().Source() ) )
            {
            // Check security
            RThread thread;
            TInt err =
                subscription->SubscriptionListener().Client( thread );
            if( err == KErrNone )
                {
                err = DoReadSecurityCheck( aCacheElement, thread );
                if( err == KErrNone )
                    {
                    aCacheElement.AddSubscriptionL( *subscription );
                    }
                else
                    {
                    subscription->SubscriptionListener().HandleContextSubscriptionError(
                        err,
            			aCacheElement.Context().Source(),
            			aCacheElement.Context().Type() );
                    }
                }
            else
                {
                subscription->SubscriptionListener().HandleContextSubscriptionError(
                    err,
            		aCacheElement.Context().Source(),
            		aCacheElement.Context().Type() );
                }
            thread.Close();
            }
        }
    }

//-----------------------------------------------------------------------------
// CCFContextManager::ValidatePublishContextL
//-----------------------------------------------------------------------------
//
void CCFContextManager::ValidatePublishContextL( CCFContextObject& aContext,
    RThread& aClientThread )
    {
    FUNC_LOG;

    // Define context
    iCacheSearchKey->Context().CopyL( aContext );
    CCFCacheElement* currentCacheElement = iCacheSearchKey;
    TInt index = KErrNone;
    TInt searchError = iContextCache.FindInOrder( iCacheSearchKey,
        index,
        *iCacheOrder);

    // Both source and type must match.
    // Otherwise context has not been defined and will lead to error.
    if( searchError == KErrNone )
        {
        if( index >= 0 && index < iContextCache.Count() )
            {
            // index and indexByType points to the matching cache elements
            currentCacheElement = iContextCache[index];
            }
        else
            {
            // index not found -> leave
            ERROR_GEN( "Context cache element not found from cache list" );
            User::Leave( KErrNotFound );
            }

        // Perform security check
        TInt err = DoWriteSecurityCheck( *currentCacheElement, aClientThread );
        ERROR_2( err, "Security check failed: [%S: %S]",
             &aContext.Source(),
             &aContext.Type() );
        User::LeaveIfError( err );
        }
    else
        {
        // Indicate client that specified context cannot be found
        ERROR_2( KErrNotFound, "Published context not found: [%S: %S]",
            &aContext.Source(),
            &aContext.Type() );
        User::Leave( KErrNotFound );
        }
    }

//-----------------------------------------------------------------------------
// CCFContextManager::DoPublishContextL
//-----------------------------------------------------------------------------
//
void CCFContextManager::DoPublishContextL( CCFContextObject& aContext,
    CCFContextDataObject* aData )
    {
    FUNC_LOG;

    // Define context
    aContext.SetTimestampToHomeTime();
    iCacheSearchKey->Context().CopyL( aContext );
    CCFCacheElement* currentCacheElement = iCacheSearchKey;
    TBool valueChanged = EFalse;
    TInt index = KErrNone;
    TInt searchError = iContextCache.FindInOrder( iCacheSearchKey,
        index,
        *iCacheOrder);

    // Both source and type must match.
    // Otherwise context has not been defined and will lead to error.
    if( searchError == KErrNone )
        {
        if( index >= 0 && index < iContextCache.Count() )
            {
            // index and indexByType points to the matching cache elements
            currentCacheElement = iContextCache[ index ];
            }
        else
            {
            // index not found -> leave
            ERROR( KErrNotFound, "Context cache element not found from cache list" );
            User::Leave( KErrNotFound );
            }

        // store previous values
        iPreviousContext->CopyL( currentCacheElement->Context() );

        // Check if we have to update cache and indicate subscribers
        if( currentCacheElement->Context().Value() != aContext.Value() )
            {
            // Update context
            currentCacheElement->Context().CopyL( aContext );
            valueChanged = ETrue;
            }

        LogContextCache();

        // Let subscriptions do the indication if needed
        iPublishingForCacheElement = currentCacheElement;
        const RPointerArray< CCFContextSubscriptionImpl >& subscriptions =
            currentCacheElement->Subscriptions();
        TInt count = subscriptions.Count();
        CCFContextSubscriptionImpl* subscription = NULL;
        for( TInt i = 0 ; i < count; i++ )
            {
            subscription = subscriptions[ i ];
            if( subscription )
                {
                if( aData && subscription->DataObjectEnabled() )
                    {
                    subscription->EvaluateContextChangeL( aContext,
                        *aData,
                        valueChanged );
                    }
                else
                    {
                    subscription->EvaluateContextChangeL( aContext,
                        *iPreviousContext,
                        valueChanged );
                    }
                }
            }
        iPublishingForCacheElement = NULL;

        INFO_3( "Published context: [%S: %S: %S]",
            &aContext.Source(),
            &aContext.Type(),
            &aContext.Value() );
        if( aData )
            {
            INFO_1( "Published data object: [0x%x]", aData->Uid() );
            }
        }
    else
        {
        // Indicate client that specified context cannot be found
        ERROR_2( KErrNotFound, "Published context not found: [%S: %S]",
            &aContext.Source(),
            &aContext.Type() );
        User::Leave( KErrNotFound );
        }
    }

//------------------------------------------------------------------------------
// CCFContextManager::RedefineAllowed
//------------------------------------------------------------------------------
//
TBool CCFContextManager::RedefineAllowed( CCFCacheElement& aElement,
    const MCFContextSource* aSource,
    const TUid& aUid ) const
    {
    FUNC_LOG;

    TBool allowed = EFalse;

    // Redefine is allowed:
    // - Same client session from the same client process
    if( aElement.ContextPublisher() == aSource &&
        aElement.PublisherUid() == aUid )
        {
        allowed = ETrue;
        }

    // - The owner is NULL and the client process is the same
    else if( !aElement.ContextPublisher() &&
        aElement.PublisherUid() == aUid )
        {
        allowed = ETrue;
        }

    // - Owner is NULL and the redefine is done by context source plug-in
    else if( !aElement.ContextPublisher() &&
        KCFServerSid == aUid )
        {
        allowed = ETrue;
        }

    return allowed;
    }

// End of file


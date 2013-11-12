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
* Description:  CCFCacheElement class implementation.
*
*/


#include "CFCacheElement.h"
#include "CFContextObjectImpl.h"
#include "cftrace.h"
#include "cfcontextsourceinterface.h"

// CONSTANTS

#ifdef _DEBUG
// Panic category
_LIT( KPanicCat, "CONTEXTCACHE" );

// Panic codes
enum TPanicReason
    {
    EInvalidSubscriptionIndex,
    };

// Local panic function
LOCAL_C void Panic( TInt aCode )
    {
    User::Panic( KPanicCat, aCode );
    }
#endif

// MEMBER FUNCTIONS

CCFCacheElement* CCFCacheElement::NewL()
    {
    FUNC_LOG;

    CCFCacheElement* self  = CCFCacheElement::NewLC();
    CleanupStack::Pop( self );
    
    return self;
    }

CCFCacheElement* CCFCacheElement::NewLC()
    {
    FUNC_LOG;

    CCFCacheElement* self = new( ELeave ) CCFCacheElement;
    CleanupStack::PushL( self );
    self->ConstructL();
    
    return self;
    }

CCFCacheElement::~CCFCacheElement()
    {
    FUNC_LOG;
    
    delete iContext;
    iSubscriptions.Close();
    }

CCFCacheElement::CCFCacheElement()
    {
    FUNC_LOG;
    }

void CCFCacheElement::ConstructL()
    {
    FUNC_LOG;

    iContext = CCFContextObject::NewL();
    }

// METHODS

//-----------------------------------------------------------------------------
// CCFCacheElement::Context
//-----------------------------------------------------------------------------
// 
CCFContextObject& CCFCacheElement::Context() const
    {
    FUNC_LOG;
    
    return *iContext;
    }

//-----------------------------------------------------------------------------
// CCFCacheElement::AddSubscriptionL
//-----------------------------------------------------------------------------
// 
void CCFCacheElement::AddSubscriptionL(
    CCFContextSubscriptionImpl& aSubscription )
    {
    FUNC_LOG;
    
    iSubscriptions.AppendL( &aSubscription );
    
    // notify context source if this was the first subscription
    if (iSubscriptions.Count() == 1 && iPublisher )
    	{
    	iPublisher->Subscribers( iContext->Source(), iContext->Type() );
    	}
    }

//-----------------------------------------------------------------------------
// CCFCacheElement::RemoveSubscription
//-----------------------------------------------------------------------------
// 
void CCFCacheElement::RemoveSubscription( TInt aIndex )
    {
    FUNC_LOG;
    
    __ASSERT_DEBUG( aIndex >= 0 && aIndex < iSubscriptions.Count(),
        Panic( EInvalidSubscriptionIndex ) );
    
    iSubscriptions.Remove( aIndex );

    // notify context source if this was the first subscription
    if (iSubscriptions.Count() == 0 && iPublisher )
    	{
    	iPublisher->NoSubscribers( iContext->Source(), iContext->Type() );
    	}
    }

//-----------------------------------------------------------------------------
// CCFCacheElement::NullifySubscription
//-----------------------------------------------------------------------------
// 
void CCFCacheElement::NullifySubscription( TInt aIndex )
    {
    FUNC_LOG;

    __ASSERT_DEBUG( aIndex >= 0 && aIndex < iSubscriptions.Count(),
        Panic( EInvalidSubscriptionIndex ) );

    iSubscriptions[ aIndex ] = NULL;
    }

//-----------------------------------------------------------------------------
// CCFCacheElement::RemoveNullSubscriptions
//-----------------------------------------------------------------------------
// 
void CCFCacheElement::RemoveNullSubscriptions()
    {
    FUNC_LOG;

    TInt count = iSubscriptions.Count();
    for ( TInt i = count - 1; i >= 0; --i )
        {
        CCFContextSubscriptionImpl* subscription = iSubscriptions[ i ];
        if ( !subscription )
            {
            iSubscriptions.Remove( i );
            }
        }

    // notify context source if no more subscriptions exist
    if ( count && iSubscriptions.Count() == 0 && iPublisher )
        {
        iPublisher->NoSubscribers( iContext->Source(), iContext->Type() );
        }
    }

//-----------------------------------------------------------------------------
// CCFCacheElement::Subscriptions
//-----------------------------------------------------------------------------
// 
const RPointerArray<CCFContextSubscriptionImpl>& CCFCacheElement::Subscriptions() const
    {
    FUNC_LOG;
    
    return iSubscriptions;
    }

//-----------------------------------------------------------------------------
// CCFCacheElement::SetReadSecurityPolicy
//-----------------------------------------------------------------------------
// 
void CCFCacheElement::SetReadSecurityPolicy(
    const TSecurityPolicy& aSecurityPolicy )
    {
    FUNC_LOG;
    
    iReadSecurityPolicy = aSecurityPolicy;
    }

//-----------------------------------------------------------------------------
// CCFCacheElement::GetReadSecurityPolicy
//-----------------------------------------------------------------------------
// 
const TSecurityPolicy& CCFCacheElement::GetReadSecurityPolicy() const
    {
    FUNC_LOG;
    
    return iReadSecurityPolicy;
    }

//-----------------------------------------------------------------------------
// CCFCacheElement::SetWriteSecurityPolicy
//-----------------------------------------------------------------------------
// 
void CCFCacheElement::SetWriteSecurityPolicy(
    const TSecurityPolicy& aSecurityPolicy )
    {
    FUNC_LOG;
    
    iWriteSecurityPolicy = aSecurityPolicy;
    }

//-----------------------------------------------------------------------------
// CCFCacheElement::GetWriteSecurityPolicy
//-----------------------------------------------------------------------------
// 
const TSecurityPolicy& CCFCacheElement::GetWriteSecurityPolicy() const
    {
    FUNC_LOG;
    
    return iWriteSecurityPolicy;
    }


//-----------------------------------------------------------------------------
// CCFCacheElement::SetContextPublisher
//-----------------------------------------------------------------------------
// 
void CCFCacheElement::SetContextPublisher( MCFContextSource* aPublisher )
    {
    FUNC_LOG;

    iPublisher = aPublisher;
    }

//-----------------------------------------------------------------------------
// CCFCacheElement::SetContextPublisher
//-----------------------------------------------------------------------------
// 
void CCFCacheElement::SetContextPublisher(
	    MCFContextSource* aPublisher, const TUid& aPublisherUid )
    {
    FUNC_LOG;
    
    iPublisher = aPublisher;
    iPublisherUid = aPublisherUid;
    }

//-----------------------------------------------------------------------------
// CCFCacheElement::ContextPublisher
//-----------------------------------------------------------------------------
// 
const MCFContextSource* CCFCacheElement::ContextPublisher() const
    {
    FUNC_LOG;
    
    return iPublisher;
    }

//-----------------------------------------------------------------------------
// CCFCacheElement::PublisherUid
//-----------------------------------------------------------------------------
// 
const TUid& CCFCacheElement::PublisherUid() const
	{
	FUNC_LOG;
	
	return iPublisherUid;
	}


//-----------------------------------------------------------------------------
// CCFCacheElement::CompareByTypeAndSource
//-----------------------------------------------------------------------------
// 
TInt CCFCacheElement::CompareByTypeAndSource(
    const CCFCacheElement& aFirst,
    const CCFCacheElement& aSecond)
    {
    FUNC_LOG;

    return CCFContextObjectImpl::CompareByTypeAndSource(
        *aFirst.iContext, *aSecond.iContext );
    }

//-----------------------------------------------------------------------------
// CCFCacheElement::CompareByType
//-----------------------------------------------------------------------------
// 
TInt CCFCacheElement::CompareByType(
    const CCFCacheElement& aFirst,
    const CCFCacheElement& aSecond)
    {
    FUNC_LOG;

    return CCFContextObjectImpl::CompareByType(
        *aFirst.iContext, *aSecond.iContext);
    }

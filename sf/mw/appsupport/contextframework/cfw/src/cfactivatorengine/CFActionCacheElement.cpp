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
* Description:  CCFActionCacheElement class implementation.
*
*/



#include "CFActionCacheElement.h"
#include "CFActionSubscriptionImpl.h"
#include "cftrace.h"
#include "cfcommon.h"

// CONSTANTS

#ifdef _DEBUG

// Panic category
_LIT( KPanicCat, "ActionCache" );

// Panic codes
enum TPanicCode
    {
    EInvalidSubscription
    };
    
LOCAL_C void Panic( TPanicCode aCode )
    {
    User::Panic( KPanicCat, aCode );
    }

#endif

// MEMBER FUNCTIONS

CCFActionCacheElement* CCFActionCacheElement::NewL(
    const TDesC& aActionId,
    const TSecurityPolicy& aSecurityPolicy,
    const TUid& aOwnerUid,
    MCFActionOwner* aOwner )
    {
    FUNC_LOG;
    
    CCFActionCacheElement* self = CCFActionCacheElement::NewLC( aActionId,
        aSecurityPolicy, aOwnerUid, aOwner );
    CleanupStack::Pop( self );
    return self;
    }
  
CCFActionCacheElement* CCFActionCacheElement::NewLC(
    const TDesC& aActionId,
    const TSecurityPolicy& aSecurityPolicy,
    const TUid& aOwnerUid,
    MCFActionOwner* aOwner )
    {
    FUNC_LOG;
    
    CCFActionCacheElement* self = new( ELeave ) CCFActionCacheElement(
        aSecurityPolicy, aOwnerUid, aOwner );
    CleanupStack::PushL( self );
    self->ConstructL( aActionId );

    return self;
    }
  
CCFActionCacheElement::CCFActionCacheElement(
    const TSecurityPolicy& aSecurityPolicy,
    const TUid& aOwnerUid,
    MCFActionOwner* aOwner ):
    iSecurityPolicy( aSecurityPolicy ),
    iOwnerUid( aOwnerUid ),
    iOwner( aOwner )
    {
    FUNC_LOG;
    }

void CCFActionCacheElement::ConstructL( const TDesC& aActionId )
    {
    FUNC_LOG;
    
    iActionId = aActionId.AllocL();
    iOwnerType = iOwner->ActionOwner();
    }

// Destructor
CCFActionCacheElement::~CCFActionCacheElement()
    {
    FUNC_LOG;

    delete iActionId;
    iSubscriptions.ResetAndDestroy();
    }

// METHODS

//-----------------------------------------------------------------------------
// CCFActionCacheElement::CompareByIdentifier
//-----------------------------------------------------------------------------
//
TBool CCFActionCacheElement::CompareByIdentifier( const TDesC& aActionId )
    {
    FUNC_LOG;
    
    TBool res = (*iActionId).CompareF( aActionId ) == KErrNone;

    return res;
    }

//-----------------------------------------------------------------------------
// CCFActionCacheElement::AddSubscriptionL
//-----------------------------------------------------------------------------
//
void CCFActionCacheElement::AddSubscriptionL(
    CCFActionSubscription* aSubscription )
    {
    FUNC_LOG;
    
    __ASSERT_DEBUG( aSubscription, Panic( EInvalidSubscription ) );

    CCFActionSubscriptionImpl* subscription =
        static_cast<CCFActionSubscriptionImpl*>( aSubscription );
    iSubscriptions.AppendL( subscription );
    }

//-----------------------------------------------------------------------------
// CCFActionCacheElement::Subscriptions
//-----------------------------------------------------------------------------
//
const RActionSubscriptionArray& CCFActionCacheElement::Subscriptions() const
    {
    FUNC_LOG;

    return iSubscriptions;
    }

//-----------------------------------------------------------------------------
// CCFActionCacheElement::SecurityPolicy
//-----------------------------------------------------------------------------
//
const TSecurityPolicy& CCFActionCacheElement::SecurityPolicy() const
    {
    FUNC_LOG;

    return iSecurityPolicy;
    }

//-----------------------------------------------------------------------------
// CCFActionCacheElement::RemoveSubscription
//-----------------------------------------------------------------------------
//
void CCFActionCacheElement::RemoveSubscription(
    CCFActionSubscription& aSubscription )
    {
    FUNC_LOG;

    CCFActionSubscriptionImpl& subscription =
        static_cast<CCFActionSubscriptionImpl&>( aSubscription );
    TInt count = iSubscriptions.Count();
    TPtrC identifier( KNullDesC );
    MCFActionSubscriptionListener* listener = NULL;
    for( TInt i = 0; i < count; i++ )
        {
        identifier.Set( iSubscriptions[i]->ActionIdentifier() );
        listener = &iSubscriptions[i]->Listener();
        if( subscription.ActionIdentifier() == identifier )
            {
            if( listener == &subscription.Listener() )
                {
                CCFActionSubscriptionImpl* subscription = iSubscriptions[i];
                iSubscriptions.Remove( i );
                delete subscription;
                }
            }
        }
    }

//------------------------------------------------------------------------------
// CCFActionCacheElement::SetOwner
//------------------------------------------------------------------------------
//
void CCFActionCacheElement::SetOwner( MCFActionOwner* aOwner )
    {
    FUNC_LOG;
    
    iOwner = aOwner;
    }

//------------------------------------------------------------------------------
// CCFActionCacheElement::Owner
//------------------------------------------------------------------------------
//
MCFActionOwner* CCFActionCacheElement::Owner() const
    {
    FUNC_LOG;
    
    return iOwner;
    }

//------------------------------------------------------------------------------
// CCFActionCacheElement::OwnerUid
//------------------------------------------------------------------------------
//
const TUid& CCFActionCacheElement::OwnerUid() const
    {
    FUNC_LOG;
    
    return iOwnerUid;
    }

//------------------------------------------------------------------------------
// CCFActionCacheElement::AcionId
//------------------------------------------------------------------------------
//
const TDesC& CCFActionCacheElement::ActionId() const
    {
    FUNC_LOG;
    
    if( iActionId )
        {
        return *iActionId;
        }
    else
        {
        return KNullDesC;
        }
    }

//------------------------------------------------------------------------------
// CCFActionCacheElement::OwnerType
//------------------------------------------------------------------------------
//
MCFActionOwner::TCFActionOwner CCFActionCacheElement::OwnerType() const
    {
    FUNC_LOG;
    
    return iOwnerType;
    }

// End of file

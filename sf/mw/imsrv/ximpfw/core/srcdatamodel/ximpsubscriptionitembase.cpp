/*
* Copyright (c) 2006 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Presence Service Connection subscription item base implementation.
*
*/


#include "ximpsubscriptionitembase.h"
#include "ximpitemparent.h"
#include "ximppanics.h"
#include "ximppsccontext.h"
#include "ximpdatasubscriptionstateimp.h"
#include "ximpobjecthelpers.h"

#include "ximptrace.h"

const TInt KXIMPContentAutoExpiryTime = 15; // Seconds


// ---------------------------------------------------------------------------
// CXIMPSubscriptionItemBase::OrderOfContexts()
// ---------------------------------------------------------------------------
//
TInt CXIMPSubscriptionItemBase::OrderOfContexts( const MXIMPPscContext& aFirst,
                                                 const MXIMPPscContext& aSecond )
    {
    if( &aFirst < &aSecond )
        {
        return -1;
        }
    else if( &aFirst == &aSecond )
        {
        return 0;
        }
    else
        {
        return 1;
        }
    }

// ---------------------------------------------------------------------------
// CXIMPSubscriptionItemBase::OrderOfContexts()
// ---------------------------------------------------------------------------
//
TInt CXIMPSubscriptionItemBase::OrderOfItems( const CXIMPSubscriptionItemBase& aFirst,
                                              const CXIMPSubscriptionItemBase& aSecond )
    {
    if( &aFirst < &aSecond )
        {
        return -1;
        }
    else if( &aFirst == &aSecond )
        {
        return 0;
        }
    else
        {
        return 1;
        }
    }


// ---------------------------------------------------------------------------
// CXIMPSubscriptionItemBase::CXIMPSubscriptionItemBase()
// ---------------------------------------------------------------------------
//
EXPORT_C CXIMPSubscriptionItemBase::CXIMPSubscriptionItemBase( MXIMPItemParentBase& aParent )
: iParent( aParent )
    {
    }

// ---------------------------------------------------------------------------
// CXIMPSubscriptionItemBase::BaseConstructL()
// ---------------------------------------------------------------------------
//
EXPORT_C void CXIMPSubscriptionItemBase::BaseConstructL()
    {
    CXIMPDataSubscriptionStateImp* datasubscription = NULL;
    SetDataSubscriptionStateL(*datasubscription);
    iSubscriptionState = CXIMPDataSubscriptionStateImp::NewL();
    }

// ---------------------------------------------------------------------------
// CXIMPSubscriptionItemBase::~CXIMPSubscriptionItemBase()
// ---------------------------------------------------------------------------
//
EXPORT_C CXIMPSubscriptionItemBase::~CXIMPSubscriptionItemBase()
    {
    __ASSERT_ALWAYS( !CountOfSubscribers(),
                     User::Panic( NXIMPPrivPanic::KCategory, NXIMPPrivPanic::EArrayCountNotValid ) );
    iContexts.Close();
    iPreContexts.Close();
    iItems.Close();
    iPreItems.Close();
    delete iSubscriptionState;
    }

// ---------------------------------------------------------------------------
// CXIMPSubscriptionItemBase::SubscriptionStatus()
// ---------------------------------------------------------------------------
//
EXPORT_C CXIMPSubscriptionItemBase::TSubscriptionStatus
           CXIMPSubscriptionItemBase::SubscriptionStatus( MXIMPPscContext* aContext )
    {
    // Calculate situation
    TInt count = iContexts.Count() + iItems.Count();
    TBool contextFound = IsContext( aContext );
    if( count == 0 )
        {
        return ENotSubscribedAtAll;
        }
    else if( contextFound )
        {
        if( count == 1 )
            {
            return ESubscribedForCtxOnly;
            }
        return ESubscribedForCtxAndOthers;
        }
    return ESubscribedForOtherCtxOnly;
    }

// ---------------------------------------------------------------------------
// CXIMPSubscriptionItemBase::AddSubscriberL()
// ---------------------------------------------------------------------------
//
EXPORT_C void CXIMPSubscriptionItemBase::AddSubscriberL( MXIMPPscContext* aContext )
    {
    TRACE_2( _L("CXIMPSubscriptionItemBase::AddSubscriberL( MXIMPPscContext* aContext ) : this = %d , aContext = %d"), this, aContext );

    TRACE_1( _L("CXIMPSubscriptionItemBase::AddSubscriberL( MXIMPPscContext* aContext ) : iPreContexts.Count 1 = %d"), iPreContexts.Count() );
    TRACE_1( _L("CXIMPSubscriptionItemBase::AddSubscriberL( MXIMPPscContext* aContext ) : iContexts.Count 1 = %d"), iContexts.Count() );        
    
    TLinearOrder< MXIMPPscContext > order( OrderOfContexts );
    if( IsContext( aContext ) )
        {
        User::Leave( KErrAlreadyExists );
        }
        
    TInt index = iPreContexts.FindInOrder( aContext, order );
    TRACE_1( _L("CXIMPSubscriptionItemBase::AddSubscriberL( MXIMPPscContext* aContext ) : iPreContexts.FindInOrder = %d"), index );
    
    if( index != KErrNotFound )
        {
        User::Leave( KErrAlreadyExists );
        }

    iPreContexts.InsertInOrderL( aContext, order );
    
    User::LeaveIfError( Open() );
    
    TRACE_1( _L("CXIMPSubscriptionItemBase::AddSubscriberL( MXIMPPscContext* aContext ) : iPreContexts.Count 2 = %d"), iPreContexts.Count() );
    TRACE_1( _L("CXIMPSubscriptionItemBase::AddSubscriberL( MXIMPPscContext* aContext ) : iContexts.Count 2 = %d"), iContexts.Count() );        

    ChangeStatusAfterAdd();
    }

// ---------------------------------------------------------------------------
// CXIMPSubscriptionItemBase::RemoveSubscriber()
// ---------------------------------------------------------------------------
//
EXPORT_C void CXIMPSubscriptionItemBase::RemoveSubscriber( MXIMPPscContext* aContext )
    {
    TRACE_1( _L("CXIMPSubscriptionItemBase::RemoveSubscriber( MXIMPPscContext* aContext ) : iPreContexts.Count 1 = %d"), iPreContexts.Count() );
    TRACE_1( _L("CXIMPSubscriptionItemBase::RemoveSubscriber( MXIMPPscContext* aContext ) : iContexts.Count 1 = %d"), iContexts.Count() );        

    TLinearOrder< MXIMPPscContext > order( OrderOfContexts );
    TInt index = iContexts.FindInOrder( aContext, order );
    TInt preIndex = iPreContexts.FindInOrder( aContext, order );

    TRACE_1( _L("CXIMPSubscriptionItemBase::RemoveSubscriber( MXIMPPscContext* aContext ) : iContexts.FindInOrder = %d"), index );
    TRACE_1( _L("CXIMPSubscriptionItemBase::RemoveSubscriber( MXIMPPscContext* aContext ) : iPreContexts.FindInOrder = %d"), preIndex );        

    if( index >= 0 )
        {
        TRACE_1( _L("CXIMPSubscriptionItemBase::RemoveSubscriber( MXIMPPscContext* aContext ) : iContexts.Remove( index ):  aContext = %d"), aContext );        
        
        iContexts.Remove( index );
        ChangeStatusAfterRemove();
        Close();
        }
    else if( preIndex >= 0 )
        {
        TRACE_1( _L("CXIMPSubscriptionItemBase::RemoveSubscriber( MXIMPPscContext* aContext ) : iPreContexts.Remove( preIndex ):  aContext = %d"), aContext );        
        
        iPreContexts.Remove( preIndex );
        ChangeStatusAfterRemove();
        Close();
        }
    else
        {
        TRACE( _L("CXIMPSubscriptionItemBase::RemoveSubscriber( MXIMPPscContext* aContext ) : iParent.UnregisterExpiringItem") );        
        
        iParent.UnregisterExpiringItem( this );
        }

    TRACE_1( _L("CXIMPSubscriptionItemBase::RemoveSubscriber( MXIMPPscContext* aContext ) : iPreContexts.Count 2 = %d"), iPreContexts.Count() );
    TRACE_1( _L("CXIMPSubscriptionItemBase::RemoveSubscriber( MXIMPPscContext* aContext ) : iContexts.Count 2 = %d"), iContexts.Count() );        

    }

// ---------------------------------------------------------------------------
// CXIMPSubscriptionItemBase::AddSubscriberL()
// ---------------------------------------------------------------------------
//
EXPORT_C void CXIMPSubscriptionItemBase::AddSubscriberL( CXIMPSubscriptionItemBase* aItem )
    {
    TRACE( _L("CXIMPSubscriptionItemBase::AddSubscriberL( CXIMPSubscriptionItemBase* aItem ) : begin") );        
    
    TLinearOrder< CXIMPSubscriptionItemBase > order( OrderOfItems );
    TInt index = iItems.FindInOrder( aItem, order );
    if( index != KErrNotFound )
        {
        return;
        }
    index = iPreItems.FindInOrder( aItem, order );
    if( index != KErrNotFound )
        {
        return;
        }

    iPreItems.InsertInOrderL( aItem, order );
    User::LeaveIfError( Open() );
    ChangeStatusAfterAdd();
    
    TRACE( _L("CXIMPSubscriptionItemBase::AddSubscriberL( CXIMPSubscriptionItemBase* aItem ) : end") );        
    }

// ---------------------------------------------------------------------------
// CXIMPSubscriptionItemBase::RemoveSubscriber()
// ---------------------------------------------------------------------------
//
EXPORT_C void CXIMPSubscriptionItemBase::RemoveSubscriber( CXIMPSubscriptionItemBase* aItem )
    {
    TRACE( _L("CXIMPSubscriptionItemBase::RemoveSubscriberL( CXIMPSubscriptionItemBase* aItem ) : begin") );        
    
    TLinearOrder< CXIMPSubscriptionItemBase > order( OrderOfItems );
    TInt index = iItems.FindInOrder( aItem, order );
    TInt preIndex = iPreItems.FindInOrder( aItem, order );
    if( index >= 0 )
        {
        iItems.Remove( index );
        ChangeStatusAfterRemove();
        Close();
        }
    else if( preIndex >= 0 )
        {
        iPreItems.Remove( index );
        ChangeStatusAfterRemove();
        Close();
        }
    else
        {
        iParent.UnregisterExpiringItem( this );
        }
    
    TRACE( _L("CXIMPSubscriptionItemBase::RemoveSubscriberL( CXIMPSubscriptionItemBase* aItem ) : end") );                
    }

// ---------------------------------------------------------------------------
// CXIMPSubscriptionItemBase::ForceClose()
// ---------------------------------------------------------------------------
//
EXPORT_C void CXIMPSubscriptionItemBase::ForceClose()
    {
    TInt count = iContexts.Count();
    for( TInt a = 0; a < count; ++a )
        {
        // RemoveSubscriber removes first one every time.
        RemoveSubscriber( iContexts[ 0 ] );
        }
    count = iItems.Count();
    for( TInt a = 0; a < count; ++a )
        {
        RemoveSubscriber( iItems[ 0 ] );
        }
    }

// ---------------------------------------------------------------------------
// CXIMPSubscriptionItemBase::CleanIfExpired()
// ---------------------------------------------------------------------------
//
EXPORT_C TBool CXIMPSubscriptionItemBase::CleanIfExpired()
    {
    TTime timeNow;
    timeNow.UniversalTime();
    TBool expired =
        ( iExpireTime + TTimeIntervalSeconds( KXIMPContentAutoExpiryTime ) ) < timeNow;
    if( expired )
        {
        CleanExpired();
        }
    return expired;
    }

// ---------------------------------------------------------------------------
// CXIMPSubscriptionItemBase::UpdateSubscriptionStateL()
// ---------------------------------------------------------------------------
//
EXPORT_C void CXIMPSubscriptionItemBase::UpdateSubscriptionStateL( TSubscriptionEvent aUpdateState )
    {
    switch( aUpdateState )
        {
        case ECompleteDataReceived:
            {
            if( CountOfSubscribers() == 0 )
                {
                UpdateExpirationL();
                }
            break;
            }
        default:
            {
            User::Panic( NXIMPPrivPanic::KCategory, NXIMPPrivPanic::ESubscriptionItemLogicError );
            break;
            }
        }
    }
    

// ---------------------------------------------------------------------------
// CXIMPSubscriptionItemBase::DataSubscriptionState()
// ---------------------------------------------------------------------------
//
EXPORT_C const MXIMPDataSubscriptionState& CXIMPSubscriptionItemBase::DataSubscriptionState()
    {
    return *iSubscriptionState;
    }

// ---------------------------------------------------------------------------
// CXIMPSubscriptionItemBase::ContextCount()
// ---------------------------------------------------------------------------
//
EXPORT_C TInt CXIMPSubscriptionItemBase::ContextCount() const
    {
//    return iContexts.Count();
    TInt count = iContexts.Count();
    TRACE_1( _L("CXIMPSubscriptionItemBase::ContextCount() : return = %d"), count );
    return count;
    }

// ---------------------------------------------------------------------------
// CXIMPSubscriptionItemBase::Context()
// ---------------------------------------------------------------------------
//
EXPORT_C MXIMPPscContext& CXIMPSubscriptionItemBase::Context( TInt aIndex )
    {
    return *iContexts[ aIndex ];
    }

// ---------------------------------------------------------------------------
// CXIMPSubscriptionItemBase::Context()
// ---------------------------------------------------------------------------
//
EXPORT_C TBool CXIMPSubscriptionItemBase::IsContext( MXIMPPscContext* aContext ) const
    {
    TLinearOrder< MXIMPPscContext > order( OrderOfContexts );
//    return iContexts.FindInOrder( aContext, order ) != KErrNotFound;
    TBool ret = iContexts.FindInOrder( aContext, order ) != KErrNotFound;
    TRACE_1( _L("CXIMPSubscriptionItemBase::IsContext( MXIMPPscContext* aContext ) : return = %d"), (TInt)ret );
    return ret;
    }

// ---------------------------------------------------------------------------
// CXIMPSubscriptionItemBase::SynthesiseSubscriptionEventToAllCtxsL()
// ---------------------------------------------------------------------------
//
EXPORT_C void CXIMPSubscriptionItemBase::SynthesiseSubscriptionEventToAllCtxsL()
    {
    TRACE_1( _L("CXIMPSubscriptionItemBase::SynthesiseSubscriptionEventToAllCtxsL() : this = %d " ), this );
    RPointerArray< MXIMPPscContext > contextArray;
    CleanupClosePushL( contextArray );
    TLinearOrder< MXIMPPscContext > order( OrderOfContexts );

    TInt contextCount = iContexts.Count();
    
    TRACE_1( _L("CXIMPSubscriptionItemBase::SynthesiseSubscriptionEventToAllCtxsL() contextCount = %d"), contextCount );
    TRACE_1( _L("CXIMPSubscriptionItemBase::SynthesiseSubscriptionEventToAllCtxsL() : iPreContexts.Count = %d"), iPreContexts.Count() );
    
    for( TInt a = 0; a < contextCount; ++a )
        {
        contextArray.InsertInOrderL( iContexts[ a ], order );
        }

    TInt itemCount = iItems.Count();
    TRACE_1( _L("CXIMPSubscriptionItemBase::SynthesiseSubscriptionEventToAllCtxsL() itemCount = %d"), itemCount );
    
    for( TInt a = 0; a < itemCount; ++a )
        {
        TInt contextCount = iItems[ a ]->ContextCount();
        for( TInt b = 0; b < contextCount; ++b )
            {
            TInt error = contextArray.InsertInOrder( &iItems[ a ]->Context( b ), order );
            if( error != KErrNotFound )
                {
                User::LeaveIfError( error );
                }
            }
        }

    TInt count = contextArray.Count();
    TRACE_1( _L("CXIMPSubscriptionItemBase::SynthesiseSubscriptionEventToAllCtxsL() contextArray.Count() = %d"), count );
    
    for( TInt a = 0; a < count; ++a )
        {
        TRACE_1( _L("CXIMPSubscriptionItemBase::SynthesiseSubscriptionEventToAllCtxsL() Sending event to = %d"), a );
        TInt error ( KErrNone );
        TRAP( error, SynthesiseSubscriptionEventToL( contextArray[ a ], ETrue ); );
        TRACE_1( _L("CXIMPSubscriptionItemBase::SynthesiseSubscriptionEventToAllCtxsL() : TRAPD( error, SynthesiseSubscriptionEventToL = %d"), error );
        }
    CleanupStack::PopAndDestroy(); // contextArray
    }

// ---------------------------------------------------------------------------
// CXIMPSubscriptionItemBase::ActivateSubscriberL()
// ---------------------------------------------------------------------------
//
EXPORT_C void CXIMPSubscriptionItemBase::ActivateSubscriberL( MXIMPPscContext* aContext )
    {
    TRACE_1( _L("CXIMPSubscriptionItemBase::ActivateSubscriberL( MXIMPPscContext* aContext ) : this = %d "),this );
    TLinearOrder< MXIMPPscContext > order( OrderOfContexts );
    TInt index = iPreContexts.FindInOrder( aContext, order );
    
    TRACE_1( _L("CXIMPSubscriptionItemBase::ActivateSubscriberL() : MXIMPPscContext* aContext = %d"), aContext );
    
    TRACE_1( _L("CXIMPSubscriptionItemBase::ActivateSubscriberL() : iPreContexts.Count 1 = %d"), iPreContexts.Count() );
    TRACE_1( _L("CXIMPSubscriptionItemBase::ActivateSubscriberL() : iContexts.Count 1 = %d"), iContexts.Count() );        
    TRACE_1( _L("CXIMPSubscriptionItemBase::ActivateSubscriberL() : iPreContexts.FindInOrder = %d"), index );
    if( index != KErrNotFound )
        {
        iContexts.InsertInOrderL( iPreContexts[ index ], order );
        iPreContexts.Remove( index );
        }
        
    TRACE_1( _L("CXIMPSubscriptionItemBase::ActivateSubscriberL() : iPreContexts.Count 2 = %d"), iPreContexts.Count() );
    TRACE_1( _L("CXIMPSubscriptionItemBase::ActivateSubscriberL() : iContexts.Count 2 = %d"), iContexts.Count() );        
    }

// ---------------------------------------------------------------------------
// CXIMPSubscriptionItemBase::ActivateSubscriberL()
// ---------------------------------------------------------------------------
//
EXPORT_C void CXIMPSubscriptionItemBase::ActivateSubscriberL( CXIMPSubscriptionItemBase* aItem )
    {
    TRACE( _L("CXIMPSubscriptionItemBase::ActivateSubscriberL( CXIMPSubscriptionItemBase* aItem )" ) );
    TLinearOrder< CXIMPSubscriptionItemBase > order( OrderOfItems );
    TInt index = iPreItems.FindInOrder( aItem, order );
    TRACE_1( _L("CXIMPSubscriptionItemBase::ActivateSubscriberL() : iPreItems.FindInOrder = %d"), index );
    
    if( index != KErrNotFound )
        {
        iItems.InsertInOrderL( iPreItems[ index ], order );
        iPreItems.Remove( index );
        }
    }

// ---------------------------------------------------------------------------
// CXIMPSubscriptionItemBase::Status()
// ---------------------------------------------------------------------------
//
EXPORT_C void CXIMPSubscriptionItemBase::SetDataSubscriptionStateL( CXIMPDataSubscriptionStateImp& aNewState )
    {
    // Some handle logic for terminating subscribers, data expiration etc. missing.
    delete iSubscriptionState;
    iSubscriptionState = &aNewState;
    }

// ---------------------------------------------------------------------------
// CXIMPSubscriptionItemBase::Status()
// ---------------------------------------------------------------------------
//
EXPORT_C CXIMPDataSubscriptionStateImp* CXIMPSubscriptionItemBase::StatusLC( MXIMPPscContext* aContext ) const
    {
    MXIMPDataSubscriptionState::TSubscriptionState subscriptionState = iSubscriptionState->SubscriptionState();
    MXIMPDataSubscriptionState::TDataState dataState = iSubscriptionState->DataState();
    CXIMPDataSubscriptionStateImp* state = 
            TXIMPObjectCloner< CXIMPDataSubscriptionStateImp >::CloneL( *iSubscriptionState );
    CleanupStack::PushL( state );
    
    state->SetSubscriptionState( MXIMPDataSubscriptionState::ESubscriptionInactive );
    state->SetDataState( MXIMPDataSubscriptionState::EDataUnavailable );
    TInt count = iItems.Count();
    for( TInt a = 0; a < count; ++a )
        {
        if( iItems[ a ]->IsContext( aContext ) )
            {
            state->SetSubscriptionState( subscriptionState );
            state->SetDataState( dataState );
            return state;
            }
        }
    count = iPreItems.Count();
    for( TInt a = 0; a < count; ++a )
        {
        if( iPreItems[ a ]->IsContext( aContext ) )
            {
            state->SetSubscriptionState( MXIMPDataSubscriptionState::ESubscriptionActive );
            }
        }

    TLinearOrder< MXIMPPscContext > order( OrderOfContexts );
    TInt index = iContexts.FindInOrder( aContext, order );
    if( index == KErrNotFound )
        {
        index = iPreContexts.FindInOrder( aContext, order );
        if( index != KErrNotFound ) // Not yet activated
            {
            state->SetSubscriptionState( MXIMPDataSubscriptionState::ESubscriptionActive );
            }
        }
    else
        {
        state->SetSubscriptionState( subscriptionState );
        state->SetDataState( dataState );
        }
    return state;
    }

// ---------------------------------------------------------------------------
// CXIMPSubscriptionItemBase::UpdateExpirationL()
// ---------------------------------------------------------------------------
//
EXPORT_C void CXIMPSubscriptionItemBase::UpdateExpirationL()
    {
    if( CountOfSubscribers() == 0 )
        {
        iParent.RegisterExpiringItemL( this );
        }
    iExpireTime.UniversalTime();
    }


// ---------------------------------------------------------------------------
// CXIMPSubscriptionItemBase::ChangeStatusAfterRemove()
// ---------------------------------------------------------------------------
//
EXPORT_C void CXIMPSubscriptionItemBase::ChangeStatusAfterRemove()
    {
    if( CountOfSubscribers() == 0 )
        {
        iSubscriptionState->SetSubscriptionState( MXIMPDataSubscriptionState::ESubscriptionInactive );
        }
    }


// ---------------------------------------------------------------------------
// CXIMPSubscriptionItemBase::ChangeStatusAfterAdd()
// ---------------------------------------------------------------------------
//
EXPORT_C void CXIMPSubscriptionItemBase::ChangeStatusAfterAdd()
    {
    iSubscriptionState->SetSubscriptionState( MXIMPDataSubscriptionState::ESubscriptionActive );
    }

// ---------------------------------------------------------------------------
// CXIMPSubscriptionItemBase::CountOfSubscribers()
// ---------------------------------------------------------------------------
//
EXPORT_C TInt CXIMPSubscriptionItemBase::CountOfSubscribers()
    {
    return iItems.Count() + iPreItems.Count() + iContexts.Count() + iPreContexts.Count();
    }
// End of file

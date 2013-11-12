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
* Description:  IM Service Connection subscription item base implementation.
*
*/


#include "ximpimsubscriptionitembase.h"
#include "ximpimitemparent.h"
#include "ximppanics.h"
//#include "ximppsccontext.h"
//#include "ximpdatasubscriptionstateimp.h"
//#include "ximpobjecthelpers.h"

//#include "ximptrace.h"

const TInt KXIMPContentAutoExpiryTime = 15; // Seconds


// ---------------------------------------------------------------------------
// CXIMPIMSubscriptionItemBase::OrderOfContexts()
// ---------------------------------------------------------------------------
//
TInt CXIMPIMSubscriptionItemBase::OrderOfContexts( const MXIMPPscContext& aFirst,
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
// CXIMPIMSubscriptionItemBase::OrderOfItems()
// ---------------------------------------------------------------------------
//
TInt CXIMPIMSubscriptionItemBase::OrderOfItems( const CXIMPIMSubscriptionItemBase& aFirst,
                                              const CXIMPIMSubscriptionItemBase& aSecond )
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
// CXIMPIMSubscriptionItemBase::CXIMPIMSubscriptionItemBase()
// ---------------------------------------------------------------------------
//
EXPORT_C CXIMPIMSubscriptionItemBase::CXIMPIMSubscriptionItemBase( MXIMPIMItemParent& aParent )
: iParent( aParent )
    {
    }

// ---------------------------------------------------------------------------
// CXIMPIMSubscriptionItemBase::BaseConstructL()
// ---------------------------------------------------------------------------
//
EXPORT_C void CXIMPIMSubscriptionItemBase::BaseConstructL()
    {
    iSubscriptionState = CXIMPDataSubscriptionStateImp::NewL();
    }

// ---------------------------------------------------------------------------
// CXIMPIMSubscriptionItemBase::~CXIMPIMSubscriptionItemBase()
// ---------------------------------------------------------------------------
//
EXPORT_C CXIMPIMSubscriptionItemBase::~CXIMPIMSubscriptionItemBase()
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
// CXIMPIMSubscriptionItemBase::SubscriptionStatus()
// ---------------------------------------------------------------------------
//
EXPORT_C CXIMPIMSubscriptionItemBase::TSubscriptionStatus
           CXIMPIMSubscriptionItemBase::SubscriptionStatus( MXIMPPscContext* aContext )
    {
    // Calculate situation
    TInt count = iContexts.Count() + iItems.Count();
    
    if( count == 0 )
        {
        return ENotSubscribedAtAll;
        }
    if( IsContext( aContext ) )
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
// CXIMPIMSubscriptionItemBase::AddSubscriberL()
// ---------------------------------------------------------------------------
//
EXPORT_C void CXIMPIMSubscriptionItemBase::AddSubscriberL( MXIMPPscContext* aContext )
    {
    TLinearOrder< MXIMPPscContext > order( OrderOfContexts );
    if( IsContext( aContext ) )
        {
        User::Leave( KErrAlreadyExists );
        }
    TInt index = iPreContexts.FindInOrder( aContext, order );
    if( index != KErrNotFound )
        {
        User::Leave( KErrAlreadyExists );
        }

    iPreContexts.InsertInOrderL( aContext, order );
    User::LeaveIfError( Open() );
    ChangeStatusAfterAdd();
    }

// ---------------------------------------------------------------------------
// CXIMPIMSubscriptionItemBase::RemoveSubscriber()
// ---------------------------------------------------------------------------
//
EXPORT_C void CXIMPIMSubscriptionItemBase::RemoveSubscriber( MXIMPPscContext* aContext )
    {
    TLinearOrder< MXIMPPscContext > order( OrderOfContexts );
    TInt index = iContexts.FindInOrder( aContext, order );
    TInt preIndex = iPreContexts.FindInOrder( aContext, order );
    if( index >= 0 )
        {
        iContexts.Remove( index );
        ChangeStatusAfterRemove();
        Close();
        }
    else if( preIndex >= 0 )
        {
        iPreContexts.Remove( preIndex );
        ChangeStatusAfterRemove();
        Close();
        }
    else
        {
        iParent.UnregisterExpiringItem( this );
        }
    }

// ---------------------------------------------------------------------------
// CXIMPIMSubscriptionItemBase::AddSubscriberL()
// ---------------------------------------------------------------------------
//
EXPORT_C void CXIMPIMSubscriptionItemBase::AddSubscriberL( CXIMPIMSubscriptionItemBase* aItem )
    {
    TLinearOrder< CXIMPIMSubscriptionItemBase > order( OrderOfItems );
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
    }

// ---------------------------------------------------------------------------
// CXIMPIMSubscriptionItemBase::RemoveSubscriber()
// ---------------------------------------------------------------------------
//
EXPORT_C void CXIMPIMSubscriptionItemBase::RemoveSubscriber( CXIMPIMSubscriptionItemBase* aItem )
    {
    TLinearOrder< CXIMPIMSubscriptionItemBase > order( OrderOfItems );
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
    }

// ---------------------------------------------------------------------------
// CXIMPIMSubscriptionItemBase::ForceClose()
// ---------------------------------------------------------------------------
//
EXPORT_C void CXIMPIMSubscriptionItemBase::ForceClose()
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
// CXIMPIMSubscriptionItemBase::CleanIfExpired()
// ---------------------------------------------------------------------------
//
EXPORT_C TBool CXIMPIMSubscriptionItemBase::CleanIfExpired()
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
// CXIMPIMSubscriptionItemBase::UpdateSubscriptionStateL()
// ---------------------------------------------------------------------------
//
EXPORT_C void CXIMPIMSubscriptionItemBase::UpdateSubscriptionStateL( TSubscriptionEvent aUpdateState )
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
// CXIMPIMSubscriptionItemBase::DataSubscriptionState()
// ---------------------------------------------------------------------------
//
EXPORT_C const MXIMPDataSubscriptionState& CXIMPIMSubscriptionItemBase::DataSubscriptionState()
    {
    return *iSubscriptionState;
    }

// ---------------------------------------------------------------------------
// CXIMPIMSubscriptionItemBase::ContextCount()
// ---------------------------------------------------------------------------
//
EXPORT_C TInt CXIMPIMSubscriptionItemBase::ContextCount() const
    {
    return iContexts.Count();
    }

// ---------------------------------------------------------------------------
// CXIMPIMSubscriptionItemBase::Context()
// ---------------------------------------------------------------------------
//
EXPORT_C MXIMPPscContext& CXIMPIMSubscriptionItemBase::Context( TInt aIndex )
    {
    return *iContexts[ aIndex ];
    }

// ---------------------------------------------------------------------------
// CXIMPIMSubscriptionItemBase::Context()
// ---------------------------------------------------------------------------
//
EXPORT_C TBool CXIMPIMSubscriptionItemBase::IsContext( MXIMPPscContext* aContext ) const
    {
    TLinearOrder< MXIMPPscContext > order( OrderOfContexts );
    return iContexts.FindInOrder( aContext, order ) != KErrNotFound;
    }

// ---------------------------------------------------------------------------
// CXIMPIMSubscriptionItemBase::SynthesiseSubscriptionEventToAllCtxsL()
// ---------------------------------------------------------------------------
//
EXPORT_C void CXIMPIMSubscriptionItemBase::SynthesiseSubscriptionEventToAllCtxsL()
    {
    //TRACE( _L("CXIMPIMSubscriptionItemBase::SynthesiseSubscriptionEventToAllCtxsL()" ) );
    RPointerArray< MXIMPPscContext > contextArray;
    CleanupClosePushL( contextArray );
    TLinearOrder< MXIMPPscContext > order( OrderOfContexts );

    TInt contextCount = iContexts.Count();
    for( TInt a = 0; a < contextCount; ++a )
        {
        contextArray.InsertInOrderL( iContexts[ a ], order );
        }

    TInt itemCount = iItems.Count();
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
    for( TInt a = 0; a < count; ++a )
        {
        SynthesiseSubscriptionEventToL( contextArray[ a ], ETrue );
        }
    CleanupStack::PopAndDestroy(); // contextArray
    }

// ---------------------------------------------------------------------------
// CXIMPIMSubscriptionItemBase::ActivateSubscriberL()
// ---------------------------------------------------------------------------
//
EXPORT_C void CXIMPIMSubscriptionItemBase::ActivateSubscriberL( MXIMPPscContext* aContext )
    {
    TLinearOrder< MXIMPPscContext > order( OrderOfContexts );
    TInt index = iPreContexts.FindInOrder( aContext, order );
    if( index != KErrNotFound && index < iPreContexts.Count())
        {
        iContexts.InsertInOrderL( iPreContexts[ index ], order );
        iPreContexts.Remove( index );
        }
    }

// ---------------------------------------------------------------------------
// CXIMPIMSubscriptionItemBase::ActivateSubscriberL()
// ---------------------------------------------------------------------------
//
EXPORT_C void CXIMPIMSubscriptionItemBase::ActivateSubscriberL( CXIMPIMSubscriptionItemBase* aItem )
    {
    TLinearOrder< CXIMPIMSubscriptionItemBase > order( OrderOfItems );
    TInt index = iPreItems.FindInOrder( aItem, order );
    if( index != KErrNotFound && index < iPreItems.Count())
        {
        iItems.InsertInOrderL( iPreItems[ index ], order );
        iPreItems.Remove( index );
        }
    }

// ---------------------------------------------------------------------------
// CXIMPIMSubscriptionItemBase::Status()
// ---------------------------------------------------------------------------
//
EXPORT_C void CXIMPIMSubscriptionItemBase::SetDataSubscriptionStateL( CXIMPDataSubscriptionStateImp& aNewState )
    {
    // Some handle logic for terminating subscribers, data expiration etc. missing.
    delete iSubscriptionState;
    iSubscriptionState = &aNewState;
    }

// ---------------------------------------------------------------------------
// CXIMPIMSubscriptionItemBase::Status()
// ---------------------------------------------------------------------------
//
EXPORT_C CXIMPDataSubscriptionStateImp* CXIMPIMSubscriptionItemBase::StatusLC( MXIMPPscContext* /*aContext*/ ) const
    {
    // not implemented as of now
    }

// ---------------------------------------------------------------------------
// CXIMPIMSubscriptionItemBase::UpdateExpirationL()
// ---------------------------------------------------------------------------
//
EXPORT_C void CXIMPIMSubscriptionItemBase::UpdateExpirationL()
    {
    if( CountOfSubscribers() == 0 )
        {
        iParent.RegisterExpiringItemL( this );
        }
    iExpireTime.UniversalTime();
    }


// ---------------------------------------------------------------------------
// CXIMPIMSubscriptionItemBase::ChangeStatusAfterRemove()
// ---------------------------------------------------------------------------
//
EXPORT_C void CXIMPIMSubscriptionItemBase::ChangeStatusAfterRemove()
    {
    // not implemented as of now
    }


// ---------------------------------------------------------------------------
// CXIMPIMSubscriptionItemBase::ChangeStatusAfterAdd()
// ---------------------------------------------------------------------------
//
EXPORT_C void CXIMPIMSubscriptionItemBase::ChangeStatusAfterAdd()
    {
	// not implemented as of now
    }

// ---------------------------------------------------------------------------
// CXIMPIMSubscriptionItemBase::CountOfSubscribers()
// ---------------------------------------------------------------------------
//
EXPORT_C TInt CXIMPIMSubscriptionItemBase::CountOfSubscribers()
    {
    return iItems.Count() + iPreItems.Count() + iContexts.Count() + iPreContexts.Count();
    }
// End of file

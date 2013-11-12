/*
* Copyright (c) 2002-2004 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Implementation of CProEngNotifyHandlerImpl.
*
*/



// INCLUDE FILES
#include    "CProEngNotifyHandlerImpl.h"
#include    "CProEngProfileActivationDelegate.h"
#include    "CProEngActiveProfileDelegate.h"
#include    "CProEngProfileNameArrayEventDelegate.h"
#include    "CProEngProfileEventDelegate.h"
#include    "ProfileEnginePrivateCRKeys.h"
#include    <MProEngProfileActivationObserver.h>
#include    <MProEngActiveProfileObserver.h>
#include    <MProEngProfileObserver.h>
#include    <MProEngProfileNameArrayObserver.h>
#include    <ProfileEngineConstants.h>
#include    <ProfileEnginePrivatePSKeys.h>


// ============================= LOCAL FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CompareDelegates
//
// Comparison function to check that RPointerArray does not already
// contain the given delegate for profile events
// -----------------------------------------------------------------------------

TBool CompareDelegates( const CProEngProfileEventDelegate& aDelegate1,
                        const CProEngProfileEventDelegate& aDelegate2 )
    {
    return ( aDelegate1.ProfileId() == aDelegate2.ProfileId() );
    }
 
// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CProEngNotifyHandlerImpl::CProEngNotifyHandlerImpl
// -----------------------------------------------------------------------------
//
CProEngNotifyHandlerImpl::CProEngNotifyHandlerImpl() : 
    iActiveIdEventDelegate( NULL ),
    iActiveProfileEventDelegate( NULL ),
    iNameArrayEventDelegate( NULL )
    {
    }

// -----------------------------------------------------------------------------
// CProEngNotifyHandlerImpl::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CProEngNotifyHandlerImpl* CProEngNotifyHandlerImpl::NewL()
    {
    return new ( ELeave ) CProEngNotifyHandlerImpl();
    }

// Destructor
CProEngNotifyHandlerImpl::~CProEngNotifyHandlerImpl()
    {
    CancelAll();
    //iProfileEventDelegates.ResetAndDestroy();
    }

// -----------------------------------------------------------------------------
// CProEngNotifyHandlerImpl::RequestProfileActivationNotifications
// -----------------------------------------------------------------------------
//
TInt CProEngNotifyHandlerImpl::RequestProfileActivationNotificationsL(
        MProEngProfileActivationObserver& aObserver )
    {
    if( iActiveIdEventDelegate )
        {
        return KErrAlreadyExists;
        }

    iActiveIdEventDelegate =
            CProEngProfileActivationDelegate::NewL( aObserver );

    // make the actual request to the Publish & Subscribe:
    TInt result( iActiveIdEventDelegate->RequestNotification() );
    if( result != KErrNone )
        {
        delete iActiveIdEventDelegate;
        iActiveIdEventDelegate = NULL;
        }

    return result;
    }

// -----------------------------------------------------------------------------
// CProEngNotifyHandlerImpl::RequestActiveProfileNotificationsL
// -----------------------------------------------------------------------------
//
TInt CProEngNotifyHandlerImpl::RequestActiveProfileNotificationsL(
        MProEngActiveProfileObserver& aObserver )
    {
    if( iActiveProfileEventDelegate )
        {
        return KErrAlreadyExists;
        }

    iActiveProfileEventDelegate = new ( ELeave ) CProEngActiveProfileDelegate(
            aObserver );

    // make the actual request to the Publish & Subscribe:
    TInt result( iActiveProfileEventDelegate->RequestNotification() );
    if( result != KErrNone )
        {
        delete iActiveProfileEventDelegate;
        iActiveProfileEventDelegate = NULL;
        }

    return result;
    }

// -----------------------------------------------------------------------------
// CProEngNotifyHandlerImpl::RequestProfileNotificationsL
// -----------------------------------------------------------------------------
//
TInt CProEngNotifyHandlerImpl::RequestProfileNotificationsL(
        MProEngProfileObserver& aObserver,
        TInt aProfileId )
    {
    if( ( aProfileId < 0 ) || // profile id cannot be negative
        // or greater than the last dynamic profile id
        ( aProfileId > KProfileBaseDynamicProfileId +
                       KProfilesMaxNumberOfDynamicProfiles ) )
        {
        return KErrNotFound;
        }

    CProEngProfileEventDelegate* delegate = CProEngProfileEventDelegate::NewL(
            aProfileId, aObserver );
            
    TIdentityRelation<CProEngProfileEventDelegate> relation( CompareDelegates );
    if( iProfileEventDelegates.Find( delegate, relation ) != KErrNotFound )
        { // an observer has already been registered for the given profile id
        delete delegate;
        return KErrAlreadyExists;
        }

    TInt result( iProfileEventDelegates.Append( delegate ) );
    if( result == KErrNone )
        {
        // make the actual request to the Central Repository:
        result = delegate->RequestNotification();
        }

    if( result != KErrNone )
        {
        iProfileEventDelegates.Remove( iProfileEventDelegates.Count() - 1 );
        delete delegate;
        }

    return result;
    }

// -----------------------------------------------------------------------------
// CProEngNotifyHandlerImpl::RequestProfileNameArrayNotificationsL
// -----------------------------------------------------------------------------
//
TInt CProEngNotifyHandlerImpl::RequestProfileNameArrayNotificationsL(
        MProEngProfileNameArrayObserver& aObserver )
    {
    if( iNameArrayEventDelegate )
        {
        return KErrAlreadyExists;
        }

    iNameArrayEventDelegate = CProEngProfileNameArrayEventDelegate::NewL(
            aObserver );
            
    // make the actual request to the Central Repository:
    TInt result( iNameArrayEventDelegate->RequestNotification() );
    if( result != KErrNone )
        {
        delete iNameArrayEventDelegate;
        iNameArrayEventDelegate = NULL;
        }

    return result;
    }

// -----------------------------------------------------------------------------
// CProEngNotifyHandlerImpl::CancelProfileActivationNotifications
// -----------------------------------------------------------------------------
//
void CProEngNotifyHandlerImpl::CancelProfileActivationNotifications()
    {
    if( iActiveIdEventDelegate )
        {
        iActiveIdEventDelegate->Cancel();
        delete iActiveIdEventDelegate;
        iActiveIdEventDelegate = NULL;
        }
    }

// -----------------------------------------------------------------------------
// CProEngNotifyHandlerImpl::CancelActiveProfileNotifications
// -----------------------------------------------------------------------------
//
void CProEngNotifyHandlerImpl::CancelActiveProfileNotifications()
    {
    if( iActiveProfileEventDelegate )
        {
        iActiveProfileEventDelegate->Cancel();
        delete iActiveProfileEventDelegate;
        iActiveProfileEventDelegate = NULL;
        }
    }

// -----------------------------------------------------------------------------
// CProEngNotifyHandlerImpl::CancelProfileNotifications
// -----------------------------------------------------------------------------
//
void CProEngNotifyHandlerImpl::CancelProfileNotifications( TInt aProfileId )
    {
    TInt count( iProfileEventDelegates.Count() );
    for( TInt i( 0 ); i<count; ++i )
        {
        if( iProfileEventDelegates[i]->ProfileId() == aProfileId )
            {
            CProEngProfileEventDelegate* delegate = iProfileEventDelegates[i];
            iProfileEventDelegates.Remove( i );
            delegate->Cancel();
            delete delegate;
            break;
            }
        }
    }

// -----------------------------------------------------------------------------
// CProEngNotifyHandlerImpl::CancelProfileNameArrayNotifications
// -----------------------------------------------------------------------------
//
void CProEngNotifyHandlerImpl::CancelProfileNameArrayNotifications()
    {
    if( iNameArrayEventDelegate )
        {
        iNameArrayEventDelegate->Cancel();
        delete iNameArrayEventDelegate;
        iNameArrayEventDelegate = NULL;
        }
    }

// -----------------------------------------------------------------------------
// CProEngNotifyHandlerImpl::CancelAll
// -----------------------------------------------------------------------------
//
void CProEngNotifyHandlerImpl::CancelAll()
    {
    CancelProfileActivationNotifications();
    CancelActiveProfileNotifications();
    CancelProfileNameArrayNotifications();
    TInt count = iProfileEventDelegates.Count();
    for( TInt i( 0 ); i<count; ++i )
        {
        iProfileEventDelegates[i]->Cancel();
        }
    iProfileEventDelegates.ResetAndDestroy();
    }

//  End of File


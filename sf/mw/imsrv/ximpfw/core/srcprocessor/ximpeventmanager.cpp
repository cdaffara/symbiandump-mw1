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
* Description:  XIMP Framework server event manager implementation
*
*/

#include "ximpeventmanager.h"
#include "ximpeventcapsule.h"
#include "ximpcontexteventqueue.h"
#include "ximppsccontextimp.h"


// ======== LOCAL FUNCTIONS ========

// ---------------------------------------------------------------------------
// OrderOfEvents()
// ---------------------------------------------------------------------------
//
TInt OrderOfEvents( const CXIMPContextEventQueue& aFirst, const CXIMPContextEventQueue& aSecond )
    {
    TUint32 firstSessionId = aFirst.SessionIdOfQueue();
    TUint32 secondSessionId = aSecond.SessionIdOfQueue();
    if( firstSessionId < secondSessionId )
        {
        return -1;
        }
    else if( firstSessionId == secondSessionId )
        {
        return 0;
        }
    else
        {
        return 1;
        }
    }

// ======== MEMBER FUNCTIONS ========

// ---------------------------------------------------------------------------
// CXIMPEventManager::CXIMPHostManager()
// ---------------------------------------------------------------------------
//
CXIMPEventManager::CXIMPEventManager()
    {
    }


// ---------------------------------------------------------------------------
// CXIMPEventManager::ConstructL()
// ---------------------------------------------------------------------------
//
void CXIMPEventManager::ConstructL()
    {
    }


// ---------------------------------------------------------------------------
// CXIMPEventManager::NewL()
// ---------------------------------------------------------------------------
//
EXPORT_C CXIMPEventManager* CXIMPEventManager::NewL()
    {
    CXIMPEventManager* self = new( ELeave ) CXIMPEventManager;
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );
    return self;
    }


// ---------------------------------------------------------------------------
// CXIMPEventManager::~CXIMPEventManager()
// ---------------------------------------------------------------------------
//
CXIMPEventManager::~CXIMPEventManager()
    {
    iEventQueues.Close();
    }


// ---------------------------------------------------------------------------
// CXIMPEventManager::RegisterSessionQueueL()
// ---------------------------------------------------------------------------
//
EXPORT_C void CXIMPEventManager::RegisterSessionQueueL( CXIMPContextEventQueue& aQueue )
    {
    TLinearOrder< CXIMPContextEventQueue > order( *OrderOfEvents );
    iEventQueues.InsertInOrderL( &aQueue, order );
    }

// ---------------------------------------------------------------------------
// CXIMPEventManager::UnregisterSessionQueue()
// ---------------------------------------------------------------------------
//
EXPORT_C void CXIMPEventManager::UnregisterSessionQueue( CXIMPContextEventQueue& aQueue )
    {
    TLinearOrder< CXIMPContextEventQueue > order( *OrderOfEvents );
    TInt index = iEventQueues.FindInOrder( &aQueue, order );
    if( index >= 0 )
        {
        iEventQueues.Remove( index );
        }
    }

// ---------------------------------------------------------------------------
// CXIMPEventManager::AddEventL()
// ---------------------------------------------------------------------------
//
EXPORT_C void CXIMPEventManager::AddEventL( CXIMPApiEventBase& aEvent, 
                                           CXIMPPscContext* aContext  )
    {
    if( aContext )
        {
        RPointerArray< CXIMPPscContext > array;
        CleanupClosePushL( array );
        array.AppendL( aContext );
        AddEventL( aEvent, array );
        CleanupStack::PopAndDestroy(); // array
        }
    }

// ---------------------------------------------------------------------------
// CXIMPEventManager::ActivateEventL()
// ---------------------------------------------------------------------------
//
EXPORT_C void CXIMPEventManager::ActivateEventL( CXIMPApiEventBase& aEvent, 
                                                CXIMPPscContext* aContext  )
    {
    if( aContext )
        {
        aContext->EventQueue().ActivatePreservedEventL( aEvent );
        }
    }

// ---------------------------------------------------------------------------
// CXIMPEventManager::ActivateRequestCompleteEvent()
// ---------------------------------------------------------------------------
//
EXPORT_C void CXIMPEventManager::ActivateRequestCompleteEvent( CXIMPRequestCompleteEventImp& aEvent, 
                                                CXIMPPscContext* aContext  )
    {
    if( aContext )
        {
        aContext->EventQueue().ActivateRequestCompleteEvent( aEvent );
        }
    }

// ---------------------------------------------------------------------------
// CXIMPEventManager::AddEventL()
// ---------------------------------------------------------------------------
//
EXPORT_C void CXIMPEventManager::AddEventL( CXIMPApiEventBase& aEvent, 
                                           RPointerArray< CXIMPPscContext >& aSessions )
    {
    CXIMPEventCapsule* capsulatedEvent = CXIMPEventCapsule::NewL( aEvent );
    CleanupClosePushL( *capsulatedEvent );
    TInt sessionCount = aSessions.Count();
    for( TInt a = 0; a < sessionCount; ++a )
        {
        aSessions[ a ]->EventQueue().AddEventL( *capsulatedEvent );
        }
    CleanupStack::PopAndDestroy( capsulatedEvent );
    }


// End of file

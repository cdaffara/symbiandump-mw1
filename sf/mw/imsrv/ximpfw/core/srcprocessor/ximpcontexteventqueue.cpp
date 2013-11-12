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
* Description: Implementation of context event queue
 *
*/

#include "ximpcontexteventqueue.h"
#include "ximpeventqueueobserver.h"

#include "ximpglobals.h"
#include "ximpeventmanager.h"
#include "ximpeventcapsule.h"
#include "ximpcontexteventfilter.h"
#include "ximppanics.h"
#include "ximpcontextstateeventimp.h"
#include "ximprequestcompleteeventimp.h"

#include <e32base.h>



// ============================ MEMBER FUNCTIONS =============================

// ---------------------------------------------------------------------------
// CXIMPContextEventQueue::NewL()
// ---------------------------------------------------------------------------
//
EXPORT_C CXIMPContextEventQueue* CXIMPContextEventQueue::NewL()
    {
    CXIMPContextEventQueue* self =
        new( ELeave ) CXIMPContextEventQueue( );
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );
    return self;
    }


// ---------------------------------------------------------------------------
// CXIMPContextEventQueue::~CXIMPContextEventQueue()
// ---------------------------------------------------------------------------
//
CXIMPContextEventQueue::~CXIMPContextEventQueue()
    {
    ClearQueuedEvents();
    delete iEventFilter;

    TInt eventCount =  iPreservedEvents.Count();
    for( TInt ix = 0; ix < eventCount ; ix++ )
        {
        iPreservedEvents[ ix ]->Close();
        }
    iPreservedEvents.Reset();
    eventCount =  iRequestCompleteEvents.Count();
    for( TInt ix = 0; ix < eventCount ; ix++ )
        {
        iRequestCompleteEvents[ ix ]->Close();
        }
    iRequestCompleteEvents.Reset();
    }


// ---------------------------------------------------------------------------
// CXIMPContextEventQueue::CXIMPContextEventQueue()
// ---------------------------------------------------------------------------
//
CXIMPContextEventQueue::CXIMPContextEventQueue()
    {
    }


// -----------------------------------------------------------------------------
// CXIMPContextEventQueue::ConstructL()
// -----------------------------------------------------------------------------
//
void CXIMPContextEventQueue::ConstructL()
    {
    iEventFilter = CXIMPContextEventFilter::NewL();
    CXIMPContextStateEventImp* tempEvent =
            CXIMPContextStateEventImp::NewLC();
            
    for( TInt a = 0; a < 6; ++a ) // Count of different states + complete
        {
        AddPreservedEventL( *tempEvent );
        }
        
    CleanupStack::PopAndDestroy(); // tempEvent
    StopConsuming();
    }


// -----------------------------------------------------------------------------
// CXIMPContextEventQueue::SetEventFilter()
// -----------------------------------------------------------------------------
//
EXPORT_C void CXIMPContextEventQueue::SetEventFilter(
    CXIMPContextEventFilter& aFilter )
    {
    delete iEventFilter;
    iEventFilter = &aFilter;

    //Refilter event queue so we don't deliver such events
    //that client isn't wanting
    TInt eventCount =  iPreservedEvents.Count();
    for( TInt ix = 0; ix < eventCount ; ix++ )
        {
        CXIMPEventCapsule* event = iPreservedEvents[ ix ];
        if( !iEventFilter->IsEventAccepted( event->EventType() ) &&
            event->Ready() )
            {
            event->SetReady( EFalse );
            }
        }

    eventCount =  iEvents.Count() - 1;
    for( TInt ix = eventCount; ix >= 0 ; ix-- )
        {
        CXIMPEventCapsule* event = iEvents[ ix ];
        if( !iEventFilter->IsEventAccepted( event->EventType() ) )
            {
            event->Close();
            iEvents.Remove( ix );
            }
        }
    }


// -----------------------------------------------------------------------------
// CXIMPContextEventQueue::StartConsuming()
// -----------------------------------------------------------------------------
//
EXPORT_C void CXIMPContextEventQueue::StartConsuming(
    MXIMPEventQueueObserver& aQueueObserver )
    {
    iObserver = &aQueueObserver;
    }


// -----------------------------------------------------------------------------
// CXIMPContextEventQueue::StopConsuming()
// -----------------------------------------------------------------------------
//
EXPORT_C void CXIMPContextEventQueue::StopConsuming()
    {
    iObserver = NULL;
    ClearQueuedEvents();
    }


// -----------------------------------------------------------------------------
// CXIMPContextEventQueue::HasElements()
// -----------------------------------------------------------------------------
//
EXPORT_C TBool CXIMPContextEventQueue::HasElements() const
    {
    TInt eventCount = iPreservedEvents.Count();
    TInt count = 0;
    for( TInt a = 0; a < eventCount; ++a )
        {
        if( iPreservedEvents[ a ]->Ready() )
            {
            ++count;
            }
        }

    eventCount = iRequestCompleteEvents.Count();
    for( TInt a = 0; a < eventCount; ++a )
        {
        if( iRequestCompleteEvents[ a ]->Ready() )
            {
            ++count;
            }
        }

    return ( iEvents.Count() + count > 0 );
    }


// -----------------------------------------------------------------------------
// CXIMPContextEventQueue::FindNextEvent()
// -----------------------------------------------------------------------------
//
TBool CXIMPContextEventQueue::FindNextEvent( RPointerArray< CXIMPEventCapsule >& aArray )
    {
    TInt eventIndex = KErrNotFound;
    TInt eventCount = aArray.Count();
    for( TInt a = 0; a < eventCount; ++a )
        {
        if( aArray[ a ]->Ready() )
            {
            if( iNextEventToGo )
                {
                if( *aArray[ a ] < *iNextEventToGo )
                    {
                    eventIndex = a;
                    iNextEventToGo = aArray[ a ];
                    }
                }
            else
                {
                eventIndex = a;
                iNextEventToGo = aArray[ a ];
                }            
            }
        }
    return eventIndex;
    }

// -----------------------------------------------------------------------------
// CXIMPContextEventQueue::SelectTopEventIfNeededL()
// -----------------------------------------------------------------------------
//
EXPORT_C void CXIMPContextEventQueue::SelectTopEventIfNeededL()
    {
    if( iNextEventToGo )
        {
        return;
        }

    __ASSERT_ALWAYS( HasElements(), User::Leave( KErrNotFound ) );

    TInt presEvent = FindNextEvent( iPreservedEvents );
    TInt reqEvent = FindNextEvent( iRequestCompleteEvents );
    TInt event = FindNextEvent( iEvents );

    if( event != KErrNotFound )
        {
        iEvents.Remove( event );
        }
    else if( reqEvent != KErrNotFound )
        {
        iRequestCompleteEvents.Remove( reqEvent );
        }
    else
        {
        if(iNextEventToGo)
         {
         User::LeaveIfError(iNextEventToGo->Open()); //Open always return KErrNone.      
         }              
        }
    }

// -----------------------------------------------------------------------------
// CXIMPContextEventQueue::TopEventDataL()
// -----------------------------------------------------------------------------
//
EXPORT_C const TDesC8& CXIMPContextEventQueue::TopEventDataL()
    {
    __ASSERT_ALWAYS( iNextEventToGo,
                     User::Leave( KErrNotFound ) );
                     
    return iNextEventToGo->EventData();
    }


// -----------------------------------------------------------------------------
// CXIMPContextEventQueue::TopEventReqIdL()
// -----------------------------------------------------------------------------
//
EXPORT_C TXIMPRequestId CXIMPContextEventQueue::TopEventReqIdL()
    {
    __ASSERT_ALWAYS( iNextEventToGo,
                     User::Leave( KErrNotFound ) );
                     
    return iNextEventToGo->EventReqId();
    }
    

// -----------------------------------------------------------------------------
// CXIMPContextEventQueue::DropTopEvent()
// -----------------------------------------------------------------------------
//
EXPORT_C void CXIMPContextEventQueue::DropTopEvent()
    {
    if( iNextEventToGo )
        {
        iNextEventToGo->SetReady( EFalse ); // For preserved events
        iNextEventToGo->Close();
        iNextEventToGo = NULL;
        }
    }


// -----------------------------------------------------------------------------
// CXIMPContextEventQueue::AddEventL()
// -----------------------------------------------------------------------------
//
void CXIMPContextEventQueue::AddEventL( CXIMPEventCapsule& aEvent )
    {
    // Queue accepts the event if:
    // 1. it has a registered observer
    // 2. event type is a accepted one
    TInt32 eventType = aEvent.EventType();
    if( eventType == MXIMPRequestCompleteEvent::KInterfaceId )
        {
        aEvent.SetReady( EFalse );
        iRequestCompleteEvents.AppendL( &aEvent );
        User::LeaveIfError(aEvent.Open()); //Open always return KErrNone.  
        }
    else if( iObserver &&
        iEventFilter->IsEventAccepted( eventType ) )
        {
        iEvents.AppendL( &aEvent );
        User::LeaveIfError(aEvent.Open()); //Open always return KErrNone.  
        aEvent.SetTimeStamp( iTimeStamp++ );
        iObserver->NewEventAvailable();
        }
    }

// -----------------------------------------------------------------------------
// CXIMPContextEventQueue::AddPreservedEventL()
// -----------------------------------------------------------------------------
//
void CXIMPContextEventQueue::AddPreservedEventL( CXIMPApiEventBase& aEvent )
    {
    // Queue accepts the event always. Activation uses filter.
    CXIMPEventCapsule* capsulatedEvent = CXIMPEventCapsule::NewL( aEvent, EFalse );
    CleanupClosePushL( *capsulatedEvent );
    iPreservedEvents.AppendL( capsulatedEvent );
    CleanupStack::Pop( capsulatedEvent );
    }

// -----------------------------------------------------------------------------
// CXIMPContextEventQueue::ActivatePreservedEventL()
// -----------------------------------------------------------------------------
//
void CXIMPContextEventQueue::ActivatePreservedEventL( CXIMPApiEventBase& aEvent )
    {
    if( iObserver )
        {
        TInt eventCount = iPreservedEvents.Count();
        for( TInt a = 0; a < eventCount; ++a )
            {
            if( !iPreservedEvents[ a ]->Ready() )
                {
                iPreservedEvents[ a ]->UpdateEventDataL( aEvent );
                if( iEventFilter->IsEventAccepted( iPreservedEvents[ a ]->EventType() ) )
                    {
                    iPreservedEvents[ a ]->SetReady( ETrue );
                    iPreservedEvents[ a ]->SetTimeStamp( iTimeStamp++ );
                    iObserver->NewEventAvailable();
                    }
                return;
                }
            }
        }
    }

// -----------------------------------------------------------------------------
// CXIMPContextEventQueue::ActivateRequestCompleteEvent()
// -----------------------------------------------------------------------------
//
void CXIMPContextEventQueue::ActivateRequestCompleteEvent( CXIMPRequestCompleteEventImp& aEvent )
    {
    TInt eventCount = iRequestCompleteEvents.Count();
    for( TInt a = 0; a < eventCount; ++a )
        {
        if( iRequestCompleteEvents[ a ]->EventReqId() == aEvent.RequestId() )
            {
            if( iObserver )
                {
                if( iEventFilter->IsEventAccepted( iRequestCompleteEvents[ a ]->EventType() ) )
                    {
                    iRequestCompleteEvents[ a ]->UpdateEventData( aEvent );
                    iRequestCompleteEvents[ a ]->SetReady( ETrue );
                    iRequestCompleteEvents[ a ]->SetTimeStamp( iTimeStamp++ );
                    iObserver->NewEventAvailable();
                    }
                }
            else
                {
                iRequestCompleteEvents[ a ]->Close();
                iRequestCompleteEvents.Remove( a );
                }
            return;
            }
        }
    }

// -----------------------------------------------------------------------------
// CXIMPContextEventQueue::ActivateRequestCompleteEvent()
// -----------------------------------------------------------------------------
//
void CXIMPContextEventQueue::RemoveRequestCompleteEvent( CXIMPRequestCompleteEventImp& aEvent )
    {
    TInt eventCount = iRequestCompleteEvents.Count();
    for( TInt a = 0; a < eventCount; ++a )
        {
        if( iRequestCompleteEvents[ a ]->EventReqId() == aEvent.RequestId() )
            {
            iRequestCompleteEvents[ a ]->Close();
            iRequestCompleteEvents.Remove( a );
            return;
            }
        }
    }

// -----------------------------------------------------------------------------
// CXIMPContextEventQueue::ClearQueuedEvents()
// -----------------------------------------------------------------------------
//
void CXIMPContextEventQueue::ClearQueuedEvents()
    {
    const TInt eventCount =  iEvents.Count();
    for( TInt ix = 0; ix < eventCount ; ix++ )
        {
        iEvents[ ix ]->Close();
        }

    iEvents.Reset();
    if( iNextEventToGo )
        {
        iNextEventToGo->Close();
        iNextEventToGo = NULL;
        }
    }

// End of file

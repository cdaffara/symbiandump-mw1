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
* Description:
*
*/

#include "prfwtestlistener.h"
#include "prfwtestcaseutilspanics.h"
#include "prfwgeneralwaiter.h"

#include <eunitmacros.h>
#include <ximpcontextstateevent.h>
#include <ximprequestcompleteevent.h>
#include <ximpstatus.h>
#include <presentitypresenceevent.h>
#include <presentitygrouplistevent.h>
#include <presentitygroupcontentevent.h>
#include <presenceinfo.h>

#include <ximpbase.h>
#include "ximpapiobjbase.h"
#include "ximpapieventbase.h"


using namespace NXIMPTestCaseUtilsPanic;

const TInt KWaitAfterEvents = 2;

// ============================ MEMBER FUNCTIONS ===============================
EXPORT_C CXIMPTestListener* CXIMPTestListener::NewL(
        MXIMPContext* aEventSource )
    {
    CXIMPTestListener* self = new (ELeave) CXIMPTestListener( aEventSource );
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );
    return self;
    }


CXIMPTestListener::~CXIMPTestListener()
    {
    if( iEventSource )
        {
        iEventSource->UnregisterObserver( *this );
        }

    iDescription.Close();

    if ( iWait.IsStarted() )
        {
        iWait.AsyncStop();
        }
    }


CXIMPTestListener::CXIMPTestListener( MXIMPContext* aEventSource ) :
    iEventSource( aEventSource )
    {
    }


void CXIMPTestListener::ConstructL()
    {
    if( iEventSource )
        {
        iEventSource->RegisterObserverL( *this, NULL );
        }

    iDescription.CreateL( 512 );
    }


// ===========================================================================
// PUBLIC FUNCTIONS
// ===========================================================================
//
EXPORT_C void CXIMPTestListener::SetEventSourceL(
        MXIMPContext* aEventSource )
    {
    if( iEventSource )
        {
        iEventSource->UnregisterObserver( *this );
        }

    if( aEventSource )
        {
        aEventSource->RegisterObserverL( *this, NULL );
        }

    iEventSource = aEventSource;
    }

// clear the stack
EXPORT_C void CXIMPTestListener::Reset()
    {
    iEventStack.ResetAndDestroy();
    }

// expect the given event
EXPORT_C void CXIMPTestListener::ExpectL( CXIMPApiObjBase* aEvent )
    {
    iEventStack.AppendL( aEvent );

    iDescription.Zero();
    iDescription.AppendFormat( _L8("Eventstack has [%d] items."), iEventStack.Count() );
    EUNIT_PRINT( iDescription );
    }

// wait for req complete and events (must come in the given order!)
EXPORT_C TInt CXIMPTestListener::WaitAndAssertL()
    {
    __ASSERT_ALWAYS( !iWait.IsStarted(),
            Panic( EStatusEventListenerAlreadyWaiting ) );

    iCoolDownActive = EFalse;
    iError = KErrNone;
    iReceivedEventCount = 0;
    iWait.Start();

    // start a timer to catch excess events, 2 seconds should
    // be enough. such events are caught by DoHandleEventL and printed.
    // if any events arrived, iError will be set.

    iCoolDownActive = ETrue;
    CXIMPTestGeneralWaiter* waiter = CXIMPTestGeneralWaiter::NewL( this );
    CleanupStack::PushL( waiter );
    waiter->WaitForL( KWaitAfterEvents );
    iWait.Start();

    // waiter callback will have ended the wait
    CleanupStack::PopAndDestroy( waiter );

    return iError;
    }

// the event observer where to pass the events
EXPORT_C void CXIMPTestListener::SetEventObserver( MXIMPContextObserver* aObserver )
    {
    iObserver = aObserver;
    }

// ===========================================================================
// FROM MPRFWPRESENCECONTEXTOBSERVER
// ===========================================================================
//
void CXIMPTestListener::DoHandlePresenceContextEventL(
    const MXIMPContext& aContext,
    const MXIMPBase& aEvent )
    {
    const TInt32 eventType = aEvent.GetInterfaceId();

    __ASSERT_ALWAYS( iEventSource == &aContext,
            Panic( EReceivingEventFromWrongContext ) );

    if( iObserver )
        {
        // give the event to the observer, if any, at the earliest possible
        // phase
        iObserver->HandlePresenceContextEvent( aContext, aEvent );

        // XXX should this be done when cool-down is active?
        }

    if ( iCoolDownActive )
    {
        iDescription.Zero();
        iDescription.AppendFormat( _L8("Excess event with interface id [%d]"), aEvent.GetInterfaceId() );
        EUNIT_PRINT( iDescription );

        iError = KErrGeneral;

        // just ignore it
        return;
    }

    iDescription.Zero();
    iDescription.AppendFormat( _L8("Expecting event to stack pos [%d]"), iReceivedEventCount );
    EUNIT_PRINT( iDescription );

    // try to find the received event from the stack - the given stack must
    // be in order
    TBool found = EFalse;
    TInt stackId = iEventStack[ 0 ]->Base().GetInterfaceId();
    found = stackId == aEvent.GetInterfaceId();

    if ( ! found )
        {
        // unexpected event
        iDescription.Zero();
        iDescription.AppendFormat( _L8("Expected interface id [%d], got [%d]"), stackId, aEvent.GetInterfaceId() );
        EUNIT_PRINT( iDescription );

        iDescription.Zero();
        iDescription.AppendFormat( _L8("Received unexpected event at pos [%d]"), iReceivedEventCount );
        EUNIT_PRINT( iDescription );

        // get out
        User::Leave( KErrGeneral );
        }

    // found the received event from the stack of expected events.
    // remove the event from the stack of expected events, then handle it.
    CXIMPApiEventBase* expected = ( CXIMPApiEventBase* ) iEventStack[ 0 ];
    CleanupStack::PushL( expected );
    iEventStack.Remove( 0 );

    CXIMPApiEventBase* received = ( CXIMPApiEventBase* ) ( aEvent.GetInterface( XIMPIMP_CLSID_CXIMPAPIEVENTBASE, MXIMPBase::EPanicIfUnknown ) );

    if ( ! expected->EqualsContent( *received ) )
        {
        // expected event mismatches with the received event
        iDescription.Zero();
        iDescription.AppendFormat( _L8("Received event with mismatching content at pos [%d]"), iReceivedEventCount );
        EUNIT_PRINT( iDescription );

        // get out
        User::Leave( KErrGeneral );
        }

    CleanupStack::PopAndDestroy( expected );

    iDescription.Zero();
    iDescription.AppendFormat( _L8("Got correct event to stack pos [%d]"), iReceivedEventCount );
    EUNIT_PRINT( iDescription );

    // we've processed a received event successfully
    iReceivedEventCount++;

    if ( iEventStack.Count() <= 0 )
        {
        // no more events to expect
        if( iWait.IsStarted() )
            {
            iWait.AsyncStop();
            }
        }
    }
    

void CXIMPTestListener::HandlePresenceContextEvent( 
    const MXIMPContext& aContext,
    const MXIMPBase& aEvent )
    {
    TRAPD( err, DoHandlePresenceContextEventL( aContext, aEvent ) );

    if ( err != KErrNone )
        {
        // something evil happened, stop async wait

        iError = err;

        if( iWait.IsStarted() )
            {
            iWait.AsyncStop();
            }
        }
    }

void CXIMPTestListener::WaitCompleted( TInt aRetVal )
    {
    if( iWait.IsStarted() )
        {
        iWait.AsyncStop();
        }
    }

// end of file


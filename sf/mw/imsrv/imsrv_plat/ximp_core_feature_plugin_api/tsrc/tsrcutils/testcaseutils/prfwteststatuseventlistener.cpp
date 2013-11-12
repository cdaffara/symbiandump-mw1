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

#include "prfwteststatuseventlistener.h"
#include "prfwtestcaseutilspanics.h"
#include <eunitmacros.h>

#include <ximpcontext.h>
#include <ximpcontextstateevent.h>
#include <ximprequestcompleteevent.h>
#include <ximpstatus.h>
#include <ownpresenceevent.h>
#include <presentitypresenceevent.h>
#include <presentitygrouplistevent.h>
#include <presentitygroupcontentevent.h>
#include <presencewatcherlistevent.h>
#include <presencewatcherinfo.h>
#include <ximpdatasubscriptionstate.h>


using namespace NXIMPTestCaseUtilsPanic;
EUNIT_DECLARE_PRINTABLE_AS_TINT( MXIMPContextState::TState )


    // ============================ MEMBER FUNCTIONS ===============================
EXPORT_C TXIMPTestStatusEventSnapshot::TXIMPTestStatusEventSnapshot()
    {
    iEventType = TXIMPTestStatusEventSnapshot::EStateValueNotPresent;
    iCtxState = (MXIMPContextState::TState) TXIMPTestStatusEventSnapshot::EStateValueNotPresent;
    iReqId = TXIMPRequestId();
    iResultCode = TXIMPTestStatusEventSnapshot::EStateValueNotPresent;
    }


EXPORT_C TXIMPTestStatusEventSnapshot::TXIMPTestStatusEventSnapshot(
        TInt aEventType,
        TInt aCtxState,
        TInt aResultCode )
    {
    iEventType = aEventType;
    iCtxState = (MXIMPContextState::TState) aCtxState;
    iResultCode = aResultCode;
    }

EXPORT_C TBool TXIMPTestStatusEventSnapshot::Equals(
        const TXIMPTestStatusEventSnapshot& aOther ) const
    {
    TBool equals = ETrue;

    if( iEventType != aOther.iEventType )
        {
        equals = EFalse;
        }

    if( iCtxState != aOther.iCtxState )
        {
        equals = EFalse;
        }

    if( iResultCode != aOther.iResultCode )
        {
        equals = EFalse;
        }

    return equals;
    }





// ============================ MEMBER FUNCTIONS ===============================
EXPORT_C CXIMPTestStatusEventListener* CXIMPTestStatusEventListener::NewL(
        TInt aStackSize,
        MXIMPContext* aEventSource )
    {
    CXIMPTestStatusEventListener* self =
        new (ELeave) CXIMPTestStatusEventListener( aStackSize, aEventSource );
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );
    return self;
    }


CXIMPTestStatusEventListener::~CXIMPTestStatusEventListener()
    {
    User::Free( iSnapshots );
    if( iEventSource )
        {
        iEventSource->UnregisterObserver( *this );
        }

    iAcceptedEventTypes.Reset();
    iDescription.Close();
    }


CXIMPTestStatusEventListener::CXIMPTestStatusEventListener(
        TInt aStackSize,
        MXIMPContext* aEventSource )
: iSnapshotMaxCount( aStackSize ),
    iEventSource( aEventSource )
    {
    }


void CXIMPTestStatusEventListener::ConstructL()
    {
    __ASSERT_ALWAYS( iSnapshotMaxCount > 0,
            Panic( EStatusEventListenerBadMaxCount ) );

    TInt allocSize = iSnapshotMaxCount * sizeof( TXIMPTestStatusEventSnapshot );
    iSnapshots = (TXIMPTestStatusEventSnapshot*) User::AllocL( allocSize );
    ResetEventStack();
    ResetEventTypeStack();

    if( iEventSource )
        {
        TArray< TInt32 > eventFilterArray = iAcceptedEventTypes.Array();
        iEventSource->RegisterObserverL( *this, &eventFilterArray );
        }

    iDescription.CreateL( 512 );
    }




// ===========================================================================
// PUBLIC FUNCTIONS
// ===========================================================================
//
EXPORT_C void CXIMPTestStatusEventListener::SetEventSourceL(
        MXIMPContext* aEventSource )
    {
    if( iEventSource )
        {
        iEventSource->UnregisterObserver( *this );
        }

    if( aEventSource )
        {
        TArray< TInt32 > eventFilterArray = iAcceptedEventTypes.Array();
        aEventSource->RegisterObserverL( *this, &eventFilterArray );
        }

    iEventSource = aEventSource;
    }



EXPORT_C void CXIMPTestStatusEventListener::ReRegisterEventFilterL()
    {
    if( iEventSource )
        {
        TArray< TInt32 > eventFilterArray = iAcceptedEventTypes.Array();
        iEventSource->RegisterObserverL( *this, &eventFilterArray );
        }
    }



EXPORT_C RArray< TInt32 >& CXIMPTestStatusEventListener::AcceptedEventTypes()
    {
    return iAcceptedEventTypes;
    }



EXPORT_C void CXIMPTestStatusEventListener::GetEventTemplateL(
        RArray< TXIMPTestStatusEventSnapshot >& aArray,
        TXIMPTestStatusEventTemplate aTemplate )
    {
    switch( aTemplate )
        {
        // session handling
        case EXIMPTestStatusEvents_BindingOk:
            {
            TXIMPTestStatusEventSnapshot event;
            event.iEventType = MXIMPContextStateEvent::KInterfaceId;
            event.iCtxState = MXIMPContextState::EBinding;
            event.iResultCode = TXIMPTestStatusEventSnapshot::EStatusObjNotPresent;
            aArray.AppendL( event );

            event.iEventType = MXIMPContextStateEvent::KInterfaceId;
            event.iCtxState = MXIMPContextState::EActive;
            event.iResultCode = TXIMPTestStatusEventSnapshot::EStatusObjNotPresent;
            aArray.AppendL( event );

            event.iEventType = MXIMPRequestCompleteEvent::KInterfaceId;
            event.iCtxState = (MXIMPContextState::TState) TXIMPTestStatusEventSnapshot::EStateValueNotPresent;
            event.iResultCode = KErrNone;
            aArray.AppendL( event );

            break;
            }

        case EXIMPTestStatusEvents_BindingFailKErrAbort:
            {
            TXIMPTestStatusEventSnapshot event;
            event.iEventType = MXIMPContextStateEvent::KInterfaceId;
            event.iCtxState = MXIMPContextState::EBinding;
            event.iResultCode = TXIMPTestStatusEventSnapshot::EStatusObjNotPresent;
            aArray.AppendL( event );

            event.iEventType = MXIMPContextStateEvent::KInterfaceId;
            event.iCtxState = MXIMPContextState::EInactive;
            event.iResultCode = TXIMPTestStatusEventSnapshot::EStatusObjNotPresent;
            aArray.AppendL( event );

            event.iEventType = MXIMPRequestCompleteEvent::KInterfaceId;
            event.iCtxState = (MXIMPContextState::TState) TXIMPTestStatusEventSnapshot::EStateValueNotPresent;
            event.iResultCode = KErrAbort;
            aArray.AppendL( event );
            break;
            }

        case EXIMPTestStatusEvents_BindingFailInvalidReqId:
            {
            TXIMPTestStatusEventSnapshot event;
            event.iEventType = MXIMPContextStateEvent::KInterfaceId;
            event.iCtxState = MXIMPContextState::EBinding;
            event.iResultCode = TXIMPTestStatusEventSnapshot::EStatusObjNotPresent;
            aArray.AppendL( event );

            event.iEventType = MXIMPRequestCompleteEvent::KInterfaceId;
            event.iCtxState = (MXIMPContextState::TState) TXIMPTestStatusEventSnapshot::EStateValueNotPresent,
            event.iResultCode = KErrServerTerminated;
            aArray.AppendL( event );            
            
            event.iEventType = MXIMPContextStateEvent::KInterfaceId;
            event.iCtxState = MXIMPContextState::EInactive;
            event.iResultCode = KErrServerTerminated;
            
            aArray.AppendL( event );
            break;
            }


        case EXIMPTestStatusEvents_BindingFailServerTermination:
            {
            TXIMPTestStatusEventSnapshot event;
            event.iEventType = MXIMPContextStateEvent::KInterfaceId;
            event.iCtxState = MXIMPContextState::EUnbinding;
            event.iResultCode = KErrServerTerminated;
            aArray.AppendL( event );

            event.iEventType = MXIMPContextStateEvent::KInterfaceId;
            event.iCtxState = MXIMPContextState::EInactive;
            event.iResultCode = KErrServerTerminated;
            aArray.AppendL( event );

            event.iEventType = MXIMPRequestCompleteEvent::KInterfaceId;
            event.iCtxState = (MXIMPContextState::TState) TXIMPTestStatusEventSnapshot::EStateValueNotPresent;
            event.iResultCode = KErrServerTerminated;
            aArray.AppendL( event );

            break;
            }

        case EXIMPTestStatusEvents_UnbindingOk:
            {
            TXIMPTestStatusEventSnapshot event;
            event.iEventType = MXIMPContextStateEvent::KInterfaceId;
            event.iCtxState = MXIMPContextState::EUnbinding;
            event.iResultCode = TXIMPTestStatusEventSnapshot::EStatusObjNotPresent;
            aArray.AppendL( event );

            event.iEventType = MXIMPContextStateEvent::KInterfaceId;
            event.iCtxState = MXIMPContextState::EInactive;
            event.iResultCode = TXIMPTestStatusEventSnapshot::EStatusObjNotPresent;
            aArray.AppendL( event );

            event.iEventType = MXIMPRequestCompleteEvent::KInterfaceId;
            event.iCtxState = (MXIMPContextState::TState) TXIMPTestStatusEventSnapshot::EStateValueNotPresent;
            event.iResultCode = KErrNone;
            aArray.AppendL( event );
            break;
            }


        case EXIMPTestStatusEvents_RequestOk:
            {
            TXIMPTestStatusEventSnapshot event;
            event.iEventType = MXIMPRequestCompleteEvent::KInterfaceId;
            event.iCtxState = (MXIMPContextState::TState) TXIMPTestStatusEventSnapshot::EStateValueNotPresent;
            event.iResultCode = KErrNone;
            aArray.AppendL( event );
            break;
            }


        case EXIMPTestStatusEvents_RequestOOM:
            {
            TXIMPTestStatusEventSnapshot event;
            event.iEventType = MXIMPRequestCompleteEvent::KInterfaceId;
            event.iCtxState = (MXIMPContextState::TState) TXIMPTestStatusEventSnapshot::EStateValueNotPresent;
            event.iResultCode = KErrNoMemory;
            aArray.AppendL( event );
            break;
            }



        // own presence
        case EXIMPTestStatusEvents_SubscribeOwnPresenceSingleCtx:
            {
            TXIMPTestStatusEventSnapshot event;
            event.iEventType = MOwnPresenceEvent::KInterfaceId;
            event.iCtxState = (MXIMPContextState::TState) ( MXIMPDataSubscriptionState::EDataUnavailable + MXIMPDataSubscriptionState::ESubscriptionActive );
            event.iResultCode = TXIMPTestStatusEventSnapshot::EStateValueNotPresent;
            aArray.AppendL( event );
            break;
            }

        case EXIMPTestStatusEvents_UnsubscribeOwnPresenceSingleCtx:
            {
            TXIMPTestStatusEventSnapshot event;
            event.iEventType = MOwnPresenceEvent::KInterfaceId;
            event.iCtxState = (MXIMPContextState::TState) ( MXIMPDataSubscriptionState::EDataUnavailable + MXIMPDataSubscriptionState::ESubscriptionInactive );
            event.iResultCode = TXIMPTestStatusEventSnapshot::EStateValueNotPresent;
            aArray.AppendL( event );
            break;
            }

        // presentity presence
        case EXIMPTestStatusEvents_SubscribePresentityPresenceSingleCtx:
            {
            TXIMPTestStatusEventSnapshot event;
            event.iEventType = MPresentityPresenceEvent::KInterfaceId;
            event.iCtxState = (MXIMPContextState::TState) ( MXIMPDataSubscriptionState::EDataUnavailable + MXIMPDataSubscriptionState::ESubscriptionActive );
            event.iResultCode = TXIMPTestStatusEventSnapshot::EStateValueNotPresent;
            aArray.AppendL( event );
            break;
            }

        case EXIMPTestStatusEvents_UnsubscribePresentityPresenceSingleCtx:
            {
            TXIMPTestStatusEventSnapshot event;
            event.iEventType = MPresentityPresenceEvent::KInterfaceId;
            event.iCtxState = (MXIMPContextState::TState) ( MXIMPDataSubscriptionState::EDataUnavailable + MXIMPDataSubscriptionState::ESubscriptionInactive );
            event.iResultCode = TXIMPTestStatusEventSnapshot::EStateValueNotPresent;
            aArray.AppendL( event );
            break;
            }

        // group lists
        case EXIMPTestStatusEvents_SubscribePresentityGroupListCtx:
            {
            // TODO check this
            TXIMPTestStatusEventSnapshot event;
            event.iEventType = MPresentityGroupListEvent::KInterfaceId;
            event.iCtxState = (MXIMPContextState::TState) TXIMPTestStatusEventSnapshot::EStateValueNotPresent;
            event.iResultCode = KErrNone;
            aArray.AppendL( event );

            event.iEventType = MXIMPRequestCompleteEvent::KInterfaceId;
            event.iCtxState = (MXIMPContextState::TState) TXIMPTestStatusEventSnapshot::EStateValueNotPresent;
            event.iResultCode = KErrNone;
            aArray.AppendL( event );
            break;
            }

        case EXIMPTestStatusEvents_UnsubscribePresentityGroupListCtx:
            {
            TXIMPTestStatusEventSnapshot event;
            event.iEventType = MPresentityGroupListEvent::KInterfaceId;
            // TODO check this
            event.iCtxState = (MXIMPContextState::TState) TXIMPTestStatusEventSnapshot::EStateValueNotPresent;
            event.iResultCode = TXIMPTestStatusEventSnapshot::EStateValueNotPresent;
            aArray.AppendL( event );
            break;
            }

        case EXIMPTestStatusEvents_CreatePresentityGroup:
            {
            TXIMPTestStatusEventSnapshot event;
            event.iEventType = MPresentityGroupListEvent::KInterfaceId;
            // TODO check this
            event.iCtxState = (MXIMPContextState::TState) TXIMPTestStatusEventSnapshot::EStateValueNotPresent;
            event.iResultCode = TXIMPTestStatusEventSnapshot::EStateValueNotPresent;
            aArray.AppendL( event );

            event.iEventType = MXIMPRequestCompleteEvent::KInterfaceId;
            event.iCtxState = (MXIMPContextState::TState) TXIMPTestStatusEventSnapshot::EStateValueNotPresent;
            event.iResultCode = KErrNone;
            aArray.AppendL( event );

            break;
            }

        case EXIMPTestStatusEvents_HandlePresentityGroupList:
            {
            TXIMPTestStatusEventSnapshot event;
            event.iEventType = MPresentityGroupListEvent::KInterfaceId;
            // TODO check this
            event.iCtxState = (MXIMPContextState::TState) TXIMPTestStatusEventSnapshot::EStateValueNotPresent;
            event.iResultCode = TXIMPTestStatusEventSnapshot::EStateValueNotPresent;
            aArray.AppendL( event );
            break;
            }

        // group content
        case EXIMPTestStatusEvents_SubscribePresentityGroupContentSingleCtx:
            {
            // TODO check this
            TXIMPTestStatusEventSnapshot event;
            event.iEventType = MPresentityGroupContentEvent::KInterfaceId;
            event.iCtxState = (MXIMPContextState::TState) TXIMPTestStatusEventSnapshot::EStateValueNotPresent;
            event.iResultCode = KErrNone;
            aArray.AppendL( event );

            event.iEventType = MXIMPRequestCompleteEvent::KInterfaceId;
            event.iCtxState = (MXIMPContextState::TState) TXIMPTestStatusEventSnapshot::EStateValueNotPresent;
            event.iResultCode = KErrNone;
            aArray.AppendL( event );
            break;
            }

        case EXIMPTestStatusEvents_UnsubscribePresentityGroupContentSingleCtx:
            {
            TXIMPTestStatusEventSnapshot event;
            event.iEventType = MPresentityGroupContentEvent::KInterfaceId;
            event.iCtxState = (MXIMPContextState::TState) TXIMPTestStatusEventSnapshot::EStateValueNotPresent;
            event.iResultCode = TXIMPTestStatusEventSnapshot::EStateValueNotPresent;
            aArray.AppendL( event );

            event.iEventType = MXIMPRequestCompleteEvent::KInterfaceId;
            event.iCtxState = (MXIMPContextState::TState) TXIMPTestStatusEventSnapshot::EStateValueNotPresent;
            event.iResultCode = KErrNone;
            aArray.AppendL( event );
            break;
            }

        // watcher list
        case EXIMPTestStatusEvents_SubscribePresenceWatcherListCtx:
            {
            TXIMPTestStatusEventSnapshot event;
            event.iEventType = MPresenceWatcherListEvent::KInterfaceId;
            event.iCtxState = (MXIMPContextState::TState) TXIMPTestStatusEventSnapshot::EStateValueNotPresent;
            event.iResultCode = KErrNone;
            aArray.AppendL( event );

            event.iEventType = MXIMPRequestCompleteEvent::KInterfaceId;
            event.iCtxState = (MXIMPContextState::TState) TXIMPTestStatusEventSnapshot::EStateValueNotPresent;
            event.iResultCode = KErrNone;
            aArray.AppendL( event );
            break;
            }

        case EXIMPTestStatusEvents_UnsubscribePresenceWatcherListCtx:
            {
            TXIMPTestStatusEventSnapshot event;
            event.iEventType = MPresenceWatcherListEvent::KInterfaceId;
            event.iCtxState = (MXIMPContextState::TState) TXIMPTestStatusEventSnapshot::EStateValueNotPresent;
            event.iResultCode = TXIMPTestStatusEventSnapshot::EStateValueNotPresent;
            aArray.AppendL( event );
            break;
            }

        case EXIMPTestStatusEvents_HandlePresenceWatcherList:
            {
            TXIMPTestStatusEventSnapshot event;
            event.iEventType = MPresenceWatcherListEvent::KInterfaceId;
            event.iCtxState = (MXIMPContextState::TState) TXIMPTestStatusEventSnapshot::EStateValueNotPresent;
            event.iResultCode = TXIMPTestStatusEventSnapshot::EStateValueNotPresent;
            aArray.AppendL( event );
            break;
            }

        case EXIMPTestStatusEvents_HandlePresentityPresence:
            {
            TXIMPTestStatusEventSnapshot event;
            event.iEventType = MPresentityPresenceEvent::KInterfaceId;
            event.iCtxState = (MXIMPContextState::TState) ( MXIMPDataSubscriptionState::EDataAvailable + MXIMPDataSubscriptionState::ESubscriptionActive );
            event.iResultCode = TXIMPTestStatusEventSnapshot::EStateValueNotPresent;
            aArray.AppendL( event );
            break;
            }


        case EXIMPTestStatusEvents_HandlePresentityPresencePending:
            {
            TXIMPTestStatusEventSnapshot event;
            event.iEventType = MPresentityPresenceEvent::KInterfaceId;
            event.iCtxState = (MXIMPContextState::TState) ( MXIMPDataSubscriptionState::EDataUnavailable + MXIMPDataSubscriptionState::ESubscriptionActive );
            event.iResultCode = TXIMPTestStatusEventSnapshot::EStateValueNotPresent;
            aArray.AppendL( event );
            break;
            }

        case EXIMPTestStatusEvents_PresentitySubscribeFailKErrAbort:
            {
            TXIMPTestStatusEventSnapshot event;
            event.iEventType = MXIMPRequestCompleteEvent::KInterfaceId;
            event.iCtxState = (MXIMPContextState::TState) TXIMPTestStatusEventSnapshot::EStateValueNotPresent;
            event.iResultCode = KErrAbort;
            aArray.AppendL( event );
            break;
            }

        // misc
        case EXIMPTestStatusEvents_PublishOk:
            {
            TXIMPTestStatusEventSnapshot event;

            // request complete with KErrNone
            event.iEventType = MXIMPRequestCompleteEvent::KInterfaceId;
            event.iCtxState = (MXIMPContextState::TState) TXIMPTestStatusEventSnapshot::EStateValueNotPresent;
            event.iResultCode = KErrNone;
            aArray.AppendL( event );

            // OwnPresenceEvent
            event.iEventType = MOwnPresenceEvent::KInterfaceId;
            event.iCtxState = (MXIMPContextState::TState) TXIMPTestStatusEventSnapshot::EStateValueNotPresent;
            event.iResultCode = TXIMPTestStatusEventSnapshot::EStatusObjNotPresent;
            aArray.AppendL( event );

            break;
            }

        case EXIMPTestStatusEvents_PublishFailKErrAbort:
            {
            TXIMPTestStatusEventSnapshot event;

            // Request completed with KErrAbort
            event.iEventType = MXIMPRequestCompleteEvent::KInterfaceId;
            event.iCtxState = (MXIMPContextState::TState) TXIMPTestStatusEventSnapshot::EStateValueNotPresent;
            event.iResultCode = KErrAbort;
            aArray.AppendL( event );

            break;
            }

        default:
            {
            Panic( EStatusEventListenerUnknownEventTemplate );
            }
        }
    }



EXPORT_C void CXIMPTestStatusEventListener::WaitRequestAndStackEvents(
        const TXIMPRequestId& aReqToWait, TBool aAnyReq /* = EFalse */ )
    {
    __ASSERT_ALWAYS( !iWait.IsStarted(),
            Panic( EStatusEventListenerAlreadyWaiting ) );

    iWaitForAnyReq = aAnyReq;
    iReqToWait = aReqToWait;
    iWaitForAnyEvent = EFalse;

    iWait.Start();
    }

EXPORT_C void CXIMPTestStatusEventListener::WaitAnyEvent()
    {
    iWaitForAnyEvent = ETrue;
    iWait.Start();
    }


EXPORT_C void CXIMPTestStatusEventListener::VerifyEventStackL(
        const TArray< TXIMPTestStatusEventSnapshot >& aEventTraits,
        const TDesC8& aErrorTitle ) const
    {
    iDescription.Zero();
    TInt ix = 0;

    //Dumb event lists if there is some mismatches
    if( !HasSameEvents( aEventTraits ) )
        {
        iDescription = aErrorTitle;
        iDescription += _L8("Mismatches in event stack");
        EUNIT_PRINT( iDescription );

        iDescription = _L8("Received: ");
        for( ix = 0; ix < iSnapshotCount; ix++ )
            {
            GetNameForEvent( iDescription, SnapshotFromStack( ix ).iEventType );
            }
        EUNIT_PRINT( iDescription );


        iDescription = _L8("Expected: ");
        const TInt traitCount = aEventTraits.Count();
        for( ix = 0; ix < traitCount; ix++ )
            {
            GetNameForEvent( iDescription, aEventTraits[ ix ].iEventType );
            }
        EUNIT_PRINT( iDescription );
        }


    //Now do the actual verification
    iDescription = aErrorTitle;
    iDescription += _L8("Event count mismatch");
    EUNIT_ASSERT_EQUALS_DESC( iSnapshotCount, aEventTraits.Count(), iDescription.PtrZ() );


    for( TInt ix = 0; ix < iSnapshotCount; ix++ )
        {
        const TXIMPTestStatusEventSnapshot& expected = aEventTraits[ ix ];
        const TXIMPTestStatusEventSnapshot& received = SnapshotFromStack( ix );

            {
            iDescription = aErrorTitle;
            iDescription.AppendFormat( _L8("Event type mismatch on event [%d]"), ix );
            EUNIT_ASSERT_EQUALS_DESC( expected.iEventType, expected.iEventType, iDescription.PtrZ() );
            }

            {
            const TInt type = received.iEventType;

            iDescription = aErrorTitle;
            iDescription += _L8("Content mismatch on ");
            GetNameForEvent( iDescription, type );
            iDescription.AppendFormat( _L8("event on position [%d]"), ix );
            switch( type )
                {
                case MXIMPRequestCompleteEvent::KInterfaceId:
                    {
                    EUNIT_ASSERT_EQUALS_DESC( expected.iResultCode, received.iResultCode, iDescription.PtrZ() );
                    break;
                    }


                case MXIMPContextStateEvent::KInterfaceId:
                    {
                    EUNIT_ASSERT_EQUALS_DESC( expected.iCtxState, received.iCtxState, iDescription.PtrZ() );
                    EUNIT_ASSERT_EQUALS_DESC( expected.iResultCode, received.iResultCode, iDescription.PtrZ() );
                    break;
                    }

                case MOwnPresenceEvent::KInterfaceId:
                case MPresentityPresenceEvent::KInterfaceId:
                    {
                    EUNIT_ASSERT_EQUALS_DESC( expected.iCtxState, received.iCtxState, iDescription.PtrZ() );
                    break;
                    }

                case MPresentityGroupListEvent::KInterfaceId:
                    {
                    EUNIT_ASSERT_EQUALS_DESC( expected.iCtxState, received.iCtxState, iDescription.PtrZ() );
                    EUNIT_ASSERT_EQUALS_DESC( expected.iResultCode, received.iResultCode, iDescription.PtrZ() );
                    break;
                    }

                case MPresentityGroupContentEvent::KInterfaceId:
                    {
                    EUNIT_ASSERT_EQUALS_DESC( expected.iCtxState, received.iCtxState, iDescription.PtrZ() );
                    EUNIT_ASSERT_EQUALS_DESC( expected.iResultCode, received.iResultCode, iDescription.PtrZ() );
                    break;
                    }

                case MPresenceWatcherListEvent::KInterfaceId:
                    {
                    EUNIT_ASSERT_EQUALS_DESC( expected.iCtxState, received.iCtxState, iDescription.PtrZ() );
                    EUNIT_ASSERT_EQUALS_DESC( expected.iResultCode, received.iResultCode, iDescription.PtrZ() );
                    break;
                    }


                default:
                    {
                    Panic( EUnsupportedEventTypeInVerification );
                    break;
                    }
                }
            }
        }
    }




EXPORT_C TInt CXIMPTestStatusEventListener::GetReqCompletionErrL( 
    const TXIMPRequestId& aReqId ) const
    {
    for( TInt ix = 0; ix < iSnapshotCount; ix++ )
        {
        const TXIMPTestStatusEventSnapshot& received = SnapshotFromStack( ix );
        const TInt type = received.iEventType;

        if( type == MXIMPRequestCompleteEvent::KInterfaceId )
            {
            if( received.iReqId == aReqId )
                {
                return received.iResultCode;
                }
            }
        }
    
    User::Leave( KErrNotFound );
    return KErrNotFound;
    }


EXPORT_C void CXIMPTestStatusEventListener::ResetEventStack()
    {
    for( TInt ix = 0; ix < iSnapshotMaxCount; ix++ )
        {
        new( &iSnapshots[ ix ] ) TXIMPTestStatusEventSnapshot();
        }

    iSnapshotCount = 0;
    }

EXPORT_C void CXIMPTestStatusEventListener::ResetEventTypeStack()
    {
    iAcceptedEventTypes.Reset();
    iAcceptedEventTypes.AppendL( MXIMPRequestCompleteEvent::KInterfaceId );
    iAcceptedEventTypes.AppendL( MXIMPContextStateEvent::KInterfaceId );
    iAcceptedEventTypes.AppendL( MOwnPresenceEvent::KInterfaceId );
    iAcceptedEventTypes.AppendL( MPresentityPresenceEvent::KInterfaceId );
    }

EXPORT_C void CXIMPTestStatusEventListener::SetEventObserver( MXIMPContextObserver* aObserver )
    {
    iObserver = aObserver;
    }








// ===========================================================================
// FROM MPRFWPRESENCECONTEXTOBSERVER
// ===========================================================================
//
void CXIMPTestStatusEventListener::HandlePresenceContextEvent(
    const MXIMPContext& aContext,
    const MXIMPBase& aEvent )
    {
    const TInt32 eventType = aEvent.GetInterfaceId();

    __ASSERT_ALWAYS( iEventSource == &aContext,
            Panic( EReceivingEventFromWrongContext ) );

    __ASSERT_ALWAYS( IsAcceptedEventType( eventType ),
            Panic( EReceivedNotAcceptedEventType ) );

    TXIMPTestStatusEventSnapshot snapShot;
    switch( eventType )
        {
        case MXIMPRequestCompleteEvent::KInterfaceId:
            {
            const MXIMPRequestCompleteEvent* event =
                TXIMPGetInterface< const MXIMPRequestCompleteEvent >::From( aEvent, MXIMPBase::EPanicIfUnknown );
                {
                snapShot.iEventType = event->GetInterfaceId();
                snapShot.iCtxState = (MXIMPContextState::TState) TXIMPTestStatusEventSnapshot::EStateValueNotPresent;
                snapShot.iReqId = event->RequestId();
                snapShot.iResultCode = event->CompletionResult().ResultCode();

                AddSnapshotToStack( snapShot );
                }

            if( iWaitForAnyReq || event->RequestId() == iReqToWait )
                {
                HandleRequestCompleteReceived();
                }

            break;
            }

        case MXIMPContextStateEvent::KInterfaceId:
            {
            const MXIMPContextStateEvent& event =
                *TXIMPGetInterface< const MXIMPContextStateEvent >::From( aEvent,
                                                                                  MXIMPBase::EPanicIfUnknown );
                {
                TXIMPTestStatusEventSnapshot snapShot;
                snapShot.iEventType = event.GetInterfaceId();
                snapShot.iCtxState = event.ContextState().ContextState();
                if( event.StateChangeReason() )
                    {
                    snapShot.iResultCode = event.StateChangeReason()->ResultCode();
                    }
                else
                    {
                    snapShot.iResultCode = TXIMPTestStatusEventSnapshot::EStatusObjNotPresent;
                    }

                AddSnapshotToStack( snapShot );
                }
            break;
            }

        case MOwnPresenceEvent::KInterfaceId:
            {
            const MOwnPresenceEvent& event =
                *TXIMPGetInterface< const MOwnPresenceEvent >::From( aEvent,
                                                                         MXIMPBase::EPanicIfUnknown );
                {
                TXIMPTestStatusEventSnapshot snapShot;
                snapShot.iEventType = event.GetInterfaceId();
                snapShot.iCtxState = ( MXIMPContextState::TState ) ( event.DataSubscriptionState().DataState() + event.DataSubscriptionState().SubscriptionState() );
                AddSnapshotToStack( snapShot );
                }
            break;
            }

        case MPresentityPresenceEvent::KInterfaceId:
            {
            const MPresentityPresenceEvent& event =
                *TXIMPGetInterface< const MPresentityPresenceEvent >::From( aEvent,
                                                                                MXIMPBase::EPanicIfUnknown );
                {
                TXIMPTestStatusEventSnapshot snapShot;
                snapShot.iEventType = event.GetInterfaceId();
                snapShot.iCtxState = ( MXIMPContextState::TState ) ( event.DataSubscriptionState().DataState() + event.DataSubscriptionState().SubscriptionState() );
                AddSnapshotToStack( snapShot );
                }
            break;
            }

        case MPresentityGroupListEvent::KInterfaceId:
            {
            const MPresentityGroupListEvent& event =
                *TXIMPGetInterface< const MPresentityGroupListEvent >::From( aEvent,
                                                                                 MXIMPBase::EPanicIfUnknown );
                {
                TXIMPTestStatusEventSnapshot snapShot;
                snapShot.iEventType = event.GetInterfaceId();
                AddSnapshotToStack( snapShot );
                }
            break;
            }

        case MPresentityGroupContentEvent::KInterfaceId:
            {
            const MPresentityGroupContentEvent& event =
            *TXIMPGetInterface< const MPresentityGroupContentEvent >::From( aEvent,
                                                                                MXIMPBase::EPanicIfUnknown );
                {
                TXIMPTestStatusEventSnapshot snapShot;
                snapShot.iEventType = event.GetInterfaceId();
                AddSnapshotToStack( snapShot );
                }
            break;
            }


        case MPresenceWatcherListEvent::KInterfaceId:
            {
            const MPresenceWatcherListEvent& event =
                *TXIMPGetInterface< const MPresenceWatcherListEvent >::From( aEvent,
                                                                                 MXIMPBase::EPanicIfUnknown );
                {
                TXIMPTestStatusEventSnapshot snapShot;
                snapShot.iEventType = event.GetInterfaceId();
                AddSnapshotToStack( snapShot );
                }
            break;
            }


        default:
            {
            break;
            }
        }

    if( iObserver )
        {
        iObserver->HandlePresenceContextEvent( aContext, aEvent );
        }

    if( iWaitForAnyEvent )
        {
        HandleRequestCompleteReceived();
        }
    }



// ===========================================================================
// HELPERS
// ===========================================================================
//
void CXIMPTestStatusEventListener::HandleRequestCompleteReceived()
    {
    if( iWait.IsStarted() )
        {
        iWait.AsyncStop();
        }
    }



TBool CXIMPTestStatusEventListener::IsAcceptedEventType( TInt32 aEventType ) const
    {
    TInt index = iAcceptedEventTypes.Find( aEventType );
    if( index == KErrNotFound )
        {
        return EFalse;
        }
    return ETrue;
    }



void CXIMPTestStatusEventListener::AddSnapshotToStack(
        TXIMPTestStatusEventSnapshot aSnapshot )
    {
    __ASSERT_ALWAYS( iSnapshotCount < iSnapshotMaxCount,
            Panic( EAddSnapshotOverflow ) );

    iSnapshots[ iSnapshotCount ] = aSnapshot;
    iSnapshotCount++;
    }



const TXIMPTestStatusEventSnapshot& CXIMPTestStatusEventListener::SnapshotFromStack( TInt aIndex ) const
    {
    __ASSERT_ALWAYS( aIndex < iSnapshotCount,
            Panic( ESnapshotIndexOutOfBounds ) );

    return iSnapshots[ aIndex ];
    }



TBool CXIMPTestStatusEventListener::HasSameEvents(
        const TArray< TXIMPTestStatusEventSnapshot >& aEventTraits ) const
    {
    TBool hasSameEvents = ETrue;

    if( iSnapshotCount != aEventTraits.Count() )
        {
        hasSameEvents = EFalse;
        }
    else
        {
        for( TInt ix = 0; ix < iSnapshotCount; ix++ )
            {
            const TXIMPTestStatusEventSnapshot& eventTrait = aEventTraits[ ix ];
            const TXIMPTestStatusEventSnapshot& receivedEvent = SnapshotFromStack( ix );

            if( !receivedEvent.Equals( eventTrait ) )
                {
                hasSameEvents = EFalse;
                }
            }
        }

    return hasSameEvents;
    }



void CXIMPTestStatusEventListener::GetNameForEvent(
        TDes8& aTarget,
        TInt aEventType ) const
    {
    switch( aEventType )
        {
        case MXIMPRequestCompleteEvent::KInterfaceId:
            {
            SafeAppendEventName( aTarget, _L8("[ReqComplete] ") );
            break;
            }

        case MXIMPContextStateEvent::KInterfaceId:
            {
            SafeAppendEventName( aTarget, _L8("[PresCtx] ") );
            break;
            }

        case MOwnPresenceEvent::KInterfaceId:
            {
            SafeAppendEventName( aTarget, _L8("[OwnPres] ") );
            break;
            }

        case MPresentityGroupListEvent::KInterfaceId:
            {
            SafeAppendEventName( aTarget, _L8("[GroupList] ") );
            break;
            }

        case MPresentityGroupContentEvent::KInterfaceId:
            {
            SafeAppendEventName( aTarget, _L8("[GroupContent] ") );
            break;
            }

        case MPresentityPresenceEvent::KInterfaceId:
            {
            SafeAppendEventName( aTarget, _L8("[Presentity] ") );
            break;
            }

        default:
            {
            SafeAppendEventName( aTarget, _L8("[Unknown] ") );
            break;
            }
        }
    }



void CXIMPTestStatusEventListener::SafeAppendEventName(
        TDes8& aTarget,
        const TDesC8& aName ) const
    {
    const TInt sizeAvailable = aTarget.MaxLength() - aTarget.Length();

    if( sizeAvailable > aName.Size() )
        {
        aTarget += aName;
        }
    else
        {
        _LIT8( KEllipsis, "..." );
        if( sizeAvailable > KEllipsis().Size() )
            {
            aTarget += KEllipsis;
            }
        }
    }




// end of file


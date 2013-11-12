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
* Description:  Event capsulation for shared ownership.
*
*/

#include "ximpeventcapsule.h"
#include "ximpeventcodec.h"
#include "ximpapieventbase.h"
#include "ximprbufhelpers.h"
#include <ximprequestcompleteevent.h>


// ---------------------------------------------------------------------------
// CXIMPEventCapsule::NewL()
// ---------------------------------------------------------------------------
//
EXPORT_C CXIMPEventCapsule* CXIMPEventCapsule::NewL( CXIMPApiEventBase& aEvent,
                                                     TBool aEventReady /* = ETrue */ )
    {
    CXIMPEventCapsule* self = new( ELeave ) CXIMPEventCapsule( aEventReady );
    CleanupClosePushL( *self );
    self->ConstructL( aEvent );
    CleanupStack::Pop( self );
    return self;
    }


// ---------------------------------------------------------------------------
// CXIMPEventCapsule::~CXIMPEventCapsule
// ---------------------------------------------------------------------------
//
CXIMPEventCapsule::~CXIMPEventCapsule()
    {
    iEventBuf.Close();
    delete iBuffer;
    }


// ---------------------------------------------------------------------------
// CXIMPEventCapsule::ConstructL()
// ---------------------------------------------------------------------------
//
void CXIMPEventCapsule::ConstructL( CXIMPApiEventBase& aEvent )
    {
    iReqId = TXIMPRequestId::Null();
    
    iBuffer = CBufFlat::NewL( 10 ); // initial granularity to 10
    XIMPEventCodec::PackL( aEvent, iEventType, iBuffer );
    iEventBuf.CreateL( iBuffer->Ptr( 0 ) );
    
    InitReqId( aEvent );
    }

// ---------------------------------------------------------------------------
// CXIMPEventCapsule::CXIMPEventCapsule()
// ---------------------------------------------------------------------------
//
CXIMPEventCapsule::CXIMPEventCapsule( TBool aEventReady )
: iEventReady( aEventReady )
    {
    }


// ---------------------------------------------------------------------------
// CXIMPEventCapsule::EventData()
// ---------------------------------------------------------------------------
//
EXPORT_C const TDesC8& CXIMPEventCapsule::EventData() const
    {
    return iEventBuf;
    }


// ---------------------------------------------------------------------------
// CXIMPEventCapsule::EventData()
// ---------------------------------------------------------------------------
//
EXPORT_C TXIMPRequestId CXIMPEventCapsule::EventReqId() const
    {
    return iReqId;
    }


// ---------------------------------------------------------------------------
// CXIMPEventCapsule::EventType()
// ---------------------------------------------------------------------------
//
EXPORT_C TInt32 CXIMPEventCapsule::EventType() const
    {
    return iEventType;
    }

    
// ---------------------------------------------------------------------------
// CXIMPEventCapsule::Ready()
// ---------------------------------------------------------------------------
//
EXPORT_C TBool CXIMPEventCapsule::Ready() const
    {
    return iEventReady;
    }

// ---------------------------------------------------------------------------
// CXIMPEventCapsule::UpdateEventDataL()
// ---------------------------------------------------------------------------
//
EXPORT_C void CXIMPEventCapsule::UpdateEventDataL( CXIMPApiEventBase& aEvent )
    {
    XIMPEventCodec::PackL( aEvent, iEventType, iBuffer );
    XIMPRBuf8Helper::SetNewValueL( iEventBuf, iBuffer->Ptr( 0 ) );
    
    InitReqId( aEvent );
    }

// ---------------------------------------------------------------------------
// CXIMPEventCapsule::UpdateEventData()
// ---------------------------------------------------------------------------
//
EXPORT_C void CXIMPEventCapsule::UpdateEventData( CXIMPApiEventBase& aEvent )
    { 
    // If update fails old data is used.
    TRAP_IGNORE(
        UpdateEventDataL( aEvent );
        )
    }

// ---------------------------------------------------------------------------
// CXIMPEventCapsule::SetReady()
// ---------------------------------------------------------------------------
//
EXPORT_C void CXIMPEventCapsule::SetReady( TBool aReady )
    {
    iEventReady = aReady;
    }

// ---------------------------------------------------------------------------
// CXIMPEventCapsule::operator<()
// ---------------------------------------------------------------------------
//
TBool CXIMPEventCapsule::operator<( const CXIMPEventCapsule& aOther ) const
    {
    return iTimeStamp < aOther.iTimeStamp;
    }
    
// ---------------------------------------------------------------------------
// CXIMPEventCapsule::SetTimeStamp()
// ---------------------------------------------------------------------------
//
void CXIMPEventCapsule::SetTimeStamp( TUint32 aTimeStamp )
    {
    iTimeStamp = aTimeStamp;
    }
    
    
// ---------------------------------------------------------------------------
// CXIMPEventCapsule::InitReqId()
// ---------------------------------------------------------------------------
//
void CXIMPEventCapsule::InitReqId( const CXIMPApiEventBase& aEvent )
    {
    if( aEvent.Base().GetInterfaceId() == MXIMPRequestCompleteEvent::KInterfaceId )
        {
        const MXIMPRequestCompleteEvent* complEvent =
            TXIMPGetInterface< const MXIMPRequestCompleteEvent >::From( aEvent.Base(),
                                                                        MXIMPBase::EPanicIfUnknown );
        
        iReqId = complEvent->RequestId();
        }
    }
    
// End of file

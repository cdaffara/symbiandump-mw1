/*
* Copyright (c) 2006-2007 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:    Connection multiplexer stream abstraction.
*
*/





#include "cncmstream.h"
#include "ncmconnectionmultiplexerlogs.h"
#include "ncmconnectionmultiplexerassert.h"
#include "natfwmediawrapper.h"
#include "cncmconnectionobserverhandler.h"
#include "cncmconnection.h"

const TUint KFirstConnectionId = 1;

// ---------------------------------------------------------------------------
// CNcmStream::CNcmStream
// ---------------------------------------------------------------------------
//
CNcmStream::CNcmStream( TUint aSessionId,
    TUint aStreamId, TInt aQos, TUint aProtocol ):
    iSessionId( aSessionId ), iStreamId( aStreamId ), iQos( aQos ),
    iStreamProtocol( aProtocol ),
    iNextConnectionId( KFirstConnectionId ),
    iMediaConnectionId( 0 )
    {
    }


// ---------------------------------------------------------------------------
// CNcmStream::NewL
// ---------------------------------------------------------------------------
//
CNcmStream* CNcmStream::NewL( TUint aSessionId,
    TUint aStreamId, TInt aQos, TUint aProtocol )
    {
    __CONNECTIONMULTIPLEXER( "CNcmStream::NewL" )
    
    CNcmStream* self =
        CNcmStream::NewLC( aSessionId, aStreamId, aQos, aProtocol );
            
    CleanupStack::Pop( self );
    return self;
    }


// ---------------------------------------------------------------------------
// CNcmStream::NewLC
// ---------------------------------------------------------------------------
//
CNcmStream* CNcmStream::NewLC( TUint aSessionId,
    TUint aStreamId, TInt aQos, TUint aProtocol )
    {
    __CONNECTIONMULTIPLEXER( "CNcmStream::NewLC" )
    
    CNcmStream* self =
        new( ELeave ) CNcmStream( aSessionId, aStreamId, aQos, aProtocol );
        
    CleanupStack::PushL( self );
    self->ConstructL();
    return self;
    }

// ---------------------------------------------------------------------------
// CNcmStream::ConstructL
// ---------------------------------------------------------------------------
//
void CNcmStream::ConstructL()
    {
    }
    
// ---------------------------------------------------------------------------
// CNcmStream::~CNcmStream
// ---------------------------------------------------------------------------
//
CNcmStream::~CNcmStream()
    {
    __CONNECTIONMULTIPLEXER( 
        "CNcmStream::~CNcmStream" )
    /*    
    TInt ind( iConnections.Count() );
    
    while ( ind-- )  
        {
        iConnectionObserverHandler->ConnectionNotify( iStreamId,
                                          iConnections[ind]->ConnectionId(),
                                          EMultiplexerConnectionRemoved,
                                          KErrNone );
                                          
        delete iConnections[ind];  
        iConnections.Remove( ind );
        }
    
    iConnections.Close();
    */
    if ( iWrapper )
        {
        iWrapper->Close();  
        }
   
    iWrapper = NULL;     
    //delete iConnectionObserverHandler;
    }
    
// ---------------------------------------------------------------------------
// CNcmStream::CreateConnectionL
// ---------------------------------------------------------------------------
//
TUint CNcmStream::CreateConnectionL(
    MNcmMultiplexerConnectionObserver& /*aObserver*/,
    CNcmConnectionObserverHandler& /*aConnectionObserverHandler*/,
    RSocketServ& /*aSocketServ*/,
    RConnection& /*aConnection*/,
    const TInetAddr& /*aLocalAddress*/,
    TBool /*aReuseAddress*/ )
    {   
    return 0;
    }

// ---------------------------------------------------------------------------
// CNcmStream::RemoveConnectionL
// ---------------------------------------------------------------------------
//
void CNcmStream::RemoveConnectionL( TUint aConnectionId )
    {
    __CONNECTIONMULTIPLEXER_INT1(
        "CNcmStream::Remove - RemoveConnectionL: ", aConnectionId )
    /*
    TInt count( iConnections.Count() );
    TBool found( EFalse );
        
    for ( TInt i = 0; i < count; i++ )
        {
        if ( iConnections[i]->ConnectionId() == aConnectionId )
            {      
            delete iConnections[i];  
            iConnections.Remove( i );
            found = ETrue;
            break;
            }
        }
       
    if ( !found )
        {
        User::Leave( KErrNotFound );
        }
     */
    }    
// ---------------------------------------------------------------------------
// CNcmStream::StreamId
// ---------------------------------------------------------------------------
//
TUint CNcmStream::StreamId() const
    {
    return iStreamId;
    }


// ---------------------------------------------------------------------------
// CNcmStream::Qos
// ---------------------------------------------------------------------------
//
TInt CNcmStream::Qos() const
    {
    return iQos;
    }


// ---------------------------------------------------------------------------
// CNcmStream::SetQos
// ---------------------------------------------------------------------------
//
void CNcmStream::SetQos( TInt aQos )
    {
    __CONNECTIONMULTIPLEXER_INT1( "CNcmStream::SetQos - aQos: ", aQos )
    
    iQos = aQos;
    }

// ---------------------------------------------------------------------------
// CNcmStream::ConnectionL
// ---------------------------------------------------------------------------
//
CNcmConnection* CNcmStream::ConnectionL(
    TUint aConnectionId )
    {
    
    __CONNECTIONMULTIPLEXER_INT1(
        "CNcmStream::ConnectionL - aConnectionId: ", aConnectionId )
    /*
    TInt count( iConnections.Count() );
    
    for ( TInt i = 0; i < count; i++ )
        {
        if ( iConnections[i]->ConnectionId() == aConnectionId )
            {
            return iConnections[i];
            }
        }
    User::Leave( KErrNotFound );    
#ifndef _DEBUG_EUNIT
    return NULL;
#endif
    */
    return NULL;
    }


// ---------------------------------------------------------------------------
// CNcmStream::RegisterWrapperL
// ---------------------------------------------------------------------------
//
void CNcmStream::RegisterWrapperL(
    MNATFWMediaWrapper* aWrapper )
    {
    __CONNECTIONMULTIPLEXER( 
        "CNcmStream::RegisterWrapperL" )
    __CONNECTIONMULTIPLEXER_ASSERT_L( NULL != aWrapper, KErrArgument );
    __CONNECTIONMULTIPLEXER_ASSERT_L( NULL == iWrapper, KErrAlreadyExists );
        
    iWrapper = aWrapper;
    }


// ---------------------------------------------------------------------------
// CNcmStream::WrapperL
// ---------------------------------------------------------------------------
//
MNATFWMediaWrapper* CNcmStream::WrapperL()
    {
    __CONNECTIONMULTIPLEXER( "CNcmStream::WrapperL" )
    __CONNECTIONMULTIPLEXER_ASSERT_L( NULL != iWrapper, KErrNotReady );

    return iWrapper;
    }


// ---------------------------------------------------------------------------
// CNcmStream::StreamProtocol
// ---------------------------------------------------------------------------
//
TUint CNcmStream::StreamProtocol() const
    {
    __CONNECTIONMULTIPLEXER( 
        "CNcmStream::StreamProtocol" )

    return iStreamProtocol;
    }


// ---------------------------------------------------------------------------
// CNcmStream::SetStreamProtocol
// ---------------------------------------------------------------------------
//
void CNcmStream::SetStreamProtocol(
    TUint aProtocol )
    {
    __CONNECTIONMULTIPLEXER( 
        "CNcmStream::SetStreamProtocol" )
        
    iStreamProtocol = aProtocol;
    }

// ---------------------------------------------------------------------------
// CNcmStream::MediaConnectionId
// ---------------------------------------------------------------------------
//
TUint CNcmStream::MediaConnectionId() const
    {
    __CONNECTIONMULTIPLEXER( 
        "CNcmStream::MediaConnectionId ")
        
    return iMediaConnectionId;
    }
    
// ---------------------------------------------------------------------------
// CNcmStream::SetMediaConnectionId
// ---------------------------------------------------------------------------
//
void CNcmStream::SetMediaConnectionId( TUint aMediaConnectionId )
    {
    __CONNECTIONMULTIPLEXER( 
        "CNcmStream::SetMediaConnectionId ")
        
    iMediaConnectionId = aMediaConnectionId;
    }


// -----------------------------------------------------------------------------
// CNcmStream::ConnectionObserverHandler
// -----------------------------------------------------------------------------
//    
CNcmConnectionObserverHandler&
    CNcmStream::ConnectionObserverHandler()   
    {
    return *iConnectionObserverHandler;
    }

// ---------------------------------------------------------------------------
// CNcmStream::ConnectionIdByDestinationAddressL(
// ---------------------------------------------------------------------------
//
CNcmConnection*
    CNcmStream::ConnectionByDestinationAddressL(
    const TInetAddr& /*aDestinationAddress*/ )
    {
    /*    
    TInt ind( iConnections.Count() );
    
    while ( ind-- )
        {
        if ( iConnections[ind]->Destination()->
            CheckSenderValidityToSending( aDestinationAddress ) )
            {
            return iConnections[ind];
            }
        }
    
    User::Leave( KErrNotFound );   
#ifndef _DEBUG_EUNIT
    return NULL;
#endif
    */
    return NULL;
    }   

// ---------------------------------------------------------------------------
// CNcmStream::RegisterMediaSourceL
// ---------------------------------------------------------------------------
//
void CNcmStream::RegisterMediaSourceL( CNcmMediaSource* /*aMediaSource*/ )
    {
    //iConnectionObserverHandler->RegisterMediaSourceL( aMediaSource );
    }

/*
// ---------------------------------------------------------------------------
// CNcmStream::SetIcmpErrorReceived
// ---------------------------------------------------------------------------
//
void CNcmStream::SetIcmpErrorReceived( TBool aIcmpErrorReceived )
    {
    iIcmpErrorReceived = aIcmpErrorReceived; 
    }
    
*/
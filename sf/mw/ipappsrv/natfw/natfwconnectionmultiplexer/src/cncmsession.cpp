/*
* Copyright (c) 2007-2008 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:    Connection multiplexer session abstraction.
*
*/




#include <e32math.h> 
#include <es_sock.h>
#include <es_sock_partner.h>

#include "cncmsession.h"
#include "cncmstream.h"
#include "cncmconnection.h"
#include "cncmconnectionobserverhandler.h"
#include "cncmlocaladdressresolver.h"
#include "ncmconnectionmultiplexerlogs.h"
#include "cncmportstore.h"
#include "mncmsessionobserver.h"
#include "mncmconnectionmultiplexerobserver.h"
#include "natfwmediawrapper.h"
#include "cncmicmpv4receiver.h"
#include "cncmicmpv6receiver.h"

const TUint KESockMessageSlots = 24;

// ---------------------------------------------------------------------------
// CNcmSessionSession::CNcmSession
// ---------------------------------------------------------------------------
//
CNcmSession::CNcmSession( TUint aSessionId,
    TUint32 aIapId,
    MNcmConnectionMultiplexerObserver& aObserver,
    MNcmSessionObserver& aSessionObserver ) :
    CActive( EPriorityStandard ),
    iSessionId( aSessionId ), iIapId( aIapId ), iObserver( aObserver ),
    iSessionObserver( aSessionObserver )
    {
    CActiveScheduler::Add( this );
    }


// ---------------------------------------------------------------------------
// CNcmSession::ConstructL
// ---------------------------------------------------------------------------
//
void CNcmSession::ConstructL( TUint aPortRangeStart, TUint aPortRangeStop )
    {
    iPortStore = CNcmPortStore::NewL( aPortRangeStart, aPortRangeStop );

    User::LeaveIfError( iSocketServ.Connect( KESockMessageSlots ) );

    iConnPref.SetDialogPreference( ECommDbDialogPrefDoNotPrompt );
    iConnPref.SetIapId( iIapId );  
                                                 
    User::LeaveIfError( iConnection.Open( iSocketServ ) );
    
    TSecurityPolicy policy( ECapabilityNetworkServices );
    TSecurityPolicyBuf securityBuf(policy);
    User::LeaveIfError( iConnection.Control( 
        KCOLConnection, KCoEnableCloneOpen, securityBuf ) );

    iConnection.Start( iConnPref, iStatus );
    SetActive();                                  
    }


// ---------------------------------------------------------------------------
// CNcmSession::NewL
// ---------------------------------------------------------------------------
//
CNcmSession* CNcmSession::NewL( TUint aSessionId,
    TUint32 aIapId, TUint aPortRangeStart, TUint aPortRangeStop,
    MNcmConnectionMultiplexerObserver& aObserver,
    MNcmSessionObserver& aSessionObserver )
    {   
    CNcmSession* self =
        CNcmSession::NewLC( aSessionId, aIapId, aPortRangeStart,
            aPortRangeStop, aObserver, aSessionObserver );
            
    CleanupStack::Pop( self );
    return self;
    }


// ---------------------------------------------------------------------------
// CNcmSession::NewLC
// ---------------------------------------------------------------------------
//
CNcmSession* CNcmSession::NewLC( TUint aSessionId,
    TUint32 aIapId, TUint aPortRangeStart, TUint aPortRangeStop,
    MNcmConnectionMultiplexerObserver& aObserver,
    MNcmSessionObserver& aSessionObserver )
    {   
    CNcmSession* self =
        new( ELeave ) CNcmSession( aSessionId, aIapId,
            aObserver, aSessionObserver );
                    
    CleanupStack::PushL( self );
    self->ConstructL( aPortRangeStart, aPortRangeStop );
    return self;
    }


// ---------------------------------------------------------------------------
// CNcmSession::~CNcmSession
// ---------------------------------------------------------------------------
//
CNcmSession::~CNcmSession()
    {
    delete iIcmpV4receiver;
    delete iIcmpV6receiver;
    delete iPortStore;
    
    TInt ind( iStreams.Count() );
    
    while ( ind-- )
        {
        TRAP_IGNORE( RemoveStreamL( iStreams[ind]->StreamId() ) )
        }   
    
    iConnection.Close();    
    Cancel();

    iSocketServ.Close();
    iStreams.Close();
    }


// ---------------------------------------------------------------------------
// CNcmSession::CreateStreamL
// ---------------------------------------------------------------------------
//
TUint CNcmSession::CreateStreamL( TInt aQos,
    TUint aProtocol )
    {
    TUint streamId( GenerateStreamId() );

    CNcmStream* item = CNcmStream::NewLC(
        iSessionId, streamId, aQos, aProtocol );

    iStreams.AppendL( item ); 
    CleanupStack::Pop( item );
    
    return item->StreamId();
    }


// ---------------------------------------------------------------------------
// CNcmSession::RemoveStreamL
// ---------------------------------------------------------------------------
//
void CNcmSession::RemoveStreamL( TUint aStreamId )
    {    
    TUint streamIndex( StreamIndexL( aStreamId ) );
    
    delete iStreams[streamIndex];
    iStreams.Remove( streamIndex );
    }
    

// ---------------------------------------------------------------------------
// CNcmSession::ConnectionId
// ---------------------------------------------------------------------------
//
TUint32 CNcmSession::IapId() const
    {
    return iIapId;
    }

    
// ---------------------------------------------------------------------------
// CNcmSession::ConnectionId
// ---------------------------------------------------------------------------
//
TUint CNcmSession::SessionId() const
    {
    return iSessionId;
    }


// ---------------------------------------------------------------------------
// From class CActive
//
// CNcmSession::RunL
// ---------------------------------------------------------------------------
//
void CNcmSession::RunL()
    {
    __CONNECTIONMULTIPLEXER_INT1(
        "CNcmSession::RunL - iStatus: ", iStatus.Int() )
                   
    if ( iStatus != KErrNone )
        {
        iObserver.Notify( iSessionId, 0,
            MNcmConnectionMultiplexerObserver::ESessionCreated,
            iStatus.Int() );
            
        iSessionObserver.SessionCreationFailed( iSessionId );
        }
    else
        {
        iInitialized = ETrue;

        CNcmLocalAddressResolver* localAddressResolver(
            CNcmLocalAddressResolver::NewLC( iSocketServ ) );
            
        localAddressResolver->GetLocalAddrL( iIpv4Addr, iIpv6Addr, iIapId );
        CleanupStack::PopAndDestroy( localAddressResolver );
        
        if ( !iIpv4Addr.IsUnspecified() )
            {
            iIcmpV4receiver =
                CNcmIcmpV4Receiver::NewL( iSocketServ, iConnection, *this );
            }

        if ( !iIpv6Addr.IsUnspecified() )
            {
            iIcmpV6receiver =
                CNcmIcmpV6Receiver::NewL( iSocketServ,iConnection, *this );
            }      
                
        iObserver.Notify( iSessionId, 0,
            MNcmConnectionMultiplexerObserver::ESessionCreated,
            iStatus.Int() );
        }
    }


// ---------------------------------------------------------------------------
// From class CActive
//
// DoCancel
// ---------------------------------------------------------------------------
//
void CNcmSession::DoCancel()
    {    
    // RConnection doesn't cancel an outstanding request at Close()
    // so we'll have to it "manually" here
    if( iStatus.Int() == KRequestPending )
        {
        TRequestStatus* status = &iStatus;
        User::RequestComplete( status, KErrCancel );
        }
    }


// -----------------------------------------------------------------------------
// From class CActive
//
// CNcmSession::RunError
// -----------------------------------------------------------------------------
//
TInt CNcmSession::RunError( TInt aError )
    {
    __CONNECTIONMULTIPLEXER_INT1(
        "CNcmSession::RunError aError: ", aError )
  
    iObserver.Notify( iSessionId, 0,
        MNcmConnectionMultiplexerObserver::ESessionCreated,
        aError );
        
    iSessionObserver.SessionCreationFailed( iSessionId );

    aError = aError;
    return KErrNone;
    }
    
        
// ---------------------------------------------------------------------------
// CNcmSession::StreamByIdL
// ---------------------------------------------------------------------------
//
CNcmStream* CNcmSession::StreamByIdL(
    TUint aStreamId )
    {
    TInt ind( iStreams.Count() );
    
    while ( ind-- )
        {
        if ( iStreams[ind]->StreamId() == aStreamId )
            {
            return iStreams[ind];
            }
        }
    
    return NULL;
    }

    
// ---------------------------------------------------------------------------
// CNcmSession::StreamIndexL
// ---------------------------------------------------------------------------
//
TUint CNcmSession::StreamIndexL( TUint aStreamId )
    {   
    TInt streamCount( iStreams.Count() );
    
    for ( TInt i = 0; i < streamCount; i++ )
        {
        if ( iStreams[i]->StreamId() == aStreamId )
            {
            return i;
            }
        }
    
    User::Leave( KErrNotFound );
    return KErrNone; // To remove a compiler warning
    }

    
// ---------------------------------------------------------------------------
// CNcmSession::SocketServer
// ---------------------------------------------------------------------------
//
RSocketServ& CNcmSession::SocketServer()
    {
    return iSocketServ;
    }


// ---------------------------------------------------------------------------
// CNcmSession::Connection
// ---------------------------------------------------------------------------
//
RConnection& CNcmSession::Connection()
    {  
    return iConnection;
    }

    
// ---------------------------------------------------------------------------
// CNcmSession::GetConnectionName
// ---------------------------------------------------------------------------
//
TInt CNcmSession::GetConnectionName( TName& aConnectionName )
    {  
    return iConnection.Name( aConnectionName );
    }


// -----------------------------------------------------------------------------
// CNcmSession::GenerateStreamId
// -----------------------------------------------------------------------------
//
TUint CNcmSession::GenerateStreamId() const
    {
    return ( static_cast<TUint>( Math::Random() ) );
    }

   
// ---------------------------------------------------------------------------
// CNcmSession::PortStore
// ---------------------------------------------------------------------------
//
CNcmPortStore& CNcmSession::PortStore() const
    {
    __CONNECTIONMULTIPLEXER( "CNcmSession::PortStore" )

    return *iPortStore;
    }


// ---------------------------------------------------------------------------
// From class MNcmIcmpObserver
//
// Called as a result for ICMP error received 
// ---------------------------------------------------------------------------
//        
void CNcmSession::IcmpError( const TInetAddr& aAddress,
    TInetAddr& aLocalAddress, TInetAddr& aRemoteAddress )
    {
    TRAP_IGNORE( this->HandleIcmpErrorL( aAddress, aLocalAddress,
        aRemoteAddress ) )
    }
    
// ---------------------------------------------------------------------------
// CNcmSession::HandleIcmpErrorL
// ---------------------------------------------------------------------------
void CNcmSession::HandleIcmpErrorL( const TInetAddr& aAddress,
    TInetAddr& aLocalAddress, TInetAddr& aRemoteAddress )
    {  
    __CONNECTIONMULTIPLEXER( "CNcmSession::HandleIcmpErrorL" )
    
    TInt ind( iStreams.Count() );
    TUint mediaConnectionId( 0 );
    
    while ( ind-- )
        { 
        mediaConnectionId = iStreams[ind]->MediaConnectionId();

        if ( iStreams[ind]->ConnectionL( mediaConnectionId )->
            CheckSenderValidityToSending( aAddress ) )
            {
            __CONNECTIONMULTIPLEXER(
                "CNcmSession::HandleIcmpErrorL - ICMP ERROR VALID" )
            
            iStreams[ind]->WrapperL()->GetAddresses(
                aLocalAddress, aRemoteAddress );
            return;
            }
        }
        
    __CONNECTIONMULTIPLEXER(
        "CNcmSession::HandleIcmpErrorL - ICMP ERROR NOT VALID" )
    User::Leave( KErrNotFound );
    }


// ---------------------------------------------------------------------------
// CNcmSession::Initialized
// ---------------------------------------------------------------------------
//
TBool CNcmSession::Initialized() const
    {
    return iInitialized;
    }

    
// ---------------------------------------------------------------------------
// CNcmSession::ResolveDestinationAddressL
// ---------------------------------------------------------------------------
//
void CNcmSession::ResolveDestinationAddressL( const TDesC8& aAddress,
    TUint aPort, TInetAddr& aResult )
    {
    __CONNECTIONMULTIPLEXER_STR(
        "CNcmSession::ResolveDestinationAddressL, FQDN", aAddress )

    HBufC* addrBuf = HBufC::NewLC( aAddress.Length() );
    TPtr addrPtr( addrBuf->Des() );
    User::LeaveIfError(
        CnvUtfConverter::ConvertToUnicodeFromUtf8( addrPtr, aAddress ) );

    RHostResolver resolver;
    CleanupClosePushL( resolver );
    User::LeaveIfError( resolver.Open(
        iSocketServ, KAfInet, KProtocolInetUdp, iConnection ) );
    TNameEntry entry;
    User::LeaveIfError( resolver.GetByName( *addrBuf, entry ) );

    CleanupStack::PopAndDestroy( 2, addrBuf );

    aResult = TInetAddr::Cast( entry().iAddr );
    aResult.SetPort( aPort );
    
    __CONNECTIONMULTIPLEXER_ADDRLOG(
        "CNcmSession::ResolveDestinationAddressL, IP", aResult )
    }
 
    
// ---------------------------------------------------------------------------
// CNcmSession::LocalIPv4Address
// ---------------------------------------------------------------------------
//
TInetAddr& CNcmSession::LocalIPv4Address()
    {
    return iIpv4Addr;
    }


// ---------------------------------------------------------------------------
// CNcmSession::LocalIPv6Address
// ---------------------------------------------------------------------------
//
TInetAddr& CNcmSession::LocalIPv6Address()
    {
    return iIpv6Addr;
    }

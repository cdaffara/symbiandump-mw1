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
* Description:  XIMP Framework PSC Server context session.
 *
*/

#include "ximpsrvcontextclient.h"
#include "ximpprocessstarter.h"
#include "ximpserverdefs.h"

#include <e32base.h>

//Import NXIMPSrv
using namespace NXIMPSrv;


// ======== MEMBER FUNCTIONS ========

// ---------------------------------------------------------------------------
// RXIMPSrvContextClient::RXIMPSrvContextClient()
// ---------------------------------------------------------------------------
//
EXPORT_C RXIMPSrvContextClient::RXIMPSrvContextClient()
    {
    }


// ---------------------------------------------------------------------------
// RXIMPSrvContextClient::Connect()
// ---------------------------------------------------------------------------
//
EXPORT_C void RXIMPSrvContextClient::ConnectL( TUid aProtocolImpUid )
    {
    iMyUid = aProtocolImpUid;

    // create the server name: "!XIMPCtxSrv9bde667a" 
    HBufC* serverName = HBufC::NewLC( NName::KSymbianServer().Length() + 
            NStartupParam::KProtocolUidMaxLen + 
            1 );

    serverName->Des().Copy( NName::KSymbianServer );
    serverName->Des().AppendNumFixedWidth( iMyUid.iUid, 
            EHex, 
            NStartupParam::KProtocolUidMaxLen );

    // Pass the UID to server side.
    // For security reasons it was not allowed to pass the complete 
    // server name descriptor but just the UID part
    RArray< TXIMPProcessStartupParam > params;
    CleanupClosePushL( params );

    TXIMPProcessStartupParam paramUid;
    paramUid.Set( NStartupParam::KProtocolUidIndex, 
            iMyUid.iUid );

    User::LeaveIfError( params.Append( paramUid ) );
    TArray< TXIMPProcessStartupParam > array = params.Array();


    //Get path to correct exe to launch
    TFileName fullExePath;
    XIMPProcessStarter::FullExePathForClienLocation( NName::KExecutable,
            fullExePath );

    //Create a connection
    TInt ret = XIMPProcessStarter::ConnectToServer( fullExePath,
            KNullDesC,
            &array,
            *this,
            *serverName,
            TVersion( NVersion::KMajor,
                      NVersion::KMinor,
                      NVersion::KBuild ),
                      NRequest::KMsgSlotCount );
    User::LeaveIfError( ret );
    CleanupStack::PopAndDestroy( 2, serverName ); // params, serverName

    //Mutate the server side session to correct type
    User::LeaveIfError( SendReceive( NRequest::EBecomeCtxSession ) );
    }



// ---------------------------------------------------------------------------
// RXIMPSrvContextClient::Close()
// ---------------------------------------------------------------------------
//
EXPORT_C void RXIMPSrvContextClient::Close()
    {
    RSessionBase::Close();
    #if _BullseyeCoverage
    cov_write();
    #endif
    }


// ---------------------------------------------------------------------------
// RXIMPSrvContextClient::DoQueueOperation()
// ---------------------------------------------------------------------------
//
EXPORT_C TInt RXIMPSrvContextClient::DoQueueOperation(
    TInt aOperationType, 
    const TDesC8& aOperationParams,
    TXIMPRequestId& aReqId )
    {
    TPckgBuf< TXIMPRequestId > reqIdPck;

    TIpcArgs msgArgs;
    msgArgs.Set( 0, aOperationType );
    msgArgs.Set( 1, &aOperationParams );
    msgArgs.Set( 2, &reqIdPck );

    TInt err = SendReceive( NRequest::ECtxSsQueueOperation, msgArgs );

    aReqId = reqIdPck();
    return err;
    }

// ---------------------------------------------------------------------------
// RXIMPSrvContextClient::DoQueueOperation2()
// ---------------------------------------------------------------------------
//
EXPORT_C TInt RXIMPSrvContextClient::DoQueueOperation2(
    TInt aOperationType, 
    const TDesC8& aOperationParams,
    TXIMPRequestId& aReqId )
    {
    TPckgBuf< TXIMPRequestId > reqIdPck;

    TIpcArgs msgArgs;
    msgArgs.Set( 0, aOperationType );
    msgArgs.Set( 1, &aOperationParams );
    msgArgs.Set( 2, &reqIdPck );

    TInt err = SendReceive( NRequest::ECtxSsQueueBindOperation, msgArgs );
    aReqId = reqIdPck();
    return err;
    }

// ---------------------------------------------------------------------------
// RXIMPSrvContextClient::GetContextCapabilitiesL()
// ---------------------------------------------------------------------------
//
EXPORT_C HBufC8* RXIMPSrvContextClient::GetContextFeaturesL() const
    {
    return PrepareAndGetDataL( NRequest::ECtxSsPrepareContextFeatures );
    }


// ---------------------------------------------------------------------------
// RXIMPSrvContextClient::ListenEvent()
// ---------------------------------------------------------------------------
//
EXPORT_C void RXIMPSrvContextClient::ListenEvent( TRequestStatus& aStatus )
    {
    SendReceive( NRequest::ECtxSsSetEventListenScout, aStatus );
    }


// ---------------------------------------------------------------------------
// RXIMPSrvContextClient::CancelListening()
// ---------------------------------------------------------------------------
//
EXPORT_C void RXIMPSrvContextClient::CancelListening()
    {
    SendReceive( NRequest::ECtxSsCancelEventListenScout );
    }


// ---------------------------------------------------------------------------
// RXIMPSrvContextClient::InstallEventFilterL()
// ---------------------------------------------------------------------------
//
EXPORT_C void RXIMPSrvContextClient::InstallEventFilterL(
        const TDesC8& aFilterData )
    {
    TIpcArgs msgArgs;
    msgArgs.Set( 0, &aFilterData );
    User::LeaveIfError( SendReceive( NRequest::ECtxSsInstallEventFilter, 
                                     msgArgs ) );
    }


// ---------------------------------------------------------------------------
// RXIMPSrvContextClient::FetchTopEventDataLC()
// ---------------------------------------------------------------------------
//
EXPORT_C HBufC8* RXIMPSrvContextClient::FetchTopEventDataLC()
    {
    TInt bufSize = SendReceive( NRequest::ECtxSsFetchTopEventDataSize );
    User::LeaveIfError( bufSize );

    HBufC8* eventBuf = HBufC8::NewLC( bufSize );
    TPtr8 ptrBuf( eventBuf->Des() );

    TIpcArgs msgArgs;
    msgArgs.Set( 0, &ptrBuf );

    User::LeaveIfError( SendReceive( NRequest::ECtxSsFetchTopEventData, 
                                     msgArgs ) );
    return eventBuf;
    }


// ---------------------------------------------------------------------------
// RXIMPSrvContextClient::FetchTopEventReqId()
// ---------------------------------------------------------------------------
//
EXPORT_C TInt RXIMPSrvContextClient::FetchTopEventReqId( 
    TXIMPRequestId& aReqId )
    {
    TPckgBuf< TXIMPRequestId > reqIdPck;

    TIpcArgs msgArgs;
    msgArgs.Set( 0, &reqIdPck );

    TInt err = SendReceive( NRequest::ECtxSsFetchTopEventReqId, msgArgs );

    aReqId = reqIdPck();
    return err;
    }



// ---------------------------------------------------------------------------
// RXIMPSrvContextClient::DropTopEvent()
// ---------------------------------------------------------------------------
//
EXPORT_C TInt RXIMPSrvContextClient::DropTopEvent()
    {
    TInt err = SendReceive( NRequest::ECtxSsDropTopEvent );
    return err;
    }



// ---------------------------------------------------------------------------
// RXIMPSrvContextClient::PrepareAndGetDataL()
// ---------------------------------------------------------------------------
//
HBufC8* RXIMPSrvContextClient::PrepareAndGetDataL( TInt aMsg ) const
    {
    TInt bufSize = SendReceive( aMsg );
    User::LeaveIfError( bufSize );

    HBufC8* buf = HBufC8::NewLC( bufSize );
    TPtr8 ptrBuf( buf->Des() );

    TIpcArgs msgArgs;
    msgArgs.Set( 0, &ptrBuf );

    User::LeaveIfError( SendReceive( NRequest::ECtxSsGetPreparedData, 
                                     msgArgs ) );

    CleanupStack::Pop( buf );
    return buf;
    }


// End of file



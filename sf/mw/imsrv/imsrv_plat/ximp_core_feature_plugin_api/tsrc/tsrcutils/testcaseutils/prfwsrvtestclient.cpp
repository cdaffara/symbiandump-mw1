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
* Description:  XIMP Framework server test client implementation
*
*/


//  INCLUDE FILES
#include "ximpbase.h"
#include "ximpsrvtestclient.h"
#include "ximpprocessstarter.h"
#include "ximprootserverdefs.h"
#include "ximpserverdefs.h"
#include "prfwtestserverdefs.h"

#include <e32base.h>
#include <s32mem.h>


// ======== MEMBER FUNCTIONS ========

// ---------------------------------------------------------------------------
// RXIMPSrvRootClient::RXIMPSrvRootClient()
// ---------------------------------------------------------------------------
//
RXIMPSrvTestClient::RXIMPSrvTestClient()
    {
    }


// ---------------------------------------------------------------------------
// RXIMPSrvRootClient::Connect()
// ---------------------------------------------------------------------------
//
TInt RXIMPSrvTestClient::Connect()
    {
    TFileName fullExePath;
    XIMPProcessStarter::FullExePathForClienLocation( NRootSrv::NName::KExecutable,
                                                     fullExePath );

    TInt ret = XIMPProcessStarter::ConnectToServer( fullExePath,
                                            KNullDesC,
                                            NULL,
                                            *this,
                                            NRootSrv::NName::KSymbianServer,
                                            TVersion( NRootSrv::NVersion::KMajor,
                                                      NRootSrv::NVersion::KMinor,
                                                      NRootSrv::NVersion::KBuild ),
                                                      NRootSrv::NRequest::KMsgSlotCount );

    if ( ret == KErrNone )
        {
        TIpcArgs msgArgs;
        ret = SendReceive( NTestSrv::NRequest::EInstantiateTestSession, msgArgs );
        }

    return ret;
    }

// ---------------------------------------------------------------------------
// RXIMPSrvContextClient::Connect()
// ---------------------------------------------------------------------------
//
void RXIMPSrvTestClient::ConnectL( TUid aProtocolImpUid )
    {
    // create the server name: "!XIMPCtxSrv9bde667a" 
    HBufC* serverName = HBufC::NewLC( NXIMPSrv::NName::KSymbianServer().Length() + 
                                      NXIMPSrv::NStartupParam::KProtocolUidMaxLen + 
                                      1 );

    serverName->Des().Copy( NXIMPSrv::NName::KSymbianServer );
    serverName->Des().AppendNumFixedWidth( aProtocolImpUid.iUid, 
                                           EHex, 
                                           NXIMPSrv::NStartupParam::KProtocolUidMaxLen );

    // Pass the UID to server side.
    // For security reasons it was not allowed to pass the complete 
    // server name descriptor but just the UID part
    RArray< TXIMPProcessStartupParam > params;
    CleanupClosePushL( params );

    TXIMPProcessStartupParam paramUid;
    paramUid.Set( NXIMPSrv::NStartupParam::KProtocolUidIndex, 
                  aProtocolImpUid.iUid );

    User::LeaveIfError( params.Append( paramUid ) );
    TArray< TXIMPProcessStartupParam > array = params.Array();


    //Get path to correct exe to launch
    TFileName fullExePath;
    XIMPProcessStarter::FullExePathForClienLocation( NXIMPSrv::NName::KExecutable,
                                                     fullExePath );

    //Create a connection
    TInt ret = XIMPProcessStarter::ConnectToServer( fullExePath,
                                                    KNullDesC,
                                                    &array,
                                                    *this,
                                                    *serverName,
                                                    TVersion( NXIMPSrv::NVersion::KMajor,
                                                              NXIMPSrv::NVersion::KMinor,
                                                              NXIMPSrv::NVersion::KBuild ),
                                                    NXIMPSrv::NRequest::KMsgSlotCount );
    User::LeaveIfError( ret );
    CleanupStack::PopAndDestroy( 2, serverName ); // params, serverName

    //Mutate the server side session to correct type
    ret = SendReceive( NTestSrv::NRequest::EInstantiateTestSession );
    User::LeaveIfError( ret );
    }

// ---------------------------------------------------------------------------
// RXIMPSrvRootClient::Close()
// ---------------------------------------------------------------------------
//
void RXIMPSrvTestClient::Close()
    {
    RSessionBase::Close();
    }


// ---------------------------------------------------------------------------
// RXIMPSrvTestClient::SetAllocFailureL()
// ---------------------------------------------------------------------------
//
void RXIMPSrvTestClient::SetAllocFailure( TInt aRate )
    {
    TPckgBuf< TInt > rateBuf( aRate );
    TIpcArgs msgArgs;
    msgArgs.Set( 0, &rateBuf );
    
    SendReceive( NTestSrv::NRequest::ESetAllocFail, msgArgs );
    }
    
// ---------------------------------------------------------------------------
// RXIMPSrvRootClient::GetLastError()
// ---------------------------------------------------------------------------
//
NTestSrv::SErrorData RXIMPSrvTestClient::FetchErrorData()
    {
    NTestSrv::SErrorData errorData;
    TPckgBuf< NTestSrv::SErrorData > errorBuf( errorData );
    TIpcArgs msgArgs;
    msgArgs.Set( 0, &errorBuf );
    TInt retVal = SendReceive( NTestSrv::NRequest::EPrepareErrorData, msgArgs );
    __ASSERT_ALWAYS( retVal >= 0, User::Panic( _L( "SendReceive failed" ), retVal ) );
    errorData = errorBuf();
    return errorData;
//    return GetData( NTestSrv::NRequest::EPrepareErrorData );
    }    

// ---------------------------------------------------------------------------
// RXIMPSrvRootClient::GetDataL()
// ---------------------------------------------------------------------------
//
NTestSrv::SErrorData RXIMPSrvTestClient::GetData( TInt aMsg )
    {
/*    TInt bufSize = SendReceive( aMsg );
    __ASSERT_ALWAYS( bufSize == NTestSrv::KErrorDataSize, User::Panic( _L( "Wrong bufsize" ), KErrArgument ) );

    NTestSrv::SErrorData errorData;
    TPckgBuf< NTestSrv::SErrorData > errorBuf( errorData );
    TIpcArgs msgArgs;
    msgArgs.Set( 0, &errorBuf );
    __ASSERT_ALWAYS( SendReceive( NTestSrv::NRequest::EFetchData, msgArgs ) >= 0, 
                    User::Panic( _L( "SendReceive failed" ) ) );
    return errorData;*/
    }


// End of file


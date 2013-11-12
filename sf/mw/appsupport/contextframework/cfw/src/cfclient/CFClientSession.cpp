/*
* Copyright (c) 2002-2007 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  RCFClientSession class implementation.
*
*/


#include "CFClientSession.h"

#include <e32svr.h>
#include <e32math.h>
#include <s32mem.h>
#include <f32file.h>
#include <centralrepository.h>
#include <cfcontextdataobject.h>

#include "cfcontextsubscriptionlistener.h"
#include "CFContextSubscriptionImpl.h"
#include "CFActionSubscriptionImpl.h"
#include "CFContextQueryImpl.h"
#include "cfcommon.h"
#include "CFContextObjectImpl.h"
#include "cftrace.h"
#include "ContextFrameworkPrivateCRKeys.h"

// CONSTANTS

//Fix for BHLS-7UPEA8
static const TUint KDefaultMessageSlots = 5;
static const TInt KCreateSessionRetryCount = 10;
static const TInt KCreateSessionDelay = 100000; // 0.1 seconds

RCFClientSession::RCFClientSession()
    {
    FUNC_LOG;
    }

// METHODS

//----------------------------------------------------------------------------
// RCFClientSession::ConnectL
//----------------------------------------------------------------------------
//
void RCFClientSession::ConnectL()
    {
    FUNC_LOG;

    TInt err = KErrNone;
    
#ifdef CFW_SERVER_START_THROUGH_CLIENT
    err = StartServer();
    ERROR_1( err, "CF server start failed with code: %d", err );
    User::LeaveIfError( err );
#endif
    
    RThread client;
    CleanupClosePushL( client );
    INFO_1( "Client [%x] session connecting to Context FW...",
        client.SecureId().iId );
    TInt retry = 0;
    do
        {
        err = CreateSession( KContextServerName,
            Version(),
            KDefaultMessageSlots );
        retry++;

        ERROR_2( err, "CF server session creation failed with code: %d. Retry count: %d", err, retry );
        if( err != KErrNone && retry < KCreateSessionRetryCount )
            {
            // Wait for a while so that CFW server is started
            User::After( KCreateSessionDelay );
            }
        }
    while( err != KErrNone && retry < KCreateSessionRetryCount );

    ERROR_1( err, "CF server session creation failed with code: %d", err );
    User::LeaveIfError( err );
    
    INFO_1( "Client [%x] session connecting to Context FW... OK",
        client.SecureId().iId );
    CleanupStack::PopAndDestroy( &client );
    }

//----------------------------------------------------------------------------
// RCFClientSession::Version
//----------------------------------------------------------------------------
//
TVersion RCFClientSession::Version() const
    {
    FUNC_LOG;
    
    return( TVersion( KContextServMajorVersionNumber,
        KContextServMinorVersionNumber,
        KContextServBuildVersionNumber ) );
    }

//----------------------------------------------------------------------------
// RCFClientSession::SendSync
//----------------------------------------------------------------------------
//
TInt RCFClientSession::SendSync( TInt aFunction ) const
    {
    FUNC_LOG;
    
    INFO_1( "SendSync: OpCode [%d]", aFunction );
    
    return SendReceive( aFunction );
    }

//----------------------------------------------------------------------------
// RCFClientSession::SendSync
//----------------------------------------------------------------------------
//
TInt RCFClientSession::SendSync( TInt aFunction, const TIpcArgs& aArgs ) const
    {
    FUNC_LOG;
    
    INFO_1( "SendSync: OpCode [%d]", aFunction );
    
    return SendReceive( aFunction, aArgs );
    }

//----------------------------------------------------------------------------
// RCFClientSession::SendAsync
//----------------------------------------------------------------------------
//
void RCFClientSession::SendAsync( TInt aFunction,
    const TIpcArgs& aArgs,
    TRequestStatus& aStatus ) const
    {
    FUNC_LOG;
    
    INFO_1( "SendAsync: OpCode [%d]", aFunction );
    
    SendReceive( aFunction, aArgs, aStatus );
    }

//----------------------------------------------------------------------------
// RCFClientSession::StartServer
//----------------------------------------------------------------------------
//
TInt RCFClientSession::StartServer()
    {
    FUNC_LOG;
    
    TInt result;
    TFindServer findContextServer( KContextServerName );
    TFullName name;
    result = findContextServer.Next( name );
    if( result == KErrNone )
        {
        // Server already running
        INFO( "CF server already running" );
        return KErrNone;
        }
    
    // Create process for server and start the process
    RProcess server;
    result = server.Create( KContextServerName, KNullDesC );
    if( result != KErrNone )
        {
        return result;
        }
    INFO_1( "CF server process created with code: %d", result );
        
    TRequestStatus status;
    server.Rendezvous( status );
    server.Resume();
    
    // Wait for rendesvouz
    User::WaitForRequest( status );
    INFO_1( "CF server rendezvous: %d", status.Int() );
    
    // Close process handle since it is not needed anymore
    server.Close();
    
    return status.Int();
    }

// End of file

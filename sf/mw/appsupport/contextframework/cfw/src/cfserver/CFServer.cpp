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
* Description:  CFServer class implementation.
*
*/


#include <e32svr.h>
#include <ecom/ecom.h>

#include "CFServer.h"
#include "CFEngine.h"
#include "cftrace.h"
#include "cfutils.h"
#include "cfcommon.h"

// CONSTANTS

/**
* Enable file trace macros
*/
#ifdef TRACE_INTO_FILE

#include <f32file.h>

    /**
    * Enables logging by creating the log folder.
    */
    LOCAL_C void EnableLog()
        {
        RFs fs;
        if( fs.Connect() == KErrNone )
            {
            fs.MkDirAll( KFullPath );
            fs.Close();
            }
        }
    #define ENABLE_LOG\
        {\
        EnableLog();\
        }
        
#else//TRACE_INTO_FILE not defined

    #define ENABLE_LOG

#endif//TRACE_INTO_FILE
 
// MEMBER FUNCTIONS

CCFServer* CCFServer::NewL()
    {
    FUNC_LOG;
    
    CCFServer* self = CCFServer::NewLC();
    CleanupStack::Pop( self );
    
    return self;
    }

CCFServer* CCFServer::NewLC()
    {
    FUNC_LOG;
    
    CCFServer* self = new( ELeave ) CCFServer( EPriorityNormal );
    CleanupStack::PushL( self );
    self->ConstructL();
    
    return self;
    }

CCFServer::~CCFServer()
    {
    FUNC_LOG;
    
    delete iCFEngine;
    
    // Call this from here to ease testing of other components
    REComSession::FinalClose();
    }

void CCFServer::ConstructL()
    {
    FUNC_LOG;
    
    // Rename cfw thread
    User::RenameThread( KContextServerName );
    
    iCFEngine = CCFEngine::NewL( *this );
    
    // Start server
    StartL( KContextServerName );
    }

CCFServer::CCFServer( TInt aPriority ):
    CServer2( aPriority )
    {
    FUNC_LOG;
    }

// METHODS

//----------------------------------------------------------------------------
// CCFServer::NewSessionL
//----------------------------------------------------------------------------
//
CSession2* CCFServer::NewSessionL(
    const TVersion &aVersion,
    const RMessage2& /*aMessage*/) const
    {
    FUNC_LOG;
    
    // check we're the right version
    TVersion version( KContextServMajorVersionNumber,
        KContextServMinorVersionNumber,
        KContextServBuildVersionNumber);
        
    if (!User::QueryVersionSupported( version, aVersion ) )
        {
        User::Leave(KErrNotSupported);
        }
    
    CSession2* session = CCFServSession::NewL( 
    	( MCFExtendedContextInterface& )*iCFEngine,
        ( MCFActionInterface& )*iCFEngine,
        ( MCFScriptInterface& )*iCFEngine );
    
    // make new session
    return session;
    }

//----------------------------------------------------------------------------
// CCFServer::NewSessionL
//----------------------------------------------------------------------------
//
TBool CCFServer::CheckClientSecurity( const RThread& aClientThread,
	const TSecurityPolicy& aSecurityPolicy )
	{
    FUNC_LOG;
    
    // Security check only needed if request is from other process than CF
    TBool securityPassed = ETrue;
    if( !CFUtils::RequestFromSameProcess( aClientThread ) )
        {
        securityPassed = aSecurityPolicy.CheckPolicy( aClientThread );
        }

    return securityPassed;
	}
        
//----------------------------------------------------------------------------
// CCFServer::PanicServer
//----------------------------------------------------------------------------
//
void CCFServer::PanicServer( TContextServPanic aPanic )
    {
    FUNC_LOG;
    
    User::Panic( KContextServerName, aPanic );
    }

//----------------------------------------------------------------------------
// CCFServer::ThreadFunctionL
//----------------------------------------------------------------------------
//
void CCFServer::ThreadFunctionL( TAny* /*aThreadParms*/ )
    {
    FUNC_LOG;
    
    // Create scheduler
    CActiveScheduler* activeScheduler = new( ELeave ) CActiveScheduler;
    CleanupStack::PushL( activeScheduler );
    CActiveScheduler::Install( activeScheduler );

    // Create Context Framework Server and signal client that we are ready
    CCFServer* server = CCFServer::NewLC();
    RProcess::Rendezvous( KErrNone );
    
    TIMESTAMP( "CF server active scheduler start" );

    // Start scheduler
    CActiveScheduler::Start();

    TIMESTAMP( "CF server process exit" );
    
    // Clean up
    CleanupStack::PopAndDestroy( server );
    CleanupStack::PopAndDestroy( activeScheduler );
    }

//----------------------------------------------------------------------------
// CCFServer::ThreadFunction
//----------------------------------------------------------------------------
//
TInt CCFServer::ThreadFunction( TAny* aThreadParms )
    {
    FUNC_LOG;
    
    CTrapCleanup* cleanupStack = CTrapCleanup::New();
    if( cleanupStack )
        {
        TRAPD( err, ThreadFunctionL( aThreadParms ) );
        if( err != KErrNone )
            {
            // Signal client that server creation failed
            RProcess::Rendezvous( err );
            }
        }
        
    // Clean up        
    delete cleanupStack;
    cleanupStack = NULL;

    return KErrNone;
    }

// Entry point
GLDEF_C TInt E32Main()
	{
	ENABLE_LOG;

    HEAP( "[START] CF server process entry" );
    TIMESTAMP( "CF server process entry" );
    
	return CCFServer::ThreadFunction( NULL );
	}

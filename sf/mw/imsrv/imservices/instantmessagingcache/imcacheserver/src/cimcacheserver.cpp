/*
* Copyright (c) 2008 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  instantmessagingcache server class implementation
*
*/

#define __INCLUDE_CAPABILITY_NAMES__
#define __INCLUDE_ALL_SUPPORTED_CAPABILITIES__

//  INCLUDE FILES
#include "cimcacheserver.h"

#include "ccacheserveractivehelper.h"
#include "cimcachesession.h"
#include "mimcachemessageheader.h"
#include "timcachemessagefactory.h"
#include "cimcachemessageheader.h"
#include "cimcachemessagebase.h"
// logs
#include "imcachedebugtrace.h" //logs

// ==============================================================
// =============== PLATSEC POLICY CONFIGURATION =================
// ==============================================================

static const TInt KIMCachePlatSecRangeCount = 2;

//Ranges for the Request values
static const TInt KIMCachePlatSecRanges[ KIMCachePlatSecRangeCount ] =
    {
    0,
    EIMCacheOperationLast
    };


// Element indexes for the defined ranges
static const TUint8 KIMCachePlatSecElementsIndex[ KIMCachePlatSecRangeCount ] =
    {
    0,
    CPolicyServer::ENotSupported
    };


// Policy elements
static const CPolicyServer::TPolicyElement KIMCachePlatSecElements[] =
    {
        {
        _INIT_SECURITY_POLICY_C2( ECapabilityReadUserData,
                                  ECapabilityWriteUserData ),
        CPolicyServer::EFailClient
        }
    };


// The platsec policy
static const CPolicyServer::TPolicy KIMCachePlatSecPolicy =
    {
    // Shortcut to the index into Elements,that is used to check a connection attempt
    0,

    // Number of ranges in the iRanges array
    KIMCachePlatSecRangeCount,

    // A pointer to an array of ordered ranges of request numbers
    KIMCachePlatSecRanges,

    // A pointer to an array of TUint8 values specifying
    // the appropriate action to take for each range in iRanges
    KIMCachePlatSecElementsIndex,

    // A pointer to an array of distinct policy elements
    KIMCachePlatSecElements
    };


// ==============================================================
// ======================= SERVER  ==============================
// ==============================================================

// -----------------------------------------------------------------------------
// CIMCacheServer::ExecuteL
// -----------------------------------------------------------------------------
//
void CIMCacheServer::ExecuteL()
    {
    TRACE( T_LIT( "CIMCacheServer::ExecuteL() begin") );
    // start scheduler
    CActiveScheduler* pA = new( ELeave )CActiveScheduler;
    CleanupStack::PushL( pA );
    CActiveScheduler::Install( pA );


    // create server
    CIMCacheServer* server = new( ELeave ) CIMCacheServer();
    CleanupStack::PushL( server );
    server->InitializeL(*server);
    server->StartL( KIMCacheServerName );


    //Signal client that we are started
    RProcess().Rendezvous( KErrNone );

    // Execute the server
    // Codescanner warning: using CActiveScheduler::Start (id:3)
    // this has to be called for server starting.
    CActiveScheduler::Start(); // CSI: 3 # See above

    //Cleanup
    CleanupStack::PopAndDestroy( server );//server
    CleanupStack::PopAndDestroy( pA );
    CActiveScheduler::Install( NULL );
    TRACE( T_LIT( "CIMCacheServer::ExecuteL() end") );
    }

// -----------------------------------------------------------------------------
// CIMCacheServer::~CIMCacheServer
// -----------------------------------------------------------------------------
//
CIMCacheServer::~CIMCacheServer()
    {
    TRACE( T_LIT( "CIMCacheServer::~CIMCacheServer() begin") );
   
    iSessions.Close();
    
	delete iActiveHelper;
	
	#if _BullseyeCoverage
    	cov_write();
	#endif
    TRACE( T_LIT( "CIMCacheServer::~CIMCacheServer() end") );
    }

// -----------------------------------------------------------------------------
// CIMCacheServer::CIMCacheServer
// -----------------------------------------------------------------------------
//

CIMCacheServer::CIMCacheServer()
    : CPolicyServer( CActive::EPriorityStandard,
                     KIMCachePlatSecPolicy )
    {
    }

// -----------------------------------------------------------------------------
// CIMCacheServer::NewSessionL
// -----------------------------------------------------------------------------
//
CSession2* CIMCacheServer::NewSessionL( const TVersion &aVersion,
                                          const RMessage2& /*aMessage*/ ) const
    {
    TRACE( T_LIT( "CIMCacheServer::NewSessionL() begin") );
    TVersion srvVersion( KIMCacheVersionMajor,
                         KIMCacheVersionMinor,
                         KIMCacheVersionBuild );

	if( !User::QueryVersionSupported( aVersion, srvVersion ) )
        {
        User::Leave( KErrNotSupported );
        }
	CIMCacheSession* session = CIMCacheSession::NewL();	
     
    TRACE( T_LIT( "CIMCacheServer::NewSessionL() end session[%d] created"), session );
    return session;
    }

// -----------------------------------------------------------------------------
// CIMCacheServer::SessionCreatedL
// -----------------------------------------------------------------------------
//
void CIMCacheServer::SessionCreatedL( CIMCacheSession* aSession )
    {
    TRACE( T_LIT( "CIMCacheServer::SessionCreatedL begin aSession[%d]"),aSession );
    iSessionCount++;
    TInt err = iSessions.Find( aSession );
    if(err == KErrNotFound )
	    {
	    iSessions.AppendL( aSession );
	    }
    TRACE( T_LIT( "CIMCacheServer::SessionCreatedL() end") );
      }
// -----------------------------------------------------------------------------
// CIMCacheServer::SessionDied
// -----------------------------------------------------------------------------
//
void CIMCacheServer::SessionDied( CIMCacheSession* aSession )
    {
    TRACE( T_LIT( "CIMCacheServer::SessionDied() begin") );
    TInt count = iSessions.Count();
      for( TInt i = 0; i < count; i++ )
			{
			if( iSessions[i] == aSession )
				{
				iSessions.Remove( i );
				break;
				}
			}
	iSessionCount--;
	if( iSessionCount == 0 )
		{
		// Codescanner warning: using CActiveScheduler::Stop ( Id: 4)
		// it is required to stop the server
		CActiveScheduler::Stop(); // CSI: 4 # See above
		}
	TRACE( T_LIT( "CIMCacheServer::SessionDied() end") );
    }
// -----------------------------------------------------------------------------
// CIMCacheManager::GetActiveHelper
// Pass pointer to toc array to client.
// -----------------------------------------------------------------------------
//
CCacheServerActiveHelper* CIMCacheServer::GetActiveHelper()
	{
    return iActiveHelper;
	}

// -----------------------------------------------------------------------------
// CIMCacheServer::InitializeL
// -----------------------------------------------------------------------------
//
void CIMCacheServer::InitializeL(CIMCacheServer& aServer)
    {
    TRACE( T_LIT( "CIMCacheServer::InitializeL begin") );
    
    iActiveHelper = CCacheServerActiveHelper::NewL( aServer );
    
    TRACE( T_LIT( "CIMCacheServer::InitializeL end") );
    }

// -----------------------------------------------------------------------------
// CIMCacheServer::PackAndNotifyEventL
// -----------------------------------------------------------------------------
// send notication to all if registration is valid
 
void CIMCacheServer::PackAndNotifyEventL(TIMCacheOperationsCode aChangeType,
                                         TInt aServiceId,
										 MIMCacheMessageHeader* aChatHeader /*=NULL*/, 
									 	 CIMCacheMessageBase* aMessage /*=NULL*/ ) 
	{
	TInt count = iSessions.Count();
	for(TInt i = 0; i<count; i++ )
		{
		if( aServiceId == iSessions[i]->ServiceId() )
		    {
		    // send changes to only those session which are has same service Id
		    iSessions[i]->PackAndNotifyEventL(aChangeType , aChatHeader, aMessage);	
		    }
		}
	}


// ==============================================================
// ====================== ENTRY POINT ===========================
// ==============================================================
GLDEF_C TInt E32Main()
    {
    TRACE( T_LIT("E32Main - enter") );

    User::RenameThread( KIMCacheServerName );

    CTrapCleanup* tc = CTrapCleanup::New();
    if( !tc )
        {
        return KErrNoMemory;
        }

    TRAPD( err, CIMCacheServer::ExecuteL() );
    delete tc;

    TRACE( T_LIT("E32Main - exit: %d"), err );
    return err;
    }



//  END OF FILE



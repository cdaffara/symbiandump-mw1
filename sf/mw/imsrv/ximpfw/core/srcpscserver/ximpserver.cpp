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
* Description:  XIMP Framework PSC server class
 *
*/

#include "ximpserverdefs.h"
#include "ximpserver.h"
#include "ximpserversessionadapter.h"
#include "ximptrace.h"
#include "ximpglobals.h"
#include "ximpoperationfactoryimp.h"
#include <ecom/ecom.h>

//Include PSC server namespace
using namespace NXIMPSrv;



// ==============================================================
// =============== PLATSEC POLICY CONFIGURATION =================
// ==============================================================
static const TInt KXIMPServerPlatSecRangeCount = 3;

static const TInt KXIMPServerPlatSecPolicyIndex0 = 0;
static const TInt KXIMPServerPlatSecPolicyIndex1 = 1;

//Ranges for the Request values
static const TInt KXIMPServerPlatSecRanges[ KXIMPServerPlatSecRangeCount ] =
    {
    0,
    NRequest::ECtxSsQueueBindOperation,
    NRequest::EOpCodeTop
    };


// Element indexes for the defined ranges
static const TUint8 KXIMPServerPlatSecElementsIndex[ KXIMPServerPlatSecRangeCount ] =
    {
    KXIMPServerPlatSecPolicyIndex0,
    KXIMPServerPlatSecPolicyIndex1,
    CPolicyServer::ENotSupported
    };


// Policy elements
static const CPolicyServer::TPolicyElement KXIMPServerPlatSecElements[] =
    {
        {
        // Policy 0, KXIMPServerPlatSecPolicyIndex0
        _INIT_SECURITY_POLICY_C3( ECapabilityReadUserData,
                                  ECapabilityWriteUserData,
                                  ECapabilityNetworkServices ),
        CPolicyServer::EFailClient
        },
        {
        // Policy 1, KXIMPServerPlatSecPolicyIndex1
        _INIT_SECURITY_POLICY_C4( ECapabilityReadDeviceData,
                                  ECapabilityReadUserData,
                                  ECapabilityWriteUserData,
                                  ECapabilityNetworkServices ),
        CPolicyServer::EFailClient
        }
    };


// The platsec policy
static const CPolicyServer::TPolicy KXIMPServerPlatSecPolicy =
    {
    // Shortcut to the index into Elements,that is used to check a connection attempt
    KXIMPServerPlatSecPolicyIndex0,

    // Number of ranges in the iRanges array
    KXIMPServerPlatSecRangeCount,

    // A pointer to an array of ordered ranges of request numbers
    KXIMPServerPlatSecRanges,

    // A pointer to an array of TUint8 values specifying
    // the appropriate action to take for each range in iRanges
    KXIMPServerPlatSecElementsIndex,

    // A pointer to an array of distinct policy elements
    KXIMPServerPlatSecElements
    };



// -----------------------------------------------------------------------------
// CXIMPServer::ExecuteL()
// -----------------------------------------------------------------------------
//
void CXIMPServer::ExecuteL()
    {
    // start scheduler
    CActiveScheduler* pA = new( ELeave )CActiveScheduler;
    CleanupStack::PushL( pA );
    CActiveScheduler::Install( pA );

    // create server
    CXIMPServer* server = CXIMPServer::NewLC();

    // rename our thread properly
    // first, find out the UID set by process starter
    TInt uid = 0;
    TInt ret = User::GetTIntParameter( NStartupParam::KProtocolUidIndex,
                                       uid );
    User::LeaveIfError( ret );

    // create the "!XIMPCtxSrv9bde667a" server name
    HBufC* serverName = HBufC::NewLC( NName::KSymbianServer().Length() +
                                      NStartupParam::KProtocolUidMaxLen +
                                      1 );

    serverName->Des().Copy( NName::KSymbianServer );
    serverName->Des().AppendNumFixedWidth( uid,
                                           EHex,
                                           NStartupParam::KProtocolUidMaxLen );

    // and set it
    User::RenameThread( *serverName );
    server->StartL( *serverName );

    CleanupStack::PopAndDestroy( serverName );

    //Signal client that we are started
    RProcess().Rendezvous( KErrNone );

    //Execute the server
    CActiveScheduler::Start(); // CSI: 3 #

    //Cleanup
    CleanupStack::PopAndDestroy( server );//server
    CleanupStack::PopAndDestroy( pA );
    CActiveScheduler::Install( NULL );
    }



// -----------------------------------------------------------------------------
// CXIMPServer::~CXIMPServer()
// -----------------------------------------------------------------------------
//
CXIMPServer::~CXIMPServer()
    {
    CXIMPGlobals::UninstallD();
    REComSession::FinalClose();

    #if _BullseyeCoverage
    cov_write();
    #endif    
    }


// -----------------------------------------------------------------------------
// CXIMPServer::CXIMPServer()
// -----------------------------------------------------------------------------
//
CXIMPServer::CXIMPServer()
    : CPolicyServer( CActive::EPriorityStandard, KXIMPServerPlatSecPolicy )
    {
    iSessionId = 1;   // start from 1. 0 reserved for host-originated operations
    }


// -----------------------------------------------------------------------------
// CXIMPServer::NewLC()
// -----------------------------------------------------------------------------
//
CXIMPServer* CXIMPServer::NewLC()
    {
    CXIMPServer* self = new( ELeave ) CXIMPServer;
    CleanupStack::PushL( self );
    self->ConstructL();
    return self;
    }


// -----------------------------------------------------------------------------
// CXIMPServer::ConstructL()
// -----------------------------------------------------------------------------
//
void CXIMPServer::ConstructL()
    {
    CXIMPOperationFactory* operationFactory = CXIMPOperationFactory::NewL();
    CleanupStack::PushL( operationFactory );
    CXIMPGlobals::InstallL( operationFactory, this );
    CleanupStack::Pop( operationFactory );
    iGlobals = CXIMPGlobals::Instance();
    }


// -----------------------------------------------------------------------------
// CXIMPServer::NewSessionL()
// -----------------------------------------------------------------------------
//
CSession2* CXIMPServer::NewSessionL( const TVersion &aVersion,
                                        const RMessage2& /* aMessage */ ) const
    {
    TVersion srvVersion( NVersion::KMajor,
                         NVersion::KMinor,
                         NVersion::KBuild );

    if( !User::QueryVersionSupported( aVersion, srvVersion ) )
        {
        User::Leave( KErrNotSupported );
        }

    CXIMPServer* self = const_cast< CXIMPServer* >( this );
    self->iSessionId++;
    return CXIMPServerSessionAdapter::NewL( iSessionId, 
                                               *self );
    };


// -----------------------------------------------------------------------------
// CXIMPServer::SessionCreated()
// -----------------------------------------------------------------------------
//
void CXIMPServer::SessionCreated()
    {
    iSessionCount++;
    }


// -----------------------------------------------------------------------------
// CXIMPServer::SessionDied()
// -----------------------------------------------------------------------------
//
void CXIMPServer::SessionDied()
    {
    iSessionCount--;
    TryToStopScheduling();
    }
    
// -----------------------------------------------------------------------------
// CXIMPServer::AllHostsDied()
// -----------------------------------------------------------------------------
//
void CXIMPServer::AllHostsDied()
    {
    // all hosts have died.
    iAllHostsDied = ETrue;
    TryToStopScheduling();
    }

// -----------------------------------------------------------------------------
// CXIMPServer::AllHostsDied()
// -----------------------------------------------------------------------------
//
void CXIMPServer::TryToStopScheduling()
    {
    if( iSessionCount == 0 && iAllHostsDied )
        {
        CActiveScheduler::Stop(); // CSI: 4 #
        }    
    }

// -----------------------------------------------------------------------------
// CXIMPServer::TestListener()
// -----------------------------------------------------------------------------
//
MXIMPSrvTestListener* CXIMPServer::TestListener() const
    {
    return iTestListener;
    }
    
// -----------------------------------------------------------------------------
// CXIMPServer::RegisterTestListener()
// -----------------------------------------------------------------------------
//
void CXIMPServer::RegisterTestListener( MXIMPSrvTestListener* aTestSession )
    {
    iTestListener = aTestSession;
    }
    
// -----------------------------------------------------------------------------
// E32Main()
// ENTRY POINT
// -----------------------------------------------------------------------------
//
GLDEF_C TInt E32Main()
    {
    TRACE( _L("PSC Server E32Main - enter") );

    __UHEAP_MARK;

    CTrapCleanup* tc = CTrapCleanup::New();
    if( !tc )
        {
        return KErrNoMemory;
        }

    User::RenameThread( NName::KMainThread );

    TRAPD( err, CXIMPServer::ExecuteL() );
    delete tc;

    __UHEAP_MARKEND;

    TRACE_1( _L("PSC Server E32Main - exit: %d"), err );
    return err;
    }


//  END OF FILE



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
* Description:  Root server implementation
 *
*/

#include <ecom/ecom.h>

#include "ximprootserver.h"
#include "ximprootserversessionadapter.h"
#include "ximprootserverdefs.h"
#include "ximptrace.h"
#include "ximpglobals.h"
#include "ximpoperationfactoryimp.h"

//Include Root server namespace
using namespace NRootSrv;


// ==============================================================
// =============== PLATSEC POLICY CONFIGURATION =================
// ==============================================================
static const TInt KXIMPRootServerPlatSecRangeCount = 2;

//Ranges for the Request values
static const TInt KXIMPRootServerPlatSecRanges[ KXIMPRootServerPlatSecRangeCount ] =
    {
    0,
    Max( ( TInt )NRootSrv::NRequest::EOpCodeTop, ( TInt )NRequest::EOpCodeTop )
    };


// Element indexes for the defined ranges
static const TUint8 KXIMPRootServerPlatSecElementsIndex[ KXIMPRootServerPlatSecRangeCount ] =
    {
    0,
    CPolicyServer::ENotSupported
    };


// Policy elements
static const CPolicyServer::TPolicyElement KXIMPRootServerPlatSecElements[] =
    {
        {
        _INIT_SECURITY_POLICY_C3( ECapabilityReadUserData,
                ECapabilityWriteUserData,
                ECapabilityNetworkServices ),
        CPolicyServer::EFailClient
        }
    };


// The platsec policy
static const CPolicyServer::TPolicy KXIMPRootServerPlatSecPolicy =
    {
    // Shortcut to the index into Elements,that is used to check a connection attempt
    0,

    // Number of ranges in the iRanges array
    KXIMPRootServerPlatSecRangeCount,

    // A pointer to an array of ordered ranges of request numbers
    KXIMPRootServerPlatSecRanges,

    // A pointer to an array of TUint8 values specifying
    // the appropriate action to take for each range in iRanges
    KXIMPRootServerPlatSecElementsIndex,

    // A pointer to an array of distinct policy elements
    KXIMPRootServerPlatSecElements
    };



// -----------------------------------------------------------------------------
// CXIMPRootServer::ExecuteL()
// -----------------------------------------------------------------------------
//
void CXIMPRootServer::ExecuteL()
    {
    // start scheduler
    CActiveScheduler* pA = new( ELeave )CActiveScheduler;
    CleanupStack::PushL( pA );
    CActiveScheduler::Install( pA );

    // create server
    CXIMPRootServer* server = CXIMPRootServer::NewLC();
    server->StartL( NName::KSymbianServer );

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
// CXIMPRootServer::~CXIMPRootServer()
// -----------------------------------------------------------------------------
//
CXIMPRootServer::~CXIMPRootServer()
    {
    CXIMPGlobals::UninstallD();
    REComSession::FinalClose();
    
    #if _BullseyeCoverage
    cov_write();
    #endif
    }


// -----------------------------------------------------------------------------
// CXIMPRootServer::CXIMPRootServer()
// -----------------------------------------------------------------------------
//
CXIMPRootServer::CXIMPRootServer()
    : CPolicyServer( CActive::EPriorityStandard,
                     KXIMPRootServerPlatSecPolicy )
    {
    }


// -----------------------------------------------------------------------------
// CXIMPRootServer::NewLC()
// -----------------------------------------------------------------------------
//
CXIMPRootServer* CXIMPRootServer::NewLC()
    {
    CXIMPRootServer* self = new( ELeave ) CXIMPRootServer;
    CleanupStack::PushL( self );
    self->ConstructL();
    return self;
    }


// -----------------------------------------------------------------------------
// CXIMPRootServer::ConstructL()
// -----------------------------------------------------------------------------
//
void CXIMPRootServer::ConstructL()
    {
    CXIMPOperationFactory* operationFactory = CXIMPOperationFactory::NewL();
    CleanupStack::PushL( operationFactory );
    CXIMPGlobals::InstallL( operationFactory, NULL );
    CleanupStack::Pop( operationFactory );
    iGlobals = CXIMPGlobals::Instance();        
    }


// -----------------------------------------------------------------------------
// CXIMPRootServer::NewSessionL()
// -----------------------------------------------------------------------------
//
CSession2* CXIMPRootServer::NewSessionL( const TVersion &aVersion,
                                         const RMessage2& /* aMessage */ ) const
    {
    TVersion srvVersion( NVersion::KMajor,
                         NVersion::KMinor,
                         NVersion::KBuild );

    if( !User::QueryVersionSupported( aVersion, srvVersion ) )
        {
        User::Leave( KErrNotSupported );
        }

    CXIMPRootServer* self = const_cast< CXIMPRootServer* >( this );
    CSession2* session = NULL;
   
    TRAPD( err, session = CXIMPRootServerSessionAdapter::NewL( *self ) );
    User::LeaveIfError( err );
    return session;
    }


// -----------------------------------------------------------------------------
// CXIMPRootServer::SessionCreated()
// -----------------------------------------------------------------------------
//
void CXIMPRootServer::SessionCreated()
    {
    iSessionCount++;
    }


// -----------------------------------------------------------------------------
// CXIMPRootServer::SessionDied()
// -----------------------------------------------------------------------------
//
void CXIMPRootServer::SessionDied()
    {
    iSessionCount--;
    if( iSessionCount == 0 )
        {
        CActiveScheduler::Stop(); // CSI: 4 #
        }
    }

// -----------------------------------------------------------------------------
// E32Main()
// ENTRY POINT
// -----------------------------------------------------------------------------
//
GLDEF_C TInt E32Main()
    {
    TRACE( _L("Root E32Main - enter") );

    __UHEAP_MARK;

    User::RenameThread( NName::KMainThread );

    CTrapCleanup* tc = CTrapCleanup::New();
    if( !tc )
        {
        return KErrNoMemory;
        }

    TRAPD( err, CXIMPRootServer::ExecuteL() );
    delete tc;

    __UHEAP_MARKEND;

    TRACE_1( _L("Root E32Main - exit: %d"), err );
    return err;
    }


//  END OF FILE



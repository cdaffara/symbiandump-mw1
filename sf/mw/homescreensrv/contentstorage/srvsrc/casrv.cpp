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
 * Description:  ?Description
 *
 */
// INCLUDE FILES
#include <sqldb.h>
#include <eikenv.h>
#include <eikappui.h>

#include "casrv.h"
#include "casrvdef.h"
#include "casrvsession.h"
#include "casrvengutils.h"
#include "castorageproxy.h"
#include "casrvmanager.h"
#include "cabackupnotifier.h"
 

// ==================== LOCAL FUNCTIONS ====================

/**
 * Create a server.
 * @param Pointer to created server (if created) returned here.
 * @return Error code.
 */
LOCAL_C TInt CreateServer( CCaSrv*& aServer )
    {
    // The TRAP is not working in the same stack frame where the
    // CTrapCleanup was created. This is why we need this function.
    TRAPD( err, aServer = CCaSrv::NewL() );
    return err;
    }

// ==================== GLOBAL FUNCTIONS ====================

// ---------------------------------------------------------
//
// ---------------------------------------------------------
//
EXPORT_C TInt RunCaServer()
    {
    __UHEAP_MARK;
    CTrapCleanup* trapCleanup = NULL;
    CActiveScheduler* activeScheduler = NULL;
    CCaSrv* server = NULL;

    TInt err = User::RenameThread( KCaSrvName );
    if( !err )
        {
        // Create a trap cleanup, make and install an active scheduler.
        err = KErrNoMemory;
        trapCleanup = CTrapCleanup::New();
        if( trapCleanup )
            {
            activeScheduler = new CActiveScheduler();
            if( activeScheduler )
                {
                CActiveScheduler::Install( activeScheduler );
                err = CreateServer( server ); // Not pushed (no leaving).
                if( !err )
                    {
                    err = server->Start( KCaSrvName );
                    }
                }
            else
                {
                err = KErrNoMemory;
                }
            }
        }

    // Let the caller know how it went.
    RProcess::Rendezvous( err );
    if( !err )
        {
        CActiveScheduler::Start(); // Start off. Exit timer will stop it.
        }
    CActiveScheduler::Install( NULL );
    delete activeScheduler;
    delete server;
    delete trapCleanup;
    __UHEAP_MARKEND;
    return err;
    }

// ==================== MEMBER FUNCTIONS ====================

// ---------------------------------------------------------
//
// ---------------------------------------------------------
//
CCaSrv* CCaSrv::NewL()
    {
    CCaSrv* srv = new ( ELeave ) CCaSrv();
    CleanupStack::PushL( srv );
    srv->ConstructL();
    CleanupStack::Pop( srv );
    return srv;
    }

// ---------------------------------------------------------
//
// ---------------------------------------------------------
//
CCaSrv::~CCaSrv()
    {
    // Cancel requests and delete all sessions first.
    // Base class would do it for us but that's too late - our sessions
    // call the server back (SessionClosed, RemoveContainer, etc.).
    Cancel();
    delete iBackupNotifier;
    CSession2* session;
    iSessionIter.SetToFirst();
    while( ( session = iSessionIter++ ) != NULL )
        {
        delete session;
        }
    delete iSrvManager;
    iSoftwareRegistry.Close();
    delete iSrvEngUtils;
    delete iStorageProxy;
    }

// ---------------------------------------------------------
//
// ---------------------------------------------------------
//
CCaStorageProxy* CCaSrv::GetStorageProxy()
    {
    return iStorageProxy;
    }

// ---------------------------------------------------------
//
// ---------------------------------------------------------
//
CCaSrv::CCaSrv() :
    CServer2( EPriorityNormal, CServer2::TServerType( EIpcSession_Sharable ) )
    {
    }

// ---------------------------------------------------------
//
// ---------------------------------------------------------
//
void CCaSrv::ConstructL()
    {
    iSessionCount = 0;
    iStorageProxy = CCaStorageProxy::NewL();
    iSrvEngUtils = CCaSrvEngUtils::NewL();
    User::LeaveIfError( iSoftwareRegistry.Connect() );
    iSrvManager = CCaSrvManager::NewL( *iStorageProxy,
        &iSoftwareRegistry, iSrvEngUtils);
    TInt errCode = iSrvManager->LoadOperationErrorCodeL();
    if( KSqlErrNotDb <= errCode && errCode <= KSqlErrGeneral )
        {
        //problem in loading one of plugins, probably data base is corrupted
        //lets load it from ROM and try again
        delete iSrvManager;
        iSrvManager = NULL;
        iStorageProxy->LoadDataBaseFromRomL();
        iSrvManager = CCaSrvManager::NewL( *iStorageProxy,
            &iSoftwareRegistry, iSrvEngUtils );
        }
    iBackupNotifier = CCaBackupNotifier::NewL( iStorageProxy );
    }

// ---------------------------------------------------------
//
// ---------------------------------------------------------
//
CSession2* CCaSrv::NewSessionL( const TVersion& aVersion,
        const RMessage2& /*aMessage*/) const
    {
    TVersion version( KCaMajorVersion, KCaMinorVersion, KCaBuild );
    if( !User::QueryVersionSupported( version, aVersion ) )
        {
        User::Leave( KErrNotSupported );
        }
    CSession2* session;
    session = CCaSrvSession::NewL( const_cast<CCaSrv&> ( *this ) );
    return session;
    }

// ---------------------------------------------------------
//
// ---------------------------------------------------------
//
void CCaSrv::IncreaseSessionCount()
    {
    iSessionCount++;
    }

// ---------------------------------------------------------
//
// ---------------------------------------------------------
//
void CCaSrv::DecreaseSessionCount()
    {
    iSessionCount--;
    }

//  End of File

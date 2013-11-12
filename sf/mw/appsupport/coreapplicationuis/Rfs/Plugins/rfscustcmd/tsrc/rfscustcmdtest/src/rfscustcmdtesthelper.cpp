/*
* Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:
* CRfsCustCmdTestHelper class implementation.
*
*/

#include <ssm/ssmcustomcommand.h>

#include "rfscustcmdfactory.h"
#include "rfscustcmdtesthelper.h"
#include "trace.h"

const TInt KWaitDelay = { 2000000 };

// ============================= LOCAL FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// WaitCallBack()
// -----------------------------------------------------------------------------
//
static TInt WaitCallBack( TAny* aSelf )
    {
    if( aSelf )
        {
        TWaitInfo* info = static_cast<TWaitInfo*>( aSelf );
        if( info->iPeriodic )
            {
            info->iPeriodic->Cancel();
            }
        if( info->iWait )
            {
            if( info->iWait->IsStarted() )
                {
                info->iWait->AsyncStop();
                }
            }
        }
    
    return KErrNone;
    }

// -----------------------------------------------------------------------------
// WaitL()
// -----------------------------------------------------------------------------
//
static void WaitL( TInt aIntervalInMicorseconds )
    {
    TWaitInfo info;
    
    // Construct periodic
    CPeriodic* periodic = CPeriodic::NewL( CActive::EPriorityStandard );
    CleanupStack::PushL( periodic );
    info.iPeriodic = periodic;
    
    // Construct active scheduler wait
    CActiveSchedulerWait* wait = new( ELeave ) CActiveSchedulerWait;
    CleanupStack::PushL( wait );
    info.iWait = wait;
    
    // Start timer and wait
    TCallBack cb( WaitCallBack, &info );
    periodic->Start( aIntervalInMicorseconds, aIntervalInMicorseconds, cb );
    wait->Start();
    
    // Cleanup
    CleanupStack::PopAndDestroy( wait );
    CleanupStack::PopAndDestroy( periodic );
    }

// ============================= MEMBER FUNCTIONS ===============================

// ---------------------------------------------------------------------------
// CRfsCustCmdTestHelper::NewL
// ---------------------------------------------------------------------------
//
CRfsCustCmdTestHelper* CRfsCustCmdTestHelper::NewL( CRfsCustCmd::TRfsCustCmdType aType )
    {
    CRfsCustCmdTestHelper* self = new( ELeave ) CRfsCustCmdTestHelper( aType );
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );
    return self;
    }


// ---------------------------------------------------------------------------
// CRfsCustCmdTestHelper::CRfsCustCmdTestHelper
// ---------------------------------------------------------------------------
//
CRfsCustCmdTestHelper::CRfsCustCmdTestHelper( CRfsCustCmd::TRfsCustCmdType aType ) :
    iType( aType )
    {
    FUNC_LOG;
    }


// ---------------------------------------------------------------------------
// CRfsCustCmdTestHelper::~CRfsCustCmdTestHelper
// ---------------------------------------------------------------------------
//
CRfsCustCmdTestHelper::~CRfsCustCmdTestHelper()
    {
    FUNC_LOG;

    if( iCmd )
        {
        iCmd->Close();
        iCmd->Release();
        }
    delete iAsyncHandler;
    }


// ---------------------------------------------------------------------------
// CRfsCustCmdTestHelper::ConstructL
// ---------------------------------------------------------------------------
//
void CRfsCustCmdTestHelper::ConstructL()
    {
    FUNC_LOG;
    
    if( iType == CRfsCustCmd::ERfsCustCmdFirstBoot )
        {
        iCmd = RfsCustCmdFactory::RfsCustCmdFirstBootNewL();
        }
    else if( iType == CRfsCustCmd::ERfsCustCmdDeep )
        {
        iCmd = RfsCustCmdFactory::RfsCustCmdDeepNewL();
        }
    else if( iType == CRfsCustCmd::ERfsCustCmdPreUi )
        {
        iCmd = RfsCustCmdFactory::RfsCustCmdPreUiNewL();
        }
    else if( iType == CRfsCustCmd::ERfsCustCmdPostUi )
        {
        iCmd = RfsCustCmdFactory::RfsCustCmdPostUiNewL();
        }
    
    // Create async handler
    iAsyncHandler = CAsyncRequestHandler<CRfsCustCmdTestHelper>::NewL( *this,
        &CRfsCustCmdTestHelper::IssueRequest,
        &CRfsCustCmdTestHelper::RunL,
        &CRfsCustCmdTestHelper::RunError,
        &CRfsCustCmdTestHelper::DoCancel,
        CAsyncRequestHandler<CRfsCustCmdTestHelper>::ERequestOneShot );
    }


// ---------------------------------------------------------------------------
// CRfsCustCmdTestHelper::RfsInitialize
// ---------------------------------------------------------------------------
//
void CRfsCustCmdTestHelper::RfsInitialize()
    {
    FUNC_LOG;
    CSsmCustomCommandEnv* cmdEnv( NULL );
    
    iCmd->Initialize( cmdEnv );
    }


// ---------------------------------------------------------------------------
// CRfsCustCmdTestHelper::RfsExecute
// ---------------------------------------------------------------------------
//
void CRfsCustCmdTestHelper::RfsExecute()
    {
    FUNC_LOG;
    CSsmCustomCommandEnv* cmdEnv( NULL );
    
    iCmd->Initialize( cmdEnv );
    iAsyncHandler->IssueRequest();
    WaitL( KWaitDelay );
    }


// ---------------------------------------------------------------------------
// CRfsCustCmdTestHelper::RfsExecuteCancel
// ---------------------------------------------------------------------------
//
void CRfsCustCmdTestHelper::RfsExecuteCancel()
    {
    FUNC_LOG;
    
    CSsmCustomCommandEnv* cmdEnv( NULL );
    
    iCmd->Initialize( cmdEnv );
    iAsyncHandler->IssueRequest();
    WaitL( KWaitDelay );
    iCmd->ExecuteCancel();
    iCmd->Close();

    iCmd->Initialize( cmdEnv );
    iAsyncHandler->IssueRequest();
    iCmd->ExecuteCancel();
    WaitL( KWaitDelay );
    iCmd->Close();
    }


//------------------------------------------------------------------------------
// CRfsCustCmdTestHelper::IssueRequest
//------------------------------------------------------------------------------
//
void CRfsCustCmdTestHelper::IssueRequest( TRequestStatus& aStatus )
    {
    FUNC_LOG;
    
    iCmd->Execute( KNullDesC8, aStatus );
    }

//------------------------------------------------------------------------------
// CRfsCustCmdTestHelper::RunL
//------------------------------------------------------------------------------
//
void CRfsCustCmdTestHelper::RunL( TInt aStatus )
    {
    FUNC_LOG;
    
    INFO_1( "Async request completed with status %d ", aStatus );
    }

//------------------------------------------------------------------------------
// CRfsCustCmdTestHelper::RunError
//------------------------------------------------------------------------------
//
TInt CRfsCustCmdTestHelper::RunError( TInt /*Error*/ )
    {
    FUNC_LOG;
    
    return KErrNone;
    }

//------------------------------------------------------------------------------
// CRfsCustCmdTestHelper::DoCancel
//------------------------------------------------------------------------------
//
void CRfsCustCmdTestHelper::DoCancel()
    {
    FUNC_LOG;
    }

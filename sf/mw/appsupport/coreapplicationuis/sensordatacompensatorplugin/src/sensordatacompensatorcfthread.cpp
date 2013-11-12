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
* Description:  Plugin controller
*
*/

// INCLUDES
#include <cfactionindication.h>
#include "sensordatacompensatorcfthread.h"
#include "trace.h"


// ============================= MEMBER FUNCTIONS =============================

// ----------------------------------------------------------------------------
// CSensorDataCompensatorCfTread::CSensorDataCompensatorCfTread()
//
// ----------------------------------------------------------------------------
CSensorDataCompensatorCfTread::CSensorDataCompensatorCfTread(
    MSensorDataCompensatorCfThreadController& aController ) :
    CActive( CActive::EPriorityHigh ),
    iController( aController )
    {
    FUNC_LOG;
    }

// ----------------------------------------------------------------------------
// CSensorDataCompensatorCfTread::~CSensorDataCompensatorCfTread()
//
// ----------------------------------------------------------------------------
CSensorDataCompensatorCfTread::~CSensorDataCompensatorCfTread()
    {
    FUNC_LOG;

    Cancel();
    delete iCfTreadNotify;
    iKeyValueArray.ResetAndDestroy();
    iThread.Close();
    iMutex.Close();
    }

// ----------------------------------------------------------------------------
// CSensorDataCompensatorCfTread::NewL()
//
// ----------------------------------------------------------------------------
CSensorDataCompensatorCfTread* CSensorDataCompensatorCfTread::NewL(
    MSensorDataCompensatorCfThreadController& aController )
    {
    FUNC_LOG;

    CSensorDataCompensatorCfTread* self =
        new (ELeave) CSensorDataCompensatorCfTread( aController );

    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );
    return self;
    }

// ----------------------------------------------------------------------------
// CSensorDataCompensatorCfTread::ConstructL()
//
// ----------------------------------------------------------------------------
void CSensorDataCompensatorCfTread::ConstructL()
    {
    FUNC_LOG;

    CActiveScheduler::Add( this );
    User::LeaveIfError( iMutex.CreateLocal() );
    iCfTreadNotify = CfTreadNotify::NewL( *this );
    User::LeaveIfError( iThread.Create(
        KNullDesC, ThreadFunction, KDefaultStackSize, NULL, this ) );
    iThread.Logon( iStatus );
    iClientId = RThread().Id();
    SetActive();
    TRequestStatus startStatus;
    iThread.Rendezvous( startStatus );
    iThread.Resume();
    // Wait for critical part of thread init to complete
    User::WaitForRequest( startStatus );
    User::LeaveIfError( startStatus.Int() );
    }

// ----------------------------------------------------------------------------
// CSensorDataCompensatorCfTread::RunL()
// Run in client thread context
// ----------------------------------------------------------------------------
void CSensorDataCompensatorCfTread::RunL()
    {
    FUNC_LOG;
    
    ERROR_1( KErrGeneral, "CSensorDataCompensatorCfTread::RunL - unexpected err %d", 
        iStatus.Int() );
    }

// ----------------------------------------------------------------------------
// CSensorDataCompensatorCfTread::DoCancel()
// Run in client thread context
// ----------------------------------------------------------------------------
void CSensorDataCompensatorCfTread::DoCancel()
    {
    FUNC_LOG;
    
    if( iCfThreadShutdown )
        {
        // Trigger shutdown of listener thread.
        iCfThreadShutdown->Complete();
        }
    }

// ----------------------------------------------------------------------------
// CSensorDataCompensatorCfTread::ThreadFunction()
// Run in listener thread context
// ----------------------------------------------------------------------------
TInt CSensorDataCompensatorCfTread::ThreadFunction( TAny* ptr )
    {
    FUNC_LOG;
    
    TInt err( KErrNone );
    CSensorDataCompensatorCfTread* self = 
        static_cast< CSensorDataCompensatorCfTread* >( ptr );
    CTrapCleanup* cleanupStack = CTrapCleanup::New();
    if ( !cleanupStack )
        {
        err = KErrNoMemory;
        }
    if ( err == KErrNone )
        {
        TRAP( err, self->ThreadFunctionL() );
        }
    if ( err != KErrNone )
        {
        ERROR( err, "CSensorDataCompensatorCfTread::ThreadFunction - Rendezvous" );
        // Notify error in critical part of thread init
        RThread::Rendezvous( err );
        }
    delete cleanupStack;
    return err;
    }

// ----------------------------------------------------------------------------
// CSensorDataCompensatorCfTread::ThreadFunctionL()
// Run in listener thread context
// ----------------------------------------------------------------------------
void CSensorDataCompensatorCfTread::ThreadFunctionL()
    {
    FUNC_LOG;
    
    CActiveScheduler* scheduler = new (ELeave) CActiveScheduler;
    CleanupStack::PushL(scheduler);
    CActiveScheduler::Install( scheduler );
    
    // Initialize shutdown manager and cf listening
    iCfThreadShutdown = CfThreadShutdown::NewL( *this );
    // Notify that critical part of thread init has been succesfully done.
    // Note that ThreadFunctionL() must not leave after call to rendezvous.
    RThread::Rendezvous( KErrNone );
    TRAPD( err, iController.ThreadInitL() ); // Let controller do its own init
    if( err != KErrNone )
        {
        ERROR( err, "CSensorDataCompensatorCfTread::ThreadFunctionL - ThreadInitL failed" );
        }
    CActiveScheduler::Start();
    iController.ThreadRelease(); // Let controller do its own clean up
    delete iCfThreadShutdown; // Must be deleted before thread exit
    iCfThreadShutdown = NULL;
    CleanupStack::PopAndDestroy(scheduler);
    }

// ----------------------------------------------------------------------------
// CSensorDataCompensatorCfTread::StoreValuesAndNotify()
// Run in listener thread context
// ----------------------------------------------------------------------------
void CSensorDataCompensatorCfTread::StoreValuesAndNotify( 
    const CCFActionIndication& aActionToExecute )
    {
    FUNC_LOG;
    
    iMutex.Wait();
    const RKeyValueArray& params = aActionToExecute.Parameters();
    TInt paramCount = params.Count();
    
    CCFKeyValuePair* pair = NULL;
    for( TInt i = 0; i < paramCount; i++ )
        {
        TRAPD( err, pair = CCFKeyValuePair::NewL( params[i]->Key(), params[i]->Value() ) );
        if( err == KErrNone )
            {
            err = iKeyValueArray.Append( pair );
            }
        
        ERROR( err, "CSensorDataCompensatorCfTread::StoreValuesAndNotify - error appending data" );
        }
    iCfTreadNotify->Complete();
    iMutex.Signal();
    }


// ============================= SHUTDOWN FUNCTIONS =============================

// ----------------------------------------------------------------------------
// CSensorDataCompensatorCfTread::CfThreadShutdown::CfThreadShutdown()
// Run in listener thread context
// ----------------------------------------------------------------------------
CSensorDataCompensatorCfTread::CfThreadShutdown::CfThreadShutdown(
    CSensorDataCompensatorCfTread& aWrapper ) :
        CActive( CActive::EPriorityHigh ),
        iWrapper( aWrapper )
    {
    FUNC_LOG;
    }

// ----------------------------------------------------------------------------
// CSensorDataCompensatorCfTread::CfThreadShutdown::NewL()
// Run in listener thread context
// ----------------------------------------------------------------------------
CSensorDataCompensatorCfTread::CfThreadShutdown*
    CSensorDataCompensatorCfTread::CfThreadShutdown::NewL(
        CSensorDataCompensatorCfTread& aWrapper )
    {
    FUNC_LOG;

    CfThreadShutdown* self = new (ELeave) CfThreadShutdown( aWrapper );
    CActiveScheduler::Add( self );
    self->iStatus = KRequestPending;
    self->iShutdownStatus = &self->iStatus;
    self->SetActive();
    return self;
    }

// ----------------------------------------------------------------------------
// CSensorDataCompensatorCfTread::CfThreadShutdown::~CfThreadShutdown()
// Run in listener thread context
// ----------------------------------------------------------------------------
CSensorDataCompensatorCfTread::CfThreadShutdown::~CfThreadShutdown()
    {
    FUNC_LOG;

    Cancel();
    }

// ----------------------------------------------------------------------------
// CSensorDataCompensatorCfTread::CfThreadShutdown::RunL()
// Run in listener thread context
// ----------------------------------------------------------------------------
void CSensorDataCompensatorCfTread::CfThreadShutdown::RunL()
    {
    FUNC_LOG;

    CActiveScheduler::Stop();
    }

// ----------------------------------------------------------------------------
// CSensorDataCompensatorCfTread::CfThreadShutdown::DoCancel()
// Run in listener thread context
// ----------------------------------------------------------------------------
void CSensorDataCompensatorCfTread::CfThreadShutdown::DoCancel()
    {
    FUNC_LOG;

    if ( iShutdownStatus )
        {
        User::RequestComplete( iShutdownStatus, KErrCancel );
        }
    }

// ----------------------------------------------------------------------------
// CSensorDataCompensatorCfTread::CfThreadShutdown::Complete()
// Run in listener thread context
// ----------------------------------------------------------------------------
void CSensorDataCompensatorCfTread::CfThreadShutdown::Complete()
    {
    FUNC_LOG;

    if ( iShutdownStatus )
        {
        iWrapper.iThread.RequestComplete( iShutdownStatus, KErrNone );
        }
    else
        {
        ERROR( KErrGeneral, "CfThreadShutdown::Complete - not active" );
        }
    }
    
// ============================= NOTIFIER FUNCTIONS =============================

// ----------------------------------------------------------------------------
// CSensorDataCompensatorCfTread::CfTreadNotify::CfTreadNotify()
// Run in client thread context
// ----------------------------------------------------------------------------
CSensorDataCompensatorCfTread::CfTreadNotify::CfTreadNotify(
    CSensorDataCompensatorCfTread& aWrapper ) :
        CActive( CActive::EPriorityStandard ),
        iWrapper( aWrapper )
    {
    FUNC_LOG;
    }

// ----------------------------------------------------------------------------
// CSensorDataCompensatorCfTread::CfTreadNotify::NewL()
// Run in client thread context
// ----------------------------------------------------------------------------
CSensorDataCompensatorCfTread::CfTreadNotify*
    CSensorDataCompensatorCfTread::CfTreadNotify::NewL(
        CSensorDataCompensatorCfTread& aWrapper )
    {
    FUNC_LOG;

    CfTreadNotify* self = new (ELeave) CfTreadNotify( aWrapper );
    CActiveScheduler::Add( self );
    self->iStatus = KRequestPending;
    self->iNotifyStatus = &self->iStatus;
    self->SetActive();
    return self;
    }

// ----------------------------------------------------------------------------
// CSensorDataCompensatorCfTread::CfTreadNotify::~CfTreadNotify()
// Run in client thread context
// ----------------------------------------------------------------------------
CSensorDataCompensatorCfTread::CfTreadNotify::~CfTreadNotify()
    {
    FUNC_LOG;

    Cancel();
    }

// ----------------------------------------------------------------------------
// CSensorDataCompensatorCfTread::CfTreadNotify::RunL()
// Run in client thread context
// ----------------------------------------------------------------------------
void CSensorDataCompensatorCfTread::CfTreadNotify::RunL()
    {
    FUNC_LOG;

    iWrapper.iMutex.Wait();
    iStatus = KRequestPending;
    iNotifyStatus = &iStatus;
    SetActive();
    // Trap thread notify so that mutex gets signaled.
    TRAPD( err, iWrapper.iController.NotifyControllerL( iWrapper.iKeyValueArray ) );
    if( err != KErrNone )
        {
        ERROR( err, "CfTreadNotify::RunL - NotifyControllerL failed" );
        }
    iWrapper.iKeyValueArray.ResetAndDestroy();
    iWrapper.iMutex.Signal();
    }

// ----------------------------------------------------------------------------
// CSensorDataCompensatorCfTread::CfTreadNotify::DoCancel()
// Run in client thread context
// ----------------------------------------------------------------------------
void CSensorDataCompensatorCfTread::CfTreadNotify::DoCancel()
    {
    FUNC_LOG;

    if ( iNotifyStatus )
        {
        User::RequestComplete( iNotifyStatus, KErrCancel );
        }
    }

// ----------------------------------------------------------------------------
// CSensorDataCompensatorCfTread::CfTreadNotify::Complete()
// Run in listener thread context
// ----------------------------------------------------------------------------
void CSensorDataCompensatorCfTread::CfTreadNotify::Complete()
    {
    FUNC_LOG;

    if ( iNotifyStatus )
        {
        RThread client;
        TInt err( client.Open( iWrapper.iClientId ) );
        ERROR( err, "CfTreadNotify::Complete - could not open handle to client thread" );
    
        if ( err == KErrNone )
            {
            client.RequestComplete( iNotifyStatus, KErrNone );
            client.Close();
            }
        }
    else
        {
        ERROR( KErrGeneral, "CfTreadNotify::Complete - not active" );
        }
    }

// End of File  

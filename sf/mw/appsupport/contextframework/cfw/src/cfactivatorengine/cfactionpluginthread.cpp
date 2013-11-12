/*
* Copyright (c) 2006-2008 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  CCFActionPlugInThread class implementation.
*
*/


#include "cfactionpluginthread.h"
#include "CFActivatorEngineActionPluginManager.h"
#include "cfactionplugin.h"
#include "cftrace.h"
#include "CFActivatorTimedSchedulerWait.h"
#include "cfactionindication.h"
#include "CFActionPluginImpl.h"
#include "cfactionexecutionhandler.h"



#include <ecom/ecom.h>

// CONSTANTS
static const TInt KExecThreadStackSize  = 0x2000;

static const TInt KSmallTimerValue      = 5000000; // 5 seconds
static const TInt KMediumTimerValue     = 15000000; // 15 seconds
static const TInt KLongTimerValue      = 30000000; // 30 seconds

//----------------------------------------------------------------------------
// CCFActionPlugInThread::CCFActionPlugInThread
//----------------------------------------------------------------------------
//
CCFActionPlugInThread::CCFActionPlugInThread(
	CCFActivatorEngineActionPluginManager& aActionPluginManager,
	CImplementationInformation& aImplInfo )
	: iPlugInUid( aImplInfo.ImplementationUid() ),
    iImplVersion( aImplInfo.Version() ),
	iActionPluginManager( &aActionPluginManager )
	{
    FUNC_LOG;

	}

//----------------------------------------------------------------------------
// CCFActionPlugInThread::CCFActionPlugInThread
//----------------------------------------------------------------------------
//
CCFActionPlugInThread::CCFActionPlugInThread(
	CCFActivatorEngineActionPluginManager& aActionPluginManager,
	TUid aUid,
    TInt aImplVersion )
	: iPlugInUid( aUid ),
    iImplVersion( aImplVersion ),
	iActionPluginManager( &aActionPluginManager )
	{
    FUNC_LOG;

	}

//----------------------------------------------------------------------------
// CCFActionPlugInThread::NewLC
//----------------------------------------------------------------------------
//
CCFActionPlugInThread* CCFActionPlugInThread::NewLC(
	CCFActivatorEngineActionPluginManager& aActionPluginManager,
	CImplementationInformation& aImplInfo )
	{
    FUNC_LOG;

    CCFActionPlugInThread* self = new ( ELeave ) CCFActionPlugInThread(
			aActionPluginManager, aImplInfo);
	CleanupStack::PushL( self );
	self->ConstructL();
	return self;
	}

//----------------------------------------------------------------------------
// CCFActionPlugInThread::NewLC
//----------------------------------------------------------------------------
//
CCFActionPlugInThread* CCFActionPlugInThread::NewLC( 
    CCFActivatorEngineActionPluginManager& aActionPluginManager,
    TUid aUid,
    TInt aImplVersion )
    {
	FUNC_LOG;

    CCFActionPlugInThread* self = new ( ELeave ) CCFActionPlugInThread(
			aActionPluginManager, aUid, aImplVersion );
	CleanupStack::PushL( self );
	self->ConstructL();
	return self;
    }

//----------------------------------------------------------------------------
// CCFActionPlugInThread::NewL
//----------------------------------------------------------------------------
//
CCFActionPlugInThread* CCFActionPlugInThread::NewL(
	CCFActivatorEngineActionPluginManager& aActionPluginManager,
	CImplementationInformation& aImplInfo )
	{
    FUNC_LOG;

	CCFActionPlugInThread* self = CCFActionPlugInThread::NewLC(
		aActionPluginManager, aImplInfo );
	CleanupStack::Pop(); // self;
	return self;
	}

//----------------------------------------------------------------------------
// CCFActionPlugInThread::ConstructL
//----------------------------------------------------------------------------
//
void CCFActionPlugInThread::ConstructL()
	{
    FUNC_LOG;

    User::LeaveIfError( iActionQueueCriticalSection.CreateLocal() );

    User::LeaveIfError( iConstructDestructSemaphore.CreateLocal( 0 ) );

    INFO_1( "Loading action plug-in: %x", iPlugInUid.iUid );
    HEAP_1( "Before loading action plug-in: %x", iPlugInUid.iUid );

    // Store thread id
    iMainThreadId = RThread().Id();

    // Create plug-in thread for executing actions
    User::LeaveIfError( iPluginThread.Create(
        KNullDesC,
        ThreadFunction,
        KExecThreadStackSize,
        NULL,      // use same heap ( equivalent to &User::Allocator() )
        this ) );

    // Execute thread
    iPluginThread.Resume();

    // wait for the plug-in thread to start up
    iConstructDestructSemaphore.Wait();

    // Check thread exit reason and leave, if error occured
    if( iPluginThread.ExitReason() != KErrNone )
        {
        // Something went wrong, leave
        User::Leave( iPluginThread.ExitReason() );
        }

    HEAP_1( "After loading action plug-in: %x", iPlugInUid.iUid );
    }

//----------------------------------------------------------------------------
// CCFActionPlugInThread::~CCFActionPlugInThread
//----------------------------------------------------------------------------
//
CCFActionPlugInThread::~CCFActionPlugInThread()
	{
    FUNC_LOG;

    // set the exit flag
    iExit = ETrue;

    // Cancel async clean up request
    if( iAsyncCleanupRequest )
        {
        iMainThread.RequestComplete( iAsyncCleanupRequest, KErrCancel );
        }

    // signal the plug-in thread (to handle the exit flag)
    if( iWaitActionsRequestStatus )
    	{
    	iPluginThread.RequestComplete( iWaitActionsRequestStatus, KErrNone );
    	}

    // wait for the plug-in thread to terminate itself if the thread is still alive
    if( iPluginThread.ExitType() == EExitPending )
        {
        iConstructDestructSemaphore.Wait();
        }

	iPluginThread.Close();
	iConstructDestructSemaphore.Close();
	}

//----------------------------------------------------------------------------
// CCFActionPlugInThread::ImplementationUid
//----------------------------------------------------------------------------
//
TUid CCFActionPlugInThread::ImplementationUid() const
	{
    FUNC_LOG;

	return iPlugInUid;
	}

//----------------------------------------------------------------------------
// CCFActionPlugInThread::Version
//----------------------------------------------------------------------------
//
TInt CCFActionPlugInThread::Version() const
	{
    FUNC_LOG;

	return iImplVersion;
	}

//----------------------------------------------------------------------------
// CCFActionPlugInThread::SetMissing
//----------------------------------------------------------------------------
//
void CCFActionPlugInThread::SetMissing( TBool aMissing )
    {
    FUNC_LOG;

    iMissing = aMissing;
    }

//------------------------------------------------------------------------------
// CCFActionPlugInThread::Missing
//------------------------------------------------------------------------------
//
TBool CCFActionPlugInThread::Missing() const
    {
    FUNC_LOG;

    return iMissing;
    }

//----------------------------------------------------------------------------
// CCFActionPlugInThread::GetActionsL
//----------------------------------------------------------------------------
//
void CCFActionPlugInThread::GetActionsL( CDesCArray& aActionList ) const
	{
    FUNC_LOG;

	iPlugIn->GetActionsL( aActionList );
	}

//------------------------------------------------------------------------------
// CCFActionPlugInThread::SecurityPolicy
//------------------------------------------------------------------------------
//
const TSecurityPolicy& CCFActionPlugInThread::SecurityPolicy(
    const TDesC& /*aActionId */) const
    {
    FUNC_LOG;

    return iPlugIn->SecurityPolicy();
    }

//------------------------------------------------------------------------------
// CCFActionPlugInThread::ReleasePlugin
//------------------------------------------------------------------------------
//
void CCFActionPlugInThread::ReleasePlugin( TRequestStatus& aStatus )
    {
    FUNC_LOG;

    // Store request
    aStatus = KRequestPending;
    iAsyncCleanupRequest = &aStatus;

    // Complete wait actions request
    if( iWaitActionsRequestStatus )
        {
        iPluginThread.RequestComplete( iWaitActionsRequestStatus, KErrReleasePlugin );
        }
    }

//------------------------------------------------------------------------------
// CCFActionPlugInThread::CancelReleasePlugin
//------------------------------------------------------------------------------
//
void CCFActionPlugInThread::CancelReleasePlugin()
    {
    if( iAsyncCleanupRequest )
        {
        iPluginThread.RequestComplete( iAsyncCleanupRequest, KErrCancel );
        }
    }

//------------------------------------------------------------------------------
// CCFActionPlugInThread::ReloadPlugin
//------------------------------------------------------------------------------
//
void CCFActionPlugInThread::ReloadPlugin()
    {
    FUNC_LOG;

    // Complete wait actions request
    if( iWaitActionsRequestStatus )
        {
        iPluginThread.RequestComplete( iWaitActionsRequestStatus, KErrReloadPlugin );
        }
    }

//------------------------------------------------------------------------------
// CCFActionPlugInThread::ActionOwner
//------------------------------------------------------------------------------
//
MCFActionOwner::TCFActionOwner CCFActionPlugInThread::ActionOwner() const
    {
    FUNC_LOG;

    return MCFActionOwner::ECFInternalAction;
    }

//----------------------------------------------------------------------------
// CCFActionPlugInThread::TriggerL
//----------------------------------------------------------------------------
//
void CCFActionPlugInThread::TriggerL(
    CCFActionIndication* aActionIndication )
    {
    FUNC_LOG;

    // Add action into the queue
    TInt err = AddToQueue( aActionIndication );
    if( err == KErrNone )
        {
        // Double check that we are not releasing the plug-in and the plug-in
        // instance is still valid
        if( !iAsyncCleanupRequest && iPlugIn )
            {
            if( iWaitActionsRequestStatus )
                {
                iPluginThread.RequestComplete( iWaitActionsRequestStatus, KErrNone );
                }
            }
        }
    }

//----------------------------------------------------------------------------
// CCFActionPlugInThread::RunPendingActions
//----------------------------------------------------------------------------
//
void CCFActionPlugInThread::RunPendingActions()
	{
	// wait until all actions are executed

	TRequestStatus request;
	request = KRequestPending;
	iRunPendingActionsRequestStatus = &request;

    if( iWaitActionsRequestStatus )
    	{
    	iPluginThread.RequestComplete( iWaitActionsRequestStatus, KErrNone );
    	}

	User::WaitForRequest( request );
	}


//----------------------------------------------------------------------------
// CCFActionPlugInThread::WaitActions
//----------------------------------------------------------------------------
//
void CCFActionPlugInThread::WaitActions( TRequestStatus& aStatus )
	{
    FUNC_LOG;

    aStatus = KRequestPending;
	iWaitActionsRequestStatus = &aStatus;
	}

//----------------------------------------------------------------------------
// CCFActionPlugInThread::CancelNewActionsWait
//----------------------------------------------------------------------------
//
void CCFActionPlugInThread::CancelActionsWait()
	{
    FUNC_LOG;

    if( iWaitActionsRequestStatus )
    	{
    	iPluginThread.RequestComplete( iWaitActionsRequestStatus, KErrCancel );
    	}
	}

//-----------------------------------------------------------------------------
// CCFActionPlugInThread::AddToQueue
//-----------------------------------------------------------------------------
//
TInt CCFActionPlugInThread::AddToQueue( CCFActionIndication* aAction )
    {
    FUNC_LOG;

    // wait that no other thread is handling the action queue
    iActionQueueCriticalSection.Wait();

    // If we have new action, append it to actions array
    TInt err = iActionQueue.Append( aAction );
    if( err != KErrNone )
        {
        // Clean up
        delete aAction;
        }

    // let other threads handle action queue
    iActionQueueCriticalSection.Signal();

    return err;
    }

//-----------------------------------------------------------------------------
// CCFActionPlugInThread::GetNextActionToExecute
//-----------------------------------------------------------------------------
//
CCFActionIndication* CCFActionPlugInThread::GetNextActionToExecute( )
    {
    FUNC_LOG;

    // wait that no other thread is handling the action queue
    iActionQueueCriticalSection.Wait();

    CCFActionIndication* action = NULL;

    // Check queue if there is a new action waiting
    if( iActionQueue.Count() > 0 )
        {
        action = iActionQueue[0];
        iActionQueue.Remove( 0 );
        }

    // let other threads handle action queue
    iActionQueueCriticalSection.Signal();

    return action;
    }


//----------------------------------------------------------------------------
// CCFActivatorEngineActionPluginManager::ThreadFunction
//----------------------------------------------------------------------------
//
TInt CCFActionPlugInThread::ThreadFunction( TAny* aSelf )
    {
    FUNC_LOG;

    TInt err = KErrNone;

    CCFActionPlugInThread* self =
        static_cast<CCFActionPlugInThread*>( aSelf );

    // Create cleanup stack and complete thread
    CTrapCleanup* cleanupStack = CTrapCleanup::New();
    if( cleanupStack )
        {
        TRAP( err, self->ThreadConstructL() );
        if( err == KErrNone )
        	{
        	TRAP( err, self->ThreadFunctionL() );
        	}
        self->ThreadDestruct();
        }
    delete cleanupStack;

    // Signal main thread that CCFActionPlugInThread can be deleted
    self->iConstructDestructSemaphore.Signal();

    return err;
    }

//----------------------------------------------------------------------------
// CCFActionPlugInThread::ThreadConstructL
//----------------------------------------------------------------------------
//
void CCFActionPlugInThread::ThreadConstructL()
    {
    FUNC_LOG;

    // Create a scheduler for the new thread
    iScheduler = new( ELeave ) CActiveScheduler;
    CActiveScheduler::Install( iScheduler );

    // Open handle to the main thread
    User::LeaveIfError( iMainThread.Open( iMainThreadId ) );

    // Create plugin
    iPlugIn = CCFActionPlugIn::NewL( iPlugInUid );

    CCFActionPlugInImpl* impl = CCFActionPlugInImpl::NewL( iPlugIn, this );
    CCFActivatorEngineActionPluginManager::SetImplPointer( *iPlugIn, impl );

    // Create timed active scheduler wait
    iWait = CCFActivatorTimedSchedulerWait::NewL();

    // Tell plug-in that it can initialize itself
    iPlugIn->InitializeL();

    // create active object taking actions from the queue
    iActionExecutionHandler = CCFActionExecutionHandler::NewL( this );
    iActionExecutionHandler->StartAsyncActionsWait();

    INFO_1( "Loaded action plug-in: %x", iPlugInUid.iUid );
    }


//----------------------------------------------------------------------------
// CCFActionPlugInThread::ThreadDestruct
//----------------------------------------------------------------------------
//
void CCFActionPlugInThread::ThreadDestruct()
    {
    FUNC_LOG;

    delete iWait;
    iWait = NULL;

    delete iCurrentAction;
    iCurrentAction = NULL;

    iActionQueue.ResetAndDestroy();
    iActionQueueCriticalSection.Close();

    if( iPlugIn )
        {
        delete iPlugIn;
        iPlugIn = NULL;

        // Clean up Ecom
        REComSession::FinalClose();
        }

    delete iActionExecutionHandler;
    iActionExecutionHandler = NULL;

    iMainThread.Close();

    // Uninstall scheduler
    CActiveScheduler::Install( NULL );
    delete iScheduler;
    iScheduler = NULL;
    }


//----------------------------------------------------------------------------
// CCFActionPlugInThread::ThreadFunctionL
//----------------------------------------------------------------------------
//
void CCFActionPlugInThread::ThreadFunctionL()
    {
    FUNC_LOG;

    // Signal main thread that plugin loading finished
    iConstructDestructSemaphore.Signal();

    // Start scheduler
    CActiveScheduler::Start();
    }

//----------------------------------------------------------------------------
// CCFActionPlugInThread::NewActionsAddedToQueueL
//----------------------------------------------------------------------------
//
void CCFActionPlugInThread::NewActionsAddedToQueueL()
	{
    FUNC_LOG;

    // Make the new asynchronous wait request so that we will not miss any
    // actions in the queue
    iActionExecutionHandler->StartAsyncActionsWait();

    // check the exit situation
    if( iExit )
		{
	    CActiveScheduler::Stop();
	    INFO_1( "Stopped action plug-in [%x] active scheduler",
	            iPlugInUid.iUid );

	    return;
	    }

    INFO_1( "Executing action plug-in [%x] thread", iPlugInUid.iUid );

    // Empty the queue
	ExecuteActionsInQueue();

	// signal the main thread that we have emptied the queue
	if( iRunPendingActionsRequestStatus )
    	{
    	iMainThread.RequestComplete( iRunPendingActionsRequestStatus, KErrNone );
    	}
	}



//-----------------------------------------------------------------------------
// CCFActionPlugInThread::ExecuteActionsInQueue
//-----------------------------------------------------------------------------
//
void CCFActionPlugInThread::ExecuteActionsInQueue()
    {
    FUNC_LOG;

    //__ASSERT_DEBUG( iPlugIn, Panic( EInvalidPluginPointer ) );

    TInt err = KErrNone;

    CCFActionPlugIn::TExecutionTime exec = CCFActionPlugIn::ENone;
    TInt timerValue = 0;

    // abort if we are already executing an action
    if ( iCurrentAction )
    	{
    	return;
    	}

    iCurrentAction = GetNextActionToExecute();

    do
        {
        // abort immediately if exit wanted
        if( iExit )
        	{
        	return;
        	}
        if( iCurrentAction )
            {
            // Prepare
            INFO( "PrepareExecution for action plug-in implementation" );
            TRAP( err, iPlugIn->PrepareExecutionL() );
            ERROR_1( err, "PrepareExecution failed with code: %d", err );

            // Execute if no errors, cannot continue if prepare fails
            if( err == KErrNone )
                {
                INFO( "ExecuteL for action plug-in implementation" );
                TRAP( err, exec = iPlugIn->ExecuteL( iCurrentAction ) );
                ERROR_1( err, "ExecuteL failed with code: %d", err );
                if( err == KErrNone )
                    {
                    // Wait completion. If request has already been completed
                    // this will return right away.
                    // Otherwise we'll wait here untill async ExecuteL is
                    // finished.
                    switch( exec )
                        {
                        case CCFActionPlugIn::ESmall:
                            {
                            timerValue = KSmallTimerValue;
                            break;
                            }
                        case CCFActionPlugIn::EMedium:
                            {
                            timerValue = KMediumTimerValue;
                            break;
                            }
                        case CCFActionPlugIn::ELong:
                            {
                            timerValue = KLongTimerValue;
                            break;
                            }
                        default:
                            {
                            // Nothing to do
                            break;
                            }
                        }
                    // Start active scheduler wait if we have an async operation.
                    // Callback timer will also be started to remove possible
                    // dead lock situations.
                    if( exec != CCFActionPlugIn::ENone )
                        {
                        INFO( "Started waiting for implementation to finnish async operation" );
                        if( !iWait->IsActive() )
                            {
                            iWait->Start( timerValue );
                            }
                        }
                    INFO( "FinishedExecution for action plug-in implementation" );
                    iPlugIn->FinishedExecution();
                    }
                }

            delete iCurrentAction;
            iCurrentAction = NULL;

            iCurrentAction = GetNextActionToExecute();
            }
        }
    while( iCurrentAction );
    }

//-----------------------------------------------------------------------------
// CCFActionPlugInThread::AsyncExecutionCompleted
//-----------------------------------------------------------------------------
//
void CCFActionPlugInThread::AsyncExecutionCompleted()
    {
    FUNC_LOG;

    iWait->Stop();
    }

//------------------------------------------------------------------------------
// CCFActionPlugInThread::DeletePlugin
//------------------------------------------------------------------------------
//
void CCFActionPlugInThread::DeletePlugin()
    {
    FUNC_LOG;

    // Make the new asynchronous wait request so that we will not miss any
    // actions in the queue
    iActionExecutionHandler->StartAsyncActionsWait();

    // Delete plug-in
    delete iPlugIn;
    iPlugIn = NULL;

    // Clean up Ecom
    REComSession::FinalClose();

    // Complete async delete request
    if( iAsyncCleanupRequest )
        {
        iMainThread.RequestComplete( iAsyncCleanupRequest, KErrNone );
        }
    }

//------------------------------------------------------------------------------
// CCFActionPlugInThread::ConstructPluginL
//------------------------------------------------------------------------------
//
void CCFActionPlugInThread::ConstructPluginL()
    {
    FUNC_LOG;

    // Make the new asynchronous wait request so that we will not miss any
    // actions in the queue
    iActionExecutionHandler->StartAsyncActionsWait();

    // Reload plug-in
    iPlugIn = CCFActionPlugIn::NewL( iPlugInUid );
    CCFActionPlugInImpl* impl = CCFActionPlugInImpl::NewL( iPlugIn, this );
    CCFActivatorEngineActionPluginManager::SetImplPointer( *iPlugIn, impl );

    // Tell plug-in that it can initialize itself
    iPlugIn->InitializeL();
    iActionPluginManager->AddNewActionsToCacheL( *this );

    // Empty queue if there are still actions pending
    if( iWaitActionsRequestStatus )
        {
        iPluginThread.RequestComplete( iWaitActionsRequestStatus, KErrNone );
        }
    }

// End of line

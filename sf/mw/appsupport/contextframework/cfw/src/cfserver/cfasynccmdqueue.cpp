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
* Description:  CCFAsyncCmdQueue implementation.
*
*/


// SYSTEM INCLUDES

// USER INCLUDES
#include "cfasynccmdqueue.h"
#include "cfcmd.h"
#include "cftrace.h"

// ======== MEMBER FUNCTIONS ========

// ---------------------------------------------------------------------------
// C++ constructor.
// ---------------------------------------------------------------------------
//
CCFAsyncCmdQueue::CCFAsyncCmdQueue()
    :   CActive( EPriorityHigh ),
        iQueue( CCFCmd::SglQueLinkOffset() ),
        iQueueIter( iQueue )
    {
    FUNC_LOG;

    CActiveScheduler::Add( this );
    }

// ---------------------------------------------------------------------------
// Symbian two phased constructor.
// ---------------------------------------------------------------------------
//
CCFAsyncCmdQueue* CCFAsyncCmdQueue::NewL( )
    {
    FUNC_LOG;

    CCFAsyncCmdQueue* self = CCFAsyncCmdQueue::NewLC();
    CleanupStack::Pop( self );
    return self;
    }

// ---------------------------------------------------------------------------
// Symbian two phased constructor.
// Leaves pointer in the cleanup stack.
// ---------------------------------------------------------------------------
//
CCFAsyncCmdQueue* CCFAsyncCmdQueue::NewLC()
    {
    FUNC_LOG;

    CCFAsyncCmdQueue* self = new( ELeave ) CCFAsyncCmdQueue();
    CleanupStack::PushL( self );
    return self;
    }

// ---------------------------------------------------------------------------
// C++ destructor.
// ---------------------------------------------------------------------------
//
CCFAsyncCmdQueue::~CCFAsyncCmdQueue()
    {
    FUNC_LOG;

    Cancel();
    if ( !iQueue.IsEmpty() )
        {
        CCFCmd* cmd = NULL;
        iQueueIter.SetToFirst(); 
        while ( ( cmd = iQueueIter++ ) != NULL )
            {
            iQueue.Remove( *cmd );
            delete cmd;
            }
        }
    delete iCurrentCmd;
    }


// ---------------------------------------------------------------------------
// CCFAsyncCmdQueue::Add
// ---------------------------------------------------------------------------
//
void CCFAsyncCmdQueue::Add( CCFCmd* aCmd )
    {
    FUNC_LOG;

    iQueue.AddLast( *aCmd );
    Activate();
    }

// ---------------------------------------------------------------------------
// CCFAsyncCmdQueue::AddFirst
// ---------------------------------------------------------------------------
//
void CCFAsyncCmdQueue::AddFirst( CCFCmd* aCmd )
    {
    FUNC_LOG;

    iQueue.AddFirst( *aCmd );
    Activate();
    }


// ---------------------------------------------------------------------------
// CCFAsyncCmdQueue::DoCancel
// ---------------------------------------------------------------------------
//
void CCFAsyncCmdQueue::DoCancel()
    {
    FUNC_LOG;
    // Nothing to do.
    }

// ---------------------------------------------------------------------------
// CCFAsyncCmdQueue::RunL
// ---------------------------------------------------------------------------
//
void CCFAsyncCmdQueue::RunL()
    {
    FUNC_LOG;

    if ( !iQueue.IsEmpty() )
        {
        iCurrentCmd = iQueue.First();
        iQueue.Remove( *iCurrentCmd );
        iCurrentCmd->ExecuteL();
        delete iCurrentCmd;
        iCurrentCmd = NULL;
        }

    Activate();
    }

// ---------------------------------------------------------------------------
// CCFAsyncCmdQueue::RunError
// ---------------------------------------------------------------------------
//
TInt CCFAsyncCmdQueue::RunError( TInt aError )
    {
    FUNC_LOG;

    if ( iCurrentCmd )
        {
        iCurrentCmd->LogError( aError );
        delete iCurrentCmd;
        iCurrentCmd = NULL;
        }

    ERROR( aError, "CCFAsyncCmdQueue::RunError - Ignoring the error" );

    Activate();

    return KErrNone; // Ignore errors.
    }

// -----------------------------------------------------------------------------
// CCFAsyncCmdQueue::Activate
// -----------------------------------------------------------------------------
//
void CCFAsyncCmdQueue::Activate()
    {
    FUNC_LOG;

    if ( !IsActive() && !iQueue.IsEmpty() )
        {
        iStatus = KRequestPending;
        SetActive();
        TRequestStatus* status = &iStatus;
        User::RequestComplete( status, KErrNone );
        }
    }

// End of file

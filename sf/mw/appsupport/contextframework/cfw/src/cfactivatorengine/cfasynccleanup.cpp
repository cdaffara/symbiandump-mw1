/*
* Copyright (c) 2008-2008 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  CCFAsyncCleanup class implementation.
 *
*/


// SYSTEM INCLUDES

// USER INCLUDES
#include "cfasynccleanup.h"
#include "cftrace.h"
#include "cfasynccleanupcallback.h"
#include "cfactionpluginthread.h"

// ======== MEMBER FUNCTIONS ========

// ---------------------------------------------------------------------------
// C++ constructor.
// ---------------------------------------------------------------------------
//
CCFAsyncCleanup::CCFAsyncCleanup( TBool aUpgrade,
    CCFActionPlugInThread& aThread,
    MCFAsyncCleanupCallback& aCallback ):
    CActive( CActive::EPriorityStandard ),
    iUpgrade( aUpgrade ),
    iThread( aThread ),
    iCallback( aCallback )
    {
    FUNC_LOG;
    
    CActiveScheduler::Add( this );
    }

// ---------------------------------------------------------------------------
// Symbian 2nd phase constructor.
// ---------------------------------------------------------------------------
//
void CCFAsyncCleanup::ConstructL( )
    {
    FUNC_LOG;
    
    // Start cleanup request
    iThread.ReleasePlugin( iStatus );
    SetActive();
    }

// ---------------------------------------------------------------------------
// Symbian two phased constructor.
// ---------------------------------------------------------------------------
//
CCFAsyncCleanup* CCFAsyncCleanup::NewL( TBool aUpgrade,
    CCFActionPlugInThread& aThread,
    MCFAsyncCleanupCallback& aCallback )
    {
    FUNC_LOG;

    CCFAsyncCleanup* self = CCFAsyncCleanup::NewLC( aUpgrade, aThread, aCallback );
    CleanupStack::Pop (self );
    return self;
    }

// ---------------------------------------------------------------------------
// Symbian two phased constructor.
// Leaves pointer in the cleanup stack.
// ---------------------------------------------------------------------------
//
CCFAsyncCleanup* CCFAsyncCleanup::NewLC( TBool aUpgrade,
    CCFActionPlugInThread& aThread,
    MCFAsyncCleanupCallback& aCallback )
    {
    FUNC_LOG;

    CCFAsyncCleanup* self = new( ELeave ) CCFAsyncCleanup( aUpgrade, aThread, aCallback );
    CleanupStack::PushL (self );
    self->ConstructL( );
    return self;
    }

// ---------------------------------------------------------------------------
// C++ destructor.
// ---------------------------------------------------------------------------
//
CCFAsyncCleanup::~CCFAsyncCleanup ( )
    {
    FUNC_LOG;
    
    Cancel();
    }

//------------------------------------------------------------------------------
// CCFAsyncCleanup::Upgrade
//------------------------------------------------------------------------------
//
TBool CCFAsyncCleanup::Upgrade() const
    {
    FUNC_LOG;
    
    return iUpgrade;
    }

//------------------------------------------------------------------------------
// CCFAsyncCleanup::Thread
//------------------------------------------------------------------------------
//
CCFActionPlugInThread& CCFAsyncCleanup::Thread() const
    {
    FUNC_LOG;
    
    return iThread;
    }

//------------------------------------------------------------------------------
// CCFAsyncCleanup::RunL
//------------------------------------------------------------------------------
//
void CCFAsyncCleanup::RunL()
    {
    FUNC_LOG;
    
    TInt err = iStatus.Int();
    if( err == KErrNone )
        {
        if( iUpgrade )
            {
            // Need to still upgrade the plug-in
            iThread.ReloadPlugin();
            }
        }
    
    // Inform activator engine that cleanup has been completed
    iCallback.CleanupCompleted( this );
    }

//------------------------------------------------------------------------------
// CCFAsyncCleanup::DoCancel
//------------------------------------------------------------------------------
//
void CCFAsyncCleanup::DoCancel()
    {
    FUNC_LOG;
    
    iThread.CancelReleasePlugin();
    }

//------------------------------------------------------------------------------
// CCFAsyncCleanup::RunError
//------------------------------------------------------------------------------
//
TInt CCFAsyncCleanup::RunError( TInt /*aError */)
    {
    FUNC_LOG;
    
    return KErrNone;
    }

// End of file

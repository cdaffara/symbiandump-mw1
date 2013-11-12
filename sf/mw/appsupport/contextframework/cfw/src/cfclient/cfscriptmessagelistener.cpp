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
* Description:  CCFScriptMessageListener class implementation.
 *
*/


// SYSTEM INCLUDES
#include <s32mem.h>
#include <cflistener.h>
#include <cfscriptownerinterface.h>

// USER INCLUDES
#include "cfscriptmessagelistener.h"
#include "CFClientSession.h"
#include "cftrace.h"
#include "cfcommon.h"

// CONSTANTS

_LIT( KPanicCat, "CFMSGLIST" );

// LOCAL FUNCTIONS

// Panic codes
enum TPanicReason
    {
    EAlreadyActive,
    EBufferNotCreated
    };

// Panics client thread
void Panic( TPanicReason aReason )
    {
    User::Panic( KPanicCat, aReason );
    }

// ======== MEMBER FUNCTIONS ========

// ---------------------------------------------------------------------------
// C++ constructor.
// ---------------------------------------------------------------------------
//
CCFScriptMessageListener::CCFScriptMessageListener( RCFClientSession& aSession,
    MCFListener& aListener ):
    CActive( EPriorityStandard ),
    iSession( aSession ),
    iListener( aListener ),
    iBufferPtr( NULL, 0, 0 ),
    iBufferSize( 0 ),
    iBufferSizePckg( iBufferSize )
    {
    FUNC_LOG;
    
    CActiveScheduler::Add( this );
    }

// ---------------------------------------------------------------------------
// Symbian 2nd phase constructor.
// ---------------------------------------------------------------------------
//
void CCFScriptMessageListener::ConstructL( )
    {
    FUNC_LOG;
    
    iBuffer = KNullDesC8().AllocL();
    }

// ---------------------------------------------------------------------------
// Symbian two phased constructor.
// ---------------------------------------------------------------------------
//
CCFScriptMessageListener* CCFScriptMessageListener::NewL(
    RCFClientSession& aSession,
    MCFListener& aListener )
    {
    FUNC_LOG;
    
    CCFScriptMessageListener* self = CCFScriptMessageListener::NewLC(
        aSession, aListener );
    CleanupStack::Pop( self );
    return self;
    }

// ---------------------------------------------------------------------------
// Symbian two phased constructor.
// Leaves pointer in the cleanup stack.
// ---------------------------------------------------------------------------
//
CCFScriptMessageListener* CCFScriptMessageListener::NewLC(
    RCFClientSession& aSession,
    MCFListener& aListener )
    {
    FUNC_LOG;
    
    CCFScriptMessageListener* self = new( ELeave ) CCFScriptMessageListener( 
        aSession, aListener );
    CleanupStack::PushL ( self );
    self->ConstructL( );
    return self;
    }

// ---------------------------------------------------------------------------
// C++ destructor.
// ---------------------------------------------------------------------------
//
CCFScriptMessageListener::~CCFScriptMessageListener( )
    {
    FUNC_LOG;
    
    Cancel();
    delete iBuffer;
    if( iDied )
        {
        *iDied = ETrue;
        }
    }

//------------------------------------------------------------------------------
// CCFScriptMessageListener::RequestNotification
//------------------------------------------------------------------------------
//
void CCFScriptMessageListener::RequestNotification()
    {
    FUNC_LOG;
    
    __ASSERT_DEBUG( !IsActive(), Panic( EAlreadyActive ) );
    __ASSERT_DEBUG( iBuffer, Panic( EBufferNotCreated ) );
    
    iBufferPtr.Set( iBuffer->Des() );
    TIpcArgs args( &iBufferPtr, &iBufferSizePckg );
    iSession.SendAsync( EScriptNotificationStart, args, iStatus );
    SetActive();
    }

//------------------------------------------------------------------------------
// CCFScriptMessageListener::RunL
//------------------------------------------------------------------------------
//
void CCFScriptMessageListener::RunL()
    {
    FUNC_LOG;
    
    // Store flag to see if client kills the instance
    TBool died = EFalse;
    iDied = &died;

    TInt status = iStatus.Int();
    switch( status )
        {
        case KErrServerTerminated:
            {
            INFO( "Asynchronous request error: [KErrServerTerminated]" );
            died = ETrue;
            break;
            }
        // Get the latest 
        case KErrTooBig:
            {
            TInt err = KErrNone;
            do
                {
                // Increase buffer and get script ids
                delete iBuffer;
                iBuffer = NULL;
                iBuffer = HBufC8::NewL( iBufferSize );
                
                // Get the content from server side
                iBufferPtr.Set( iBuffer->Des() );
                TIpcArgs args( &iBufferPtr );
                err = iSession.SendSync( EGetScriptNotification, args );
                if( err == KErrNone )
                    {
                    // inform client that scripts have been deregistered
                    NotifyClientL();
                    }
                }
            // Keep getting new script ids until we have received all
            while( err == KErrTooBig );
            break;
            }
        case KErrNone:
            {
            // Everything is ok, notify
            NotifyClientL();
            break;
            }
        }
        
    if( !died )
        {
        // Check if the buffer size has increased over the treshold value
        if( iBufferSize > KMaxBufferSize )
            {
            iBufferSize = 0;
            delete iBuffer;
            iBuffer = NULL;
            iBuffer = KNullDesC8().AllocL();
            }
        
        // Renew request
        RequestNotification();
        iDied = NULL;
        }
    }

//------------------------------------------------------------------------------
// CCFScriptMessageListener::DoCancel
//------------------------------------------------------------------------------
//
void CCFScriptMessageListener::DoCancel()
    {
    FUNC_LOG;
    
    iSession.SendSync( EScriptNotificationCancel );
    }

//------------------------------------------------------------------------------
// CCFScriptMessageListener::RunError
//------------------------------------------------------------------------------
//
TInt CCFScriptMessageListener::RunError( TInt /*aError*/ )
    {
    FUNC_LOG;
    
    // Continue listening
    RequestNotification();
    
    return KErrNone;
    }

//------------------------------------------------------------------------------
// CCFScriptMessageListener::NotifyClientL
//------------------------------------------------------------------------------
//
void CCFScriptMessageListener::NotifyClientL()
    {
    FUNC_LOG;

    TAny* extension = iListener.Extension( KCFScriptOwnerInterfaceUid );
    if( extension )
        {
        MCFScriptOwnerInterface* owner =
            static_cast<MCFScriptOwnerInterface*>( extension );
        
        // Get script ids from the buffer
        RDesReadStream read( iBufferPtr );
        read.PushL();
        TInt count = read.ReadInt16L();
        if( count )
            {
            RArray<TInt> idList;
            CleanupClosePushL( idList );
            for( TInt i = 0; i < count; i++ )
                {
                TInt id = read.ReadInt16L();
                idList.AppendL( id );
                }
            owner->HandleScriptsRemovedL( idList );
            CleanupStack::PopAndDestroy( &idList );
            }
        
        // Clean up
        CleanupStack::PopAndDestroy( &read );
        }
    }

// End of file

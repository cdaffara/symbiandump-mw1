/*
* Copyright (c) 2002-2007 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  CCFContextSubscriptionListener class declaration.
*
*/


#include "cfcontextsubscriptionlistener.h"

#include <e32svr.h>
#include <s32mem.h>

#include "CFContextObjectImpl.h"
#include "CFContextIndicationImpl.h"
#include "cflistener.h"
#include "CFClientSession.h"
#include "cftrace.h"
#include "cfcommon.h"

// CONSTANTS

#ifdef _DEBUG
// Panic category
_LIT( KPanicCat, "CFCLIENT" );

// Panic reason
enum TPanicReason
    {
    ENoBufferCreated,
    EAlreadyReceiving
    };
    
// Local panic function
LOCAL_C void Panic( TInt aCode )
    {
    User::Panic( KPanicCat, aCode );
    }
#endif

// If buffer grows bigger than 1kb it will be deleted
static const TInt KIndicationTreshold = 1024;

// MEMBER FUNCTIONS

CCFContextSubscriptionListener* CCFContextSubscriptionListener::NewL(
    MCFListener& aListener,
    RCFClientSession& aSession )
    {
    FUNC_LOG;
    
    CCFContextSubscriptionListener* self =
        CCFContextSubscriptionListener::NewLC( aListener, aSession );
    CleanupStack::Pop( self );
    
    return self;
    }

CCFContextSubscriptionListener* CCFContextSubscriptionListener::NewLC(
    MCFListener& aListener,
    RCFClientSession& aSession )
    {
    FUNC_LOG;
    
    CCFContextSubscriptionListener* self =
        new( ELeave ) CCFContextSubscriptionListener( aListener, aSession );
    CleanupStack::PushL( self );
    self->ConstructL();
    
    return self;
    }

CCFContextSubscriptionListener::~CCFContextSubscriptionListener()
    {
    FUNC_LOG;
    
    Cancel();
    delete iBuffer;
    if( iDied )
        {
        *iDied = ETrue;
        }
    }

CCFContextSubscriptionListener::CCFContextSubscriptionListener(
    MCFListener& aListener,
    RCFClientSession& aSession ):
    CActive( EPriorityStandard ),
    iSession( aSession ),
    iListener( aListener ),
    iBufferPtr( 0, 0 ),
    iBufferSizePckg( iBufferSize )
    {
    FUNC_LOG;
    
    CActiveScheduler::Add( this );
    }
    
void CCFContextSubscriptionListener::ConstructL()
    {
    FUNC_LOG;
    
    // Initialize buffer into minimum size
    iBuffer = KNullDesC8().AllocL();
    }

// METHODS

//----------------------------------------------------------------------------
// CCFContextSubscriptionListener::ReceiveContextIndications
//----------------------------------------------------------------------------
//
void CCFContextSubscriptionListener::ReceiveContextIndications()
    {
    FUNC_LOG;
    
    __ASSERT_DEBUG( iBuffer, Panic( ENoBufferCreated ) );
    __ASSERT_DEBUG( !IsActive(), Panic( EAlreadyReceiving ) );
    
    iBufferPtr.Set( iBuffer->Des() );
    iBufferPtr.Zero();

    TIpcArgs args( &iBufferPtr, &iBufferSizePckg );
    iSession.SendAsync( EReceiveContextIndicationMessage, args, iStatus );
    SetActive();
    }

//----------------------------------------------------------------------------
// CCFContextSubscriptionListener::RunL
//----------------------------------------------------------------------------
//
void CCFContextSubscriptionListener::RunL()
    {
    FUNC_LOG;
    
    TBool died = EFalse;
    iDied = &died;
    if( iStatus == KErrNone )
        {
        INFO( "Context indication received - Notifying client" );
        
        // Context indication succesfully received
        // Interpret indication from stream and forward to client
        NotifyContextIndicationL();
        }
    else 
    	{
    	TInt err = iStatus.Int();
    	if( err == KErrServerTerminated )
    		{
            ERROR( err, "Cfserver terminated!!!" );
    		died = ETrue;	
    		}
        else if( err == KErrTooBig )
	        {
	        // Buffer too small to receive all data from server
            INFO( "Too small buffer size for context indication" );

            // Increase buffer, receive data and notify client
	        UpdateBufferL();
            INFO( "Context indication receive buffer increased" );

            TIpcArgs args( &iBufferPtr );
            TInt err = iSession.SendSync( EGetContextIndication, args );
	        if( err == KErrNone )
	            {
                INFO( "Context indication received - Notifying client" );
	            // Latest indication succesfully received, inform client
	            NotifyContextIndicationL();
	            }
	        else
	            {
                ERROR( err, "Context indication receive error - Notifying client" );
	            // Some other error occured, inform client
	            NotifyContextIndicationErrorL();
	            }
	        }
	    else
	        {
            ERROR ( err, "Context indication receive error - Notifying client" );
	        // Some other error occured, inform client
	        NotifyContextIndicationErrorL();
	        }
    	}
    if( !died )
        {
        if( iBufferSize > KIndicationTreshold )
            {
            // Zero buffer so that we do not accidentally keep huge amount
            // of memory reserved
            iBufferSize = 0;
            UpdateBufferL();
            }
        INFO( "Succesfully updated buffer - Continue receiving context indications" );
    	ReceiveContextIndications();
    	iDied = NULL;
        }
    	
    }

//----------------------------------------------------------------------------
// CCFContextSubscriptionListener::DoCancel
//----------------------------------------------------------------------------
//
void CCFContextSubscriptionListener::DoCancel()
    {
    FUNC_LOG;
    
    iSession.SendSync( ECancelReceiveContextIndicationMessage );
    }

//----------------------------------------------------------------------------
// CCFContextSubscriptionListener::RunError
//----------------------------------------------------------------------------
//
TInt CCFContextSubscriptionListener::RunError( TInt aError )
    {
    FUNC_LOG;
    
    ERROR( aError, "Context subscription listener RunL error" );
    
    // Minimize buffer since we propably are here due to OOM
    iBufferSize = 0;
    TInt err = aError;
    UpdateBuffer();
    if( iBuffer )
        {
        INFO( "Succesfully updated buffer - Continue receiving context indications" );
        
        // Continue receiving
        ReceiveContextIndications();
        
        INFO( "Notifying client from context indication error" );
        // Notify client about general error
        iListener.HandleContextFrameworkError( MCFListener::EGeneralError,
            KNullDesC,
            KNullDesC );        
        err = KErrNone;
        }
    else
        {
        err = KErrNoMemory;
        }
    
    ERROR( err, "Unable to handle context subscription listener RunL error" );
    
    return err;
    }

//----------------------------------------------------------------------------
// CCFContextSubscriptionListener::NotifyContextIndicationL
//----------------------------------------------------------------------------
//
void CCFContextSubscriptionListener::NotifyContextIndicationL()
    {
    FUNC_LOG;
    
    // Stream indication
    CCFContextIndicationImpl* indication =
        CCFContextIndicationImpl::NewLC( KNullUid );
    RDesReadStream readStream( iBufferPtr );
    readStream.PushL();
    indication->InternalizeL( readStream );
    CleanupStack::PopAndDestroy( &readStream );
    
    // Notify client
    iListener.ContextIndicationL( *indication );
        
    // Clean up
    CleanupStack::PopAndDestroy( indication );
    }

//----------------------------------------------------------------------------
// CCFContextSubscriptionListener::NotifyContextIndicationErrorL
//----------------------------------------------------------------------------
//
void CCFContextSubscriptionListener::NotifyContextIndicationErrorL()
    {
    FUNC_LOG;
    
    // Check error and stream possible context related to subscription
    MCFListener::TCFError error;
    CCFContextObjectImpl* context = CCFContextObjectImpl::NewLC();
    switch( iStatus.Int() )
        {
        case KErrAccessDenied:
            {
            error = MCFListener::ESecurityCheckErrorForContextSubscription;
            if( iBufferSize > iBufferPtr.MaxLength() )
                {
                UpdateBufferL();
                }
            else
                {
                iBufferPtr.SetLength( iBufferSize );
                }
            TIpcArgs args( &iBufferPtr );
            TInt err = iSession.SendSync( EGetContextSubscriptionError, args );
            if( err == KErrNone )
                {
                RDesReadStream readStream( iBufferPtr );
                readStream.PushL();
                context->InternalizeL( readStream );
                CleanupStack::PopAndDestroy( &readStream );
                }
            break;
            }
        default:
            {
            error = MCFListener::EGeneralError;
            break;
            }
        }
        
    // Notify client
    iListener.HandleContextFrameworkError( error,
        context->Source(),
        context->Type() );
    CleanupStack::PopAndDestroy( context );
    }

//----------------------------------------------------------------------------
// CCFContextSubscriptionListener::UpdateBufferL
//----------------------------------------------------------------------------
//
void CCFContextSubscriptionListener::UpdateBufferL()
    {
    FUNC_LOG;

    UpdateBuffer();
    User::LeaveIfNull( iBuffer );
    }

//----------------------------------------------------------------------------
// CCFContextSubscriptionListener::UpdateBuffer
//----------------------------------------------------------------------------
//
void CCFContextSubscriptionListener::UpdateBuffer()
    {
    FUNC_LOG;
    
    delete iBuffer;
    iBuffer = NULL;
    iBuffer = HBufC8::New( iBufferSize );
    if( iBuffer )
        {
        iBufferPtr.Set( iBuffer->Des() );
        }
    }

// End of file

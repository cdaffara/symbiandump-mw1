/*
* Copyright (c) 2006-2006 Nokia Corporation and/or its subsidiary(-ies). 
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


#include <s32mem.h>

#include "CFActionSubscriptionListener.h"
#include "cflistener.h"
#include "CFActionIndicationImpl.h"
#include "cftrace.h"
#include "cfcommon.h"

// CONSTANTS

#ifdef _DEBUG

_LIT( KPanicCat, "ActSubsList" );

enum TPanicCode
    {
    EAlreadyListening
    };
    
LOCAL_C void Panic( TPanicCode aCode )
    {
    User::Panic( KPanicCat, aCode );
    }

#endif

// MEMBER FUNCTIONS

CCFActionSubscriptionListener* CCFActionSubscriptionListener::NewL(
    MCFListener& aCFListener,
    RCFClientSession& aSession )
    {
    FUNC_LOG;
    
    CCFActionSubscriptionListener* self = 
        CCFActionSubscriptionListener::NewLC( aCFListener, aSession );
    CleanupStack::Pop( self );
    
    return self;
    }

CCFActionSubscriptionListener* CCFActionSubscriptionListener::NewLC(
    MCFListener& aCFListener,
    RCFClientSession& aSession )
    {
    FUNC_LOG;
    
    CCFActionSubscriptionListener* self = new( ELeave )
        CCFActionSubscriptionListener( aCFListener, aSession );
    CleanupStack::PushL( self );
    self->ConstructL();
    
    return self;
    }

CCFActionSubscriptionListener::~CCFActionSubscriptionListener()
    {
    FUNC_LOG;
    
    Cancel();
    delete iIndicationBuffer;
    if( iDied )
        {
        *iDied = ETrue;
        }
    }

CCFActionSubscriptionListener::CCFActionSubscriptionListener(
    MCFListener& aCFListener,
    RCFClientSession& aSession ):
    CActive( EPriorityStandard ),
    iSession( aSession ),
    iCFListener( aCFListener ),
    iIndicationBufferSizePckg( iIndicationBufferSize ),
    iIndicationBufferPtr( 0, 0 )
    {
    FUNC_LOG;
    
    CActiveScheduler::Add( this );
    }

void CCFActionSubscriptionListener::ConstructL()
    {
    FUNC_LOG;
    
    iIndicationBuffer = KNullDesC8().AllocL();
    }

// METHODS

//-----------------------------------------------------------------------------
// CCFActionSubscriptionListener::RequestActionIndication
//-----------------------------------------------------------------------------
//
void CCFActionSubscriptionListener::RequestActionIndication()
    {
    FUNC_LOG;
    
    __ASSERT_DEBUG( !IsActive(), Panic( EAlreadyListening ) );
    
    iIndicationBufferPtr.Set( iIndicationBuffer->Des() );
    iIndicationBufferPtr.Zero();

    TIpcArgs args( &iIndicationBufferPtr, &iIndicationBufferSizePckg );
    iSession.SendAsync( EListenActionSubscription, args, iStatus );
    SetActive();
    }

//-----------------------------------------------------------------------------
// CCFActionSubscriptionListener::RunL
//-----------------------------------------------------------------------------
//
void CCFActionSubscriptionListener::RunL()
    {
    FUNC_LOG;
    
    TBool died = EFalse;
    iDied = &died;
    if( iStatus == KErrNone )
        {
        NotifyActionIndicationL();
        }
    else
    	{
    	TInt err = iStatus.Int();
    	
    	if( iStatus == KErrServerTerminated )
    		{
            INFO( "CCFActionSubscriptionListener::RunL - Asynchronous request error: [KErrServerTerminated]" );
    		died = ETrue;	
    		}
        else if( iStatus == KErrTooBig )
	        {
            INFO( "CCFActionSubscriptionListener::RunL - Insufficient buffer size - increasing..." );
	        // Increase buffer and request latest action indication
	        delete iIndicationBuffer;
	        iIndicationBuffer = NULL;
	        iIndicationBuffer = HBufC8::NewL( iIndicationBufferSize );
	        iIndicationBufferPtr.Set( iIndicationBuffer->Des() );
	        
	        // Get latest action indication
            TIpcArgs args( &iIndicationBufferPtr );
            TInt err = iSession.SendSync( EGetActionIndication, args );
	        if( err == KErrNone )
	            {
	            NotifyActionIndicationL();
	            }
	        }
        else
            {
            ERROR ( err, "CCFActionSubscriptionListener::RunL - Asynchronous request-error." );
            }
    	}
    if( !died )
        {
        RequestActionIndication();
        iDied = NULL;
        }
    }

//-----------------------------------------------------------------------------
// CCFActionSubscriptionListener::DoCancel
//-----------------------------------------------------------------------------
//
void CCFActionSubscriptionListener::DoCancel()
    {
    FUNC_LOG;
    
    iSession.SendSync( ECancelListenActionSubscription );
    }

//-----------------------------------------------------------------------------
// CCFActionSubscriptionListener::InterpretActionIndicationLC
//-----------------------------------------------------------------------------
//
CCFActionIndication* CCFActionSubscriptionListener::InterpretActionIndicationLC()
    {
    FUNC_LOG;
    
    CCFActionIndicationImpl* indication = CCFActionIndicationImpl::NewLC();
    TPtr8 indicationBufferPtr = iIndicationBuffer->Des();
    RDesReadStream stream( indicationBufferPtr );
    stream.PushL();
    indication->InternalizeL( stream );
    CleanupStack::PopAndDestroy( &stream );
    
    return indication;
    }

//-----------------------------------------------------------------------------
// CCFActionSubscriptionListener::NotifyActionIndicationL
//-----------------------------------------------------------------------------
//
void CCFActionSubscriptionListener::NotifyActionIndicationL()
    {
    FUNC_LOG;
    
    CCFActionIndication* indication = InterpretActionIndicationLC();
    
    INFO_1( "Notfying CFClient listener from action indication: %S",
        &indication->Identifier() );
    
    iCFListener.ActionIndicationL( *indication );
    CleanupStack::PopAndDestroy( indication );
    }

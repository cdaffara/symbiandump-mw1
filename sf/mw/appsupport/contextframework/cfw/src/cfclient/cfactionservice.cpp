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
* Description:  CCFActionService class implementation.
*
*/


// SYSTEM INCLUDES
#include <s32mem.h>

// USER INCLUDES
#include "cfactionservice.h"
#include "cftrace.h"
#include "cfcommon.h"
#include "CFClientSession.h"
#include "CFActionSubscriptionImpl.h"
#include "CFActionSubscriptionListener.h"

// ======== MEMBER FUNCTIONS ========

// ---------------------------------------------------------------------------
// C++ constructor.
// ---------------------------------------------------------------------------
//
CCFActionService::CCFActionService( RCFClientSession& aSession,
    MCFListener& aListener ) :
    CCFServiceBase( CCFServiceBase::ECFActionService, aSession, aListener )
    {
    FUNC_LOG;
    }

// ---------------------------------------------------------------------------
// Symbian 2nd phase constructor.
// ---------------------------------------------------------------------------
//
void CCFActionService::ConstructL( )
    {
    FUNC_LOG;

    // Create action subscription listener
    iActionSubscriptionListener = CCFActionSubscriptionListener::NewL(
        iListener,
        iSession );
    }

// ---------------------------------------------------------------------------
// Symbian two phased constructor.
// ---------------------------------------------------------------------------
//
CCFActionService* CCFActionService::NewL( RCFClientSession& aSession,
    MCFListener& aListener )
    {
    FUNC_LOG;
    
    CCFActionService* self = CCFActionService::NewLC( aSession, aListener );
    CleanupStack::Pop ( self );
    return self;
    }

// ---------------------------------------------------------------------------
// Symbian two phased constructor.
// Leaves pointer in the cleanup stack.
// ---------------------------------------------------------------------------
//
CCFActionService* CCFActionService::NewLC( RCFClientSession& aSession,
    MCFListener& aListener )
    {
    FUNC_LOG;
    
    CCFActionService* self = new( ELeave ) CCFActionService(
        aSession,
        aListener );
    CleanupStack::PushL ( self );
    self->ConstructL( );
    return self;
    }

// ---------------------------------------------------------------------------
// C++ destructor.
// ---------------------------------------------------------------------------
//
CCFActionService::~CCFActionService( )
    {
    FUNC_LOG;
    
    delete iActionSubscriptionListener;
    }

//----------------------------------------------------------------------------
// CCFActionService::SubscribeActionL
//----------------------------------------------------------------------------
//
void CCFActionService::SubscribeActionL( CCFActionSubscription& aSubscription )
    {
    FUNC_LOG;
    
    TInt err = KErrNone;
    
    // Stream subscription
    HBufC8* buffer = WriteStreamActionSubscriptionLC( aSubscription );
    TIpcArgs args( buffer );
    err = iSession.SendSync( ESubscribeAction, args );
    CleanupStack::PopAndDestroy( buffer );
    
    User::LeaveIfError( err );

    if( err == KErrNone )
        {
        if( !iActionSubscriptionListener->IsActive() )
            {
            // Activate listening
            iActionSubscriptionListener->RequestActionIndication();
            }
        }        
    }
    
//----------------------------------------------------------------------------
// CCFActionService::UnsubscribeActionL
//----------------------------------------------------------------------------
//
void CCFActionService::UnsubscribeActionL(
    CCFActionSubscription& aSubscription )
    {
    FUNC_LOG;

    TInt err = KErrNone;
    
    // Stream subscription
    HBufC8* buffer = WriteStreamActionSubscriptionLC( aSubscription );
    TIpcArgs args( buffer );
    err = iSession.SendSync( EUnsubscribeAction, args );
    CleanupStack::PopAndDestroy( buffer );
    
    User::LeaveIfError( err );
    }

//----------------------------------------------------------------------------
// CCFActionService::DefineAction
//----------------------------------------------------------------------------
//
TInt CCFActionService::DefineAction( const TDesC& aActionIdentifier,
    const TSecurityPolicy& aSecurityPolicy )
    {
    FUNC_LOG;
    
    TSecurityPolicyBuf secPolicyBuf( aSecurityPolicy );
    TIpcArgs args( &aActionIdentifier, &secPolicyBuf );
    TInt err = iSession.SendSync( EDefineAction, args );
    
    return err;
    }

//----------------------------------------------------------------------------
// CCFActionService::WriteStreamActionSubscriptionLC
//----------------------------------------------------------------------------
//
HBufC8* CCFActionService::WriteStreamActionSubscriptionLC(
    CCFActionSubscription& aSubscription )
    {
    FUNC_LOG;
    
    CCFActionSubscriptionImpl& subscription =
        static_cast<CCFActionSubscriptionImpl&>( aSubscription );

    HBufC8* buffer = HBufC8::NewLC( subscription.Size() );
    TPtr8 bufferPtr = buffer->Des();

    RDesWriteStream stream( bufferPtr );
    stream.PushL();
    subscription.ExternalizeL( stream );

    // Cleanup
    CleanupStack::PopAndDestroy( &stream );

    return buffer;
    }

// End of file

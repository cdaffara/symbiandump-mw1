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
* Description:  Message handler context class implementation.
*
*/


#include <s32mem.h>

#include "CFMessageHandlerAction.h"
#include "cfcommon.h"
#include "CFActionInterface.h"
#include "CFActionSubscriptionImpl.h"
#include "CFActionIndicationImpl.h"
#include "CFServSession.h"
#include "cftrace.h"

// CONSTANTS

#ifdef _DEBUG

LOCAL_C void Panic( const RMessage2& aMessage, TInt aCode )
    {
    CCFServSession::PanicClient( aMessage, aCode );
    }

#endif

CCFMessageHandlerAction* CCFMessageHandlerAction::NewL(
	MCFExtendedContextInterface& aCFContext,
    MCFActionInterface& aCFAction,
    MCFScriptInterface& aScriptInterface )
    {
    FUNC_LOG;
    
    CCFMessageHandlerAction* self = CCFMessageHandlerAction::NewLC(
        aCFContext,
        aCFAction,
        aScriptInterface );
    CleanupStack::Pop( self );
    
    return self;
    }

CCFMessageHandlerAction* CCFMessageHandlerAction::NewLC(
	MCFExtendedContextInterface& aCFContext,
    MCFActionInterface& aCFAction,
    MCFScriptInterface& aScriptInterface )
    {
    FUNC_LOG;
    
    CCFMessageHandlerAction* self = new( ELeave ) CCFMessageHandlerAction(
        aCFContext,
        aCFAction,
        aScriptInterface );
    CleanupStack::PushL( self );
    self->ConstructL();
    
    return self;
    }

CCFMessageHandlerAction::~CCFMessageHandlerAction()
    {
    FUNC_LOG;
    
    if( !iSubscriptionListenMessage.IsNull() )
        {
        // Need to complete message
        iSubscriptionListenMessage.Complete( KErrCancel );
        }
    iCFAction.UnsubscribeActions( *this );
    iCFAction.DeregisterActions( this );
    iActionQueue.ResetAndDestroy();
    delete iCurrentAction;
    }

CCFMessageHandlerAction::CCFMessageHandlerAction(
	MCFExtendedContextInterface& aCFContext,
    MCFActionInterface& aCFAction,
    MCFScriptInterface& aScriptInterface ):
    CCFMessageHandlerBase( aCFContext, aCFAction, aScriptInterface )
    {
    FUNC_LOG;
    }
    
void CCFMessageHandlerAction::ConstructL()
    {
    FUNC_LOG;
    }

// METHODS

//-----------------------------------------------------------------------------
// CCFMessageHandlerAction::HandleMessageL
//-----------------------------------------------------------------------------
//
TBool CCFMessageHandlerAction::HandleMessageL( const RMessage2& aMessage )
    {
    FUNC_LOG;

    TBool handled = ETrue;
    switch( aMessage.Function() )
        {
        // Slot[0] = TDesC
        // Slot[1] = TSecurityPolicyBuf
        case EDefineAction:
            {
            INFO( "Handling IPC message [EDefineAction]" );
            
            // Read action identifier
            HBufC* actionId = HBufC::NewLC( aMessage.GetDesLengthL( 0 ) );
            TPtr actionIdPtr = actionId->Des();
            aMessage.ReadL( 0, actionIdPtr );
            
            // Read security policy
            TSecurityPolicyBuf secPolicyBuf;
            aMessage.ReadL( 1, secPolicyBuf );
            TSecurityPolicy secPolicy;
            User::LeaveIfError( secPolicy.Set( secPolicyBuf ) );
            
            // Get client thread
            RThread client;
            aMessage.ClientL( client );
            CleanupClosePushL( client );
            
            // Define action
            TInt err = iCFAction.DefineAction( actionIdPtr, secPolicy,
                client.SecureId(), this );
            
            // Cleanup
            CleanupStack::PopAndDestroy( &client );
            CleanupStack::PopAndDestroy( actionId );
            
            // Complete message and return
            aMessage.Complete( err );
            
            INFO( "Handled IPC message [EDefineAction]" );
            
            break;
            }

        // Slot[0] = TDesC - CCFActionSubscription
        case ESubscribeAction:
            {
            INFO( "Handling IPC message [ESubscribeAction]" );
            
            // Read subscription
            // Start with buffer
            HBufC8* buf = HBufC8::NewLC( aMessage.GetDesLengthL( 0 ) );
            TPtr8 bufPtr = buf->Des();
            aMessage.ReadL( 0, bufPtr );
            
            // Open stream and fetch client thread
            RDesReadStream stream;
            stream.Open( bufPtr );
            stream.PushL();
            RThread client;
            aMessage.ClientL( client );
            CleanupClosePushL( client );
            
            // Create subscription from stream
            CCFActionSubscriptionImpl* subscription = static_cast
                <CCFActionSubscriptionImpl*>( CCFActionSubscription::NewLC() );
            subscription->InternalizeL( stream );
            CleanupStack::Pop( subscription );
            
            // Set subscription
            TInt err = iCFAction.SubscribeAction( subscription, this, client );

            // Cleanup
            CleanupStack::PopAndDestroy( &client );
            CleanupStack::PopAndDestroy( &stream );
            CleanupStack::PopAndDestroy( buf );

            // Complete message and return
            aMessage.Complete( err );

            INFO( "Handled IPC message [ESubscribeAction]" );
            
            break;
            }

        // Slot[0] = TDesC - CCFActionSubscription
        case EUnsubscribeAction:
            {
            INFO( "Handling IPC message [EUnsubscribeAction]" );
            
            // Read subscription
            // Start with buffer
            HBufC8* buf = HBufC8::NewLC( aMessage.GetDesLengthL( 0 ) );
            TPtr8 bufPtr = buf->Des();
            aMessage.ReadL( 0, bufPtr );
            
            // Open stream and fetch client thread
            RDesReadStream stream;
            stream.Open( bufPtr );
            stream.PushL();
            RThread client;
            aMessage.ClientL( client );
            CleanupClosePushL( client );
            
            // Create subscription from stream
            CCFActionSubscriptionImpl* subscription = static_cast
                <CCFActionSubscriptionImpl*>( CCFActionSubscription::NewLC() );
            subscription->InternalizeL( stream );
            
            // Set subscription
            iCFAction.UnsubscribeAction( *subscription, *this );

            // Cleanup
            CleanupStack::PopAndDestroy( subscription );
            CleanupStack::PopAndDestroy( &client );
            CleanupStack::PopAndDestroy( &stream );
            CleanupStack::PopAndDestroy( buf );

            // Complete message and return
            aMessage.Complete( KErrNone );

            INFO( "Handled IPC message [EUnsubscribeAction]" );
            
            break;
            }
            
        // Slot[0] = TDesC - Action indication buffer
        // Slot[1] = TDesC - Buffer size if current too small
        case EListenActionSubscription:
            {
            INFO( "Handling IPC message [EListenActionSubscription]" );
            
            // Store message and check if there is action indication
            // waiting
            iSubscriptionListenMessage = aMessage;
            CheckActionQueueL();

            INFO( "Handled IPC message [EListenActionSubscription]" );
            
            break;
            }
            
        // Slot[0] = TDesC - Action indication buffer
        case EGetActionIndication:
            {
            INFO( "Handling IPC message [EGetActionIndication]" );

            TInt err = KErrNone;
            
            // check that we really have an action indication waiting
            if( iCurrentAction )
                {
                // Indicate current action to client
                // This should not fail since we are here with this op code
                // only if server side has requested client to increase buffer.
                HBufC8* buffer = ExternalizeIndicationLC( *iCurrentAction );
                TPtr8 bufferPtr = buffer->Des();
                
                // Write message
                aMessage.WriteL( 0, bufferPtr );
                
                // Cleanup
                CleanupStack::PopAndDestroy( buffer );
                
                // Current action processed, delete and zero
                delete iCurrentAction;
                iCurrentAction = NULL;
                }
            else
                {
                // No action indications found
                err = KErrNotFound;
                }
            
            // Complete message and return
            aMessage.Complete( err );

            INFO( "Handled IPC message [EGetActionIndication]" );
            
            break;
            }
            
        case ECancelListenActionSubscription:
            {
            INFO( "Handling IPC message [ECancelListenActionSubscription]" );
            
            // Check if subscription message has not been completed yet
            if( !iSubscriptionListenMessage.IsNull() )
                {
                iSubscriptionListenMessage.Complete( KErrCancel );
                }
            aMessage.Complete( KErrNone );

            INFO( "Handled IPC message [ECancelListenActionSubscription]" );
            
            break;
            }
            
        default:
            {
            // Message not handled
            handled = EFalse;
            break;
            }
        }
    
    return handled;
    }

//-----------------------------------------------------------------------------
// CCFMessageHandlerAction::ActionIndicationL
//-----------------------------------------------------------------------------
//
void CCFMessageHandlerAction::ActionIndicationL(
    CCFActionIndication* aIndication )
    {
    FUNC_LOG;
    
    INFO_1( "Received action indication: %S", &aIndication->Identifier() );

    // Append action indication in queue and check queue
    TInt err = iActionQueue.Append( aIndication );
    if( err == KErrNone )
        {
        CheckActionQueueL();
        }
    else
        {
        // Something odd happened, try to serve this indication
        // We still need to have request message pending
        if( !iSubscriptionListenMessage.IsNull() && !iCurrentAction )
            {
            iCurrentAction = aIndication;
            IndicateCurrentActionL();
            }
        else
            {
            // Cannot serve this, delete
            delete aIndication;
            }
        }
    }

//------------------------------------------------------------------------------
// CCFMessageHandlerAction::ActionOwner
//------------------------------------------------------------------------------
//
MCFActionOwner::TCFActionOwner CCFMessageHandlerAction::ActionOwner() const
    {
    FUNC_LOG;
    
    return MCFActionOwner::ECFExternalAction;
    }

//-----------------------------------------------------------------------------
// CCFMessageHandlerAction::CheckActionQueueL
//-----------------------------------------------------------------------------
//
void CCFMessageHandlerAction::CheckActionQueueL()
    {
    FUNC_LOG;
    
    // If we have:
    // - Actions in the queue
    // - Currently not processing any actions
    // - Listen message outstanding
    if( iActionQueue.Count() &&
        !iCurrentAction &&
        !iSubscriptionListenMessage.IsNull() )
        {
        // Get latest action
        iCurrentAction = iActionQueue[0];
        iActionQueue.Remove( 0 );
        
        // Indicate current action to client
        IndicateCurrentActionL();
        }
    }

//-----------------------------------------------------------------------------
// CCFMessageHandlerAction::IndicateCurrentActionL
//-----------------------------------------------------------------------------
//
void CCFMessageHandlerAction::IndicateCurrentActionL()
    {
    FUNC_LOG;
    
    __ASSERT_DEBUG( iCurrentAction,
        Panic( iSubscriptionListenMessage, ENoCurrentAction ) );
    
    // Check client buffer
    TInt err = KErrNone;
    CCFActionIndicationImpl* indication =
        static_cast<CCFActionIndicationImpl*>( iCurrentAction );
    TInt indicationSize = indication->Size();
    TInt clientBufSize = iSubscriptionListenMessage.GetDesMaxLengthL( 0 );
    if( indicationSize > clientBufSize )
        {
        // Client buffer too small
        // Write new buffer size in Slot[1]
        TPckgBuf<TInt> indicationSizeBuf( indicationSize );
        iSubscriptionListenMessage.WriteL( 1, indicationSizeBuf );
        err = KErrTooBig;
        }
    else
        {
        // Client buffer sufficient
        // Stream indication into descriptor
        HBufC8* buffer = ExternalizeIndicationLC( *indication );
        TPtr8 bufferPtr = buffer->Des();
        
        // Write message
        iSubscriptionListenMessage.WriteL( 0, bufferPtr );
        
        // Cleanup
        CleanupStack::PopAndDestroy( buffer );
        delete iCurrentAction;
        iCurrentAction = NULL;
        }
        
    // Complete message
    iSubscriptionListenMessage.Complete( err );
    }

//-----------------------------------------------------------------------------
// CCFMessageHandlerAction::ExternalizeIndicationL
//-----------------------------------------------------------------------------
//
HBufC8* CCFMessageHandlerAction::ExternalizeIndicationLC(
    CCFActionIndication& aIndication )
    {
    FUNC_LOG;
    
    CCFActionIndicationImpl& indication =
        static_cast<CCFActionIndicationImpl&>( aIndication );
    
    HBufC8* buffer = HBufC8::NewLC( indication.Size() );
    TPtr8 bufferPtr = buffer->Des();
    RDesWriteStream stream( bufferPtr );
    stream.PushL();
    indication.ExternalizeL( stream );
    
    // Cleanup
    CleanupStack::PopAndDestroy( &stream );
    
    return buffer;
    }

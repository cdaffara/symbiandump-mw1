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
* Description:  Message handler context class implementation.
*
*/


#include <e32base.h>
#include <s32mem.h>
#include <e32cmn.h>
#include <cfcontextsourcecommand.h>

#include "CFMessageHandlerContext.h"
#include "CFContextObjectImpl.h"
#include "cfextendedcontextinterface.h"
#include "CFScriptInterface.h"
#include "CFContextSubscriptionImpl.h"
#include "cfcontextindication.h"
#include "CFContextIndicationImpl.h"
#include "CFServSession.h"
#include "CFContextQueryImpl.h"
#include "cfcommon.h"
#include "cftrace.h"
#include "CFContextDataProxy.h"
#include "cfserviceutils.h"

// CONSTANTS

#ifdef _DEBUG
LOCAL_C void Panic( const RMessage2& aMessage, TInt aCode )
    {
    CCFServSession::PanicClient( aMessage, aCode );
    }
#endif

// Local function to clean up RContextQueryArray
LOCAL_C void CleanupContextQueryArray( TAny* aArray )
    {
    RContextQueryArray* array = static_cast<RContextQueryArray*>( aArray );
    array->ResetAndDestroy();
    }
    
// Puts RContextQueryArray in clean up stack
LOCAL_C void CleanupResetAndDestroyPushL( RContextQueryArray& aArray )
    {
    TCleanupItem item( CleanupContextQueryArray, &aArray );
    CleanupStack::PushL( item );
    }

// METHODS

CCFMessageHandlerContext* CCFMessageHandlerContext::NewL(
	MCFExtendedContextInterface& aCFContext,
    MCFActionInterface& aCFAction,
    MCFScriptInterface& aScriptInterface )
    {
    FUNC_LOG;
    
    CCFMessageHandlerContext* self = CCFMessageHandlerContext::NewLC(
        aCFContext,
        aCFAction,
        aScriptInterface );
    CleanupStack::Pop( self );
    
    return self;
    }

CCFMessageHandlerContext* CCFMessageHandlerContext::NewLC(
	MCFExtendedContextInterface& aCFContext,
    MCFActionInterface& aCFAction,
    MCFScriptInterface& aScriptInterface )
    {
    FUNC_LOG;
    
    CCFMessageHandlerContext* self = new( ELeave ) CCFMessageHandlerContext(
        aCFContext,
        aCFAction,
        aScriptInterface );
    CleanupStack::PushL( self );
    self->ConstructL();
    
    return self;
    }

CCFMessageHandlerContext::~CCFMessageHandlerContext()
    {
    FUNC_LOG;
    
    // Cancel outsanding request from client
    CancelReceiveContextIndicationMessage();
    
    // Remove all subscriptions from this session
    iCFContext.UnsubscribeContexts( *this );
    iCFContext.DeregisterPublisher( *this );

    // Clean buffers
    iIndicationQueue.ResetAndDestroy();
    TInt count = iSubscriptionErrors.Count();
    for( TInt i = 0; i < count; i++ )
        {
        delete iSubscriptionErrors[i].iContext;
        }
    
    // Close arrays which context are not owned
    iContextRequestBuffer.Close();
    }

CCFMessageHandlerContext::CCFMessageHandlerContext(
	MCFExtendedContextInterface& aCFContext,
    MCFActionInterface& aCFAction,
    MCFScriptInterface& aScriptInterface )
    :   CCFMessageHandlerBase( aCFContext, aCFAction, aScriptInterface )
    {
    FUNC_LOG;
    }
    
void CCFMessageHandlerContext::ConstructL()
    {
    FUNC_LOG;
    }

// METHODS

//-----------------------------------------------------------------------------
// CCFMessageHandlerContext::HandleMessageL
//-----------------------------------------------------------------------------
//
TBool CCFMessageHandlerContext::HandleMessageL( const RMessage2& aMessage )
    {
    FUNC_LOG;
    
    INFO_1( "[MessageHandlerContext] Handling IPC message: %d", aMessage.Function() );

    TBool handled = ETrue;
    switch( aMessage.Function() )
        {
        
        // Slot[0] = TDesC - CCFContextObject
        case EPublishContext:
            {
            INFO( "Handling IPC message [EPublishContext]" );
            
            // Stream context from client buffer
            CCFContextObjectImpl* context = CCFContextObjectImpl::NewLC();
            TInt length = aMessage.GetDesLengthL( 0 );
            HBufC8* buffer = HBufC8::NewLC( length );
            TPtr8 bufferPtr = buffer->Des();
            aMessage.ReadL( 0, bufferPtr );
            RDesReadStream readStream( bufferPtr );
            readStream.PushL();
            context->InternalizeL( readStream );
            
            // Clean up
            CleanupStack::PopAndDestroy( &readStream );
            CleanupStack::PopAndDestroy( buffer );
            
            // Send request to CF Engine
            RThread clientThread;
            aMessage.ClientL( clientThread );
            CleanupClosePushL( clientThread );
            TInt err = PublishContext( *context, clientThread );
            
            // Clean up
            CleanupStack::PopAndDestroy( &clientThread );
            CleanupStack::PopAndDestroy( context );
            
            // Complete
            aMessage.Complete( err );

            INFO( "Handled IPC message [EPublishContext]" );

            break;
            }
            
        // Slot[0] = TDesC - CCFContextSubscription
        case EAddSubscription:
            {
            INFO( "Handling IPC message [EAddSubscription]" );

            // Stream subscription from client buffer
            CCFContextSubscriptionImpl* subscription =
                CCFContextSubscriptionImpl::NewLC();
            TInt length = aMessage.GetDesLengthL( 0 );
            HBufC8* buffer = HBufC8::NewLC( length );
            TPtr8 bufferPtr = buffer->Des();
            aMessage.ReadL( 0, bufferPtr );
            RDesReadStream readStream( bufferPtr );
            readStream.PushL();
            subscription->InternalizeL( readStream );
            
            // Clean up
            CleanupStack::PopAndDestroy( &readStream );
            CleanupStack::PopAndDestroy( buffer );
            
            // Send request to CF Engine
            RThread clientThread;
            aMessage.ClientL( clientThread );
            CleanupClosePushL( clientThread );
            TInt err = AddSubscription( subscription, clientThread );

            // Clean up
            CleanupStack::PopAndDestroy( &clientThread );
            CleanupStack::Pop( subscription );

            // Handle results            
            if( err != KErrNone )
                {
                // Notify error
                DoHandleContextSubscriptionErrorL( err,
                    subscription->ContextSource(),
                    subscription->ContextType() );
                }

            // Complete
            aMessage.Complete( KErrNone );

            INFO( "Handled IPC message [EAddSubscription]" );

            break;
            }
            
        // Slot[0] = TDesC - CCFContextSubscription
        case ERemoveSubscription:
            {
            INFO( "Handling IPC message [ERemoveSubscription]" );

            // Stream subscription from client buffer
            CCFContextSubscriptionImpl* subscription =
                CCFContextSubscriptionImpl::NewLC();
            TInt length = aMessage.GetDesLengthL( 0 );
            HBufC8* buffer = HBufC8::NewLC( length );
            TPtr8 bufferPtr = buffer->Des();
            aMessage.ReadL( 0, bufferPtr );
            RDesReadStream readStream( bufferPtr );
            readStream.PushL();
            subscription->InternalizeL( readStream );
            
            // Clean up
            CleanupStack::PopAndDestroy( &readStream );
            CleanupStack::PopAndDestroy( buffer );

            // Send request to CF Engine
            RemoveSubscription( *subscription );
            
            // Clean up
            CleanupStack::PopAndDestroy( subscription );
            
            // Complete
            aMessage.Complete( KErrNone );

            INFO( "Handled IPC message [ERemoveSubscription]" );

            break;
            }
            
        // Slot[0] = TDes - Buffer
        // Slot[1] = TPckg<TInt> - Buffer size
        case EReceiveContextIndicationMessage:
            {
            INFO( "Handling IPC message [EReceiveContextIndicationMessage]" );

            ReceiveContextIndicationMessageL( aMessage );

            INFO( "Handled IPC message [EReceiveContextIndicationMessage]" );

            break;
            }
            
        // Slot[0] = TDes - Buffer
        // Slot[1] = TPckg<TInt> - Buffer size
        case EReceiveMessage:
            {
            INFO( "Handling IPC message [EReceiveMessage]" );

            ReceiveMessageL( aMessage );

            INFO( "Handled IPC message [EReceiveMessage]" );

            break;
            }
        
        // Slot[0] = TDes - Buffer
        case EGetContextIndication:
            {
            INFO( "Handling IPC message [EGetContextIndication]" );

            // Client has increased buffer size
            // Get next indication from queue
            // Message will be completed in the method
            CompleteNextIndicationFromQueueL( aMessage );

            INFO( "Handled IPC message [EGetContextIndication]" );

            break;
            }

        // Slot[0] = TDes - Buffer
        case EGetMessage:
            {
            INFO( "Handling IPC message [EGetMessage]" );

            // Client has increased buffer size
            // Get next indication from queue
            // Message will be completed in the method
            CompleteNextMessageFromQueueL( aMessage );

            INFO( "Handled IPC message [EGetMessage]" );

            break;
            }
    
            
        // No parameters
        case ECancelReceiveContextIndicationMessage:
            {
            INFO( "Handling IPC message [ECancelReceiveContextIndicationMessage]" );

            CancelReceiveContextIndicationMessage();
            aMessage.Complete( KErrNone );

            INFO( "Handled IPC message [ECancelReceiveContextIndicationMessage]" );

            break;
            }

        // No parameters
        case ECancelReceiveMessage:
            {
            INFO( "Handling IPC message [ECancelReceiveContextIndicationMessage]" );

            CancelReceiveMessage();
            aMessage.Complete( KErrNone );

            INFO( "Handled IPC message [ECancelReceiveContextIndicationMessage]" );

            break;
            }
            
            
        // Slot[0] = TDes - Result buffer
        // Slot[1] = TDesC - CCFContextQuery
        // Slot[2] = TPckg<TInt> - Buffer size
        case ERequestContext:
            {
            INFO( "Handling IPC message [ERequestContext]" );

            TInt err = KErrNone;
            
            // Check if the client has already requested contexts but
            // has failed due too small buffer size
            if( iContextRequestBuffer.Count() )
                {
                // Write request result into client side buffer
                CompleteRequestContextL( aMessage );
                
                // Reset buffer
                iContextRequestBuffer.Reset();
                }
            else
                {
                // New request
                // Stream query
                CCFContextQueryImpl* query =
                    CCFContextQueryImpl::NewLC( KNullDesC, KNullDesC );
                TInt bufferSize = aMessage.GetDesLengthL( 1 );
                HBufC8* buffer = HBufC8::NewLC( bufferSize );
                TPtr8 bufferPtr = buffer->Des();
                aMessage.ReadL( 1, bufferPtr );
                RDesReadStream readStream( bufferPtr );
                readStream.PushL();
                query->InternalizeL( readStream );
                
                // Clean up
                CleanupStack::PopAndDestroy( &readStream );
                CleanupStack::PopAndDestroy( buffer );
                
                // Send request to CF Engine
                RThread clientThread;
                aMessage.ClientL( clientThread );
                CleanupClosePushL( clientThread );
                err = RequestContext( *query, clientThread );
                
                // Clean up
                CleanupStack::PopAndDestroy( &clientThread );
                CleanupStack::PopAndDestroy( query );
                
                // Handle request context results
                if( err == KErrNone && iContextRequestBuffer.Count() )
                    {
                    // Write request result into client side buffer if
                    // request succesfull
                    CompleteRequestContextL( aMessage );
                    }
                else
                    {
                    // Complete with error code
                    aMessage.Complete( err );
                    }
                }

            INFO( "Handled IPC message [ERequestContext]" );

            break;
            }
            
        // Slot[0] = TDes - Result buffer
        // Slot[1] = TDesC - CCFContextQuery
        // Slot[2] = TPckg<TInt> - Buffer size
        case ERequestContextSet:
        	{
            INFO( "Handling IPC message [ERequestContextSet]" );

            TInt err = KErrNone;
            
            // Check if the client has already requested contexts but
            // has failed due too small buffer size
            if( iContextRequestBuffer.Count() )
                {
                // Write request result into client side buffer
                CompleteRequestContextL( aMessage );
                
                // Reset buffer
                iContextRequestBuffer.Reset();
                }
            else
                {
                // New request
                // Stream query
                RContextQueryArray queryArray;
                CleanupResetAndDestroyPushL( queryArray );
                TInt bufferSize = aMessage.GetDesLengthL( 1 );
                HBufC8* buffer = HBufC8::NewLC( bufferSize );
                TPtr8 bufferPtr = buffer->Des();
                aMessage.ReadL( 1, bufferPtr );
                RDesReadStream readStream( bufferPtr );
                readStream.PushL();
                TInt count = readStream.ReadInt16L();
                for( TInt i = 0; i < count; i++ )
                    {
                    CCFContextQueryImpl* query =
                        CCFContextQueryImpl::NewLC( KNullDesC, KNullDesC );
                    query->InternalizeL( readStream );
                    queryArray.AppendL( query );
                    CleanupStack::Pop( query );
                    }
                
                // Clean up
                CleanupStack::PopAndDestroy( &readStream );
                CleanupStack::PopAndDestroy( buffer );
                
                // Send request to CF Engine
                RThread clientThread;
                aMessage.ClientL( clientThread );
                CleanupClosePushL( clientThread );
                count = queryArray.Count();
                for( TInt i = 0; i < count; i++ )
                    {
                    RequestContext( *queryArray[i], clientThread );
                    }
                
                // Clean up
                CleanupStack::PopAndDestroy( &clientThread );
                CleanupStack::PopAndDestroy( &queryArray );
                
                // Handle request context results
                if( err == KErrNone && iContextRequestBuffer.Count() )
                    {
                    // Write request result into client side buffer if
                    // request succesfull
                    CompleteRequestContextL( aMessage );
                    }
                else
                    {
                    // Complete with error code
                    aMessage.Complete( err );
                    }
                }

            INFO( "Handled IPC message [ERequestContextSet]" );

        	break;
        	}
        	
        // Slot[0] = TDesC&
        // Slot[1] = TDesC&
        // Slot[2] = TSecurityPolicyBuf - read policy
        // Slot[3] = TSecurityPolicyBuf - write policy
        case EDefineContext:
            {
            INFO( "Handling IPC message [EDefineContext]" );

            TInt err = KErrNone;
            
            // Read context source
            HBufC* contextSource = HBufC::NewLC( aMessage.GetDesLengthL( 0 ) );
            TPtr contextSourcePtr = contextSource->Des();
            aMessage.ReadL( 0, contextSourcePtr );
            
            // Read context type
            HBufC* contextType = HBufC::NewLC( aMessage.GetDesLengthL( 1 ) );
            TPtr contextTypePtr = contextType->Des();
            aMessage.ReadL( 1, contextTypePtr );
            
            // Read security policy
            TSecurityPolicyBuf readSecPolicyBuf;
            aMessage.ReadL( 2, readSecPolicyBuf );
            TSecurityPolicy readSecPolicy;
            User::LeaveIfError( readSecPolicy.Set( readSecPolicyBuf ) ); // verifies the security policy
            
            TSecurityPolicyBuf writeSecPolicyBuf;
            aMessage.ReadL( 3, writeSecPolicyBuf );
            TSecurityPolicy writeSecPolicy;
            User::LeaveIfError( writeSecPolicy.Set( writeSecPolicyBuf ) ); // verifies the security policy
            
            
            RThread clientThread;
            aMessage.ClientL( clientThread );
            CleanupClosePushL( clientThread );

            // Define context in CF
            err = iCFContext.DefineContext( contextSourcePtr,
                contextTypePtr,
                readSecPolicy,
                writeSecPolicy,
                this,
                clientThread.SecureId() );

            // Clean up
            CleanupStack::PopAndDestroy( &clientThread );

            // Cleanup
            CleanupStack::PopAndDestroy( contextType );
            CleanupStack::PopAndDestroy( contextSource );
            
            aMessage.Complete( err );

            INFO( "Handled IPC message [EDefineContext]" );

            break;
            }
            
        // Slot[0] = TDes - Buffer
        case EGetContextSubscriptionError:
            {
            INFO( "Handling IPC message [EGetContextSubscriptionError]" );
            
            // Check that we actually have errors in the list
            TInt err = KErrNone;
            if( iSubscriptionErrors.Count() )
                {
                // We have a context subscription error, send it to client
                CCFContextObjectImpl* context = iSubscriptionErrors[0].iContext;
                iSubscriptionErrors.Remove( 0 );
                CleanupStack::PushL( context );
                HBufC8* buffer = HBufC8::NewLC( aMessage.GetDesMaxLengthL( 0 ) );
                TPtr8 bufferPtr = buffer->Des();
                RDesWriteStream writeStream( bufferPtr );
                writeStream.PushL();
                context->ExternalizeL( writeStream );
                
                // Write content into client side buffer
                aMessage.WriteL( 0, bufferPtr );
                
                // Clean up
                CleanupStack::PopAndDestroy( &writeStream );
                CleanupStack::PopAndDestroy( buffer );
                CleanupStack::PopAndDestroy( context );
                }
            else
                {
                // No errors found
                err = KErrNotFound;
                }
            
            // Complete
            aMessage.Complete( err );

            INFO( "Handled IPC message [EGetContextSubscriptionError]" );

            break;
            }
            
        // Slot[0]= TDesC - Context
        // Slot[1]= TDesC - Context data
        // Slot[2]= TPckg<TUid> - Context data UID
        case EPublishContextWithData:
            {
            INFO( "Handling IPC message [EPublishContextWithData]" );
            
            // Get context
            CCFContextObjectImpl* context = CCFContextObjectImpl::NewLC();
            TInt len = aMessage.GetDesLengthL( 0 );
            HBufC8* contextBuf = HBufC8::NewLC( len );
            TPtr8 contextBufPtr( contextBuf->Des() );
            aMessage.ReadL( 0, contextBufPtr );
            RDesReadStream readStream( contextBufPtr );
            readStream.PushL();
            context->InternalizeL( readStream );
            CleanupStack::PopAndDestroy( &readStream );
            CleanupStack::PopAndDestroy( contextBuf );
            
            // Get context data
            CCFContextDataProxy* contextData = CCFContextDataProxy::NewLC();
            len = aMessage.GetDesLengthL( 1 );
            HBufC8* contextDataBuf = HBufC8::NewLC( len );
            TPtr8 contextDataBufPtr( contextDataBuf->Des() );
            aMessage.ReadL( 1, contextDataBufPtr );
            TPckgBuf<TUid> uidPckgBuf;
            aMessage.ReadL( 2, uidPckgBuf );

            contextData->SetContextData( contextDataBuf );
            contextData->SetContextDataUid( uidPckgBuf() );
            CleanupStack::Pop( contextDataBuf );
            
            // Publish context
            RThread clientThread;
            aMessage.ClientL( clientThread );
            CleanupClosePushL( clientThread );
            TInt err = iCFContext.PublishContext( *context,
                *contextData,
                clientThread );
            CleanupStack::PopAndDestroy( &clientThread );
            
            // Clean up
            CleanupStack::PopAndDestroy( contextData );
            CleanupStack::PopAndDestroy( context );
            
            // Complete
            aMessage.Complete( err );
            
            INFO( "Handled IPC message [EPublishContextWithData]" );
            
            break;
            }
            
        // Slot[0] = RFs handle
        // Slot[1] = RFile handle
        // Slot[2] = TPckgC< TUid > - Context Source UID
        case EInstallSourceSetting:
            {
            RFile settingFile;   
            TInt err = settingFile.AdoptFromClient( aMessage, 0, 1 );
            CleanupClosePushL( settingFile );           // CLEANUP<< settingFile
            if ( err == KErrNone )
                {
                TPckgBuf< TUid > uidPckgBuf;
                aMessage.ReadL( 2, uidPckgBuf );

                RThread client;
                aMessage.ClientL( client );

                // Install setting
                err = iCFContext.InstallContextSourceSetting( settingFile,
                        uidPckgBuf(),
                        client );

                client.Close();
                }
            CleanupStack::PopAndDestroy( &settingFile );// CLEANUP>> settingFile
            aMessage.Complete( err );

            break;
            }

        // Slot[0] = TDesC& - Setting filename
        // Slot[1] = TPckgC< TUid > - Context Source UID
        case EUninstallSourceSetting:
            {
            // Read setting filename                    // CLEANUP<< filename
            HBufC* filename = HBufC::NewLC( aMessage.GetDesLengthL( 0 ) );
            TPtr filenamePtr = filename->Des();
            aMessage.ReadL( 0, filenamePtr );

            TPckgBuf< TUid > uidPckgBuf;
            aMessage.ReadL( 1, uidPckgBuf );

            RThread client;
            aMessage.ClientL( client );

            // Uninstall setting
            TInt err = iCFContext.UninstallContextSourceSetting( *filename,
                    uidPckgBuf(),
                    client );

            client.Close();
            CleanupStack::PopAndDestroy( filename );    // CLEANUP>> filename
            aMessage.Complete( err );

            break;
            }

        // Slot[0] = TPckgC< TUid > - Context Source UID
        case EUninstallSourceSettings:
            {
            // Read context source UID
            TPckgBuf< TUid > uidPckgBuf;
            aMessage.ReadL( 0, uidPckgBuf );

            RThread client;
            aMessage.ClientL( client );

            // Uninstall setting
            TInt err = iCFContext.UninstallContextSourceSettings( uidPckgBuf(),
                    client );

            client.Close();
            aMessage.Complete( err );

            break;
            }

        default:
            {
            // Message not handled
            handled = EFalse;
            }
        }

    INFO_1( "Handled IPC message: %d", handled );

    return handled;
    }

//-----------------------------------------------------------------------------
// CCFMessageHandlerContext::ContextIndicationL
//-----------------------------------------------------------------------------
//
void CCFMessageHandlerContext::ContextIndicationL(
    CCFContextIndication* aIndication )
    {
    FUNC_LOG;
    
    INFO_2( "Received context indication: %S: %S",
        &aIndication->Context().Source(),
        &aIndication->Context().Type() );
    
    CCFContextIndicationImpl* indication =
        static_cast<CCFContextIndicationImpl*>( aIndication );
    iIndicationQueue.AppendL( indication );
    CheckContextIndicationQueueL();    
    }

//-----------------------------------------------------------------------------
// CCFMessageHandlerContext::Client
//-----------------------------------------------------------------------------
//
TInt CCFMessageHandlerContext::Client( RThread& aThread ) const
    {
    FUNC_LOG;
    
    TInt err = KErrNone;
    if( !iReceiveMessage.IsNull() )
        {
        // We have a message pending
        err = iReceiveMessage.Client( aThread );
        }
    else
        {
        err = KErrNotFound;
        }
    
    return err;
    }

//-----------------------------------------------------------------------------
// CCFMessageHandlerContext::HandleContextSubscriptionError
//-----------------------------------------------------------------------------
//
void CCFMessageHandlerContext::HandleContextSubscriptionError( TInt aError,
    const TDesC& aSource,
    const TDesC& aType )
    {
    FUNC_LOG;
    
    // Client will not get notified about error if this fails
    // Failure only due to OOM
    TRAP_IGNORE( DoHandleContextSubscriptionErrorL( aError, aSource, aType ) );
    }


//-----------------------------------------------------------------------------
// CCFMessageHandlerContext::Subscribers
//-----------------------------------------------------------------------------
//
// from MCFContextSource
void CCFMessageHandlerContext::Subscribers( const TDesC& aContextSource,
		const TDesC& aContextType )
	{
    FUNC_LOG;

    INFO_2( "Received subscribers indication: %S: %S",
            &aContextSource,
            &aContextType );
    
    TRAPD( err, SubscribersL( aContextSource, aContextType ) );
    if( err != KErrNone )
        {
        ERROR_2( err, "Failed to notify subscribers to client: [%S, %S]",
            &aContextSource, &aContextType );
        }
    }

//-----------------------------------------------------------------------------
// CCFMessageHandlerContext::PublishContext
//-----------------------------------------------------------------------------
//
// from MCFContextSource
void CCFMessageHandlerContext::NoSubscribers( const TDesC& aContextSource,
		const TDesC& aContextType )
	{
    FUNC_LOG;

    INFO_2( "Received no subscribers indication: %S: %S",
            &aContextSource,
            &aContextType );
    
    TRAPD( err, NoSubscribersL( aContextSource, aContextType ) );
    if( err != KErrNone )
        {
        ERROR_2( err, "Failed to notify no subscribers to client: [%S, %S]",
            &aContextSource, &aContextType );
        }
	}

//-----------------------------------------------------------------------------
// CCFMessageHandlerContext::HandleCommand
//-----------------------------------------------------------------------------
//
void CCFMessageHandlerContext::HandleCommand(
    const CCFContextSourceCommand& aCommand )
    {
    FUNC_LOG;

    TPtrC commandName( aCommand.Name() );
    INFO_1( "Received handle command action: %S", &commandName );
    
    TRAPD( err, HandleCommandL( aCommand ) );
    if( err != KErrNone )
        {
        ERROR_2( err, "Failed to notify handle command action [%S] to client: [%x]",
                &commandName,
                aCommand.SourceUid() );
        }
    }

//-----------------------------------------------------------------------------
// CCFMessageHandlerContext::PublishContext
//-----------------------------------------------------------------------------
//
TInt CCFMessageHandlerContext::PublishContext( CCFContextObject& aContext, 
    RThread& aClientThread )
    {
    FUNC_LOG;
    
    return iCFContext.PublishContext( aContext, aClientThread );
    }

//-----------------------------------------------------------------------------
// CCFMessageHandlerContext::AddSubscription
//-----------------------------------------------------------------------------
//
TInt CCFMessageHandlerContext::AddSubscription( CCFContextSubscription* aSubscription,
    RThread& aClientThread )
    {
    FUNC_LOG;
    
    return iCFContext.SubscribeContext( aSubscription, this, aClientThread );
    }

//-----------------------------------------------------------------------------
// CCFMessageHandlerContext::RemoveSubscriptionL
//-----------------------------------------------------------------------------
//
void CCFMessageHandlerContext::RemoveSubscription(
    CCFContextSubscription& aSubscription )
    {
    FUNC_LOG;
    
    iCFContext.UnsubscribeContext( aSubscription, *this );
    }

//-----------------------------------------------------------------------------
// CCFMessageHandlerContext::CancelReceiveContextIndicationMessage
//-----------------------------------------------------------------------------
//
void CCFMessageHandlerContext::CancelReceiveContextIndicationMessage()
    {
    FUNC_LOG;
    
    if( !iReceiveMessage.IsNull() )
        {
        iReceiveMessage.Complete( KErrCancel );
        }
    }

//-----------------------------------------------------------------------------
// CCFMessageHandlerContext::CancelReceiveMessage
//-----------------------------------------------------------------------------
//
void CCFMessageHandlerContext::CancelReceiveMessage()
    {
    FUNC_LOG;
    
    if( !iReceiveContextSourceMsg.IsNull() )
        {
        iReceiveContextSourceMsg.Complete( KErrCancel );
        }
    }


//-----------------------------------------------------------------------------
// CCFMessageHandlerContext::RequestContext
//-----------------------------------------------------------------------------
//
TInt CCFMessageHandlerContext::RequestContext( CCFContextQuery& aContextQuery,
    RThread& aClientThread )
    {
    FUNC_LOG;
    
    // request the context
    return iCFContext.RequestContext( iContextRequestBuffer,
        aContextQuery,
        aClientThread );
    }

//-----------------------------------------------------------------------------
// CCFMessageHandlerContext::ReceiveContextIndicationMessageL
//-----------------------------------------------------------------------------
//
void CCFMessageHandlerContext::ReceiveContextIndicationMessageL(
    const RMessage2& aMessage)
    {
    FUNC_LOG;
    
    __ASSERT_DEBUG( iReceiveMessage.IsNull(),
        CCFServSession::PanicClient( aMessage, EReceiveReceiveAlreadyActive ) );

    // Store message    
    iReceiveMessage = aMessage;
    
    // First check if we have subscription errors in the queue
    if( iSubscriptionErrors.Count() )
        {
        IndicateSubscriptionErrorL();
        }
    
    // Check indications
    else if( iIndicationQueue.Count() )
        {
        CheckContextIndicationQueueL();
        }
    }


//-----------------------------------------------------------------------------
// CCFMessageHandlerContext::ReceiveMessageL
//-----------------------------------------------------------------------------
//
void CCFMessageHandlerContext::ReceiveMessageL( const RMessage2& aMessage )
    {
    FUNC_LOG;
    
    __ASSERT_DEBUG( iReceiveContextSourceMsg.IsNull(),
        CCFServSession::PanicClient( aMessage, EReceiveReceiveAlreadyActive ) );

    // Store message    
    iReceiveContextSourceMsg = aMessage;
    
    // Check if we have messages in the queue
    if( iMessageQueue.Count() )
        {
        CheckMessageQueueL();
        }
    }



//-----------------------------------------------------------------------------
// CCFMessageHandlerContext::IndicateSubscriptionErrorL
//-----------------------------------------------------------------------------
//
void CCFMessageHandlerContext::IndicateSubscriptionErrorL()
    {
    FUNC_LOG;

    // Get next error, client will request the context related to the error
    TSubscriptionError error = iSubscriptionErrors[0];
    TPckgBuf<TInt> sizeBuf( error.iContext->Size() );
    iReceiveMessage.WriteL( 1, sizeBuf );
    iReceiveMessage.Complete( error.iError );
    }

//-----------------------------------------------------------------------------
// CCFMessageHandlerContext::CompleteNextIndicationFromQueueL
//-----------------------------------------------------------------------------
//
void CCFMessageHandlerContext::CompleteNextIndicationFromQueueL(
    const RMessage2& aMessage )
    {
    FUNC_LOG;

    // Leave if the aMessage is null
    if( aMessage.IsNull() )
        {
        User::Leave( KErrArgument );
        }
    
    // Check that we have indications in queue
    else if( iIndicationQueue.Count() )
        {
        CCFContextIndicationImpl* indication = iIndicationQueue[0];
        HBufC8* buffer = HBufC8::NewLC( indication->Size() );
        TPtr8 bufferPtr = buffer->Des();
        RDesWriteStream writeStream( bufferPtr );
        writeStream.PushL();
        indication->ExternalizeL( writeStream );
        CleanupStack::PopAndDestroy( &writeStream );
        
        // Write buffer into client side and complete
        aMessage.WriteL( 0, bufferPtr );
        aMessage.Complete( KErrNone );

        // Remove indication from queue
        delete indication;
        iIndicationQueue.Remove( 0 );
        
        // Clean up
        CleanupStack::PopAndDestroy( buffer );
        }
    }

//-----------------------------------------------------------------------------
// CCFMessageHandlerContext::CheckContextIndicationQueueL
//-----------------------------------------------------------------------------
//
void CCFMessageHandlerContext::CheckContextIndicationQueueL()
    {
    FUNC_LOG;
    
    // We must have something in the queue and receive message pending
    if( iIndicationQueue.Count() && !iReceiveMessage.IsNull() )
        {
        // First check that client side buffer is big enough
        CCFContextIndicationImpl* indication = iIndicationQueue[0];
        TInt clientBufferSize = iReceiveMessage.GetDesMaxLengthL( 0 );
        TInt indicationSize = indication->Size();
        if( indicationSize <= clientBufferSize )
            {
            // Client has sufficient buffer
            CompleteNextIndicationFromQueueL( iReceiveMessage );
            }
        else
            {
            // Client has insufficient buffer size
            // Write new buffer size in client memory and complete
            TPckgBuf<TInt> sizeBuf( indicationSize );
            iReceiveMessage.WriteL( 1, sizeBuf );
            iReceiveMessage.Complete( KErrTooBig );
            }
        }
    }

//-----------------------------------------------------------------------------
// CCFMessageHandlerContext::CompleteNextMessageFromQueueL
//-----------------------------------------------------------------------------
//
void CCFMessageHandlerContext::CompleteNextMessageFromQueueL(
    const RMessage2& aMessage )
    {
    FUNC_LOG;
    
    __ASSERT_DEBUG( iMessageQueue.Count(), Panic( aMessage, KErrNotFound ) );
    __ASSERT_DEBUG( !aMessage.IsNull(), Panic( aMessage, KErrArgument ) );

    if( iMessageQueue.Count() )
        {
        // Write buffer into client side and complete
        HBufC8* messageBuffer = iMessageQueue[0];
        aMessage.WriteL( 0, *messageBuffer );
        aMessage.Complete( KErrNone );

        // Remove indication from queue
        delete messageBuffer;
        iMessageQueue.Remove( 0 );
        }
    }


//-----------------------------------------------------------------------------
// CCFMessageHandlerContext::CheckMessageQueueL
//-----------------------------------------------------------------------------
//
void CCFMessageHandlerContext::CheckMessageQueueL()
    {
    FUNC_LOG;
    
    // We must have something in the queue and receive message pending
    if( iMessageQueue.Count() && !iReceiveContextSourceMsg.IsNull() )
        {
        // First check that client side buffer is big enough
        TInt clientBufferSize = iReceiveContextSourceMsg.GetDesMaxLengthL( 0 );
        TInt indicationSize = iMessageQueue[0]->Size();
        if( indicationSize <= clientBufferSize )
            {
            // Client has sufficient buffer
            // Write buffer into client side and complete
            HBufC8* messageBuffer = iMessageQueue[0];
            iReceiveContextSourceMsg.WriteL( 0, *messageBuffer );
            iReceiveContextSourceMsg.Complete( KErrNone );

            // Remove indication from queue
            delete messageBuffer;
            iMessageQueue.Remove( 0 );

            }
        else
            {
            // Client has insufficient buffer size
            // Write new buffer size in client memory and complete
            TPckgBuf<TInt> sizeBuf( indicationSize );
            iReceiveContextSourceMsg.WriteL( 1, sizeBuf );
            iReceiveContextSourceMsg.Complete( KErrTooBig );
            }
        }
    }



//-----------------------------------------------------------------------------
// CCFMessageHandlerContext::CompleteRequestContextL
//-----------------------------------------------------------------------------
//
void CCFMessageHandlerContext::CompleteRequestContextL(
    const RMessage2& aMessage )
    {
    FUNC_LOG;
    
    // Check if client has a sufficient buffer
    TInt resultArraySize = 0;
    resultArraySize += sizeof( TInt );
    TInt count = iContextRequestBuffer.Count();
    CCFContextObjectImpl* context = NULL;
    for( TInt i = 0; i < count; i++ )
        {
        context = static_cast<CCFContextObjectImpl*>(
            iContextRequestBuffer[i] );
        resultArraySize += context->Size();
        }
    TInt clientBufferSize = aMessage.GetDesMaxLengthL( 0 );
    if( resultArraySize <= clientBufferSize )
        {
        // Client has a sufficient buffer
        HBufC8* buffer = HBufC8::NewLC( resultArraySize );
        TPtr8 bufferPtr = buffer->Des();
        RDesWriteStream writeStream( bufferPtr );
        writeStream.PushL();
        writeStream.WriteInt16L( count );
        for( TInt i = 0; i < count; i++ )
            {
            context = static_cast<CCFContextObjectImpl*>(
                iContextRequestBuffer[i] );
            context->ExternalizeL( writeStream );
            }
        writeStream.CommitL();
        aMessage.WriteL( 0, bufferPtr );
        
        // Clean up
        CleanupStack::PopAndDestroy( &writeStream );
        CleanupStack::PopAndDestroy( buffer );
        
        // Reset context request buffer
        iContextRequestBuffer.Reset();

        // Complete message
        aMessage.Complete( KErrNone );
        }
    else
        {
        // Client has an insufficient buffer
        TPckgBuf<TInt> resultArraySizeBuf( resultArraySize );
        aMessage.WriteL( 2, resultArraySizeBuf );
        
        // Complete message
        aMessage.Complete( KErrTooBig );
        }
    }

//-----------------------------------------------------------------------------
// CCFMessageHandlerContext::DoHandleContextSubscriptionErrorL
//-----------------------------------------------------------------------------
//
void CCFMessageHandlerContext::DoHandleContextSubscriptionErrorL( TInt aError,
    const TDesC& aSource,
    const TDesC& aType )
    {
    FUNC_LOG;
    
    // Context related to error
    CCFContextObjectImpl* context = CCFContextObjectImpl::NewLC();
    context->SetSourceL( aSource );
    context->SetTypeL( aType );
    
    // Append error to queue
    TSubscriptionError error;
    error.iError = aError;
    error.iContext = context;
    
    // Ignore append errors
    iSubscriptionErrors.AppendL( error );
    CleanupStack::Pop( context );
    if( !iReceiveMessage.IsNull() )
        {
        // Can only fail due OOM
        IndicateSubscriptionErrorL();
        }
    }

//------------------------------------------------------------------------------
// CCFMessageHandlerContext::SubscribersL
//------------------------------------------------------------------------------
//
void CCFMessageHandlerContext::SubscribersL( const TDesC& aContextSource,
    const TDesC& aContextType )
    {
    FUNC_LOG;

    // calculate message length
    TInt size = 0;
    size += sizeof( TInt ); // message type
    size += sizeof( TInt ) + aContextSource.Size();
    size += sizeof( TInt ) + aContextType.Size();
    
    // create a required buffer and stream
    HBufC8* buffer = HBufC8::NewLC( size ); // CLEANUP << buffer
    TPtr8 bufferPtr = buffer->Des();
    RDesWriteStream writeStream( bufferPtr );
    writeStream.PushL();     // CLEANUP << writeStream
    
    // write message to the buffer
    writeStream.WriteInt32L( ESubscribers );
    writeStream.WriteInt32L( aContextSource.Length() );
    writeStream.WriteL( aContextSource, aContextSource.Length() );
    writeStream.WriteInt32L( aContextType.Length() );
    writeStream.WriteL( aContextType, aContextType.Length() );
    writeStream.CommitL();
    
    CleanupStack::PopAndDestroy( &writeStream );  // CLEANUP >> writeStream
    
    // append pointer to the message array
    iMessageQueue.AppendL( buffer );
    CleanupStack::Pop( buffer );   // CLEANUP >> buffer

    CheckMessageQueueL();
    }

//------------------------------------------------------------------------------
// CCFMessageHandlerContext::NoSubscribersL
//------------------------------------------------------------------------------
//
void CCFMessageHandlerContext::NoSubscribersL( const TDesC& aContextSource,
    const TDesC& aContextType )
    {
    FUNC_LOG;

    // calculate message length
    TInt size = 0;
    size += sizeof( TInt ); // message type
    size += sizeof( TInt ) + aContextSource.Size();
    size += sizeof( TInt ) + aContextType.Size();
    
    // create a required buffer and stream
    HBufC8* buffer = HBufC8::NewLC( size ); // CLEANUP << buffer
    TPtr8 bufferPtr = buffer->Des();
    RDesWriteStream writeStream( bufferPtr );
    writeStream.PushL();     // CLEANUP << writeStream
    
    // write message to the buffer
    writeStream.WriteInt32L( ENoSubscribers );
    writeStream.WriteInt32L( aContextSource.Length() );
    writeStream.WriteL( aContextSource, aContextSource.Length() );
    writeStream.WriteInt32L( aContextType.Length() );
    writeStream.WriteL( aContextType, aContextType.Length() );
    writeStream.CommitL();

    CleanupStack::PopAndDestroy( &writeStream );  // CLEANUP >> writeStream
    
    // append pointer to the message array
    iMessageQueue.AppendL( buffer );
    CleanupStack::Pop( buffer );   // CLEANUP >> buffer

    CheckMessageQueueL();   
    }

void CCFMessageHandlerContext::HandleCommandL(
    const CCFContextSourceCommand& aCommand )
    {
    FUNC_LOG;

    TInt size = sizeof( TInt ); // message type
    size += aCommand.Size(); // the command

    // Stream command into descriptor
    HBufC8* buffer = HBufC8::NewLC( size );         // CLEANUP<< buffer
    TPtr8 bufferPtr = buffer->Des();
    RDesWriteStream stream( bufferPtr );
    stream.PushL();                                 // CLEANUP<< stream

    // Write message to the buffer
    stream.WriteInt32L( EHandleCommand );
    aCommand.ExternalizeL( stream );
    stream.CommitL();

    CleanupStack::PopAndDestroy( &stream );         // CLEANUP>> stream

    // Append buffer to the message array
    iMessageQueue.AppendL( buffer );
    CleanupStack::Pop( buffer );                    // CLEANUP>> buffer

    CheckMessageQueueL();
    }

// End of File

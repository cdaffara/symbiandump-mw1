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
* Description:  CCFContextService class implementation.
 *
*/


// SYSTEM INCLUDES
#include <cfcontextdataobject.h>
#include <s32mem.h>
#include <f32file.h>
#include <centralrepository.h>

// USER INCLUDES
#include "cfcontextservice.h"
#include "cftrace.h"
#include "cfcommon.h"
#include "CFContextObjectImpl.h"
#include "cfcontextsubscriptionlistener.h"
#include "CFClientSession.h"
#include "cfservermessagelistener.h"
#include "ContextFrameworkPrivateCRKeys.h"
#include "CFContextQueryImpl.h"
#include "CFContextSubscriptionImpl.h"

// CONSTANTS
static const TInt KDefaultBufferSize = 1024;

// ======== MEMBER FUNCTIONS ========

// ---------------------------------------------------------------------------
// C++ constructor.
// ---------------------------------------------------------------------------
//
CCFContextService::CCFContextService( RCFClientSession& aSession,
    MCFListener& aListener ):
    CCFServiceBase( CCFServiceBase::ECFContextService, aSession, aListener ),
    iContextDataMaxSize( KContextDataObjectDefaultSize )
    {
    FUNC_LOG;
    }

// ---------------------------------------------------------------------------
// Symbian 2nd phase constructor.
// ---------------------------------------------------------------------------
//
void CCFContextService::ConstructL( )
    {
    FUNC_LOG;

    // Create context subscription listener
    iContextSubscriptionListener = CCFContextSubscriptionListener::NewL(
        iListener,
        iSession );
    
    // Create server message listener
    iServerMessageListener = CCFServerMessageListener::NewL(
      iListener,
      iSession );

    // Get context data object maximum size
    CRepository* cenrep = NULL;
    TRAPD( err, cenrep = CRepository::NewL( KCRUidCFCommonConf ) );
    if( cenrep && err == KErrNone )
        {
        INFO( "Context data object size read from cenrep." );
        err = cenrep->Get( KContextDataObjectMaxSize, iContextDataMaxSize );
        if( err != KErrNone )
            {
            iContextDataMaxSize = KContextDataObjectDefaultSize;
            }
        delete cenrep;
        }
    INFO_1( "Context data object max size set to: [0x%x].",
        iContextDataMaxSize );
    }

// ---------------------------------------------------------------------------
// Symbian two phased constructor.
// ---------------------------------------------------------------------------
//
CCFContextService* CCFContextService::NewL( RCFClientSession& aSession,
    MCFListener& aListener )
    {
    FUNC_LOG;

    CCFContextService* self = CCFContextService::NewLC( aSession, aListener );
    CleanupStack::Pop ( self );
    return self;
    }

// ---------------------------------------------------------------------------
// Symbian two phased constructor.
// Leaves pointer in the cleanup stack.
// ---------------------------------------------------------------------------
//
CCFContextService* CCFContextService::NewLC( RCFClientSession& aSession,
    MCFListener& aListener )
    {
    FUNC_LOG;

    CCFContextService* self = new( ELeave ) CCFContextService(
        aSession,
        aListener );
    CleanupStack::PushL ( self );
    self->ConstructL ( );
    return self;
    }

// ---------------------------------------------------------------------------
// C++ destructor.
// ---------------------------------------------------------------------------
//
CCFContextService::~CCFContextService( )
    {
    FUNC_LOG;

    delete iContextSubscriptionListener;
    delete iServerMessageListener;
    }

//----------------------------------------------------------------------------
// CCFContextService::DefineContext
//----------------------------------------------------------------------------
//
TInt CCFContextService::DefineContext( const TDesC& aContextSource,
    const TDesC& aContextType,
    const TSecurityPolicy& aReadSecurityPolicy,
    const TSecurityPolicy& aWriteSecurityPolicy )
    {
    FUNC_LOG;
    
    TInt err = KErrNone;
    if( aContextSource.Length() && aContextType.Length() )
        {
        TSecurityPolicyBuf readSecPolicyBuf( aReadSecurityPolicy );
        TSecurityPolicyBuf writeSecPolicyBuf( aWriteSecurityPolicy );
        TIpcArgs args( &aContextSource, &aContextType, &readSecPolicyBuf,
                &writeSecPolicyBuf );
        err = iSession.SendSync( EDefineContext, args );
        }
    else
        {
        // Incorrect context definition
        err = KErrArgument;        
        }

    if( err == KErrNone )
        {
        if( !iServerMessageListener->IsActive() )
            {
            // Activate listening
            iServerMessageListener->ReceiveMessages();
            }
        }

    return err;
    }

//----------------------------------------------------------------------------
// CCFContextService::PublishContextL
//----------------------------------------------------------------------------
//
void CCFContextService::PublishContextL( CCFContextObject& aContext )
    {
    FUNC_LOG;

    TInt err = KErrNone;
    if( IsContextLegalForAdd( aContext ) )
        {
        HBufC8* buffer = WriteStreamContextObjectLC( aContext );
        TIpcArgs args( buffer );
        err = iSession.SendSync( EPublishContext, args );
        CleanupStack::PopAndDestroy( buffer );
        }
    else
        {
        err = KErrArgument;
        }
    
    User::LeaveIfError( err );
    }

//----------------------------------------------------------------------------
// CCFContextService::PublishContextL
//----------------------------------------------------------------------------
//
void CCFContextService::PublishContextL( CCFContextObject& aContext,
    CCFContextDataObject& aData )
    {
    FUNC_LOG;

    TInt err = KErrNone;
    
    if( IsContextLegalForAdd( aContext ) )
        {
        err = IsContextDataLegalForAdd( aData );
        if( err == KErrNone )
            {
            // Define IPC arguments
            HBufC8* contextBuffer = WriteStreamContextObjectLC( aContext );
            HBufC8* contextDataBuffer = WriteStreamContextDataObjectLC( aData );
            TPckg<TUid> uidPckg( aData.Uid() );
            TIpcArgs args( contextBuffer, contextDataBuffer, &uidPckg );
            
            // Send message
            err = iSession.SendSync( EPublishContextWithData, args );

            // Clean up
            CleanupStack::PopAndDestroy( contextDataBuffer );
            CleanupStack::PopAndDestroy( contextBuffer );
            }
        }
    else
        {
        err = KErrArgument;
        }
    
    User::LeaveIfError( err );
    }

//----------------------------------------------------------------------------
// CCFContextService::SubscribeContextL
//----------------------------------------------------------------------------
//
void CCFContextService::SubscribeContextL(
    CCFContextSubscription& aSubscription)
    {
    FUNC_LOG;
    
    TInt err = KErrNone;

    // Stream subscription and send
    HBufC8* buffer = WriteStreamContextSubscriptionLC( aSubscription );
    TIpcArgs args( buffer );
    err = iSession.SendSync( EAddSubscription, args );
    CleanupStack::PopAndDestroy( buffer );
    
    User::LeaveIfError( err );

    if( err == KErrNone )
        {
        if( !iContextSubscriptionListener->IsActive() )
            {
            // Activate listening
            iContextSubscriptionListener->ReceiveContextIndications();
            }
        }
    }

//----------------------------------------------------------------------------
// CCFContextService::UnsubscribeContextL
//----------------------------------------------------------------------------
//
void CCFContextService::UnsubscribeContextL(
    CCFContextSubscription& aSubscription)
    {
    FUNC_LOG;
    
    TInt err = KErrNone;

    // Stream subscription and send
    HBufC8* buffer = WriteStreamContextSubscriptionLC( aSubscription );
    TIpcArgs args( buffer );
    err = iSession.SendSync( ERemoveSubscription, args );
    CleanupStack::PopAndDestroy( buffer );
    
    User::LeaveIfError( err );
    }

//----------------------------------------------------------------------------
// CCFContextService::RequestContextL
//----------------------------------------------------------------------------
//
void CCFContextService::RequestContextL( CCFContextQuery& aContextQuery,
    RContextObjectArray& aRequestResults )
    {
    FUNC_LOG;
    
    TInt err = KErrNone;

    // Stream context query set into buffer
    HBufC8* buffer = buffer = WriteStreamContextQueryLC( aContextQuery );
    HBufC8* resultBuffer = HBufC8::NewLC( iRequestBufferSize );
    TPtr8 resultBufferPtr = resultBuffer->Des();
    TPckg<TInt> bufferSizePtr( iRequestBufferSize );

    // Send request to server
    TIpcArgs args( &resultBufferPtr, buffer, &bufferSizePtr );
    err = iSession.SendSync( ERequestContext, args );
    if( err == KErrNone )
        {
        // Results succesfully received
        ReadStreamContextObjectL( *resultBuffer, aRequestResults );
        }
    else if( err == KErrTooBig )
        {
        // Insufficient buffer size, increase buffer and try again
        CleanupStack::PopAndDestroy( resultBuffer );
        resultBuffer = HBufC8::NewLC( iRequestBufferSize );
        resultBufferPtr.Set( resultBuffer->Des() );
        TIpcArgs args( &resultBufferPtr );
        err = iSession.SendSync( ERequestContext, args );
        if( err == KErrNone )
            {
            ReadStreamContextObjectL( *resultBuffer, aRequestResults );
            }
        else
            {
            err = KErrNoMemory;
            }
        }
    CleanupStack::PopAndDestroy( resultBuffer );
    CleanupStack::PopAndDestroy( buffer );

    // If requst buffer size grows too big, zero it
    if( iRequestBufferSize > KDefaultBufferSize )
        {
        iRequestBufferSize = 0;
        }

    User::LeaveIfError( err );
    }

//----------------------------------------------------------------------------
// CCFContextService::RequestContextSetL
//----------------------------------------------------------------------------
//
void CCFContextService::RequestContextSetL(
    RContextQueryArray& aContextQuerySet,
    RContextObjectArray& aRequestResults)
    {
    FUNC_LOG;
    
    TInt err = KErrNone;
    
    // Stream context query set into buffer
    HBufC8* buffer = WriteStreamContextQueryLC( aContextQuerySet );
    HBufC8* resultBuffer = HBufC8::NewLC( iRequestBufferSize );
    TPtr8 resultBufferPtr = resultBuffer->Des();
    resultBufferPtr.Set( resultBuffer->Des() );
    
    // Send request to server
    TPckg<TInt> bufferSizePtr( iRequestBufferSize );
    TIpcArgs args( &resultBufferPtr, buffer, &bufferSizePtr );
    err = iSession.SendSync( ERequestContextSet, args );
    if( err == KErrNone )
        {
        // Results succesfully received
        ReadStreamContextObjectL( *resultBuffer, aRequestResults );
        }
    else if( err == KErrTooBig )
        {
        // Insufficient buffer size, increase buffer and try again
        CleanupStack::PopAndDestroy( resultBuffer );
        resultBuffer = HBufC8::NewLC( iRequestBufferSize );
        resultBufferPtr.Set( resultBuffer->Des() );
        TIpcArgs args( &resultBufferPtr );
        err = iSession.SendSync( ERequestContextSet, args );
        if( err == KErrNone )
            {
            ReadStreamContextObjectL( *resultBuffer, aRequestResults );
            }
        else
            {
            err = KErrNoMemory;
            }
        }
    CleanupStack::PopAndDestroy( resultBuffer );
    CleanupStack::PopAndDestroy( buffer );

    // If requst buffer size grows too big, zero it
    if( iRequestBufferSize > KDefaultBufferSize )
        {
        iRequestBufferSize = 0;
        }

    User::LeaveIfError( err );
    }

//----------------------------------------------------------------------------
// CCFContextService::InstallContextSourceSetting
//----------------------------------------------------------------------------
//
TInt CCFContextService::InstallContextSourceSetting(
    const TDesC& aSettingFilename,
    const TUid& aContextSourceUid )
    {
    FUNC_LOG;
    
    RFs fs;
    TInt err = fs.Connect();
    if ( err != KErrNone )
        {
        return err; 
        }
    
    // Make possible to share the file server session with the CF server.
    err = fs.ShareProtected();
    if( err != KErrNone )
        {
        fs.Close();
        return err;
        }
    
    // Open the file.
    RFile settingFile;
    err = settingFile.Open( fs, aSettingFilename, EFileRead );
    if( err != KErrNone )
        {
        fs.Close();
        return err;
        }
    
    // Deliver the file handle to the server.
    TIpcArgs args;
    err = settingFile.TransferToServer( args, 0, 1 );
    if( err != KErrNone )
        {
        settingFile.Close();
        fs.Close();
        return err;
        }
    
    TPckgC< TUid > uidPckgC( aContextSourceUid );
    args.Set( 2, &uidPckgC );
    
    err = iSession.SendSync( EInstallSourceSetting, args );
    
    settingFile.Close();
    fs.Close();
    
    return err;
    }

//----------------------------------------------------------------------------
// CCFContextService::UninstallContextSourceSetting
//----------------------------------------------------------------------------
//
TInt CCFContextService::UninstallContextSourceSetting(
    const TDesC& aSettingFilename,
    const TUid& aContextSourceUid )
    {
    FUNC_LOG;

    // Deliver setting filename and context source UID to the server.
    TPckgC< TUid > uidPckgC( aContextSourceUid );
    TIpcArgs args( &aSettingFilename, &uidPckgC );

    TInt err = iSession.SendSync( EUninstallSourceSetting, args );

    return err;
    }

//----------------------------------------------------------------------------
// CCFContextService::UninstallContextSourceSettings
//----------------------------------------------------------------------------
//
TInt CCFContextService::UninstallContextSourceSettings(
    const TUid& aContextSourceUid )
    {
    FUNC_LOG;

    // Deliver context source UID to the server.
    TPckgC< TUid > uidPckgC( aContextSourceUid );
    TIpcArgs args( &uidPckgC );

    TInt err = iSession.SendSync( EUninstallSourceSettings, args );

    return err;
    }

//----------------------------------------------------------------------------
// CCFContextService::IsContextLegalForAdd
//----------------------------------------------------------------------------
//
TBool CCFContextService::IsContextLegalForAdd(
    const CCFContextObject& aContext)
    {
    FUNC_LOG;
    
    TBool retVal = EFalse;
    
    if( aContext.Type().Length() > 0 &&
        aContext.Source().Length() > 0 &&
        aContext.Value().Length() > 0 )
        {
        retVal = ETrue;
        }
        
    return retVal;
    }

//----------------------------------------------------------------------------
// CCFContextService::IsContextDataLegalForAdd
//----------------------------------------------------------------------------
//
TInt CCFContextService::IsContextDataLegalForAdd(
    const CCFContextDataObject& aContextData )
    {
    FUNC_LOG;
    
    TInt retVal = KErrNone;
    if( aContextData.Size() > iContextDataMaxSize )
        {
        retVal = KErrTooBig;
        }
    else if( aContextData.Uid() == KNullUid )
        {
        retVal = KErrNotSupported;
        }
    
    return retVal;
    }

//----------------------------------------------------------------------------
// CCFContextService::ReadStreamContextObjectL
//----------------------------------------------------------------------------
//
void CCFContextService::ReadStreamContextObjectL( const TDesC8& aData,
    RContextObjectArray& aArray )
    {
    FUNC_LOG;

    // Check that we have received data from server    
    if( aData.Length() )
        {
        CCFContextObjectImpl* context = NULL;
        RDesReadStream stream( aData );
        stream.PushL();
        TInt count = stream.ReadInt16L();
        for( TInt i = 0; i < count; i++ )
            {
            context = CCFContextObjectImpl::NewLC();
            context->InternalizeL( stream );
            aArray.AppendL( context );
            CleanupStack::Pop( context );
            }
        
        // Clean up
        CleanupStack::PopAndDestroy( &stream );
        }
    }

//----------------------------------------------------------------------------
// CCFContextService::WriteStreamContextSubscriptionLC
//----------------------------------------------------------------------------
//
HBufC8* CCFContextService::WriteStreamContextSubscriptionLC(
    CCFContextSubscription& aSubscription )
    {
    FUNC_LOG;
    
    CCFContextSubscriptionImpl& subscription =
        static_cast<CCFContextSubscriptionImpl&>( aSubscription );

    HBufC8* buffer = HBufC8::NewLC( subscription.Size() );
    TPtr8 bufferPtr = buffer->Des();

    RDesWriteStream stream( bufferPtr );
    stream.PushL();
    subscription.ExternalizeL( stream );

    // Cleanup
    CleanupStack::PopAndDestroy( &stream );

    return buffer;
    }

//----------------------------------------------------------------------------
// CCFContextService::WriteStreamContextObjectLC
//----------------------------------------------------------------------------
//
HBufC8* CCFContextService::WriteStreamContextObjectLC(
    CCFContextObject& aContext )
    {
    FUNC_LOG;
    
    CCFContextObjectImpl& context =
        static_cast<CCFContextObjectImpl&>( aContext );
    
    HBufC8* buffer = HBufC8::NewLC( context.Size() );
    TPtr8 bufferPtr = buffer->Des();

    RDesWriteStream stream( bufferPtr );
    stream.PushL();
    context.ExternalizeL( stream );

    // Cleanup
    CleanupStack::PopAndDestroy( &stream );

    return buffer;
    }

//----------------------------------------------------------------------------
// CCFContextService::WriteStreamContextQueryLC
//----------------------------------------------------------------------------
//
HBufC8* CCFContextService::WriteStreamContextQueryLC(
    RContextQueryArray& aQueryArray )
    {
    FUNC_LOG;

    // Calculate buffer size
    // Count of array and queries
    CCFContextQueryImpl* query = NULL;
    TInt size = 0;
    size += sizeof( TInt );
    TInt count = aQueryArray.Count();
    for( TInt i = 0; i < count; i++ )
        {
        query = static_cast<CCFContextQueryImpl*>( aQueryArray[i] );
        size += query->Size();
        }
    
    HBufC8* buffer = HBufC8::NewLC( size );
    TPtr8 bufferPtr = buffer->Des();

    RDesWriteStream stream( bufferPtr );
    stream.PushL();
    stream.WriteInt16L( count );
    for( TInt i = 0; i < count; i++ )
        {
        query = static_cast<CCFContextQueryImpl*>( aQueryArray[i] );
        query->ExternalizeL( stream );
        }

    // Commit stream
    stream.CommitL();

    // Cleanup
    CleanupStack::PopAndDestroy( &stream );

    return buffer;
    }

//----------------------------------------------------------------------------
// CCFContextService::WriteStreamContextQueryLC
//----------------------------------------------------------------------------
//
HBufC8* CCFContextService::WriteStreamContextQueryLC( CCFContextQuery& aQuery )
    {
    FUNC_LOG;

    CCFContextQueryImpl& query =
        static_cast<CCFContextQueryImpl&>( aQuery );
    
    HBufC8* buffer = HBufC8::NewLC( query.Size() );
    TPtr8 bufferPtr = buffer->Des();

    RDesWriteStream stream( bufferPtr );
    stream.PushL();
    query.ExternalizeL( stream );

    // Cleanup
    CleanupStack::PopAndDestroy( &stream );
    
    return buffer;
    }

//----------------------------------------------------------------------------
// CCFContextService::WriteStreamContextDataObjectLC
//----------------------------------------------------------------------------
//
HBufC8* CCFContextService::WriteStreamContextDataObjectLC(
    CCFContextDataObject& aContextData )
    {
    FUNC_LOG;
    
    HBufC8* streamBuf = HBufC8::NewLC( aContextData.Size() );
    TPtr8 streamBufPtr( streamBuf->Des() );
    RDesWriteStream writeStream( streamBufPtr );
    CleanupClosePushL( writeStream );
    aContextData.ExternalizeL( writeStream );
    
    // Clean up
    CleanupStack::PopAndDestroy( &writeStream );
    
    return streamBuf;
    }

// End of file

/*
* Copyright (c) 2002-2008 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  CFServer class implementation.
*
*/


#include <e32svr.h>
#include <e32math.h>
#include <e32uid.h>
#include <ecom/ecom.h>
#include <centralrepository.h>

#include <cfcontextdataobject.h>
#include <cfscriptevent.h>
#include <cfcontextsourcecommand.h>

#include "CFEngine.h"
#include "CFContextManager.h"
#include "CFContextSourceManager.h"
#include "CFScriptHandler.h"
#include "CFActivator.h"
#include "CFContextSubscriptionImpl.h"
#include "cfstarter.h"
#include "cfecomobserver.h"
#include "cftrace.h"
#include "CFActionSubscriptionImpl.h"
#include "ContextFrameworkPrivateCRKeys.h"
#include "cfasynccmdqueue.h"
#include "cfactioncmd.h"
#include "cfpublishcontextcmd.h"
#include "cfremovesubscriptioncleanupcmd.h"
#include "cfcontextsourcecmd.h"

// CONSTANTS

#ifdef _DEBUG
_LIT( KPanicCat, "CFEngine" );

enum TPanicReason
    {
    ENULLParam
    };

    
LOCAL_C void Panic( TPanicReason aReason )
    {
    User::Panic( KPanicCat, aReason );
    }
#endif

// MEMBER FUNCTIONS

CCFEngine* CCFEngine::NewL( MCFSecurityChecker& aSecurityChecker )
    {
    FUNC_LOG;
    
    CCFEngine* self = CCFEngine::NewLC( aSecurityChecker );
    CleanupStack::Pop( self );
    
    return self;
    }

CCFEngine* CCFEngine::NewLC( MCFSecurityChecker& aSecurityChecker )
    {
    FUNC_LOG;
    
    CCFEngine* self = new( ELeave ) CCFEngine( aSecurityChecker );
    CleanupStack::PushL( self );
    self->ConstructL();
    
    return self;
    }

CCFEngine::~CCFEngine()
    {
    FUNC_LOG;

    delete iScriptEngine;
    delete iActivatorEngine;
    delete iContextSourceManager;
    delete iContextManager;
    delete iEComObserver;
    delete iStarter;
    delete iAsyncCmdQueue;
    iFs.Close();
    }

void CCFEngine::ConstructL()
    {
    FUNC_LOG;
    
    // Open file handle to share it everywhere in context manager
    User::LeaveIfError( iFs.Connect() );
    
    // Construct cf started
    iStarter = CCFStarter::NewL( *this );
    
    // CCFEComObserver
    iEComObserver = CCFEComObserver::NewL();
    
	// ContextManager
    iContextManager = CCFContextManager::NewL( *iSecurityChecker );

    // Context source manager    
    iContextSourceManager = CCFContextSourceManager::NewL( *this, iFs );
    iStarter->AddObserverL( iContextSourceManager );
    iEComObserver->AddObserverL( iContextSourceManager );

    // Activator engine
    iActivatorEngine = CCFActivator::NewL();
    iStarter->AddObserverL( iActivatorEngine );
    iEComObserver->AddObserverL( iActivatorEngine );
    
    // Script engine
    iScriptEngine = CCFScriptHandler::NewL( *this, iFs,
    	*this,
    	*iSecurityChecker );
    iStarter->AddObserverL( iScriptEngine );
    iEComObserver->AddObserverL( iScriptEngine );

    // Queue for asynchronous commands.
    iAsyncCmdQueue = CCFAsyncCmdQueue::NewL();

    // Get context data object maximum size
    CRepository* cenrep = NULL;
    TRAPD( err, cenrep = CRepository::NewL( KCRUidCFCommonConf ) );
    if( cenrep && err == KErrNone )
        {
        err = cenrep->Get( KContextDataObjectMaxSize, iMaxDataObjectSize );
        if( err != KErrNone )
            {
            iMaxDataObjectSize = KContextDataObjectDefaultSize;
            }
        delete cenrep;
        } 
    
    // Start cf starter
    iStarter->StartL();
    }

CCFEngine::CCFEngine( MCFSecurityChecker& aSecurityChecker ):
    iSecurityChecker( &aSecurityChecker ),
    iMaxDataObjectSize( KContextDataObjectDefaultSize )
    {
    FUNC_LOG;
    }

// METHODS

//----------------------------------------------------------------------------
// CCFEngine::DefineContext
//----------------------------------------------------------------------------
//
TInt CCFEngine::DefineContext( const TDesC& aContextSource,
    const TDesC& aContextType,
    const TSecurityPolicy& aSecurityPolicy )
    {
    FUNC_LOG;
    
    INFO_2( "Defining context: [%S: %S]",
        &aContextSource,
        &aContextType );
    
    TRAPD( err, iContextManager->DefineContextL(
        aContextSource, aContextType,
        aSecurityPolicy, aSecurityPolicy ) );
        
    ERROR_1( err, "DefineContext returned with code: %d", err );
        
    return err;
    }

//----------------------------------------------------------------------------
// CCFEngine::DefineContext
//----------------------------------------------------------------------------
//
TInt CCFEngine::DefineContext( const TDesC& aContextSource,
    const TDesC& aContextType,
    const TSecurityPolicy& aReadWritePolicy,
    MCFContextSource& aOwner )
    {
    FUNC_LOG;
    
    INFO_2( "Defining context: [%S: %S]",
        &aContextSource,
        &aContextType );

    RThread thread;

    TRAPD( err, iContextManager->DefineContextL(
        aContextSource, aContextType,
        aReadWritePolicy, aReadWritePolicy, &aOwner, thread.SecureId() ) );

    thread.Close();
    
    ERROR_1( err, "DefineContext returned with code: %d", err );
        
    return err;
    }

//----------------------------------------------------------------------------
// CCFEngine::DefineContext
//----------------------------------------------------------------------------
//
TInt CCFEngine::DefineContext( const TDesC& aContextSource,
    const TDesC& aContextType,
    const TSecurityPolicy& aReadPolicy,
    const TSecurityPolicy& aWritePolicy,
    MCFContextSource& aOwner )
    {
    FUNC_LOG;
    
    INFO_2( "Defining context: [%S: %S]",
        &aContextSource,
        &aContextType );
    
    RThread thread;
    
    TRAPD( err, iContextManager->DefineContextL(
        aContextSource, aContextType,
        aReadPolicy, aWritePolicy, &aOwner, thread.SecureId() ) );
    
    thread.Close();
    
    ERROR_1( err, "DefineContext returned with code: %d", err );
        
    return err;
    }

//----------------------------------------------------------------------------
// CCFEngine::PublishContext
//----------------------------------------------------------------------------
//
TInt CCFEngine::PublishContext( CCFContextObject& aContext,
    RThread& aClientThread )
    {
    FUNC_LOG;

    TRAPD( err, EnqueuePublishContextL( aContext, aClientThread ) );

    ERROR_3( err, "CCFEngine::EnqueuePublishContextL - Failed for context [%S: %S :%S].",
            &aContext.Source(),
            &aContext.Type(),
            &aContext.Value() );

    return err;
    }

//----------------------------------------------------------------------------
// CCFEngine::PublishContext
//----------------------------------------------------------------------------
//
TInt CCFEngine::PublishContext( CCFContextObject& aContext,
    CCFContextDataObject& aData,
    RThread& aClientThread )
    {
    FUNC_LOG;

    TInt err = KErrNone;
    if( aData.Size() > iMaxDataObjectSize )
        {
        err = KErrTooBig;
        ERROR_GEN_2( "Context data object [0x%x] is too big. Maximum size: [0x%x]",
            err,
            iMaxDataObjectSize );
        }
    else if( aData.Uid() == KNullUid )
        {
        err = KErrNotSupported;
        ERROR_GEN( "Context data object with NULL UID not supported." );
        }
    else
        {
        TRAP( err, EnqueuePublishContextL( aContext, aClientThread, &aData ) );

        ERROR_3( err, "CCFEngine::EnqueuePublishContextL - Failed for context [%S: %S :%S].",
                &aContext.Source(),
                &aContext.Type(),
                &aContext.Value() );
        ERROR_1( err, "CCFEngine::EnqueuePublishContextL - Failed with data object: [0x%x]",
                aData.Uid() );
        }

    return err;
    }

//----------------------------------------------------------------------------
// CCFEngine::SubscribeContext
//----------------------------------------------------------------------------
//
TInt CCFEngine::SubscribeContext(
    CCFContextSubscription* aSubscription,
    MCFContextSubscriptionListener* aListener,
    RThread& aClientThread )
    {
    FUNC_LOG;
    
    __ASSERT_DEBUG( aSubscription && aListener, Panic( ENULLParam ) );
    
    INFO_2( "Subscribing context: [%S: %S]",
        &aSubscription->ContextSource(),
        &aSubscription->ContextType() );
    
    TRAPD( err,
        iContextManager->SubscribeContextL( aSubscription, aListener,
            aClientThread ) );
    
    ERROR_1( err, "SubscribeContext returned with code: %d", err );
        
    return err;
    }

//----------------------------------------------------------------------------
// CCFEngine::UnsubscribeContext
//----------------------------------------------------------------------------
//
void CCFEngine::UnsubscribeContext(
    CCFContextSubscription& aSubscription,
    MCFContextSubscriptionListener& aListener )
    {
    FUNC_LOG;

    INFO_2( "CCFEngine::UnsubscribeContext - For context [%S: %S].",
            &aSubscription.ContextSource(),
            &aSubscription.ContextType() );

    if ( iContextManager->RemoveSubscription( aSubscription, aListener ) )
        {
        TInt err = KErrNone;
        TRAP( err, EnqueueUnsubscribeContextCleanupL() );
        ERROR( err, "CCFEngine::EnqueueUnsubscribeContextCleanupL - Failed after unsubscribing a context." );
        }
    }

//----------------------------------------------------------------------------
// CCFEngine::UnsubscribeContexts
//----------------------------------------------------------------------------
//
void CCFEngine::UnsubscribeContexts(
    MCFContextSubscriptionListener& aObserver )
    {
    FUNC_LOG;

    INFO( "CCFEngine::UnsubscribeContexts - For a listener." );

    if ( iContextManager->RemoveSubscriptions( &aObserver ) )
        {
        TInt err = KErrNone;
        TRAP( err, EnqueueUnsubscribeContextCleanupL() );
        ERROR( err, "CCFEngine::EnqueueUnsubscribeContextCleanupL - Failed after unsubscribing contexts." );
        }
    }

//----------------------------------------------------------------------------
// CCFEngine::RequestContextL
//----------------------------------------------------------------------------
//
TInt CCFEngine::RequestContext(
    RContextObjectArray& aResultBuffer,
    CCFContextQuery& aContextQuery,
    RThread& aClientThread )
    {
    FUNC_LOG;
    
    INFO_2( "Requesting context: [%S: %S]",
        &aContextQuery.Source(),
        &aContextQuery.Type() );
    
    TRAPD( err, iContextManager->RequestContextL( aResultBuffer,
        aContextQuery,
        aClientThread ) );

    ERROR_1( err, "RequestContext returned with code: %d", err );
        
    return err;
    }

//----------------------------------------------------------------------------
// CCFEngine::GetSecurityPolicy
//----------------------------------------------------------------------------
//
TInt CCFEngine::GetSecurityPolicy( const TDesC& aContextSource,
    const TDesC& aContextType,
	TSecurityPolicy& aPolicy )
    {
    FUNC_LOG;
    
    INFO_2( "Getting security policy: [%S: %S]",
        &aContextSource,
        &aContextType );
    
    TRAPD( err, iContextManager->GetWriteSecurityPolicyL( aContextSource,
        aContextType,
        aPolicy ) );

    ERROR_1( err, "RequestContext returned with code: %d", err );
        
    return err;
    }

//----------------------------------------------------------------------------
// CCFEngine::GetReadSecurityPolicy
//----------------------------------------------------------------------------
//
TInt CCFEngine::GetReadSecurityPolicy( const TDesC& aContextSource,
	const TDesC& aContextType,
	TSecurityPolicy& aPolicy )
	{
    FUNC_LOG;
    
    INFO_2( "Getting security policy: [%S: %S]",
        &aContextSource,
        &aContextType );
    
    TRAPD( err, iContextManager->GetReadSecurityPolicyL( aContextSource,
        aContextType,
        aPolicy ) );

    ERROR_1( err, "RequestContext returned with code: %d", err );
        
    return err;
    }

//----------------------------------------------------------------------------
// CCFEngine::GetWriteSecurityPolicy
//----------------------------------------------------------------------------
//
TInt CCFEngine::GetWriteSecurityPolicy( const TDesC& aContextSource,
	const TDesC& aContextType,
	TSecurityPolicy& aPolicy )
	{
    FUNC_LOG;
    
    INFO_2( "Getting security policy: [%S: %S]",
        &aContextSource,
        &aContextType );
    
    TRAPD( err, iContextManager->GetWriteSecurityPolicyL( aContextSource,
        aContextType,
        aPolicy ) );

    ERROR_1( err, "RequestContext returned with code: %d", err );
        
    return err;
    }


//----------------------------------------------------------------------------
// CCFEngine::DefineContext
//----------------------------------------------------------------------------
//
TInt CCFEngine::DefineContext( const TDesC& aContextSource,
    const TDesC& aContextType,
    const TSecurityPolicy& aReadPolicy,
    const TSecurityPolicy& aWritePolicy,
    MCFContextSource* aPublisher,
    const TUid& aPublisherUid )
	{
    FUNC_LOG;

    INFO_2( "Defining context: [%S: %S]", &aContextSource, &aContextType );

    TRAPD( err, iContextManager->DefineContextL(
        aContextSource, aContextType,
        aReadPolicy, aWritePolicy, aPublisher, aPublisherUid ) );

    if ( err == KErrNone )
        {
        // Make sure the call is coming from a client and not CF thread.
        RThread thread;
        if ( aPublisherUid != thread.SecureId() )
            {
            iContextSourceManager->RegisterClientContextSource( aPublisher,
                    aPublisherUid );
            }
        thread.Close();
        }

    ERROR_1( err, "DefineContext returned with code: %d", err );

    return err;
    }
    
//----------------------------------------------------------------------------
// CCFEngine::DeregisterPublisher
//----------------------------------------------------------------------------
//
void CCFEngine::DeregisterPublisher( MCFContextSource& aPublisher )
	{
    FUNC_LOG;
    
    INFO( "Deregistering Publisher" );
    
    iContextManager->DeregisterPublisher( aPublisher );
    iContextSourceManager->DeregisterClientContextSource( aPublisher );
    }
	
//----------------------------------------------------------------------------
// CCFEngine::InstallContextSourceSetting
//----------------------------------------------------------------------------
//
TInt CCFEngine::InstallContextSourceSetting( RFile& aSettingFile,
    const TUid& aContextSourceUid,
    RThread& aClientThread )
    {
    FUNC_LOG;

    INFO( "Installing Context Source Setting" );

    TRAPD( err, iContextSourceManager->InstallSettingL( aSettingFile,
            aContextSourceUid,
            aClientThread ) );

    ERROR( err, "Installing Context Source Setting failed" );

    return err;
    }

//----------------------------------------------------------------------------
// CCFEngine::UninstallContextSourceSetting
//----------------------------------------------------------------------------
//
TInt CCFEngine::UninstallContextSourceSetting( const TDesC& aSettingFilename,
    const TUid& aContextSourceUid,
    RThread& aClientThread )
    {
    FUNC_LOG;

    INFO_1( "Uninstalling Context Source Setting [%S]", &aSettingFilename );

    TRAPD( err, iContextSourceManager->UninstallSettingL( aSettingFilename,
            aContextSourceUid,
            aClientThread ) );

    ERROR_1( err, "Uninstalling Context Source Setting [%S] failed",
            &aSettingFilename );

    return err;
    }

//----------------------------------------------------------------------------
// CCFEngine::UninstallContextSourceSettings
//----------------------------------------------------------------------------
//
TInt CCFEngine::UninstallContextSourceSettings( const TUid& aContextSourceUid,
    RThread& aClientThread )
    {
    FUNC_LOG;

    INFO( "Uninstalling Context Source Settings" );

    TRAPD( err, iContextSourceManager->UninstallSettingsL( aContextSourceUid,
            aClientThread ) );

    ERROR( err, "Uninstalling Context Source Settings failed" );

    return err;
    }

//----------------------------------------------------------------------------
// CCFEngine::PublishContextFromAction
//----------------------------------------------------------------------------
//
TInt CCFEngine::PublishContextFromAction( CCFContextObject& aContext,
    TBool aAutoDefine )
    {
    FUNC_LOG;

    RThread thread;
    TRAPD( err, iContextManager->ValidatePublishContextL( aContext, thread ) );
    if ( err == KErrNotFound && aAutoDefine )
        {
        // Only cfserver can publish
        TSecurityPolicy writePolicy( thread.SecureId() );
        // Every one can subscribe        
        _LIT_SECURITY_POLICY_PASS( readPolicy );
        TRAP( err, iContextManager->DefineContextL( aContext.Source(),
                aContext.Type(),
                readPolicy,
                writePolicy,
                NULL,
                thread.SecureId() ) );
        ERROR_3( err, "CCFEngine::PublishContextFromAction - Auto-define failed for context [%S: %S :%S].",
                &aContext.Source(),
                &aContext.Type(),
                &aContext.Value() );
        }
    thread.Close();

    if ( err == KErrNone  || err == KErrAlreadyExists )
        {
        CCFPublishContextCmd* cmd = NULL;
        TRAP( err, cmd = CCFPublishContextCmd::NewL( *iContextManager,
                aContext ) );
        ERROR( err, "CCFEngine::PublishContextFromAction - Creating publish context cmd failed." );
        if ( err == KErrNone )
            {
            INFO_3( "CCFEngine::PublishContextFromAction - Appending publish context cmd for context [%S: %S: %S] to async queue.",
                    &aContext.Source(),
                    &aContext.Type(),
                    &aContext.Value() );

            iAsyncCmdQueue->Add( cmd );
            }
        }

    return err;
    }


//----------------------------------------------------------------------------
// CCFEngine::DefineAction
//----------------------------------------------------------------------------
//
TInt CCFEngine::DefineAction( const TDesC& aActionIdentifier,
    const TSecurityPolicy& aSecurityPolicy,
    const TUid& aOwnerUid,
    MCFActionOwner* aOwner )
    {
    FUNC_LOG;
    
    INFO_1( "Defining action: [%S]", &aActionIdentifier );

    MCFActivatorCallBackInterface* interface =
        static_cast<MCFActivatorCallBackInterface*>( iActivatorEngine );
    TRAPD( err, interface->DefineActionL( aActionIdentifier,
        aSecurityPolicy, aOwnerUid, aOwner ) );
    
    ERROR_1( err, "DefineAction returned with code: %d", err );
        
    return err;
    }

//----------------------------------------------------------------------------
// CCFEngine::SubscribeAction
//----------------------------------------------------------------------------
//
TInt CCFEngine::SubscribeAction( CCFActionSubscription* aActionSubscription,
    MCFActionSubscriptionListener* aListener,
    RThread& aClientThread )
    {
    FUNC_LOG;
    
    INFO_1( "Subscribing action: [%S]",
        &aActionSubscription->ActionIdentifier() );
    
    MCFActivatorCallBackInterface* interface =
        static_cast<MCFActivatorCallBackInterface*>( iActivatorEngine );
    TRAPD( err, interface->SubscribeActionL(
        aActionSubscription,
        aListener,
        aClientThread ) );
    
    ERROR_1( err, "SubscribeAction returned with code: %d", err );
        
    return err;
    }

//----------------------------------------------------------------------------
// CCFEngine::UnsubscribeAction
//----------------------------------------------------------------------------
//
void CCFEngine::UnsubscribeAction( CCFActionSubscription& aActionSubscription,
    MCFActionSubscriptionListener& aListener )
    {
    FUNC_LOG;
    
    INFO_1( "Subscribing action: [%S]",
        &aActionSubscription.ActionIdentifier() );

    MCFActivatorCallBackInterface* interface =
        static_cast<MCFActivatorCallBackInterface*>( iActivatorEngine );
    interface->UnsubscribeAction( aActionSubscription, aListener );
    }

//----------------------------------------------------------------------------
// CCFEngine::UnsubscribeActions
//----------------------------------------------------------------------------
//
void CCFEngine::UnsubscribeActions(
    MCFActionSubscriptionListener& aListener )
    {
    FUNC_LOG;

    MCFActivatorCallBackInterface* interface =
        static_cast<MCFActivatorCallBackInterface*>( iActivatorEngine );
    interface->RemoveSubscriptions( aListener );
    }

//------------------------------------------------------------------------------
// CCFEngine::DeregisterActions
//------------------------------------------------------------------------------
//
void CCFEngine::DeregisterActions( MCFActionOwner* aOwner )
    {
    FUNC_LOG;

    MCFActivatorCallBackInterface* interface =
        static_cast<MCFActivatorCallBackInterface*>( iActivatorEngine );
    interface->DeregisterActions( aOwner );
    }
    
//----------------------------------------------------------------------------
// CCFEngine::RegisterScript
//----------------------------------------------------------------------------
//
TInt CCFEngine::RegisterScript( const TDesC& aName,
    const TDesC8& aScript,
    TInt& aScriptId,
	const RThread& aClientThread,
    MCFScriptOwner& aOwner )
	{
    FUNC_LOG;

    INFO_1( "CCFEngine::RegisterScript - Registering by NAME=[%S]", &aName );

    MCFScriptEngineInterface* interface =
        static_cast<MCFScriptEngineInterface*>( iScriptEngine );
	TInt err = interface->AddScript( aName, aScript, aClientThread.SecureId(),
            aClientThread, *this, &aOwner );
	
	if ( err < 0 )
		{
        INFO_2( "CCFEngine::RegisterScript - Failed by NAME=[%S] with code %d", &aName, err );

		return err;
		}
	else
		{
        INFO_2( "CCFEngine::RegisterScript - Registered by NAME=[%S] into ID=[%d]", &aName, err );

        aScriptId = err;
		return KErrNone;	
		}
	}
        
//----------------------------------------------------------------------------
// CCFEngine::DeregisterScript
//----------------------------------------------------------------------------
//
TInt CCFEngine::DeregisterScript( TInt aScriptId, const RThread& aClientThread )
	{
    FUNC_LOG;

	MCFScriptEngineInterface* interface =
        static_cast<MCFScriptEngineInterface*>( iScriptEngine );
	return interface->RemoveScriptById( aScriptId, aClientThread );
	}

//----------------------------------------------------------------------------
// CCFEngine::DeregisterScript
//----------------------------------------------------------------------------
//
TInt CCFEngine::DeregisterScript( const TDesC& aScriptName,
    const RThread& aClientThread )
    {
    FUNC_LOG;
    
    MCFScriptEngineInterface* interface =
        static_cast<MCFScriptEngineInterface*>( iScriptEngine );
    return interface->RemoveScriptByName( aScriptName, aClientThread.SecureId() );
    }

//----------------------------------------------------------------------------
// CCFEngine::DeregisterScripts
//----------------------------------------------------------------------------
//
TInt CCFEngine::DeregisterScripts( const RThread& aClientThread )
    {
    FUNC_LOG;
    
    MCFScriptEngineInterface* interface =
        static_cast<MCFScriptEngineInterface*>( iScriptEngine );
    return interface->RemoveScriptByUid( aClientThread.SecureId() );
    }

//------------------------------------------------------------------------------
// CCFEngine::DeregisterScriptOwner
//------------------------------------------------------------------------------
//
void CCFEngine::DeregisterScriptOwner( MCFScriptOwner& aOwner )
    {
    FUNC_LOG;

    MCFScriptEngineInterface* interface =
        static_cast<MCFScriptEngineInterface*>( iScriptEngine );
    interface->DeregisterScriptOwner( &aOwner );
    }

//----------------------------------------------------------------------------
// CCFEngine::SaveScript
//----------------------------------------------------------------------------
//
TInt CCFEngine::SaveScript( const TDesC8& aScript,
    TInt aScriptId,
    const RThread& aClient )
    {
    FUNC_LOG;
    
    MCFScriptEngineInterface* interface =
        static_cast<MCFScriptEngineInterface*>( iScriptEngine );
    return interface->SaveScript( aScript, aScriptId, aClient.SecureId() );
    }

//----------------------------------------------------------------------------
// CCFEngine::DeleteScript
//----------------------------------------------------------------------------
//
TInt CCFEngine::DeleteScript( const TDesC& aScriptName,
    RThread& aClientThread )
    {
    FUNC_LOG;
    
    MCFScriptEngineInterface* interface =
        static_cast<MCFScriptEngineInterface*>( iScriptEngine );
    return interface->DeleteScriptByName( aScriptName,
        aClientThread.SecureId() );
    }

//----------------------------------------------------------------------------
// CCFEngine::DeleteScripts
//----------------------------------------------------------------------------
//
TInt CCFEngine::DeleteScripts( RThread& aClientThread )
    {
    FUNC_LOG;
    
    MCFScriptEngineInterface* interface =
        static_cast<MCFScriptEngineInterface*>( iScriptEngine );
    return interface->DeleteScriptByUid( aClientThread.SecureId() );
    }

//----------------------------------------------------------------------------
// CCFEngine::AlreadyExists
//----------------------------------------------------------------------------
//
TBool CCFEngine::AlreadyExists( const TDesC& aScriptName,
    const RThread& aClient,
    TInt& aScriptId ) const
    {
    FUNC_LOG;
    
    MCFScriptEngineInterface* interface =
        static_cast<MCFScriptEngineInterface*>( iScriptEngine );
    return interface->AlreadyExists( aScriptName, aClient.SecureId(), aScriptId );
    }

//----------------------------------------------------------------------------
// CCFEngine::CleanupPersistentDataByName
//----------------------------------------------------------------------------
//
void CCFEngine::CleanupPersistentDataByName( const TDesC& aScriptName,
    const RThread& aClientThread )
    {
    FUNC_LOG;

    MCFScriptEngineInterface* interface =
        static_cast<MCFScriptEngineInterface*>( iScriptEngine );
    interface->CleanupPersistentDataByName( aScriptName,
        aClientThread.SecureId() );
    }

//----------------------------------------------------------------------------
// CCFEngine::CleanupPersistentDataByUid
//----------------------------------------------------------------------------
//
void CCFEngine::CleanupPersistentDataByUid( const RThread& aClient )
    {
    FUNC_LOG;

    MCFScriptEngineInterface* interface =
        static_cast<MCFScriptEngineInterface*>( iScriptEngine );
    interface->CleanupPersistentDataByUid( aClient.SecureId() );
    }

//----------------------------------------------------------------------------
// CCFEngine::UpdateScript
//----------------------------------------------------------------------------
//
TInt CCFEngine::UpdateScript( TInt aScriptID,
    const RThread& aOwnerThread,
    const TDesC8& aUpdatedScript,
    MCFScriptOwner& aOwner )
    {
    FUNC_LOG;

    MCFScriptEngineInterface* interface = static_cast
        <MCFScriptEngineInterface*>( iScriptEngine );
    return interface->UpdateScript(
        aScriptID, aOwnerThread, aUpdatedScript, &aOwner );
    }

//----------------------------------------------------------------------------
// CCFEngine::UpdateScript
//----------------------------------------------------------------------------
//
TInt CCFEngine::RestoreRomScript( TInt aScriptId,
    const RThread& aClient )
    {
    FUNC_LOG;
    
    MCFScriptEngineInterface* interface = static_cast
        <MCFScriptEngineInterface*>( iScriptEngine );
    return interface->RestoreRomScript( aScriptId, aClient.SecureId(), aClient );
    }

//----------------------------------------------------------------------------
// CCFEngine::IsUpgradeAllowed
//----------------------------------------------------------------------------
//
TInt CCFEngine::IsUpgradeAllowed( const TDesC& aName,
    const TDesC8& aScript,
    const RThread& aOwnerThread )
    {
    FUNC_LOG;
    
    MCFScriptEngineInterface* interface = static_cast
        <MCFScriptEngineInterface*>( iScriptEngine );
    return interface->IsUpgradeAllowed( aName, aScript,
        aOwnerThread.SecureId(), aOwnerThread, *this );
    }

//----------------------------------------------------------------------------
// CCFEngine::FireActionL
//----------------------------------------------------------------------------
//
void CCFEngine::FireActionL( CCFScriptEvent* aEvent )
    {
    FUNC_LOG;

    TPtrC actionId( aEvent->Identifier() );
    INFO_1( "CCFEngine::FireActionL - Enqueueing fire action for id [%S].",
            &actionId );

    EnqueueFireActionL( *iActivatorEngine, aEvent );
    }

//----------------------------------------------------------------------------
// CCFEngine::FireActionL
//----------------------------------------------------------------------------
//
void CCFEngine::FireActionL( const CCFContextSourceCommand& aCommand )
    {
    FUNC_LOG;

    TPtrC commandName( aCommand.Name() );
    INFO_1( "CCFEngine::FireActionL - Enqueueing fire action for id [%S].",
            &commandName );

    EnqueueSourceCommandActionL( *this, aCommand );
    }

//----------------------------------------------------------------------------
// CCFEngine::GetActionSecurityPolicy
//----------------------------------------------------------------------------
//
TInt CCFEngine::GetActionSecurityPolicy( const TDesC& aActionIdentifier,
    TSecurityPolicy& aPolicy )
    {
    FUNC_LOG;

    return static_cast< MCFActionHandler* >( iActivatorEngine )
            ->GetActionSecurityPolicy( aActionIdentifier, aPolicy );
    }

//----------------------------------------------------------------------------
// CCFEngine::HandleCommandL
//----------------------------------------------------------------------------
//
void CCFEngine::HandleCommandL( const CCFContextSourceCommand& aCommand )
    {
    FUNC_LOG;

    iContextSourceManager->HandleContextSourceCommandL( aCommand );
    }


//----------------------------------------------------------------------------
// CCFEngine::EnqueuePublishContextL
//----------------------------------------------------------------------------
//
void CCFEngine::EnqueuePublishContextL( CCFContextObject& aContext,
    RThread& aClientThread,
    CCFContextDataObject* aData )
    {
    FUNC_LOG;

    iContextManager->ValidatePublishContextL( aContext, aClientThread );

    CCFPublishContextCmd* cmd = NULL;
    if ( aData )
        {
        cmd = CCFPublishContextCmd::NewL( *iContextManager, aContext, *aData );
        }
    else
        {
        cmd = CCFPublishContextCmd::NewL( *iContextManager, aContext );
        }

    INFO_3( "CCFEngine::EnqueuePublishContextL - Appending publish context cmd for context [%S: %S: %S] to async queue.",
            &aContext.Source(),
            &aContext.Type(),
            &aContext.Value() );
    if ( aData )
        {
        INFO_1( "CCFEngine::EnqueuePublishContextL - Appending publish context cmd with data object: [0x%x]",
                aData->Uid() );
        }

    iAsyncCmdQueue->Add( cmd );
    }

//----------------------------------------------------------------------------
// CCFEngine::EnqueueUnsubscribeContextCleanupL
//----------------------------------------------------------------------------
//
void CCFEngine::EnqueueUnsubscribeContextCleanupL()
    {
    FUNC_LOG;

    CCFRemoveSubscriptionCleanupCmd* cmd
        = CCFRemoveSubscriptionCleanupCmd::NewL( *iContextManager );

    INFO( "CCFEngine::EnqueueUnsubscribeContextCleanupL - Adding remove subscription cleanup cmd to the front of async queue." );

    iAsyncCmdQueue->AddFirst( cmd );
    }

//----------------------------------------------------------------------------
// CCFEngine::EnqueueFireActionL
//----------------------------------------------------------------------------
//
void CCFEngine::EnqueueFireActionL( MCFActionHandler& aActionHandler,
    CCFScriptEvent* aEvent )
    {
    FUNC_LOG;

    CCFActionCmd* cmd = CCFActionCmd::NewL( aActionHandler, aEvent );

    TPtrC actionId( aEvent->Identifier() );
    INFO_1( "CCFEngine::EnqueueFireActionL - Appending action cmd for id [%S] to async queue.",
            &actionId );

    iAsyncCmdQueue->Add( cmd );
    }

//----------------------------------------------------------------------------
// CCFEngine::EnqueueSourceCommandActionL
//----------------------------------------------------------------------------
//
void CCFEngine::EnqueueSourceCommandActionL(
    MCFContextSourceCommand& aSourceCommandHandler,
    const CCFContextSourceCommand& aCommand )
    {
    FUNC_LOG;

    CCFContextSourceCmd* cmd
        = CCFContextSourceCmd::NewL( aSourceCommandHandler, aCommand );

    INFO( "CCFEngine::EnqueueSourceCommandActionL - Appending source command action cmd to async queue." );

    iAsyncCmdQueue->Add( cmd );
    }

// End of file

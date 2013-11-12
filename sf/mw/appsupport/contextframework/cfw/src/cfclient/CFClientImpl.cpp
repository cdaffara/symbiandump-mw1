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
* Description:  CFClientImpl class implementation.
*
*/


// SYSTEM INCLUDE FILES
#include <cfcontextdataobject.h>

// USER INCLUDES
#include "CFClientImpl.h"
#include "cfactionsubscription.h"
#include "cftrace.h"
#include "cfcontextservice.h"
#include "cfactionservice.h"
#include "cfscriptservice.h"

CCFClientImpl* CCFClientImpl::NewL(
    MCFListener& aListener )
    {
    FUNC_LOG;
    
    CCFClientImpl* self = CCFClientImpl::NewLC( aListener );
    CleanupStack::Pop( self );
    
    return self;
    }

CCFClientImpl* CCFClientImpl::NewLC(
    MCFListener& aListener )
    {
    FUNC_LOG;
    
    CCFClientImpl* self = new( ELeave ) CCFClientImpl;
    
    CleanupStack::PushL( self );
    self->ConstructL( aListener );
    
    return self;
    }
    
CCFClientImpl::~CCFClientImpl()
    {
    FUNC_LOG;

    delete iContextService;
    delete iActionService;
    delete iScriptService;
    iCFClientSession.Close();
    }
    
void CCFClientImpl::ConstructL( MCFListener& aListener )
    {
    FUNC_LOG;
    
    // Connect to cfserver
    iCFClientSession.ConnectL();
    
    // Create services
    iContextService = CCFContextService::NewL( iCFClientSession, aListener );
    iActionService = CCFActionService::NewL( iCFClientSession, aListener );
    iScriptService = CCFScriptService::NewL( iCFClientSession, aListener );
    }
    
CCFClientImpl::CCFClientImpl()
    {
    FUNC_LOG;
    }

// METHODS

//----------------------------------------------------------------------------
// CCFClientImpl::DefineContext
//----------------------------------------------------------------------------
//
TInt CCFClientImpl::DefineContext( const TDesC& aContextSource,
    const TDesC& aContextType,
    const TSecurityPolicy& aSecurityPolicy )
    {
    FUNC_LOG;
    
    INFO_2( "Defining context through CFClient: [%S: %S]",
        &aContextSource,
        &aContextType );
    
    TInt err = iContextService->DefineContext( aContextSource,
        aContextType,
        aSecurityPolicy, aSecurityPolicy );

    ERROR_1( err, "Define context failed with code: %d", err );

    return err;
    }

//----------------------------------------------------------------------------
// CCFClientImpl::DefineContext
//----------------------------------------------------------------------------
//
TInt CCFClientImpl::DefineContext( const TDesC& aContextSource,
    const TDesC& aContextType,
    const TSecurityPolicy& aReadPolicy,
    const TSecurityPolicy& aWritePolicy )
    {
    FUNC_LOG;
    
    INFO_2( "Defining context through CFClient: [%S: %S]",
        &aContextSource,
        &aContextType );
        
    TInt err = iContextService->DefineContext( aContextSource,
        aContextType,
        aReadPolicy, aWritePolicy );

    ERROR_1( err, "Define context failed with code: %d", err );

    return err;
    }

//----------------------------------------------------------------------------
// CCFClientImpl::PublishContext
//----------------------------------------------------------------------------
//
TInt CCFClientImpl::PublishContext( CCFContextObject& aContext )
    {
    FUNC_LOG;
    
    INFO_2( "Publishing context through CFClient: [%S: %S]",
        &aContext.Source(),
        &aContext.Type() );
    
    TRAPD( err, iContextService->PublishContextL( aContext ) );
        
    ERROR_1( err, "Publish context failed with code: %d", err );

    return err;
    }

//----------------------------------------------------------------------------
// CCFClientImpl::PublishContext
//----------------------------------------------------------------------------
//
TInt CCFClientImpl::PublishContext( CCFContextObject& aContext,
    CCFContextDataObject& aData )
    {
    FUNC_LOG;

    INFO_2( "Publishing context through CFClient: [%S: %S]",
        &aContext.Source(),
        &aContext.Type() );
    INFO_1( "Publishing context data through CFClient: [%x]",
        aData.Uid() );
    
    TRAPD( err, iContextService->PublishContextL( aContext, aData ) );
        
    ERROR_1( err, "Publish context failed with code: %d", err );
    
    return err;
    }

//----------------------------------------------------------------------------
// CCFClientImpl::SubscribeContext
//----------------------------------------------------------------------------
//
TInt CCFClientImpl::SubscribeContext( CCFContextSubscription& aSubscription )
    {
    FUNC_LOG;
    
    INFO_2( "Subscribing context through CFClient: [%S: %S]",
        &aSubscription.ContextSource(),
        &aSubscription.ContextType() );
    
    TRAPD( err, iContextService->SubscribeContextL( aSubscription ) );
    
    ERROR_1( err, "Subscribe context failed with code: %d", err );

    return err;
    }

//----------------------------------------------------------------------------
// CCFClientImpl::Unsubscribe
//----------------------------------------------------------------------------
//
TInt CCFClientImpl::UnsubscribeContext( CCFContextSubscription& aSubscription )
    {
    FUNC_LOG;
    
    INFO_2( "Unsubscribing context through CFClient: [%S: %S]",
        &aSubscription.ContextSource(),
        &aSubscription.ContextType() );

    TRAPD( err, iContextService->UnsubscribeContextL( aSubscription ) );
    
    ERROR_1( err, "Unsubscribe context failed with code: %d", err );

    return err;
    }

//----------------------------------------------------------------------------
// CCFClientImpl::RequestContext
//----------------------------------------------------------------------------
//
TInt CCFClientImpl::RequestContext( CCFContextQuery& aContextQuery,
    RContextObjectArray& aRequestResults)
    {
    FUNC_LOG;
    
    INFO_2( "Requesting context through CFClient: [%S: %S]",
        &aContextQuery.Source(),
        &aContextQuery.Type() );

    TRAPD( err, iContextService->RequestContextL(
        aContextQuery, aRequestResults ) );

    ERROR_1( err, "Request context failed with code: %d", err );
    
    return err;
    }

//----------------------------------------------------------------------------
// CCFClientImpl::RequestContextSet
//----------------------------------------------------------------------------
//
TInt CCFClientImpl::RequestContextSet( RContextQueryArray& aContextQuerySet,
    RContextObjectArray& aRequestResults)
    {
    FUNC_LOG;
    
    TInt count = aContextQuerySet.Count();
    for( TInt i = 0; i < count; i++ )
        {
        INFO_2( "Requesting context through CFClient: [%S: %S]",
            &aContextQuerySet[i]->Source(),
            &aContextQuerySet[i]->Type() );
        }
    
    TRAPD( err, iContextService->RequestContextSetL(
        aContextQuerySet, aRequestResults ) );

    ERROR_1( err, "Request context set failed with code: %d", err );

    return err;
    }

//----------------------------------------------------------------------------
// CCFClientImpl::SubscribeAction
//----------------------------------------------------------------------------
//
TInt CCFClientImpl::SubscribeAction( CCFActionSubscription& aSubscription )
    {
    FUNC_LOG;
    
    INFO_1( "Subscribing action through CFClient: [%S]",
        &aSubscription.ActionIdentifier() );
    
    TRAPD( err, iActionService->SubscribeActionL( aSubscription ) );
    
    ERROR_1( err, "Subscribe action failed with code: %d", err );
    
    return err;
    }

//----------------------------------------------------------------------------
// CCFClientImpl::UnsubscribeAction
//----------------------------------------------------------------------------
//
TInt CCFClientImpl::UnsubscribeAction( CCFActionSubscription& aSubscription )
    {
    FUNC_LOG;
    
    INFO_1( "Unsubscribing action through CFClient: [%S]",
        &aSubscription.ActionIdentifier() );
    
    TRAPD( err, iActionService->UnsubscribeActionL( aSubscription ) );
    
    ERROR_1( err, "Unsubscribe action failed with code: %d", err );
    
    return err;
    }

//----------------------------------------------------------------------------
// CCFClientImpl::RegisterScript
//----------------------------------------------------------------------------
//
TInt CCFClientImpl::RegisterScript( const TDesC& aScript, TInt& aScriptId )
    {
    FUNC_LOG;
    
    INFO( "Registering script through CFClient" );

    TInt scriptId = iScriptService->RegisterScript( aScript );
    if (scriptId < 0)
    	{
    	ERROR_1( scriptId, "Script registering failed with code: %d",
    		scriptId );
    	return scriptId; // returned as an error
    	}
    else 
    	{
    	aScriptId = scriptId;
    	return KErrNone;
    	}
    }

//----------------------------------------------------------------------------
// CCFClientImpl::RegisterScript
//----------------------------------------------------------------------------
//
TInt CCFClientImpl::RegisterScript( const TDesC& aScriptFileName,
    const TDesC8& aScript,
    TInt& aScriptId )
    {
    FUNC_LOG;
    
    INFO( "Registering script through CFClient" );

    TInt scriptId = iScriptService->RegisterScript(
        aScript, aScriptFileName );
    if (scriptId < 0)
    	{
    	ERROR_1( scriptId, "Script registering failed with code: %d",
    		scriptId );
    	return scriptId; // returned as an error
    	}
    else 
    	{
    	aScriptId = scriptId;
    	return KErrNone;
    	}
    }

//----------------------------------------------------------------------------
// CCFClientImpl::DeregisterScript
//----------------------------------------------------------------------------
//
TInt CCFClientImpl::DeregisterScript( TInt aScriptId )
    {
    FUNC_LOG;
    
    INFO_1( "Deregistering script with id [%d] through CFClient", aScriptId );

    TInt err = iScriptService->DeregisterScript( aScriptId );
    
    ERROR( err, "Script deregistering failed!" );

    return err;
    }

//----------------------------------------------------------------------------
// CCFClientImpl::DefineAction
//----------------------------------------------------------------------------
//
TInt CCFClientImpl::DefineAction( const TDesC& aActionIdentifier,
    const TSecurityPolicy& aSecurityPolicy )
    {
    FUNC_LOG;
    
    INFO_1( "Defining action through CFClient: [%S]", &aActionIdentifier );
    
    TInt err = iActionService->DefineAction(
        aActionIdentifier,
        aSecurityPolicy );
    
    ERROR_1( err, "Define action failed with code: %d", err );
    
    return err;
    }

//----------------------------------------------------------------------------
// CCFClientImpl::SaveContextSourceSetting
//----------------------------------------------------------------------------
//
TInt CCFClientImpl::SaveContextSourceSetting( const TDesC& aSettingFilename,
    const TUid& aContextSourceUid )
    {
    FUNC_LOG;
    
    INFO_1( "Installing context source setting [%S] through CFClient",
            &aSettingFilename );
    
    TInt err = iContextService->InstallContextSourceSetting( aSettingFilename,
            aContextSourceUid );
    
    ERROR_1( err, "Installing context source setting [%S] through CFClient failed",
            &aSettingFilename );

    return err;
    }

//----------------------------------------------------------------------------
// CCFClientImpl::DeleteContextSourceSetting
//----------------------------------------------------------------------------
//
TInt CCFClientImpl::DeleteContextSourceSetting( const TDesC& aSettingFilename,
    const TUid& aContextSourceUid )
    {
    FUNC_LOG;
    
    INFO_1( "Uninstalling context source setting [%S] through CFClient",
            &aSettingFilename );
    
    TInt err = iContextService->UninstallContextSourceSetting( aSettingFilename,
            aContextSourceUid );
    
    ERROR_1( err, "Uninstalling context source setting [%S] through CFClient failed",
            &aSettingFilename );

    return err;
    }

//----------------------------------------------------------------------------
// CCFClientImpl::DeleteContextSourceSettings
//----------------------------------------------------------------------------
//
TInt CCFClientImpl::DeleteContextSourceSettings( const TUid& aContextSourceUid )
    {
    FUNC_LOG;
    
    INFO( "Uninstalling context source settings through CFClient" );
    
    TInt err = iContextService->UninstallContextSourceSettings(
            aContextSourceUid );
    
    ERROR( err, "Uninstalling context source settings through CFClient failed" );

    return err;
    }

//----------------------------------------------------------------------------
// CCFClientImpl::SaveScript
//----------------------------------------------------------------------------
//
TInt CCFClientImpl::SaveScript( const TDesC& aScriptFileName,
    TInt& aScriptId )
    {
    FUNC_LOG;
    
    INFO( "Register and save script through CFClient" );

    TInt err = iScriptService->SaveScript( aScriptFileName );
    if( err >= KErrNone )
        {
        aScriptId = err;
        err = KErrNone;
        }
    
    ERROR( err, "Register and save script through CFClient failed" );
    
    return err;
    }

//----------------------------------------------------------------------------
// CCFClientImpl::SaveScript
//----------------------------------------------------------------------------
//
TInt CCFClientImpl::SaveScript( const TDesC& aScriptFileName,
    const TDesC8& aScript,
    TInt& aScriptId )
    {
    FUNC_LOG;

    INFO( "Register and save script stream through CFClient" );

    TInt err = iScriptService->SaveScript(
        aScript, aScriptFileName );
    if( err >= KErrNone )
        {
        aScriptId = err;
        err = KErrNone;
        }
    
    ERROR( err, "Register and save script stream through CFClient failed" );
    
    return err;
    }

//----------------------------------------------------------------------------
// CCFClientImpl::DeleteScript
//----------------------------------------------------------------------------
//
TInt CCFClientImpl::DeleteScript( const TDesC& aScriptFileName )
    {
    FUNC_LOG;
    
    INFO( "Deregister and delete script through CFClient" );
    
    TInt err = iScriptService->DeleteScript( aScriptFileName );
    
    ERROR( err, "Deregister and delete script through CFClient failed" );
    
    return err;
    }

//----------------------------------------------------------------------------
// CCFClientImpl::DeleteScript
//----------------------------------------------------------------------------
//
TInt CCFClientImpl::DeleteScripts()
    {
    FUNC_LOG;
    
    INFO( "Deregister and delete scripts through CFClient" );
    
    TInt err = iScriptService->DeleteScripts();
    
    ERROR( err, "Deregister and delete scripts through CFClient failed" );

    return err;
    }

//----------------------------------------------------------------------------
// CCFClientImpl::UpgradeRomScript
//----------------------------------------------------------------------------
//
TInt CCFClientImpl::UpgradeRomScript( const TDesC& aScriptFileName )
    {
    FUNC_LOG;
    
    INFO( "Upgrade rom script through CFClient" );

    TInt err = iScriptService->UpgradeRomScript( aScriptFileName );
    
    ERROR( err, "Upgrade rom script through CFClient failed" );

    return err;
    }

//----------------------------------------------------------------------------
// CCFClientImpl::UpgradeRomScript
//----------------------------------------------------------------------------
//
TInt CCFClientImpl::UpgradeRomScript( const TDesC& aScriptFileName,
    const TDesC8& aScript )
    {
    FUNC_LOG;
    
    INFO( "Upgrade rom script des through CFClient" );

    TInt err = iScriptService->UpgradeRomScriptDes( aScriptFileName, aScript );
    
    ERROR( err, "Upgrade rom script des through CFClient failed" );

    return err;
    }

//----------------------------------------------------------------------------
// CCFClientImpl::RestoreRomScript
//----------------------------------------------------------------------------
//
TInt CCFClientImpl::RestoreRomScript( const TDesC& aScriptFileName )
    {
    FUNC_LOG;
    
    INFO( "Restore rom script through CFClient" );

    TInt err = iScriptService->RestoreRomScript( aScriptFileName );
    
    ERROR( err, "Restore rom script through CFClient failed" );

    return err;
    }

// End of file

/*
* Copyright (c) 2007-2008 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  CCFFakeEnv class implementation.
*
*/



// INCLUDES
#include "cffakeenv.h"

#include "CFScriptHandler.h"
#include "CFContextManager.h"
#include "CFScriptEvent.h"

#include <CFContextIndication.h>

// CONSTANTS

// Panic category
_LIT( KPanicCat, "CCFFakeEnv" );

// Panic codes
enum TPanicCode
    {
    ENullArg
    };

// ======== LOCAL FUNCTIONS ========

// Local panic function
LOCAL_C void Panic( TPanicCode aCode )
    {
    User::Panic( KPanicCat, aCode );
    }

// ======== MEMBER FUNCTIONS ========

// ---------------------------------------------------------------------------
// CCFFakeEnv::CCFFakeEnv
// C++ default constructor can NOT contain any code, that might leave.
// ---------------------------------------------------------------------------
//
CCFFakeEnv::CCFFakeEnv()
    {
    }

// ---------------------------------------------------------------------------
// CCFFakeEnv::ConstructL
// Symbian 2nd phase constructor can leave.
// ---------------------------------------------------------------------------
//
void CCFFakeEnv::ConstructL()
    {
    User::LeaveIfError( iFs.Connect() );
    iScriptHandler = CCFScriptHandler::NewL(
            *this, iFs, *this, *this);
    iContextManager = CCFContextManager::NewL( *this );
    }

// ---------------------------------------------------------------------------
// CCFCount::NewL
// Two-phased constructor.
// ---------------------------------------------------------------------------
//
CCFFakeEnv* CCFFakeEnv::NewL()
    {
    CCFFakeEnv* self = NewLC();
    CleanupStack::Pop( self );
    return self;
    }

// ---------------------------------------------------------------------------
// CCFFakeEnv::NewLC
// Two-phased constructor.
// ---------------------------------------------------------------------------
//
CCFFakeEnv* CCFFakeEnv::NewLC()
    {
    CCFFakeEnv* self = new( ELeave ) CCFFakeEnv();
	CleanupStack::PushL( self );
    self->ConstructL();
    return self;
    }


// Destructor
CCFFakeEnv::~CCFFakeEnv()
    {
    delete iContextIndication;
    delete iScriptEvent;
    delete iContextManager;
    delete iScriptHandler;
    iFs.Close();
    }


// ---------------------------------------------------------------------------
// CCFFakeEnv::ClearFireClientEvent
// ---------------------------------------------------------------------------
//
void CCFFakeEnv::ClearFireClientEvent()
    {
    iFireClientEvent = EFalse;
    }

// ---------------------------------------------------------------------------
// CCFFakeEnv::ClientEventFired
// ---------------------------------------------------------------------------
//
TBool CCFFakeEnv::ClientEventFired() const
    {
    return iFireClientEvent;
    }

// ---------------------------------------------------------------------------
// CCFFakeEnv::ClearFireClientEventCount
// ---------------------------------------------------------------------------
//
void CCFFakeEnv::ClearFireClientEventCount()
    {
    iFireClientEventCount = 0;
    }

// ---------------------------------------------------------------------------
// CCFFakeEnv::ClientEventFiredCount
// ---------------------------------------------------------------------------
//
TInt CCFFakeEnv::ClientEventFiredCount() const
    {
    return iFireClientEventCount;
    }

// ---------------------------------------------------------------------------
// CCFFakeEnv::ScriptEventScriptId
// ---------------------------------------------------------------------------
//
TInt CCFFakeEnv::ScriptEventScriptId() const
    {
    return iScriptEvent->ScriptId();
    }

// ---------------------------------------------------------------------------
// CCFFakeEnv::ScriptEventIdentifier
// ---------------------------------------------------------------------------
//
TPtrC CCFFakeEnv::ScriptEventIdentifier() const
    {
    return iScriptEvent->Identifier();
    }

// ---------------------------------------------------------------------------
// CCFFakeEnv::ScriptEventParameters
// ---------------------------------------------------------------------------
//
const RKeyValueArray& CCFFakeEnv::ScriptEventParameters() const
    {
    return iScriptEvent->Parameters();
    }

// ---------------------------------------------------------------------------
// CCFFakeEnv::SetActionNotFound
// ---------------------------------------------------------------------------
//
void CCFFakeEnv::SetActionNotFound( TInt aCount )
    {
    iActionNotFound = aCount;
    }

// ---------------------------------------------------------------------------
// CCFFakeEnv::SetAlwaysFailingActionPolicy
// ---------------------------------------------------------------------------
//
void CCFFakeEnv::SetAlwaysFailingActionPolicy( TInt aCount )
    {
    iAlwaysFailingActionPolicy = aCount;
    }

// ---------------------------------------------------------------------------
// CCFFakeEnv::IndicationArrived
// ---------------------------------------------------------------------------
//
TBool CCFFakeEnv::IndicationArrived()
    {
    return iIndication;
    }

// ---------------------------------------------------------------------------
// CCFFakeEnv::SetIndicationArrived
// ---------------------------------------------------------------------------
//
void CCFFakeEnv::SetIndicationArrived( TBool aValue )
    {
    iIndication = aValue;
    }

// ---------------------------------------------------------------------------
// CCFFakeEnv::IndicationContext
// ---------------------------------------------------------------------------
//
const CCFContextObject& CCFFakeEnv::IndicationContext()
    {
    return iContextIndication->Context();
    }

// ---------------------------------------------------------------------------
// CCFFakeEnv::SetBypassSecurityCheckCount
// ---------------------------------------------------------------------------
//
void CCFFakeEnv::SetBypassSecurityCheckCount( TInt aCount )
    {
    iBypassSecurityCheck = aCount;
    }

// ---------------------------------------------------------------------------
// CCFFakeEnv::DefineContext
// ---------------------------------------------------------------------------
//
TInt CCFFakeEnv::DefineContext( const TDesC& aContextSource,
    const TDesC& aContextType,
    const TSecurityPolicy& aReadPolicy,
    const TSecurityPolicy& aWritePolicy )
    {
    TRAPD( err, iContextManager->DefineContextL(
        aContextSource, aContextType,
        aReadPolicy, aWritePolicy ) );
    return err;
    }

// ---------------------------------------------------------------------------
// CCFFakeEnv::AddScript
// ---------------------------------------------------------------------------
//
TInt CCFFakeEnv::AddScript( const TDesC& aName,
    const TDesC8& aScript,
    const TUid& aOwner,
    const RThread& aOwnerThread )
    {
    return AddScript( aName, aScript, aOwner, aOwnerThread, *this, this );
    }

// ---------------------------------------------------------------------------
// CCFFakeEnv::SubscribeContext
// ---------------------------------------------------------------------------
//
TInt CCFFakeEnv::SubscribeContext( CCFContextSubscription* aSubscription,
    RThread& aClientThread )
    {
    return SubscribeContext( aSubscription, this, aClientThread );
    }

// ---------------------------------------------------------------------------
// CCFFakeEnv::InitDeviceStartingPhaseL
// ---------------------------------------------------------------------------
//
void CCFFakeEnv::InitDeviceStartingPhaseL()
    {
    MCFStarterObserver* instance =
        static_cast<MCFStarterObserver*>( iScriptHandler );
        instance->InitializePhaseL( CCFPhaseBase::ECFDeviceStarting );
    }

// ---------------------------------------------------------------------------
// CCFFakeEnv::InitDeviceStartedPhaseL
// ---------------------------------------------------------------------------
//
void CCFFakeEnv::InitDeviceStartedPhaseL()
    {
    MCFStarterObserver* instance =
        static_cast<MCFStarterObserver*>( iScriptHandler );
    instance->InitializePhaseL( CCFPhaseBase::ECFDeviceStarted );
    }

//------------------------------------------------------------------------------
// CCFFakeEnv::InitAllPhasesL
//------------------------------------------------------------------------------
//
void CCFFakeEnv::InitAllPhasesL()
    {
    MCFStarterObserver* instance =
        static_cast<MCFStarterObserver*>( iScriptHandler );
        
    instance->InitializePhaseL( CCFPhaseBase::ECFDeviceStarting );
    instance->InitializePhaseL( CCFPhaseBase::ECFDeviceStarted );
    instance->InitializePhaseL( CCFPhaseBase::ECFLoadingPlugins );
    instance->InitializePhaseL( CCFPhaseBase::ECFLoadingRules );
    instance->InitializePhaseL( CCFPhaseBase::ECFCfwReady );
    }

// ---------------------------------------------------------------------------
// CCFFakeEnv::DefineContext
// ---------------------------------------------------------------------------
//
TInt CCFFakeEnv::DefineContext( const TDesC& aContextSource,
    const TDesC& aContextType,
    const TSecurityPolicy& aReadWritePolicy )
    {
    TRAPD( err, iContextManager->DefineContextL(
        aContextSource, aContextType,
        aReadWritePolicy, aReadWritePolicy ) );
    return err;
    }

// ---------------------------------------------------------------------------
// CCFFakeEnv::DefineContext
// ---------------------------------------------------------------------------
//
TInt CCFFakeEnv::DefineContext( const TDesC& /*aContextSource*/,
    const TDesC& /*aContextType*/,
    const TSecurityPolicy& /*aReadWritePolicy*/,
    MCFContextSource& /*aOwner */)
    {
    return KErrNotSupported;
    }

// ---------------------------------------------------------------------------
// CCFFakeEnv::DefineContext
// ---------------------------------------------------------------------------
//
TInt CCFFakeEnv::DefineContext( const TDesC& /*aContextSource*/,
    const TDesC& /*aContextType*/,
    const TSecurityPolicy& /*aReadPolicy*/,
    const TSecurityPolicy& /*aWritePolicy*/,
    MCFContextSource& /*aOwner*/ )
    {
    return KErrNotSupported;
    }

// ---------------------------------------------------------------------------
// CCFFakeEnv::PublishContext
// ---------------------------------------------------------------------------
//
TInt CCFFakeEnv::PublishContext( CCFContextObject& aContext,
    RThread& aClientThread )
    {
    TRAPD( err, iContextManager->PublishContextL( aContext,
        aClientThread ) );
    return err;
    }

// ---------------------------------------------------------------------------
// CCFFakeEnv::PublishContext
// ---------------------------------------------------------------------------
//
TInt CCFFakeEnv::PublishContext( CCFContextObject& /*aContext*/,
    CCFContextDataObject& /*aData*/,
    RThread& /*aClientThread */ )
    {
    return KErrNone;
    }

// ---------------------------------------------------------------------------
// CCFFakeEnv::SubscribeContext
// ---------------------------------------------------------------------------
//
TInt CCFFakeEnv::SubscribeContext( CCFContextSubscription* aSubscription,
    MCFContextSubscriptionListener* aListener,
    RThread& aClientThread )
    {
    __ASSERT_ALWAYS( aSubscription != NULL, Panic( ENullArg ) );
    __ASSERT_ALWAYS( aListener != NULL, Panic( ENullArg ) );
    
    TRAPD( err,
        iContextManager->SubscribeContextL( aSubscription, aListener,
            aClientThread ) );
    return err;
    }

// ---------------------------------------------------------------------------
// CCFFakeEnv::UnsubscribeContext
// ---------------------------------------------------------------------------
//
void CCFFakeEnv::UnsubscribeContext( CCFContextSubscription& aSubscription,
    MCFContextSubscriptionListener& aListener )
    {
    iContextManager->RemoveSubscription( aSubscription, aListener );
    }

// ---------------------------------------------------------------------------
// CCFFakeEnv::UnsubscribeContexts
// ---------------------------------------------------------------------------
//
void CCFFakeEnv::UnsubscribeContexts(
    MCFContextSubscriptionListener& aObserver )
    {
    iContextManager->RemoveSubscriptions( &aObserver );
    }        

// ---------------------------------------------------------------------------
// CCFFakeEnv::RequestContext
// ---------------------------------------------------------------------------
//
TInt CCFFakeEnv::RequestContext( RContextObjectArray& aResultBuffer,
    CCFContextQuery& aContextQuery,
    RThread& aClientThread )
    {
    TRAPD( err, iContextManager->RequestContextL( aResultBuffer,
        aContextQuery,
        aClientThread ) );
    return err;
    }

// ---------------------------------------------------------------------------
// CCFFakeEnv::GetSecurityPolicy
// ---------------------------------------------------------------------------
//
TInt CCFFakeEnv::GetSecurityPolicy( const TDesC& aContextSource,
    const TDesC& aContextType,
    TSecurityPolicy& aPolicy )
    {
    TRAPD( err, iContextManager->GetWriteSecurityPolicyL( aContextSource,
        aContextType,
        aPolicy ) );
    return err; 
    }

// ---------------------------------------------------------------------------
// CCFFakeEnv::GetReadSecurityPolicy
// ---------------------------------------------------------------------------
//
TInt CCFFakeEnv::GetReadSecurityPolicy( const TDesC& aContextSource,
    const TDesC& aContextType,
    TSecurityPolicy& aPolicy )
    {
    TRAPD( err, iContextManager->GetReadSecurityPolicyL( aContextSource,
        aContextType,
        aPolicy ) );
    return err; 
    }

// ---------------------------------------------------------------------------
// CCFFakeEnv::GetWriteSecurityPolicy
// ---------------------------------------------------------------------------
//
TInt CCFFakeEnv::GetWriteSecurityPolicy( const TDesC& aContextSource,
    const TDesC& aContextType,
    TSecurityPolicy& aPolicy )
    {
    TRAPD( err, iContextManager->GetWriteSecurityPolicyL( aContextSource,
        aContextType,
        aPolicy ) );
    return err; 
    }

// ---------------------------------------------------------------------------
// CCFFakeEnv::DefineContext
// ---------------------------------------------------------------------------
//
TInt CCFFakeEnv::DefineContext( const TDesC& aContextSource,
    const TDesC& aContextType,
    const TSecurityPolicy& aReadPolicy,
    const TSecurityPolicy& aWritePolicy,
    MCFContextSource* aPublisher, const TUid& aPublisherUid )
    {
    TRAPD( err, iContextManager->DefineContextL(
        aContextSource, aContextType,
        aReadPolicy, aWritePolicy, aPublisher, aPublisherUid ) );
    return err;
    }

// ---------------------------------------------------------------------------
// CCFFakeEnv::DeregisterPublisher
// ---------------------------------------------------------------------------
//
void CCFFakeEnv::DeregisterPublisher( MCFContextSource& aPublisher )
    {
    iContextManager->DeregisterPublisher( aPublisher );
    }

// ---------------------------------------------------------------------------
// CCFFakeEnv::InstallContextSourceSetting
// ---------------------------------------------------------------------------
//
TInt CCFFakeEnv::InstallContextSourceSetting( RFile& /*aSettingFile*/,
    const TUid& /*aContextSourceUid*/,
    RThread& /*aClientThread*/ )
    {
    return KErrExtensionNotSupported;
    }

// ---------------------------------------------------------------------------
// CCFFakeEnv::UninstallContextSourceSetting
// ---------------------------------------------------------------------------
//
TInt CCFFakeEnv::UninstallContextSourceSetting(
    const TDesC& /*aSettingFilename*/,
    const TUid& /*aContextSourceUid*/,
    RThread& /*aClientThread*/ )
    {
    return KErrExtensionNotSupported;
    }

// ---------------------------------------------------------------------------
// CCFFakeEnv::UninstallContextSourceSettings
// ---------------------------------------------------------------------------
//
TInt CCFFakeEnv::UninstallContextSourceSettings(
    const TUid& /*aContextSourceUid*/,
    RThread& /*aClientThread*/ )
    {
    return KErrExtensionNotSupported;
    }

// ---------------------------------------------------------------------------
// CCFFakeEnv::PublishContextFromAction
// ---------------------------------------------------------------------------
//
TInt CCFFakeEnv::PublishContextFromAction( CCFContextObject& aContext,
    TBool aAutoDefine )
    {
    RThread thread;
    TInt err( KErrNone );
    TRAP( err, iContextManager->PublishContextL( aContext, thread ) );
    if( err == KErrNotFound && aAutoDefine )
        {
        TSecurityPolicy writePolicy( thread.SecureId() );
        _LIT_SECURITY_POLICY_PASS( readPolicy );
        TRAP( err, iContextManager->DefineContextL( aContext.Source(),
                aContext.Type(),
                readPolicy,
                writePolicy,
                NULL,
                thread.SecureId() ) );
        if( err == KErrNone )
            {
            TRAP( err, iContextManager->PublishContextL( aContext, thread ) );
            }
        }
    thread.Close();
    return err;
    }

//------------------------------------------------------------------------------
// CCFFakeEnv::HandleScriptsRemoved
//------------------------------------------------------------------------------
//
void CCFFakeEnv::HandleScriptsRemoved( const RArray<TInt>& /*aScriptIds*/ )
    {
    
    }

// ---------------------------------------------------------------------------
// CCFFakeEnv::RemoveScriptById
// ---------------------------------------------------------------------------
//
TInt CCFFakeEnv::RemoveScriptById( TInt aScriptId, const RThread& aOwner )
    {
    return iScriptHandler->RemoveScriptById( aScriptId, aOwner );
    }

// ---------------------------------------------------------------------------
// CCFFakeEnv::RemoveScriptsBySession
// ---------------------------------------------------------------------------
//
TInt CCFFakeEnv::RemoveScriptsBySession(
    const MCFActionHandler& aSession )
    {
    return iScriptHandler->RemoveScriptsBySession( aSession );
    }

// ---------------------------------------------------------------------------
// CCFFakeEnv::RemoveScriptByName
// ---------------------------------------------------------------------------
//
TInt CCFFakeEnv::RemoveScriptByName( const TDesC& aScriptName,
    const TUid& aOwnerUid )
    {
    return iScriptHandler->RemoveScriptByName( aScriptName, aOwnerUid );
    }

// ---------------------------------------------------------------------------
// CCFFakeEnv::RemoveScriptByUid
// ---------------------------------------------------------------------------
//
TInt CCFFakeEnv::RemoveScriptByUid( const TUid& aUid )
    {
    return iScriptHandler->RemoveScriptByUid( aUid );
    }

// ---------------------------------------------------------------------------
// CCFFakeEnv::AddScript
// ---------------------------------------------------------------------------
//
TInt CCFFakeEnv::AddScript( const TDesC& aName,
    const TDesC8& aScript,
    const TUid& aOwner,
    const RThread& aOwnerThread,
    MCFActionHandler& aSession,
    MCFScriptOwner* aScriptOwner )
    {
    return iScriptHandler->AddScript(
            aName, aScript, aOwner, aOwnerThread, aSession, aScriptOwner );
    }

// ---------------------------------------------------------------------------
// CCFFakeEnv::NumberOfScriptsByOwner
// ---------------------------------------------------------------------------
//
TInt CCFFakeEnv::NumberOfScriptsByOwner( const TUid& aOwner )
    {
    return iScriptHandler->NumberOfScriptsByOwner( aOwner );
    }

// ---------------------------------------------------------------------------
// CCFFakeEnv::NumberOfScripts
// ---------------------------------------------------------------------------
//
TInt CCFFakeEnv::NumberOfScripts()
    {
    return iScriptHandler->NumberOfScripts();
    }

// ---------------------------------------------------------------------------
// CCFFakeEnv::ScriptLength
// ---------------------------------------------------------------------------
//
TInt CCFFakeEnv::ScriptLength( TInt aScriptId )
    {
    return iScriptHandler->ScriptLength( aScriptId );
    }

// ---------------------------------------------------------------------------
// CCFFakeEnv::GetEveryScriptId
// ---------------------------------------------------------------------------
//
const RArray< TInt >& CCFFakeEnv::GetEveryScriptId()
    {
    return iScriptHandler->GetEveryScriptId();
    }

// ---------------------------------------------------------------------------
// CCFFakeEnv::GetEveryScriptIdByOwner
// ---------------------------------------------------------------------------
//
const RArray< TInt >& CCFFakeEnv::GetEveryScriptIdByOwner(
    const TUid& aScriptOwner )
    {
    return iScriptHandler->GetEveryScriptIdByOwner( aScriptOwner );
    }

// ---------------------------------------------------------------------------
// CCFFakeEnv::UpdateScript
// ---------------------------------------------------------------------------
//
TInt CCFFakeEnv::UpdateScript( TInt aScriptID,
    const RThread& aOwnerThread,
    const TDesC8& aUpdatedScript,
    MCFScriptOwner* aScriptOwner )
    {
    return iScriptHandler->UpdateScript(
            aScriptID, aOwnerThread, aUpdatedScript, aScriptOwner );
    }

// ---------------------------------------------------------------------------
// CCFFakeEnv::SaveScript
// ---------------------------------------------------------------------------
//
TInt CCFFakeEnv::SaveScript( const TDesC8& aScript,
    TInt aScriptId,
    const TUid& aOwnerUid )
    {
    return iScriptHandler->SaveScript( aScript, aScriptId, aOwnerUid );
    }

// ---------------------------------------------------------------------------
// CCFFakeEnv::DeleteScriptByName
// ---------------------------------------------------------------------------
//
TInt CCFFakeEnv::DeleteScriptByName( const TDesC& aScriptName,
    const TUid& aOwner )
    {
    return iScriptHandler->DeleteScriptByName( aScriptName, aOwner );
    }

// ---------------------------------------------------------------------------
// CCFFakeEnv::DeleteScriptByUid
// ---------------------------------------------------------------------------
//
TInt CCFFakeEnv::DeleteScriptByUid( const TUid& aUid )
    {
    return iScriptHandler->DeleteScriptByUid( aUid );
    }

// ---------------------------------------------------------------------------
// CCFFakeEnv::AlreadyExists
// ---------------------------------------------------------------------------
//
TBool CCFFakeEnv::AlreadyExists( const TDesC& aScriptName,
    const TUid& aOwnerUid,
    TInt& aScriptId ) const
    {
    return iScriptHandler->AlreadyExists( aScriptName, aOwnerUid, aScriptId );
    }

// ---------------------------------------------------------------------------
// CCFFakeEnv::CleanupPersistentDataByName
// ---------------------------------------------------------------------------
//
void CCFFakeEnv::CleanupPersistentDataByName( const TDesC& aScriptName,
    const TUid& aOwnerUid )
    {
    return iScriptHandler->CleanupPersistentDataByName( aScriptName, aOwnerUid );
    }

// ---------------------------------------------------------------------------
// CCFFakeEnv::CleanupPersistentDataByUid
// ---------------------------------------------------------------------------
//
void CCFFakeEnv::CleanupPersistentDataByUid( const TUid& aOwnerUid )
    {
    return iScriptHandler->CleanupPersistentDataByUid( aOwnerUid );
    }

// ---------------------------------------------------------------------------
// CCFFakeEnv::RestoreRomScript
// ---------------------------------------------------------------------------
//
TInt CCFFakeEnv::RestoreRomScript( TInt aScriptId, const TUid& aOwnerUid,
    const RThread& aClient )
    {
    return iScriptHandler->RestoreRomScript( aScriptId, aOwnerUid, aClient );
    }

// ---------------------------------------------------------------------------
// CCFFakeEnv::IsUpgradeAllowed
// ---------------------------------------------------------------------------
//
TInt CCFFakeEnv::IsUpgradeAllowed( const TDesC& aName,
    const TDesC8& aScript,
    const TUid& aOwner,
    const RThread& aOwnerThread,
    MCFActionHandler& aActionHandler )
    {
    return iScriptHandler->IsUpgradeAllowed( aName, aScript, aOwner,
        aOwnerThread, aActionHandler );
    }

// ---------------------------------------------------------------------------
// CCFFakeEnv::RemoveScriptByProviderUid
// ---------------------------------------------------------------------------
//
TInt CCFFakeEnv::RemoveScriptByProviderUid( const TUid& aProviderUid,
    TBool aRollback )
    {
    return iScriptHandler->RemoveScriptByProviderUid( aProviderUid,
            aRollback );
    }

// ---------------------------------------------------------------------------
// CCFFakeEnv::RollbackScripts
// ---------------------------------------------------------------------------
//
void CCFFakeEnv::RollbackScripts()
    {
    iScriptHandler->RollbackScripts();
    }

//------------------------------------------------------------------------------
// CCFFakeEnv::DeregisterScriptOwner
//------------------------------------------------------------------------------
//
void CCFFakeEnv::DeregisterScriptOwner( MCFScriptOwner* aScriptOwner )
    {
    iScriptHandler->DeregisterScriptOwner( aScriptOwner );
    }

// ---------------------------------------------------------------------------
// CCFFakeEnv::FireActionL
// ---------------------------------------------------------------------------
//
void CCFFakeEnv::FireActionL( CCFScriptEvent* aEvent )
    {
    iFireClientEvent = ETrue;
    delete iScriptEvent;
    iScriptEvent = aEvent;
    iFireClientEventCount += 1;
    }

// ---------------------------------------------------------------------------
// CCFFakeEnv::FireActionL
// ---------------------------------------------------------------------------
//
void CCFFakeEnv::FireActionL( const CCFContextSourceCommand& /*aCommand*/ )
    {
    // Nothing must be done.
    }

// ---------------------------------------------------------------------------
// CCFFakeEnv::GetActionSecurityPolicy
// ---------------------------------------------------------------------------
//
TInt CCFFakeEnv::GetActionSecurityPolicy( const TDesC& /*aActionIdentifier*/,
    TSecurityPolicy& aPolicy )
    {
    static _LIT_SECURITY_POLICY_FAIL( alwaysFail );
    static _LIT_SECURITY_POLICY_PASS( alwaysPass );

    if ( iActionNotFound > 0 )
        {
        iActionNotFound--;
        return KErrNotFound;
        }

    if ( iAlwaysFailingActionPolicy > 0 )
        {
        aPolicy = alwaysFail;
        iAlwaysFailingActionPolicy--;     
        return KErrNone;
        }

    aPolicy = alwaysPass;
    return KErrNone;
    }

// ---------------------------------------------------------------------------
// CCFFakeEnv::CheckClientSecurity
// ---------------------------------------------------------------------------
//
TBool CCFFakeEnv::CheckClientSecurity( const RThread& aClientThread,
    const TSecurityPolicy& aSecurityPolicy )
    {
    // Security check only needed if request is from other process than CF
    TBool securityPassed = ETrue;

    if ( iBypassSecurityCheck == 0 )
        {
        securityPassed = aSecurityPolicy.CheckPolicy( aClientThread );
        }
    else
        {
        iBypassSecurityCheck--; 
        }
    return securityPassed;
    }

// ---------------------------------------------------------------------------
// CCFFakeEnv::ContextIndicationL
// ---------------------------------------------------------------------------
//
void CCFFakeEnv::ContextIndicationL( CCFContextIndication* aIndication )
    {
    iIndication = ETrue;
    delete iContextIndication;
    iContextIndication = aIndication;       
    }

// ---------------------------------------------------------------------------
// CCFFakeEnv::Client
// ---------------------------------------------------------------------------
//
TInt CCFFakeEnv::Client( RThread& aThread ) const
    {
    aThread = RThread();
    return KErrNone;
    }

// ---------------------------------------------------------------------------
// CCFFakeEnv::HandleContextSubscriptionError
// ---------------------------------------------------------------------------
//
void CCFFakeEnv::HandleContextSubscriptionError( TInt /*aError*/,
    const TDesC& /*aSource*/,
    const TDesC& /*aType*/ )
    {
    iSubscriptionError = ETrue;
    }

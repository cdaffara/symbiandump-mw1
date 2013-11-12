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
* Description:  
*
*/



#include <e32std.h>
#include <gmxmlelement.h>
#include <cfcontextsourcecommand.h>

#include "cfextendedcontextinterface.h"
#include "cfcontextindication.h"
#include "cfpluginoperation.h"
#include "CFScript.h"
#include "CFContextSubscriptionImpl.h"
#include "cfscriptsubscription.h"
#include "cfoperationnode.h"
#include "cfscriptroot.h"
#include "cfconstants.h"
#include "cfscriptlistener.h"
#include "cfactionhandler.h"
#include "cftrace.h"
#include "cfpersistentdata.h"
#include "cfscriptinfo.h"

// MEMBER FUNCTIONS

// -----------------------------------------------------------------------------
// CCFScript::CCFScript
// C++ default constructor can NOT contain any code, that might leave.
// -----------------------------------------------------------------------------
//
CCFScript::CCFScript( MCFExtendedContextInterface& aCF,
    MCFActionHandler& aActionHandler,
    MCFSecurityChecker& aSecurityChecker,
    MCFPlugInOperation& aPlugInOperation,
    RFs& aFs ):
    iCF( aCF ),
    iActionHandler( aActionHandler ),
    iSecurityChecker( aSecurityChecker ),
    iPlugInOperation( aPlugInOperation ),
    iFs( aFs )
    {
    FUNC_LOG;
    }

// -----------------------------------------------------------------------------
// CCFScript::ConstructL
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CCFScript::ConstructL( CMDXMLNode& aStartNode, const TDesC& aName,
    TInt aScriptId, const TUid& aOwner, TInt aLength,
    MCFScriptOwner* aScriptOwner )
    {
    FUNC_LOG;

    // local variable declaration & initialization
    TInt err( KErrNone );

    // Script related information
    iInfo = CCFScriptInfo::NewL( aName, aScriptId, aOwner, aLength, aScriptOwner );
    iPersistentData = CCFPersistentData::NewL( iFs, aOwner, iInfo->Name() );
    
    // parse script to objects
    err = ParseScriptL( aStartNode );
    ERROR_1( err, "Parse script error: %d", err );

    // leave if error occurred
    User::LeaveIfError( err );
    }

// -----------------------------------------------------------------------------
// CCFScript::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CCFScript* CCFScript::NewL( CMDXMLNode& aStartNode,
    const TDesC& aName,
    TInt aScriptId,
    const TUid& aOwner,
    MCFActionHandler& aActionHandler,
    TInt aLength,
    MCFExtendedContextInterface& aCF,
    MCFSecurityChecker& aSecurityChecker,
    MCFPlugInOperation& aPlugInOperation,
    RFs& aFs,
    MCFScriptOwner* aScriptOwner )
    {
    FUNC_LOG;

    CCFScript* self = NewLC( aStartNode,
        aName,
        aScriptId,
        aOwner,
        aActionHandler,
        aLength,
        aCF,
        aSecurityChecker,
        aPlugInOperation,
        aFs,
        aScriptOwner );
    CleanupStack::Pop( self );
    return self;
    }

// -----------------------------------------------------------------------------
// CCFScript::NewLC
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CCFScript* CCFScript::NewLC( CMDXMLNode& aStartNode,
    const TDesC& aName,
    TInt aScriptId,
    const TUid& aOwner,
    MCFActionHandler& aActionHandler,
    TInt aLength,
    MCFExtendedContextInterface& aCF,
    MCFSecurityChecker& aSecurityChecker,
    MCFPlugInOperation& aPlugInOperation,
    RFs& aFs,
    MCFScriptOwner* aScriptOwner )
    {
    FUNC_LOG;

    CCFScript* self = new( ELeave ) CCFScript( aCF,
        aActionHandler,
        aSecurityChecker,
        aPlugInOperation,
        aFs );
    CleanupStack::PushL( self );
    self->ConstructL( aStartNode, aName, aScriptId, aOwner, aLength, aScriptOwner );
    return self;
    }

// Destructor
CCFScript::~CCFScript()
    {
    FUNC_LOG;

	iCF.UnsubscribeContexts( *this );
    iScriptSubscriptions.ResetAndDestroy();
    iRequiresAll.Close();
    delete iScriptRoot;
    iScriptRoot = NULL;
    delete iPersistentData;
    delete iInfo;
    iDependencyList.Close();
    }

// -----------------------------------------------------------------------------
// CCFScript::ActivateL
// -----------------------------------------------------------------------------
//
void CCFScript::ActivateL()
    {
    FUNC_LOG;

    iScriptRoot->ActivateL();
    }

// -----------------------------------------------------------------------------
// CCFScript::CheckSecurity
// -----------------------------------------------------------------------------
//
TInt CCFScript::CheckSecurity( const RThread& aOwnerThread )
    {
    FUNC_LOG;

    iScriptThreadForSecurityCheck = &aOwnerThread;

    TInt err = iScriptRoot->CheckSecurity();

    iScriptThreadForSecurityCheck = NULL;

    return err;
    }

// -----------------------------------------------------------------------------
// CCFScript::ActionHandler
// -----------------------------------------------------------------------------
//
MCFActionHandler& CCFScript::ActionHandler() const
    {
    FUNC_LOG;

    return iActionHandler;
    }

// -----------------------------------------------------------------------------
// CCFScript::UpgradeSecurity
// -----------------------------------------------------------------------------
//
const TCapabilitySet& CCFScript::UpgradeSecurity() const
    {
    FUNC_LOG;
    
    return iScriptRoot->UpgradeSecurity();
    }
 
// -----------------------------------------------------------------------------
// CCFScript::ContextIndicationL
// 
// -----------------------------------------------------------------------------
//
void CCFScript::ContextIndicationL( CCFContextIndication* aIndication ) 
    {
    FUNC_LOG;

    // Initialize local variables.
    TInt contextLevelDelay( 0 );

    // Guards firing actions so that an operation of the script must have been
    // evaluated before actions are allowed to be fired.
    TBool evaluated = EvaluateScript( aIndication->Context(),
            contextLevelDelay );
    delete aIndication; // Cleanup the transferred indication.
    aIndication = NULL;

    if ( evaluated )
        {
        TInt err( KErrNone );
        TRAP( err, iScriptRoot->ContextEvaluatedL( contextLevelDelay ) );

        ERROR( err, "Signalling script evaluation by context to script root leaved." );
        }
    }

// -----------------------------------------------------------------------------
// CCFScript::Client
// 
// -----------------------------------------------------------------------------
//
TInt CCFScript::Client( RThread& /*aThread*/ ) const
    {
    FUNC_LOG;

    return KErrNone;
    }

// -----------------------------------------------------------------------------
// CCFScript::HandleContextSubscriptionError
// 
// -----------------------------------------------------------------------------
//
void CCFScript::HandleContextSubscriptionError( TInt /*aError*/,
    const TDesC& /*aSource*/,
    const TDesC& /*aType*/ )
    {
    FUNC_LOG;

    // Nothing to do.
    // Generally this can only happen with OOM
    }


// -----------------------------------------------------------------------------
// CCFScript::SubscribeContextL
// -----------------------------------------------------------------------------
//
void CCFScript::SubscribeContextL( MCFScriptListener* aListener )
    {
    FUNC_LOG;

    CCFScriptSubscription* subscription = NULL;
    for ( TInt i = 0; i < iScriptSubscriptions.Count(); ++i )
        {
        subscription = iScriptSubscriptions[ i ];
        if ( subscription->Match( *aListener ) )
            {
            break;
            }
        subscription = NULL;
        }

    if ( subscription )
        {
        subscription->AddListenerL( aListener );
        }
    else
        {
        // below                                    // CLEANUP<< subscription
        subscription = CCFScriptSubscription::NewLC( iCF, *this, aListener );
        iScriptSubscriptions.AppendL( subscription );
        CleanupStack::Pop( subscription );          // CLEANUP>> subscription
        }

    if ( aListener->IsAllRequired() )
        {
        iRequiresAll.AppendL( aListener );
        }
    }

// -----------------------------------------------------------------------------
// CCFScript::RemoveSubscription
// -----------------------------------------------------------------------------
//
void CCFScript::RemoveSubscription( MCFScriptListener* aListener )
    {
    FUNC_LOG;

    for ( TInt i = 0; i < iScriptSubscriptions.Count(); ++i )
        {
        CCFScriptSubscription* subscription = iScriptSubscriptions[ i ];
        if ( subscription->Match( *aListener ) )
            {
            if ( subscription->RemoveListener( aListener ) )
                {
                // Last listener removed, clean up.
                iScriptSubscriptions.Remove( i );
                delete subscription;
                }
            break;
            }
        }

    if ( aListener->IsAllRequired() )
        {
        TInt index = iRequiresAll.Find( aListener );
        if ( index != KErrNotFound )
            {
            iRequiresAll.Remove( index );
            }
        }
    }

// -----------------------------------------------------------------------------
// CCFScript::CheckContextReadSecurity
// -----------------------------------------------------------------------------
//
TInt CCFScript::CheckContextReadSecurity( const TDesC& aSource,
    const TDesC& aType )
    {
    FUNC_LOG;

    TSecurityPolicy securityPolicy;
    TInt err = iCF.GetReadSecurityPolicy( aSource, aType, securityPolicy );
    if ( err != KErrNone )
        {
        return err;
        }

    TBool securityPassed = iSecurityChecker.CheckClientSecurity( 
        *iScriptThreadForSecurityCheck, securityPolicy );
    if (!securityPassed)
        {
        return KErrPermissionDenied;
        }

    return KErrNone;
    }

// -----------------------------------------------------------------------------
// CCFScript::CheckWriteSecurity
// -----------------------------------------------------------------------------
//
TInt CCFScript::CheckScriptOwnerAccess( const TSecurityPolicy& aPolicy )
    {
    FUNC_LOG;

    TInt ret( KErrNone );

    TBool securityPassed = iSecurityChecker.CheckClientSecurity( 
        *iScriptThreadForSecurityCheck, aPolicy );
    if ( !securityPassed )
        {
        ret = KErrPermissionDenied;
        }

    return ret;
    }

// -----------------------------------------------------------------------------
// CCFScript::ParseL
// -----------------------------------------------------------------------------
//
CCFOperationNode* CCFScript::ParseL( CCFOperationNode* aParent,
    CMDXMLNode& aNode )
    {
    FUNC_LOG;

    TUid providerUid = KNullUid;
    CCFOperationNode* opNode = iPlugInOperation.ParseL(
        aParent, aNode, *this, providerUid );
    if ( opNode )
        {
        // Node found, add dependency
        AddDependencyL( providerUid );
        }
    else
        {
        TPtrC nodeName( aNode.NodeName() );
        ERROR_GEN_1( "Unknown node name: %S" , &nodeName );
        }

    return opNode;
    }

//------------------------------------------------------------------------------
// CCFScript::ScriptId
//------------------------------------------------------------------------------
//
TInt CCFScript::ScriptId() const
    {
    FUNC_LOG;
    
    return iInfo->Id();
    }

// -----------------------------------------------------------------------------
// CCFScript::RestoreL
// -----------------------------------------------------------------------------
//
void CCFScript::RestoreL( const TDesC& aFile, CCFOperationNode& aOperation )
    {
    FUNC_LOG;

    iPersistentData->RestoreL( aFile, aOperation );
    }

// -----------------------------------------------------------------------------
// CCFScript::StoreL
// -----------------------------------------------------------------------------
//
void CCFScript::StoreL( const TDesC& aFile, CCFOperationNode& aOperation )
    {
    FUNC_LOG;

    iPersistentData->StoreL( aFile, aOperation );
    }

// -----------------------------------------------------------------------------
// CCFScript::StoreL
// -----------------------------------------------------------------------------
//
void CCFScript::Delete( const TDesC& aFile )
	{
	FUNC_LOG;
	
	iPersistentData->Delete( aFile );
	}


// -----------------------------------------------------------------------------
// CCFScript::LaunchActivatedActions
// -----------------------------------------------------------------------------
//
void CCFScript::LaunchActivatedActions()
	{
    FUNC_LOG;

    TInt err( KErrNone );
    TRAP( err, iScriptRoot->EvaluatedL() );

    ERROR( err, "Signalling script evaluation without context to script root leaved." );
	}

// -----------------------------------------------------------------------------
// CCFScript::ContextInterface
// -----------------------------------------------------------------------------
//
MCFContextInterface& CCFScript::ContextInterface() const
    {
    FUNC_LOG;

    return iCF;
    }

// -----------------------------------------------------------------------------
// CCFScript::FireActionL
// -----------------------------------------------------------------------------
//
void CCFScript::FireActionL( CCFScriptEvent* aEvent )
    {
    FUNC_LOG;

    iActionHandler.FireActionL( aEvent );
    }

// -----------------------------------------------------------------------------
// CCFScript::FireActionL
// -----------------------------------------------------------------------------
//
void CCFScript::FireActionL( const CCFContextSourceCommand& aCommand )
    {
    FUNC_LOG;

    iActionHandler.FireActionL( aCommand );
    }

// -----------------------------------------------------------------------------
// CCFScript::GetSecurityPolicy
// -----------------------------------------------------------------------------
//
TInt CCFScript::GetActionSecurityPolicy( const TDesC& aActionId,
    TSecurityPolicy& aPolicy )
    {
    FUNC_LOG;

    return iActionHandler.GetActionSecurityPolicy( aActionId, aPolicy );
    }

// -----------------------------------------------------------------------------
// CCFScript::CleanupPersistentData
// -----------------------------------------------------------------------------
//
void CCFScript::CleanupPersistentData()
    {
    FUNC_LOG;
    
    iScriptRoot->Cleanup();
    }

//------------------------------------------------------------------------------
// CCFScript::HasDependency
//------------------------------------------------------------------------------
//
TBool CCFScript::HasDependency( const TUid& aUid ) const
    {
    FUNC_LOG;

    // Check if the dependency is needed to be added
    TBool found = EFalse;
    if( aUid != KNullUid )
        {
        for( TInt i = 0; i < iDependencyList.Count(); i++ )
            {
            if( iDependencyList[i] == aUid )
                {
                found = ETrue;
                break;
                }
            }
        }
    return found;
    }

//------------------------------------------------------------------------------
// CCFScript::CopyInfoLC
//------------------------------------------------------------------------------
//
CCFScriptInfo* CCFScript::CopyInfoLC() const
    {
    FUNC_LOG;

    return CCFScriptInfo::NewLC( iInfo->Name(),
        iInfo->Id(),
        iInfo->OwnerUid(),
        iInfo->Length(),
        iInfo->OwnerSession() );
    }

//------------------------------------------------------------------------------
// CCFScript::OwnerUid
//------------------------------------------------------------------------------
//
const TUid& CCFScript::OwnerUid() const
    {
    FUNC_LOG;
    
    return iInfo->OwnerUid();
    }

//------------------------------------------------------------------------------
// CCFScript::Name
//------------------------------------------------------------------------------
//
TPtrC CCFScript::Name() const
    {
    FUNC_LOG;
    
    return iInfo->Name();
    }

//------------------------------------------------------------------------------
// CCFScript::Length
//------------------------------------------------------------------------------
//
TInt CCFScript::Length() const
    {
    FUNC_LOG;
    
    return iInfo->Length();
    }

//------------------------------------------------------------------------------
// CCFScript::Info
//------------------------------------------------------------------------------
//
CCFScriptInfo& CCFScript::Info() const
    {
    FUNC_LOG;
    
    return *iInfo;
    }

// -----------------------------------------------------------------------------
// CCFScript::ParseScriptL
// -----------------------------------------------------------------------------
//
TInt CCFScript::ParseScriptL( CMDXMLNode& aStartNode )
    {
    FUNC_LOG;

    TUid providerUid = KNullUid;
    iScriptRoot = iPlugInOperation.ParseScriptRootL(
        NULL, aStartNode, *this, providerUid );
    TInt err( KErrNone );
    if ( iScriptRoot )
        {
        // Script root found, add dependency
        AddDependencyL( providerUid );
        }
    else
        {
        ERROR_GEN( "CCFScript::ParseScriptL: The script has invalid root element" );
        err = KErrNotFound;
        }

    return err;
    }

// -----------------------------------------------------------------------------
// CCFScript::EvaluateScript
// -----------------------------------------------------------------------------
//
TBool CCFScript::EvaluateScript( const CCFContextObject& aContext,
    TInt& aContextLevelDelay )
    {
    FUNC_LOG;

    TBool evaluated( EFalse );
    TInt delayRequirement( 0 );
    TInt delay( 0 );

    TPtrC name( iInfo->Name() );
    INFO_2( "CCFScript::EvaluateScript - Evaluating script NAME=[%S] ID=[%d] with context:",
        &name, iInfo->Id() );
    INFO_3( ">> %S: %S: %S", &aContext.Source(), &aContext.Type(), &aContext.Value() );

    // Evaluate first all operations requiring all context indications.
    for ( TInt index = 0; index < iRequiresAll.Count(); ++index )
        {
        if ( iRequiresAll[ index ]->Evaluate( aContext, delay ) )
            {
            evaluated = ETrue;
            if ( delay > delayRequirement )
                {
                delayRequirement = delay;
                }
            }
        }

    for ( TInt j = 0; j < iScriptSubscriptions.Count(); ++j )
        {
        CCFScriptSubscription* subscription = iScriptSubscriptions[ j ];
        if ( subscription->Match( aContext ) )
            {
            if ( subscription->NotifyListeners( aContext, delay ) )
                {
                evaluated = ETrue;
                }

            if ( delay > delayRequirement )
                {
                delayRequirement = delay;
                }
            break;
            }
        }

    if ( delayRequirement > 0 )
        {
        aContextLevelDelay = delayRequirement;
        }

    INFO_3( "CCFScript::EvaluateScript - Evaluated script ID=[%d]: CtxLevDelay=%dms, (1=evaluated, 0=no evaluation): %d",
            iInfo->Id(), aContextLevelDelay, evaluated );

    return evaluated;
    }

//------------------------------------------------------------------------------
// CCFScript::AddDependencyL
//------------------------------------------------------------------------------
//
void CCFScript::AddDependencyL( const TUid& aUid )
    {
    FUNC_LOG;
    
    // Check if the dependency is needed to be added
    TBool found = HasDependency( aUid );
    if( !found )
        {
        iDependencyList.AppendL( aUid );
        }
    }

// End of file


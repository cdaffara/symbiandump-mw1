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
* Description:  CFActivator class implementation.
*
*/


#include <f32file.h>
#include <s32file.h>

#include "CFActivator.h"
#include "CFActivatorEngineActionPluginManager.h"
#include "cfscriptevent.h"
#include "CFActionCacheElement.h"
#include "CFActionSubscriptionImpl.h"
#include "CFActionSubscriptionListener.h"
#include "CFActionIndicationImpl.h"
#include "cftrace.h"
#include "cfactionowner.h"

// CONSTANTS

// Reserved action ids not to be defined by context source plugins or clients.
// Note that the count constant must be updated if you change the number of
// reserved action ids.
static const TText* const KReservedActionIds[] =
    {
    _S( "publishContext" ), // publish context action used by scripts
    _S( "sourceCommand" ) // context source command action used by scripts
    };
static const TInt KReservedActionIdCount = 2;

#ifdef _DEBUG

_LIT( KPanicCat, "CCFActivatorEng" );

#endif

// MEMBER FUNCTIONS

EXPORT_C CCFActivator* CCFActivator::NewL()
    {
    FUNC_LOG;
    
    CCFActivator* self = new( ELeave ) CCFActivator;
    
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );
    
    return self;
    }

// Destructor
EXPORT_C CCFActivator::~CCFActivator()
    {
    FUNC_LOG;
    
    // Delete action plug-in manager
    delete iActionPluginManager;
    
    // Reset action cache
    iActionCache.ResetAndDestroy();
    }

CCFActivator::CCFActivator()
    {
    FUNC_LOG;
    }

void CCFActivator::ConstructL()
    {
    FUNC_LOG;
    
    // Construct action plugin manager
    iActionPluginManager = CCFActivatorEngineActionPluginManager::NewL(
        iActionCache );
    }

// METHODS

//------------------------------------------------------------------------------
// CCFActivator::ActionCacheElementExists
//------------------------------------------------------------------------------
//
CCFActionCacheElement* CCFActivator::ActionCacheElementExists(
    const TDesC& aActionIdentifier,
    RActionCacheArray& aActionCache )
    {
    FUNC_LOG;
    
    // Search action cache element for the action id
    CCFActionCacheElement* element = NULL;
    for( TInt i = 0; i < aActionCache.Count(); i++ )
        {
        element = aActionCache[i];
        if( element->CompareByIdentifier( aActionIdentifier ) )
            {
            // Action cache element found
            break;
            }
        element = NULL;
        }
    
    return element;
    }

// -----------------------------------------------------------------------------
// CCFActivator::FireActionL
// -----------------------------------------------------------------------------
//
void CCFActivator::FireActionL( CCFScriptEvent* aEvent )
    {
    FUNC_LOG;
    
    __ASSERT_DEBUG( aEvent, User::Panic( KPanicCat, 0 ) );

    // Create indication
    TPtrC id( aEvent->Identifier() );
    CCFActionCacheElement* element = CCFActivator::ActionCacheElementExists(
        id, iActionCache );
    if( element )
        {
        // Action exists
        INFO_1( "Action id [%S] found from action cache", &id );
        MCFActionOwner::TCFActionOwner ownerType = element->OwnerType();
        switch( ownerType )
            {
            case MCFActionOwner::ECFExternalAction:
                {
                TriggerExternalActionL( *aEvent, element->Subscriptions() );
                break;
                }
            case MCFActionOwner::ECFInternalAction:
            // Fall through
            default:
                {
                TriggerInternalActionL( *aEvent );
                break;
                }
            }
        }
    else
        {
        // Action not found
        INFO_1( "Action id [%S] not found from action cache", &id );
        }

    delete aEvent; // Ownership was transferred, cleanup now.
    }

// -----------------------------------------------------------------------------
// CCFActivator::FireActionL
// -----------------------------------------------------------------------------
//
void CCFActivator::FireActionL( const CCFContextSourceCommand& /*aCommand*/ )
    {
    FUNC_LOG;

    // Not handled by activator, nothing to do.
    }

// -----------------------------------------------------------------------------
// CCFActivator::GetActionSecurityPolicy
// -----------------------------------------------------------------------------
//
TInt CCFActivator::GetActionSecurityPolicy( const TDesC& aActionIdentifier,
    TSecurityPolicy& aPolicy )
	{
	FUNC_LOG;
    
    static _LIT_SECURITY_POLICY_FAIL( alwaysFail );
    TInt count = iActionCache.Count();
    for( TInt i = 0; i < count; i++ )
        {
        if( iActionCache[i]->CompareByIdentifier( aActionIdentifier ) )
            {
			TSecurityPolicy policy = iActionCache[i]->SecurityPolicy();
			aPolicy = policy;
            return KErrNone;
            }
        }
    aPolicy = alwaysFail;
    return KErrNotFound;
	}

// -----------------------------------------------------------------------------
// CCFActivator::DefineActionL
// -----------------------------------------------------------------------------
//
void CCFActivator::DefineActionL( const TDesC& aActionIdentifier,
    const TSecurityPolicy& aSecurityPolicy,
    const TUid& aOwnerUid,
    MCFActionOwner* aOwner )
    {
    FUNC_LOG;
    
    if ( IsReservedActionId( aActionIdentifier ) )
        {
        User::Leave( KErrAlreadyExists );
        }
    
    TInt err = KErrNone;
    CCFActionCacheElement* element = CCFActivator::ActionCacheElementExists(
        aActionIdentifier, iActionCache );
    if( element )
        {
        // Check if client is re-defining existing action
        if( element->OwnerUid() == aOwnerUid && !element->Owner() )
            {
            element->SetOwner( aOwner );
            INFO_2( "Action id [%S] from client [%x] re-defined",
                &aActionIdentifier, aOwnerUid );
            }
        else
            {
            // Action already exists and the owner is in place
            err = KErrAlreadyExists;
            }
        }
    else
        {
        // New action defined
        element = CCFActionCacheElement::NewLC( aActionIdentifier,
            aSecurityPolicy, aOwnerUid, aOwner );
        iActionCache.AppendL( element );
        CleanupStack::Pop( element );
        INFO_2( "Action id [%S] from client process [%x] defined",
            &aActionIdentifier, aOwnerUid );
        }
    
    ERROR_2( err, "Action id [%S] from client [%x] already defined",
        &aActionIdentifier, aOwnerUid );
    User::LeaveIfError( err );
    }

// -----------------------------------------------------------------------------
// CCFActivator::SubscribeActionL
// -----------------------------------------------------------------------------
//
void CCFActivator::SubscribeActionL(
    CCFActionSubscription* aActionSubscription,
    MCFActionSubscriptionListener* aListener,
    RThread& aClientThread )
    {
    FUNC_LOG;
    
    TPtrC actionIdentifier( aActionSubscription->ActionIdentifier() );

    // Check if we have action defined
    CCFActionCacheElement* element = ActionCacheElementExists(
        actionIdentifier, iActionCache );
    if( element )
        {
        // Check if listener already has subscribed, this will remove duplicate
        // subscription possibility
        if( !AlreadySubscribed( element, aListener ) )
            {
            // Action found, check security
            if( element->SecurityPolicy().CheckPolicy( aClientThread ) )
                {
                // Security check ok, add subscriber
                CCFActionSubscriptionImpl* actionSubscriptionImpl =
                    static_cast<CCFActionSubscriptionImpl*>( aActionSubscription );
                actionSubscriptionImpl->SetListener( aListener );
                element->AddSubscriptionL( actionSubscriptionImpl );

                INFO_1( "Action [%S] subscribed", &actionIdentifier );
                }
            else
                {
                // Security check failed, leave
                ERROR_1( KErrAccessDenied, "Security check error for action [%S]",
                    &actionIdentifier );
                User::Leave( KErrAccessDenied );
                }
            }
        else
            {
            // Already subscribed for the action
            ERROR_1( KErrAlreadyExists, "Already subscribed for action [%S]",
                &actionIdentifier );
            User::Leave( KErrAlreadyExists );
            }
        }
    else
        {
        // Action not found, leave
        ERROR_1( KErrNotFound, "Action [%S] not found",
            &actionIdentifier );
        User::Leave( KErrNotFound );
        }
    }

// -----------------------------------------------------------------------------
// CCFActivator::UnsubscribeAction
// -----------------------------------------------------------------------------
//
void CCFActivator::UnsubscribeAction(
    CCFActionSubscription& aActionSubscription,
    MCFActionSubscriptionListener& aListener )
    {
    FUNC_LOG;
    
    TPtrC actionIdentifier( aActionSubscription.ActionIdentifier() );

    // Check if we have action defined
    // Do nothing if the action is not found
    CCFActionCacheElement* element = ActionCacheElementExists(
        actionIdentifier, iActionCache );
    if( element )
        {
        CCFActionSubscriptionImpl& subscription =
            static_cast<CCFActionSubscriptionImpl&>( aActionSubscription );
        subscription.SetListener( &aListener );
        element->RemoveSubscription( subscription );
        }
    }

//------------------------------------------------------------------------------
// CCFActivator::DeregisterActions
//------------------------------------------------------------------------------
//
void CCFActivator::DeregisterActions( MCFActionOwner* aOwner )
    {
    FUNC_LOG;
    
    for( TInt i = 0; i < iActionCache.Count(); i++ )
        {
        CCFActionCacheElement* element = iActionCache[i];
        if( element->Owner() == aOwner )
            {
            element->SetOwner( NULL );
            INFO_2( "Deregistered action: [%S] from owner: [0x%x]",
                &element->ActionId(), element->OwnerUid().iUid );
            }
        }
    }

// -----------------------------------------------------------------------------
// CCFActivator::RemoveSubscriptions
// -----------------------------------------------------------------------------
//
void CCFActivator::RemoveSubscriptions(
    MCFActionSubscriptionListener& aListener )
    {
    FUNC_LOG;
    
    TInt count = iActionCache.Count();
    for( TInt i = 0; i < count; i++ )
        {
        RActionSubscriptionArray& subscriptions =
            const_cast<RActionSubscriptionArray&>(
                iActionCache[i]->Subscriptions() );
        TInt subscriptionCount = subscriptions.Count();
        CCFActionSubscriptionImpl* subscription = NULL;
        for( TInt j = 0; j < subscriptionCount; j++ )
            {
            subscription = subscriptions[j];
            if( &subscription->Listener() == &aListener )
                {
                // Subscription found for the listener, remove it
                delete subscription;
                subscriptions.Remove( j );
                break;
                }
            }
        }
    }

// -----------------------------------------------------------------------------
// void CCFActivator::InitializePhaseL
// -----------------------------------------------------------------------------
//
void CCFActivator::InitializePhaseL( CCFPhaseBase::TCFPhaseId aPhaseId )
    {
    FUNC_LOG;
    
    // Action plug-in manager will update cache if needed
    iActionPluginManager->InitializePhaseL( aPhaseId );
    }

//----------------------------------------------------------------------------
// CCFActivator::SetEventHandler
//----------------------------------------------------------------------------
//
void CCFActivator::SetEventHandler( MCFStarterEventHandler& aEventHandler )
    {
    FUNC_LOG;
	  iActionPluginManager->SetEventHandler( aEventHandler );
    }
    
// -----------------------------------------------------------------------------
// void CCFActivator::UpdatePlugInsL
// -----------------------------------------------------------------------------
//
void CCFActivator::UpdatePlugInsL()
	{
    FUNC_LOG;
    
    // Action plug-in manager will update cache if needed
    iActionPluginManager->UpdatePlugInsL();
	}

// -----------------------------------------------------------------------------
// CCFActivator::ActionIdentifier
// -----------------------------------------------------------------------------
//
TPtrC CCFActivator::ActionIdentifier( const CCFScriptEvent& aEvent ) const
    {
    FUNC_LOG;
    
    TPtrC actionIdentifer( aEvent.Identifier() );

    return actionIdentifer;
    }

// -----------------------------------------------------------------------------
// CCFActivator::CreateActionIndicationL
// -----------------------------------------------------------------------------
//
CCFActionIndication* CCFActivator::CreateActionIndicationL(
    const TDesC& aActionIdentifier,
    const CCFScriptEvent& aEvent ) const
    {
    FUNC_LOG;
    
    CCFActionIndicationImpl* indication = CCFActionIndicationImpl::NewLC();
    indication->SetIdentifierL( aActionIdentifier );
    const RKeyValueArray& eventParameters = aEvent.Parameters();
    for ( TInt i = 0; i < eventParameters.Count(); ++i )
        {
        indication->AddParameterL( eventParameters[ i ]->Key(),
            eventParameters[ i ]->Value() );
        }

    // Clean up
    CleanupStack::Pop( indication );

    return indication;
    }

// -----------------------------------------------------------------------------
// CCFActivator::AlreadySubscribed
// -----------------------------------------------------------------------------
//
TBool CCFActivator::AlreadySubscribed( CCFActionCacheElement* aElement,
    MCFActionSubscriptionListener* aListener ) const
    {
    FUNC_LOG;
    
    TBool retVal = EFalse;
    
    const RActionSubscriptionArray& subscriptions = aElement->Subscriptions();
    TInt count = subscriptions.Count();
    for( TInt i = 0; i < count; i++ )
        {
        if( &subscriptions[i]->Listener() == aListener )
            {
            retVal = ETrue;
            break;
            }
        }

    return retVal;    
    }

//------------------------------------------------------------------------------
// CCFActivator::TriggerInternalActionL
//------------------------------------------------------------------------------
//
void CCFActivator::TriggerInternalActionL( const CCFScriptEvent& aEvent )
    {
    FUNC_LOG;

    TPtrC id( aEvent.Identifier() );
    CCFActionIndication* indication = CreateActionIndicationL(
        aEvent.Identifier(), aEvent );
    CleanupStack::PushL( indication );
    
    TBool executed = iActionPluginManager->TriggerL( indication );
    INFO_2( "Action id [%S] executed internally with code [%d]",
        &id, executed );
    if( !executed )
        {
        // For some reason there is an action defined in action cache
        // but the action executor cannot be found
        ERROR_GEN_1( "Action id [%S] defined as internal action\
            but could not find action plug-in",
            &id );
        CleanupStack::PopAndDestroy( indication );                    
        }
    else
        {
        CleanupStack::Pop( indication );
        }
    }

//------------------------------------------------------------------------------
// CCFActivator::TriggerExternalActionL
//------------------------------------------------------------------------------
//
void CCFActivator::TriggerExternalActionL( const CCFScriptEvent& aEvent,
    const RActionSubscriptionArray& aSubscriptions )
    {
    FUNC_LOG;

    TPtrC id( aEvent.Identifier() );
    if( aSubscriptions.Count() )
        {
        for( TInt i = 0; i < aSubscriptions.Count(); i++ )
            {
            CCFActionSubscriptionImpl* subscription = aSubscriptions[i];
            CCFActionIndication* indication = CreateActionIndicationL(
                id, aEvent );
            subscription->Listener().ActionIndicationL( indication );
            INFO_1( "Action id [%S] notification sent to external client", &id );
            }
        }
    else
        {
        INFO_1( "No subscribers exists for action id [%S]", &id );
        }
    }

//------------------------------------------------------------------------------
// CCFActivator::IsReservedActionId
//------------------------------------------------------------------------------
//
TBool CCFActivator::IsReservedActionId( const TDesC& aActionId )
    {
    FUNC_LOG;

    for ( TInt i = 0; i < KReservedActionIdCount; ++i )
        {
        TPtrC reservedId( KReservedActionIds[ i ] );
        if ( reservedId.CompareF( aActionId ) == 0 )
            {
            return ETrue;
            }
        }

    return EFalse;
    }

// End of file

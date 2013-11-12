/*
* Copyright (c) 2006-2010 Nokia Corporation and/or its subsidiary(-ies). 
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


//  CLASS HEADER
#include "UT_CFActivatorEngine.h"

//  EXTERNAL INCLUDES
#include <EUnitMacros.h>
#include <e32property.h>


//  INTERNAL INCLUDES
#include "CFActionCacheElement.h"
#include "CFActionPlugin.h"
#include "CFActionPlugInImpl.h"
#include "CFActivator.h"
#include "CFActivatorEngineActionPluginManager.h"
#include "CFActivatorTimedSchedulerWait.h"
#include "TestActionPlugIn.h"
#include "cfactionindicationimpl.h"
#include "cfactionsubscriptionimpl.h"
#include "cfscriptevent.h"
#include "TestActionPluginConst.hrh"
#include "cfactionpluginthread.h"

// CONSTANTS

const TInt KOneSecond = 1000000;
const TInt KThreeSeconds = 3 * KOneSecond;
const TInt KFiveSeconds = 5 * KOneSecond;
const TInt KActionExecutionDelay = KFiveSeconds; // 5 second
const TInt KTeardownDelay = KOneSecond; // 1 second
const TInt KPluginLoadWaitDelay = KThreeSeconds; // 3 second

_LIT( KFalseIdentifier, "false" );
_LIT( KTestActionIdTestAc3, "AC3" );
_LIT( KTestActionIdLeave, "AC_Leave" );
// Security policy
_LIT_SECURITY_POLICY_PASS( KSecurity );

// LOCAL FUNCTIONS

/**
* Clenup operation for RImplInfoPtrArray
*/
LOCAL_C void CleanUpImplInfoArray( TAny* aParams )
    {
    RImplInfoPtrArray* array = static_cast<RImplInfoPtrArray*>( aParams );
    array->ResetAndDestroy();
    }

/**
* Push operation for RImplInfoPtrArray
*/
LOCAL_C void CleanupPushImplInfoArrayL( RImplInfoPtrArray& aArray )
    {
    TCleanupItem item( CleanUpImplInfoArray, &aArray );
    CleanupStack::PushL( item );
    }

// CONSTRUCTION
UT_CFActivatorEngine* UT_CFActivatorEngine::NewL()
    {
    UT_CFActivatorEngine* self = UT_CFActivatorEngine::NewLC();
    CleanupStack::Pop();

    return self;
    }

UT_CFActivatorEngine* UT_CFActivatorEngine::NewLC()
    {
    UT_CFActivatorEngine* self = new( ELeave ) UT_CFActivatorEngine();
    CleanupStack::PushL( self );

    self->ConstructL();

    return self;
    }

// Destructor (virtual by CBase)
UT_CFActivatorEngine::~UT_CFActivatorEngine()
    {
    Teardown();
    }

// Default constructor
UT_CFActivatorEngine::UT_CFActivatorEngine()
    {
    }

// Second phase construct
void UT_CFActivatorEngine::ConstructL()
    {
    // The ConstructL from the base class CEUnitTestSuiteClass must be called.
    // It generates the test case table.
    CEUnitTestSuiteClass::ConstructL();
    }

// METHODS

void UT_CFActivatorEngine::ActionIndicationL( CCFActionIndication* aIndication )
    {
    delete aIndication;
    }

MCFActionOwner::TCFActionOwner UT_CFActivatorEngine::ActionOwner() const
    {
    return MCFActionOwner::ECFExternalAction;
    }

TInt UT_CFActivatorEngine::CallBack( TAny* aSelf )
    {
    UT_CFActivatorEngine* self = static_cast<UT_CFActivatorEngine*>( aSelf );
    self->iPeriodic->Cancel();
    if( self->iWait->IsStarted() )
        {
        self->iWait->AsyncStop();
        }
    return KErrNone;
    }

void UT_CFActivatorEngine::Wait( TTimeIntervalMicroSeconds32& aDelay )
    {
    if( iPeriodic )
        {
        if( !iPeriodic->IsActive() )
            {
            TCallBack cb( CallBack, this );
            iPeriodic->Start( aDelay, aDelay, cb );
            iWait->Start();
            }
        }

    if( iTimedWait )
        {
        iTimedWait->Stop();
        }
    }

void UT_CFActivatorEngine::SetupEmptyL(  )
    {
    }

void UT_CFActivatorEngine::SetupL(  )
    {
    TInt err = RProperty::Define( KActionPluginTestCategory,
        KLeaveInitializationFunction,
        RProperty::EInt,
        KSecurity,
        KSecurity );
    EUNIT_PRINT( _L("Define P&S key KActionPluginTestCategory, KLeaveInitializationFunction: err = %d" ), err );
    
    iActionPluginManager = CCFActivatorEngineActionPluginManager::NewL(
        iActionCache );
    iPeriodic = CPeriodic::NewL( CActive::EPriorityStandard );
    iWait = new( ELeave ) CActiveSchedulerWait;
    iOwnerUid = RThread().SecureId();
    }

void UT_CFActivatorEngine::Teardown(  )
    {
    iActionCache.ResetAndDestroy();

    delete iActionPluginManager;
    iActionPluginManager = NULL;

    delete iTimedWait;
    iTimedWait = NULL;

    delete iPeriodic;
    iPeriodic = NULL;

    if( iWait )
        {
        if( iWait->IsStarted() )
            {
            iWait->AsyncStop();
            }
        delete iWait;
        iWait = NULL;
        }
    // If key exist, it has to be assigned to value false.
    // Doesn't matter if call fails because key doesn't exist.
    RProperty::Set( KActionPluginTestCategory, KLeaveInitializationFunction, EFalse );
    }

void UT_CFActivatorEngine::TeardownDelayed(  )
    {
    iActionCache.ResetAndDestroy();

    delete iActionPluginManager;
    iActionPluginManager = NULL;

    delete iTimedWait;
    iTimedWait = NULL;

    // Wait here until the thread has fully been closed
    TTimeIntervalMicroSeconds32 interval( KTeardownDelay * 2 );
    Wait( interval );

    delete iPeriodic;
    iPeriodic = NULL;

    if( iWait )
        {
        if( iWait->IsStarted() )
            {
            iWait->AsyncStop();
            }
        delete iWait;
        iWait = NULL;
        }
    // If key exist, it has to be assigned to value false.
    // Doesn't matter if call fails because key doesn't exist.
    RProperty::Set( KActionPluginTestCategory, KLeaveInitializationFunction, EFalse );
    }

//-----------------------------------------------------------------------------
// UT_CFActivatorEngine::UT_CCFActionCacheElement_NewLL
//-----------------------------------------------------------------------------
//
void UT_CFActivatorEngine::UT_CCFActionCacheElement_NewLL(  )
    {
    _LIT( KActionId, "ActionId" );
    _LIT_SECURITY_POLICY_PASS( KSecurity );

    CCFActionCacheElement* instance =
        CCFActionCacheElement::NewL( KActionId, KSecurity, iOwnerUid, this );
    CleanupStack::PushL( instance );

    EUNIT_ASSERT_DESC( instance, "CCFActionCacheElement instance not created!" );

    CleanupStack::PopAndDestroy( instance );
    }

//-----------------------------------------------------------------------------
// UT_CFActivatorEngine::UT_CCFActionCacheElement_NewLCL
//-----------------------------------------------------------------------------
//
void UT_CFActivatorEngine::UT_CCFActionCacheElement_NewLCL(  )
    {
    _LIT( KActionId, "ActionId" );
    _LIT_SECURITY_POLICY_PASS( KSecurity );

    CCFActionCacheElement* instance =
        CCFActionCacheElement::NewLC( KActionId, KSecurity, iOwnerUid, this );

    EUNIT_ASSERT_DESC( instance, "CCFActionCacheElement instance not created!" );

    CleanupStack::PopAndDestroy( instance );
    }

//-----------------------------------------------------------------------------
// UT_CFActivatorEngine::UT_CCFActionCacheElement_CompareByIdentifierL
//-----------------------------------------------------------------------------
//
void UT_CFActivatorEngine::UT_CCFActionCacheElement_CompareByIdentifierL(  )
    {
    _LIT( KCorrectId, "ActionId" );
    _LIT( KFalseId1, "False1" );
    _LIT( KFalseId2, "False2" );
    _LIT_SECURITY_POLICY_PASS( KSecurity );
    TBool pass = EFalse;
    CCFActionCacheElement* actionCache =
        CCFActionCacheElement::NewLC( KCorrectId, KSecurity, iOwnerUid, this );

    // These should not pass
    pass = actionCache->CompareByIdentifier( KFalseId1 );
    EUNIT_ASSERT_DESC( !pass, "Invalid return value!" );
    pass = actionCache->CompareByIdentifier( KFalseId2 );
    EUNIT_ASSERT_DESC( !pass, "Invalid return value!" );

    // This should pass
    pass = actionCache->CompareByIdentifier( KCorrectId );
    EUNIT_ASSERT_DESC( pass, "Invalid return value!" );

    // Cleanup
    CleanupStack::PopAndDestroy( actionCache );
    }

//-----------------------------------------------------------------------------
// UT_CFActivatorEngine::UT_CCFActionCacheElement_AddSubscriptionLL
//-----------------------------------------------------------------------------
//
void UT_CFActivatorEngine::UT_CCFActionCacheElement_AddSubscriptionLL(  )
    {
    TInt count = 0;
    _LIT( KCorrectId, "ActionId" );
    _LIT_SECURITY_POLICY_PASS( KSecurity );
    CCFActionCacheElement* actionCache =
        CCFActionCacheElement::NewLC( KCorrectId, KSecurity, iOwnerUid, this );

    // Test empty
    count = actionCache->Subscriptions().Count();
    EUNIT_ASSERT_DESC( !count, "Invalid subscription count!" );

    // Add subscription
    CCFActionSubscriptionImpl* subscription =
        static_cast<CCFActionSubscriptionImpl*>( CCFActionSubscription::NewLC() );
    subscription->SetActionIdentifierL( KCorrectId );
    subscription->SetListener( this );
    actionCache->AddSubscriptionL( subscription );
    CleanupStack::Pop( subscription );
    count = actionCache->Subscriptions().Count();
    EUNIT_ASSERT_DESC( count == 1, "Invalid subscription count!" );
    EUNIT_ASSERT_DESC( subscription == actionCache->Subscriptions()[0], "Invalid subscriber!" )

    // Cleanup
    CleanupStack::PopAndDestroy( actionCache );
    }

//-----------------------------------------------------------------------------
// UT_CFActivatorEngine::UT_CCFActionCacheElement_SecurityPolicyL
//-----------------------------------------------------------------------------
//
void UT_CFActivatorEngine::UT_CCFActionCacheElement_SecurityPolicyL(  )
    {
    _LIT( KCorrectId, "ActionId" );
    _LIT_SECURITY_POLICY_FAIL( KSecurity );
    CCFActionCacheElement* actionCache =
        CCFActionCacheElement::NewLC( KCorrectId, KSecurity, iOwnerUid, this );

    // Test security against EUnit thread, should fail
    RThread thread;
    TBool check = actionCache->SecurityPolicy().CheckPolicy( thread );
    EUNIT_ASSERT_DESC( !check, "Invalid security policy!" );
    thread.Close();

    // Cleanup
    CleanupStack::PopAndDestroy( actionCache );
    }

//-----------------------------------------------------------------------------
// UT_CFActivatorEngine::UT_CCFActionCacheElement_RemoveSubscriptionL
//-----------------------------------------------------------------------------
//
void UT_CFActivatorEngine::UT_CCFActionCacheElement_RemoveSubscriptionL(  )
    {
    TInt count = 0;
    _LIT( KCorrectId, "ActionId" );
    _LIT_SECURITY_POLICY_PASS( KSecurity );
    CCFActionCacheElement* actionCache =
        CCFActionCacheElement::NewLC( KCorrectId, KSecurity, iOwnerUid, this );
    CCFActionSubscriptionImpl* subscription =
        static_cast<CCFActionSubscriptionImpl*>( CCFActionSubscription::NewLC() );
    subscription->SetActionIdentifierL( KCorrectId );
    subscription->SetListener( this );
    actionCache->AddSubscriptionL( subscription );
    CleanupStack::Pop( subscription );

    // Test
    subscription = static_cast<CCFActionSubscriptionImpl*>( CCFActionSubscription::NewLC() );
    subscription->SetActionIdentifierL( KCorrectId );
    subscription->SetListener( this );
    count = actionCache->Subscriptions().Count();
    actionCache->RemoveSubscription( *subscription );
    EUNIT_ASSERT_DESC( count == actionCache->Subscriptions().Count() + 1,
        "Invalid subscription count!" );

    // Cleanup
    CleanupStack::PopAndDestroy( subscription );
    CleanupStack::PopAndDestroy( actionCache );
    }

//-----------------------------------------------------------------------------
// UT_CFActivatorEngine::UT_CCFActionPlugInImpl_NewLL
//-----------------------------------------------------------------------------
//
void UT_CFActivatorEngine::UT_CCFActionPlugInImpl_NewLL(  )
    {
    CCFActionPlugInImpl* obj = CCFActionPlugInImpl::NewL( NULL, NULL );
    CleanupStack::PushL( obj );

    EUNIT_ASSERT_DESC( obj, "CCFActionPlugInImpl instance not created!" );

    CleanupStack::PopAndDestroy( obj );
    }

//-----------------------------------------------------------------------------
// UT_CFActivatorEngine::UT_CCFActionPlugInImpl_NewLCL
//-----------------------------------------------------------------------------
//
void UT_CFActivatorEngine::UT_CCFActionPlugInImpl_NewLCL(  )
    {
    CCFActionPlugInImpl* obj = CCFActionPlugInImpl::NewLC( NULL, NULL );

    EUNIT_ASSERT_DESC( obj, "CCFActionPlugInImpl instance not created!" );

    CleanupStack::PopAndDestroy( obj );
    }

//-----------------------------------------------------------------------------
// UT_CFActivatorEngine::UT_CCFActivator_NewLL
//-----------------------------------------------------------------------------
//
void UT_CFActivatorEngine::UT_CCFActivator_NewLL(  )
    {
    CCFActivator* instance = CCFActivator::NewL();
    CleanupStack::PushL( instance );

    EUNIT_ASSERT_DESC( instance, "CCFActivator instance not created!" );

    CleanupStack::PopAndDestroy( instance );
    }

//-----------------------------------------------------------------------------
// UT_CFActivatorEngine::UT_CCFActivatorEngineActionPluginManager_NewLL
//-----------------------------------------------------------------------------
//
void UT_CFActivatorEngine::UT_CCFActivatorEngineActionPluginManager_NewLL(  )
    {
    CCFActivatorEngineActionPluginManager* obj =
        CCFActivatorEngineActionPluginManager::NewL( iActionCache );
    CleanupStack::PushL( obj );

    EUNIT_ASSERT_DESC( obj, "CCFActivatorEngineActionPluginManager instance not created!" );

    CleanupStack::PopAndDestroy( obj );
    }

//-----------------------------------------------------------------------------
// UT_CFActivatorEngine::UT_CCFActivatorEngineActionPluginManager_NewLCL
//-----------------------------------------------------------------------------
//
void UT_CFActivatorEngine::UT_CCFActivatorEngineActionPluginManager_NewLCL(  )
    {
    CCFActivatorEngineActionPluginManager* obj =
        CCFActivatorEngineActionPluginManager::NewLC( iActionCache );

    EUNIT_ASSERT_DESC( obj, "CCFActivatorEngineActionPluginManager instance not created!" );

    CleanupStack::PopAndDestroy( obj );
    }


//-----------------------------------------------------------------------------
// UT_CFActivatorEngine::PluginThreadCreationFailureL
//-----------------------------------------------------------------------------
//
void UT_CFActivatorEngine::PluginThreadCreationFailureL()
    {
    RImplInfoPtrArray implInfoArray;
    CleanupPushImplInfoArrayL( implInfoArray );
    REComSession::ListImplementationsL( KActionPluginUid, implInfoArray );

    CImplementationInformation* impInfo( NULL );
    for( TInt ii = 0; ii < implInfoArray.Count(); ii++ )
        {
        impInfo = implInfoArray[ii];
        if( impInfo->ImplementationUid() == TUid::Uid( KTestActionPluginImplementationUid ) )
            {
            // Implementation info found
            break;
            }
        impInfo = NULL;
        }
    EUNIT_ASSERT_DESC( impInfo, "KTestActionPluginImplementationUid not found!" );

    TInt err = RProperty::Set( KActionPluginTestCategory, KLeaveInitializationFunction, ETrue );
    EUNIT_ASSERT_DESC( KErrNone == err, "Setting KActionPluginTestCategory, KLeaveInitializationFunction, ETrue failed!" );

    CCFActivatorEngineActionPluginManager* obj =
        CCFActivatorEngineActionPluginManager::NewLC( iActionCache );
    EUNIT_ASSERT_DESC( obj, "Creating CCFActivatorEngineActionPluginManager object failed!" );

    CCFActionPlugInThread* plugInThread = NULL;
    TRAP( err, plugInThread = CCFActionPlugInThread::NewL( *obj, *impInfo ) );
    EUNIT_ASSERT( err == KErrAbort );
    EUNIT_ASSERT( !plugInThread );
    
    CleanupStack::PopAndDestroy( obj );
    CleanupStack::PopAndDestroy( &implInfoArray );
    }

//-----------------------------------------------------------------------------
// UT_CFActivatorEngine::UT_CCFActivatorEngineActionPluginManager_TriggerLL
//-----------------------------------------------------------------------------
//
void UT_CFActivatorEngine::UT_CCFActivatorEngineActionPluginManager_TriggerLL(  )
    {
    // Initialize
    iActionPluginManager->InitializePhaseL( CCFPhaseBase::ECFDeviceStarting );
    // Wait here to complete plug-in loading
    TTimeIntervalMicroSeconds32 interval( KPluginLoadWaitDelay );
    Wait( interval );

    iActionPluginManager->InitializePhaseL( CCFPhaseBase::ECFDeviceStarted );
    Wait( interval );
    iActionPluginManager->InitializePhaseL( CCFPhaseBase::ECFLoadingPlugins );
    Wait( interval );

    // Trigger some actions
    CCFActionIndicationImpl* action = NULL;
    CCFActionIndicationImpl* action2 = NULL;
    TBool executed = EFalse;
    TBool executed2 = EFalse;

    // False actions
    action = static_cast<CCFActionIndicationImpl*>( CCFActionIndication::NewLC() );
    action->SetIdentifierL( KFalseIdentifier );
    executed = iActionPluginManager->TriggerL( action );
    EUNIT_ASSERT_DESC( !executed, "Incorrect action executed!" );
    CleanupStack::PopAndDestroy( action );

    // True actions
    action = static_cast<CCFActionIndicationImpl*>( CCFActionIndication::NewLC() );
    action->SetIdentifierL( KTestActionIdTestAc3 );
    action2 = static_cast<CCFActionIndicationImpl*>( CCFActionIndication::NewLC() );
    action2->SetIdentifierL( KTestActionIdTestAc3 );
    executed = iActionPluginManager->TriggerL( action );
    executed2 = iActionPluginManager->TriggerL( action2 );
    EUNIT_ASSERT_DESC( executed, "Action not executed!" );
    EUNIT_ASSERT_DESC( executed2, "Action not executed!" );
    CleanupStack::Pop( action2 );
    CleanupStack::Pop( action );

    // Wait here to complete actions
    interval = KActionExecutionDelay;
    Wait( interval );

    // Leave action and try correct ones after the leave
    executed = EFalse;
    executed2 = EFalse;

    action = static_cast<CCFActionIndicationImpl*>( CCFActionIndication::NewLC() );
    action->SetIdentifierL( KTestActionIdLeave );
    executed = iActionPluginManager->TriggerL( action );
    EUNIT_ASSERT_DESC( executed, "Incorrect action executed!" );
    CleanupStack::Pop( action );

    action = static_cast<CCFActionIndicationImpl*>( CCFActionIndication::NewLC() );
    action->SetIdentifierL( KTestActionIdTestAc3 );
    action2 = static_cast<CCFActionIndicationImpl*>( CCFActionIndication::NewLC() );
    action2->SetIdentifierL( KTestActionIdTestAc3 );
    executed = iActionPluginManager->TriggerL( action );
    executed2 = iActionPluginManager->TriggerL( action2 );
    EUNIT_ASSERT_DESC( executed, "Action not executed!" );
    EUNIT_ASSERT_DESC( executed2, "Action not executed!" );
    CleanupStack::Pop( action2 );
    CleanupStack::Pop( action );

    // Wait here to complete actions
    interval = KActionExecutionDelay;
    Wait( interval );
    }

//-----------------------------------------------------------------------------
// UT_CFActivatorEngine::UT_CCFActivatorEngineActionPluginManager_InitializePhaseLL
//-----------------------------------------------------------------------------
//
void UT_CFActivatorEngine::UT_CCFActivatorEngineActionPluginManager_InitializePhaseLL(  )
    {
    TTimeIntervalMicroSeconds32 interval( KPluginLoadWaitDelay );

    iActionPluginManager->InitializePhaseL( CCFPhaseBase::ECFDeviceStarting );
    Wait( interval );

    iActionPluginManager->InitializePhaseL( CCFPhaseBase::ECFDeviceStarted );
    Wait( interval );

    iActionPluginManager->InitializePhaseL( CCFPhaseBase::ECFLoadingPlugins );
    Wait( interval );

    delete iActionPluginManager;
    iActionPluginManager = NULL;
    iActionPluginManager = CCFActivatorEngineActionPluginManager::NewL( iActionCache );

    RProperty::Set( KActionPluginTestCategory, KLeaveInitializationFunction, ETrue );

    // One action plugin leaves
    iActionPluginManager->InitializePhaseL( CCFPhaseBase::ECFDeviceStarting );
    Wait( interval );

    iActionPluginManager->InitializePhaseL( CCFPhaseBase::ECFDeviceStarted );
    Wait( interval );

    iActionPluginManager->InitializePhaseL( CCFPhaseBase::ECFLoadingPlugins );
    Wait( interval );
    }

//-----------------------------------------------------------------------------
// UT_CFActivatorEngine::UT_CCFActivatorTimedSchedulerWait_NewLL
//-----------------------------------------------------------------------------
//
void UT_CFActivatorEngine::UT_CCFActivatorTimedSchedulerWait_NewLL(  )
    {
    CCFActivatorTimedSchedulerWait* instance = CCFActivatorTimedSchedulerWait::NewL();
    CleanupStack::PushL( instance );

    EUNIT_ASSERT_DESC( instance, "CCFActivatorTimedSchedulerWait instance not created!" );

    CleanupStack::PopAndDestroy( instance );
    }

//-----------------------------------------------------------------------------
// UT_CFActivatorEngine::UT_CCFActivatorTimedSchedulerWait_NewLCL
//-----------------------------------------------------------------------------
//
void UT_CFActivatorEngine::UT_CCFActivatorTimedSchedulerWait_NewLCL(  )
    {
    CCFActivatorTimedSchedulerWait* instance = CCFActivatorTimedSchedulerWait::NewLC();

    EUNIT_ASSERT_DESC( instance, "CCFActivatorTimedSchedulerWait instance not created!" );

    CleanupStack::PopAndDestroy( instance );
    }

//-----------------------------------------------------------------------------
// UT_CFActivatorEngine::UT_CCFActivatorTimedSchedulerWait_StartL
//-----------------------------------------------------------------------------
//
void UT_CFActivatorEngine::UT_CCFActivatorTimedSchedulerWait_StartL(  )
    {
    iTimedWait = CCFActivatorTimedSchedulerWait::NewL();

    // Start wait. It should timeout and continue after 1sec
    iTimedWait->Start( 1000000 );
    }

//-----------------------------------------------------------------------------
// UT_CFActivatorEngine::
//-----------------------------------------------------------------------------
//
void UT_CFActivatorEngine::UT_CCFActivatorTimedSchedulerWait_StopL(  )
    {
    iTimedWait = CCFActivatorTimedSchedulerWait::NewL();

    // Start wait for 5sec
    TTimeIntervalMicroSeconds32 interval( KFiveSeconds );
    iTimedWait->Start( interval );
    interval = KOneSecond;
    Wait( interval );
    }

//  TEST TABLE

EUNIT_BEGIN_TEST_TABLE(
    UT_CFActivatorEngine,
    "CFActivatorEngine unit test",
    "UNIT" )

EUNIT_TEST(
    "NewL - test ",
    "CCFActionCacheElement",
    "NewL",
    "FUNCTIONALITY",
    SetupEmptyL, UT_CCFActionCacheElement_NewLL, Teardown)

EUNIT_TEST(
    "NewLC - test ",
    "CCFActionCacheElement",
    "NewLC",
    "FUNCTIONALITY",
    SetupEmptyL, UT_CCFActionCacheElement_NewLCL, Teardown)

EUNIT_TEST(
    "CompareByIdentifier - test ",
    "CCFActionCacheElement",
    "CompareByIdentifier",
    "FUNCTIONALITY",
    SetupL, UT_CCFActionCacheElement_CompareByIdentifierL, Teardown)

EUNIT_TEST(
    "AddSubscriptionL - test ",
    "CCFActionCacheElement",
    "AddSubscriptionL",
    "FUNCTIONALITY",
    SetupL, UT_CCFActionCacheElement_AddSubscriptionLL, Teardown)

EUNIT_TEST(
    "SecurityPolicy - test ",
    "CCFActionCacheElement",
    "SecurityPolicy",
    "FUNCTIONALITY",
    SetupL, UT_CCFActionCacheElement_SecurityPolicyL, Teardown)

EUNIT_TEST(
    "RemoveSubscription - test ",
    "CCFActionCacheElement",
    "RemoveSubscription",
    "FUNCTIONALITY",
    SetupL, UT_CCFActionCacheElement_RemoveSubscriptionL, Teardown)

EUNIT_TEST(
    "NewL - test ",
    "CCFActionPlugInImpl",
    "NewL",
    "FUNCTIONALITY",
    SetupEmptyL, UT_CCFActionPlugInImpl_NewLL, Teardown)

EUNIT_TEST(
    "NewLC - test ",
    "CCFActionPlugInImpl",
    "NewLC",
    "FUNCTIONALITY",
    SetupEmptyL, UT_CCFActionPlugInImpl_NewLCL, Teardown)

EUNIT_TEST(
    "NewL - test ",
    "CCFActivator",
    "NewL",
    "FUNCTIONALITY",
    SetupL, UT_CCFActivator_NewLL, TeardownDelayed)

EUNIT_TEST(
    "NewL - test ",
    "CCFActivatorEngineActionPluginManager",
    "NewL",
    "FUNCTIONALITY",
    SetupEmptyL, UT_CCFActivatorEngineActionPluginManager_NewLL, Teardown)

EUNIT_TEST(
    "NewLC - test ",
    "CCFActivatorEngineActionPluginManager",
    "NewLC",
    "FUNCTIONALITY",
    SetupEmptyL, UT_CCFActivatorEngineActionPluginManager_NewLCL, Teardown)

EUNIT_TEST(
    "TriggerL - test ",
    "CCFActivatorEngineActionPluginManager",
    "TriggerL",
    "FUNCTIONALITY",
    SetupL, UT_CCFActivatorEngineActionPluginManager_TriggerLL, TeardownDelayed)

EUNIT_TEST(
    "InitializePhaseL - test ",
    "CCFActivatorEngineActionPluginManager",
    "InitializePhaseL",
    "FUNCTIONALITY",
    SetupL, UT_CCFActivatorEngineActionPluginManager_InitializePhaseLL, TeardownDelayed)

EUNIT_TEST(
    "NewL - test ",
    "CCFActivatorTimedSchedulerWait",
    "NewL",
    "FUNCTIONALITY",
    SetupEmptyL, UT_CCFActivatorTimedSchedulerWait_NewLL, Teardown)

EUNIT_TEST(
    "NewLC - test ",
    "CCFActivatorTimedSchedulerWait",
    "NewLC",
    "FUNCTIONALITY",
    SetupEmptyL, UT_CCFActivatorTimedSchedulerWait_NewLCL, Teardown)

EUNIT_TEST(
    "Start - test ",
    "CCFActivatorTimedSchedulerWait",
    "Start",
    "FUNCTIONALITY",
    SetupL, UT_CCFActivatorTimedSchedulerWait_StartL, Teardown)

EUNIT_TEST(
    "Stop - test ",
    "CCFActivatorTimedSchedulerWait",
    "Stop",
    "FUNCTIONALITY",
    SetupL, UT_CCFActivatorTimedSchedulerWait_StopL, Teardown)

EUNIT_TEST(
    "CCFActionPlugInThread::NewL - test ",
    "CCFActionPlugInThread::NewL",
    "NewL",
    "FUNCTIONALITY",
    SetupL, PluginThreadCreationFailureL, TeardownDelayed)
    
EUNIT_END_TEST_TABLE

//  END OF FILE

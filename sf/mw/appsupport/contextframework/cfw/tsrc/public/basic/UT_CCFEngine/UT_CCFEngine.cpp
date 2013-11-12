/*
* Copyright (c) 2007-2010 Nokia Corporation and/or its subsidiary(-ies). 
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
#include "UT_CCFEngine.h"
#include "CFTestDelay.h"

//  EXTERNAL INCLUDES
#include <EUnitMacros.h>
#include <ECom/ECom.h>

//  INTERNAL INCLUDES
#include "CFEngine.h"
#include "cftestcontextlistener.h"
#include "CFContextInterface.h"
#include "CFContextSubscription.h"
#include "CFContextIndication.h"

// CONSTANTS
const TInt KSecond = 1000000;

// CONSTRUCTION
UT_CCFEngine* UT_CCFEngine::NewL()
    {
    UT_CCFEngine* self = UT_CCFEngine::NewLC();
    CleanupStack::Pop();

    return self;
    }

UT_CCFEngine* UT_CCFEngine::NewLC()
    {
    UT_CCFEngine* self = new( ELeave ) UT_CCFEngine();
    CleanupStack::PushL( self );

    self->ConstructL();

    return self;
    }

// Destructor (virtual by CBase)
UT_CCFEngine::~UT_CCFEngine()
    {
    }

// Default constructor
UT_CCFEngine::UT_CCFEngine()
    {
    }

// Second phase construct
void UT_CCFEngine::ConstructL()
    {
    // The ConstructL from the base class CEUnitTestSuiteClass must be called.
    // It generates the test case table.
    CEUnitTestSuiteClass::ConstructL();
    }
    
    
// INTERFACE IMPLEMENTATIONS

// from MCFContextSubscriptionListener
void UT_CCFEngine::ContextIndicationL( CCFContextIndication* aIndication )
	{
	iIndication = ETrue;
	delete aIndication;	

    if ( iRemoveSubscription )
        {
        iCFContextInterface->UnsubscribeContext( *iRemoveSubscription, *this);
        iRemoveSubscription = NULL;
        }
    if ( iRemoveSubscriptions )
        {
        iCFContextInterface->UnsubscribeContexts( *this );
        }
    }

// from MCFContextSubscriptionListener
TInt UT_CCFEngine::Client( RThread& aThread ) const
	{
	aThread = RThread();
	return KErrNone;
	}
        
// from MCFContextSubscriptionListener
void UT_CCFEngine::HandleContextSubscriptionError( TInt /*aError*/,
            const TDesC& /*aSource*/,
            const TDesC& /*aType*/ )
    {
    iSubscriptionError = ETrue;
    StopWait();
    }
    

// from MCFSecurityChecker
TBool UT_CCFEngine::CheckClientSecurity( const RThread& aClientThread,
	const TSecurityPolicy& aSecurityPolicy )
	{
    // Security check only needed if request is from other process than CF
    TBool securityPassed = ETrue;
    
    if ( iDoSecurityCheck > 0 )
    	{
    	iDoSecurityCheck--;	
    	securityPassed = aSecurityPolicy.CheckPolicy( aClientThread );
    	}
    return securityPassed;
	}


//  METHODS



void UT_CCFEngine::SetupForNewL(  )
    {
	iWait = new( ELeave ) CActiveSchedulerWait;
	iTestDelay = CCFTestDelay::NewL();
    }

void UT_CCFEngine::StopWait()
	{
	if( iWait )
        {
        if( iWait->IsStarted() )
            {
            iWait->AsyncStop();
            }
        delete iWait;
        iWait = NULL;
        }	
	}

void UT_CCFEngine::TeardownForNewL(  )
    {
    iTestDelay->Start( KSecond );
    
    delete iTestDelay;
    iTestDelay = NULL;
	if( iWait )
        {
        if( iWait->IsStarted() )
            {
            iWait->AsyncStop();
            }
        delete iWait;
        iWait = NULL;
        }

#ifdef __WINS__
    REComSession::FinalClose();
#endif
    }
    
void UT_CCFEngine::SetupL(  )
    {
	SetupForNewL();
	iCFEngine = CCFEngine::NewL( *this );
	iCFContextInterface = (MCFContextInterface*) iCFEngine;
	
	iTestDelay->Start( KSecond * 2 );
    }

void UT_CCFEngine::Teardown(  )
    {
	delete iCFEngine;
    iCFEngine = NULL;
	iCFContextInterface = NULL;
	iIndication = EFalse;
	iSubscriptionError = EFalse;
	TeardownForNewL();
    }

void UT_CCFEngine::UT_CCFEngine_NewLL(  )
    {
    CCFEngine* engine = CCFEngine::NewL( *this );
    
    CleanupStack::PushL( engine );
    
    EUNIT_ASSERT_DESC( engine, "CFEngine not created!" );
    
    iTestDelay->Start( KSecond * 4 );
    
    CleanupStack::PopAndDestroy();
    }

void UT_CCFEngine::UT_CCFEngine_NewLCL(  )
    {
    CCFEngine* engine = CCFEngine::NewLC( *this );
    
    EUNIT_ASSERT_DESC( engine, "CFEngine not created!" );
    
    iTestDelay->Start( KSecond * 2 );

    CleanupStack::PopAndDestroy(engine);
    }

void UT_CCFEngine::UT_PublishContextL( )
	{
	RThread thread;
	CleanupClosePushL(thread);

	CCFContextObject* co = CCFContextObject::NewLC();
	co->SetSourceL(_L("testsource"));
	co->SetTypeL(_L("testtype"));
	co->SetValueL(_L("testvalue"));
	TInt error = iCFContextInterface->PublishContext( *co, thread );	
	EUNIT_ASSERT_DESC ( error, "Context published succeed without defining!" );
	
	static _LIT_SECURITY_POLICY_PASS( alwaysPass );
	error = iCFContextInterface->DefineContext( _L("testsource"),
		_L("testtype"), alwaysPass );
	
	EUNIT_ASSERT_DESC ( !error, "Could not define context!" );
	
	error = iCFContextInterface->PublishContext( *co, thread );	
	EUNIT_ASSERT_DESC ( !error, "Context published returned an error!" );
		
	CleanupStack::PopAndDestroy( co );
	CleanupStack::PopAndDestroy( &thread );
	}



void UT_CCFEngine::UT_SubscribeContextL()
	{
	RThread thread;
	CleanupClosePushL(thread);

	CCFContextSubscription* subscription = CCFContextSubscription::NewLC();
	subscription->SetContextSourceL(_L("testsource"));
	subscription->SetContextTypeL(_L("listenedtype"));
	CleanupStack::Pop( subscription );

	// make subscription	
	TInt error = iCFContextInterface->SubscribeContext(subscription,
		this, thread);
	EUNIT_ASSERT_DESC ( !error, "Subscription did not succeeded!");

	// define context 1
	static _LIT_SECURITY_POLICY_PASS( alwaysPass );
	error = iCFContextInterface->DefineContext( _L("testsource"),
		_L("notlistenedtype"), alwaysPass );
	EUNIT_ASSERT_DESC ( !error, "Could not define context!" );

	// define context 2
	error = iCFContextInterface->DefineContext( _L("testsource"),
		_L("listenedtype"), alwaysPass );
	EUNIT_ASSERT_DESC ( !error, "Could not define context!" );


	CCFContextObject* co = CCFContextObject::NewLC();
	co->SetSourceL(_L("testsource"));
	co->SetTypeL(_L("listenedtype"));
	co->SetValueL(_L("testvalue"));
	error = iCFContextInterface->PublishContext( *co, thread );	
	EUNIT_ASSERT_DESC ( !error, "Context publish returned an error!" );

    iTestDelay->Start( KSecond / 2 );

	// check that we get the published context
	EUNIT_ASSERT_DESC ( iIndication, "Context indication did not arrive");
	iIndication = EFalse;
	
	//CCFContextObject* co = CCFContextObject::NewLC();
	co->SetSourceL(_L("testsource"));
	co->SetTypeL(_L("notlistenedtype"));
	co->SetValueL(_L("testvalue"));
	error = iCFContextInterface->PublishContext( *co, thread );	
	EUNIT_ASSERT_DESC ( !error, "Context publish returned an error!" );
	
	// check that we did not get the published context
	EUNIT_ASSERT_DESC ( !iIndication,
		"Context indication arrived for context not listened!");
	
	
	// Test subscription that fails later because of capabilities
	// Cannot be tested from the same process because
	// subscription from same process will always succeed.
	/* 
	CCFContextSubscription* subscription2 = CCFContextSubscription::NewL();
	subscription->SetContextSource(_L("testsource"));
	subscription->SetContextType(_L("highcapabilities"));
	
	error = iCFContextInterface->SubscribeContext(subscription2,
		this, thread);
	EUNIT_ASSERT_DESC ( !error, "Subscription did not succeeded!");
	
	static _LIT_SECURITY_POLICY_FAIL( alwaysFail );
	error = iCFContextInterface->DefineContext( _L("testsource"),
		_L("highcapabilities"), alwaysFail );
	EUNIT_ASSERT_DESC ( !error, "Could not define context!" );
	
	iTestDelay->Start(1000000); // 1 seconds
	
	EUNIT_ASSERT_DESC ( iSubscriptionError, 
		"Subscription remains although high capabilities required");
	
	*/
	
	CleanupStack::PopAndDestroy( co );	
	CleanupStack::PopAndDestroy( &thread );	
	}

void UT_CCFEngine::UT_RemoveContextSubscriptionL()
	{
	RThread thread;
	CleanupClosePushL(thread);

	CCFContextSubscription* subscription = CCFContextSubscription::NewLC();
	subscription->SetContextSourceL(_L("testsource"));
	subscription->SetContextTypeL(_L("listenedtype"));
	CleanupStack::Pop( subscription );

	// make subscription	
	TInt error = iCFContextInterface->SubscribeContext(subscription,
		this, thread);
	EUNIT_ASSERT_DESC ( !error, "Subscription did not succeeded!");

	// define context
	static _LIT_SECURITY_POLICY_PASS( alwaysPass );
	error = iCFContextInterface->DefineContext( _L("testsource"),
		_L("listenedtype"), alwaysPass );
	EUNIT_ASSERT_DESC ( !error, "Could not define context!" );

	iCFContextInterface->UnsubscribeContext(*subscription,
		*this);
	EUNIT_ASSERT_DESC ( !error, "Could not remove context subscription!" );	
    
    iTestDelay->Start( KSecond / 2 );

	CCFContextObject* co = CCFContextObject::NewLC();
	co->SetSourceL(_L("testsource"));
	co->SetTypeL(_L("listenedtype"));
	co->SetValueL(_L("testvalue"));
	error = iCFContextInterface->PublishContext( *co, thread );	
	EUNIT_ASSERT_DESC ( !error, "Context publish returned an error!" );

    iTestDelay->Start( KSecond / 2 );

	// check that we get the published context
	EUNIT_ASSERT_DESC ( !iIndication, "Context indication did arrive");
	iIndication = EFalse;
		
	CleanupStack::PopAndDestroy( co );	
	CleanupStack::PopAndDestroy( &thread );	
	}

void UT_CCFEngine::UT_RemoveSubscriptionDuringIdicationL()
    {
    RThread thread;
    CleanupClosePushL(thread);

    CCFContextSubscription* subscription1 = CCFContextSubscription::NewLC();
    subscription1->SetContextSourceL(_L("testsource"));
    subscription1->SetContextTypeL(_L("listenedtype"));
    CleanupStack::Pop( subscription1 );

    // make the dummy subscription
    TInt error = iCFContextInterface->SubscribeContext(subscription1,
        this, thread);
    EUNIT_ASSERT_DESC ( error == KErrNone, "Subscription did not succeeded!");

    CCFContextSubscription* subscription2 = CCFContextSubscription::NewLC();
    subscription2->SetContextSourceL(_L("testsource"));
    subscription2->SetContextTypeL(_L("listenedtype"));
    CleanupStack::Pop( subscription2 );

    // make subscription    
    error = iCFContextInterface->SubscribeContext(subscription2, this, thread);
    EUNIT_ASSERT_DESC ( error == KErrNone, "Subscription did not succeeded!");

    // define context
    static _LIT_SECURITY_POLICY_PASS( alwaysPass );
    error = iCFContextInterface->DefineContext( _L("testsource"),
        _L("listenedtype"), alwaysPass );
    EUNIT_ASSERT_DESC ( error == KErrNone, "DefineContext did not succeeded!");

    // Set remove susbcription that will be removed during next context
    // indication
    iRemoveSubscription = subscription1;

    iIndication = EFalse;
    CCFContextObject* co = CCFContextObject::NewLC();
    co->SetSourceL(_L("testsource"));
    co->SetTypeL(_L("listenedtype"));
    co->SetValueL(_L("testvalue"));
    error = iCFContextInterface->PublishContext( *co, thread );

    iTestDelay->Start( KSecond / 2 );

    // check that we get the published context
    EUNIT_ASSERT_DESC ( iIndication, "Context indication shoud have arrived");
    iIndication = EFalse;

    CleanupStack::PopAndDestroy( co );
    CleanupStack::PopAndDestroy( &thread );
    }

void UT_CCFEngine::UT_RemoveSubscriptionDuringIdication2L()
    {
    RThread thread;
    CleanupClosePushL(thread);

    CCFTestContextListener* listener = CCFTestContextListener::NewLC();

    CCFContextSubscription* subscription1 = CCFContextSubscription::NewLC();
    subscription1->SetContextSourceL(_L("testsource"));
    subscription1->SetContextTypeL(_L("listenedtype"));
    CleanupStack::Pop( subscription1 );

    // make the dummy subscription
    TInt error = iCFContextInterface->SubscribeContext(subscription1,
        this, thread);
    EUNIT_ASSERT_DESC ( error == KErrNone, "Subscription did not succeeded!");

    CCFContextSubscription* subscription2 = CCFContextSubscription::NewLC();
    subscription2->SetContextSourceL(_L("testsource"));
    subscription2->SetContextTypeL(_L("listenedtype"));
    CleanupStack::Pop( subscription2 );

    // make subscription    
    error = iCFContextInterface->SubscribeContext(subscription2, this, thread);
    EUNIT_ASSERT_DESC ( error == KErrNone, "Subscription did not succeeded!");

    CCFContextSubscription* subscription3 = CCFContextSubscription::NewLC();
    subscription3->SetContextSourceL(_L("testsource"));
    subscription3->SetContextTypeL(_L("listenedtype"));
    CleanupStack::Pop( subscription3 );

    // make subscription with listener
    error = iCFContextInterface->SubscribeContext(subscription3, listener, thread);
    EUNIT_ASSERT_DESC ( error == KErrNone, "Subscription did not succeeded!");

    // define context
    static _LIT_SECURITY_POLICY_PASS( alwaysPass );
    error = iCFContextInterface->DefineContext( _L("testsource"),
        _L("listenedtype"), alwaysPass );
    EUNIT_ASSERT_DESC ( error == KErrNone, "DefineContext did not succeeded!");

    // Set remove susbcriptions flag so that all subscription from this
    // (UT_CCFEngine) listener will be removed during next context indication.
    iRemoveSubscriptions = ETrue;

    EUNIT_ASSERT_DESC ( !listener->iLastIndication, "Separate listener should not have context indication");

    iIndication = EFalse;
    CCFContextObject* co = CCFContextObject::NewLC();
    co->SetSourceL(_L("testsource"));
    co->SetTypeL(_L("listenedtype"));
    co->SetValueL(_L("testvalue"));
    error = iCFContextInterface->PublishContext( *co, thread );

    iTestDelay->Start( KSecond / 2 );

    // check that we get the published context
    EUNIT_ASSERT_DESC ( iIndication, "Context indication shoud have arrived");
    iIndication = EFalse;
    // check that separate listener class also received indication
    EUNIT_ASSERT_DESC ( listener->iLastIndication, "Context indication shoud have arrived for separate listener");
    EUNIT_ASSERT_DESC ( !listener->iSubscriptionError, "Subscription error for separate listener");

    CleanupStack::PopAndDestroy( co );
    CleanupStack::PopAndDestroy( listener );
    CleanupStack::PopAndDestroy( &thread );
    }

void UT_CCFEngine::UT_DefineContextL()
	{
	RThread thread;
	CleanupClosePushL( thread );
    static _LIT_SECURITY_POLICY_PASS( alwaysPass );

    // x. Define test context
    //--------------------------------------------------------------------------
    TInt err = iCFContextInterface->DefineContext(
        _L("testsource"), _L("testtype"), alwaysPass );
    EUNIT_ASSERT_DESC ( err == KErrNone, "Define context failed" );

    // x. Re-define test context
    //--------------------------------------------------------------------------
    err = iCFContextInterface->DefineContext(
        _L("testsource"), _L("testtype"), alwaysPass );
    EUNIT_ASSERT_DESC ( err == KErrNone, "Define context failed" );

	CleanupStack::PopAndDestroy( &thread );
	}


//  TEST TABLE

EUNIT_BEGIN_TEST_TABLE(
    UT_CCFEngine,
    "CCFEngine class unit test.",
    "UNIT" )

EUNIT_TEST(
    "NewL - test ",
    "CCFEngine",
    "NewL",
    "FUNCTIONALITY",
    SetupForNewL, UT_CCFEngine_NewLL, TeardownForNewL)

EUNIT_TEST(
    "NewLC - test ",
    "CCFEngine",
    "NewLC",
    "FUNCTIONALITY",
    SetupForNewL, UT_CCFEngine_NewLCL, TeardownForNewL)

EUNIT_TEST(
    "PublishContext - test ",
    "CCFEngine",
    "PublishContext",
    "FUNCTIONALITY",
    SetupL, UT_PublishContextL, Teardown)

EUNIT_TEST(
    "SubscribeContext - test ",
    "CCFEngine",
    "SubscribeContext",
    "FUNCTIONALITY",
    SetupL, UT_SubscribeContextL, Teardown)
    
EUNIT_TEST(
    "DefineContext - test ",
    "CCFEngine",
    "DefineContext",
    "FUNCTIONALITY",
    SetupL, UT_DefineContextL, Teardown)

EUNIT_TEST(
    "RemoveContextSubscription - test ",
    "CCFEngine",
    "RemoveContextSubscription",
    "FUNCTIONALITY",
    SetupL, UT_RemoveContextSubscriptionL, Teardown)

EUNIT_TEST(
    "2 RemoveContextSubscription - during context indication",
    "CCFEngine",
    "RemoveContextSubscription",
    "FUNCTIONALITY",
    SetupL, UT_RemoveSubscriptionDuringIdicationL, Teardown)

EUNIT_TEST(
    "3 RemoveContextSubscription - 2nd during context indication",
    "CCFEngine",
    "RemoveContextSubscription",
    "FUNCTIONALITY",
    SetupL, UT_RemoveSubscriptionDuringIdication2L, Teardown)


EUNIT_END_TEST_TABLE

//  END OF FILE

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
#include "UT_CCFContextManager.h"

//  EXTERNAL INCLUDES
#include <EUnitMacros.h>


//  INTERNAL INCLUDES
#include "CFContextManager.h"
#include "CFContextIndication.h"
#include "CFContextSubscription.h"
#include "cfcontextobjectimpl.h"


// CONSTRUCTION
UT_CCFContextManager* UT_CCFContextManager::NewL()
    {
    UT_CCFContextManager* self = UT_CCFContextManager::NewLC();
    CleanupStack::Pop();

    return self;
    }

UT_CCFContextManager* UT_CCFContextManager::NewLC()
    {
    UT_CCFContextManager* self = new( ELeave ) UT_CCFContextManager();
    CleanupStack::PushL( self );

    self->ConstructL();

    return self;
    }

// Destructor (virtual by CBase)
UT_CCFContextManager::~UT_CCFContextManager()
    {
    }

// Default constructor
UT_CCFContextManager::UT_CCFContextManager()
    {
    }

// Second phase construct
void UT_CCFContextManager::ConstructL()
    {
    // The ConstructL from the base class CEUnitTestSuiteClass must be called.
    // It generates the test case table.
    CEUnitTestSuiteClass::ConstructL();
    }


// INTERFACE IMPLEMENTATIONS

// from MCFContextSubscriptionListener
void UT_CCFContextManager::ContextIndicationL( 
	CCFContextIndication* aIndication )
	{
	iIndication = ETrue;
	delete iContextIndication;
	iContextIndication = aIndication;		
    }

// from MCFContextSubscriptionListener
TInt UT_CCFContextManager::Client( RThread& aThread ) const
	{
	aThread = RThread();
	return KErrNone;
	}
        
// from MCFContextSubscriptionListener
void UT_CCFContextManager::HandleContextSubscriptionError( TInt /*aError*/,
            const TDesC& /*aSource*/,
            const TDesC& /*aType*/ )
    {
    iSubscriptionError = ETrue;
    StopWait();
    }




// from MCFContextSource
void UT_CCFContextManager::Subscribers( const TDesC& aContextSource,
    const TDesC& aContextType )
	{
	iSubscribersCalled = ETrue;
	EUNIT_ASSERT_DESC( iExpectedSubscribersSource.Compare( aContextSource ) == 0,
			"No expected source");
	EUNIT_ASSERT_DESC( iExpectedSubscribersType.Compare( aContextType ) == 0,
			"No expected type");
	}

// from MCFContextSource
void UT_CCFContextManager::NoSubscribers( const TDesC& aContextSource,
    const TDesC& aContextType )
	{
	iNoSubscribersCalled = ETrue;
	EUNIT_ASSERT_DESC( iExpectedSubscribersSource.Compare( aContextSource ) == 0,
	"No expected source");
	EUNIT_ASSERT_DESC( iExpectedSubscribersType.Compare( aContextType ) == 0,
	"No expected type");
	}

// from MCFContextSource
void UT_CCFContextManager::HandleCommand(
    const CCFContextSourceCommand& /*aCommand*/ )
    {
    // Nothing to do.
    }


// from MCFSecurityChecker
TBool UT_CCFContextManager::CheckClientSecurity( const RThread& aClientThread,
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


//  METHODS


void UT_CCFContextManager::SetupForNewL(  )
    {
	iWait = new( ELeave ) CActiveSchedulerWait;
    }

void UT_CCFContextManager::StopWait()
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

void UT_CCFContextManager::TeardownForNewL(  )
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


void UT_CCFContextManager::SetupL(  )
    {
	SetupForNewL();
	iCFContextManager = CCFContextManager::NewL( *this );
	iCFContextManagerInterface = iCFContextManager;
    }

void UT_CCFContextManager::Teardown(  )
    {
    delete iContextIndication;
    iContextIndication = NULL;
	delete iCFContextManager;
	iCFContextManagerInterface = NULL;
	iIndication = EFalse;
	iSubscriptionError = EFalse;
	iSubscribersCalled = EFalse;
	iNoSubscribersCalled = EFalse;
	TeardownForNewL();
    }


void UT_CCFContextManager::SetupContextCacheL()
	{
	RThread thread;
	CleanupClosePushL(thread);
	CCFContextObject* co = CCFContextObject::NewLC();
	static _LIT_SECURITY_POLICY_PASS( alwaysPass );
	
	static _LIT_SECURITY_POLICY_FAIL( alwaysFail );

	// CONTEXT 1
	co->SetSourceL(_L("SourceA"));
	co->SetTypeL(_L("TypeA"));
	co->SetValueL(_L("ValueBA"));
	EUNIT_ASSERT_NO_LEAVE (
		iCFContextManagerInterface->DefineContextL( co->Source(),
		co->Type(), alwaysPass, alwaysPass ) );
	EUNIT_ASSERT_NO_LEAVE (
		iCFContextManagerInterface->PublishContextL( *co, thread ) );	

	// CONTEXT 2
	co->SetSourceL(_L("SourceB"));
	co->SetTypeL(_L("TypeA"));
	co->SetValueL(_L("ValueBA"));
	EUNIT_ASSERT_NO_LEAVE (
		iCFContextManagerInterface->DefineContextL( co->Source(),
		co->Type(), alwaysPass, alwaysPass ) );
	EUNIT_ASSERT_NO_LEAVE (
		iCFContextManagerInterface->PublishContextL( *co, thread ) );	

	// CONTEXT 3
	co->SetSourceL(_L("SourceC"));
	co->SetTypeL(_L("TypeC"));
	co->SetValueL(_L("ValueCC"));
	EUNIT_ASSERT_NO_LEAVE (
		iCFContextManagerInterface->DefineContextL( co->Source(),
		co->Type(), alwaysPass, alwaysPass ) );
	EUNIT_ASSERT_NO_LEAVE (
		iCFContextManagerInterface->PublishContextL( *co, thread ) );	

	// CONTEXT 4
	co->SetSourceL(_L("SourceC"));
	co->SetTypeL(_L("TypeD"));
	co->SetValueL(_L("ValueCD"));
	EUNIT_ASSERT_NO_LEAVE (
		iCFContextManagerInterface->DefineContextL( co->Source(),
		co->Type(), alwaysPass, alwaysPass ) );
	EUNIT_ASSERT_NO_LEAVE (
		iCFContextManagerInterface->PublishContextL( *co, thread ) );	

	// CONTEXT 5
	co->SetSourceL(_L("SourceC"));
	co->SetTypeL(_L("TypeC.SubtypeA"));
	co->SetValueL(_L("ValueCCA"));
	EUNIT_ASSERT_NO_LEAVE (
		iCFContextManagerInterface->DefineContextL( co->Source(),
		co->Type(), alwaysPass, alwaysPass ) );
	EUNIT_ASSERT_NO_LEAVE (
		iCFContextManagerInterface->PublishContextL( *co, thread ) );	

	// CONTEXT 6
	co->SetSourceL(_L("SourceC"));
	co->SetTypeL(_L("TypeC.SubtypeB"));
	co->SetValueL(_L("ValueCCB"));
	EUNIT_ASSERT_NO_LEAVE (
		iCFContextManagerInterface->DefineContextL( co->Source(),
		co->Type(), alwaysPass, alwaysPass ) );
	EUNIT_ASSERT_NO_LEAVE (
		iCFContextManagerInterface->PublishContextL( *co, thread ) );	


	// bypass security check so that we publish context with high capabilities

	// CONTEXT  - no read or write access
	co->SetSourceL(_L("SourceE"));
	co->SetTypeL(_L("TypeE.NoaccessA"));
	co->SetValueL(_L("ValueEEA"));
	EUNIT_ASSERT_NO_LEAVE (
		iCFContextManagerInterface->DefineContextL( co->Source(),
		co->Type(), alwaysFail, alwaysFail ) );
	iBypassSecurityCheck = 1;
	EUNIT_ASSERT_NO_LEAVE (
		iCFContextManagerInterface->PublishContextL( *co, thread ) );	

	// CONTEXT - no write access
	co->SetSourceL(_L("SourceE"));
	co->SetTypeL(_L("TypeE.NoWriteAccessA"));
	co->SetValueL(_L("ValueEEA"));
	EUNIT_ASSERT_NO_LEAVE (
		iCFContextManagerInterface->DefineContextL( co->Source(),
		co->Type(), alwaysPass, alwaysFail ) );
	iBypassSecurityCheck = 1;
	EUNIT_ASSERT_NO_LEAVE (
		iCFContextManagerInterface->PublishContextL( *co, thread ) );	

	// CONTEXT - no read access
	co->SetSourceL(_L("SourceE"));
	co->SetTypeL(_L("TypeE.NoReadAccessA"));
	co->SetValueL(_L("ValueEEA"));
	EUNIT_ASSERT_NO_LEAVE (
		iCFContextManagerInterface->DefineContextL( co->Source(),
		co->Type(), alwaysFail, alwaysPass ) );
	iBypassSecurityCheck = 1;
	EUNIT_ASSERT_NO_LEAVE (
		iCFContextManagerInterface->PublishContextL( *co, thread ) );	
	
	// CONTEXT - full access
	co->SetSourceL(_L("SourceE"));
	co->SetTypeL(_L("TypeE.AccessB"));
	co->SetValueL(_L("ValueEEB"));
	EUNIT_ASSERT_NO_LEAVE (
		iCFContextManagerInterface->DefineContextL( co->Source(),
		co->Type(), alwaysPass, alwaysPass ) );
	iBypassSecurityCheck = 1;
	EUNIT_ASSERT_NO_LEAVE (
		iCFContextManagerInterface->PublishContextL( *co, thread ) );	

	CleanupStack::PopAndDestroy( co );
	CleanupStack::PopAndDestroy( &thread );
	}

TInt UT_CCFContextManager::FindContext(RContextObjectArray& aArray, 
	const TDesC& aSource, const TDesC& aType, const TDesC& aValue)
	{
	TInt index = KErrNotFound;
	for (TInt i = 0; i < aArray.Count(); i++)
		{
		if (aArray[i]->Source() == aSource &&
			aArray[i]->Type() == aType)
			{
			if (index == KErrNotFound)
				{
				index = i;
				if (aArray[i]->Value() == aValue)
					{
					return index;	
					}
				else
					{
					return -3; // found but wrong value!
					}
				}
			else 
				{
				index = -2; // Duplicate found!
				}
			}
		}
	return index;
	} 


// TESTS

void UT_CCFContextManager::UT_CCFContextManager_NewLL(  )
    {
    CCFContextManager* instance = CCFContextManager::NewL( *this );
    CleanupStack::PushL ( instance );
    EUNIT_ASSERT_DESC ( instance, "CCFContextManager not created!" );
    CleanupStack::PopAndDestroy( instance );
    }

void UT_CCFContextManager::UT_CCFContextManager_NewLCL(  )
    {
    CCFContextManager* instance = CCFContextManager::NewLC( *this );
    EUNIT_ASSERT_DESC ( instance, "CCFContextManager not created!" );
    CleanupStack::PopAndDestroy( instance );
    }

void UT_CCFContextManager::UT_CCFContextManager_PublishContextLL(  )
    {
	RThread thread;
	CleanupClosePushL(thread);

	CCFContextObject* co = CCFContextObject::NewLC();
	co->SetSourceL(_L("testsource"));
	co->SetTypeL(_L("testtype"));
	co->SetValueL(_L("testvalue"));
	EUNIT_ASSERT_LEAVE ( iCFContextManagerInterface
		->PublishContextL( *co, thread ) );
		//, "Context published succeed without defining!" );
	
	static _LIT_SECURITY_POLICY_PASS( alwaysPass );
	EUNIT_ASSERT_NO_LEAVE ( 
		iCFContextManagerInterface->DefineContextL( _L("testsource"),
		_L("testtype"), alwaysPass, alwaysPass ) );
	
	EUNIT_ASSERT_NO_LEAVE ( 
		iCFContextManagerInterface->PublishContextL( *co, thread ) );	
		
	CleanupStack::PopAndDestroy( co );
	CleanupStack::PopAndDestroy( &thread );
    }


void UT_CCFContextManager::UT_CCFContextManager_RequestContextL_2L(  )
    {
	RThread thread;
	CleanupClosePushL(thread);
	
	SetupContextCacheL();

	RContextObjectArray resultBuffer;
	resultBuffer.Reset();

	// QUERY 1

	CCFContextQuery* query = CCFContextQuery::NewLC( KNullDesC, KNullDesC );
	query->SetSourceL(_L("SourceC"));
	query->SetTypeL(_L("TypeC"));
	
    EUNIT_ASSERT_NO_LEAVE( 
    	iCFContextManagerInterface->RequestContextL(
    	resultBuffer,
        *query,
        thread )
		);

	EUNIT_ASSERT_DESC( resultBuffer.Count() == 1, 
		"More than 1 context returned!" );

	TInt index = FindContext(resultBuffer, _L("SourceC"), _L("TypeC"), _L("ValueCC"));
	EUNIT_ASSERT_DESC( index >= 0, "ResultError!" );

	// QUERY 2
	
	query->SetSourceL(_L("SourceC"));
	query->SetTypeL(_L(""));
	
	resultBuffer.Reset();
	query->SetSubTypeMatch(ETrue);

	EUNIT_ASSERT_NO_LEAVE( 
    	iCFContextManagerInterface->RequestContextL(
    	resultBuffer,
        *query,
        thread )
		);

	EUNIT_ASSERT_DESC( resultBuffer.Count() == 4, 
		"Did not return 4 contexts!" );

	index = FindContext(resultBuffer, _L("SourceC"), _L("TypeC"), _L("ValueCC"));
	EUNIT_ASSERT_DESC( index >= 0, "ResultError!" );

	index = FindContext(resultBuffer, _L("SourceC"), _L("TypeD"), _L("ValueCD"));
	EUNIT_ASSERT_DESC( index >= 0, "ResultError!" );

	index = FindContext(resultBuffer, _L("SourceC"), _L("TypeC.SubtypeA"), _L("ValueCCA"));
	EUNIT_ASSERT_DESC( index >= 0, "ResultError!" );

	index = FindContext(resultBuffer, _L("SourceC"), _L("TypeC.SubtypeB"), _L("ValueCCB"));
	EUNIT_ASSERT_DESC( index >= 0, "ResultError!" );

	resultBuffer.Close();
	CleanupStack::PopAndDestroy( query );
	CleanupStack::PopAndDestroy( &thread );
    }

void UT_CCFContextManager::UT_CCFContextManager_DefineContextLL(  )
    {
	RThread thread;
	CleanupClosePushL(thread);

	// define context 1
	static _LIT_SECURITY_POLICY_PASS( alwaysPass );
	EUNIT_ASSERT_NO_LEAVE ( 
		iCFContextManagerInterface->DefineContextL( _L("testsource"),
		_L("testtype"), alwaysPass, alwaysPass ) );

	// try to redefine 
    EUNIT_ASSERT_NO_LEAVE ( 
		iCFContextManagerInterface->DefineContextL( _L("testsource"),
		_L("testtype"), alwaysPass, alwaysPass ) );

	CleanupStack::PopAndDestroy( &thread );	
    }

void UT_CCFContextManager::UT_CCFContextManager_DefineContextL2L(  )
    {
	RThread thread;
	CleanupClosePushL(thread);

	TUid uid = TUid::Uid( 123 );
	
	// define context 1
	static _LIT_SECURITY_POLICY_PASS( alwaysPass );
	EUNIT_ASSERT_NO_LEAVE ( 
		iCFContextManagerInterface->DefineContextL( _L("testsource"),
		_L("testtype"), alwaysPass, alwaysPass, this, uid ) );

	// try to redefine 
	EUNIT_ASSERT_NO_LEAVE ( 
		iCFContextManagerInterface->DefineContextL( _L("testsource"),
		_L("testtype"), alwaysPass, alwaysPass, this, uid ) );

	// try to redefine with another UID
	TUid anotherUid = TUid::Uid( 345 );
	EUNIT_ASSERT_LEAVE (
		iCFContextManagerInterface->DefineContextL( _L("testsource"),
		_L("testtype"), alwaysPass, alwaysPass, this, anotherUid ) );
	
	// check that we get the subscribers notification
	CCFContextSubscription* subscription = CCFContextSubscription::NewLC();
	subscription->SetContextSourceL(_L("testsource"));
	subscription->SetContextTypeL(_L("testtype"));
	CleanupStack::Pop( subscription );

	iSubscribersCalled = EFalse;
	iExpectedSubscribersSource.Copy(_L("testsource"));
	iExpectedSubscribersType.Copy(_L("testtype"));
	
	// make subscription	
	EUNIT_ASSERT_NO_LEAVE (
		iCFContextManagerInterface->SubscribeContextL(subscription,
		this, thread) );
	
	EUNIT_ASSERT( iSubscribersCalled );
	iSubscribersCalled = EFalse;
	iNoSubscribersCalled = EFalse;
	
	
	EUNIT_ASSERT_NO_LEAVE (
			iCFContextManagerInterface->RemoveSubscription( *subscription,
			*this) );

	EUNIT_ASSERT( iNoSubscribersCalled );
	
	
	CleanupStack::PopAndDestroy( &thread );	
    }



void UT_CCFContextManager::UT_CCFContextManager_RequestContextL_3L(  )
    {
	RThread thread;
	CleanupClosePushL(thread);
	
	SetupContextCacheL();
	CCFContextObject* co;
	
    EUNIT_ASSERT_NO_LEAVE( 
    	co = iCFContextManagerInterface->RequestContextL(
    	_L("TypeA"),
        _L("SourceA"),
        thread )
		);

	EUNIT_ASSERT_DESC( co->Source() == _L("SourceA"), "Source doesn't match!");
	EUNIT_ASSERT_DESC( co->Type() == _L("TypeA"), "Type doesn't match!");
	EUNIT_ASSERT_DESC( co->Value() == _L("ValueBA"), "Value doesn't match!");

	CleanupStack::PopAndDestroy( &thread );	
    }

void UT_CCFContextManager::UT_CCFContextManager_SubscribeContextLL(  )
    {
	RThread thread;
	CleanupClosePushL(thread);

	SetupContextCacheL();

	CCFContextSubscription* subscription = CCFContextSubscription::NewLC();
	subscription->SetContextSourceL(_L("SourceC"));
	subscription->SetContextTypeL(_L("TypeC"));
	CleanupStack::Pop( subscription );

	// make subscription	
	EUNIT_ASSERT_NO_LEAVE (
		iCFContextManagerInterface->SubscribeContextL(subscription,
		this, thread) );


	CCFContextObject* co = CCFContextObject::NewLC();
	co->SetSourceL(_L("SourceC"));
	co->SetTypeL(_L("TypeC"));
	co->SetValueL(_L("ValueCC2"));
	EUNIT_ASSERT_NO_LEAVE (
		iCFContextManagerInterface->PublishContextL( *co, thread ) );	
	
	// check that we get the published context
	EUNIT_ASSERT_DESC ( iIndication, "Context indication did not arrive");
	const CCFContextObject* indicatedCo = &iContextIndication->Context();
	EUNIT_ASSERT_DESC ( indicatedCo->Source() == _L("SourceC"),
		"Source of indication does not match!" );
	EUNIT_ASSERT_DESC ( indicatedCo->Type() == _L("TypeC"),
		"Type of indication does not match!" );
	EUNIT_ASSERT_DESC ( indicatedCo->Value() == _L("ValueCC2"),
		"Value of indication does not match!" );
	iIndication = EFalse;
	
	// Publish a context 	
	//CCFContextObject co;
	co->SetSourceL(_L("SourceC"));
	co->SetTypeL(_L("TypeC.SubtypeB"));
	co->SetValueL(_L("ValueCCB2"));
	EUNIT_ASSERT_NO_LEAVE (
		iCFContextManagerInterface->PublishContextL( *co, thread ) );	
	
	// check that we did not get the published context
	EUNIT_ASSERT_DESC ( !iIndication,
		"Context indication arrived for context not listened!");


	// check that when value not changed, still indicated default
	co->SetSourceL(_L("SourceC"));
	co->SetTypeL(_L("TypeC"));
	co->SetValueL(_L("ValueCC2"));
	EUNIT_ASSERT_NO_LEAVE (
		iCFContextManagerInterface->PublishContextL( *co, thread ) );	
	
	// check that we get the published context
	EUNIT_ASSERT_DESC ( iIndication, "Context indication did not arrive");
	indicatedCo = &iContextIndication->Context();
	EUNIT_ASSERT_DESC ( indicatedCo->Source() == _L("SourceC"),
		"Source of indication does not match!" );
	EUNIT_ASSERT_DESC ( indicatedCo->Type() == _L("TypeC"),
		"Type of indication does not match!" );
	EUNIT_ASSERT_DESC ( indicatedCo->Value() == _L("ValueCC2"),
		"Value of indication does not match!" );
	iIndication = EFalse;
	
	// Test subscription for subtypes
	iIndication = EFalse;

	CCFContextSubscription* subscription2 = CCFContextSubscription::NewL();
	subscription2->SetContextSourceL(_L("SourceC"));
	subscription2->SetContextTypeL(_L("TypeC"));
	subscription2->SetSubTypeMatch( ETrue );
	subscription2->SetOnlyChangesIndicated( ETrue );

	// make subscription	
	EUNIT_ASSERT_NO_LEAVE (
		iCFContextManagerInterface->SubscribeContextL(subscription2,
		this, thread) );


	//CCFContextObject co;
	co->SetSourceL(_L("SourceC"));
	co->SetTypeL(_L("TypeC.SubtypeA"));
	co->SetValueL(_L("ValueCCA2"));
	EUNIT_ASSERT_NO_LEAVE (
		iCFContextManagerInterface->PublishContextL( *co, thread ) );	
	
	// check that we get the published context
	EUNIT_ASSERT_DESC ( iIndication, "Context indication did not arrive");
	indicatedCo = &iContextIndication->Context();
	EUNIT_ASSERT_DESC ( indicatedCo->Source() == _L("SourceC"),
		"Source of indication does not match!" );
	EUNIT_ASSERT_DESC ( indicatedCo->Type() == _L("TypeC.SubtypeA"),
		"Type of indication does not match!" );
	EUNIT_ASSERT_DESC ( indicatedCo->Value() == _L("ValueCCA2"),
		"Value of indication does not match!" );
	iIndication = EFalse;


	// because of SetOnlyChangesIndicated( ETrue ) we should not get
	// an indication when the remain the same
	//CCFContextObject co;
	co->SetSourceL(_L("SourceC"));
	co->SetTypeL(_L("TypeC.SubtypeA"));
	co->SetValueL(_L("ValueCCA2"));
	EUNIT_ASSERT_NO_LEAVE (
		iCFContextManagerInterface->PublishContextL( *co, thread ) );	
	
	// check that we get the published context
	EUNIT_ASSERT_DESC ( !iIndication, 
		"Context indication despite of same value");



	// Test subscription cases where there is no enough capabilities
	CCFContextSubscription* subscription3 = CCFContextSubscription::NewL();
	subscription3->SetContextSourceL(_L("SourceE"));
	subscription3->SetContextTypeL(_L("TypeE.NoaccessA"));
	//subscription2->SetSubTypeMatch( ETrue );
	//subscription2->SetOnlyChangesIndicated( ETrue );

	// make subscription	
	EUNIT_ASSERT_NO_LEAVE (
		iCFContextManagerInterface->SubscribeContextL(subscription3,
		this, thread) );
	
	EUNIT_ASSERT_DESC ( iSubscriptionError,
	"No subscription error when subscribing context defined with impossible capabilities");


	// Verity that the subscription with insufficient capabilities does not
	// indicate 
	//CCFContextObject co;
	iIndication = EFalse;
	co->SetSourceL(_L("SourceE"));
	co->SetTypeL(_L("TypeE.NoaccessA"));
	co->SetValueL(_L("ValueEEA2"));
	iBypassSecurityCheck = ETrue;
	EUNIT_ASSERT_NO_LEAVE (
		iCFContextManagerInterface->PublishContextL( *co, thread ) );	
	iBypassSecurityCheck = EFalse;
	
	// check that we get the published context
	EUNIT_ASSERT_DESC ( !iIndication, 
		"Context indication despite of insufficient capabilities");
	
	// Make a subscription with subtypes for TypeE
	iSubscriptionError = EFalse;
	CCFContextSubscription* subscription4 = CCFContextSubscription::NewL();
	subscription4->SetContextSourceL(_L("SourceE"));
	subscription4->SetContextTypeL(_L("TypeE"));
	subscription4->SetSubTypeMatch( ETrue );
	//subscription2->SetOnlyChangesIndicated( ETrue );

	// make subscription	
	EUNIT_ASSERT_NO_LEAVE (
		iCFContextManagerInterface->SubscribeContextL(subscription4,
		this, thread) );
	
	EUNIT_ASSERT_DESC ( iSubscriptionError,
	"No subscription error when subscribing context defined with impossible capabilities");


	// Verity that the subscription with insufficient capabilities does not
	// indicate 
	//CCFContextObject co;
	iIndication = EFalse;
	co->SetSourceL(_L("SourceE"));
	co->SetTypeL(_L("TypeE.NoaccessA"));
	co->SetValueL(_L("ValueEEA3"));
	iBypassSecurityCheck = 1;
	EUNIT_ASSERT_NO_LEAVE (
		iCFContextManagerInterface->PublishContextL( *co, thread ) );	
	
	// check that we do not get the published context
	EUNIT_ASSERT_DESC ( !iIndication, 
		"Context indication despite of insufficient capabilities");
	
	
	// Check that the subtree with sufficient capabilities indicates
	// indicate 
	//CCFContextObject co;
	iIndication = EFalse;
	co->SetSourceL(_L("SourceE"));
	co->SetTypeL(_L("TypeE.AccessB"));
	co->SetValueL(_L("ValueEEB2"));
	iBypassSecurityCheck = 1;
	EUNIT_ASSERT_NO_LEAVE (
		iCFContextManagerInterface->PublishContextL( *co, thread ) );	

	
	// check that we get the published context
	EUNIT_ASSERT_DESC ( iIndication, 
		"Did not get indication for TypeE.AccessB although sufficient capabilities");


	// define and publish context to verify cases if already made
	// subscriptions subscribe right way to new definitions
	
	static _LIT_SECURITY_POLICY_PASS( alwaysPass );
	
	static _LIT_SECURITY_POLICY_FAIL( alwaysFail );
	
	
	iSubscriptionError = EFalse;
	
	// CONTEXT 
	co->SetSourceL(_L("SourceE"));
	co->SetTypeL(_L("TypeE.NoaccessC"));
	co->SetValueL(_L("ValueEEC"));
	EUNIT_ASSERT_NO_LEAVE (
		iCFContextManagerInterface->DefineContextL( co->Source(),
		co->Type(), alwaysFail, alwaysFail ) );
		
	EUNIT_ASSERT_DESC ( iSubscriptionError,
	"No subscription error when impossible capabilities!");
		
		
	iIndication = EFalse;		
	iBypassSecurityCheck = 1;
	EUNIT_ASSERT_NO_LEAVE (
		iCFContextManagerInterface->PublishContextL( *co, thread ) );	

	EUNIT_ASSERT_DESC ( !iIndication, 
		"Indication for TypeE.NoaccessC although insufficient capabilities!");

	iSubscriptionError = EFalse;

	// CONTEXT 
	co->SetSourceL(_L("SourceE"));
	co->SetTypeL(_L("TypeE.AccessD"));
	co->SetValueL(_L("ValueEED"));
	EUNIT_ASSERT_NO_LEAVE (
		iCFContextManagerInterface->DefineContextL( co->Source(),
		co->Type(), alwaysPass, alwaysPass ) );

	EUNIT_ASSERT_DESC ( !iSubscriptionError,
	"Subscription error when capabilities are sufficient!");

	iIndication = EFalse;
	iBypassSecurityCheck = 1;
	EUNIT_ASSERT_NO_LEAVE (
		iCFContextManagerInterface->PublishContextL( *co, thread ) );	

	EUNIT_ASSERT_DESC ( iIndication, 
		"No indication for TypeE.AccessD although sufficient capabilities!");
	
	
	
	// Test subscription cases where there is no enough capabilities
	iSubscriptionError = EFalse;
	CCFContextSubscription* subscription5 = CCFContextSubscription::NewL();
	subscription5->SetContextSourceL(_L("SourceE"));
	subscription5->SetContextTypeL(_L("TypeE.NoReadAccessA"));
	//subscription2->SetSubTypeMatch( ETrue );
	//subscription2->SetOnlyChangesIndicated( ETrue );

	// make subscription	
	EUNIT_ASSERT_NO_LEAVE (
		iCFContextManagerInterface->SubscribeContextL(subscription5,
		this, thread) );
	
	EUNIT_ASSERT_DESC ( iSubscriptionError,
	"No subscription error when subscribing context defined with impossible capabilities");

	
	// Verity that the subscription with insufficient capabilities does not
	// indicate 
	//CCFContextObject co;
	iIndication = EFalse;
	co->SetSourceL(_L("SourceE"));
	co->SetTypeL(_L("TypeE.NoReadAccessA"));
	co->SetValueL(_L("ValueEEA3"));
	
	// with write access we do not have to bypass security check
	// iBypassSecurityCheck = 1; 
	EUNIT_ASSERT_NO_LEAVE (
		iCFContextManagerInterface->PublishContextL( *co, thread ) );	
	
	// check that we do not get the published context
	EUNIT_ASSERT_DESC ( !iIndication, 
		"Context indication despite of insufficient capabilities");

	

	// Test subscription cases where there is no enough capabilities
	iSubscriptionError = EFalse;
	CCFContextSubscription* subscription6 = CCFContextSubscription::NewL();
	subscription6->SetContextSourceL(_L("SourceE"));
	subscription6->SetContextTypeL(_L("TypeE.NoWriteAccessA"));
	//subscription2->SetSubTypeMatch( ETrue );
	//subscription2->SetOnlyChangesIndicated( ETrue );

	// make subscription	
	EUNIT_ASSERT_NO_LEAVE (
		iCFContextManagerInterface->SubscribeContextL(subscription6,
		this, thread) );
	
	EUNIT_ASSERT_DESC ( !iSubscriptionError,
	"Subscription error when subscribing context defined with impossible capabilities");
	
	// Verity that we cannot publish context with no write access
	// indicate 
	//CCFContextObject co;
	iIndication = EFalse;
	co->SetSourceL(_L("SourceE"));
	co->SetTypeL(_L("TypeE.NoWriteAccessA"));
	co->SetValueL(_L("ValueEEA3"));
	
	// do not bypass
	iBypassSecurityCheck = 0;
	
	EUNIT_ASSERT_LEAVE (
		iCFContextManagerInterface->PublishContextL( *co, thread ) );	
	
	// check that we do not get the published context
	EUNIT_ASSERT_DESC ( !iIndication, 
		"Context indication despite of insufficient capabilities");

	
	
	
	CleanupStack::PopAndDestroy( co );
	CleanupStack::PopAndDestroy( &thread );
    }

void UT_CCFContextManager::UT_CCFContextManager_RemoveSubscriptionL(  )
    {
	RThread thread;
	CleanupClosePushL(thread);

	CCFContextSubscription* subscription = CCFContextSubscription::NewLC();
	subscription->SetContextSourceL(_L("testsource"));
	subscription->SetContextTypeL(_L("listenedtype"));
	CleanupStack::Pop( subscription );

	// make subscription	
	EUNIT_ASSERT_NO_LEAVE (
		iCFContextManagerInterface->SubscribeContextL(subscription,
		this, thread) );

	// define context
	static _LIT_SECURITY_POLICY_PASS( alwaysPass );
	EUNIT_ASSERT_NO_LEAVE (
		iCFContextManagerInterface->DefineContextL( _L("testsource"),
		_L("listenedtype"), alwaysPass, alwaysPass ) );

	EUNIT_ASSERT_NO_LEAVE (
		iCFContextManagerInterface->RemoveSubscription(*subscription,
		*this) );

	CCFContextObject* co = CCFContextObject::NewLC();
	co->SetSourceL(_L("testsource"));
	co->SetTypeL(_L("listenedtype"));
	co->SetValueL(_L("testvalue"));
	EUNIT_ASSERT_NO_LEAVE (
		iCFContextManagerInterface->PublishContextL( *co, thread ) );	
	
	// check that we get the published context
	EUNIT_ASSERT_DESC ( !iIndication, "Context indication shoud not have arrived");
	iIndication = EFalse;
		
	CleanupStack::PopAndDestroy( co );
	CleanupStack::PopAndDestroy( &thread );
    }

void UT_CCFContextManager::UT_CCFContextManager_RemoveSubscription2L()
    {
    RThread thread;
    CleanupClosePushL(thread);

    CCFContextSubscription* dummySubscription = CCFContextSubscription::NewLC();
    dummySubscription->SetContextSourceL(_L("testsource"));
    dummySubscription->SetContextTypeL(_L("listenedtype"));
    CleanupStack::Pop( dummySubscription );

    // make the dummy subscription
    EUNIT_ASSERT_NO_LEAVE (
        iCFContextManagerInterface->SubscribeContextL(dummySubscription,
        this, thread) );

    CCFContextSubscription* subscription = CCFContextSubscription::NewLC();
    subscription->SetContextSourceL(_L("testsource"));
    subscription->SetContextTypeL(_L("listenedtype"));
    CleanupStack::Pop( subscription );

    // make subscription    
    EUNIT_ASSERT_NO_LEAVE (
        iCFContextManagerInterface->SubscribeContextL(subscription,
        this, thread) );

    // define context
    static _LIT_SECURITY_POLICY_PASS( alwaysPass );
    EUNIT_ASSERT_NO_LEAVE (
        iCFContextManagerInterface->DefineContextL( _L("testsource"),
        _L("listenedtype"), alwaysPass, alwaysPass ) );

    EUNIT_ASSERT_NO_LEAVE (
        iCFContextManagerInterface->RemoveSubscription(*subscription,
        *this) );

    CCFContextObject* co = CCFContextObject::NewLC();
    co->SetSourceL(_L("testsource"));
    co->SetTypeL(_L("listenedtype"));
    co->SetValueL(_L("testvalue"));
    EUNIT_ASSERT_NO_LEAVE (
        iCFContextManagerInterface->PublishContextL( *co, thread ) );   
    
    // check that we get the published context
    EUNIT_ASSERT_DESC ( !iIndication, "Context indication shoud not have arrived");
    iIndication = EFalse;

    CleanupStack::PopAndDestroy( co );
    CleanupStack::PopAndDestroy( &thread );
    }

void UT_CCFContextManager::UT_CCFContextManager_RemoveSubscriptionsL(  )
    {
    RThread thread;
	CleanupClosePushL(thread);

	CCFContextSubscription* subscription = CCFContextSubscription::NewLC();
	subscription->SetContextSourceL(_L("testsource"));
	subscription->SetContextTypeL(_L("listenedtype"));
	CleanupStack::Pop( subscription );

	// make subscription	
	EUNIT_ASSERT_NO_LEAVE (
		iCFContextManagerInterface->SubscribeContextL(subscription,
		this, thread) );

	// define context
	static _LIT_SECURITY_POLICY_PASS( alwaysPass );
	EUNIT_ASSERT_NO_LEAVE (
		iCFContextManagerInterface->DefineContextL( _L("testsource"),
		_L("listenedtype"), alwaysPass, alwaysPass ) );

	EUNIT_ASSERT_NO_LEAVE (
		iCFContextManagerInterface->RemoveSubscriptions(this) );

	CCFContextObject* co = CCFContextObject::NewLC();
	co->SetSourceL(_L("testsource"));
	co->SetTypeL(_L("listenedtype"));
	co->SetValueL(_L("testvalue"));
	EUNIT_ASSERT_NO_LEAVE (
		iCFContextManagerInterface->PublishContextL( *co, thread ) );	
	
	// check that we get the published context
	EUNIT_ASSERT_DESC ( !iIndication, "Context indication did not arrive");
	iIndication = EFalse;
		
	CleanupStack::PopAndDestroy( co );
	CleanupStack::PopAndDestroy( &thread );
    }

void UT_CCFContextManager::UT_CCFContextManager_DeregisterPublisherL(  )
    {
	RThread thread;
	CleanupClosePushL(thread);

	TUid uid = TUid::Uid( 123 );
	
	// define context 1
	static _LIT_SECURITY_POLICY_PASS( alwaysPass );
	EUNIT_ASSERT_NO_LEAVE ( 
		iCFContextManagerInterface->DefineContextL( _L("testsource"),
		_L("testtype"), alwaysPass, alwaysPass, this, uid ) );

	
	// do deregistering
	iCFContextManagerInterface->DeregisterPublisher( *this );
	
	// check that we get the subscribers notification
	CCFContextSubscription* subscription = CCFContextSubscription::NewLC();
	subscription->SetContextSourceL(_L("testsource"));
	subscription->SetContextTypeL(_L("testtype"));
	CleanupStack::Pop( subscription );

	iSubscribersCalled = EFalse;
	iExpectedSubscribersSource.Copy(_L("testsource"));
	iExpectedSubscribersType.Copy(_L("testtype"));
	
	// make subscription	
	EUNIT_ASSERT_NO_LEAVE (
		iCFContextManagerInterface->SubscribeContextL(subscription,
		this, thread) );
	
	EUNIT_ASSERT( !iSubscribersCalled );
	iSubscribersCalled = EFalse;
	iNoSubscribersCalled = EFalse;
	
	
	EUNIT_ASSERT_NO_LEAVE (
			iCFContextManagerInterface->RemoveSubscription( *subscription,
			*this) );

	EUNIT_ASSERT( !iNoSubscribersCalled );
	
	
	CleanupStack::PopAndDestroy( &thread );	
    }



//  TEST TABLE

EUNIT_BEGIN_TEST_TABLE(
    UT_CCFContextManager,
    "CCFContextManager class unit test.",
    "UNIT" )

EUNIT_TEST(
    "NewL - test ",
    "CCFContextManager",
    "NewL",
    "FUNCTIONALITY",
    SetupForNewL, UT_CCFContextManager_NewLL, TeardownForNewL)

EUNIT_TEST(
    "NewLC - test ",
    "CCFContextManager",
    "NewLC",
    "FUNCTIONALITY",
    SetupForNewL, UT_CCFContextManager_NewLCL, TeardownForNewL)

EUNIT_TEST(
    "PublishContextL - test ",
    "CCFContextManager",
    "PublishContextL",
    "FUNCTIONALITY",
    SetupL, UT_CCFContextManager_PublishContextLL, Teardown)

EUNIT_TEST(
    "RequestContextL - test ",
    "CCFContextManager",
    "RequestContextL",
    "FUNCTIONALITY",
    SetupL, UT_CCFContextManager_RequestContextL_2L, Teardown)

EUNIT_TEST(
    "DefineContextL - test ",
    "CCFContextManager",
    "DefineContextL",
    "FUNCTIONALITY",
    SetupL, UT_CCFContextManager_DefineContextLL, Teardown)
    
EUNIT_TEST(
    "DefineContextL2 - test ",
    "CCFContextManager",
    "DefineContextL2",
    "FUNCTIONALITY",
    SetupL, UT_CCFContextManager_DefineContextL2L, Teardown)

EUNIT_TEST(
    "RequestContextL - test ",
    "CCFContextManager",
    "RequestContextL",
    "FUNCTIONALITY",
    SetupL, UT_CCFContextManager_RequestContextL_3L, Teardown)

EUNIT_TEST(
    "AddSubscriptionL - test ",
    "CCFContextManager",
    "AddSubscriptionL",
    "FUNCTIONALITY",
    SetupL, UT_CCFContextManager_SubscribeContextLL, Teardown)

EUNIT_TEST(
    "RemoveSubscription - test ",
    "CCFContextManager",
    "RemoveSubscription",
    "FUNCTIONALITY",
    SetupL, UT_CCFContextManager_RemoveSubscriptionL, Teardown)

EUNIT_TEST(
    "RemoveSubscription2 - two identical subscriptions and removing the last one",
    "CCFContextManager",
    "RemoveSubscription",
    "FUNCTIONALITY",
    SetupL, UT_CCFContextManager_RemoveSubscription2L, Teardown)

EUNIT_TEST(
    "RemoveSubscriptions - via listener interface",
    "CCFContextManager",
    "RemoveSubscription",
    "FUNCTIONALITY",
    SetupL, UT_CCFContextManager_RemoveSubscriptionsL, Teardown)

EUNIT_TEST(
    "DeregisterPublisher - test ",
    "CCFContextManager",
    "DeregisterPublisher",
    "FUNCTIONALITY",
    SetupL, UT_CCFContextManager_DeregisterPublisherL, Teardown)
    

EUNIT_END_TEST_TABLE

//  END OF FILE

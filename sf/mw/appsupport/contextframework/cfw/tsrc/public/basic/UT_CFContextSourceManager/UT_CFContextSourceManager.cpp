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
#include "UT_CFContextSourceManager.h"
#include "testcontextsourcepluginconst.hrh"

//  EXTERNAL INCLUDES
#include <EUnitMacros.h>
#include <f32file.h>
#include <cfcontextsourcesettingsmanager.h>

//  INTERNAL INCLUDES
#include "CFContextSourcePlugIn.h"
#include "CFContextSourceManager.h"

_LIT_SECURITY_POLICY_PASS( KPassSec );

// CONSTRUCTION
UT_CFContextSourceManager* UT_CFContextSourceManager::NewL()
    {
    UT_CFContextSourceManager* self = UT_CFContextSourceManager::NewLC();
    CleanupStack::Pop();

    return self;
    }

UT_CFContextSourceManager* UT_CFContextSourceManager::NewLC()
    {
    UT_CFContextSourceManager* self = new( ELeave ) UT_CFContextSourceManager();
    CleanupStack::PushL( self );

    self->ConstructL();

    return self;
    }

// Destructor (virtual by CBase)
UT_CFContextSourceManager::~UT_CFContextSourceManager()
    {
    }

// Default constructor
UT_CFContextSourceManager::UT_CFContextSourceManager()
    {
    }

// Second phase construct
void UT_CFContextSourceManager::ConstructL()
    {
    // The ConstructL from the base class CEUnitTestSuiteClass must be called.
    // It generates the test case table.
    CEUnitTestSuiteClass::ConstructL();
    }

//  METHODS

void UT_CFContextSourceManager::UnsubscribeContexts(
    MCFContextSubscriptionListener& /*aObserver*/ )
    {
    
    }
    
TInt UT_CFContextSourceManager::PublishContext(
    CCFContextObject& /*aContext*/,
    RThread& /*aClientThread */)
    {
    return KErrNone;
    }
    
TInt UT_CFContextSourceManager::PublishContext(
    CCFContextObject& /*aContext*/,
    CCFContextDataObject& /*aData*/,
    RThread& /*aClientThread */)
    {
    return KErrNone;
    }
    
TInt UT_CFContextSourceManager::SubscribeContext(
    CCFContextSubscription* /*aSubscription*/,
    MCFContextSubscriptionListener* /*aListener*/,
    RThread& /*aClientThread */)
    {
    return KErrNone;
    }
    
void UT_CFContextSourceManager::UnsubscribeContext(
    CCFContextSubscription& /*aSubscription*/,
    MCFContextSubscriptionListener& /*aListener */)
    {
    
    }
    
TInt UT_CFContextSourceManager::RequestContext(
    RContextObjectArray& /*aResultBuffer*/,
    CCFContextQuery& /*aContextQuery*/,
    RThread& /*aClientThread */)
    {
    return KErrNone;
    }
    
TInt UT_CFContextSourceManager::DefineContext( const TDesC& /*aContextSource*/,
    const TDesC& /*aContextType*/,
    const TSecurityPolicy& /*aSecurityPolicy */)
    {
    return KErrNone;
    }

TInt UT_CFContextSourceManager::DefineContext( const TDesC& /*aContextSource*/,
    const TDesC& /*aContextType*/,
    const TSecurityPolicy& /*aReadWritePolicy*/,
    MCFContextSource& /*aOwner */)
    {
    return KErrNone;
    }

TInt UT_CFContextSourceManager::DefineContext( const TDesC& /*aContextSource*/,
    const TDesC& /*aContextType*/,
    const TSecurityPolicy& /*aReadPolicy*/,
    const TSecurityPolicy& /*aWritePolicy*/,
    MCFContextSource& /*aOwner */)
    {
    return KErrNone;
    }

TInt UT_CFContextSourceManager::GetSecurityPolicy( 
	const TDesC& /*aContextSource*/,
    const TDesC& /*aContextType*/,
	TSecurityPolicy& aPolicy )
	{
	aPolicy = KPassSec;	
	return KErrNone;	
	}

TInt UT_CFContextSourceManager::GetReadSecurityPolicy( const TDesC& /*aContextSource*/,
    const TDesC& /*aContextType*/,
    TSecurityPolicy& aPolicy )
    {
    aPolicy = KPassSec;
    return KErrNone;
    }

TInt UT_CFContextSourceManager::GetWriteSecurityPolicy( const TDesC& /*aContextSource*/,
    const TDesC& /*aContextType*/,
    TSecurityPolicy& aPolicy )
    {
    aPolicy = KPassSec;
    return KErrNone;
    }

TInt UT_CFContextSourceManager::DefineContext( const TDesC& /*aContextSource*/,
    const TDesC& /*aContextType*/,
    const TSecurityPolicy& /*aReadPolicy*/,
    const TSecurityPolicy& /*aWritePolicy*/,
    MCFContextSource* /*aPublisher*/,
    const TUid& /*aPublisherUid*/ )
    {
    return KErrNone;
    }

void UT_CFContextSourceManager::DeregisterPublisher( MCFContextSource& /*aPublisher*/ )
    {
    
    }

TInt UT_CFContextSourceManager::InstallContextSourceSetting( RFile& /*aSettingFile*/,
    const TUid& /*aContextSourceUid*/,
    RThread& /*aClientThread */)
    {
    return KErrNone;
    }

TInt UT_CFContextSourceManager::UninstallContextSourceSetting(
    const TDesC& /*aSettingFilename*/,
    const TUid& /*aContextSourceUid*/,
    RThread& /*aClientThread */)
    {
    return KErrNone;
    }

TInt UT_CFContextSourceManager::UninstallContextSourceSettings(
    const TUid& /*aContextSourceUid*/,
    RThread& /*aClientThread */)
    {
    return KErrNone;
    }

TInt UT_CFContextSourceManager::PublishContextFromAction(
    CCFContextObject& /*aContext*/,
    TBool /*aAutoDefine*/ )
    {
    return KErrNone;
    }

void UT_CFContextSourceManager::SetupL(  )
    {

    }

void UT_CFContextSourceManager::Teardown(  )
    {

    }
    
//-----------------------------------------------------------------------------
// CCFContextSourcePlugIn class unit tests
//-----------------------------------------------------------------------------
//
void UT_CFContextSourceManager::UT_CCFContextSourcePlugIn_NewLL(  )
    {
    TContextSourceInitParams params( *this );
    CCFContextSourcePlugIn* instance = NULL;
    EUNIT_ASSERT_LEAVE( instance = CCFContextSourcePlugIn::NewL( KNullUid, &params ) );
    EUNIT_ASSERT_DESC( !instance, "CCFContextSourcePlugIn instance created!" );
    }

void UT_CFContextSourceManager::UT_CCFContextSourcePlugIn_NewLCL(  )
    {
    TContextSourceInitParams params( *this );
    CCFContextSourcePlugIn* instance = NULL;
    EUNIT_ASSERT_LEAVE( instance = CCFContextSourcePlugIn::NewLC( KNullUid, &params ) );
    EUNIT_ASSERT_DESC( !instance, "CCFContextSourcePlugIn instance created!" );
    }

//-----------------------------------------------------------------------------
// CCFContextSourceManager class unit tests
//-----------------------------------------------------------------------------
//
void UT_CFContextSourceManager::UT_CCFContextSourceManager_NewLL(  )
    {
    RFs fs;
    User::LeaveIfError( fs.Connect() );
    CleanupClosePushL( fs );

    CCFContextSourceManager* instance =
        CCFContextSourceManager::NewL( *this, fs );
    CleanupStack::PushL( instance );

    EUNIT_ASSERT_DESC( instance, "CCFContextSourceManager instance not created!" );
    
    CleanupStack::PopAndDestroy( instance );
    CleanupStack::PopAndDestroy( &fs );
    }

void UT_CFContextSourceManager::UT_CCFContextSourceManager_NewLCL(  )
    {
    RFs fs;
    User::LeaveIfError( fs.Connect() );
    CleanupClosePushL( fs );

    CCFContextSourceManager* instance =
        CCFContextSourceManager::NewLC( *this, fs );

    EUNIT_ASSERT_DESC( instance, "CCFContextSourceManager instance not created!" );
    
    CleanupStack::PopAndDestroy( instance );
    CleanupStack::PopAndDestroy( &fs );
    }

//  TEST TABLE

EUNIT_BEGIN_TEST_TABLE(
    UT_CFContextSourceManager,
    "CFContextSourceManager unit test",
    "UNIT" )

EUNIT_TEST(
    "NewL - test ",
    "CCFContextSourcePlugIn",
    "NewL",
    "FUNCTIONALITY",
    SetupL, UT_CCFContextSourcePlugIn_NewLL, Teardown)

EUNIT_TEST(
    "NewLC - test ",
    "CCFContextSourcePlugIn",
    "NewLC",
    "FUNCTIONALITY",
    SetupL, UT_CCFContextSourcePlugIn_NewLCL, Teardown)

EUNIT_TEST(
    "NewL - test ",
    "CCFContextSourceManager",
    "NewL",
    "FUNCTIONALITY",
    SetupL, UT_CCFContextSourceManager_NewLL, Teardown)

EUNIT_TEST(
    "NewLC - test ",
    "CCFContextSourceManager",
    "NewLC",
    "FUNCTIONALITY",
    SetupL, UT_CCFContextSourceManager_NewLCL, Teardown)


EUNIT_END_TEST_TABLE

//  END OF FILE

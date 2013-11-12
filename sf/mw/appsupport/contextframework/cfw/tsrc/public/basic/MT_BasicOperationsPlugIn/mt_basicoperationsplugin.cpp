/*
* Copyright (c) 2002-2010 Nokia Corporation and/or its subsidiary(-ies). 
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
#include "mt_basicoperationsplugin.h"

//  EXTERNAL INCLUDES
#include <EUnitMacros.h>
#include <EUnitDecorators.h>
#include <ECom/ECom.h>

#include <bautils.h>

#include "CFContextSubscription.h"
#include "CFContextSubscriptionImpl.h"
#include "CFContextObject.h"
#include "CFContextObjectImpl.h"
#include "CFScriptHandler.h"

//  INTERNAL INCLUDES
#include "CFTestDelay.h"
#include "cffakeenv.h"
#include "ScriptEventNotifierSession.h"

// CONSTANTS

const TUid KMT_CfScriptEngineUid = {0x7E577E57};
//const TUid KMT_CfScriptEngineOtherUid = {0x074E074E};
_LIT8( KScriptStartTag, "<script" );


// CONSTRUCTION
mt_basicoperationsplugin* mt_basicoperationsplugin::NewL()
    {
    mt_basicoperationsplugin* self = mt_basicoperationsplugin::NewLC();
    CleanupStack::Pop();

    return self;
    }

mt_basicoperationsplugin* mt_basicoperationsplugin::NewLC()
    {
    mt_basicoperationsplugin* self = new( ELeave ) mt_basicoperationsplugin();
    CleanupStack::PushL( self );

    self->ConstructL();

    return self;
    }

// Destructor (virtual by CBase)
mt_basicoperationsplugin::~mt_basicoperationsplugin()
    {
    }

// Default constructor
mt_basicoperationsplugin::mt_basicoperationsplugin()
    {
    }

// Second phase construct
void mt_basicoperationsplugin::ConstructL()
    {
    // The ConstructL from the base class CEUnitTestSuiteClass must be called.
    // It generates the test case table.
    CEUnitTestSuiteClass::ConstructL();
    }

//  METHODS


void mt_basicoperationsplugin::SetupEmptyL(  )
    {
    User::LeaveIfError( iFs.Connect() );
    iTestDelay = CCFTestDelay::NewL();
    }

void mt_basicoperationsplugin::SetupL(  )
    {
    SetupEmptyL();

    iDummyScriptEngineSession = CScriptEventNotifierSession::NewL();

    iCFFakeEnv = CCFFakeEnv::NewL();
    }

void mt_basicoperationsplugin::SetupMidtestL( CCFContextObject& aContext1,
    CCFContextObject& aContext2,
    const TDesC& aScriptFile )
    {
    iDummyScriptEngineSession = CScriptEventNotifierSession::NewL();

    iCFFakeEnv = CCFFakeEnv::NewL();

    RThread thread;
    CleanupClosePushL( thread );

    // Define contexts
    static _LIT_SECURITY_POLICY_PASS( alwaysPass );
    TInt err = iCFFakeEnv->DefineContext( aContext1.Source(),
            aContext1.Type(), alwaysPass, alwaysPass );
    EUNIT_ASSERT_DESC( err == KErrNone, "DefineContext failed!" );

    err = iCFFakeEnv->DefineContext( aContext2.Source(),
            aContext2.Type(), alwaysPass, alwaysPass );
    EUNIT_ASSERT_DESC( err == KErrNone, "DefineContext failed!" );

    // Publish context before loading so that subscribing will find the initial
    // value used to evaluate the operation
    aContext1.SetValueL(_L("A"));
    err = iCFFakeEnv->PublishContext( aContext1, thread );
    EUNIT_ASSERT_DESC( err == KErrNone, "PublishContext failed!" );
    EUNIT_ASSERT_DESC( !iCFFakeEnv->ClientEventFired(),
        "Script did cause event!" );

    aContext2.SetValueL(_L("True"));
    err = iCFFakeEnv->PublishContext( aContext2, thread );
    EUNIT_ASSERT_DESC( err == KErrNone, "PublishContext failed!" );
    EUNIT_ASSERT_DESC( !iCFFakeEnv->ClientEventFired(),
        "Script did cause event!" );

    // load script
    HBufC8* script = LoadScriptFromFile( aScriptFile );
    TInt id = iCFFakeEnv->AddScript( aScriptFile, *script,
            KMT_CfScriptEngineUid, thread );
    EUNIT_ASSERT_DESC( id >= 0, "Parse error!" );
    delete script;

    iCFFakeEnv->ClearFireClientEventCount();

    CleanupStack::PopAndDestroy( &thread );
    }

void mt_basicoperationsplugin::TeardownMidtest(  )
    {
    iTestDelay->Start(100000); // 0.1 seconds

    delete iDummyScriptEngineSession;
    iDummyScriptEngineSession = NULL;

    delete iCFFakeEnv;
    iCFFakeEnv = NULL;

#ifdef __WINS__    
    REComSession::FinalClose();
#endif
    }

void mt_basicoperationsplugin::Teardown(  )
    {
    iTestDelay->Start(100000); // 0.1 seconds

    delete iDummyScriptEngineSession;
    iDummyScriptEngineSession = NULL;

    iTestDelay->Start(100000); // 0.1 seconds
    delete iTestDelay;
    iTestDelay = NULL;

    delete iCFFakeEnv;
    iCFFakeEnv = NULL;

    iFs.Close();

#ifdef __WINS__    
    REComSession::FinalClose();
#endif
    }

// -----------------------------------------------------------------------------
// mt_basicoperationsplugin::LoadScriptFromFile
// -----------------------------------------------------------------------------
//
HBufC8* mt_basicoperationsplugin::LoadScriptFromFile( const TDesC& aFilePath )
    {
    HBufC8* script = NULL;
    RFile file;
    TFileName fileName;
    fileName.Copy(_L("z:\\data\\cftestdata\\testscripts\\"));
    fileName.Append(aFilePath);
    TInt err = file.Open( iFs, fileName, EFileRead );
    if( err == KErrNone )
        {
        TInt size = 0;
        err = file.Size( size );
        if( err == KErrNone )
            {
            script = HBufC8::New( size );
            if( script )
                {
                TPtr8 scriptPtr = script->Des();
                err = file.Read( scriptPtr );
                if( err == KErrNone )
                    {
                    // Strip all unnecassary data from script
                    TInt pos = scriptPtr.FindF( KScriptStartTag );
                    if( pos != KErrNotFound )
                        {
                        scriptPtr.Copy( scriptPtr.MidTPtr( pos ) );
                        }
                    else
                        {
                        // Incorrect script
                        delete script;
                        script = NULL;
                        }
                    }
                else
                    {
                    delete script;
                    script = NULL;
                    }
                }
            }
        }

    // Cleanup
    file.Close();
    return script;
    }

// Format string to create persistent data file from:
// - Owner uid
// - Script name
// - Persistent data name
_LIT( KCFPersistenDataFormat, "c:\\private\\10282bc4\\rules\\%x\\%S-%S.dat" );

void mt_basicoperationsplugin::DeletePersistencyFile( const TDesC& aScriptName,
    const TDesC& aPersistencyFilename )
    {
    // Format name
    TParsePtrC parse( aScriptName );
    TPtrC scriptFilename( parse.Name() );
    TParsePtrC parsePersistency( aPersistencyFilename );
    TPtrC persistencyFilename( parsePersistency.Name() );

    TFileName fileName;
    fileName.Format( KCFPersistenDataFormat,
            KMT_CfScriptEngineUid, &scriptFilename, &persistencyFilename );

    TInt err = BaflUtils::DeleteFile( iFs, fileName );
    EUNIT_ASSERT_DESC(
            err == KErrNone || err == KErrNotFound || err == KErrPathNotFound, 
            "Persistency file delete error!");
    }

TInt mt_basicoperationsplugin::FindParameter(const RKeyValueArray& aArray,
    const TDesC& aKey, const TDesC& aValue)
    {
    TInt index = KErrNotFound;
    TBool found = EFalse;
    for (TInt i = 0; i < aArray.Count(); i++)
        {
        if ( aArray[i]->Key() == aKey )
            {
            if ( index == KErrNotFound )
                {
                index = i;
                if ( aArray[i]->Value() == aValue )
                    {
                    found = ETrue;
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
    if (found)
        {
        return index;
        }
    return KErrNotFound;
    }


TInt mt_basicoperationsplugin::FindIndex( const RArray<TInt>& aArray, TInt aIndex )
    {
    TInt index = KErrNotFound;
    TBool found = EFalse;
    for (TInt i = 0; i < aArray.Count(); i++)
        {
        if ( aArray[i] == aIndex )
            {
            if ( index == KErrNotFound )
                {
                index = i;
                found = ETrue;
                }
            else
                {
                index = -2; // Duplicate found!
                }
            }
        }
    if (found)
        {
        return index;
        }
    return KErrNotFound;
    }

    



void mt_basicoperationsplugin::MT_ScriptTest_equals_L(  )
    {
    RThread thread;
    CleanupClosePushL(thread);

    TInt id = iCFFakeEnv->AddScript(_L("dummyscript"),
            _L8("dummyscript"), KMT_CfScriptEngineUid, thread);
    EUNIT_ASSERT_DESC( id < 0, "No parse error of dummy script" );

    // Define and publish context
    CCFContextObject* co = CCFContextObject::NewLC();
    static _LIT_SECURITY_POLICY_PASS( alwaysPass );
    co->SetSourceL(_L("t"));
    co->SetTypeL(_L("t.p.app"));
    TInt err = iCFFakeEnv->DefineContext( co->Source(),
            co->Type(), alwaysPass, alwaysPass );
    EUNIT_ASSERT_DESC( err == KErrNone, "DefineContext failed!" );

    iCFFakeEnv->ClearFireClientEvent();
    // load script
    HBufC8* script = LoadScriptFromFile(_L("test-equals.xml"));
    id = iCFFakeEnv->AddScript(
            _L("test-equals.xml"), *script, KMT_CfScriptEngineUid, thread);
    EUNIT_ASSERT_DESC( id >= 0, "Parse error!" );
    delete script;

    // publish context
    co->SetValueL(_L("ok"));
    err = iCFFakeEnv->PublishContext( *co, thread );
    EUNIT_ASSERT_DESC( err == KErrNone, "PublishContext failed!" );
    EUNIT_ASSERT_DESC( iCFFakeEnv->ClientEventFired(), "Script did not fire event!" );
    EUNIT_ASSERT_DESC( iCFFakeEnv->ScriptEventScriptId() == id,
         "Script ID does not match!" );
    EUNIT_ASSERT_DESC( iCFFakeEnv->ScriptEventIdentifier() == _L("startApp"),
        "Identifier does not match!" );

    // Check parameters:
    // appUid='10005901' viewUid='00000001' msgUid='00000000' msg='msg'
    EUNIT_ASSERT(iCFFakeEnv->ScriptEventParameters().Count() == 4);
    EUNIT_ASSERT( FindParameter(iCFFakeEnv->ScriptEventParameters(),
        _L("appUid"), _L("10005901")) >= 0 );
    EUNIT_ASSERT( FindParameter(iCFFakeEnv->ScriptEventParameters(),
        _L("viewUid"), _L("00000001")) >= 0 );
    EUNIT_ASSERT( FindParameter(iCFFakeEnv->ScriptEventParameters(),
        _L("msgUid"), _L("00000000")) >= 0 );
    EUNIT_ASSERT( FindParameter(iCFFakeEnv->ScriptEventParameters(),
        _L("msg"), _L("msg")) >= 0 );

    iCFFakeEnv->ClearFireClientEvent();
    err = iCFFakeEnv->PublishContext( *co, thread );
    EUNIT_ASSERT_DESC( err == KErrNone, "PublishContext failed!" );
    EUNIT_ASSERT_DESC( !iCFFakeEnv->ClientEventFired(),
         "Script did fired although context value did not change!" );

    co->SetValueL(_L("notok"));
    iCFFakeEnv->ClearFireClientEvent();
    err = iCFFakeEnv->PublishContext( *co, thread );
    EUNIT_ASSERT_DESC( err == KErrNone, "PublishContext failed!" );
    EUNIT_ASSERT_DESC( !iCFFakeEnv->ClientEventFired(),
         "Script did fired although wrong context value!" );

    co->SetValueL(_L("ok"));
    iCFFakeEnv->ClearFireClientEvent();
    err = iCFFakeEnv->PublishContext( *co, thread );
    EUNIT_ASSERT_DESC( err == KErrNone, "PublishContext failed!" );
    EUNIT_ASSERT_DESC( iCFFakeEnv->ClientEventFired(),
         "Script did not fire event!" );

    CleanupStack::PopAndDestroy( co );
    CleanupStack::PopAndDestroy( &thread );
    }


void mt_basicoperationsplugin::MT_ScriptTest_notEqual_L(  )
    {
    RThread thread;
    CleanupClosePushL(thread);

    // Define context
    CCFContextObject* co = CCFContextObject::NewLC();
    static _LIT_SECURITY_POLICY_PASS( alwaysPass );
    co->SetSourceL(_L("t"));
    co->SetTypeL(_L("t.p.app"));
    TInt err = iCFFakeEnv->DefineContext( co->Source(),
            co->Type(), alwaysPass, alwaysPass );
    EUNIT_ASSERT_DESC( err == KErrNone, "DefineContext failed!" );

    // load script
    HBufC8* script = LoadScriptFromFile(_L("test-notEqual.xml"));
    TInt id = iCFFakeEnv->AddScript(
            _L("test-notEqual.xml"), *script, KMT_CfScriptEngineUid, thread);
    EUNIT_ASSERT_DESC( id >= 0, "Parse error!" );
    delete script;

    // TEST 1: Should fire event if not ok value
    co->SetValueL(_L("notok"));
    err = iCFFakeEnv->PublishContext( *co, thread );
    EUNIT_ASSERT_DESC( err == KErrNone, "PublishContext failed!" );
    EUNIT_ASSERT_DESC( iCFFakeEnv->ClientEventFired(), "Script did not fire event!" );
    EUNIT_ASSERT_DESC( iCFFakeEnv->ScriptEventScriptId() == id,
         "Script ID does not match!" );
    EUNIT_ASSERT_DESC( iCFFakeEnv->ScriptEventIdentifier() == _L("startApp"),
        "Identifier does not match!" );

    // TEST 2: Should not fire event if the context value did not change
    iCFFakeEnv->ClearFireClientEvent();
    err = iCFFakeEnv->PublishContext( *co, thread );
    EUNIT_ASSERT_DESC( err == KErrNone, "PublishContext failed!" );
    EUNIT_ASSERT_DESC( !iCFFakeEnv->ClientEventFired(),
         "Script did fired although context value did not change!" );

    // TEST 3: Should not fire event if context value ok
    co->SetValueL(_L("ok"));
    iCFFakeEnv->ClearFireClientEvent();
    err = iCFFakeEnv->PublishContext( *co, thread );
    EUNIT_ASSERT_DESC( err == KErrNone, "PublishContext failed!" );
    EUNIT_ASSERT_DESC( !iCFFakeEnv->ClientEventFired(),
         "Script did fired although wrong context value!" );

    // TEST 4: Should fire event if context value not ok
    co->SetValueL(_L("notok"));
    iCFFakeEnv->ClearFireClientEvent();
    err = iCFFakeEnv->PublishContext( *co, thread );
    EUNIT_ASSERT_DESC( err == KErrNone, "PublishContext failed!" );
    EUNIT_ASSERT_DESC( iCFFakeEnv->ClientEventFired(),
         "Script did not fire event!" );
    err = iCFFakeEnv->PublishContext( *co, thread );
    EUNIT_ASSERT_DESC( err == KErrNone, "PublishContext failed!" );

    CleanupStack::PopAndDestroy( co );
    CleanupStack::PopAndDestroy( &thread );
    }


void mt_basicoperationsplugin::MT_ScriptTest_less_L(  )
    {
    RThread thread;
    CleanupClosePushL(thread);

    // Define context
    CCFContextObject* co = CCFContextObject::NewLC();
    static _LIT_SECURITY_POLICY_PASS( alwaysPass );
    co->SetSourceL(_L("Sensor"));
    co->SetTypeL(_L("X.RawValue"));
    TInt err = iCFFakeEnv->DefineContext( co->Source(),
            co->Type(), alwaysPass, alwaysPass );
    EUNIT_ASSERT_DESC( err == KErrNone, "DefineContext failed!" );

    HBufC8* script = LoadScriptFromFile(_L("test-less.xml"));
    TInt id = iCFFakeEnv->AddScript(
            _L("test-less.xml"), *script, KMT_CfScriptEngineUid, thread);
    EUNIT_ASSERT_DESC( id >= 0, "Parse error!" );
    delete script;

    // TEST 1: Should fire event if value < 150
    co->SetValueL(_L("149")); // value in script: 150
    err = iCFFakeEnv->PublishContext( *co, thread );
    EUNIT_ASSERT_DESC( err == KErrNone, "PublishContext failed!" );
    EUNIT_ASSERT_DESC( iCFFakeEnv->ClientEventFired(), "Script did not fire event!" );
    EUNIT_ASSERT_DESC( iCFFakeEnv->ScriptEventScriptId() == id,
         "Script ID does not match!" );
    EUNIT_ASSERT_DESC( iCFFakeEnv->ScriptEventIdentifier() == _L("startApp"),
        "Identifier does not match!" );

    // TEST 2: Should not fire event if the context value did not change
    iCFFakeEnv->ClearFireClientEvent();
    err = iCFFakeEnv->PublishContext( *co, thread );
    EUNIT_ASSERT_DESC( err == KErrNone, "PublishContext failed!" );
    EUNIT_ASSERT_DESC( !iCFFakeEnv->ClientEventFired(),
         "Script did fired although context value did not change!" );

    // TEST 3: Should not fire event if context value = 150
    co->SetValueL(_L("150"));
    iCFFakeEnv->ClearFireClientEvent();
    err = iCFFakeEnv->PublishContext( *co, thread );
    EUNIT_ASSERT_DESC( err == KErrNone, "PublishContext failed!" );
    EUNIT_ASSERT_DESC( !iCFFakeEnv->ClientEventFired(),
         "Script did fired although wrong context value!" );

    // TEST 4: Should fire event if context value < 150
    co->SetValueL(_L("-1"));
    iCFFakeEnv->ClearFireClientEvent();
    err = iCFFakeEnv->PublishContext( *co, thread );
    EUNIT_ASSERT_DESC( err == KErrNone, "PublishContext failed!" );
    EUNIT_ASSERT_DESC( iCFFakeEnv->ClientEventFired(),
         "Script did not fire event!" );

    CleanupStack::PopAndDestroy( co );
    CleanupStack::PopAndDestroy( &thread );
    }


void mt_basicoperationsplugin::MT_ScriptTest_greater_L(  )
    {
    RThread thread;
    CleanupClosePushL(thread);

    // Define context
    static _LIT_SECURITY_POLICY_PASS( alwaysPass );
    CCFContextObject* co = CCFContextObject::NewLC();
    co->SetSourceL(_L("Sensor"));
    co->SetTypeL(_L("X.RawValue"));
    TInt err = iCFFakeEnv->DefineContext( co->Source(),
            co->Type(), alwaysPass, alwaysPass );
    EUNIT_ASSERT_DESC( err == KErrNone, "DefineContext failed!" );

    HBufC8* script = LoadScriptFromFile(_L("test-greater.xml"));
    TInt id = iCFFakeEnv->AddScript(
            _L("test-greater.xml"), *script, KMT_CfScriptEngineUid, thread);
    EUNIT_ASSERT_DESC( id >= 0, "Parse error!" );
    delete script;

    // TEST 1: Should fire event if value > 149
    co->SetValueL(_L("150")); // value in script: 149
    err = iCFFakeEnv->PublishContext( *co, thread );
    EUNIT_ASSERT_DESC( err == KErrNone, "PublishContext failed!" );
    EUNIT_ASSERT_DESC( iCFFakeEnv->ClientEventFired(), "Script did not fire event!" );
    EUNIT_ASSERT_DESC( iCFFakeEnv->ScriptEventScriptId() == id,
         "Script ID does not match!" );
    EUNIT_ASSERT_DESC( iCFFakeEnv->ScriptEventIdentifier() == _L("startApp"),
        "Identifier does not match!" );

    // TEST 2: Should not fire event if the context value did not change
    iCFFakeEnv->ClearFireClientEvent();
    err = iCFFakeEnv->PublishContext( *co, thread );
    EUNIT_ASSERT_DESC( err == KErrNone, "PublishContext failed!" );
    EUNIT_ASSERT_DESC( !iCFFakeEnv->ClientEventFired(),
         "Script did fired although context value did not change!" );

    // TEST 3: Should not fire event if context value = 149
    co->SetValueL(_L("149"));
    iCFFakeEnv->ClearFireClientEvent();
    err = iCFFakeEnv->PublishContext( *co, thread );
    EUNIT_ASSERT_DESC( err == KErrNone, "PublishContext failed!" );
    EUNIT_ASSERT_DESC( !iCFFakeEnv->ClientEventFired(),
         "Script did fired although wrong context value!" );

    // TEST 4: Should fire event if context value > 149
    co->SetValueL(_L("17400"));
    iCFFakeEnv->ClearFireClientEvent();
    err = iCFFakeEnv->PublishContext( *co, thread );
    EUNIT_ASSERT_DESC( err == KErrNone, "PublishContext failed!" );
    EUNIT_ASSERT_DESC( iCFFakeEnv->ClientEventFired(),
         "Script did not fire event!" );

    CleanupStack::PopAndDestroy( co );
    CleanupStack::PopAndDestroy( &thread );
    }

void mt_basicoperationsplugin::MT_ScriptTest_lessOrEqual_L(  )
    {
    RThread thread;
    CleanupClosePushL(thread);

    // Define and publish context
    CCFContextObject* co = CCFContextObject::NewLC();
    static _LIT_SECURITY_POLICY_PASS( alwaysPass );

    co->SetSourceL(_L("Sensor"));
    co->SetTypeL(_L("Y.RawValue"));
    TInt err = iCFFakeEnv->DefineContext( co->Source(),
            co->Type(), alwaysPass, alwaysPass );
    EUNIT_ASSERT_DESC( err == KErrNone, "DefineContext failed!" );

    HBufC8* script = LoadScriptFromFile(_L("test-lessOrEqual.xml"));
    TInt id = iCFFakeEnv->AddScript(
            _L("test-lessOrEqual.xml"), *script, KMT_CfScriptEngineUid, thread);
    EUNIT_ASSERT_DESC( id >= 0, "Parse error!" );
    delete script;

    // TEST 1: Should fire event if value <= 150
    co->SetValueL(_L("150")); // value in script: 150
    err = iCFFakeEnv->PublishContext( *co, thread );
    EUNIT_ASSERT_DESC( err == KErrNone, "PublishContext failed!" );
    EUNIT_ASSERT_DESC( iCFFakeEnv->ClientEventFired(), "Script did not fire event!" );
    EUNIT_ASSERT_DESC( iCFFakeEnv->ScriptEventScriptId() == id,
         "Script ID does not match!" );
    EUNIT_ASSERT_DESC( iCFFakeEnv->ScriptEventIdentifier() == _L("startApp"),
        "Identifier does not match!" );

    // TEST 2: Should not fire event if the context value did not change
    iCFFakeEnv->ClearFireClientEvent();
    err = iCFFakeEnv->PublishContext( *co, thread );
    EUNIT_ASSERT_DESC( err == KErrNone, "PublishContext failed!" );
    EUNIT_ASSERT_DESC( !iCFFakeEnv->ClientEventFired(),
         "Script did fired although context value did not change!" );

    // TEST 3: Should not fire event if context value > 150
    co->SetValueL(_L("151"));
    iCFFakeEnv->ClearFireClientEvent();
    err = iCFFakeEnv->PublishContext( *co, thread );
    EUNIT_ASSERT_DESC( err == KErrNone, "PublishContext failed!" );
    EUNIT_ASSERT_DESC( !iCFFakeEnv->ClientEventFired(),
         "Script did fired although wrong context value!" );

    // TEST 4: Should fire event if context value <= 150
    co->SetValueL(_L("-378"));
    iCFFakeEnv->ClearFireClientEvent();
    err = iCFFakeEnv->PublishContext( *co, thread );
    EUNIT_ASSERT_DESC( err == KErrNone, "PublishContext failed!" );
    EUNIT_ASSERT_DESC( iCFFakeEnv->ClientEventFired(),
         "Script did not fire event!" );

    CleanupStack::PopAndDestroy( co );
    CleanupStack::PopAndDestroy( &thread );
    }


void mt_basicoperationsplugin::MT_ScriptTest_greaterOrEqual_L(  )
    {
    RThread thread;
    CleanupClosePushL(thread);

    // Define context
    CCFContextObject* co = CCFContextObject::NewLC();
    static _LIT_SECURITY_POLICY_PASS( alwaysPass );

    co->SetSourceL(_L("Sensor"));
    co->SetTypeL(_L("Y.RawValue"));
    TInt err = iCFFakeEnv->DefineContext( co->Source(),
            co->Type(), alwaysPass, alwaysPass );
    EUNIT_ASSERT_DESC( err == KErrNone, "DefineContext failed!" );

    HBufC8* script = LoadScriptFromFile(_L("test-greaterOrEqual.xml"));
    TInt id = iCFFakeEnv->AddScript(_L("test-greaterOrEqual.xml"),
            *script, KMT_CfScriptEngineUid, thread);
    EUNIT_ASSERT_DESC( id >= 0, "Parse error!" );
    delete script;

    // TEST 1: Should fire event if value >= 151
    co->SetValueL(_L("151")); // value in script: 151
    err = iCFFakeEnv->PublishContext( *co, thread );
    EUNIT_ASSERT_DESC( err == KErrNone, "PublishContext failed!" );
    EUNIT_ASSERT_DESC( iCFFakeEnv->ClientEventFired(), "Script did not fire event!" );
    EUNIT_ASSERT_DESC( iCFFakeEnv->ScriptEventScriptId() == id,
         "Script ID does not match!" );
    EUNIT_ASSERT_DESC( iCFFakeEnv->ScriptEventIdentifier() == _L("startApp"),
        "Identifier does not match!" );

    // TEST 2: Should not fire event if the context value did not change
    iCFFakeEnv->ClearFireClientEvent();
    err = iCFFakeEnv->PublishContext( *co, thread );
    EUNIT_ASSERT_DESC( err == KErrNone, "PublishContext failed!" );
    EUNIT_ASSERT_DESC( !iCFFakeEnv->ClientEventFired(),
         "Script did fired although context value did not change!" );

    // TEST 3: Should not fire event if context value < 151
    co->SetValueL(_L("150"));
    iCFFakeEnv->ClearFireClientEvent();
    err = iCFFakeEnv->PublishContext( *co, thread );
    EUNIT_ASSERT_DESC( err == KErrNone, "PublishContext failed!" );
    EUNIT_ASSERT_DESC( !iCFFakeEnv->ClientEventFired(),
         "Script did fired although wrong context value!" );

    // TEST 4: Should fire event if context value >= 150
    co->SetValueL(_L("23689"));
    iCFFakeEnv->ClearFireClientEvent();
    err = iCFFakeEnv->PublishContext( *co, thread );
    EUNIT_ASSERT_DESC( err == KErrNone, "PublishContext failed!" );
    EUNIT_ASSERT_DESC( iCFFakeEnv->ClientEventFired(),
         "Script did not fire event!" );

    CleanupStack::PopAndDestroy( co );
    CleanupStack::PopAndDestroy( &thread );
    }


void mt_basicoperationsplugin::MT_ScriptTest_contains_L(  )
    {
    RThread thread;
    CleanupClosePushL(thread);

    TInt id = iCFFakeEnv->AddScript(_L("dummyscript"), 
            _L8("dummyscript"), KMT_CfScriptEngineUid, thread);
    EUNIT_ASSERT_DESC( id < 0, "No parse error of dummy script" );

    // Define and publish context
    CCFContextObject* co = CCFContextObject::NewLC();
    static _LIT_SECURITY_POLICY_PASS( alwaysPass );
    co->SetSourceL(_L("t"));
    co->SetTypeL(_L("t.p.app"));
    TInt err = iCFFakeEnv->DefineContext( co->Source(),
            co->Type(), alwaysPass, alwaysPass );
    EUNIT_ASSERT_DESC( err == KErrNone, "DefineContext failed!" );

    iCFFakeEnv->ClearFireClientEvent();

    // load script
    HBufC8* script = LoadScriptFromFile(_L("test-contains.xml"));
    id = iCFFakeEnv->AddScript(
            _L("test-contains.xml"), *script, KMT_CfScriptEngineUid, thread);
    EUNIT_ASSERT_DESC( id >= 0, "Parse error!" );
    delete script;

    // publish context
    co->SetValueL(_L("sisaltaa"));
    err = iCFFakeEnv->PublishContext( *co, thread );
    EUNIT_ASSERT_DESC( err == KErrNone, "PublishContext failed!" );
    EUNIT_ASSERT_DESC( iCFFakeEnv->ClientEventFired(), "Script did not fire event!" );
    EUNIT_ASSERT_DESC( iCFFakeEnv->ScriptEventScriptId() == id,
         "Script ID does not match!" );
    EUNIT_ASSERT_DESC( iCFFakeEnv->ScriptEventIdentifier() == _L("startApp"),
        "Identifier does not match!" );

    // Check parameters:
    // appUid='10005901' viewUid='00000001' msgUid='00000000' msg='msg'
    EUNIT_ASSERT(iCFFakeEnv->ScriptEventParameters().Count() == 4);
    EUNIT_ASSERT( FindParameter(iCFFakeEnv->ScriptEventParameters(),
        _L("appUid"), _L("10005901")) >= 0 );
    EUNIT_ASSERT( FindParameter(iCFFakeEnv->ScriptEventParameters(),
        _L("viewUid"), _L("00000001")) >= 0 );
    EUNIT_ASSERT( FindParameter(iCFFakeEnv->ScriptEventParameters(),
        _L("msgUid"), _L("00000000")) >= 0 );
    EUNIT_ASSERT( FindParameter(iCFFakeEnv->ScriptEventParameters(),
        _L("msg"), _L("msg")) >= 0 );

    iCFFakeEnv->ClearFireClientEvent();
    err = iCFFakeEnv->PublishContext( *co, thread );
    EUNIT_ASSERT_DESC( err == KErrNone, "PublishContext failed!" );
    EUNIT_ASSERT_DESC( !iCFFakeEnv->ClientEventFired(),
         "Script did fired although context value did not change!" );

    co->SetValueL(_L("sisalt"));
    iCFFakeEnv->ClearFireClientEvent();
    err = iCFFakeEnv->PublishContext( *co, thread );
    EUNIT_ASSERT_DESC( err == KErrNone, "PublishContext failed!" );
    EUNIT_ASSERT_DESC( !iCFFakeEnv->ClientEventFired(),
         "Script did fired although wrong context value!" );

    co->SetValueL(_L("isalta"));
    iCFFakeEnv->ClearFireClientEvent();
    err = iCFFakeEnv->PublishContext( *co, thread );
    EUNIT_ASSERT_DESC( err == KErrNone, "PublishContext failed!" );
    EUNIT_ASSERT_DESC( iCFFakeEnv->ClientEventFired(),
         "Script did not fire event!" );

    CleanupStack::PopAndDestroy( co );
    CleanupStack::PopAndDestroy( &thread );
    }

void mt_basicoperationsplugin::MT_ScriptTest_inRange_L(  )
    {
    RThread thread;
    CleanupClosePushL(thread);

    // Define context
    CCFContextObject* co = CCFContextObject::NewLC();
    static _LIT_SECURITY_POLICY_PASS( alwaysPass );
    co->SetSourceL(_L("Sensor"));
    co->SetTypeL(_L("Z.RawValue"));
    TInt err = iCFFakeEnv->DefineContext( co->Source(),
            co->Type(), alwaysPass, alwaysPass );
    EUNIT_ASSERT_DESC( err == KErrNone, "DefineContext failed!" );

    // load script
    HBufC8* script = LoadScriptFromFile(_L("test-inRange.xml"));
    TInt id = iCFFakeEnv->AddScript(
            _L("test-inRange.xml"), *script, KMT_CfScriptEngineUid, thread);
    EUNIT_ASSERT_DESC( id >= 0, "Parse error!" );
    delete script;

    // TEST 1: Should fire event if value 100-200
    co->SetValueL(_L("100")); // range in script: 100-200
    err = iCFFakeEnv->PublishContext( *co, thread );
    EUNIT_ASSERT_DESC( err == KErrNone, "PublishContext failed!" );
    EUNIT_ASSERT_DESC( iCFFakeEnv->ClientEventFired(), "Script did not fire event!" );
    EUNIT_ASSERT_DESC( iCFFakeEnv->ScriptEventScriptId() == id,
         "Script ID does not match!" );
    EUNIT_ASSERT_DESC( iCFFakeEnv->ScriptEventIdentifier() == _L("startApp"),
        "Identifier does not match!" );

    // TEST 2: Should not fire event if the context value did not change
    iCFFakeEnv->ClearFireClientEvent();
    err = iCFFakeEnv->PublishContext( *co, thread );
    EUNIT_ASSERT_DESC( err == KErrNone, "PublishContext failed!" );
    EUNIT_ASSERT_DESC( !iCFFakeEnv->ClientEventFired(),
         "Script did fired although context value did not change!" );

    // TEST 3: Should not fire event if context value is outside 100-200
    co->SetValueL(_L("99"));
    iCFFakeEnv->ClearFireClientEvent();
    err = iCFFakeEnv->PublishContext( *co, thread );
    EUNIT_ASSERT_DESC( err == KErrNone, "PublishContext failed!" );
    EUNIT_ASSERT_DESC( !iCFFakeEnv->ClientEventFired(),
         "Script did fired although wrong context value!" );

    // TEST 3: Should not fire event if context value is outside 100-200
    co->SetValueL(_L("201"));
    iCFFakeEnv->ClearFireClientEvent();
    err = iCFFakeEnv->PublishContext( *co, thread );
    EUNIT_ASSERT_DESC( err == KErrNone, "PublishContext failed!" );
    EUNIT_ASSERT_DESC( !iCFFakeEnv->ClientEventFired(),
         "Script did fired although wrong context value!" );

    // TEST 4: Should fire event if context value in range 100-200
    co->SetValueL(_L("200"));
    iCFFakeEnv->ClearFireClientEvent();
    err = iCFFakeEnv->PublishContext( *co, thread );
    EUNIT_ASSERT_DESC( err == KErrNone, "PublishContext failed!" );
    EUNIT_ASSERT_DESC( iCFFakeEnv->ClientEventFired(),
         "Script did not fire event!" );

    CleanupStack::PopAndDestroy( co );
    CleanupStack::PopAndDestroy( &thread );
    }

void mt_basicoperationsplugin::MT_ScriptTest_contextChanged_L(  )
    {
    RThread thread;
    CleanupClosePushL(thread);

    // Define context
    CCFContextObject* co = CCFContextObject::NewLC();
    static _LIT_SECURITY_POLICY_PASS( alwaysPass );
    co->SetSourceL(_L("Application.State"));
    co->SetTypeL(_L("Foreground.View"));
    TInt err = iCFFakeEnv->DefineContext( co->Source(),
            co->Type(), alwaysPass, alwaysPass );
    EUNIT_ASSERT_DESC( err == KErrNone, "DefineContext failed!" );

    // load script
    HBufC8* script = LoadScriptFromFile(_L("test-contextChanged.xml"));
    TInt id = iCFFakeEnv->AddScript(_L("test-contextChanged.xml"),
            *script, KMT_CfScriptEngineUid, thread);
    EUNIT_ASSERT_DESC( id >= 0, "Parse error!" );
    delete script;

    iCFFakeEnv->ClearFireClientEventCount();

    // TEST 1: Context Changed, should fire event
    co->SetValueL(_L("WebBrowser"));
    iCFFakeEnv->ClearFireClientEvent();
    err = iCFFakeEnv->PublishContext( *co, thread );
    EUNIT_ASSERT_DESC( err == KErrNone, "PublishContext failed!" );
    EUNIT_ASSERT_DESC( iCFFakeEnv->ClientEventFired(), "Script did not fire event!" );
    EUNIT_ASSERT_DESC( iCFFakeEnv->ScriptEventScriptId() == id,
         "Script ID does not match!" );
    EUNIT_ASSERT_DESC( iCFFakeEnv->ScriptEventIdentifier() == _L("startApp"),
        "Identifier does not match!" );

    EUNIT_ASSERT_DESC( iCFFakeEnv->ClientEventFiredCount() == 1,
        "Wrong amount of events!" );

    // TEST 2: Should not fire event if the context value did not change
    iCFFakeEnv->ClearFireClientEvent();
    err = iCFFakeEnv->PublishContext( *co, thread );
    EUNIT_ASSERT_DESC( err == KErrNone, "PublishContext failed!" );
    EUNIT_ASSERT_DESC( !iCFFakeEnv->ClientEventFired(),
         "Script did fired although context value did not change!" );

    EUNIT_ASSERT_DESC( iCFFakeEnv->ClientEventFiredCount() == 1,
        "Wrong amount of events!" );

    // TEST 3: Should not fire again if value is changed
    co->SetValueL(_L("WormGame"));
    iCFFakeEnv->ClearFireClientEvent();
    err = iCFFakeEnv->PublishContext( *co, thread );
    EUNIT_ASSERT_DESC( err == KErrNone, "PublishContext failed!" );
    EUNIT_ASSERT_DESC( iCFFakeEnv->ClientEventFired(),
         "Script did not fired although context changed!" );

    EUNIT_ASSERT_DESC( iCFFakeEnv->ClientEventFiredCount() == 2,
        "Wrong amount of events!" );

    CleanupStack::PopAndDestroy( co );
    CleanupStack::PopAndDestroy( &thread );
    }

void mt_basicoperationsplugin::MT_ScriptTest_contextUpdated_L(  )
    {
    RThread thread;
    CleanupClosePushL(thread);

    // Define and publish context
    CCFContextObject* co = CCFContextObject::NewLC();
    static _LIT_SECURITY_POLICY_PASS( alwaysPass );
    co->SetSourceL(_L("Application.State"));
    co->SetTypeL(_L("Foreground.View"));
    TInt err = iCFFakeEnv->DefineContext( co->Source(),
            co->Type(), alwaysPass, alwaysPass );
    EUNIT_ASSERT_DESC( err == KErrNone, "DefineContext failed!" );

    // load script
    HBufC8* script = LoadScriptFromFile(_L("test-contextUpdated.xml"));
    TInt id = iCFFakeEnv->AddScript(_L("test-contextUpdated.xml"),
            *script, KMT_CfScriptEngineUid, thread);
    EUNIT_ASSERT_DESC( id >= 0, "Parse error!" );
    delete script;

    iCFFakeEnv->ClearFireClientEventCount();

    // TEST 1: Context Changed, should fire event
    co->SetValueL(_L("WebBrowser"));
    iCFFakeEnv->ClearFireClientEvent();
    err = iCFFakeEnv->PublishContext( *co, thread );
    EUNIT_ASSERT_DESC( err == KErrNone, "PublishContext failed!" );
    EUNIT_ASSERT_DESC( iCFFakeEnv->ClientEventFired(), "Script did not fire event!" );
    EUNIT_ASSERT_DESC( iCFFakeEnv->ScriptEventScriptId() == id,
         "Script ID does not match!" );
    EUNIT_ASSERT_DESC( iCFFakeEnv->ScriptEventIdentifier() == _L("startApp"),
        "Identifier does not match!" );

    // TEST 2: Should fire event even if the context value did not change
    iCFFakeEnv->ClearFireClientEvent();
    err = iCFFakeEnv->PublishContext( *co, thread );
    EUNIT_ASSERT_DESC( err == KErrNone, "PublishContext failed!" );
    EUNIT_ASSERT_DESC( iCFFakeEnv->ClientEventFired(),
         "Script did not fired!" );

    // TEST 3: Should not fire again if value is changed
    co->SetValueL(_L("WormGame"));
    iCFFakeEnv->ClearFireClientEvent();
    err = iCFFakeEnv->PublishContext( *co, thread );
    EUNIT_ASSERT_DESC( err == KErrNone, "PublishContext failed!" );
    EUNIT_ASSERT_DESC( iCFFakeEnv->ClientEventFired(),
         "Script did not fired although context changed!" );

    CleanupStack::PopAndDestroy( co );
    CleanupStack::PopAndDestroy( &thread );
    }

void mt_basicoperationsplugin::MT_ScriptTest_and_L(  )
    {
    RThread thread;
    CleanupClosePushL(thread);

    // Define contexts
    static _LIT_SECURITY_POLICY_PASS( alwaysPass );
    CCFContextObject* co1 = CCFContextObject::NewLC();
    co1->SetSourceL(_L("Application.State"));
    co1->SetTypeL(_L("Foreground.Application"));
    TInt err = iCFFakeEnv->DefineContext( co1->Source(),
            co1->Type(), alwaysPass, alwaysPass );
    EUNIT_ASSERT_DESC( err == KErrNone, "DefineContext failed!" );

    CCFContextObject* co2 = CCFContextObject::NewLC();
    co2->SetSourceL(_L("Application.State"));
    co2->SetTypeL(_L("Foreground.View"));
    err = iCFFakeEnv->DefineContext( co2->Source(),
            co2->Type(), alwaysPass, alwaysPass );
    EUNIT_ASSERT_DESC( err == KErrNone, "DefineContext failed!" );

    // load script
    HBufC8* script = LoadScriptFromFile(_L("test-and.xml"));
    TInt id = iCFFakeEnv->AddScript(
            _L("test-and.xml"), *script, KMT_CfScriptEngineUid, thread);
    EUNIT_ASSERT_DESC( id >= 0, "Parse error!" );
    delete script;

    iCFFakeEnv->ClearFireClientEventCount();

    // TEST 1: Only first context published should not cause event
    co1->SetValueL(_L("MusicPlayer"));
    iCFFakeEnv->ClearFireClientEvent();
    err = iCFFakeEnv->PublishContext( *co1, thread );
    EUNIT_ASSERT_DESC( err == KErrNone, "PublishContext failed!" );
    EUNIT_ASSERT_DESC( !iCFFakeEnv->ClientEventFired(),
        "Script cause event although both contexts not set!" );

    // TEST 2: Both contexts published with right values should cause event
    co2->SetValueL(_L("NowPlaying"));
    iCFFakeEnv->ClearFireClientEvent();
    err = iCFFakeEnv->PublishContext( *co2, thread );
    EUNIT_ASSERT_DESC( err == KErrNone, "PublishContext failed!" );
    EUNIT_ASSERT_DESC( iCFFakeEnv->ClientEventFired(),
        "Script did not cause event although both context set with right values!" );

    // TEST 3: Publish Context 1 again, no event should come
    iCFFakeEnv->ClearFireClientEvent();
    err = iCFFakeEnv->PublishContext( *co1, thread );
    EUNIT_ASSERT_DESC( err == KErrNone, "PublishContext failed!" );
    EUNIT_ASSERT_DESC( !iCFFakeEnv->ClientEventFired(),
        "Script did cause event althought context 1 did not change!" );

    // TEST 4: Change context 1 to wrong value, no event should come
    co1->SetValueL(_L("WebBrowser"));
    iCFFakeEnv->ClearFireClientEvent();
    err = iCFFakeEnv->PublishContext( *co1, thread );
    EUNIT_ASSERT_DESC( err == KErrNone, "PublishContext failed!" );
    EUNIT_ASSERT_DESC( !iCFFakeEnv->ClientEventFired(),
        "Script did cause event althought context 1 did not change!" );

    // TEST 5: Change context 1 to right value, event should come
    co1->SetValueL(_L("MusicPlayer"));
    iCFFakeEnv->ClearFireClientEvent();
    err = iCFFakeEnv->PublishContext( *co1, thread );
    EUNIT_ASSERT_DESC( err == KErrNone, "PublishContext failed!" );
    EUNIT_ASSERT_DESC( iCFFakeEnv->ClientEventFired(),
        "Script did not cause event although both contexts have right value!" );

    CleanupStack::PopAndDestroy( co2 );
    CleanupStack::PopAndDestroy( co1 );
    CleanupStack::PopAndDestroy( &thread );
    }

void mt_basicoperationsplugin::MT_ScriptTest_and_contextChanged_L(  )
    {
    RThread thread;
    CleanupClosePushL(thread);

    // Define and publish context
    static _LIT_SECURITY_POLICY_PASS( alwaysPass );
    CCFContextObject* co1 = CCFContextObject::NewLC();
    co1->SetSourceL(_L("Application.State"));
    co1->SetTypeL(_L("Foreground.Application"));
    TInt err = iCFFakeEnv->DefineContext( co1->Source(),
            co1->Type(), alwaysPass, alwaysPass );
    EUNIT_ASSERT_DESC( err == KErrNone, "DefineContext failed!" );

    CCFContextObject* co2 = CCFContextObject::NewLC();;
    co2->SetSourceL(_L("Application.State"));
    co2->SetTypeL(_L("Foreground.View"));
    err = iCFFakeEnv->DefineContext( co2->Source(),
            co2->Type(), alwaysPass, alwaysPass );
    EUNIT_ASSERT_DESC( err == KErrNone, "DefineContext failed!" );

    // load script
    HBufC8* script = LoadScriptFromFile(_L("test-and-contextChanged.xml"));
    TInt id = iCFFakeEnv->AddScript(_L("test-and-contextChanged.xml"),
            *script, KMT_CfScriptEngineUid, thread);
    EUNIT_ASSERT_DESC( id >= 0, "Parse error!" );
    delete script;

    iCFFakeEnv->ClearFireClientEventCount();

    // TEST 1: Only first context published should not cause event
    co1->SetValueL(_L("MusicPlayer"));
    iCFFakeEnv->ClearFireClientEvent();
    err = iCFFakeEnv->PublishContext( *co1, thread );
    EUNIT_ASSERT_DESC( err == KErrNone, "PublishContext failed!" );
    EUNIT_ASSERT_DESC( !iCFFakeEnv->ClientEventFired(),
        "Script cause event although both contexts not set!" );

    // TEST 2: Both contexts published with right values should cause event
    co2->SetValueL(_L("NowPlaying"));
    iCFFakeEnv->ClearFireClientEvent();
    err = iCFFakeEnv->PublishContext( *co2, thread );
    EUNIT_ASSERT_DESC( err == KErrNone, "PublishContext failed!" );
    EUNIT_ASSERT_DESC( iCFFakeEnv->ClientEventFired(),
        "Script did not cause event although both context set with right values!" );

    // TEST 3: Publish Context 2 again, no event should come
    iCFFakeEnv->ClearFireClientEvent();
    err = iCFFakeEnv->PublishContext( *co2, thread );
    EUNIT_ASSERT_DESC( err == KErrNone, "PublishContext failed!" );
    EUNIT_ASSERT_DESC( !iCFFakeEnv->ClientEventFired(),
        "Script did cause event althought context 1 did not change!" );

    // TEST 4: Change context 1 to wrong value, no event should come
    co1->SetValueL(_L("WebBrowser"));
    iCFFakeEnv->ClearFireClientEvent();
    err = iCFFakeEnv->PublishContext( *co1, thread );
    EUNIT_ASSERT_DESC( err == KErrNone, "PublishContext failed!" );
    EUNIT_ASSERT_DESC( !iCFFakeEnv->ClientEventFired(),
        "Script did cause event althought context 1 did not change!" );

    // TEST 5: Change context 1 to right value, no event should come
    // because contextChanged -operation
    co1->SetValueL(_L("MusicPlayer"));
    iCFFakeEnv->ClearFireClientEvent();
    err = iCFFakeEnv->PublishContext( *co1, thread );
    EUNIT_ASSERT_DESC( err == KErrNone, "PublishContext failed!" );
    EUNIT_ASSERT_DESC( !iCFFakeEnv->ClientEventFired(),
        "Script did cause event despite of contextChanged-operation!" );

    // TEST 6: Change context 2 to wrong value, no event should come
    co2->SetValueL(_L("Playlist"));
    iCFFakeEnv->ClearFireClientEvent();
    err = iCFFakeEnv->PublishContext( *co2, thread );
    EUNIT_ASSERT_DESC( err == KErrNone, "PublishContext failed!" );
    EUNIT_ASSERT_DESC( !iCFFakeEnv->ClientEventFired(),
        "Script cause event although context 2 changed to wrong value!" );

    // TEST 7: Change context 2 to right value, event should come
    co2->SetValueL(_L("NowPlaying"));
    iCFFakeEnv->ClearFireClientEvent();
    err = iCFFakeEnv->PublishContext( *co2, thread );
    EUNIT_ASSERT_DESC( err == KErrNone, "PublishContext failed!" );
    EUNIT_ASSERT_DESC( iCFFakeEnv->ClientEventFired(),
        "Script did not cause event although context 2 changed to right value!" );

    CleanupStack::PopAndDestroy( co2 );
    CleanupStack::PopAndDestroy( co1 );
    CleanupStack::PopAndDestroy( &thread );
    }

void mt_basicoperationsplugin::MT_ScriptTest_and_contextUpdated_L(  )
    {
    RThread thread;
    CleanupClosePushL(thread);

    // Define context
    static _LIT_SECURITY_POLICY_PASS( alwaysPass );
    CCFContextObject* co1 = CCFContextObject::NewLC();
    co1->SetSourceL(_L("Sensor"));
    co1->SetTypeL(_L("DoubleTap"));
    TInt err = iCFFakeEnv->DefineContext( co1->Source(),
            co1->Type(), alwaysPass, alwaysPass );
    EUNIT_ASSERT_DESC( err == KErrNone, "DefineContext failed!" );

    // load script
    HBufC8* script = LoadScriptFromFile(_L("test-and-contextUpdated.xml"));
    TInt id = iCFFakeEnv->AddScript(_L("test-and-contextUpdated.xml"),
            *script, KMT_CfScriptEngineUid, thread);
    EUNIT_ASSERT_DESC( id >= 0, "Parse error!" );
    delete script;

    // TEST 1: Wrong value should not cause event
    iCFFakeEnv->ClearFireClientEventCount();
    co1->SetValueL(_L("Left"));
    iCFFakeEnv->ClearFireClientEvent();
    err = iCFFakeEnv->PublishContext( *co1, thread );
    EUNIT_ASSERT_DESC( err == KErrNone, "PublishContext failed!" );
    EUNIT_ASSERT_DESC( !iCFFakeEnv->ClientEventFired(),
        "Script cause event although wrong value!" );

    // TEST 2: Value Right should cause event
    co1->SetValueL(_L("Right"));
    iCFFakeEnv->ClearFireClientEvent();
    err = iCFFakeEnv->PublishContext( *co1, thread );
    EUNIT_ASSERT_DESC( err == KErrNone, "PublishContext failed!" );
    EUNIT_ASSERT_DESC( iCFFakeEnv->ClientEventFired(),
        "Script did not cause event although right value!" );

    // TEST 3: Second publish should also give event although context did
    // not change
    iCFFakeEnv->ClearFireClientEvent();
    err = iCFFakeEnv->PublishContext( *co1, thread );
    EUNIT_ASSERT_DESC( err == KErrNone, "PublishContext failed!" );
    EUNIT_ASSERT_DESC( iCFFakeEnv->ClientEventFired(),
        "Script did not cause event when value did not change!" );

    CleanupStack::PopAndDestroy( co1 );
    CleanupStack::PopAndDestroy( &thread );
    }

void mt_basicoperationsplugin::MT_ScriptTest_or_L(  )
    {
    RThread thread;
    CleanupClosePushL(thread);

    // Define context
    static _LIT_SECURITY_POLICY_PASS( alwaysPass );
    CCFContextObject* co1 = CCFContextObject::NewLC();
    co1->SetSourceL(_L("Application.State"));
    co1->SetTypeL(_L("Foreground.View"));
    TInt err = iCFFakeEnv->DefineContext( co1->Source(),
            co1->Type(), alwaysPass, alwaysPass );
    EUNIT_ASSERT_DESC( err == KErrNone, "DefineContext failed!" );

    // load script
    HBufC8* script = LoadScriptFromFile(_L("test-or.xml"));
    TInt id = iCFFakeEnv->AddScript(
            _L("test-or.xml"), *script, KMT_CfScriptEngineUid, thread);
    EUNIT_ASSERT_DESC( id >= 0, "Parse error!" );
    delete script;

    iCFFakeEnv->ClearFireClientEventCount();

    // TEST 1: First value should cause event
    co1->SetValueL(_L("MusicLibrary"));
    iCFFakeEnv->ClearFireClientEvent();
    err = iCFFakeEnv->PublishContext( *co1, thread );
    EUNIT_ASSERT_DESC( err == KErrNone, "PublishContext failed!" );
    EUNIT_ASSERT_DESC( iCFFakeEnv->ClientEventFired(),
        "Script did not cause event although right context value!" );

    // TEST 3: Publish Context 1 again, no event should come
    iCFFakeEnv->ClearFireClientEvent();
    err = iCFFakeEnv->PublishContext( *co1, thread );
    EUNIT_ASSERT_DESC( err == KErrNone, "PublishContext failed!" );
    EUNIT_ASSERT_DESC( !iCFFakeEnv->ClientEventFired(),
        "Script did cause event althought context 1 did not change!" );

    // TEST 4: Change context 1 to wrong value, no event should come
    co1->SetValueL(_L("WebBrowser"));
    iCFFakeEnv->ClearFireClientEvent();
    err = iCFFakeEnv->PublishContext( *co1, thread );
    EUNIT_ASSERT_DESC( err == KErrNone, "PublishContext failed!" );
    EUNIT_ASSERT_DESC( !iCFFakeEnv->ClientEventFired(),
        "Script did cause event althought context 1 did not change!" );

    // TEST 5: Change context 1 to right value, event should come
    co1->SetValueL(_L("MusicShop"));
    iCFFakeEnv->ClearFireClientEvent();
    err = iCFFakeEnv->PublishContext( *co1, thread );
    EUNIT_ASSERT_DESC( err == KErrNone, "PublishContext failed!" );
    EUNIT_ASSERT_DESC( iCFFakeEnv->ClientEventFired(),
        "Script did not cause event although right value!" );

    // TEST 6: Change context 1 to right value, event should come
    co1->SetValueL(_L("MusicLibrary"));
    iCFFakeEnv->ClearFireClientEvent();
    err = iCFFakeEnv->PublishContext( *co1, thread );
    EUNIT_ASSERT_DESC( err == KErrNone, "PublishContext failed!" );
    EUNIT_ASSERT_DESC( iCFFakeEnv->ClientEventFired(),
        "Script did not cause event although right value!" );

    CleanupStack::PopAndDestroy( co1 );
    CleanupStack::PopAndDestroy( &thread );
    }

void mt_basicoperationsplugin::MT_ScriptTest_not_L(  )
    {
    RThread thread;
    CleanupClosePushL(thread);

    // Define and publish context
    static _LIT_SECURITY_POLICY_PASS( alwaysPass );
    CCFContextObject* co = CCFContextObject::NewLC();
    co->SetSourceL(_L("Application.State"));
    co->SetTypeL(_L("Foreground.View"));
    TInt err = iCFFakeEnv->DefineContext( co->Source(),
            co->Type(), alwaysPass, alwaysPass );
    EUNIT_ASSERT_DESC( err == KErrNone, "DefineContext failed!" );

    // load script
    HBufC8* script = LoadScriptFromFile(_L("test-not.xml"));
    TInt id = iCFFakeEnv->AddScript(
            _L("test-not.xml"), *script, KMT_CfScriptEngineUid, thread);
    EUNIT_ASSERT_DESC( id >= 0, "Parse error!" );
    delete script;

    // TEST 1: "Right" context should not cause event
    co->SetValueL(_L("Settings"));
    iCFFakeEnv->ClearFireClientEvent();
    err = iCFFakeEnv->PublishContext( *co, thread );
    EUNIT_ASSERT_DESC( err == KErrNone, "PublishContext failed!" );
    EUNIT_ASSERT_DESC( !iCFFakeEnv->ClientEventFired(), "Script did fire event!" );

    // TEST 2: "Wrong" context should cause event
    co->SetValueL(_L("MainView"));
    iCFFakeEnv->ClearFireClientEvent();
    err = iCFFakeEnv->PublishContext( *co, thread );
    EUNIT_ASSERT_DESC( err == KErrNone, "PublishContext failed!" );
    EUNIT_ASSERT_DESC( iCFFakeEnv->ClientEventFired(), "Script did not fire event!" );

    // TEST 3: Same "Wrong" context again should not cause event
    co->SetValueL(_L("MainView"));
    iCFFakeEnv->ClearFireClientEvent();
    err = iCFFakeEnv->PublishContext( *co, thread );
    EUNIT_ASSERT_DESC( err == KErrNone, "PublishContext failed!" );
    EUNIT_ASSERT_DESC( !iCFFakeEnv->ClientEventFired(), "Script did fire event!" );

    // TEST 4: Another "Wrong" context should cause event
    co->SetValueL(_L("AdvancedSettings"));
    iCFFakeEnv->ClearFireClientEvent();
    err = iCFFakeEnv->PublishContext( *co, thread );
    EUNIT_ASSERT_DESC( err == KErrNone, "PublishContext failed!" );
    EUNIT_ASSERT_DESC( iCFFakeEnv->ClientEventFired(), "Script did not fire event!" );

    CleanupStack::PopAndDestroy( co );
    CleanupStack::PopAndDestroy( &thread );
    }

void mt_basicoperationsplugin::MT_ScriptTest_publishContext_L(  )
    {
    RThread thread;
    CleanupClosePushL(thread);

    // Define context
    static _LIT_SECURITY_POLICY_PASS( alwaysPass );
    CCFContextObject* co = CCFContextObject::NewLC();
    co->SetSourceL(_L("t"));
    co->SetTypeL(_L("t.p.app"));
    TInt err = iCFFakeEnv->DefineContext( co->Source(),
            co->Type(), alwaysPass, alwaysPass );
    EUNIT_ASSERT_DESC( err == KErrNone, "DefineContext failed!" );

    // we must define the context for <publishContext>
    err = iCFFakeEnv->DefineContext( _L("Sensor"),
            _L("Z.Acceleration"), alwaysPass, alwaysPass );
    EUNIT_ASSERT_DESC( err == KErrNone, "DefineContext failed!" );

    // load script
    HBufC8* script = LoadScriptFromFile(_L("test-publishContext.xml"));
    TInt id = iCFFakeEnv->AddScript(_L("test-publishContext.xml"),
            *script, KMT_CfScriptEngineUid, thread);
    EUNIT_ASSERT_DESC( id >= 0, "Parse error!" );
    delete script;

    CCFContextSubscription* subscription = CCFContextSubscription::NewL();
    subscription->SetContextSourceL(_L("Sensor"));
    subscription->SetContextTypeL(_L("Z.Acceleration"));

    // make subscription
    err = iCFFakeEnv->SubscribeContext( subscription, thread );
    EUNIT_ASSERT_DESC( err == KErrNone, "SubscribeContext failed!" );

    // TEST 1: "Right" context should not cause event
    co->SetValueL(_L("ok"));
    iCFFakeEnv->ClearFireClientEvent();
    err = iCFFakeEnv->PublishContext( *co, thread );
    EUNIT_ASSERT_DESC( err == KErrNone, "PublishContext failed!" );

    // Check if we get context Sensor: Z.Acceleration: Medium
    EUNIT_ASSERT_DESC ( iCFFakeEnv->IndicationArrived(),
            "Context indication did not arrive");
    const CCFContextObject& indicatedCo = iCFFakeEnv->IndicationContext();
    EUNIT_ASSERT_DESC ( indicatedCo.Source() == _L("Sensor"),
        "Source of indication does not match!" );
    EUNIT_ASSERT_DESC ( indicatedCo.Type() == _L("Z.Acceleration"),
        "Type of indication does not match!" );
    EUNIT_ASSERT_DESC ( indicatedCo.Value() == _L("Medium"),
        "Value of indication does not match!" );
    iCFFakeEnv->SetIndicationArrived( EFalse );

    CleanupStack::PopAndDestroy( co );
    CleanupStack::PopAndDestroy( &thread );
    }

void mt_basicoperationsplugin::MT_ScriptTest_publishContext2_L(  )
    {
    // x. Preconditions
    // -------------------------------------------------------------------------
    RThread thread;
    CleanupClosePushL(thread);
    static _LIT_SECURITY_POLICY_PASS( alwaysPass );
    CCFContextObject* co = CCFContextObject::NewLC();
    
    // x. Register script and publish the same context. 
    //    Check that script is loaded and context published. 
    // -------------------------------------------------------------------------

    // Define context:
    //  source.publishContext2
    //  type.publishContext2
    TInt err = iCFFakeEnv->DefineContext( _L("source.publishContext2"),
        _L("type.publishContext2"),
        alwaysPass );
    EUNIT_ASSERT_DESC( err == KErrNone, "Define context failed!" );

    // Define context:
    //  source.publishContext2
    //  type.publishContext2.reply
    err = iCFFakeEnv->DefineContext( _L("source.publishContext2"),
        _L("type.publishContext2.reply"),
        alwaysPass );
    EUNIT_ASSERT_DESC( err == KErrNone, "Define context failed!" );
    
    // Load script test_publishContext2.xml
    HBufC8* script = LoadScriptFromFile(_L("test-publishContext2.xml"));
    TInt id = iCFFakeEnv->AddScript(_L("test-publishContext2.xml"),
            *script, KMT_CfScriptEngineUid, thread);
    EUNIT_ASSERT_DESC( id >= 0, "test-publishContext2.xml: Parse error!" );
    delete script;

    // Subscribe context:
    //  source.publishContext2
    //  type.publishContext2.reply
    CCFContextSubscription* subscription = CCFContextSubscription::NewL();
    subscription->SetContextSourceL(_L("source.publishContext2"));
    subscription->SetContextTypeL(_L("type.publishContext2.reply"));

    err = iCFFakeEnv->SubscribeContext( subscription, thread );
    EUNIT_ASSERT_DESC( err == KErrNone, "SubscribeContext failed!" );

    // Clear events 
    iCFFakeEnv->ClearFireClientEvent();

    // Subscribe context:
    //  source.publishContext2
    //  type.publishContext2
    //  true
    co->SetSourceL( _L("source.publishContext2") );
    co->SetTypeL( _L("type.publishContext2") );
    co->SetValueL( _L("true") );
    
    err = iCFFakeEnv->PublishContext( *co, thread );
    EUNIT_ASSERT_DESC( err == KErrNone, "Published context failed!" );

    // Check if we get context:
    //  source.publishContext2
    //  type.publishContext2.reply
    //  true
    EUNIT_ASSERT_DESC ( iCFFakeEnv->IndicationArrived(),
            "Context indication did not arrive");
    
    const CCFContextObject& indicatedCo = iCFFakeEnv->IndicationContext();
    EUNIT_ASSERT_DESC ( indicatedCo.Source() == _L("source.publishContext2"),
        "Source of indication does not match!" );
    
    EUNIT_ASSERT_DESC ( indicatedCo.Type() == _L("type.publishContext2.reply"),
        "Type of indication does not match!" );
    
    EUNIT_ASSERT_DESC ( indicatedCo.Value() == _L("true"),
        "Value of indication does not match!" );
    
    iCFFakeEnv->SetIndicationArrived( EFalse );
    
    // x. Cleanup
    // -------------------------------------------------------------------------
    CleanupStack::PopAndDestroy( co );
    CleanupStack::PopAndDestroy( &thread );
    }

void mt_basicoperationsplugin::MT_ScriptTest_publishContext3_L(  )
    {
    // x. Preconditions
    // -------------------------------------------------------------------------
    RThread thread;
    CleanupClosePushL(thread);
    static _LIT_SECURITY_POLICY_PASS( alwaysPass );
    CCFContextObject* co = CCFContextObject::NewLC();
    
    // x. Register script and publish the same context. 
    //    Check that script is loaded and context published. 
    // -------------------------------------------------------------------------

    // Define context:
    //  source.publishContext3
    //  type.publishContext3
    TInt err = iCFFakeEnv->DefineContext( _L("source.publishContext3"),
        _L("type.publishContext3"),
        alwaysPass );
    EUNIT_ASSERT_DESC( err == KErrNone, "Define context failed!" );

    // Define context:
    //  source.publishContext3
    //  type.publishContext3.reply
    err = iCFFakeEnv->DefineContext( _L("source.publishContext3"),
        _L("type.publishContext3.reply"),
        alwaysPass );
    EUNIT_ASSERT_DESC( err == KErrNone, "Define context failed!" );
    
    // Load script test_publishContext3.xml
    HBufC8* script = LoadScriptFromFile(_L("test-publishContext3.xml"));
    TInt id = iCFFakeEnv->AddScript(_L("test-publishContext3.xml"),
            *script, KMT_CfScriptEngineUid, thread);
    EUNIT_ASSERT_DESC( id >= 0, "test-publishContext3.xml: Parse error!" );
    delete script;

    // Subscribe context:
    //  source.publishContext3
    //  type.publishContext3.reply
    CCFContextSubscription* subscription = CCFContextSubscription::NewL();
    subscription->SetContextSourceL(_L("source.publishContext3"));
    subscription->SetContextTypeL(_L("type.publishContext3.reply"));

    err = iCFFakeEnv->SubscribeContext( subscription, thread );
    EUNIT_ASSERT_DESC( err == KErrNone, "SubscribeContext failed!" );

    // Clear events 
    iCFFakeEnv->ClearFireClientEvent();

    // Subscribe context:
    //  source.publishContext3
    //  type.publishContext3
    //  true
    co->SetSourceL( _L("source.publishContext3") );
    co->SetTypeL( _L("type.publishContext3") );
    co->SetValueL( _L("true") );
    
    err = iCFFakeEnv->PublishContext( *co, thread );
    EUNIT_ASSERT_DESC( err == KErrNone, "Published context failed!" );

    // Check if we get context:
    //  source.publishContext3
    //  type.publishContext3.reply
    //  true
    EUNIT_ASSERT_DESC ( iCFFakeEnv->IndicationArrived(),
            "Context indication did not arrive");
    
    const CCFContextObject& indicatedCo = iCFFakeEnv->IndicationContext();
    EUNIT_ASSERT_DESC ( indicatedCo.Source() == _L("source.publishContext3"),
        "Source of indication does not match!" );
    
    EUNIT_ASSERT_DESC ( indicatedCo.Type() == _L("type.publishContext3.reply"),
        "Type of indication does not match!" );
    
    EUNIT_ASSERT_DESC ( indicatedCo.Value() == _L("true"),
        "Value of indication does not match!" );
    
    iCFFakeEnv->SetIndicationArrived( EFalse );
    
    // x. Cleanup
    // -------------------------------------------------------------------------
    CleanupStack::PopAndDestroy( co );
    CleanupStack::PopAndDestroy( &thread );
    }

void mt_basicoperationsplugin::MT_ScriptTest_elseif_L(  )
    {
    RThread thread;
    CleanupClosePushL(thread);

    // Define context
    static _LIT_SECURITY_POLICY_PASS( alwaysPass );
    CCFContextObject* co1 = CCFContextObject::NewLC();
    co1->SetSourceL(_L("Application.State"));
    co1->SetTypeL(_L("Foreground.View"));
    TInt err = iCFFakeEnv->DefineContext( co1->Source(),
            co1->Type(), alwaysPass, alwaysPass );
    EUNIT_ASSERT_DESC( err == KErrNone, "DefineContext failed!" );

    // load script
    HBufC8* script = LoadScriptFromFile(_L("test-elseif.xml"));
    TInt id = iCFFakeEnv->AddScript(
            _L("test-elseif.xml"), *script, KMT_CfScriptEngineUid, thread);
    EUNIT_ASSERT_DESC( id >= 0, "Parse error!" );
    delete script;

    iCFFakeEnv->ClearFireClientEventCount();

    // TEST 1: First value should cause first event
    co1->SetValueL(_L("NowPlaying"));
    iCFFakeEnv->ClearFireClientEvent();
    err = iCFFakeEnv->PublishContext( *co1, thread );
    EUNIT_ASSERT_DESC( err == KErrNone, "PublishContext failed!" );
    EUNIT_ASSERT_DESC( iCFFakeEnv->ClientEventFired(),
        "Script did not cause event although both contexts not set!" );
    EUNIT_ASSERT_DESC( iCFFakeEnv->ScriptEventScriptId() == id,
         "Script ID does not match!" );
    EUNIT_ASSERT_DESC( iCFFakeEnv->ScriptEventIdentifier() == _L("firstTestAction"),
        "Identifier does not match!" );

    // TEST 2: Second value should cause second event
    co1->SetValueL(_L("Settings"));
    err = iCFFakeEnv->PublishContext( *co1, thread );
    EUNIT_ASSERT_DESC( err == KErrNone, "PublishContext failed!" );
    EUNIT_ASSERT_DESC( iCFFakeEnv->ClientEventFired(),
        "Script did not cause event although both contexts not set!" );
    EUNIT_ASSERT_DESC( iCFFakeEnv->ScriptEventScriptId() == id,
         "Script ID does not match!" );
    EUNIT_ASSERT_DESC( iCFFakeEnv->ScriptEventIdentifier() == _L("secondTestAction"),
        "Identifier does not match!" );

    CleanupStack::PopAndDestroy( co1 );
    CleanupStack::PopAndDestroy( &thread );
    }

void mt_basicoperationsplugin::MT_ScriptTest_else_L(  )
    {
    RThread thread;
    CleanupClosePushL(thread);

    // Define and publish context
    static _LIT_SECURITY_POLICY_PASS( alwaysPass );
    CCFContextObject* co1 = CCFContextObject::NewLC();
    co1->SetSourceL(_L("Application.State"));
    co1->SetTypeL(_L("Foreground.View"));
    TInt err = iCFFakeEnv->DefineContext( co1->Source(),
            co1->Type(), alwaysPass, alwaysPass );
    EUNIT_ASSERT_DESC( err == KErrNone, "DefineContext failed!" );

    // load script
    HBufC8* script = LoadScriptFromFile(_L("test-else.xml"));
    TInt id = iCFFakeEnv->AddScript(
            _L("test-else.xml"), *script, KMT_CfScriptEngineUid, thread);
    EUNIT_ASSERT_DESC( id >= 0, "Parse error!" );
    delete script;

    iCFFakeEnv->ClearFireClientEventCount();

    // TEST 1: First value should cause first event
    co1->SetValueL(_L("NowPlaying"));
    iCFFakeEnv->ClearFireClientEvent();
    err = iCFFakeEnv->PublishContext( *co1, thread );
    EUNIT_ASSERT_DESC( err == KErrNone, "PublishContext failed!" );
    EUNIT_ASSERT_DESC( iCFFakeEnv->ClientEventFired(),
        "Script did not cause event although both contexts not set!" );
    EUNIT_ASSERT_DESC( iCFFakeEnv->ScriptEventScriptId() == id,
         "Script ID does not match!" );
    EUNIT_ASSERT_DESC( iCFFakeEnv->ScriptEventIdentifier() == _L("firstTestAction"),
        "Identifier does not match!" );

    // TEST 2: Republishing same context should not cause event
    iCFFakeEnv->ClearFireClientEvent();
    err = iCFFakeEnv->PublishContext( *co1, thread );
    EUNIT_ASSERT_DESC( err == KErrNone, "PublishContext failed!" );
    EUNIT_ASSERT_DESC( !iCFFakeEnv->ClientEventFired(),
        "Script caused event although context did not change!" );

    // TEST 3: Second value should cause second event
    co1->SetValueL(_L("Settings"));
    err = iCFFakeEnv->PublishContext( *co1, thread );
    EUNIT_ASSERT_DESC( err == KErrNone, "PublishContext failed!" );
    EUNIT_ASSERT_DESC( iCFFakeEnv->ClientEventFired(),
        "Script did not cause event although both contexts not set!" );
    EUNIT_ASSERT_DESC( iCFFakeEnv->ScriptEventScriptId() == id,
         "Script ID does not match!" );
    EUNIT_ASSERT_DESC( iCFFakeEnv->ScriptEventIdentifier() == _L("secondTestAction"),
        "Identifier does not match!" );

    // TEST 4: Republishing same context should not cause event
    // because context value did not change -> no evaluation
    iCFFakeEnv->ClearFireClientEvent();
    err = iCFFakeEnv->PublishContext( *co1, thread );
    EUNIT_ASSERT_DESC( err == KErrNone, "PublishContext failed!" );
    EUNIT_ASSERT_DESC( !iCFFakeEnv->ClientEventFired(),
        "Script caused event although context did not change!" );

    CleanupStack::PopAndDestroy( co1 );
    CleanupStack::PopAndDestroy( &thread );
    }

void mt_basicoperationsplugin::MT_ScriptTest_delay_L(  )
    {
    RThread thread;
    CleanupClosePushL(thread);

    // Define and publish context
    static _LIT_SECURITY_POLICY_PASS( alwaysPass );
    CCFContextObject* co1 = CCFContextObject::NewLC();
    co1->SetSourceL(_L("t"));
    co1->SetTypeL(_L("t.se.delay.a"));
    TInt err = iCFFakeEnv->DefineContext( co1->Source(),
            co1->Type(), alwaysPass, alwaysPass );
    EUNIT_ASSERT_DESC( err == KErrNone, "DefineContext failed!" );

    CCFContextObject* co2 = CCFContextObject::NewLC();;
    co2->SetSourceL(_L("t"));
    co2->SetTypeL(_L("t.se.delay.b"));
    err = iCFFakeEnv->DefineContext( co2->Source(),
            co2->Type(), alwaysPass, alwaysPass );
    EUNIT_ASSERT_DESC( err == KErrNone, "DefineContext failed!" );

    // load scripts
    HBufC8* script1 = LoadScriptFromFile(_L("test-delay-script1.xml"));
    TInt id1 = iCFFakeEnv->AddScript(_L("test-delay-script1.xml"),
            *script1, KMT_CfScriptEngineUid, thread);
    EUNIT_ASSERT_DESC( id1 >= 0, "Parse error!" );
    delete script1;

    HBufC8* script2 = LoadScriptFromFile(_L("test-delay-script2.xml"));
    TInt id2 = iCFFakeEnv->AddScript(_L("test-delay-script2.xml"),
            *script2, KMT_CfScriptEngineUid, thread);
    EUNIT_ASSERT_DESC( id2 >= 0, "Parse error!" );
    delete script2;

    iCFFakeEnv->ClearFireClientEventCount();

    // TEST 1: A -> white, B = not initialized -> no events
    co1->SetValueL(_L("white"));
    iCFFakeEnv->ClearFireClientEvent();
    err = iCFFakeEnv->PublishContext( *co1, thread );
    EUNIT_ASSERT_DESC( err == KErrNone, "PublishContext failed!" );
    EUNIT_ASSERT_DESC( !iCFFakeEnv->ClientEventFired(),
        "Script cause event although both contexts not set!" );

    // TEST 2: A = white, B -> white   -> no events
    co2->SetValueL(_L("white"));
    iCFFakeEnv->ClearFireClientEvent();
    err = iCFFakeEnv->PublishContext( *co2, thread );
    EUNIT_ASSERT_DESC( err == KErrNone, "PublishContext failed!" );

    iTestDelay->Start(250000); // 250 ms
    EUNIT_ASSERT_DESC( !iCFFakeEnv->ClientEventFired(),
        "Script caused event although wrong values!" );

    // TEST 3: A->blue, B=white, no event in 10 ms
    co1->SetValueL(_L("blue"));
    iCFFakeEnv->ClearFireClientEvent();
    err = iCFFakeEnv->PublishContext( *co1, thread );
    EUNIT_ASSERT_DESC( err == KErrNone, "PublishContext failed!" );
    iTestDelay->Start(10000); // 10 ms
    EUNIT_ASSERT_DESC( !iCFFakeEnv->ClientEventFired(),
        "Script caused event although wrong values!" );

    // TEST 4: A=blue, B->red
    // script 1 should not fire events although temporary
    // A=blue, B=white -situation because of evaluation delay
    // script 2 should cause event at 200 ms
    co2->SetValueL(_L("red"));
    iCFFakeEnv->ClearFireClientEvent();
    err = iCFFakeEnv->PublishContext( *co2, thread );
    EUNIT_ASSERT_DESC( err == KErrNone, "PublishContext failed!" );
    iTestDelay->Start(50000); // 50 ms
    EUNIT_ASSERT_DESC( !iCFFakeEnv->ClientEventFired(),
        "Event before 50 ms!" );
    iTestDelay->Start(250000); // 50 ms + 250 ms = 300 ms
    EUNIT_ASSERT_DESC( iCFFakeEnv->ClientEventFired(),
        "Script did not cause event!" );
    EUNIT_ASSERT_DESC( iCFFakeEnv->ScriptEventScriptId() == id2,
         "Script ID does not match!" );
    EUNIT_ASSERT_DESC( iCFFakeEnv->ScriptEventIdentifier() == _L("endUserAction2"),
        "Identifier does not match!" );

    EUNIT_ASSERT_DESC( iCFFakeEnv->ClientEventFiredCount() == 1,
        "Wrong number of events!" );

    // TEST 5: When context B is set back to white,
    // script 1 should cause event at 200 ms
    co2->SetValueL(_L("white"));
    iCFFakeEnv->ClearFireClientEvent();
    err = iCFFakeEnv->PublishContext( *co2, thread );
    EUNIT_ASSERT_DESC( err == KErrNone, "PublishContext failed!" );
    iTestDelay->Start(50000); // 50 ms
    EUNIT_ASSERT_DESC( !iCFFakeEnv->ClientEventFired(),
        "Event before 50 ms!" );
    iTestDelay->Start(250000); // 50 ms + 250 ms = 300 ms
    EUNIT_ASSERT_DESC( iCFFakeEnv->ClientEventFired(),
        "Script did not cause event!" );
    EUNIT_ASSERT_DESC( iCFFakeEnv->ScriptEventScriptId() == id1,
         "Script ID does not match!" );
    EUNIT_ASSERT_DESC( iCFFakeEnv->ScriptEventIdentifier() == _L("endUserAction1"),
        "Identifier does not match!" );

    EUNIT_ASSERT_DESC( iCFFakeEnv->ClientEventFiredCount() == 2,
        "Wrong number of events!" );

    CleanupStack::PopAndDestroy( co2 );
    CleanupStack::PopAndDestroy( co1 );
    CleanupStack::PopAndDestroy( &thread );
    }

void mt_basicoperationsplugin::MT_ScriptTest_delay_contextRef_L(  )
    {
    RThread thread;
    CleanupClosePushL(thread);

    // Define and publish context
    static _LIT_SECURITY_POLICY_PASS( alwaysPass );
    CCFContextObject* co1 = CCFContextObject::NewLC();
    co1->SetSourceL(_L("t"));
    co1->SetTypeL(_L("t.se.delay.ca"));
    TInt err = iCFFakeEnv->DefineContext( co1->Source(),
            co1->Type(), alwaysPass, alwaysPass );
    EUNIT_ASSERT_DESC( err == KErrNone, "DefineContext failed!" );

    CCFContextObject* co2 = CCFContextObject::NewLC();;
    co2->SetSourceL(_L("t"));
    co2->SetTypeL(_L("t.se.delay.cb"));
    err = iCFFakeEnv->DefineContext( co2->Source(),
            co2->Type(), alwaysPass, alwaysPass );
    EUNIT_ASSERT_DESC( err == KErrNone, "DefineContext failed!" );

    CCFContextObject* co3 = CCFContextObject::NewLC();
    co3->SetSourceL(_L("t"));
    co3->SetTypeL(_L("t.se.delay.cc"));
    err = iCFFakeEnv->DefineContext( co3->Source(),
            co3->Type(), alwaysPass, alwaysPass );
    EUNIT_ASSERT_DESC( err == KErrNone, "DefineContext failed!" );

    // load script
    HBufC8* script = LoadScriptFromFile(_L("test-delay-contextRef.xml"));
    TInt id = iCFFakeEnv->AddScript(_L("test-delay-contextRef.xml"),
            *script, KMT_CfScriptEngineUid, thread);
    EUNIT_ASSERT_DESC( id >= 0, "Parse error!" );
    delete script;

    iCFFakeEnv->ClearFireClientEventCount();

    // TEST 1: First context should cause indication at 100 ms
    co1->SetValueL(_L("update"));
    iCFFakeEnv->ClearFireClientEvent();
    err = iCFFakeEnv->PublishContext( *co1, thread );
    EUNIT_ASSERT_DESC( err == KErrNone, "PublishContext failed!" );

    // There should not be event at 50 ms
    iTestDelay->Start(50000); // 50 ms
    EUNIT_ASSERT_DESC( !iCFFakeEnv->ClientEventFired(),
        "Script caused event at 50 ms!" );

    // But at 150 ms there should
    iTestDelay->Start(100000); // 50 + 100 ms = 150 ms
    EUNIT_ASSERT_DESC( iCFFakeEnv->ClientEventFired(),
        "Script did not give event at 150 ms!" );

    // TEST 2: Second context should cause indication at 200 ms
    co2->SetValueL(_L("update"));
    iCFFakeEnv->ClearFireClientEvent();
    err = iCFFakeEnv->PublishContext( *co2, thread );
    EUNIT_ASSERT_DESC( err == KErrNone, "PublishContext failed!" );

    // There should not be event at 50 ms
    iTestDelay->Start(50000); // 50 ms
    EUNIT_ASSERT_DESC( !iCFFakeEnv->ClientEventFired(),
        "Script caused event at 50 ms!" );

    // But at 300 ms there should
    iTestDelay->Start(250000); // 50 + 250 ms = 300 ms
    EUNIT_ASSERT_DESC( iCFFakeEnv->ClientEventFired(),
        "Script did not give event at 300 ms!" );

    // TEST 3: Third context should cause event immediately
    co3->SetValueL(_L("update"));
    iCFFakeEnv->ClearFireClientEvent();
    err = iCFFakeEnv->PublishContext( *co3, thread );
    EUNIT_ASSERT_DESC( err == KErrNone, "PublishContext failed!" );

    // There should not be event immediately
    EUNIT_ASSERT_DESC( iCFFakeEnv->ClientEventFired(),
        "Script did not cause event immediately!" );

    // TEST 4: Second context should cause indication at 200 ms
    iCFFakeEnv->ClearFireClientEvent();
    err = iCFFakeEnv->PublishContext( *co2, thread );
    EUNIT_ASSERT_DESC( err == KErrNone, "PublishContext failed!" );

    err = iCFFakeEnv->PublishContext( *co3, thread );
    EUNIT_ASSERT_DESC( err == KErrNone, "PublishContext failed!" );

    // There should not be event at 10 ms
    iTestDelay->Start(10000); // 10 ms
    EUNIT_ASSERT_DESC( !iCFFakeEnv->ClientEventFired(),
        "Script caused event before 10 ms!" );

    // But at 300 ms there should
    iTestDelay->Start(250000); // 10 + 250 ms = 260 ms
    EUNIT_ASSERT_DESC( iCFFakeEnv->ClientEventFired(),
        "Script did NOT give event at 250 ms!" );

    CleanupStack::PopAndDestroy( co3 );
    CleanupStack::PopAndDestroy( co2 );
    CleanupStack::PopAndDestroy( co1 );
    CleanupStack::PopAndDestroy( &thread );
    }

void mt_basicoperationsplugin::MT_ScriptTest_countRepeating_withoutValue_L()
    {
    RThread thread;
    CleanupClosePushL(thread);

    // Define context
    static _LIT_SECURITY_POLICY_PASS( alwaysPass );
    CCFContextObject* co1 = CCFContextObject::NewLC();
    co1->SetSourceL(_L("Test"));
    co1->SetTypeL(_L("Counting"));
    TInt err = iCFFakeEnv->DefineContext( co1->Source(),
            co1->Type(), alwaysPass, alwaysPass );
    EUNIT_ASSERT_DESC( err == KErrNone, "DefineContext failed!" );

    CCFContextObject* co2 = CCFContextObject::NewLC();
    co2->SetSourceL(_L("Test"));
    co2->SetTypeL(_L("Control"));
    err = iCFFakeEnv->DefineContext( co2->Source(),
            co2->Type(), alwaysPass, alwaysPass );
    EUNIT_ASSERT_DESC( err == KErrNone, "DefineContext failed!" );

    // Publish context before loading so that subscribing will find the initial
    // value used to evaluate the operation
    co1->SetValueL(_L("A"));
    iCFFakeEnv->ClearFireClientEventCount();
    iCFFakeEnv->ClearFireClientEvent();
    err = iCFFakeEnv->PublishContext( *co1, thread );
    EUNIT_ASSERT_DESC( err == KErrNone, "PublishContext failed!" );
    EUNIT_ASSERT_DESC( !iCFFakeEnv->ClientEventFired(),
        "Script did cause event!" );

    co2->SetValueL(_L("True"));
    iCFFakeEnv->ClearFireClientEventCount();
    iCFFakeEnv->ClearFireClientEvent();
    err = iCFFakeEnv->PublishContext( *co2, thread );
    EUNIT_ASSERT_DESC( err == KErrNone, "PublishContext failed!" );
    EUNIT_ASSERT_DESC( !iCFFakeEnv->ClientEventFired(),
        "Script did cause event!" );

    // load script
    HBufC8* script = LoadScriptFromFile(_L("test-countRepeatingWithoutValue.xml"));
    TInt id = iCFFakeEnv->AddScript(_L("test-countRepeatingWithoutValue.xml"),
            *script, KMT_CfScriptEngineUid, thread);
    EUNIT_ASSERT_DESC( id >= 0, "Parse error!" );
    delete script;

    iCFFakeEnv->ClearFireClientEventCount();

    // Publish COUNTED context, no event should come
    co1->SetValueL(_L("B"));
    iCFFakeEnv->ClearFireClientEvent();
    err = iCFFakeEnv->PublishContext( *co1, thread );
    EUNIT_ASSERT_DESC( err == KErrNone, "PublishContext failed!" );
    EUNIT_ASSERT_DESC( !iCFFakeEnv->ClientEventFired(),
        "Script did cause event!" );

    // Publish COUNTED context, event MUST come
    co1->SetValueL(_L("A"));
    iCFFakeEnv->ClearFireClientEvent();
    err = iCFFakeEnv->PublishContext( *co1, thread );
    EUNIT_ASSERT_DESC( err == KErrNone, "PublishContext failed!" );
    EUNIT_ASSERT_DESC( iCFFakeEnv->ClientEventFired(),
        "Script did not cause event!" );

    // Publish COUNTED context, no event should come
    co1->SetValueL(_L("B"));
    iCFFakeEnv->ClearFireClientEvent();
    err = iCFFakeEnv->PublishContext( *co1, thread );
    EUNIT_ASSERT_DESC( err == KErrNone, "PublishContext failed!" );
    EUNIT_ASSERT_DESC( !iCFFakeEnv->ClientEventFired(),
        "Script did cause event!" );

    // Publish COUNTED context, event MUST come
    co1->SetValueL(_L("A"));
    iCFFakeEnv->ClearFireClientEvent();
    err = iCFFakeEnv->PublishContext( *co1, thread );
    EUNIT_ASSERT_DESC( err == KErrNone, "PublishContext failed!" );
    EUNIT_ASSERT_DESC( iCFFakeEnv->ClientEventFired(),
        "Script did not cause event!" );

    // Publish CONTROL context, no event should come
    co2->SetValueL(_L("False"));
    iCFFakeEnv->ClearFireClientEventCount();
    iCFFakeEnv->ClearFireClientEvent();
    err = iCFFakeEnv->PublishContext( *co2, thread );
    EUNIT_ASSERT_DESC( err == KErrNone, "PublishContext failed!" );
    EUNIT_ASSERT_DESC( !iCFFakeEnv->ClientEventFired(),
        "Script did cause event!" );

    // Publish CONTROL context, event MUST come
    co2->SetValueL(_L("True"));
    iCFFakeEnv->ClearFireClientEventCount();
    iCFFakeEnv->ClearFireClientEvent();
    err = iCFFakeEnv->PublishContext( *co2, thread );
    EUNIT_ASSERT_DESC( err == KErrNone, "PublishContext failed!" );
    EUNIT_ASSERT_DESC( iCFFakeEnv->ClientEventFired(),
        "Script did not cause event!" );

    // Publish COUNTED context, no event should come
    co1->SetValueL(_L("B"));
    iCFFakeEnv->ClearFireClientEvent();
    err = iCFFakeEnv->PublishContext( *co1, thread );
    EUNIT_ASSERT_DESC( err == KErrNone, "PublishContext failed!" );
    EUNIT_ASSERT_DESC( !iCFFakeEnv->ClientEventFired(),
        "Script did cause event!" );
    
    // Publish CONTROL context, no event should come
    co2->SetValueL(_L("False"));
    iCFFakeEnv->ClearFireClientEventCount();
    iCFFakeEnv->ClearFireClientEvent();
    err = iCFFakeEnv->PublishContext( *co2, thread );
    EUNIT_ASSERT_DESC( err == KErrNone, "PublishContext failed!" );
    EUNIT_ASSERT_DESC( !iCFFakeEnv->ClientEventFired(),
        "Script did cause event!" );

    // Publish CONTROL context, no event should come
    co2->SetValueL(_L("True"));
    iCFFakeEnv->ClearFireClientEventCount();
    iCFFakeEnv->ClearFireClientEvent();
    err = iCFFakeEnv->PublishContext( *co2, thread );
    EUNIT_ASSERT_DESC( err == KErrNone, "PublishContext failed!" );
    EUNIT_ASSERT_DESC( !iCFFakeEnv->ClientEventFired(),
        "Script did cause event!" );

    // Publish COUNTED context, event MUST come
    co1->SetValueL(_L("A"));
    iCFFakeEnv->ClearFireClientEvent();
    err = iCFFakeEnv->PublishContext( *co1, thread );
    EUNIT_ASSERT_DESC( err == KErrNone, "PublishContext failed!" );
    EUNIT_ASSERT_DESC( iCFFakeEnv->ClientEventFired(),
        "Script did not cause event!" );

    // Publish CONTROL context, no event should come
    co2->SetValueL(_L("False"));
    iCFFakeEnv->ClearFireClientEventCount();
    iCFFakeEnv->ClearFireClientEvent();
    err = iCFFakeEnv->PublishContext( *co2, thread );
    EUNIT_ASSERT_DESC( err == KErrNone, "PublishContext failed!" );
    EUNIT_ASSERT_DESC( !iCFFakeEnv->ClientEventFired(),
        "Script did cause event!" );

    // Publish CONTROL context, event MUST come
    co2->SetValueL(_L("True"));
    iCFFakeEnv->ClearFireClientEventCount();
    iCFFakeEnv->ClearFireClientEvent();
    err = iCFFakeEnv->PublishContext( *co2, thread );
    EUNIT_ASSERT_DESC( err == KErrNone, "PublishContext failed!" );
    EUNIT_ASSERT_DESC( iCFFakeEnv->ClientEventFired(),
        "Script did not cause event!" );

    // Publish COUNTED context, no event should come
    co1->SetValueL(_L("B"));
    iCFFakeEnv->ClearFireClientEvent();
    err = iCFFakeEnv->PublishContext( *co1, thread );
    EUNIT_ASSERT_DESC( err == KErrNone, "PublishContext failed!" );
    EUNIT_ASSERT_DESC( !iCFFakeEnv->ClientEventFired(),
        "Script did cause event!" );

    // Publish COUNTED context, event MUST come
    co1->SetValueL(_L("A"));
    iCFFakeEnv->ClearFireClientEvent();
    err = iCFFakeEnv->PublishContext( *co1, thread );
    EUNIT_ASSERT_DESC( err == KErrNone, "PublishContext failed!" );
    EUNIT_ASSERT_DESC( iCFFakeEnv->ClientEventFired(),
        "Script did not cause event!" );

    // Publish COUNTED context, no event should come
    co1->SetValueL(_L("B"));
    iCFFakeEnv->ClearFireClientEvent();
    err = iCFFakeEnv->PublishContext( *co1, thread );
    EUNIT_ASSERT_DESC( err == KErrNone, "PublishContext failed!" );
    EUNIT_ASSERT_DESC( !iCFFakeEnv->ClientEventFired(),
        "Script did cause event!" );

    // Publish CONTROL context, no event should come
    co2->SetValueL(_L("False"));
    iCFFakeEnv->ClearFireClientEventCount();
    iCFFakeEnv->ClearFireClientEvent();
    err = iCFFakeEnv->PublishContext( *co2, thread );
    EUNIT_ASSERT_DESC( err == KErrNone, "PublishContext failed!" );
    EUNIT_ASSERT_DESC( !iCFFakeEnv->ClientEventFired(),
        "Script did cause event!" );

    // Publish CONTROL context, no event should come
    co2->SetValueL(_L("True"));
    iCFFakeEnv->ClearFireClientEventCount();
    iCFFakeEnv->ClearFireClientEvent();
    err = iCFFakeEnv->PublishContext( *co2, thread );
    EUNIT_ASSERT_DESC( err == KErrNone, "PublishContext failed!" );
    EUNIT_ASSERT_DESC( !iCFFakeEnv->ClientEventFired(),
        "Script did cause event!" );

    CleanupStack::PopAndDestroy( co2 );
    CleanupStack::PopAndDestroy( co1 );
    CleanupStack::PopAndDestroy( &thread );
    }

void mt_basicoperationsplugin::MT_ScriptTest_countRepeating_triggerWithoutValue_L()
    {
    RThread thread;
    CleanupClosePushL(thread);

    // Define context
    static _LIT_SECURITY_POLICY_PASS( alwaysPass );
    CCFContextObject* co1 = CCFContextObject::NewLC();
    co1->SetSourceL(_L("Test"));
    co1->SetTypeL(_L("Counting"));
    TInt err = iCFFakeEnv->DefineContext( co1->Source(),
            co1->Type(), alwaysPass, alwaysPass );
    EUNIT_ASSERT_DESC( err == KErrNone, "DefineContext failed!" );

    CCFContextObject* co2 = CCFContextObject::NewLC();
    co2->SetSourceL(_L("Test"));
    co2->SetTypeL(_L("Control"));
    err = iCFFakeEnv->DefineContext( co2->Source(),
            co2->Type(), alwaysPass, alwaysPass );
    EUNIT_ASSERT_DESC( err == KErrNone, "DefineContext failed!" );

    // Publish context before loading so that subscribing will find the initial
    // value used to evaluate the operation
    co1->SetValueL(_L("A"));
    iCFFakeEnv->ClearFireClientEventCount();
    iCFFakeEnv->ClearFireClientEvent();
    err = iCFFakeEnv->PublishContext( *co1, thread );
    EUNIT_ASSERT_DESC( err == KErrNone, "PublishContext failed!" );
    EUNIT_ASSERT_DESC( !iCFFakeEnv->ClientEventFired(),
        "Script did cause event!" );

    co2->SetValueL(_L("True"));
    iCFFakeEnv->ClearFireClientEventCount();
    iCFFakeEnv->ClearFireClientEvent();
    err = iCFFakeEnv->PublishContext( *co2, thread );
    EUNIT_ASSERT_DESC( err == KErrNone, "PublishContext failed!" );
    EUNIT_ASSERT_DESC( !iCFFakeEnv->ClientEventFired(),
        "Script did cause event!" );

    // load script
    HBufC8* script = LoadScriptFromFile(_L("test-countRepeatingWithoutValue2.xml"));
    TInt id = iCFFakeEnv->AddScript(_L("test-countRepeatingWithoutValue2.xml"),
            *script, KMT_CfScriptEngineUid, thread);
    EUNIT_ASSERT_DESC( id >= 0, "Parse error!" );
    delete script;

    iCFFakeEnv->ClearFireClientEventCount();

    // Publish COUNTED context, no event should come
    co1->SetValueL(_L("B"));
    iCFFakeEnv->ClearFireClientEvent();
    err = iCFFakeEnv->PublishContext( *co1, thread );
    EUNIT_ASSERT_DESC( err == KErrNone, "PublishContext failed!" );
    EUNIT_ASSERT_DESC( !iCFFakeEnv->ClientEventFired(),
        "Script did cause event!" );

    // Publish COUNTED context, event MUST come
    co1->SetValueL(_L("A"));
    iCFFakeEnv->ClearFireClientEvent();
    err = iCFFakeEnv->PublishContext( *co1, thread );
    EUNIT_ASSERT_DESC( err == KErrNone, "PublishContext failed!" );
    EUNIT_ASSERT_DESC( iCFFakeEnv->ClientEventFired(),
        "Script did not cause event!" );

    // Publish CONTROL context, no event should come
    co2->SetValueL(_L("True"));
    iCFFakeEnv->ClearFireClientEventCount();
    iCFFakeEnv->ClearFireClientEvent();
    err = iCFFakeEnv->PublishContext( *co2, thread );
    EUNIT_ASSERT_DESC( err == KErrNone, "PublishContext failed!" );
    EUNIT_ASSERT_DESC( !iCFFakeEnv->ClientEventFired(),
        "Script did cause event!" );

    // Publish CONTROL context, no event should come
    co2->SetValueL(_L("False"));
    iCFFakeEnv->ClearFireClientEventCount();
    iCFFakeEnv->ClearFireClientEvent();
    err = iCFFakeEnv->PublishContext( *co2, thread );
    EUNIT_ASSERT_DESC( err == KErrNone, "PublishContext failed!" );
    EUNIT_ASSERT_DESC( !iCFFakeEnv->ClientEventFired(),
        "Script did cause event!" );

    // Publish CONTROL context, no event should come
    co2->SetValueL(_L("True"));
    iCFFakeEnv->ClearFireClientEventCount();
    iCFFakeEnv->ClearFireClientEvent();
    err = iCFFakeEnv->PublishContext( *co2, thread );
    EUNIT_ASSERT_DESC( err == KErrNone, "PublishContext failed!" );
    EUNIT_ASSERT_DESC( !iCFFakeEnv->ClientEventFired(),
        "Script did cause event!" );

    // Publish COUNTED context, no event should come
    co1->SetValueL(_L("B"));
    iCFFakeEnv->ClearFireClientEvent();
    err = iCFFakeEnv->PublishContext( *co1, thread );
    EUNIT_ASSERT_DESC( err == KErrNone, "PublishContext failed!" );
    EUNIT_ASSERT_DESC( !iCFFakeEnv->ClientEventFired(),
        "Script did cause event!" );

    // Publish COUNTED context, event MUST come
    co1->SetValueL(_L("A"));
    iCFFakeEnv->ClearFireClientEvent();
    err = iCFFakeEnv->PublishContext( *co1, thread );
    EUNIT_ASSERT_DESC( err == KErrNone, "PublishContext failed!" );
    EUNIT_ASSERT_DESC( iCFFakeEnv->ClientEventFired(),
        "Script did not cause event!" );

    // Publish CONTROL context, no event should come
    co2->SetValueL(_L("False"));
    iCFFakeEnv->ClearFireClientEventCount();
    iCFFakeEnv->ClearFireClientEvent();
    err = iCFFakeEnv->PublishContext( *co2, thread );
    EUNIT_ASSERT_DESC( err == KErrNone, "PublishContext failed!" );
    EUNIT_ASSERT_DESC( !iCFFakeEnv->ClientEventFired(),
        "Script did cause event!" );

    // Publish CONTROL context, no event should come
    co2->SetValueL(_L("True"));
    iCFFakeEnv->ClearFireClientEventCount();
    iCFFakeEnv->ClearFireClientEvent();
    err = iCFFakeEnv->PublishContext( *co2, thread );
    EUNIT_ASSERT_DESC( err == KErrNone, "PublishContext failed!" );
    EUNIT_ASSERT_DESC( !iCFFakeEnv->ClientEventFired(),
        "Script did cause event!" );

    // Publish COUNTED context, no event should come
    co1->SetValueL(_L("B"));
    iCFFakeEnv->ClearFireClientEvent();
    err = iCFFakeEnv->PublishContext( *co1, thread );
    EUNIT_ASSERT_DESC( err == KErrNone, "PublishContext failed!" );
    EUNIT_ASSERT_DESC( !iCFFakeEnv->ClientEventFired(),
        "Script did cause event!" );
    
    // Publish CONTROL context, no event should come
    co2->SetValueL(_L("False"));
    iCFFakeEnv->ClearFireClientEventCount();
    iCFFakeEnv->ClearFireClientEvent();
    err = iCFFakeEnv->PublishContext( *co2, thread );
    EUNIT_ASSERT_DESC( err == KErrNone, "PublishContext failed!" );
    EUNIT_ASSERT_DESC( !iCFFakeEnv->ClientEventFired(),
        "Script did cause event!" );

    // Publish CONTROL context, no event should come
    co2->SetValueL(_L("True"));
    iCFFakeEnv->ClearFireClientEventCount();
    iCFFakeEnv->ClearFireClientEvent();
    err = iCFFakeEnv->PublishContext( *co2, thread );
    EUNIT_ASSERT_DESC( err == KErrNone, "PublishContext failed!" );
    EUNIT_ASSERT_DESC( !iCFFakeEnv->ClientEventFired(),
        "Script did cause event!" );

    // Publish CONTROL context, no event should come
    co2->SetValueL(_L("False"));
    iCFFakeEnv->ClearFireClientEventCount();
    iCFFakeEnv->ClearFireClientEvent();
    err = iCFFakeEnv->PublishContext( *co2, thread );
    EUNIT_ASSERT_DESC( err == KErrNone, "PublishContext failed!" );
    EUNIT_ASSERT_DESC( !iCFFakeEnv->ClientEventFired(),
        "Script did cause event!" );

    // Publish CONTROL context, no event should come
    co1->SetValueL(_L("A"));
    iCFFakeEnv->ClearFireClientEvent();
    err = iCFFakeEnv->PublishContext( *co1, thread );
    EUNIT_ASSERT_DESC( err == KErrNone, "PublishContext failed!" );
    EUNIT_ASSERT_DESC( !iCFFakeEnv->ClientEventFired(),
        "Script did cause event!" );

    // Publish CONTROL context, no event should come
    co2->SetValueL(_L("True"));
    iCFFakeEnv->ClearFireClientEventCount();
    iCFFakeEnv->ClearFireClientEvent();
    err = iCFFakeEnv->PublishContext( *co2, thread );
    EUNIT_ASSERT_DESC( err == KErrNone, "PublishContext failed!" );
    EUNIT_ASSERT_DESC( !iCFFakeEnv->ClientEventFired(),
        "Script did cause event!" );

    // Publish COUNTED context, no event should come
    co1->SetValueL(_L("B"));
    iCFFakeEnv->ClearFireClientEvent();
    err = iCFFakeEnv->PublishContext( *co1, thread );
    EUNIT_ASSERT_DESC( err == KErrNone, "PublishContext failed!" );
    EUNIT_ASSERT_DESC( !iCFFakeEnv->ClientEventFired(),
        "Script did cause event!" );

    // Publish COUNTED context, event MUST come
    co1->SetValueL(_L("A"));
    iCFFakeEnv->ClearFireClientEvent();
    err = iCFFakeEnv->PublishContext( *co1, thread );
    EUNIT_ASSERT_DESC( err == KErrNone, "PublishContext failed!" );
    EUNIT_ASSERT_DESC( iCFFakeEnv->ClientEventFired(),
        "Script did not cause event!" );

    // Publish COUNTED context, no event should come
    co1->SetValueL(_L("B"));
    iCFFakeEnv->ClearFireClientEvent();
    err = iCFFakeEnv->PublishContext( *co1, thread );
    EUNIT_ASSERT_DESC( err == KErrNone, "PublishContext failed!" );
    EUNIT_ASSERT_DESC( !iCFFakeEnv->ClientEventFired(),
        "Script did cause event!" );

    // Publish CONTROL context, no event should come
    co2->SetValueL(_L("False"));
    iCFFakeEnv->ClearFireClientEventCount();
    iCFFakeEnv->ClearFireClientEvent();
    err = iCFFakeEnv->PublishContext( *co2, thread );
    EUNIT_ASSERT_DESC( err == KErrNone, "PublishContext failed!" );
    EUNIT_ASSERT_DESC( !iCFFakeEnv->ClientEventFired(),
        "Script did cause event!" );

    // Publish CONTROL context, no event should come
    co2->SetValueL(_L("True"));
    iCFFakeEnv->ClearFireClientEventCount();
    iCFFakeEnv->ClearFireClientEvent();
    err = iCFFakeEnv->PublishContext( *co2, thread );
    EUNIT_ASSERT_DESC( err == KErrNone, "PublishContext failed!" );
    EUNIT_ASSERT_DESC( !iCFFakeEnv->ClientEventFired(),
        "Script did cause event!" );

    CleanupStack::PopAndDestroy( co2 );
    CleanupStack::PopAndDestroy( co1 );
    CleanupStack::PopAndDestroy( &thread );
    }

void mt_basicoperationsplugin::MT_ScriptTest_countRepeating_withValue_L()
    {
    RThread thread;
    CleanupClosePushL(thread);

    // Define context
    static _LIT_SECURITY_POLICY_PASS( alwaysPass );
    CCFContextObject* co1 = CCFContextObject::NewLC();
    co1->SetSourceL(_L("Test"));
    co1->SetTypeL(_L("Counting"));
    TInt err = iCFFakeEnv->DefineContext( co1->Source(),
            co1->Type(), alwaysPass, alwaysPass );
    EUNIT_ASSERT_DESC( err == KErrNone, "DefineContext failed!" );

    CCFContextObject* co2 = CCFContextObject::NewLC();
    co2->SetSourceL(_L("Test"));
    co2->SetTypeL(_L("Control"));
    err = iCFFakeEnv->DefineContext( co2->Source(),
            co2->Type(), alwaysPass, alwaysPass );
    EUNIT_ASSERT_DESC( err == KErrNone, "DefineContext failed!" );

    // Publish context before loading so that subscribing will find the initial
    // value used to evaluate the operation
    co1->SetValueL(_L("A"));
    iCFFakeEnv->ClearFireClientEventCount();
    iCFFakeEnv->ClearFireClientEvent();
    err = iCFFakeEnv->PublishContext( *co1, thread );
    EUNIT_ASSERT_DESC( err == KErrNone, "PublishContext failed!" );
    EUNIT_ASSERT_DESC( !iCFFakeEnv->ClientEventFired(),
        "Script did cause event!" );

    co2->SetValueL(_L("True"));
    iCFFakeEnv->ClearFireClientEventCount();
    iCFFakeEnv->ClearFireClientEvent();
    err = iCFFakeEnv->PublishContext( *co2, thread );
    EUNIT_ASSERT_DESC( err == KErrNone, "PublishContext failed!" );
    EUNIT_ASSERT_DESC( !iCFFakeEnv->ClientEventFired(),
        "Script did cause event!" );

    // load script
    HBufC8* script = LoadScriptFromFile(_L("test-countRepeatingWithValue.xml"));
    TInt id = iCFFakeEnv->AddScript(_L("test-countRepeatingWithValue.xml"),
            *script, KMT_CfScriptEngineUid, thread);
    EUNIT_ASSERT_DESC( id >= 0, "Parse error!" );
    delete script;

    iCFFakeEnv->ClearFireClientEventCount();

    // Publish COUNTED context, no event should come
    co1->SetValueL(_L("B"));
    iCFFakeEnv->ClearFireClientEvent();
    err = iCFFakeEnv->PublishContext( *co1, thread );
    EUNIT_ASSERT_DESC( err == KErrNone, "PublishContext failed!" );
    EUNIT_ASSERT_DESC( !iCFFakeEnv->ClientEventFired(),
        "Script did cause event!" );

    // Publish COUNTED context, no event should come
    co1->SetValueL(_L("A"));
    iCFFakeEnv->ClearFireClientEvent();
    err = iCFFakeEnv->PublishContext( *co1, thread );
    EUNIT_ASSERT_DESC( err == KErrNone, "PublishContext failed!" );
    EUNIT_ASSERT_DESC( !iCFFakeEnv->ClientEventFired(),
        "Script did cause event!" );

    // Publish COUNTED context, no event should come
    co1->SetValueL(_L("B"));
    iCFFakeEnv->ClearFireClientEvent();
    err = iCFFakeEnv->PublishContext( *co1, thread );
    EUNIT_ASSERT_DESC( err == KErrNone, "PublishContext failed!" );
    EUNIT_ASSERT_DESC( !iCFFakeEnv->ClientEventFired(),
        "Script did cause event!" );

    // Publish COUNTED context, event MUST come
    co1->SetValueL(_L("A"));
    iCFFakeEnv->ClearFireClientEvent();
    err = iCFFakeEnv->PublishContext( *co1, thread );
    EUNIT_ASSERT_DESC( err == KErrNone, "PublishContext failed!" );
    EUNIT_ASSERT_DESC( iCFFakeEnv->ClientEventFired(),
        "Script did not cause event!" );

    // Publish COUNTED context, no event should come
    co1->SetValueL(_L("B"));
    iCFFakeEnv->ClearFireClientEvent();
    err = iCFFakeEnv->PublishContext( *co1, thread );
    EUNIT_ASSERT_DESC( err == KErrNone, "PublishContext failed!" );
    EUNIT_ASSERT_DESC( !iCFFakeEnv->ClientEventFired(),
        "Script did cause event!" );
    
    // Publish CONTROL context, no event should come
    co2->SetValueL(_L("False"));
    iCFFakeEnv->ClearFireClientEventCount();
    iCFFakeEnv->ClearFireClientEvent();
    err = iCFFakeEnv->PublishContext( *co2, thread );
    EUNIT_ASSERT_DESC( err == KErrNone, "PublishContext failed!" );
    EUNIT_ASSERT_DESC( !iCFFakeEnv->ClientEventFired(),
        "Script did cause event!" );

    // Publish CONTROL context, event MUST come
    co2->SetValueL(_L("True"));
    iCFFakeEnv->ClearFireClientEventCount();
    iCFFakeEnv->ClearFireClientEvent();
    err = iCFFakeEnv->PublishContext( *co2, thread );
    EUNIT_ASSERT_DESC( err == KErrNone, "PublishContext failed!" );
    EUNIT_ASSERT_DESC( iCFFakeEnv->ClientEventFired(),
        "Script did not cause event!" );

    // Publish COUNTED context, no event should come
    co1->SetValueL(_L("A"));
    iCFFakeEnv->ClearFireClientEvent();
    err = iCFFakeEnv->PublishContext( *co1, thread );
    EUNIT_ASSERT_DESC( err == KErrNone, "PublishContext failed!" );
    EUNIT_ASSERT_DESC( !iCFFakeEnv->ClientEventFired(),
        "Script did cause event!" );

    // Publish CONTROL context, no event should come
    co2->SetValueL(_L("False"));
    iCFFakeEnv->ClearFireClientEventCount();
    iCFFakeEnv->ClearFireClientEvent();
    err = iCFFakeEnv->PublishContext( *co2, thread );
    EUNIT_ASSERT_DESC( err == KErrNone, "PublishContext failed!" );
    EUNIT_ASSERT_DESC( !iCFFakeEnv->ClientEventFired(),
        "Script did cause event!" );

    // Publish CONTROL context, no event should come
    co2->SetValueL(_L("True"));
    iCFFakeEnv->ClearFireClientEventCount();
    iCFFakeEnv->ClearFireClientEvent();
    err = iCFFakeEnv->PublishContext( *co2, thread );
    EUNIT_ASSERT_DESC( err == KErrNone, "PublishContext failed!" );
    EUNIT_ASSERT_DESC( !iCFFakeEnv->ClientEventFired(),
        "Script did cause event!" );

    // Publish COUNTED context, no event should come
    co1->SetValueL(_L("B"));
    iCFFakeEnv->ClearFireClientEvent();
    err = iCFFakeEnv->PublishContext( *co1, thread );
    EUNIT_ASSERT_DESC( err == KErrNone, "PublishContext failed!" );
    EUNIT_ASSERT_DESC( !iCFFakeEnv->ClientEventFired(),
        "Script did cause event!" );

    // Publish COUNTED context, event MUST come
    co1->SetValueL(_L("A"));
    iCFFakeEnv->ClearFireClientEvent();
    err = iCFFakeEnv->PublishContext( *co1, thread );
    EUNIT_ASSERT_DESC( err == KErrNone, "PublishContext failed!" );
    EUNIT_ASSERT_DESC( iCFFakeEnv->ClientEventFired(),
        "Script did not cause event!" );

    // Publish COUNTED context, no event should come
    co1->SetValueL(_L("B"));
    iCFFakeEnv->ClearFireClientEvent();
    err = iCFFakeEnv->PublishContext( *co1, thread );
    EUNIT_ASSERT_DESC( err == KErrNone, "PublishContext failed!" );
    EUNIT_ASSERT_DESC( !iCFFakeEnv->ClientEventFired(),
        "Script did cause event!" );

    // Publish CONTROL context, no event should come
    co2->SetValueL(_L("False"));
    iCFFakeEnv->ClearFireClientEventCount();
    iCFFakeEnv->ClearFireClientEvent();
    err = iCFFakeEnv->PublishContext( *co2, thread );
    EUNIT_ASSERT_DESC( err == KErrNone, "PublishContext failed!" );
    EUNIT_ASSERT_DESC( !iCFFakeEnv->ClientEventFired(),
        "Script did cause event!" );

    // Publish CONTROL context, event MUST come
    co2->SetValueL(_L("True"));
    iCFFakeEnv->ClearFireClientEventCount();
    iCFFakeEnv->ClearFireClientEvent();
    err = iCFFakeEnv->PublishContext( *co2, thread );
    EUNIT_ASSERT_DESC( err == KErrNone, "PublishContext failed!" );
    EUNIT_ASSERT_DESC( iCFFakeEnv->ClientEventFired(),
        "Script did not cause event!" );

    CleanupStack::PopAndDestroy( co2 );
    CleanupStack::PopAndDestroy( co1 );
    CleanupStack::PopAndDestroy( &thread );
    }

void mt_basicoperationsplugin::MT_ScriptTest_countOneShot_withoutValue_L()
    {
    RThread thread;
    CleanupClosePushL(thread);

    // Define context
    static _LIT_SECURITY_POLICY_PASS( alwaysPass );
    CCFContextObject* co1 = CCFContextObject::NewLC();
    co1->SetSourceL(_L("Test"));
    co1->SetTypeL(_L("Counting"));
    TInt err = iCFFakeEnv->DefineContext( co1->Source(),
            co1->Type(), alwaysPass, alwaysPass );
    EUNIT_ASSERT_DESC( err == KErrNone, "DefineContext failed!" );

    // Publish context before loading so that subscribing will find the initial
    // value used to evaluate the operation
    co1->SetValueL(_L("A"));
    iCFFakeEnv->ClearFireClientEventCount();
    iCFFakeEnv->ClearFireClientEvent();
    err = iCFFakeEnv->PublishContext( *co1, thread );
    EUNIT_ASSERT_DESC( err == KErrNone, "PublishContext failed!" );
    EUNIT_ASSERT_DESC( !iCFFakeEnv->ClientEventFired(),
        "Script did cause event although interval was not met!" );

    // load script
    HBufC8* script = LoadScriptFromFile(_L("test-countOneShotWithoutValue.xml"));
    TInt id = iCFFakeEnv->AddScript(_L("test-countOneShotWithoutValue.xml"),
            *script, KMT_CfScriptEngineUid, thread);
    EUNIT_ASSERT_DESC( id >= 0, "Parse error!" );
    delete script;


    iCFFakeEnv->ClearFireClientEventCount();

    // Publish COUNTED context, no event should come
    co1->SetValueL(_L("B"));
    iCFFakeEnv->ClearFireClientEvent();
    err = iCFFakeEnv->PublishContext( *co1, thread );
    EUNIT_ASSERT_DESC( err == KErrNone, "PublishContext failed!" );
    EUNIT_ASSERT_DESC( !iCFFakeEnv->ClientEventFired(),
        "Script did cause event!" );

    // Publish COUNTED context, event MUST come
    co1->SetValueL(_L("A"));
    iCFFakeEnv->ClearFireClientEvent();
    err = iCFFakeEnv->PublishContext( *co1, thread );
    EUNIT_ASSERT_DESC( err == KErrNone, "PublishContext failed!" );
    EUNIT_ASSERT_DESC( iCFFakeEnv->ClientEventFired(),
        "Script did not cause event!" );

    // Publish COUNTED context, no event should come
    co1->SetValueL(_L("B"));
    iCFFakeEnv->ClearFireClientEvent();
    err = iCFFakeEnv->PublishContext( *co1, thread );
    EUNIT_ASSERT_DESC( err == KErrNone, "PublishContext failed!" );
    EUNIT_ASSERT_DESC( !iCFFakeEnv->ClientEventFired(),
        "Script did cause event!" );

    // Publish COUNTED context, no event should come
    co1->SetValueL(_L("A"));
    iCFFakeEnv->ClearFireClientEvent();
    err = iCFFakeEnv->PublishContext( *co1, thread );
    EUNIT_ASSERT_DESC( err == KErrNone, "PublishContext failed!" );
    EUNIT_ASSERT_DESC( !iCFFakeEnv->ClientEventFired(),
        "Script did cause event!" );

    // Publish COUNTED context, no event should come
    co1->SetValueL(_L("B"));
    iCFFakeEnv->ClearFireClientEvent();
    err = iCFFakeEnv->PublishContext( *co1, thread );
    EUNIT_ASSERT_DESC( err == KErrNone, "PublishContext failed!" );
    EUNIT_ASSERT_DESC( !iCFFakeEnv->ClientEventFired(),
        "Script did cause event!" );

    // Publish COUNTED context, no event should come
    co1->SetValueL(_L("A"));
    iCFFakeEnv->ClearFireClientEvent();
    err = iCFFakeEnv->PublishContext( *co1, thread );
    EUNIT_ASSERT_DESC( err == KErrNone, "PublishContext failed!" );
    EUNIT_ASSERT_DESC( !iCFFakeEnv->ClientEventFired(),
        "Script did cause event!" );

    // Publish COUNTED context, no event should come
    co1->SetValueL(_L("B"));
    iCFFakeEnv->ClearFireClientEvent();
    err = iCFFakeEnv->PublishContext( *co1, thread );
    EUNIT_ASSERT_DESC( err == KErrNone, "PublishContext failed!" );
    EUNIT_ASSERT_DESC( !iCFFakeEnv->ClientEventFired(),
        "Script did cause event!" );

    // Publish COUNTED context, no event should come
    co1->SetValueL(_L("A"));
    iCFFakeEnv->ClearFireClientEvent();
    err = iCFFakeEnv->PublishContext( *co1, thread );
    EUNIT_ASSERT_DESC( err == KErrNone, "PublishContext failed!" );
    EUNIT_ASSERT_DESC( !iCFFakeEnv->ClientEventFired(),
        "Script did cause event!" );

    CleanupStack::PopAndDestroy( co1 );
    CleanupStack::PopAndDestroy( &thread );
    }

void mt_basicoperationsplugin::MT_ScriptTest_countOneShot_withValue_L()
    {
    RThread thread;
    CleanupClosePushL(thread);

    // Define context
    static _LIT_SECURITY_POLICY_PASS( alwaysPass );
    CCFContextObject* co1 = CCFContextObject::NewLC();
    co1->SetSourceL(_L("Test"));
    co1->SetTypeL(_L("Counting"));
    TInt err = iCFFakeEnv->DefineContext( co1->Source(),
            co1->Type(), alwaysPass, alwaysPass );
    EUNIT_ASSERT_DESC( err == KErrNone, "DefineContext failed!" );

    // Publish context before loading so that subscribing will find the initial
    // value used to evaluate the operation
    co1->SetValueL(_L("A"));
    iCFFakeEnv->ClearFireClientEventCount();
    iCFFakeEnv->ClearFireClientEvent();
    err = iCFFakeEnv->PublishContext( *co1, thread );
    EUNIT_ASSERT_DESC( err == KErrNone, "PublishContext failed!" );
    EUNIT_ASSERT_DESC( !iCFFakeEnv->ClientEventFired(),
        "Script did cause event although interval was not met!" );

    // load script
    HBufC8* script = LoadScriptFromFile(_L("test-countOneShotWithValue.xml"));
    TInt id = iCFFakeEnv->AddScript(_L("test-countOneShotWithValue.xml"),
            *script, KMT_CfScriptEngineUid, thread);
    EUNIT_ASSERT_DESC( id >= 0, "Parse error!" );
    delete script;

    iCFFakeEnv->ClearFireClientEventCount();

    // Publish COUNTED context, no event should come
    co1->SetValueL(_L("B"));
    iCFFakeEnv->ClearFireClientEvent();
    err = iCFFakeEnv->PublishContext( *co1, thread );
    EUNIT_ASSERT_DESC( err == KErrNone, "PublishContext failed!" );
    EUNIT_ASSERT_DESC( !iCFFakeEnv->ClientEventFired(),
        "Script did cause event!" );

    // Publish COUNTED context, no event should come
    co1->SetValueL(_L("A"));
    iCFFakeEnv->ClearFireClientEvent();
    err = iCFFakeEnv->PublishContext( *co1, thread );
    EUNIT_ASSERT_DESC( err == KErrNone, "PublishContext failed!" );
    EUNIT_ASSERT_DESC( !iCFFakeEnv->ClientEventFired(),
        "Script did cause event!" );

    // Publish COUNTED context, no event should come
    co1->SetValueL(_L("B"));
    iCFFakeEnv->ClearFireClientEvent();
    err = iCFFakeEnv->PublishContext( *co1, thread );
    EUNIT_ASSERT_DESC( err == KErrNone, "PublishContext failed!" );
    EUNIT_ASSERT_DESC( !iCFFakeEnv->ClientEventFired(),
        "Script did cause event!" );

    // Publish COUNTED context, event MUST come
    co1->SetValueL(_L("A"));
    iCFFakeEnv->ClearFireClientEvent();
    err = iCFFakeEnv->PublishContext( *co1, thread );
    EUNIT_ASSERT_DESC( err == KErrNone, "PublishContext failed!" );
    EUNIT_ASSERT_DESC( iCFFakeEnv->ClientEventFired(),
        "Script did not cause event!" );

    // Publish COUNTED context, no event should come
    co1->SetValueL(_L("B"));
    iCFFakeEnv->ClearFireClientEvent();
    err = iCFFakeEnv->PublishContext( *co1, thread );
    EUNIT_ASSERT_DESC( err == KErrNone, "PublishContext failed!" );
    EUNIT_ASSERT_DESC( !iCFFakeEnv->ClientEventFired(),
        "Script did cause event!" );

    // Publish COUNTED context, no event should come
    co1->SetValueL(_L("A"));
    iCFFakeEnv->ClearFireClientEvent();
    err = iCFFakeEnv->PublishContext( *co1, thread );
    EUNIT_ASSERT_DESC( err == KErrNone, "PublishContext failed!" );
    EUNIT_ASSERT_DESC( !iCFFakeEnv->ClientEventFired(),
        "Script did cause event!" );

    // Publish COUNTED context, no event should come
    co1->SetValueL(_L("B"));
    iCFFakeEnv->ClearFireClientEvent();
    err = iCFFakeEnv->PublishContext( *co1, thread );
    EUNIT_ASSERT_DESC( err == KErrNone, "PublishContext failed!" );
    EUNIT_ASSERT_DESC( !iCFFakeEnv->ClientEventFired(),
        "Script did cause event!" );

    // Publish COUNTED context, no event should come
    co1->SetValueL(_L("A"));
    iCFFakeEnv->ClearFireClientEvent();
    err = iCFFakeEnv->PublishContext( *co1, thread );
    EUNIT_ASSERT_DESC( err == KErrNone, "PublishContext failed!" );
    EUNIT_ASSERT_DESC( !iCFFakeEnv->ClientEventFired(),
        "Script did cause event!" );

    CleanupStack::PopAndDestroy( co1 );
    CleanupStack::PopAndDestroy( &thread );
    }

void mt_basicoperationsplugin::MT_ScriptTest_countOneShot_remainWithValue_duration_L()
    {
    RThread thread;
    CleanupClosePushL(thread);

    // Define context
    static _LIT_SECURITY_POLICY_PASS( alwaysPass );
    CCFContextObject* co1 = CCFContextObject::NewLC();
    co1->SetSourceL(_L("Test"));
    co1->SetTypeL(_L("Counting"));
    TInt err = iCFFakeEnv->DefineContext( co1->Source(),
            co1->Type(), alwaysPass, alwaysPass );
    EUNIT_ASSERT_DESC( err == KErrNone, "DefineContext failed!" );

    CCFContextObject* co2 = CCFContextObject::NewLC();
    co2->SetSourceL(_L("Test"));
    co2->SetTypeL(_L("Control"));
    err = iCFFakeEnv->DefineContext( co2->Source(),
            co2->Type(), alwaysPass, alwaysPass );
    EUNIT_ASSERT_DESC( err == KErrNone, "DefineContext failed!" );

    // Publish context before loading so that subscribing will find the initial
    // value used to evaluate the operation
    co1->SetValueL(_L("A"));
    iCFFakeEnv->ClearFireClientEventCount();
    iCFFakeEnv->ClearFireClientEvent();
    err = iCFFakeEnv->PublishContext( *co1, thread );
    EUNIT_ASSERT_DESC( err == KErrNone, "PublishContext failed!" );
    EUNIT_ASSERT_DESC( !iCFFakeEnv->ClientEventFired(),
        "Script did cause event!" );

    co2->SetValueL(_L("True"));
    iCFFakeEnv->ClearFireClientEventCount();
    iCFFakeEnv->ClearFireClientEvent();
    err = iCFFakeEnv->PublishContext( *co2, thread );
    EUNIT_ASSERT_DESC( err == KErrNone, "PublishContext failed!" );
    EUNIT_ASSERT_DESC( !iCFFakeEnv->ClientEventFired(),
        "Script did cause event!" );

    // load script
    HBufC8* script = LoadScriptFromFile(_L("test-countOneShotWithValueDuration.xml"));
    TInt id = iCFFakeEnv->AddScript(_L("test-countOneShotWithValueDuration.xml"),
            *script, KMT_CfScriptEngineUid, thread);
    EUNIT_ASSERT_DESC( id >= 0, "Parse error!" );
    delete script;

    iCFFakeEnv->ClearFireClientEventCount();

    // Publish COUNTED context, no event should come
    co1->SetValueL(_L("B"));
    iCFFakeEnv->ClearFireClientEvent();
    err = iCFFakeEnv->PublishContext( *co1, thread );
    EUNIT_ASSERT_DESC( err == KErrNone, "PublishContext failed!" );
    EUNIT_ASSERT_DESC( !iCFFakeEnv->ClientEventFired(),
        "Script did cause event!" );

    // Publish COUNTED context, no event should come
    co1->SetValueL(_L("A"));
    iCFFakeEnv->ClearFireClientEvent();
    err = iCFFakeEnv->PublishContext( *co1, thread );
    EUNIT_ASSERT_DESC( err == KErrNone, "PublishContext failed!" );
    EUNIT_ASSERT_DESC( !iCFFakeEnv->ClientEventFired(),
        "Script did cause event!" );

    // Publish COUNTED context, no event should come
    co1->SetValueL(_L("B"));
    iCFFakeEnv->ClearFireClientEvent();
    err = iCFFakeEnv->PublishContext( *co1, thread );
    EUNIT_ASSERT_DESC( err == KErrNone, "PublishContext failed!" );
    EUNIT_ASSERT_DESC( !iCFFakeEnv->ClientEventFired(),
        "Script did cause event!" );

    // Publish COUNTED context, event MUST come
    co1->SetValueL(_L("A"));
    iCFFakeEnv->ClearFireClientEvent();
    err = iCFFakeEnv->PublishContext( *co1, thread );
    EUNIT_ASSERT_DESC( err == KErrNone, "PublishContext failed!" );
    EUNIT_ASSERT_DESC( iCFFakeEnv->ClientEventFired(),
        "Script did not cause event!" );

    // Publish COUNTED context, no event should come
    co1->SetValueL(_L("B"));
    iCFFakeEnv->ClearFireClientEvent();
    err = iCFFakeEnv->PublishContext( *co1, thread );
    EUNIT_ASSERT_DESC( err == KErrNone, "PublishContext failed!" );
    EUNIT_ASSERT_DESC( !iCFFakeEnv->ClientEventFired(),
        "Script did cause event!" );
    
    // Publish CONTROL context, no event should come
    co2->SetValueL(_L("False"));
    iCFFakeEnv->ClearFireClientEventCount();
    iCFFakeEnv->ClearFireClientEvent();
    err = iCFFakeEnv->PublishContext( *co2, thread );
    EUNIT_ASSERT_DESC( err == KErrNone, "PublishContext failed!" );
    EUNIT_ASSERT_DESC( !iCFFakeEnv->ClientEventFired(),
        "Script did cause event!" );

    // Publish CONTROL context, event MUST come
    co2->SetValueL(_L("True"));
    iCFFakeEnv->ClearFireClientEventCount();
    iCFFakeEnv->ClearFireClientEvent();
    err = iCFFakeEnv->PublishContext( *co2, thread );
    EUNIT_ASSERT_DESC( err == KErrNone, "PublishContext failed!" );
    EUNIT_ASSERT_DESC( iCFFakeEnv->ClientEventFired(),
        "Script did not cause event!" );

    // Publish COUNTED context, no event should come
    co1->SetValueL(_L("A"));
    iCFFakeEnv->ClearFireClientEvent();
    err = iCFFakeEnv->PublishContext( *co1, thread );
    EUNIT_ASSERT_DESC( err == KErrNone, "PublishContext failed!" );
    EUNIT_ASSERT_DESC( !iCFFakeEnv->ClientEventFired(),
        "Script did cause event!" );

    // Publish COUNTED context, no event should come
    co1->SetValueL(_L("B"));
    iCFFakeEnv->ClearFireClientEvent();
    err = iCFFakeEnv->PublishContext( *co1, thread );
    EUNIT_ASSERT_DESC( err == KErrNone, "PublishContext failed!" );
    EUNIT_ASSERT_DESC( !iCFFakeEnv->ClientEventFired(),
        "Script did cause event!" );

    // Publish COUNTED context, no event should come
    co1->SetValueL(_L("A"));
    iCFFakeEnv->ClearFireClientEvent();
    err = iCFFakeEnv->PublishContext( *co1, thread );
    EUNIT_ASSERT_DESC( err == KErrNone, "PublishContext failed!" );
    EUNIT_ASSERT_DESC( !iCFFakeEnv->ClientEventFired(),
        "Script did cause event!" );

    // Publish CONTROL context, no event should come
    co2->SetValueL(_L("False"));
    iCFFakeEnv->ClearFireClientEventCount();
    iCFFakeEnv->ClearFireClientEvent();
    err = iCFFakeEnv->PublishContext( *co2, thread );
    EUNIT_ASSERT_DESC( err == KErrNone, "PublishContext failed!" );
    EUNIT_ASSERT_DESC( !iCFFakeEnv->ClientEventFired(),
        "Script did cause event!" );

    // Publish CONTROL context, event MUST come
    co2->SetValueL(_L("True"));
    iCFFakeEnv->ClearFireClientEventCount();
    iCFFakeEnv->ClearFireClientEvent();
    err = iCFFakeEnv->PublishContext( *co2, thread );
    EUNIT_ASSERT_DESC( err == KErrNone, "PublishContext failed!" );
    EUNIT_ASSERT_DESC( iCFFakeEnv->ClientEventFired(),
        "Script did not cause event!" );

    CleanupStack::PopAndDestroy( co2 );
    CleanupStack::PopAndDestroy( co1 );
    CleanupStack::PopAndDestroy( &thread );
    }

void mt_basicoperationsplugin::MT_ScriptTest_countOneShot_changeWithValue_duration_L()
    {
    RThread thread;
    CleanupClosePushL(thread);

    // Define context
    static _LIT_SECURITY_POLICY_PASS( alwaysPass );
    CCFContextObject* co1 = CCFContextObject::NewLC();
    co1->SetSourceL(_L("Test"));
    co1->SetTypeL(_L("Counting"));
    TInt err = iCFFakeEnv->DefineContext( co1->Source(),
            co1->Type(), alwaysPass, alwaysPass );
    EUNIT_ASSERT_DESC( err == KErrNone, "DefineContext failed!" );

    CCFContextObject* co2 = CCFContextObject::NewLC();
    co2->SetSourceL(_L("Test"));
    co2->SetTypeL(_L("Control"));
    err = iCFFakeEnv->DefineContext( co2->Source(),
            co2->Type(), alwaysPass, alwaysPass );
    EUNIT_ASSERT_DESC( err == KErrNone, "DefineContext failed!" );

    // Publish context before loading so that subscribing will find the initial
    // value used to evaluate the operation
    co1->SetValueL(_L("A"));
    iCFFakeEnv->ClearFireClientEventCount();
    iCFFakeEnv->ClearFireClientEvent();
    err = iCFFakeEnv->PublishContext( *co1, thread );
    EUNIT_ASSERT_DESC( err == KErrNone, "PublishContext failed!" );
    EUNIT_ASSERT_DESC( !iCFFakeEnv->ClientEventFired(),
        "Script did cause event!" );

    co2->SetValueL(_L("True"));
    iCFFakeEnv->ClearFireClientEventCount();
    iCFFakeEnv->ClearFireClientEvent();
    err = iCFFakeEnv->PublishContext( *co2, thread );
    EUNIT_ASSERT_DESC( err == KErrNone, "PublishContext failed!" );
    EUNIT_ASSERT_DESC( !iCFFakeEnv->ClientEventFired(),
        "Script did cause event!" );

    // load script
    HBufC8* script = LoadScriptFromFile(_L("test-countOneShotWithValueDuration2.xml"));
    TInt id = iCFFakeEnv->AddScript(_L("test-countOneShotWithValueDuration2.xml"),
            *script, KMT_CfScriptEngineUid, thread);
    EUNIT_ASSERT_DESC( id >= 0, "Parse error!" );
    delete script;

    iCFFakeEnv->ClearFireClientEventCount();

    // Publish COUNTED context, no event should come
    co1->SetValueL(_L("B"));
    iCFFakeEnv->ClearFireClientEvent();
    err = iCFFakeEnv->PublishContext( *co1, thread );
    EUNIT_ASSERT_DESC( err == KErrNone, "PublishContext failed!" );
    EUNIT_ASSERT_DESC( !iCFFakeEnv->ClientEventFired(),
        "Script did cause event!" );

    // Publish COUNTED context, no event should come
    co1->SetValueL(_L("A"));
    iCFFakeEnv->ClearFireClientEvent();
    err = iCFFakeEnv->PublishContext( *co1, thread );
    EUNIT_ASSERT_DESC( err == KErrNone, "PublishContext failed!" );
    EUNIT_ASSERT_DESC( !iCFFakeEnv->ClientEventFired(),
        "Script did cause event!" );

    // Publish COUNTED context, no event should come
    co1->SetValueL(_L("B"));
    iCFFakeEnv->ClearFireClientEvent();
    err = iCFFakeEnv->PublishContext( *co1, thread );
    EUNIT_ASSERT_DESC( err == KErrNone, "PublishContext failed!" );
    EUNIT_ASSERT_DESC( !iCFFakeEnv->ClientEventFired(),
        "Script did cause event!" );

    // Publish COUNTED context, event MUST come
    co1->SetValueL(_L("A"));
    iCFFakeEnv->ClearFireClientEvent();
    err = iCFFakeEnv->PublishContext( *co1, thread );
    EUNIT_ASSERT_DESC( err == KErrNone, "PublishContext failed!" );
    EUNIT_ASSERT_DESC( iCFFakeEnv->ClientEventFired(),
        "Script did not cause event!" );

    // Publish COUNTED context, no event should come
    co1->SetValueL(_L("B"));
    iCFFakeEnv->ClearFireClientEvent();
    err = iCFFakeEnv->PublishContext( *co1, thread );
    EUNIT_ASSERT_DESC( err == KErrNone, "PublishContext failed!" );
    EUNIT_ASSERT_DESC( !iCFFakeEnv->ClientEventFired(),
        "Script did cause event!" );

    // Publish CONTROL context, no event should come
    co2->SetValueL(_L("False"));
    iCFFakeEnv->ClearFireClientEventCount();
    iCFFakeEnv->ClearFireClientEvent();
    err = iCFFakeEnv->PublishContext( *co2, thread );
    EUNIT_ASSERT_DESC( err == KErrNone, "PublishContext failed!" );
    EUNIT_ASSERT_DESC( !iCFFakeEnv->ClientEventFired(),
        "Script did cause event!" );

    // Publish CONTROL context, event MUST come
    co2->SetValueL(_L("True"));
    iCFFakeEnv->ClearFireClientEventCount();
    iCFFakeEnv->ClearFireClientEvent();
    err = iCFFakeEnv->PublishContext( *co2, thread );
    EUNIT_ASSERT_DESC( err == KErrNone, "PublishContext failed!" );
    EUNIT_ASSERT_DESC( iCFFakeEnv->ClientEventFired(),
        "Script did not cause event!" );

    // Publish COUNTED context, no event should come
    co1->SetValueL(_L("A"));
    iCFFakeEnv->ClearFireClientEvent();
    err = iCFFakeEnv->PublishContext( *co1, thread );
    EUNIT_ASSERT_DESC( err == KErrNone, "PublishContext failed!" );
    EUNIT_ASSERT_DESC( !iCFFakeEnv->ClientEventFired(),
        "Script did cause event!" );
    
    // Publish CONTROL context, no event should come
    co2->SetValueL(_L("False"));
    iCFFakeEnv->ClearFireClientEventCount();
    iCFFakeEnv->ClearFireClientEvent();
    err = iCFFakeEnv->PublishContext( *co2, thread );
    EUNIT_ASSERT_DESC( err == KErrNone, "PublishContext failed!" );
    EUNIT_ASSERT_DESC( !iCFFakeEnv->ClientEventFired(),
        "Script did cause event!" );

    // Publish CONTROL context, no event should come
    co2->SetValueL(_L("True"));
    iCFFakeEnv->ClearFireClientEventCount();
    iCFFakeEnv->ClearFireClientEvent();
    err = iCFFakeEnv->PublishContext( *co2, thread );
    EUNIT_ASSERT_DESC( err == KErrNone, "PublishContext failed!" );
    EUNIT_ASSERT_DESC( !iCFFakeEnv->ClientEventFired(),
        "Script did cause event!" );

    // Publish COUNTED context, no event should come
    co1->SetValueL(_L("B"));
    iCFFakeEnv->ClearFireClientEvent();
    err = iCFFakeEnv->PublishContext( *co1, thread );
    EUNIT_ASSERT_DESC( err == KErrNone, "PublishContext failed!" );
    EUNIT_ASSERT_DESC( !iCFFakeEnv->ClientEventFired(),
        "Script did cause event!" );

    // Publish COUNTED context, no event should come
    co1->SetValueL(_L("A"));
    iCFFakeEnv->ClearFireClientEvent();
    err = iCFFakeEnv->PublishContext( *co1, thread );
    EUNIT_ASSERT_DESC( err == KErrNone, "PublishContext failed!" );
    EUNIT_ASSERT_DESC( !iCFFakeEnv->ClientEventFired(),
        "Script did cause event!" );

    // Publish COUNTED context, no event should come
    co1->SetValueL(_L("B"));
    iCFFakeEnv->ClearFireClientEvent();
    err = iCFFakeEnv->PublishContext( *co1, thread );
    EUNIT_ASSERT_DESC( err == KErrNone, "PublishContext failed!" );
    EUNIT_ASSERT_DESC( !iCFFakeEnv->ClientEventFired(),
        "Script did cause event!" );

    // Publish COUNTED context, no event should come
    co1->SetValueL(_L("A"));
    iCFFakeEnv->ClearFireClientEvent();
    err = iCFFakeEnv->PublishContext( *co1, thread );
    EUNIT_ASSERT_DESC( err == KErrNone, "PublishContext failed!" );
    EUNIT_ASSERT_DESC( !iCFFakeEnv->ClientEventFired(),
        "Script did cause event!" );

    // Publish CONTROL context, no event should come
    co2->SetValueL(_L("False"));
    iCFFakeEnv->ClearFireClientEventCount();
    iCFFakeEnv->ClearFireClientEvent();
    err = iCFFakeEnv->PublishContext( *co2, thread );
    EUNIT_ASSERT_DESC( err == KErrNone, "PublishContext failed!" );
    EUNIT_ASSERT_DESC( !iCFFakeEnv->ClientEventFired(),
        "Script did cause event!" );

    // Publish CONTROL context, no event should come
    co2->SetValueL(_L("True"));
    iCFFakeEnv->ClearFireClientEventCount();
    iCFFakeEnv->ClearFireClientEvent();
    err = iCFFakeEnv->PublishContext( *co2, thread );
    EUNIT_ASSERT_DESC( err == KErrNone, "PublishContext failed!" );
    EUNIT_ASSERT_DESC( !iCFFakeEnv->ClientEventFired(),
        "Script did cause event!" );

    CleanupStack::PopAndDestroy( co2 );
    CleanupStack::PopAndDestroy( co1 );
    CleanupStack::PopAndDestroy( &thread );
    }

void mt_basicoperationsplugin::MT_ScriptTest_countOneShot_triggerWithValue_duration_L()
    {
    RThread thread;
    CleanupClosePushL(thread);

    // Define context
    static _LIT_SECURITY_POLICY_PASS( alwaysPass );
    CCFContextObject* co1 = CCFContextObject::NewLC();
    co1->SetSourceL(_L("Test"));
    co1->SetTypeL(_L("Counting"));
    TInt err = iCFFakeEnv->DefineContext( co1->Source(),
            co1->Type(), alwaysPass, alwaysPass );
    EUNIT_ASSERT_DESC( err == KErrNone, "DefineContext failed!" );

    CCFContextObject* co2 = CCFContextObject::NewLC();
    co2->SetSourceL(_L("Test"));
    co2->SetTypeL(_L("Control"));
    err = iCFFakeEnv->DefineContext( co2->Source(),
            co2->Type(), alwaysPass, alwaysPass );
    EUNIT_ASSERT_DESC( err == KErrNone, "DefineContext failed!" );

    // Publish context before loading so that subscribing will find the initial
    // value used to evaluate the operation
    co1->SetValueL(_L("A"));
    iCFFakeEnv->ClearFireClientEventCount();
    iCFFakeEnv->ClearFireClientEvent();
    err = iCFFakeEnv->PublishContext( *co1, thread );
    EUNIT_ASSERT_DESC( err == KErrNone, "PublishContext failed!" );
    EUNIT_ASSERT_DESC( !iCFFakeEnv->ClientEventFired(),
        "Script did cause event!" );

    co2->SetValueL(_L("True"));
    iCFFakeEnv->ClearFireClientEventCount();
    iCFFakeEnv->ClearFireClientEvent();
    err = iCFFakeEnv->PublishContext( *co2, thread );
    EUNIT_ASSERT_DESC( err == KErrNone, "PublishContext failed!" );
    EUNIT_ASSERT_DESC( !iCFFakeEnv->ClientEventFired(),
        "Script did cause event!" );

    // load script
    HBufC8* script = LoadScriptFromFile(_L("test-countOneShotWithValueDuration3.xml"));
    TInt id = iCFFakeEnv->AddScript(_L("test-countOneShotWithValueDuration3.xml"),
            *script, KMT_CfScriptEngineUid, thread);
    EUNIT_ASSERT_DESC( id >= 0, "Parse error!" );
    delete script;

    iCFFakeEnv->ClearFireClientEventCount();

    // Publish COUNTED context, no event should come
    co1->SetValueL(_L("B"));
    iCFFakeEnv->ClearFireClientEvent();
    err = iCFFakeEnv->PublishContext( *co1, thread );
    EUNIT_ASSERT_DESC( err == KErrNone, "PublishContext failed!" );
    EUNIT_ASSERT_DESC( !iCFFakeEnv->ClientEventFired(),
        "Script did cause event!" );

    // Publish COUNTED context, event MUST come
    co1->SetValueL(_L("A"));
    iCFFakeEnv->ClearFireClientEvent();
    err = iCFFakeEnv->PublishContext( *co1, thread );
    EUNIT_ASSERT_DESC( err == KErrNone, "PublishContext failed!" );
    EUNIT_ASSERT_DESC( iCFFakeEnv->ClientEventFired(),
        "Script did not cause event!" );

    iTestDelay->Start(100000); // 0.1 seconds

    // Publish CONTROL context, no event should come
    co2->SetValueL(_L("True"));
    iCFFakeEnv->ClearFireClientEventCount();
    iCFFakeEnv->ClearFireClientEvent();
    err = iCFFakeEnv->PublishContext( *co2, thread );
    EUNIT_ASSERT_DESC( err == KErrNone, "PublishContext failed!" );
    EUNIT_ASSERT_DESC( !iCFFakeEnv->ClientEventFired(),
        "Script did cause event!" );

    // Publish CONTROL context, no event should come
    co2->SetValueL(_L("False"));
    iCFFakeEnv->ClearFireClientEventCount();
    iCFFakeEnv->ClearFireClientEvent();
    err = iCFFakeEnv->PublishContext( *co2, thread );
    EUNIT_ASSERT_DESC( err == KErrNone, "PublishContext failed!" );
    EUNIT_ASSERT_DESC( !iCFFakeEnv->ClientEventFired(),
        "Script did cause event!" );

    // Publish CONTROL context, no event should come
    co2->SetValueL(_L("True"));
    iCFFakeEnv->ClearFireClientEventCount();
    iCFFakeEnv->ClearFireClientEvent();
    err = iCFFakeEnv->PublishContext( *co2, thread );
    EUNIT_ASSERT_DESC( err == KErrNone, "PublishContext failed!" );
    EUNIT_ASSERT_DESC( !iCFFakeEnv->ClientEventFired(),
        "Script did cause event!" );

    // Publish COUNTED context, no event should come
    co1->SetValueL(_L("B"));
    iCFFakeEnv->ClearFireClientEvent();
    err = iCFFakeEnv->PublishContext( *co1, thread );
    EUNIT_ASSERT_DESC( err == KErrNone, "PublishContext failed!" );
    EUNIT_ASSERT_DESC( !iCFFakeEnv->ClientEventFired(),
        "Script did cause event!" );

    // Publish COUNTED context, no event should come
    co1->SetValueL(_L("A"));
    iCFFakeEnv->ClearFireClientEvent();
    err = iCFFakeEnv->PublishContext( *co1, thread );
    EUNIT_ASSERT_DESC( err == KErrNone, "PublishContext failed!" );
    EUNIT_ASSERT_DESC( !iCFFakeEnv->ClientEventFired(),
        "Script did cause event!" );

    // Publish COUNTED context, no event should come
    co1->SetValueL(_L("B"));
    iCFFakeEnv->ClearFireClientEvent();
    err = iCFFakeEnv->PublishContext( *co1, thread );
    EUNIT_ASSERT_DESC( err == KErrNone, "PublishContext failed!" );
    EUNIT_ASSERT_DESC( !iCFFakeEnv->ClientEventFired(),
        "Script did cause event!" );

    // Publish COUNTED context, no event should come
    co1->SetValueL(_L("A"));
    iCFFakeEnv->ClearFireClientEvent();
    err = iCFFakeEnv->PublishContext( *co1, thread );
    EUNIT_ASSERT_DESC( err == KErrNone, "PublishContext failed!" );
    EUNIT_ASSERT_DESC( !iCFFakeEnv->ClientEventFired(),
        "Script did cause event!" );
    
    // Publish CONTROL context, no event should come
    co2->SetValueL(_L("False"));
    iCFFakeEnv->ClearFireClientEventCount();
    iCFFakeEnv->ClearFireClientEvent();
    err = iCFFakeEnv->PublishContext( *co2, thread );
    EUNIT_ASSERT_DESC( err == KErrNone, "PublishContext failed!" );
    EUNIT_ASSERT_DESC( !iCFFakeEnv->ClientEventFired(),
        "Script did cause event!" );

    // Publish CONTROL context, no event should come
    co2->SetValueL(_L("True"));
    iCFFakeEnv->ClearFireClientEventCount();
    iCFFakeEnv->ClearFireClientEvent();
    err = iCFFakeEnv->PublishContext( *co2, thread );
    EUNIT_ASSERT_DESC( err == KErrNone, "PublishContext failed!" );
    EUNIT_ASSERT_DESC( !iCFFakeEnv->ClientEventFired(),
        "Script did cause event!" );

    // Publish COUNTED context, no event should come
    co1->SetValueL(_L("B"));
    iCFFakeEnv->ClearFireClientEvent();
    err = iCFFakeEnv->PublishContext( *co1, thread );
    EUNIT_ASSERT_DESC( err == KErrNone, "PublishContext failed!" );
    EUNIT_ASSERT_DESC( !iCFFakeEnv->ClientEventFired(),
        "Script did cause event!" );

    // Publish COUNTED context, no event should come
    co1->SetValueL(_L("A"));
    iCFFakeEnv->ClearFireClientEvent();
    err = iCFFakeEnv->PublishContext( *co1, thread );
    EUNIT_ASSERT_DESC( err == KErrNone, "PublishContext failed!" );
    EUNIT_ASSERT_DESC( !iCFFakeEnv->ClientEventFired(),
        "Script did cause event!" );

    // Publish COUNTED context, no event should come
    co1->SetValueL(_L("B"));
    iCFFakeEnv->ClearFireClientEvent();
    err = iCFFakeEnv->PublishContext( *co1, thread );
    EUNIT_ASSERT_DESC( err == KErrNone, "PublishContext failed!" );
    EUNIT_ASSERT_DESC( !iCFFakeEnv->ClientEventFired(),
        "Script did cause event!" );

    // Publish COUNTED context, no event should come
    co1->SetValueL(_L("A"));
    iCFFakeEnv->ClearFireClientEvent();
    err = iCFFakeEnv->PublishContext( *co1, thread );
    EUNIT_ASSERT_DESC( err == KErrNone, "PublishContext failed!" );
    EUNIT_ASSERT_DESC( !iCFFakeEnv->ClientEventFired(),
        "Script did cause event!" );

    // Publish CONTROL context, no event should come
    co2->SetValueL(_L("False"));
    iCFFakeEnv->ClearFireClientEventCount();
    iCFFakeEnv->ClearFireClientEvent();
    err = iCFFakeEnv->PublishContext( *co2, thread );
    EUNIT_ASSERT_DESC( err == KErrNone, "PublishContext failed!" );
    EUNIT_ASSERT_DESC( !iCFFakeEnv->ClientEventFired(),
        "Script did cause event!" );

    // Publish CONTROL context, no event should come
    co2->SetValueL(_L("True"));
    iCFFakeEnv->ClearFireClientEventCount();
    iCFFakeEnv->ClearFireClientEvent();
    err = iCFFakeEnv->PublishContext( *co2, thread );
    EUNIT_ASSERT_DESC( err == KErrNone, "PublishContext failed!" );
    EUNIT_ASSERT_DESC( !iCFFakeEnv->ClientEventFired(),
        "Script did cause event!" );

    CleanupStack::PopAndDestroy( co2 );
    CleanupStack::PopAndDestroy( co1 );
    CleanupStack::PopAndDestroy( &thread );
    }

void mt_basicoperationsplugin::MT_ScriptTest_countOneShot_remainWithValue_persistentDuration_L()
    {
    RThread thread;
    CleanupClosePushL(thread);

    _LIT( KTestScript, "test-countOneShotWithValuePersistentDuration.xml" );
    _LIT( KPersistencyFileName, "testCountPersistency.dat");
    DeletePersistencyFile( KTestScript, KPersistencyFileName );

    // Define context
    static _LIT_SECURITY_POLICY_PASS( alwaysPass );
    CCFContextObject* co1 = CCFContextObject::NewLC();
    co1->SetSourceL(_L("Test"));
    co1->SetTypeL(_L("Counting"));
    TInt err = iCFFakeEnv->DefineContext( co1->Source(),
            co1->Type(), alwaysPass, alwaysPass );
    EUNIT_ASSERT_DESC( err == KErrNone, "DefineContext failed!" );

    CCFContextObject* co2 = CCFContextObject::NewLC();
    co2->SetSourceL(_L("Test"));
    co2->SetTypeL(_L("Control"));
    err = iCFFakeEnv->DefineContext( co2->Source(),
            co2->Type(), alwaysPass, alwaysPass );
    EUNIT_ASSERT_DESC( err == KErrNone, "DefineContext failed!" );

    // Publish context before loading so that subscribing will find the initial
    // value used to evaluate the operation
    co1->SetValueL(_L("A"));
    iCFFakeEnv->ClearFireClientEventCount();
    iCFFakeEnv->ClearFireClientEvent();
    err = iCFFakeEnv->PublishContext( *co1, thread );
    EUNIT_ASSERT_DESC( err == KErrNone, "PublishContext failed!" );
    EUNIT_ASSERT_DESC( !iCFFakeEnv->ClientEventFired(),
        "Script did cause event!" );

    co2->SetValueL(_L("True"));
    iCFFakeEnv->ClearFireClientEventCount();
    iCFFakeEnv->ClearFireClientEvent();
    err = iCFFakeEnv->PublishContext( *co2, thread );
    EUNIT_ASSERT_DESC( err == KErrNone, "PublishContext failed!" );
    EUNIT_ASSERT_DESC( !iCFFakeEnv->ClientEventFired(),
        "Script did cause event!" );

    // load script
    HBufC8* script = LoadScriptFromFile( KTestScript );
    TInt id = iCFFakeEnv->AddScript(
            KTestScript, *script, KMT_CfScriptEngineUid, thread);
    EUNIT_ASSERT_DESC( id >= 0, "Parse error!" );
    delete script;

    iCFFakeEnv->ClearFireClientEventCount();

    // Publish COUNTED context, no event should come
    co1->SetValueL(_L("B"));
    iCFFakeEnv->ClearFireClientEvent();
    err = iCFFakeEnv->PublishContext( *co1, thread );
    EUNIT_ASSERT_DESC( err == KErrNone, "PublishContext failed!" );
    EUNIT_ASSERT_DESC( !iCFFakeEnv->ClientEventFired(),
        "Script did cause event!" );

    // REINITIALIZE
    TeardownMidtest();
    iTestDelay->Start(300000); // 0.3 s
    SetupMidtestL( *co1, *co2, KTestScript );

    // Publish COUNTED context, no event should come
    co1->SetValueL(_L("A"));
    iCFFakeEnv->ClearFireClientEvent();
    err = iCFFakeEnv->PublishContext( *co1, thread );
    EUNIT_ASSERT_DESC( err == KErrNone, "PublishContext failed!" );
    EUNIT_ASSERT_DESC( !iCFFakeEnv->ClientEventFired(),
        "Script did cause event!" );

    // REINITIALIZE
    TeardownMidtest();
    iTestDelay->Start(300000); // 0.3 s
    SetupMidtestL( *co1, *co2, KTestScript );

    // Publish COUNTED context, no event should come
    co1->SetValueL(_L("B"));
    iCFFakeEnv->ClearFireClientEvent();
    err = iCFFakeEnv->PublishContext( *co1, thread );
    EUNIT_ASSERT_DESC( err == KErrNone, "PublishContext failed!" );
    EUNIT_ASSERT_DESC( !iCFFakeEnv->ClientEventFired(),
        "Script did cause event!" );

    // Publish COUNTED context, event MUST come
    co1->SetValueL(_L("A"));
    iCFFakeEnv->ClearFireClientEvent();
    err = iCFFakeEnv->PublishContext( *co1, thread );
    EUNIT_ASSERT_DESC( err == KErrNone, "PublishContext failed!" );
    EUNIT_ASSERT_DESC( iCFFakeEnv->ClientEventFired(),
        "Script did not cause event!" );

    // REINITIALIZE
    TeardownMidtest();
    iTestDelay->Start(300000); // 0.3 s
    SetupMidtestL( *co1, *co2, KTestScript );

    // Publish COUNTED context, no event should come
    co1->SetValueL(_L("B"));
    iCFFakeEnv->ClearFireClientEvent();
    err = iCFFakeEnv->PublishContext( *co1, thread );
    EUNIT_ASSERT_DESC( err == KErrNone, "PublishContext failed!" );
    EUNIT_ASSERT_DESC( !iCFFakeEnv->ClientEventFired(),
        "Script did cause event!" );

    // REINITIALIZE
    TeardownMidtest();
    iTestDelay->Start(300000); // 0.3 s
    SetupMidtestL( *co1, *co2, KTestScript );

    // Publish CONTROL context, no event should come
    co2->SetValueL(_L("False"));
    iCFFakeEnv->ClearFireClientEventCount();
    iCFFakeEnv->ClearFireClientEvent();
    err = iCFFakeEnv->PublishContext( *co2, thread );
    EUNIT_ASSERT_DESC( err == KErrNone, "PublishContext failed!" );
    EUNIT_ASSERT_DESC( !iCFFakeEnv->ClientEventFired(),
        "Script did cause event!" );

    // Publish CONTROL context, event MUST come
    co2->SetValueL(_L("True"));
    iCFFakeEnv->ClearFireClientEventCount();
    iCFFakeEnv->ClearFireClientEvent();
    err = iCFFakeEnv->PublishContext( *co2, thread );
    EUNIT_ASSERT_DESC( err == KErrNone, "PublishContext failed!" );
    EUNIT_ASSERT_DESC( iCFFakeEnv->ClientEventFired(),
        "Script did not cause event!" );

    // Publish COUNTED context, no event should come
    co1->SetValueL(_L("A"));
    iCFFakeEnv->ClearFireClientEvent();
    err = iCFFakeEnv->PublishContext( *co1, thread );
    EUNIT_ASSERT_DESC( err == KErrNone, "PublishContext failed!" );
    EUNIT_ASSERT_DESC( !iCFFakeEnv->ClientEventFired(),
        "Script did cause event!" );

    // REINITIALIZE
    TeardownMidtest();
    iTestDelay->Start(300000); // 0.3 s
    SetupMidtestL( *co1, *co2, KTestScript );

    // Publish COUNTED context, no event should come
    co1->SetValueL(_L("B"));
    iCFFakeEnv->ClearFireClientEvent();
    err = iCFFakeEnv->PublishContext( *co1, thread );
    EUNIT_ASSERT_DESC( err == KErrNone, "PublishContext failed!" );
    EUNIT_ASSERT_DESC( !iCFFakeEnv->ClientEventFired(),
        "Script did cause event!" );

    // Publish COUNTED context, no event should come
    co1->SetValueL(_L("A"));
    iCFFakeEnv->ClearFireClientEvent();
    err = iCFFakeEnv->PublishContext( *co1, thread );
    EUNIT_ASSERT_DESC( err == KErrNone, "PublishContext failed!" );
    EUNIT_ASSERT_DESC( !iCFFakeEnv->ClientEventFired(),
        "Script did cause event!" );

    // Publish CONTROL context, no event should come
    co2->SetValueL(_L("False"));
    iCFFakeEnv->ClearFireClientEventCount();
    iCFFakeEnv->ClearFireClientEvent();
    err = iCFFakeEnv->PublishContext( *co2, thread );
    EUNIT_ASSERT_DESC( err == KErrNone, "PublishContext failed!" );
    EUNIT_ASSERT_DESC( !iCFFakeEnv->ClientEventFired(),
        "Script did cause event!" );

    // Publish CONTROL context, event MUST come
    co2->SetValueL(_L("True"));
    iCFFakeEnv->ClearFireClientEventCount();
    iCFFakeEnv->ClearFireClientEvent();
    err = iCFFakeEnv->PublishContext( *co2, thread );
    EUNIT_ASSERT_DESC( err == KErrNone, "PublishContext failed!" );
    EUNIT_ASSERT_DESC( iCFFakeEnv->ClientEventFired(),
        "Script did not cause event!" );

    CleanupStack::PopAndDestroy( co2 );
    CleanupStack::PopAndDestroy( co1 );
    CleanupStack::PopAndDestroy( &thread );
    }

void mt_basicoperationsplugin::MT_ScriptTest_countOneShot_changeWithValue_persistentDuration_L()
    {
    RThread thread;
    CleanupClosePushL(thread);

    _LIT( KTestScript, "test-countOneShotWithValuePersistentDuration2.xml" );
    _LIT( KPersistencyFileName, "testCountPersistency.dat");
    DeletePersistencyFile( KTestScript, KPersistencyFileName );

    // Define context
    static _LIT_SECURITY_POLICY_PASS( alwaysPass );
    CCFContextObject* co1 = CCFContextObject::NewLC();
    co1->SetSourceL(_L("Test"));
    co1->SetTypeL(_L("Counting"));
    TInt err = iCFFakeEnv->DefineContext( co1->Source(),
            co1->Type(), alwaysPass, alwaysPass );
    EUNIT_ASSERT_DESC( err == KErrNone, "DefineContext failed!" );

    CCFContextObject* co2 = CCFContextObject::NewLC();
    co2->SetSourceL(_L("Test"));
    co2->SetTypeL(_L("Control"));
    err = iCFFakeEnv->DefineContext( co2->Source(),
            co2->Type(), alwaysPass, alwaysPass );
    EUNIT_ASSERT_DESC( err == KErrNone, "DefineContext failed!" );

    // Publish context before loading so that subscribing will find the initial
    // value used to evaluate the operation
    co1->SetValueL(_L("A"));
    iCFFakeEnv->ClearFireClientEventCount();
    iCFFakeEnv->ClearFireClientEvent();
    err = iCFFakeEnv->PublishContext( *co1, thread );
    EUNIT_ASSERT_DESC( err == KErrNone, "PublishContext failed!" );
    EUNIT_ASSERT_DESC( !iCFFakeEnv->ClientEventFired(),
        "Script did cause event!" );

    co2->SetValueL(_L("True"));
    iCFFakeEnv->ClearFireClientEventCount();
    iCFFakeEnv->ClearFireClientEvent();
    err = iCFFakeEnv->PublishContext( *co2, thread );
    EUNIT_ASSERT_DESC( err == KErrNone, "PublishContext failed!" );
    EUNIT_ASSERT_DESC( !iCFFakeEnv->ClientEventFired(),
        "Script did cause event!" );

    // load script
    HBufC8* script = LoadScriptFromFile( KTestScript );
    TInt id = iCFFakeEnv->AddScript(
            KTestScript, *script, KMT_CfScriptEngineUid, thread);
    EUNIT_ASSERT_DESC( id >= 0, "Parse error!" );
    delete script;

    iCFFakeEnv->ClearFireClientEventCount();

    // Publish COUNTED context, no event should come
    co1->SetValueL(_L("B"));
    iCFFakeEnv->ClearFireClientEvent();
    err = iCFFakeEnv->PublishContext( *co1, thread );
    EUNIT_ASSERT_DESC( err == KErrNone, "PublishContext failed!" );
    EUNIT_ASSERT_DESC( !iCFFakeEnv->ClientEventFired(),
        "Script did cause event!" );

    // REINITIALIZE
    TeardownMidtest();
    iTestDelay->Start(300000); // 0.3 s
    SetupMidtestL( *co1, *co2, KTestScript );

    // Publish COUNTED context, no event should come
    co1->SetValueL(_L("A"));
    iCFFakeEnv->ClearFireClientEvent();
    err = iCFFakeEnv->PublishContext( *co1, thread );
    EUNIT_ASSERT_DESC( err == KErrNone, "PublishContext failed!" );
    EUNIT_ASSERT_DESC( !iCFFakeEnv->ClientEventFired(),
        "Script did cause event!" );

    // REINITIALIZE
    TeardownMidtest();
    iTestDelay->Start(300000); // 0.3 s
    SetupMidtestL( *co1, *co2, KTestScript );

    // Publish COUNTED context, no event should come
    co1->SetValueL(_L("B"));
    iCFFakeEnv->ClearFireClientEvent();
    err = iCFFakeEnv->PublishContext( *co1, thread );
    EUNIT_ASSERT_DESC( err == KErrNone, "PublishContext failed!" );
    EUNIT_ASSERT_DESC( !iCFFakeEnv->ClientEventFired(),
        "Script did cause event!" );

    // Publish COUNTED context, event MUST come
    co1->SetValueL(_L("A"));
    iCFFakeEnv->ClearFireClientEvent();
    err = iCFFakeEnv->PublishContext( *co1, thread );
    EUNIT_ASSERT_DESC( err == KErrNone, "PublishContext failed!" );
    EUNIT_ASSERT_DESC( iCFFakeEnv->ClientEventFired(),
        "Script did not cause event!" );

    // REINITIALIZE
    TeardownMidtest();
    iTestDelay->Start(300000); // 0.3 s
    SetupMidtestL( *co1, *co2, KTestScript );

    // Publish COUNTED context, no event should come
    co1->SetValueL(_L("B"));
    iCFFakeEnv->ClearFireClientEvent();
    err = iCFFakeEnv->PublishContext( *co1, thread );
    EUNIT_ASSERT_DESC( err == KErrNone, "PublishContext failed!" );
    EUNIT_ASSERT_DESC( !iCFFakeEnv->ClientEventFired(),
        "Script did cause event!" );

    // REINITIALIZE
    TeardownMidtest();
    iTestDelay->Start(300000); // 0.3 s
    SetupMidtestL( *co1, *co2, KTestScript );

    // Publish CONTROL context, no event should come
    co2->SetValueL(_L("False"));
    iCFFakeEnv->ClearFireClientEventCount();
    iCFFakeEnv->ClearFireClientEvent();
    err = iCFFakeEnv->PublishContext( *co2, thread );
    EUNIT_ASSERT_DESC( err == KErrNone, "PublishContext failed!" );
    EUNIT_ASSERT_DESC( !iCFFakeEnv->ClientEventFired(),
        "Script did cause event!" );

    // Publish CONTROL context, event MUST come
    co2->SetValueL(_L("True"));
    iCFFakeEnv->ClearFireClientEventCount();
    iCFFakeEnv->ClearFireClientEvent();
    err = iCFFakeEnv->PublishContext( *co2, thread );
    EUNIT_ASSERT_DESC( err == KErrNone, "PublishContext failed!" );
    EUNIT_ASSERT_DESC( iCFFakeEnv->ClientEventFired(),
        "Script did not cause event!" );

    // Publish COUNTED context, no event should come
    co1->SetValueL(_L("A"));
    iCFFakeEnv->ClearFireClientEvent();
    err = iCFFakeEnv->PublishContext( *co1, thread );
    EUNIT_ASSERT_DESC( err == KErrNone, "PublishContext failed!" );
    EUNIT_ASSERT_DESC( !iCFFakeEnv->ClientEventFired(),
        "Script did cause event!" );
    
    // REINITIALIZE
    TeardownMidtest();
    iTestDelay->Start(300000); // 0.3 s
    SetupMidtestL( *co1, *co2, KTestScript );

    // Publish CONTROL context, no event should come
    co2->SetValueL(_L("False"));
    iCFFakeEnv->ClearFireClientEventCount();
    iCFFakeEnv->ClearFireClientEvent();
    err = iCFFakeEnv->PublishContext( *co2, thread );
    EUNIT_ASSERT_DESC( err == KErrNone, "PublishContext failed!" );
    EUNIT_ASSERT_DESC( !iCFFakeEnv->ClientEventFired(),
        "Script did cause event!" );

    // Publish CONTROL context, no event should come
    co2->SetValueL(_L("True"));
    iCFFakeEnv->ClearFireClientEventCount();
    iCFFakeEnv->ClearFireClientEvent();
    err = iCFFakeEnv->PublishContext( *co2, thread );
    EUNIT_ASSERT_DESC( err == KErrNone, "PublishContext failed!" );
    EUNIT_ASSERT_DESC( !iCFFakeEnv->ClientEventFired(),
        "Script did cause event!" );

    // Publish COUNTED context, no event should come
    co1->SetValueL(_L("B"));
    iCFFakeEnv->ClearFireClientEvent();
    err = iCFFakeEnv->PublishContext( *co1, thread );
    EUNIT_ASSERT_DESC( err == KErrNone, "PublishContext failed!" );
    EUNIT_ASSERT_DESC( !iCFFakeEnv->ClientEventFired(),
        "Script did cause event!" );

    // Publish COUNTED context, no event should come
    co1->SetValueL(_L("A"));
    iCFFakeEnv->ClearFireClientEvent();
    err = iCFFakeEnv->PublishContext( *co1, thread );
    EUNIT_ASSERT_DESC( err == KErrNone, "PublishContext failed!" );
    EUNIT_ASSERT_DESC( !iCFFakeEnv->ClientEventFired(),
        "Script did cause event!" );

    // Publish COUNTED context, no event should come
    co1->SetValueL(_L("B"));
    iCFFakeEnv->ClearFireClientEvent();
    err = iCFFakeEnv->PublishContext( *co1, thread );
    EUNIT_ASSERT_DESC( err == KErrNone, "PublishContext failed!" );
    EUNIT_ASSERT_DESC( !iCFFakeEnv->ClientEventFired(),
        "Script did cause event!" );

    // Publish COUNTED context, no event should come
    co1->SetValueL(_L("A"));
    iCFFakeEnv->ClearFireClientEvent();
    err = iCFFakeEnv->PublishContext( *co1, thread );
    EUNIT_ASSERT_DESC( err == KErrNone, "PublishContext failed!" );
    EUNIT_ASSERT_DESC( !iCFFakeEnv->ClientEventFired(),
        "Script did cause event!" );

    // Publish CONTROL context, no event should come
    co2->SetValueL(_L("False"));
    iCFFakeEnv->ClearFireClientEventCount();
    iCFFakeEnv->ClearFireClientEvent();
    err = iCFFakeEnv->PublishContext( *co2, thread );
    EUNIT_ASSERT_DESC( err == KErrNone, "PublishContext failed!" );
    EUNIT_ASSERT_DESC( !iCFFakeEnv->ClientEventFired(),
        "Script did cause event!" );

    // Publish CONTROL context, no event should come
    co2->SetValueL(_L("True"));
    iCFFakeEnv->ClearFireClientEventCount();
    iCFFakeEnv->ClearFireClientEvent();
    err = iCFFakeEnv->PublishContext( *co2, thread );
    EUNIT_ASSERT_DESC( err == KErrNone, "PublishContext failed!" );
    EUNIT_ASSERT_DESC( !iCFFakeEnv->ClientEventFired(),
        "Script did cause event!" );

    CleanupStack::PopAndDestroy( co2 );
    CleanupStack::PopAndDestroy( co1 );
    CleanupStack::PopAndDestroy( &thread );
    }

void mt_basicoperationsplugin::MT_ScriptTest_countOneShot_triggerWithValue_persistentDuration_L()
    {
    RThread thread;
    CleanupClosePushL(thread);

    _LIT( KTestScript, "test-countOneShotWithValuePersistentDuration3.xml" );
    _LIT( KPersistencyFileName, "testCountPersistency.dat");
    DeletePersistencyFile( KTestScript, KPersistencyFileName );

    // Define context
    static _LIT_SECURITY_POLICY_PASS( alwaysPass );
    CCFContextObject* co1 = CCFContextObject::NewLC();
    co1->SetSourceL(_L("Test"));
    co1->SetTypeL(_L("Counting"));
    TInt err = iCFFakeEnv->DefineContext( co1->Source(),
            co1->Type(), alwaysPass, alwaysPass );
    EUNIT_ASSERT_DESC( err == KErrNone, "DefineContext failed!" );

    CCFContextObject* co2 = CCFContextObject::NewLC();
    co2->SetSourceL(_L("Test"));
    co2->SetTypeL(_L("Control"));
    err = iCFFakeEnv->DefineContext( co2->Source(),
            co2->Type(), alwaysPass, alwaysPass );
    EUNIT_ASSERT_DESC( err == KErrNone, "DefineContext failed!" );

    // Publish context before loading so that subscribing will find the initial
    // value used to evaluate the operation
    co1->SetValueL(_L("A"));
    iCFFakeEnv->ClearFireClientEventCount();
    iCFFakeEnv->ClearFireClientEvent();
    err = iCFFakeEnv->PublishContext( *co1, thread );
    EUNIT_ASSERT_DESC( err == KErrNone, "PublishContext failed!" );
    EUNIT_ASSERT_DESC( !iCFFakeEnv->ClientEventFired(),
        "Script did cause event!" );

    co2->SetValueL(_L("True"));
    iCFFakeEnv->ClearFireClientEventCount();
    iCFFakeEnv->ClearFireClientEvent();
    err = iCFFakeEnv->PublishContext( *co2, thread );
    EUNIT_ASSERT_DESC( err == KErrNone, "PublishContext failed!" );
    EUNIT_ASSERT_DESC( !iCFFakeEnv->ClientEventFired(),
        "Script did cause event!" );

    // load script
    HBufC8* script = LoadScriptFromFile( KTestScript );
    TInt id = iCFFakeEnv->AddScript(
            KTestScript, *script, KMT_CfScriptEngineUid, thread);
    EUNIT_ASSERT_DESC( id >= 0, "Parse error!" );
    delete script;

    iCFFakeEnv->ClearFireClientEventCount();

    // Publish COUNTED context, no event should come
    co1->SetValueL(_L("B"));
    iCFFakeEnv->ClearFireClientEvent();
    err = iCFFakeEnv->PublishContext( *co1, thread );
    EUNIT_ASSERT_DESC( err == KErrNone, "PublishContext failed!" );
    EUNIT_ASSERT_DESC( !iCFFakeEnv->ClientEventFired(),
        "Script did cause event!" );

    // REINITIALIZE
    TeardownMidtest();
    iTestDelay->Start(300000); // 0.3 s
    SetupMidtestL( *co1, *co2, KTestScript );

    // Publish COUNTED context, event MUST come
    co1->SetValueL(_L("A"));
    iCFFakeEnv->ClearFireClientEvent();
    err = iCFFakeEnv->PublishContext( *co1, thread );
    EUNIT_ASSERT_DESC( err == KErrNone, "PublishContext failed!" );
    EUNIT_ASSERT_DESC( iCFFakeEnv->ClientEventFired(),
        "Script did not cause event!" );

    // REINITIALIZE
    TeardownMidtest();
    iTestDelay->Start(300000); // 0.3 s
    SetupMidtestL( *co1, *co2, KTestScript );

    // Publish CONTROL context, no event should come
    co2->SetValueL(_L("True"));
    iCFFakeEnv->ClearFireClientEventCount();
    iCFFakeEnv->ClearFireClientEvent();
    err = iCFFakeEnv->PublishContext( *co2, thread );
    EUNIT_ASSERT_DESC( err == KErrNone, "PublishContext failed!" );
    EUNIT_ASSERT_DESC( !iCFFakeEnv->ClientEventFired(),
        "Script did cause event!" );

    // Publish CONTROL context, no event should come
    co2->SetValueL(_L("False"));
    iCFFakeEnv->ClearFireClientEventCount();
    iCFFakeEnv->ClearFireClientEvent();
    err = iCFFakeEnv->PublishContext( *co2, thread );
    EUNIT_ASSERT_DESC( err == KErrNone, "PublishContext failed!" );
    EUNIT_ASSERT_DESC( !iCFFakeEnv->ClientEventFired(),
        "Script did cause event!" );

    // Publish CONTROL context, no event should come
    co2->SetValueL(_L("True"));
    iCFFakeEnv->ClearFireClientEventCount();
    iCFFakeEnv->ClearFireClientEvent();
    err = iCFFakeEnv->PublishContext( *co2, thread );
    EUNIT_ASSERT_DESC( err == KErrNone, "PublishContext failed!" );
    EUNIT_ASSERT_DESC( !iCFFakeEnv->ClientEventFired(),
        "Script did cause event!" );

    // Publish COUNTED context, no event should come
    co1->SetValueL(_L("B"));
    iCFFakeEnv->ClearFireClientEvent();
    err = iCFFakeEnv->PublishContext( *co1, thread );
    EUNIT_ASSERT_DESC( err == KErrNone, "PublishContext failed!" );
    EUNIT_ASSERT_DESC( !iCFFakeEnv->ClientEventFired(),
        "Script did cause event!" );

    // Publish COUNTED context, no event should come
    co1->SetValueL(_L("A"));
    iCFFakeEnv->ClearFireClientEvent();
    err = iCFFakeEnv->PublishContext( *co1, thread );
    EUNIT_ASSERT_DESC( err == KErrNone, "PublishContext failed!" );
    EUNIT_ASSERT_DESC( !iCFFakeEnv->ClientEventFired(),
        "Script did cause event!" );

    // REINITIALIZE
    TeardownMidtest();
    iTestDelay->Start(300000); // 0.3 s
    SetupMidtestL( *co1, *co2, KTestScript );

    // Publish COUNTED context, no event should come
    co1->SetValueL(_L("B"));
    iCFFakeEnv->ClearFireClientEvent();
    err = iCFFakeEnv->PublishContext( *co1, thread );
    EUNIT_ASSERT_DESC( err == KErrNone, "PublishContext failed!" );
    EUNIT_ASSERT_DESC( !iCFFakeEnv->ClientEventFired(),
        "Script did cause event!" );

    // Publish COUNTED context, no event should come
    co1->SetValueL(_L("A"));
    iCFFakeEnv->ClearFireClientEvent();
    err = iCFFakeEnv->PublishContext( *co1, thread );
    EUNIT_ASSERT_DESC( err == KErrNone, "PublishContext failed!" );
    EUNIT_ASSERT_DESC( !iCFFakeEnv->ClientEventFired(),
        "Script did cause event!" );
    
    // REINITIALIZE
    TeardownMidtest();
    iTestDelay->Start(300000); // 0.3 s
    SetupMidtestL( *co1, *co2, KTestScript );

    // Publish CONTROL context, no event should come
    co2->SetValueL(_L("False"));
    iCFFakeEnv->ClearFireClientEventCount();
    iCFFakeEnv->ClearFireClientEvent();
    err = iCFFakeEnv->PublishContext( *co2, thread );
    EUNIT_ASSERT_DESC( err == KErrNone, "PublishContext failed!" );
    EUNIT_ASSERT_DESC( !iCFFakeEnv->ClientEventFired(),
        "Script did cause event!" );

    // Publish CONTROL context, no event should come
    co2->SetValueL(_L("True"));
    iCFFakeEnv->ClearFireClientEventCount();
    iCFFakeEnv->ClearFireClientEvent();
    err = iCFFakeEnv->PublishContext( *co2, thread );
    EUNIT_ASSERT_DESC( err == KErrNone, "PublishContext failed!" );
    EUNIT_ASSERT_DESC( !iCFFakeEnv->ClientEventFired(),
        "Script did cause event!" );

    // Publish COUNTED context, no event should come
    co1->SetValueL(_L("B"));
    iCFFakeEnv->ClearFireClientEvent();
    err = iCFFakeEnv->PublishContext( *co1, thread );
    EUNIT_ASSERT_DESC( err == KErrNone, "PublishContext failed!" );
    EUNIT_ASSERT_DESC( !iCFFakeEnv->ClientEventFired(),
        "Script did cause event!" );

    // Publish COUNTED context, no event should come
    co1->SetValueL(_L("A"));
    iCFFakeEnv->ClearFireClientEvent();
    err = iCFFakeEnv->PublishContext( *co1, thread );
    EUNIT_ASSERT_DESC( err == KErrNone, "PublishContext failed!" );
    EUNIT_ASSERT_DESC( !iCFFakeEnv->ClientEventFired(),
        "Script did cause event!" );

    // Publish COUNTED context, no event should come
    co1->SetValueL(_L("B"));
    iCFFakeEnv->ClearFireClientEvent();
    err = iCFFakeEnv->PublishContext( *co1, thread );
    EUNIT_ASSERT_DESC( err == KErrNone, "PublishContext failed!" );
    EUNIT_ASSERT_DESC( !iCFFakeEnv->ClientEventFired(),
        "Script did cause event!" );

    // Publish COUNTED context, no event should come
    co1->SetValueL(_L("A"));
    iCFFakeEnv->ClearFireClientEvent();
    err = iCFFakeEnv->PublishContext( *co1, thread );
    EUNIT_ASSERT_DESC( err == KErrNone, "PublishContext failed!" );
    EUNIT_ASSERT_DESC( !iCFFakeEnv->ClientEventFired(),
        "Script did cause event!" );

    // Publish CONTROL context, no event should come
    co2->SetValueL(_L("False"));
    iCFFakeEnv->ClearFireClientEventCount();
    iCFFakeEnv->ClearFireClientEvent();
    err = iCFFakeEnv->PublishContext( *co2, thread );
    EUNIT_ASSERT_DESC( err == KErrNone, "PublishContext failed!" );
    EUNIT_ASSERT_DESC( !iCFFakeEnv->ClientEventFired(),
        "Script did cause event!" );

    // Publish CONTROL context, no event should come
    co2->SetValueL(_L("True"));
    iCFFakeEnv->ClearFireClientEventCount();
    iCFFakeEnv->ClearFireClientEvent();
    err = iCFFakeEnv->PublishContext( *co2, thread );
    EUNIT_ASSERT_DESC( err == KErrNone, "PublishContext failed!" );
    EUNIT_ASSERT_DESC( !iCFFakeEnv->ClientEventFired(),
        "Script did cause event!" );

    CleanupStack::PopAndDestroy( co2 );
    CleanupStack::PopAndDestroy( co1 );
    CleanupStack::PopAndDestroy( &thread );
    }

void mt_basicoperationsplugin::
MT_ScriptTest_countOneShot_remainWithValue_persistentDuration2_L()
    {
    RThread thread;
    CleanupClosePushL(thread);

    _LIT( KTestScript, "test-countOneShotWithValuePersistentDuration4.xml" );
    _LIT( KPersistencyFileName, "testCountPersistency.dat");
    DeletePersistencyFile( KTestScript, KPersistencyFileName );

    // Define context
    static _LIT_SECURITY_POLICY_PASS( alwaysPass );
    CCFContextObject* co1 = CCFContextObject::NewLC();
    co1->SetSourceL(_L("Test"));
    co1->SetTypeL(_L("Counting"));
    TInt err = iCFFakeEnv->DefineContext( co1->Source(),
            co1->Type(), alwaysPass, alwaysPass );
    EUNIT_ASSERT_DESC( err == KErrNone, "DefineContext failed!" );

    CCFContextObject* co2 = CCFContextObject::NewLC();
    co2->SetSourceL(_L("Test"));
    co2->SetTypeL(_L("Control"));
    err = iCFFakeEnv->DefineContext( co2->Source(),
            co2->Type(), alwaysPass, alwaysPass );
    EUNIT_ASSERT_DESC( err == KErrNone, "DefineContext failed!" );

    // load script
    HBufC8* script = LoadScriptFromFile( KTestScript );
    TInt id = iCFFakeEnv->AddScript( KTestScript, *script, KMT_CfScriptEngineUid, thread);
    EUNIT_ASSERT_DESC( id >= 0, "Parse error!" );
    delete script;

    iCFFakeEnv->ClearFireClientEventCount();

    // Publish CONTROL context, event should come since count is zero and value
    // is false and thus not operation should be true
    co2->SetValueL(_L("True"));
    iCFFakeEnv->ClearFireClientEventCount();
    iCFFakeEnv->ClearFireClientEvent();
    err = iCFFakeEnv->PublishContext( *co2, thread );
    EUNIT_ASSERT_DESC( err == KErrNone, "PublishContext failed!" );
    EUNIT_ASSERT_DESC( iCFFakeEnv->ClientEventFired(), "No event!" );

    // Publish COUNTED context, no event should come
    co1->SetValueL(_L("B"));
    iCFFakeEnv->ClearFireClientEvent();
    err = iCFFakeEnv->PublishContext( *co1, thread );
    EUNIT_ASSERT_DESC( err == KErrNone, "PublishContext failed!" );
    EUNIT_ASSERT_DESC( !iCFFakeEnv->ClientEventFired(), "Script did cause event!" );

    // REINITIALIZE
    TeardownMidtest();
    iTestDelay->Start(300000); // 0.3 s
    SetupMidtestL( *co1, *co2, KTestScript );

    // Publish CONTROL context, no event should come
    co2->SetValueL(_L("False"));
    iCFFakeEnv->ClearFireClientEventCount();
    iCFFakeEnv->ClearFireClientEvent();
    err = iCFFakeEnv->PublishContext( *co2, thread );
    EUNIT_ASSERT_DESC( err == KErrNone, "PublishContext failed!" );
    EUNIT_ASSERT_DESC( !iCFFakeEnv->ClientEventFired(), "Script did cause event!" );

    // Publish CONTROL context, event should come since count is zero and value
    // is false and thus not operation should be true
    co2->SetValueL(_L("True"));
    iCFFakeEnv->ClearFireClientEventCount();
    iCFFakeEnv->ClearFireClientEvent();
    err = iCFFakeEnv->PublishContext( *co2, thread );
    EUNIT_ASSERT_DESC( err == KErrNone, "PublishContext failed!" );
    EUNIT_ASSERT_DESC( iCFFakeEnv->ClientEventFired(), "No event!" );

    // Publish COUNTED context, event should come
    co1->SetValueL(_L("A"));
    iCFFakeEnv->ClearFireClientEvent();
    err = iCFFakeEnv->PublishContext( *co1, thread );
    EUNIT_ASSERT_DESC( err == KErrNone, "PublishContext failed!" );
    EUNIT_ASSERT_DESC( iCFFakeEnv->ClientEventFired(), "No event!" );

    // Publish COUNTED context, no event should come
    co1->SetValueL(_L("B"));
    iCFFakeEnv->ClearFireClientEvent();
    err = iCFFakeEnv->PublishContext( *co1, thread );
    EUNIT_ASSERT_DESC( err == KErrNone, "PublishContext failed!" );
    EUNIT_ASSERT_DESC( !iCFFakeEnv->ClientEventFired(), "Script did cause event!" );

    // Publish COUNTED context, no event should come
    co1->SetValueL(_L("A"));
    iCFFakeEnv->ClearFireClientEvent();
    err = iCFFakeEnv->PublishContext( *co1, thread );
    EUNIT_ASSERT_DESC( err == KErrNone, "PublishContext failed!" );
    EUNIT_ASSERT_DESC( !iCFFakeEnv->ClientEventFired(), "Script did cause event!" );

    // Publish COUNTED context, no event should come
    co1->SetValueL(_L("B"));
    iCFFakeEnv->ClearFireClientEvent();
    err = iCFFakeEnv->PublishContext( *co1, thread );
    EUNIT_ASSERT_DESC( err == KErrNone, "PublishContext failed!" );
    EUNIT_ASSERT_DESC( !iCFFakeEnv->ClientEventFired(), "Script did cause event!" );
    
    // Publish CONTROL context, no event should come
    co2->SetValueL(_L("False"));
    iCFFakeEnv->ClearFireClientEventCount();
    iCFFakeEnv->ClearFireClientEvent();
    err = iCFFakeEnv->PublishContext( *co2, thread );
    EUNIT_ASSERT_DESC( err == KErrNone, "PublishContext failed!" );
    EUNIT_ASSERT_DESC( !iCFFakeEnv->ClientEventFired(), "Script did cause event!" );

    // Publish CONTROL context, no event should come
    co2->SetValueL(_L("True"));
    iCFFakeEnv->ClearFireClientEventCount();
    iCFFakeEnv->ClearFireClientEvent();
    err = iCFFakeEnv->PublishContext( *co2, thread );
    EUNIT_ASSERT_DESC( err == KErrNone, "PublishContext failed!" );
    EUNIT_ASSERT_DESC( !iCFFakeEnv->ClientEventFired(), "Script did cause event!" );

    // REINITIALIZE
    TeardownMidtest();
    iTestDelay->Start(300000); // 0.3 s
    SetupMidtestL( *co1, *co2, KTestScript );

    // Publish COUNTED context, no event should come
    co1->SetValueL(_L("A"));
    iCFFakeEnv->ClearFireClientEvent();
    err = iCFFakeEnv->PublishContext( *co1, thread );
    EUNIT_ASSERT_DESC( err == KErrNone, "PublishContext failed!" );
    EUNIT_ASSERT_DESC( !iCFFakeEnv->ClientEventFired(), "Script did cause event!" );

    // Publish COUNTED context, no event should come
    co1->SetValueL(_L("B"));
    iCFFakeEnv->ClearFireClientEvent();
    err = iCFFakeEnv->PublishContext( *co1, thread );
    EUNIT_ASSERT_DESC( err == KErrNone, "PublishContext failed!" );
    EUNIT_ASSERT_DESC( !iCFFakeEnv->ClientEventFired(), "Script did cause event!" );

    // Publish COUNTED context, no event should come
    co1->SetValueL(_L("A"));
    iCFFakeEnv->ClearFireClientEvent();
    err = iCFFakeEnv->PublishContext( *co1, thread );
    EUNIT_ASSERT_DESC( err == KErrNone, "PublishContext failed!" );
    EUNIT_ASSERT_DESC( !iCFFakeEnv->ClientEventFired(), "Script did cause event!" );

    // Publish CONTROL context, no event should come
    co2->SetValueL(_L("False"));
    iCFFakeEnv->ClearFireClientEventCount();
    iCFFakeEnv->ClearFireClientEvent();
    err = iCFFakeEnv->PublishContext( *co2, thread );
    EUNIT_ASSERT_DESC( err == KErrNone, "PublishContext failed!" );
    EUNIT_ASSERT_DESC( !iCFFakeEnv->ClientEventFired(), "Script did cause event!" );

    // Publish CONTROL context, no event should come
    co2->SetValueL(_L("True"));
    iCFFakeEnv->ClearFireClientEventCount();
    iCFFakeEnv->ClearFireClientEvent();
    err = iCFFakeEnv->PublishContext( *co2, thread );
    EUNIT_ASSERT_DESC( err == KErrNone, "PublishContext failed!" );
    EUNIT_ASSERT_DESC( !iCFFakeEnv->ClientEventFired(), "Script did cause event!" );

    CleanupStack::PopAndDestroy( co2 );
    CleanupStack::PopAndDestroy( co1 );
    CleanupStack::PopAndDestroy( &thread );
    }

void mt_basicoperationsplugin::
MT_ScriptTest_countOneShot_remainWithValue_persistentDuration3_L()
    {
    RThread thread;
    CleanupClosePushL(thread);

    _LIT( KTestScript, "test-countOneShotWithValuePersistentDuration5.xml" );
    _LIT( KPersistencyFileName, "testCountPersistency.dat");
    DeletePersistencyFile( KTestScript, KPersistencyFileName );

    // Define context
    static _LIT_SECURITY_POLICY_PASS( alwaysPass );
    CCFContextObject* co1 = CCFContextObject::NewLC();
    co1->SetSourceL(_L("Test"));
    co1->SetTypeL(_L("Counting"));
    TInt err = iCFFakeEnv->DefineContext( co1->Source(),
            co1->Type(), alwaysPass, alwaysPass );
    EUNIT_ASSERT_DESC( err == KErrNone, "DefineContext failed!" );

    CCFContextObject* co2 = CCFContextObject::NewLC();
    co2->SetSourceL(_L("Test"));
    co2->SetTypeL(_L("Control"));
    err = iCFFakeEnv->DefineContext( co2->Source(),
            co2->Type(), alwaysPass, alwaysPass );
    EUNIT_ASSERT_DESC( err == KErrNone, "DefineContext failed!" );

    // load script
    HBufC8* script = LoadScriptFromFile( KTestScript );
    TInt id = iCFFakeEnv->AddScript( KTestScript, *script, KMT_CfScriptEngineUid, thread);
    EUNIT_ASSERT_DESC( id >= 0, "Parse error!" );
    delete script;

    iCFFakeEnv->ClearFireClientEventCount();

    // Publish CONTROL context, event should come since counting to zero
    co2->SetValueL(_L("True"));
    iCFFakeEnv->ClearFireClientEventCount();
    iCFFakeEnv->ClearFireClientEvent();
    err = iCFFakeEnv->PublishContext( *co2, thread );
    EUNIT_ASSERT_DESC( err == KErrNone, "PublishContext failed!" );
    EUNIT_ASSERT_DESC( iCFFakeEnv->ClientEventFired(), "No event!" );

    // Publish COUNTED context, no event should come
    co1->SetValueL(_L("B"));
    iCFFakeEnv->ClearFireClientEvent();
    err = iCFFakeEnv->PublishContext( *co1, thread );
    EUNIT_ASSERT_DESC( err == KErrNone, "PublishContext failed!" );
    EUNIT_ASSERT_DESC( !iCFFakeEnv->ClientEventFired(), "Script did cause event!" );

    // Publish CONTROL context, no event should come
    co2->SetValueL(_L("False"));
    iCFFakeEnv->ClearFireClientEventCount();
    iCFFakeEnv->ClearFireClientEvent();
    err = iCFFakeEnv->PublishContext( *co2, thread );
    EUNIT_ASSERT_DESC( err == KErrNone, "PublishContext failed!" );
    EUNIT_ASSERT_DESC( !iCFFakeEnv->ClientEventFired(), "Script did cause event!" );

    // Publish CONTROL context, event should come
    co2->SetValueL(_L("True"));
    iCFFakeEnv->ClearFireClientEventCount();
    iCFFakeEnv->ClearFireClientEvent();
    err = iCFFakeEnv->PublishContext( *co2, thread );
    EUNIT_ASSERT_DESC( err == KErrNone, "PublishContext failed!" );
    EUNIT_ASSERT_DESC( iCFFakeEnv->ClientEventFired(), "No event!" );

    // REINITIALIZE
    TeardownMidtest();
    iTestDelay->Start(300000); // 0.3 s
    SetupMidtestL( *co1, *co2, KTestScript );

    // Publish CONTROL context, no event should come
    co2->SetValueL(_L("False"));
    iCFFakeEnv->ClearFireClientEventCount();
    iCFFakeEnv->ClearFireClientEvent();
    err = iCFFakeEnv->PublishContext( *co2, thread );
    EUNIT_ASSERT_DESC( err == KErrNone, "PublishContext failed!" );
    EUNIT_ASSERT_DESC( !iCFFakeEnv->ClientEventFired(), "Script did cause event!" );

    // Publish CONTROL context, event should come
    co2->SetValueL(_L("True"));
    iCFFakeEnv->ClearFireClientEventCount();
    iCFFakeEnv->ClearFireClientEvent();
    err = iCFFakeEnv->PublishContext( *co2, thread );
    EUNIT_ASSERT_DESC( err == KErrNone, "PublishContext failed!" );
    EUNIT_ASSERT_DESC( iCFFakeEnv->ClientEventFired(), "No event!" );

    // Publish COUNTED context, no event should come
    co1->SetValueL(_L("A"));
    iCFFakeEnv->ClearFireClientEvent();
    err = iCFFakeEnv->PublishContext( *co1, thread );
    EUNIT_ASSERT_DESC( err == KErrNone, "PublishContext failed!" );
    EUNIT_ASSERT_DESC( !iCFFakeEnv->ClientEventFired(), "Script did cause event!" );

    // Publish CONTROL context, no event should come
    co2->SetValueL(_L("False"));
    iCFFakeEnv->ClearFireClientEventCount();
    iCFFakeEnv->ClearFireClientEvent();
    err = iCFFakeEnv->PublishContext( *co2, thread );
    EUNIT_ASSERT_DESC( err == KErrNone, "PublishContext failed!" );
    EUNIT_ASSERT_DESC( !iCFFakeEnv->ClientEventFired(), "Script did cause event!" );

    // Publish CONTROL context, no event should come
    co2->SetValueL(_L("True"));
    iCFFakeEnv->ClearFireClientEventCount();
    iCFFakeEnv->ClearFireClientEvent();
    err = iCFFakeEnv->PublishContext( *co2, thread );
    EUNIT_ASSERT_DESC( err == KErrNone, "PublishContext failed!" );
    EUNIT_ASSERT_DESC( !iCFFakeEnv->ClientEventFired(), "Script did cause event!" );

    // REINITIALIZE
    TeardownMidtest();
    iTestDelay->Start(300000); // 0.3 s
    SetupMidtestL( *co1, *co2, KTestScript );

    // Publish COUNTED context, no event should come
    co1->SetValueL(_L("B"));
    iCFFakeEnv->ClearFireClientEvent();
    err = iCFFakeEnv->PublishContext( *co1, thread );
    EUNIT_ASSERT_DESC( err == KErrNone, "PublishContext failed!" );
    EUNIT_ASSERT_DESC( !iCFFakeEnv->ClientEventFired(), "Script did cause event!" );

    // Publish COUNTED context, no event should come
    co1->SetValueL(_L("A"));
    iCFFakeEnv->ClearFireClientEvent();
    err = iCFFakeEnv->PublishContext( *co1, thread );
    EUNIT_ASSERT_DESC( err == KErrNone, "PublishContext failed!" );
    EUNIT_ASSERT_DESC( !iCFFakeEnv->ClientEventFired(), "Script did cause event!" );

    // Publish CONTROL context, no event should come
    co2->SetValueL(_L("False"));
    iCFFakeEnv->ClearFireClientEventCount();
    iCFFakeEnv->ClearFireClientEvent();
    err = iCFFakeEnv->PublishContext( *co2, thread );
    EUNIT_ASSERT_DESC( err == KErrNone, "PublishContext failed!" );
    EUNIT_ASSERT_DESC( !iCFFakeEnv->ClientEventFired(), "Script did cause event!" );

    // Publish CONTROL context, no event should come
    co2->SetValueL(_L("True"));
    iCFFakeEnv->ClearFireClientEventCount();
    iCFFakeEnv->ClearFireClientEvent();
    err = iCFFakeEnv->PublishContext( *co2, thread );
    EUNIT_ASSERT_DESC( err == KErrNone, "PublishContext failed!" );
    EUNIT_ASSERT_DESC( !iCFFakeEnv->ClientEventFired(), "Script did cause event!" );

    CleanupStack::PopAndDestroy( co2 );
    CleanupStack::PopAndDestroy( co1 );
    CleanupStack::PopAndDestroy( &thread );
    }

void mt_basicoperationsplugin::
MT_ScriptTest_countOneShot_remainWithValue_persistentDuration4_L()
    {
    RThread thread;
    CleanupClosePushL(thread);

    _LIT( KTestScript, "test-countOneShotWithValuePersistentDuration6.xml" );
    _LIT( KPersistencyFileName, "testCountPersistency.dat");
    DeletePersistencyFile( KTestScript, KPersistencyFileName );

    // Define context
    static _LIT_SECURITY_POLICY_PASS( alwaysPass );
    CCFContextObject* co1 = CCFContextObject::NewLC();
    co1->SetSourceL(_L("Test"));
    co1->SetTypeL(_L("Counting"));
    TInt err = iCFFakeEnv->DefineContext( co1->Source(),
            co1->Type(), alwaysPass, alwaysPass );
    EUNIT_ASSERT_DESC( err == KErrNone, "DefineContext failed!" );

    CCFContextObject* co2 = CCFContextObject::NewLC();
    co2->SetSourceL(_L("Test"));
    co2->SetTypeL(_L("Control"));
    err = iCFFakeEnv->DefineContext( co2->Source(),
            co2->Type(), alwaysPass, alwaysPass );
    EUNIT_ASSERT_DESC( err == KErrNone, "DefineContext failed!" );

    // load script
    HBufC8* script = LoadScriptFromFile( KTestScript );
    TInt id = iCFFakeEnv->AddScript( KTestScript, *script, KMT_CfScriptEngineUid, thread);
    EUNIT_ASSERT_DESC( id >= 0, "Parse error!" );
    delete script;

    iCFFakeEnv->ClearFireClientEventCount();

    // Publish CONTROL context, no event should come since counting to zero and not
    co2->SetValueL(_L("True"));
    iCFFakeEnv->ClearFireClientEventCount();
    iCFFakeEnv->ClearFireClientEvent();
    err = iCFFakeEnv->PublishContext( *co2, thread );
    EUNIT_ASSERT_DESC( err == KErrNone, "PublishContext failed!" );
    EUNIT_ASSERT_DESC( !iCFFakeEnv->ClientEventFired(), "Script did cause event!" );

    // Publish COUNTED context, no event should come
    co1->SetValueL(_L("B"));
    iCFFakeEnv->ClearFireClientEvent();
    err = iCFFakeEnv->PublishContext( *co1, thread );
    EUNIT_ASSERT_DESC( err == KErrNone, "PublishContext failed!" );
    EUNIT_ASSERT_DESC( !iCFFakeEnv->ClientEventFired(), "Script did cause event!" );

    // Publish CONTROL context, no event should come
    co2->SetValueL(_L("False"));
    iCFFakeEnv->ClearFireClientEventCount();
    iCFFakeEnv->ClearFireClientEvent();
    err = iCFFakeEnv->PublishContext( *co2, thread );
    EUNIT_ASSERT_DESC( err == KErrNone, "PublishContext failed!" );
    EUNIT_ASSERT_DESC( !iCFFakeEnv->ClientEventFired(), "Script did cause event!" );

    // Publish CONTROL context, no event should come
    co2->SetValueL(_L("True"));
    iCFFakeEnv->ClearFireClientEventCount();
    iCFFakeEnv->ClearFireClientEvent();
    err = iCFFakeEnv->PublishContext( *co2, thread );
    EUNIT_ASSERT_DESC( err == KErrNone, "PublishContext failed!" );
    EUNIT_ASSERT_DESC( !iCFFakeEnv->ClientEventFired(), "Script did cause event!" );

    // REINITIALIZE
    TeardownMidtest();
    iTestDelay->Start(300000); // 0.3 s
    SetupMidtestL( *co1, *co2, KTestScript );

    // Publish CONTROL context, no event should come
    co2->SetValueL(_L("False"));
    iCFFakeEnv->ClearFireClientEventCount();
    iCFFakeEnv->ClearFireClientEvent();
    err = iCFFakeEnv->PublishContext( *co2, thread );
    EUNIT_ASSERT_DESC( err == KErrNone, "PublishContext failed!" );
    EUNIT_ASSERT_DESC( !iCFFakeEnv->ClientEventFired(), "Script did cause event!" );

    // Publish CONTROL context, no event should come
    co2->SetValueL(_L("True"));
    iCFFakeEnv->ClearFireClientEventCount();
    iCFFakeEnv->ClearFireClientEvent();
    err = iCFFakeEnv->PublishContext( *co2, thread );
    EUNIT_ASSERT_DESC( err == KErrNone, "PublishContext failed!" );
    EUNIT_ASSERT_DESC( !iCFFakeEnv->ClientEventFired(), "Script did cause event!" );

    // Publish COUNTED context, event should come
    co1->SetValueL(_L("A"));
    iCFFakeEnv->ClearFireClientEvent();
    err = iCFFakeEnv->PublishContext( *co1, thread );
    EUNIT_ASSERT_DESC( err == KErrNone, "PublishContext failed!" );
    EUNIT_ASSERT_DESC( iCFFakeEnv->ClientEventFired(), "No event!" );

    // Publish CONTROL context, no event should come
    co2->SetValueL(_L("False"));
    iCFFakeEnv->ClearFireClientEventCount();
    iCFFakeEnv->ClearFireClientEvent();
    err = iCFFakeEnv->PublishContext( *co2, thread );
    EUNIT_ASSERT_DESC( err == KErrNone, "PublishContext failed!" );
    EUNIT_ASSERT_DESC( !iCFFakeEnv->ClientEventFired(), "Script did cause event!" );

    // Publish CONTROL context, event should come
    co2->SetValueL(_L("True"));
    iCFFakeEnv->ClearFireClientEventCount();
    iCFFakeEnv->ClearFireClientEvent();
    err = iCFFakeEnv->PublishContext( *co2, thread );
    EUNIT_ASSERT_DESC( err == KErrNone, "PublishContext failed!" );
    EUNIT_ASSERT_DESC( iCFFakeEnv->ClientEventFired(), "No event!" );

    // REINITIALIZE
    TeardownMidtest();
    iTestDelay->Start(300000); // 0.3 s
    SetupMidtestL( *co1, *co2, KTestScript );

    // Publish COUNTED context, no event should come
    co1->SetValueL(_L("B"));
    iCFFakeEnv->ClearFireClientEvent();
    err = iCFFakeEnv->PublishContext( *co1, thread );
    EUNIT_ASSERT_DESC( err == KErrNone, "PublishContext failed!" );
    EUNIT_ASSERT_DESC( !iCFFakeEnv->ClientEventFired(), "Script did cause event!" );

    // Publish COUNTED context, no event should come
    co1->SetValueL(_L("A"));
    iCFFakeEnv->ClearFireClientEvent();
    err = iCFFakeEnv->PublishContext( *co1, thread );
    EUNIT_ASSERT_DESC( err == KErrNone, "PublishContext failed!" );
    EUNIT_ASSERT_DESC( !iCFFakeEnv->ClientEventFired(), "Script did cause event!" );

    // Publish CONTROL context, no event should come
    co2->SetValueL(_L("False"));
    iCFFakeEnv->ClearFireClientEventCount();
    iCFFakeEnv->ClearFireClientEvent();
    err = iCFFakeEnv->PublishContext( *co2, thread );
    EUNIT_ASSERT_DESC( err == KErrNone, "PublishContext failed!" );
    EUNIT_ASSERT_DESC( !iCFFakeEnv->ClientEventFired(), "Script did cause event!" );

    // Publish CONTROL context, event should come
    co2->SetValueL(_L("True"));
    iCFFakeEnv->ClearFireClientEventCount();
    iCFFakeEnv->ClearFireClientEvent();
    err = iCFFakeEnv->PublishContext( *co2, thread );
    EUNIT_ASSERT_DESC( err == KErrNone, "PublishContext failed!" );
    EUNIT_ASSERT_DESC( iCFFakeEnv->ClientEventFired(), "No event!" );

    CleanupStack::PopAndDestroy( co2 );
    CleanupStack::PopAndDestroy( co1 );
    CleanupStack::PopAndDestroy( &thread );
    }

void mt_basicoperationsplugin::
MT_ScriptTest_countOneShot_changeWithValue_persistentDuration2_L()
    {
    RThread thread;
    CleanupClosePushL(thread);

    _LIT( KTestScript, "test-countOneShotWithValuePersistentDuration7.xml" );
    _LIT( KPersistencyFileName, "testCountPersistency.dat");
    DeletePersistencyFile( KTestScript, KPersistencyFileName );

    // Define context
    static _LIT_SECURITY_POLICY_PASS( alwaysPass );
    CCFContextObject* co1 = CCFContextObject::NewLC();
    co1->SetSourceL(_L("Test"));
    co1->SetTypeL(_L("Counting"));
    TInt err = iCFFakeEnv->DefineContext( co1->Source(),
            co1->Type(), alwaysPass, alwaysPass );
    EUNIT_ASSERT_DESC( err == KErrNone, "DefineContext failed!" );

    CCFContextObject* co2 = CCFContextObject::NewLC();
    co2->SetSourceL(_L("Test"));
    co2->SetTypeL(_L("Control"));
    err = iCFFakeEnv->DefineContext( co2->Source(),
            co2->Type(), alwaysPass, alwaysPass );
    EUNIT_ASSERT_DESC( err == KErrNone, "DefineContext failed!" );

    // load script
    HBufC8* script = LoadScriptFromFile( KTestScript );
    TInt id = iCFFakeEnv->AddScript( KTestScript, *script, KMT_CfScriptEngineUid, thread);
    EUNIT_ASSERT_DESC( id >= 0, "Parse error!" );
    delete script;

    iCFFakeEnv->ClearFireClientEventCount();

    // Publish CONTROL context, event should come since counting to zero
    co2->SetValueL(_L("True"));
    iCFFakeEnv->ClearFireClientEventCount();
    iCFFakeEnv->ClearFireClientEvent();
    err = iCFFakeEnv->PublishContext( *co2, thread );
    EUNIT_ASSERT_DESC( err == KErrNone, "PublishContext failed!" );
    EUNIT_ASSERT_DESC( iCFFakeEnv->ClientEventFired(), "No event!" );

    // Publish COUNTED context, no event should come
    co1->SetValueL(_L("B"));
    iCFFakeEnv->ClearFireClientEvent();
    err = iCFFakeEnv->PublishContext( *co1, thread );
    EUNIT_ASSERT_DESC( err == KErrNone, "PublishContext failed!" );
    EUNIT_ASSERT_DESC( !iCFFakeEnv->ClientEventFired(), "Script did cause event!" );

    // Publish CONTROL context, no event should come
    co2->SetValueL(_L("False"));
    iCFFakeEnv->ClearFireClientEventCount();
    iCFFakeEnv->ClearFireClientEvent();
    err = iCFFakeEnv->PublishContext( *co2, thread );
    EUNIT_ASSERT_DESC( err == KErrNone, "PublishContext failed!" );
    EUNIT_ASSERT_DESC( !iCFFakeEnv->ClientEventFired(), "Script did cause event!" );

    // Publish CONTROL context, event should come
    co2->SetValueL(_L("True"));
    iCFFakeEnv->ClearFireClientEventCount();
    iCFFakeEnv->ClearFireClientEvent();
    err = iCFFakeEnv->PublishContext( *co2, thread );
    EUNIT_ASSERT_DESC( err == KErrNone, "PublishContext failed!" );
    EUNIT_ASSERT_DESC( iCFFakeEnv->ClientEventFired(), "No event!" );

    // REINITIALIZE
    TeardownMidtest();
    iTestDelay->Start(300000); // 0.3 s
    SetupMidtestL( *co1, *co2, KTestScript );

    // Publish CONTROL context, no event should come
    co2->SetValueL(_L("False"));
    iCFFakeEnv->ClearFireClientEventCount();
    iCFFakeEnv->ClearFireClientEvent();
    err = iCFFakeEnv->PublishContext( *co2, thread );
    EUNIT_ASSERT_DESC( err == KErrNone, "PublishContext failed!" );
    EUNIT_ASSERT_DESC( !iCFFakeEnv->ClientEventFired(), "Script did cause event!" );

    // Publish CONTROL context, event should come
    co2->SetValueL(_L("True"));
    iCFFakeEnv->ClearFireClientEventCount();
    iCFFakeEnv->ClearFireClientEvent();
    err = iCFFakeEnv->PublishContext( *co2, thread );
    EUNIT_ASSERT_DESC( err == KErrNone, "PublishContext failed!" );
    EUNIT_ASSERT_DESC( iCFFakeEnv->ClientEventFired(), "No event!" );

    // Publish COUNTED context, no event should come
    co1->SetValueL(_L("A"));
    iCFFakeEnv->ClearFireClientEvent();
    err = iCFFakeEnv->PublishContext( *co1, thread );
    EUNIT_ASSERT_DESC( err == KErrNone, "PublishContext failed!" );
    EUNIT_ASSERT_DESC( !iCFFakeEnv->ClientEventFired(), "Script did cause event!" );

    // Publish CONTROL context, no event should come
    co2->SetValueL(_L("False"));
    iCFFakeEnv->ClearFireClientEventCount();
    iCFFakeEnv->ClearFireClientEvent();
    err = iCFFakeEnv->PublishContext( *co2, thread );
    EUNIT_ASSERT_DESC( err == KErrNone, "PublishContext failed!" );
    EUNIT_ASSERT_DESC( !iCFFakeEnv->ClientEventFired(), "Script did cause event!" );

    // Publish CONTROL context, no event should come
    co2->SetValueL(_L("True"));
    iCFFakeEnv->ClearFireClientEventCount();
    iCFFakeEnv->ClearFireClientEvent();
    err = iCFFakeEnv->PublishContext( *co2, thread );
    EUNIT_ASSERT_DESC( err == KErrNone, "PublishContext failed!" );
    EUNIT_ASSERT_DESC( !iCFFakeEnv->ClientEventFired(), "Script did cause event!" );

    // REINITIALIZE
    TeardownMidtest();
    iTestDelay->Start(300000); // 0.3 s
    SetupMidtestL( *co1, *co2, KTestScript );

    // Publish COUNTED context, no event should come
    co1->SetValueL(_L("B"));
    iCFFakeEnv->ClearFireClientEvent();
    err = iCFFakeEnv->PublishContext( *co1, thread );
    EUNIT_ASSERT_DESC( err == KErrNone, "PublishContext failed!" );
    EUNIT_ASSERT_DESC( !iCFFakeEnv->ClientEventFired(), "Script did cause event!" );

    // Publish COUNTED context, no event should come
    co1->SetValueL(_L("A"));
    iCFFakeEnv->ClearFireClientEvent();
    err = iCFFakeEnv->PublishContext( *co1, thread );
    EUNIT_ASSERT_DESC( err == KErrNone, "PublishContext failed!" );
    EUNIT_ASSERT_DESC( !iCFFakeEnv->ClientEventFired(), "Script did cause event!" );

    // Publish CONTROL context, no event should come
    co2->SetValueL(_L("False"));
    iCFFakeEnv->ClearFireClientEventCount();
    iCFFakeEnv->ClearFireClientEvent();
    err = iCFFakeEnv->PublishContext( *co2, thread );
    EUNIT_ASSERT_DESC( err == KErrNone, "PublishContext failed!" );
    EUNIT_ASSERT_DESC( !iCFFakeEnv->ClientEventFired(), "Script did cause event!" );

    // Publish CONTROL context, no event should come
    co2->SetValueL(_L("True"));
    iCFFakeEnv->ClearFireClientEventCount();
    iCFFakeEnv->ClearFireClientEvent();
    err = iCFFakeEnv->PublishContext( *co2, thread );
    EUNIT_ASSERT_DESC( err == KErrNone, "PublishContext failed!" );
    EUNIT_ASSERT_DESC( !iCFFakeEnv->ClientEventFired(), "Script did cause event!" );

    CleanupStack::PopAndDestroy( co2 );
    CleanupStack::PopAndDestroy( co1 );
    CleanupStack::PopAndDestroy( &thread );
    }

void mt_basicoperationsplugin::
MT_ScriptTest_countOneShot_triggerWithValue_persistentDuration2_L()
    {
    RThread thread;
    CleanupClosePushL(thread);

    _LIT( KTestScript, "test-countOneShotWithValuePersistentDuration8.xml" );
    _LIT( KPersistencyFileName, "testCountPersistency.dat");
    DeletePersistencyFile( KTestScript, KPersistencyFileName );

    // Define context
    static _LIT_SECURITY_POLICY_PASS( alwaysPass );
    CCFContextObject* co1 = CCFContextObject::NewLC();
    co1->SetSourceL(_L("Test"));
    co1->SetTypeL(_L("Counting"));
    TInt err = iCFFakeEnv->DefineContext( co1->Source(),
            co1->Type(), alwaysPass, alwaysPass );
    EUNIT_ASSERT_DESC( err == KErrNone, "DefineContext failed!" );

    CCFContextObject* co2 = CCFContextObject::NewLC();
    co2->SetSourceL(_L("Test"));
    co2->SetTypeL(_L("Control"));
    err = iCFFakeEnv->DefineContext( co2->Source(),
            co2->Type(), alwaysPass, alwaysPass );
    EUNIT_ASSERT_DESC( err == KErrNone, "DefineContext failed!" );

    // load script
    HBufC8* script = LoadScriptFromFile( KTestScript );
    TInt id = iCFFakeEnv->AddScript( KTestScript, *script, KMT_CfScriptEngineUid, thread);
    EUNIT_ASSERT_DESC( id >= 0, "Parse error!" );
    delete script;

    iCFFakeEnv->ClearFireClientEventCount();

    // Publish CONTROL context, event should come since counting to zero
    co2->SetValueL(_L("True"));
    iCFFakeEnv->ClearFireClientEventCount();
    iCFFakeEnv->ClearFireClientEvent();
    err = iCFFakeEnv->PublishContext( *co2, thread );
    EUNIT_ASSERT_DESC( err == KErrNone, "PublishContext failed!" );
    EUNIT_ASSERT_DESC( iCFFakeEnv->ClientEventFired(), "No event!" );

    // Publish COUNTED context, no event should come
    co1->SetValueL(_L("B"));
    iCFFakeEnv->ClearFireClientEvent();
    err = iCFFakeEnv->PublishContext( *co1, thread );
    EUNIT_ASSERT_DESC( err == KErrNone, "PublishContext failed!" );
    EUNIT_ASSERT_DESC( !iCFFakeEnv->ClientEventFired(), "Script did cause event!" );

    // Publish CONTROL context, no event should come
    co2->SetValueL(_L("False"));
    iCFFakeEnv->ClearFireClientEventCount();
    iCFFakeEnv->ClearFireClientEvent();
    err = iCFFakeEnv->PublishContext( *co2, thread );
    EUNIT_ASSERT_DESC( err == KErrNone, "PublishContext failed!" );
    EUNIT_ASSERT_DESC( !iCFFakeEnv->ClientEventFired(), "Script did cause event!" );

    // Publish CONTROL context, event should come
    co2->SetValueL(_L("True"));
    iCFFakeEnv->ClearFireClientEventCount();
    iCFFakeEnv->ClearFireClientEvent();
    err = iCFFakeEnv->PublishContext( *co2, thread );
    EUNIT_ASSERT_DESC( err == KErrNone, "PublishContext failed!" );
    EUNIT_ASSERT_DESC( iCFFakeEnv->ClientEventFired(), "No event!" );

    // REINITIALIZE
    TeardownMidtest();
    iTestDelay->Start(300000); // 0.3 s
    SetupMidtestL( *co1, *co2, KTestScript );

    // Publish CONTROL context, no event should come
    co2->SetValueL(_L("False"));
    iCFFakeEnv->ClearFireClientEventCount();
    iCFFakeEnv->ClearFireClientEvent();
    err = iCFFakeEnv->PublishContext( *co2, thread );
    EUNIT_ASSERT_DESC( err == KErrNone, "PublishContext failed!" );
    EUNIT_ASSERT_DESC( !iCFFakeEnv->ClientEventFired(), "Script did cause event!" );

    // Publish CONTROL context, event should come
    co2->SetValueL(_L("True"));
    iCFFakeEnv->ClearFireClientEventCount();
    iCFFakeEnv->ClearFireClientEvent();
    err = iCFFakeEnv->PublishContext( *co2, thread );
    EUNIT_ASSERT_DESC( err == KErrNone, "PublishContext failed!" );
    EUNIT_ASSERT_DESC( iCFFakeEnv->ClientEventFired(), "No event!" );

    // Publish COUNTED context, no event should come
    co1->SetValueL(_L("A"));
    iCFFakeEnv->ClearFireClientEvent();
    err = iCFFakeEnv->PublishContext( *co1, thread );
    EUNIT_ASSERT_DESC( err == KErrNone, "PublishContext failed!" );
    EUNIT_ASSERT_DESC( !iCFFakeEnv->ClientEventFired(), "Script did cause event!" );

    // Publish CONTROL context, no event should come
    co2->SetValueL(_L("False"));
    iCFFakeEnv->ClearFireClientEventCount();
    iCFFakeEnv->ClearFireClientEvent();
    err = iCFFakeEnv->PublishContext( *co2, thread );
    EUNIT_ASSERT_DESC( err == KErrNone, "PublishContext failed!" );
    EUNIT_ASSERT_DESC( !iCFFakeEnv->ClientEventFired(), "Script did cause event!" );

    // Publish CONTROL context, no event should come
    co2->SetValueL(_L("True"));
    iCFFakeEnv->ClearFireClientEventCount();
    iCFFakeEnv->ClearFireClientEvent();
    err = iCFFakeEnv->PublishContext( *co2, thread );
    EUNIT_ASSERT_DESC( err == KErrNone, "PublishContext failed!" );
    EUNIT_ASSERT_DESC( !iCFFakeEnv->ClientEventFired(), "Script did cause event!" );

    // REINITIALIZE
    TeardownMidtest();
    iTestDelay->Start(300000); // 0.3 s
    SetupMidtestL( *co1, *co2, KTestScript );

    // Publish COUNTED context, no event should come
    co1->SetValueL(_L("B"));
    iCFFakeEnv->ClearFireClientEvent();
    err = iCFFakeEnv->PublishContext( *co1, thread );
    EUNIT_ASSERT_DESC( err == KErrNone, "PublishContext failed!" );
    EUNIT_ASSERT_DESC( !iCFFakeEnv->ClientEventFired(), "Script did cause event!" );

    // Publish COUNTED context, no event should come
    co1->SetValueL(_L("A"));
    iCFFakeEnv->ClearFireClientEvent();
    err = iCFFakeEnv->PublishContext( *co1, thread );
    EUNIT_ASSERT_DESC( err == KErrNone, "PublishContext failed!" );
    EUNIT_ASSERT_DESC( !iCFFakeEnv->ClientEventFired(), "Script did cause event!" );

    // Publish CONTROL context, no event should come
    co2->SetValueL(_L("False"));
    iCFFakeEnv->ClearFireClientEventCount();
    iCFFakeEnv->ClearFireClientEvent();
    err = iCFFakeEnv->PublishContext( *co2, thread );
    EUNIT_ASSERT_DESC( err == KErrNone, "PublishContext failed!" );
    EUNIT_ASSERT_DESC( !iCFFakeEnv->ClientEventFired(), "Script did cause event!" );

    // Publish CONTROL context, no event should come
    co2->SetValueL(_L("True"));
    iCFFakeEnv->ClearFireClientEventCount();
    iCFFakeEnv->ClearFireClientEvent();
    err = iCFFakeEnv->PublishContext( *co2, thread );
    EUNIT_ASSERT_DESC( err == KErrNone, "PublishContext failed!" );
    EUNIT_ASSERT_DESC( !iCFFakeEnv->ClientEventFired(), "Script did cause event!" );

    CleanupStack::PopAndDestroy( co2 );
    CleanupStack::PopAndDestroy( co1 );
    CleanupStack::PopAndDestroy( &thread );
    }

void mt_basicoperationsplugin::MT_ScriptTest_countRepeating_withValue_persistent_L()
    {
    RThread thread;
    CleanupClosePushL(thread);

    _LIT( KTestScript, "test-countRepeatingWithValuePersistent.xml" );
    _LIT( KPersistencyFileName, "testCountPersistency.dat");
    DeletePersistencyFile( KTestScript, KPersistencyFileName );

    // Define context
    static _LIT_SECURITY_POLICY_PASS( alwaysPass );
    CCFContextObject* co1 = CCFContextObject::NewLC();
    co1->SetSourceL(_L("Test"));
    co1->SetTypeL(_L("Counting"));
    TInt err = iCFFakeEnv->DefineContext( co1->Source(),
            co1->Type(), alwaysPass, alwaysPass );
    EUNIT_ASSERT_DESC( err == KErrNone, "DefineContext failed!" );

    CCFContextObject* co2 = CCFContextObject::NewLC();
    co2->SetSourceL(_L("Test"));
    co2->SetTypeL(_L("Control"));
    err = iCFFakeEnv->DefineContext( co2->Source(),
            co2->Type(), alwaysPass, alwaysPass );
    EUNIT_ASSERT_DESC( err == KErrNone, "DefineContext failed!" );

    // Publish context before loading so that subscribing will find the initial
    // value used to evaluate the operation
    co1->SetValueL(_L("A"));
    iCFFakeEnv->ClearFireClientEventCount();
    iCFFakeEnv->ClearFireClientEvent();
    err = iCFFakeEnv->PublishContext( *co1, thread );
    EUNIT_ASSERT_DESC( err == KErrNone, "PublishContext failed!" );
    EUNIT_ASSERT_DESC( !iCFFakeEnv->ClientEventFired(),
        "Script did cause event!" );

    co2->SetValueL(_L("True"));
    iCFFakeEnv->ClearFireClientEventCount();
    iCFFakeEnv->ClearFireClientEvent();
    err = iCFFakeEnv->PublishContext( *co2, thread );
    EUNIT_ASSERT_DESC( err == KErrNone, "PublishContext failed!" );
    EUNIT_ASSERT_DESC( !iCFFakeEnv->ClientEventFired(),
        "Script did cause event!" );

    // load script
    HBufC8* script = LoadScriptFromFile( KTestScript );
    TInt id = iCFFakeEnv->AddScript(
            KTestScript, *script, KMT_CfScriptEngineUid, thread);
    EUNIT_ASSERT_DESC( id >= 0, "Parse error!" );
    delete script;

    iCFFakeEnv->ClearFireClientEventCount();

    // Publish COUNTED context, no event should come
    co1->SetValueL(_L("B"));
    iCFFakeEnv->ClearFireClientEvent();
    err = iCFFakeEnv->PublishContext( *co1, thread );
    EUNIT_ASSERT_DESC( err == KErrNone, "PublishContext failed!" );
    EUNIT_ASSERT_DESC( !iCFFakeEnv->ClientEventFired(),
        "Script did cause event!" );

    // REINITIALIZE
    TeardownMidtest();
    iTestDelay->Start(300000); // 0.3 s
    SetupMidtestL( *co1, *co2, KTestScript );

    // Publish COUNTED context, no event should come
    co1->SetValueL(_L("A"));
    iCFFakeEnv->ClearFireClientEvent();
    err = iCFFakeEnv->PublishContext( *co1, thread );
    EUNIT_ASSERT_DESC( err == KErrNone, "PublishContext failed!" );
    EUNIT_ASSERT_DESC( !iCFFakeEnv->ClientEventFired(),
        "Script did cause event!" );

    // REINITIALIZE
    TeardownMidtest();
    iTestDelay->Start(300000); // 0.3 s
    SetupMidtestL( *co1, *co2, KTestScript );

    // Publish COUNTED context, no event should come
    co1->SetValueL(_L("B"));
    iCFFakeEnv->ClearFireClientEvent();
    err = iCFFakeEnv->PublishContext( *co1, thread );
    EUNIT_ASSERT_DESC( err == KErrNone, "PublishContext failed!" );
    EUNIT_ASSERT_DESC( !iCFFakeEnv->ClientEventFired(),
        "Script did cause event!" );

    // Publish COUNTED context, event MUST come
    co1->SetValueL(_L("A"));
    iCFFakeEnv->ClearFireClientEvent();
    err = iCFFakeEnv->PublishContext( *co1, thread );
    EUNIT_ASSERT_DESC( err == KErrNone, "PublishContext failed!" );
    EUNIT_ASSERT_DESC( iCFFakeEnv->ClientEventFired(),
        "Script did not cause event!" );

    // REINITIALIZE
    TeardownMidtest();
    iTestDelay->Start(300000); // 0.3 s
    SetupMidtestL( *co1, *co2, KTestScript );

    // Publish COUNTED context, no event should come
    co1->SetValueL(_L("B"));
    iCFFakeEnv->ClearFireClientEvent();
    err = iCFFakeEnv->PublishContext( *co1, thread );
    EUNIT_ASSERT_DESC( err == KErrNone, "PublishContext failed!" );
    EUNIT_ASSERT_DESC( !iCFFakeEnv->ClientEventFired(),
        "Script did cause event!" );
    
    // REINITIALIZE
    TeardownMidtest();
    iTestDelay->Start(300000); // 0.3 s
    SetupMidtestL( *co1, *co2, KTestScript );

    // Publish CONTROL context, no event should come
    co2->SetValueL(_L("False"));
    iCFFakeEnv->ClearFireClientEventCount();
    iCFFakeEnv->ClearFireClientEvent();
    err = iCFFakeEnv->PublishContext( *co2, thread );
    EUNIT_ASSERT_DESC( err == KErrNone, "PublishContext failed!" );
    EUNIT_ASSERT_DESC( !iCFFakeEnv->ClientEventFired(),
        "Script did cause event!" );

    // Publish CONTROL context, event MUST come
    co2->SetValueL(_L("True"));
    iCFFakeEnv->ClearFireClientEventCount();
    iCFFakeEnv->ClearFireClientEvent();
    err = iCFFakeEnv->PublishContext( *co2, thread );
    EUNIT_ASSERT_DESC( err == KErrNone, "PublishContext failed!" );
    EUNIT_ASSERT_DESC( iCFFakeEnv->ClientEventFired(),
        "Script did not cause event!" );

    // Publish COUNTED context, no event should come
    co1->SetValueL(_L("A"));
    iCFFakeEnv->ClearFireClientEvent();
    err = iCFFakeEnv->PublishContext( *co1, thread );
    EUNIT_ASSERT_DESC( err == KErrNone, "PublishContext failed!" );
    EUNIT_ASSERT_DESC( !iCFFakeEnv->ClientEventFired(),
        "Script did cause event!" );

    // REINITIALIZE
    TeardownMidtest();
    iTestDelay->Start(300000); // 0.3 s
    SetupMidtestL( *co1, *co2, KTestScript );

    // Publish CONTROL context, no event should come
    co2->SetValueL(_L("False"));
    iCFFakeEnv->ClearFireClientEventCount();
    iCFFakeEnv->ClearFireClientEvent();
    err = iCFFakeEnv->PublishContext( *co2, thread );
    EUNIT_ASSERT_DESC( err == KErrNone, "PublishContext failed!" );
    EUNIT_ASSERT_DESC( !iCFFakeEnv->ClientEventFired(),
        "Script did cause event!" );

    // Publish CONTROL context, no event should come
    co2->SetValueL(_L("True"));
    iCFFakeEnv->ClearFireClientEventCount();
    iCFFakeEnv->ClearFireClientEvent();
    err = iCFFakeEnv->PublishContext( *co2, thread );
    EUNIT_ASSERT_DESC( err == KErrNone, "PublishContext failed!" );
    EUNIT_ASSERT_DESC( !iCFFakeEnv->ClientEventFired(),
        "Script did cause event!" );

    // Publish COUNTED context, no event should come
    co1->SetValueL(_L("B"));
    iCFFakeEnv->ClearFireClientEvent();
    err = iCFFakeEnv->PublishContext( *co1, thread );
    EUNIT_ASSERT_DESC( err == KErrNone, "PublishContext failed!" );
    EUNIT_ASSERT_DESC( !iCFFakeEnv->ClientEventFired(),
        "Script did cause event!" );

    // Publish COUNTED context, event MUST come
    co1->SetValueL(_L("A"));
    iCFFakeEnv->ClearFireClientEvent();
    err = iCFFakeEnv->PublishContext( *co1, thread );
    EUNIT_ASSERT_DESC( err == KErrNone, "PublishContext failed!" );
    EUNIT_ASSERT_DESC( iCFFakeEnv->ClientEventFired(),
        "Script did not cause event!" );

    // Publish COUNTED context, no event should come
    co1->SetValueL(_L("B"));
    iCFFakeEnv->ClearFireClientEvent();
    err = iCFFakeEnv->PublishContext( *co1, thread );
    EUNIT_ASSERT_DESC( err == KErrNone, "PublishContext failed!" );
    EUNIT_ASSERT_DESC( !iCFFakeEnv->ClientEventFired(),
        "Script did cause event!" );

    // REINITIALIZE
    TeardownMidtest();
    iTestDelay->Start(300000); // 0.3 s
    SetupMidtestL( *co1, *co2, KTestScript );

    // Publish CONTROL context, no event should come
    co2->SetValueL(_L("False"));
    iCFFakeEnv->ClearFireClientEventCount();
    iCFFakeEnv->ClearFireClientEvent();
    err = iCFFakeEnv->PublishContext( *co2, thread );
    EUNIT_ASSERT_DESC( err == KErrNone, "PublishContext failed!" );
    EUNIT_ASSERT_DESC( !iCFFakeEnv->ClientEventFired(),
        "Script did cause event!" );

    // Publish CONTROL context, event MUST come
    co2->SetValueL(_L("True"));
    iCFFakeEnv->ClearFireClientEventCount();
    iCFFakeEnv->ClearFireClientEvent();
    err = iCFFakeEnv->PublishContext( *co2, thread );
    EUNIT_ASSERT_DESC( err == KErrNone, "PublishContext failed!" );
    EUNIT_ASSERT_DESC( iCFFakeEnv->ClientEventFired(),
        "Script did not cause event!" );

    CleanupStack::PopAndDestroy( co2 );
    CleanupStack::PopAndDestroy( co1 );
    CleanupStack::PopAndDestroy( &thread );
    }

void mt_basicoperationsplugin::MT_ScriptTest_countRepeating_withValue_persistent2_L()
    {
    RThread thread;
    CleanupClosePushL(thread);

    _LIT( KTestScript, "test-countRepeatingWithValuePersistent3.xml" );
    _LIT( KPersistencyFileName, "testCountPersistency");
    DeletePersistencyFile( KTestScript, KPersistencyFileName );

    // Define context
    static _LIT_SECURITY_POLICY_PASS( alwaysPass );
    CCFContextObject* co1 = CCFContextObject::NewLC();
    co1->SetSourceL(_L("Test"));
    co1->SetTypeL(_L("Counting"));
    TInt err = iCFFakeEnv->DefineContext( co1->Source(),
            co1->Type(), alwaysPass, alwaysPass );
    EUNIT_ASSERT_DESC( err == KErrNone, "DefineContext failed!" );

    CCFContextObject* co2 = CCFContextObject::NewLC();
    co2->SetSourceL(_L("Test"));
    co2->SetTypeL(_L("Control"));
    err = iCFFakeEnv->DefineContext( co2->Source(),
            co2->Type(), alwaysPass, alwaysPass );
    EUNIT_ASSERT_DESC( err == KErrNone, "DefineContext failed!" );

    // load script
    HBufC8* script = LoadScriptFromFile( KTestScript );
    TInt id = iCFFakeEnv->AddScript( KTestScript, *script, KMT_CfScriptEngineUid, thread);
    EUNIT_ASSERT_DESC( id >= 0, "Parse error!" );
    delete script;

    iCFFakeEnv->ClearFireClientEventCount();

    // Publish CONTROL context, event should come since counting to zero
    co2->SetValueL(_L("True"));
    iCFFakeEnv->ClearFireClientEventCount();
    iCFFakeEnv->ClearFireClientEvent();
    err = iCFFakeEnv->PublishContext( *co2, thread );
    EUNIT_ASSERT_DESC( err == KErrNone, "PublishContext failed!" );
    EUNIT_ASSERT_DESC( iCFFakeEnv->ClientEventFired(), "No event!" );

    // Publish COUNTED context, no event should come
    co1->SetValueL(_L("B"));
    iCFFakeEnv->ClearFireClientEvent();
    err = iCFFakeEnv->PublishContext( *co1, thread );
    EUNIT_ASSERT_DESC( err == KErrNone, "PublishContext failed!" );
    EUNIT_ASSERT_DESC( !iCFFakeEnv->ClientEventFired(), "Script did cause event!" );

    // Publish CONTROL context, no event should come
    co2->SetValueL(_L("False"));
    iCFFakeEnv->ClearFireClientEventCount();
    iCFFakeEnv->ClearFireClientEvent();
    err = iCFFakeEnv->PublishContext( *co2, thread );
    EUNIT_ASSERT_DESC( err == KErrNone, "PublishContext failed!" );
    EUNIT_ASSERT_DESC( !iCFFakeEnv->ClientEventFired(), "Script did cause event!" );

    // Publish CONTROL context, event should come
    co2->SetValueL(_L("True"));
    iCFFakeEnv->ClearFireClientEventCount();
    iCFFakeEnv->ClearFireClientEvent();
    err = iCFFakeEnv->PublishContext( *co2, thread );
    EUNIT_ASSERT_DESC( err == KErrNone, "PublishContext failed!" );
    EUNIT_ASSERT_DESC( iCFFakeEnv->ClientEventFired(), "No event!" );

    // REINITIALIZE
    TeardownMidtest();
    iTestDelay->Start(300000); // 0.3 s
    SetupMidtestL( *co1, *co2, KTestScript );

    // Publish CONTROL context, no event should come
    co2->SetValueL(_L("False"));
    iCFFakeEnv->ClearFireClientEventCount();
    iCFFakeEnv->ClearFireClientEvent();
    err = iCFFakeEnv->PublishContext( *co2, thread );
    EUNIT_ASSERT_DESC( err == KErrNone, "PublishContext failed!" );
    EUNIT_ASSERT_DESC( !iCFFakeEnv->ClientEventFired(), "Script did cause event!" );

    // Publish CONTROL context, event should come
    co2->SetValueL(_L("True"));
    iCFFakeEnv->ClearFireClientEventCount();
    iCFFakeEnv->ClearFireClientEvent();
    err = iCFFakeEnv->PublishContext( *co2, thread );
    EUNIT_ASSERT_DESC( err == KErrNone, "PublishContext failed!" );
    EUNIT_ASSERT_DESC( iCFFakeEnv->ClientEventFired(), "No event!" );

    // Publish COUNTED context, no event should come
    co1->SetValueL(_L("A"));
    iCFFakeEnv->ClearFireClientEvent();
    err = iCFFakeEnv->PublishContext( *co1, thread );
    EUNIT_ASSERT_DESC( err == KErrNone, "PublishContext failed!" );
    EUNIT_ASSERT_DESC( !iCFFakeEnv->ClientEventFired(), "Script did cause event!" );

    // Publish CONTROL context, no event should come
    co2->SetValueL(_L("False"));
    iCFFakeEnv->ClearFireClientEventCount();
    iCFFakeEnv->ClearFireClientEvent();
    err = iCFFakeEnv->PublishContext( *co2, thread );
    EUNIT_ASSERT_DESC( err == KErrNone, "PublishContext failed!" );
    EUNIT_ASSERT_DESC( !iCFFakeEnv->ClientEventFired(), "Script did cause event!" );

    // Publish CONTROL context, no event should come
    co2->SetValueL(_L("True"));
    iCFFakeEnv->ClearFireClientEventCount();
    iCFFakeEnv->ClearFireClientEvent();
    err = iCFFakeEnv->PublishContext( *co2, thread );
    EUNIT_ASSERT_DESC( err == KErrNone, "PublishContext failed!" );
    EUNIT_ASSERT_DESC( !iCFFakeEnv->ClientEventFired(), "Script did cause event!" );

    // REINITIALIZE
    TeardownMidtest();
    iTestDelay->Start(300000); // 0.3 s
    SetupMidtestL( *co1, *co2, KTestScript );

    // Publish COUNTED context, no event should come
    co1->SetValueL(_L("B"));
    iCFFakeEnv->ClearFireClientEvent();
    err = iCFFakeEnv->PublishContext( *co1, thread );
    EUNIT_ASSERT_DESC( err == KErrNone, "PublishContext failed!" );
    EUNIT_ASSERT_DESC( !iCFFakeEnv->ClientEventFired(), "Script did cause event!" );

    // Publish COUNTED context, no event should come
    co1->SetValueL(_L("A"));
    iCFFakeEnv->ClearFireClientEvent();
    err = iCFFakeEnv->PublishContext( *co1, thread );
    EUNIT_ASSERT_DESC( err == KErrNone, "PublishContext failed!" );
    EUNIT_ASSERT_DESC( !iCFFakeEnv->ClientEventFired(), "Script did cause event!" );

    // Publish CONTROL context, no event should come
    co2->SetValueL(_L("False"));
    iCFFakeEnv->ClearFireClientEventCount();
    iCFFakeEnv->ClearFireClientEvent();
    err = iCFFakeEnv->PublishContext( *co2, thread );
    EUNIT_ASSERT_DESC( err == KErrNone, "PublishContext failed!" );
    EUNIT_ASSERT_DESC( !iCFFakeEnv->ClientEventFired(), "Script did cause event!" );

    // Publish CONTROL context, no event should come
    co2->SetValueL(_L("True"));
    iCFFakeEnv->ClearFireClientEventCount();
    iCFFakeEnv->ClearFireClientEvent();
    err = iCFFakeEnv->PublishContext( *co2, thread );
    EUNIT_ASSERT_DESC( err == KErrNone, "PublishContext failed!" );
    EUNIT_ASSERT_DESC( !iCFFakeEnv->ClientEventFired(), "Script did cause event!" );

    CleanupStack::PopAndDestroy( co2 );
    CleanupStack::PopAndDestroy( co1 );
    CleanupStack::PopAndDestroy( &thread );
    }

void mt_basicoperationsplugin::MT_ScriptTest_countRepeating_triggerWithoutValue_persistent_L()
    {
    RThread thread;
    CleanupClosePushL(thread);

    _LIT( KTestScript, "test-countRepeatingWithoutValuePersistent.xml" );
    _LIT( KPersistencyFileName, "testCountPersistency.dat");
    DeletePersistencyFile( KTestScript, KPersistencyFileName );

    // Define context
    static _LIT_SECURITY_POLICY_PASS( alwaysPass );
    CCFContextObject* co1 = CCFContextObject::NewLC();
    co1->SetSourceL(_L("Test"));
    co1->SetTypeL(_L("Counting"));
    TInt err = iCFFakeEnv->DefineContext( co1->Source(),
            co1->Type(), alwaysPass, alwaysPass );
    EUNIT_ASSERT_DESC( err == KErrNone, "DefineContext failed!" );

    CCFContextObject* co2 = CCFContextObject::NewLC();
    co2->SetSourceL(_L("Test"));
    co2->SetTypeL(_L("Control"));
    err = iCFFakeEnv->DefineContext( co2->Source(),
            co2->Type(), alwaysPass, alwaysPass );
    EUNIT_ASSERT_DESC( err == KErrNone, "DefineContext failed!" );

    // Publish context before loading so that subscribing will find the initial
    // value used to evaluate the operation
    co1->SetValueL(_L("A"));
    iCFFakeEnv->ClearFireClientEventCount();
    iCFFakeEnv->ClearFireClientEvent();
    err = iCFFakeEnv->PublishContext( *co1, thread );
    EUNIT_ASSERT_DESC( err == KErrNone, "PublishContext failed!" );
    EUNIT_ASSERT_DESC( !iCFFakeEnv->ClientEventFired(),
        "Script did cause event!" );

    co2->SetValueL(_L("True"));
    iCFFakeEnv->ClearFireClientEventCount();
    iCFFakeEnv->ClearFireClientEvent();
    err = iCFFakeEnv->PublishContext( *co2, thread );
    EUNIT_ASSERT_DESC( err == KErrNone, "PublishContext failed!" );
    EUNIT_ASSERT_DESC( !iCFFakeEnv->ClientEventFired(),
        "Script did cause event!" );

    // load script
    HBufC8* script = LoadScriptFromFile( KTestScript );
    TInt id = iCFFakeEnv->AddScript(
            KTestScript, *script, KMT_CfScriptEngineUid, thread);
    EUNIT_ASSERT_DESC( id >= 0, "Parse error!" );
    delete script;

    iCFFakeEnv->ClearFireClientEventCount();

    // Publish COUNTED context, no event should come
    co1->SetValueL(_L("B"));
    iCFFakeEnv->ClearFireClientEvent();
    err = iCFFakeEnv->PublishContext( *co1, thread );
    EUNIT_ASSERT_DESC( err == KErrNone, "PublishContext failed!" );
    EUNIT_ASSERT_DESC( !iCFFakeEnv->ClientEventFired(),
        "Script did cause event!" );

    // REINITIALIZE
    TeardownMidtest();
    iTestDelay->Start(300000); // 0.3 s
    SetupMidtestL( *co1, *co2, KTestScript );

    // Publish COUNTED context, event MUST come
    co1->SetValueL(_L("A"));
    iCFFakeEnv->ClearFireClientEvent();
    err = iCFFakeEnv->PublishContext( *co1, thread );
    EUNIT_ASSERT_DESC( err == KErrNone, "PublishContext failed!" );
    EUNIT_ASSERT_DESC( iCFFakeEnv->ClientEventFired(),
        "Script did not cause event!" );

    // REINITIALIZE
    TeardownMidtest();
    iTestDelay->Start(300000); // 0.3 s
    SetupMidtestL( *co1, *co2, KTestScript );

    // Publish CONTROL context, no event should come
    co2->SetValueL(_L("True"));
    iCFFakeEnv->ClearFireClientEventCount();
    iCFFakeEnv->ClearFireClientEvent();
    err = iCFFakeEnv->PublishContext( *co2, thread );
    EUNIT_ASSERT_DESC( err == KErrNone, "PublishContext failed!" );
    EUNIT_ASSERT_DESC( !iCFFakeEnv->ClientEventFired(),
        "Script did cause event!" );

    // Publish CONTROL context, no event should come
    co2->SetValueL(_L("False"));
    iCFFakeEnv->ClearFireClientEventCount();
    iCFFakeEnv->ClearFireClientEvent();
    err = iCFFakeEnv->PublishContext( *co2, thread );
    EUNIT_ASSERT_DESC( err == KErrNone, "PublishContext failed!" );
    EUNIT_ASSERT_DESC( !iCFFakeEnv->ClientEventFired(),
        "Script did cause event!" );

    // Publish CONTROL context, no event should come
    co2->SetValueL(_L("True"));
    iCFFakeEnv->ClearFireClientEventCount();
    iCFFakeEnv->ClearFireClientEvent();
    err = iCFFakeEnv->PublishContext( *co2, thread );
    EUNIT_ASSERT_DESC( err == KErrNone, "PublishContext failed!" );
    EUNIT_ASSERT_DESC( !iCFFakeEnv->ClientEventFired(),
        "Script did cause event!" );

    // Publish COUNTED context, no event should come
    co1->SetValueL(_L("B"));
    iCFFakeEnv->ClearFireClientEvent();
    err = iCFFakeEnv->PublishContext( *co1, thread );
    EUNIT_ASSERT_DESC( err == KErrNone, "PublishContext failed!" );
    EUNIT_ASSERT_DESC( !iCFFakeEnv->ClientEventFired(),
        "Script did cause event!" );

    // Publish COUNTED context, event MUST come
    co1->SetValueL(_L("A"));
    iCFFakeEnv->ClearFireClientEvent();
    err = iCFFakeEnv->PublishContext( *co1, thread );
    EUNIT_ASSERT_DESC( err == KErrNone, "PublishContext failed!" );
    EUNIT_ASSERT_DESC( iCFFakeEnv->ClientEventFired(),
        "Script did not cause event!" );

    // REINITIALIZE
    TeardownMidtest();
    iTestDelay->Start(300000); // 0.3 s
    SetupMidtestL( *co1, *co2, KTestScript );

    // Publish CONTROL context, no event should come
    co2->SetValueL(_L("False"));
    iCFFakeEnv->ClearFireClientEventCount();
    iCFFakeEnv->ClearFireClientEvent();
    err = iCFFakeEnv->PublishContext( *co2, thread );
    EUNIT_ASSERT_DESC( err == KErrNone, "PublishContext failed!" );
    EUNIT_ASSERT_DESC( !iCFFakeEnv->ClientEventFired(),
        "Script did cause event!" );

    // Publish CONTROL context, no event should come
    co2->SetValueL(_L("True"));
    iCFFakeEnv->ClearFireClientEventCount();
    iCFFakeEnv->ClearFireClientEvent();
    err = iCFFakeEnv->PublishContext( *co2, thread );
    EUNIT_ASSERT_DESC( err == KErrNone, "PublishContext failed!" );
    EUNIT_ASSERT_DESC( !iCFFakeEnv->ClientEventFired(),
        "Script did cause event!" );

    // Publish COUNTED context, no event should come
    co1->SetValueL(_L("B"));
    iCFFakeEnv->ClearFireClientEvent();
    err = iCFFakeEnv->PublishContext( *co1, thread );
    EUNIT_ASSERT_DESC( err == KErrNone, "PublishContext failed!" );
    EUNIT_ASSERT_DESC( !iCFFakeEnv->ClientEventFired(),
        "Script did cause event!" );
    
    // REINITIALIZE
    TeardownMidtest();
    iTestDelay->Start(300000); // 0.3 s
    SetupMidtestL( *co1, *co2, KTestScript );

    // Publish CONTROL context, no event should come
    co2->SetValueL(_L("False"));
    iCFFakeEnv->ClearFireClientEventCount();
    iCFFakeEnv->ClearFireClientEvent();
    err = iCFFakeEnv->PublishContext( *co2, thread );
    EUNIT_ASSERT_DESC( err == KErrNone, "PublishContext failed!" );
    EUNIT_ASSERT_DESC( !iCFFakeEnv->ClientEventFired(),
        "Script did cause event!" );

    // Publish CONTROL context, no event should come
    co2->SetValueL(_L("True"));
    iCFFakeEnv->ClearFireClientEventCount();
    iCFFakeEnv->ClearFireClientEvent();
    err = iCFFakeEnv->PublishContext( *co2, thread );
    EUNIT_ASSERT_DESC( err == KErrNone, "PublishContext failed!" );
    EUNIT_ASSERT_DESC( !iCFFakeEnv->ClientEventFired(),
        "Script did cause event!" );

    // Publish CONTROL context, no event should come
    co2->SetValueL(_L("False"));
    iCFFakeEnv->ClearFireClientEventCount();
    iCFFakeEnv->ClearFireClientEvent();
    err = iCFFakeEnv->PublishContext( *co2, thread );
    EUNIT_ASSERT_DESC( err == KErrNone, "PublishContext failed!" );
    EUNIT_ASSERT_DESC( !iCFFakeEnv->ClientEventFired(),
        "Script did cause event!" );

    // Publish CONTROL context, no event should come
    co1->SetValueL(_L("A"));
    iCFFakeEnv->ClearFireClientEvent();
    err = iCFFakeEnv->PublishContext( *co1, thread );
    EUNIT_ASSERT_DESC( err == KErrNone, "PublishContext failed!" );
    EUNIT_ASSERT_DESC( !iCFFakeEnv->ClientEventFired(),
        "Script did cause event!" );

    // REINITIALIZE
    TeardownMidtest();
    iTestDelay->Start(300000); // 0.3 s
    SetupMidtestL( *co1, *co2, KTestScript );

    // Publish CONTROL context, no event should come
    co2->SetValueL(_L("True"));
    iCFFakeEnv->ClearFireClientEventCount();
    iCFFakeEnv->ClearFireClientEvent();
    err = iCFFakeEnv->PublishContext( *co2, thread );
    EUNIT_ASSERT_DESC( err == KErrNone, "PublishContext failed!" );
    EUNIT_ASSERT_DESC( !iCFFakeEnv->ClientEventFired(),
        "Script did cause event!" );

    // Publish COUNTED context, no event should come
    co1->SetValueL(_L("B"));
    iCFFakeEnv->ClearFireClientEvent();
    err = iCFFakeEnv->PublishContext( *co1, thread );
    EUNIT_ASSERT_DESC( err == KErrNone, "PublishContext failed!" );
    EUNIT_ASSERT_DESC( !iCFFakeEnv->ClientEventFired(),
        "Script did cause event!" );

    // Publish COUNTED context, event MUST come
    co1->SetValueL(_L("A"));
    iCFFakeEnv->ClearFireClientEvent();
    err = iCFFakeEnv->PublishContext( *co1, thread );
    EUNIT_ASSERT_DESC( err == KErrNone, "PublishContext failed!" );
    EUNIT_ASSERT_DESC( iCFFakeEnv->ClientEventFired(),
        "Script did not cause event!" );

    // Publish COUNTED context, no event should come
    co1->SetValueL(_L("B"));
    iCFFakeEnv->ClearFireClientEvent();
    err = iCFFakeEnv->PublishContext( *co1, thread );
    EUNIT_ASSERT_DESC( err == KErrNone, "PublishContext failed!" );
    EUNIT_ASSERT_DESC( !iCFFakeEnv->ClientEventFired(),
        "Script did cause event!" );

    // Publish CONTROL context, no event should come
    co2->SetValueL(_L("False"));
    iCFFakeEnv->ClearFireClientEventCount();
    iCFFakeEnv->ClearFireClientEvent();
    err = iCFFakeEnv->PublishContext( *co2, thread );
    EUNIT_ASSERT_DESC( err == KErrNone, "PublishContext failed!" );
    EUNIT_ASSERT_DESC( !iCFFakeEnv->ClientEventFired(),
        "Script did cause event!" );

    // Publish CONTROL context, no event should come
    co2->SetValueL(_L("True"));
    iCFFakeEnv->ClearFireClientEventCount();
    iCFFakeEnv->ClearFireClientEvent();
    err = iCFFakeEnv->PublishContext( *co2, thread );
    EUNIT_ASSERT_DESC( err == KErrNone, "PublishContext failed!" );
    EUNIT_ASSERT_DESC( !iCFFakeEnv->ClientEventFired(),
        "Script did cause event!" );

    CleanupStack::PopAndDestroy( co2 );
    CleanupStack::PopAndDestroy( co1 );
    CleanupStack::PopAndDestroy( &thread );
    }

void mt_basicoperationsplugin::MT_ScriptTest_SubscriptionSecurity_L( )
    {
    RThread thread;
    CleanupClosePushL(thread);

    // load script
    HBufC8* script = LoadScriptFromFile(_L("test-equals.xml"));
    TInt id = iCFFakeEnv->AddScript(
            _L("test-equals.xml"), *script, KMT_CfScriptEngineUid, thread);

    // adding script should cause error because contexts
    // used have not been defined
    EUNIT_ASSERT_DESC( id < 0,
        "No error of loading script without contexts defined!" );
    delete script;
    script = NULL;

    // Define and publish context
//    static _LIT_SECURITY_POLICY_PASS( alwaysPass );
    static _LIT_SECURITY_POLICY_FAIL( alwaysFail );
    CCFContextObject* co = CCFContextObject::NewLC();
    co->SetSourceL(_L("t"));
    co->SetTypeL(_L("t.p.app"));
    TInt err = iCFFakeEnv->DefineContext( co->Source(),
            co->Type(), alwaysFail, alwaysFail );
    EUNIT_ASSERT_DESC( err == KErrNone, "DefineContext failed!" );

    script = LoadScriptFromFile(_L("test-equals.xml"));
    id = iCFFakeEnv->AddScript(
            _L("test-equals.xml"), *script, KMT_CfScriptEngineUid, thread);

    // adding script should cause error because contexts
    // used have capability requirements that cannot be met
    EUNIT_ASSERT_DESC( id < 0,
        "No error despite of impossible capabilities defined!" );
    delete script;

    CleanupStack::PopAndDestroy( co );
    CleanupStack::PopAndDestroy( &thread );
    }

void mt_basicoperationsplugin::MT_ScriptTest_ActionSecurity_L( )
    {
    RThread thread;
    CleanupClosePushL(thread);

    // Define context
    static _LIT_SECURITY_POLICY_PASS( alwaysPass );
    CCFContextObject* co = CCFContextObject::NewLC();
    co->SetSourceL(_L("t"));
    co->SetTypeL(_L("t.p.app"));
    TInt err = iCFFakeEnv->DefineContext( co->Source(),
            co->Type(), alwaysPass, alwaysPass );
    EUNIT_ASSERT_DESC( err == KErrNone, "DefineContext failed!" );

    // load script
    HBufC8* script = LoadScriptFromFile(_L("test-equals.xml"));

    iCFFakeEnv->SetActionNotFound( 1 );
    TInt id = iCFFakeEnv->AddScript(
            _L("test-equals.xml"), *script, KMT_CfScriptEngineUid, thread);

    // adding script should cause error because
    // action is not present
    EUNIT_ASSERT_DESC( id < 0,
        "No error of loading script although action not found!" );
    delete script;
    script = NULL;

    script = LoadScriptFromFile( _L("test-equals.xml") );

    iCFFakeEnv->SetActionNotFound( 0 );
    iCFFakeEnv->SetAlwaysFailingActionPolicy( 1 );
    id = iCFFakeEnv->AddScript(
            _L("test-equals.xml"), *script, KMT_CfScriptEngineUid, thread);

    // adding script should cause error because actions used
    // have capability requirements that cannot be met
    EUNIT_ASSERT_DESC( id < 0,
        "No error of despite of impossible capabilities in action!" );
    delete script;

    CleanupStack::PopAndDestroy( co );
    CleanupStack::PopAndDestroy( &thread );
    }

void mt_basicoperationsplugin::MT_ScriptTest_OperationParsingL()
    {
    RThread thread;
    CleanupClosePushL(thread);

    // Define contexts
    static _LIT_SECURITY_POLICY_PASS( alwaysPass );
    CCFContextObject* co1 = CCFContextObject::NewLC();
    co1->SetSourceL(_L("Application.State"));
    co1->SetTypeL(_L("Foreground.Application"));
    TInt err = iCFFakeEnv->DefineContext( co1->Source(),
            co1->Type(), alwaysPass, alwaysPass );
    EUNIT_ASSERT_DESC( err == KErrNone, "DefineContext failed!" );

    // load script
    HBufC8* script = LoadScriptFromFile(_L("test-parsing.xml"));
    TInt id = iCFFakeEnv->AddScript(
            _L("test-parsing.xml"), *script, KMT_CfScriptEngineUid, thread);
    EUNIT_ASSERT_DESC( id >= 0, "Parse error!" );
    delete script;
    script = NULL;

    script = LoadScriptFromFile(_L("test-parsing2.xml"));
    id = iCFFakeEnv->AddScript(
            _L("test-parsing2.xml"), *script, KMT_CfScriptEngineUid, thread);
    EUNIT_ASSERT_DESC( id < 0, "Parse error!" );
    delete script;
    script = NULL;

    script = LoadScriptFromFile(_L("test-parsing3.xml"));
    id = iCFFakeEnv->AddScript(
            _L("test-parsing3.xml"), *script, KMT_CfScriptEngineUid, thread);
    EUNIT_ASSERT_DESC( id < 0, "Parse error!" );
    delete script;

    CleanupStack::PopAndDestroy( co1 );
    CleanupStack::PopAndDestroy( &thread );
    }


//  TEST TABLE
EUNIT_BEGIN_TEST_TABLE(
    mt_basicoperationsplugin,
    "Add test suite description here.",
    "MODULE" )


EUNIT_TEST(
    "<equals> - test ",
    "MCFScriptEngineInterface",
    "Parameters",
    "FUNCTIONALITY",
    SetupL, MT_ScriptTest_equals_L, Teardown)

EUNIT_TEST(
    "<notEqual> - test ",
    "MCFScriptEngineInterface",
    "Parameters",
    "FUNCTIONALITY",
    SetupL, MT_ScriptTest_notEqual_L, Teardown)

EUNIT_TEST(
    "<less> - test ",
    "MCFScriptEngineInterface",
    "Parameters",
    "FUNCTIONALITY",
    SetupL, MT_ScriptTest_less_L, Teardown)

EUNIT_TEST(
    "<greater> - test ",
    "MCFScriptEngineInterface",
    "Parameters",
    "FUNCTIONALITY",
    SetupL, MT_ScriptTest_greater_L, Teardown)

EUNIT_TEST(
    "<lessOrEqual> - test ",
    "MCFScriptEngineInterface",
    "Parameters",
    "FUNCTIONALITY",
    SetupL, MT_ScriptTest_lessOrEqual_L, Teardown)

EUNIT_TEST(
    "<greaterOrEqual> - test ",
    "MCFScriptEngineInterface",
    "Parameters",
    "FUNCTIONALITY",
    SetupL, MT_ScriptTest_greaterOrEqual_L, Teardown)

EUNIT_TEST(
    "<contains> - test ",
    "MCFScriptEngineInterface",
    "Parameters",
    "FUNCTIONALITY",
    SetupL, MT_ScriptTest_contains_L, Teardown)

EUNIT_TEST(
    "<inRange> - test ",
    "MCFScriptEngineInterface",
    "Parameters",
    "FUNCTIONALITY",
    SetupL, MT_ScriptTest_inRange_L, Teardown)

EUNIT_TEST(
    "<contextChanged> - test ",
    "MCFScriptEngineInterface",
    "Parameters",
    "FUNCTIONALITY",
    SetupL, MT_ScriptTest_contextChanged_L, Teardown)

EUNIT_TEST(
    "<contextUpdated> - test ",
    "MCFScriptEngineInterface",
    "Parameters",
    "FUNCTIONALITY",
    SetupL, MT_ScriptTest_contextUpdated_L, Teardown)

EUNIT_TEST(
    "<and> - test ",
    "MCFScriptEngineInterface",
    "Parameters",
    "FUNCTIONALITY",
    SetupL, MT_ScriptTest_and_L, Teardown)

EUNIT_TEST(
    "<and><contextChanged> - test ",
    "MCFScriptEngineInterface",
    "Parameters",
    "FUNCTIONALITY",
    SetupL, MT_ScriptTest_and_contextChanged_L, Teardown)

EUNIT_TEST(
    "<and><contextUpdated> - test ",
    "MCFScriptEngineInterface",
    "Parameters",
    "FUNCTIONALITY",
    SetupL, MT_ScriptTest_and_contextUpdated_L, Teardown)

EUNIT_TEST(
    "<or> - test ",
    "MCFScriptEngineInterface",
    "Parameters",
    "FUNCTIONALITY",
    SetupL, MT_ScriptTest_or_L, Teardown)

EUNIT_TEST(
    "<not> - test ",
    "MCFScriptEngineInterface",
    "Parameters",
    "FUNCTIONALITY",
    SetupL, MT_ScriptTest_not_L, Teardown)

EUNIT_TEST(
    "<publishContext> - test ",
    "MCFScriptEngineInterface",
    "Parameters",
    "FUNCTIONALITY",
    SetupL, MT_ScriptTest_publishContext_L, Teardown)

EUNIT_TEST(
    "<publishContext> - test 2",
    "MCFScriptEngineInterface",
    "Parameters",
    "FUNCTIONALITY",
    SetupL, MT_ScriptTest_publishContext2_L, Teardown)

EUNIT_TEST(
    "<publishContext> - test 3",
    "MCFScriptEngineInterface",
    "Parameters",
    "FUNCTIONALITY",
    SetupL, MT_ScriptTest_publishContext3_L, Teardown)

EUNIT_TEST(
    "<elseIf> - test ",
    "MCFScriptEngineInterface",
    "Parameters",
    "FUNCTIONALITY",
    SetupL, MT_ScriptTest_elseif_L, Teardown)

EUNIT_TEST(
    "<else> - test ",
    "MCFScriptEngineInterface",
    "Parameters",
    "FUNCTIONALITY",
    SetupL, MT_ScriptTest_else_L, Teardown)

EUNIT_TEST(
    "<script delay> - test ",
    "MCFScriptEngineInterface",
    "Parameters",
    "FUNCTIONALITY",
    SetupL, MT_ScriptTest_delay_L, Teardown)

EUNIT_TEST(
    "<contextRef delay> - test ",
    "MCFScriptEngineInterface",
    "Parameters",
    "FUNCTIONALITY",
    SetupL, MT_ScriptTest_delay_contextRef_L, Teardown)

EUNIT_TEST(
    "<count repeatInterval> - test without value",
    "MCFScriptEngineInterface",
    "Parameters",
    "FUNCTIONALITY",
    SetupL, MT_ScriptTest_countRepeating_withoutValue_L, Teardown)

EUNIT_TEST(
    "<count repeatInterval trigger> - test without value",
    "MCFScriptEngineInterface",
    "Parameters",
    "FUNCTIONALITY",
    SetupL, MT_ScriptTest_countRepeating_triggerWithoutValue_L, Teardown)

EUNIT_TEST(
    "<count repeatInterval> - test with value",
    "MCFScriptEngineInterface",
    "Parameters",
    "FUNCTIONALITY",
    SetupL, MT_ScriptTest_countRepeating_withValue_L, Teardown)

EUNIT_TEST(
    "<count oneShot> - test without value",
    "MCFScriptEngineInterface",
    "Parameters",
    "FUNCTIONALITY",
    SetupL, MT_ScriptTest_countOneShot_withoutValue_L, Teardown)

EUNIT_TEST(
    "<count oneShot> - test with value",
    "MCFScriptEngineInterface",
    "Parameters",
    "FUNCTIONALITY",
    SetupL, MT_ScriptTest_countOneShot_withValue_L, Teardown)

EUNIT_TEST(
    "<count oneShot remain> - test duration with value",
    "MCFScriptEngineInterface",
    "Parameters",
    "FUNCTIONALITY",
    SetupL, MT_ScriptTest_countOneShot_remainWithValue_duration_L, Teardown)

EUNIT_TEST(
    "<count oneShot change> - test duration with value",
    "MCFScriptEngineInterface",
    "Parameters",
    "FUNCTIONALITY",
    SetupL, MT_ScriptTest_countOneShot_changeWithValue_duration_L, Teardown)

EUNIT_TEST(
    "<count oneShot trigger> - test duration with value",
    "MCFScriptEngineInterface",
    "Parameters",
    "FUNCTIONALITY",
    SetupL, MT_ScriptTest_countOneShot_triggerWithValue_duration_L, Teardown)

EUNIT_TEST(
    "<count oS pers remain> - test persist duration with value",
    "MCFScriptEngineInterface",
    "Parameters",
    "FUNCTIONALITY",
    SetupL, MT_ScriptTest_countOneShot_remainWithValue_persistentDuration_L, Teardown)

EUNIT_TEST(
    "<count oS pers change> - test persist duration with value",
    "MCFScriptEngineInterface",
    "Parameters",
    "FUNCTIONALITY",
    SetupL, MT_ScriptTest_countOneShot_changeWithValue_persistentDuration_L, Teardown)

EUNIT_TEST(
    "<count oS pers trigger> - test persist duration with value",
    "MCFScriptEngineInterface",
    "Parameters",
    "FUNCTIONALITY",
    SetupL, MT_ScriptTest_countOneShot_triggerWithValue_persistentDuration_L, Teardown)

EUNIT_TEST(
    "<count oS pers2 remain> - test persist 2 duration with value and not",
    "MCFScriptEngineInterface",
    "Parameters",
    "FUNCTIONALITY",
    SetupL, MT_ScriptTest_countOneShot_remainWithValue_persistentDuration2_L, Teardown)

EUNIT_TEST(
    "<count oS pers3 remain> - test persist 3 count to zero with duration and value",
    "MCFScriptEngineInterface",
    "Parameters",
    "FUNCTIONALITY",
    SetupL, MT_ScriptTest_countOneShot_remainWithValue_persistentDuration3_L, Teardown)

EUNIT_TEST(
    "<count oS pers4 remain> - test persist 4 count to zero with duration, value and not",
    "MCFScriptEngineInterface",
    "Parameters",
    "FUNCTIONALITY",
    SetupL, MT_ScriptTest_countOneShot_remainWithValue_persistentDuration4_L, Teardown)

EUNIT_TEST(
    "<count oS pers2 change> - test persist 2 count to zero with duration and value",
    "MCFScriptEngineInterface",
    "Parameters",
    "FUNCTIONALITY",
    SetupL, MT_ScriptTest_countOneShot_changeWithValue_persistentDuration2_L, Teardown)

EUNIT_TEST(
    "<count oS pers2 trigger> - test persist 2 count to zero with duration and value",
    "MCFScriptEngineInterface",
    "Parameters",
    "FUNCTIONALITY",
    SetupL, MT_ScriptTest_countOneShot_triggerWithValue_persistentDuration2_L, Teardown)

EUNIT_TEST(
    "<count repeatInterval> - test persist with value",
    "MCFScriptEngineInterface",
    "Parameters",
    "FUNCTIONALITY",
    SetupL, MT_ScriptTest_countRepeating_withValue_persistent_L, Teardown)

EUNIT_TEST(
    "<count 2 repeatInterval> - test zero repeat interval and persist with value",
    "MCFScriptEngineInterface",
    "Parameters",
    "FUNCTIONALITY",
    SetupL, MT_ScriptTest_countRepeating_withValue_persistent2_L, Teardown)

EUNIT_TEST(
    "<count repeatInterval trigger> - test persist without value",
    "MCFScriptEngineInterface",
    "Parameters",
    "FUNCTIONALITY",
    SetupL, MT_ScriptTest_countRepeating_triggerWithoutValue_persistent_L, Teardown)

EUNIT_TEST(
    "parsing - test ",
    "MCFScriptEngineInterface",
    "Parameters",
    "FUNCTIONALITY",
    SetupL, MT_ScriptTest_OperationParsingL, Teardown)

EUNIT_TEST(
    "script subscription security",
    "MCFScriptEngineInterface",
    "Parameters",
    "FUNCTIONALITY",
    SetupL, MT_ScriptTest_SubscriptionSecurity_L, Teardown)

EUNIT_TEST(
    "script action security",
    "MCFScriptEngineInterface",
    "Parameters",
    "FUNCTIONALITY",
    SetupL, MT_ScriptTest_ActionSecurity_L, Teardown)
    

EUNIT_END_TEST_TABLE

//  END OF FILE

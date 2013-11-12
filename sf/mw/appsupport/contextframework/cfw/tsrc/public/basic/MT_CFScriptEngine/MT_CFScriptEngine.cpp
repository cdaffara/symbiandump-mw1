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
#include "MT_CFScriptEngine.h"

//  EXTERNAL INCLUDES
#include <EUnitMacros.h>
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
#include "basicoperationspluginconst.hrh"
#include "cfcommon.h"

// CONSTANTS

const TUid KMT_CfScriptEngineUid = {0x7E577E57};
const TUid KMT_CfScriptEngineOtherUid = {0x074E074E};
const TUid KCFServerUid = {0x10282BC4};

_LIT( KCFPrivateFolder, "c:\\private\\10282BC4\\rules\\" );

// CONSTRUCTION
MT_CFScriptEngine* MT_CFScriptEngine::NewL()
    {
    MT_CFScriptEngine* self = MT_CFScriptEngine::NewLC();
    CleanupStack::Pop();

    return self;
    }

MT_CFScriptEngine* MT_CFScriptEngine::NewLC()
    {
    MT_CFScriptEngine* self = new( ELeave ) MT_CFScriptEngine();
    CleanupStack::PushL( self );

    self->ConstructL();

    return self;
    }

// Destructor (virtual by CBase)
MT_CFScriptEngine::~MT_CFScriptEngine()
    {
    }

// Default constructor
MT_CFScriptEngine::MT_CFScriptEngine()
    {
    }

// Second phase construct
void MT_CFScriptEngine::ConstructL()
    {
    // The ConstructL from the base class CEUnitTestSuiteClass must be called.
    // It generates the test case table.
    CEUnitTestSuiteClass::ConstructL();
    }


//  METHODS

void MT_CFScriptEngine::SetupEmptyL(  )
    {
    User::LeaveIfError( iFs.Connect() );
    BaflUtils::EnsurePathExistsL( iFs, KCFPrivateFolder );
    iFileMan = CFileMan::NewL( iFs );
    iTestDelay = CCFTestDelay::NewL();
    }

void MT_CFScriptEngine::SetupL(  )
    {
    SetupEmptyL();

    iDummyScriptEngineSession = CScriptEventNotifierSession::NewL();

    iCFFakeEnv = CCFFakeEnv::NewL();    
    }

void MT_CFScriptEngine::Teardown(  )
    {
    TeardownNoFinalClose();
    
#ifdef __WINS__
    REComSession::FinalClose();
#endif
    }

void MT_CFScriptEngine::TeardownNoFinalClose(  )
    {    
    iTestDelay->Start(100000); // 0.1 seconds

    delete iDummyScriptEngineSession;
    iDummyScriptEngineSession = NULL;

    iTestDelay->Start(100000); // 0.1 seconds
    delete iTestDelay;
    iTestDelay = NULL;

    delete iCFFakeEnv;
    iCFFakeEnv = NULL;

    CleanupSystemDrive();
    delete iFileMan;
    
    iFs.Close();
    }

// -----------------------------------------------------------------------------
// MT_CFScriptEngine::LoadScriptFromFile
// -----------------------------------------------------------------------------
//
HBufC8* MT_CFScriptEngine::LoadScriptFromFile( const TDesC& aFilePath )
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


TInt MT_CFScriptEngine::FindParameter(const RKeyValueArray& aArray,
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


TInt MT_CFScriptEngine::FindIndex( const RArray<TInt>& aArray, TInt aIndex )
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


void MT_CFScriptEngine::CleanupSystemDrive()
    {
    iFileMan->RmDir( KCFPrivateFolder );
    }


void MT_CFScriptEngine::MT_CCFScriptEvent_NewLL(  )
    {
    EUNIT_ASSERT_DESC( EFalse, "Test case not implemented!" );
    }

void MT_CFScriptEngine::MT_CCFScriptEvent_ScriptIdL(  )
    {
    EUNIT_ASSERT_DESC( EFalse, "Test case not implemented!" );
    }

void MT_CFScriptEngine::MT_CCFScriptEvent_IdentifierL(  )
    {
    EUNIT_ASSERT_DESC( EFalse, "Test case not implemented!" );
    }

void MT_CFScriptEngine::MT_CCFScriptEvent_ParametersL(  )
    {
    EUNIT_ASSERT_DESC( EFalse, "Test case not implemented!" );
    }

void MT_CFScriptEngine::MT_CCFScriptHandler_NewLL(  )
    {
    // below                                        // CLEANUP<< fakeEnv
    CCFFakeEnv* fakeEnv = CCFFakeEnv::NewLC();

    CCFScriptHandler* instance
        = CCFScriptHandler::NewL( *fakeEnv, iFs, *fakeEnv, *fakeEnv );
    CleanupStack::PushL ( instance );
    EUNIT_ASSERT_DESC ( instance, "CCFScriptHandler not created!" );

    CleanupStack::PopAndDestroy( instance );
    CleanupStack::PopAndDestroy( fakeEnv );         // CLEANUP<< fakeEnv
    }

void MT_CFScriptEngine::MT_CCFScriptHandler_NewLCL(  )
    {
    // below                                        // CLEANUP<< fakeEnv
    CCFFakeEnv* fakeEnv = CCFFakeEnv::NewLC();

    CCFScriptHandler* instance
        = CCFScriptHandler::NewLC( *fakeEnv, iFs, *fakeEnv, *fakeEnv );
    EUNIT_ASSERT_DESC ( instance, "CCFScriptHandler not created!" );

    MCFStarterObserver* observer = (MCFStarterObserver*) instance;
    observer->InitializePhaseL( CCFPhaseBase::ECFDeviceStarting );
    observer->InitializePhaseL( CCFPhaseBase::ECFDeviceStarted );
    observer->InitializePhaseL( CCFPhaseBase::ECFStartEnd );

    CleanupStack::PopAndDestroy( instance );
    CleanupStack::PopAndDestroy( fakeEnv );         // CLEANUP<< fakeEnv
    }


void MT_CFScriptEngine::MT_CCFScriptHandler_RemoveScriptByIdL(  )
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
    HBufC8* script = LoadScriptFromFile(_L("test-equals.xml"));
    TInt id = iCFFakeEnv->AddScript(
            _L("test-equals.xml"), *script, thread.SecureId(), thread);
    EUNIT_ASSERT_DESC( id >= 0, "Parse error!" );
    delete script;

    // CONTEXT 1
    co->SetValueL(_L("ok"));
    iCFFakeEnv->ClearFireClientEvent();
    err = iCFFakeEnv->PublishContext( *co, thread );
    EUNIT_ASSERT_DESC( err == KErrNone, "PublishContext failed!" );
    EUNIT_ASSERT_DESC( iCFFakeEnv->ClientEventFired(), "Script did not fire event!" );
    EUNIT_ASSERT_DESC( iCFFakeEnv->ScriptEventScriptId() == id,
         "Script ID does not match!" );
    EUNIT_ASSERT_DESC( iCFFakeEnv->ScriptEventIdentifier() == _L("startApp"),
        "Identifier does not match!" );

    err = iCFFakeEnv->RemoveScriptById( id, thread );
    EUNIT_ASSERT( err == KErrNone );

    // Check that original script does not cause events anymore
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
    EUNIT_ASSERT_DESC( !iCFFakeEnv->ClientEventFired(),
         "Script did fire event!" );

    CleanupStack::PopAndDestroy( co );
    CleanupStack::PopAndDestroy( &thread );
    }


void MT_CFScriptEngine::MT_CCFScriptHandler_RemoveScriptsBySessionL(  )
    {
    RThread thread;
    CleanupClosePushL(thread);

    // Define and publish context
    static _LIT_SECURITY_POLICY_PASS( alwaysPass );
    TInt err = iCFFakeEnv->DefineContext( _L("t"),
            _L("t.p.app"), alwaysPass, alwaysPass );
    EUNIT_ASSERT_DESC( err == KErrNone, "DefineContext failed!" );

    HBufC8* script = LoadScriptFromFile(_L("test-equals.xml"));
    TInt id1 = iCFFakeEnv->AddScript(
            _L("test-equals.xml"), *script, KMT_CfScriptEngineUid, thread);
    EUNIT_ASSERT_DESC( id1 >= 0, "Parse error!" );
    delete script;

    // Define context
    err = iCFFakeEnv->DefineContext( _L("Sensor"),
            _L("X.RawValue"), alwaysPass, alwaysPass );
    EUNIT_ASSERT_DESC( err == KErrNone, "DefineContext failed!" );

    script = LoadScriptFromFile(_L("test-greater.xml"));
    TInt id2 = iCFFakeEnv->AddScript(
        _L("test-greater.xml"), *script, KMT_CfScriptEngineOtherUid, thread,
        *iDummyScriptEngineSession, NULL );
    EUNIT_ASSERT_DESC( id2 >= 0, "Parse error!" );
    delete script;

    // Define contexts
    err = iCFFakeEnv->DefineContext( _L("Application.State"),
            _L("Foreground.Application"), alwaysPass, alwaysPass );
    EUNIT_ASSERT_DESC( err == KErrNone, "DefineContext failed!" );
    err = iCFFakeEnv->DefineContext( _L("Application.State"),
            _L("Foreground.View"), alwaysPass, alwaysPass );
    EUNIT_ASSERT_DESC( err == KErrNone, "DefineContext failed!" );

    script = LoadScriptFromFile(_L("test-and.xml"));
    TInt id3 = iCFFakeEnv->AddScript(
            _L("test-and.xml"),*script, KMT_CfScriptEngineUid, thread);
    EUNIT_ASSERT_DESC( id3 >= 0, "Parse error!" );
    delete script;

    TInt numRemoved = iCFFakeEnv->RemoveScriptsBySession( *iCFFakeEnv );

    const RArray<TInt>& scriptIds = iCFFakeEnv->GetEveryScriptId();
    EUNIT_ASSERT_DESC( scriptIds.Count() == 1,
        "Wrong count of Ids in array!" );

    // Check that all Ids are found
    EUNIT_ASSERT ( FindIndex( scriptIds, id2 ) >= 0 );

    CleanupStack::PopAndDestroy( &thread );
    }


void MT_CFScriptEngine::MT_CCFScriptHandler_RemoveScriptByNameL()
    {
    _LIT( KScriptName1, "test-and.xml" );
    _LIT( KScriptName2, "test-or.xml" );
    HBufC8* script1 = LoadScriptFromFile( KScriptName1 );
    HBufC8* script2 = LoadScriptFromFile( KScriptName2 );
    EUNIT_ASSERT_DESC( script1 && script2, "Scripts not loaded" );
    if( script1 && script2 )
        {
        CleanupStack::PushL( script1 );
        CleanupStack::PushL( script2 );
        
        const TUid KClient1Uid = {0x12340001};
        const TUid KClient2Uid = {0x12340002};

        // Define needed contexts for scripts
        static _LIT_SECURITY_POLICY_PASS( alwaysPass );
        TInt err = iCFFakeEnv->DefineContext( _L("Application.State"),
            _L("Foreground.Application"), alwaysPass, alwaysPass );
        EUNIT_ASSERT_DESC( err == KErrNone, "DefineContext failed!" );
        err = iCFFakeEnv->DefineContext( _L("Application.State"),
            _L("Foreground.View"), alwaysPass, alwaysPass );
        EUNIT_ASSERT_DESC( err == KErrNone, "DefineContext failed!" );
        
        // Test data
        RThread thread;
        CleanupClosePushL( thread );
        TInt id = 0;
        TInt scriptCount = 0;
        
        // Add script 1 from client 1
        id = iCFFakeEnv->AddScript( KScriptName1, *script1,
            KClient1Uid, thread );
        EUNIT_ASSERT_DESC( id >= 0, "Add script failed" );
        scriptCount = iCFFakeEnv->NumberOfScripts();
        EUNIT_ASSERT_DESC( scriptCount == id, "Invalid number of scripts" );
        
        // Add script 2 from client 1
        id = iCFFakeEnv->AddScript( KScriptName2, *script2,
            KClient1Uid, thread );
        EUNIT_ASSERT_DESC( id >= 0, "Add script failed" );
        scriptCount = iCFFakeEnv->NumberOfScripts();
        EUNIT_ASSERT_DESC( scriptCount == id, "Invalid number of scripts" );
        
        // Add script 1 from client 2
        id = iCFFakeEnv->AddScript( KScriptName1, *script1,
            KClient2Uid, thread );
        EUNIT_ASSERT_DESC( id >= 0, "Add script failed" );
        scriptCount = iCFFakeEnv->NumberOfScripts();
        EUNIT_ASSERT_DESC( scriptCount == id, "Invalid number of scripts" );
        
        // Add script 2 from client 2
        id = iCFFakeEnv->AddScript( KScriptName2, *script1,
            KClient2Uid, thread );
        EUNIT_ASSERT_DESC( id >= 0, "Add script failed" );
        scriptCount = iCFFakeEnv->NumberOfScripts();
        EUNIT_ASSERT_DESC( scriptCount == id, "Invalid number of scripts" );
        
        // Remove script 1 by name from client 1
        err = iCFFakeEnv->RemoveScriptByName( KScriptName1, KClient1Uid );
        EUNIT_ASSERT_DESC( err == KErrNone, "Invalid return value" );
        scriptCount = iCFFakeEnv->NumberOfScriptsByOwner( KClient1Uid );
        EUNIT_ASSERT_DESC( scriptCount == 1, "Invalid number of scripts" );
        
        // Remove script 1 by name from client 2
        err = iCFFakeEnv->RemoveScriptByName( KScriptName1, KClient2Uid );
        EUNIT_ASSERT_DESC( err == KErrNone, "Invalid return value" );
        scriptCount = iCFFakeEnv->NumberOfScriptsByOwner( KClient2Uid );
        EUNIT_ASSERT_DESC( scriptCount == 1, "Invalid number of scripts" );
        
        // Remove script 2 by name from client 1
        err = iCFFakeEnv->RemoveScriptByName( KScriptName2, KClient1Uid );
        EUNIT_ASSERT_DESC( err == KErrNone, "Invalid return value" );
        scriptCount = iCFFakeEnv->NumberOfScriptsByOwner( KClient1Uid );
        EUNIT_ASSERT_DESC( scriptCount == 0, "Invalid number of scripts" );
        
        // Remove script 2 by name from client 2
        err = iCFFakeEnv->RemoveScriptByName( KScriptName2, KClient2Uid );
        EUNIT_ASSERT_DESC( err == KErrNone, "Invalid return value" );
        scriptCount = iCFFakeEnv->NumberOfScriptsByOwner( KClient2Uid );
        EUNIT_ASSERT_DESC( scriptCount == 0, "Invalid number of scripts" );
        
        // Clean up
        CleanupStack::PopAndDestroy( &thread );
        CleanupStack::PopAndDestroy( script2 );
        CleanupStack::PopAndDestroy( script1 );
        }
    }


void MT_CFScriptEngine::MT_CCFScriptHandler_RemoveScriptByUidL()
    {
    _LIT( KScriptName1, "test-and.xml" );
    _LIT( KScriptName2, "test-or.xml" );
    HBufC8* script1 = LoadScriptFromFile( KScriptName1 );
    HBufC8* script2 = LoadScriptFromFile( KScriptName2 );
    EUNIT_ASSERT_DESC( script1 && script2, "Scripts not loaded" );
    if( script1 && script2 )
        {
        CleanupStack::PushL( script1 );
        CleanupStack::PushL( script2 );
        
        // Define needed contexts for scripts
        static _LIT_SECURITY_POLICY_PASS( alwaysPass );
        TInt err = iCFFakeEnv->DefineContext( _L("Application.State"),
            _L("Foreground.Application"), alwaysPass, alwaysPass );
        EUNIT_ASSERT_DESC( err == KErrNone, "DefineContext failed!" );
        err = iCFFakeEnv->DefineContext( _L("Application.State"),
            _L("Foreground.View"), alwaysPass, alwaysPass );
        EUNIT_ASSERT_DESC( err == KErrNone, "DefineContext failed!" );
        
        // Test data
        RThread thread;
        CleanupClosePushL( thread );
        TInt id = 0;
        TInt scriptCount = 0;
        const TUid KUid1 = {0x12340000};
        const TUid KUid2 = {0x12340001};
        
        // Add script 1 twice
        TInt count = 0;
        for( TInt i = 0; i < 2; i++ )
            {
            id = iCFFakeEnv->AddScript( KScriptName1, *script1,
                KUid1, thread );
            count++;
            EUNIT_ASSERT_DESC( id >= 0, "Add script failed" );
            scriptCount = iCFFakeEnv->NumberOfScripts();
            EUNIT_ASSERT_DESC( scriptCount == count, "Invalid number of scripts" );
            }
        
        // Add script 2 three times
        for( TInt i = 0; i < 3; i++ )
            {
            id = iCFFakeEnv->AddScript( KScriptName2, *script2,
                KUid2, thread );
            count++;
            EUNIT_ASSERT_DESC( id >= 0, "Add script failed" );
            scriptCount = iCFFakeEnv->NumberOfScripts();
            EUNIT_ASSERT_DESC( scriptCount == count, "Invalid number of scripts" );
            }
        
        // Remove scripts by uid1
        err = iCFFakeEnv->RemoveScriptByUid( KUid1 );
        EUNIT_ASSERT_DESC( err == KErrNone, "RemoveScriptByUid failed" );
        scriptCount = iCFFakeEnv->NumberOfScripts();
        EUNIT_ASSERT_DESC( scriptCount == 3, "Invalid number of scripts" );
        scriptCount = iCFFakeEnv->NumberOfScriptsByOwner( KUid1 );
        EUNIT_ASSERT_DESC( scriptCount == 0, "Invalid number of scripts" );

        // Remove scripts by uid2
        err = iCFFakeEnv->RemoveScriptByUid( KUid2 );
        EUNIT_ASSERT_DESC( err == KErrNone, "RemoveScriptByUid failed" );
        scriptCount = iCFFakeEnv->NumberOfScripts();
        EUNIT_ASSERT_DESC( scriptCount == 0, "Invalid number of scripts" );
        scriptCount = iCFFakeEnv->NumberOfScriptsByOwner( KUid2 );
        EUNIT_ASSERT_DESC( scriptCount == 0, "Invalid number of scripts" );

        // Clean up
        CleanupStack::PopAndDestroy( &thread );
        CleanupStack::PopAndDestroy( script2 );
        CleanupStack::PopAndDestroy( script1 );
        }
    }


void MT_CFScriptEngine::MT_CCFScriptHandler_NumberOfScriptsL(  )
    {
    RThread thread;
    CleanupClosePushL(thread);

    EUNIT_ASSERT_DESC( iCFFakeEnv->NumberOfScripts() == 0,
            "Wrong number of scripts returned!" );

    // Define and publish context
    static _LIT_SECURITY_POLICY_PASS( alwaysPass );
    TInt err = iCFFakeEnv->DefineContext( _L("t"),
            _L("t.p.app"), alwaysPass, alwaysPass );
    EUNIT_ASSERT_DESC( err == KErrNone, "DefineContext failed!" );

    HBufC8* script = LoadScriptFromFile(_L("test-equals.xml"));
    TInt id = iCFFakeEnv->AddScript(
            _L("test-equals.xml"),*script, KMT_CfScriptEngineUid, thread);
    EUNIT_ASSERT_DESC( id >= 0, "Parse error!" );
    delete script;

    EUNIT_ASSERT_DESC( iCFFakeEnv->NumberOfScripts() == 1,
            "Wrong number of scripts returned!" );

    // Define context
    err = iCFFakeEnv->DefineContext( _L("Sensor"),
            _L("X.RawValue"), alwaysPass, alwaysPass );
    EUNIT_ASSERT_DESC( err == KErrNone, "DefineContext failed!" );

    script = LoadScriptFromFile(_L("test-greater.xml"));
    id = iCFFakeEnv->AddScript(_L("test-greater.xml"),
            *script, KMT_CfScriptEngineOtherUid, thread);
    EUNIT_ASSERT_DESC( id >= 0, "Parse error!" );
    delete script;

    EUNIT_ASSERT_DESC( iCFFakeEnv->NumberOfScripts() == 2,
            "Wrong number of scripts returned!" );

    CleanupStack::PopAndDestroy( &thread );
    }


void MT_CFScriptEngine::MT_CCFScriptHandler_ScriptLengthL(  )
    {
    RThread thread;
    CleanupClosePushL(thread);

    // Define and publish context
    static _LIT_SECURITY_POLICY_PASS( alwaysPass );
    TInt err = iCFFakeEnv->DefineContext( _L("t"),
            _L("t.p.app"), alwaysPass, alwaysPass );
    EUNIT_ASSERT_DESC( err == KErrNone, "DefineContext failed!" );

    HBufC8* script = LoadScriptFromFile(_L("test-equals.xml"));
    TInt id1 = iCFFakeEnv->AddScript(
            _L("test-equals.xml"), *script, KMT_CfScriptEngineUid, thread);
    EUNIT_ASSERT_DESC( id1 >= 0, "Parse error!" );

    TInt origLength = script->Length();
    delete script;

    EUNIT_ASSERT( iCFFakeEnv->ScriptLength( id1 ) == origLength);

    CleanupStack::PopAndDestroy( &thread );
    }


void MT_CFScriptEngine::MT_CCFScriptHandler_NumberOfScriptsByOwnerL(  )
    {
    RThread thread;
    CleanupClosePushL(thread);

    EUNIT_ASSERT_DESC(
            iCFFakeEnv->NumberOfScriptsByOwner(KMT_CfScriptEngineUid) == 0,
            "Wrong number of scripts returned!" );

    // Define and publish context
    static _LIT_SECURITY_POLICY_PASS( alwaysPass );
    TInt err = iCFFakeEnv->DefineContext( _L("t"),
            _L("t.p.app"), alwaysPass, alwaysPass );
    EUNIT_ASSERT_DESC( err == KErrNone, "DefineContext failed!" );

    HBufC8* script = LoadScriptFromFile(_L("test-equals.xml"));
    TInt id = iCFFakeEnv->AddScript(
            _L("test-equals.xml"), *script, KMT_CfScriptEngineUid, thread);
    EUNIT_ASSERT_DESC( id >= 0, "Parse error!" );
    delete script;

    EUNIT_ASSERT_DESC(
            iCFFakeEnv->NumberOfScriptsByOwner(KMT_CfScriptEngineUid) == 1,
            "Wrong number of scripts returned!" );

    // Define context for test-greater.xml
    err = iCFFakeEnv->DefineContext( _L("Sensor"),
            _L("X.RawValue"), alwaysPass, alwaysPass );
    EUNIT_ASSERT_DESC( err == KErrNone, "DefineContext failed!" );

    script = LoadScriptFromFile(_L("test-greater.xml"));
    id = iCFFakeEnv->AddScript(_L("test-greater.xml"),
            *script, KMT_CfScriptEngineOtherUid, thread);
    EUNIT_ASSERT_DESC( id >= 0, "Parse error!" );
    delete script;

    EUNIT_ASSERT_DESC(
            iCFFakeEnv->NumberOfScriptsByOwner(KMT_CfScriptEngineUid) == 1,
            "Wrong number of scripts returned!" );

    // Define contexts test-and.xml -script
    err = iCFFakeEnv->DefineContext( _L("Application.State"),
            _L("Foreground.Application"), alwaysPass, alwaysPass );
    EUNIT_ASSERT_DESC( err == KErrNone, "DefineContext failed!" );
    err = iCFFakeEnv->DefineContext( _L("Application.State"),
            _L("Foreground.View"), alwaysPass, alwaysPass );
    EUNIT_ASSERT_DESC( err == KErrNone, "DefineContext failed!" );

    script = LoadScriptFromFile(_L("test-and.xml"));
    id = iCFFakeEnv->AddScript(
            _L("test-and.xml"), *script, KMT_CfScriptEngineUid, thread);
    EUNIT_ASSERT_DESC( id >= 0, "Parse error!" );
    delete script;

    EUNIT_ASSERT_DESC(
            iCFFakeEnv->NumberOfScriptsByOwner(KMT_CfScriptEngineUid) == 2,
            "Wrong number of scripts returned!" );

    CleanupStack::PopAndDestroy( &thread );
    }

void MT_CFScriptEngine::MT_CCFScriptHandler_GetEveryScriptIdL(  )
    {
    RThread thread;
    CleanupClosePushL(thread);

    // Define context for test-equals.xml
    static _LIT_SECURITY_POLICY_PASS( alwaysPass );
    TInt err = iCFFakeEnv->DefineContext( _L("t"),
            _L("t.p.app"), alwaysPass, alwaysPass );
    EUNIT_ASSERT_DESC( err == KErrNone, "DefineContext failed!" );

    HBufC8* script = LoadScriptFromFile(_L("test-equals.xml"));
    TInt id1 = iCFFakeEnv->AddScript(
            _L("test-equals.xml"), *script, KMT_CfScriptEngineUid, thread);
    EUNIT_ASSERT_DESC( id1 >= 0, "Parse error!" );
    delete script;

    // Define context for test-greater.xml
    err = iCFFakeEnv->DefineContext( _L("Sensor"),
            _L("X.RawValue"), alwaysPass, alwaysPass );
    EUNIT_ASSERT_DESC( err == KErrNone, "DefineContext failed!" );

    script = LoadScriptFromFile(_L("test-greater.xml"));
    TInt id2 = iCFFakeEnv->AddScript(_L("test-greater.xml"),
            *script, KMT_CfScriptEngineOtherUid, thread);
    EUNIT_ASSERT_DESC( id2 >= 0, "Parse error!" );
    delete script;


    // Define contexts test-and.xml -script
    err = iCFFakeEnv->DefineContext( _L("Application.State"),
            _L("Foreground.Application"), alwaysPass, alwaysPass );
    EUNIT_ASSERT_DESC( err == KErrNone, "DefineContext failed!" );
    err = iCFFakeEnv->DefineContext( _L("Application.State"),
            _L("Foreground.View"), alwaysPass, alwaysPass );
    EUNIT_ASSERT_DESC( err == KErrNone, "DefineContext failed!" );

    script = LoadScriptFromFile(_L("test-and.xml"));
    TInt id3 = iCFFakeEnv->AddScript(
            _L("test-and.xml"), *script, KMT_CfScriptEngineUid, thread);
    EUNIT_ASSERT_DESC( id3 >= 0, "Parse error!" );
    delete script;

    const RArray<TInt>& scriptIds = iCFFakeEnv->GetEveryScriptId();
    EUNIT_ASSERT_DESC( scriptIds.Count() == 3, "Wrong count of Ids in array!" );

    // Check that all Ids are found
    EUNIT_ASSERT ( FindIndex( scriptIds, id1 ) >= 0 );
    EUNIT_ASSERT ( FindIndex( scriptIds, id2 ) >= 0 );
    EUNIT_ASSERT ( FindIndex( scriptIds, id3 ) >= 0 );

    CleanupStack::PopAndDestroy( &thread );
    }


void MT_CFScriptEngine::MT_CCFScriptHandler_GetEveryScriptIdByOwnerL(  )
    {
    RThread thread;
    CleanupClosePushL(thread);

    // Define context for test-equals.xml
    static _LIT_SECURITY_POLICY_PASS( alwaysPass );
    TInt err = iCFFakeEnv->DefineContext( _L("t"),
            _L("t.p.app"), alwaysPass, alwaysPass );
    EUNIT_ASSERT_DESC( err == KErrNone, "DefineContext failed!" );

    HBufC8* script = LoadScriptFromFile(_L("test-equals.xml"));
    TInt id1 = iCFFakeEnv->AddScript(
            _L("test-equals.xml"), *script, KMT_CfScriptEngineUid, thread);
    EUNIT_ASSERT_DESC( id1 >= 0, "Parse error!" );
    delete script;

    // Define context for test-greater.xml
    err = iCFFakeEnv->DefineContext( _L("Sensor"),
            _L("X.RawValue"), alwaysPass, alwaysPass );
    EUNIT_ASSERT_DESC( err == KErrNone, "DefineContext failed!" );

    script = LoadScriptFromFile(_L("test-greater.xml"));
    TInt id2 = iCFFakeEnv->AddScript(_L("test-greater.xml"),
            *script, KMT_CfScriptEngineOtherUid, thread);
    EUNIT_ASSERT_DESC( id2 >= 0, "Parse error!" );
    delete script;

    // Define contexts test-and.xml -script
    err = iCFFakeEnv->DefineContext( _L("Application.State"),
            _L("Foreground.Application"), alwaysPass, alwaysPass );
    EUNIT_ASSERT_DESC( err == KErrNone, "DefineContext failed!" );
    err = iCFFakeEnv->DefineContext( _L("Application.State"),
            _L("Foreground.View"), alwaysPass, alwaysPass );
    EUNIT_ASSERT_DESC( err == KErrNone, "DefineContext failed!" );

    script = LoadScriptFromFile(_L("test-and.xml"));
    TInt id3 = iCFFakeEnv->AddScript(
            _L("test-and.xml"), *script, KMT_CfScriptEngineUid, thread);
    EUNIT_ASSERT_DESC( id3 >= 0, "Parse error!" );
    delete script;

    const RArray<TInt>& scriptIds
        = iCFFakeEnv->GetEveryScriptIdByOwner(KMT_CfScriptEngineUid);
    EUNIT_ASSERT_DESC( scriptIds.Count() == 2,
        "Wrong count of Ids in array!" );

    EUNIT_ASSERT ( FindIndex( scriptIds, id1 ) >= 0 );
    EUNIT_ASSERT ( FindIndex( scriptIds, id3 ) >= 0 );

    CleanupStack::PopAndDestroy( &thread );
    }


void MT_CFScriptEngine::MT_CCFScriptHandler_UpdateScriptL(  )
    {
    RThread thread;
    CleanupClosePushL(thread);

    // Define and publish context
    static _LIT_SECURITY_POLICY_PASS( alwaysPass );
    CCFContextObject* co = CCFContextObject::NewLC();
    co->SetSourceL(_L("t"));
    co->SetTypeL(_L("t.p.app"));
    TInt err = iCFFakeEnv->DefineContext( co->Source(),
            co->Type(), alwaysPass, alwaysPass );
    EUNIT_ASSERT_DESC( err == KErrNone, "DefineContext failed!" );

    // load script
    HBufC8* script = LoadScriptFromFile(_L("test-equals.xml"));
    TInt id = iCFFakeEnv->AddScript(
            _L("test-equals.xml"), *script, KMT_CfScriptEngineUid, thread);
    EUNIT_ASSERT_DESC( id >= 0, "Parse error!" );
    delete script;

    // CONTEXT 1
    co->SetValueL(_L("ok"));
    iCFFakeEnv->ClearFireClientEvent();
    err = iCFFakeEnv->PublishContext( *co, thread );
    EUNIT_ASSERT_DESC( err == KErrNone, "PublishContext failed!" );
    EUNIT_ASSERT_DESC( iCFFakeEnv->ClientEventFired(), "Script did not fire event!" );
    EUNIT_ASSERT_DESC( iCFFakeEnv->ScriptEventScriptId() == id,
         "Script ID does not match!" );
    EUNIT_ASSERT_DESC( iCFFakeEnv->ScriptEventIdentifier() == _L("startApp"),
        "Identifier does not match!" );

    // Define context
    CCFContextObject* co2 = CCFContextObject::NewLC();
    co2->SetSourceL(_L("Sensor"));
    co2->SetTypeL(_L("X.RawValue"));
    err = iCFFakeEnv->DefineContext( co2->Source(),
            co2->Type(), alwaysPass, alwaysPass );
    EUNIT_ASSERT_DESC( err == KErrNone, "DefineContext failed!" );

    // update script
    HBufC8* updatedScript = LoadScriptFromFile(_L("test-less.xml"));
    err = iCFFakeEnv->UpdateScript(id,  thread,  *updatedScript, NULL );
    EUNIT_ASSERT_DESC( err == KErrNone, "Update error!" );
    delete updatedScript;

    // TEST 1: Should fire event if value < 150
    co2->SetValueL(_L("149")); // value in script: 150
    err = iCFFakeEnv->PublishContext( *co2, thread );
    EUNIT_ASSERT_DESC( err == KErrNone, "PublishContext failed!" );
    EUNIT_ASSERT_DESC( iCFFakeEnv->ClientEventFired(), "Script did not fire event!" );
    EUNIT_ASSERT_DESC( iCFFakeEnv->ScriptEventScriptId() == id,
         "Script ID does not match!" );
    EUNIT_ASSERT_DESC( iCFFakeEnv->ScriptEventIdentifier() == _L("startApp"),
        "Identifier does not match!" );

    // Check that original script does not cause events anymore
    co->SetValueL(_L("notok"));
    iCFFakeEnv->ClearFireClientEvent();
    err = iCFFakeEnv->PublishContext( *co, thread );
    EUNIT_ASSERT_DESC( err == KErrNone, "PublishContext failed!" );
    EUNIT_ASSERT_DESC( !iCFFakeEnv->ClientEventFired(), "Script did fire event!" );

    co->SetValueL(_L("ok"));
    iCFFakeEnv->ClearFireClientEvent();
    err = iCFFakeEnv->PublishContext( *co, thread );
    EUNIT_ASSERT_DESC( err == KErrNone, "PublishContext failed!" );
    EUNIT_ASSERT_DESC( !iCFFakeEnv->ClientEventFired(), "Script did fire event!" );

    CleanupStack::PopAndDestroy( co2 );
    CleanupStack::PopAndDestroy( co );
    CleanupStack::PopAndDestroy( &thread );
    }


void MT_CFScriptEngine::MT_CCFScriptHandler_SaveScriptL()
    {    
    // Change timestamp to yesterday
    TTime time;
    time.UniversalTime();
    TDateTime dateTime = time.DateTime();
    dateTime.SetHour( dateTime.Hour() - 1 );
    
    // Update new time
    TTime entryTime( dateTime );

    // Load script
    HBufC8* script1 = LoadScriptFromFile( _L( "test-saveScript.xml" ) );
    EUNIT_ASSERT_DESC( script1 , "Scripts not loaded" );
    if( script1 )
        {
        CleanupStack::PushL( script1 );
        const TUid KUidClient1 = {0x12340001};
        const TUid KUidClient2 = {0x12340002};
        TInt err = KErrNone;
        TInt scriptId = 0;
        TBool exists = EFalse;
        _LIT( KClient1Path1, "c:\\private\\10282BC4\\rules\\12340001\\script1.rul" );
        _LIT( KClient1Path2, "c:\\private\\10282BC4\\rules\\12340001\\script2.rul" );
        _LIT( KClient2Path1, "c:\\private\\10282BC4\\rules\\12340002\\script1.rul" );
        _LIT( KClient2Path2, "c:\\private\\10282BC4\\rules\\12340002\\script2.rul" );
        _LIT( KUpgradePath, "c:\\private\\10282BC4\\rules\\scriptupgrade.rul" );
        RThread client;
        CleanupClosePushL( client );
        
        // Define needed contexts for scripts
        static _LIT_SECURITY_POLICY_PASS( alwaysPass );
        CCFContextObject* co = CCFContextObject::NewLC();
        co->SetSourceL( _L("test") );
        co->SetTypeL( _L("save.script") );
        err = iCFFakeEnv->DefineContext( co->Source(), co->Type(), alwaysPass, alwaysPass );
        EUNIT_ASSERT_DESC( err == KErrNone, "DefineContext failed!" );

        // Test script saving from different clients
        const TInt KCount = 5;
        for( TInt i = 0; i < KCount; i++ )
            {
            TPtrC fullPath( KNullDesC );
            TUid uid = {0x00000000};
            switch( i )
                {
                case 0:
                    {
                    fullPath.Set( KClient1Path1 );
                    uid = KUidClient1;
                    break;
                    }
                case 1:
                    {
                    fullPath.Set( KClient1Path2 );
                    uid = KUidClient1;
                    break;
                    }
                case 2:
                    {
                    fullPath.Set( KClient2Path1 );
                    uid = KUidClient2;
                    break;
                    }
                case 3:
                    {
                    fullPath.Set( KClient2Path2 );
                    uid = KUidClient2;
                    break;
                    }
                case 4:
                    {
                    fullPath.Set( KUpgradePath );
                    uid = KCFServerUid;
                    break;
                    }
                default:
                    {
                    break;
                    }
                }
            TParsePtrC parse( fullPath );
            scriptId = iCFFakeEnv->AddScript( parse.Name(), *script1, uid,
                client );
            EUNIT_ASSERT_DESC( scriptId == i + 1, "Invalid script id" );
            err = iCFFakeEnv->SaveScript( *script1, scriptId, uid );
            EUNIT_ASSERT_DESC( err == KErrNone, "SaveScript failed" );
            exists = BaflUtils::FolderExists( iFs, parse.DriveAndPath() );
            EUNIT_ASSERT_DESC( exists, "Folder not created" );
            exists = BaflUtils::FileExists( iFs, parse.FullName() );
            EUNIT_ASSERT_DESC( exists, "File not created" );
            
            err = iFs.SetModified( fullPath, entryTime );
            EUNIT_ASSERT_DESC( err == KErrNone, "Failed to set modified time" );
            TEntry oldEntry;
            err = iFs.Entry( fullPath, oldEntry );
            EUNIT_ASSERT_DESC( err == KErrNone, "Failed to get file entry" );
            
            // Save script again and check that it has been replaced
            User::After( 100000 );
            err = iCFFakeEnv->SaveScript( *script1, scriptId, uid );
            EUNIT_ASSERT_DESC( err == KErrNone, "SaveScript failed" );
            TEntry newEntry;
            err = iFs.Entry( fullPath, newEntry );
            EUNIT_ASSERT_DESC( err == KErrNone, "Failed to get file entry" );
            EUNIT_ASSERT_DESC( oldEntry.iModified < newEntry.iModified,
                "Saved script not updated" );
            }
        
        // Try to save a script from with a wrong owner UID
        scriptId = iCFFakeEnv->AddScript( _L( "test" ), *script1, KUidClient1,   
            client );
        err = iCFFakeEnv->SaveScript( *script1, scriptId, KUidClient2 );
        EUNIT_ASSERT_DESC( err == KErrAccessDenied,
            "Allowed to save script with a wrong client uid" );
        
        // Check that the saved script are loaded when script engine
        // is initialized
        delete iCFFakeEnv;
        iCFFakeEnv = NULL;
        iCFFakeEnv = CCFFakeEnv::NewL();
        iCFFakeEnv->InitAllPhasesL();
        TInt numOfScripts = iCFFakeEnv->NumberOfScriptsByOwner( KUidClient1 );
        EUNIT_ASSERT_DESC( numOfScripts == 2, "Invalid number of scripts" );
        numOfScripts = iCFFakeEnv->NumberOfScriptsByOwner( KUidClient2 );
        EUNIT_ASSERT_DESC( numOfScripts == 2, "Invalid number of scripts" );

        co->SetSourceL( _L("test") );
        co->SetTypeL( _L("save.script") );
        err = iCFFakeEnv->DefineContext( co->Source(), co->Type(), alwaysPass, alwaysPass );
        EUNIT_ASSERT_DESC( err == KErrNone, "DefineContext failed!" );
        co->SetValueL( _L("true") );
        err = iCFFakeEnv->PublishContext( *co, client );
        EUNIT_ASSERT_DESC( err == KErrNone, "Publish context failed" );
        
        TBool eventFired = iCFFakeEnv->ClientEventFired();
        EUNIT_ASSERT_DESC( eventFired, "Event not fired" );
        TPtrC actionId( iCFFakeEnv->ScriptEventIdentifier() );
        EUNIT_ASSERT_DESC( actionId == _L("saveScript"), "Incorrect action id" );
        const RKeyValueArray& params = iCFFakeEnv->ScriptEventParameters();
        EUNIT_ASSERT_DESC( params[0]->Key() == _L("id"), "Incorrect action id" );
        EUNIT_ASSERT_DESC( params[0]->Value() == _L("triggered"), "Incorrect action id value" );
        
        // Clean up
        CleanupStack::PopAndDestroy( co );
        CleanupStack::PopAndDestroy( &client );
        CleanupStack::PopAndDestroy( script1 );
        }
    }


void MT_CFScriptEngine::MT_CCFScriptHandler_DeleteScriptByNameL()
    {
    // Load script
    HBufC8* script = LoadScriptFromFile( _L( "test-and.xml" ) );
    EUNIT_ASSERT_DESC( script , "Scripts not loaded" );
    User::LeaveIfNull( script );
    CleanupStack::PushL( script );
    RThread client;
    CleanupClosePushL( client );
    
    // Test data
    const TUid KUidClient1 = {0x12340001};
    const TUid KUidClient2 = {0x12340002};
    _LIT( KClient1Path1, "c:\\private\\10282BC4\\rules\\12340001\\script1.rul" );
    _LIT( KClient1Path2, "c:\\private\\10282BC4\\rules\\12340001\\script2.rul" );
    _LIT( KClient2Path1, "c:\\private\\10282BC4\\rules\\12340002\\script1.rul" );
    _LIT( KClient2Path2, "c:\\private\\10282BC4\\rules\\12340002\\script2.rul" );

    TInt err = KErrNone;
    TInt scriptId = 0;
    TBool exists = EFalse;        
    
    // Define needed contexts for scripts
    static _LIT_SECURITY_POLICY_PASS( alwaysPass );
    err = iCFFakeEnv->DefineContext( _L("Application.State"),
        _L("Foreground.Application"), alwaysPass, alwaysPass );
    EUNIT_ASSERT_DESC( err == KErrNone, "DefineContext failed!" );
    err = iCFFakeEnv->DefineContext( _L("Application.State"),
        _L("Foreground.View"), alwaysPass, alwaysPass );
    EUNIT_ASSERT_DESC( err == KErrNone, "DefineContext failed!" );

    // Add and save scripts
    const TInt KCount = 4;
    for( TInt i = 0; i < KCount; i++ )
        {
        TPtrC fullPath( KNullDesC );
        TUid uid = {0x00000000};
        switch( i )
            {
            case 0:
                {
                fullPath.Set( KClient1Path1 );
                uid = KUidClient1;
                break;
                }
            case 1:
                {
                fullPath.Set( KClient1Path2 );
                uid = KUidClient1;
                break;
                }
            case 2:
                {
                fullPath.Set( KClient2Path1 );
                uid = KUidClient2;
                break;
                }
            case 3:
                {
                fullPath.Set( KClient2Path2 );
                uid = KUidClient2;
                break;
                }
            default:
                {
                break;
                }
            }
        TParsePtrC parse( fullPath );
        scriptId = iCFFakeEnv->AddScript( parse.Name(), *script, uid,
            client );
        EUNIT_ASSERT_DESC( scriptId == i + 1, "Invalid script id" );
        err = iCFFakeEnv->SaveScript( *script, scriptId, uid );
        EUNIT_ASSERT_DESC( err == KErrNone, "SaveScript failed" );
        }
    
    // Delete and check scripts
    for( TInt i = 0; i < KCount; i++ )
        {
        switch( i )
            {
            case 0:
                {
                err = iCFFakeEnv->DeleteScriptByName( KClient1Path1, KUidClient1 );
                EUNIT_ASSERT_DESC( err == KErrNone, "Delete script failed" );
                exists = BaflUtils::FileExists( iFs, KClient1Path1 );
                EUNIT_ASSERT_DESC( !exists, "Script not deleted" );
                
                err = iCFFakeEnv->RemoveScriptByName( KClient1Path1, KUidClient1 );
                EUNIT_ASSERT_DESC( err == KErrNone, "Remove script by name failed" );
                break;
                }
            case 1:
                {
                err = iCFFakeEnv->DeleteScriptByName( KClient1Path2, KUidClient1 );
                EUNIT_ASSERT_DESC( err == KErrNone, "Delete script failed" );
                exists = BaflUtils::FileExists( iFs, KClient1Path2 );
                EUNIT_ASSERT_DESC( !exists, "Script not deleted" );
                
                err = iCFFakeEnv->RemoveScriptByName( KClient1Path2, KUidClient1 );
                EUNIT_ASSERT_DESC( err == KErrNone, "Remove script by name failed" );
                
                // Check that folder is deleted
                TParsePtrC parse( KClient1Path1 );
                exists = BaflUtils::FolderExists( iFs, parse.DriveAndPath() );
                EUNIT_ASSERT_DESC( !exists, "Folder not deleted" );
                break;
                }
            case 2:
                {
                err = iCFFakeEnv->RemoveScriptByName( KClient2Path1, KUidClient2 );
                EUNIT_ASSERT_DESC( err == KErrNone, "Remove script by name failed" );

                err = iCFFakeEnv->DeleteScriptByName( KClient2Path1, KUidClient2 );
                EUNIT_ASSERT_DESC( err == KErrDeregisterNotNeeded, "Delete script failed" );
                exists = BaflUtils::FileExists( iFs, KClient2Path1 );
                EUNIT_ASSERT_DESC( !exists, "Script not deleted" );
                break;
                }
            case 3:
                {
                err = iCFFakeEnv->RemoveScriptByName( KClient2Path2, KUidClient2 );
                EUNIT_ASSERT_DESC( err == KErrNone, "Remove script by name failed" );

                err = iCFFakeEnv->DeleteScriptByName( KClient2Path2, KUidClient2 );
                EUNIT_ASSERT_DESC( err == KErrDeregisterNotNeeded, "Delete script failed" );
                exists = BaflUtils::FileExists( iFs, KClient2Path2 );
                EUNIT_ASSERT_DESC( !exists, "Script not deleted" );
                
                // Check that folder is deleted
                TParsePtrC parse( KClient2Path1 );
                exists = BaflUtils::FolderExists( iFs, parse.DriveAndPath() );
                EUNIT_ASSERT_DESC( !exists, "Folder not deleted" );
                break;
                }
            default:
                {
                break;
                }
            }
        }
    
    // Try to delete a script which does not exist
    err = iCFFakeEnv->DeleteScriptByName( _L( "notfound.rul" ), KUidClient1 );
    EUNIT_ASSERT_DESC( err == KErrNotFound, "Delete script failed" );
    
    // x. Clean up
    // -------------------------------------------------------------------------
    CleanupStack::PopAndDestroy( &client );
    CleanupStack::PopAndDestroy( script );
    }


void MT_CFScriptEngine::MT_CCFScriptHandler_DeleteScriptByName2L()
    {
    // Preconditions
    // -------------------------------------------------------------------------
    HBufC8* script = LoadScriptFromFile( _L("test-countRepeatingWithValuePersistent.xml") );
    EUNIT_ASSERT_DESC( script , "Scripts not loaded" );
    User::LeaveIfNull( script );
    CleanupStack::PushL( script );
    RThread client;
    CleanupClosePushL( client );    
    CCFContextObject* co = CCFContextObject::NewLC();

    const TUid KClientUid = {0x12340001};
    _LIT( KScript,
        "c:\\private\\10282BC4\\rules\\12340001\\test-countRepeatingWithValuePersistent.rul" );
    _LIT( KScriptPresistencyFile,
        "c:\\private\\10282BC4\\rules\\12340001\\test-countRepeatingWithValuePersistent-testCountPersistency.dat" );

    // Define needed contexts
    static _LIT_SECURITY_POLICY_PASS( alwaysPass );
    TInt err = iCFFakeEnv->DefineContext( _L("Test"), _L("Counting"), alwaysPass, alwaysPass );
    EUNIT_ASSERT_DESC( err == KErrNone, "Define context failed" );
    err = iCFFakeEnv->DefineContext( _L("Test"), _L("Control"), alwaysPass, alwaysPass );
    EUNIT_ASSERT_DESC( err == KErrNone, "Define context failed" );

    // Add script and save it
    // -------------------------------------------------------------------------
    TInt scriptId = iCFFakeEnv->AddScript( KScript, *script, KClientUid, client );
    EUNIT_ASSERT_DESC( scriptId > 0, "Add script failed" );
    err = iCFFakeEnv->SaveScript( *script, scriptId, KClientUid );
    EUNIT_ASSERT_DESC( err == KErrNone, "Save script failed" );

    // Publish context so that persistent data file is created
    // -------------------------------------------------------------------------
    co->SetSourceL( _L("Test") );
    co->SetTypeL( _L("Counting") );
    co->SetValueL( _L("A") );
    err = iCFFakeEnv->PublishContext( *co, client );
    EUNIT_ASSERT_DESC( err == KErrNone, "Publish context failed" );
    TBool exists = BaflUtils::FileExists( iFs, KScriptPresistencyFile );
    EUNIT_ASSERT_DESC( exists, "Persistency file not created" );
    
    // Remove script by name and delete script by name
    // -------------------------------------------------------------------------
    err = iCFFakeEnv->RemoveScriptByName( KScript, KClientUid );
    EUNIT_ASSERT_DESC( err == KErrNone, "Remove script by name failed" );
    err = iCFFakeEnv->DeleteScriptByName( KScript, KClientUid );
    EUNIT_ASSERT_DESC( err == KErrDeregisterNotNeeded, "Delete script by name failed" );
    
    // Check that script file and persistency file are removed
    exists = BaflUtils::FileExists( iFs, KScript );
    EUNIT_ASSERT_DESC( !exists, "Script file not deleted" );
    exists = BaflUtils::FileExists( iFs, KScriptPresistencyFile );
    EUNIT_ASSERT_DESC( !exists, "Script persistency file not deleted" );
    exists = BaflUtils::FolderExists( iFs, KScript );
    EUNIT_ASSERT_DESC( !exists, "Client rule folder not removed" );

    // Clean up
    // -------------------------------------------------------------------------
    CleanupStack::PopAndDestroy( co );
    CleanupStack::PopAndDestroy( &client );
    CleanupStack::PopAndDestroy( script );
    }


void MT_CFScriptEngine::MT_CCFScriptHandler_DeleteScriptByUidL()
    {
    // Load script
    HBufC8* script = LoadScriptFromFile( _L( "test-and.xml" ) );
    EUNIT_ASSERT_DESC( script , "Scripts not loaded" );
    if( script )
        {
        CleanupStack::PushL( script );
        RThread client;
        CleanupClosePushL( client );
        
        // Test data
        const TUid KUidClient1 = {0x12340001};
        const TUid KUidClient2 = {0x12340002};
        _LIT( KClient1Path1, "c:\\private\\10282BC4\\rules\\12340001\\script1.rul" );
        _LIT( KClient1Path2, "c:\\private\\10282BC4\\rules\\12340001\\script2.rul" );
        _LIT( KClient2Path1, "c:\\private\\10282BC4\\rules\\12340002\\script1.rul" );
        _LIT( KClient2Path2, "c:\\private\\10282BC4\\rules\\12340002\\script2.rul" );

        TInt err = KErrNone;
        TInt scriptId = 0;
        TBool exists = EFalse;        
        
        // Define needed contexts for scripts
        static _LIT_SECURITY_POLICY_PASS( alwaysPass );
        err = iCFFakeEnv->DefineContext( _L("Application.State"),
            _L("Foreground.Application"), alwaysPass, alwaysPass );
        EUNIT_ASSERT_DESC( err == KErrNone, "DefineContext failed!" );
        err = iCFFakeEnv->DefineContext( _L("Application.State"),
            _L("Foreground.View"), alwaysPass, alwaysPass );
        EUNIT_ASSERT_DESC( err == KErrNone, "DefineContext failed!" );

        // Add and save scripts
        const TInt KCount = 4;
        for( TInt i = 0; i < KCount; i++ )
            {
            TPtrC fullPath( KNullDesC );
            TUid uid = {0x00000000};
            switch( i )
                {
                case 0:
                    {
                    fullPath.Set( KClient1Path1 );
                    uid = KUidClient1;
                    break;
                    }
                case 1:
                    {
                    fullPath.Set( KClient1Path2 );
                    uid = KUidClient1;
                    break;
                    }
                case 2:
                    {
                    fullPath.Set( KClient2Path1 );
                    uid = KUidClient2;
                    break;
                    }
                case 3:
                    {
                    fullPath.Set( KClient2Path2 );
                    uid = KUidClient2;
                    break;
                    }
                default:
                    {
                    break;
                    }
                }
            TParsePtrC parse( fullPath );
            scriptId = iCFFakeEnv->AddScript( parse.Name(), *script, uid,
                client );
            EUNIT_ASSERT_DESC( scriptId == i + 1, "Invalid script id" );
            err = iCFFakeEnv->SaveScript( *script, scriptId, uid );
            EUNIT_ASSERT_DESC( err == KErrNone, "SaveScript failed" );
            }
        
        // Delete and check scripts - test 1
        err = iCFFakeEnv->DeleteScriptByUid( KUidClient1 );
        EUNIT_ASSERT_DESC( err == KErrNone, "Delete script by uid failed" );
        exists = BaflUtils::FileExists( iFs, KClient1Path1 );
        EUNIT_ASSERT_DESC( !exists, "Script not deleted" );
        exists = BaflUtils::FileExists( iFs, KClient1Path2 );
        EUNIT_ASSERT_DESC( !exists, "Script not deleted" );
        TParsePtrC parse1( KClient1Path1 );
        exists = BaflUtils::FolderExists( iFs, parse1.DriveAndPath() );
        EUNIT_ASSERT_DESC( !exists, "Folder not removed" );
        
        // Remove, delete and check scripts - test 2
        err = iCFFakeEnv->RemoveScriptByUid( KUidClient2 );
        EUNIT_ASSERT_DESC( err == KErrNone, "Remove script by uid failed" );
        err = iCFFakeEnv->DeleteScriptByUid( KUidClient2 );
        EUNIT_ASSERT_DESC( err == KErrDeregisterNotNeeded, "Delete script by uid failed" );
        exists = BaflUtils::FileExists( iFs, KClient2Path1 );
        EUNIT_ASSERT_DESC( !exists, "Script not deleted" );
        exists = BaflUtils::FileExists( iFs, KClient2Path2 );
        EUNIT_ASSERT_DESC( !exists, "Script not deleted" );
        TParsePtrC parse2( KClient2Path1 );
        exists = BaflUtils::FolderExists( iFs, parse2.DriveAndPath() );
        EUNIT_ASSERT_DESC( !exists, "Folder not removed" );
        
        // Delete scripts form a client which does not own any scripts
        err = iCFFakeEnv->DeleteScriptByUid( KUidClient2 );
        EUNIT_ASSERT_DESC( err == KErrNotFound || err == KErrPathNotFound,
            "Delete script by uid failed" );
        
        // Clean up
        CleanupStack::PopAndDestroy( &client );
        CleanupStack::PopAndDestroy( script );
        }
    }


void MT_CFScriptEngine::MT_CCFScriptHandler_AlreadyExistsL()
    {
    // Load script
    HBufC8* script = LoadScriptFromFile( _L( "test-and.xml" ) );
    EUNIT_ASSERT_DESC( script , "Scripts not loaded" );
    if( script )
        {
        CleanupStack::PushL( script );
        RThread client;
        CleanupClosePushL( client );

        // Test data
        const TUid KUidClient1 = {0x12340001};
        const TUid KUidClient2 = {0x12340002};
        _LIT( KClient1Path1, "c:\\private\\10282BC4\\rules\\12340001\\script1.rul" );
        _LIT( KClient1Path2, "c:\\private\\10282BC4\\rules\\12340001\\script2.rul" );
        _LIT( KClient2Path1, "c:\\private\\10282BC4\\rules\\12340002\\script1.rul" );
        _LIT( KClient2Path2, "c:\\private\\10282BC4\\rules\\12340002\\script2.rul" );

        TInt err = KErrNone;
        TInt scriptId = 0;
        TBool exists = EFalse;        

        // Define needed contexts for scripts
        static _LIT_SECURITY_POLICY_PASS( alwaysPass );
        err = iCFFakeEnv->DefineContext( _L("Application.State"),
            _L("Foreground.Application"), alwaysPass, alwaysPass );
        EUNIT_ASSERT_DESC( err == KErrNone, "DefineContext failed!" );
        err = iCFFakeEnv->DefineContext( _L("Application.State"),
            _L("Foreground.View"), alwaysPass, alwaysPass );
        EUNIT_ASSERT_DESC( err == KErrNone, "DefineContext failed!" );

        // Add and save scripts
        const TInt KCount = 4;
        for( TInt i = 0; i < KCount; i++ )
            {
            TPtrC fullPath( KNullDesC );
            TUid uid = {0x00000000};
            switch( i )
                {
                case 0:
                    {
                    fullPath.Set( KClient1Path1 );
                    uid = KUidClient1;
                    break;
                    }
                case 1:
                    {
                    fullPath.Set( KClient1Path2 );
                    uid = KUidClient1;
                    break;
                    }
                case 2:
                    {
                    fullPath.Set( KClient2Path1 );
                    uid = KUidClient2;
                    break;
                    }
                case 3:
                    {
                    fullPath.Set( KClient2Path2 );
                    uid = KUidClient2;
                    break;
                    }
                default:
                    {
                    break;
                    }
                }
            TParsePtrC parse( fullPath );
            scriptId = iCFFakeEnv->AddScript( parse.Name(), *script, uid,
                client );
            EUNIT_ASSERT_DESC( scriptId == i + 1, "Invalid script id" );
            err = iCFFakeEnv->SaveScript( *script, scriptId, uid );
            EUNIT_ASSERT_DESC( err == KErrNone, "SaveScript failed" );
            exists = iCFFakeEnv->AlreadyExists( parse.Name(), uid, scriptId );
            EUNIT_ASSERT_DESC( exists, "Already exists failed" );
            }
        
        // Check script which has not been added
        scriptId = KErrNotFound;
        exists = iCFFakeEnv->AlreadyExists( _L( "notadded.rul" ), KUidClient1, scriptId );
        EUNIT_ASSERT_DESC( !exists, "Already exists failed" );
        EUNIT_ASSERT_DESC( scriptId == KErrNotFound, "Invalid script id" );

        // Clean up
        CleanupStack::PopAndDestroy( &client );
        CleanupStack::PopAndDestroy( script );
        }
    }


void MT_CFScriptEngine::MT_CCFScriptHandler_CleanupPersistentDataByNameL()
    {
    // Load script
    HBufC8* script = LoadScriptFromFile( _L("test-countRepeatingWithValuePersistent.xml") );
    EUNIT_ASSERT_DESC( script , "Scripts not loaded" );
    if( script )
        {
        CleanupStack::PushL( script );
        RThread client;
        CleanupClosePushL( client );
        
        // Test data
        const TUid KUidClient1 = {0x12340001};
        const TUid KUidClient2 = {0x12340002};
        _LIT( KClient1Path,
            "c:\\private\\10282BC4\\rules\\12340001\\test-countRepeatingWithValuePersistent.rul" );
        _LIT( KClient2Path,
            "c:\\private\\10282BC4\\rules\\12340002\\test-countRepeatingWithValuePersistent.rul" );
        _LIT( KClient1PresistencyFile,
            "c:\\private\\10282BC4\\rules\\12340001\\test-countRepeatingWithValuePersistent-testCountPersistency.dat" );
        _LIT( KClient2PresistencyFile,
            "c:\\private\\10282BC4\\rules\\12340002\\test-countRepeatingWithValuePersistent-testCountPersistency.dat" );
        
        // Define needed contexts
        static _LIT_SECURITY_POLICY_PASS( alwaysPass );
        TInt err = iCFFakeEnv->DefineContext( _L("Test"), _L("Counting"),
            alwaysPass, alwaysPass );
        EUNIT_ASSERT_DESC( err == KErrNone, "Define context failed" );
        err = iCFFakeEnv->DefineContext( _L("Test"), _L("Control"),
            alwaysPass, alwaysPass );
        EUNIT_ASSERT_DESC( err == KErrNone, "Define context failed" );
        
        // Add scripts
        const TInt KCount = 2;
        for( TInt i = 0; i < KCount; i++ )
            {
            TPtrC fullPath( KNullDesC );
            TUid uid = {0x0};
            switch( i )
                {
                case 0:
                    {
                    fullPath.Set( KClient1Path );
                    uid = KUidClient1;
                    break;
                    }
                case 1:
                    {
                    fullPath.Set( KClient2Path );
                    uid = KUidClient2;
                    break;
                    }
                default:
                    {
                    break;
                    }
                }
            TParsePtrC parse( fullPath );
            TInt scriptId = iCFFakeEnv->AddScript( parse.Name(), *script, uid,
                client );
            EUNIT_ASSERT_DESC( err == KErrNone, "Add script failed" );
            }
        
        // Publish new context for Test, Counting
        CCFContextObject* context = CCFContextObject::NewLC(
            _L("Test"), _L("Counting"), _L("A") );
        err = iCFFakeEnv->PublishContext( *context, client );
        EUNIT_ASSERT_DESC( err == KErrNone, "Publish context failed" );
        
        // check that persistency files exists
        TBool exists = BaflUtils::FileExists( iFs, KClient1PresistencyFile );
        EUNIT_ASSERT_DESC( exists, "Persistency file not created" );
        exists = BaflUtils::FileExists( iFs, KClient2PresistencyFile );
        EUNIT_ASSERT_DESC( exists, "Persistency file not created" );
        
        // Cleanup persistent data
        for( TInt i = 0; i < KCount; i++ )
            {
            TPtrC scriptFilePath( KNullDesC );
            TPtrC presistencyFilePath( KNullDesC );
            TUid uid = {0x0};
            switch( i )
                {
                case 0:
                    {
                    scriptFilePath.Set( KClient1Path );
                    presistencyFilePath.Set( KClient1PresistencyFile );
                    uid = KUidClient1;
                    break;
                    }
                case 1:
                    {
                    scriptFilePath.Set( KClient2Path );
                    presistencyFilePath.Set( KClient2PresistencyFile );
                    uid = KUidClient2;
                    break;
                    }
                default:
                    {
                    break;
                    }
                }
            TParsePtrC parse( scriptFilePath );
            iCFFakeEnv->CleanupPersistentDataByName( parse.Name(), uid );
            exists = BaflUtils::FileExists( iFs, presistencyFilePath );
            EUNIT_ASSERT_DESC( !exists, "Cleanup persistent data by name failed" );
            }

        // Clean up
        CleanupStack::PopAndDestroy( context );
        CleanupStack::PopAndDestroy( &client );
        CleanupStack::PopAndDestroy( script );
        }
    }


void MT_CFScriptEngine::MT_CCFScriptHandler_CleanupPersistentDataByUidL()
    {
    // Load script
    HBufC8* script1 = LoadScriptFromFile( _L("test-countRepeatingWithValuePersistent.xml") );
    EUNIT_ASSERT_DESC( script1 , "Script1 not loaded" );
    CleanupStack::PushL( script1 );
    
    HBufC8* script2 = LoadScriptFromFile( _L("test-countRepeatingWithValuePersistent2.xml") );
    EUNIT_ASSERT_DESC( script2 , "Script2 not loaded" );
    CleanupStack::PushL( script2 );

    RThread client;
    CleanupClosePushL( client );
    
    // Test data
    const TUid KUidClient1 = {0x12340001};
    const TUid KUidClient2 = {0x12340002};
    _LIT( KClient1Path,
        "c:\\private\\10282BC4\\rules\\12340001\\test-countRepeatingWithValuePersistent.rul" );
    _LIT( KClient1Path2,
        "c:\\private\\10282BC4\\rules\\12340001\\test-countRepeatingWithValuePersistent2.rul" );
    _LIT( KClient1PresistencyFile,
        "c:\\private\\10282BC4\\rules\\12340001\\test-countRepeatingWithValuePersistent-testCountPersistency.dat" );
    _LIT( KClient1PresistencyFile2,
        "c:\\private\\10282BC4\\rules\\12340001\\test-countRepeatingWithValuePersistent2-testCountPersistency2.dat" );
    _LIT( KClient2Path,
        "c:\\private\\10282BC4\\rules\\12340002\\test-countRepeatingWithValuePersistent.rul" );
    _LIT( KClient2PresistencyFile,
        "c:\\private\\10282BC4\\rules\\12340002\\test-countRepeatingWithValuePersistent-testCountPersistency.dat" );
    
    // Define needed contexts
    static _LIT_SECURITY_POLICY_PASS( alwaysPass );
    TInt err = iCFFakeEnv->DefineContext( _L("Test"), _L("Counting"),
        alwaysPass, alwaysPass );
    EUNIT_ASSERT_DESC( err == KErrNone, "Define context failed" );
    err = iCFFakeEnv->DefineContext( _L("Test"), _L("Control"),
        alwaysPass, alwaysPass );
    EUNIT_ASSERT_DESC( err == KErrNone, "Define context failed" );
    
    // Add scripts
    const TInt KCount = 3;
    for( TInt i = 0; i < KCount; i++ )
        {
        TPtrC fullPath( KNullDesC );
        TUid uid = {0x0};
        TPtrC8 script( KNullDesC8 );
        switch( i )
            {
            case 0:
                {
                fullPath.Set( KClient1Path );
                uid = KUidClient1;
                script.Set( *script1 );
                break;
                }
            case 1:
                {
                fullPath.Set( KClient1Path2 );
                uid = KUidClient1;
                script.Set( *script2 );
                break;
                }
            case 2:
                {
                fullPath.Set( KClient2Path );
                uid = KUidClient2;
                script.Set( *script1 );
                break;
                }
            default:
                {
                break;
                }
            }
        TParsePtrC parse( fullPath );
        TInt scriptId = iCFFakeEnv->AddScript( parse.Name(), script, uid,
            client );
        EUNIT_ASSERT_DESC( err == KErrNone, "Add script failed" );
        }
    
    // Publish new context for Test, Counting
    CCFContextObject* context = CCFContextObject::NewLC(
        _L("Test"), _L("Counting"), _L("A") );
    err = iCFFakeEnv->PublishContext( *context, client );
    EUNIT_ASSERT_DESC( err == KErrNone, "Publish context failed" );
    
    // check that persistency files exists
    TBool exists = BaflUtils::FileExists( iFs, KClient1PresistencyFile );
    EUNIT_ASSERT_DESC( exists, "Persistency file not created" );
    exists = BaflUtils::FileExists( iFs, KClient1PresistencyFile2 );
    EUNIT_ASSERT_DESC( exists, "Persistency file not created" );
    exists = BaflUtils::FileExists( iFs, KClient2PresistencyFile );
    EUNIT_ASSERT_DESC( exists, "Persistency file not created" );
    
    // Cleanup persistent data
    iCFFakeEnv->CleanupPersistentDataByUid( KUidClient1 );
    exists = BaflUtils::FileExists( iFs, KClient1PresistencyFile );
    EUNIT_ASSERT_DESC( !exists, "Persistency file not created" );
    exists = BaflUtils::FileExists( iFs, KClient1PresistencyFile2 );
    EUNIT_ASSERT_DESC( !exists, "Persistency file not created" );

    iCFFakeEnv->CleanupPersistentDataByUid( KUidClient2 );
    exists = BaflUtils::FileExists( iFs, KClient2PresistencyFile );
    EUNIT_ASSERT_DESC( !exists, "Persistency file not created" );

    // Clean up
    CleanupStack::PopAndDestroy( context );
    CleanupStack::PopAndDestroy( &client );
    CleanupStack::PopAndDestroy( script2 );
    CleanupStack::PopAndDestroy( script1 );
    }


void MT_CFScriptEngine::MT_CCFScriptHandler_RestoreRomScriptL()
    {
    // Load script
    HBufC8* script = LoadScriptFromFile( _L("test-scriptUpgradeRom.xml") );
    EUNIT_ASSERT_DESC( script , "Scripts not loaded" );
    if( script )
        {
        CleanupStack::PushL( script );
        RThread client;
        CleanupClosePushL( client );
        CCFContextObject* context = CCFContextObject::NewLC(
            _L("test"), _L("upgrade"), _L("false") );
        
        // Define needed contexts
        static _LIT_SECURITY_POLICY_PASS( alwaysPass );
        TInt err = iCFFakeEnv->DefineContext( context->Source(), context->Type(),
            alwaysPass, alwaysPass );
        EUNIT_ASSERT_DESC( err == KErrNone, "Define context failed" );

        // Initialize script engine and test that rom script is used
        iCFFakeEnv->InitAllPhasesL();
        context->SetValueL( _L("false") );
        err = iCFFakeEnv->PublishContext( *context, client );
        EUNIT_ASSERT_DESC( err == KErrNone, "Publish context failed" );
        TBool eventFired = iCFFakeEnv->ClientEventFired();
        EUNIT_ASSERT_DESC( eventFired, "Action not fired" );
        TPtrC actionId = iCFFakeEnv->ScriptEventIdentifier();
        EUNIT_ASSERT_DESC( actionId == _L("romUpgrade"), "Invalid action id" );
        const RKeyValueArray& params = iCFFakeEnv->ScriptEventParameters();
        EUNIT_ASSERT_DESC( params[0]->Key() == _L("id"), "Invalid action attribute" );
        EUNIT_ASSERT_DESC( params[0]->Value() == _L("false"), "Invalid action attribute value" );
        
        // Upgrade rom script and test upgrade is used
        TInt scriptId = 0;
        TBool exists = iCFFakeEnv->AlreadyExists( _L("test-scriptUpgradeRom"),
            KCFServerUid, scriptId );
        EUNIT_ASSERT_DESC( exists, "Already exists failed" );
        EUNIT_ASSERT_DESC( scriptId > 0, "Invalid script id" );
        err = iCFFakeEnv->UpdateScript( scriptId, client, *script, NULL );
        EUNIT_ASSERT_DESC( err == KErrNone, "Update script failed" );
        err = iCFFakeEnv->SaveScript( *script, scriptId, KCFServerUid );
        EUNIT_ASSERT_DESC( err == KErrNone, "Save script failed" );
        
        context->SetValueL( _L("true") );
        err = iCFFakeEnv->PublishContext( *context, client );
        EUNIT_ASSERT_DESC( err == KErrNone, "Publish context failed" );
        eventFired = iCFFakeEnv->ClientEventFired();
        EUNIT_ASSERT_DESC( eventFired, "Action not fired" );
        actionId.Set( iCFFakeEnv->ScriptEventIdentifier() );
        EUNIT_ASSERT_DESC( actionId == _L("romUpgrade"), "Invalid action id" );
        const RKeyValueArray& params1 = iCFFakeEnv->ScriptEventParameters();
        EUNIT_ASSERT_DESC( params1[0]->Key() == _L("id"), "Invalid action attribute" );
        EUNIT_ASSERT_DESC( params1[0]->Value() == _L("true"), "Invalid action attribute value" );
        
        // Delete script engine and initialize it again
        // Check that rom upgrade is used instead of original rom script
        delete iCFFakeEnv;
        iCFFakeEnv = NULL;
        iCFFakeEnv = CCFFakeEnv::NewL();
        iCFFakeEnv->InitAllPhasesL();

        err = iCFFakeEnv->DefineContext( context->Source(), context->Type(),
            alwaysPass, alwaysPass );
        EUNIT_ASSERT_DESC( err == KErrNone, "Define context failed" );

        context->SetValueL( _L("true") );
        err = iCFFakeEnv->PublishContext( *context, client );
        EUNIT_ASSERT_DESC( err == KErrNone, "Publish context failed" );
        eventFired = iCFFakeEnv->ClientEventFired();
        EUNIT_ASSERT_DESC( eventFired, "Action not fired" );
        actionId.Set( iCFFakeEnv->ScriptEventIdentifier() );
        EUNIT_ASSERT_DESC( actionId == _L("romUpgrade"), "Invalid action id" );
        const RKeyValueArray& params2 = iCFFakeEnv->ScriptEventParameters();
        EUNIT_ASSERT_DESC( params2[0]->Key() == _L("id"), "Invalid action attribute" );
        EUNIT_ASSERT_DESC( params2[0]->Value() == _L("true"), "Invalid action attribute value" );
        
        // Restore rom script and check that rom script is used again
        exists = iCFFakeEnv->AlreadyExists( _L("test-scriptUpgradeRom"),
            KCFServerUid, scriptId );
        EUNIT_ASSERT_DESC( exists, "Already exists failed" );
        EUNIT_ASSERT_DESC( scriptId > 0, "Invalid script id" );
        err = iCFFakeEnv->RestoreRomScript( scriptId, KCFServerUid, client );
        EUNIT_ASSERT_DESC( err == KErrNone, "Restore script failed" );
        
        context->SetValueL( _L("false") );
        err = iCFFakeEnv->PublishContext( *context, client );
        EUNIT_ASSERT_DESC( err == KErrNone, "Publish context failed" );
        eventFired = iCFFakeEnv->ClientEventFired();
        EUNIT_ASSERT_DESC( eventFired, "Action not fired" );
        actionId.Set( iCFFakeEnv->ScriptEventIdentifier() );
        EUNIT_ASSERT_DESC( actionId == _L("romUpgrade"), "Invalid action id" );
        const RKeyValueArray& params3 = iCFFakeEnv->ScriptEventParameters();
        EUNIT_ASSERT_DESC( params3[0]->Key() == _L("id"), "Invalid action attribute" );
        EUNIT_ASSERT_DESC( params3[0]->Value() == _L("false"), "Invalid action attribute value" );

        // Clean up
        CleanupStack::PopAndDestroy( context );
        CleanupStack::PopAndDestroy( &client );
        CleanupStack::PopAndDestroy( script );
        }
    }


void MT_CFScriptEngine::MT_CCFScriptHandler_InitAllPhasesL()
    {
    // Just check that script loading don't leak memory
    iCFFakeEnv->InitAllPhasesL();
    }


void MT_CFScriptEngine::MT_CCFScriptHandler_IsUpgradeAllowedL()
    {
    HBufC8* script = LoadScriptFromFile(
        _L("test-scriptUpgradeRom.xml") );
    EUNIT_ASSERT_DESC( script, "Failed to load script" );
    CleanupStack::PushL( script );
    
    HBufC8* scriptMissingCaps = LoadScriptFromFile(
        _L("test-scriptUpgradeRomCapabilitiesMissing.xml") );
    EUNIT_ASSERT_DESC( scriptMissingCaps, "Failed to load script" );
    CleanupStack::PushL( scriptMissingCaps );
    
    HBufC8* scriptNoCapsSet = LoadScriptFromFile(
        _L("test-scriptUpgradeRomNoUpgradeSecurity.xml") );
    EUNIT_ASSERT_DESC( scriptNoCapsSet, "Failed to load script" );
    CleanupStack::PushL( scriptNoCapsSet );
    
    HBufC8* scriptClientMissingCaps = LoadScriptFromFile(
        _L("test-scriptUpgradeRomClientMissingCapability.xml") );
    EUNIT_ASSERT_DESC( scriptClientMissingCaps, "Failed to load script" );
    CleanupStack::PushL( scriptClientMissingCaps );
    
    RThread client;
    CleanupClosePushL( client );
    
    // Preconditions
    // -------------------------------------------------------------------------
    static _LIT_SECURITY_POLICY_PASS( alwaysPass );
    CCFContextObject* co = CCFContextObject::NewLC();
    co->SetSourceL( _L("test") );
    co->SetTypeL( _L("upgrade") );
    TInt err = iCFFakeEnv->DefineContext( co->Source(), co->Type(),
        alwaysPass, alwaysPass );
    EUNIT_ASSERT_DESC( err == KErrNone, "Define context failed" );
    
    // Load all scripts found
    iCFFakeEnv->InitAllPhasesL();
    
    // x. Check that upgrade is allowed with correct script
    // -------------------------------------------------------------------------
    err = iCFFakeEnv->IsUpgradeAllowed( _L("test-scriptUpgradeRom"),
        *script, KCFServerUid, client, *iCFFakeEnv );
    EUNIT_ASSERT_DESC( err == KErrNone, "Is upgrade allowed failed" );
    
    // x. Check that upgrade is not allowed -
    //    Cabability missing from upgrade script
    // -------------------------------------------------------------------------
    err = iCFFakeEnv->IsUpgradeAllowed( _L("test-scriptUpgradeRom"),
        *scriptMissingCaps, KCFServerUid, client, *iCFFakeEnv );
    EUNIT_ASSERT_DESC( err == KErrAccessDenied, "Is upgrade allowed failed" );

    // x. Check that upgrade is not allowed -
    //    Capabilities not set in the script (using default AllFiles)
    // -------------------------------------------------------------------------
    err = iCFFakeEnv->IsUpgradeAllowed( _L("test-scriptUpgradeRom"),
        *scriptNoCapsSet, KCFServerUid, client, *iCFFakeEnv );
    EUNIT_ASSERT_DESC( err == KErrAccessDenied, "Is upgrade allowed failed" );

    // x. Check that upgrade is not allowed -
    //    Client does not have the required capability set
    // -------------------------------------------------------------------------
    err = iCFFakeEnv->IsUpgradeAllowed( _L("test-scriptUpgradeRomClientMissingCapability"),
        *scriptNoCapsSet, KCFServerUid, client, *iCFFakeEnv );
    EUNIT_ASSERT_DESC( err == KErrAccessDenied, "Is upgrade allowed failed" );

    // Cleanup
    CleanupStack::PopAndDestroy( co );
    CleanupStack::PopAndDestroy( &client );
    CleanupStack::PopAndDestroy( scriptClientMissingCaps );
    CleanupStack::PopAndDestroy( scriptNoCapsSet );
    CleanupStack::PopAndDestroy( scriptMissingCaps );
    CleanupStack::PopAndDestroy( script );
    }


void MT_CFScriptEngine::MT_CCFScriptHandler_RemoveScriptByProviderUidL()
    {
    // x. Remove all scripts which have dependency to basic operations plugin
    //--------------------------------------------------------------------------
    iCFFakeEnv->InitAllPhasesL();
    
    // Get script count
    TInt count = iCFFakeEnv->NumberOfScripts();
    EUNIT_PRINT( _L("NumberOfScripts: count = %d"), count );
    
    // Get script ids
    const RArray<TInt>& tempScriptIds = iCFFakeEnv->GetEveryScriptId();
    RArray<TInt> scriptIds;
    CleanupClosePushL( scriptIds );
    for( TInt i = 0; i < tempScriptIds.Count(); i++ )
        {
        scriptIds.AppendL( tempScriptIds[i] );
        }
    
    // Remove scripts byt provider uid KBasicOperationsPluginImplementationUid
    TInt err = iCFFakeEnv->RemoveScriptByProviderUid(
        TUid::Uid( KBasicOperationsPluginImplementationUid ), ETrue );
    EUNIT_PRINT( _L("RemoveScriptByProviderUid: err = %d"), err );
    EUNIT_ASSERT_DESC( err == KErrNone, "Remove script by provider uid failed" );

    // x. Rollback all scripts and check
    //--------------------------------------------------------------------------
    iCFFakeEnv->RollbackScripts();
    TInt newCount = iCFFakeEnv->NumberOfScripts();
    EUNIT_PRINT( _L("NumberOfScripts: newCount = %d"), newCount );
    EUNIT_ASSERT_DESC( count == newCount, "Rollback scripts failed" );
    
    // Get script ids again, should match
    const RArray<TInt>& tempNewScriptIds = iCFFakeEnv->GetEveryScriptId();
    RArray<TInt> newScriptIds;
    CleanupClosePushL( newScriptIds );
    for( TInt i = 0; i < tempNewScriptIds.Count(); i++ )
        {
        newScriptIds.AppendL( tempScriptIds[i] );
        }
    EUNIT_ASSERT_DESC( scriptIds.Count() == newScriptIds.Count(), "Incorrect script ids count" );
    for( TInt i = 0; i < scriptIds.Count(); i++ )
        {
        TInt id = KErrNotFound;
        for( TInt y = 0; y < newScriptIds.Count(); y++ )
            {
            id = newScriptIds[y];
            if( id == scriptIds[i] )
                {
                id = KErrNone;
                break;
                }
            }
        EUNIT_ASSERT_DESC( id == KErrNone, "Script ids does not match" );
        }

    // x. Check from a rom script test-rollbackRom.xml that actions are fired
    //--------------------------------------------------------------------------
    static _LIT_SECURITY_POLICY_PASS( alwaysPass );
    CCFContextObject* co = CCFContextObject::NewLC();
    RThread client;
    CleanupClosePushL( client );
    
    // Define context
    co->SetSourceL( _L("rollback") );
    co->SetTypeL( _L("rollback.status") );
    err = iCFFakeEnv->DefineContext( co->Source(), co->Type(), alwaysPass, alwaysPass );
    EUNIT_ASSERT_DESC( err == KErrNone, "Define context failed" );
    
    // Publish context
    iCFFakeEnv->ClearFireClientEvent();
    iCFFakeEnv->ClearFireClientEventCount();
    co->SetValueL( _L("ok") );
    err = iCFFakeEnv->PublishContext( *co, client );
    
    // Clean up
    CleanupStack::PopAndDestroy( &client );
    CleanupStack::PopAndDestroy( co );
    EUNIT_ASSERT_DESC( err == KErrNone, "Publish context failed" );
    
    // Check that action is fired
    TBool eventFired = iCFFakeEnv->ClientEventFired();
    EUNIT_ASSERT_DESC( eventFired, "Action not fired" );
    
    TPtrC actionId( KNullDesC );
    actionId.Set( iCFFakeEnv->ScriptEventIdentifier() );
    EUNIT_ASSERT_DESC( actionId == _L("rollback"), "Invalid action id" );

    const RKeyValueArray& params = iCFFakeEnv->ScriptEventParameters();
    EUNIT_ASSERT_DESC( params[0]->Key() == _L("status"), "Invalid action attribute" );
    EUNIT_ASSERT_DESC( params[0]->Value() == _L("ok"), "Invalid action attribute value" );

    // x. clean up
    //--------------------------------------------------------------------------
    CleanupStack::PopAndDestroy( &newScriptIds );
    CleanupStack::PopAndDestroy( &scriptIds );
    }


//  TEST TABLE

EUNIT_BEGIN_TEST_TABLE(
    MT_CFScriptEngine,
    "CFScriptEngine module test.",
    "MODULE" )

EUNIT_TEST(
    "NewL - test ",
    "CCFScriptHandler",
    "NewL",
    "FUNCTIONALITY",
    SetupEmptyL, MT_CCFScriptHandler_NewLL, TeardownNoFinalClose)

EUNIT_TEST(
    "NewLC - test ",
    "CCFScriptHandler",
    "NewLC",
    "FUNCTIONALITY",
    SetupEmptyL, MT_CCFScriptHandler_NewLCL, Teardown)

EUNIT_TEST(
    "RemoveScriptById - test ",
    "MCFScriptEngineInterface",
    "Parameters",
    "FUNCTIONALITY",
    SetupL, MT_CCFScriptHandler_RemoveScriptByIdL, Teardown)

EUNIT_TEST(
    "RemoveScriptsBySession - test ",
    "MCFScriptEngineInterface",
    "Parameters",
    "FUNCTIONALITY",
    SetupL, MT_CCFScriptHandler_RemoveScriptsBySessionL, Teardown)

EUNIT_TEST(
    "RemoveScriptByName - test ",
    "MCFScriptEngineInterface",
    "Parameters",
    "FUNCTIONALITY",
    SetupL, MT_CCFScriptHandler_RemoveScriptByNameL, Teardown)

EUNIT_TEST(
    "RemoveScriptByUid - test ",
    "MCFScriptEngineInterface",
    "Parameters",
    "FUNCTIONALITY",
    SetupL, MT_CCFScriptHandler_RemoveScriptByUidL, Teardown)

EUNIT_TEST(
    "NumberOfScripts - test ",
    "MCFScriptEngineInterface",
    "Parameters",
    "FUNCTIONALITY",
    SetupL, MT_CCFScriptHandler_NumberOfScriptsL, Teardown)

EUNIT_TEST(
    "ScriptLength - test ",
    "MCFScriptEngineInterface",
    "Parameters",
    "FUNCTIONALITY",
    SetupL, MT_CCFScriptHandler_ScriptLengthL, Teardown)

EUNIT_TEST(
    "NumberOfScriptsByOwner - test ",
    "MCFScriptEngineInterface",
    "Parameters",
    "FUNCTIONALITY",
    SetupL, MT_CCFScriptHandler_NumberOfScriptsByOwnerL, Teardown)

EUNIT_TEST(
    "GetEveryScriptIdL - test ",
    "MCFScriptEngineInterface",
    "Parameters",
    "FUNCTIONALITY",
    SetupL, MT_CCFScriptHandler_GetEveryScriptIdL, Teardown)

EUNIT_TEST(
    "GetEveryScriptIdByOwnerL - test ",
    "MCFScriptEngineInterface",
    "Parameters",
    "FUNCTIONALITY",
    SetupL, MT_CCFScriptHandler_GetEveryScriptIdByOwnerL, Teardown)

EUNIT_TEST(
    "UpdateScriptL - test ",
    "MCFScriptEngineInterface",
    "Parameters",
    "FUNCTIONALITY",
    SetupL, MT_CCFScriptHandler_UpdateScriptL, Teardown)

EUNIT_TEST(
    "SaveScript - test ",
    "MCFScriptEngineInterface",
    "Parameters",
    "FUNCTIONALITY",
    SetupL, MT_CCFScriptHandler_SaveScriptL, Teardown)

EUNIT_TEST(
    "DeleteScriptByName - test ",
    "MCFScriptEngineInterface",
    "Parameters",
    "FUNCTIONALITY",
    SetupL, MT_CCFScriptHandler_DeleteScriptByNameL, Teardown)

EUNIT_TEST(
    "DeleteScriptByName 2 - test",
    "MCFScriptEngineInterface",
    "Parameters",
    "FUNCTIONALITY",
    SetupL, MT_CCFScriptHandler_DeleteScriptByName2L, Teardown)

EUNIT_TEST(
    "DeleteScriptByUid - test ",
    "MCFScriptEngineInterface",
    "Parameters",
    "FUNCTIONALITY",
    SetupL, MT_CCFScriptHandler_DeleteScriptByUidL, Teardown)

EUNIT_TEST(
    "AlreadyExists - test ",
    "MCFScriptEngineInterface",
    "Parameters",
    "FUNCTIONALITY",
    SetupL, MT_CCFScriptHandler_AlreadyExistsL, Teardown)

EUNIT_TEST(
    "CleanupPersistentDataByName - test ",
    "MCFScriptEngineInterface",
    "Parameters",
    "FUNCTIONALITY",
    SetupL, MT_CCFScriptHandler_CleanupPersistentDataByNameL, Teardown)

EUNIT_TEST(
    "CleanupPersistentDataByUid - test ",
    "MCFScriptEngineInterface",
    "Parameters",
    "FUNCTIONALITY",
    SetupL, MT_CCFScriptHandler_CleanupPersistentDataByUidL, Teardown)

EUNIT_TEST(
    "RestoreRomScript - test ",
    "MCFScriptEngineInterface",
    "Parameters",
    "FUNCTIONALITY",
    SetupL, MT_CCFScriptHandler_RestoreRomScriptL, Teardown)

EUNIT_TEST(
    "Init all phases - test ",
    "MCFScriptEngineInterface",
    "Parameters",
    "FUNCTIONALITY",
    SetupL, MT_CCFScriptHandler_InitAllPhasesL, Teardown)

EUNIT_TEST(
    "IsUpgradeAllowed - test ",
    "MCFScriptEngineInterface",
    "IsUpgradeAllowed",
    "FUNCTIONALITY",
    SetupL, MT_CCFScriptHandler_IsUpgradeAllowedL, Teardown)

EUNIT_TEST(
    "RemoveScriptByProviderUid - test ",
    "MCFScriptEngineInterface",
    "RemoveScriptByProviderUid",
    "FUNCTIONALITY",
    SetupL, MT_CCFScriptHandler_RemoveScriptByProviderUidL, Teardown)

EUNIT_END_TEST_TABLE

//  END OF FILE

/*
* Copyright (c) 2007 Nokia Corporation and/or its subsidiary(-ies). 
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


#ifndef __MT_CFSCRIPTENGINE_H__
#define __MT_CFSCRIPTENGINE_H__

//  EXTERNAL INCLUDES
#include <CEUnitTestSuiteClass.h>

#include <f32file.h>
#include <CFKeyValuePair.h>

//  INTERNAL INCLUDES

//  FORWARD DECLARATIONS
class CCFTestDelay;
class CCFFakeEnv;
class CScriptEventNotifierSession;
class CFileMan;

#include <e32def.h>
#ifndef NONSHARABLE_CLASS
#define NONSHARABLE_CLASS(x) class x
#endif

//  CLASS DEFINITION
/**
 *
 * EUnitWizard generated test class.
 *
 */
NONSHARABLE_CLASS( MT_CFScriptEngine ) : public CEUnitTestSuiteClass
    {
public: // Constructors and destructors

    /**
     * Two phase construction
     */
    static MT_CFScriptEngine* NewL();
    static MT_CFScriptEngine* NewLC();

    /**
     * Destructor
     */
    ~MT_CFScriptEngine();

private: // Constructors and destructors

    MT_CFScriptEngine();
    void ConstructL();

private: // New methods

    void SetupEmptyL();
    void SetupL();

    void Teardown();
    void TeardownNoFinalClose();

    HBufC8* LoadScriptFromFile( const TDesC& aFilePath );

    TInt FindParameter( const RKeyValueArray& aArray,
        const TDesC& aKey, const TDesC& aValue );

    TInt FindIndex( const RArray<TInt>& aArray, TInt aIndex );

    void CleanupSystemDrive();

    void MT_CCFScriptEvent_NewLL();
    void MT_CCFScriptEvent_ScriptIdL();
    void MT_CCFScriptEvent_IdentifierL();
    void MT_CCFScriptEvent_ParametersL();

    void MT_CCFScriptHandler_NewLL();
    void MT_CCFScriptHandler_NewLCL();
    void MT_CCFScriptHandler_RemoveScriptByIdL();
    void MT_CCFScriptHandler_RemoveScriptsBySessionL();
    void MT_CCFScriptHandler_RemoveScriptByNameL();
    void MT_CCFScriptHandler_RemoveScriptByUidL();
    void MT_CCFScriptHandler_NumberOfScriptsL();
    void MT_CCFScriptHandler_ScriptLengthL();
    void MT_CCFScriptHandler_NumberOfScriptsByOwnerL();
    void MT_CCFScriptHandler_GetEveryScriptIdL();
    void MT_CCFScriptHandler_GetEveryScriptIdByOwnerL();
    void MT_CCFScriptHandler_UpdateScriptL();
    void MT_CCFScriptHandler_SaveScriptL();
    void MT_CCFScriptHandler_DeleteScriptByNameL();
    void MT_CCFScriptHandler_DeleteScriptByName2L();
    void MT_CCFScriptHandler_DeleteScriptByUidL();
    void MT_CCFScriptHandler_AlreadyExistsL();
    void MT_CCFScriptHandler_CleanupPersistentDataByNameL();
    void MT_CCFScriptHandler_CleanupPersistentDataByUidL();
    void MT_CCFScriptHandler_RestoreRomScriptL();
    void MT_CCFScriptHandler_InitAllPhasesL();
    void MT_CCFScriptHandler_IsUpgradeAllowedL();
    void MT_CCFScriptHandler_RemoveScriptByProviderUidL();

private: // Data

    EUNIT_DECLARE_TEST_TABLE;

    RFs iFs;
    CCFFakeEnv* iCFFakeEnv;
    CCFTestDelay* iTestDelay;
    CScriptEventNotifierSession* iDummyScriptEngineSession;
    CFileMan* iFileMan;
    };

#endif      //  __MT_CFSCRIPTENGINE_H__

// End of file

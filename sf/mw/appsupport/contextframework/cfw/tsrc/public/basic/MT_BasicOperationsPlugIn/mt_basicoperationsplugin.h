/*
* Copyright (c) 2002 Nokia Corporation and/or its subsidiary(-ies). 
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


#ifndef __MT_BASICOPERATIONSPLUGIN_H__
#define __MT_BASICOPERATIONSPLUGIN_H__

//  EXTERNAL INCLUDES
#include <CEUnitTestSuiteClass.h>
#include <EUnitDecorators.h>
#include <f32file.h>
#include <CFKeyValuePair.h>

//  INTERNAL INCLUDES


//  FORWARD DECLARATIONS
class CCFTestDelay;
class CCFFakeEnv;
class CScriptEventNotifierSession;
class CCFContextObject;


//  CLASS DEFINITION
/**
 * Auto-generated test suite
 *
 */
NONSHARABLE_CLASS( mt_basicoperationsplugin )
	: public CEUnitTestSuiteClass
    {
    public:     // Constructors and destructors

        /**
         * Two phase construction
         */
        static mt_basicoperationsplugin* NewL();
        static mt_basicoperationsplugin* NewLC();
        /**
         * Destructor
         */
        ~mt_basicoperationsplugin();

    private:    // Constructors and destructors

        mt_basicoperationsplugin();
        void ConstructL();

    private:    // New methods
	
	    void SetupEmptyL();
	    void SetupL();

        void SetupMidtestL( CCFContextObject& aContext1,
                CCFContextObject& aContext2,
                const TDesC& aScriptFile );
        void TeardownMidtest();

	    void Teardown();
	
	    HBufC8* LoadScriptFromFile( const TDesC& aFilePath );
        void DeletePersistencyFile( const TDesC& aScriptName,
                const TDesC& aPersistencyFilename );


	    TInt FindParameter( const RKeyValueArray& aArray,
	            const TDesC& aKey, const TDesC& aValue );

	    TInt FindIndex( const RArray<TInt>& aArray, TInt aIndex );


         void MT_ScriptTest_equals_L();
         void MT_ScriptTest_notEqual_L();
         void MT_ScriptTest_less_L();
         void MT_ScriptTest_greater_L();
         void MT_ScriptTest_lessOrEqual_L();
         void MT_ScriptTest_greaterOrEqual_L();
         void MT_ScriptTest_contains_L();
         void MT_ScriptTest_inRange_L();
         void MT_ScriptTest_contextChanged_L();
         void MT_ScriptTest_contextUpdated_L();
         void MT_ScriptTest_and_L();
         void MT_ScriptTest_and_contextChanged_L();
         void MT_ScriptTest_and_contextUpdated_L();
         void MT_ScriptTest_or_L();
         void MT_ScriptTest_not_L();
         void MT_ScriptTest_publishContext_L();
         void MT_ScriptTest_publishContext2_L();
         void MT_ScriptTest_publishContext3_L();
         void MT_ScriptTest_elseif_L();
         void MT_ScriptTest_else_L();
         void MT_ScriptTest_delay_L();
         void MT_ScriptTest_delay_contextRef_L();
         void MT_ScriptTest_countRepeating_withoutValue_L();
         void MT_ScriptTest_countRepeating_withValue_L();
         void MT_ScriptTest_countRepeating_triggerWithoutValue_L();
         void MT_ScriptTest_countOneShot_withoutValue_L();
         void MT_ScriptTest_countOneShot_withValue_L();
         void MT_ScriptTest_countOneShot_remainWithValue_duration_L();
         void MT_ScriptTest_countOneShot_changeWithValue_duration_L();
         void MT_ScriptTest_countOneShot_triggerWithValue_duration_L();
         void MT_ScriptTest_countOneShot_remainWithValue_persistentDuration_L();
         void MT_ScriptTest_countOneShot_changeWithValue_persistentDuration_L();
         void MT_ScriptTest_countOneShot_triggerWithValue_persistentDuration_L();
         void MT_ScriptTest_countOneShot_remainWithValue_persistentDuration2_L();
         void MT_ScriptTest_countOneShot_remainWithValue_persistentDuration3_L();
         void MT_ScriptTest_countOneShot_remainWithValue_persistentDuration4_L();
         void MT_ScriptTest_countOneShot_changeWithValue_persistentDuration2_L();
         void MT_ScriptTest_countOneShot_triggerWithValue_persistentDuration2_L();
         void MT_ScriptTest_countRepeating_withValue_persistent_L();
         void MT_ScriptTest_countRepeating_withValue_persistent2_L();
         void MT_ScriptTest_countRepeating_triggerWithoutValue_persistent_L();

         void MT_ScriptTest_SubscriptionSecurity_L();
         void MT_ScriptTest_ActionSecurity_L();
         void MT_ScriptTest_OperationParsingL();


    private:    // Data
		
        EUNIT_DECLARE_TEST_TABLE; 
        
        
        RFs iFs;
        CCFFakeEnv* iCFFakeEnv;

        CCFTestDelay* iTestDelay;

        CScriptEventNotifierSession* iDummyScriptEngineSession;


    };

#endif      //  __MT_BASICOPERATIONSPLUGIN_H__

// End of file

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





// INCLUDE FILES
#include <e32hal.h>	//User::HAL
#include <e32property.h> //RProperty
#include "t_oomharness.h"
#include "CDummyApplicationHandle.h"
#include "../../t_oomdummyapp/inc/t_oomdummyappmsgs.h"
#include "t_oomdummyplugin_properties.h"
#include <hal.h>

// EXTERNAL DATA STRUCTURES
// None

// EXTERNAL FUNCTION PROTOTYPES  
// None

// CONSTANTS
// None

// MACROS
// None

// LOCAL CONSTANTS AND MACROS

const TInt KOomJustUnderLowMemoryThreshold = 4000;
const TInt KOomSignificantlyUnderLowMemoryThreshold = 2024;
const TInt KOomBetweenLowAndGoodThresholds = 4300;
const TInt KOomJustAboveGoodMemoryThreshold = 5100;

const TInt KOomTestFirstIdlePeriod = 40000000; // 40 seconds


// MODULE DATA STRUCTURES
// None

// LOCAL FUNCTION PROTOTYPES
// None

// FORWARD DECLARATIONS
// None

// ==================== LOCAL FUNCTIONS =======================================

/*
-------------------------------------------------------------------------------

    DESCRIPTION

    This module contains the implementation of COomTestHarness class 
    member functions that does the actual tests.

-------------------------------------------------------------------------------
*/

// ============================ MEMBER FUNCTIONS ===============================

/*
-------------------------------------------------------------------------------

    Class: COomTestHarness

    Method: Case

    Description: Returns a test case by number.

    This function contains an array of all available test cases 
    i.e pair of case name and test function. If case specified by parameter
    aCaseNumber is found from array, then that item is returned.

    The reason for this rather complicated function is to specify all the
    test cases only in one place. It is not necessary to understand how
    function pointers to class member functions works when adding new test
    cases. See function body for instructions how to add new test case.
    
    Parameters:    const TInt aCaseNumber :in:      Test case number

    Return Values: const TCaseInfo Struct containing case name & function

    Errors/Exceptions: None

    Status: Proposal

-------------------------------------------------------------------------------
*/ 
const TCaseInfo COomTestHarness::Case ( 
    const TInt aCaseNumber ) const 
     {

    /*
    * To add new test cases, implement new test case function and add new 
    * line to KCases array specify the name of the case and the function 
    * doing the test case
    * In practice, do following
    *
    * 1) Make copy of existing test case function and change its name
    *    and functionality. Note that the function must be added to 
    *    OOMHard.cpp file and to OOMHard.h 
    *    header file.
    *
    * 2) Add entry to following KCases array either by using:
    *
    * 2.1: FUNCENTRY or ENTRY macro
    * ENTRY macro takes two parameters: test case name and test case 
    * function name.
    *
    * FUNCENTRY macro takes only test case function name as a parameter and
    * uses that as a test case name and test case function name.
    *
    * Or
    *
    * 2.2: OOM_FUNCENTRY or OOM_ENTRY macro. Note that these macros are used
    * only with OOM (Out-Of-Memory) testing!
    *
    * OOM_ENTRY macro takes five parameters: test case name, test case 
    * function name, TBool which specifies is method supposed to be run using
    * OOM conditions, TInt value for first heap memory allocation failure and 
    * TInt value for last heap memory allocation failure.
    * 
    * OOM_FUNCENTRY macro takes test case function name as a parameter and uses
    * that as a test case name, TBool which specifies is method supposed to be
    * run using OOM conditions, TInt value for first heap memory allocation 
    * failure and TInt value for last heap memory allocation failure. 
    */ 

    static TCaseInfoInternal const KCases[] =
        {
        // To add new test cases, add new items to this array
        
        // NOTE: When compiled to GCCE, there must be Classname::
        // declaration in front of the method name, e.g. 
        // COomTestHarness::PrintTest. Otherwise the compiler
        // gives errors.
                        		
        ENTRY( "App Close Test 1", COomTestHarness::AppCloseTest1L ),
        ENTRY( "App Close Idle Time", COomTestHarness::AppCloseIdleTimeTest1L ),
        ENTRY( "App Close Equal Priorities", COomTestHarness::AppCloseEqualPrioritiesL ),        
        ENTRY( "Plugin Test 1", COomTestHarness::PluginTest1L ),
        ENTRY( "Plugin Test 2", COomTestHarness::PluginTest2L ),
        ENTRY( "App Plugin Test 1", COomTestHarness::AppPluginTest1L ),
        ENTRY( "App Plugin Idle Time Test 1", COomTestHarness::AppPluginIdleTimeTest1L ),
        ENTRY( "Optional Allocation Test 1", COomTestHarness::OptionalAllocationTest1L ),
        ENTRY( "Optional Allocation Test 2", COomTestHarness::OptionalAllocationTest2L ),
        ENTRY( "Optional Allocation Test 3", COomTestHarness::OptionalAllocationTest3L ),
        ENTRY( "Plugin V2 Test 1", COomTestHarness::PluginV2Test1L ),
        ENTRY( "Force Priority Check Test 1", COomTestHarness::ForcePriorityCheck1L ),
		ENTRY( "Plugin Sync Mode Test 1", COomTestHarness::PluginSyncModeTest1L ),
		ENTRY( "Never Close Test 1", COomTestHarness::NeverCloseTest1L ),
		ENTRY( "Optional Allocation Async Test 1", COomTestHarness::OptionalAllocationAsyncTest1L ),
		ENTRY( "Optional Allocation Async Test 2", COomTestHarness::OptionalAllocationAsyncTest2L ),
		ENTRY( "Optional Allocation Async Test 3", COomTestHarness::OptionalAllocationAsyncTest3L ),
#ifdef CLIENT_REQUEST_QUEUE
		ENTRY( "App Close Two Client Sessions", COomTestHarness::AppCloseTwoSessionsL ),
#endif		
        ENTRY( "Call If Target App Not Running Test 1", COomTestHarness::CallIfTargetAppNotRunningTest1L ),		
#ifdef __WINS__
        ENTRY( "App Close Specific Threshold Test 1", COomTestHarness::AppCloseSpecificThresholdTest1L ),     
#endif
        ENTRY( "Plugin Test Insufficient Memory Freed", COomTestHarness::PluginTestInsufficientMemoryFreedL ),
        ENTRY( "Plugin Test Insufficient Memory Freed 2", COomTestHarness::PluginTestInsufficientMemoryFreed2L ),
        };

    // Verify that case number is valid
    if( (TUint) aCaseNumber >= sizeof( KCases ) / 
                               sizeof( TCaseInfoInternal ) )
        {

        // Invalid case, construct empty object
        TCaseInfo null( (const TText*) L"" );
        null.iMethod = NULL;
        null.iIsOOMTest = EFalse;
        null.iFirstMemoryAllocation = 0;
        null.iLastMemoryAllocation = 0;
        return null;

        } 

    // Construct TCaseInfo object and return it
    TCaseInfo tmp ( KCases[ aCaseNumber ].iCaseName );
    tmp.iMethod = KCases[ aCaseNumber ].iMethod;
    tmp.iIsOOMTest = KCases[ aCaseNumber ].iIsOOMTest;
    tmp.iFirstMemoryAllocation = KCases[ aCaseNumber ].iFirstMemoryAllocation;
    tmp.iLastMemoryAllocation = KCases[ aCaseNumber ].iLastMemoryAllocation;
    return tmp;

    }

TInt COomTestHarness::AppCloseTest1L(TTestResult& aResult)
	{
	ResetL();
	
	// The lowest priority app should be in the foregound
	// ... with the next lowest behind it
	// ... followed by the highest priority application at the back
	StartApplicationL(KOomTestAppUid, 5 * 1024); // P9 app
	StartApplicationL(KOomTestApp2Uid, 5 * 1024); // P8 app, configure it to eat 5MB
	StartApplicationL(KOomTestApp3Uid, 0); // P7 app
	
	BringAppToForeground(KOomTestApp3Uid);
	
	//start watchers
	TInt memTestState = ETestInit;
	TInt err = KErrNone;
	
	StartMemoryMonitorStatusWatcher(memTestState);

    // Go significantly under the low memory threshold
    EatMemoryL(KOomSignificantlyUnderLowMemoryThreshold);

    //start timer, start scheduler & stop watchers when done
    StartTimerAndRunWatcher(memTestState);
    
    if (err == KErrNone && memTestState == ETestAbove)
        {
        err = AppIsRunning(KOomTestApp2Uid, EFalse);
        
        if (err != KErrNone)
            {
            _LIT( KResult ,"P8 App KOomTestApp2Uid incorrectly running");
            aResult.iResultDes.Copy( KResult );
            aResult.iResult = KErrGeneral;
           // Case was executed but failed
            }
        if (err == KErrNone)
            {	
            err = AppIsRunning(KOomTestApp3Uid, ETrue);	
            if (err != KErrNone)
                {
                // The P7 app should still be running because it was in the foreground
                _LIT( KResult ,"(P7 App KOomTestApp3Uid not running");
                aResult.iResultDes.Copy( KResult );
                aResult.iResult = KErrGeneral;
                // Case was executed but failed
                }
            }
        if (err == KErrNone)
            {	
            err = AppIsRunning(KOomTestAppUid, ETrue);	
            if (err != KErrNone)
                {
                // The P9 app should still be running because the P8 application freed enough memory
                _LIT( KResult ,"P9 App KOomTestAppUid not running");
                aResult.iResultDes.Copy( KResult );
                aResult.iResult = KErrGeneral;
                // Case was executed but failed
                }
            }
        if (err == KErrNone)
            {
            _LIT( KDescription , "Test case passed");
            aResult.SetResult( KErrNone, KDescription );
            }
        }
    else
        {
        _LIT( KResult ,"Watchers failed to start or Application Timeout or Memory Still Below Treshhold ");
        aResult.iResultDes.Copy( KResult );
        aResult.iResult = KErrGeneral;
        // case was not executed or case was executed but never finished or failed 
        }
	ResetL();    
    return KErrNone;	
	}

// Tests the idle time rule mechanism for app closure
// Start three applications
// The lowest priority app should be in the foregound
// ... with the next lowest behind it
// ... followed by the highest priority application at the back
// After an idle period the highest priority app is configured to become the lowest priority
// The middle and highest application should be configured to eat 5MB of memory
// A low memory event is triggered and the middle application only should be closed
// Note that this test should be the same as AppCloseTest1L, with the exception of the idle period which causes different apps to be closed
TInt COomTestHarness::AppCloseIdleTimeTest1L(TTestResult& aResult)
	{
	ResetL();
	
	// The lowest priority app should be in the foregound
	// ... with the next lowest behind it
	// ... followed by the highest priority application at the back
	StartApplicationL(KOomTestAppUid, 5 * 1024); // P9 app (which becomes a P2 app after the idle time)
	StartApplicationL(KOomTestApp2Uid, 5 * 1024); // P8 app, configure it to eat 5MB
	StartApplicationL(KOomTestApp3Uid, 0); // P7 app
	
	BringAppToForeground(KOomTestApp3Uid);
	
	// Wait for the first set of idle time rules to apply
	User::After(KOomTestFirstIdlePeriod);
	
	//start watchers
	TInt memTestState = ETestInit;
    TInt err = KErrNone;
    StartMemoryMonitorStatusWatcher(memTestState);
    
    // Go significantly under the low memory threshold
    EatMemoryL(KOomSignificantlyUnderLowMemoryThreshold);

    //start timer, start scheduler & stop watchers when done
    StartTimerAndRunWatcher(memTestState);        

    if (err == KErrNone && memTestState == ETestAbove)
        {
	    err = AppIsRunning(KOomTestAppUid, EFalse);		

	    if (err != KErrNone)
	        {
	        // The P9 app should have become a P2 app after the idle period, therefore it should have been the first candidate for closure
	        _LIT( KResult ,"P9->P2 App KOomTestAppUid running");
	        aResult.iResultDes.Copy( KResult );
	        aResult.iResult = KErrGeneral;
	        // Case was executed but failed
	        }	

	    if (err == KErrNone)
	        {	
	        err = AppIsRunning(KOomTestApp2Uid, ETrue);
	        if (err != KErrNone)
	            {
	            // The P8 application should still be running because the P9 app (that has become a P2 app) has freed the required memory
	            _LIT( KResult ,"P8 App KOomTestApp2Uid not running");
	            aResult.iResultDes.Copy( KResult );
	            aResult.iResult = KErrGeneral;
	            // Case was executed but failed
	            }		
	        }

	    if (err == KErrNone)
	        {	
	        err = AppIsRunning(KOomTestApp3Uid, ETrue);	
	        if (err != KErrNone)
	            {
	            // The P7 app should still be running because it was in the foreground
	            _LIT( KResult ,"P7 App KOomTestApp3Uid not running");
	            aResult.iResultDes.Copy( KResult );
	            aResult.iResult = KErrGeneral;
	            // Case was executed but failed
	            }	
	        }

	    if (err == KErrNone)
	        {
	        _LIT( KDescription , "Test case passed");
	        aResult.SetResult( KErrNone, KDescription );
	        }
        }
    else
        {
        _LIT( KResult ,"Watchers failed to start or Application Timeout or Memory Still Below Treshhold ");
        aResult.iResultDes.Copy( KResult );
        aResult.iResult = KErrGeneral;
        // case was not executed or case was executed but never finished or failed 
        }
    
	ResetL();    
    return KErrNone;	
	}

TInt COomTestHarness::AppCloseEqualPrioritiesL(TTestResult& aResult)
	{
	ResetL();
	
	StartApplicationL(KOomTestApp4Uid, 3 * 1024); // P7 app1, configure it to eat 5MB

	Settle();	
	
	StartApplicationL(KOomTestApp3Uid, 3 * 1024); // P7 app2, configure it to eat 5MB

	Settle();
	
	StartApplicationL(KOomTestApp5Uid, 3 * 1024); // P7 app3, configure it to eat 5MB

	Settle();	
	
	StartApplicationL(KOomTestAppUid, 0); //P9 app
	
	Settle();
	
    //start watchers
    TInt memTestState = ETestInit;
    TInt err = KErrNone;
    
    StartMemoryMonitorStatusWatcher(memTestState);
	
    // Go significantly under the low memory threshold
    EatMemoryL(KOomSignificantlyUnderLowMemoryThreshold);

    //start timer, start scheduler & stop watchers when done
    StartTimerAndRunWatcher(memTestState);


    if (err == KErrNone && memTestState == ETestAbove)
        {
        err = AppIsRunning(KOomTestApp4Uid, EFalse);	
        if (err != KErrNone)
            {
            // The P7 app furthest to the back in the z order should be closed.
            // This should release sufficient memory.
            _LIT( KResult ,"P7 App KOomTestApp4Uid incorrectly running");
            aResult.iResultDes.Copy( KResult );
            aResult.iResult = KErrGeneral;
           // Case was executed but failed
            }
        
        // the other 3 apps should all still be running
        if (err == KErrNone)
            {	
            err = AppIsRunning(KOomTestApp3Uid, ETrue);	
            if (err != KErrNone)
                {
                _LIT( KResult ,"App KOomTestApp3Uid not running");
                aResult.iResultDes.Copy( KResult );
                aResult.iResult = KErrGeneral;
                // Case was executed but failed
                }
            }
        if (err == KErrNone)
            {	
            err = AppIsRunning(KOomTestApp5Uid, ETrue);	
            if (err != KErrNone)
                {
                _LIT( KResult ,"App KOomTestApp5Uid not running");
                aResult.iResultDes.Copy( KResult );
                aResult.iResult = KErrGeneral;
                // Case was executed but failed
                }
            }
        if (err == KErrNone)
            {	
            err = AppIsRunning(KOomTestAppUid, ETrue);	
            if (err != KErrNone)
                {
                // The P9 app should still be running because the P8 application freed enough memory
                _LIT( KResult ,"P9 App KOomTestAppUid not running");
                aResult.iResultDes.Copy( KResult );
                aResult.iResult = KErrGeneral;
                // Case was executed but failed
                }
            }
        if (err == KErrNone)
            {
            _LIT( KDescription , "Test case passed");
            aResult.SetResult( KErrNone, KDescription );
            }
        }
    else
        {
        _LIT( KResult ,"Watchers failed to start or Application Timeout or Memory Still Below Treshhold ");
        aResult.iResultDes.Copy( KResult );
        aResult.iResult = KErrGeneral;
        // case was not executed or case was executed but never finished or failed 
        }
    
	ResetL();    
    return KErrNone;	
	}


TInt COomTestHarness::PluginTest1L(TTestResult& aResult)
	{
	ResetL();
	
	// Configure the plugins to eat 0.6MB each:
	for (TInt pluginIndex = KUidOOMDummyPluginFirstImplementation; pluginIndex <= KUidOOMDummyPluginLastImplementation - 2; pluginIndex++)
		{
		User::LeaveIfError(iAllocServer.Configure(TUid::Uid(pluginIndex), 0, 0x96000, 0x96000));
		}
	//start application so that plugin 0x10286A3A which does the check can be called
	StartApplicationL(KOomTestApp2Uid, 0);
	
    //start watchers
    TInt memTestState = ETestInit;
    TInt err = KErrNone;
    
    StartMemoryMonitorStatusWatcher(memTestState);

    // Go just under the low memory threshold
    EatMemoryL(KOomJustUnderLowMemoryThreshold);
    
    //start timer, start scheduler & stop watchers when done
    StartTimerAndRunWatcher(memTestState);
    
    if (err == KErrNone && memTestState == ETestAbove)
        {	
        // The first two app plugins should have been run, releasing 
        // the required memory. The second app plugin to be run - 10286A3A - has a check,
        // so no other plugins should have run.
        err = PluginFreeRamCalledL(0x10286A3B, ETrue);	
        if (err != KErrNone)
            {
            _LIT( KResult ,"Plugin 0x10286A3B FreeRam not called");
            aResult.iResultDes.Copy( KResult );
            aResult.iResult = KErrGeneral;
            // Case was executed but failed
            }
        
        if (err == KErrNone)
            {	
            err = PluginFreeRamCalledL(0x10286A3A, ETrue);
            if (err != KErrNone)
                {
                _LIT( KResult ,"Plugin 0x10286A3A FreeRam not called");
                aResult.iResultDes.Copy( KResult );
                aResult.iResult = KErrGeneral;
                // Case was executed but failed
                }
            }
    
        // These two plugins should not be called	
        if (err == KErrNone)
            {	
            err = PluginFreeRamCalledL(0x10286A38, EFalse);
            if (err != KErrNone)
                {
                _LIT( KResult ,"Plugin 0x10286A38 FreeRam incorrectly called");
                aResult.iResultDes.Copy( KResult );
                aResult.iResult = KErrGeneral;
                // Case was executed but failed
                }
            }	
        
        if 	(err == KErrNone)
            {
            err = PluginFreeRamCalledL(0x10286A34, EFalse);
            if (err != KErrNone)
                {
                _LIT( KResult ,"Plugin 0x10286A34 FreeRam incorrectly called");
                aResult.iResultDes.Copy( KResult );
                aResult.iResult = KErrGeneral;
                // Case was executed but failed
                }
            }	
    
        // Finally check that the plugins have been notified of the final good memory state
        if (err == KErrNone)
            {					
            err = PluginMemoryGoodCalledL(0x10286A3A, ETrue);
            if (err != KErrNone)
                {
                _LIT( KResult ,"plugin  0x10286A3A MemoryGood not called");
                aResult.iResultDes.Copy( KResult );
                aResult.iResult = KErrGeneral;
                // Case was executed but failed
                }
            }
            
        if (err == KErrNone)
            {					
            err = PluginMemoryGoodCalledL(0x10286A3B, ETrue);
            if (err != KErrNone)
                {
                _LIT( KResult ,"plugin  0x10286A3B MemoryGood not called");
                aResult.iResultDes.Copy( KResult );
                aResult.iResult = KErrGeneral;
                // Case was executed but failed
                }
            }
        
        if (err == KErrNone)
            {					
            err = PluginMemoryGoodCalledL(0x10286A34, EFalse);
            if (err != KErrNone)
                {
                _LIT( KResult ,"plugin  0x10286A34 MemoryGood incorrectly called");
                aResult.iResultDes.Copy( KResult );
                aResult.iResult = KErrGeneral;
                // Case was executed but failed
                }
            }
        
        if (err == KErrNone)
            {
            // MemoryGood should not be called on this plugin because FreeMemory was never called on it
            // 10286A38
            err = PluginMemoryGoodCalledL(0x10286A38, EFalse);	
            if (err != KErrNone)
                {
                _LIT( KResult ,"plugin 0x10286A38 MemoryGood incorrectly called");
                aResult.iResultDes.Copy( KResult );
                aResult.iResult = KErrGeneral;
                // Case was executed but failed
                }
            }
        
        
        if (err == KErrNone)
            {
            _LIT( KDescription , "Test case passed");
            aResult.SetResult( KErrNone, KDescription );
            }
        }
    else
        {
        _LIT( KResult ,"Watchers failed to start or Application Timeout or Memory Still Below Treshhold");
        aResult.iResultDes.Copy( KResult );
        aResult.iResult = KErrGeneral;
        // case was not executed or case was executed but never finished or failed 
        }

	ResetL();    
    return KErrNone;	
	}


// Test application plugins
// Start two target apps
// Simulate a low memory event
// The one of the application plugins should now be run, displacing one of the system plugins
// The target apps are of sufficiently high priority that they will not be closed
TInt COomTestHarness::PluginTest2L(TTestResult& aResult)
	{
	ResetL();
	
	// Configure the plugins to eat 0.5MB each:
	for (TInt pluginIndex = KUidOOMDummyPluginFirstImplementation; pluginIndex <= KUidOOMDummyPluginLastImplementation - 2; pluginIndex++)
		{
		User::LeaveIfError(iAllocServer.Configure(TUid::Uid(pluginIndex), 0, 0x80000, 0x80000));
		}
	
	// Start the two target applications (plus a third so that the target apps are not in the foreground)
	StartApplicationL(KOomTestApp2Uid, 0);
	StartApplicationL(KOomTestApp3Uid, 0);
	StartApplicationL(KOomTestAppUid, 0);
	
	BringAppToForeground(KOomTestAppUid);
    
    //start watchers
    TInt memTestState = ETestInit;
    TInt err = KErrNone;
    
    StartMemoryMonitorStatusWatcher(memTestState);

    // Go just under the low memory threshold
    EatMemoryL(KOomJustUnderLowMemoryThreshold);

    //start timer, start scheduler & stop watchers when done
    StartTimerAndRunWatcher(memTestState);
    
    if (err == KErrNone && memTestState == ETestAbove)
        {
        // The first two system plugins should have been closed, releasing the required memory
        // The following two system plugins won't be called (the app plugins will now take their place)
        
        // The following plugins should be called...
        // Application plugins: 10286A3A 10286A3B
        // System plugins: 10286A3C(v2 plugin) 10286A34
        
        err = PluginFreeRamCalledL(0x10286A3B, ETrue);
        if (err != KErrNone)
            {
            _LIT( KResult ,"plugin  0x10286A3B not called ");
            aResult.iResultDes.Copy( KResult );
            aResult.iResult = KErrGeneral;
            // Case was executed but failed
            }
        
        if (err == KErrNone)
            {					
            err = PluginFreeRamCalledL(0x10286A3A, ETrue);
            if (err != KErrNone)
                {
                _LIT( KResult ,"plugin  0x10286A3A not called ");
                aResult.iResultDes.Copy( KResult );
                aResult.iResult = KErrGeneral;
                // Case was executed but failed
                }
            }
        
        if (err == KErrNone)
            {	
            // Plugins and apps with higher priorities will not be called/closed because 0x10286A3A is configured to check memory before running anything else
            err = PluginFreeRamCalledL(0x10286A3C, EFalse);
            if (err != KErrNone)
                {
                _LIT( KResult ,"plugin 0x10286A3C incorrectly called ");
                aResult.iResultDes.Copy( KResult );
                aResult.iResult = KErrGeneral;
                // Case was executed but failed
                }
            }
        
        if (err == KErrNone)
            {		
            err = PluginFreeRamCalledL(0x10286A34, EFalse);
            if (err != KErrNone)
                {
                _LIT( KResult ,"plugin 0x10286A34 incorrectly called");
                aResult.iResultDes.Copy( KResult );
                aResult.iResult = KErrGeneral;
                // Case was executed but failed
                }
            }
        
        if (err == KErrNone)
            {	
            // The following plugin should also be called (max_batch_size only applies to application closures, not running plugins)
            // 10286A35
            err = PluginFreeRamCalledL(0x10286A35, EFalse);	
            if (err != KErrNone)
                {
                _LIT( KResult ,"Plugin 0x10286A35 incorrectly called");
                aResult.iResultDes.Copy( KResult );
                aResult.iResult = KErrGeneral;
                // Case was executed but failed
                }
            }
        
        if (err == KErrNone)
            {		
            err = AppIsRunning(KOomTestApp3Uid, ETrue);		
            if (err != KErrNone)
                {
                _LIT( KResult ,"App KOomTestApp3Uid not running");
                aResult.iResultDes.Copy( KResult );
                aResult.iResult = KErrGeneral;
                // Case was executed but failed
                }
            }
        
        if (err == KErrNone)
            {		
            err = AppIsRunning(KOomTestApp2Uid, ETrue);
            if (err != KErrNone)
                {
                _LIT( KResult ,"App KOomTestApp2Uid not running");
                aResult.iResultDes.Copy( KResult );
                aResult.iResult = KErrGeneral;
                // Case was executed but failed
                }
            }
        
        if (err == KErrNone)
            {			
            err = AppIsRunning(KOomTestAppUid, ETrue);		
            if (err != KErrNone)
                {
                _LIT( KResult ,"plugin KOomTestAppUid not running");
                aResult.iResultDes.Copy( KResult );
                aResult.iResult = KErrGeneral;
                // Case was executed but failed
                }
            }
        
        if (err == KErrNone)
            {
            _LIT( KDescription , "Test case passed");
            aResult.SetResult( KErrNone, KDescription );
            }
        }
    else
        {
        _LIT( KResult ,"Watchers failed to start or Application Timeout or Memory Still Below Treshhold ");
        aResult.iResultDes.Copy( KResult );
        aResult.iResult = KErrGeneral;
        // case was not executed or case was executed but never finished or failed 
        }

	ResetL();    
    return KErrNone;	
	}



// Test that the aBytesRequested is correctly passed to the FreeMemory function of V2 plugins
// Initialise P4 plugin to consume 5MB of memory
// No other plugins are configured to release memory
// Simulate a low memory event (go just below the low threshold)
// Check that the P4 plugin has been called
// Check that the P4 plugin has received a request for > 500K and less than <1500K
TInt COomTestHarness::PluginV2Test1L(TTestResult& aResult)
	{
	ResetL();
	
	// Configure the P4 V2 plugin to eat 5MB:
	User::LeaveIfError(iAllocServer.Configure(TUid::Uid(0x10286A3C), 0, 0x500000, 0x500000));
	
    //start watchers
    TInt memTestState = ETestInit;
    TInt err = KErrNone;
    
    StartMemoryMonitorStatusWatcher(memTestState);
	
    // Go just under the low memory threshold
    EatMemoryL(KOomJustUnderLowMemoryThreshold);

    //start timer, start scheduler & stop watchers when done
    StartTimerAndRunWatcher(memTestState);
    
    if (err == KErrNone && memTestState == ETestAbove)
        {
        // The first four system plugins should have been closed, releasing the required memory
        // All four plugins should be called, even though the first two will release enough memory (this is because
        // plugins are always run in continue mode)
        
        // Check that the P4 plugin has been called
        err = PluginFreeRamCalledL(0x10286A3C, ETrue);
        
        // Check that the request for memory was about right
        // Note: regular system variation makes it impossible to test for an exact number
        TInt requestedMemory = 0;
        err = RProperty::Get(KUidOomPropertyCategory, 0x10286A3C + KOOMDummyPluginBytesRequested, requestedMemory);
        if ((requestedMemory < 512 * 1024) || (requestedMemory > 1500 * 1024))
            {
            _LIT( KResult ,"requestedMemory incorrect");
            aResult.iResultDes.Copy( KResult );
            aResult.iResult = KErrGeneral;
            // Case was executed but failed
            }		
    
        if (err == KErrNone)
            {
            // Check that the higher priority V2 plugin has not been called
            err = PluginFreeRamCalledL(0x10286A3D, EFalse);
            if (err != KErrNone)
                {
                _LIT( KResult ,"plugin 0x10286A3D incorrectly called");
                aResult.iResultDes.Copy( KResult );
                aResult.iResult = KErrGeneral;
                // Case was executed but failed
                }
            }
        
        if (err == KErrNone)
            {
            _LIT( KDescription , "Test case passed");
            aResult.SetResult( KErrNone, KDescription );
            }
        }
    else
        {
        _LIT( KResult ,"Watchers failed to start or Application Timeout or Memory Still Below Treshhold ");
        aResult.iResultDes.Copy( KResult );
        aResult.iResult = KErrGeneral;
        // case was not executed or case was executed but never finished or failed 
        }

	ResetL();    
    return KErrNone;	
	}


// Test simple prioritisation of application plugins
// Start two target applications
// Configure all plugins to consume 0.5MB
// Simulate a low memory event
// Some of the low priority app plugins with those target applications should be called
// The highest priority app with that target application should not be called (the lower priority plugins should free enough memory)
TInt COomTestHarness::AppPluginTest1L(TTestResult& aResult)
	{
	ResetL();
	
	// Configure the plugins to eat 0.5MB each:
	for (TInt pluginIndex = KUidOOMDummyPluginFirstImplementation; pluginIndex <= KUidOOMDummyPluginLastImplementation - 2; pluginIndex++)
		{
		User::LeaveIfError(iAllocServer.Configure(TUid::Uid(pluginIndex), 0, 0x80000, 0x80000));
		}
	
	// Start the two target applications (plus a third so that the target apps are not in the foreground)
	StartApplicationL(KOomTestApp2Uid, 0);
	StartApplicationL(KOomTestApp3Uid, 0);
	StartApplicationL(KOomTestAppUid, 0);
	

    TInt err = KErrNone;
    
    // Check that all of the apps are running
    err = AppIsRunning(KOomTestApp2Uid, ETrue);
    if (err != KErrNone)
        {
        _LIT( KResult ,"app KOomTestApp2Uid not running");
        aResult.iResultDes.Copy( KResult );
        aResult.iResult = KErrGeneral;
        // Case was executed but failed
        }
    
    if (err == KErrNone)
        {
        err = AppIsRunning(KOomTestApp3Uid, ETrue);
        if (err != KErrNone)
            {
            _LIT( KResult ,"app KOomTestApp3Uid not running");
            aResult.iResultDes.Copy( KResult );
            aResult.iResult = KErrGeneral;
            // Case was executed but failed
            }
        }
    
    if (err == KErrNone)
        {	
        err = AppIsRunning(KOomTestAppUid, ETrue);
        if (err != KErrNone)
            {
            _LIT( KResult ,"app KOomTestAppUid not running");
            aResult.iResultDes.Copy( KResult );
            aResult.iResult = KErrGeneral;
            // Case was executed but failed
            }
        }

    // Now eat some memory till we are below treshhold and wait for the memory monitor to bring us above treshhold again
    TInt memTestState = ETestInit;
    
    if (err == KErrNone)
        {	
        BringAppToForeground(KOomTestAppUid);
        
        //start watchers
        StartMemoryMonitorStatusWatcher(memTestState);
        
        // Go just under the low memory threshold
        EatMemoryL(KOomJustUnderLowMemoryThreshold);

        //start timer, start scheduler & stop watchers when done
        StartTimerAndRunWatcher(memTestState);
        
        // fail tests if watchers failed or memory was not freed
        if (memTestState != ETestAbove)
            {
            _LIT( KResult ,"Watchers failed to start or Application Timeout or Memory Still Below Treshhold ");
            aResult.iResultDes.Copy( KResult );
            aResult.iResult = KErrGeneral;
            // case was not executed or case was executed but never finished or failed 
            //if memteststate is not desired signal to other cases that they should not pass
            if (err == KErrNone)
                {
                err = KErrGeneral;
                }
            }
        }
    
    // Check plugins for memory free calls
    // The following application plugins should be called...
    // Application plugins: 10286A3A 10286A3B
    if (err == KErrNone)
        {
        err = PluginFreeRamCalledL(0x10286A3A, ETrue);
        if (err != KErrNone)
            {
            _LIT( KResult ,"plugin 0x10286A3A FreeRam not called");
            aResult.iResultDes.Copy( KResult );
            aResult.iResult = KErrGeneral;
            // Case was executed but failed
            }
        }
    
    if (err == KErrNone)
        {	
        err = PluginFreeRamCalledL(0x10286A3B, ETrue);
        if (err != KErrNone)
            {
            _LIT( KResult ,"plugin 0x10286A3B FreeRam not called");
            aResult.iResultDes.Copy( KResult );
            aResult.iResult = KErrGeneral;
            // Case was executed but failed
            }
        }
    
    if (err == KErrNone)
        {
        // The following plugin should not be called because other plugins (including some unchecked system plugins) have freed enough memory
        // 10286A38
        err = PluginFreeRamCalledL(0x10286A38, EFalse);	
        if (err != KErrNone)
            {
            _LIT( KResult ,"plugin 0x10286A38 FreeRam incorrectly called");
            aResult.iResultDes.Copy( KResult );
            aResult.iResult = KErrGeneral;
            // Case was executed but failed
            }
        }
    
    if (err == KErrNone)
        {
        _LIT( KDescription , "Test case passed");
        aResult.SetResult( KErrNone, KDescription );
        }
    
	ResetL();    
    return KErrNone;	
	}

// Test simple prioritisation of application plugins
// Start two target applications
// Configure all plugins to consume 0.5MB
// The app plugin with the highest priority is configured to be assigned the lowest priority after an idle time
// Wait until the idle time rule applies
// Simulate a low memory event
// The plugin that initially had the highest priority (but now has the lowest priority) should be called
// Note that this test should be the same as AppPluginTest1L with the addition of the idle period
TInt COomTestHarness::AppPluginIdleTimeTest1L(TTestResult& aResult)
	{
	ResetL();
	
	// Configure the plugins to eat 0.5MB each:
	for (TInt pluginIndex = KUidOOMDummyPluginFirstImplementation; pluginIndex <= KUidOOMDummyPluginLastImplementation - 2; pluginIndex++)
		{
		User::LeaveIfError(iAllocServer.Configure(TUid::Uid(pluginIndex), 0, 0x80000, 0x80000));
		}
	
	// Start the two target applications (plus a third so that the target apps are not in the foreground)
	StartApplicationL(KOomTestApp2Uid, 0);
	StartApplicationL(KOomTestApp3Uid, 0);
	StartApplicationL(KOomTestAppUid, 0);
	
	BringAppToForeground(KOomTestAppUid);
	
	User::After(KOomTestFirstIdlePeriod);
	
   //start watchers
    TInt memTestState = ETestInit;
    TInt err = KErrNone;
    
    StartMemoryMonitorStatusWatcher(memTestState);
    
    // Go just under the low memory threshold
    EatMemoryL(KOomJustUnderLowMemoryThreshold);

    //start timer, start scheduler & stop watchers when done
    StartTimerAndRunWatcher(memTestState);

    if (err == KErrNone && memTestState == ETestAbove)
        {
        // The following application plugins should be called...
        // Application plugins: 10286A3A 10286A3B
        
        err = PluginFreeRamCalledL(0x10286A3A, ETrue);
        if (err != KErrNone)
            {
            _LIT( KResult ,"plugin 0x10286A3A FreeRam not called");
            aResult.iResultDes.Copy( KResult );
            aResult.iResult = KErrGeneral;
            // Case was executed but failed
            }
    
        if (err == KErrNone)
            {	
            err = PluginFreeRamCalledL(0x10286A3B, ETrue);
            if (err != KErrNone)
                {
                _LIT( KResult ,"plugin 0x10286A3B FreeRam not called");
                aResult.iResultDes.Copy( KResult );
                aResult.iResult = KErrGeneral;
                // Case was executed but failed
                }
            }
        
        if (err == KErrNone)
            {	
            // The following plugin should also be called (its priority was initially too high but has been reduced after the idle time)
            // 10286A38
            err = PluginFreeRamCalledL(0x10286A38, ETrue);	
            if (err != KErrNone)
                {
                _LIT( KResult ,"plugin 0x10286A38 FreeRam not called");
                aResult.iResultDes.Copy( KResult );
                aResult.iResult = KErrGeneral;
                // Case was executed but failed
                }
            }
        
        if (err == KErrNone)
            {
            _LIT( KDescription , "Test case passed");
            aResult.SetResult( KErrNone, KDescription );
            }
        }
    else
        {
        _LIT( KResult ,"Watchers failed to start or Application Timeout or Memory Still Below Treshhold ");
        aResult.iResultDes.Copy( KResult );
        aResult.iResult = KErrGeneral;
        // case was not executed or case was executed but never finished or failed 
        }

    ResetL();    
    return KErrNone;	
	}


// Test the optional allocation mechanism
// Configure the plugins not to release any RAM when FreeRam is called
// Configure one priority 7 application to release 12MB when FreeRam is called
// Start this application (plus another one so it isn't in the foreground)
// Drop just above the good memory level
// Request an optional allocation of 10MB referencing a priority 8 plugin
// All of the plugins below P8 should be called
// The P7 application should be closed freeing the required memory
TInt COomTestHarness::OptionalAllocationTest1L(TTestResult& aResult)
	{
	ResetL();
	
	// Start an application (plus a second so that the first app is not in the foreground)
	// The first application is set to consume 12MB of RAM
	StartApplicationL(KOomTestApp3Uid, 12 * 1024);
	StartApplicationL(KOomTestAppUid, 0);
	
	BringAppToForeground(KOomTestAppUid);

   // Go just above the good memory threshold
    EatMemoryL(KOomJustAboveGoodMemoryThreshold);
	    
    //start watchers
    TInt memTestState = ETestInit;
    TInt err = KErrNone;
    TInt bytesAvailable;
    
    StartMemoryMonitorStatusWatcher(memTestState);
    
    // Request 10 MB of data, using the priority of the referenced plugin (constant priority 8)
    err = iOomSession.RequestOptionalRam(10 * 1024 * 1024, 10 * 1024 * 1024, 0x10286A37, bytesAvailable);
    
    if(err == KErrNone)
        {
        //start timer, start scheduler & stop watchers when done
        StartTimerAndRunWatcher(memTestState);
        }

    if (err == KErrNone && memTestState == ETestAbove)
        {
        // Check that all system plugins below P8 have been called
        if (err == KErrNone)
            {		
            err = PluginFreeRamCalledL(0x10286A34, ETrue);
            if (err != KErrNone)
                {
                _LIT( KResult ,"plugin 0x10286A34 FreeRam not called");
                aResult.iResultDes.Copy( KResult );
                aResult.iResult = KErrGeneral;
                // Case was executed but failed
                }
            }
        
        if (err == KErrNone)
            {			
            err = PluginFreeRamCalledL(0x10286A35, ETrue);
            if (err != KErrNone)
                {
                _LIT( KResult ,"plugin 0x10286A35 FreeRam not called");
                aResult.iResultDes.Copy( KResult );
                aResult.iResult = KErrGeneral;
                // Case was executed but failed
                }		
            }
        
        if (err == KErrNone)
            {	
            err = PluginFreeRamCalledL(0x10286A36, ETrue);
            if (err != KErrNone)
                {
                _LIT( KResult ,"plugin 0x10286A36 FreeRam not called");
                aResult.iResultDes.Copy( KResult );
                aResult.iResult = KErrGeneral;
                // Case was executed but failed
                }
            }
        
        if (err == KErrNone)
            {			
            // This the P8 system plugin has not been called
            err = PluginFreeRamCalledL(0x10286A37, EFalse);
            if (err != KErrNone)
                {
                _LIT( KResult ,"plugin 0x10286A37 FreeRam incorrectly called");
                aResult.iResultDes.Copy( KResult );
                aResult.iResult = KErrGeneral;
                // Case was executed but failed
                }
            }
        
        if (err == KErrNone)
            {	
            // Check that the low priority app plugin has been called
            err = PluginFreeRamCalledL(0x10286A3B, ETrue);
            if (err != KErrNone)
                {
                _LIT( KResult ,"low prioirity app 0x10286A3B not called");
                aResult.iResultDes.Copy( KResult );
                aResult.iResult = KErrGeneral;
                // Case was executed but failed
                }
            }
        
        if (err == KErrNone)
            {			
            // Check that the P7 application has been closed
            err = AppIsRunning(KOomTestApp3Uid, EFalse);
            if (err != KErrNone)
                {
                _LIT( KResult ,"P7 app KOomTestApp3Uid not closed");
                aResult.iResultDes.Copy( KResult );
                aResult.iResult = KErrGeneral;
                // Case was executed but failed
                }
            }
    
        if (err == KErrNone)
            {
            _LIT( KDescription , "Test case passed");
            aResult.SetResult( KErrNone, KDescription );
            }
        }
    else 
        {
        _LIT( KResult ,"RequestOptionalRam failed or Memory Still Below Treshhold or could not start watchers");
        aResult.iResultDes.Copy( KResult );
        aResult.iResult = KErrGeneral;
        // case was not executed or case was executed but never finished or failed 
        }
    
	ResetL();    
    return KErrNone;	
	}


// Test the optional allocation mechanism - minimum requested RAM behaviour - successful request
// Configure the plugins not to release any RAM when FreeRam is called
// Configure one priority 7 application to release 5MB when FreeRam is called
// Start this application (plus another one so it isn't in the foreground)
// Drop just above the good memory level
// Request an optional allocation of 10MB (5MB minimum) referencing a priority 8 plugin
// All of the plugins below P8 should be called
// The P7 application should be closed freeing the required minimum amount memory
// The returned available memory should be about 5MB ( > 3MB and < 7MB )
TInt COomTestHarness::OptionalAllocationTest2L(TTestResult& aResult)
	{
	ResetL();
	
	// Start an application (plus a second so that the first app is not in the foreground)
	// The first application is set to consume 5MB of RAM
	StartApplicationL(KOomTestApp3Uid, 5 * 1024);
	StartApplicationL(KOomTestAppUid, 0);
	
	BringAppToForeground(KOomTestAppUid);

	// Go just above the good memory threshold
    EatMemoryL(KOomJustAboveGoodMemoryThreshold);
    
    //start watchers
    TInt memTestState = ETestInit;
    TInt err = KErrNone;
    StartMemoryMonitorStatusWatcher(memTestState);
    
    // Request 10 MB of data, using the priority of the referenced plugin (constant priority 8)
    // Say that 5MB is the minimum we need
    TInt bytesAvailable;
    err = iOomSession.RequestOptionalRam(10 * 1024 * 1024, 5 * 1024 * 1024, 0x10286A37, bytesAvailable);

    if(err == KErrNone)
        {
        //start scheduler & stop watchers when done
        StartTimerAndRunWatcher(memTestState);
        }

    if (err == KErrNone && memTestState == ETestAbove)
        {
        if (err == KErrNone)
            {			
            // Check that the reported bytes available is > 3MB and < 7MB
            if ((bytesAvailable < 3 * 1024 * 1024)
                    || (bytesAvailable > 7 * 1024 * 1024))
                {
                _LIT( KResult ,"reported bytes not > 3MB and < 7MB");
                aResult.iResultDes.Copy( KResult );
                aResult.iResult = KErrGeneral;
                // Case was executed but failed
                }
            }
        
        // Check that all system plugins below P8 have been called
    
        if (err == KErrNone)
            {			
            err = PluginFreeRamCalledL(0x10286A34, ETrue);
            if (err != KErrNone)
                {
                _LIT( KResult ,"Plugin 0x10286A34 FreeRam not called");
                aResult.iResultDes.Copy( KResult );
                aResult.iResult = KErrGeneral;
                // Case was executed but failed
                }
            }
        
        if (err == KErrNone)
            {			err = PluginFreeRamCalledL(0x10286A35, ETrue);
            if (err != KErrNone)
                {
                _LIT( KResult ,"Plugin 0x10286A35 FreeRam not called");
                aResult.iResultDes.Copy( KResult );
                aResult.iResult = KErrGeneral;
                // Case was executed but failed
                }
            }
        
        if (err == KErrNone)
            {			
            err = PluginFreeRamCalledL(0x10286A36, ETrue);
            if (err != KErrNone)
                {
                _LIT( KResult ,"Plugin 0x10286A36 FreeRam not called");
                aResult.iResultDes.Copy( KResult );
                aResult.iResult = KErrGeneral;
                // Case was executed but failed
                }
            }
        
        if (err == KErrNone)
            {			
            // This the P8 system plugin has not been called
            err = PluginFreeRamCalledL(0x10286A37, EFalse);
            if (err != KErrNone)
                {
                _LIT( KResult ,"Plugin 0x10286A37 FreeRam incorrectly called");
                aResult.iResultDes.Copy( KResult );
                aResult.iResult = KErrGeneral;
                // Case was executed but failed
                }
            }
        
        if (err == KErrNone)
            {			
            // Check that the low priority app plugin has been called
            err = PluginFreeRamCalledL(0x10286A3B, ETrue);
            if (err != KErrNone)
                {
                _LIT( KResult ,"Plugin 0x10286A3B FreeRam not called");
                aResult.iResultDes.Copy( KResult );
                aResult.iResult = KErrGeneral;
                // Case was executed but failed
                }
            }
        
        if (err == KErrNone)
            {			
            // Check that the P7 application has been closed
            err = AppIsRunning(KOomTestApp3Uid, EFalse);
            if (err != KErrNone)
                {
                _LIT( KResult ,"P7 app KOomTestApp3Uid not closed");
                aResult.iResultDes.Copy( KResult );
                aResult.iResult = KErrGeneral;
                // Case was executed but failed
                }
            }
        
        if (err == KErrNone)
            {
            _LIT( KDescription , "Test case passed");
            aResult.SetResult( KErrNone, KDescription );
            }
        }
    else
        {
        _LIT( KResult ,"RequestOptionalRam failed or Memory Still Below Treshhold or could not start watchers");
        aResult.iResultDes.Copy( KResult );
        aResult.iResult = KErrGeneral;
        // case was not executed or case was executed but never finished or failed 
        }

	ResetL();    
    return KErrNone;	
	}


// Test the optional allocation mechanism - minimum requested RAM behaviour - failed request
// Configure the plugins not to release any RAM when FreeRam is called
// Configure one priority 7 application to release 3MB when FreeRam is called
// Start this application (plus another one so it isn't in the foreground)
// Drop just above the good memory level
// Request an optional allocation of 10MB (5MB minimum) referencing a priority 8 plugin
// All of the plugins below P8 should be called
// The P7 application should be closed but it won't free enough memory
// The optional allocation should fail with KErrNoMemory
TInt COomTestHarness::OptionalAllocationTest3L(TTestResult& aResult)
	{
	ResetL();
	
	// Start an application (plus a second so that the first app is not in the foreground)
	// The first application is set to consume 5MB of RAM
	StartApplicationL(KOomTestApp3Uid, 3 * 1024);
	StartApplicationL(KOomTestAppUid, 0);
	
	BringAppToForeground(KOomTestAppUid);

    // Go just above the good memory threshold
    EatMemoryL(KOomJustAboveGoodMemoryThreshold);
    
    //start watchers
    TInt memTestState = ETestInit;
    TInt err = KErrNone;
    
    StartMemoryMonitorStatusWatcher(memTestState);

    // Request 10 MB of data, using the priority of the referenced plugin (constant priority 8)
    // Say that 5MB is the minimum we need
    TInt bytesAvailable;
    err = iOomSession.RequestOptionalRam(10 * 1024 * 1024, 5 * 1024 * 1024, 0x10286A37, bytesAvailable);
    // We expect an error because it has not been possible to free the minimum memory
    if (err == KErrNoMemory)
        {
        err = KErrNone;
        }
    
    if(err == KErrNone)
        {
        //start scheduler & stop watchers when done
        StartTimerAndRunWatcher(memTestState);
        }

    if (err == KErrNone && memTestState == ETestAbove)
        {      
        // Check that all system plugins below P8 have been called
    
        if (err == KErrNone)
            {		
            err = PluginFreeRamCalledL(0x10286A34, ETrue);
            if (err != KErrNone)
                {
                _LIT( KResult ,"Plugin 0x10286A34 FreeRam not called");
                aResult.iResultDes.Copy( KResult );
                aResult.iResult = KErrGeneral;
                // Case was executed but failed
                }
            }
        
        if (err == KErrNone)
            {			
            err = PluginFreeRamCalledL(0x10286A35, ETrue);
            if (err != KErrNone)
                {
                _LIT( KResult ,"Plugin 0x10286A35 FreeRam not called");
                aResult.iResultDes.Copy( KResult );
                aResult.iResult = KErrGeneral;
                // Case was executed but failed
                }
            }
        
        if (err == KErrNone)
            {			
            err = PluginFreeRamCalledL(0x10286A36, ETrue);
            if (err != KErrNone)
                {
                _LIT( KResult ,"Plugin 0x10286A36 FreeRam not called");
                aResult.iResultDes.Copy( KResult );
                aResult.iResult = KErrGeneral;
                // Case was executed but failed
                }
            }
        
        if (err == KErrNone)
            {			
            // This the P8 system plugin has not been called
            err = PluginFreeRamCalledL(0x10286A37, EFalse);
            if (err != KErrNone)
                {
                _LIT( KResult ,"Plugin 0x10286A37 FreeRam incorrectly called");
                aResult.iResultDes.Copy( KResult );
                aResult.iResult = KErrGeneral;
                // Case was executed but failed
                }
            }
        
        if (err == KErrNone)
            {			
            // Check that the low priority app plugin has been called
            err = PluginFreeRamCalledL(0x10286A3B, ETrue);
            if (err != KErrNone)
                {
                _LIT( KResult ,"Plugin 0x10286A3B FreeRam not called");
                aResult.iResultDes.Copy( KResult );
                aResult.iResult = KErrGeneral;
                // Case was executed but failed
                }
            }
        
        if (err == KErrNone)
            {			
            // Check that the P7 application has been closed
            err = AppIsRunning(KOomTestApp3Uid, EFalse);
            if (err != KErrNone)
                {
                _LIT( KResult ,"P7 App KOomTestApp3Uid not closed");
                aResult.iResultDes.Copy( KResult );
                aResult.iResult = KErrGeneral;
                // Case was executed but failed
                }
            }
        
        if (err == KErrNone)
            {
            _LIT( KDescription , "Test case passed");
            aResult.SetResult( KErrNone, KDescription );
            }
        }
    else
        {
        _LIT( KResult ,"RequestOptionalRam failed or Memory Still Below Treshhold or could not start watchers");
        aResult.iResultDes.Copy( KResult );
        aResult.iResult = KErrGeneral;
        // case was not executed or case was executed but never finished or failed 
        }
    
	ResetL();    
    return KErrNone;	
	}


// Test that force priority check applies (only) to selected priorities
// Setup three plugins (priorities 7, 8 & 9)to eat 5MB each
// The configuration file should force a check after priority 8
// Drop just under the low threshold
// Plugins P7 & P8 should be called (P8 is called even though the P7 plugin freed enough memory)
// Plugin P9 should not be called because enou
TInt COomTestHarness::ForcePriorityCheck1L(TTestResult& aResult)
	{
	ResetL();
	
	// Configure the P7, P8 and P9 plugins to eat 5MB each:
	User::LeaveIfError(iAllocServer.Configure(TUid::Uid(0x10286A36), 0, 0x300000, 0x300000)); // P7
	User::LeaveIfError(iAllocServer.Configure(TUid::Uid(0x10286A37), 0, 0x300000, 0x300000)); // P8
	User::LeaveIfError(iAllocServer.Configure(TUid::Uid(0x10286A3D), 0, 0x300000, 0x300000)); // P9
	
    //start watchers
    TInt memTestState = ETestInit;
    TInt err = KErrNone;
    
    StartMemoryMonitorStatusWatcher(memTestState);
	
    // Go just under the low memory threshold
    EatMemoryL(KOomJustUnderLowMemoryThreshold);
    
    //start scheduler & stop watchers when done
    StartTimerAndRunWatcher(memTestState);

    if (err == KErrNone && memTestState == ETestAbove)
        {
        
        // The P7 plugin should have been called and is the first to release RAM
        err = PluginFreeRamCalledL(0x10286A36, ETrue);
        if (err != KErrNone)
            {
            _LIT( KResult ,"P7 Plugin 0x10286A36 FreeRam not called");
            aResult.iResultDes.Copy( KResult );
            aResult.iResult = KErrGeneral;
            // Case was executed but failed
            }
    
        if (err == KErrNone)
            {		
            // The P8 plugin should be called even though the P7 plugin has already released enough memory because the plugin runs in continue mode and there is no forced check
            err = PluginFreeRamCalledL(0x10286A37, ETrue);
            if (err != KErrNone)
                {
                _LIT( KResult ,"P8 Plugin 0x10286A37 FreeRam not called");
                aResult.iResultDes.Copy( KResult );
                aResult.iResult = KErrGeneral;
                // Case was executed but failed
                }
            }
        
        if (err == KErrNone)
            {		
            // The P9 plugin should not be called because of the force priority check (the P7 & P8 plugins have already released plenty of RAM)
            err = PluginFreeRamCalledL(0x10286A3D, EFalse);	
            if (err != KErrNone)
                {
                _LIT( KResult ,"P9 Plugin 0x10286A3D FreeRam incorrectly called");
                aResult.iResultDes.Copy( KResult );
                aResult.iResult = KErrGeneral;
                // Case was executed but failed
                }
            }
        
        if (err == KErrNone)
            {
            _LIT( KDescription , "Test case passed");
            aResult.SetResult( KErrNone, KDescription );
            }
        }
    else
        {
        _LIT( KResult ,"Watchers failed to start or Application Timeout or Memory Still Below Treshhold ");
        aResult.iResultDes.Copy( KResult );
        aResult.iResult = KErrGeneral;
        // case was not executed or case was executed but never finished or failed 
        }
    
	ResetL();    
    return KErrNone;	
	}



// Test the Busy API on the OOM server
// Start three applications
// Ensure that the lowest priority app is not in the foreground
// Call the busy API on the OOM monitor for the lowest priority app
// Simulate a low memory event by going just under the low threshold
// The busy application should not be closed
// The other (non-foreground) application should be closed
TInt COomTestHarness::BusyApplicationTest1L(TTestResult& aResult)
	{
	ResetL();
	
	// Start the two target applications (plus a third so that the target apps are not in the foreground)
	StartApplicationL(KOomTestApp2Uid, 0);
	StartApplicationL(KOomTestAppUid, 0);
	StartApplicationL(KOomTestApp3Uid, 0); // Lowest priority app
	
	BringAppToForeground(KOomTestAppUid); // TODO: this doesn't seem to be working - message not getting through to the dummy application
	
	Settle();
	
	// Send the second app a message to make itself busy
	TInt id = iApps.Find<TUid>(TUid::Uid(KOomTestApp3Uid), CCDummyApplicationHandle::CompareTo);
	User::LeaveIfError(id);
	iApps[id]->SendMessage(KOomDummyAppSetBusy);
	
    //start watchers
    TInt memTestState = ETestInit;
    TInt err = KErrNone;

    StartMemoryMonitorStatusWatcher(memTestState);
	

    // Go just under the low memory threshold
    EatMemoryL(KOomJustUnderLowMemoryThreshold);

    //start scheduler & stop watchers when done
    StartTimerAndRunWatcher(memTestState);

    if (err == KErrNone && memTestState == ETestAbove)
        {
        
        // The first four system plugins should have been closed, releasing the required memory
        // All four plugins should be called, even though the first two will release enough memory (this is because
        // plugins are always run in continue mode)
        
        // Check that the busy application is still running
        err = AppIsRunning(KOomTestApp3Uid, ETrue);
        if (err != KErrNone)
            {
            _LIT( KResult ,"P7 App KOomTestApp3Uid not running");
            aResult.iResultDes.Copy( KResult );
            aResult.iResult = KErrGeneral;
            // Case was executed but failed
            }
    
        if (err == KErrNone)
            {
            // Check that the higher priority but non-busy application has been close
            err = AppIsRunning(KOomTestApp2Uid, EFalse);
            if (err != KErrNone)
                {
                _LIT( KResult ,"P7 App KOomTestApp2Uid not closed");
                aResult.iResultDes.Copy( KResult );
                aResult.iResult = KErrGeneral;
                // Case was executed but failed
                }
            }
        
        if (err == KErrNone)
            {
            _LIT( KDescription , "Test case passed");
            aResult.SetResult( KErrNone, KDescription );
            }
        }
    else
        {
        _LIT( KResult ,"Watchers failed to start or Application Timeout or Memory Still Below Treshhold ");
        aResult.iResultDes.Copy( KResult );
        aResult.iResult = KErrGeneral;
        // case was not executed or case was executed but never finished or failed 
        }
    
	ResetL();    
    return KErrNone;	
	}


// Test the NotBusy API on the OOM server
// Start three applications
// Ensure that the lowest priority app is not in the foreground
// Call the busy API on the OOM monitor for the lowest priority app
// Then call the not-busy API on the OOM monitor for the lowest priority app
// Simulate a low memory event by going just under the low threshold
// The lowest priority app should be closed (because it is no longer busy)
TInt COomTestHarness::NormalPriorityApplicationTest1L(TTestResult& aResult)
	{
	ResetL();
	
	// Start the two target applications (plus a third so that the target apps are not in the foreground)
	StartApplicationL(KOomTestApp2Uid, 0);
	StartApplicationL(KOomTestApp3Uid, 0); // Lowest priority app
	StartApplicationL(KOomTestAppUid, 0);
	
	BringAppToForeground(KOomTestAppUid);
	
	Settle();
	
	// Send the second app a message to make itself busy
	TInt id = iApps.Find<TUid>(TUid::Uid(KOomTestApp3Uid), CCDummyApplicationHandle::CompareTo);
	User::LeaveIfError(id);
	// TODO: temp removed - needs to be put back
//	iApps[id]->SendMessage(KOomDummyAppSetBusy);
	
	// Send the second app a message to make itself normal priority
	id = iApps.Find<TUid>(TUid::Uid(KOomTestApp3Uid), CCDummyApplicationHandle::CompareTo);
	User::LeaveIfError(id);
	iApps[id]->SendMessage(KOomDummyAppSetNormalPriority);

    //start watchers
    TInt memTestState = ETestInit;
    TInt err = KErrNone;
    
    StartMemoryMonitorStatusWatcher(memTestState);
    

    // Go just under the low memory threshold
    EatMemoryL(KOomJustUnderLowMemoryThreshold);

    //start scheduler & stop watchers when done
    StartTimerAndRunWatcher(memTestState);


    if (err == KErrNone && memTestState == ETestAbove)
        {
        
        // The first four system plugins should have been closed, releasing the required memory
        // All four plugins should be called, even though the first two will release enough memory (this is because
        // plugins are always run in continue mode)
        
        // Check that the no-longer-busy application has been closed
        err = AppIsRunning(KOomTestApp3Uid, EFalse);
        if (err != KErrNone)
            {
            _LIT( KResult ,"App KOomTestApp3Uid not closed");
            aResult.iResultDes.Copy( KResult );
            aResult.iResult = KErrGeneral;
            // Case was executed but failed
            }
        
        if (err == KErrNone)
            {
            _LIT( KDescription , "Test case passed");
            aResult.SetResult( KErrNone, KDescription );
            }
        }
    else
        {
        _LIT( KResult ,"Watchers failed to start or Application Timeout or Memory Still Below Treshhold ");
        aResult.iResultDes.Copy( KResult );
        aResult.iResult = KErrGeneral;
        // case was not executed or case was executed but never finished or failed 
        }
    
	ResetL();    
    return KErrNone;	
	}



// Start three applications
// One is set to NEVER_CLOSE, one is low priority, one is a dummy app to ensure that the first two are not in the foreground
// Configure applications not to release any memory
// Go just significantly under the low memory threshold
// Wait for the system to recover, if we have moved above the low memory threshold then go significantly under it again. Repeat this step until we no longer go above low.
// Check that the low priority application is closed
// Check that the NEVER_CLOSE application is not closed (even though we're still below the low theshold)
TInt COomTestHarness::NeverCloseTest1L(TTestResult& aResult)
	{
	ResetL();
	
	// Start the two target applications (plus a third so that the target apps are not in the foreground)
	StartApplicationL(KOomTestApp3Uid, 0); // Lowest priority app
	StartApplicationL(KOomTestApp7Uid, 0); // NEVER_CLOSE application
	StartApplicationL(KOomTestAppUid, 0);
	
	BringAppToForeground(KOomTestAppUid);
	
	
    //start watchers
    TInt memTestState = ETestInit;
    TInt err = KErrNone;
    TInt attemps = 10;
    
    while (memTestState != ETestBelow && attemps > 0)
        {
    StartMemoryMonitorStatusWatcher(memTestState);
	
    // eat memory
        EatMemoryL(KOomSignificantlyUnderLowMemoryThreshold);

    //start scheduler & stop watchers when done
    StartTimerAndRunWatcher(memTestState);
        attemps--;
        }

    if (err == KErrNone && memTestState == ETestBelow)
        {
        
        // The NEVER_CLOSE app should still be running (even though we are still below the low threshold)
        err = AppIsRunning(KOomTestApp7Uid, ETrue);
        if (err != KErrNone)
            {
            _LIT( KResult ,"NEVER_CLOSE app KOomTestApp7Uid not running");
            aResult.iResultDes.Copy( KResult );
            aResult.iResult = KErrGeneral;
            // Case was executed but failed
            }
    
        if (err == KErrNone)
            {	
            // The low priority app should still be closed (even though we are still below the low threshold)
            err = AppIsRunning(KOomTestApp3Uid, EFalse);
            if (err != KErrNone)
                {
                _LIT( KResult ,"low priority app KOomTestApp3Uid incorrectly running");
                aResult.iResultDes.Copy( KResult );
                aResult.iResult = KErrGeneral;
                // Case was executed but failed
                }
            }
        
        if (err == KErrNone)
            {
            _LIT( KDescription , "Test case passed");
            aResult.SetResult( KErrNone, KDescription );
            }
        }
    else
        {        
        _LIT( KResult ,"Watchers failed to start or Application Timeout or Memory Incorrectly Above Treshhold ");
        aResult.iResultDes.Copy( KResult );
        aResult.iResult = KErrGeneral;
        // case was not executed or case was executed but never finished or failed 
        }
    
	ResetL();    
    return KErrNone;	
	}





// Test that sync mode configuration is working for system plugins
// Configure three system plugins to release 5MB of memory each.
// The plugins are configured as follows
// Plugin 1: Priority 7, sync mode continue
// Plugin 2: Priority 8, sync mode check
// Plugin 3: Priority 9, sync mode continue
// Drop just under the low threshold
// Plugins 1 & 2 should be called (even though plugin 1 alone has freed enough RAM)
// Plugin 3 won't be called because the check on the priority 8 plugin discovers that enough RAM has been freed
TInt COomTestHarness::PluginSyncModeTest1L(TTestResult& aResult)
	{
	ResetL();
	
	// Configure three plugins to eat 5MB each:
	User::LeaveIfError(iAllocServer.Configure(TUid::Uid(0x10286A36), 0, 0x300000, 0x300000));
	User::LeaveIfError(iAllocServer.Configure(TUid::Uid(0x10286A37), 0, 0x300000, 0x300000));
	User::LeaveIfError(iAllocServer.Configure(TUid::Uid(0x10286A3D), 0, 0x300000, 0x300000));
	
    //start watchers
    TInt memTestState = ETestInit;
    TInt err = KErrNone;
    
    StartMemoryMonitorStatusWatcher(memTestState);
	
    // Go just under the low memory threshold
    EatMemoryL(KOomJustUnderLowMemoryThreshold);

    //start scheduler & stop watchers when done
    StartTimerAndRunWatcher(memTestState);

    if (err == KErrNone && memTestState == ETestAbove)
        {
        
        // The first four system plugins should have been run, releasing the required memory
        // All four plugins should be called, even though the first two will release enough memory (this is because
        // plugins are always run in continue mode)
        
        // Check that the first two plugins have been called, but not the third
        err = PluginFreeRamCalledL(0x10286A36, ETrue);
        if (err != KErrNone)
            {
            _LIT( KResult ,"plugin 0x10286A36 Free Ram not called");
            aResult.iResultDes.Copy( KResult );
            aResult.iResult = KErrGeneral;
            // Case was executed but failed
            }
    
        if (err == KErrNone)
            {		
            err = PluginFreeRamCalledL(0x10286A37, ETrue);
            if (err != KErrNone)
                {
                _LIT( KResult ,"plugin 0x10286A37 Free Ram not called");
                aResult.iResultDes.Copy( KResult );
                aResult.iResult = KErrGeneral;
                // Case was executed but failed
                }
            }
    
        if (err == KErrNone)
            {			
            err = PluginFreeRamCalledL(0x10286A3D, EFalse);
            if (err != KErrNone)
                {
                _LIT( KResult ,"plugin 0x10286A3D Free Ram incorrectly called");
                aResult.iResultDes.Copy( KResult );
                aResult.iResult = KErrGeneral;
                // Case was executed but failed
                }
            }
        
        if (err == KErrNone)
            {
            _LIT( KDescription , "Test case passed");
            aResult.SetResult( KErrNone, KDescription );
            }
        }
    else
        {
        _LIT( KResult ,"Watchers failed to start or Application Timeout or Memory Still Below Treshhold ");
        aResult.iResultDes.Copy( KResult );
        aResult.iResult = KErrGeneral;
        // case was not executed or case was executed but never finished or failed 
        }
    
	ResetL();    
    return KErrNone;	
	}




// Test the optional allocation mechanism
// Configure the plugins not to release any RAM when FreeRam is called
// Configure one priority 7 application to release 12MB when FreeRam is called
// Start this application (plus another one so it isn't in the foreground)
// Drop just above the good memory level
// Request an optional allocation of 10MB referencing a priority 8 plugin
// All of the plugins below P8 should be called
// The P7 application should be closed freeing the required memory
TInt COomTestHarness::OptionalAllocationAsyncTest1L(TTestResult& aResult)
	{
	ResetL();
	
	// Start an application (plus a second so that the first app is not in the foreground)
	// The first application is set to consume 12MB of RAM
	StartApplicationL(KOomTestApp3Uid, 12 * 1024);
	StartApplicationL(KOomTestAppUid, 0);
	
	BringAppToForeground(KOomTestAppUid);
	
	// Go just above the good memory threshold
	EatMemoryL(KOomJustAboveGoodMemoryThreshold);
    
    //start watchers
    TInt memTestState = ETestInit;
    TInt err = KErrNone; 
    TInt bytesAvailable;
    
    StartMemoryMonitorStatusWatcher(memTestState);
    
    // Request 10 MB of data, using the priority of the referenced plugin (constant priority 8)
    err = RequestOptionalRamASyncWrapper(10 * 1024 * 1024, 10 * 1024 * 1024, 0x10286A37, bytesAvailable);

    if (err == KErrNone)
        {
        //start scheduler & stop watchers when done
        StartTimerAndRunWatcher(memTestState);
        }

    if (err == KErrNone && memTestState == ETestAbove)
        {        
        // Check that all system plugins below P8 have been called
        
        err = PluginFreeRamCalledL(0x10286A34, ETrue);
        if (err != KErrNone)
            {
            _LIT( KResult ,"plugin 0x10286A34 Free Ram not called");
            aResult.iResultDes.Copy( KResult );
            aResult.iResult = KErrGeneral;
            // Case was executed but failed
            }
    
        if (err == KErrNone)
            {		
            err = PluginFreeRamCalledL(0x10286A35, ETrue);
            if (err != KErrNone)
                {
                _LIT( KResult ,"plugin 0x10286A35 Free Ram not called");
                aResult.iResultDes.Copy( KResult );
                aResult.iResult = KErrGeneral;
                // Case was executed but failed
                }
            }
    
        if (err == KErrNone)
            {	
            err = PluginFreeRamCalledL(0x10286A36, ETrue);
            if (err != KErrNone)
                {
                _LIT( KResult ,"plugin 0x10286A36 Free Ram not called");
                aResult.iResultDes.Copy( KResult );
                aResult.iResult = KErrGeneral;
                // Case was executed but failed
                }
            }
    
        if (err == KErrNone)
            {			
            // This the P8 system plugin has not been called
            err = PluginFreeRamCalledL(0x10286A37, EFalse);
            if (err != KErrNone)
                {
                _LIT( KResult ,"plugin 0x10286A37 Free Ram incorrectly called");
                aResult.iResultDes.Copy( KResult );
                aResult.iResult = KErrGeneral;
                // Case was executed but failed
                }
            }
    
        if (err == KErrNone)
            {				
            // Check that the low priority app plugin has been called
            err = PluginFreeRamCalledL(0x10286A3B, ETrue);
            if (err != KErrNone)
                {
                _LIT( KResult ,"plugin 0x10286A3B Free Ram not called");
                aResult.iResultDes.Copy( KResult );
                aResult.iResult = KErrGeneral;
                // Case was executed but failed
                }
            }
    
        if (err == KErrNone)
            {				
            // Check that the P7 application has been closed
            err = AppIsRunning(KOomTestApp3Uid, EFalse);
            if (err != KErrNone)
                {
                _LIT( KResult ,"P7 App KOomTestApp3Uid incorrectly running");
                aResult.iResultDes.Copy( KResult );
                aResult.iResult = KErrGeneral;
                // Case was executed but failed
                }
            }
        
        if (err == KErrNone)
            {
            _LIT( KDescription , "Test case passed");
            aResult.SetResult( KErrNone, KDescription );
            }
        }
    else
        {
        _LIT( KResult ,"RequestOptionalRam failed or Memory Still Below Treshhold or could not start watchers");
        aResult.iResultDes.Copy( KResult );
        aResult.iResult = KErrGeneral;
        // case was not executed or case was executed but never finished or failed 
        }
    
	ResetL();    
    return KErrNone;	
	}


// Test the optional allocation mechanism - minimum requested RAM behaviour - successful request
// Configure the plugins not to release any RAM when FreeRam is called
// Configure one priority 7 application to release 5MB when FreeRam is called
// Start this application (plus another one so it isn't in the foreground)
// Drop just above the good memory level
// Request an optional allocation of 10MB (5MB minimum) referencing a priority 8 plugin
// All of the plugins below P8 should be called
// The P7 application should be closed freeing the required minimum amount memory
// The returned available memory should be about 5MB ( > 3MB and < 7MB )
TInt COomTestHarness::OptionalAllocationAsyncTest2L(TTestResult& aResult)
	{
	ResetL();
	
	// Start an application (plus a second so that the first app is not in the foreground)
	// The first application is set to consume 5MB of RAM
	StartApplicationL(KOomTestApp3Uid, 5 * 1024);
	StartApplicationL(KOomTestAppUid, 0);
	
	BringAppToForeground(KOomTestAppUid);
	
	// Go just above the good memory threshold
	EatMemoryL(KOomJustAboveGoodMemoryThreshold);
    
	//start watchers
    TInt memTestState = ETestInit;
    TInt err = KErrNone;
    TInt bytesAvailable;
    
    StartMemoryMonitorStatusWatcher(memTestState);
  
    // Request 10 MB of data, using the priority of the referenced plugin (constant priority 8)
    // Say that 5MB is the minimum we need
    err = RequestOptionalRamASyncWrapper(10 * 1024 * 1024, 5 * 1024 * 1024, 0x10286A37, bytesAvailable);  

    if (err == KErrNone)
        {
        //start scheduler & stop watchers when done
        StartTimerAndRunWatcher(memTestState);
        }

    if (err == KErrNone && memTestState == ETestAbove)
        {    
        if (err == KErrNone)
            {
            // Check that the reported bytes available is > 3MB and < 7MB
            if ((bytesAvailable < 3 * 1024 * 1024)
                    || (bytesAvailable > 7 * 1024 * 1024))
                {
                _LIT( KResult ,"bytesAvailable not ( > 3MB and < 7MB )");
                aResult.iResultDes.Copy( KResult );
                aResult.iResult = KErrGeneral;
                // Case was executed but failed
                }
            }
    
        if (err == KErrNone)
            {		
            // Check that all system plugins below P8 have been called
            
            err = PluginFreeRamCalledL(0x10286A34, ETrue);
            if (err != KErrNone)
                {
                _LIT( KResult ,"plugin 0x10286A34 Free Ram not called");
                aResult.iResultDes.Copy( KResult );
                aResult.iResult = KErrGeneral;
                // Case was executed but failed
                }
            }
    
        if (err == KErrNone)
            {				
            err = PluginFreeRamCalledL(0x10286A35, ETrue);
            if (err != KErrNone)
                {
                _LIT( KResult ,"plugin 0x10286A35 Free Ram not called");
                aResult.iResultDes.Copy( KResult );
                aResult.iResult = KErrGeneral;
                // Case was executed but failed
                }
            }
    
        if (err == KErrNone)
            {				
            err = PluginFreeRamCalledL(0x10286A36, ETrue);
            if (err != KErrNone)
                {
                _LIT( KResult ,"plugin 0x10286A36 Free Ram not called");
                aResult.iResultDes.Copy( KResult );
                aResult.iResult = KErrGeneral;
                // Case was executed but failed
                }
            }
    
        if (err == KErrNone)
            {				
            // This the P8 system plugin has not been called
            err = PluginFreeRamCalledL(0x10286A37, EFalse);
            if (err != KErrNone)
                {
                _LIT( KResult ,"plugin 0x10286A37 Free Ram incorrectly called");
                aResult.iResultDes.Copy( KResult );
                aResult.iResult = KErrGeneral;
                // Case was executed but failed
                }
            }
    
        if (err == KErrNone)
            {				
            // Check that the low priority app plugin has been called
            err = PluginFreeRamCalledL(0x10286A3B, ETrue);
            if (err != KErrNone)
                {
                _LIT( KResult ,"plugin 0x10286A3B Free Ram not called");
                aResult.iResultDes.Copy( KResult );
                aResult.iResult = KErrGeneral;
                // Case was executed but failed
                }
            }
    
        if (err == KErrNone)
            {				
            // Check that the P7 application has been closed
            err = AppIsRunning(KOomTestApp3Uid, EFalse);
            if (err != KErrNone)
                {
                _LIT( KResult ,"P7 App KOomTestApp3Uid incorrectly running");
                aResult.iResultDes.Copy( KResult );
                aResult.iResult = KErrGeneral;
                // Case was executed but failed
                }
            }
        
        if (err == KErrNone)
            {
            _LIT( KDescription , "Test case passed");
            aResult.SetResult( KErrNone, KDescription );
            }
        }
    else
        {
        _LIT( KResult ,"RequestOptionalRam failed or Memory Still Below Treshhold or could not start watchers");
        aResult.iResultDes.Copy( KResult );
        aResult.iResult = KErrGeneral;
        // case was not executed or case was executed but never finished or failed 
        }

	ResetL();    
    return KErrNone;	
	}



// Test the optional allocation mechanism - minimum requested RAM behaviour - failed request
// Configure the plugins not to release any RAM when FreeRam is called
// Configure one priority 7 application to release 3MB when FreeRam is called
// Start this application (plus another one so it isn't in the foreground)
// Drop just above the good memory level
// Request an optional allocation of 10MB (5MB minimum) referencing a priority 8 plugin
// All of the plugins below P8 should be called
// The P7 application should be closed but it won't free enough memory
// The optional allocation should fail with KErrNoMemory
TInt COomTestHarness::OptionalAllocationAsyncTest3L(TTestResult& aResult)
	{
	ResetL();
	
	// Start an application (plus a second so that the first app is not in the foreground)
	// The first application is set to consume 5MB of RAM
	StartApplicationL(KOomTestApp3Uid, 3 * 1024);
	StartApplicationL(KOomTestAppUid, 0);
	
	BringAppToForeground(KOomTestAppUid);
	
	// Go just above the good memory threshold
	EatMemoryL(KOomJustAboveGoodMemoryThreshold);
    
    //start watchers
    TInt memTestState = ETestInit;
    TInt err = KErrNone;
    
    StartMemoryMonitorStatusWatcher(memTestState);
    
    // Request 10 MB of data, using the priority of the referenced plugin (constant priority 8)
    // Say that 5MB is the minimum we need
    TInt bytesAvailable;
       
    err = RequestOptionalRamASyncWrapper(10 * 1024 * 1024, 5 * 1024 * 1024, 0x10286A37, bytesAvailable);
    // We expect an error because it has not been possible to free the minimum memory
    if(err == KErrNoMemory)
        {
        err = KErrNone;
        }

    if (err == KErrNone)
        {
        //start scheduler & stop watchers when done
        StartTimerAndRunWatcher(memTestState);
        }

    if (err == KErrNone && memTestState == ETestAbove)
        {

        if (err == KErrNone)
            {	
            // Check that all system plugins below P8 have been called
            
            err = PluginFreeRamCalledL(0x10286A34, ETrue);
            if (err != KErrNone)
                {
                _LIT( KResult ,"plugin 0x10286A34 Free Ram not called");
                aResult.iResultDes.Copy( KResult );
                aResult.iResult = KErrGeneral;
                // Case was executed but failed
                }
            }
    
        if (err == KErrNone)
            {			
            err = PluginFreeRamCalledL(0x10286A35, ETrue);
            if (err != KErrNone)
                {
                _LIT( KResult ,"plugin 0x10286A35 Free Ram not called");
                aResult.iResultDes.Copy( KResult );
                aResult.iResult = KErrGeneral;
                // Case was executed but failed
                }
            }
    
        if (err == KErrNone)
            {			
            err = PluginFreeRamCalledL(0x10286A36, ETrue);
            if (err != KErrNone)
                {
                _LIT( KResult ,"plugin 0x10286A36 Free Ram not called");
                aResult.iResultDes.Copy( KResult );
                aResult.iResult = KErrGeneral;
                // Case was executed but failed
                }
            }
    
        if (err == KErrNone)
            {				
            // This the P8 system plugin has not been called
            err = PluginFreeRamCalledL(0x10286A37, EFalse);
            if (err != KErrNone)
                {
                _LIT( KResult ,"plugin 0x10286A37 Free Ram incorrectly called");
                aResult.iResultDes.Copy( KResult );
                aResult.iResult = KErrGeneral;
                // Case was executed but failed
                }
            }
    
        if (err == KErrNone)
            {			
            // Check that the low priority app plugin has been called
            err = PluginFreeRamCalledL(0x10286A3B, ETrue);
            if (err != KErrNone)
                {
                _LIT( KResult ,"plugin 0x10286A3B Free Ram not called");
                aResult.iResultDes.Copy( KResult );
                aResult.iResult = KErrGeneral;
                // Case was executed but failed
                }
            }
    
        if (err == KErrNone)
            {			
            // Check that the P7 application has been closed
            err = AppIsRunning(KOomTestApp3Uid, EFalse);
            if (err != KErrNone)
                {
                _LIT( KResult ,"P7 App KOomTestApp3Uid incorrectly running");
                aResult.iResultDes.Copy( KResult );
                aResult.iResult = KErrGeneral;
                // Case was executed but failed
                }
            }
        
        if (err == KErrNone)
            {
            _LIT( KDescription , "Test case passed");
            aResult.SetResult( KErrNone, KDescription );
            }
        }
    else
        {
        _LIT( KResult ,"minimum memory unexpectedly freed or still below treshhold or could not start watchers");
        aResult.iResultDes.Copy( KResult );
        aResult.iResult = KErrGeneral;
        // Case was executed but failed
        }
    
	ResetL();    
    return KErrNone;	
	}







void COomTestHarness::BringAppToForeground(TInt32 aUid)
	{
	//bring app to foreground
	TInt id = iApps.Find<TUid>(TUid::Uid(aUid), CCDummyApplicationHandle::CompareTo);
	User::LeaveIfError(id);
	iApps[id]->BringToForeground();
	User::After(TTimeIntervalMicroSeconds32(KSettlingTime));
	}


void COomTestHarness::ResetL()
	{
	Settle();
	
	// Close any dummy apps
	iApps.ResetAndDestroy();
	
	Settle();

	// Free all memory being eaten by the server
	iChunk.Adjust(KOomHarnessInitialEatenMemory); // Just eat 1K of memory initially, this can grow later
	iChunkSize = KOomHarnessInitialEatenMemory;
	
	iDummyChunk.Close();
	iDummyChunkSize = 0;
	
	User::LeaveIfError(iAllocServer.Reset());
	
	// Wait for the system to settle before getting the call counts (freeing the memory could cause some more movement).
	Settle();
	
	// Update the call counts on the plugins (add them if they're not there already)
	for (TInt pluginUid = KUidOOMDummyPluginFirstImplementation; pluginUid <= KUidOOMDummyPluginLastImplementation; pluginUid++)
		{
		TInt lowMemoryCount = 0;
		RProperty::Get(KUidOomPropertyCategory, pluginUid + KOOMDummyPluginLowMemoryCount, lowMemoryCount);

		TInt goodMemoryCallCount = 0;
		RProperty::Get(KUidOomPropertyCategory, pluginUid + KOOMDummyPluginGoodMemoryCount, goodMemoryCallCount);
		
		TPluginCallCount pluginCallCount;
		pluginCallCount.iFreeRamCallCount = lowMemoryCount;
		pluginCallCount.iMemoryGoodCallCount = goodMemoryCallCount;
		
		iPluginCallCounts.InsertL(pluginUid, pluginCallCount);
		};
	} 

void COomTestHarness::EatMemoryL(TInt aKBytesToLeaveFree)
	{
	Settle();
	
    TMemoryInfoV1Buf meminfo;
    UserHal::MemoryInfo(meminfo);
    TInt freeMem = meminfo().iFreeRamInBytes;

    // Resize the dummy chunk to consume the correct ammount of memory
	iChunkSize = freeMem + iChunkSize - (aKBytesToLeaveFree * 1024);
	
	if( iChunkSize > KOomHarnessMaxEatenMemory)
	    {
	    TInt dummy = (iChunkSize - KOomHarnessMaxEatenMemory)+ 2*1024*1024;
	    /*
	    if(dummy > (2 *1024 * 1024))
	        {
	        //User::LeaveIfError(iDummyChunk.CreateLocal(KOomHarnessInitialEatenMemory, ((dummy-(2*1024*1024)));
	        dummy = dummy - 2*1024*1024;
	        }
	    */
	    User::LeaveIfError(iDummyChunk.CreateLocal(KOomHarnessInitialEatenMemory, dummy));
	    iDummyChunkSize = KOomHarnessInitialEatenMemory;
	    TInt err1 = iDummyChunk.Adjust(dummy-1024);	
	    UserHal::MemoryInfo(meminfo);
	    freeMem = meminfo().iFreeRamInBytes;
	    iChunkSize = freeMem + KOomHarnessInitialEatenMemory - (aKBytesToLeaveFree * 1024);
	    }
	    
	    
	TInt err = iChunk.Adjust(iChunkSize);
	User::LeaveIfError(err);
	}

void COomTestHarness::AllocateMemoryL(TInt aKBytes)
    {
    TInt err = iChunk.Allocate(aKBytes * 1024);
    User::LeaveIfError(err);    
    }

// Set up the plugins and applications - this is the starting point for many test cases
// 5 applications are started with approx 0.5 MB used for each
// 0.5MB (approx) of memory is reserved for each one of the plugins
void COomTestHarness::CommonSetUpL()
	{
	// Configure the plugins to eat 0.5MB each:
	for (TInt pluginIndex = KUidOOMDummyPluginFirstImplementation; pluginIndex <= KUidOOMDummyPluginLastImplementation; pluginIndex++)
		{
		User::LeaveIfError(iAllocServer.Configure(TUid::Uid(pluginIndex), 0, 0x80000, 0x80000));
		}
	}
	
void COomTestHarness::StartApplicationL(TInt32 aUid, TInt aInitialAllocationInKBs)
	{
	User::ResetInactivityTime();
	CCDummyApplicationHandle *app = CCDummyApplicationHandle::NewLC(TUid::Uid(aUid), aInitialAllocationInKBs * 1024);
	iApps.AppendL(app);
	CleanupStack::Pop(app);
	Settle();
	}

// Returns KErrNone if we get the expected result
TInt COomTestHarness::AppIsRunning(TInt32 aUid, TBool aExpectedResult)
	{
	TBool appRunning = ETrue;
	
	TInt id = iApps.Find<TUid>(TUid::Uid(aUid), CCDummyApplicationHandle::CompareTo);
	if (id < 0)
		appRunning = EFalse;
	else if(iApps[id]->Process().ExitType() != EExitPending)
		appRunning = EFalse;
	
	TInt err = KErrNone;
	
	if (aExpectedResult != appRunning)
		err = KErrGeneral;
	
	if (aExpectedResult != appRunning)
		{
		err = KErrGeneral;
		}	
	
	return err;
	}

// Has FreeRam been called on this plugin since the last call to this function (or since the whole class has been reset)?
// Returns KErrNone if we get the expected result
TInt COomTestHarness::PluginFreeRamCalledL(TInt32 aUid, TBool aExpectedResult)
	{
	TInt newLowMemoryCount = 0;
	TInt err = RProperty::Get(KUidOomPropertyCategory, aUid + KOOMDummyPluginLowMemoryCount, newLowMemoryCount);

	//handle error?
	
	TBool freeRamHasBeenCalledOnPlugin = EFalse;
	TPluginCallCount* pluginCallCount = iPluginCallCounts.Find(aUid);
	
	if (pluginCallCount)
		{
		freeRamHasBeenCalledOnPlugin = (pluginCallCount->iFreeRamCallCount != newLowMemoryCount);
		
		if (freeRamHasBeenCalledOnPlugin)
			{
			// Update the list of old counts with the current value so we can see if it has changed next time this function is called
			pluginCallCount->iFreeRamCallCount = newLowMemoryCount;
			iPluginCallCounts.InsertL(aUid, *pluginCallCount);
			}
		}
	else
		{
		err = KErrNotFound;
		}
		
	if (aExpectedResult != freeRamHasBeenCalledOnPlugin)
		{
		err = KErrGeneral;
		}
	
	return err;
	}

// Has FreeRam been called on this plugin since the last call to this function (or since the whole class has been reset)?
// Returns KErrNone if we get the expected result
TInt COomTestHarness::PluginMemoryGoodCalledL(TInt32 aUid, TBool aExpectedResult)
	{
	TInt newGoodMemoryCount = 0;
	RProperty::Get(KUidOomPropertyCategory, aUid + KOOMDummyPluginGoodMemoryCount, newGoodMemoryCount);

	TBool memoryGoodHasBeenCalledOnPlugin = EFalse;
	TPluginCallCount* pluginCallCount = iPluginCallCounts.Find(aUid);
	
	if (pluginCallCount)
		{
		memoryGoodHasBeenCalledOnPlugin = (pluginCallCount->iMemoryGoodCallCount != newGoodMemoryCount);
		
		if (memoryGoodHasBeenCalledOnPlugin)
			{
			// Update the list of old counts with the current value so we can see if it has changed next time this function is called
			pluginCallCount->iMemoryGoodCallCount = newGoodMemoryCount;
			iPluginCallCounts.InsertL(aUid, *pluginCallCount);
			}
		}
	
	TInt err = KErrNone;
	
	if (aExpectedResult != memoryGoodHasBeenCalledOnPlugin)
		err = KErrGeneral;
	
	return err;
	}

// Utility function which calls the async version of optional RAM request and makes it behave like the sync version
TInt COomTestHarness::RequestOptionalRamASyncWrapper(TInt aBytesRequested, TInt aMinimumBytesNeeded, TInt aPluginId, TInt& aBytesAvailable)
	{
	TInt err = KErrNone;
	TRequestStatus status;
	iOomSession.RequestOptionalRam(aBytesRequested, aMinimumBytesNeeded, aPluginId, status);
	User::WaitForRequest(status);
	
	if (status.Int() < 0)
		err = status.Int();
	else
		aBytesAvailable = status.Int();
	
	return err;
	}

void COomTestHarness::StartMemoryMonitorStatusWatcher(TInt& aTestState)
    {
    iStatusWatcher->Start(&aTestState);
    }

void COomTestHarness::StartTimerAndRunWatcher(TInt& aTestState)
    {
    //start timer
    iTimeoutWatcher->Start(&aTestState, KTimeout);
    
    //start active scheduler to catch mem monitor status changes
    CActiveScheduler::Start();
    
    //One of the active objects has fired. Cancelling all pending requests
    iStatusWatcher->Cancel();
    iTimeoutWatcher->Cancel();
    }

TInt COomTestHarness::GetFreeMemory()
	{
	User::CompressAllHeaps();
	
	TInt current = 0;
	HAL::Get( HALData::EMemoryRAMFree, current );
	
	return current;
	}


CMemoryMonitorStatusWatcher* CMemoryMonitorStatusWatcher::NewL()
    {
    CMemoryMonitorStatusWatcher* self = new (ELeave) CMemoryMonitorStatusWatcher();
    CleanupStack::PushL(self);
    self->ConstructL();
    CleanupStack::Pop(); // self;
    return self;
    }

CMemoryMonitorStatusWatcher::~CMemoryMonitorStatusWatcher()
    {
    Cancel();
    iMonitorProperty.Close();
    }

void CMemoryMonitorStatusWatcher::Start(TInt* aTestState)
    {
    iMonitorProperty.Subscribe(iStatus);
    SetActive();

    iTestState = aTestState;
    }

CMemoryMonitorStatusWatcher::CMemoryMonitorStatusWatcher() : CActive(CActive::EPriorityStandard)
    {
    }

void CMemoryMonitorStatusWatcher::ConstructL()
    {
    CActiveScheduler::Add(this); // Add to scheduler
    User::LeaveIfError(iMonitorProperty.Attach(KOomMemoryMonitorStatusPropertyCategory, KOomMemoryMonitorStatusPropertyKey));
    }

void CMemoryMonitorStatusWatcher::DoCancel()
    {
    iMonitorProperty.Cancel();
    }

void CMemoryMonitorStatusWatcher::RunL()
    {
    iMonitorProperty.Subscribe(iStatus);
    SetActive();
    
    TInt monitorState = EFreeingMemory;
    User::LeaveIfError(iMonitorProperty.Get(monitorState));
    
    if (monitorState != EFreeingMemory)
        {
        //Do not reschedule - signal client that request has completed
        Cancel();
        if(monitorState == EBelowTreshHold)
            {
            *iTestState = ETestBelow;
            }
        else
            {
            *iTestState = ETestAbove;
            }
        CActiveScheduler::Stop();
        }
    }

CMemoryMonitorTimeoutWatcher* CMemoryMonitorTimeoutWatcher::NewL()
    {
    CMemoryMonitorTimeoutWatcher* self = new (ELeave) CMemoryMonitorTimeoutWatcher();
    CleanupStack::PushL(self);
    self->ConstructL();
    CleanupStack::Pop(); // self;
    return self;
    }

CMemoryMonitorTimeoutWatcher::~CMemoryMonitorTimeoutWatcher()
    {
    Cancel();
    }

void CMemoryMonitorTimeoutWatcher::Start(TInt* aTestState, const TTimeIntervalMicroSeconds32& aTimeout)
    {
    iTestState = aTestState;
    After(aTimeout);
    }

void CMemoryMonitorTimeoutWatcher::ConstructL()
    {
    CTimer::ConstructL();
    CActiveScheduler::Add(this); // Add to scheduler
    }

CMemoryMonitorTimeoutWatcher::CMemoryMonitorTimeoutWatcher() : CTimer(CActive::EPriorityStandard)
    {
    }

void CMemoryMonitorTimeoutWatcher::DoCancel()
    {
    CTimer::DoCancel();
    }

void CMemoryMonitorTimeoutWatcher::RunL()
    {
    Cancel();
    *iTestState = ETestTimeout;
    CActiveScheduler::Stop();
    }

////////

CAsyncRequester* CAsyncRequester::NewL(RChunk aChunk, TInt aChunkSize)
    {
    CAsyncRequester* self = new (ELeave) CAsyncRequester(aChunk, aChunkSize);
    CleanupStack::PushL(self);
    self->ConstructL();
    CleanupStack::Pop(); // self;
    return self;
    }

CAsyncRequester::~CAsyncRequester()
    {
    Cancel();
    iSession.Close();
    }

void CAsyncRequester::Start(TInt aBytesToRequest, TReturnStatus* aReturnStatus)
    {
    iReturnStatus = aReturnStatus;
    //Request 2.8MB which will close KOomTestApp3Uid
    iSession.RequestOptionalRam(aBytesToRequest, 0, 0x10286A3D, iStatus);
    SetActive();
    }

void CAsyncRequester::ConstructL()
    {
    User::LeaveIfError(iSession.Connect());    
    CActiveScheduler::Add(this); // Add to scheduler
    }

CAsyncRequester::CAsyncRequester(RChunk aChunk, TInt aChunkSize) 
    : CActive(CActive::EPriorityStandard),
    iChunk(aChunk),
    iChunkSize(aChunkSize)
    {
    }

void CAsyncRequester::DoCancel()
    {
    }

void CAsyncRequester::RunL()
    {
    iReturnStatus->iCompleted = ETrue;
    iReturnStatus->iReturnStatus = iStatus.Int();
    if (iReturnStatus->iId == 1)
        {
        TMemoryInfoV1Buf meminfo;
        UserHal::MemoryInfo(meminfo);
        TInt freeMem = meminfo().iFreeRamInBytes;

        // Resize the dummy chunk to consume the correct ammount of memory
        iChunkSize = freeMem + iChunkSize - (KOomJustAboveGoodMemoryThreshold * 1024);
        TInt err = iChunk.Adjust(iChunkSize);
        User::LeaveIfError(err);
        }
    else 
        {
        CActiveScheduler::Stop();
        }
    }

/////////


TInt COomTestHarness::AppCloseTwoSessionsL(TTestResult& aResult)
    {
    ResetL();
    
    StartApplicationL(KOomTestApp2Uid, 3 * 1024); // P8 app to be closed 
    StartApplicationL(KOomTestAppUid, 3 * 1024);  // P9 app should not be closed
    StartApplicationL(KOomTestApp3Uid, 3 * 1024); // P7 app to be closed 
    StartApplicationL(KOomTestApp4Uid, 3 * 1024); // P7 app foreground should not be closed
    
    // Go just above the good memory threshold
    EatMemoryL(KOomJustAboveGoodMemoryThreshold);
    
    Settle();
    
    TInt err = KErrNone;
    CAsyncRequester* req1 = CAsyncRequester::NewL(iChunk, iChunkSize);
    CleanupStack::PushL(req1);
    CAsyncRequester* req2 = CAsyncRequester::NewL(iChunk, iChunkSize);
    CleanupStack::PushL(req2);
    TReturnStatus status1;
    status1.iId = 1;
    TReturnStatus status2;
    status2.iId = 2;
    
    TInt memTestState = ETestInit;
    iTimeoutWatcher->Start(&memTestState, KTimeout);
        
    //This request should close KOomTestApp3Uid
    //On return the active object will allocate the memory it requested
    req1->Start(3 * 1024 * 1024, &status1);

    //The second request should be queued until the first request has completed, there will then be
    //a pause for the memory to be allocated before the request is properly serviced.
    req2->Start(3 * 1024 * 1024, &status2);

    CActiveScheduler::Start();

    //The active scheduler is stopped, we should return here once both AOs have been completed.
    iTimeoutWatcher->Cancel();
    
    if (memTestState == ETestTimeout)
        {
        err = KErrTimedOut;
        _LIT( KResult ,"Test has timed out, requests have not been completed");
        aResult.iResultDes.Copy( KResult );
        aResult.iResult = KErrGeneral;
        // Case was executed but failed
        }                
    if (err == KErrNone)
        {   
        err = AppIsRunning(KOomTestApp4Uid, ETrue); 
        if (err != KErrNone)
            {
            _LIT( KResult ,"Foreground App KOomTestApp4Uid not running");
            aResult.iResultDes.Copy( KResult );
            aResult.iResult = KErrGeneral;
            // Case was executed but failed
            }
        }
    if (err == KErrNone)
        {   
        err = AppIsRunning(KOomTestAppUid, ETrue); 
        if (err != KErrNone)
            {
            _LIT( KResult ,"P9 App KOomTestAppUid not running");
            aResult.iResultDes.Copy( KResult );
            aResult.iResult = KErrGeneral;
            // Case was executed but failed
            }
        }
    if (err == KErrNone)
        {   
        err = AppIsRunning(KOomTestApp3Uid, EFalse);  
        if (err != KErrNone)
            {
            _LIT( KResult ,"P7 App KOomTestApp3Uid incorrectly  running");
            aResult.iResultDes.Copy( KResult );
            aResult.iResult = KErrGeneral;
            // Case was executed but failed
            }
        }
    if (err == KErrNone)
        {   
        err = AppIsRunning(KOomTestApp2Uid, EFalse);  
        if (err != KErrNone)
            {
            _LIT( KResult ,"P7 App KOomTestApp2Uid incorrectly  running");
            aResult.iResultDes.Copy( KResult );
            aResult.iResult = KErrGeneral;
            // Case was executed but failed
            }
        }
    if (err == KErrNone)
        {
        _LIT( KDescription , "Test case passed");
        aResult.SetResult( KErrNone, KDescription );
        }
    
    CleanupStack::PopAndDestroy(2);
    ResetL();    
    return KErrNone;        
    }


TInt COomTestHarness::CallIfTargetAppNotRunningTest1L(TTestResult& aResult)
    {
    ResetL();
    
    // Configure the plugins to eat 0.5MB each:
    for (TInt pluginIndex = KUidOOMDummyPluginFirstImplementation; pluginIndex <= KUidOOMDummyPluginLastImplementation - 2; pluginIndex++)
        {
        User::LeaveIfError(iAllocServer.Configure(TUid::Uid(pluginIndex), 0, 0x80000, 0x80000));
        }
    
    StartApplicationL(KOomTestApp3Uid, 0);
    

    TInt err = KErrNone;
    
    // Now eat some memory till we are below treshhold and wait for the memory monitor to bring us above treshhold again
    TInt memTestState = ETestInit;
    
    //start watchers
    StartMemoryMonitorStatusWatcher(memTestState);
    
    // Go just under the low memory threshold
    EatMemoryL(KOomJustUnderLowMemoryThreshold);

    //start timer, start scheduler & stop watchers when done
    StartTimerAndRunWatcher(memTestState);
    
    // fail tests if watchers failed or memory was not freed
    if (memTestState != ETestAbove)
        {
        _LIT( KResult ,"Watchers failed to start or Application Timeout or Memory Still Below Treshhold ");
        aResult.iResultDes.Copy( KResult );
        aResult.iResult = KErrGeneral;
        // case was not executed or case was executed but never finished or failed 
        //if memteststate is not desired signal to other cases that they should not pass
        err = KErrGeneral;
        }
    
    // The following application plugins should be called...
    // Application plugins: 10286A3B
    // The following application plugins should not be called...
    // Application plugins: 10286A3A, 0x10286A38 
    if (err == KErrNone)
        {
        //target app not running for this priority 3 app plugin
        err = PluginFreeRamCalledL(0x10286A3A, EFalse);
        if (err != KErrNone)
            {
            _LIT( KResult ,"plugin 0x10286A3A FreeRam incorrectly called");
            aResult.iResultDes.Copy( KResult );
            aResult.iResult = KErrGeneral;
            // Case was executed but failed
            }
        }
    
    if (err == KErrNone)
        {   
        //target app is running for this priority 4 app plugin
        err = PluginFreeRamCalledL(0x10286A3B, ETrue);
        if (err != KErrNone)
            {
            _LIT( KResult ,"plugin 0x10286A3B FreeRam not called");
            aResult.iResultDes.Copy( KResult );
            aResult.iResult = KErrGeneral;
            // Case was executed but failed
            }
        }
    
    //By the time we get to this priority 9 plugin, the app plugins and other sys plugins 
    // have freed enough memory and a sys plugin with "check ram" has been called
    if (err == KErrNone)
        {
        err = PluginFreeRamCalledL(0x10286A38, EFalse); 
        if (err != KErrNone)
            {
            _LIT( KResult ,"plugin 0x10286A38 FreeRam incorrectly called");
            aResult.iResultDes.Copy( KResult );
            aResult.iResult = KErrGeneral;
            // Case was executed but failed
            }
        }
    
    if (err == KErrNone)
        {
        _LIT( KDescription , "Test case passed");
        aResult.SetResult( KErrNone, KDescription );
        }
    
    ResetL();
    return KErrNone;    
    }

//Test that an app that has an app_specific_thresholds but no close_app can still be closed
//
//This test only works on emulator. On hw there are many more apps started which have default priority 
//and these are closed before KOomTestApp8Uid as they are behind it in the z order, freeing sufficient memory
//before we would get to KOomTestApp8Uid
TInt COomTestHarness::AppCloseSpecificThresholdTest1L(TTestResult& aResult)
    {
    ResetL();
    
    StartApplicationL(KOomTestApp8Uid, 5 * 1024); // app with app_specific_thresholds but no close_app
    StartApplicationL(KOomTestApp3Uid, 0); // P7 foreground app. Will not be closed
    
    BringAppToForeground(KOomTestApp3Uid);
    
    //start watchers
    TInt memTestState = ETestInit;
    TInt err = KErrNone;
    
    StartMemoryMonitorStatusWatcher(memTestState);

    // Go significantly under the low memory threshold
    EatMemoryL(KOomSignificantlyUnderLowMemoryThreshold);

    //start timer, start scheduler & stop watchers when done
    StartTimerAndRunWatcher(memTestState);
    
    if (memTestState == ETestAbove)
        {
        err = AppIsRunning(KOomTestApp8Uid, EFalse);
        
        if (err != KErrNone)
            {
            _LIT( KResult ,"P8 App KOomTestApp2Uid incorrectly running");
            aResult.iResultDes.Copy( KResult );
            aResult.iResult = KErrGeneral;
           // Case was executed but failed
            }
        if (err == KErrNone)
            {   
            err = AppIsRunning(KOomTestApp3Uid, ETrue); 
            if (err != KErrNone)
                {
                // The P7 app should still be running because it was in the foreground
                _LIT( KResult ,"(P7 App KOomTestApp3Uid not running");
                aResult.iResultDes.Copy( KResult );
                aResult.iResult = KErrGeneral;
                // Case was executed but failed
                }
            }
        if (err == KErrNone)
            {
            _LIT( KDescription , "Test case passed");
            aResult.SetResult( KErrNone, KDescription );
            }
        }
    else
        {
        _LIT( KResult ,"Watchers failed to start or Application Timeout or Memory Still Below Treshhold ");
        aResult.iResultDes.Copy( KResult );
        aResult.iResult = KErrGeneral;
        // case was not executed or case was executed but never finished or failed 
        }
    
    ResetL();    
    return KErrNone;        
    }

TInt COomTestHarness::PluginTestInsufficientMemoryFreedL(TTestResult& aResult)
    {
    ResetL();
    
    // Configure the plugins to eat 1kB each:
    for (TInt pluginIndex = KUidOOMDummyPluginFirstImplementation; pluginIndex <= KUidOOMDummyPluginLastImplementation - 2; pluginIndex++)
        {
        User::LeaveIfError(iAllocServer.Configure(TUid::Uid(pluginIndex), 0, 0x400, 0x400));
        }
    
    //start watchers
    TInt err = KErrNone;
    TInt bytesAvailable;
    
    // Go significantly under the low memory threshold
    EatMemoryL(KOomJustAboveGoodMemoryThreshold);

    // Request 5 MB of data, using the priority of the referenced plugin (constant priority 8)
    err = iOomSession.RequestOptionalRam(5 * 1024 * 1024, 5 * 1024 * 1024, 0x10286A37, bytesAvailable);
   
    // There should be nothing that can be done to free this memory
    if (err == KErrNoMemory)
        {
        err = KErrNone;
        }
    else    
        {
        _LIT( KResult ,"There should not have been actions available to successfully complete request");
        aResult.iResultDes.Copy( KResult );
        aResult.iResult = KErrGeneral;
        // Case was executed but failed
        }
    
    //Check a couple of plugins that should have been called
    if (err == KErrNone)
        {   
        err = PluginFreeRamCalledL(0x10286A36, ETrue);
        if (err != KErrNone)
            {
            _LIT( KResult ,"Plugin 0x10286A36 FreeRam not called");
            aResult.iResultDes.Copy( KResult );
            aResult.iResult = KErrGeneral;
            // Case was executed but failed
            }
        }
    if (err == KErrNone)
        {   
        err = PluginFreeRamCalledL(0x10286A3B, ETrue);
        if (err != KErrNone)
            {
            _LIT( KResult ,"Plugin 0x10286A3B FreeRam not called");
            aResult.iResultDes.Copy( KResult );
            aResult.iResult = KErrGeneral;
            // Case was executed but failed
            }
        }
    
    // This plugin should not be called as it is priority 9
    if (err == KErrNone)
        {   
        err = PluginFreeRamCalledL(0x10286A3D, EFalse);
        if (err != KErrNone)
            {
            _LIT( KResult ,"Plugin 0x10286A3D FreeRam incorrectly called");
            aResult.iResultDes.Copy( KResult );
            aResult.iResult = KErrGeneral;
            // Case was executed but failed
            }
        }   
    //This plugins should not be called as the target app is not running
    if (err == KErrNone)
        {   
        err = PluginFreeRamCalledL(0x10286A3A, EFalse);  
        if (err != KErrNone)
            {
            _LIT( KResult ,"Plugin 0x10286A3A FreeRam incorrectly called");
            aResult.iResultDes.Copy( KResult );
            aResult.iResult = KErrGeneral;
            // Case was executed but failed
            }
        }

    // MemoryGood should have been called on all plugins which were run as the request to 
    // go below memory was triggered by an optional RAM request
    if (err == KErrNone)
        {                   
        err = PluginMemoryGoodCalledL(0x10286A3B, ETrue);
        if (err != KErrNone)
            {
            _LIT( KResult ,"plugin  0x10286A3B MemoryGood not called");
            aResult.iResultDes.Copy( KResult );
            aResult.iResult = KErrGeneral;
            // Case was executed but failed
            }
        }
        
    if (err == KErrNone)
        {                   
        err = PluginMemoryGoodCalledL(0x10286A3C, ETrue);
        if (err != KErrNone)
            {
            _LIT( KResult ,"plugin  0x10286A3C MemoryGood not called");
            aResult.iResultDes.Copy( KResult );
            aResult.iResult = KErrGeneral;
            // Case was executed but failed
            }
        }
    
    if (err == KErrNone)
        {                   
        // MemoryGood should not be called on this plugin because FreeMemory was never called on it
        err = PluginMemoryGoodCalledL(0x10286A37, EFalse);
        if (err != KErrNone)
            {
            _LIT( KResult ,"plugin  0x10286A37 MemoryGood incorrectly called");
            aResult.iResultDes.Copy( KResult );
            aResult.iResult = KErrGeneral;
            // Case was executed but failed
            }
        }
    
    if (err == KErrNone)
        {
        _LIT( KDescription , "Test case passed");
        aResult.SetResult( KErrNone, KDescription );
        }

    ResetL();    
    return KErrNone;        
    }

TInt COomTestHarness::PluginTestInsufficientMemoryFreed2L(TTestResult& aResult)
    {
    ResetL();
    
    // Configure the plugins to eat 1kB each:
    for (TInt pluginIndex = KUidOOMDummyPluginFirstImplementation; pluginIndex <= KUidOOMDummyPluginLastImplementation - 2; pluginIndex++)
        {
        User::LeaveIfError(iAllocServer.Configure(TUid::Uid(pluginIndex), 0, 0x400, 0x400));
        }
    
    //start watchers
    TInt err = KErrNone;
    TInt bytesAvailable;
    
    // Go significantly under the low memory threshold
    EatMemoryL(KOomBetweenLowAndGoodThresholds);

    // Request 5 MB of data, using the priority of the referenced plugin (constant priority 8)
    err = iOomSession.RequestOptionalRam(5 * 1024 * 1024, 5 * 1024 * 1024, 0x10286A37, bytesAvailable);
   
    // There should be nothing that can be done to free this memory
    if (err == KErrNoMemory)
        {
        err = KErrNone;
        }
    else    
        {
        _LIT( KResult ,"There should not have been actions available to successfully complete request");
        aResult.iResultDes.Copy( KResult );
        aResult.iResult = KErrGeneral;
        // Case was executed but failed
        }
    
    //Check a couple of plugins that should have been called
    if (err == KErrNone)
        {   
        err = PluginFreeRamCalledL(0x10286A36, ETrue);
        if (err != KErrNone)
            {
            _LIT( KResult ,"Plugin 0x10286A36 FreeRam not called");
            aResult.iResultDes.Copy( KResult );
            aResult.iResult = KErrGeneral;
            // Case was executed but failed
            }
        }
    if (err == KErrNone)
        {   
        err = PluginFreeRamCalledL(0x10286A3B, ETrue);
        if (err != KErrNone)
            {
            _LIT( KResult ,"Plugin 0x10286A3B FreeRam not called");
            aResult.iResultDes.Copy( KResult );
            aResult.iResult = KErrGeneral;
            // Case was executed but failed
            }
        }
    
    // This plugin should not be called as it is priority 9
    if (err == KErrNone)
        {   
        err = PluginFreeRamCalledL(0x10286A3D, EFalse);
        if (err != KErrNone)
            {
            _LIT( KResult ,"Plugin 0x10286A3D FreeRam incorrectly called");
            aResult.iResultDes.Copy( KResult );
            aResult.iResult = KErrGeneral;
            // Case was executed but failed
            }
        }   
    //This plugins should not be called as the target app is not running
    if (err == KErrNone)
        {   
        err = PluginFreeRamCalledL(0x10286A3A, EFalse);  
        if (err != KErrNone)
            {
            _LIT( KResult ,"Plugin 0x10286A3A FreeRam incorrectly called");
            aResult.iResultDes.Copy( KResult );
            aResult.iResult = KErrGeneral;
            // Case was executed but failed
            }
        }

    // MemoryGood should have been called on all plugins which were run as the request to 
    // go below memory was triggered by an optional RAM request
    if (err == KErrNone)
        {                   
        err = PluginMemoryGoodCalledL(0x10286A3B, ETrue);
        if (err != KErrNone)
            {
            _LIT( KResult ,"plugin  0x10286A3B MemoryGood not called");
            aResult.iResultDes.Copy( KResult );
            aResult.iResult = KErrGeneral;
            // Case was executed but failed
            }
        }
        
    if (err == KErrNone)
        {                   
        err = PluginMemoryGoodCalledL(0x10286A3C, ETrue);
        if (err != KErrNone)
            {
            _LIT( KResult ,"plugin  0x10286A3C MemoryGood not called");
            aResult.iResultDes.Copy( KResult );
            aResult.iResult = KErrGeneral;
            // Case was executed but failed
            }
        }
    
    if (err == KErrNone)
        {                   
        // MemoryGood should not be called on this plugin because FreeMemory was never called on it
        err = PluginMemoryGoodCalledL(0x10286A37, EFalse);
        if (err != KErrNone)
            {
            _LIT( KResult ,"plugin  0x10286A37 MemoryGood incorrectly called");
            aResult.iResultDes.Copy( KResult );
            aResult.iResult = KErrGeneral;
            // Case was executed but failed
            }
        }
    
    if (err == KErrNone)
        {
        _LIT( KDescription , "Test case passed");
        aResult.SetResult( KErrNone, KDescription );
        }

    ResetL();    
    return KErrNone;        
    }

// ================= OTHER EXPORTED FUNCTIONS =================================

// End of File

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



#ifndef T_OOMHARNESS_H
#define T_OOMHARNESS_H

// INCLUDES
#include "StifTestModule.h"
#include <StifLogger.h>
#include <e32hashtab.h>
#include <e32cmn.h>
#include "CDummyApplicationHandle.h"
#include "t_oomclient.h"
#include <oommonitorsession.h>
#include <e32property.h>

// CONSTANTS

const TInt KSettlingTime = 2000000;
const TInt KOomHarnessInitialEatenMemory= 1024; // Just eat 1K of memory initially, this can grow later
const TInt KOomHarnessMaxEatenMemory = 64 * 1024 * 1024;

const TInt KTimeout = 20000000; //20 secs

enum TTestStates
    {
    // EInit: Test initialisation phase
    ETestInit,
    
    // EPossiblePass: In the case of memorymonitor reporting EAboveTreshhold
    ETestAbove,
    
    // EFail: All other cases including timeout
    ETestBelow,
    
    ETestTimeout
    };

// MACROS
// None

// Logging path
_LIT( KT_OomHarnessLogPath, "\\logs\\testframework\\T_OomHarness\\" ); 
// Log file
_LIT( KT_OomHarnessLogFile, "T_OomHarness.txt" ); 
_LIT( KT_OomHarnessLogFileWithTitle, "T_OomHarness_[%S].txt" ); 

// Function pointer related internal definitions
// Rounding known bug in GCC
#define GETPTR &  
#define ENTRY(str,func) {_S(str), GETPTR func,0,0,0}
#define FUNCENTRY(func) {_S(#func), GETPTR func,0,0,0}
#define OOM_ENTRY(str,func,a,b,c) {_S(str), GETPTR func,a,b,c}
#define OOM_FUNCENTRY(func,a,b,c) {_S(#func), GETPTR func,a,b,c}

// FUNCTION PROTOTYPES
// None

// FORWARD DECLARATIONS
class COomTestHarness;
class CMemoryMonitorStatusWatcher;
class CMemoryMonitorTimeoutWatcher;

// DATA TYPES
// None

// A typedef for function that does the actual testing,
// function is a type 
// TInt COomTestHarness::<NameOfFunction> ( TTestResult& aResult )
typedef TInt (COomTestHarness::* TestFunction)(TTestResult&);    

// CLASS DECLARATION

/**
*  An internal structure containing a test case name and
*  the pointer to function doing the test
*
*  @lib ?library
*  @since ?Series60_version
*/
class TCaseInfoInternal
    {
    public:
        const TText* iCaseName;
        TestFunction iMethod;
        TBool           iIsOOMTest;
        TInt            iFirstMemoryAllocation;
        TInt            iLastMemoryAllocation;
    };

// CLASS DECLARATION

/**
*  A structure containing a test case name and
*  the pointer to function doing the test
*
*  @lib ?library
*  @since ?Series60_version
*/
class TCaseInfo
    {
    public:
        TPtrC iCaseName;    
        TestFunction iMethod;
        TBool           iIsOOMTest;
        TInt            iFirstMemoryAllocation;
        TInt            iLastMemoryAllocation;

    TCaseInfo( const TText* a ) : iCaseName( (TText*) a )
        {        
        };

    };

// CLASS DECLARATION

/**
*  This a T_OomHarness class.
*  ?other_description_lines
*
*  @lib ?library
*  @since ?Series60_version
*/
NONSHARABLE_CLASS(COomTestHarness) : public CTestModuleBase
    {
    public:  // Constructors and destructor


        /**
        * Two-phased constructor.
        */
        static COomTestHarness* NewL();

        /**
        * Destructor.
        */
        virtual ~COomTestHarness();

    public: // New functions
    // None

    public: // Functions from base classes

        /**
        * From CTestModuleBase InitL is used to initialize the 
        *       T_OomHarness. It is called once for every instance of 
        *       TestModule T_OomHarness after its creation.
        * @since ?Series60_version
        * @param aIniFile Initialization file for the test module (optional)
        * @param aFirstTime Flag is true when InitL is executed for first 
        *               created instance of T_OomHarness.
        * @return Symbian OS error code
        */
        TInt InitL( TFileName& aIniFile, TBool aFirstTime );

        /**
        * From CTestModuleBase GetTestCasesL is used to inquiry test cases 
        *   from T_OomHarness. 
        * @since ?Series60_version
        * @param aTestCaseFile Test case file (optional)
        * @param aTestCases  Array of TestCases returned to test framework
        * @return Symbian OS error code
        */
        TInt GetTestCasesL( const TFileName& aTestCaseFile, 
                            RPointerArray<TTestCaseInfo>& aTestCases );
    
        /**
        * From CTestModuleBase RunTestCaseL is used to run an individual 
        *   test case. 
        * @since ?Series60_version
        * @param aCaseNumber Test case number
        * @param aTestCaseFile Test case file (optional)
        * @param aResult Test case result returned to test framework (PASS/FAIL)
        * @return Symbian OS error code (test case execution error, which is 
        *           not reported in aResult parameter as test case failure).
        */   
        TInt RunTestCaseL( const TInt aCaseNumber, 
                           const TFileName& aTestCaseFile,
                           TTestResult& aResult );

        /**
        * From CTestModuleBase; OOMTestQueryL is used to specify is particular
        * test case going to be executed using OOM conditions
        * @param aTestCaseFile Test case file (optional)
        * @param aCaseNumber Test case number (optional)
        * @param aFailureType OOM failure type (optional)
        * @param aFirstMemFailure The first heap memory allocation failure value (optional)
        * @param aLastMemFailure The last heap memory allocation failure value (optional)
        * @return TBool
        */
        virtual TBool OOMTestQueryL( const TFileName& /* aTestCaseFile */, 
                                     const TInt /* aCaseNumber */, 
                                     TOOMFailureType& aFailureType,
                                     TInt& /* aFirstMemFailure */, 
                                     TInt& /* aLastMemFailure */ );

        /**
        * From CTestModuleBase; OOMTestInitializeL may be used to initialize OOM
        * test environment
        * @param aTestCaseFile Test case file (optional)
        * @param aCaseNumber Test case number (optional)
        * @return None
        */
        virtual void OOMTestInitializeL( const TFileName& /* aTestCaseFile */, 
                                    const TInt /* aCaseNumber */ ); 
        /**
        * From CTestModuleBase; OOMTestFinalizeL may be used to finalize OOM
        * test environment
        * @param aTestCaseFile Test case file (optional)
        * @param aCaseNumber Test case number (optional)
        * @return None
        */
        virtual void OOMTestFinalizeL( const TFileName& /* aTestCaseFile */, 
                                       const TInt /* aCaseNumber */ );

        /**
        * From CTestModuleBase; OOMHandleWarningL
        * @param aTestCaseFile Test case file (optional)
        * @param aCaseNumber Test case number (optional)
        * @param aFailNextValue FailNextValue for OOM test execution (optional)
        * @return None
        */
        virtual void OOMHandleWarningL( const TFileName& /* aTestCaseFile */,
                                        const TInt /* aCaseNumber */, 
                                        TInt& /* aFailNextValue */);
        void AllocateMemoryL(TInt aKBytes);
        
    protected:  // New functions    
    // None

    protected:  // Functions from base classes
    // None

    private:

        /**
        * C++ default constructor.
        */
        COomTestHarness();

        /**
        * By default Symbian 2nd phase constructor is private.
        */
        void ConstructL();

        // Prohibit copy constructor if not deriving from CBase.
        // ?classname( const ?classname& );
        // Prohibit assigment operator if not deriving from CBase.
        // ?classname& operator=( const ?classname& );

        /**
        * Function returning test case name and pointer to test case function.
        * @since ?Series60_version
        * @param aCaseNumber test case number
        * @return TCaseInfo 
        */
        const TCaseInfo Case ( const TInt aCaseNumber ) const;

        // Test setup functions...
        	
        	// Close any dummy apps
        	// Free all memory being eaten by the server
        	// Update the call counts on the plugins (add them if they're not there already)
        	void ResetL();
        	
        	void EatMemoryL(TInt aKBytesToLeaveFree);
        	        	
        	// Set up the plugins and applications - this is the starting point for many test cases
        	// 5 applications are started with approx 1 MB used for each
        	// 1MB (approx) of memory is reserved for each one of the plugins
        	void CommonSetUpL();
        	
        	void StartApplicationL(TInt32 aUid, TInt aInitialAllocationInKBs);
        	
        // Results checking functions...
        	
        	// Returns KErrNone if we get the expected result
        	TInt AppIsRunning(TInt32 aUid, TBool aExpectedResult);
        	
        	// Has FreeRam been called on this plugin since the last call to this function (or since the whole class has been reset)?
        	// Returns KErrNone if we get the expected result
        	TInt PluginFreeRamCalledL(TInt32 aUid, TBool aExpectedResult);

        	// Has FreeRam been called on this plugin since the last call to this function (or since the whole class has been reset)?
        	// Returns KErrNone if we get the expected result
        	TInt PluginMemoryGoodCalledL(TInt32 aUid, TBool aExpectedResult);        
        
        	// Utility functions
        	
        	// Wait a while for the system to settle down
        	inline void Settle();

        	void BringAppToForeground(TInt32 aUid);
        	
 
        	TInt GetFreeMemory();
        	
        	// Utility function which calls the async version of optional RAM request and makes it behave like the sync version
        	TInt RequestOptionalRamASyncWrapper(TInt aBytesRequested, TInt aMinimumBytesNeeded, TInt aPluginId, TInt& aBytesAvailable);        	
        	
        	// Utility function which starts the memory monitor property watcher.
        	// The watcher stops the active scheduler when the monitor status changes
        	void StartMemoryMonitorStatusWatcher(TInt& aTestState);
        	
        	// Utility function which starts the timeout watcher and the active scheduler 
			// The watcher stops the active scheduler when the monitor status changes
			// This function also cancels any pending requests when we are done
        	void StartTimerAndRunWatcher(TInt& aTestState);
        	
        	// The tests...
        	
        	// Test normal application closure for a single app
        	// Start three applications
        	// The lowest priority app should be in the foregound
        	// ... with the next lowest behind it
        	// ... followed by the highest priority application at the back
        	// The middle application should be configured to eat 5MB of memory
        	// A low memory event is triggered and the middle application only should be closed
        	TInt AppCloseTest1L(TTestResult& aResult);
        	
        	// Tests the idle time rule mechanism for app closure
        	// Start three applications
        	// The lowest priority app should be in the foregound
        	// ... with the next lowest behind it
        	// ... followed by the highest priority application at the back
        	// After an idle period the highest priority app is configured to become the lowest priority
        	// The middle and highest application should be configured to eat 5MB of memory
        	// A low memory event is triggered and the middle application only should be closed
        	// Note that this test should be the same as AppCloseTest1L, with the exception of the idle period which causes different apps to be closed
        	TInt AppCloseIdleTimeTest1L(TTestResult& aResult);

        	//Tests the that window group z order is considered when closing apps.
        	//Start 3 apps of equal priority, ensuring their z order is known
        	//Starts another lower priority app and puts that in the foreground
        	//A low memory event is triggered such that only one app needs to be closed
        	//The closed app should be the one furthest from the foreground in the z order
        	TInt AppCloseEqualPrioritiesL(TTestResult& aResult);        	
        	
        	// Test system plugins and continue mode
        	// Simulate a low memory event
        	// Two system plugins should free enough memory, but four will be run because they work in "continue" mode
        	TInt PluginTest1L(TTestResult& aResult);
        	
        	// Test application plugins
        	// Start two target apps
        	// Simulate a low memory event
        	// The one of the application plugins should now be run, displacing one of the system plugins
        	// The target apps are of sufficiently high priority that they will not be closed
        	TInt PluginTest2L(TTestResult& aResult);
        	
        	// Test that the aBytesRequested is correctly passed to the FreeMemory function of V2 plugins
        	// Initialise P4 plugin to consume 5MB of memory
        	// No other plugins are configured to release memory
        	// Simulate a low memory event (go just below the low threshold)
        	// Check that the P4 plugin has been called
        	// Check that the P4 plugin has received a request for > 500K and less than <1500K
        	TInt PluginV2Test1L(TTestResult& aResult);
        	
        	// Test simple prioritisation of application plugins
        	// Start two target applications
        	// Configure all plugins to consume 0.5MB
        	// Simulate a low memory event
        	// Some of the low priority app plugins with those target applications should be called
        	// The highest priority app with that target application should not be called (the lower priority plugins should free enough memory)
        	TInt AppPluginTest1L(TTestResult& aResult);
        	
        	// Test simple prioritisation of application plugins
        	// Start two target applications
        	// Configure all plugins to consume 0.5MB
        	// The app plugin with the highest priority is configured to be assigned the lowest priority after an idle time
        	// Wait until the idle time rule applies
        	// Simulate a low memory event
        	// The plugin that initially had the highest priority (but now has the lowest priority) should be called
        	// Note that this test should be the same as AppPluginTest1L with the addition of the idle period
        	TInt AppPluginIdleTimeTest1L(TTestResult& aResult);	
        	
        	// Test idle time handling for plugins
        	// Start two target apps
        	// Simulate a low memory event
        	// The one of the application plugins should now be run, displacing one of the system plugins
        	// The target apps are of sufficiently high priority that they will not be closed
        	TInt PluginIdleTimeTest2L(TTestResult& aResult);
        	
        	// Test the optional allocation mechanism
        	// Configure the plugins not to release any RAM when FreeRam is called
        	// Configure one priority 7 application to release 12MB when FreeRam is called
        	// Start this application (plus another one so it isn't in the foreground)
        	// Drop just below the good memory level
        	// Request an optional allocation of 10MB referencing a priority 8 plugin
        	// All of the plugins below P8 should be called
        	// The P7 application should be closed freeing the required memory
        	TInt OptionalAllocationTest1L(TTestResult& aResult);
        	
        	
        	// Test the optional allocation mechanism - minimum requested RAM behaviour - successful request
        	// Configure the plugins not to release any RAM when FreeRam is called
        	// Configure one priority 7 application to release 5MB when FreeRam is called
        	// Start this application (plus another one so it isn't in the foreground)
        	// Drop just above the good memory level
        	// Request an optional allocation of 10MB (5MB minimum) referencing a priority 8 plugin
        	// All of the plugins below P8 should be called
        	// The P7 application should be closed freeing the required minimum amount memory
        	// The returned available memory should be about 5MB ( > 3MB and < 7MB )
        	TInt OptionalAllocationTest2L(TTestResult& aResult);
        	
        	// Test the optional allocation mechanism - minimum requested RAM behaviour - failed request
        	// Configure the plugins not to release any RAM when FreeRam is called
        	// Configure one priority 7 application to release 3MB when FreeRam is called
        	// Start this application (plus another one so it isn't in the foreground)
        	// Drop just above the good memory level
        	// Request an optional allocation of 10MB (5MB minimum) referencing a priority 8 plugin
        	// All of the plugins below P8 should be called
        	// The P7 application should be closed but it won't free enough memory
        	// The optional allocation should fail with KErrNoMemory
        	TInt OptionalAllocationTest3L(TTestResult& aResult);
        	
        	
        	// Test that force priority check applies (only) to selected priorities
        	// Setup three plugins (priorities 7, 8 & 9)to eat 5MB each
        	// The configuration file should force a check after priority 8
        	// Drop just under the low threshold
        	// Plugins P7 & P8 should be called (P8 is called even though the P7 plugin freed enough memory)
        	// Plugin P9 should not be called because enou
        	TInt ForcePriorityCheck1L(TTestResult& aResult);
        	
        	// Test the Busy API on the OOM server
        	// Start three applications
        	// Ensure that the lowest priority app is not in the foreground
        	// Call the busy API on the OOM monitor for the lowest priority app
        	// Simulate a low memory event by going just under the low threshold
        	// The busy application should not be closed
        	// The other (non-foreground) application should be closed
        	TInt BusyApplicationTest1L(TTestResult& aResult);

        	// Test the Normal-priority API on the OOM server
        	// Start three applications
        	// Ensure that the lowest priority app is not in the foreground
        	// Call the busy API on the OOM monitor for the lowest priority app
        	// Then call the not-busy API on the OOM monitor for the lowest priority app
        	// Simulate a low memory event by going just under the low threshold
        	// The lowest priority app should be closed (because it is no longer busy)
        	TInt NormalPriorityApplicationTest1L(TTestResult& aResult);
        
        	// Test the async optional allocation mechanism
        	// Configure the plugins not to release any RAM when FreeRam is called
        	// Configure one priority 7 application to release 12MB when FreeRam is called
        	// Start this application (plus another one so it isn't in the foreground)
        	// Drop just below the good memory level
        	// Request an optional allocation of 10MB referencing a priority 8 plugin
        	// All of the plugins below P8 should be called
        	// The P7 application should be closed freeing the required memory
        	TInt OptionalAllocationAsyncTest1L(TTestResult& aResult);
        	
        	
        	// Test the async optional allocation mechanism - minimum requested RAM behaviour - successful request
        	// Configure the plugins not to release any RAM when FreeRam is called
        	// Configure one priority 7 application to release 5MB when FreeRam is called
        	// Start this application (plus another one so it isn't in the foreground)
        	// Drop just above the good memory level
        	// Request an optional allocation of 10MB (5MB minimum) referencing a priority 8 plugin
        	// All of the plugins below P8 should be called
        	// The P7 application should be closed freeing the required minimum amount memory
        	// The returned available memory should be about 5MB ( > 3MB and < 7MB )
        	TInt OptionalAllocationAsyncTest2L(TTestResult& aResult);
        	
        	// Test the async optional allocation mechanism - minimum requested RAM behaviour - failed request
        	// Configure the plugins not to release any RAM when FreeRam is called
        	// Configure one priority 7 application to release 3MB when FreeRam is called
        	// Start this application (plus another one so it isn't in the foreground)
        	// Drop just above the good memory level
        	// Request an optional allocation of 10MB (5MB minimum) referencing a priority 8 plugin
        	// All of the plugins below P8 should be called
        	// The P7 application should be closed but it won't free enough memory
        	// The optional allocation should fail with KErrNoMemory
        	TInt OptionalAllocationAsyncTest3L(TTestResult& aResult);

        	// Test that sync mode configuration is working for system plugins
        	// Configure three system plugins to release 5MB of memory each.
        	// The plugins are configured as follows
        	// Plugin 1: Priority 7, sync mode continue
        	// Plugin 2: Priority 8, sync mode check
        	// Plugin 3: Priority 9, sync mode continue
        	// Drop just under the low threshold
        	// Plugins 1 & 2 should be called (even though plugin 1 alone has freed enough RAM)
        	// Plugin 3 won't be called because the check on the priority 8 plugin discovers that enough RAM has been freed
        	TInt PluginSyncModeTest1L(TTestResult& aResult);	
        	
        	
        	// Start three applications
        	// One is set to NEVER_CLOSE, one is low priority, one is a dummy app to ensure that the first two are not in the foreground
        	// Configure applications not to release any memory
        	// Go just significantly under the low memory threshold
        	// Wait for the system to recover, if we have moved above the low memory threshold then go significantly under it again. Repeat this step until we no longer go above low.
        	// Check that the low priority application is closed
        	// Check that the NEVER_CLOSE application is not closed (even though we're still below the low theshold)
        	TInt NeverCloseTest1L(TTestResult& aResult);
        	
        	TInt AppCloseTwoSessionsL(TTestResult& aResult);

            TInt CallIfTargetAppNotRunningTest1L(TTestResult& aResult);            	
        
            TInt AppCloseSpecificThresholdTest1L(TTestResult& aResult);
            
            // test that the plugins are left in the off state if a request for optional RAM 
            // cannot be granted
            TInt PluginTestInsufficientMemoryFreedL(TTestResult& aResult);
            
            // test that the plugins are left in the off state if a request for optional RAM 
            // cannot be granted
            // The test makes the optional RAM call from a state where memory is between the 
            // global low and good thresholds
            TInt PluginTestInsufficientMemoryFreed2L(TTestResult& aResult);
            
            
    public:     // Data
    // None

    protected:  // Data
    // None

    private:    // Data
        // Pointer to test (function) to be executed
        TestFunction iMethod;

        // Pointer to logger
        CStifLogger * iLog;

        // Normal logger
        CStifLogger* iStdLog;

        // Test case logger
        CStifLogger* iTCLog;

        // Flag saying if test case title should be added to log file name
        TBool iAddTestCaseTitleToLogName;

        // ?one_line_short_description_of_data
        //?data_declaration;

        // Reserved pointer for future extension
        //TAny* iReserved;

    	struct TPluginCallCount
    		{
    		TInt iFreeRamCallCount;
    		TInt iMemoryGoodCallCount;
    		};
    	
    	RHashMap<TInt32, TPluginCallCount> iPluginCallCounts;
    	
    	RPointerArray<CCDummyApplicationHandle> iApps;
    	
    	ROOMAllocSession iAllocServer;
    	
    	RChunk iChunk;
    	TInt iChunkSize;    	

        RChunk iDummyChunk;
        TInt iDummyChunkSize;        
    	
    	ROomMonitorSession iOomSession;
    	
    	//CActiveScheduler needed by the memory monitor watchers
    	CActiveScheduler* iScheduler;
    	
    	//The watchers
    	CMemoryMonitorStatusWatcher* iStatusWatcher;
    	CMemoryMonitorTimeoutWatcher* iTimeoutWatcher;      
    	
    	
    public:     // Friend classes
    // None

    protected:  // Friend classes
    // None

    private:    // Friend classes
    // None

    };

/**
*  This CMemoryMonitorStatusWatcher class signals a client if the memorymonitor status becomes different from EFreeingMemory.
*  ?other_description_lines
*
*  @lib ?library
*  @since ?Series60_version
*/
NONSHARABLE_CLASS(CMemoryMonitorStatusWatcher) : public CActive
    {
    public:
        static CMemoryMonitorStatusWatcher* NewL();
        ~CMemoryMonitorStatusWatcher();
        void Start(TInt* aWatcherState);
    private:
        CMemoryMonitorStatusWatcher();
        void ConstructL();
        void DoCancel();
        void RunL();
    private:
        RProperty iMonitorProperty;
        TInt* iTestState;
    };

/**
*  This CMemoryMonitorTimeoutWatcher class signals a client if the memorymonitor takes too long to leave EFreeingMemory state.
*  ?other_description_lines
*
*  @lib ?library
*  @since ?Series60_version
*/
NONSHARABLE_CLASS(CMemoryMonitorTimeoutWatcher) : public CTimer
    {
public:
    static CMemoryMonitorTimeoutWatcher* NewL();
    ~CMemoryMonitorTimeoutWatcher();
    void Start(TInt* aTestState, const TTimeIntervalMicroSeconds32& aTimeout);
    
private:
    CMemoryMonitorTimeoutWatcher();
    void ConstructL();
    void DoCancel();
    void RunL();
private:
    TInt* iTestState;
    };

struct TReturnStatus
    {
    TInt iId;
    TBool iCompleted;
    TInt iReturnStatus;
    };


NONSHARABLE_CLASS(CAsyncRequester) : public CActive
    {
public:
    static CAsyncRequester* NewL(RChunk aChunk, TInt aChunkSize);
    ~CAsyncRequester();
    void Start(TInt aBytesToRequest, TReturnStatus* aReturnStatus);
    
private:
    CAsyncRequester(RChunk aChunk, TInt aChunkSize);
    void ConstructL();
    void DoCancel();
    void RunL();
private:
    ROomMonitorSession iSession;
    TReturnStatus* iReturnStatus;
    RChunk iChunk;
    TInt iChunkSize;
    };



inline void COomTestHarness::Settle()
	{
	//wait for oom system to settle
	User::After(TTimeIntervalMicroSeconds32(KSettlingTime));
	}

#endif      // T_OOMHARNESS_H

// End of File

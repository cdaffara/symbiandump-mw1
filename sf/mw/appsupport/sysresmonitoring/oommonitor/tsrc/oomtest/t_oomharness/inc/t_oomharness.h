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




#ifndef __T_OOMHARNESS_H__
#define __T_OOMHARNESS_H__

//  Include Files

#include <e32base.h>
#include <e32hashtab.h>
#include <e32cmn.h>

#include "CDummyApplicationHandle.h"
#include "t_oomclient.h"
#include <oommonitorsession.h>

const TTimeIntervalMicroSeconds32 KSettlingTime = 500000;

//  Function Prototypes

GLDEF_C TInt E32Main();

class COomTestHarness : public CBase
	{
public:
	// Constructors and destructor
	static COomTestHarness* NewLC();
	
	~COomTestHarness();
	
	void StartL();
	
private:

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

	TInt BringAppToForeground(TInt32 aUid);
	
	void EatMemoryUntilWeAreStuckUnderTheLowThresholdL();
	
// The tests...
	
	// Test normal application closure for a single app
	// Start three applications
	// The lowest priority app should be in the foregound
	// ... with the next lowest behind it
	// ... followed by the highest priority application at the back
	// The middle application should be configured to eat 5MB of memory
	// A low memory event is triggered and the middle application only should be closed
	TInt AppCloseTest1L();
	
	// Tests the idle time rule mechanism for app closure
	// Start three applications
	// The lowest priority app should be in the foregound
	// ... with the next lowest behind it
	// ... followed by the highest priority application at the back
	// After an idle period the highest priority app is configured to become the lowest priority
	// The middle and highest application should be configured to eat 5MB of memory
	// A low memory event is triggered and the middle application only should be closed
	// Note that this test should be the same as AppCloseTest1L, with the exception of the idle period which causes different apps to be closed
	TInt AppCloseIdleTimeTest1L();
	
	// Test system plugins and continue mode
	// Simulate a low memory event
	// Two system plugins should free enough memory, but four will be run because they work in "continue" mode
	TInt PluginTest1L();
	
	// Test application plugins
	// Start two target apps
	// Simulate a low memory event
	// The one of the application plugins should now be run, displacing one of the system plugins
	// The target apps are of sufficiently high priority that they will not be closed
	TInt PluginTest2L();
	
	// Test that the aBytesRequested is correctly passed to the FreeMemory function of V2 plugins
	// Initialise P4 plugin to consume 5MB of memory
	// No other plugins are configured to release memory
	// Simulate a low memory event (go just below the low threshold)
	// Check that the P4 plugin has been called
	// Check that the P4 plugin has received a request for > 500K and less than <1500K
	TInt PluginV2Test1L();
	
	// Test simple prioritisation of application plugins
	// Start two target applications
	// Configure all plugins to consume 0.5MB
	// Simulate a low memory event
	// Some of the low priority app plugins with those target applications should be called
	// The highest priority app with that target application should not be called (the lower priority plugins should free enough memory)
	TInt AppPluginTest1L();
	
	// Test simple prioritisation of application plugins
	// Start two target applications
	// Configure all plugins to consume 0.5MB
	// The app plugin with the highest priority is configured to be assigned the lowest priority after an idle time
	// Wait until the idle time rule applies
	// Simulate a low memory event
	// The plugin that initially had the highest priority (but now has the lowest priority) should be called
	// Note that this test should be the same as AppPluginTest1L with the addition of the idle period
	TInt AppPluginIdleTimeTest1L();	
	
	// Test idle time handling for plugins
	// Start two target apps
	// Simulate a low memory event
	// The one of the application plugins should now be run, displacing one of the system plugins
	// The target apps are of sufficiently high priority that they will not be closed
	TInt PluginIdleTimeTest2L();
	
	// Test the optional allocation mechanism
	// Configure the plugins not to release any RAM when FreeRam is called
	// Configure one priority 7 application to release 12MB when FreeRam is called
	// Start this application (plus another one so it isn't in the foreground)
	// Drop just below the good memory level
	// Request an optional allocation of 10MB referencing a priority 8 plugin
	// All of the plugins below P8 should be called
	// The P7 application should be closed freeing the required memory
	TInt OptionalAllocationTest1L();
	
	
	// Test the optional allocation mechanism - minimum requested RAM behaviour - successful request
	// Configure the plugins not to release any RAM when FreeRam is called
	// Configure one priority 7 application to release 5MB when FreeRam is called
	// Start this application (plus another one so it isn't in the foreground)
	// Drop just above the good memory level
	// Request an optional allocation of 10MB (5MB minimum) referencing a priority 8 plugin
	// All of the plugins below P8 should be called
	// The P7 application should be closed freeing the required minimum amount memory
	// The returned available memory should be about 5MB ( > 3MB and < 7MB )
	TInt OptionalAllocationTest2L();
	
	// Test the optional allocation mechanism - minimum requested RAM behaviour - failed request
	// Configure the plugins not to release any RAM when FreeRam is called
	// Configure one priority 7 application to release 3MB when FreeRam is called
	// Start this application (plus another one so it isn't in the foreground)
	// Drop just above the good memory level
	// Request an optional allocation of 10MB (5MB minimum) referencing a priority 8 plugin
	// All of the plugins below P8 should be called
	// The P7 application should be closed but it won't free enough memory
	// The optional allocation should fail with KErrNoMemory
	TInt OptionalAllocationTest3L();
	
	
	// Test the async optional allocation mechanism
	// Configure the plugins not to release any RAM when FreeRam is called
	// Configure one priority 7 application to release 12MB when FreeRam is called
	// Start this application (plus another one so it isn't in the foreground)
	// Drop just below the good memory level
	// Request an optional allocation of 10MB referencing a priority 8 plugin
	// All of the plugins below P8 should be called
	// The P7 application should be closed freeing the required memory
	TInt OptionalAllocationAsyncTest1L();
	
	
	// Test the async optional allocation mechanism - minimum requested RAM behaviour - successful request
	// Configure the plugins not to release any RAM when FreeRam is called
	// Configure one priority 7 application to release 5MB when FreeRam is called
	// Start this application (plus another one so it isn't in the foreground)
	// Drop just above the good memory level
	// Request an optional allocation of 10MB (5MB minimum) referencing a priority 8 plugin
	// All of the plugins below P8 should be called
	// The P7 application should be closed freeing the required minimum amount memory
	// The returned available memory should be about 5MB ( > 3MB and < 7MB )
	TInt OptionalAllocationAsyncTest2L();
	
	// Test the async optional allocation mechanism - minimum requested RAM behaviour - failed request
	// Configure the plugins not to release any RAM when FreeRam is called
	// Configure one priority 7 application to release 3MB when FreeRam is called
	// Start this application (plus another one so it isn't in the foreground)
	// Drop just above the good memory level
	// Request an optional allocation of 10MB (5MB minimum) referencing a priority 8 plugin
	// All of the plugins below P8 should be called
	// The P7 application should be closed but it won't free enough memory
	// The optional allocation should fail with KErrNoMemory
	TInt OptionalAllocationAsyncTest3L();
	
	
	// Test that force priority check applies (only) to selected priorities
	// Setup three plugins (priorities 7, 8 & 9)to eat 5MB each
	// The configuration file should force a check after priority 8
	// Drop just under the low threshold
	// Plugins P7 & P8 should be called (P8 is called even though the P7 plugin freed enough memory)
	// Plugin P9 should not be called because enou
	TInt ForcePriorityCheck1L();
	
	// Test the Busy API on the OOM server
	// Start three applications
	// Ensure that the lowest priority app is not in the foreground
	// Call the busy API on the OOM monitor for the lowest priority app
	// Simulate a low memory event by going just under the low threshold
	// The busy application should not be closed
	// The other (non-foreground) application should be closed
	TInt BusyApplicationTest1L();

	// Test the Normal-priority API on the OOM server
	// Start three applications
	// Ensure that the lowest priority app is not in the foreground
	// Call the busy API on the OOM monitor for the lowest priority app
	// Then call the not-busy API on the OOM monitor for the lowest priority app
	// Simulate a low memory event by going just under the low threshold
	// The lowest priority app should be closed (because it is no longer busy)
	TInt NormalPriorityApplicationTest1L();
	
	// Test that sync mode configuration is working for system plugins
	// Configure three system plugins to release 5MB of memory each.
	// The plugins are configured as follows
	// Plugin 1: Priority 7, sync mode continue
	// Plugin 2: Priority 8, sync mode check
	// Plugin 3: Priority 9, sync mode continue
	// Drop just under the low threshold
	// Plugins 1 & 2 should be called (even though plugin 1 alone has freed enough RAM)
	// Plugin 3 won't be called because the check on the priority 8 plugin discovers that enough RAM has been freed
	TInt PluginSyncModeTest1L();	
	
	
	// Start three applications
	// One is set to NEVER_CLOSE, one is low priority, one is a dummy app to ensure that the first two are not in the foreground
	// Configure applications not to release any memory
	// Go just significantly under the low memory threshold
	// Wait for the system to recover, if we have moved above the low memory threshold then go significantly under it again. Repeat this step until we no longer go above low.
	// Check that the low priority application is closed
	// Check that the NEVER_CLOSE application is not closed (even though we're still below the low theshold)
	TInt NeverCloseTest1L();
	
	TInt GetFreeMemory();
	
	// Utility function which calls the async version of optional RAM request and makes it behave like the sync version
	TInt RequestOptionalRamASyncWrapper(TInt aBytesRequested, TInt aMinimumBytesNeeded, TInt aPluginId, TInt& aBytesAvailable);
	
private:
	COomTestHarness();
	
	void ConstructL();

private:	
	
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
	
	ROomMonitorSession iOomSession;
	};

#endif  // __T_OOMHARNESS_H__

inline void COomTestHarness::Settle()
	{
	//wait for oom system to settle
	User::After(KSettlingTime);
	}

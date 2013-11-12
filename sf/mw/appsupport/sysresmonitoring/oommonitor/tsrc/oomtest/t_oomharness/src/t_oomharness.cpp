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




//  Include Files  

#include "t_oomharness.h"
#include "CDummyApplicationHandle.h"
#include "cleanuputils.h"
#include "t_oomdummyplugin_properties.h"
#include <e32base.h>
#include <e32std.h>
#include <e32cons.h>			// Console
#include <e32hal.h>
#include <e32property.h>
#include <e32cmn.h>
#include <hal.h>
#include "t_oomdummyplugin_properties.h"
#include "../../t_oomdummyapp/inc/t_oomdummyappmsgs.h"


//  Constants

_LIT(KTextConsoleTitle, "Console");
_LIT(KTextFailed, " failed, leave code = %d");
_LIT(KTextPressAnyKey, " [press any key]\n");

const TInt KOomLowMemoryThreshold = 4096;
const TInt KOomJustUnderLowMemoryThreshold = 4000;
const TInt KOomSignificantlyUnderLowMemoryThreshold = 2024;
const TInt KOomJustAboveGoodMemoryThreshold = 5100;
const TInt KOomHarnessInitialEatenMemory = 1024; // Just eat 1K of memory initially, this can grow later

const TInt KOomTestFirstIdlePeriod = 40000000; // 40 seconds

const TInt KOomHarnessMaxEatenMemory = 64 * 1024 * 1024;

//  Global Variables

LOCAL_D CConsoleBase* console; // write all messages to this


void COomTestHarness::StartL()
	{
	ResetL();

	TInt err = KErrNone;
	TBool testsFailed = EFalse;

	err = AppCloseTest1L();
	if (err != KErrNone)
		testsFailed = ETrue;
	
	ResetL();
	
	err = AppCloseIdleTimeTest1L();
	if (err != KErrNone)
		testsFailed = ETrue;

	ResetL();
	
	err = PluginTest1L();
	if (err != KErrNone)
		testsFailed = ETrue;

	ResetL();

	
	err = PluginTest2L();
	if (err != KErrNone)
		testsFailed = ETrue;
	
	ResetL();

	err = AppPluginTest1L();
	if (err != KErrNone)
		testsFailed = ETrue;

	ResetL();
	
	err = AppPluginIdleTimeTest1L();
	if (err != KErrNone)
		testsFailed = ETrue;

	ResetL();
	
	err = OptionalAllocationTest1L();
	if (err != KErrNone)
		testsFailed = ETrue;

	ResetL();	
	
	err = OptionalAllocationTest2L();
	if (err != KErrNone)
		testsFailed = ETrue;
	
	ResetL();	
	
	err = OptionalAllocationTest3L();
	if (err != KErrNone)
		testsFailed = ETrue;
	
	ResetL();
		
	err = PluginV2Test1L();
	if (err != KErrNone)
		testsFailed = ETrue;

	ResetL();
	
	err = ForcePriorityCheck1L();
	if (err != KErrNone)
		testsFailed = ETrue;
	
	ResetL();

	err = PluginSyncModeTest1L();
	if (err != KErrNone)
		testsFailed = ETrue;

	ResetL();

	err = NeverCloseTest1L();
	if (err != KErrNone)
		testsFailed = ETrue;

	ResetL();
	
	err = OptionalAllocationAsyncTest1L();
	if (err != KErrNone)
		testsFailed = ETrue;

	ResetL();	
	
	err = OptionalAllocationAsyncTest2L();
	if (err != KErrNone)
		testsFailed = ETrue;
	
	ResetL();	
	
	err = OptionalAllocationAsyncTest3L();
	if (err != KErrNone)
		testsFailed = ETrue;
	
	ResetL();

//	if (err == KErrNone)
//		err = BusyApplicationTest1L();

	ResetL();
		
//	if (err == KErrNone)
//		err = NormalPriorityApplicationTest1L();
	
	if (!testsFailed)
		console->Printf(_L("All tests passed\n"));
	else
		console->Printf(_L("Tests failed\n"));
	}

// Test normal application closure for a single app
// Start three applications
// The lowest priority app should be in the foregound
// ... with the next lowest behind it
// ... followed by the highest priority application at the back
// The middle and highest priority application should be configured to eat 5MB of memory
// A low memory event is triggered and the middle application only should be closed
TInt COomTestHarness::AppCloseTest1L()
	{
	// The lowest priority app should be in the foregound
	// ... with the next lowest behind it
	// ... followed by the highest priority application at the back
	StartApplicationL(0x101f8599, 5 * 1024); // P9 app
	StartApplicationL(0x10005901, 5 * 1024); // P8 app, configure it to eat 5MB
	StartApplicationL(0x10005a22, 0); // P7 app
	
	BringAppToForeground(0x10005a22);
	
	Settle();
	
	// Go significantly under the low memory threshold
	EatMemoryL(KOomSignificantlyUnderLowMemoryThreshold);
	
	Settle();
	
	TInt err = KErrNone;
	
	// The P8 application should be closed
	err = AppIsRunning(0x10005901, EFalse);
	
	// The P7 app should still be running because it was in the foreground
	if (err == KErrNone)
		err = AppIsRunning(0x10005a22, ETrue);	
	
	// The P9 app should still be running because the P8 application freed enough memory
	if (err == KErrNone)
		err = AppIsRunning(0x101f8599, ETrue);	

	if (err == KErrNone)
		console->Printf(_L("App Close Test 1 passed\n"));
	else
		console->Printf(_L("App Close Test 1 failed\n"));	
	
	return err;
	
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
TInt COomTestHarness::AppCloseIdleTimeTest1L()
	{
	// The lowest priority app should be in the foregound
	// ... with the next lowest behind it
	// ... followed by the highest priority application at the back
	StartApplicationL(0x101f8599, 5 * 1024); // P9 app (which becomes a P2 app after the idle time)
	StartApplicationL(0x10005901, 5 * 1024); // P8 app, configure it to eat 5MB
	StartApplicationL(0x10005a22, 0); // P7 app
	
	BringAppToForeground(0x10005a22);
	
	Settle();
	
	// Wait for the first set of idle time rules to apply
	User::After(KOomTestFirstIdlePeriod);
	
	// Go significantly under the low memory threshold
	EatMemoryL(KOomSignificantlyUnderLowMemoryThreshold);
	
	Settle();
	
	// The first two system plugins should have been closed, releasing the required memory
	// The following two system plugins won't be called (the app plugins will now take their place)
	TInt err = KErrNone;
	
	// The P9 app should have become a P2 app after the idle period, therefore it should have been the first candidate for closure
	if (err == KErrNone)
		err = AppIsRunning(0x101f8599, EFalse);		
	
	// The P8 application should still be running because the P9 app (that has become a P2 app) has freed the required memory
	err = AppIsRunning(0x10005901, ETrue);
	
	// The P7 app should still be running because it was in the foreground
	if (err == KErrNone)
		err = AppIsRunning(0x10005a22, ETrue);	
	
	if (err == KErrNone)
		console->Printf(_L("App Close Idle Time Test 1 passed\n"));
	else
		console->Printf(_L("App Close Idle Time Test 1 failed\n"));	
	
	return err;
	
	}



TInt COomTestHarness::PluginTest1L()
	{
	// Configure the plugins to eat 0.5MB each:
	for (TInt pluginIndex = KUidOOMDummyPluginFirstImplementation; pluginIndex <= KUidOOMDummyPluginLastImplementation - 2; pluginIndex++)
		{
		User::LeaveIfError(iAllocServer.Configure(TUid::Uid(pluginIndex), 0, 0x80000, 0x80000));
		}
	
	Settle();
	
	// Go just under the low memory threshold
	EatMemoryL(KOomJustUnderLowMemoryThreshold);
	
	Settle();
	
	// The first four system plugins should have been run, releasing the required memory
	// All four plugins should be called, even though the first two will release enough memory (this is because
	// plugins are always run in continue mode)
	TInt err = KErrNone;
	
	err = PluginFreeRamCalledL(KUidOOMDummyPluginFirstImplementation, ETrue);
	
	if (err == KErrNone)
		err = PluginFreeRamCalledL(KUidOOMDummyPluginFirstImplementation + 1, ETrue);
	
	if (err == KErrNone)
		err = PluginFreeRamCalledL(KUidOOMDummyPluginFirstImplementation + 2, ETrue);
	
	if (err == KErrNone)
		err = PluginFreeRamCalledL(0x10286A3C, ETrue);
	
	
	// Finally check that the plugins have been notified of the final good memory state
	if (err == KErrNone)
		err = PluginMemoryGoodCalledL(KUidOOMDummyPluginFirstImplementation, ETrue);
	
	if (err == KErrNone)
		err = PluginMemoryGoodCalledL(KUidOOMDummyPluginFirstImplementation + 1, ETrue);
	
	if (err == KErrNone)
		err = PluginMemoryGoodCalledL(KUidOOMDummyPluginFirstImplementation + 2, ETrue);
	
	if (err == KErrNone)
		err = PluginMemoryGoodCalledL(0x10286A3C, ETrue);

	if (err == KErrNone)
		console->Printf(_L("Plugin Test 1 passed\n"));
	else
		console->Printf(_L("Plugin Test 1 failed\n"));	
	
	return err;
	}


// Test application plugins
// Start two target apps
// Simulate a low memory event
// The one of the application plugins should now be run, displacing one of the system plugins
// The target apps are of sufficiently high priority that they will not be closed
TInt COomTestHarness::PluginTest2L()
	{
	// Configure the plugins to eat 0.5MB each:
	for (TInt pluginIndex = KUidOOMDummyPluginFirstImplementation; pluginIndex <= KUidOOMDummyPluginLastImplementation - 2; pluginIndex++)
		{
		User::LeaveIfError(iAllocServer.Configure(TUid::Uid(pluginIndex), 0, 0x80000, 0x80000));
		}
	
	// Start the two target applications (plus a third so that the target apps are not in the foreground)
	StartApplicationL(0x10005901, 0);
	StartApplicationL(0x10005a22, 0);
	StartApplicationL(0x101f8599, 0);
	
	BringAppToForeground(0x101f8599);
	
	Settle();
	
	// Go just under the low memory threshold
	EatMemoryL(KOomJustUnderLowMemoryThreshold);
	
	Settle();
	
	// The first two system plugins should have been closed, releasing the required memory
	// The following two system plugins won't be called (the app plugins will now take their place)
	TInt err = KErrNone;
	
	// The following plugins should be called...
	// Application plugins: 10286A3A 10286A3B
	// System plugins: 10286A3C(v2 plugin) 10286A34
	
	if (err == KErrNone)
		err = PluginFreeRamCalledL(0x10286A3B, ETrue);

	if (err == KErrNone)
		err = PluginFreeRamCalledL(0x10286A3A, ETrue);

	// Plugins and apps with higher priorities will not be called/closed because 0x10286A3A is configured to check memory before running anything else
	if (err == KErrNone)
		err = PluginFreeRamCalledL(0x10286A3C, EFalse);
	
	if (err == KErrNone)
		err = PluginFreeRamCalledL(0x10286A34, EFalse);

	if (err == KErrNone)
		err = PluginFreeRamCalledL(0x10286A35, EFalse);	
	
	if (err == KErrNone)
		err = AppIsRunning(0x10005a22, ETrue);		
	
	if (err == KErrNone)
		err = AppIsRunning(0x10005901, ETrue);
		
	if (err == KErrNone)
		err = AppIsRunning(0x101f8599, ETrue);		
	
	
	if (err == KErrNone)
		console->Printf(_L("Plugin Test 2 passed\n"));
	else
		console->Printf(_L("Plugin Test 2 failed\n"));	
	
	return err;
	
	}



// Test that the aBytesRequested is correctly passed to the FreeMemory function of V2 plugins
// Initialise P4 plugin to consume 5MB of memory
// No other plugins are configured to release memory
// Simulate a low memory event (go just below the low threshold)
// Check that the P4 plugin has been called
// Check that the P4 plugin has received a request for > 500K and less than <1500K
TInt COomTestHarness::PluginV2Test1L()
	{
	// Configure the P4 V2 plugin to eat 5MB:
	User::LeaveIfError(iAllocServer.Configure(TUid::Uid(0x10286A3C), 0, 0x500000, 0x500000));
	
	Settle();
	
	// Go just under the low memory threshold
	EatMemoryL(KOomJustUnderLowMemoryThreshold);
	
	Settle();
	
	// The first four system plugins should have been closed, releasing the required memory
	// All four plugins should be called, even though the first two will release enough memory (this is because
	// plugins are always run in continue mode)
	TInt err = KErrNone;
	
	// Check that the P4 plugin has been called
	err = PluginFreeRamCalledL(0x10286A3C, ETrue);
	
	// Check that the request for memory was about right
	// Note: regular system variation makes it impossible to test for an exact number
	TInt requestedMemory = 0;
	err = RProperty::Get(KUidOomPropertyCategory, 0x10286A3C + KOOMDummyPluginBytesRequested, requestedMemory);
	if ((requestedMemory < 512 * 1024)
			|| (requestedMemory > 1500 * 1024))
		err = KErrGeneral;
	
	// Check that the higher priority V2 plugin has not been called
	if (err == KErrNone)
		err = PluginFreeRamCalledL(0x10286A3D, EFalse);
	
	if (err == KErrNone)
		console->Printf(_L("Plugin V2 Test 1 passed\n"));
	else
		console->Printf(_L("Plugin V2 Test 1 failed\n"));	
	
	return err;
	}


// Test simple prioritisation of application plugins
// Start two target applications
// Configure all plugins to consume 0.5MB
// Simulate a low memory event
// Some of the low priority app plugins with those target applications should be called
// The highest priority app with that target application should not be called (the lower priority plugins should free enough memory)
TInt COomTestHarness::AppPluginTest1L()
	{
	// Configure the plugins to eat 0.5MB each:
	for (TInt pluginIndex = KUidOOMDummyPluginFirstImplementation; pluginIndex <= KUidOOMDummyPluginLastImplementation - 2; pluginIndex++)
		{
		User::LeaveIfError(iAllocServer.Configure(TUid::Uid(pluginIndex), 0, 0x80000, 0x80000));
		}
	
	// Start the two target applications (plus a third so that the target apps are not in the foreground)
	StartApplicationL(0x10005901, 0);
	StartApplicationL(0x10005a22, 0);
	StartApplicationL(0x101f8599, 0);
	
	TInt err = KErrNone;
	
	// Check that all of the apps are running
	if (err == KErrNone)
		err = AppIsRunning(0x10005901, ETrue);

	if (err == KErrNone)
		err = AppIsRunning(0x10005a22, ETrue);

	if (err == KErrNone)
		err = AppIsRunning(0x101f8599, ETrue);

	if (err == KErrNone)
		console->Printf(_L("Apps started\n"));
	else
		console->Printf(_L("Apps not started\n"));
	

	
	BringAppToForeground(0x101f8599);
	
	Settle();
	
	// Go just under the low memory threshold
	EatMemoryL(KOomJustUnderLowMemoryThreshold);
	
	Settle();
	
	// The following application plugins should be called...
	// Application plugins: 10286A3A 10286A3B
	
	if (err == KErrNone)
		err = PluginFreeRamCalledL(0x10286A3A, ETrue);
	
	if (err == KErrNone)
		err = PluginFreeRamCalledL(0x10286A3B, ETrue);

	// The following plugin should not be called because other plugins (including some unchecked system plugins) have freed enough memory
	// 10286A38

	if (err == KErrNone)
		err = PluginFreeRamCalledL(0x10286A38, EFalse);	
	
	if (err == KErrNone)
		console->Printf(_L("App Plugin Test 1 passed\n"));
	else
		console->Printf(_L("App Plugin Test 1 failed\n"));	
	
	return err;

	}

// Test simple prioritisation of application plugins
// Start two target applications
// Configure all plugins to consume 0.5MB
// The app plugin with the highest priority is configured to be assigned the lowest priority after an idle time
// Wait until the idle time rule applies
// Simulate a low memory event
// The plugin that initially had the highest priority (but now has the lowest priority) should be called
// Note that this test should be the same as AppPluginTest1L with the addition of the idle period
TInt COomTestHarness::AppPluginIdleTimeTest1L()
	{
	// Configure the plugins to eat 0.5MB each:
	for (TInt pluginIndex = KUidOOMDummyPluginFirstImplementation; pluginIndex <= KUidOOMDummyPluginLastImplementation - 2; pluginIndex++)
		{
		User::LeaveIfError(iAllocServer.Configure(TUid::Uid(pluginIndex), 0, 0x80000, 0x80000));
		}
	
	// Start the two target applications (plus a third so that the target apps are not in the foreground)
	StartApplicationL(0x10005901, 0);
	StartApplicationL(0x10005a22, 0);
	StartApplicationL(0x101f8599, 0);
	
	BringAppToForeground(0x101f8599);
	
	TInt err = KErrNone;	
	
	Settle();
	
	User::After(KOomTestFirstIdlePeriod);
	
	// Go just under the low memory threshold
	EatMemoryL(KOomJustUnderLowMemoryThreshold);
	
	Settle();
	
	// The following application plugins should be called...
	// Application plugins: 10286A3A 10286A3B
	
	if (err == KErrNone)
		err = PluginFreeRamCalledL(0x10286A3A, ETrue);
	
	if (err == KErrNone)
		err = PluginFreeRamCalledL(0x10286A3B, ETrue);

	// The following plugin should also be called (its priority was initially too high but has been reduced after the idle time)
	// 10286A38

	if (err == KErrNone)
		err = PluginFreeRamCalledL(0x10286A38, ETrue);	
	
	if (err == KErrNone)
		console->Printf(_L("Plugin Idle Time Test 1 passed\n"));
	else
		console->Printf(_L("Plugin Idle Time Test 1 failed\n"));	
	
	return err;

	}


// Test the optional allocation mechanism
// Configure the plugins not to release any RAM when FreeRam is called
// Configure one priority 7 application to release 12MB when FreeRam is called
// Start this application (plus another one so it isn't in the foreground)
// Drop just above the good memory level
// Request an optional allocation of 10MB referencing a priority 8 plugin
// All of the plugins below P8 should be called
// The P7 application should be closed freeing the required memory
TInt COomTestHarness::OptionalAllocationTest1L()
	{
	// Start an application (plus a second so that the first app is not in the foreground)
	// The first application is set to consume 12MB of RAM
	StartApplicationL(0x10005A22, 12 * 1024);
	StartApplicationL(0x101F8599, 0);
	
	BringAppToForeground(0x101F8599);
	
	Settle();
	
	// Go just above the good memory threshold
	EatMemoryL(KOomJustAboveGoodMemoryThreshold);
	
	Settle();
	
	// Request 10 MB of data, using the priority of the referenced plugin (constant priority 8)
	TInt bytesAvailable;
	TInt err = iOomSession.RequestOptionalRam(10 * 1024 * 1024, 10 * 1024 * 1024, 0x10286A37, bytesAvailable);
	
	// Check that all system plugins below P8 have been called
	
	if (err == KErrNone)
		err = PluginFreeRamCalledL(0x10286A34, ETrue);
	
	if (err == KErrNone)
		err = PluginFreeRamCalledL(0x10286A35, ETrue);

	if (err == KErrNone)
		err = PluginFreeRamCalledL(0x10286A36, ETrue);
	
	// This the P8 system plugin has not been called
	if (err == KErrNone)
		err = PluginFreeRamCalledL(0x10286A37, EFalse);
	
	// Check that the low priority app plugin has been called
	if (err == KErrNone)
		err = PluginFreeRamCalledL(0x10286A3B, ETrue);

	// Check that the P7 application has been closed
	if (err == KErrNone)
		err = AppIsRunning(0x10005A22, EFalse);

	if (err == KErrNone)
		console->Printf(_L("Optional Allocation Test 1 passed\n"));
	else
		console->Printf(_L("Optional Allocation Test 1 failed\n"));	
		
	
	return err;
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
TInt COomTestHarness::OptionalAllocationTest2L()
	{
	// Start an application (plus a second so that the first app is not in the foreground)
	// The first application is set to consume 5MB of RAM
	StartApplicationL(0x10005A22, 5 * 1024);
	StartApplicationL(0x101F8599, 0);
	
	BringAppToForeground(0x101F8599);
	
	Settle();
	
	// Go just above the good memory threshold
	EatMemoryL(KOomJustAboveGoodMemoryThreshold);
	
	Settle();
	
	// Request 10 MB of data, using the priority of the referenced plugin (constant priority 8)
	// Say that 5MB is the minimum we need
	TInt bytesAvailable;
	TInt err = iOomSession.RequestOptionalRam(10 * 1024 * 1024, 5 * 1024 * 1024, 0x10286A37, bytesAvailable);
	
	if (err == KErrNone)
		{
		// Check that the reported bytes available is > 3MB and < 7MB
		if ((bytesAvailable < 3 * 1024 * 1024)
				|| (bytesAvailable > 7 * 1024 * 1024))
			{
			err = KErrGeneral;
			}
		}
	
	// Check that all system plugins below P8 have been called
	
	if (err == KErrNone)
		err = PluginFreeRamCalledL(0x10286A34, ETrue);
	
	if (err == KErrNone)
		err = PluginFreeRamCalledL(0x10286A35, ETrue);
	
	if (err == KErrNone)
		err = PluginFreeRamCalledL(0x10286A36, ETrue);
	
	// This the P8 system plugin has not been called
	if (err == KErrNone)
		err = PluginFreeRamCalledL(0x10286A37, EFalse);
	
	// Check that the low priority app plugin has been called
	if (err == KErrNone)
		err = PluginFreeRamCalledL(0x10286A3B, ETrue);
	
	// Check that the P7 application has been closed
	if (err == KErrNone)
		err = AppIsRunning(0x10005A22, EFalse);

	if (err == KErrNone)
		console->Printf(_L("Optional Allocation Test 2 passed\n"));
	else
		console->Printf(_L("Optional Allocation Test 2 failed\n"));	
		
	
	return err;
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
TInt COomTestHarness::OptionalAllocationTest3L()
	{
	// Start an application (plus a second so that the first app is not in the foreground)
	// The first application is set to consume 5MB of RAM
	StartApplicationL(0x10005A22, 3 * 1024);
	StartApplicationL(0x101F8599, 0);
	
	BringAppToForeground(0x101F8599);
	
	Settle();
	
	// Go just above the good memory threshold
	EatMemoryL(KOomJustAboveGoodMemoryThreshold);
	
	Settle();
	
	// Request 10 MB of data, using the priority of the referenced plugin (constant priority 8)
	// Say that 5MB is the minimum we need
	TInt bytesAvailable;
	TInt err = iOomSession.RequestOptionalRam(10 * 1024 * 1024, 5 * 1024 * 1024, 0x10286A37, bytesAvailable);
	
	// We expect an error because it has not been possible to free the minimum memory
	if (err == KErrNoMemory)
		{
		err = KErrNone;
		}
	else
		{
		err = KErrGeneral;
		}
	
	// Check that all system plugins below P8 have been called
	
	if (err == KErrNone)
		err = PluginFreeRamCalledL(0x10286A34, ETrue);
	
	if (err == KErrNone)
		err = PluginFreeRamCalledL(0x10286A35, ETrue);
	
	if (err == KErrNone)
		err = PluginFreeRamCalledL(0x10286A36, ETrue);
	
	// This the P8 system plugin has not been called
	if (err == KErrNone)
		err = PluginFreeRamCalledL(0x10286A37, EFalse);
	
	// Check that the low priority app plugin has been called
	if (err == KErrNone)
		err = PluginFreeRamCalledL(0x10286A3B, ETrue);
	
	// Check that the P7 application has been closed
	if (err == KErrNone)
		err = AppIsRunning(0x10005A22, EFalse);

	if (err == KErrNone)
		console->Printf(_L("Optional Allocation Test 3 passed\n"));
	else
		console->Printf(_L("Optional Allocation Test 3 failed\n"));	
		
	
	return err;
	}


// Test that force priority check applies (only) to selected priorities
// Setup three plugins (priorities 7, 8 & 9)to eat 5MB each
// The configuration file should force a check after priority 8
// Drop just under the low threshold
// Plugins P7 & P8 should be called (P8 is called even though the P7 plugin freed enough memory)
// Plugin P9 should not be called because enou
TInt COomTestHarness::ForcePriorityCheck1L()
	{
	// Configure the P7, P8 and P9 plugins to eat 5MB each:
	User::LeaveIfError(iAllocServer.Configure(TUid::Uid(0x10286A36), 0, 0x500000, 0x500000)); // P7
	User::LeaveIfError(iAllocServer.Configure(TUid::Uid(0x10286A37), 0, 0x500000, 0x500000)); // P8
	User::LeaveIfError(iAllocServer.Configure(TUid::Uid(0x10286A3D), 0, 0x500000, 0x500000)); // P9
	
	Settle();
	
	// Go just under the low memory threshold
	EatMemoryL(KOomJustUnderLowMemoryThreshold);
	
	Settle();
	
	TInt err = KErrNone;
	
	// The P7 plugin should have been called and is the first to release RAM
	if (err == KErrNone)
		err = PluginFreeRamCalledL(0x10286A36, ETrue);
	
	// The P8 plugin should be called even though the P7 plugin has already released enough memory because the plugin runs in continue mode and there is no forced check
	if (err == KErrNone)
		err = PluginFreeRamCalledL(0x10286A37, ETrue);

	// The P9 plugin should not be called because of the force priority check (the P7 & P8 plugins have already released plenty of RAM)
	if (err == KErrNone)
		err = PluginFreeRamCalledL(0x10286A3D, EFalse);	
	
	if (err == KErrNone)
		console->Printf(_L("Force Prioirty Check Test 1 passed\n"));
	else
		console->Printf(_L("Force Prioirty Check Test 1 failed\n"));	
	
	return err;
	
	}



// Test the Busy API on the OOM server
// Start three applications
// Ensure that the lowest priority app is not in the foreground
// Call the busy API on the OOM monitor for the lowest priority app
// Simulate a low memory event by going just under the low threshold
// The busy application should not be closed
// The other (non-foreground) application should be closed
TInt COomTestHarness::BusyApplicationTest1L()
	{
	// Start the two target applications (plus a third so that the target apps are not in the foreground)
	StartApplicationL(0x10005901, 0);
	StartApplicationL(0x101f8599, 0);
	StartApplicationL(0x10005a22, 0); // Lowest priority app
	
	BringAppToForeground(0x101f8599); // TODO: this doesn't seem to be working - message not getting through to the dummy application
	
	Settle();
	
	// Send the second app a message to make itself busy
	TInt id = iApps.Find<TUid>(TUid::Uid(0x10005a22), CCDummyApplicationHandle::CompareTo);
	User::LeaveIfError(id);
	iApps[id]->SendMessage(KOomDummyAppSetBusy);
	
	Settle();
	
	// Go just under the low memory threshold
	EatMemoryL(KOomJustUnderLowMemoryThreshold);
	
	Settle();
	
	// The first four system plugins should have been closed, releasing the required memory
	// All four plugins should be called, even though the first two will release enough memory (this is because
	// plugins are always run in continue mode)
	TInt err = KErrNone;
	
	// Check that the busy application is still running
	if (err == KErrNone)
		err = AppIsRunning(0x10005a22, ETrue);

	// Check that the higher priority but non-busy application has been close
	if (err == KErrNone)
		err = AppIsRunning(0x10005901, EFalse);

	if (err == KErrNone)
		console->Printf(_L("Busy Application Test 1 passed\n"));
	else
		console->Printf(_L("Busy Application Test 1 failed\n"));	
	
	return err;
	}


// Test the NotBusy API on the OOM server
// Start three applications
// Ensure that the lowest priority app is not in the foreground
// Call the busy API on the OOM monitor for the lowest priority app
// Then call the not-busy API on the OOM monitor for the lowest priority app
// Simulate a low memory event by going just under the low threshold
// The lowest priority app should be closed (because it is no longer busy)
TInt COomTestHarness::NormalPriorityApplicationTest1L()
	{
	// Start the two target applications (plus a third so that the target apps are not in the foreground)
	StartApplicationL(0x10005901, 0);
	StartApplicationL(0x10005a22, 0); // Lowest priority app
	StartApplicationL(0x101f8599, 0);
	
	BringAppToForeground(0x101f8599);
	
	Settle();
	
	// Send the second app a message to make itself busy
	TInt id = iApps.Find<TUid>(TUid::Uid(0x10005a22), CCDummyApplicationHandle::CompareTo);
	User::LeaveIfError(id);
	// TODO: temp removed - needs to be put back
//	iApps[id]->SendMessage(KOomDummyAppSetBusy);
	
	// Send the second app a message to make itself normal priority
	id = iApps.Find<TUid>(TUid::Uid(0x10005a22), CCDummyApplicationHandle::CompareTo);
	User::LeaveIfError(id);
	iApps[id]->SendMessage(KOomDummyAppSetNormalPriority);


	// Go just under the low memory threshold
	EatMemoryL(KOomJustUnderLowMemoryThreshold);
	
	Settle();
	
	// The first four system plugins should have been closed, releasing the required memory
	// All four plugins should be called, even though the first two will release enough memory (this is because
	// plugins are always run in continue mode)
	TInt err = KErrNone;
	
	// Check that the no-longer-busy application has been closed
	if (err == KErrNone)
		err = AppIsRunning(0x10005a22, EFalse);

	if (err == KErrNone)
		console->Printf(_L("Normal Priority Application Test 1 passed\n"));
	else
		console->Printf(_L("Normal Priority  Application Test 2 failed\n"));	
	
	return err;
	}



// Start three applications
// One is set to NEVER_CLOSE, one is low priority, one is a dummy app to ensure that the first two are not in the foreground
// Configure applications not to release any memory
// Go just significantly under the low memory threshold
// Wait for the system to recover, if we have moved above the low memory threshold then go significantly under it again. Repeat this step until we no longer go above low.
// Check that the low priority application is closed
// Check that the NEVER_CLOSE application is not closed (even though we're still below the low theshold)
TInt COomTestHarness::NeverCloseTest1L()
	{
	// Start the two target applications (plus a third so that the target apps are not in the foreground)
	StartApplicationL(0x10005a22, 0); // Lowest priority app
	StartApplicationL(0x10008d39, 0); // NEVER_CLOSE application
	StartApplicationL(0x101f8599, 0);
	
	BringAppToForeground(0x101f8599);
	
	Settle();
	
	EatMemoryUntilWeAreStuckUnderTheLowThresholdL();
	
	Settle();
	
	// The first four system plugins should have been closed, releasing the required memory
	// All four plugins should be called, even though the first two will release enough memory (this is because
	// plugins are always run in continue mode)
	TInt err = KErrNone;
	
	// The NEVER_CLOSE app should still be running (even though we are still below the low threshold)
	if (err == KErrNone)
		err = AppIsRunning(0x10008d39, ETrue);

	// The low priority app should still be closed (even though we are still below the low threshold)
	if (err == KErrNone)
		err = AppIsRunning(0x10005a22, EFalse);

	if (err == KErrNone)
		console->Printf(_L("Never Close Test 1 passed\n"));
	else
		console->Printf(_L("Never Close Test 2 failed\n"));	
	
	return err;
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
TInt COomTestHarness::PluginSyncModeTest1L()
	{
	// Configure three plugins to eat 5MB each:
	User::LeaveIfError(iAllocServer.Configure(TUid::Uid(0x10286A36), 0, 0x500000, 0x500000));
	User::LeaveIfError(iAllocServer.Configure(TUid::Uid(0x10286A37), 0, 0x500000, 0x500000));
	User::LeaveIfError(iAllocServer.Configure(TUid::Uid(0x10286A3D), 0, 0x500000, 0x500000));
	
	Settle();
	
	// Go just under the low memory threshold
	EatMemoryL(KOomJustUnderLowMemoryThreshold);
	
	Settle();
	
	// The first four system plugins should have been run, releasing the required memory
	// All four plugins should be called, even though the first two will release enough memory (this is because
	// plugins are always run in continue mode)
	TInt err = KErrNone;
	
	// Check that the first two plugins have been called, but not the third
	err = PluginFreeRamCalledL(0x10286A36, ETrue);
	
	if (err == KErrNone)
		err = PluginFreeRamCalledL(0x10286A37, ETrue);
	
	if (err == KErrNone)
		err = PluginFreeRamCalledL(0x10286A3D, EFalse);
	
	if (err == KErrNone)
		console->Printf(_L("Plugin Sync Mode Test 1 passed\n"));
	else
		console->Printf(_L("Plugin Sync Mode Test 1 failed\n"));	
	
	return err;
	}




// Test the optional allocation mechanism
// Configure the plugins not to release any RAM when FreeRam is called
// Configure one priority 7 application to release 12MB when FreeRam is called
// Start this application (plus another one so it isn't in the foreground)
// Drop just above the good memory level
// Request an optional allocation of 10MB referencing a priority 8 plugin
// All of the plugins below P8 should be called
// The P7 application should be closed freeing the required memory
TInt COomTestHarness::OptionalAllocationAsyncTest1L()
	{
	// Start an application (plus a second so that the first app is not in the foreground)
	// The first application is set to consume 12MB of RAM
	StartApplicationL(0x10005A22, 12 * 1024);
	StartApplicationL(0x101F8599, 0);
	
	BringAppToForeground(0x101F8599);
	
	Settle();
	
	// Go just above the good memory threshold
	EatMemoryL(KOomJustAboveGoodMemoryThreshold);
	
	Settle();
	
	// Request 10 MB of data, using the priority of the referenced plugin (constant priority 8)
	TInt bytesAvailable;
	TInt err = RequestOptionalRamASyncWrapper(10 * 1024 * 1024, 10 * 1024 * 1024, 0x10286A37, bytesAvailable);
	
	// Check that all system plugins below P8 have been called
	
	if (err == KErrNone)
		err = PluginFreeRamCalledL(0x10286A34, ETrue);
	
	if (err == KErrNone)
		err = PluginFreeRamCalledL(0x10286A35, ETrue);
	
	if (err == KErrNone)
		err = PluginFreeRamCalledL(0x10286A36, ETrue);
	
	// This the P8 system plugin has not been called
	if (err == KErrNone)
		err = PluginFreeRamCalledL(0x10286A37, EFalse);
	
	// Check that the low priority app plugin has been called
	if (err == KErrNone)
		err = PluginFreeRamCalledL(0x10286A3B, ETrue);
	
	// Check that the P7 application has been closed
	if (err == KErrNone)
		err = AppIsRunning(0x10005A22, EFalse);

	if (err == KErrNone)
		console->Printf(_L("Optional Allocation (Async) Test 1 passed\n"));
	else
		console->Printf(_L("Optional Allocation (Async) Test 1 failed\n"));	
		
	
	return err;
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
TInt COomTestHarness::OptionalAllocationAsyncTest2L()
	{
	// Start an application (plus a second so that the first app is not in the foreground)
	// The first application is set to consume 5MB of RAM
	StartApplicationL(0x10005A22, 5 * 1024);
	StartApplicationL(0x101F8599, 0);
	
	BringAppToForeground(0x101F8599);
	
	Settle();
	
	// Go just above the good memory threshold
	EatMemoryL(KOomJustAboveGoodMemoryThreshold);
	
	Settle();
	
	// Request 10 MB of data, using the priority of the referenced plugin (constant priority 8)
	// Say that 5MB is the minimum we need
	TInt bytesAvailable;
	TInt err = RequestOptionalRamASyncWrapper(10 * 1024 * 1024, 5 * 1024 * 1024, 0x10286A37, bytesAvailable);
	
	if (err == KErrNone)
		{
		// Check that the reported bytes available is > 3MB and < 7MB
		if ((bytesAvailable < 3 * 1024 * 1024)
				|| (bytesAvailable > 7 * 1024 * 1024))
			{
			err = KErrGeneral;
			}
		}
	
	// Check that all system plugins below P8 have been called
	
	if (err == KErrNone)
		err = PluginFreeRamCalledL(0x10286A34, ETrue);
	
	if (err == KErrNone)
		err = PluginFreeRamCalledL(0x10286A35, ETrue);
	
	if (err == KErrNone)
		err = PluginFreeRamCalledL(0x10286A36, ETrue);
	
	// This the P8 system plugin has not been called
	if (err == KErrNone)
		err = PluginFreeRamCalledL(0x10286A37, EFalse);
	
	// Check that the low priority app plugin has been called
	if (err == KErrNone)
		err = PluginFreeRamCalledL(0x10286A3B, ETrue);
	
	// Check that the P7 application has been closed
	if (err == KErrNone)
		err = AppIsRunning(0x10005A22, EFalse);

	if (err == KErrNone)
		console->Printf(_L("Optional Allocation (Async) Test 2 passed\n"));
	else
		console->Printf(_L("Optional Allocation (Async) Test 2 failed\n"));	
		
	
	return err;
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
TInt COomTestHarness::OptionalAllocationAsyncTest3L()
	{
	// Start an application (plus a second so that the first app is not in the foreground)
	// The first application is set to consume 5MB of RAM
	StartApplicationL(0x10005A22, 3 * 1024);
	StartApplicationL(0x101F8599, 0);
	
	BringAppToForeground(0x101F8599);
	
	Settle();
	
	// Go just above the good memory threshold
	EatMemoryL(KOomJustAboveGoodMemoryThreshold);
	
	Settle();
	
	// Request 10 MB of data, using the priority of the referenced plugin (constant priority 8)
	// Say that 5MB is the minimum we need
	TInt bytesAvailable;
	TInt err = RequestOptionalRamASyncWrapper(10 * 1024 * 1024, 5 * 1024 * 1024, 0x10286A37, bytesAvailable);
	
	// We expect an error because it has not been possible to free the minimum memory
	if (err == KErrNoMemory)
		{
		err = KErrNone;
		}
	else
		{
		err = KErrGeneral;
		}
	
	// Check that all system plugins below P8 have been called
	
	if (err == KErrNone)
		err = PluginFreeRamCalledL(0x10286A34, ETrue);
	
	if (err == KErrNone)
		err = PluginFreeRamCalledL(0x10286A35, ETrue);
	
	if (err == KErrNone)
		err = PluginFreeRamCalledL(0x10286A36, ETrue);
	
	// This the P8 system plugin has not been called
	if (err == KErrNone)
		err = PluginFreeRamCalledL(0x10286A37, EFalse);
	
	// Check that the low priority app plugin has been called
	if (err == KErrNone)
		err = PluginFreeRamCalledL(0x10286A3B, ETrue);
	
	// Check that the P7 application has been closed
	if (err == KErrNone)
		err = AppIsRunning(0x10005A22, EFalse);

	if (err == KErrNone)
		console->Printf(_L("Optional Allocation (Async) Test 3 passed\n"));
	else
		console->Printf(_L("Optional Allocation (Async) Test 3 failed\n"));	
		
	
	return err;
	}




//////////////////////////////////////////////////////////////////////////////


TInt COomTestHarness::BringAppToForeground(TInt32 aUid)
	{
	//bring app to foreground
	TInt id = iApps.Find<TUid>(TUid::Uid(aUid), CCDummyApplicationHandle::CompareTo);
	User::LeaveIfError(id);
	iApps[id]->BringToForeground();
	User::After(KSettlingTime);
	}


COomTestHarness* COomTestHarness::NewLC()
	{
	COomTestHarness* self = new (ELeave) COomTestHarness();
	CleanupStack::PushL(self);
	self->ConstructL();
	return self;
	}

void COomTestHarness::ConstructL()
	{
	User::LeaveIfError(iAllocServer.Connect());
	User::LeaveIfError(iOomSession.Connect());
	
	User::LeaveIfError(iChunk.CreateLocal(KOomHarnessInitialEatenMemory, KOomHarnessMaxEatenMemory));
	iChunkSize = KOomHarnessInitialEatenMemory;
	
	ResetL();
	}


COomTestHarness::~COomTestHarness()
	{
	iApps.ResetAndDestroy();
	iAllocServer.Close();
	iChunk.Close();
	iPluginCallCounts.Close();
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
	TInt err = iChunk.Adjust(iChunkSize);
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
		console->Printf(_L("Application running state not as expected %x \n"), aUid);
		err = KErrGeneral;
		}	
	
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


TInt COomTestHarness::GetFreeMemory()
	{
	User::CompressAllHeaps();
	
	TInt current = 0;
	HAL::Get( HALData::EMemoryRAMFree, current );
	
	return current;
	}

// Has FreeRam been called on this plugin since the last call to this function (or since the whole class has been reset)?
// Returns KErrNone if we get the expected result
TInt COomTestHarness::PluginFreeRamCalledL(TInt32 aUid, TBool aExpectedResult)
	{
	TInt newLowMemoryCount = 0;
	TInt err = RProperty::Get(KUidOomPropertyCategory, aUid + KOOMDummyPluginLowMemoryCount, newLowMemoryCount);

	if (err != KErrNone)
		console->Printf(_L("Unable to get plugin FreeRam count\n"));
	
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
		console->Printf(_L("Unable to find call count.\n"));
		err = KErrNotFound;
		}
		
	if (aExpectedResult != freeRamHasBeenCalledOnPlugin)
		{
		console->Printf(_L("Plugin FreeRam count not as expected %x \n"), aUid);
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

void COomTestHarness::EatMemoryUntilWeAreStuckUnderTheLowThresholdL()
	{
	while (ETrue)
		{
		// Go significantly under the low memory threshold
		EatMemoryL(KOomSignificantlyUnderLowMemoryThreshold);
		Settle();
		// If we are still under the memory threshold then the OOM monitor has not been able to recover,
		// so our job here is done...
		if (GetFreeMemory() < (KOomLowMemoryThreshold * 1024))
			break;
		
		// If memory has been released to take us above the low memory threshold then go again...
		}	
	}


COomTestHarness::COomTestHarness()
	{
	// TODO
	}


//  Local Functions
LOCAL_C void MainL()
	{
	COomTestHarness* testHarness = COomTestHarness::NewLC();
	
	testHarness->StartL();
	
	CleanupStack::PopAndDestroy(testHarness);
	}

//  Local Functions
LOCAL_C void OldMainL()
	{
	//
	// add your program code here, example code below
	//
	const TInt implementations(KUidOOMDummyPluginLastImplementation-KUidOOMDummyPluginFirstImplementation+1);
	TInt initialLowCount[implementations];
	TInt initialGoodCount[implementations];
	TInt finalLowCount[implementations];
	TInt finalGoodCount[implementations];
	TInt tmp;
	TBuf<80> line;
	TBool pass = ETrue;
	ROOMAllocSession allocServer;
    TMemoryInfoV1Buf meminfo;
	//connect to alloc server
	User::LeaveIfError(allocServer.Connect());
	CleanupClosePushL(allocServer);
	//configure alloc server
	User::LeaveIfError(allocServer.Reset());
/*	User::LeaveIfError(allocServer.Configure(TUid::Uid(KUidOOMDummyPluginFirstImplementation), 0x100000, 0x1000, 0x400000));
	User::LeaveIfError(allocServer.Configure(TUid::Uid(KUidOOMDummyPluginFirstImplementation+3), 0x100000, 0x1000, 0x400000));
	User::LeaveIfError(allocServer.StartAllocating());
	for(TInt i=0;i<5;i++)
		{
	    UserHal::MemoryInfo(meminfo);
	    console->Printf(_L("free mem %d"), meminfo().iFreeRamInBytes);
	    User::After(1000000);
		}*/
	//create dummy apps (note app with that UID *must* exist or apparc won't allow contruction due to missing app_reg.rsc)
	RPointerArray<CCDummyApplicationHandle> apps;
	CleanupResetAndDestroyPushL(apps);
	CCDummyApplicationHandle *app = CCDummyApplicationHandle::NewLC(KUidCalendar, 0x100000);
	apps.AppendL(app);
	CleanupStack::Pop(app);
	app = CCDummyApplicationHandle::NewLC(KUidClock, 0x100000);
	apps.AppendL(app);
	CleanupStack::Pop(app);
	app = CCDummyApplicationHandle::NewLC(KUidAbout, 0x100000);
	apps.AppendL(app);
	CleanupStack::Pop(app);
	
	//wait for oom system to settle
	User::After(KSettlingTime);
	
	//verify apps started OK
	for(TInt i=0;i<apps.Count();i++)
		{
		if(apps[i]->Process().ExitType() != EExitPending)
			{
			pass = EFalse;
			console->Printf(_L("app %x not running (status %d)\n"),apps[i]->Uid().iUid,apps[i]->Process().ExitType());
			}
		}
	
	//bring calendar to foreground
	TInt id = apps.Find<TUid>(KUidCalendar, CCDummyApplicationHandle::CompareTo);
	User::LeaveIfError(id);
	apps[id]->BringToForeground();
	User::After(KSettlingTime);
	
	//eat memory to invoke low memory handler
    UserHal::MemoryInfo(meminfo);
    TInt freeMem = meminfo().iFreeRamInBytes;
	TAny *ptr = User::Alloc(freeMem - 247 * 1024);
	CleanupStack::PushL(ptr);
	
	//wait for oom system to settle
	User::After(KSettlingTime);
	
	//verify some apps were closed
	for(TInt i=0;i<apps.Count();i++)
		{
		if(apps[i]->Process().ExitType() != EExitPending)
			{
			console->Printf(_L("app %x not running (status %d)\n"),apps[i]->Uid().iUid,apps[i]->Process().ExitType());
			}
		else
			{
			console->Printf(_L("app %x still running\n"),apps[i]->Uid().iUid);
			}
		}
	
	//release memory to invoke good memory handler
	CleanupStack::PopAndDestroy(ptr);
	
	//wait for oom system to settle
	User::After(KSettlingTime);
	
/*	console->Write(_L("Initial Counts\n"));
	for(TInt id=0;id<KOOMDummyPluginImplementationCount;id++)
		{
		User::LeaveIfError(RProperty::Get(KUidOomPropertyCategory, id + KUidOOMDummyPluginFirstImplementation + KOOMDummyPluginLowMemoryCount, tmp));
		initialLowCount[id] = tmp;
		User::LeaveIfError(RProperty::Get(KUidOomPropertyCategory, id + KUidOOMDummyPluginFirstImplementation + KOOMDummyPluginGoodMemoryCount, tmp));
		initialGoodCount[id] = tmp;
		line.Format(_L("%x %d %d\n"), KUidOOMDummyPluginFirstImplementation + id, initialLowCount[id], initialGoodCount[id]);
		console->Write(line);
		}
	//check status of dummy apps
	User::After(2000000);
	line.Format(_L("app1 %d app2 %d\n"), app1.ExitType(), app2.ExitType());
	console->Write(line);
	if(app1.ExitType() != EExitPending || app2.ExitType() != EExitPending) pass = EFalse;
	if(pass)
		console->Write(_L("verdict: pass"));
	else
		console->Write(_L("verdict: fail"));
	console->Getch();

    UserHal::MemoryInfo(meminfo);
    TInt freeMem = meminfo().iFreeRamInBytes;
	TAny *ptr = User::Alloc(freeMem - 260000);
	CleanupStack::PushL(ptr);
	console->Write(_L("Counts after alloc\n"));
	for(TInt id=0;id<KOOMDummyPluginImplementationCount;id++)
		{
		User::LeaveIfError(RProperty::Get(KUidOomPropertyCategory, id + KUidOOMDummyPluginFirstImplementation + KOOMDummyPluginLowMemoryCount, tmp));
		finalLowCount[id] = tmp;
		User::LeaveIfError(RProperty::Get(KUidOomPropertyCategory, id + KUidOOMDummyPluginFirstImplementation + KOOMDummyPluginGoodMemoryCount, tmp));
		finalGoodCount[id] = tmp;
		line.Format(_L("%x %d %d\n"), KUidOOMDummyPluginFirstImplementation + id, finalLowCount[id], finalGoodCount[id]);
		console->Write(line);
		if(finalLowCount[id] - initialLowCount[id] != 1) pass = EFalse;
		}
	//check status of dummy apps
	User::After(2000000);
	line.Format(_L("app1 %d app2 %d\n"), app1.ExitType(), app2.ExitType());
	console->Write(line);
	if(app1.ExitType() == EExitPending || app2.ExitType() == EExitPending) pass = EFalse;
	if(pass)
		console->Write(_L("verdict: pass"));
	else
		console->Write(_L("verdict: fail"));
	console->Getch();
	CleanupStack::PopAndDestroy(ptr);
	console->Write(_L("Counts after free\n"));
	for(TInt id=0;id<KOOMDummyPluginImplementationCount;id++)
		{
		User::LeaveIfError(RProperty::Get(KUidOomPropertyCategory, id + KUidOOMDummyPluginFirstImplementation + KOOMDummyPluginLowMemoryCount, tmp));
		finalLowCount[id] = tmp;
		User::LeaveIfError(RProperty::Get(KUidOomPropertyCategory, id + KUidOOMDummyPluginFirstImplementation + KOOMDummyPluginGoodMemoryCount, tmp));
		finalGoodCount[id] = tmp;
		line.Format(_L("%x %d %d\n"), KUidOOMDummyPluginFirstImplementation + id, finalLowCount[id], finalGoodCount[id]);
		console->Write(line);
		if(finalGoodCount[id] - initialGoodCount[id] != 1) pass = EFalse;
		}
	//check status of dummy apps
	User::After(2000000);
	line.Format(_L("app1 %d app2 %d\n"), app1.ExitType(), app2.ExitType());
	console->Write(line);
	if(app1.ExitType() == EExitPending || app2.ExitType() == EExitPending) pass = EFalse;*/
	if(pass)
		console->Write(_L("verdict: pass"));
	else
		console->Write(_L("verdict: fail"));
	console->Getch();
	allocServer.Reset();
	CleanupStack::PopAndDestroy(2);//apps, allocServer
	}

LOCAL_C void DoStartL()
	{
	// Create active scheduler (to run active objects)
	CActiveScheduler* scheduler = new (ELeave) CActiveScheduler();
	CleanupStack::PushL(scheduler);
	CActiveScheduler::Install(scheduler);

	MainL();

	// Delete active scheduler
	CleanupStack::PopAndDestroy(scheduler);
	}

//  Global Functions

GLDEF_C TInt E32Main()
	{
	// Create cleanup stack
	__UHEAP_MARK;
	CTrapCleanup* cleanup = CTrapCleanup::New();

	// Create output console
	TRAPD(createError, console = Console::NewL(KTextConsoleTitle, TSize(
			KConsFullScreen, KConsFullScreen)));
	if (createError)
		return createError;

	// Run application code inside TRAP harness, wait keypress when terminated
	TRAPD(mainError, DoStartL());
	if (mainError)
		console->Printf(KTextFailed, mainError);
	console->Printf(KTextPressAnyKey);
	console->Getch();

	delete console;
	delete cleanup;
	__UHEAP_MARKEND;
	return KErrNone;
	}


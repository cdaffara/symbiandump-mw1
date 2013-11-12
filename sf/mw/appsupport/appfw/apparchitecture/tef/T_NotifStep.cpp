// Copyright (c) 2005-2010 Nokia Corporation and/or its subsidiary(-ies).
// All rights reserved.
// This component and the accompanying materials are made available
// under the terms of "Eclipse Public License v1.0"
// which accompanies this distribution, and is available
// at the URL "http://www.eclipse.org/legal/epl-v10.html".
//
// Initial Contributors:
// Nokia Corporation - initial contribution.
//
// Contributors:
//
// Description:
// Checks for notifications when application list changes.\n
// 
// t_notifstep.cpp
//

/**
 @file t_notifstep.cpp
 @test
 @internalComponent - Internal Symbian test code
*/

#include <f32file.h>
#include <fbs.h>
#include <apaid.h>
#ifdef SYMBIAN_ENABLE_SPLIT_HEADERS
#include <apaidpartner.h>
#include <apgicnflpartner.h>
#endif //SYMBIAN_ENABLE_SPLIT_HEADERS
#include <apgaplst.h>
#include <apaflrec.h>
#include <apgcli.h>
#include <apacmdln.h>
#include <apfrec.h>
#include <datastor.h>
#include <apgicnfl.h>
#include <apasvst.h>
#include <apgnotif.h>
#include <e32test.h>

#include "appfwk_test_utils.h"
#include "T_NotifStep.h"
#include "T_SisFileInstaller.h"

_LIT(KApparcTestAppSisFile, "z:\\apparctest\\apparctestsisfiles\\TApparcTestApp.sis");
_LIT(KApparcTestAppComponent, "TApparcTestApp");


_LIT(KImportAppsDir,"c:\\private\\10003a3f\\import\\apps\\");
_LIT(KResourceAppsDir,"c:\\resource\\apps\\");

_LIT(KForceRegAppSourcePath_Reg,"Z:\\ApparcTest\\testforceregistrationapp1_reg.rsc");
_LIT(KForceRegAppSourcePath_Loc,"Z:\\ApparcTest\\testforceregistrationapp1_loc.rsc");

_LIT(KForceRegAppTargetPath_Reg,"c:\\private\\10003a3f\\import\\apps\\testforceregistrationapp1_reg.rsc");
_LIT(KForceRegAppTargetPath_Loc,"c:\\resource\\apps\\testforceregistrationapp1_loc.rsc");

_LIT(KForceRegAppCaption,"TestForceRegistration");
_LIT(KForceRegAppDefaultCaption,"testforceregistrationapp1");

/**
 
  Overridden from virtual method MApaAppListServObserver::HandleAppListEvent().
  This method is used to receive notification for change in application list.\n
 
*/
void CTestObserver::HandleAppListEvent(TInt /*aEvent*/)
	{

	iNotified++;
	if (iNotifier)
		iNotifier->Cancel();
	CActiveScheduler::Stop();
	}
	
/**
  Auxiliary Fn for Test Case ID T-NotifStep-TestAppNotificationL
 
  Copy a registration resource file in the path  "c:\private\10003a3f\import\apps" .
*/
void CT_NotifStep::CreateAppL(const TDesC& aAppName)
	{
	TFileName appFullName;
	TFileName appTargetName; 
	CFileMan* fileManager = CFileMan::NewL(iFs);
	CleanupStack::PushL(fileManager);	
	appFullName.Format(_L("z:\\ApparcTest\\%S_reg.RSC"),&aAppName);
	appTargetName.Format(_L("C:\\Private\\10003a3f\\Import\\apps\\%S_reg.Rsc"), &aAppName);
	INFO_PRINTF2(_L("copying the file: %S"), &appTargetName);
	TInt ret = fileManager->Copy (appFullName, appTargetName, CFileMan::ERecurse);
	TEST(ret==KErrNone);
	CleanupStack::PopAndDestroy(fileManager);
	}

/**
	Auxiliary Fn for Test Case ID T-NotifStep-TestAppNotificationL
	Delete a registration resource file from the path  "c:\private\10003a3f\import\apps" .
*/
void CT_NotifStep::DeleteAppL(const TDesC& aAppName)
	{
	TFileName appFullName;
	CFileMan* fileManager = CFileMan::NewL(iFs);
	CleanupStack::PushL(fileManager);
	appFullName.Format(_L("C:\\Private\\10003a3f\\Import\\apps\\%S_reg.Rsc"),&aAppName);
		
	INFO_PRINTF2(_L("Deleting the file: %S"), &appFullName);
	TRequestStatus status;
	TTime tempTime(0); // added tempTime to avoid asynch CFileMan::Attribs request completing with KErrArgument
	TEST(fileManager->Attribs(appFullName,0,KEntryAttReadOnly, tempTime, CFileMan::ERecurse, status)==KErrNone);
	User::WaitForRequest(status);
	TEST(status.Int() == KErrNone);
	TInt ret = fileManager->Delete(appFullName, CFileMan::ERecurse);
	TEST(ret==KErrNone);
	CleanupStack::PopAndDestroy(fileManager);
	}
 
/**
   @SYMTestCaseID T-NotifStep-TestAppNotificationL
  
   @SYMPREQ REQ1087, PCHY-5L3RDW
  
   @SYMTestCaseDesc Test whether notification is received from CApaAppListNotifier
   whenever application list changes. 
   
   @SYMTestPriority High 
  
   @SYMTestStatus Implemented
   
   @SYMTestActions The method creates an object of user-defined observer CTestObserver
   and creates a CApaAppListNotifier active object which is associated to the
   observer. It then starts the active scheduler and adds a new application or deletes
   an application from the application list. To ensure that notifications are received on
   change in application list HandleAppListEvent() method which is derived
   from interface class MApaAppListServObserver is observed.\n
   API Calls:\n	
   CApaAppListNotifier::NewL(MApaAppListServObserver* aObserver, TPriority aPriority)\n
   
   @SYMTestExpectedResults On deletion of the app the notification for change in
   the application list is received.
    
 */
void CT_NotifStep::TestAppNotificationL()
	{
	// Force the applist to be updated 
	//To ensure that server has time to count all applications in the system
    TRequestStatus status;
    iSession.SetNotify(ETrue, status);
    User::WaitForRequest(status);
    
	TInt theAppCount = 0;
	TInt theErr1 = iSession.AppCount(theAppCount);
	TEST(theErr1==KErrNone);

	INFO_PRINTF2(_L("The number of applications : %d"), theAppCount);
		
	CTestObserver* obs=new(ELeave) CTestObserver();
	CleanupStack::PushL(obs);
	CApaAppListNotifier* notif=CApaAppListNotifier::NewL(obs,CActive::EPriorityHigh);
	CleanupStack::PushL(notif);
	obs->iNotifier=notif;	
	INFO_PRINTF1(_L("Creating and deleting apps for notification"));
    CSisFileInstaller sisFileInstaller;
    INFO_PRINTF2(_L("Installing sis file from -> %S"), &KApparcTestAppSisFile);
    sisFileInstaller.InstallSisAndWaitForAppListUpdateL(KApparcTestAppSisFile);

	CActiveScheduler::Start();

	TInt theAppCount1 = 0;
	theErr1 = iSession.AppCount(theAppCount1);
	TEST((theAppCount1 - 1) == theAppCount);
	INFO_PRINTF2(_L("The number of applications : %d"), theAppCount1);
	CleanupStack::PopAndDestroy(notif);
	
	notif = CApaAppListNotifier::NewL(obs,CActive::EPriorityHigh);
	CleanupStack::PushL(notif);
	obs->iNotifier = notif;
	INFO_PRINTF1(_L("Deleting the application"));
	sisFileInstaller.UninstallSisL(KApparcTestAppComponent);
	CActiveScheduler::Start();
	
	CleanupStack::PopAndDestroy(notif);	
	theErr1 = iSession.AppCount(theAppCount1);
	
	TEST(theErr1==KErrNone);
	TEST(theAppCount1 == theAppCount);
	
	INFO_PRINTF2(_L("The number of applications : %d"), theAppCount1);
	TEST(obs->iNotified>0);
	
	INFO_PRINTF2(_L("Received %d notifications"),obs->iNotified);
	CleanupStack::PopAndDestroy(obs);
	}

void CT_NotifStep::ModifyIconFileTimeStampL()
	{
	_LIT(KMbmIconFile, "c:\\resource\\apps\\tupgradeiconapp.mbm");
	_LIT(KTestIconFile, "c:\\TestUpgradeIcon\\tupgradeiconapp.mbm");
	_LIT(KTestIconFileTempPath, "c:\\TestUpgradeIcon\\");

	// Create KMbmIconFileTempPath
	TInt ret = iUtils.CreateDirectoryL(KTestIconFileTempPath);
	TEST(ret == KErrNone || ret == KErrAlreadyExists);
	INFO_PRINTF1(_L("Copy icon file from C: drive to temp path"));
	ret = iUtils.CopyFileL(KMbmIconFile, KTestIconFileTempPath);
	
	INFO_PRINTF1(_L("Modify timestamp of the icon file"));
	TTime modifiedTime(0);
	modifiedTime.HomeTime();
 	ret = iFs.SetModified(KTestIconFile, modifiedTime);
 	TEST(ret==KErrNone);
 	ret = iUtils.SetReadOnly(KTestIconFile, 0);
	TEST(ret==KErrNone);	
	INFO_PRINTF2(_L("Replace the modified icon file in C: drive (%S)"), &KMbmIconFile);
 	ret = iUtils.CopyFileL(KTestIconFile, KMbmIconFile);
	// wait 5 seconds to finish re-scan
	User::After(5*1000000);
	TEST(ret==KErrNone);
	}

/**
   @SYMTestCaseID	T-NotifStep-TTestIconFileNotificationL
  
   @SYMPREQ 		PDEF102804
  
   @SYMTestCaseDesc Test whether notification is received from CApaAppListNotifier   whenever the application icon file is changed
   
   @SYMTestPriority High 
  
   @SYMTestStatus 	Implemented
   
   @SYMTestActions 	Modify the application icon file in a temporary location. Move icon file to a location specified in the resource file.
   
   @SYMTestExpectedResults A notification is recieved when the updated icon file is replaced to a location specified in the resource file.
*/ 
void CT_NotifStep::TestIconFileNotificationL()
	{
	// Force the applist to be updated 
	//To ensure that server has time to count all applications in the system
	RPointerArray<TDesC> dummy;
	User::LeaveIfError(iSession.ForceRegistration(dummy));

	// Create observer
	CTestObserver* obs = new(ELeave) CTestObserver();
	CleanupStack::PushL(obs);
	CApaAppListNotifier* notif = CApaAppListNotifier::NewL(obs, CActive::EPriorityHigh);
	CleanupStack::PushL(notif);
	obs->iNotifier=notif;

	// Change the timestamp of the icon file
	INFO_PRINTF1(_L("Modifying the icon file...attempt to check it's notified"));
	ModifyIconFileTimeStampL();
	
	CActiveScheduler::Start();
	
	CleanupStack::PopAndDestroy(notif);
	// Test if notification is recieved.
	TEST(obs->iNotified > 0);
	if (obs->iNotified > 0)
		{
		INFO_PRINTF2(_L("Received %d notifications"),obs->iNotified);
		}
	
	CleanupStack::PopAndDestroy(obs); //obs
	}

/*
 * TestForceRegistrationL copies a registration file and requests for force registration.
 * It checks whether default caption is assigned, as the localisable file
 * does not exist. Then it copies the localisable file and exits.  
 */
void TestForceRegistrationL(CT_NotifStep *aNotifStep)
	{
    	RApaLsSession ls;
    	User::LeaveIfError(ls.Connect());
    	CleanupClosePushL(ls);
    
    	//Copy the registration file to c:\private\10003a3f\import\apps
	User::LeaveIfError((aNotifStep->iUtils).CopyFileL(KForceRegAppSourcePath_Reg, KForceRegAppTargetPath_Reg));
	aNotifStep->INFO_PRINTF1(_L("Successfully copied testforceregistrationapp1_reg.rsc from Z:\\ApparcTest\\testforceregistrationapp1_reg.rsc"));

	RPointerArray<TDesC> dummy;
	User::LeaveIfError(ls.ForceRegistration(dummy));
    
    	TApaAppInfo info;
    	TUid appUid={0x102826E0};
    	aNotifStep->TEST(ls.GetAppInfo(info,appUid)==KErrNone);
    	aNotifStep->INFO_PRINTF2(_L("Caption of the application before copying localisable file: %S"), &info.iCaption);
    	//Test whether default captions are assigned as localisable file not exists.   
    	aNotifStep->TEST(info.iCaption.Compare(KForceRegAppDefaultCaption)==0);    

    	//Waits for some time as test thread gets chance to execute. As apparc does not notifiy
    	//applist change due to force registration to all the clients, the test thread still waits
    	//for applist change notification.
    	User::After(5000000); 

    	//Copy localisable file to c:\resource\apps
	User::LeaveIfError((aNotifStep->iUtils).CopyFileL(KForceRegAppSourcePath_Loc, KForceRegAppTargetPath_Loc));
	aNotifStep->INFO_PRINTF1(_L("Successfully copied testforceregistrationapp1_loc.rsc from Z:\\ApparcTest\\testforceregistrationapp1_loc.rsc"));

	CleanupStack::PopAndDestroy();
	}

TInt ForceRegistrationThread(TAny* aPtr)
    	{
    	__UHEAP_MARK;
    	CTrapCleanup* trapCleanup = NULL;
	trapCleanup = CTrapCleanup::New();
	
    	CT_NotifStep *notifStep=(CT_NotifStep *) aPtr;
    	
    	TRAPD(errorCode, TestForceRegistrationL(notifStep));
    
    	delete trapCleanup;
    	__UHEAP_MARKEND;    
    	return(errorCode);
    	}

//Deletes the file if it exists.
TInt CT_NotifStep::DeleteFileL(RSmlTestUtils &aFs, const TDesC &aFileName)
    	{
    	TInt fileExists = EFalse;
    	TInt err;
    	aFs.IsFilePresent(aFileName, fileExists);
    	if (fileExists)
        	{
        	aFs.ChangeFilePermissionL(aFileName);
        	err=aFs.DeleteFileL(aFileName);
        	if(err==KErrNone)
            		INFO_PRINTF2(_L("Removed file %S"), &aFileName);
        	else
            		INFO_PRINTF2(_L("Failed to remove file %S"), &aFileName);
        	}
    	else
        	{
        		err=KErrNotFound;
        	}

    	return(err);
	}

/**
   @SYMTestCaseID           APPFWK-APPARC-0106
  

   @SYMDEF                  DEF141223
  
    @SYMTestCaseDesc         Tests apparc does not notifiy applist change, which occurred
                            due to force registration, to all the other clients who have not requested
                            force registration, before the installation is completed.
  
   @SYMTestPriority         High
  
   @SYMTestStatus           Implemented
   
   @SYMTestActions          1. The test registers with apparc for applist change notification.
                            2. Starts another thread, which copies a registration file and requests
                               force registration. Later copies the localisable file of the same
                               application.
                            3. Waits till the applist change event occurs or till 15 seconds
                            4. Tests the caption of the application is same as the caption specified in
                               localisable file. 
                             
   
   @SYMTestExpectedResults The retrieved application caption should be same as the caption specified in 
                           localisable file. 
 */
void CT_NotifStep::TestForceRegistrationNotificationL()
    	{
    	//Share the handle with other threads in the process    
    	User::LeaveIfError(iUtils.ShareAuto()); 
    	User::LeaveIfError(Logger().ShareAuto());
    
    	INFO_PRINTF1(_L("Creating c:\\private\\10003a3f\\import\\apps\\ directory......."));    
	TInt err = iUtils.CreateDirectoryL(KImportAppsDir);
	TESTEL((err == KErrNone || err == KErrAlreadyExists), err);
	INFO_PRINTF1(_L("c:\\private\\10003a3f\\import\\apps\\ is created successfully or already exists"));

    	INFO_PRINTF1(_L("Creating c:\\resource\\apps\\ directory......."));    
    	err = iUtils.CreateDirectoryL(KResourceAppsDir);
    	TESTEL((err == KErrNone || err == KErrAlreadyExists), err);
    	INFO_PRINTF1(_L("c:\\resource\\apps\\ is created successfully or already exists"));
    
    	INFO_PRINTF1(_L("Make sure that registraiton and localisable files does not exist before the test starts..."));    
    	DeleteFileL(iUtils, KForceRegAppTargetPath_Reg);
    	DeleteFileL(iUtils, KForceRegAppTargetPath_Loc);
        
    	INFO_PRINTF1(_L("Connecting to apparc server..."));    
    	RApaLsSession ls;
    	User::LeaveIfError(ls.Connect());
    	CleanupClosePushL(ls);
    
    	TRequestStatus status;
    	INFO_PRINTF1(_L("Registering with apparc server for applist change notification."));       
    	ls.SetNotify(EFalse, status); 
  
    	INFO_PRINTF1(_L("Creating ForceRegistrationThread............"));
    	_LIT(KForceRegThreadName,"ForceRegThread");
    	TBuf<0x100> threadName(KForceRegThreadName);
    	RThread thread;
    
    	User::LeaveIfError(thread.Create(threadName, ForceRegistrationThread, 0x1000, NULL, (TAny*) this));
    	CleanupClosePushL(thread);
	TRequestStatus threadStatus;
	thread.Logon(threadStatus);
	
	//Starts a thread which actually requests for force registration.
    	thread.Resume(); 
    
	RTimer timer;
	CleanupClosePushL(timer);
	User::LeaveIfError(timer.CreateLocal());
	TRequestStatus timerStatus;
	timer.After(timerStatus,15 * 1000000);

    	//Wait till the applist change event is receivied or till 15 Seconds
    	User::WaitForRequest(status, timerStatus); 

    	//Test whether the applist change event is received
	TEST(status != KRequestPending); 
    
	TApaAppInfo info;
	TUid appUid={0x102826E0};
	TEST(ls.GetAppInfo(info,appUid)==KErrNone);
	
	//Checks the caption of the application is "TestForceRegistration". The caption will not be 
	//"TestForceRegistration" if apparc server notifies all the clients about the applist change
	//due to force registration, before installation is completed.
	INFO_PRINTF2(_L("Caption of the application after the applist change notification is received: %S"), &info.iCaption);
    	TEST(info.iCaption.Compare(KForceRegAppCaption)==0);


    	//Wait till the the ForceRegistrationThread exits	
	User::WaitForRequest(threadStatus);
	
	TInt theadExitCode=thread.ExitReason();
	INFO_PRINTF2(_L("ForceRegistrationThread thread is exited with %d"), theadExitCode);
    	//Checks ForceRegistrationThread completed successfully.	
	TEST(theadExitCode==KErrNone); 

    	//Delete the registration and localisation files
	TEST(DeleteFileL(iUtils, KForceRegAppTargetPath_Reg)==KErrNone);
    	TEST(DeleteFileL(iUtils, KForceRegAppTargetPath_Loc)==KErrNone);

    	CleanupStack::PopAndDestroy(3,&ls);
    	}

CT_NotifStep::~CT_NotifStep()
/**
   Destructor
 */
	{
	delete iScheduler;
	}

CT_NotifStep::CT_NotifStep()
/**
   Constructor
 */
	{
	// Call base class method to set up the human readable name for logging
	SetTestStepName(KT_NotifStep);
	}

TVerdict CT_NotifStep::doTestStepL()
/**
   @return - TVerdict code
   Override of base class virtual
 */
	{
	INFO_PRINTF1(_L("Testing Apparc...T_Notif"));

	TInt ret = FbsStartup();
	TEST(ret==KErrNone);
	ret=RFbsSession::Connect();
	TEST(ret==KErrNone);
	// start an active scheduler
	iScheduler=new(ELeave) CActiveScheduler();
	CActiveScheduler::Install(iScheduler);

	TEST(KErrNone == iFs.Connect());
	TEST(KErrNone == iSession.Connect());
	TEST(KErrNone == iUtils.Connect());

    TApaAppInfo info;
    TUid uid = {0x100048F3};
    TInt err = iSession.GetAppInfo(info, uid);
    if(err == KErrNone)
        {       
        CSisFileInstaller sisFileInstaller;
        sisFileInstaller.UninstallSisL(KApparcTestAppComponent);
        }
	
	// run the testcode (inside an alloc heaven harness)	
	__UHEAP_MARK;
	iUtils.Connect();
//#if defined (__WINSCW__)
//	INFO_PRINTF1(_L("T-NotifStep-TTestIconFileNotificationL Test Started..."));
//	TRAP(ret,TestIconFileNotificationL());
//	TEST(ret==KErrNone);
//	INFO_PRINTF2(_L("TestIconFileNotificationL() finished with return code '%d'\n"), ret);
//#endif
	INFO_PRINTF1(_L("T-NotifStep-TestAppNotificationL Test Started..."));
	TRAP(ret,TestAppNotificationL());
	TEST(ret==KErrNone);
	INFO_PRINTF2(_L("TestAppNotificationL() finished with return code '%d'\n"), ret);

//	INFO_PRINTF1(_L("TestForceRegistrationNotificationL Test Started..."));
//	TRAP(ret, TestForceRegistrationNotificationL());
//	TEST(ret==KErrNone);	
//	INFO_PRINTF2(_L("TestForceRegistrationNotificationL() finished with return code '%d'\n"), ret);
	iUtils.Close();	
	__UHEAP_MARKEND;
	
	iUtils.Close();
	iSession.Close();
	iFs.Close();
	RFbsSession::Disconnect();
	INFO_PRINTF1(_L("T_Notif Completed."));
	return TestStepResult();
	}

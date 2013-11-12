// Copyright (c) 2007-2010 Nokia Corporation and/or its subsidiary(-ies).
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
//

/**
 @file
 @test
 @internalComponent - Internal Symbian test code
*/

#include "T_ServiceRegistryStep.h"
#include <serviceregistry.h>
#include "testableapalssession.h"
#ifdef SYMBIAN_ENABLE_SPLIT_HEADERS
#include <apaidpartner.h>
#endif
#include "T_SisFileInstaller.h" 
 
_LIT(KTstAppStandAloneSisFile, "z:\\apparctest\\apparctestsisfiles\\TSTAPP_standalone.sis");
_LIT(KTstAppStandAloneComponent, "TSTAPP_standalone");

_LIT(KServerApp2SisFile, "z:\\apparctest\\apparctestsisfiles\\serverapp2.sis");
_LIT(KServerApp2Component, "serverapp2");

_LIT(KApparcTestAppSisFile, "z:\\apparctest\\apparctestsisfiles\\TApparcTestApp.sis");
_LIT(KApparcTestAppComponent, "TApparcTestApp");

_LIT8(KLitMimeType,"mime/updregappuninstall");

/**
 * Constructor
 */	
CT_ServiceRegistryTestStep::CT_ServiceRegistryTestStep()
	{
	// Call base class method to set up the human readable name for logging
	SetTestStepName(KT_ServiceRegistryStep);
	}

/**
 * Destructor
 */
CT_ServiceRegistryTestStep::~CT_ServiceRegistryTestStep()
	{
	}

/**
 * @return - TVerdict code
 * Override of base class virtual
 */	
TVerdict CT_ServiceRegistryTestStep::doTestStepPreambleL()
	{
    CSisFileInstaller sisFileInstaller;
    INFO_PRINTF2(_L("Installing sis file from -> %S"), &KServerApp2SisFile);
    sisFileInstaller.InstallSisAndWaitForAppListUpdateL(KServerApp2SisFile);
    
	SetTestStepResult(EPass);
	return TestStepResult();
	}

/**
 * @return - TVerdict code
 * Override of base class virtual
 */
TVerdict CT_ServiceRegistryTestStep::doTestStepPostambleL()
	{
    CSisFileInstaller sisFileInstaller;
    sisFileInstaller.UninstallSisL(KServerApp2Component);
    
	return TestStepResult();
	}

TVerdict CT_ServiceRegistryTestStep::doTestStepL()
	{
	INFO_PRINTF1(_L("Testing Apparc...T_ServiceRegistry Test Cases Running..."));

	// start an active scheduler
	iScheduler=new(ELeave) CActiveScheduler();
	CActiveScheduler::Install(iScheduler);

	__UHEAP_MARK;
	TInt ret;
	TRAP(ret,RunTestCasesL())
	TEST(ret==KErrNone);
	__UHEAP_MARKEND;
	
	INFO_PRINTF1(_L("...T_ServiceRegistry Test Cases Completed."));

	return TestStepResult();
	}

TInt CT_ServiceRegistryTestStep::RunTestCasesL()
	{
	RTestableApaLsSession ls;
	TEST(KErrNone == ls.Connect());
	CleanupClosePushL(ls);
	
	TApaAppInfo info;
	TUid uid = {0x100048F3};
	TInt err = ls.GetAppInfo(info, uid);
	if(err == KErrNone)
        {       
        CSisFileInstaller sisFileInstaller;
        sisFileInstaller.UninstallSisL(KApparcTestAppComponent);
        } 

	// Use DONT_CHECK because it complaints of heap unbalance (a CTypeStoreManager object, althought it is not actually leaked,
	//   but reallocated in CApaAppListServer::DoUpdateTypeStoreL(void)). 
	//   This unbalance happens 'randonmly', possibly a timing issue, as the TypeStore refresh is performed in response to a callback from 
	//	 a CApaFsMonitor object, which introduces an extra 0.25 second delay before invoking the callback.
	//	 *** See DEF101056 ****
	HEAP_TEST_LS_SESSION(ls, 0, DONT_CHECK, TestAssociation1L(), NO_CLEANUP);
#ifndef SYMBIAN_UNIVERSAL_INSTALL_FRAMEWORK	
	HEAP_TEST_LS_SESSION(ls, 0, DONT_CHECK, TestAppForDataTypeAndServiceL(ls), NO_CLEANUP);
#endif	
    HEAP_TEST_LS_SESSION(ls, 0, DONT_CHECK, TestUpdateOfServiceRegistryOnAppUninstallationL(ls), NO_CLEANUP);
    HEAP_TEST_LS_SESSION(ls, 0, DONT_CHECK, TestServiceRegistryOnAppUpgradeL(ls), NO_CLEANUP);    
    
	CleanupStack::PopAndDestroy(&ls);	
	return KErrNone;
	}
	
/**
   @SYMTestCaseID T-ServiceRegistryTestStep-TestAssociation1L
  
   @SYMPREQ 
  
   @SYMTestCaseDesc Test the functionality of ServiceRegistry 
   
   @SYMTestPriority 
  
   @SYMTestStatus Implemented
   
   @SYMTestActions Tests the addition, retrieval and deletions of 
   associations
   
   @SYMTestExpectedResults The association are added, retrieved and deleted
   from the registry
 */
void CT_ServiceRegistryTestStep::TestAssociation1L()
	{
	INFO_PRINTF1(_L("TestAssociation1L"));
	CServiceRegistry* registry = CServiceRegistry::NewL();
	CleanupStack::PushL(registry);
	
	const TUid uid1 = { 0x11111111 };
	const TUid uid2 = { 0x22222222 };
	
	const TUid serviceUid1 = { 0x00001111 };
	const TUid serviceUid2 = { 0x00002222 };

	const TDataType data1(uid1);
	const TDataType data2(uid2);
	
	TUid result;
	
	TInt err = registry->GetDefault(serviceUid1, data1, result);
	TEST(err == KErrNotFound);
	TEST(result == KNullUid);
	
	err = registry->SetDefault(serviceUid1, data1, uid1);
	TEST(err == KErrNone);
	err = registry->SetDefault(serviceUid2, data2, uid2);
	TEST(err == KErrNone);
	
	err = registry->GetDefault(serviceUid1, data1, result);
	TEST(err == KErrNone);
	TEST(result == uid1);
	
	err = registry->GetDefault(serviceUid2, data2, result);
	TEST(err == KErrNone);
	TEST(result == uid2);
	
	err = registry->RemoveEntry(serviceUid1, data1);
	TEST(err == KErrNone);
	
	err = registry->RemoveEntry(serviceUid1, data1);
	TEST(err == KErrNotFound);
	
	err = registry->GetDefault(serviceUid1, data1, result);
	TEST(err == KErrNotFound);
	TEST(result == KNullUid);
	
	err = registry->GetDefault(serviceUid2, data2, result);
	TEST(err == KErrNone);
	TEST(result == uid2);
	
	err = registry->RemoveEntry(serviceUid2, data2);
	TEST(err == KErrNone);
	
	err = registry->GetDefault(serviceUid2, data2, result);
	TEST(err == KErrNotFound);
	TEST(result == KNullUid);
	
	CleanupStack::PopAndDestroy(registry);
	}
	
/**
   @SYMTestCaseID T-ServiceRegistryTestStep-TestAppForDataTypeAndServiceL
  
   @SYMPREQ 
  
   @SYMTestCaseDesc Test the functionality of ServiceRegistry 
   
   @SYMTestPriority 
  
   @SYMTestStatus Implemented
   
   @SYMTestActions Tests that the RApaLsSession::AppForDataTypeAndService
   function behaves correctly when associations are added to the registry
   
   @SYMTestExpectedResults The correct application is returned
 */
void CT_ServiceRegistryTestStep::TestAppForDataTypeAndServiceL(RApaLsSession& aLs)
	{
	INFO_PRINTF1(_L("TestAppForDataTypeAndServiceL"));

	const TUid KUidServerApp = {0x10004c56};
	const TUid KUidServerApp2 = {0x10004c58};
	const TUid KUidServerAppDummy = {0x12344268};
	const TUid KUidService01020333 = {0x01020333};
	_LIT8(KLitMimeTypePriority,"text/priority");

	CServiceRegistry* registry = CServiceRegistry::NewL();
	CleanupStack::PushL(registry);

	// Check first the default association (no association in registry)
	TUid appUid;
	TInt err = aLs.AppForDataTypeAndService(TDataType(KLitMimeTypePriority), KUidService01020333,appUid);
	TEST(err == KErrNone);
	TEST(appUid == KUidServerApp);

	// Check after having added an association in the registry
	err = registry->SetDefault(KUidService01020333, TDataType(KLitMimeTypePriority), KUidServerApp2);
	TEST(err == KErrNone);

	err = aLs.AppForDataTypeAndService(TDataType(KLitMimeTypePriority), KUidService01020333,appUid);
	TEST(err == KErrNone);
	TEST(appUid == KUidServerApp2);

	err = registry->RemoveEntry(KUidService01020333, TDataType(KLitMimeTypePriority));
	TEST(err == KErrNone);

	err = aLs.AppForDataTypeAndService(TDataType(KLitMimeTypePriority), KUidService01020333,appUid);
	TEST(err == KErrNone);
	TEST(appUid == KUidServerApp);

	// Check association with app that doesn't exist
	err = registry->SetDefault(KUidService01020333, TDataType(KLitMimeTypePriority), KUidServerAppDummy);
	TEST(err == KErrNone);

	err = aLs.AppForDataTypeAndService(TDataType(KLitMimeTypePriority), KUidService01020333,appUid);
	TEST(err == KErrNone);
	TEST(appUid == KUidServerApp);

	err = registry->RemoveEntry(KUidService01020333, TDataType(KLitMimeTypePriority));
	TEST(err == KErrNone);

	err = aLs.AppForDataTypeAndService(TDataType(KLitMimeTypePriority), KUidService01020333,appUid);
	TEST(err == KErrNone);
	TEST(appUid == KUidServerApp);

	CleanupStack::PopAndDestroy(registry);
	}


/**
   @SYMTestCaseID APPFWK-APPARC-0104
  
   @SYMDEF INC138417 
  
   @SYMTestCaseDesc Tests whether a data mapping of an applicaiton is removed from 
                    service registry if that applicaiton is uninstalled.
   
   @SYMTestPriority 
  
   @SYMTestStatus Implemented
   
   @SYMTestActions Inserts data mapping for an applicaion and uninstalls that application.
                   Tests whether the inserted data mapping is removed from the service registry. 
   
   @SYMTestExpectedResults The inserted data mapping is removed from the service registry.
 */

void CT_ServiceRegistryTestStep::TestUpdateOfServiceRegistryOnAppUninstallationL(RApaLsSession& aLs)   
    {
    INFO_PRINTF1(_L("TestUpdateOfServiceRegistryOnAppUninstallationL"));
    RSmlTestUtils fs;
    CleanupClosePushL(fs);
    User::LeaveIfError(fs.Connect());

    CSisFileInstaller sisFileInstaller;
    INFO_PRINTF2(_L("Installing sis file from -> %S"), &KApparcTestAppSisFile);
    sisFileInstaller.InstallSisAndWaitForAppListUpdateL(KApparcTestAppSisFile);
    
     CServiceRegistry* registry = CServiceRegistry::NewL();
     CleanupStack::PushL(registry);
     
     TUid appUid = {0x100048f3};
     TUid resultUid={KNullUidValue};    
     TDataType dataType (KLitMimeType);
     
     //Test whether 0x100048f3 application is in application list.
     TApaAppInfo appInfo;
     TEST(aLs.GetAppInfo(appInfo,appUid)==KErrNone);

     //Set 0x100048f3 as default application for "mime/updregappuninstall" MIME type.
     registry->SetDefault(KOpenServiceUid,dataType, appUid);
     registry->GetDefault(KOpenServiceUid,dataType, resultUid);
     TEST(appUid==resultUid);
 
     sisFileInstaller.UninstallSisAndWaitForAppListUpdateL(KApparcTestAppComponent);
     
     //Check the application is removed from the application list
     TEST(aLs.GetAppInfo(appInfo,appUid)==KErrNotFound);
     
     //Check data mappings related to uninstalled applicaiton is also removed.
     TEST(registry->GetDefault(KOpenServiceUid,dataType, resultUid)==KErrNotFound);

     CleanupStack::PopAndDestroy(registry);
     CleanupStack::PopAndDestroy(&fs);  
    }

void CTestApplistChangeObserver::HandleAppListEvent(TInt /*aEvent*/)
    {
    CActiveScheduler::Stop();    
    }

/**
   @SYMTestCaseID APPFWK-APPARC-0105
  
   @SYMDEF INC138417 
  
   @SYMTestCaseDesc Tests whether a data mapping of an applicaiton is not changed in 
                    service registry if that applicaiton is upgraded.
   
   @SYMTestPriority 
  
   @SYMTestStatus Implemented
   
   @SYMTestActions Inserts data mapping for an applicaion and upgrades that application with newer version.
                   Tests the existence of inserted data mapping in the service registry. 
   
   @SYMTestExpectedResults Inserted data mapping is not removed from the service registry.
 */

void CT_ServiceRegistryTestStep::TestServiceRegistryOnAppUpgradeL(RApaLsSession& aLs)   
    {
    INFO_PRINTF1(_L("TestServiceRegistryOnAppUpgrade"));
 
     CServiceRegistry* registry = CServiceRegistry::NewL();
     CleanupStack::PushL(registry);
     
     TUid appUid = {0xA};
     TUid resultUid={KNullUidValue};    
     TDataType dataType (KLitMimeType);
     
     //Test whether 0xA application is in application list.
     TApaAppInfo appInfo;
     TEST(aLs.GetAppInfo(appInfo,appUid)==KErrNone);

     //Set 0xA as default application for "mime/updregappuninstall" MIME type.
     registry->SetDefault(KOpenServiceUid,dataType, appUid);
     registry->GetDefault(KOpenServiceUid,dataType, resultUid);
     TEST(appUid==resultUid);
     
     CSisFileInstaller sisFIleInstaller;
     INFO_PRINTF2(_L("Installing sis file from -> %S"), &KTstAppStandAloneSisFile);
     sisFIleInstaller.InstallSisAndWaitForAppListUpdateL(KTstAppStandAloneSisFile);
     
     //Check the application is not removed from the application list
     TEST(aLs.GetAppInfo(appInfo,appUid)==KErrNone);
     
     //Check data mappings related to upgraded applicaiton is not removed.
     TEST(registry->GetDefault(KOpenServiceUid,dataType, resultUid)==KErrNone);
     TEST(resultUid==appUid);
     
     sisFIleInstaller.UninstallSisL(KTstAppStandAloneComponent);

     CleanupStack::PopAndDestroy(registry);
    }


//Deletes the file if it exists.
TInt CT_ServiceRegistryTestStep::DeleteFileL(RSmlTestUtils &aFs, const TDesC &aFileName)
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

//Wait till the application list is updated.
void CT_ServiceRegistryTestStep::WaitForAppListUpdateL()
    {
     CTestApplistChangeObserver* obs = new(ELeave) CTestApplistChangeObserver();
     CleanupStack::PushL(obs);
     CApaAppListNotifier* notif = CApaAppListNotifier::NewL(obs, CActive::EPriorityHigh);
     CleanupStack::PushL(notif);
     obs->iNotifier = notif;
     CActiveScheduler::Start();
     
     CleanupStack::PopAndDestroy(notif);  
     CleanupStack::PopAndDestroy(obs);     
     INFO_PRINTF1(_L("Applicaiton list is updated."));     
    }

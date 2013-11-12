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
// Tests UpdateAppList API of RApaLsSession class.
//

/**
 @file
 @internalComponent - Internal Symbian test code
*/

#include <apgcli.h>
#include "T_UpdateAppList.h"
#include "T_SisFileInstaller.h"

_LIT(KApparcTestAppSisFile, "z:\\apparctest\\apparctestsisfiles\\TApparcTestApp.sis");
_LIT(KApparcTestAppComponent, "TApparcTestApp");

_LIT(KTestMultipleAppsSisFile, "z:\\apparctest\\apparctestsisfiles\\TestMultipleApps.sis");
_LIT(KTestMultipleAppsComponent, "TestMultipleApps");


CT_TestUpdateAppListStep::CT_TestUpdateAppListStep()
    {
    }

CT_TestUpdateAppListStep::~CT_TestUpdateAppListStep()
    {
    iApaLsSession.Close();
    delete iScheduler;    
    }


void CT_TestUpdateAppListStep::TestUpdateAppListWithInvalidArgumentsL()
    {
    TApaAppInfo appInfo;
    TUid uid = {0x10003A3F};
    TInt ret = iApaLsSession.GetAppInfo(appInfo,uid);
    TEST(ret==KErrNotFound);
    
    RArray<TApaAppUpdateInfo> updateAppInfo;
    CleanupClosePushL(updateAppInfo);
    updateAppInfo.AppendL(TApaAppUpdateInfo(uid, TApaAppUpdateInfo::EAppPresent));
    iApaLsSession.UpdateAppListL(updateAppInfo);
    User::After(1000000);
    ret = iApaLsSession.GetAppInfo(appInfo,uid);
    TEST(ret==KErrNotFound);

    updateAppInfo.Reset();
    updateAppInfo.AppendL(TApaAppUpdateInfo(uid, TApaAppUpdateInfo::EAppNotPresent));
    iApaLsSession.UpdateAppListL(updateAppInfo);
    User::After(1000000);
    ret = iApaLsSession.GetAppInfo(appInfo,uid);
    TEST(ret==KErrNotFound);

    updateAppInfo.Reset();
    updateAppInfo.AppendL(TApaAppUpdateInfo(uid, TApaAppUpdateInfo::EAppInfoChanged));
    iApaLsSession.UpdateAppListL(updateAppInfo);
    User::After(3000000);
    ret = iApaLsSession.GetAppInfo(appInfo,uid);
    TEST(ret==KErrNotFound);
    CleanupStack::PopAndDestroy(&updateAppInfo);
    }


void CT_TestUpdateAppListStep::TestAppInstallAndUninstallationL()
    {
    TApaAppInfo appInfo;
    TUid uid = {0x100048F3};
    TInt ret = iApaLsSession.GetAppInfo(appInfo,uid);
    TEST(ret==KErrNotFound);

    //Install app
    CSisFileInstaller sisFileInstaller;
    INFO_PRINTF2(_L("Installing sis file from -> %S"), &KApparcTestAppSisFile);
    TRAPD(err, sisFileInstaller.InstallSisAndWaitForAppListUpdateL(KApparcTestAppSisFile));
    
    ret = iApaLsSession.GetAppInfo(appInfo,uid);
    TEST(ret==KErrNone);

    //Uninstall app
    TRAP(err, sisFileInstaller.UninstallSisAndWaitForAppListUpdateL(KApparcTestAppComponent));
    
    ret = iApaLsSession.GetAppInfo(appInfo,uid);
    TEST(ret==KErrNotFound);
    
    }


void CT_TestUpdateAppListStep::TestMultipleAppInstallAndUninstallationL()
    {
    TApaAppInfo appInfo;
    TUid uid1 = {0x102032AB};
    TUid uid2 = {0x10208183};  
    TUid uid3 = {0x10208184};  
    
    TInt ret = iApaLsSession.GetAppInfo(appInfo,uid1);
    TEST(ret==KErrNotFound);

    ret = iApaLsSession.GetAppInfo(appInfo,uid2);
    TEST(ret==KErrNotFound);

    ret = iApaLsSession.GetAppInfo(appInfo,uid3);
    TEST(ret==KErrNotFound);

    //Install applications
    CSisFileInstaller sisFileInstaller;
    INFO_PRINTF2(_L("Installing sis file from -> %S"), &KTestMultipleAppsSisFile);
    sisFileInstaller.InstallSisAndWaitForAppListUpdateL(KTestMultipleAppsSisFile);
    
    ret = iApaLsSession.GetAppInfo(appInfo,uid1);
    TEST(ret==KErrNone);
   
    ret = iApaLsSession.GetAppInfo(appInfo,uid2);
    TEST(ret==KErrNone);

    //uninstall applications
    sisFileInstaller.UninstallSisAndWaitForAppListUpdateL(KTestMultipleAppsComponent);
    
   
    ret = iApaLsSession.GetAppInfo(appInfo,uid1);
    TEST(ret==KErrNotFound);

    ret = iApaLsSession.GetAppInfo(appInfo,uid2);
    TEST(ret==KErrNotFound);
    
    ret = iApaLsSession.GetAppInfo(appInfo,uid3);
    TEST(ret==KErrNotFound);
    }


TVerdict CT_TestUpdateAppListStep::doTestStepL()
    {
    INFO_PRINTF1(_L("Test T_UpdateAppList Started"));
    
    // start an active scheduler
    iScheduler=new(ELeave) CActiveScheduler();
    CActiveScheduler::Install(iScheduler);
    
    // Connect to RApaLsSession
    User::LeaveIfError(iApaLsSession.Connect());
    
    HEAP_TEST_LS_SESSION(iApaLsSession, 0, DONT_CHECK, TestUpdateAppListWithInvalidArgumentsL(), NO_CLEANUP);
    HEAP_TEST_LS_SESSION(iApaLsSession, 0, DONT_CHECK, TestAppInstallAndUninstallationL(), NO_CLEANUP);
    HEAP_TEST_LS_SESSION(iApaLsSession, 0, DONT_CHECK, TestMultipleAppInstallAndUninstallationL(), NO_CLEANUP);    
    
    INFO_PRINTF1(_L("Test Finished"));  
    return TestStepResult();
    }

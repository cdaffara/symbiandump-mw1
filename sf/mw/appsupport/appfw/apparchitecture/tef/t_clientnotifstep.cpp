// Copyright (c) 2010 Nokia Corporation and/or its subsidiary(-ies).
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
 @internalComponent - Internal Symbian test code 
*/

#include "t_clientnotifstep.h"
#include <e32base.h>
#include <e32cmn.h>
#include "T_SisFileInstaller.h"
#include <apgtask.h>
#include <apgnotif.h>

_LIT(KSimpleAppSisFile, "z:\\apparctest\\apparctestsisfiles\\SimpleApparcTestApp.sis");
_LIT(KSimpleAppComponent, "SimpleApparcTestApp");

_LIT(KTestMultipleAppsSisFile, "z:\\apparctest\\apparctestsisfiles\\TestMultipleApps.sis");
_LIT(KTestMultipleAppsComponent, "TestMultipleApps");

_LIT(KTestMultipleAppsDowngradeSisFile, "z:\\apparctest\\apparctestsisfiles\\TestMultipleAppsDowngrade.sis");
_LIT(KTestMultipleAppsDowngradeComponent, "TestMultipleApps");

_LIT(KTstAppStandAloneSisFile, "z:\\apparctest\\apparctestsisfiles\\TSTAPP_standalone.sis");
_LIT(KTstAppStandAloneComponent, "TSTAPP_standalone");

const TUid KUidSimpleApp={0x12008ACE};
const TUid KUidMultipleApp1={0x102032AB};
const TUid KUidMultipleApp2={0x10208183};
const TUid KUidMultipleApp3={0x10208184};
const TUid KUidTstAppStandalone={10};

CT_ClientNotifStep::~CT_ClientNotifStep()
/**
   Destructor
 */
    {
    }

CT_ClientNotifStep::CT_ClientNotifStep()
/**
   Constructor
 */
    {
    // Call base class method to set up the human readable name for logging
    SetTestStepName(KT_ClientNotifStep);
    }

TVerdict CT_ClientNotifStep::doTestStepPreambleL()
/**
   @return - TVerdict code
   Override of base class virtual
 */
    {
    SetTestStepResult(EPass);
    return TestStepResult();
    }

TVerdict CT_ClientNotifStep::doTestStepPostambleL()
/**
   @return - TVerdict code
   Override of base class virtual
 */
    {
    return TestStepResult();
    }
    
    
TVerdict CT_ClientNotifStep::doTestStepL()
{
    RunTestCases();
    return TestStepResult();
}


void CT_ClientNotifStep::RunTestCases()
    {
    RTestableApaLsSession ls;
    User::LeaveIfError(ls.Connect());
    
    TApaAppInfo info;
    TUid uid = {0x12008ACE};     
    TInt err = ls.GetAppInfo(info, uid);
    if(err == KErrNone)
        {       
        CSisFileInstaller sisFileInstaller;
        sisFileInstaller.UninstallSisL(KSimpleAppComponent);
        }
   
    //DONT_CHECK is used because when an application is installed the updated application
    //information is added in the session objects. This causes increase of memory at server side.
    //As there can be multiple applications registered with apparc for applist change notifications,
    //its not possible to clear the information in all the session objects.
    HEAP_TEST_LS_SESSION(ls, 0, DONT_CHECK, TestClientNotifInitialApplistCreation(ls), NO_CLEANUP);
    HEAP_TEST_LS_SESSION(ls, 0, DONT_CHECK, TestClientNotifLanguageChange(ls), NO_CLEANUP);     
    HEAP_TEST_LS_SESSION(ls, 0, DONT_CHECK, TestClientNotifSingleAppInstallation(ls), NO_CLEANUP);
    HEAP_TEST_LS_SESSION(ls, 0, DONT_CHECK, TestClientNotifMultipleAppInstallation(ls), NO_CLEANUP);
    HEAP_TEST_LS_SESSION(ls, 0, DONT_CHECK, TestClientNotifUpgradeApp(ls), NO_CLEANUP);
    HEAP_TEST_LS_SESSION(ls, 0, DONT_CHECK, TestClientNotifMultipleInstallations(ls), NO_CLEANUP);
    HEAP_TEST_LS_SESSION(ls, 0, DONT_CHECK, TestClientNotifWithoutSetNotify(ls), NO_CLEANUP);
    HEAP_TEST_LS_SESSION(ls, 0, DONT_CHECK, TestClientNotifOnPackageUpgrade(ls), NO_CLEANUP);  
    HEAP_TEST_LS_SESSION(ls, 0, DONT_CHECK, TestClientNotifOnPackageDowngrade(ls), NO_CLEANUP);        
    ls.Close();
    }


/**
   @SYMTestCaseID           APPFWK-APPARC-0106
  

    @SYMTestCaseDesc        Tests apparc will not provide updated application information when the application list
                            created first time after device bootup.
                            
   @SYMTestPriority         High
  
   @SYMTestStatus           Implemented
   
   @SYMTestActions          1. Wait till the applist got created after phone boot.
                            2. Retrieve the updated application information and check it is empty.
                            
                             
   @SYMTestExpectedResults Apparc will not provide updated application information.
 */
void CT_ClientNotifStep::TestClientNotifInitialApplistCreation(RTestableApaLsSession &aLs)
    {
    INFO_PRINTF1(_L("TestClientNotifInitialApplistCreation started............"));    
  
    TRequestStatus applistChangeStatus;
    TRequestStatus applistCreationStatus;
    //Register with apparc for applist change notification
    aLs.SetNotify(ETrue, applistChangeStatus);
    aLs.RegisterListPopulationCompleteObserver(applistCreationStatus);    
    User::WaitForRequest(applistCreationStatus);
    User::WaitForRequest(applistChangeStatus);
    
    //Get the updated application information from apparc.
    RArray<TApaAppUpdateInfo> updatedAppsInfo;
    CleanupClosePushL(updatedAppsInfo);
    INFO_PRINTF1(_L("Retrieving updated application information after installation"));    
    aLs.UpdatedAppsInfoL(updatedAppsInfo);
    INFO_PRINTF2(_L("Updated application count: %d expected: 0"), updatedAppsInfo.Count()); 
    TEST(updatedAppsInfo.Count() == 0);
    
    CleanupStack::PopAndDestroy(); //updatedAppsInfo   
    INFO_PRINTF1(_L("TestClientNotifInitialApplistCreation ended............")); 
    }


void CT_ClientNotifStep::ChangeLocaleL(TLanguage aLanguage)
    {
    _LIT(KLitLanguageLocaleDllNameBase, "elocl_lan");
    //Region and collation code values are hard coded, as the check, after changing the locale is made for the language only.
    _LIT(KLitRegionLocaleDllNameBase, "elocl_reg.826");        
    _LIT(KLitCollationLocaleDllNameBase, "elocl_col.001");
    _LIT(ThreeDigExt,".%03d");
    TExtendedLocale localeDll;    
    const TUidType uidType(TUid::Uid(0x10000079),TUid::Uid(0x100039e6));
    TBuf<16> languageLocaleDllName(KLitLanguageLocaleDllNameBase);  
    languageLocaleDllName.AppendFormat(ThreeDigExt, aLanguage);
    TBuf<16> regionLocaleDllName(KLitRegionLocaleDllNameBase);  
    TBuf<16> collationLocaleDllName(KLitCollationLocaleDllNameBase);  
    // Try to load the locale dll
    TInt error=localeDll.LoadLocale(languageLocaleDllName, regionLocaleDllName, collationLocaleDllName);
        
    if (error==KErrNotFound)
        {
        // Locale dll is not found for the asked language. 
        ERR_PRINTF2(_L("Failed to find the locale dll for %d"), aLanguage);
        }
           
    User::LeaveIfError(error);
    localeDll.SaveSystemSettings();
    }



/**
   @SYMTestCaseID           APPFWK-APPARC-0106
  

    @SYMTestCaseDesc        Tests apparc will not provide updated application information when the application list
                            updated due phone language change.
                            
   @SYMTestPriority         High
  
   @SYMTestStatus           Implemented
   
   @SYMTestActions          1. Wait till the applist got updated after phone language change.
                            2. Retrieve the updated application information and check it is empty.
                            
                             
   @SYMTestExpectedResults Apparc will not provide updated application information.
 */
void CT_ClientNotifStep::TestClientNotifLanguageChange(RTestableApaLsSession &aLs)
    {
    INFO_PRINTF1(_L("TestClientNotifLanguageChange started............"));    
  
    TRequestStatus applistChangeStatus;
    aLs.SetNotify(EFalse, applistChangeStatus);
    //Change language
    ChangeLocaleL(ELangGerman);    
    User::WaitForRequest(applistChangeStatus);

    //Get the updated application information from apparc.
    RArray<TApaAppUpdateInfo> updatedAppsInfo;
    CleanupClosePushL(updatedAppsInfo);
    INFO_PRINTF1(_L("Retrieving updated application information after installation"));    
    aLs.UpdatedAppsInfoL(updatedAppsInfo);
    INFO_PRINTF2(_L("Updated application count: %d"), updatedAppsInfo.Count()); 
    TEST(updatedAppsInfo.Count() == 0);
    
    applistChangeStatus=KRequestPending;
    aLs.SetNotify(EFalse, applistChangeStatus);
    ChangeLocaleL(ELangEnglish);
    User::WaitForRequest(applistChangeStatus);
    
    CleanupStack::PopAndDestroy(); //updatedAppsInfo   
    INFO_PRINTF1(_L("TestClientNotifInitialApplistCreation ended............")); 
    }

/**
   @SYMTestCaseID           APPFWK-APPARC-0106
  

    @SYMTestCaseDesc        Tests whether apparc provides updated application information when single application is installed 
                            and uninstalled.
  
   @SYMTestPriority         High
  
   @SYMTestStatus           Implemented
   
   @SYMTestActions          1. Installs a sis file which has single application and waits till applist is changed.
                            2. Checks the updated information provided by apparc is as expected.
                            3. Uninstalls the component installed in Step 1
                            4. Checks the updated information provided by apparc is as expected.
                            
                             
   @SYMTestExpectedResults Apparc provides updated application information.,
 */
void CT_ClientNotifStep::TestClientNotifSingleAppInstallation(RTestableApaLsSession &aLs)
    {
    INFO_PRINTF1(_L("TestSingleAppInstallation started............"));    
    CSisFileInstaller sisInstaller;
    TRequestStatus applistChangeStatus;
 
    INFO_PRINTF1(_L("Installing single application"));     
    aLs.SetNotify(EFalse, applistChangeStatus);
    INFO_PRINTF2(_L("Installing sis file from -> %S"), &KSimpleAppSisFile);
    sisInstaller.InstallSisL(KSimpleAppSisFile);
    User::WaitForRequest(applistChangeStatus);
    
    //Get the updated application information from apparc.
    RArray<TApaAppUpdateInfo> updatedAppsInfo;
    CleanupClosePushL(updatedAppsInfo);
    INFO_PRINTF1(_L("Retrieving updated application information after installation"));    
    aLs.UpdatedAppsInfoL(updatedAppsInfo);
    INFO_PRINTF2(_L("Updated application count: %d"), updatedAppsInfo.Count()); 
    
    //Check the information provided by apparc is what is expected.
    TEST(updatedAppsInfo.Count()== 1);
    TEST(updatedAppsInfo[0].iAppUid == KUidSimpleApp);
    TEST(updatedAppsInfo[0].iAction == TApaAppUpdateInfo::EAppPresent);
    
    //Check the application is available in the applist. 
    TApaAppInfo appInfo;
    TEST(aLs.GetAppInfo(appInfo, KUidSimpleApp) == KErrNone);
    
    INFO_PRINTF1(_L("Unnstalling single application"));        
    applistChangeStatus=KRequestPending;
    aLs.SetNotify(EFalse, applistChangeStatus);    
    sisInstaller.UninstallSisL(KSimpleAppComponent);
    User::WaitForRequest(applistChangeStatus);  
    
    //Get the updated application information from apparc.    
    updatedAppsInfo.Reset();
    INFO_PRINTF1(_L("Retrieving updated application information after uninstallation"));    
    aLs.UpdatedAppsInfoL(updatedAppsInfo);
    INFO_PRINTF2(_L("Updated application count: %d"), updatedAppsInfo.Count());
    
    //Check the information provided by apparc is what is expected.    
    TEST(updatedAppsInfo.Count()== 1);
    TEST(updatedAppsInfo[0].iAppUid == KUidSimpleApp);
    TEST(updatedAppsInfo[0].iAction == TApaAppUpdateInfo::EAppNotPresent);
    
    //Check the application is not available in the applist.    
    TEST(aLs.GetAppInfo(appInfo, KUidSimpleApp) == KErrNotFound);
    
    CleanupStack::PopAndDestroy(); //updatedAppsInfo
    INFO_PRINTF1(_L("TestSingleAppInstallation ended............")); 
    }


/**
   @SYMTestCaseID           APPFWK-APPARC-0106
  

    @SYMTestCaseDesc        Tests whether apparc provides updated application information when multiple applications are installed 
                            and uninstalled.
  
   @SYMTestPriority         High
  
   @SYMTestStatus           Implemented
   
   @SYMTestActions          1. Installs a sis file which has multiple applications and waits till applist is changed.
                            2. Checks the updated information provided by apparc is as expected.
                            3. Uninstalls the component installed in Step 1
                            4. Checks the updated information provided by apparc is as expected.
                            
                             
   @SYMTestExpectedResults Apparc provides updated application information.
  */
   
void CT_ClientNotifStep::TestClientNotifMultipleAppInstallation(RTestableApaLsSession &aLs)
    {
    INFO_PRINTF1(_L("TestMultipleAppInstallation started............"));
    CSisFileInstaller sisInstaller;
    TRequestStatus applistChangeStatus;
    
    INFO_PRINTF1(_L("Installing multiple applications"));     
    aLs.SetNotify(EFalse, applistChangeStatus);
    INFO_PRINTF2(_L("Installing sis file from -> %S"), &KTestMultipleAppsSisFile);
    sisInstaller.InstallSisL(KTestMultipleAppsSisFile);
    User::WaitForRequest(applistChangeStatus);
 
    //Get the updated application information from apparc.    
    RArray<TApaAppUpdateInfo> updatedAppsInfo;
    CleanupClosePushL(updatedAppsInfo);    
    INFO_PRINTF1(_L("Retrieving updated application information after installation"));
    aLs.UpdatedAppsInfoL(updatedAppsInfo);
    INFO_PRINTF2(_L("Updated application count: %d"), updatedAppsInfo.Count()); 
    
    //Check the information provided by apparc is what is expected.    
    TEST(updatedAppsInfo.Count()== 3);
    TEST(updatedAppsInfo[0].iAppUid == KUidMultipleApp1);
    TEST(updatedAppsInfo[0].iAction == TApaAppUpdateInfo::EAppPresent);
    TEST(updatedAppsInfo[1].iAppUid == KUidMultipleApp2);
    TEST(updatedAppsInfo[1].iAction == TApaAppUpdateInfo::EAppPresent);    
    TEST(updatedAppsInfo[2].iAppUid == KUidMultipleApp3);
    TEST(updatedAppsInfo[2].iAction == TApaAppUpdateInfo::EAppPresent);
    
    //Check the application is available in the applist.    
    TApaAppInfo appInfo;
    TEST(aLs.GetAppInfo(appInfo, KUidMultipleApp1) == KErrNone);
    TEST(aLs.GetAppInfo(appInfo, KUidMultipleApp2) == KErrNone);
    TEST(aLs.GetAppInfo(appInfo, KUidMultipleApp3) == KErrNone);
    
    INFO_PRINTF1(_L("Uninstalling multiple applications"));    
    applistChangeStatus=KRequestPending;
    aLs.SetNotify(EFalse, applistChangeStatus);    
    sisInstaller.UninstallSisL(KTestMultipleAppsComponent);
    User::WaitForRequest(applistChangeStatus); 
    
    //Get the updated application information from apparc.    
    updatedAppsInfo.Reset();
    INFO_PRINTF1(_L("Retrieving updated application information after uninstallation"));    
    aLs.UpdatedAppsInfoL(updatedAppsInfo);
    INFO_PRINTF2(_L("Updated application count: %d"), updatedAppsInfo.Count());
    TEST(updatedAppsInfo.Count()== 3);
    
    //Check the information provided by apparc is what is expected.    
    TEST(updatedAppsInfo[0].iAppUid == KUidMultipleApp1);
    TEST(updatedAppsInfo[0].iAction == TApaAppUpdateInfo::EAppNotPresent);
    TEST(updatedAppsInfo[1].iAppUid == KUidMultipleApp2);
    TEST(updatedAppsInfo[1].iAction == TApaAppUpdateInfo::EAppNotPresent);    
    TEST(updatedAppsInfo[2].iAppUid == KUidMultipleApp3);
    TEST(updatedAppsInfo[2].iAction == TApaAppUpdateInfo::EAppNotPresent);
    
    //Check the application is not available in the applist.     
    TEST(aLs.GetAppInfo(appInfo, KUidMultipleApp1) == KErrNotFound);
    TEST(aLs.GetAppInfo(appInfo, KUidMultipleApp2) == KErrNotFound);
    TEST(aLs.GetAppInfo(appInfo, KUidMultipleApp3) == KErrNotFound);
    
    CleanupStack::PopAndDestroy(); //updatedAppsInfo
    INFO_PRINTF1(_L("TestMultipleAppInstallation ended............"));    
    }


/**
   @SYMTestCaseID           APPFWK-APPARC-0106
  

    @SYMTestCaseDesc        Tests whether apparc provides updated application information when an applications is
                            upgraded.
  
   @SYMTestPriority         High
  
   @SYMTestStatus           Implemented
   
   @SYMTestActions          1. Checks whether upgrading application exists in applist.
                            2. Installs a sis file which upgrades  existing applications and waits till applist is changed.
                            2. Checks the updated information provided by apparc is as expected.
                            3. Uninstalls the component installed in Step 2
                            4. Checks the updated information provided by apparc is as expected.
                            
                             
   @SYMTestExpectedResults Apparc provides updated application information.
  */
void CT_ClientNotifStep::TestClientNotifUpgradeApp(RTestableApaLsSession &aLs)
{
    INFO_PRINTF1(_L("TestClientNotifUpgradeApp started............"));    
    CSisFileInstaller sisInstaller;
    TRequestStatus applistChangeStatus;
 
    INFO_PRINTF1(_L("Check the application is present before upgrade")); 
    //Check the application is available in the applist. 
    TApaAppInfo appInfo;
    TEST(aLs.GetAppInfo(appInfo, KUidTstAppStandalone) == KErrNone);
    
    INFO_PRINTF1(_L("Installing upgrade application"));     
    aLs.SetNotify(EFalse, applistChangeStatus);
    INFO_PRINTF2(_L("Installing sis file from -> %S"), &KTstAppStandAloneSisFile);
    sisInstaller.InstallSisL(KTstAppStandAloneSisFile);
    User::WaitForRequest(applistChangeStatus);
    
    //Get the updated application information from apparc.
    RArray<TApaAppUpdateInfo> updatedAppsInfo;
    CleanupClosePushL(updatedAppsInfo);
    INFO_PRINTF1(_L("Retrieving updated application information after installation"));    
    aLs.UpdatedAppsInfoL(updatedAppsInfo);
    INFO_PRINTF2(_L("Updated application count: %d"), updatedAppsInfo.Count()); 
    
    //Check the information provided by apparc is what is expected.
    TEST(updatedAppsInfo.Count()== 1);
    TEST(updatedAppsInfo[0].iAppUid == KUidTstAppStandalone);
    TEST(updatedAppsInfo[0].iAction == TApaAppUpdateInfo::EAppPresent);
    
    //Check the application is available in the applist. 
    TEST(aLs.GetAppInfo(appInfo, KUidTstAppStandalone) == KErrNone);
    
    INFO_PRINTF1(_L("Unnstalling upgrade application"));        
    applistChangeStatus=KRequestPending;
    aLs.SetNotify(EFalse, applistChangeStatus);    
    sisInstaller.UninstallSisL(KTstAppStandAloneComponent);
    User::WaitForRequest(applistChangeStatus);  
    
    //Get the updated application information from apparc.    
    updatedAppsInfo.Reset();
    INFO_PRINTF1(_L("Retrieving updated application information after uninstallation"));    
    aLs.UpdatedAppsInfoL(updatedAppsInfo);
    INFO_PRINTF2(_L("Updated application count: %d"), updatedAppsInfo.Count());
    
    //Check the information provided by apparc is what is expected.    
    TEST(updatedAppsInfo.Count()== 1);
    TEST(updatedAppsInfo[0].iAppUid == KUidTstAppStandalone);
    TEST(updatedAppsInfo[0].iAction == TApaAppUpdateInfo::EAppPresent);
    
    //Check the application is still available in the applist.    
    TEST(aLs.GetAppInfo(appInfo, KUidTstAppStandalone) == KErrNone);
    
    CleanupStack::PopAndDestroy(); //updatedAppsInfo
    INFO_PRINTF1(_L("TestClientNotifUpgradeApp ended............"));     
}


/**
   @SYMTestCaseID           APPFWK-APPARC-0106
  

    @SYMTestCaseDesc        Tests whether apparc provides updated application information when there are multiple installations
                            happened before the clients requests for updated application information.
  
   @SYMTestPriority         High
  
   @SYMTestStatus           Implemented
   
   @SYMTestActions          1. Installs a sis file and waits till applist is changed.
                            2. Installs another sis file and waits till applist is changed.
                            3. Checks the updated information provided by apparc is as expected.
                            4. Uninstalls the component installed in Step 1 and waits till applist is changed.
                            5. Uninstalls the component installed in Step 2 and waits till applist is changed.                            
                            6. Checks the updated information provided by apparc is as expected.
                            
                             
   @SYMTestExpectedResults Apparc provides updated application information.
  */
void CT_ClientNotifStep::TestClientNotifMultipleInstallations(RTestableApaLsSession &aLs)
{
    INFO_PRINTF1(_L("TestClientNotifMultipleInstallations started............"));    
    CSisFileInstaller sisInstaller;
    TRequestStatus applistChangeStatus;
 
    INFO_PRINTF1(_L("Installing first sis file"));     
    aLs.SetNotify(EFalse, applistChangeStatus);
    INFO_PRINTF2(_L("Installing sis file from -> %S"), &KSimpleAppSisFile);
    sisInstaller.InstallSisL(KSimpleAppSisFile);
    User::WaitForRequest(applistChangeStatus);
    
    INFO_PRINTF1(_L("Installing second sis file")); 
    applistChangeStatus=KRequestPending;    
    aLs.SetNotify(EFalse, applistChangeStatus);
    INFO_PRINTF2(_L("Installing sis file from -> %S"), &KTestMultipleAppsSisFile);
    sisInstaller.InstallSisL(KTestMultipleAppsSisFile);
    User::WaitForRequest(applistChangeStatus);
    
    //Get the updated application information from apparc.
    RArray<TApaAppUpdateInfo> updatedAppsInfo;
    CleanupClosePushL(updatedAppsInfo);
    INFO_PRINTF1(_L("Retrieving updated application information after installations"));    
    aLs.UpdatedAppsInfoL(updatedAppsInfo);
    INFO_PRINTF2(_L("Updated application count: %d"), updatedAppsInfo.Count()); 
    
    //Check the information provided by apparc is what is expected.
    TEST(updatedAppsInfo.Count()== 4);
    TEST(updatedAppsInfo[0].iAppUid == KUidSimpleApp);
    TEST(updatedAppsInfo[0].iAction == TApaAppUpdateInfo::EAppPresent);
    TEST(updatedAppsInfo[1].iAppUid == KUidMultipleApp1);
    TEST(updatedAppsInfo[1].iAction == TApaAppUpdateInfo::EAppPresent);
    TEST(updatedAppsInfo[2].iAppUid == KUidMultipleApp2);
    TEST(updatedAppsInfo[2].iAction == TApaAppUpdateInfo::EAppPresent);
    TEST(updatedAppsInfo[3].iAppUid == KUidMultipleApp3);
    TEST(updatedAppsInfo[2].iAction == TApaAppUpdateInfo::EAppPresent);
    
    //Check the applications is available in the applist. 
    TApaAppInfo appInfo;    
    TEST(aLs.GetAppInfo(appInfo, KUidSimpleApp) == KErrNone);
    TEST(aLs.GetAppInfo(appInfo, KUidMultipleApp1) == KErrNone);
    TEST(aLs.GetAppInfo(appInfo, KUidMultipleApp2) == KErrNone);
    TEST(aLs.GetAppInfo(appInfo, KUidMultipleApp3) == KErrNone);
    
    INFO_PRINTF1(_L("Unnstalling first component"));        
    applistChangeStatus=KRequestPending;
    aLs.SetNotify(EFalse, applistChangeStatus);    
    sisInstaller.UninstallSisL(KSimpleAppComponent);
    User::WaitForRequest(applistChangeStatus);  
    
    INFO_PRINTF1(_L("Unnstalling second component"));        
    applistChangeStatus=KRequestPending;
    aLs.SetNotify(EFalse, applistChangeStatus);    
    sisInstaller.UninstallSisL(KTestMultipleAppsComponent);
    User::WaitForRequest(applistChangeStatus);  
    
    //Get the updated application information from apparc.    
    updatedAppsInfo.Reset();
    INFO_PRINTF1(_L("Retrieving updated application information after uninstallation"));    
    aLs.UpdatedAppsInfoL(updatedAppsInfo);
    INFO_PRINTF2(_L("Updated application count: %d"), updatedAppsInfo.Count());
    
    //Check the information provided by apparc is what is expected.    
    TEST(updatedAppsInfo.Count()== 4);
    TEST(updatedAppsInfo[0].iAppUid == KUidSimpleApp);
    TEST(updatedAppsInfo[0].iAction == TApaAppUpdateInfo::EAppNotPresent);
    TEST(updatedAppsInfo[1].iAppUid == KUidMultipleApp1);
    TEST(updatedAppsInfo[1].iAction == TApaAppUpdateInfo::EAppNotPresent);
    TEST(updatedAppsInfo[2].iAppUid == KUidMultipleApp2);
    TEST(updatedAppsInfo[2].iAction == TApaAppUpdateInfo::EAppNotPresent);
    TEST(updatedAppsInfo[3].iAppUid == KUidMultipleApp3);
    TEST(updatedAppsInfo[3].iAction == TApaAppUpdateInfo::EAppNotPresent);
    
    //Check the applications are not available in the applist. 
    TEST(aLs.GetAppInfo(appInfo, KUidSimpleApp) == KErrNotFound);
    TEST(aLs.GetAppInfo(appInfo, KUidMultipleApp1) == KErrNotFound);
    TEST(aLs.GetAppInfo(appInfo, KUidMultipleApp2) == KErrNotFound);
    TEST(aLs.GetAppInfo(appInfo, KUidMultipleApp3) == KErrNotFound);
    
    CleanupStack::PopAndDestroy(); //updatedAppsInfo
    INFO_PRINTF1(_L("TestClientNotifMultipleInstallations ended............"));     
}


/**
   @SYMTestCaseID           APPFWK-APPARC-0106
  

    @SYMTestCaseDesc        Tests apparc will not provide updated application information if SetNotify is not called
                            on an apparc session. Apparc only maintains updated application information with the sessions
                            which actually requested SetNotify and not yet called UpdatedAppsInfoL API.
                            
   @SYMTestPriority         High
  
   @SYMTestStatus           Implemented
   
   @SYMTestActions          1. Installs a sis file which has single application and waits till applist is changed.
                            2. Retrieves the updated applist from different apparc session and checks the list is empty.
                            
                             
   @SYMTestExpectedResults Apparc provides updated application information.
 */
void CT_ClientNotifStep::TestClientNotifWithoutSetNotify(RTestableApaLsSession &aLs)
    {
    INFO_PRINTF1(_L("TestClientNotifWithoutSetNotify started............"));    
    CSisFileInstaller sisInstaller;
    TRequestStatus applistChangeStatus;
 
    INFO_PRINTF1(_L("Installing application"));     
    sisInstaller.InstallSisAndWaitForAppListUpdateL(KSimpleAppSisFile);
    
    //Get the updated application information from apparc.
    RArray<TApaAppUpdateInfo> updatedAppsInfo;
    CleanupClosePushL(updatedAppsInfo);
    INFO_PRINTF1(_L("Retrieving updated application information after installation"));    
    aLs.UpdatedAppsInfoL(updatedAppsInfo);
    INFO_PRINTF2(_L("Updated application count: %d"), updatedAppsInfo.Count()); 
    
    //Check the information provided by apparc is what is expected.
    TEST(updatedAppsInfo.Count()== 0);
    
    INFO_PRINTF1(_L("Unnstalling application"));        
    sisInstaller.UninstallSisAndWaitForAppListUpdateL(KSimpleAppComponent);
    
    //Get the updated application information from apparc.    
    updatedAppsInfo.Reset();
    INFO_PRINTF1(_L("Retrieving updated application information after uninstallation"));    
    aLs.UpdatedAppsInfoL(updatedAppsInfo);
    INFO_PRINTF2(_L("Updated application count: %d"), updatedAppsInfo.Count());
    
    //Check the information provided by apparc is what is expected.    
    TEST(updatedAppsInfo.Count()== 0);
    
    CleanupStack::PopAndDestroy(); //updatedAppsInfo
    INFO_PRINTF1(_L("TestClientNotifWithoutSetNotify ended............")); 
    }


/**
   @SYMTestCaseID           APPFWK-APPARC-0106
  

    @SYMTestCaseDesc        Tests whether apparc provides updated application information a component is upgraded.
  
   @SYMTestPriority         High
  
   @SYMTestStatus           Implemented
   
   @SYMTestActions          1. Installs a sis file which has 2 applications and waits till applist is changed.
                            2. Checks the updated information provided by apparc is as expected.
                            3. Upgrades the component which is installed in Step 1 which has one more application.
                            4. Checks the updated information provided by apparc is as expected.
                            3. Uninstalls the component installed in Step 3
                            4. Checks the updated information provided by apparc is as expected.
                            
                             
   @SYMTestExpectedResults Apparc provides updated application information.
  */
   
void CT_ClientNotifStep::TestClientNotifOnPackageUpgrade(RTestableApaLsSession &aLs)
    {
    INFO_PRINTF1(_L("TestClientNotifOnPackageUpgrade started............"));
    CSisFileInstaller sisInstaller;
    TRequestStatus applistChangeStatus;
    
    INFO_PRINTF1(_L("Installing sis file which has 2 applications"));     
    aLs.SetNotify(EFalse, applistChangeStatus);
    INFO_PRINTF2(_L("Installing sis file from -> %S"), &KTestMultipleAppsDowngradeSisFile);
    sisInstaller.InstallSisL(KTestMultipleAppsDowngradeSisFile);
    User::WaitForRequest(applistChangeStatus);
 
    //Get the updated application information from apparc.    
    RArray<TApaAppUpdateInfo> updatedAppsInfo;
    CleanupClosePushL(updatedAppsInfo);    
    INFO_PRINTF1(_L("Retrieving updated application information after installation"));
    aLs.UpdatedAppsInfoL(updatedAppsInfo);
    INFO_PRINTF2(_L("Updated application count: %d"), updatedAppsInfo.Count()); 
    
    //Check the information provided by apparc is what is expected.    
    TEST(updatedAppsInfo.Count()== 2);
    TEST(updatedAppsInfo[0].iAppUid == KUidMultipleApp2);
    TEST(updatedAppsInfo[0].iAction == TApaAppUpdateInfo::EAppPresent);    
    TEST(updatedAppsInfo[1].iAppUid == KUidMultipleApp3);
    TEST(updatedAppsInfo[1].iAction == TApaAppUpdateInfo::EAppPresent);
    
    //Check the application is available in the applist.    
    TApaAppInfo appInfo;
    TEST(aLs.GetAppInfo(appInfo, KUidMultipleApp2) == KErrNone);
    TEST(aLs.GetAppInfo(appInfo, KUidMultipleApp3) == KErrNone);
    
 
    INFO_PRINTF1(_L("Installing upgrade sis file which has 3 applications"));   
    applistChangeStatus=KRequestPending;    
    aLs.SetNotify(EFalse, applistChangeStatus);
    INFO_PRINTF2(_L("Installing sis file from -> %S"), &KTestMultipleAppsSisFile);
    sisInstaller.InstallSisL(KTestMultipleAppsSisFile);
    User::WaitForRequest(applistChangeStatus);
 
    //Get the updated application information from apparc.    
    updatedAppsInfo.Reset();    
    INFO_PRINTF1(_L("Retrieving updated application information after installation"));
    aLs.UpdatedAppsInfoL(updatedAppsInfo);
    INFO_PRINTF2(_L("Updated application count: %d"), updatedAppsInfo.Count()); 
    
    //Check the information provided by apparc is what is expected.    
    TEST(updatedAppsInfo.Count()== 3);
    TEST(updatedAppsInfo[0].iAppUid == KUidMultipleApp1);
    TEST(updatedAppsInfo[0].iAction == TApaAppUpdateInfo::EAppPresent);     
    TEST(updatedAppsInfo[1].iAppUid == KUidMultipleApp2);
    TEST(updatedAppsInfo[1].iAction == TApaAppUpdateInfo::EAppPresent);    
    TEST(updatedAppsInfo[2].iAppUid == KUidMultipleApp3);
    TEST(updatedAppsInfo[2].iAction == TApaAppUpdateInfo::EAppPresent);
    
    //Check the application is available in the applist.    
    TEST(aLs.GetAppInfo(appInfo, KUidMultipleApp1) == KErrNone);    
    TEST(aLs.GetAppInfo(appInfo, KUidMultipleApp2) == KErrNone);
    TEST(aLs.GetAppInfo(appInfo, KUidMultipleApp3) == KErrNone);
    
    INFO_PRINTF1(_L("Uninstalling applications ............"));    
    applistChangeStatus=KRequestPending;
    aLs.SetNotify(EFalse, applistChangeStatus);    
    sisInstaller.UninstallSisL(KTestMultipleAppsComponent);
    User::WaitForRequest(applistChangeStatus); 
    
    //Get the updated application information from apparc.    
    updatedAppsInfo.Reset();
    INFO_PRINTF1(_L("Retrieving updated application information after uninstallation"));    
    aLs.UpdatedAppsInfoL(updatedAppsInfo);
    INFO_PRINTF2(_L("Updated application count: %d"), updatedAppsInfo.Count());
    TEST(updatedAppsInfo.Count()== 3);
    
    //Check the information provided by apparc is what is expected.    
    TEST(updatedAppsInfo[0].iAppUid == KUidMultipleApp1);
    TEST(updatedAppsInfo[0].iAction == TApaAppUpdateInfo::EAppNotPresent);
    TEST(updatedAppsInfo[1].iAppUid == KUidMultipleApp2);
    TEST(updatedAppsInfo[1].iAction == TApaAppUpdateInfo::EAppNotPresent);    
    TEST(updatedAppsInfo[2].iAppUid == KUidMultipleApp3);
    TEST(updatedAppsInfo[2].iAction == TApaAppUpdateInfo::EAppNotPresent);
    
    //Check the application is not available in the applist.     
    TEST(aLs.GetAppInfo(appInfo, KUidMultipleApp1) == KErrNotFound);
    TEST(aLs.GetAppInfo(appInfo, KUidMultipleApp2) == KErrNotFound);
    TEST(aLs.GetAppInfo(appInfo, KUidMultipleApp3) == KErrNotFound);
    
    CleanupStack::PopAndDestroy(); //updatedAppsInfo
    INFO_PRINTF1(_L("TestMultipleAppInstallation ended............"));    
    }



/**
   @SYMTestCaseID           APPFWK-APPARC-0106
  

    @SYMTestCaseDesc        Tests whether apparc provides updated application information a component is upgraded.
  
   @SYMTestPriority         High
  
   @SYMTestStatus           Implemented
   
   @SYMTestActions          1. Installs a sis file which has 3 applications and waits till applist is changed.
                            2. Checks the updated information provided by apparc is as expected.
                            3. Degrades the component which is installed in Step 1 which has only 2 applications.
                            4. Checks the updated information provided by apparc is as expected.
                            3. Uninstalls the component installed in Step 3
                            4. Checks the updated information provided by apparc is as expected.
                            
                             
   @SYMTestExpectedResults Apparc provides updated application information.
  */
   
void CT_ClientNotifStep::TestClientNotifOnPackageDowngrade(RTestableApaLsSession &aLs)
    {
    INFO_PRINTF1(_L("TestClientNotifOnPackageDowngrade started............"));
    CSisFileInstaller sisInstaller;
    TRequestStatus applistChangeStatus;
    
    INFO_PRINTF1(_L("Installing sis file which has 3 applications"));     
    aLs.SetNotify(EFalse, applistChangeStatus);
    INFO_PRINTF2(_L("Installing sis file from -> %S"), &KTestMultipleAppsSisFile);
    sisInstaller.InstallSisL(KTestMultipleAppsSisFile);
    User::WaitForRequest(applistChangeStatus);
 
    //Get the updated application information from apparc.    
    RArray<TApaAppUpdateInfo> updatedAppsInfo;
    CleanupClosePushL(updatedAppsInfo);    
    INFO_PRINTF1(_L("Retrieving updated application information after installation"));
    aLs.UpdatedAppsInfoL(updatedAppsInfo);
    INFO_PRINTF2(_L("Updated application count: %d"), updatedAppsInfo.Count()); 
    
    //Check the information provided by apparc is what is expected.    
    TEST(updatedAppsInfo.Count()== 3);
    TEST(updatedAppsInfo[0].iAppUid == KUidMultipleApp1);
    TEST(updatedAppsInfo[0].iAction == TApaAppUpdateInfo::EAppPresent);     
    TEST(updatedAppsInfo[1].iAppUid == KUidMultipleApp2);
    TEST(updatedAppsInfo[1].iAction == TApaAppUpdateInfo::EAppPresent);    
    TEST(updatedAppsInfo[2].iAppUid == KUidMultipleApp3);
    TEST(updatedAppsInfo[2].iAction == TApaAppUpdateInfo::EAppPresent);
    

    //Check the application is available in the applist.    
    TApaAppInfo appInfo;
    TEST(aLs.GetAppInfo(appInfo, KUidMultipleApp1) == KErrNone);    
    TEST(aLs.GetAppInfo(appInfo, KUidMultipleApp2) == KErrNone);
    TEST(aLs.GetAppInfo(appInfo, KUidMultipleApp3) == KErrNone);
    
 
    INFO_PRINTF1(_L("Installing sis file which has only 2 applications"));   
    applistChangeStatus=KRequestPending;    
    aLs.SetNotify(EFalse, applistChangeStatus);
    INFO_PRINTF2(_L("Installing sis file from -> %S"), &KTestMultipleAppsDowngradeSisFile);
    sisInstaller.InstallSisL(KTestMultipleAppsDowngradeSisFile);
    User::WaitForRequest(applistChangeStatus);
 
    //Get the updated application information from apparc.    
    updatedAppsInfo.Reset();    
    INFO_PRINTF1(_L("Retrieving updated application information after installation"));
    aLs.UpdatedAppsInfoL(updatedAppsInfo);
    INFO_PRINTF2(_L("Updated application count: %d"), updatedAppsInfo.Count()); 
    
    //Check the information provided by apparc is what is expected.    
    TEST(updatedAppsInfo.Count()== 3);
    TEST(updatedAppsInfo[0].iAppUid == KUidMultipleApp1);
    TEST(updatedAppsInfo[0].iAction == TApaAppUpdateInfo::EAppNotPresent);     
    TEST(updatedAppsInfo[1].iAppUid == KUidMultipleApp2);
    TEST(updatedAppsInfo[1].iAction == TApaAppUpdateInfo::EAppPresent);    
    TEST(updatedAppsInfo[2].iAppUid == KUidMultipleApp3);
    TEST(updatedAppsInfo[2].iAction == TApaAppUpdateInfo::EAppPresent);

    //Check the application is not available in the applist.    
    TEST(aLs.GetAppInfo(appInfo, KUidMultipleApp1) == KErrNotFound);
    
    //Check the application is available in the applist.    
    TEST(aLs.GetAppInfo(appInfo, KUidMultipleApp2) == KErrNone);
    TEST(aLs.GetAppInfo(appInfo, KUidMultipleApp3) == KErrNone);    

    
    INFO_PRINTF1(_L("Uninstalling applications"));    
    applistChangeStatus=KRequestPending;
    aLs.SetNotify(EFalse, applistChangeStatus);    
    sisInstaller.UninstallSisL(KTestMultipleAppsDowngradeComponent);
    User::WaitForRequest(applistChangeStatus); 
    
    //Get the updated application information from apparc.    
    updatedAppsInfo.Reset();
    INFO_PRINTF1(_L("Retrieving updated application information after uninstallation"));    
    aLs.UpdatedAppsInfoL(updatedAppsInfo);
    INFO_PRINTF2(_L("Updated application count: %d"), updatedAppsInfo.Count());
    TEST(updatedAppsInfo.Count()== 2);
    
    //Check the information provided by apparc is what is expected.    
    TEST(updatedAppsInfo[0].iAppUid == KUidMultipleApp2);
    TEST(updatedAppsInfo[0].iAction == TApaAppUpdateInfo::EAppNotPresent);    
    TEST(updatedAppsInfo[1].iAppUid == KUidMultipleApp3);
    TEST(updatedAppsInfo[1].iAction == TApaAppUpdateInfo::EAppNotPresent);
    
    //Check the application is not available in the applist.     
    TEST(aLs.GetAppInfo(appInfo, KUidMultipleApp2) == KErrNotFound);
    TEST(aLs.GetAppInfo(appInfo, KUidMultipleApp3) == KErrNotFound);
    
    CleanupStack::PopAndDestroy(); //updatedAppsInfo
    INFO_PRINTF1(_L("TestClientNotifOnPackageDowngrade ended............"));    
    }

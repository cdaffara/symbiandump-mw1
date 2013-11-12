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

#include "t_forceregstep.h"
#include <e32base.h>
#include <e32cmn.h>
#include "T_SisFileInstaller.h"
#include <apgtask.h>
#include <apgnotif.h>
#include <e32property.h>
#include<usif/scr/appregentries.h>
#include "testableapalssession.h"

_LIT(KForceRegApp1SisFile, "z:\\apparctest\\apparctestsisfiles\\ForceRegApp1.sis");
_LIT(KForceRegApp1Component, "ForceRegApp1");

_LIT(KForceRegMultipleAppsSisFile, "z:\\apparctest\\apparctestsisfiles\\ForceRegMultipleApps.sis");
_LIT(KForceRegMultipleAppsComponent, "ForceRegMultipleApps");

_LIT(KForceRegApp2SisFile, "z:\\apparctest\\apparctestsisfiles\\ForceRegApp2.sis");
_LIT(KForceRegApp2Component, "ForceRegApp2");

const TUint KForceRegistratioWaitTime=5000000; //5s
const TUid KUidForceRegApp2={0xA0001001};
_LIT(KTestClientNotificationThreadName, "TestClientNotificationThreadName");


CT_ForceRegStep::~CT_ForceRegStep()
/**
   Destructor
 */
    {
    }

CT_ForceRegStep::CT_ForceRegStep()
/**
   Constructor
 */
    {
    // Call base class method to set up the human readable name for logging
    SetTestStepName(KT_ForceRegStep);
    }

TVerdict CT_ForceRegStep::doTestStepPreambleL()
/**
   @return - TVerdict code
   Override of base class virtual
 */
    {
   
    //Create property which is used for communication between test and force registered applications.
    TInt error;
    error=RProperty::Define(KPropertyCategory, KForceRegTestPropertyKey, RProperty::EInt);
    if((error != KErrNone) && (error != KErrAlreadyExists))
        User::Leave(error);
    
    SetTestStepResult(EPass);
    return TestStepResult();
    }

TVerdict CT_ForceRegStep::doTestStepPostambleL()
/**
   @return - TVerdict code
   Override of base class virtual
 */
    {
    RProperty::Delete(KPropertyCategory, KForceRegTestPropertyKey);
    
    return TestStepResult();
    }
    
    
TVerdict CT_ForceRegStep::doTestStepL()
{
    RunTestCases();
    return TestStepResult();
}


void CT_ForceRegStep::RunTestCases()
    {
    RTestableApaLsSession ls;
    User::LeaveIfError(ls.Connect());
    RPointerArray<TDesC> regFiles;
    //Check the earlier force registration API is not supported
    TEST(ls.ForceRegistration(regFiles) == KErrNotSupported);
    
    CSisFileInstaller sisFileInstaller;
    TApaAppInfo info;
    TUid uid1 = {0xA0001000};
    TInt err = ls.GetAppInfo(info, uid1);
    if(err == KErrNone)
        {      
        sisFileInstaller.UninstallSisL(KForceRegApp1Component);
        }    

    TUid uid2 = {0xA0001001};
    err = ls.GetAppInfo(info, uid2);
    if(err == KErrNone)
        {        
        sisFileInstaller.UninstallSisL(KForceRegApp2Component);
        }    

    //Wait if apparc updating the applist
    TRequestStatus applistChangeStatus;
    ls.SetNotify(ETrue, applistChangeStatus);
    User::WaitForRequest(applistChangeStatus);
    
    //DONT_CHECK is used because when an application is installed the updated application
    //information is added in the session objects. This causes increase of memory at server side.
    //As there can be multiple applications registered with apparc for applist change notifications,
    //its not possible to clear the information in all the session objects.
    HEAP_TEST_LS_SESSION(ls, 0, DONT_CHECK, TestSingleForceRegistration(ls), NO_CLEANUP);
    HEAP_TEST_LS_SESSION(ls, 0, DONT_CHECK, TestMultipleForceRegistration(ls), NO_CLEANUP);
    HEAP_TEST_LS_SESSION(ls, 0, 0, TestForceRegistrationSecurity(ls), NO_CLEANUP);
    //HEAP_TEST_LS_SESSION(ls, 0, 0, TestForceRegistrationWhenInstallationFailed(ls), NO_CLEANUP);
    HEAP_TEST_LS_SESSION(ls, 0, DONT_CHECK, TestForceRegistrationAndNoClientNotification(ls), NO_CLEANUP);
    ls.Close();
    }


/**
   @SYMTestCaseID           APPFWK-APPARC-0106
  

    @SYMTestCaseDesc        Tests whether force registration works with a sis file which has single run on install
                            application.
  
   @SYMTestPriority         High
  
   @SYMTestStatus           Implemented
   
   @SYMTestActions          1. Attaches to a property which will be set by force registered (or run on install) 
                               application when it got executed.
                            2. Installs sis file which has run on install application.
                            3. Waits till the run on install application changes the propery or a timer expires.
                            4. Checks the property is changed by run on install application.
                             
   @SYMTestExpectedResults The property is changed by run on install application.
 */
void CT_ForceRegStep::TestSingleForceRegistration(RTestableApaLsSession &aLs)
    {
    INFO_PRINTF1(_L("TestSingleForceRegistration test started..........."));
    RProperty forceRegStatus;
    User::LeaveIfError(forceRegStatus.Attach(KPropertyCategory, KForceRegTestPropertyKey, EOwnerThread));
    CleanupClosePushL(forceRegStatus);
    
    TRequestStatus forceRegStatusChange;
    TRequestStatus forceRegWait; //Timer expiration status
    
    //Timer to wait for definite time.
    RTimer timerToWait;
    User::LeaveIfError(timerToWait.CreateLocal()); 
    CleanupClosePushL(timerToWait);
    timerToWait.After(forceRegWait, KForceRegistratioWaitTime);
    
    //The property value will be changed by forceregistered application when its executed.
    forceRegStatus.Subscribe(forceRegStatusChange);
    
    INFO_PRINTF1(_L("Install forceregapp1 application"));    
    //Install the forceregapp1
    CSisFileInstaller sisFileInstaller;
    INFO_PRINTF2(_L("Installing sis file from -> %S"), &KForceRegApp1SisFile);
    sisFileInstaller.InstallSisL(KForceRegApp1SisFile);
    
    INFO_PRINTF1(_L("Wait till the timer expires or force registered application changes the property"));   
    //Wait till the property is changed by forceregapp1 or till timer expires
    User::WaitForRequest(forceRegWait, forceRegStatusChange);
    
    TInt value;
    forceRegStatus.Get(value);
    INFO_PRINTF2(_L("Property value: %d"), value);   
    //Check the property value is changed by forceregapp1
    TEST(value == KForceRegApp1Executed);
 
    INFO_PRINTF1(_L("Uninstall forceregapp1 application"));     
    //Uninstall the forceregapp1
    sisFileInstaller.UninstallSisAndWaitForAppListUpdateL(KForceRegApp1Component);
    CleanupStack::PopAndDestroy(2); //forceRegStatus, timerToWait
    INFO_PRINTF1(_L("TestSingleForceRegistration test ended..........."));
    }


/**
   @SYMTestCaseID           APPFWK-APPARC-0106
  

    @SYMTestCaseDesc        Tests whether force registration works with a sis file which has multiple run on install
                            applications.
  
   @SYMTestPriority         High
  
   @SYMTestStatus           Implemented
   
   @SYMTestActions          1. Attaches to a property which will be set by force registered (or run on install) 
                               application when it got executed.
                            2. Installs sis file which has multiple run on install applications.
                            3. Waits till the run on install applications changes the propery or a timer expires.
                            4. Checks the property is changed by run on install applications.
                             
   @SYMTestExpectedResults The property is changed by run on install applications.
  */
   
void CT_ForceRegStep::TestMultipleForceRegistration(RTestableApaLsSession &aLs)
    {
    INFO_PRINTF1(_L("TestMultipleForceRegistration test started..........."));
    RProperty forceRegStatus;
    User::LeaveIfError(forceRegStatus.Attach(KPropertyCategory, KForceRegTestPropertyKey, EOwnerThread));
    //Set the property to a KStopForceRegApp2 to make the forceregapp2 to exit.
    forceRegStatus.Set(KStopForceRegApp2);
    
    TRequestStatus forceRegApp1Executed, forceRegApp2Executed;
    TRequestStatus forceRegWait;
    
    //Timer to wait for definite time.
    RTimer timeToWait;
    User::LeaveIfError(timeToWait.CreateLocal());    
    timeToWait.After(forceRegWait, KForceRegistratioWaitTime);
    //The property value will be changed by forceregistered application. Subscribe to property change.
    forceRegStatus.Subscribe(forceRegApp1Executed);
    
    INFO_PRINTF1(_L("Install forceregapp1 and forceregapp2 application"));     
    //Install the forceregapp1
    CSisFileInstaller sisFileInstaller;
    INFO_PRINTF2(_L("Installing sis file from -> %S"), &KForceRegMultipleAppsSisFile);
    sisFileInstaller.InstallSisL(KForceRegMultipleAppsSisFile);
    
    INFO_PRINTF1(_L("Wait till the timer expires or force registered applications changes the property"));      
    User::WaitForRequest(forceRegWait, forceRegApp1Executed);
    
    TInt value;
    forceRegStatus.Get(value);
    
    //If the property value is not as expected, wait till the forceregapp2 is executed.
    if(value != (KForceRegApp1Executed|KForceRegApp2Executed|KStopForceRegApp2))
        {
        forceRegStatus.Subscribe(forceRegApp2Executed);
        forceRegWait=KRequestPending;
        timeToWait.After(forceRegWait, KForceRegistratioWaitTime);
        User::WaitForRequest(forceRegWait, forceRegApp2Executed);        
        }
    
    forceRegStatus.Get(value);
    INFO_PRINTF2(_L("Property value: %d"), value); 
    //Check whether both force registered applications executed.
    TEST(value == (KForceRegApp1Executed|KForceRegApp2Executed|KStopForceRegApp2));
    
    INFO_PRINTF1(_L("Uninstall forceregapp1 and forceregapp2 application"));     
    sisFileInstaller.UninstallSisAndWaitForAppListUpdateL(KForceRegMultipleAppsComponent);
    forceRegStatus.Close(); 
    //Wait for time so that appac completes applist cache creation.
    User::After(2000000);
    }


/**
   @SYMTestCaseID           APPFWK-APPARC-0106
  

    @SYMTestCaseDesc        Tests force registration security. Force registration can only be used by SWI.
  
   @SYMTestPriority         High
  
   @SYMTestStatus           Implemented
   
   @SYMTestActions          1. Creates dummy application information.
                            2. Call the force registration with this application info.
                            3. Check return value is KErrNotSupported.
                             
   @SYMTestExpectedResults ForceRegistration should return KErrNotSupported if other Software Installer 
                           trying to use it.
  */

void CT_ForceRegStep::TestForceRegistrationSecurity(RTestableApaLsSession &aLs)
    {
    INFO_PRINTF1(_L("TestForceRegistrationSecurity test started..........."));    
    Usif::CApplicationRegistrationData *appData=Usif::CApplicationRegistrationData::NewL();
    CleanupStack::PushL(appData);
    RPointerArray<Usif::CApplicationRegistrationData> appArray;
    
    INFO_PRINTF1(_L("Call ForceRegistration with empty TApaAppUpdate info array.........."));    
    TEST(aLs.ForceRegistration(appArray)==KErrNone);
    appArray.AppendL(appData);
    INFO_PRINTF1(_L("Call ForceRegistration with TApaAppUpdate info array.........."));       
    TEST(aLs.ForceRegistration(appArray)==KErrNotSupported);
    
    CleanupStack::PopAndDestroy(appData);
    appArray.Close();    
    INFO_PRINTF1(_L("TestForceRegistrationSecurity test ended..........."));     
    }


/**
   @SYMTestCaseID           APPFWK-APPARC-0106
  
   @SYMTestCaseDesc        Tests force registered applications information removed from the applist if 
                            installation fails.
  
   @SYMTestPriority         High
  
   @SYMTestStatus           Implemented
   
   @SYMTestActions          1. Install sis file which has force registered application.
                            2. Cancel the installation.
                            2. Check the force registered application is not available in the applist.
                             
   @SYMTestExpectedResults Force registered application information is removed from the applist
  */

void CT_ForceRegStep::TestForceRegistrationWhenInstallationFailed(RTestableApaLsSession &aLs)
    {
    INFO_PRINTF1(_L("TestForceRegistrationWhenInstallationFailed test started..........."));    
    TApaAppInfo appInfo;
    TInt err;
    
    //Check whether the application is already in the applist.
    err=aLs.GetAppInfo(appInfo, KUidForceRegApp2);
    INFO_PRINTF2(_L("Error Code returned: %d"), err);
    TEST(err==KErrNotFound);

    CSisFileInstaller sisFileInstaller;
    RProperty forceRegStatus;
    User::LeaveIfError(forceRegStatus.Attach(KPropertyCategory, KForceRegTestPropertyKey, EOwnerThread));
    TRequestStatus propertyStatus;
    forceRegStatus.Subscribe(propertyStatus);

    //Install ForceRegApp2 asynchronously
    TRequestStatus installStatus;
    INFO_PRINTF1(_L("Install the sis file which eventually cancelled by test"));
    err=KErrNone;
    INFO_PRINTF2(_L("Installing sis file from -> %S"), &KForceRegApp2SisFile);
    TRAP(err, sisFileInstaller.InstallSisAsynchronouslyL(KForceRegApp2SisFile, installStatus));
    TEST(err == KErrNone);

    //ForceRegApp2 is run on install application. The property is changed when the application executed.
    User::WaitForRequest(propertyStatus); 

    //Make ForceRegApp2 to exit by adding KStopForceRegApp2 to property.
    TInt value;
    forceRegStatus.Get(value);
    value |= KStopForceRegApp2;
    forceRegStatus.Set(value); 
    forceRegStatus.Close();
    
    //Cancel the installation.
    sisFileInstaller.CancelInstallation();
    User::WaitForRequest(installStatus);
    sisFileInstaller.Close();
    
    User::After(2000000);
    INFO_PRINTF2(_L("Installation ended with error code: %d"), installStatus.Int());    
    TEST(installStatus.Int() != KErrNone);
    
    //Check the force registered application is no longer exists in the applist.
    err=aLs.GetAppInfo(appInfo, KUidForceRegApp2);
    INFO_PRINTF2(_L("Error Code returned: %d"), err);    
    TEST(err==KErrNotFound);  
    INFO_PRINTF1(_L("TestForceRegistrationWhenInstallationFailed test ended..........."));       
    }

TInt TestClientNotificationThread(TAny* aPtr);

/**
   @SYMTestCaseID           APPFWK-APPARC-0106
  

    @SYMTestCaseDesc        Tests if applist is changed due to force registration, apparc will not notify the 
                            clients.
  
   @SYMTestPriority         High
  
   @SYMTestStatus           Implemented
   
   @SYMTestActions          1. Start TestClientNotificationThread which registers with apparc for applist change 
                               notification and waits till applist change notification occurs or a timer expires.
                               Once any of the event occurs, then changes the property to make the forceregapp2 to
                               exit and makes the status of applist change reflects in property.
                            2. Installs a sis file which has run on install forcereg1 and forcereg2 applications
                            3. Waits till the TestClientNotificationThread exits.
                            4. Checks whether applist change notification recieved or not.
                             
   @SYMTestExpectedResults  Apparc does not notify clients about applist change which occur due to force registration.
  */

void CT_ForceRegStep::TestForceRegistrationAndNoClientNotification(RTestableApaLsSession &aLs)
    {
    INFO_PRINTF1(_L("TestForceRegistrationAndNoClientNotification test started..........."));       
    RProperty forceRegStatus;
    User::LeaveIfError(forceRegStatus.Attach(KPropertyCategory, KForceRegTestPropertyKey, EOwnerThread));
    forceRegStatus.Set(0);
    
    TBuf<0x100> threadName(KTestClientNotificationThreadName);
    RThread thread;

    INFO_PRINTF1(_L("Start TestClientNotificationThread thread"));   
    User::LeaveIfError(thread.Create(threadName, TestClientNotificationThread, 0x1000, NULL, (TAny*) this));
    CleanupClosePushL(thread);
    TRequestStatus status;
    thread.Logon(status);
    thread.Resume();
    
    INFO_PRINTF1(_L("Install sis file"));      
    //Install the forceregapp1
    CSisFileInstaller sisFileInstaller;
    INFO_PRINTF2(_L("Installing sis file from -> %S"), &KForceRegMultipleAppsSisFile);
    sisFileInstaller.InstallSisL(KForceRegMultipleAppsSisFile);
    
    INFO_PRINTF1(_L("Waiting till TestClientNotificationThread thread exits"));     
    User::WaitForRequest(status);
    
    TInt value;
    forceRegStatus.Get(value);
    INFO_PRINTF2(_L("Property value: %d"), value);     
    TEST(!(value & KApplistChanged));
    INFO_PRINTF1(_L("Uninstall sis file"));     
    sisFileInstaller.UninstallSisL(KForceRegMultipleAppsComponent);
    CleanupStack::PopAndDestroy();
    forceRegStatus.Close();  
    INFO_PRINTF1(_L("TestForceRegistrationAndNoClientNotification test ended..........."));    
    }

/*
 * TestClientNotificationThread registers with apparc for applist change notification and waits till applist 
 * change notification occurs or a timer expires. Once any of the event occurs, then changes the property to 
 * make the forceregapp2 to exit and makes the status of applist change reflects in property.
 */

TInt TestClientNotificationThread(TAny* aPtr)
    {
    RApaLsSession ls;
    User::LeaveIfError(ls.Connect());
    TRequestStatus applistChangeStatus, timeOutStatus;
    ls.SetNotify(EFalse, applistChangeStatus);

    //Timer to wait for definite time.
    RTimer timeToWait;
    User::LeaveIfError(timeToWait.CreateLocal());    
    timeToWait.After(timeOutStatus, KForceRegistratioWaitTime);
    User::WaitForRequest(applistChangeStatus, timeOutStatus);

    RProperty forceRegStatus;
    User::LeaveIfError(forceRegStatus.Attach(KPropertyCategory, KForceRegTestPropertyKey, EOwnerThread));
    TInt status;
    forceRegStatus.Get(status);
    
    if(applistChangeStatus.Int() == MApaAppListServObserver::EAppListChanged)
        status |= KApplistChanged;

    status |= KStopForceRegApp2;
    forceRegStatus.Set(status);
    forceRegStatus.Close();
    
    return(KErrNone);
    }

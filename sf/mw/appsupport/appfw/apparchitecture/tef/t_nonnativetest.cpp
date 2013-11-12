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

#include "t_nonnativetest.h"
#include <e32base.h>
#include <e32cmn.h> 
#include "T_SisFileInstaller.h"
#include <apgtask.h>
#include <apgnotif.h>
#include <e32property.h>
#include<usif/scr/appregentries.h>
#include "testableapalssession.h"
#include "appfwk_test_utils.h"
#include "apacmdln.h"
#include "apgicnflpartner.h"

_LIT(KTNonNativeRuntimeSisFile, "z:\\apparctest\\apparctestsisfiles\\tnonnativeruntime.sis");
_LIT(KTNonNativeRuntimeComponent, "TNonNativeRunTime"); 

_LIT(KTestScrDBSource, "z:\\apparctest\\scr_test.db");
_LIT(KScrDBSource, "z:\\apparctest\\scr.db");
_LIT(KScrDBTarget, "c:\\sys\\install\\scr\\scr.db");
_LIT(KScrDBTempTarget, "c:\\sys\\install\\scr\\scr_temp.db");


_LIT(KNonNotiveAppName, "\\268454131.fakeapp"); 

const TUid KMidletUid={0x10210E26};
const TUid KWidgetUid={0x10282821};
const TUid KTestNonNativeUid={0xFF51233};

const TUid KUidNonNativeRuntime={0xA0001002};

const TUid KPropertyCategory = {0x101F289C};
const TUint KNonNativeTestPropertyKey = 2;

CT_NonNativeTestStep::~CT_NonNativeTestStep()
/**
   Destructor
 */
    {
    }

CT_NonNativeTestStep::CT_NonNativeTestStep()
/**
   Constructor
 */
    {
    // Call base class method to set up the human readable name for logging
    SetTestStepName(KT_NonNativeTestStep);
    }

TVerdict CT_NonNativeTestStep::doTestStepPreambleL()
/**
   @return - TVerdict code
   Override of base class virtual
 */
    {
   
    //Create property which is used for communication between test and non-native runtime.
    TInt error;
    error=RProperty::Define(KPropertyCategory, KNonNativeTestPropertyKey, RProperty::EInt);
    if((error != KErrNone) && (error != KErrAlreadyExists))
        User::Leave(error);
    
    SetTestStepResult(EPass);
    return TestStepResult();
    }

TVerdict CT_NonNativeTestStep::doTestStepPostambleL()
/**
   @return - TVerdict code
   Override of base class virtual
 */
    {
    RProperty::Delete(KPropertyCategory, KNonNativeTestPropertyKey);
    
    return TestStepResult();
    }
    
    
TVerdict CT_NonNativeTestStep::doTestStepL()
{
    RunTestCases();
    return TestStepResult();
}


void CT_NonNativeTestStep::RunTestCases()
    {
    RTestableApaLsSession ls;
    User::LeaveIfError(ls.Connect());
    
    //Wait if apparc updating the applist
    TRequestStatus applistChangeStatus;
    ls.SetNotify(ETrue, applistChangeStatus);
    User::WaitForRequest(applistChangeStatus);
    
    //DONT_CHECK is used because when an application is installed the updated application
    //information is added in the session objects. This causes increase of memory at server side.
    //As there can be multiple applications registered with apparc for applist change notifications,
    //its not possible to clear the information in all the session objects.
    HEAP_TEST_LS_SESSION(ls, 0, DONT_CHECK, TestNonNativeAppLaunchWithUnavailableMappingL(ls), NO_CLEANUP);
    HEAP_TEST_LS_SESSION(ls, 0, 0, TestNonNativeUnsupportedAPIs(ls), NO_CLEANUP); 
    HEAP_TEST_LS_SESSION(ls, 0, DONT_CHECK, TestGetAppTypeL(ls), NO_CLEANUP);
    ls.Close();
    }


/**
   @SYMTestCaseID           APPFWK-APPARC-0106
  

    @SYMTestCaseDesc        Tests whether apparc able to launch a non-native application for which the mapping is 
                            not avaialable in apparc mapping table but its available in SCR. 
                            
   @SYMTestPriority         High
  
   @SYMTestStatus           Implemented
   
                             
   @SYMTestExpectedResults Apparc will launch the non-native applciation.
 */

void CT_NonNativeTestStep::TestNonNativeAppLaunchWithUnavailableMappingL(RTestableApaLsSession &aLs)
    {
    INFO_PRINTF1(_L("TestNonNativeAppLaunchWithUnavailableMapping test started..........."));
    
    RProperty property;
    User::LeaveIfError(property.Attach(KPropertyCategory, KNonNativeTestPropertyKey, EOwnerThread));
    CleanupClosePushL(property);
    User::After(2000000); //Wait for 2 secconds till SCR server closed.
    
    RSmlTestUtils fs;
    User::LeaveIfError(fs.Connect());
    CleanupClosePushL(fs);  
    
    INFO_PRINTF1(_L("Delete existing scr.db"));    
    //Copy the existing SCR db to temp file and delete it
    fs.CopyFileL(KScrDBTarget, KScrDBTempTarget);
    fs.DeleteFileL(KScrDBTarget);
    
    INFO_PRINTF1(_L("Copying scr_test.db"));    
    //Copy the scr_test.db which has non-native application information and 
    //non-native type to its runtime.
    fs.CopyFileL(KTestScrDBSource, KScrDBTarget);
    fs.ChangeFilePermissionL(KScrDBTarget);
    
    INFO_PRINTF1(_L("Updating applist with a non-native application exists in scr_test.db"));     
    TApaAppUpdateInfo appUpdateInfo;
    RArray<TApaAppUpdateInfo> updateAppArray;
    CleanupClosePushL(updateAppArray);    
    
    appUpdateInfo.iAppUid=TUid::Uid(0x100048F3); //Test non-native app
    appUpdateInfo.iAction= TApaAppUpdateInfo::EAppPresent;
    updateAppArray.AppendL(appUpdateInfo);
    
    TRequestStatus status;
    aLs.SetNotify(EFalse, status);    
    aLs.UpdateAppListL(updateAppArray);
    CleanupStack::PopAndDestroy(&updateAppArray);
    User::WaitForRequest(status);
    
    INFO_PRINTF1(_L("Starting non-native application"));    
    CApaCommandLine* cmd=CApaCommandLine::NewLC();
    cmd->SetExecutableNameL(KNonNotiveAppName);
    aLs.StartApp(*cmd);
    CleanupStack::PopAndDestroy(cmd);
    User::After(2000000);
   
    //Once the non-native runtime executes, the property value is set to 1.
    TInt value;
    property.Get(value);
    INFO_PRINTF2(_L("Property value: %d"), value);   
    TEST(value == 1);
 
    INFO_PRINTF1(_L("Restoring the scr.db"));    
    User::After(2000000);   
    fs.DeleteFileL(KScrDBTarget);
    fs.CopyFileL(KScrDBTempTarget, KScrDBTarget);
    fs.DeleteFileL(KScrDBTempTarget);
    
    CleanupStack::PopAndDestroy(2, &property);
    INFO_PRINTF1(_L("TestNonNativeAppLaunchWithUnavailableMapping test ended..........."));
    }



void CT_NonNativeTestStep::TestNonNativeUnsupportedAPIs(RTestableApaLsSession &aLs)
{
    INFO_PRINTF1(_L("TestNonNativeUnsupportedAPIs test started..........."));  
    TUid uid=KNullUid;
    TDriveUnit drive;
    CApaRegistrationResourceFileWriter* regFileWriter=NULL;
    CApaLocalisableResourceFileWriter* locFileWriter=NULL;
    RFile *file=NULL;
    TPtrC fileName;
    
    TRAPD(err, aLs.RegisterNonNativeApplicationTypeL(uid, fileName));
    TEST(err == KErrNotSupported);

    TRAP(err, aLs.DeregisterNonNativeApplicationTypeL(uid));
    TEST(err == KErrNotSupported);
    
    TRAP(err, aLs.PrepareNonNativeApplicationsUpdatesL());
    TEST(err == KErrNotSupported);

    TRAP(err, aLs.RegisterNonNativeApplicationL(uid, drive, *regFileWriter, locFileWriter, file));
    TEST(err == KErrNotSupported);
    
    TRAP(err, aLs.DeregisterNonNativeApplicationL(uid));
    TEST(err == KErrNotSupported);
    
    TRAP(err, aLs.CommitNonNativeApplicationsUpdatesL());
    TEST(err == KErrNotSupported);
    
    TRAP(err, aLs.ForceCommitNonNativeApplicationsUpdatesL());
    TEST(err == KErrNotSupported);
    
    TEST(aLs.RollbackNonNativeApplicationsUpdates() == KErrNotSupported);
    
    INFO_PRINTF1(_L("TestNonNativeUnsupportedAPIs test ended..........."));    
}


void CT_NonNativeTestStep::TestGetAppTypeL(RTestableApaLsSession &aLs)
    {
    INFO_PRINTF1(_L("TestGetAppTypeL test started..........."));
    
    RSmlTestUtils fs;
    User::LeaveIfError(fs.Connect());
    CleanupClosePushL(fs);  
    User::After(2000000); //Wait for 2 secconds till SCR server closed.    
    
    INFO_PRINTF1(_L("Delete existing scr.db"));    
    //Copy the existing SCR db to temp file and delete it
    fs.CopyFileL(KScrDBTarget, KScrDBTempTarget);
    fs.DeleteFileL(KScrDBTarget);
    
    INFO_PRINTF1(_L("Copying scr_test.db"));    
    //Copy the scr_test.db which has non-native application information and 
    //non-native type to its runtime.
    fs.CopyFileL(KTestScrDBSource, KScrDBTarget);
    fs.ChangeFilePermissionL(KScrDBTarget);    
    
     INFO_PRINTF1(_L("Updating applist with a non-native application exists in scr_test.db"));     
    TApaAppUpdateInfo appUpdateInfo;
    RArray<TApaAppUpdateInfo> updateAppArray;
    CleanupClosePushL(updateAppArray);    
    
    appUpdateInfo.iAppUid=TUid::Uid(0x100048F3); //Test non-native app
    appUpdateInfo.iAction= TApaAppUpdateInfo::EAppPresent;
    updateAppArray.AppendL(appUpdateInfo);
    
    appUpdateInfo.iAppUid=TUid::Uid(0x10201D0E); //Test java app
    appUpdateInfo.iAction= TApaAppUpdateInfo::EAppPresent;
    updateAppArray.AppendL(appUpdateInfo);
    
    appUpdateInfo.iAppUid=TUid::Uid(0x10286B0D); //Test widget app
    appUpdateInfo.iAction= TApaAppUpdateInfo::EAppPresent;
    updateAppArray.AppendL(appUpdateInfo);
    
    TRequestStatus status;
    aLs.SetNotify(EFalse, status);    
    aLs.UpdateAppListL(updateAppArray);
    CleanupStack::PopAndDestroy(&updateAppArray);
    User::WaitForRequest(status);
    
    INFO_PRINTF1(_L("Test GetAppType returns valid uids"));
    TUid appTypeID;
    User::LeaveIfError(aLs.GetAppType(appTypeID, TUid::Uid(0x10201D0E))); //Java app
    TEST(appTypeID == KMidletUid);

    User::LeaveIfError(aLs.GetAppType(appTypeID, TUid::Uid(0x100048F3))); //Test non-native app
    TEST(appTypeID == KTestNonNativeUid);
    
    User::LeaveIfError(aLs.GetAppType(appTypeID, TUid::Uid(0x10286B0D))); //widget app
    TEST(appTypeID == KWidgetUid);
    
   
    INFO_PRINTF1(_L("Restoring the scr.db"));    
    User::After(2000000);    
    fs.DeleteFileL(KScrDBTarget);
    fs.CopyFileL(KScrDBTempTarget, KScrDBTarget);
    fs.DeleteFileL(KScrDBTempTarget);
    
    CleanupStack::PopAndDestroy(&fs);
    INFO_PRINTF1(_L("TestGetAppTypeL test ended..........."));
    }


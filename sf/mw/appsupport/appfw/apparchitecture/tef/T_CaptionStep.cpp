// Copyright (c) 2008-2010 Nokia Corporation and/or its subsidiary(-ies).
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
// Test for GT0247 Bravo.
// CR0583: Apparc should allow short caption to be modified dynamically
// Test for UIKON GT0143 Typhoon Work Series 60 Changes.
// REQ758.1: Add support for short captions
// This test requires that tstapp.app has been built
// tstapp.mmp has RESOURCE tstapp_caption.rss what caption is built into the .rsc file
// tstapp_loc.rss decides what version of the caption file to include:
// tstapp01.rls
// tstapp02.rls
// tstapp03.rls
// tstapp04.rls
// tstapp05.rls
// tstappsc.rls
// 
// t_captionstep.cpp
//

/**
 @file t_captionstep.cpp
 @test
 @internalComponent - Internal Symbian test code
*/

#include <apgicnfl.h>
#include <hal.h>
#include <e32test.h>

#include <apgctl.h>
#ifdef SYMBIAN_ENABLE_SPLIT_HEADERS
#include <apgctllist.h>
#endif //SYMBIAN_ENABLE_SPLIT_HEADERS
#include <apfctlf.h>
#include <apgcli.h>
#include "../apserv/apsserv.h"
#include <apaflrec.h>
#include <apparc.h>
#include <apgdoor.h>
#include <centralrepository.h>

#include <s32file.h>
#include "tstapp.h"		// KUidTestApp defined here
#include "T_CaptionStep.h"
#include "appfwk_test_utils.h"
#include "appfwk_test.h"
#include "ticoncaptionoverride.h" //KUidTestIconCapOverride defined here
#include "TIconLoaderAndIconArrayForLeaks.h"
#include "T_SisFileInstaller.h"


//
//
//		global consts and declarations
//
//


_LIT(KTestTApaAppInfoCaptionEnglish,"TstCap UK");
_LIT(KTestTApaAppInfoShortCaptionEnglish,"TC UK");

_LIT(KTestTApaAppInfoCaptionFrench,"TstCap FR");
_LIT(KTestTApaAppInfoShortCaptionFrench,"TC FR");

_LIT(KTestTApaAppInfoCaptionGerman,"TstCap GE");
_LIT(KTestTApaAppInfoShortCaptionGerman,"TC GE");

// If _loc.rss does not define Short Caption, it takes long caption
_LIT(KTestTApaAppInfoCaptionSpanish,"TstCap SP");
_LIT(KTestTApaAppInfoShortCaptionSpanish,"TstCap SP");

// If _loc.rss does not define Caption, it takes app name
_LIT(KTestTApaAppInfoCaptionItalian,"tstapp");
_LIT(KTestTApaAppInfoShortCaptionItalian,"TC IT");

_LIT(KTestTApaAppInfoCaptionDefault,"TstCap UK");
_LIT(KTestTApaAppInfoShortCaptionDefault,"TC UK");

// Cenrep configuration details for English language
_LIT(KCenRepCaption, "CRTstCap UK");
_LIT(KCenRepShortCaption, "CRTC UK");
_LIT(KCenRepIconFilename, "C:\\resource\\apps\\ticoncapoverride.mbm");

_LIT(KTstAppStandAloneSisFile, "z:\\apparctest\\apparctestsisfiles\\TSTAPP_standalone.sis");
_LIT(KTstAppStandAloneComponent, "TSTAPP_standalone");

_LIT(KTstAppTiconcaptionoverrideSisFile, "z:\\apparctest\\apparctestsisfiles\\ticoncaptionoverride.sis");
_LIT(KTstAppTiconcaptionoverrideComponent, "ticoncaptionoverride");

_LIT(KForceRegAppSisFile, "z:\\apparctest\\apparctestsisfiles\\ForceRegApp1.sis");
_LIT(KForceRegAppComponent, "ForceRegApp1");    

const TUid KUidIconCaptionRepository = {0x1028583d}; // Central Repository UID.
const TInt KTextSize = 100;

const TInt KDelay = 4000000;
//
//
//		CT_CaptionStep
//
//


void CT_CaptionStep::ChangeLocaleL(TLanguage aLanguage)
	{
#ifdef SYMBIAN_DISTINCT_LOCALE_MODEL 
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
#else
    _LIT(KLitLocaleDllNameBase, "ELOCL");
    _LIT(TwoDigExt,".%02d");
    
    RLibrary localeDll; 
    CleanupClosePushL(localeDll);
    
    const TUidType uidType(TUid::Uid(0x10000079),TUid::Uid(0x100039e6));
    TBuf<16> localeDllName(KLitLocaleDllNameBase);  
    localeDllName.AppendFormat(TwoDigExt, language);
    
    // Try to load the locale dll
    TInt error=localeDll.Load(localeDllName, uidType);
    if (error==KErrNotFound)
        {
        // Locale dll is not found for the asked language. 
        ERR_PRINTF2(_L("Failed to find the locale dll for %d"), language);
        }
    
    User::LeaveIfError(error);
    User::LeaveIfError(UserSvr::ChangeLocale(localeDllName));
    CleanupStack: opAndDestroy(); // localeDll
#endif
    
    // Check if the device locale has changed
    if (aLanguage == User::Language())
        {
        SetTestStepResult(EPass);
        }
    else
        {
        ERR_PRINTF3(_L("Failed to change the locale to %d whereas the current locale is"), aLanguage, User::Language());
        }	
	}


void CT_CaptionStep::DoLanguageTestL()
	{
	TLanguage language = User::Language();	// keep a copy to restore it later on.
	
	TInt ch = 0;
	for (ch=0; ch < 6; ch++)
		{
		TLanguage languageToTest = ELangTest;	// init to supress compiler remark
		switch (ch)
			{
			case 0:
				languageToTest=ELangEnglish;
				INFO_PRINTF1(_L("--------- Testing English"));
				break;
			case 1:
				languageToTest=ELangFrench;
				INFO_PRINTF1(_L("--------- Testing French"));
				break;
			case 2:
				languageToTest=ELangGerman;
				INFO_PRINTF1(_L("--------- Testing German"));
				break;
			case 3:
				languageToTest=ELangSpanish;
				INFO_PRINTF1(_L("--------- Testing Spanish"));
				break;
			case 4:
				languageToTest=ELangItalian;
				INFO_PRINTF1(_L("--------- Testing Italian"));
				break;
			case 5:
				languageToTest=ELangAmerican;
				INFO_PRINTF1(_L("--------- Testing American"));
				break;
			default:
				User::LeaveIfError(KErrNotSupported);
				break;
			};

		// Change the locale
		if(languageToTest != User::Language())
		    {
            ChangeLocaleWaitForApplistUpdate(languageToTest);		
            TEST(User::Language() == languageToTest);
		    }
		
		// Do the same set of tests for each language
		TestCApaSystemControlListL();
		TestCApaDoorL();
		TestTApaAppInfoStreamsL();
		HEAP_TEST_LS_SESSION(iLs, 0, 0, TestTApaAppInfoL(), iLs.ClearAppInfoArray() );

		INFO_PRINTF1(_L("Test for that language finished..."));
		}

	// restore original locale, just in case...
	if(User::Language() != language)
	    ChangeLocaleWaitForApplistUpdate(language);
	
	TEST(User::Language() == language);
	}	

/**
   @SYMTestCaseID	t_caption_TestCApaSystemControlListL
  
   @SYMPREQ			0
  
   @SYMTestCaseDesc	Check (short)caption by means of a control on the list CApaSystemControlList 

   @SYMTestPriority High
  
   @SYMTestStatus 	Implemented
  
   @SYMTestActions	The test creates a CApaSystemControlList and checks the value of (short)caption
   					by creating a CApaSystemControl from index 0 of CApaSystemControlList

   @SYMTestExpectedResults	The (short)caption is set to a value not null (length is not zero)
 */	
void CT_CaptionStep::TestCApaSystemControlListL()
	{
	INFO_PRINTF1(_L("Testing CApaSystemControlList... "));
	CApaSystemControlList* list=NULL;
	User::LeaveIfError(FbsStartup());
	RFbsSession fbs;
	User::LeaveIfError(fbs.Connect());
	TRAPD(ret, list=CApaSystemControlList::NewL(iFs));
	TEST(ret==KErrNone);
	CApaSystemControl* control=list->Control(0);
	TPtrC testShortCap=control->ShortCaption();
	TEST(testShortCap.Length()>0);
	TPtrC testCap=control->Caption();
	TEST(testCap.Length()>0);
	}


/**
   @SYMTestCaseID	t_caption_TestTApaAppInfoL
  
   @SYMPREQ			0
  
   @SYMTestCaseDesc Check appInfo contains the right (short)caption for the current test language.

   @SYMTestPriority High
  
   @SYMTestStatus 	Implemented
  
   @SYMTestActions	The test searches for KUidTestApp by calling GetNextApp until the UIDs match. 
   					Once the right app is found, it retrieves its details by calling GetAppInfo().
   					The (short)caption is check to ensure it matches the expected result.

   @SYMTestExpectedResults	The (short)caption matches the one specified for the application, with the
   							american one being the default one.
 */	
void CT_CaptionStep::TestTApaAppInfoL()
	{
	INFO_PRINTF1(_L("Testing GetAppInfo()... "));
	
	// go through the list of apps until it finds KUidTestApp
	TApaAppInfo appInfo;
	TInt ret = iLs.GetAllApps();
	TEST(ret==KErrNone);
	FOREVER
		{
		TApaAppInfo info;
		ret=iLs.GetNextApp(info);
		TEST(ret==KErrNone);
		if (info.iUid==KUidTestApp)
			{
			iLs.GetAppInfo(appInfo, info.iUid);
			break;
			}
		}

	switch (User::Language())
		{
		case ELangEnglish:
			TEST(appInfo.iCaption==KTestTApaAppInfoCaptionEnglish);
			TEST(appInfo.iShortCaption==KTestTApaAppInfoShortCaptionEnglish);
			break;
		case ELangFrench:
			TEST(appInfo.iCaption==KTestTApaAppInfoCaptionFrench);
			TEST(appInfo.iShortCaption==KTestTApaAppInfoShortCaptionFrench);
			break;
		case ELangGerman:
			TEST(appInfo.iCaption==KTestTApaAppInfoCaptionGerman);
			TEST(appInfo.iShortCaption==KTestTApaAppInfoShortCaptionGerman);
			break;
		case ELangSpanish:
			TEST(appInfo.iCaption==KTestTApaAppInfoCaptionSpanish);
			TEST(appInfo.iShortCaption==KTestTApaAppInfoShortCaptionSpanish);
			break;
		case ELangItalian:
			TEST(appInfo.iCaption==KTestTApaAppInfoCaptionItalian);
			TEST(appInfo.iShortCaption==KTestTApaAppInfoShortCaptionItalian);
			break;
		case ELangAmerican:
			TEST(appInfo.iCaption==KTestTApaAppInfoCaptionDefault);
			TEST(appInfo.iShortCaption==KTestTApaAppInfoShortCaptionDefault);
			break;
		default:
			User::Leave(KErrNotSupported);
			break;
		};
	}


/**
   @SYMTestCaseID	t_caption_TestCApaDoorL
  
   @SYMPREQ			0
  
   @SYMTestCaseDesc	Check the caption returned by a CApaDoor (a wrapper for an embedded document).

   @SYMTestPriority High
  
   @SYMTestStatus 	Implemented
  
   @SYMTestActions	The test starts by creating an active scheduler.
   					A CApaProcess is created, and a new CApaDocument is added to it using the specified 
   					application factory. This document is then used to create a CApaDoor object which 
   					is used to retrieve the test app caption.

   @SYMTestExpectedResults	The (short)caption matches the one specified for the application, with the
   							american one being the default one.
 */	
void CT_CaptionStep::TestCApaDoorL()
	{
	INFO_PRINTF1(_L("Testing CApaDoor... "));
	
	// create an active scheduler
	CActiveScheduler* scheduler = new(ELeave) CActiveScheduler;
	CActiveScheduler::Install(scheduler);
	CleanupStack::PushL(scheduler);
	INFO_PRINTF1(_L("Created and installed active scheduler... "));

	CApaProcess* process=NULL;
	TRAPD(ret,process = CApaProcess::NewL(iFs));
	TEST(ret==KErrNone);
	INFO_PRINTF1(_L("CApaProcess is created... "));

	CleanupStack::PushL(process);

	CApaDocument* doc=NULL;
	TApaApplicationFactory appFact(KUidTestApp);
	TRAP(ret,doc=process->AddNewDocumentL(appFact));
	INFO_PRINTF1(_L("Added a new document to the process by providing app factory which contains test app... "));
	TEST(ret==KErrNone);

	CApaDoor* door = NULL;
	TRAP(ret,door=CApaDoor::NewL(iFs,*doc,TSize(400,400)));
	INFO_PRINTF1(_L("Created a CApaDoor with document generated... "));
	TEST(ret==KErrNone);

	switch (User::Language())
		{
		case ELangEnglish:
			TEST(*door->Caption()==KTestTApaAppInfoCaptionEnglish);
			break;
		case ELangFrench:
			TEST(*door->Caption()==KTestTApaAppInfoCaptionFrench);
			break;
		case ELangGerman:
			TEST(*door->Caption()==KTestTApaAppInfoCaptionGerman);
			break;
		case ELangSpanish:
			TEST(*door->Caption()==KTestTApaAppInfoCaptionSpanish);
			break;
		case ELangItalian:
			TEST(*door->Caption()==KTestTApaAppInfoCaptionItalian);
			break;
		case ELangAmerican:
			TEST(*door->Caption()==KTestTApaAppInfoCaptionDefault);
			break;
		default:
			User::Leave(KErrNotSupported);
			break;
		};
	INFO_PRINTF1(_L("Tested the caption... "));
	process->DestroyDocument(doc);
	INFO_PRINTF1(_L("Deleted the process... "));
	CleanupStack::PopAndDestroy(2); //process,scheduler
	}


/**
   @SYMTestCaseID	t_caption_TestTApaAppInfoStreamsL
  
   @SYMPREQ			0
  
   @SYMTestCaseDesc Check TApaAppInfo streaming works as expected

   @SYMTestPriority High
  
   @SYMTestStatus 	Implemented
  
   @SYMTestActions	Create and TApaAppInfo object with the KUidTestApp test app.
   					Also, create a temporal file, associates the file with a write stream, 
   					and prepares the stream for writing. 
   					When the object is externalized to the streem, we open a file containing a 
   					read stream and prepares the stream for reading.

   @SYMTestExpectedResults	The test case is completed without any errors or panics
 */	
void CT_CaptionStep::TestTApaAppInfoStreamsL()
	{
	INFO_PRINTF1(_L("Testing TApaAppInfo streams... "));

	TApaAppInfo appInfoShort(KUidTestApp, _L("c:\\sys\\bin\\tstapp.exe"), _L("TstCap UK"),_L("TC UK"));
	TEST(appInfoShort.iShortCaption.Compare(_L("TC UK"))==0);

	TFileName tempFile=_L("c:\\system\\test\\TC_temp.txt");
	TInt ret = iFs.MkDirAll(tempFile);
	if (ret != KErrNone && ret != KErrAlreadyExists)
		User::LeaveIfError(ret);
	
	TApaAppInfo appInfo;

	RFileWriteStream writer;
	writer.PushL();
	User::LeaveIfError(writer.Replace(iFs,tempFile,EFileWrite));
	TRAP(ret, appInfo.ExternalizeL(writer));
	TEST(ret==KErrNone);
	writer.CommitL();
	CleanupStack::PopAndDestroy(&writer);

	RFileReadStream reader;
	reader.PushL();
	User::LeaveIfError(reader.Open(iFs,tempFile,EFileRead));
	TRAP(ret, appInfo.InternalizeL(reader));
	TEST(ret==KErrNone);

	CleanupStack::PopAndDestroy(&reader);

	User::LeaveIfError(iFs.Delete(tempFile));
	}
	


/**
   @SYMTestCaseID APPFWK-APPARC-0041 APPFWK-APPARC-0042 APPFWK-APPARC-0043 APPFWK-APPARC-0044 APPFWK-APPARC-0045 APPFWK-APPARC-0046
  
   @SYMPREQ PCHY-6FTLPK
  
   @SYMTestCaseDesc Test the RApaLsSession::SetAppShortCaption function, used to set the app's short caption.
  
   @SYMTestPriority High
  
   @SYMTestStatus Implemented
  
   @SYMTestActions Call SetAppShortCaption() to set the caption and read it back using the GetApaAppInfo().
  
   @SYMTestExpectedResults The returned short caption must match the one set by the SetAppShortCaption function.
 */	
void CT_CaptionStep::DoShortCaptionTestL()
	{
	INFO_PRINTF1(_L("Testing SetAppShortCaptionL() for short caption changes... "));

	_LIT(KShortCaption1, "SC1");
	_LIT(KShortCaption2, "SC2");
	_LIT(KShortCaption3, "SC3");
	_LIT(KShortCaption4, "SC4");
	_LIT(KShortCaption5, "SC5");

	// keep a copy of the current language, and set it to a known one
	TLanguage language = User::Language();
	ChangeLocaleL(ELangEnglish);
	TEST(User::Language() == ELangEnglish);
	
	// Force the applist to be updated (so test app gets new language settings)
	ForceApplistUpdate();

	// SetAppShortCaption should return KErrNotFound if it could not find the app
	INFO_PRINTF1(_L(".....setting short caption for an unknown app"));
	TInt err = iLs.SetAppShortCaption(KShortCaption1, ELangNone, TUid::Uid(0xFFF34556));
	TEST(err == KErrNotFound);

	// Set the NULL short caption i.e. "", should return KErrArgument,
	INFO_PRINTF1(_L(".....setting an empty short caption"));
	err = iLs.SetAppShortCaption(KNullDesC, ELangNone, KUidTestApp);
	TEST(err == KErrArgument);

	// SetAppShortCaption should return KErrArgument if short caption > KApaMaxAppCaption
	INFO_PRINTF1(_L(".....setting short caption of more then KApaMaxAppCaption length"));
	HBufC* longCaption = HBufC::NewL(KApaMaxAppCaption+1);
	longCaption->Des().SetLength(KApaMaxAppCaption+1);
	err = iLs.SetAppShortCaption(*longCaption, ELangNone, KUidTestApp);
	TEST(err == KErrArgument);
	delete longCaption;

	// SetAppShortCaption should return KErrNone if short caption == KApaMaxAppCaption
	INFO_PRINTF1(_L(".....setting short caption of same size as KApaMaxAppCaption"));
	longCaption = HBufC::NewL(KApaMaxAppCaption);
	longCaption->Des().SetLength(KApaMaxAppCaption);
	err = iLs.SetAppShortCaption(*longCaption, ELangCatalan, KUidTestApp);	// just testing the arg length, not actual values
	TEST(err == KErrNone);
	delete longCaption;

	// Set short caption of an app for the current locale language (ELangEnglish so far)
	INFO_PRINTF1(_L(".....setting short caption of an app for the current locale language"));
	err = iLs.SetAppShortCaption(KShortCaption1, ELangEnglish, KUidTestApp);
	TEST(err == KErrNone);
	TApaAppInfo appInfo;
	iLs.GetAppInfo(appInfo, KUidTestApp);
	TEST(appInfo.iShortCaption == KShortCaption1);

	// Check short caption remains updated after a refresh of the applist
	INFO_PRINTF1(_L(".....checking short caption remains updated after a refresh of the applist"));
	ForceApplistUpdate();
	iLs.GetAppInfo(appInfo, KUidTestApp);
	TEST(appInfo.iShortCaption == KShortCaption1);

	// Check short caption remains updated after a locale change
	INFO_PRINTF1(_L(".....checking short caption remains updated after a locale change"));
	ChangeLocaleWaitForApplistUpdate(ELangJapanese);
	TEST(User::Language() == ELangJapanese);	// Japanese locale exists in epoc32 tree but not defined in test app
    ChangeLocaleWaitForApplistUpdate(ELangEnglish);				// back to English to see what happened in between
	TEST(User::Language() == ELangEnglish);
	iLs.GetAppInfo(appInfo, KUidTestApp);
	TEST(appInfo.iShortCaption == KShortCaption1);

	// Set short caption of an app for a language which is not the current one (ELangFrench, for example)
	INFO_PRINTF1(_L(".....setting short caption of an app for a language which is not the current one"));
	err = iLs.SetAppShortCaption(KShortCaption2, ELangFrench, KUidTestApp);
	TEST(err == KErrNone);
	iLs.GetAppInfo(appInfo, KUidTestApp);
	TEST(appInfo.iShortCaption == KShortCaption1);	// English, the current app language, doesn't change...
	ChangeLocaleWaitForApplistUpdate(ELangFrench);
	TEST(User::Language() == ELangFrench);
	iLs.GetAppInfo(appInfo, KUidTestApp);
	TEST(appInfo.iShortCaption == KShortCaption2);	
	
	// Set short caption of an app for a language which the app does not include (ELangAmerican)
	INFO_PRINTF1(_L(".....setting short caption of an app for a language which the app does not include"));
	ChangeLocaleWaitForApplistUpdate(ELangAmerican);
	TEST(User::Language() == ELangAmerican);
	err = iLs.SetAppShortCaption(KShortCaption2, ELangAmerican, KUidTestApp);
	iLs.GetAppInfo(appInfo, KUidTestApp);
	TEST(appInfo.iShortCaption == KShortCaption1);	// American takes the default...so English...which has just been updated.

	// Set short caption of an app for all languages with ELangNone
	INFO_PRINTF1(_L(".....setting short caption for all languages with ELangNone"));
	err = iLs.SetAppShortCaption(KShortCaption3, ELangNone, KUidTestApp);
	TEST(err == KErrNone);
	iLs.GetAppInfo(appInfo, KUidTestApp);
	TEST(appInfo.iShortCaption == KShortCaption1);	// American takes the default...so English...which has just been updated.
	ChangeLocaleWaitForApplistUpdate(ELangEnglish);
	TEST(User::Language() == ELangEnglish);
	iLs.GetAppInfo(appInfo, KUidTestApp);
	TEST(appInfo.iShortCaption == KShortCaption1);	// English set individually ===> not updated by ELangNone
	ChangeLocaleWaitForApplistUpdate(ELangFrench);
	TEST(User::Language() == ELangFrench);
	iLs.GetAppInfo(appInfo, KUidTestApp);
	TEST(appInfo.iShortCaption == KShortCaption2);	// French set individually ===> not updated by ELangNone
	ChangeLocaleWaitForApplistUpdate(ELangGerman);
	TEST(User::Language() == ELangGerman);
	iLs.GetAppInfo(appInfo, KUidTestApp);
	TEST(appInfo.iShortCaption == KShortCaption3);	// German takes the one set by  ELangNone
	ChangeLocaleWaitForApplistUpdate(ELangItalian);
	TEST(User::Language() == ELangItalian);
	iLs.GetAppInfo(appInfo, KUidTestApp);
	TEST(appInfo.iShortCaption == KShortCaption3);	// Italian takes the one set by ELangNone
	ChangeLocaleWaitForApplistUpdate(ELangSpanish);
	TEST(User::Language() == ELangSpanish);
	iLs.GetAppInfo(appInfo, KUidTestApp);
	TEST(appInfo.iShortCaption == KShortCaption3);	// Spanish takes the one set by ELangNone

	// Set short caption of an app for a language which was set by the previous ELangNone
	INFO_PRINTF1(_L(".....setting short caption of an app which was set by the previous ELangNone"));
	ChangeLocaleWaitForApplistUpdate(ELangItalian);
	TEST(User::Language() == ELangItalian);
	err = iLs.SetAppShortCaption(KShortCaption4, ELangItalian, KUidTestApp);
	iLs.GetAppInfo(appInfo, KUidTestApp);
	TEST(appInfo.iShortCaption == KShortCaption4);

	// Repeat set short caption of an app for all languages with ELangNone
	INFO_PRINTF1(_L(".....repeat setting short caption for all languages with ELangNone"));
	err = iLs.SetAppShortCaption(KShortCaption5, ELangNone, KUidTestApp);
	TEST(err == KErrNone);
	iLs.GetAppInfo(appInfo, KUidTestApp);
	TEST(appInfo.iShortCaption == KShortCaption4);	// Italian set individually ===> not updated by ELangNone
	ChangeLocaleWaitForApplistUpdate(ELangEnglish);
	TEST(User::Language() == ELangEnglish);
	iLs.GetAppInfo(appInfo, KUidTestApp);
	TEST(appInfo.iShortCaption == KShortCaption1);	// English set individually ===> not updated by ELangNone
	ChangeLocaleWaitForApplistUpdate(ELangFrench);
	TEST(User::Language() == ELangFrench);
	iLs.GetAppInfo(appInfo, KUidTestApp);
	TEST(appInfo.iShortCaption == KShortCaption2);	// French set individually ===> not updated by ELangNone
	ChangeLocaleWaitForApplistUpdate(ELangGerman);
	TEST(User::Language() == ELangGerman);
	iLs.GetAppInfo(appInfo, KUidTestApp);
	TEST(appInfo.iShortCaption == KShortCaption5);	// German takes the one set by  ELangNone
	ChangeLocaleWaitForApplistUpdate(ELangSpanish);
	TEST(User::Language() == ELangSpanish);
	iLs.GetAppInfo(appInfo, KUidTestApp);
	TEST(appInfo.iShortCaption == KShortCaption5);	// Spanish takes the one set by ELangNone
	ChangeLocaleWaitForApplistUpdate(ELangAmerican);
	TEST(User::Language() == ELangAmerican);
	iLs.GetAppInfo(appInfo, KUidTestApp);
	TEST(appInfo.iShortCaption == KShortCaption1);	// American takes the default...so English...which has just been updated.

	// restore original settings....
	INFO_PRINTF1(_L(".....restoring original settings"));
	ChangeLocaleWaitForApplistUpdate(language);
	TEST(User::Language() == language);
	// restore original short captions for all langs....(h4 doesn't perform reboots between tests)
	TEST(iLs.SetAppShortCaption(KTestTApaAppInfoShortCaptionEnglish, ELangEnglish, KUidTestApp) == KErrNone);
	TEST(iLs.SetAppShortCaption(KTestTApaAppInfoShortCaptionFrench, ELangFrench, KUidTestApp) == KErrNone);
	TEST(iLs.SetAppShortCaption(KTestTApaAppInfoShortCaptionGerman, ELangGerman, KUidTestApp) == KErrNone);
	TEST(iLs.SetAppShortCaption(KTestTApaAppInfoShortCaptionItalian, ELangItalian, KUidTestApp) == KErrNone);
	TEST(iLs.SetAppShortCaption(KTestTApaAppInfoShortCaptionSpanish, ELangSpanish, KUidTestApp) == KErrNone);
	}


CT_CaptionStep::~CT_CaptionStep()
	{
	iFs.Close();
	iLs.Close();
	}

CT_CaptionStep::CT_CaptionStep()
	{
	SetTestStepName(KT_CaptionStep);
	}

/**
@SYMTestCaseID 				APPFWK-APPARC-0087
@SYMPREQ 					CR1187
@SYMREQ						REQ9167
@SYMTestCaseDesc			Tests whether the captions, icon configuration details have been retrieved properly from
							the central repository settings and has the precedence over its resource file information.
@SYMTestType				CIT
@SYMTestPriority			Critical
@SYMTestActions				A test application with its captions, icon details defined in a test central repository file
							as well as resource file (suitable field values have been given for both cenrep and resource
							configurations to identify it clearly).
							Gets the information about the test application by passing its UID.
							Check the given configuration details matches with central repository entries.
@SYMTestExpectedResults 	The given configuration details should match with central repository entries.
*/

void CT_CaptionStep::TestIconCaptionOverridesL()
	{
	INFO_PRINTF1(_L("APPFWK-APPARC-0087:TestIconCaptionOverridesL started..."));
	
    //Change the system language to English before starting the tests
    TRAPD(ret,ChangeLocaleL(ELangEnglish));
    TEST(ret == KErrNone);
    TEST(User::Language() == ELangEnglish);
    ForceApplistUpdate();
				
	TApaAppInfo appInfo;
	//Get test app's information
	iLs.GetAppInfo(appInfo, KUidTestIconCapOverride);

	RFile file;
	TBuf<KTextSize> fileName;
	//Get test app's icon information
	iLs.GetAppIcon(KUidTestIconCapOverride,file);
	file.FullName(fileName);

	//compare the captions, icon with values in CenRep file.
	INFO_PRINTF1(_L("Compares Caption with value in CenRep..."));
	TBuf<KTextSize> cencap(KCenRepCaption);
	
	INFO_PRINTF2(_L("----Expected caption==>%S"), &cencap);
	INFO_PRINTF2(_L("----Retrieved caption==>%S"), &appInfo.iCaption);
	TEST(appInfo.iCaption == KCenRepCaption);
	INFO_PRINTF1(_L("Compares ShortCaption with the value in CenRep..."));
	cencap.Zero();
	cencap.Copy(KCenRepShortCaption);
	
	INFO_PRINTF2(_L("----Expected short caption==>%S"), &cencap);
	INFO_PRINTF2(_L("----Retrieved short caption==>%S"), &appInfo.iShortCaption);
	TEST(appInfo.iShortCaption == KCenRepShortCaption);
	
	INFO_PRINTF1(_L("Compares Icon file with the value in CenRep..."));
	cencap.Zero();
	cencap.Copy(KCenRepIconFilename);
	INFO_PRINTF2(_L("----Expected icon filename==>%S"), &cencap);
	INFO_PRINTF2(_L("----Retrieved icon filename==>%S"), &fileName);
	TEST(fileName.Compare(KCenRepIconFilename)==0);
	
	file.Close();
	INFO_PRINTF1(_L("APPFWK-APPARC-0087:TestIconCaptionOverridesL finished..."));
	}	

/**
@SYMTestCaseID 				APPFWK-APPARC-0088
@SYMPREQ 					CR1187
@SYMREQ						REQ9167
@SYMTestCaseDesc			Tests whether the shortcaption is set through the API has got the precedence over central
							repository and resource file customisation.
@SYMTestType				CIT	
@SYMTestPriority			Critical
@SYMTestActions				A test application with its short caption defined in central repository as well as in resource file.
							Update the test application's short caption through API.
							Gets the information about the test application by passing its UID.
							Check whether the provided short caption matches with the one which has already been set through the API.
@SYMTestExpectedResults 	It should retrieve the short caption set through the API and match with the given short caption value.
*/

void CT_CaptionStep::TestApiPrecedenceOverCenRepConfigInfoL()
	{
	INFO_PRINTF1(_L("APPFWK-APPARC-0088:TestApiPrecedenceOverCenRepConfigInfoL started..."));
	_LIT(KTApiShortCaption,"APITC UK");
	
	TInt err = iLs.SetAppShortCaption(KTApiShortCaption, ELangEnglish, KUidTestIconCapOverride);
	TEST(err == KErrNone);

	TApaAppInfo appInfo;
	//Get test app's information
	iLs.GetAppInfo(appInfo, KUidTestIconCapOverride);
	
	TBuf<KTextSize> shortCaption(KTApiShortCaption);

	//compares the short caption with the value which has been set through API.
	INFO_PRINTF1(_L("Compares short caption with the value that has been set through API..."));
	INFO_PRINTF2(_L("----Expected short caption==>%S"), &shortCaption);
	INFO_PRINTF2(_L("----Retrieved short caption==>%S"), &appInfo.iShortCaption);
	TEST(appInfo.iShortCaption == KTApiShortCaption);
	
	//Sets the short caption back to the actual one
	INFO_PRINTF1(_L("Sets the short caption back to the actual one through API..."));
	err = iLs.SetAppShortCaption(KCenRepShortCaption, ELangEnglish, KUidTestIconCapOverride);
	TEST(err == KErrNone);

	INFO_PRINTF1(_L("APPFWK-APPARC-0088:TestApiPrecedenceOverCenRepConfigInfoL finished..."));
	}

/**
@SYMTestCaseID 				APPFWK-APPARC-0089
@SYMPREQ 					CR1187
@SYMREQ						REQ9167
@SYMTestCaseDesc			Tests whether the changes made in central repository is notified by the central repository
							observer (CApaIconCaptionCenrepObserver), which watches for changes in the Central Repository
							and updates the overrides when they occur.
@SYMTestType				CIT	
@SYMTestPriority			Critical
@SYMTestActions				A test application with its caption, icon configuration details defined in central repository file.
							Access the central repository.
							Update the test application short caption's value in the settings.
							Get the information about the test application by passing its UID.
							Check the short caption value retrieved from the central repository matches with the given information.
@SYMTestExpectedResults 	The given new short caption value should match with the central repository entry.
*/
void CT_CaptionStep::TestCenRepChangeNotificationL()
	{
	INFO_PRINTF1(_L("APPFWK-APPARC-0089:TestCenRepChangeNotificationL started..."));
	
	//access central repository
	CRepository* cenRep = CRepository::NewL(KUidIconCaptionRepository);
	CleanupStack::PushL(cenRep);

	_LIT(KCenRepSC,"CRNTC UK");	
	TUint32 shortCapKey = 0x00000001;  // for English
	
	//update the short caption value in central repository.
	TInt error = cenRep->Set(shortCapKey,KCenRepSC);
	
	//tests whether the process with WriteDeviceData capability can update the configuration settings.
	TEST(error == KErrNone);
	
	ForceApplistUpdate();
    
	TApaAppInfo appInfo;
	//Get test app's information
	iLs.GetAppInfo(appInfo, KUidTestIconCapOverride);

	//compare the short caption with the updated value in central repository
	INFO_PRINTF1(_L("Compares short caption with the updated value in CenRep..."));
	TBuf<KTextSize> shortCaption(KCenRepSC);
	
	INFO_PRINTF2(_L("----Expected short caption==>%S"), &shortCaption);
	INFO_PRINTF2(_L("----Retrieved shortcaption==>%S"), &appInfo.iShortCaption);
	TEST(appInfo.iShortCaption == KCenRepSC);
	
	//sets the short caption back to the actual for other tests to work
	error = cenRep->Set(shortCapKey,KCenRepShortCaption);
	
    // Force the applist to be updated (so test app gets new language settings)
    ForceApplistUpdate();
	
	CleanupStack::PopAndDestroy(cenRep); //cenRep object
	INFO_PRINTF1(_L("APPFWK-APPARC-0089:TestCenRepChangeNotificationL finished..."));
	}

/**
@SYMTestCaseID 				APPFWK-APPARC-0090
@SYMPREQ 					CR1187
@SYMREQ						REQ9167
@SYMTestCaseDesc			Tests whether the caption, icon configuration details have been retrieved properly from the
							Central Repository with respect to change in system language.
							Tests if the particular language entry is not there in the central repository file, it will
							then try to get the information from the resource file.
@SYMTestType				CIT	
@SYMTestPriority			Critical
@SYMTestActions				A multilingual test application with its caption, icon configuration details defined in central repository file.
							Change the system language to French.
							Gets the information about the test application by passing its UID.
							Checks the retrieved captions, icon from Central Repository are in French.
							Change the system language to German (the settings are not there for German in Central Repository).
							Again gets the information about the test application by passing its UID.
							Checks the retrieved captions, icon from the Resource file are in German.
							Restores the system language to English.
							Again gets the information about the test application by passing its UID.
							Checks the retrieved captions, icon from Central Repository are in English.
@SYMTestExpectedResults 	All the retrieved Central Repository configuration details should match with the given details by changing
							system language.
*/

void CT_CaptionStep::TestIconCaptionOverridesWithChangeLangL()
	{
	INFO_PRINTF1(_L("APPFWK-APPARC-0090:TestIconCaptionOverridesWithChangeLangL started..."));
	
	TInt ret = 0;
	//French
	_LIT(KCaptionFrench, "CRTstCap FR");
	_LIT(KShortCaptionFrench, "CRTC FR");
	_LIT(KIconFilenameFrench, "C:\\resource\\apps\\svg_icon.svg");

	//German
	_LIT(KCaptionGerman, "TstCap GE");
	_LIT(KShortCaptionGerman, "TC GE");
	_LIT(KIconFilenameGerman, "C:\\resource\\apps\\ticoncapoverride.mbm");

	TApaAppInfo appInfo;
	RFile file;
	TBuf<KTextSize> fileName;
		
	// Change the locale to French
	INFO_PRINTF1(_L("Change the locale to French..."));
	TRAP(ret,ChangeLocaleL(ELangFrench));
	TEST(ret == KErrNone);
	INFO_PRINTF2(_L("----System Language==>%d"), User::Language());
	
	TEST(User::Language() == ELangFrench);//check language is set to French.
	
    ForceApplistUpdate();
	
	//Get test app's information
	iLs.GetAppInfo(appInfo, KUidTestIconCapOverride);

	//Get test app's icon information
	iLs.GetAppIcon(KUidTestIconCapOverride,file);
	file.FullName(fileName);
	
	TBuf<KTextSize> printString(KCaptionFrench);
	
	//compare the captions, icon in French with the value in central repository
	INFO_PRINTF1(_L("Compares Caption with value in CenRep..."));
	INFO_PRINTF2(_L("----Exepcted Caption in French==>%S"), &printString);
	INFO_PRINTF2(_L("----Retrieved Caption==>%S"), &appInfo.iCaption);	
	TEST(appInfo.iCaption == KCaptionFrench);
	
	INFO_PRINTF1(_L("Compares Short Caption with value in CenRep..."));
	printString.Zero();
	printString.Copy(KShortCaptionFrench);
	INFO_PRINTF2(_L("----Expected short caption in French==>%S"), &printString);
	INFO_PRINTF2(_L("----Retrieved short caption==>%S"), &appInfo.iShortCaption);
	TEST(appInfo.iShortCaption == KShortCaptionFrench);
	
	INFO_PRINTF1(_L("Compares Icon filename with value in CenRep..."));
	printString.Zero();
	printString.Copy(KIconFilenameFrench);
	INFO_PRINTF2(_L("----Expected icon filename in French==>%S"), &printString);
	INFO_PRINTF2(_L("----Retrieved icon filename==>%S"), &fileName);
	TEST(fileName.Compare(KIconFilenameFrench)==0);
	
	//Change the locale to German.
	INFO_PRINTF1(_L("Change the locale to German..."));
	TRAP(ret,ChangeLocaleL(ELangGerman));
	TEST(ret == KErrNone);
	INFO_PRINTF2(_L("----System Language==>%d"), User::Language());
	TEST(User::Language() == ELangGerman);//check language is set to German.
	
	// Force the applist to be updated (so test app gets new language settings)
	ForceApplistUpdate();
	
	//Get test app's information
	iLs.GetAppInfo(appInfo, KUidTestIconCapOverride);

	file.Close();
	//Get test app's icon information
	iLs.GetAppIcon(KUidTestIconCapOverride,file);
	
	fileName.Zero();
	file.FullName(fileName);
	
	// The entry for language German is not there in Central repository so it compares
	// the captions, icon in this language with the values in resource file.
	INFO_PRINTF1(_L("Compares Caption with value in Resource file..."));
	printString.Zero();
	printString.Copy(KCaptionGerman);
	INFO_PRINTF2(_L("----Expected caption in German==>%S"), &printString);
	INFO_PRINTF2(_L("----Retrieved caption==>%S"), &appInfo.iCaption);
	TEST(appInfo.iCaption == KCaptionGerman);
	
	INFO_PRINTF1(_L("Compares short caption with value in Resource file..."));
	printString.Zero();
	printString.Copy(KShortCaptionGerman);
	INFO_PRINTF2(_L("----Expected short caption in German==>%S"), &printString);
	INFO_PRINTF2(_L("----Retrieved short caption==>%S"), &appInfo.iShortCaption);
	TEST(appInfo.iShortCaption == KShortCaptionGerman);
	
	INFO_PRINTF1(_L("Compares icon filename with value in Resource file..."));
	printString.Zero();
	printString.Copy(KIconFilenameGerman);
	INFO_PRINTF2(_L("----Expected icon filename in German==>%S"), &printString);
	INFO_PRINTF2(_L("----Retrieved icon filename==>%S"), &fileName);
	TEST(fileName.Compare(KIconFilenameGerman)==0);

	// Change the locale to English for changing the settings back to the original
	INFO_PRINTF1(_L("Change the locale back to English..."));
	TRAP(ret,ChangeLocaleL(ELangEnglish));
	TEST(ret == KErrNone);
	INFO_PRINTF2(_L("----System Language==>%d"), User::Language());
	
	TEST(User::Language() == ELangEnglish);//check language is set to English.
	
	// Force the applist to be updated (so test app gets new language settings)
	ForceApplistUpdate();
	
	//Get test app's information
	iLs.GetAppInfo(appInfo, KUidTestIconCapOverride);

	file.Close();
	//Get test app's icon information
	iLs.GetAppIcon(KUidTestIconCapOverride,file);

	fileName.Zero();
	file.FullName(fileName);
	
	//compare the captions, icon in English with the value in central repository
	INFO_PRINTF1(_L("Compares Caption with value in CenRep..."));
	printString.Zero();
	printString.Copy(KCenRepCaption);
	INFO_PRINTF2(_L("----Expected caption in English==>%S"), &printString);
	INFO_PRINTF2(_L("----Retrieved caption==>%S"), &appInfo.iCaption);
	TEST(appInfo.iCaption == KCenRepCaption);
	
	INFO_PRINTF1(_L("Compares short caption with value in CenRep..."));
	printString.Zero();
	printString.Copy(KCenRepShortCaption);
	INFO_PRINTF2(_L("----Expected short caption==>%S"), &printString);
	INFO_PRINTF2(_L("----Retrieved short caption==>%S"), &appInfo.iShortCaption);
	
	TEST(appInfo.iShortCaption == KCenRepShortCaption);
	INFO_PRINTF1(_L("Compares icon filename with value in CenRep..."));
	printString.Zero();
	printString.Copy(KCenRepIconFilename);
	INFO_PRINTF2(_L("----Expected icon filename==>%S"), &printString);
	INFO_PRINTF2(_L("----Retrieved icon filename==>%S"), &fileName);
	TEST(fileName.Compare(KCenRepIconFilename)==0);
	file.Close();
	INFO_PRINTF1(_L("APPFWK-APPARC-0090:TestIconCaptionOverridesWithChangeLangL finished..."));
	}
		
/**
@SYMTestCaseID 				APPFWK-APPARC-0091
@SYMPREQ 					CR1187
@SYMREQ						REQ9167
@SYMDEF						DEF125262
@SYMTestCaseDesc			Checks there is no memory leaks when CApaIconCaptionOverridesForApp & CApaIconCaptionCenrepObserver
							objects are created and destroyed.
@SYMTestType				CIT	
@SYMTestPriority			Critical
@SYMTestActions				Marks the Heap memory
							Invokes the static method 'TestIconCaptionOverridesL()' defined in 'ticonloaderandiconarrayforleaks.dll',
							that creates objects of CApaIconCaptionOverridesForApp, CApaIconCaptionCenrepObserver and deletes those.
							And then tests, there is no memory leak from the creation to destruction of those objects.
@SYMTestExpectedResults 	There should be no memory leaks.
*/

void CT_CaptionStep::TestIconCaptionOverridesMemoryLeaksL()
	{
	INFO_PRINTF1(_L("APPFWK-APPARC-0091:TestIconCaptionOverridesMemoryLeaksL started..."));
	__UHEAP_MARK;
	TIconLoaderAndIconArrayForLeaks::TestIconCaptionOverridesL();
	__UHEAP_MARKEND;
	INFO_PRINTF1(_L("APPFWK-APPARC-0091:TestIconCaptionOverridesMemoryLeaksL finished..."));
	}

void CT_CaptionStep::DoIconCaptionOverridesTestL()
	{
	TestIconCaptionOverridesL();
	TestIconCaptionOverridesWithChangeLangL();
	TestCenRepChangeNotificationL();
	TestIconCaptionOverridesMemoryLeaksL();
	TestApiPrecedenceOverCenRepConfigInfoL();
	}
	
void CT_CaptionStep::ForceApplistUpdate()
{
    // Force the applist to be updated (so test app gets new language settings)
    CSisFileInstaller sisFileInstaller;
    INFO_PRINTF2(_L("Installing sis file from -> %S"), &KForceRegAppSisFile);
    sisFileInstaller.InstallSisL(KForceRegAppSisFile);
    sisFileInstaller.UninstallSisAndWaitForAppListUpdateL(KForceRegAppComponent); 
}

void CT_CaptionStep::ChangeLocaleWaitForApplistUpdate(TLanguage aLanguage)
    {
    TRequestStatus status;
    iLs.SetNotify(EFalse, status);
    ChangeLocaleL(aLanguage);
    User::WaitForRequest(status);
    }

TVerdict CT_CaptionStep::doTestStepPreambleL()
    {
    CSisFileInstaller sisFileInstaller;
    INFO_PRINTF2(_L("Installing sis file from -> %S"), &KTstAppStandAloneSisFile);
    sisFileInstaller.InstallSisAndWaitForAppListUpdateL(KTstAppStandAloneSisFile);
    INFO_PRINTF2(_L("Installing sis file from -> %S"), &KTstAppTiconcaptionoverrideSisFile);
    sisFileInstaller.InstallSisAndWaitForAppListUpdateL(KTstAppTiconcaptionoverrideSisFile);
    SetTestStepResult(EPass);
    return TestStepResult();
    }

TVerdict CT_CaptionStep::doTestStepPostambleL()
    {
    CSisFileInstaller sisFileInstaller;
    sisFileInstaller.UninstallSisL(KTstAppStandAloneComponent);
    sisFileInstaller.UninstallSisL(KTstAppTiconcaptionoverrideComponent);
    return TestStepResult();    
    }

TVerdict CT_CaptionStep::doTestStepL()
	{
	INFO_PRINTF1(_L("Test T_Caption step started....\n"));

	// Connect to Servers
	TEST(iFs.Connect() == KErrNone);
	TEST(iLs.Connect() == KErrNone);

    // Change the locale
    ChangeLocaleL(ELangEnglish);
    TEST(User::Language() == ELangEnglish);
    ForceApplistUpdate();
    
	// run language tests for the test caption
	TRAPD(r, DoLanguageTestL());
	TEST(r==KErrNone);
	INFO_PRINTF2(_L("DoLanguageTestL() finished with value '%d'\n"), r);
	
	// run icon caption overrides tests
	TRAP(r, DoIconCaptionOverridesTestL());
	TEST(r==KErrNone);
	INFO_PRINTF2(_L("DoIconCaptionOverridesTestL() finished with value '%d'\n"), r);

	// run shortcaption setting tests
	TRAP(r, DoShortCaptionTestL());
	TEST(r==KErrNone);
	INFO_PRINTF2(_L("DoShortCaptionTestL() finished with value '%d'\n"), r);

	INFO_PRINTF1(_L("....test T_Caption step finished!"));
	return TestStepResult();
	}


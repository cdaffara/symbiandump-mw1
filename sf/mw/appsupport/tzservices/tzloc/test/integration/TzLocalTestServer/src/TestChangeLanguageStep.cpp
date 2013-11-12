// Copyright (c) 2005-2009 Nokia Corporation and/or its subsidiary(-ies).
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

#include <test/testexecutelog.h>
#include "TestChangeLanguageStep.h"

using namespace tzconstants;

CTestChangeLanguageStep::~CTestChangeLanguageStep()
/**
 * Destructor
 */
	{
	delete iLocalizer;
	}

CTestChangeLanguageStep::CTestChangeLanguageStep()
/**
 * Constructor
 */
	{
	// Call base class method to set up the human readable name for logging
	SetTestStepName(KTestChangeLanguageStep); 
	}

TVerdict CTestChangeLanguageStep::doTestStepPreambleL()
/**
 * @return - TVerdict code
 * Override of base class virtual
 */
	{
	TPtrC langFromIni;
	GetStringFromConfig(ConfigSection(),KLanguageTag,langFromIni); 
	
	if (langFromIni.Compare(_L("English")) == KErrNone)
	    {
		iLanguage = ELangEnglish;
	    }
	else if (langFromIni.Compare(_L("French")) == KErrNone)
	    {
		iLanguage = ELangFrench;
	    }
	else
	    {
	    _LIT(KLanguageNotFound,"TestChangeLanguage: Language Tag not found, default to English");
		INFO_PRINTF1(KLanguageNotFound);
		iLanguage = ELangEnglish;
	    }
	    	
	return TestStepResult();
	}

TVerdict CTestChangeLanguageStep::doTestStepL()
/**
 * @return - TVerdict code
 * Override of base class virtual
 * The step is called when its necessary to change the language setting.
 * The CheckLanguage() method must be called in the TzLocalization
 * component in order to swap resource files.
 *
 */
	{
	SetTestStepResult(EFail);	
	TRAPD(err, iLocalizer = CTzLocalizer::NewL());	
	__UHEAP_MARK;
	
	if (err == KErrNone)
	    {
	    TRAPD(res, ChangeLocaleL(iLanguage));
	    if (res == KErrNone)
	        {
	        _LIT(KChangeLocaleLOK,"TestChangeLanguageStep: ChangeLocaleL returned KErrNone");
	        INFO_PRINTF1(KChangeLocaleLOK);
	        TBool lang = iLocalizer->CheckLanguage();
	        if (lang)
	            {
	            _LIT(KCheckLanguageTrue,"TestChangeLanguageStep: CheckLanguage() returned ETrue");
	        	INFO_PRINTF1(KCheckLanguageTrue);
	        	SetTestStepResult(EPass);
	            }
	        else
	            {
	            _LIT(KCheck,"TestChangeLanguageStep: CheckLanguage() returned EFalse");
	        	ERR_PRINTF1(KCheck);
	            }	        
	        }
	    else
	        {
	        _LIT(KChange,"TestChangeLanguageStep: ChangeLocaleL() error = %d");
	        ERR_PRINTF2(KChange,res);
	    	
	        }
	    }
	else
    	{   
    	_LIT(KNewLeft,"TestChangeLanguageStep: NewL left");	    	    
	    ERR_PRINTF1(KNewLeft);	    
	    }
	    
	__UHEAP_MARK;
	return TestStepResult();    
	}
	    
	    
TVerdict CTestChangeLanguageStep::doTestStepPostambleL()
/**
 * @return - TVerdict code
 * Override of base class virtual
 */
	{
	return TestStepResult();
	}


void CTestChangeLanguageStep::ChangeLocaleL(TLanguage aLanguage)
	{
#ifdef SYMBIAN_DISTINCT_LOCALE_MODEL
    _LIT(KLitLocaleDllNameBase, "elocl_lan");
    _LIT(KLitLocaleDllNameExtension, ".loc");
#else 
    _LIT(KLitLocaleDllNameBase, "ELOCL");
    _LIT(KLitLocaleDllNameExtension, ".LOC");
#endif
    RLibrary localeDll;
    TBuf<16> localeDllName(KLitLocaleDllNameBase);
    CleanupClosePushL(localeDll);
    const TUidType uidType(TUid::Uid(0x10000079),TUid::Uid(0x100039e6));
#ifdef SYMBIAN_DISTINCT_LOCALE_MODEL
    _LIT(ThreeDigExt,".%03d");
    localeDllName.AppendFormat(ThreeDigExt, aLanguage);
#else
    _LIT(KTwoDigExt,".%02d");
    localeDllName.AppendFormat(KTwoDigExt, aLanguage);
#endif        
       
    TInt error=localeDll.Load(localeDllName, uidType);
    if (error == KErrNotFound)
        {
        _LIT(KLoadError,"ChangeLanguageStep : localeDll-Load() returned error = %d");
        ERR_PRINTF2(KLoadError,error);
        localeDllName=KLitLocaleDllNameBase;
        localeDllName.Append(KLitLocaleDllNameExtension);
        error=localeDll.Load(localeDllName, uidType);   
        }
    User::LeaveIfError(error);
#ifdef SYMBIAN_DISTINCT_LOCALE_MODEL
    _LIT(KLoadLocale,"Using API LoadLocaleAspect()");
    INFO_PRINTF1(KLoadLocale);
    TExtendedLocale myExtendedLocale;
    User::LeaveIfError(myExtendedLocale.LoadLocaleAspect(localeDllName));
    User::LeaveIfError(myExtendedLocale.SaveSystemSettings());
#else
	_LIT(KSecure,"Using Secure API ChangeLocale()");
	INFO_PRINTF1(KSecure);
	User::LeaveIfError(UserSvr::ChangeLocale(localeDllName));
#endif
	CleanupStack::PopAndDestroy(); // localeDll
	}

// Copyright (c) 2004-2009 Nokia Corporation and/or its subsidiary(-ies).
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

#include "TestSetTimeZoneStep.h"
#include "TestTzLocalBaseStep.h"
#include <test/testexecutelog.h>

using namespace tzconstants;

CTestSetTimeZoneStep::~CTestSetTimeZoneStep()
/**
 * Destructor
 */
	{
	iTzServer.Close();
	}

CTestSetTimeZoneStep::CTestSetTimeZoneStep()
/**
 * Constructor
 */
	{
	// Call base class method to set up the human readable name for logging
	SetTestStepName(KSetTimeZoneStep); 
	}

TVerdict CTestSetTimeZoneStep::doTestStepPreambleL()
/**
 * @return - TVerdict code
 * Override of base class virtual
 * Get the time zone from the ini file eg Europe/London and make another 
 * call to tz to validate data
 */
	{
	TPtrC tzIdString;
	User::LeaveIfError(iTzServer.Connect());	
	
	if (GetStringFromConfig(ConfigSection(), KTzIdTextTag, tzIdString))
	    {
	    if (tzIdString.Length() > 0)	
	        {
	        iTzIdName.Copy(tzIdString);
	        TBuf<50> buf;
	        buf.Copy(iTzIdName);
	        _LIT(KTzIdIni,"Set TimeZone : TzId String Returned From INI File = %S");
	        INFO_PRINTF2(KTzIdIni, &buf);	
	        }	     
	    else
	        {	 
	        _LIT(KNoTzId,"Set TimeZone : Cant find data in the ini file"); 	
	        ERR_PRINTF1(KNoTzId);
	        }
	    }
	else
	    {
	    _LIT(KNoString,"Set TimeZone : The GetStringFromConfig() returned not found");
	    ERR_PRINTF1(KNoString);	
	    }
	
	if (GetIntFromConfig(ConfigSection(),KSetTzInTzLocalTag,iSetTzInTzLocal))
	    {
		if (!GetIntFromConfig(ConfigSection(),KTzIdTag,iTzId))
		    {
			_LIT(KNoTzId,"Set Time Zone : Time Zone Id not in the ini file");
			ERR_PRINTF1(KNoTzId);
		    }		
	    }		
	
	return TestStepResult();
	}

TVerdict CTestSetTimeZoneStep::doTestStepL()
/**
 * @return - TVerdict code
 * Override of base class pure virtual

 */
	{		
    __UHEAP_MARK;
	SetTestStepResult(EFail);
	
	if (iSetTzInTzLocal)
	    {
		TRAPD(setintz,SetTzInTzLocalL());
		if (setintz != KErrNone)
		    {
			_LIT(KNoSet,"SetTimeZoneStep : SetTzInTzLocalL() left With err = %d");
			ERR_PRINTF2(KNoSet,setintz);
		    }	
	    }
	else
	    {	    
        TRAPD(seterr, SetCurrentTimeZoneL());
        if (seterr != KErrNone)
            {
            _LIT(KSetTzLeft,"Set Time Zone : SetCurrentTimeZoneL Left With err = %d");
    	    ERR_PRINTF2(KSetTzLeft, seterr);
            }        
	    }
  
    TRAPD(checkerr, CheckTimeZoneL());
    if (checkerr != KErrNone)
        {
        _LIT(KCheckTzLeft,"Setting Time Zone : ChecTimeZoneL Left With err = %d");
    	ERR_PRINTF2(KCheckTzLeft, checkerr);
        }
    __UHEAP_MARKEND;
	return TestStepResult();
	}

TVerdict CTestSetTimeZoneStep::doTestStepPostambleL()
/**
 * @return - TVerdict code
 * Override of base class virtual
 */
	{
	return TestStepResult();
	}


void CTestSetTimeZoneStep::SetCurrentTimeZoneL()
    {    
    CTzId* TzId = NULL;
    TRAPD(err,TzId = CTzId::NewL(iTzIdName)); 
    CleanupStack::PushL(TzId);
    User::LeaveIfError(err);
	iTzServer.SetTimeZoneL(*TzId); 
	CleanupStack::PopAndDestroy(TzId);
    }

void CTestSetTimeZoneStep::CheckTimeZoneL()
    {  
    TBuf<50> buf;  
    CTzId* TzId = NULL;
    TRAPD(err,TzId = iTzServer.GetTimeZoneIdL());
    CleanupStack::PushL(TzId);
    User::LeaveIfError(err);
    buf.Copy(TzId->TimeZoneNameID());
    
	if(TzId->TimeZoneNameID() == iTzIdName)
	    {
	    _LIT(KTzFound,"Set Time Zone : Time Zone Name Found : Tz = %S");
		INFO_PRINTF2(KTzFound,&buf);
		SetTestStepResult(EPass);
	    }
	else
	    {
	    _LIT(KTzNoMatch,"Set Time Zone : Time Zone Name Didnt match : Tz = %S");
		ERR_PRINTF2(KTzNoMatch,&buf);
	    }
	
	CleanupStack::PopAndDestroy(TzId);	
    }

void CTestSetTimeZoneStep::SetTzInTzLocalL()
    {
    __UHEAP_MARK;
    CTzLocalizer* myLocalizer = CTzLocalizer::NewL();
	CleanupStack::PushL(myLocalizer);
	myLocalizer->SetTimeZoneL(iTzId);
	CleanupStack::PopAndDestroy(myLocalizer);
	__UHEAP_MARKEND;
    }

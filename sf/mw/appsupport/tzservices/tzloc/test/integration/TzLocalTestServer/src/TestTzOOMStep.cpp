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
#include "TestTzOOMStep.h"

using namespace tzconstants;

CTestTzOOMStep::~CTestTzOOMStep()
/**
 * Destructor
 */
	{
	delete iLocalizer;
	}

CTestTzOOMStep::CTestTzOOMStep()
/**
 * Constructor
 */
	{
	// Call base class method to set up the human readable name for logging
	SetTestStepName(KTestTzOOMStep); 
	}


TVerdict CTestTzOOMStep::doTestStepPreambleL()
/**
 * @return - TVerdict code
 * Override of base class virtual
 */
    {  
    return TestStepResult(); 	 
    }
   
TVerdict CTestTzOOMStep::doTestStepL()
/**
 * @return - TVerdict code
 * Override of base class virtual
 */
    {            
 	SetTestStepResult(EPass);
    TRAPD(errL, iLocalizer = CTzLocalizer::NewL());
           	 
	if (errL == KErrNone)
	    {
	 	TRAPD(result,TestGetTzWithIdOOML(iLocalizer));
	 	if (result != KErrNone)
	 	    {
	 	    SetTestStepResult(EFail);
	 	    _LIT(KRes,"Test Tz with id OOMStep : ErrCode = %d");
	 	    ERR_PRINTF2(KRes,result);
	 	    }
	 	    
	 	TRAPD(result2,TestGetAllTzOOML(iLocalizer));
	 	if (result2 != KErrNone)
	 	    {
	 	    _LIT(KRes2,"Test Get all Tz OOMStep : ErrCode = %d");
	 		SetTestStepResult(EFail);
	 	    ERR_PRINTF2(KRes2,result2);
	 	    }
	 	    
	 	TRAPD(result3,TestGetTzWithCityOOML(iLocalizer));
	 	if (result3 != KErrNone)
	 	    {
	 		SetTestStepResult(EFail);
	 		_LIT(KRes3,"Test Get Tz With City OOMStep : ErrCode = %d");
	 	    ERR_PRINTF2(KRes3,result3);
	 	    }
	 	    
	 	TRAPD(result4,TestGetAllTzWithUtcOOML(iLocalizer))
	 	if (result4 != KErrNone )
	 	    {
	 		SetTestStepResult(EFail);
	 		_LIT(KRes4,"Test Get Tz With UTC OOMStep : ErrCode = %d");
	 	    ERR_PRINTF2(KRes4,result4);
	 	    }
	 	TRAPD(result5,TestSetTzOOML(iLocalizer))
	 	if (result5 != KErrNone)
	 	    {
	 		SetTestStepResult(EFail);
	 		_LIT(KRes5,"Test Set time zone OOM Step : ErrCode = %d");
	 	    ERR_PRINTF2(KRes5,result5);
	 	    }
	    }
 	else
 	    { 
 	    SetTestStepResult(EFail);	 
 	    _LIT(KNewLeft,"TestTzOOMStep: Problem with NewL");	 
		ERR_PRINTF1(KNewLeft);	     	
 	    } 	  	
 	return TestStepResult();
    }
        
    
TVerdict CTestTzOOMStep::doTestStepPostambleL()
/**
 * @return - TVerdict code
 * Override of base class virtual
 */
	{
	return TestStepResult();
	}
	

void CTestTzOOMStep::TestGetTzWithIdOOML(CTzLocalizer* aLoc)
    { 
    CTzLocalizedTimeZone* tz = NULL;
	for(TInt i = 1; ; i++)
	    {
		__UHEAP_FAILNEXT(i);				
		TRAPD(err, tz=aLoc->GetLocalizedTimeZoneL(KDefaultTzId));
		delete tz;
				
		if (err != KErrNoMemory)
		    {
			User::LeaveIfError(err);
			break; // where r == KErrNone
		    }		    				
	    }

	__UHEAP_RESET;
    }


void CTestTzOOMStep::TestGetTzWithCityOOML(CTzLocalizer* aLoc)
    {
	CTzLocalizedTimeZone* tz = NULL;
	CTzLocalizedCity* defCity = NULL;
	
	TRAPD(err1, defCity = aLoc->GetDefaultCityL(KDefaultTzId));
	User::LeaveIfError(err1);
	CleanupStack::PushL(defCity);
	
	for(TInt i = 1; ; i++)
	    {
		__UHEAP_FAILNEXT(i);	
    	TRAPD(err2, tz = aLoc->GetLocalizedTimeZoneL(*defCity));
	    delete tz;
	
    	if (err2 != KErrNoMemory)
		    {
			User::LeaveIfError(err2);
			break; // where r == KErrNone
		    }		    				
	    }	    
	__UHEAP_RESET;	
	CleanupStack::PopAndDestroy(defCity);	
    }


void CTestTzOOMStep::TestGetAllTzOOML(CTzLocalizer* aLoc)
    { 
    CTzLocalizedTimeZoneArray* tz = NULL;
	for(TInt i = 1; ; i++)
	    {
		__UHEAP_FAILNEXT(i);
		TRAPD(err, tz=aLoc->GetAllTimeZonesL());
		delete tz;
		
		if (err != KErrNoMemory)
		    {
			User::LeaveIfError(err);
			break; // where r == KErrNone
	    	}
	    }	
	__UHEAP_RESET;
    }

void CTestTzOOMStep::TestGetAllTzWithUtcOOML(CTzLocalizer* aLoc)
    { 
    CTzLocalizedTimeZoneArray* tz = NULL;
	for(TInt i = 1; ; i++)
	    {
		__UHEAP_FAILNEXT(i);
		TRAPD(err, tz=aLoc->GetTimeZonesWithUTCOffsetL(KDefaultUtcOffset));
		delete tz;
		
		if (err != KErrNoMemory)
		    {
			User::LeaveIfError(err);
			break; // where r == KErrNone
	    	}
	    }	
	__UHEAP_RESET;
    }

void CTestTzOOMStep::TestSetTzOOML(CTzLocalizer* aLoc)
    {
	for(TInt i = 1; ; i++)
	    {
		__UHEAP_FAILNEXT(i);
		TRAPD(err,aLoc->SetTimeZoneL(KDefaultTzId));
		
		if (err != KErrNoMemory)
		    {
			User::LeaveIfError(err);
			break; // where r == KErrNone
	    	}
	    }	
	__UHEAP_RESET;
    }
    

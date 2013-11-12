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
#include "TestTzManipOOMStep.h"

using namespace tzconstants;

CTzLocalizedTimeZone::TTzFrequentlyUsedZone FrequentlyUsedZone = CTzLocalizedTimeZone::EInterestZone;

CTestTzManipOOMStep::~CTestTzManipOOMStep()
/**
 * Destructor
 */
	{
	delete iLocalizer;
	}

CTestTzManipOOMStep::CTestTzManipOOMStep()
/**
 * Constructor
 */
	{
	// Call base class method to set up the human readable name for logging
	SetTestStepName(KTestTzManipOOMStep); 
	}


TVerdict CTestTzManipOOMStep::doTestStepPreambleL()
/**
 * @return - TVerdict code
 * Override of base class virtual
 */
    {  
    return TestStepResult(); 	 
    }
   
TVerdict CTestTzManipOOMStep::doTestStepL()
/**
 * @return - TVerdict code
 * Override of base class virtual
 */
    {            
 	SetTestStepResult(EPass);
 	TRAPD(err, iLocalizer = CTzLocalizer::NewL());
    
    if (err == KErrNone)
        {
        TRAPD(result, TestGetFrequentlyUsedZoneOOML(iLocalizer));
    	if (result != KErrNone)
		    {
			SetTestStepResult(EFail);
			_LIT(KRes,"Test Get Cached Zone OOMStep : ErrCode = %d");
	 	    ERR_PRINTF2(KRes,result);
		    }
		    
		TRAPD(result2, TestGetCachedZCityOOML(iLocalizer));
    	if (result2 != KErrNone)
		    {
			SetTestStepResult(EFail);
			_LIT(KRes2,"Test Get Cached City OOMStep : ErrCode = %d");
	 	    ERR_PRINTF2(KRes2,result2);
		    }
		    
		TRAPD(result3, TestSetFrequentlyUsedZoneOOML(iLocalizer));
    	if (result3 != KErrNone)
		    {
			SetTestStepResult(EFail);
			_LIT(KRes3,"Test Set Cached Zone OOMStep : ErrCode = %d");
	 	    ERR_PRINTF2(KRes3,result3);
		    }
        
        TRAPD(result4, TestSetCachedZWithTzOOML(iLocalizer));
    	if (result4 != KErrNone)
		    {
			SetTestStepResult(EFail);
			_LIT(KRes4,"Test Set Cached Zone with Tz OOMStep : ErrCode = %d");
	 	    ERR_PRINTF2(KRes4,result4);
		    }
		    
		TRAPD(result5, TestSetCachedZWithCityOOML(iLocalizer));
    	if (result5 != KErrNone)
		    {
			SetTestStepResult(EFail);
			_LIT(KRes5,"Test Set Cached Zone with City OOMStep : ErrCode = %d");
	 	    ERR_PRINTF2(KRes5,result5);
	 	    }              
        }
    else
        {
    	SetTestStepResult(EFail);
    	_LIT(KNewLeft,"TestTzManipOOMStep: Problem with NewL");	 	 
		ERR_PRINTF1(KNewLeft);
        }
        
    return TestStepResult();
    }
    
    
 TVerdict CTestTzManipOOMStep::doTestStepPostambleL()
/**
 * @return - TVerdict code
 * Override of base class virtual
 */
	{
	return TestStepResult();
	} 
	

void CTestTzManipOOMStep::TestGetFrequentlyUsedZoneOOML(CTzLocalizer* aLoc)
    {
	CTzLocalizedTimeZone* tz = NULL;	
	for(TInt i = 1;;i++)
	    {
		__UHEAP_FAILNEXT(i);
		TRAPD(err, tz = aLoc->GetFrequentlyUsedZoneL(FrequentlyUsedZone));
		delete tz;
		
		if(err != KErrNoMemory)
		    {
			User::LeaveIfError(err);
			break; //where r == KErrNone
		    }
	    }
	    
	    __UHEAP_RESET;	
    }

void CTestTzManipOOMStep::TestGetCachedZCityOOML(CTzLocalizer* aLoc)
    {
	CTzLocalizedCity* cachedCity = NULL;
	for(TInt i = 1;;i++)
	    {
		__UHEAP_FAILNEXT(i);
		TRAPD(err, cachedCity = aLoc->GetFrequentlyUsedZoneCityL(FrequentlyUsedZone));
		delete cachedCity;
		
		if(err != KErrNoMemory)
		    {
			User::LeaveIfError(err);
			break; //where r == KErrNone
		    }
	    }
	    
	    __UHEAP_RESET;	
    }
    
void CTestTzManipOOMStep::TestSetFrequentlyUsedZoneOOML(CTzLocalizer* aLoc)
    {
	for(TInt i = 1;;i++)
	    {
		__UHEAP_FAILNEXT(i);
		TRAPD(err, aLoc->SetFrequentlyUsedZoneL(KDefaultTzId, FrequentlyUsedZone));
				
		if(err != KErrNoMemory)
		    {
			User::LeaveIfError(err);
			break; //where r == KErrNone
		    }
	    }
	    
	    __UHEAP_RESET;	
    }
    
void CTestTzManipOOMStep::TestSetCachedZWithTzOOML(CTzLocalizer* aLoc)
    {
    CTzLocalizedTimeZone* tz = NULL;
    TRAPD(err1, tz = aLoc->GetLocalizedTimeZoneL(KDefaultTzId));
    User::LeaveIfError(err1);
    CleanupStack::PushL(tz);
    
	for(TInt i = 1;;i++)
	    {
		__UHEAP_FAILNEXT(i);
		TRAPD(err2, aLoc->SetFrequentlyUsedZoneL(*tz,FrequentlyUsedZone));
				
		if(err2 != KErrNoMemory)
		    {
			User::LeaveIfError(err2);
			break; //where r == KErrNone
		    }
	    }
	    
	    __UHEAP_RESET;
	    CleanupStack::PopAndDestroy(tz);	
    }


void CTestTzManipOOMStep::TestSetCachedZWithCityOOML(CTzLocalizer* aLoc)
    {
    CTzLocalizedCity* defCity = NULL;
    TRAPD(err1, defCity = aLoc->GetDefaultCityL(KDefaultTzId));
    User::LeaveIfError(err1);
    CleanupStack::PushL(defCity);
    
	for(TInt i = 1;;i++)
	    {
		__UHEAP_FAILNEXT(i);
		TRAPD(err2, aLoc->SetFrequentlyUsedZoneL(*defCity,FrequentlyUsedZone));
				
		if(err2 != KErrNoMemory)
		    {
			User::LeaveIfError(err2);
			break; //where r == KErrNone
		    }
	    }
	    
	    __UHEAP_RESET;
	    CleanupStack::PopAndDestroy(defCity);	
    }


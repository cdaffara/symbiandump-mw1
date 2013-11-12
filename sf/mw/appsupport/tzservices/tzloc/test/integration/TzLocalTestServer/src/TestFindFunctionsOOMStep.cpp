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
#include "TestFindFunctionsOOMStep.h"

using namespace tzconstants;

CTestFindFunctionsOOMStep::~CTestFindFunctionsOOMStep()
/**
 * Destructor
 */
	{
	delete iLocalizer;
	}

CTestFindFunctionsOOMStep::CTestFindFunctionsOOMStep()
/**
 * Constructor
 */
	{
	// Call base class method to set up the human readable name for logging
	SetTestStepName(KTestFindFunctionsOOMStep); 
	}


TVerdict CTestFindFunctionsOOMStep::doTestStepPreambleL()
/**
 * @return - TVerdict code
 * Override of base class virtual
 */
    {  
    if(!GetStringFromConfig(ConfigSection(),KDefaultCityTag,iCityFromIni))
        {
        _LIT(KNoCity,"TestFindFunctionsOOMStep : Cant find default city in ini file");
    	ERR_PRINTF1(KNoCity);
        }
    
    
    if(!GetStringFromConfig(ConfigSection(),KStandardNameTag,iTzFromIni))
        {
        _LIT(KNoStName,"TestFindFunctionsOOMStep : Cant find standard name in ini file");
        ERR_PRINTF1(KNoStName);	
        }
        
    
    if(!GetStringFromConfig(ConfigSection(),KCityGroupNameTag,iCityGroupFromIni))
        {
        _LIT(KNoGroup,"TestFindFunctionsOOMStep : Cant find city group name in ini file");
    	ERR_PRINTF1(KNoGroup);
        }
        
    return TestStepResult(); 	 
    }
    
   
TVerdict CTestFindFunctionsOOMStep::doTestStepL()
/**
 * @return - TVerdict code
 * Override of base class virtual
 */
    {            
 	SetTestStepResult(EPass);
    TRAPD(err, iLocalizer = CTzLocalizer::NewL());
    
    if (err == KErrNone)
        {
        TRAPD(result, TestFindCityOOML(iLocalizer));
        if (result != KErrNone)
		    {
			SetTestStepResult(EFail);
			_LIT(KRes,"Test Find City OOMStep : ErrCode = %d");
	 	    ERR_PRINTF2(KRes,result);
		    }
		    
		TRAPD(result2, FindTzOOML(iLocalizer))
		if (result2 != KErrNone)
		    {
			SetTestStepResult(EFail);
			_LIT(KRes2,"Test Find Tz OOMStep : ErrCode = %d");
	 	    ERR_PRINTF2(KRes2,result2);
		    }	
        TRAPD(result3, FindCityGroupOOML(iLocalizer))
		if (result3 != KErrNone)
		    {
			SetTestStepResult(EFail);
			_LIT(KRes3,"Test Find City Group OOMStep : ErrCode = %d");
	 	    ERR_PRINTF2(KRes3,result3);
	 	    }
        }
    else
        {
    	SetTestStepResult(EFail);	
    	_LIT(KNewLeft,"TestFindFunctionsOOMStep: Problem with NewL"); 	 
		ERR_PRINTF1(KNewLeft);
        }
        
    return TestStepResult();
    }
    
    
 TVerdict CTestFindFunctionsOOMStep::doTestStepPostambleL()
/**
 * @return - TVerdict code
 * Override of base class virtual
 */
	{
	return TestStepResult();
	}
	
	
void CTestFindFunctionsOOMStep::TestFindCityOOML(CTzLocalizer* aLoc)
    {
	CTzLocalizedCity* city = NULL;
	for(TInt i = 1;;i++)
	    {
		__UHEAP_FAILNEXT(i);
		TRAPD(err,city = aLoc->FindCityByNameL(iCityFromIni));
		delete city;
		
		if(err != KErrNoMemory)
		    {
			User::LeaveIfError(err);
			break; //where r == KErrNone
		    }
	    }
	    
	    __UHEAP_RESET;	
    }
    
void CTestFindFunctionsOOMStep::FindTzOOML(CTzLocalizer* aLoc)
    {
	CTzLocalizedTimeZone* tz = NULL;
	for(TInt i = 1;;i++)
	    {
		__UHEAP_FAILNEXT(i);
		TRAPD(err,tz = aLoc->FindTimeZoneByNameL(iTzFromIni));
		delete tz;
		
		if(err != KErrNoMemory)
		    {
			User::LeaveIfError(err);
			break; //where r == KErrNone
		    }
	    }
	    
	    __UHEAP_RESET;		
    }
    
void CTestFindFunctionsOOMStep::FindCityGroupOOML(CTzLocalizer* aLoc)
    {
	CTzLocalizedCityGroup* cityGroup = NULL;
	for(TInt i = 1;;i++)
	    {
		__UHEAP_FAILNEXT(i);
		TRAPD(err,cityGroup = aLoc->FindCityGroupByNameL(iCityGroupFromIni));
		delete cityGroup;
		
		if(err != KErrNoMemory)
		    {
			User::LeaveIfError(err);
			break; //where r == KErrNone
		    }
	    }
	    
	    __UHEAP_RESET;		
    }






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
#include "TestGroupsOOMStep.h"

using namespace tzconstants;

CTestGroupsOOMStep::~CTestGroupsOOMStep()
/**
 * Destructor
 */
	{
	delete iLocalizer;
	}

CTestGroupsOOMStep::CTestGroupsOOMStep()
/**
 * Constructor
 */
	{
	// Call base class method to set up the human readable name for logging
	SetTestStepName(KTestGroupsOOMStep); 
	}

TVerdict CTestGroupsOOMStep::doTestStepPreambleL()
/**
 * @return - TVerdict code
 * Override of base class virtual
 */
 
    {   
    return TestStepResult();  
    }   
    
    
TVerdict CTestGroupsOOMStep::doTestStepL()
/**
 * @return - TVerdict code
 * Override of base class virtual
 */
    {    
  	SetTestStepResult(EPass);
    TRAPD(err, iLocalizer = CTzLocalizer::NewL());
    
    if (err == KErrNone)
        {
    	TRAPD(result, TestGetAllGroupsOOML(iLocalizer));
    	if (result != KErrNone)
		    {
			SetTestStepResult(EFail);
			_LIT(KRes,"Test Get All Groups OOMStep : ErrCode = %d");
	 	    ERR_PRINTF2(KRes,result);
		    }
		    
		TRAPD(result2, TestGetGroupFromCityOOML(iLocalizer));
		if (result2 != KErrNone)
		    {
			SetTestStepResult(EFail);
			_LIT(KRes2,"Test Get Groups From City OOMStep : ErrCode = %d");
	 	    ERR_PRINTF2(KRes2, result2);
		    }
    	
    	TRAPD(result3, TestGetGroupFromIdOOML(iLocalizer));
    	if (result3 != KErrNone)
		    {
			SetTestStepResult(EFail);
			_LIT(KRes3,"Test Get Groups From Id OOMStep : ErrCode = %d");
	 	    ERR_PRINTF2(KRes3,result3);
		    }
    	
    	TRAPD(result4, TestGetCitiesFromGroupOOML(iLocalizer));
    	if (result4 != KErrNone)
    	    {
    		SetTestStepResult(EFail);
    		_LIT(KRes4,"Test Get City Groups From Group OOMStep : ErrCode = %d");
	 	    ERR_PRINTF2(KRes4,result4);
    	    }
    	
    	TRAPD(result5, TestGetCitiesFromCityOOML(iLocalizer));
    	if(result5 != KErrNone)
    	    {
    		SetTestStepResult(EFail);
    		_LIT(KRes5,"Test Get Cities In Group From Cities OOMStep : ErrCode = %d");
	 	    ERR_PRINTF2(KRes5,result5);
    	    }
    	
    	TRAPD(result6, TestGetCitiesFromGroupIdOOML(iLocalizer));
    	if(result5 != KErrNone)
    	    {
    		SetTestStepResult(EFail);
    		_LIT(KRes6,"Test Get Cities In Group From Group Id OOMStep : ErrCode = %d");
	 	    ERR_PRINTF2(KRes6,result6);
    	    }  	   	    	
        }
    else
        {
    	SetTestStepResult(EFail);	
    	_LIT(KNewLeft,"TesGroupsOOMStep: Problem with NewL");	 
		ERR_PRINTF1(KNewLeft);
        }
        
    return TestStepResult();
    }
    
 TVerdict CTestGroupsOOMStep::doTestStepPostambleL()
/**
 * @return - TVerdict code
 * Override of base class virtual
 */
	{
	return TestStepResult();
	} 
	
	
void CTestGroupsOOMStep::TestGetAllGroupsOOML(CTzLocalizer* aLoc)  
    {
	CTzLocalizedCityGroupArray* allGroups = NULL;
	
	for(TInt i = 1;;i++)
	    {
		__UHEAP_FAILNEXT(i);
		TRAPD(err, allGroups = aLoc->GetAllCityGroupsL());
		delete allGroups;
		
		if(err != KErrNoMemory)
		    {
			User::LeaveIfError(err);
			break; //where r == KErrNone
		    }
	    }
	
	    __UHEAP_RESET;			
    }
    
void CTestGroupsOOMStep::TestGetGroupFromCityOOML(CTzLocalizer* aLoc)
    {
	CTzLocalizedCity* defCity = NULL;
	CTzLocalizedCityGroup* retGroup = NULL;
	
	TRAPD(err1, defCity = aLoc->GetDefaultCityL(KDefaultTzId));
	User::LeaveIfError(err1);
	CleanupStack::PushL(defCity);
	
	for(TInt i = 1;;i++)
	    {
	 	__UHEAP_FAILNEXT(i);
		TRAPD(err2, retGroup=aLoc->GetCityGroupL(*defCity));
		delete retGroup;
		
		if(err2 != KErrNoMemory)
		    {
			User::LeaveIfError(err2);
			break; //where r == KErrNone
		    }		
	    }
	
	    __UHEAP_RESET;	
	    CleanupStack::PopAndDestroy(defCity);		
    }
    
    
void CTestGroupsOOMStep::TestGetGroupFromIdOOML(CTzLocalizer* aLoc)
    {
	CTzLocalizedCityGroup* retGroup = NULL;
	
	for(TInt i = 1;;i++)
	    {
		__UHEAP_FAILNEXT(i);
		TRAPD(err, retGroup = aLoc->GetCityGroupL(KDefaultCityGroupId));
		delete retGroup;
		
		if(err != KErrNoMemory)
		    {
			User::LeaveIfError(err);
			break; //where r == KErrNone
		    }		
	    }
	
	    __UHEAP_RESET;	
	
    }
    
void CTestGroupsOOMStep::TestGetCitiesFromGroupOOML(CTzLocalizer* aLoc)
    {
	CTzLocalizedCityGroup* retGroup = NULL;
	CTzLocalizedCityArray* allCities = NULL;
	
	TRAPD(err1, retGroup = aLoc->GetCityGroupL(KDefaultCityGroupId));
	User::LeaveIfError(err1);
	CleanupStack::PushL(retGroup);
	
	for(TInt i = 1;;i++)
	    {
		__UHEAP_FAILNEXT(i);
		TRAPD(err2, allCities = aLoc->GetCitiesInGroupL(*retGroup));
		delete allCities;
		
		if(err2 != KErrNoMemory)
		    {
			User::LeaveIfError(err2);
			break; //where r == KErrNone
		    }		
	    }
	
	    __UHEAP_RESET;
	    CleanupStack::PopAndDestroy(retGroup);	
    }
        
    
void CTestGroupsOOMStep::TestGetCitiesFromCityOOML(CTzLocalizer* aLoc)
    {
	CTzLocalizedCity* defCity = NULL;
	CTzLocalizedCityArray* allCities = NULL;
	
	TRAPD(err1, defCity = aLoc->GetDefaultCityL(KDefaultTzId));
	User::LeaveIfError(err1);
	CleanupStack::PushL(defCity);
	
	for(TInt i = 1;;i++)
	    {
		__UHEAP_FAILNEXT(i);
		TRAPD(err2,allCities=aLoc->GetCitiesInGroupL(*defCity))
		delete allCities;
		
		if (err2 != KErrNoMemory)
		    {
			User::LeaveIfError(err2);
			break; //where r == KErrNone
		    }
	    }
		__UHEAP_RESET;
	    CleanupStack::PopAndDestroy(defCity);
    }
    
 
 void CTestGroupsOOMStep::TestGetCitiesFromGroupIdOOML(CTzLocalizer* aLoc)
     {
 	 CTzLocalizedCityArray* allCities = NULL;
 	 for(TInt i = 1;;i++)
 	     {
 	     __UHEAP_FAILNEXT(i);
 	 	 TRAPD(err1, allCities = aLoc->GetCitiesInGroupL(KDefaultCityGroupId));
 	 	 delete allCities;
 	 	 
 	 	 if(err1 != KErrNoMemory)
 	 	     {
 	 	 	 User::LeaveIfError(err1);
			 break; //where r == KErrNone
 	 	     }
 	     }
 	     
 	     __UHEAP_RESET;
     }

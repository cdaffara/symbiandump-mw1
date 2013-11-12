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
#include "TestCityOOMStep.h"

using namespace tzconstants;

CTestCityOOMStep::~CTestCityOOMStep()
/**
 * Destructor
 */
	{
	delete iLocalizer;
	}

CTestCityOOMStep::CTestCityOOMStep()
/**
 * Constructor
 */
	{
	// Call base class method to set up the human readable name for logging
	SetTestStepName(KTestCityOOMStep); 
	}


TVerdict CTestCityOOMStep::doTestStepPreambleL()
/**
 * @return - TVerdict code
 * Override of base class virtual
 */
    {  
    if(!GetStringFromConfig(ConfigSection(),KUserDefinedCityTag,iCityName))
        {
        _LIT(KNoCity,"TestCityOOMStep: User added city not found in ini file");
    	ERR_PRINTF1(KNoCity);
        }
       
    if(!GetStringFromConfig(ConfigSection(),KStandardNameTag,iTzName))
        {
        _LIT(KNoName,"TestCityOOMStep: Standard name not found in ini file");
    	ERR_PRINTF1(KNoName);
        }
        
    return TestStepResult(); 	 
    }
    
   
TVerdict CTestCityOOMStep::doTestStepL()
/**
 * @return - TVerdict code
 * Override of base class virtual
 */
    {            
 	SetTestStepResult(EPass);
    TRAPD(err, iLocalizer = CTzLocalizer::NewL());
    if (err == KErrNone)
        {
    	TRAPD(result, TestGetDefCityWithIdOOML(iLocalizer));
    	if (result != KErrNone)
		    {
			SetTestStepResult(EFail);
			_LIT(KRes,"Test Def City with id OOMStep : ErrCode = %d");
	 	    ERR_PRINTF2(KRes,result);
		    }
        
        TRAPD(result2, TestGetCitiesOOML(iLocalizer))
        if (result2 != KErrNone)
            {
        	SetTestStepResult(EFail);
        	_LIT(KRes2,"Test Get Cities OOMStep : ErrCode = %d");
        	ERR_PRINTF2(KRes2,result2);
            }
            
        TRAPD(result3, TestGetCitiesWithIdOOML(iLocalizer)); 
        if (result3 != KErrNone)
            {
        	SetTestStepResult(EFail);
        	_LIT(KRes3,"Test Get Cities With Id OOMStep : ErrCode = %d");
        	ERR_PRINTF2(KRes3,result3);
            }  
            
        TRAPD(result4, TestAddCityOOML(iLocalizer));
        if (result4 != KErrNone)
            {
        	SetTestStepResult(EFail);
        	_LIT(KRes4,"Test Add City OOMStep : ErrCode = %d");
        	ERR_PRINTF2(KRes4,result4);
            }
            
        TRAPD(result5,TestGetDefCityFromCityOOML(iLocalizer));
        if (result5 != KErrNone)
            {
        	SetTestStepResult(EFail);
        	_LIT(KRes5,"Test Get Def City From City OOMStep : ErrCode = %d");
        	ERR_PRINTF2(KRes5,result5);
            }  
                         
        TRAPD(result6,TestGetDefCityFromTzOOML(iLocalizer));
        if (result6 != KErrNone)
            {
        	SetTestStepResult(EFail);
        	_LIT(KRes6,"Test Get DefCity From Tz OOMStep : ErrCode = %d");
        	ERR_PRINTF2(KRes6, result6);
            }  
            
        TRAPD(result7, TestGetCitiesFromTzOOML(iLocalizer));    
        if (result7 != KErrNone)
            {
        	SetTestStepResult(EFail);
        	_LIT(KRes7,"Test Get Cities From Tz OOMStep : ErrCode = %d");
        	ERR_PRINTF2(KRes7,result7);
            }     
            
        TRAPD(result8, TestGetCitiesFromCityOOML(iLocalizer));  
        if(result8 != KErrNone)
            {
            SetTestStepResult(EFail);
            _LIT(KRes8,"Test Get Cities From City OOMStep : ErrCode = %d");
        	ERR_PRINTF2(KRes8,result8);
            }  
                              
        TRAPD(result9, TestRemoveCityOOML(iLocalizer));
        if (result9 != KErrNone)
            {
        	SetTestStepResult(EFail);
        	_LIT(KRes9,"Tes Remove City OOMStep : ErrCode = %d");
        	ERR_PRINTF2(KRes9,result9);
            }
            
        TRAPD(result10, TestGetCitiesWithUtcOOML(iLocalizer))
        if (result10 != KErrNone)
            {
        	SetTestStepResult(EFail);
        	_LIT(KRes10,"Test GetCities WithUtcOOML : ErrCode = %d");
        	ERR_PRINTF2(KRes10,result10);
            }    
        }
    else
 	    { 
 	    SetTestStepResult(EFail);
 	    _LIT(KNewLeft,"TestCityOOMStep: Problem with NewL");	 	 
		ERR_PRINTF1(KNewLeft);
 	    } 
 	    	  	
 	return TestStepResult();
    }
    
   
TVerdict CTestCityOOMStep::doTestStepPostambleL()
/**
 * @return - TVerdict code
 * Override of base class virtual
 */
	{
	return TestStepResult();
	}
    

// OOM test for GetDefaultCityL()  
void CTestCityOOMStep::TestGetDefCityWithIdOOML(CTzLocalizer* aLoc)
    {
	CTzLocalizedCity* defCity = NULL;
	for(TInt i = 1;;i++)
	    {
		__UHEAP_FAILNEXT(i);
		TRAPD(err, defCity = aLoc->GetDefaultCityL(KDefaultTzId));
		delete defCity;
		
		if (err != KErrNoMemory)
		    {
			User::LeaveIfError(err);
			break; // where r == KErrNone
	   	    }
	    }
	    
	__UHEAP_RESET;
    }
    
// OOM test for GetCitiesL()    
void CTestCityOOMStep::TestGetCitiesOOML(CTzLocalizer* aLoc)
    {
    CTzLocalizedCityArray* allCities = NULL;
	for(TInt i = 1;;i++)
	    {
		__UHEAP_FAILNEXT(i);
		TRAPD(err, allCities = aLoc->GetCitiesL());
		delete allCities;
		
		if(err != KErrNoMemory)
		    {
			User::LeaveIfError(err);
			break; // where r == KErrNone
		    }
	    }
	    
	__UHEAP_RESET;
    }
    

// OOM test for GetCitiesL()   
void CTestCityOOMStep::TestGetCitiesWithIdOOML(CTzLocalizer* aLoc)
    {
    CTzLocalizedCityArray* allCities = NULL;
	for(TInt i = 1;;i++)
	    {
		__UHEAP_FAILNEXT(i);
		TRAPD(err, allCities = aLoc->GetCitiesL(KDefaultTzId));
		delete allCities;
		
		if(err != KErrNoMemory)
		    {
			User::LeaveIfError(err);
			break; // where r == KErrNone
		    }
	    }
	    
	__UHEAP_RESET;
    }

//OOM test for AddCityL()    
void CTestCityOOMStep::TestAddCityOOML(CTzLocalizer* aLoc)
    {
    CTzLocalizedCity* tempCity = NULL;
    
	for(TInt i = 1;;i++)
	    {
		__UHEAP_FAILNEXT(i);
		TRAPD(err1, tempCity = aLoc->AddCityL(KDefaultTzId,iCityName));
	    delete tempCity;
		
		if(err1 != KErrNoMemory)
		    {
			User::LeaveIfError(err1);
			break; // where r == KErrNone
		    }
	    }
	    
	    __UHEAP_RESET;
    }
 
 
// OOM test for GetDefaultCityL
void CTestCityOOMStep::TestGetDefCityFromCityOOML(CTzLocalizer* aLoc)
    {
	CTzLocalizedCity* tempCity = NULL;
	CTzLocalizedCity* defCity = NULL;
	TRAPD(err1, tempCity = aLoc->FindCityByNameL(iCityName));
    User::LeaveIfError(err1); 
    CleanupStack::PushL(tempCity);
    
    for(TInt i = 1;;i++)
        {
    	__UHEAP_FAILNEXT(i);
		TRAPD(err2, defCity=aLoc->GetDefaultCityL(*tempCity));
		delete defCity;
		
		if (err2 != KErrNoMemory)
		    {
			User::LeaveIfError(err2);
			break;
		    }   
						
        } 
         
        __UHEAP_RESET;
        CleanupStack::PopAndDestroy(tempCity);   
    }
  
    
//OOM test for GetDefaultCityL()
void CTestCityOOMStep::TestGetDefCityFromTzOOML(CTzLocalizer* aLoc)
    {
	CTzLocalizedCity* defCity = NULL;
	CTzLocalizedTimeZone* tz = NULL;
	TRAPD(err1, tz = aLoc->FindTimeZoneByNameL(iTzName));
    User::LeaveIfError(err1); 
    CleanupStack::PushL(tz);
    
    for(TInt i = 1;;i++)
        {
    	__UHEAP_FAILNEXT(i);
		TRAPD(err2, defCity=aLoc->GetDefaultCityL(*tz));
		delete defCity;
		
		if (err2 != KErrNoMemory)
		    {
			User::LeaveIfError(err2);
			break;
		    }   
						
        } 
         
        __UHEAP_RESET;
        CleanupStack::PopAndDestroy(tz);   
    }


// OOM test for GetCitiesL(tz)
void CTestCityOOMStep::TestGetCitiesFromTzOOML(CTzLocalizer* aLoc)
    {
    CTzLocalizedCityArray* allCities = NULL;
    CTzLocalizedTimeZone* tz = NULL;
	TRAPD(err1, tz = aLoc->FindTimeZoneByNameL(iTzName));
    User::LeaveIfError(err1); 
    CleanupStack::PushL(tz);
    
	for(TInt i = 1;;i++)
	    {
		__UHEAP_FAILNEXT(i);
		TRAPD(err2, allCities = aLoc->GetCitiesL(*tz));
		delete allCities;
		
		if(err2 != KErrNoMemory)
		    {
			User::LeaveIfError(err2);
			break; // where r == KErrNone
		    }
	    }
	    
    	__UHEAP_RESET;
    	CleanupStack::PopAndDestroy(tz);
    }
    
    
 //OOM test for GetCitiesL(city)   
 void CTestCityOOMStep::TestGetCitiesFromCityOOML(CTzLocalizer* aLoc)
     {
 	 CTzLocalizedCityArray* allCities = NULL;
 	 CTzLocalizedCity* aCity = NULL;
	 TRAPD(err1, aCity=aLoc->FindCityByNameL(iCityName));
     User::LeaveIfError(err1); 
     CleanupStack::PushL(aCity);
     
     for(TInt i = 1;;i++)
         {
         __UHEAP_FAILNEXT(i);
     	 TRAPD(err2, allCities = aLoc->GetCitiesL(*aCity));
     	 delete allCities;
     	 
     	 if(err2 != KErrNoMemory)
     	     {
     	 	 User::LeaveIfError(err2);
			 break; // where r == KErrNone
     	     }
     	 
         }
         
         __UHEAP_RESET;
         CleanupStack::PopAndDestroy(aCity);         
     }
 

//OOM test for RemoveCityL()
 void CTestCityOOMStep::TestRemoveCityOOML(CTzLocalizer* aLoc)
    {
    CTzLocalizedCity* tempCity = NULL;  
     
	for(TInt i = 1;;i++)
	    {	    
	    TRAPD(err1, tempCity = aLoc->FindCityByNameL(iCityName));
        User::LeaveIfError(err1);
        
		__UHEAP_FAILNEXT(i);
		TRAPD(err2, aLoc->RemoveCityL(tempCity));
		
		if(err2 != KErrNoMemory)
		    {
			User::LeaveIfError(err2);
			break; // where r == KErrNone
		    }
	    }
	    
	    __UHEAP_RESET;
    }

// OOM test for GetCitiesWithUTCOffsetL()   
void CTestCityOOMStep::TestGetCitiesWithUtcOOML(CTzLocalizer* aLoc)
    {
    CTzLocalizedCityArray* allCities = NULL;
	for(TInt i = 1; ; i++)
	    {
		__UHEAP_FAILNEXT(i);
		TRAPD(err, allCities = aLoc->GetCitiesWithUTCOffsetL(KDefaultUtcOffset));
		delete allCities;
		
		if(err != KErrNoMemory)
		    {
			User::LeaveIfError(err);
			break; // where r == KErrNone
		    }
	    }
	    
	__UHEAP_RESET;
    }
    


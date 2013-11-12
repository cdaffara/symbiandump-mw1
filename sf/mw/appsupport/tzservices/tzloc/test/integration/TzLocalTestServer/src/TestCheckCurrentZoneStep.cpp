// Copyright (c) 2006-2009 Nokia Corporation and/or its subsidiary(-ies).
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
#include "TestCheckCurrentZoneStep.h"

using namespace tzconstants;

CTestCheckCurrentZoneStep::~CTestCheckCurrentZoneStep()
/**
 * Destructor
 */
	{
	delete iLocalizer;
	}

CTestCheckCurrentZoneStep::CTestCheckCurrentZoneStep()
/**
 * Constructor
 */
	{
	// Call base class method to set up the human readable name for logging
	SetTestStepName(KTestCheckCurrentZoneStep); 
	}

TVerdict CTestCheckCurrentZoneStep::doTestStepPreambleL()
/**
 * @return - TVerdict code
 * Override of base class virtual
 */
	{
	if (GetIntFromConfig(ConfigSection(),KTzIdTag,iTzIdFromIni))
	    {
	    _LIT(KTzIdFound,"Set Cached Zone Step: Tz Id tag found in the ini file");
		INFO_PRINTF1(KTzIdFound);
	    }
	else
	    {
	    _LIT(KTzIdNotFound,"Set Cached Zone Step: Tz Id not found in the ini file");
		ERR_PRINTF1(KTzIdNotFound);
	    }
		
	if (GetIntFromConfig(ConfigSection(),KCachedTzTag,iFrequentlyUsedZoneType))
	    {
	    _LIT(KCachedTzFound,"Set Cached Zone Step: Cached tz found in the ini file");
		INFO_PRINTF1(KCachedTzFound);
	    }
	else
	    {
	    _LIT(KCachedTzNotFound,"Set Cached Zone Step: Cached Tz not found in the ini file");
		ERR_PRINTF1(KCachedTzNotFound);
	    }		
	
	if (GetStringFromConfig(ConfigSection(),KUserDefinedCityTag,iUserAddedCity))
	    {
	    _LIT(KCityFound,"Set Cached Zone Step: User added city found in the ini file");
		INFO_PRINTF1(KCityFound);
	    }
	else
	    {
	    _LIT(KCityNotFound,"Set Cached Zone Step: User added city not found in the ini file");
		ERR_PRINTF1(KCityNotFound);
	    }	
		
	return TestStepResult();
	}
	
TVerdict CTestCheckCurrentZoneStep::doTestStepL()
/**
 * @return - TVerdict code
 * Override of base class virtual
 * The user defined city is used to set thr cached zone. The cached
 * zone type is read from the ini file. When setting a cached zone
 * only EHomeZone and EInterestZone are valid. A NULL check
 * must always be carried out on the object returned from FindCityByNameL()
 * as the function doesnt leave if a city isnt found.
 */
	{
	
	SetTestStepResult(EFail);
	CTzLocalizedTimeZone::TTzFrequentlyUsedZone frequentlyUsedZone;
	
	switch (iFrequentlyUsedZoneType)
	    {
		case(1):
		frequentlyUsedZone = CTzLocalizedTimeZone::ECurrentZone;
		break;
		
		case(2):
		frequentlyUsedZone = CTzLocalizedTimeZone::	EHomeZone;
		break;
	
		case(3):
		frequentlyUsedZone = CTzLocalizedTimeZone::	EInterestZone;
		break;
		
		case(4):
		frequentlyUsedZone = CTzLocalizedTimeZone::	ERecentZone1;
		break;
		
		case(5):
		frequentlyUsedZone = CTzLocalizedTimeZone::	ERecentZone2;
		break;
		
		default:
		_LIT(KNoZone, "TestSetFrequentlyUsedZone: No zone type selected");
		INFO_PRINTF1(KNoZone);
		//default to ECachedTimeZones for negative testing
		frequentlyUsedZone = CTzLocalizedTimeZone::ECachedTimeZones;		
	    }
		
	
	TRAPD(err, iLocalizer = CTzLocalizer::NewL());
	
	__UHEAP_MARK;
	
	SetTestStepResult(EFail);
	if (err == KErrNone)
	    {			
        //get the city name
        CTzLocalizedCity* foundCity = NULL;     
        TRAPD(err1, foundCity = iLocalizer->FindCityByNameL(iUserAddedCity));
        CleanupStack::PushL(foundCity);
    
        if (err1 == KErrNone) 
            {
            if (foundCity != NULL)
                {
                //set the city name and zone
                TRAPD(err2, iLocalizer->SetFrequentlyUsedZoneL(*foundCity, frequentlyUsedZone));
    	        if(err2 == KErrNone)
    	            {
    	            _LIT(KFrequentlyUsedZoneSet, "TestSetFrequentlyUsedZone: Cached zone has been set");
    		        INFO_PRINTF1(KFrequentlyUsedZoneSet);
    	            }
    	        else
    	            {
    	            _LIT(KSetCacheLeft, "TestSetFrequentlyUsedZone: SetFrequentlyUsedZoneL left");
    		        ERR_PRINTF1(KSetCacheLeft);
    		        User::Leave(err2);
    	            }	
                }
            else
                {
                _LIT(KNull, "TestSetFrequentlyUsedZone: FindCityByNameL returned NULL pointer");
                ERR_PRINTF1(KNull);	
   		        User::Leave(KErrNotFound);
                }            	   	   	   	
   			CleanupStack::PopAndDestroy(foundCity);
            
			//extract city name
		    CTzLocalizedCity* returnCity = NULL;
		    TRAPD(err1, returnCity = iLocalizer->GetFrequentlyUsedZoneCityL(frequentlyUsedZone));
		    CleanupStack::PushL(returnCity);
		   
		    if (err1 == KErrNone)
		    	{
		   	    if (returnCity->Name() == iUserAddedCity)
		   	        {
    	            SetTestStepResult(EPass);
		   	        _LIT(KCorrectCity, "Get Cached Zone Step: Correct user added city found");
		   	 	    INFO_PRINTF1(KCorrectCity);
		   	        }
		   	    else
		   	        {
		   	        _LIT(KDefaultCity, "Get Cached Zone Step: Returned default city");
		   	 	    INFO_PRINTF1(KDefaultCity);
		   	        }	 
		       	}
		    else
		       	{
		       	_LIT(KGetCacheLeft, "Get Cached Zone Step: GetFrequentlyUsedZoneCityL left");
		   	   	ERR_PRINTF1(KGetCacheLeft);
				User::Leave(err1);
		       	}
		    CleanupStack::PopAndDestroy(returnCity);
            }
        else
            {
            _LIT(KFinLeft, "TestSetFrequentlyUsedZone: FindCityByNameL left");
    	    ERR_PRINTF1(KFinLeft);
			User::Leave(err1);
            }
	    }
	else
	    {
	    _LIT(KNewLeft, "TestSetFrequentlyUsedZone: NewL left");
		ERR_PRINTF1(KNewLeft);	
		User::Leave(err);
	    }
	
	__UHEAP_MARKEND;	
	return TestStepResult();		
	}
	
TVerdict CTestCheckCurrentZoneStep::doTestStepPostambleL()
/**
 * @return - TVerdict code
 * Override of base class virtual
 */
	{
	return TestStepResult();
	}

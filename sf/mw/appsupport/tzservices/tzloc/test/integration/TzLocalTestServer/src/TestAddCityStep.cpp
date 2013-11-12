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
#include "TestAddCityStep.h"
using namespace tzconstants;

CTestAddCityStep::~CTestAddCityStep()
/**
 * Destructor
 */
	{
	delete iLocalizer;
	}

CTestAddCityStep::CTestAddCityStep()
/**
 * Constructor
 */
	{
	// Call base class method to set up the human readable name for logging
	SetTestStepName(KTestAddCityStep); 
	}

TVerdict CTestAddCityStep::doTestStepPreambleL()
/**
 * @return - TVerdict code
 * Override of base class virtual
 */
	{
	if (GetIntFromConfig(ConfigSection(),KTzIdTag,iTzIdFromIni))
	    {
	    _LIT(KTzIdTagFound,"Add City Step: Tz Id tag found in the ini file");
		INFO_PRINTF1(KTzIdTagFound);
	    }
	else
	    {
	    _LIT(KTzIdNotFound,"Add City Step: Tz Id tag found in the ini file");
		ERR_PRINTF1(KTzIdNotFound);
	    }
		
	if (GetStringFromConfig(ConfigSection(),KUserDefinedCityTag, iAddedCity))
	    {
	    _LIT(KAddedCityFound,"Add City Step: Added city found in the ini file");
		INFO_PRINTF1(KAddedCityFound);
	    }
	else
	    {
	    _LIT(KAddedCityNotFound,"Add City Step: Add city not found in the ini file");
		ERR_PRINTF1(KAddedCityNotFound);
	    }
	
	if (GetIntFromConfig(ConfigSection(),KCityGroupIdIdTag,iCityGroupId))
	    {
	    _LIT(KCityGroupIdFound,"Add City Step: City Group Id tag found in the ini file");
		INFO_PRINTF1(KCityGroupIdFound);
	    }
	else
	    {
	    _LIT(KCityGroupIdNotFound,"Add City Step: CityGroupId not found in the ini file : use zero");
		INFO_PRINTF1(KCityGroupIdNotFound);
		iCityGroupId = 0;
	    }
		
	return TestStepResult();
	}

TVerdict CTestAddCityStep::doTestStepL()
/**
 * @return - TVerdict code
 * Override of base class virtual
 * Adds a user defined city to time zone with optional city group id
 */
	{
	SetTestStepResult(EFail);	
	TRAPD(err, iLocalizer = CTzLocalizer::NewL());
		
	if (err == KErrNone)
	    {
	   	CTzLocalizedCity* addedCity = NULL;	
	    TRAPD(res, addedCity = iLocalizer->AddCityL(iTzIdFromIni, iAddedCity, iCityGroupId));
    	
    	if (res == KErrNone)
    	    {	
            if (addedCity->Name() != iAddedCity)
    	        {
    	        _LIT(KUnexpectedCity,"TestAddCityStep : Unexpected City Name");
	            INFO_PRINTF1(KUnexpectedCity);		
	            }
	        else
	            {
	            _LIT(KExpectedCity,"TestAddCityStep : Expected City returned");
	            INFO_PRINTF1(KExpectedCity);
	            SetTestStepResult(EPass);	
    	        }
    	    }
    	else
    	    {
    	    _LIT(KAddCityLeft,"TestAddCityStep: AddCityL left");
    	    ERR_PRINTF1(KAddCityLeft);
    	    }
	    }	
	else
	    {
	    _LIT(KNewLLeft,"TestAddCityStep: Problem with NewL");
		ERR_PRINTF1(KNewLLeft);	
	    }
	
	return TestStepResult();	
	}
	
	
	
TVerdict CTestAddCityStep::doTestStepPostambleL()
/**
 * @return - TVerdict code
 * Override of base class virtual
 */
	{
	return TestStepResult();
	}



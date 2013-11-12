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
#include "TestRemoveCityStep.h"

using namespace tzconstants;

CTestRemoveCityStep::~CTestRemoveCityStep()
/**
 * Destructor
 */
	{
	delete iLocalizer;
	}

CTestRemoveCityStep::CTestRemoveCityStep()
/**
 * Constructor
 */
	{
	// Call base class method to set up the human readable name for logging
	SetTestStepName(KTestRemoveCityStep); 
	}

TVerdict CTestRemoveCityStep::doTestStepPreambleL()
/**
 * @return - TVerdict code
 * Override of base class virtual
 */
	{
	if (GetStringFromConfig(ConfigSection(),KUserDefinedCityTag, iRemovedCity))
	    {
	    _LIT(KRemoveCity,"Remove City Step: Remove city found in the ini file");
		INFO_PRINTF1(KRemoveCity);
	    }
	else
	    {
	    _LIT(KNoCity,"Remove City Step: Remove city not found in the ini file");
		ERR_PRINTF1(KNoCity);
	    }
	    		
	return TestStepResult();
	}

TVerdict CTestRemoveCityStep::doTestStepL()
/**
 * @return - TVerdict code
 * Override of base class virtual
 * Remove the user added city from DB. When calling FindCityByNameL()
 * a NULL check must be carried out first.
 */
	{
	SetTestStepResult(EFail);	
	TRAPD(err, iLocalizer = CTzLocalizer::NewL());	
	__UHEAP_MARK;
	
	if (err == KErrNone)
	    {
	    CTzLocalizedCity* foundCity = NULL;
	    TRAPD(err, foundCity = iLocalizer->FindCityByNameL(iRemovedCity));
	    if (err == KErrNone)
	        {	    
	        if (foundCity != NULL)
	            {
	      	    TRAPD(err1, iLocalizer->RemoveCityL(foundCity));	       
		        if (err1 == KErrNone)
		            {
		            _LIT(KCityRemoved,"Test Remove City: City is removed");
		   	        INFO_PRINTF1(KCityRemoved);
		   	        SetTestStepResult(EPass);
		            }
	            else
	                {
	                _LIT(KRemoveLeft,"Test Remove City: RemoveCityL left");
	                ERR_PRINTF1(KRemoveLeft);
	                }
	            }
	        else
	            {
	            _LIT(KNull,"Test Remove City: FindCityByNameL returned NULL pointer");
	      	    ERR_PRINTF1(KNull);	        
	            }	  		    		    	
	        }
	    else
	        {
	        _LIT(KFindCityLeft,"Test Remove City: FindCityByNameL left");
	        ERR_PRINTF1(KFindCityLeft);
	        }
	    }	
	else
	    {
	    _LIT(KNewLeft,"Test Remove City Step: Problem with NewL");
		ERR_PRINTF1(KNewLeft);	
    	}
	
	__UHEAP_MARKEND;
	return TestStepResult();	
	}
	
TVerdict CTestRemoveCityStep::doTestStepPostambleL()
/**
 * @return - TVerdict code
 * Override of base class virtual
 */
	{
	return TestStepResult();
	}


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

#include <test/testexecutelog.h>
#include "TestGetTzInfoFromDefCityStep.h"

using namespace tzconstants;

CTestGetTzInfoFromDefCityStep::~CTestGetTzInfoFromDefCityStep()
/**
 * Destructor
 */
	{
	delete iLocalizer;
	}

CTestGetTzInfoFromDefCityStep::CTestGetTzInfoFromDefCityStep()
/**
 * Constructor
 */
	{
	// Call base class method to set up the human readable name for logging
	SetTestStepName(KTestGetTzInfoFromDefCity); 
	}

TVerdict CTestGetTzInfoFromDefCityStep::doTestStepPreambleL()
/**
 * @return - TVerdict code
 * Override of base class virtual
 * Get the TzId from the section in the ini file
 */
	{
	if(!GetIntFromConfig(ConfigSection(),KTzIdTag,iTzIdFromIni))
	    {
	    _LIT(KTzIdNotFound,"TestGetTzInfoFromDefCityStep : TzId not found in ini file");
		ERR_PRINTF1(KTzIdNotFound);
	    }
	    
	return TestStepResult();
	}
	
TVerdict CTestGetTzInfoFromDefCityStep::doTestStepL()
/**
 * @return - TVerdict code
 * Override of base class virtual
 * Using the TzId from the ini file, retrieve the default city
 * for that time zone. Then use the default city to get the localized
 * time zone information.
 */
	{
	SetTestStepResult(EFail);	
	TRAPD(err, iLocalizer = CTzLocalizer::NewL());
	__UHEAP_MARK;
	
	if(err == KErrNone)
  	    {
		CTzLocalizedTimeZone* timeZone = NULL;
	    CTzLocalizedCity* defaultCity = NULL;
	
     	TRAPD(errcity, defaultCity = iLocalizer->GetDefaultCityL(iTzIdFromIni));
	    CleanupStack::PushL(defaultCity);
	    
	    if (errcity == KErrNone)
	        {
	        TRAPD(tzerr,timeZone = iLocalizer->GetLocalizedTimeZoneL(*defaultCity));
	        CleanupStack::PushL(timeZone);
	       
	        if(tzerr == KErrNone)
	            {
	        	TBool tzres;
	        	tzres = CheckReturnedTzInfo(*timeZone);
	        	if (tzres)
	        	    {
	        	    _LIT(KTzDataMatch,"TestGetTzInfoFromDefCityStep : TZ Data match");
	        		INFO_PRINTF1(KTzDataMatch);
	        		SetTestStepResult(EPass);
	        	    }
	        	else
	        	    {
	        	    _LIT(KTzNoMatch,"TestGetTzInfoFromDefCityStep : TZ Data Didnt match");
	        	    ERR_PRINTF1(KTzNoMatch);	
	        	    }	            
	            }
	        else
	            {
	            _LIT(KGetLocTzLeft,"TestGetTzInfoFromDefCityStep : GetLocalizedTimeZoneL left");
	       	    ERR_PRINTF1(KGetLocTzLeft);
	            }
	       
	        CleanupStack::PopAndDestroy(timeZone);    	   	
	        }
	    else
	        {
	        _LIT(KGetDefCityLeft,"TestGetTzInfoFromDefCityStep : GetDefaultCityL left");
	    	ERR_PRINTF1(KGetDefCityLeft);
	        }
	    
	    CleanupStack::PopAndDestroy(defaultCity);		    	
	    }
	else
	    {
	    _LIT(KNewLeft,"TestGetTzInfoFromDefCityStep: Calling NewL Failed");
		ERR_PRINTF1(KNewLeft);
	    }
		
	__UHEAP_MARKEND;
	return TestStepResult();	
	}
	
TVerdict CTestGetTzInfoFromDefCityStep::doTestStepPostambleL()
/**
 * @return - TVerdict code
 * Override of base class virtual
 */
	{
	return TestStepResult();
	}
	


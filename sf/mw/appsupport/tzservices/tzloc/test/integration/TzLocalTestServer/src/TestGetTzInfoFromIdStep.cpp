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
#include "TestGetTzInfoFromIdStep.h"

using namespace tzconstants;

CTestGetTzInfoFromIdStep::~CTestGetTzInfoFromIdStep()
/**
 * Destructor
 */
	{
	delete iLocalizer;
	}

CTestGetTzInfoFromIdStep::CTestGetTzInfoFromIdStep()
/**
 * Constructor
 */
	{
	// Call base class method to set up the human readable name for logging
	SetTestStepName(KTestGetTzInfoFromId); 
	}

TVerdict CTestGetTzInfoFromIdStep::doTestStepPreambleL()
/**
 * @return - TVerdict code
 * Override of base class virtual
 */
	{	
	if (!GetIntFromConfig(ConfigSection(),KTzIdTag,iTzIdFromIni))
	    {
	    _LIT(KTzIdNotFound,"TestGetTzInfoFromIdStep : TzId data not found in ini file");
		ERR_PRINTF1(KTzIdNotFound);
	    }
	    
	return TestStepResult();
	}
	
TVerdict CTestGetTzInfoFromIdStep::doTestStepL()
/**
 * @return - TVerdict code
 * Override of base class virtual
 * Using the TzId read from the ini file get the localized time zone information
 */
	{
	SetTestStepResult(EFail);	
	TRAPD(err,iLocalizer = CTzLocalizer::NewL());
	__UHEAP_MARK;
	
	if (err == KErrNone)
	    {
		CTzLocalizedTimeZone* timeZone = NULL;	
    	TRAPD(tzerr,timeZone = iLocalizer->GetLocalizedTimeZoneL(iTzIdFromIni));
		CleanupStack::PushL(timeZone);
		
	    if(tzerr == KErrNone)
	        {
	    	TBool tzres;
	    	tzres = CheckReturnedTzInfo(*timeZone);
	    	if (tzres)
	    	    {
	    		SetTestStepResult(EPass);
	    		_LIT(KTzMatch,"TestGetTzInfoFromIdStep : Tz Data match");
	    		INFO_PRINTF1(KTzMatch);	
	    	    }
	    	else
	    	    {
	    	    _LIT(KTzNoMatch,"TestGetTzInfoFromIdStep : Tz Data Didnt match");
	    	    ERR_PRINTF1(KTzNoMatch);
	    	    }
	        }
	    else
	        {
	        _LIT(KGetTzLeft,"TestGetTzInfoFromIdStep : GetLocalizedTimeZoneL left");
	    	ERR_PRINTF1(KGetTzLeft);	
	        }
			
		CleanupStack::PopAndDestroy(timeZone);	
	    }
	else
	    {
	    _LIT(KNewLeft,"TestGetTzInfoFromIdStep: NewL Left");
		ERR_PRINTF1(KNewLeft);	
	    }
		
	__UHEAP_MARKEND;
	return TestStepResult();
	}
	
TVerdict CTestGetTzInfoFromIdStep::doTestStepPostambleL()
/**
 * @return - TVerdict code
 * Override of base class virtual
 */
	{
	return TestStepResult();
	}
	


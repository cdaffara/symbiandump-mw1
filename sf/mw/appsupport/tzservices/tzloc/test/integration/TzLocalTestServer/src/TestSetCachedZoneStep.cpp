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
#include "TestSetCachedZoneStep.h"

using namespace tzconstants;

CTestSetFrequentlyUsedZoneStep::~CTestSetFrequentlyUsedZoneStep()
/**
 * Destructor
 */
	{
	delete iLocalizer;
	}

CTestSetFrequentlyUsedZoneStep::CTestSetFrequentlyUsedZoneStep()
/**
 * Constructor
 */
	{
	// Call base class method to set up the human readable name for logging
	SetTestStepName(KTestSetFrequentlyUsedZoneStep); 
	}

TVerdict CTestSetFrequentlyUsedZoneStep::doTestStepPreambleL()
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
	
TVerdict CTestSetFrequentlyUsedZoneStep::doTestStepL()
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
	CTzLocalizedTimeZone::TTzFrequentlyUsedZone FrequentlyUsedZone;
	
	switch (iFrequentlyUsedZoneType)
	    {
		case(1):
		FrequentlyUsedZone = CTzLocalizedTimeZone::ECurrentZone;
		break;
		
		case(2):
		FrequentlyUsedZone = CTzLocalizedTimeZone::	EHomeZone;
		break;
	
		case(3):
		FrequentlyUsedZone = CTzLocalizedTimeZone::	EInterestZone;
		break;
		
		case(4):
		FrequentlyUsedZone = CTzLocalizedTimeZone::	ERecentZone1;
		break;
		
		case(5):
		FrequentlyUsedZone = CTzLocalizedTimeZone::	ERecentZone2;
		break;
		
		default:
		_LIT(KNoZone,"TestSetFrequentlyUsedZone: No zone type selected");
		INFO_PRINTF1(KNoZone);
		//default to ECachedTimeZones for negative testing
		FrequentlyUsedZone = CTzLocalizedTimeZone::ECachedTimeZones;		
	    }
		
	
	TRAPD(err, iLocalizer = CTzLocalizer::NewL());
	
	__UHEAP_MARK;
	
	if (err == KErrNone)
	    {			
        CTzLocalizedCity* foundCity = NULL;     
        TRAPD(err1,foundCity = iLocalizer->FindCityByNameL(iUserAddedCity));
        CleanupStack::PushL(foundCity);
    
        if (err1 == KErrNone) 
            {
            if (foundCity != NULL)
                {
                TRAPD(err2,iLocalizer->SetFrequentlyUsedZoneL(*foundCity, FrequentlyUsedZone));
    	        if(err2 == KErrNone)
    	            {
    	            SetTestStepResult(EPass);
    	            _LIT(KFrequentlyUsedZoneSet,"TestSetFrequentlyUsedZone: Cached zone has been set");
    		        INFO_PRINTF1(KFrequentlyUsedZoneSet);
    	            }
    	        else
    	            {
    	            _LIT(KSetCacheLeft,"TestSetFrequentlyUsedZone: SetFrequentlyUsedZoneL left");
    		        ERR_PRINTF1(KSetCacheLeft);
    	            }	
                }
            else
                {
                _LIT(KNull,"TestSetFrequentlyUsedZone: FindCityByNameL returned NULL pointer");
                ERR_PRINTF1(KNull);	
                }            	   	   	   	
            }
        else
            {
            _LIT(KFinLeft,"TestSetFrequentlyUsedZone: FindCityByNameL left");
    	    ERR_PRINTF1(KFinLeft);
            }
     
   	CleanupStack::PopAndDestroy(foundCity);
	    }
	else
	    {
	    _LIT(KNewLeft,"TestSetFrequentlyUsedZone: NewL left");
		ERR_PRINTF1(KNewLeft);	
	    }
	
	__UHEAP_MARKEND;	
	return TestStepResult();		
	}
	
TVerdict CTestSetFrequentlyUsedZoneStep::doTestStepPostambleL()
/**
 * @return - TVerdict code
 * Override of base class virtual
 */
	{
	return TestStepResult();
	}

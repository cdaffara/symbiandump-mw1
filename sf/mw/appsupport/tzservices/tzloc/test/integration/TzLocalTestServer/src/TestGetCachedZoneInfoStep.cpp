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
#include "testserver.h"
#include "TestGetCachedZoneInfoStep.h"

using namespace tzconstants;

CTestGetFrequentlyUsedZoneInfoStep::~CTestGetFrequentlyUsedZoneInfoStep()
/**
 * Destructor
 */
	{
	delete iLocalizer;
	}

CTestGetFrequentlyUsedZoneInfoStep::CTestGetFrequentlyUsedZoneInfoStep()
/**
 * Constructor
 */
	{
	// Call base class method to set up the human readable name for logging
	SetTestStepName(KTestGetFrequentlyUsedZoneInfoStep); 
	}
	
	
TVerdict CTestGetFrequentlyUsedZoneInfoStep::doTestStepPreambleL()
/**
 * @return - TVerdict code
 * Override of base class virtual
 */
	
	{	
	if (GetStringFromConfig(ConfigSection(),KUserDefinedCityTag,iUserAddedCity))
	    {
	    _LIT(KFoundCity,"Get Cached Zone Step: User added city found in the ini file");
		INFO_PRINTF1(KFoundCity);
	    }
	else
	    {
	    _LIT(KCityNotFound,"Get Cached Zone Step: User added city not found in the ini file");
		ERR_PRINTF1(KCityNotFound);
	    }
		
	if (GetIntFromConfig(ConfigSection(),KCachedTzTag,iFrequentlyUsedZoneType))
	    {
	    _LIT(KCachedTzFound,"Get Cached Zone Step: Cached tz found in the ini file");
		INFO_PRINTF1(KCachedTzFound);
	    }
	else
	    {
	    _LIT(KCachedTzNotFound,"Get Cached Zone Step: Cached Tz not found in the ini file");
		ERR_PRINTF1(KCachedTzNotFound);
	    }	
	
	// The Central Repository server caches the contents of repositories for 2
	// minutes.  Some test cases copy TZ Server repositories with different
	// content to the Central Repository private data cage.  These test cases
	// require the Central Repository server to be killed to ensure that the new
	// content of the TZ Server repository is used and not the cached content.
	KillCenRepServerL();
	
	return TestStepResult();
	}
	
TVerdict CTestGetFrequentlyUsedZoneInfoStep::doTestStepL()
/**
 * @return - TVerdict code
 * Override of base class virtual
 * Get the city used to set the cached zone, then get the time zone
 * information object for the specified cached zone.
 */
	{
	SetTestStepResult(EFail);
	CTzLocalizedTimeZone::TTzFrequentlyUsedZone aFrequentlyUsedZone;
	
	switch (iFrequentlyUsedZoneType)
	    {
		case(1):
		aFrequentlyUsedZone = CTzLocalizedTimeZone::ECurrentZone;
		break;
		
		case(2):
		aFrequentlyUsedZone = CTzLocalizedTimeZone::EHomeZone;
		break;
	
		case(3):
		aFrequentlyUsedZone = CTzLocalizedTimeZone::EInterestZone;
		break;
		
		case(4):
		aFrequentlyUsedZone = CTzLocalizedTimeZone::ERecentZone1;
		break;
		
		case(5):
		aFrequentlyUsedZone = CTzLocalizedTimeZone::ERecentZone2;
		break;
		
		default:
		_LIT(KNoZone,"TestSetFrequentlyUsedZone: No zone type selected");
		INFO_PRINTF1(KNoZone);
		//default setting for negative tests
		aFrequentlyUsedZone = CTzLocalizedTimeZone::ECachedTimeZones;
	    }
	
	TRAPD(err, iLocalizer = CTzLocalizer::NewL());
	__UHEAP_MARK;
	
	if (err == KErrNone)
	    {
	    CTzLocalizedCity* returnCity = NULL;
	    TRAPD(err1,returnCity = iLocalizer->GetFrequentlyUsedZoneCityL(aFrequentlyUsedZone));
	    CleanupStack::PushL(returnCity);
	   
	    if (err1 == KErrNone)
	        {
	   	    if (returnCity->Name() == iUserAddedCity)
	   	        {
	   	        _LIT(KCorrectCity,"Get Cached Zone Step: Correct user added city found");
	   	 	    INFO_PRINTF1(KCorrectCity);
	   	        }
	   	    else
	   	        {
	   	        _LIT(KDefaultCity,"Get Cached Zone Step: Returned default city");
	   	 	    INFO_PRINTF1(KDefaultCity);
	   	        }	 
	       }
	    else
	       {
	       _LIT(KGetCacheLeft,"Get Cached Zone Step: GetFrequentlyUsedZoneCityL left");
	   	   ERR_PRINTF1(KGetCacheLeft);
	       }
	   
	    CleanupStack::PopAndDestroy(returnCity);
	   
	    CTzLocalizedTimeZone* timeZone = NULL;
	    TRAPD(tzerr,timeZone = iLocalizer->GetFrequentlyUsedZoneL(aFrequentlyUsedZone));
	    CleanupStack::PushL(timeZone);
	   
	    if(tzerr == KErrNone)
	        {
	        TBool tzres;
	        tzres = CheckReturnedTzInfo(*timeZone);
	        if (tzres && !err1 )
	            {
	            _LIT(KMatch,"Get cached zone Step : TZ Data match");
	      	    INFO_PRINTF1(KMatch);
	      	    SetTestStepResult(EPass);
	            }
	        else
	            {
	            _LIT(KNoMatch,"Get cached zone Step : TZ Data Didnt match");
	            ERR_PRINTF1(KNoMatch);	
	            }	            
	        }
	    else
	        {
	        _LIT(KGetCacheLeft,"Get cached zone Step : GetFrequentlyUsedZoneL left");
	   	    ERR_PRINTF1(KGetCacheLeft);
	        }
	   	   
	    CleanupStack::PopAndDestroy(timeZone);
	   
	    }
	else
	    {
	    _LIT(KNewLeft,"Get Cached Zone Info: Problem with NewL");
	    ERR_PRINTF1(KNewLeft);
	    }
	
	__UHEAP_MARKEND;	
	return TestStepResult();			
	}
	
TVerdict CTestGetFrequentlyUsedZoneInfoStep::doTestStepPostambleL()
/**
 * @return - TVerdict code
 * Override of base class virtual
 */
	{
	// See script file for why this is done.
	KillCenRepServerL();
	
	return TestStepResult();
	}

/**
Kill the Central Repository server if the test configuration requires it.
*/
void CTestGetFrequentlyUsedZoneInfoStep::KillCenRepServerL()
	{
	TBool killCenRepServer = EFalse;

	GetBoolFromConfig(ConfigSection(), _L("KillCenRepServer"), killCenRepServer);

	if (killCenRepServer == EFalse)
		{
		return;
		}

	RPIMTestServer serv;
	User::LeaveIfError(serv.Connect());
	CleanupClosePushL(serv);
   	
	_LIT(KCenRepServer,"centralrepositorysrv*");
	TRAP_IGNORE(serv.KillProcessL(KCenRepServer));
   	
	CleanupStack::PopAndDestroy(&serv);

    _LIT(KCenRepServerKilled, "Central Repository server has been killed.");
	INFO_PRINTF1(KCenRepServerKilled);
	}

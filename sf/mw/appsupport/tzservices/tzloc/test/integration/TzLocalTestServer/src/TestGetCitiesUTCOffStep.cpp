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
#include "TestGetCitiesUTCOffStep.h"

using namespace tzconstants;

CTestGetCitiesUTCOffStep::~CTestGetCitiesUTCOffStep()
/**
 * Destructor
 */
	{
	delete iLocalizer;
	}

CTestGetCitiesUTCOffStep::CTestGetCitiesUTCOffStep()
/**
 * Constructor
 */
	{
	// Call base class method to set up the human readable name for logging
	SetTestStepName(KTestGetCitiesUTCOffStep); 
	}


TVerdict CTestGetCitiesUTCOffStep::doTestStepPreambleL()
/**
 * @return - TVerdict code
 * Override of base class virtual
 */
    {  
    if(!GetIntFromConfig(ConfigSection(),KUtcOffsetTag,iUtcOffset))
        {
        _LIT(KNoUtcOffset,"TestGetCitiesUTCOffset : No UTC Offset in ini");
    	ERR_PRINTF1(KNoUtcOffset);
        }       
        
    return TestStepResult(); 	 
    }
    
   
TVerdict CTestGetCitiesUTCOffStep::doTestStepL()
/**
 * @return - TVerdict code
 * Override of base class virtual
 */
    {            
 	SetTestStepResult(EPass);
    TRAPD(err, iLocalizer = CTzLocalizer::NewL());
    __UHEAP_MARK;
    
    if (err == KErrNone)
        {
        CTzLocalizedCityArray* cities = NULL;
        TRAPD(res,cities = iLocalizer->GetCitiesWithUTCOffsetL(iUtcOffset,CTzLocalizer::ETzAlphaNameAscending));
        CleanupStack::PushL(cities);
   
        if (res == KErrNone)
            {
            TBuf<KTagLength>cityTag;
        	TInt numCities = cities->Count();
        	for(TInt n=0; n<numCities; n++)
        	    {
        		cityTag = KCityTag;
        		cityTag.AppendNum(n+1);
        		TPtrC cityFromIni;
        		GetStringFromConfig(ConfigSection(),cityTag,cityFromIni);
        		if (cities->At(n).Name() != cityFromIni)
        		    {
        		    SetTestStepResult(EFail);
        			ERR_PRINTF1(cities->At(n).Name());
        		    }       		
        	    }
            }
        else
            {
            SetTestStepResult(EFail);
        	_LIT(KGetCitiesLeft,"TestGetCitiesUTCOffStep: GetCitiesWithUTCOffsetL Left");
		    ERR_PRINTF1(KGetCitiesLeft);
            }
            
        CleanupStack::PopAndDestroy(cities);
               
        }
    else
	    {
	    SetTestStepResult(EFail);
	    _LIT(KNewLLeft,"TestGetCitiesUTCOffStep: Problem with NewL");
		ERR_PRINTF1(KNewLLeft);
	    }
	
	__UHEAP_MARKEND;    
	return TestStepResult();	
    }

TVerdict CTestGetCitiesUTCOffStep::doTestStepPostambleL()
/**
 * @return - TVerdict code
 * Override of base class virtual
 */
	{
	return TestStepResult();
	}
	

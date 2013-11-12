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
#include "TestGetSortedCitiesStep.h"
#include <tz.h>
#ifdef SYMBIAN_ENABLE_SPLIT_HEADERS
#include <tzuserdefineddata.h>
#endif

using namespace tzconstants;

CTestGetSortedCitiesStep::~CTestGetSortedCitiesStep()
/**
 * Destructor
 */
	{
	delete iLocalizer;
	}

CTestGetSortedCitiesStep::CTestGetSortedCitiesStep()
/**
 * Constructor
 */
	{
	// Call base class method to set up the human readable name for logging
	SetTestStepName(KTestGetSortedCitiesStep); 
	}

TVerdict CTestGetSortedCitiesStep::doTestStepPreambleL()
/**
 * @return - TVerdict code
 * Override of base class virtual
 */
 	{
 	iScheduler = new CActiveScheduler();
	CActiveScheduler::Install(iScheduler);
 	
 	TInt SortKeyFromIni;
 	iDataSource = CTzLocalizer::ETzDataSourceSystem;
 	TInt dataSourceFlag;
 	
    GetIntFromConfig(ConfigSection(), KDataSourceTag, dataSourceFlag);
	if(dataSourceFlag == 1)
		{//iDataSource is default value ETzDataSourceSystem. Test system localization data which is default behaviour
	     _LIT(KDataSourceSystem,"Test GetSortedCitiesStep: Test system localization data - default");
		 INFO_PRINTF1(KDataSourceSystem);
		}
	else if (dataSourceFlag == 2)
		{//Test system localization data and user data"
		iDataSource = CTzLocalizer::ETzDataSourceUser;
	     _LIT(KDataSourceUser,"Test GetSortedCitiesStep: Test user data");
	     INFO_PRINTF1(KDataSourceUser);
		}
	else if (dataSourceFlag == 3)
		{//Test system localization data and user data"
		iDataSource = iDataSource|CTzLocalizer::ETzDataSourceUser;
		_LIT(KDataSourceSystemAndUser,"Test GetSortedCitiesStep: Test system localization data and user data");
		INFO_PRINTF1(KDataSourceSystemAndUser);
		}

 	if (GetIntFromConfig(ConfigSection(),KSortKeyTag,SortKeyFromIni))
	    {
		    
	    _LIT(KSortKey,"Test Get Sorted Cities: Sort Key tag found in ini file");
		INFO_PRINTF1(KSortKey);

    	switch(SortKeyFromIni)
    	    {		
        	case (1) :
	         {
	    	iSortOrder = CTzLocalizer::ETzUnsorted;
	    	_LIT(KUnsorted,"Unsorted");
    		INFO_PRINTF1(KUnsorted);
	         }
	    	break;
		
	        case (2) :
	         {
	        iSortOrder = CTzLocalizer::ETzUTCAscending;
	        _LIT(KUTCAscending,"UTCAscending");
	    	INFO_PRINTF1(KUTCAscending);
	         }
	    	break;
		
        	case (3) :
	         {
        	iSortOrder = CTzLocalizer::ETzUTCDescending;
        	_LIT(KUTCDescending,"UTCDescending");
	    	INFO_PRINTF1(KUTCDescending);
	         }
	    	break;
		
	        case (4) :
	         {
        	iSortOrder = CTzLocalizer::ETzAlphaNameAscending;
        	_LIT(KAlphaNameAscending,"AlphaNameAscending");
	     	INFO_PRINTF1(KAlphaNameAscending);
	         }
	    	break;
		
        	case (5) :
	         {
	        iSortOrder = CTzLocalizer::ETzAlphaNameDescending;
	        _LIT(KAlphaNameDescending,"AlphaNameDescending");
	    	INFO_PRINTF1(KAlphaNameDescending);
	         }
	    	break;
		
        	case (6) :
	         {
        	iSortOrder = CTzLocalizer::ETzAlphaStandardNameAscending;
        	_LIT(KAlphaStandardNameAscending,"AlphaStandardNameAscending");
	    	INFO_PRINTF1(KAlphaStandardNameAscending);
	         }
	    	break;
		
        	case (7) :
	         {
	        iSortOrder = CTzLocalizer::ETzAlphaDaylightNameAscending;
	        _LIT(KAlphaDaylightNameAscending,"AlphaDaylightNameAscending");
	    	INFO_PRINTF1(KAlphaDaylightNameAscending);
	         }
	    	break;
		
        	case (8) :
	         {
        	iSortOrder = CTzLocalizer::ETzAlphaShortStandardNameAscending;
        	_LIT(KAlphaShortStandardNameAscending,"AlphaShortStandardNameAscending");
	    	INFO_PRINTF1(KAlphaShortStandardNameAscending);
	         }
	    	break;
		
        	case (9) :
	         {
        	iSortOrder = CTzLocalizer::ETzAlphaShortDaylightNameAscending;
        	_LIT(KAlphaShortDaylightNameAscending,"AlphaShortDaylightNameAscending");
	    	INFO_PRINTF1(KAlphaShortDaylightNameAscending);
	         }
	    	break;
		
        	case (10) :
	         {
        	iSortOrder = CTzLocalizer::ETzAlphaStandardNameDescending;
        	_LIT(KAlphaStandardNameDescending,"AlphaStandardNameDescending");
	    	INFO_PRINTF1(KAlphaStandardNameDescending);
	         }
	    	break;
		
        	case (11) :
	         {
    	    iSortOrder = CTzLocalizer::ETzAlphaDaylightNameDescending;
    	    _LIT(KAlphaDaylightNameDescending,"AlphaDaylightNameDescending");
	    	INFO_PRINTF1(KAlphaDaylightNameDescending);
	         }
	    	break;
		
        	case (12) :
	         {
        	iSortOrder = CTzLocalizer::ETzAlphaShortStandardNameDescending;
        	_LIT(KAlphaShortStandardNameDescending,"AlphaShortStandardNameDescending");
	    	INFO_PRINTF1(KAlphaShortStandardNameDescending);
	         }
	    	break;
		
         	case (13) :
	         {
        	iSortOrder = CTzLocalizer::ETzAlphaShortDaylightNameDescending;
        	_LIT(KAlphaShortDaylightNameDescending,"AlphaShortDaylightNameDescending");
	    	INFO_PRINTF1(KAlphaShortDaylightNameDescending);
	         }
	    	break;		
    	    }
        }
	else
	    {
	    _LIT(KNoSortKey,"Test Get Sorted Cities: Sort Key not found in ini file");
	    ERR_PRINTF1(KNoSortKey);
	    }
 		 	
 	return TestStepResult();	
 	}

 	
TVerdict CTestGetSortedCitiesStep::doTestStepL()
/**
 * @return - TVerdict code
 * Override of base class virtual
 * The test is tesing GetCitiesL() with various input parameters,
 * firstly the TzId and sort key, then a default city and sort key
 * and finally just a sort key so all cities are found.
 */
	{ 
   	SetTestStepResult(EPass);
    TRAPD(errL, iLocalizer = CTzLocalizer::NewL());
 	__UHEAP_MARK;

 	RTz tz;
    User::LeaveIfError(tz.Connect());
    CleanupClosePushL(tz);
    CTzUserData* userdata = CTzUserData::NewL(tz);
    CleanupStack::PushL(userdata); 
 	//CreateAUserRuleLC should be in doTestStepPreambleL and clean the data in postamble when Shazia submits her code
    CreateUserRuleLC(*userdata);
   	iLocalizer->SetDataSource(iDataSource);
 	if (errL == KErrNone)
	    {
	    CTzLocalizedCityArray* cities = NULL;		  	  	  	  
	 	TInt tzId;
	  	TPtrC defaultCityFromIni; 
	  	  
	  	if (GetIntFromConfig(ConfigSection(),KTzIdTag,tzId))
	  	    {
	  	    _LIT(KTzIdFound,"Get sorted Cities : tz id tag found");
	  	    INFO_PRINTF1(KTzIdFound);
	  	  	TRAPD(err1, cities = iLocalizer->GetCitiesL(tzId, iSortOrder));
	  	  	if (err1 != KErrNone)
	  	  	    {
	  	  	    _LIT(KGetCitiesLeft,"Get sorted Cities : GetCitiesL left");
	  	  	    ERR_PRINTF1(KGetCitiesLeft);
	  	  	    SetTestStepResult(EFail); 	
	  	  	    }
	  	  	    
	  	  	    CleanupStack::PushL(cities);	  	  	
	  	    }
	  	else if (GetStringFromConfig(ConfigSection(),KDefaultCityTag,defaultCityFromIni))
	  	    {	  	  	
	  	    TBuf<40> buf;
	  	    buf.Copy(defaultCityFromIni);
	  	    _LIT(KDefCityFound,"Get sorted Cities : Default City Tag found = %S");
            INFO_PRINTF2(KDefCityFound, &buf);
	  	  	CTzLocalizedCity* defaultCity = NULL;
	  	    TRAPD(errdefcity, defaultCity = iLocalizer->GetDefaultCityL(KDefaultTzId));
	  	    if (errdefcity == KErrNone)
	            {
	 	        CleanupStack::PushL(defaultCity);
	  	        TRAPD(err2, cities = iLocalizer->GetCitiesL(*defaultCity,iSortOrder));
	  	  	    if (err2 != KErrNone)	
	  	  	        {
	  	  	        _LIT(KGetCities2Left,"Get sorted Cities : GetCitiesL(city,sort) left");
	  	  	        ERR_PRINTF1(KGetCities2Left);
	  	  	        SetTestStepResult(EFail); 
	  	  	        }
	  	  	   
	  	  	    CleanupStack::PopAndDestroy(defaultCity);
	  	  	    CleanupStack::PushL(cities);
	  	  	    }
	  	  	else
	  	  	    {
	  	  	    _LIT(KGetDefCityLeft,"Get sorted Cities : GetDefaultCity left");
	  	  	    ERR_PRINTF1(KGetDefCityLeft);
	  	  	   	SetTestStepResult(EFail);
	  	  	    }	  	  		  	  	
	  	    }	  	  	  	  	  	  
	  	else
	  	    {
	  	  	TRAPD(err3, cities = iLocalizer->GetCitiesL(iSortOrder));
	  	  	if (err3 != KErrNone)
	  	  	    {
	  	  	    _LIT(KGetCities3Left,"Get sorted Cities : GetCities(sort) left");
	  	  		ERR_PRINTF1(KGetCities3Left);
	  	  	   	SetTestStepResult(EFail);
	  	  	    }
	  	  	CleanupStack::PushL(cities);
	  	    }
	  	
	  	//Loop through the cities array and match against ini file data  	  	  
        if (cities != NULL)
            {
            TBuf<50> cityTag;
            for(TInt n=0; n < cities->Count(); n++)
                {
                cityTag = KCityTag;
                cityTag.AppendNum(n + 1);
          	
                TPtrC CityNameFromIni;	
                TPtrC temp(cities->At(n).Name() );         	
                if ( !GetStringFromConfig(ConfigSection(), cityTag, CityNameFromIni)) //i.e. cities->At(n).Name() != CityNameFromIni)
                    {          
					if ( (&temp)->Length()!=0  )
						{
						if ( (&CityNameFromIni)->Length()!=0)
							{
							_LIT(KMessage,"Got city: %S, instead of: %S");
		           	        ERR_PRINTF3(KMessage, &temp, &CityNameFromIni);
							}
						else
							{
							_LIT(KMessage,"Got city: %S, Was expecting NULL");
           	        		ERR_PRINTF2(KMessage, &temp);
							}
						}
					else
						{
						_LIT(KMessage,"Got NULL, Was expecting city: %S");
           	        	ERR_PRINTF2(KMessage, &CityNameFromIni);
						}
           	        SetTestStepResult(EFail);          		
                    }
                else if (cities->At(n).Name()!=CityNameFromIni)
                	{
                   	_LIT(KNoMatch,"TestGetSortedCitiesStep: City names do not match.  Expected %S, got %S.");
                   	
                   	ERR_PRINTF3(KNoMatch, &CityNameFromIni, &temp);      
                	SetTestStepResult(EFail);  
                	}
                }
            }
            CleanupStack::PopAndDestroy(cities);    	
	    }
    else
	    {
	    SetTestStepResult(EFail);
	    _LIT(KNewLeft,"TestGetSortedCitiesStep: Problem with NewL");
		ERR_PRINTF1(KNewLeft);	
     	}
	
   //the following doesn't need when Shazia submits her code
   	CleanupStack::PopAndDestroy(2);
	 
    __UHEAP_MARK;
	return TestStepResult();
	}
	
TVerdict CTestGetSortedCitiesStep::doTestStepPostambleL()
/**
 * @return - TVerdict code
 * Override of base class virtual
 */
	{
	ResetTzUserDataL();
	delete iScheduler;
	iScheduler = NULL;
	CActiveScheduler::Install(NULL);
	return TestStepResult();
	}


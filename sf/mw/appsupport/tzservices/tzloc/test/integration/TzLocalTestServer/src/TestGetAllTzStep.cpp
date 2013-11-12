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
#include "TestGetAllTzStep.h"
#include <tz.h>
#ifdef SYMBIAN_ENABLE_SPLIT_HEADERS
#include <tzuserdefineddata.h>
#endif

using namespace tzconstants;

CTestGetAllTzStep::~CTestGetAllTzStep()
/**
 * Destructor
 */
	{
	delete iLocalizer;
	}

CTestGetAllTzStep::CTestGetAllTzStep()
/**
 * Constructor
 */
	{
	// Call base class method to set up the human readable name for logging
	SetTestStepName(KTestGetAllTzStep); 
	}

TVerdict CTestGetAllTzStep::doTestStepPreambleL()
/**
 * @return - TVerdict code
 * Override of base class virtual
 */
 	{
 	
 	iScheduler = new CActiveScheduler();
	CActiveScheduler::Install(iScheduler);
	iDataSource = CTzLocalizer::ETzDataSourceSystem;
	TInt dataSourceFlag;
 	if(GetIntFromConfig(ConfigSection(),KDataSourceTag,dataSourceFlag))
		{
		if(dataSourceFlag == 1)
			{//iDataSource is default value ETzDataSourceSystem. Test system localization data which is default behaviour
		     _LIT(KDataSourceSystem,"Test GetAllTzStep: Test system localization data - default");
			 INFO_PRINTF1(KDataSourceSystem);
			}
		else if (dataSourceFlag == 2)
			{//Test system localization data and user data"
			iDataSource = CTzLocalizer::ETzDataSourceUser;
		     _LIT(KDataSourceUser,"Test GetAllTzStep: Test user data");
		     INFO_PRINTF1(KDataSourceUser);
			}
		else if (dataSourceFlag == 3)
			{//Test system localization data and user data"
			iDataSource = iDataSource|CTzLocalizer::ETzDataSourceUser;
			_LIT(KDataSourceSystemAndUser,"Test GetAllTzStep: Test system localization data and user data");
			INFO_PRINTF1(KDataSourceSystemAndUser);
			}
		}
 	
 	TInt SortKeyFromIni;
	 if (GetIntFromConfig(ConfigSection(),KSortKeyTag,SortKeyFromIni))
	     {
	     _LIT(KSortFound,"Test GetAllTzStep: Sort Key tag found in ini file");
		 INFO_PRINTF1(KSortFound);

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
	     _LIT(KNoSort,"Test GetAllTzStep : Sort Key tag not found in ini file");
		 ERR_PRINTF1(KNoSort);
	     }
 		 	
 	 return TestStepResult();	
 	}

TVerdict CTestGetAllTzStep::doTestStepL()
/**
 * @return - TVerdict code
 * Override of base class virtual
 * Tes of GetAllTimeZonesL()
 */
	{ 
    SetTestStepResult(EPass);
    TRAPD(err, iLocalizer = CTzLocalizer::NewL());
	__UHEAP_MARK;
  	
	RTz tz;
    User::LeaveIfError(tz.Connect());
    CleanupClosePushL(tz);
    CTzUserData* userdata = CTzUserData::NewL(tz);
    CleanupStack::PushL(userdata); 
    CreateUserRuleLC(*userdata);
  
   	iLocalizer->SetDataSource(iDataSource);
   			
	if (err == KErrNone)
        {
	    CTzLocalizedTimeZoneArray* allTz = NULL;
	    TRAPD(err1, allTz = iLocalizer->GetAllTimeZonesL(iSortOrder));
	    if(err1 == KErrNone)
	        {
	        CleanupStack::PushL(allTz);
	        TInt numTz = allTz->Count();
	        
	        for(TInt i = 0; i < numTz; i++)
	            {
	        	TBuf<KTagLength> tzTag;
                tzTag = KStandardNameTag;
                TPtrC tzNameFromIni;
            	tzTag.AppendNum(i + 1);
                          	
            	if( !GetStringFromConfig(ConfigSection(),tzTag,tzNameFromIni) ) //i.e. allTz->At(i).StandardName() != tzNameFromIni
            	    {       		
               		_LIT(KSXint,"Tz zone id was %d - at iter = %d ");
     		    	INFO_PRINTF3(KSXint, allTz->At(i).TimeZoneId(), i+1 );
					_LIT(KSX1,"TestGetAllTzStep: Tz StandardName and ShortStandardName was %S and %S !");
					TPtrC temp(allTz->At(i).StandardName() );
				
					if ( (&temp)->Length()!=0  )
 						{
						TPtrC temp1=allTz->At(i).ShortStandardName() ;
  						INFO_PRINTF3(KSX1, &temp, &temp1 );
                  		SetTestStepResult(EFail);
						}
	           	    }
            	else if (allTz->At(i).StandardName() != tzNameFromIni)
            		{
                 	_LIT(KNoMatch,"TestGetAllTzStep: Tz Names Didnt Match, expected: %S from inifile");
               		ERR_PRINTF2(KNoMatch, &tzNameFromIni);
            		SetTestStepResult(EFail);
            		}
	            } //for
	            
	        CleanupStack::PopAndDestroy(allTz);    
	        
	        }
	    else
	        {
	        SetTestStepResult(EFail);
	        _LIT(KGetLeft,"TestGetAllTzStep: GetAllTimeZonesL Left");
	        ERR_PRINTF1(KGetLeft);	
	        }
        }
	else
	    {
	    SetTestStepResult(EFail);
	    _LIT(KNewLeft,"TestGetAllTzStep: Problem with NewL");
		ERR_PRINTF1(KNewLeft);	
     	}

    
  	
    //the following doesn't need when Shazia submits her code
   	CleanupStack::PopAndDestroy(2);
  
   	__UHEAP_MARK;
	return TestStepResult();
	
	}
	
TVerdict CTestGetAllTzStep::doTestStepPostambleL()
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



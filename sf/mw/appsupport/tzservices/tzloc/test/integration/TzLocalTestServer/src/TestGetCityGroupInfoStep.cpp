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
#include "TestGetCityGroupInfoStep.h"
#include <tz.h>
#ifdef SYMBIAN_ENABLE_SPLIT_HEADERS
#include <tzuserdefineddata.h>
#endif

using namespace tzconstants;

CTestGetCityGroupInfoStep::~CTestGetCityGroupInfoStep()
/**
 * Destructor
 */
	{
	delete iLocalizer;
	}

CTestGetCityGroupInfoStep::CTestGetCityGroupInfoStep()
/**
 * Constructor
 */
	{
	// Call base class method to set up the human readable name for logging
	SetTestStepName(KTestGetCityGroupInfoStep); 
	}

TVerdict CTestGetCityGroupInfoStep::doTestStepPreambleL()
/**
 * @return - TVerdict code
 * Override of base class virtual
 */
	{
	iDataSource = CTzLocalizer::ETzDataSourceSystem;
	TInt dataSourceFlag;
	if(GetIntFromConfig(ConfigSection(),KDataSourceTag,dataSourceFlag))
		{
		if(dataSourceFlag == 1)
			{//iDataSource is default value CTzLocalizer::ETzDataSourceSystem. Test system localization data which is default behaviour
		     _LIT(KDataSourceSystem,"Test GetCityGroupInfoStep: Test system localization data - default");
			 INFO_PRINTF1(KDataSourceSystem);
			}
		else if (dataSourceFlag == 2)
			{//Test user data source"
			iDataSource = CTzLocalizer::ETzDataSourceUser;
		     _LIT(KDataSourceUser,"Test GetCityGroupInfoStep: Test user data");
		     INFO_PRINTF1(KDataSourceUser);
			}
		else if (dataSourceFlag == 3)
			{//Test system localization data and user data source"
			iDataSource = iDataSource|CTzLocalizer::ETzDataSourceUser;
			_LIT(KDataSourceSystemAndUser,"Test GetCityGroupInfoStep: Test system localization data and user data");
			INFO_PRINTF1(KDataSourceSystemAndUser);
			}
		}
	return TestStepResult();
	}

TVerdict CTestGetCityGroupInfoStep::doTestStepL()
/**
 * @return - TVerdict code
 * Override of base class virtual
 */
	{	
	SetTestStepResult(EPass);	
	TRAPD(err, iLocalizer = CTzLocalizer::NewL());
	__UHEAP_MARK;

 	//CreateAUserRuleLC should be in doTestStepPreambleL and clean the data in postamble when Shazia submits her code
	RTz tz;
    User::LeaveIfError(tz.Connect());
    CleanupClosePushL(tz);
    CTzUserData* userdata = CTzUserData::NewL(tz);
    CleanupStack::PushL(userdata); 
	CreateUserRuleLC(*userdata);
	iLocalizer->SetDataSource(iDataSource);
	if (err == KErrNone)
	    {
	    //Test Get all group sorted by names (ascending).
	    TRAPD(res, GetAllGroupsL(iLocalizer, CTzLocalizer::ETzAlphaNameAscending));
	    if (res != KErrNone)
          {           
          SetTestStepResult(EFail);
          _LIT(KRes,"TestGetCityGroupsInfoStep: GetCityGroupL (ascending) Left");
          ERR_PRINTF1(KRes);
          }
	   
	    //Test Get all group sorted by names (descending).
	 	TRAPD(res1, GetAllGroupsL(iLocalizer, CTzLocalizer::ETzAlphaNameDescending));
        if (res1!= KErrNone)
            {           
            SetTestStepResult(EFail);
            _LIT(KRes,"TestGetCityGroupsInfoStep: GetCityGroupL (descending) Left");
		    ERR_PRINTF1(KRes);
            }
        
        if(iDataSource == 1) 
        	{//iDataSource ==1 meants testing only default data sorce since GetCityGroupL doesn't apply to user data.
	        TRAPD(res2, GetAllCitiesInGroupL(iLocalizer));  
	        if (res2 != KErrNone)
	            {           
	            SetTestStepResult(EFail);
	            _LIT(KRes2,"TestGetCityGroupsInfoStep: GetCitiesInGroupL left");
			    ERR_PRINTF1(KRes2);
	            }   
	        }
	    }
	else
	    {
    	SetTestStepResult(EFail);	
    	_LIT(KNewLeft,"TestGetCityGroupsInfoStep: Problem with NewL"); 	 
		ERR_PRINTF1(KNewLeft);
        }
	
	//the following doesn't need when Shazia submits her code
	CleanupStack::PopAndDestroy(2);
	       
    __UHEAP_MARKEND;    
    return TestStepResult();
    
	}
	
TVerdict CTestGetCityGroupInfoStep::doTestStepPostambleL()
/**
 * @return - TVerdict code
 * Override of base class virtual
 */
	{
	ResetTzUserDataL();
	return TestStepResult();
	}

/* Get all groups 
 @param aLocalizer A pointer to CTzLocalizer class 
 @param aOrder A Sorting Order
 */
void CTestGetCityGroupInfoStep::GetAllGroupsL(CTzLocalizer* aLocalizer, CTzLocalizer::TTzSortOrder aOrder)
    {    
	CTzLocalizedCityGroupArray* allGroups = NULL;	
	TRAPD(err, allGroups = aLocalizer->GetAllCityGroupsL(aOrder));
	CleanupStack::PushL(allGroups);
    User::LeaveIfError(err);
    	
    TInt numGroups = allGroups->Count();    
    for(TInt i = 0; i<numGroups; i++)
        {
        //The variable "index" specifies the order of city group names in the section [CitiesInUkGroup] and [CitiesInUkGroup]
        //in tzlocaltestdata.ini. When the soring order is descendent, the element in array variable "allGroups" should have reversed order
        //as those specified in ini file. For example, in section [CitiesInUkGroup], the last group (GroupName129) is Washington ("index" = 129 results in groupNameTag is GroupName129) which  
        //should be identical to the first element in "allGroups" (tested in statement "if (allGroups->At(i).Name() != groupNameFromIni")
        TInt index = i+1;
        if (aOrder == CTzLocalizer::ETzAlphaNameDescending)
          	{
          	index = numGroups-i;
          	}
   
	    TBuf<KTagLength> groupNameTag;
	    groupNameTag = KGroupNameTag;
	    TBuf<KTagLength> groupIdTag;
	    groupIdTag = KCityGroupIdIdTag;
	    TInt groupIdFromIni;
        TPtrC groupNameFromIni;
        
	    groupNameTag.AppendNum(index);
	    groupIdTag.AppendNum(index);
	    	    
	    TBool groupNameHasFound = GetStringFromConfig(ConfigSection(), groupNameTag, groupNameFromIni);
	    GetIntFromConfig(ConfigSection(), groupIdTag, groupIdFromIni);
		    		   	
	    _LIT(KGroupNoMatch,"TestGetCityGroupsInfoStep: Group Name didnt match");
	    if(!groupNameHasFound && allGroups->At(i).Name()!=KNullDesC)
     		{
     		//Whe the city name is not found in the ini file, it implies
     		// the city group name is not defined. Those non-named city groups
     		//should be in the front of the first named city group.
     	    ERR_PRINTF1(KGroupNoMatch);
     	    User::Leave(KErrNotFound);	
     		}
     	else if (groupNameHasFound)
     		{
	     	if (allGroups->At(i).Name() != groupNameFromIni)
		        {
	            ERR_PRINTF1(KGroupNoMatch);
	            User::Leave(KErrNotFound);	
	            }				
		          
		    if (allGroups->At(i).Id() != groupIdFromIni)
		        { 
		        _LIT(KGroupIdNoMatch,"TestGetCityGroupsInfoStep: Group Id didnt match");
		        ERR_PRINTF1(KGroupIdNoMatch);
		        User::Leave(KErrNotFound);
			    }
	     	}
	    }
	                
	CleanupStack::PopAndDestroy(allGroups);             
    }
    
void CTestGetCityGroupInfoStep::GetAllCitiesInGroupL(CTzLocalizer* aLocalizer)
    {
    CTzLocalizer::TTzSortOrder aSortOrder;
    aSortOrder = CTzLocalizer::ETzAlphaNameAscending;
	CTzLocalizedCityGroup* cityGroup = NULL;
	CTzLocalizedCityArray* cities = NULL;
	
    TRAPD(err, cityGroup = aLocalizer->GetCityGroupL(KDefaultCityGroupId));
    CleanupStack::PushL(cityGroup);
    User::LeaveIfError(err);
       
    TRAPD(err1, cities = aLocalizer->GetCitiesInGroupL(*cityGroup,aSortOrder));
    CleanupStack::PushL(cities);
    User::LeaveIfError(err1);
    
    TInt numCities = cities->Count();
    
    for(TInt i=0; i<numCities; i++)
        {
        TBuf<KTagLength> cityTag;
        cityTag = KCityTag;
        TPtrC cityNameFromIni;
    	cityTag.AppendNum(i+1);
    	
    	GetStringFromConfig(ConfigSection(),cityTag,cityNameFromIni);
    	
    	if(cities->At(i).Name() != cityNameFromIni)
    	    {
    	    _LIT(KNameNoMatch,"GetCitiesInGroupL: Name didnt match");
    		ERR_PRINTF1(KNameNoMatch);
    		User::Leave(KErrNotFound);
    	    }
        }
    
    CleanupStack::PopAndDestroy(cities);
    CleanupStack::PopAndDestroy(cityGroup);    
    
    }
        	    
    

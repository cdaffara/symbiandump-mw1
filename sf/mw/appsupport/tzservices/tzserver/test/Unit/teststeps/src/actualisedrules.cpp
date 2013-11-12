// Copyright (c) 2007-2009 Nokia Corporation and/or its subsidiary(-ies).
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

#include "actualisedrules.h"
#include "vtzrules.h"


CActualisedRules::~CActualisedRules()
	{
	iRules.Close();
	}

CActualisedRules::CActualisedRules()
 : 	iRules(KVTzRulesGranularity)
	{
	SetTestStepName(KActualisedRules);
	}

TVerdict CActualisedRules::doTestStepPreambleL()
	{
	TVerdict verdict(ETestSuiteError);	
	
	_LIT(KTagStartYear,	        "StartYear");           // TInt
	_LIT(KTagEndYear,           "EndYear");             // TInt

	// Read parameters
	if (!GetIntFromConfig(ConfigSection(), KTagStartYear, iStartYear))	
	    {
        // cannot retrieve the start year
        ERR_PRINTF2(_L("[%S] failed.  Unable to retrieve start year to setup Actualised rules."), 
                    &ConfigSection());        
        
        return ETestSuiteError;                                       	                	    
	    }
	    
	if (!GetIntFromConfig(ConfigSection(), KTagEndYear, iEndYear))
	    {
        // cannot retrieve the end year
        ERR_PRINTF2(_L("[%S] failed.  Unable to retrieve end year to setup Actualised rules."), 
                    &ConfigSection());        
        
        return ETestSuiteError;                                       	                	    	        	        
	    }
	
    verdict = ReadActualisedRulesL();
    if (verdict != EPass)
        {
        return verdict;                        
        }        
   
    verdict = ReadOffsetTestSettings();
    if (verdict != EPass)
        {
        return verdict;                        
        }        
    
    verdict = ReadDSTTestSettings();
    if (verdict != EPass)
        {
        return verdict;                        
        }        
    
	return EPass;
	}

TVerdict CActualisedRules::doTestStepL()
	{
	TVerdict verdict = EPass;
	
    SetTestStepResult(EFail);	
	
	CVTzActualisedRules* rules = CVTzActualisedRules::NewL(iStartYear, iEndYear);
	CleanupStack::PushL(rules);
	
	ASSERT(rules->Count() == 0);
	ASSERT(iStartYear == rules->StartYear());
	ASSERT(iEndYear == rules->EndYear());
	
	// add all the rules retrieved from the config file
	for (TInt i = 0; i < iRules.Count(); i++)
	    {
	    const TVTzActualisedRule& rule = iRules[i];	     
	    rules->AddRuleL(rule);
	    }
		    
    // perform offset check
    if (iOffsetTest)
        {            
        TInt offsetFromRule = rules->GetOffsetFromRuleL(iOffsetCheckTime, iOffsetCheckRef);
        if (offsetFromRule != iOffsetCheckResult)
            {
            ERR_PRINTF4(_L("[%S] failed.  Offset test failed.  Expected result=%d, actual result=%d."), 
                        &ConfigSection(), iOffsetCheckResult, offsetFromRule);        
            
            verdict = EFail;
            }
        }

    // perform DST check
    if (iDSTTest)
        {            
        TBool dst = rules->IsDaylightSavingOn(iDSTCheckTime);
        if (dst != iDSTCheckResult)
            {
        	_LIT(KTrue,    "True");  
        	_LIT(KFalse,   "False"); 
            
            ERR_PRINTF4(_L("[%S] failed.  DST test failed.  Expected result=%S, actual result=%S."), 
                        &ConfigSection(), 
                        iDSTCheckResult ? &KTrue() : &KFalse(), 
                        dst ? &KTrue() : &KFalse());        
            
            verdict = EFail;                        
            }
        }

    // clean up
    CleanupStack::PopAndDestroy(rules);		

    // check if at least one test ran
    if (!iOffsetTest && !iDSTTest)
        {
        ERR_PRINTF2(_L("[%S] failed.  Neither DST nor Offset test has enough information to run."), 
                    &ConfigSection());                                        
        SetTestStepResult(ETestSuiteError);
        }        
    else
        {
        SetTestStepResult(verdict);	    
        }		
		
	return TestStepResult();
	}	
	
TVerdict CActualisedRules::doTestStepPostambleL()
	{
	iRules.Reset();	
	return TestStepResult();
	}
	
TVerdict CActualisedRules::ReadActualisedRulesL()	
    {
	_LIT(KTagRuleCount,	        "NumberOfRules");	    // TInt
	_LIT(KTagRuleTime,	        "Rule%dTime");          // TDesC
	_LIT(KTagRuleNewOffset,	    "Rule%dNewOffset");     // TInt
    _LIT(KTagRuleRef,	        "Rule%dRef");	        // TInt 
    
	TInt num(0);
	if (!GetIntFromConfig(ConfigSection(), KTagRuleCount, num))
	    {
        ERR_PRINTF3(_L("[%S] failed.  Missing attribute <%S>."), 
                    &ConfigSection(), &KTagRuleCount());                                        	    
        return ETestSuiteError;	        	        
	    }
    else if (num == 0)
        {
        return EPass;            
        }
	
    const TInt indexLength = 10;
    RBuf tagName;
    TPtrC strPtr(KNullDesC);
    CleanupClosePushL(tagName);
	
	for (TInt i = 1; i <= num; i++)
	    {
	    tagName.CreateL(KTagRuleTime().Length() + indexLength);
	    tagName.AppendFormat(KTagRuleTime, i);
	    
	    if (GetStringFromConfig(ConfigSection(), tagName, strPtr))
	        {	           
            TTime time(0);	    
	        if (strPtr.Length() > 0)
	            {
	            time.Set(strPtr);	            
	            }
            else
                {                    
                time.HomeTime();
                }
                
            TInt offset = 0;
            TInt ref = 0;                
            
            // retrieve offset for the current rule
            tagName.Close();
    	    tagName.CreateL(KTagRuleNewOffset().Length() + indexLength);
    	    tagName.AppendFormat(KTagRuleNewOffset, i);    	    
	        GetIntFromConfig(ConfigSection(), tagName, offset);
    	    
            // retrieve ref for the current rule
            tagName.Close();
    	    tagName.CreateL(KTagRuleRef().Length() + indexLength);
    	    tagName.AppendFormat(KTagRuleRef, i);    	    
	        GetIntFromConfig(ConfigSection(), tagName, ref);
    	    
            TVTzActualisedRule rule(time, offset, static_cast<TTzTimeReference>(ref));
    	    iRules.Append(rule);	                        
	        }
	    else
	        {
    	    TVTzActualisedRule rule;
    	    iRules.Append(rule);	        
	        }
	        
        tagName.Close();	        
	    }
	    	    
    CleanupStack::PopAndDestroy(&tagName);	       
    
    return EPass;
    }
	
TVerdict CActualisedRules::ReadOffsetTestSettings()	
    {
	_LIT(KTagOffsetCheckTime,   "OffsetCheckTime");     // TDesC	
	_LIT(KTagOffsetCheckRef,    "OffsetCheckRef");      // TInt
	_LIT(KTagOffsetCheckResult, "OffsetCheckResult");   // TBool
    
    TInt num(0);
	TBool ref = GetIntFromConfig(ConfigSection(), KTagOffsetCheckRef, num);
	if (ref)
	    {	        
	    iOffsetCheckRef = static_cast<TTzTimeReference> (num);        	    	    
	    }
               
    TPtrC strPtr(KNullDesC);
	TBool time = GetStringFromConfig(ConfigSection(), KTagOffsetCheckTime, strPtr);
    if (time)
	    {	        
    	if (strPtr.Length() > 0)
    	    {    
            iOffsetCheckTime.Set(strPtr);
    	    }
        else
            {
            // cannot retrieve the time zone reference to check for offset 
            ERR_PRINTF2(_L("[%S] failed.  Missing time for the Offset test."), 
                        &ConfigSection());        
            
            return ETestSuiteError;                                       
            }
	    }
	    
    TBool result = GetIntFromConfig(ConfigSection(), KTagOffsetCheckResult, iOffsetCheckResult);
    
    if (ref && time && result)      // if all attributes are provided with proper values
        {
        iOffsetTest = ETrue;
        }
    else if (ref || time || result) // if some of the attributes exists with empty values
        {
        // cannot retrieve the time zone reference to check for offset 
        ERR_PRINTF2(_L("[%S] failed.  Incomplete info to complete the Offset test."), 
                    &ConfigSection());        
        
        return ETestSuiteError;                                                   
        }
        
    return EPass;        
    }
	
	
TVerdict CActualisedRules::ReadDSTTestSettings()
    {
	_LIT(KTagDSTCheckTime,      "DSTCheckTime");        // TDesC
	_LIT(KTagDSTCheckResult,    "DSTCheckResult");      // TBool
    
    TPtrC strPtr(KNullDesC);
	TBool time = GetStringFromConfig(ConfigSection(), KTagDSTCheckTime, strPtr);		
	if (time)
	    {	        
    	if (strPtr.Length() > 0)
    	    {    
            iDSTCheckTime.Set(strPtr);
    	    }
        else
            {
            // cannot retrieve the expected result of the offset check
            ERR_PRINTF2(_L("[%S] failed.  Unable to retrieve entry \"DSTCheckTime\"."), 
                        &ConfigSection());        
            
            return ETestSuiteError;            
            }
	    }
        
    TBool result = GetBoolFromConfig(ConfigSection(), KTagDSTCheckResult, iDSTCheckResult);

    if (result && time)     // if all attributes are provided with proper values
        {
        iDSTTest = ETrue;                        
        }
    else if (result || time) // if some of the attributes exists with empty values
        {                    
        // cannot retrieve the expected result of the offset check
        ERR_PRINTF2(_L("[%S] failed.  Incomplete info to complete the DST test."), 
                    &ConfigSection());        
        
        return ETestSuiteError;                                       	                
        }     
            
    return EPass;        
    }

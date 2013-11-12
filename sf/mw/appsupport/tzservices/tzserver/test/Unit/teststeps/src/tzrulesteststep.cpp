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

#include <s32file.h>

#include "tzrulesteststep.h"
#include "vtzrules.h"

const TInt KMaxIndexLength = 10;

_LIT(KDateFormatString, "%3-%2-%1 %H:%T:%S");
_LIT(KTestFileName, "C:\\Logs\\test");

CTzRulesTestStep::~CTzRulesTestStep()
    {
    iRules.Close();
    }

CTzRulesTestStep::CTzRulesTestStep() :
    iNewStartYear(-1),
    iNewEndYear(-1),
    iRules(KVTzRulesGranularity)
    {
    SetTestStepName(KTzRules);
    }

TVerdict CTzRulesTestStep::doTestStepPreambleL()
    {
    TDesC& name=ConfigSection();
    
    SetTestStepResult(EFail);

    _LIT(KTagOrgStartYear,  "OrgStartYear");           // TInt
    _LIT(KTagOrgEndYear,    "OrgEndYear");             // TInt
    _LIT(KTagNewStartYear,  "NewStartYear");           // TInt
    _LIT(KTagNewEndYear,    "NewEndYear");             // TInt
    _LIT(KTagInitStdOffset, "InitStdOffset");          // TInt
    _LIT(KTagRuleAccess,    "RuleAccess");             // TInt

    _LIT(KTagTestClone,     "TestClone");               // TBool
    _LIT(KTagTestReadWrite, "TestReadWrite");           // TBool

    // Read parameters
    if (!GetIntFromConfig(ConfigSection(), KTagOrgStartYear, iOrgStartYear))
        {
        ERR_PRINTF2(_L("[%S] failed.  Insufficient information to setup test."),
                    &ConfigSection());

        return ETestSuiteError;
        }

    if (!GetIntFromConfig(ConfigSection(), KTagOrgEndYear, iOrgEndYear))
        {
        ERR_PRINTF2(_L("[%S] failed.  Insufficient information to setup test."),
                    &ConfigSection());

        return ETestSuiteError;
        }

    GetIntFromConfig(ConfigSection(), KTagNewStartYear, iNewStartYear);
    GetIntFromConfig(ConfigSection(), KTagNewEndYear, iNewEndYear);
    GetIntFromConfig(ConfigSection(), KTagInitStdOffset, iInitStdOffset);

    GetBoolFromConfig(ConfigSection(), KTagTestClone, iTestClone);
    GetBoolFromConfig(ConfigSection(), KTagTestReadWrite, iTestReadWrite);

    iTestRuleAccess = GetIntFromConfig(ConfigSection(), KTagRuleAccess, iRuleAccessIndex);
    // rule index is zero base
    iRuleAccessIndex--;

    ReadConversionTestSetup();

    TVerdict verdict = ReadTzRulesL();

    return verdict;
    }

TVerdict CTzRulesTestStep::doTestStepL()
    {
    TBool testPass = ETrue;

    SetTestStepResult(EFail);

    CTzRules* rules = CTzRules::NewL(iOrgStartYear, iOrgEndYear);
    CleanupStack::PushL(rules);

    __ASSERT_ALWAYS(rules->Count() == 0, User::Panic(KTzRules, KErrCorrupt));
    __ASSERT_ALWAYS(iOrgStartYear == rules->StartYear(), User::Panic(KTzRules, KErrCorrupt));
    __ASSERT_ALWAYS(iOrgEndYear == rules->EndYear(), User::Panic(KTzRules, KErrCorrupt));
    __ASSERT_ALWAYS(rules->InitialStdTimeOffset() == 0, User::Panic(KTzRules, KErrCorrupt));

    if (iNewStartYear >= 0)
        {
        rules->SetStartYear(iNewStartYear);
        __ASSERT_ALWAYS(iNewStartYear == rules->StartYear(), User::Panic(KTzRules, KErrCorrupt));
        }
    if (iNewEndYear >= 0)
        {
        rules->SetEndYear(iNewEndYear);
        __ASSERT_ALWAYS(iNewEndYear == rules->EndYear(), User::Panic(KTzRules, KErrCorrupt));
        }
    if (iInitStdOffset != 0)
        {
        rules->SetInitialStdTimeOffset(iInitStdOffset);
        __ASSERT_ALWAYS(iInitStdOffset == rules->InitialStdTimeOffset(),
                        User::Panic(KTzRules, KErrCorrupt));
        }

    // add all the rules retrieved from the config file
    for (TInt i = 0; i < iRules.Count(); i++)
        {
        const TTzRule& rule = iRules[i];
        rules->AddRuleL(rule);
        }

    // perform rule access
    if (iTestRuleAccess)
        {
        // candidate for PANIC test
        TTzRule& rule = (*rules)[iRuleAccessIndex];
        }

    // perform clone test
    if (iTestClone)
        {
        CTzRules* clone = rules->CloneL();
        if (!clone->IsEqualTo(*rules) || !rules->IsEqualTo(*clone))
            {
            // cloning failed!
            ERR_PRINTF2(_L("[%S] failed.  Clone test failed."), &ConfigSection());

            testPass &= EFalse;
            }

        delete clone;
        }

    // perform Externalise/Internalise test
    if (iTestReadWrite)
        {
        testPass &= doTestReadWriteL(*rules);
        }

    // Perform time conversion test last.    
    // Due to the "missing hour" (DST kicks eg. 1:59:59 -> 3:00:00, where 2:00 - 2:59 is missing), or 
    // the "double local time" (DST ends eg. 1:59:59 -> 1:00:00 where 1:00 - 1:59 happens twice),
    // Local -> UTC -> Local conversion will cause asymmetric conversion to happen.  If so, use the leave
    // code KErrExtensionNotSupported(-47) to indicate that.
    // However, all the rest of the case should not fail, thus scheduling the conversion test to be
    // ran last.
    // 
    // Note: Rule of thumb for dealing w/ the "Missing Hour" or "Double Local Time"
    // 
    // Missing Hour:
    // Use the rule before DST kicks in.
    // 
    // Double Local:
    // Use the DST rule
    // 
    // eg. 12:59:59am UK local -> 12:59:59 UTC 
    //     02:00:00am UK local -> 01:00:00 UTC = DST starts
    //
    //     Missing Hour: 01:15am local -> 01:15am UTC
    //
    //     01:59:59am UK local -> 12:59:59 UTC
    //     01:00:00am UK local -> 01:00:00 UTC = DST ends
    //
    //     Double Local: 01:15am local -> 12:15am UTC    
    if (iTestConvert)
        {
        TBool asymmetricOccurred = EFalse;
        testPass &= doTestConvertL(*rules, asymmetricOccurred);
        if (asymmetricOccurred)
            {
            User::LeaveIfError(KErrExtensionNotSupported);                                
            }
        }

    if (testPass)
        {
        SetTestStepResult(EPass);
        }

    // clean up
    CleanupStack::PopAndDestroy(rules);

    return TestStepResult();
    }

TBool CTzRulesTestStep::doTestReadWriteL(const CTzRules& aRules)
    {
    TBool pass = ETrue;

    RFs fs;
    CleanupClosePushL(fs);
    User::LeaveIfError(fs.Connect());

    RFileWriteStream fileWriteStream;
    CleanupClosePushL(fileWriteStream);
    User::LeaveIfError(fileWriteStream.Replace(fs, KTestFileName, EFileStream | EFileWrite));

    aRules.ExternalizeL(fileWriteStream);
    CleanupStack::PopAndDestroy(&fileWriteStream);

    RFileReadStream fileReadStream;
    CleanupClosePushL(fileReadStream);
    User::LeaveIfError(fileReadStream.Open(fs, KTestFileName, EFileStream | EFileRead));

    CTzRules* clone = CTzRules::NewL();
    CleanupStack::PushL(clone);

    clone->InternalizeL(fileReadStream);

    // if after writing out the TzRules and then reading them back, it is not the same,
    // then something about the read/write process is wrong
    if (!clone->IsEqualTo(aRules) || !aRules.IsEqualTo(*clone))
        {
        // read/write failed!
        ERR_PRINTF2(_L("[%S] failed.  Read/Write test failed."), &ConfigSection());

        pass = EFalse;
        }

    CleanupStack::PopAndDestroy(clone);
    CleanupStack::PopAndDestroy(&fileReadStream);
    CleanupStack::PopAndDestroy(&fs);

    return pass;
    }

TBool CTzRulesTestStep::doTestConvertL(const CTzRules& aTzRules, 
                                       TBool& aAsymmetricOccurred)
    {
    aAsymmetricOccurred = EFalse;
    
    // Assume the time passed in is Local time and check if the user has
    // provided an expected UTC time after conversion for comparison.
    // Then convert the UTC back to local time and ensure they are the same.
    // 
    // Note:
    // Due to the "missing hour" (DST kicks eg. 1:59:59 -> 3:00:00, where 2:00 - 2:59 is missing), 
    // Local -> UTC -> Local conversion may cause asymmetric conversion to happen.      
    //
    TBool aAsymConvert1 = EFalse;   // ConvertToUtcL -> ConvertToLocalL -> ConvertToUtcL
    TBool aAsymConvert2 = EFalse;   // ConvertTime -> ConvertTime -> ConvertTime
    TBool pass = doTestUtcConversionL(aTzRules, aAsymConvert1, aAsymConvert2);
    if (pass && aAsymConvert1 && aAsymConvert2)
        {
        // if the basic conversion is correct and yet asymmetric conversion happens
        aAsymmetricOccurred = ETrue;
        }

    // Assume the time passed in is UTC time and check if the user has
    // provided an expected local time after conversion for comparison.
    // Then convert the local time back to UTC time and ensure they are the same.
    // 
    // Note:
    // Due to the "double local time" (DST ends eg. 1:59:59 -> 1:00:00 where 1:00 - 1:59 happens 
    // twice), UTC -> Local -> UTC conversion may cause asymmetric conversion to happen.          
    //
    aAsymConvert1 = EFalse;   // ConvertToLocalL -> ConvertToUtcL -> ConvertToLocalL
    aAsymConvert2 = EFalse;   // ConvertTime -> ConvertTime -> ConvertTime    
    pass &= doTestLocalConversionL(aTzRules, aAsymConvert1, aAsymConvert2);
    if (pass && aAsymConvert1 && aAsymConvert2)
        {
        // if the basic conversion is correct and yet asymmetric conversion happens
        aAsymmetricOccurred = ETrue;
        }
        
    return pass;
    }

TBool CTzRulesTestStep::doTestLocalConversionL(const CTzRules& aTzRules,
                                               TBool& aAsymmetricConvert1,
                                               TBool& aAsymmetricConvert2)
    {
    TBool pass = ETrue;
    TTime testTime(iConvertTestTime.Int64());   // make a copy of the time to be converted
                                                // and assume it's UTC time

    // 1. Convert from UTC to Local
    aTzRules.ConvertToLocalL(testTime);
    if (iTestConvertLocal)
        {
        if (iExpectedLocalTime != testTime)
            {
            const TInt KMaxDateString = 50;

            TBuf<KMaxDateString> expected;
            TBuf<KMaxDateString> actual;

            iExpectedLocalTime.FormatL(expected, KDateFormatString);
            testTime.FormatL(actual, KDateFormatString);

            // ConvertTime failed!
            ERR_PRINTF4(_L("[%S] failed.  CTzRules::ConvertToLocalL() failed.  Expected Result: %S, Actual Result: %S"),
                        &ConfigSection(), &expected, &actual);

            pass = EFalse;
            }
        }

    // 2. Convert from local back to utc
    aTzRules.ConvertToUtcL(testTime);

    // UTC -> Local -> UTC should return the same time as the original input time
    if (testTime != iConvertTestTime)
        {
        // ConvertTime failed!
        ERR_PRINTF2(_L("[%S] failed.  CTzRules::ConvertToLocalL() -> ConvertToUtcL() does not return the original value."),
                    &ConfigSection());

        if (testTime < iConvertTestTime)
            {
            // If after double conversion (UTC -> local -> UTC), the time returned                             
            // is larger than the original time, it's possible that it's due to the 
            // double local translation.
            // 
            // Do not fail the test, but set flag signaling asymmetrical conversion happened.
            aAsymmetricConvert1 = ETrue;            
            }
        else
            {
            // if not, then it's caused by some other problem
            pass = EFalse;                
            }
        }

    // retrieve the collection of Actualised rules to be used for the other conversion
    // method
    testTime = iConvertTestTime;      // reset the copy again, assuming UTC time again
    TDateTime testDateTime = iConvertTestTime.DateTime();

    CVTzActualisedRules* actualisedRules
        = CVTzActualisedRules::NewL(testDateTime.Year() - 1, testDateTime.Year() + 1);
    CleanupStack::PushL(actualisedRules);

    aTzRules.GetActualisedRulesL(*actualisedRules);

    // 3. Convert from UTC back to local
    TInt err = aTzRules.ConvertTime(*actualisedRules, testTime, ETzUtcTimeReference);
    if (err != KErrNone)
        {
        // ConvertTime failed!
        ERR_PRINTF3(_L("[%S] failed.  CTzRules::ConvertTime() failed with error=%d"),
                    &ConfigSection(), err);

        pass = EFalse;
        }

    if (iTestConvertLocal)
        {
        if (iExpectedLocalTime != testTime)
            {
            const TInt KMaxDateString = 50;

            TBuf<KMaxDateString> expected;
            TBuf<KMaxDateString> actual;

            iExpectedLocalTime.FormatL(expected, KDateFormatString);
            testTime.FormatL(actual, KDateFormatString);

            // ConvertTime failed!
            ERR_PRINTF4(_L("[%S] failed.  CTzRules::ConvertTime(UTC->Local) failed.  Expected Result: %S, Actual Result: %S"),
                        &ConfigSection(), &expected, &actual);

            pass = EFalse;
            }
        }

    // 4. Convert from Local to UTC
    err = aTzRules.ConvertTime(*actualisedRules, testTime, ETzWallTimeReference);
    if (err != KErrNone)
        {
        // ConvertTime failed!
        ERR_PRINTF3(_L("[%S] failed.  CTzRules::ConvertTime() failed with error=%d"),
                    &ConfigSection(), err);
        pass = EFalse;                
        }

    // UTC -> Local -> UTC should return the same time as the original input time
    if (testTime != iConvertTestTime)
        {
        // ConvertTime failed!
        ERR_PRINTF2(_L("[%S] failed.  CTzRules::ConvertTime(UTC->Local) -> ConvertTime(Local->UTC) failed to return the original value."),
                    &ConfigSection());

        if (testTime < iConvertTestTime)
            {
            // If after double conversion (UTC -> local -> UTC), the time returned                             
            // is larger than the original time, it's possible that it's due to the 
            // double local translation.
            // 
            // Do not fail the test, but set flag signaling asymmetrical conversion happened.
            aAsymmetricConvert2 = ETrue;            
            }
        else
            {
            // if not, then it's caused by some other problem
            pass = EFalse;                
            }
        }

    CleanupStack::PopAndDestroy(actualisedRules);

    return pass;
    }


TBool CTzRulesTestStep::doTestUtcConversionL(const CTzRules& aTzRules,
                                             TBool& aAsymmetricConvert1,
                                             TBool& aAsymmetricConvert2)
    {
    TBool pass = ETrue;
    TTime testTime(iConvertTestTime.Int64());  // make a copy of the time to be converted
                                               // and assume it's local time

    // 1. Convert from Local to UTC
    aTzRules.ConvertToUtcL(testTime);
    if (iTestConvertUTC)
        {
        if (iExpectedUTCTime != testTime)
            {
            const TInt KMaxDateString = 50;

            TBuf<KMaxDateString> expected;
            TBuf<KMaxDateString> actual;

            iExpectedUTCTime.FormatL(expected, KDateFormatString);
            testTime.FormatL(actual, KDateFormatString);

            // ConvertTime failed!
            ERR_PRINTF4(_L("[%S] failed.  CTzRules::ConvertToUtcL() failed.  Expected Result: %S, Actual Result: %S"),
                        &ConfigSection(), &expected, &actual);

            pass = EFalse;
            }
        }

    // 2. Convert from UTC back to local
    aTzRules.ConvertToLocalL(testTime);

    // Local -> UTC -> Local should return the same time as the original input time
    if (testTime != iConvertTestTime)
        {
        // ConvertTime failed!
        ERR_PRINTF2(_L("[%S] failed.  CTzRules::ConvertToUtcL() -> CTzRules::ConvertToLocalL() does not return the original time."),
                    &ConfigSection());

        if (testTime > iConvertTestTime)
            {
            // If after double conversion (local -> UTC -> local), the time returned                             
            // is larger than the original time, it's possible that it's due to the 
            // missing hour translation.
            // 
            // Do not fail the test, but set flag signaling asymmetrical conversion happened.
            aAsymmetricConvert1 = ETrue;            
            }
        else
            {
            // if not, then it's caused by some other problem
            pass = EFalse;                
            }
        }


    // retrieve the collection of Actualised rules to be used for the other conversion
    // method
    testTime = iConvertTestTime;      // reset the copy again, assuming local time again
    TDateTime testDateTime = iConvertTestTime.DateTime();

    CVTzActualisedRules* actualisedRules
        = CVTzActualisedRules::NewL(testDateTime.Year() - 1, testDateTime.Year() + 1);
    CleanupStack::PushL(actualisedRules);

    aTzRules.GetActualisedRulesL(*actualisedRules);

    // 3. Convert from local to UTC
    TInt err = aTzRules.ConvertTime(*actualisedRules, testTime, ETzWallTimeReference);
    if (err != KErrNone)
        {
        // ConvertTime failed!
        ERR_PRINTF3(_L("[%S] failed.  CTzRules::ConvertTime() failed with error=%d"),
                    &ConfigSection(), err);

        pass = EFalse;
        }

    if (iTestConvertUTC)
        {
        if (iExpectedUTCTime != testTime)
            {
            const TInt KMaxDateString = 50;

            TBuf<KMaxDateString> expected;
            TBuf<KMaxDateString> actual;

            iExpectedUTCTime.FormatL(expected, KDateFormatString);
            testTime.FormatL(actual, KDateFormatString);

            // ConvertTime failed!
            ERR_PRINTF4(_L("[%S] failed.  CTzRules::ConvertTime(Local->UTC) failed.  Expected Result: %S, Actual Result: %S"),
                        &ConfigSection(), &expected, &actual);

            pass = EFalse;
            }
        }

    // 4. Convert from UTC back to Local
    err = aTzRules.ConvertTime(*actualisedRules, testTime, ETzUtcTimeReference);
    if (err != KErrNone)
        {
        // ConvertTime failed!
        ERR_PRINTF3(_L("[%S] failed.  CTzRules::ConvertTime() failed with error=%d"),
                    &ConfigSection(), err);

        pass = EFalse;
        }

    // Local -> UTC -> Local should return the same time as the original input time
    if (testTime != iConvertTestTime)
        {
        // ConvertTime failed!
        ERR_PRINTF2(_L("[%S] failed.  CTzRules::ConvertTime(local -> UTC) -> CTzRules::ConvertTime(UTC -> local) failed to return the original time."),
                    &ConfigSection());

        if (testTime > iConvertTestTime)
            {
            // If after double conversion (local -> UTC -> local), the time returned                             
            // is larger than the original time, it's possible that it's due to the 
            // missing hour translation.
            // 
            // Do not fail the test, but set flag signaling asymmetrical conversion happened.
            aAsymmetricConvert2 = ETrue;            
            }
        else
            {
            // if not, then it's caused by some other problem
            pass = EFalse;                
            }
        }

    CleanupStack::PopAndDestroy(actualisedRules);

    return pass;
    }

TVerdict CTzRulesTestStep::doTestStepPostambleL()
    {
    // cleanup of the array
    iRules.Reset();

    // remove test file, if any, used during the test
    RFs fs;
    User::LeaveIfError(fs.Connect());
    fs.Delete(KTestFileName);
    fs.Close();

    return TestStepResult();
    }

void CTzRulesTestStep::ReadConversionTestSetup()
    {
    _LIT(KTagConvertTestTime,   "ConvertTestTime");     // TDesC
    _LIT(KTagUTCTestTime,       "ExpectedUTCTime");     // TDesC
    _LIT(KTagLocalTestTime,     "ExpectedLocalTime");   // TDesC

    TPtrC strPtr;
    iTestConvert = GetStringFromConfig(ConfigSection(), KTagConvertTestTime, strPtr);
    if (iTestConvert && strPtr.Length() > 0)
        {
        iConvertTestTime.Set(strPtr);

        // check if user wants to test the value after converting the given time to
        // UTC time
        iTestConvertUTC = GetStringFromConfig(ConfigSection(), KTagUTCTestTime, strPtr);
        if (iTestConvertUTC && strPtr.Length() > 0)
            {
            iExpectedUTCTime.Set(strPtr);
            }
        else
            {
            iTestConvertUTC = EFalse;
            }

        // check if user wants to test the value after converting the given time to
        // local time
        iTestConvertLocal = GetStringFromConfig(ConfigSection(), KTagLocalTestTime, strPtr);
        if (iTestConvertLocal && strPtr.Length() > 0)
            {
            iExpectedLocalTime.Set(strPtr);
            }
        else
            {
            iTestConvertLocal = EFalse;
            }
        }
    else
        {
        iTestConvert = EFalse;
        }
    }

TVerdict CTzRulesTestStep::ReadTzRulesL()
    {
    _LIT(KTagRuleCount,                "NumberOfRules");       // TInt

    TInt numRules(0);
    if (!GetIntFromConfig(ConfigSection(), KTagRuleCount, numRules))
        {
        ERR_PRINTF3(_L("[%S] failed.  Missing attribute <%S>."),
                    &ConfigSection(), &KTagRuleCount());
        return ETestSuiteError;
        }
    else if (numRules == 0)
        {
        // no rules test
        return EPass;
        }

    for (TInt i = 1; i <= numRules; i++)
        {
        if (ReadAndAddRuleL(i) != EPass)
            {
            return ETestSuiteError;
            }
        }

    return EPass;
    }

TVerdict CTzRulesTestStep::ReadAndAddRuleL(TInt aIndex)
    {
    _LIT(KTagRuleSectionName,       "Rule%dName");    // TDesC
    _LIT(KTagRuleUseDefault,        "UseDefault");    // TBool
    _LIT(KTagRuleFromYrInt,         "FromYrInt");     // TInt
    _LIT(KTagRuleFromYrTime,        "FromYrTime");    // TDesC
    _LIT(KTagRuleFromYrTimeRef,     "FromYrTimeRef"); // TInt
    _LIT(KTagRuleToYrInt,           "ToYrInt");       // TInt
    _LIT(KTagRuleToYrTime,          "ToYrTime");      // TDesC
    _LIT(KTagRuleToYrTimeRef,       "ToYrTimeRef");   // TInt
    _LIT(KTagRuleOldOffset,         "OldOffset");     // TInt
    _LIT(KTagRuleNewOffset,         "NewOffset");     // TInt
    _LIT(KTagRuleMonth,             "Month");         // TInt
    _LIT(KTagRuleDayRule,           "DayRule");       // TInt
    _LIT(KTagRuleDayOfMonth,        "DayOfMonth");    // TInt
    _LIT(KTagRuleDayOfWeek,         "DayOfWeek");     // TInt
    _LIT(KTagRuleRef,               "TimeRef");       // TInt
    _LIT(KTagRuleTOC,               "TOC");           // TInt - Time Of Change

    // Read the Rule Section to be used for this Rule
    RBuf tagName;
    tagName.CreateL(KTagRuleSectionName().Length() + KMaxIndexLength);
    tagName.AppendFormat(KTagRuleSectionName, aIndex);

    TPtrC ruleSectionName;
    if (!GetStringFromConfig(ConfigSection(), tagName, ruleSectionName) ||
        ruleSectionName.Length() == 0)
        {
        // if no rule is specified
        ERR_PRINTF3(_L("[%S] failed.  Section Name for Rule %d not specified."),
                    &ConfigSection(), aIndex);

        return ETestSuiteError;
        }

    tagName.Close();

    // check if to use default TTzRule
    TBool useDefault(EFalse);
    if (GetBoolFromConfig(ruleSectionName, KTagRuleUseDefault, useDefault))
        {
        if (useDefault)
            {
            TTzRule rule;
            iRules.Append(rule);
            }
        }
    if (useDefault)
        {
        return EPass;
        }

    // check for From Year as TInt first
    TTimeWithReference fromYrTimeRef;
    TInt fromYrInt(-1);

    if (!GetIntFromConfig(ruleSectionName, KTagRuleFromYrInt, fromYrInt))
        {
        // check for From Year as TTimeWithReference
        if (!ReadTimeWithReferenceFromKeys(KTagRuleFromYrTime,
                                           KTagRuleFromYrTimeRef,
                                           ruleSectionName,
                                           fromYrTimeRef))
            {
            // missing sufficient information to from TTzRule
            // if use Rule is specified w/ a non-existent rule
            ERR_PRINTF3(_L("[%S] failed.  Missing information to form Tz rule %d."),
                        &ConfigSection(), aIndex);

            return ETestSuiteError;
            }
        }

    // check for To Year as TInt first
    TTimeWithReference toYrTimeRef;
    TInt toYrInt(-1);
    if (!GetIntFromConfig(ruleSectionName, KTagRuleToYrInt, toYrInt))
        {
        // check for To Year as TTimeWithReference
        if (!ReadTimeWithReferenceFromKeys(KTagRuleToYrTime,
                                           KTagRuleToYrTimeRef,
                                           ruleSectionName,
                                           toYrTimeRef))
            {
            // missing sufficient information to from TTzRule
            // if use Rule is specified w/ a non-existent rule
            ERR_PRINTF3(_L("[%S] failed.  Missing information to form Tz rule %d."),
                        &ConfigSection(), aIndex);

            return ETestSuiteError;
            }
        }

    TInt temp;

    // check for old offset
    TInt16 oldOffset(0);
    if (!GetIntFromConfig(ruleSectionName, KTagRuleOldOffset, temp))
        {
        // missing sufficient information to from TTzRule
        // if use Rule is specified w/ a non-existent rule
        ERR_PRINTF3(_L("[%S] failed.  Missing information to form Tz rule %d."),
                    &ConfigSection(), aIndex);

        return ETestSuiteError;
        }
    else
        {
        oldOffset = static_cast<TInt16> (temp);
        }

    // check for new offset
    TInt16 newOffset(0);
    if (!GetIntFromConfig(ruleSectionName, KTagRuleNewOffset, temp))
        {
        // missing sufficient information to from TTzRule
        // if use Rule is specified w/ a non-existent rule
        ERR_PRINTF3(_L("[%S] failed.  Missing information to form Tz rule %d."),
                    &ConfigSection(), aIndex);

        return ETestSuiteError;
        }
    else
        {
        newOffset = static_cast<TInt16> (temp);
        }

    // check for month
    TMonth month;
    if (!GetIntFromConfig(ruleSectionName, KTagRuleMonth, temp))
        {
        // missing sufficient information to from TTzRule
        // if use Rule is specified w/ a non-existent rule
        ERR_PRINTF3(_L("[%S] failed.  Missing information to form Tz rule %d."),
                    &ConfigSection(), aIndex);

        return ETestSuiteError;
        }
    else
        {
        month = static_cast<TMonth> (temp);
        }

    // check for rule day
    TTzRuleDay ruleDay;
    if (!GetIntFromConfig(ruleSectionName, KTagRuleDayRule, temp))
        {
        // missing sufficient information to from TTzRule
        // if use Rule is specified w/ a non-existent rule
        ERR_PRINTF3(_L("[%S] failed.  Missing information to form Tz rule %d."),
                    &ConfigSection(), aIndex);

        return ETestSuiteError;
        }
    else
        {
        ruleDay = static_cast<TTzRuleDay> (temp);
        }

    TUint8 dayOfMonth;
    if (!GetIntFromConfig(ruleSectionName, KTagRuleDayOfMonth, temp))
        {
        // missing sufficient information to from TTzRule
        // if use Rule is specified w/ a non-existent rule
        ERR_PRINTF3(_L("[%S] failed.  Missing information to form Tz rule %d."),
                    &ConfigSection(), aIndex);

        return ETestSuiteError;
        }
    else
        {
        dayOfMonth = static_cast<TUint8> (temp);
        }

    TUint8 dayOfWeek;
    if (!GetIntFromConfig(ruleSectionName, KTagRuleDayOfWeek, temp))
        {
        // missing sufficient information to from TTzRule
        // if use Rule is specified w/ a non-existent rule
        ERR_PRINTF3(_L("[%S] failed.  Missing information to form Tz rule %d."),
                    &ConfigSection(), aIndex);

        return ETestSuiteError;
        }
    else
        {
        dayOfWeek = static_cast<TUint8> (temp);
        }

    TTzTimeReference timeRef;
    if (!GetIntFromConfig(ruleSectionName, KTagRuleRef, temp))
        {
        // missing sufficient information to from TTzRule
        // if use Rule is specified w/ a non-existent rule
        ERR_PRINTF3(_L("[%S] failed.  Missing information to form Tz rule %d."),
                    &ConfigSection(), aIndex);

        return ETestSuiteError;
        }
    else
        {
        timeRef = static_cast<TTzTimeReference> (temp);
        }

    TUint16 toc;
    if (!GetIntFromConfig(ruleSectionName, KTagRuleTOC, temp))
        {
        // missing sufficient information to from TTzRule
        // if use Rule is specified w/ a non-existent rule
        ERR_PRINTF3(_L("[%S] failed.  Missing information to form Tz rule %d."),
                    &ConfigSection(), aIndex);

        return ETestSuiteError;
        }
    else
        {
        toc = static_cast<TUint16> (temp);
        }

    if (fromYrInt >= 0 && toYrInt >= 0)
        {
        TTzRule rule(fromYrInt,
                     toYrInt,
                     oldOffset,
                     newOffset,
                     month,
                     ruleDay,
                     dayOfMonth,
                     dayOfWeek,
                     timeRef,
                     toc);

        iRules.Append(rule);
        }
    else
        {
        TTzRule rule(fromYrTimeRef,
                     toYrTimeRef,
                     oldOffset,
                     newOffset,
                     month,
                     ruleDay,
                     dayOfMonth,
                     dayOfWeek,
                     timeRef,
                     toc);

        iRules.Append(rule);
        }

    return EPass;
    }

TInt CTzRulesTestStep::ReadTimeWithReferenceFromKeys(const TDesC& aTimeKey,
                                                     const TDesC& aRefKey,
                                                     const TDesC& aSectionName,
                                                     TTimeWithReference& aOutput)
    {
    // read string representing TTime first
    TPtrC ptrStr;
    TBool result = GetStringFromConfig(aSectionName, aTimeKey, ptrStr);
    if (result)
        {
        aOutput.iTime.Set(ptrStr);

        // read TInt representing TTzTimeReference
        TInt ref(-1);
        result = GetIntFromConfig(aSectionName, aRefKey, ref);
        if (result)
            {
            aOutput.iTimeReference = static_cast<TTzTimeReference> (ref);
            }
        }

    return result;
    }

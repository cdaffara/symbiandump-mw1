/**
* Copyright (c) 2007-2009 Nokia Corporation and/or its subsidiary(-ies).
* All rights reserved.
* This component and the accompanying materials are made available
* under the terms of "Eclipse Public License v1.0"
* which accompanies this distribution, and is available
* at the URL "http://www.eclipse.org/legal/epl-v10.html".
*
* Initial Contributors:
* Nokia Corporation - initial contribution.
*
* Contributors:
*
* Description:
*
*/



#ifndef __TZRULESTESTSTEP_H__
#define __TZRULESTESTSTEP_H__

#include <test/testexecutestepbase.h>
#include <vtzrules.h>

// String for the server create test step code
_LIT(KTzRules, "TzRules");

/**
TestCaseID(s):  APPSRV-TZ-TZRULES-0001

The purpose of this test step is to verify the published APIs of the Tz Rule classes
- TTzRule, CTzRules:

    1. Setup of Tz Rules (eg. add/remove/access rules, setup start/end year range,
       UTC offset, etc...)
    2. Conversion between local and UTC time
    3. Internalize/Externalize Tz Rules from read/write streams (eg. file)
*/
class CTzRulesTestStep : public CTestStep
    {
public:
    CTzRulesTestStep();
    ~CTzRulesTestStep();

protected:
    // from CTestStep
    TVerdict doTestStepL();
    TVerdict doTestStepPreambleL();
    TVerdict doTestStepPostambleL();

private:
    TVerdict ReadTzRulesL();
    TVerdict ReadAndAddRuleL(TInt aIndex);
    void ReadConversionTestSetup();
    TBool ReadTimeWithReferenceFromKeys(const TDesC& aTimeKey,
                                        const TDesC& aRefKey,
                                        const TDesC& aSectionName,
                                        TTimeWithReference& aOutput);

    TBool doTestReadWriteL(const CTzRules& aRules);
    TBool doTestConvertL(const CTzRules& aTzRules,
                         TBool& aAsymmetricOccurred);
    TBool doTestUtcConversionL(const CTzRules& aTzRules,
                               TBool& aAsymmetricConvert1,
                               TBool& aAsymmetricConvert2);
    TBool doTestLocalConversionL(const CTzRules& aTzRules,
                                 TBool& aAsymmetricConvert1,
                                 TBool& aAsymmetricConvert2);

private:
    TInt iOrgStartYear;
    TInt iOrgEndYear;
    TInt iNewStartYear;
    TInt iNewEndYear;
    TInt iInitStdOffset;

    TBool iTestClone;
    TBool iTestReadWrite;

    TBool iTestRuleAccess;
    TInt iRuleAccessIndex;

    TBool iTestConvert;
    TTime iConvertTestTime;
    TBool iTestConvertUTC;
    TTime iExpectedUTCTime;
    TBool iTestConvertLocal;
    TTime iExpectedLocalTime;

    RArray<TTzRule> iRules;
    };

#endif

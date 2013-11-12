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



#ifndef __ACTUALISEDRULES_H__
#define __ACTUALISEDRULES_H__

#include <test/testexecutestepbase.h>
#include <vtzrules.h>

// String for the server create test step code
_LIT(KActualisedRules, "ActualisedRules");

/**
TestCaseID(s):  APPSRV-TZ-ACTRUL-0001
                APPSRV-TZ-ACTRUL-0002

The purpose of this test step is to verify the following:

    1. Proper handling of inappropriate use of actualised rules and of invalid actualised
       rules.
    2. Use of actualised rules to correctly retrieve UTC offset and test if daylight savings time
       is in effect.
*/
class CActualisedRules : public CTestStep
    {
public:
    CActualisedRules();
    ~CActualisedRules();

protected:
    // from CTestStep
    TVerdict doTestStepL();
    TVerdict doTestStepPreambleL();
    TVerdict doTestStepPostambleL();

private:
    TVerdict ReadActualisedRulesL();
    TVerdict ReadOffsetTestSettings();
    TVerdict ReadDSTTestSettings();

private:
    TInt iStartYear;
    TInt iEndYear;

    TBool iOffsetTest;
    TTime iOffsetCheckTime;
    TTzTimeReference iOffsetCheckRef;
    TInt iOffsetCheckResult;    // in minutes

    TBool iDSTTest;
    TTime iDSTCheckTime;
    TBool iDSTCheckResult;

    RArray<TVTzActualisedRule> iRules;
    };

#endif

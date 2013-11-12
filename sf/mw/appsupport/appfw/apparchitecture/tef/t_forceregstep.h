// Copyright (c) 2010 Nokia Corporation and/or its subsidiary(-ies).
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

/**
 @file
 @internalComponent - Internal Symbian test code 
*/

#if (!defined __T_FORCEREG_H__)
#define __T_FORCEREG_H__


#include <test/testexecutestepbase.h>

class RTestableApaLsSession;

const TUid KPropertyCategory = {0x101F289C};
const TUint KForceRegTestPropertyKey = 1;

const TUint KForceRegApp1Executed = 0x1;
const TUint KForceRegApp2Executed = 0x2;
const TUint KStopForceRegApp2 = 0x10;
const TUint KApplistChanged = 0x0100;


/*Tests force registration functionality */

class CT_ForceRegStep : public CTestStep 
    {
public:
    CT_ForceRegStep();
    ~CT_ForceRegStep();
    virtual TVerdict doTestStepPreambleL();
    virtual TVerdict doTestStepPostambleL();
    virtual TVerdict doTestStepL();

private:
    void RunTestCases();   
    void TestSingleForceRegistration(RTestableApaLsSession &aLs);
    void TestMultipleForceRegistration(RTestableApaLsSession &aLs);
    void TestForceRegistrationSecurity(RTestableApaLsSession &aLs);
    void TestForceRegistrationWhenInstallationFailed(RTestableApaLsSession &aLs);
    void TestForceRegistrationAndNoClientNotification(RTestableApaLsSession &aLs); 
    };

_LIT(KT_ForceRegStep,"T_ForceReg");

#endif

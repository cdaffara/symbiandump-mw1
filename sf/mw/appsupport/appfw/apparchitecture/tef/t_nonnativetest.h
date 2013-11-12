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

#if (!defined __T_NONNATIVETEST_H__)
#define __T_NONNATIVETEST_H__


#include <test/testexecutestepbase.h>

class RTestableApaLsSession;


/*Tests non-native application launching functionality */

class CT_NonNativeTestStep : public CTestStep 
    {
public:
    CT_NonNativeTestStep();
    ~CT_NonNativeTestStep();
    virtual TVerdict doTestStepPreambleL();
    virtual TVerdict doTestStepPostambleL();
    virtual TVerdict doTestStepL();

private:
    void RunTestCases();   
    void TestNonNativeAppLaunchWithUnavailableMappingL(RTestableApaLsSession &aLs);
    void TestNonNativeUnsupportedAPIs(RTestableApaLsSession &aLs);
    void TestGetAppTypeL(RTestableApaLsSession &aLs);
    };

_LIT(KT_NonNativeTestStep,"T_NonNativeTest");

#endif

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

#if (!defined __T_CLIENTNOTIF_H__)
#define __T_CLIENTNOTIF_H__


#include <test/testexecutestepbase.h>

class RTestableApaLsSession;


/*Tests force registration functionality */

class CT_ClientNotifStep : public CTestStep 
    {
public:
    CT_ClientNotifStep();
    ~CT_ClientNotifStep();
    virtual TVerdict doTestStepPreambleL();
    virtual TVerdict doTestStepPostambleL();
    virtual TVerdict doTestStepL();
    
private:
    void RunTestCases();
    void TestClientNotifInitialApplistCreation(RTestableApaLsSession &aLs);
    void ChangeLocaleL(TLanguage aLanguage);    
    void TestClientNotifLanguageChange(RTestableApaLsSession &aLs);  
    
    void TestClientNotifSingleAppInstallation(RTestableApaLsSession &aLs);
    void TestClientNotifMultipleAppInstallation(RTestableApaLsSession &aLs);    
    void TestClientNotifUpgradeApp(RTestableApaLsSession &aLs); 
    void TestClientNotifMultipleInstallations(RTestableApaLsSession &aLs); 
    void TestClientNotifWithoutSetNotify(RTestableApaLsSession &aLs); 
    void TestClientNotifOnPackageUpgrade(RTestableApaLsSession &aLs); 
    void TestClientNotifOnPackageDowngrade(RTestableApaLsSession &aLs);     
    
    };


_LIT(KT_ClientNotifStep,"T_ClientNotif");

#endif

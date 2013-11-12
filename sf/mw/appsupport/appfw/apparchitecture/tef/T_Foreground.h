// Copyright (c) 2005-2010 Nokia Corporation and/or its subsidiary(-ies).
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

#ifndef		__T_FOREGROUND_H
#define		__T_FOREGROUND_H


#include <test/testexecutestepbase.h>
#include "apparctestserver.h"
#include "appfwk_test_appui.h"


const TInt KTVwaStandardAppUiId=0;

//
// A CBase Derived Class
//
class CTestDriver : public CBase
	{
public:
	CTestDriver(CCoeAppUi& aAppUi);
	~CTestDriver();
public:
	void LoadAppL();
	void CloseAppL();
	void BringToForegroundL();
	TInt SwitchCreate();
	TInt SwitchOpen();
	void CloseNonExistantWindowGroupTask();
private:
	inline CTestAppUi& AppUi(TInt aAppUiId=KTVwaStandardAppUiId); 
private:
	TInt iTestNum;
	TInt iState;
	CCoeAppUi& iAppUi;
	};


//
// A CTmsTestStep Derived Class
//
class CTestForegroundStep : public CTmsTestStep
	{
public:
	CTestForegroundStep();
	~CTestForegroundStep();
    virtual TVerdict doTestStepPreambleL();
    virtual TVerdict doTestStepPostambleL();
	virtual TVerdict doTestStepL();
	void ConstructAppL(CCoeEnv* aCoe);
private:
	};


//
// A CTestCoeAppUi derived Class
//
class CTestForegroundAppUi : public CTestCoeAppUi
    {
public:	// from CCoeAppUi
	CTestForegroundAppUi(CTmsTestStep* aStep);
	~CTestForegroundAppUi();
	void ConstructL();
	void RunTestStepL(TInt aNumStep);
private:
	CTestDriver* iDriver;
    };


//
// Test Step Name
//
_LIT(KTestForegroundStep, "T_Foreground");


#endif



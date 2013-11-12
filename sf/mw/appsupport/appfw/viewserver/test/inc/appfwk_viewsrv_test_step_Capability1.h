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

/**
 @file
 @test
 @internalComponent - Internal Symbian test code 
*/

#if !defined(__APPFWK_VIEWSRV_TEST_STEP_CAPABILITY1_H__)
#define __APPFWK_VIEWSRV_TEST_STEP_CAPABILITY1_H__

#include <vwsappst.h>
#include <test/testexecutestepbase.h>

/**
Literal used as name for test case.
*/
_LIT(KCAppfwkViewsrvTestStepCapability1,"Capability1");


//
// The CAppStarterCap1 class implements the MVwsAppStarter interface and is required to start the server
//
class CAppStarterCap1 : public CBase, public MVwsAppStarter
	{
public:
	static CAppStarterCap1* NewL();
	~CAppStarterCap1();
public: // From MVwsAppStarter.
	void StartAppL(TUid aAppUid,TThreadId& aThreadId);
	};


/**
CAppfwkViewsrvTestStepCapability1 encapsulates the functionality required 
for the Capability1 ViewSrv test case.
*/
class CAppfwkViewsrvTestStepCapability1 : public CTestStep
	{
public:
	CAppfwkViewsrvTestStepCapability1();
	virtual TVerdict doTestStepL();
	void TestCapability1L();
private:
	~CAppfwkViewsrvTestStepCapability1();
	};

#endif

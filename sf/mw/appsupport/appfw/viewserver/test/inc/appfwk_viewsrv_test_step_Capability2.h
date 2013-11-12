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

#if !defined(__APPFWK_VIEWSRV_TEST_STEP_CAPABILITY2_H__)
#define __APPFWK_VIEWSRV_TEST_STEP_CAPABILITY2_H__

#include <vwsappst.h>
#include <test/testexecutestepbase.h>

/**
Literal used as name for test case.
*/
_LIT(KCAppfwkViewsrvTestStepCapability2,"Capability2");


//
// The CAppStarterCap2 class implements the MVwsAppStarter interface and is required to start the server
//
class CAppStarterCap2 : public CBase, public MVwsAppStarter
	{
public:
	static CAppStarterCap2* NewL();
	~CAppStarterCap2();
public: // From MVwsAppStarter.
	void StartAppL(TUid aAppUid,TThreadId& aThreadId);
	};


/**
CAppfwkViewsrvTestStepCapability2 encapsulates the functionality required 
for the Capability2 ViewSrv test case.
*/
class CAppfwkViewsrvTestStepCapability2 : public CTestStep
	{
public:
	CAppfwkViewsrvTestStepCapability2();
	virtual TVerdict doTestStepL();
	void TestCapability2L();
private:
	~CAppfwkViewsrvTestStepCapability2();
	};

#endif

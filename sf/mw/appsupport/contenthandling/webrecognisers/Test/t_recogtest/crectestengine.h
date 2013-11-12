// Copyright (c) 1997-2009 Nokia Corporation and/or its subsidiary(-ies).
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

#ifndef __CRECTESTENGINE_H__
#define __CRECTESTENGINE_H__

#include <e32base.h>
#include <ecom/ecom.h>
#include <e32test.h>
#include <apgcli.h>

#include "cecomrecogtest.h"
#include "tfailedrectest.h"

class CRecTestLogger;

class CRecTestEngine : public CBase
	{
public:
	static CRecTestEngine* NewL();
	~CRecTestEngine();
	
	void StartTestsL();
	
private:
	CRecTestEngine();
	void ConstructL();
	
	void RunTestCasesFromPluginL(TInt aPluginIndex);
	void ProcessTestResultL(TInt aError);

private:
	REComSession iEcomSession;
	RImplInfoPtrArray iPluginArray;
	RPointerArray<CEComRecogTest> iTestArray;
	RTest iTest;
	CRecTestLogger* iLogger;
	RApaLsSession iAppArcSession;
	TInt iTestPassCount;
	TInt iTestFailCount;
	TInt iCurrentTest;
	const TDesC* iCurrentTestTitle;
	RPointerArray<TFailedRecTest> iFailedTests;
	};

#endif // __CRECTESTENGINE_H__

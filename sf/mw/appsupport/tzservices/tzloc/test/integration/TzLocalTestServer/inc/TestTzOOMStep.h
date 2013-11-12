/**
* Copyright (c) 2005-2009 Nokia Corporation and/or its subsidiary(-ies).
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



#ifndef __TESTTZOOMSTEP_H__
#define __TESTTZOOMSTEP_H__

#include <test/testexecutestepbase.h>
#include "TestTzLocalBaseStep.h"
#include "TzLocalTestServer.h"
#include "tzlocalizer.h"

_LIT(KTestTzOOMStep, "TestTzOOMStep");

class CTestTzOOMStep : public CTestTzLocalBaseStep
	{
public:
	CTestTzOOMStep();
	~CTestTzOOMStep();
    TVerdict doTestStepPreambleL();
    TVerdict doTestStepPostambleL();
	TVerdict doTestStepL();	 	
	 
private:
    CTzLocalizer* iLocalizer;
    
    void TestGetTzWithIdOOML(CTzLocalizer* aLoc);
    void TestGetTzWithCityOOML(CTzLocalizer* aLoc);
    void TestGetAllTzOOML(CTzLocalizer* aLoc);
    void TestGetAllTzWithUtcOOML(CTzLocalizer* aLoc);
    void TestSetTzOOML(CTzLocalizer* aLoc);
  	};

#endif

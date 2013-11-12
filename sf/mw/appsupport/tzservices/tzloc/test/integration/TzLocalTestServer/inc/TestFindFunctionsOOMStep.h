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



#ifndef __TESTFINDFUNCTIONSOOMSTEP_H__
#define __TESTFINDFUNCTIONSOOMSTEP_H__

#include <test/testexecutestepbase.h>
#include "TestTzLocalBaseStep.h"
#include "TzLocalTestServer.h"
#include "tzlocalizer.h"

_LIT(KTestFindFunctionsOOMStep, "TestFindFunctionsOOMStep");

class CTestFindFunctionsOOMStep : public CTestTzLocalBaseStep
	{
public:
	CTestFindFunctionsOOMStep();
	~CTestFindFunctionsOOMStep();
    TVerdict doTestStepPreambleL();
    TVerdict doTestStepPostambleL();
	TVerdict doTestStepL();	 	
	 
private:
    CTzLocalizer* iLocalizer;
    TPtrC iCityFromIni;
    TPtrC iTzFromIni;
    TPtrC iCityGroupFromIni;
    
    void TestFindCityOOML(CTzLocalizer* aLoc);
    void FindTzOOML(CTzLocalizer* aLoc);
    void FindCityGroupOOML(CTzLocalizer* aLoc);
  	};



#endif

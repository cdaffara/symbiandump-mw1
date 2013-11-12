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



#ifndef __TESTCITYOOMSTEP_H__
#define __TESTCITYOOMSTEP_H__

#include <test/testexecutestepbase.h>
#include "TestTzLocalBaseStep.h"
#include "TzLocalTestServer.h"
#include "tzlocalizer.h"

_LIT(KTestCityOOMStep, "TestCityOOMStep");

class CTestCityOOMStep : public CTestTzLocalBaseStep
	{
public:
	CTestCityOOMStep();
	~CTestCityOOMStep();
    TVerdict doTestStepPreambleL();
    TVerdict doTestStepPostambleL();
	TVerdict doTestStepL();	 	
	 
private:
    
    CTzLocalizer* iLocalizer;
    TPtrC iCityName;
    TPtrC iTzName;
    
    void TestGetDefCityWithIdOOML(CTzLocalizer* iLocalizer);
    void TestGetCitiesOOML(CTzLocalizer* iLocalizer);
    void TestGetCitiesWithIdOOML(CTzLocalizer* iLocalizer);
    void TestAddCityOOML(CTzLocalizer* iLocalizer);
    void TestGetDefCityFromCityOOML(CTzLocalizer* iLocalizer);
    void TestGetDefCityFromTzOOML(CTzLocalizer* iLocalizer);
    void TestGetCitiesFromTzOOML(CTzLocalizer* iLocalizer);
    void TestGetCitiesFromCityOOML(CTzLocalizer* iLocalizer);
    void TestRemoveCityOOML(CTzLocalizer* iLocalizer);
    void TestGetCitiesWithUtcOOML(CTzLocalizer* iLocalizer);
  	};



#endif

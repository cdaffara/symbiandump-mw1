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



#ifndef __TESTTZMANIPOOMSTEP_H__
#define __TESTTZMANIPOOMSTEP_H__

#include <test/testexecutestepbase.h>
#include "TestTzLocalBaseStep.h"
#include "TzLocalTestServer.h"
#include "tzlocalizer.h"


_LIT(KTestTzManipOOMStep, "TestTzManipOOMStep");

class CTestTzManipOOMStep : public CTestTzLocalBaseStep
	{
public:
	CTestTzManipOOMStep();
	~CTestTzManipOOMStep();
    TVerdict doTestStepPreambleL();
    TVerdict doTestStepPostambleL();
	TVerdict doTestStepL();	 	
	 
private:
    CTzLocalizer* iLocalizer;
    
    void TestGetFrequentlyUsedZoneOOML(CTzLocalizer* aLoc);
    void TestGetCachedZCityOOML(CTzLocalizer* aLoc);
    void TestSetFrequentlyUsedZoneOOML(CTzLocalizer* aLoc);
    void TestSetCachedZWithTzOOML(CTzLocalizer* aLoc);
    void TestSetCachedZWithCityOOML(CTzLocalizer* aLoc);
  	};



#endif

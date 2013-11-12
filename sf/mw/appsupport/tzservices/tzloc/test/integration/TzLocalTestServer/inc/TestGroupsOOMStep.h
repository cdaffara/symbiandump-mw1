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



#ifndef __TESTGROUPSOOMSTEP_H__
#define __TESTGROUPSOOMSTEP_H__

#include <test/testexecutestepbase.h>
#include "TestTzLocalBaseStep.h"
#include "TzLocalTestServer.h"
#include "tzlocalizer.h"

_LIT(KTestGroupsOOMStep, "TestGroupsOOMStep");

class CTestGroupsOOMStep : public CTestTzLocalBaseStep
	{
public:
	CTestGroupsOOMStep();
	~CTestGroupsOOMStep();
    TVerdict doTestStepPreambleL();
    TVerdict doTestStepPostambleL();
	TVerdict doTestStepL();	 	
	 
private:
    CTzLocalizer* iLocalizer;
    
    void TestGetAllGroupsOOML(CTzLocalizer* aLoc);
    void TestGetGroupFromCityOOML(CTzLocalizer* aLoc);
    void TestGetGroupFromIdOOML(CTzLocalizer* aLoc);
    void TestGetCitiesFromGroupOOML(CTzLocalizer* aLoc);
    void TestGetCitiesFromCityOOML(CTzLocalizer* aLoc);
    void TestGetCitiesFromGroupIdOOML(CTzLocalizer* aLoc);
  	};



#endif

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



#ifndef __TESTGETCITYGROUPINFOSTEP_H__
#define __TESTGETCITYGROUPINFOSTEP_H__

#include <test/testexecutestepbase.h>
#include "TestTzLocalBaseStep.h"
#include "TzLocalTestServer.h"
#include "tzlocalizer.h"

_LIT(KTestGetCityGroupInfoStep, "TestGetCityGroupInfoStep");

class CTestGetCityGroupInfoStep : public CTestTzLocalBaseStep
	{
public:
	CTestGetCityGroupInfoStep();
	~CTestGetCityGroupInfoStep();
    TVerdict doTestStepPreambleL();
    TVerdict doTestStepPostambleL();
	TVerdict doTestStepL();	 	
	 
private:
    CTzLocalizer* iLocalizer;
    
    void GetAllGroupsL(CTzLocalizer* iLocalizer, CTzLocalizer::TTzSortOrder aOrder);
    void GetAllCitiesInGroupL(CTzLocalizer* iLocalizer);
    TUint iDataSource;//1 - Use only system TZ localization. 2. Use only non-localized user TZ names. 3 - Use both system TZ localization and non-localized user TZ names.
  	};



#endif

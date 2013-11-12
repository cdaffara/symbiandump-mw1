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



#ifndef __TESTGETSORTEDCITIESSTEP_H__
#define __TESTGETSORTEDCITIESSTEP_H__

#include <test/testexecutestepbase.h>
#include "TestTzLocalBaseStep.h"
#include "TzLocalTestServer.h"
#include "tzlocalizer.h"

_LIT(KTestGetSortedCitiesStep, "TestGetSortedCitiesStep");

class CTestGetSortedCitiesStep : public CTestTzLocalBaseStep
	{
public:
	CTestGetSortedCitiesStep();
	~CTestGetSortedCitiesStep();
    TVerdict doTestStepPreambleL();
    TVerdict doTestStepPostambleL();
	TVerdict doTestStepL();	 	
	 
private:
    CTzLocalizer* iLocalizer;
    CTzLocalizer::TTzSortOrder iSortOrder;
	CActiveScheduler* iScheduler;
	TUint iDataSource;//1 - Use only system TZ localization. 2 - Use both system TZ localization and non-localized user TZ names.
  	};



#endif

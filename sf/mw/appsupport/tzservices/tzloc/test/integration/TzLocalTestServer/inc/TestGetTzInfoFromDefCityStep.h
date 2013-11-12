/**
* Copyright (c) 2004-2009 Nokia Corporation and/or its subsidiary(-ies).
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



#ifndef __TESTGETTZINFOFROMDEFCITYSTEP_H__
#define __TESTGETTZINFOFROMDEFCITYSTEP_H__

#include <test/testexecutestepbase.h>
#include "TzLocalTestServer.h"
#include "TestTzLocalBaseStep.h"
#include "tzlocalizer.h"

_LIT(KTestGetTzInfoFromDefCity, "TestGetTzInfoFromDefCityStep");

class CTestGetTzInfoFromDefCityStep : public CTestTzLocalBaseStep
	{
public:
	CTestGetTzInfoFromDefCityStep();
	~CTestGetTzInfoFromDefCityStep();
    TVerdict doTestStepPreambleL();
    TVerdict doTestStepPostambleL();
	TVerdict doTestStepL();	 
	 
private:
    CTzLocalizer* iLocalizer;
    TInt iTzIdFromIni;
  	};



#endif

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



#ifndef __TESTGETTZUTCOFFSTEP_H__
#define __TESTGETTZUTCOFFSTEP_H__

#include <test/testexecutestepbase.h>
#include "TestTzLocalBaseStep.h"
#include "TzLocalTestServer.h"
#include "tzlocalizer.h"

_LIT(KTestGetTzUTCOffStep, "TestGetTzUTCOffStep");

class CTestGetTzUTCOffStep : public CTestTzLocalBaseStep
	{
public:
	CTestGetTzUTCOffStep();
	~CTestGetTzUTCOffStep();
    TVerdict doTestStepPreambleL();
    TVerdict doTestStepPostambleL();
	TVerdict doTestStepL();	 	
	 
private:
    TInt iUtcOffset;
    CTzLocalizer* iLocalizer;    
  	};



#endif

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



#ifndef __TESTSETTIMEZONESTEP_H__
#define __TESTSETTIMEZONESTEP_H__

#include <test/testexecutestepbase.h>
#include "TestTzLocalBaseStep.h"
#include "TzLocalTestServer.h"
#include <tz.h>

_LIT(KSetTimeZoneStep, "TestSetTimeZoneStep");
_LIT(KDefaulTzTag, "DefaultTimeZone");
_LIT(KTzIdTextTag, "TzIdText");

const TInt KMaxTzIdNameLength = 64;


class CTestSetTimeZoneStep : public CTestTzLocalBaseStep
	{
public:
	CTestSetTimeZoneStep();
	~CTestSetTimeZoneStep();
    TVerdict doTestStepPreambleL();
    TVerdict doTestStepPostambleL();
	TVerdict doTestStepL();
	 
private:
    void SetCurrentTimeZoneL();
	void CheckTimeZoneL();
	void SetTzInTzLocalL();

    TInt iSetTzInTzLocal;
    TInt iTzId;
    TBuf8<KMaxTzIdNameLength> iTzIdName; 
    RTz iTzServer;
  	};



#endif

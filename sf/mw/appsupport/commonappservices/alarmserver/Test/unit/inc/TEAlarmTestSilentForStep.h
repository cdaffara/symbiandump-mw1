// Copyright (c) 2005-2009 Nokia Corporation and/or its subsidiary(-ies).
// All rights reserved.
// This component and the accompanying materials are made available
// under the terms of "Eclipse Public License v1.0"
// which accompanies this distribution, and is available
// at the URL "http://www.eclipse.org/legal/epl-v10.html".
//
// Initial Contributors:
// Nokia Corporation - initial contribution.
//
// Contributors:
//
// Description:
//

#ifndef __TEALARMTESTSILENTFORSTEP_H__
#define __TEALARMTESTSILENTFORSTEP_H__

#include "TEAlarmTestSilentStep.h"

class RASCliSession;

class CTEAlarmTestSilentForStep : public CTEAlarmTestSilentStep
	{
public:
	CTEAlarmTestSilentForStep();
	~CTEAlarmTestSilentForStep();
	
	virtual TVerdict doTestStepPreambleL();
	virtual TVerdict doTestStepL();
	virtual TVerdict doTestStepPostambleL();
private:
	TBool CheckSilentForL(const RASCliSession &aSession, TBool aSecondPass);
	
private:
	TTimeIntervalMinutes iSilentFor;
	TBool iTestUTCoffset;
	TBool iTestSysTime;
	TInt iSysTimeOffset;
	};

_LIT(KTEAlarmTestSilentForStep, "SilentForStep");

#endif // __TEALARMTESTSILENTFORSTEP_H__


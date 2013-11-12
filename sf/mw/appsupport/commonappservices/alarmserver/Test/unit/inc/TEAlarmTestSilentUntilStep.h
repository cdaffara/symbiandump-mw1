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

#ifndef __TEALARMTESTSILENTUNTILSTEP_H__
#define __TEALARMTESTSILENTUNTILSTEP_H__

#include "TEAlarmTestSilentStep.h"
class RASCliSession;

class CTEAlarmTestSilentUntilStep : public CTEAlarmTestSilentStep
	{
public:
	CTEAlarmTestSilentUntilStep();
	~CTEAlarmTestSilentUntilStep();
	
	virtual TVerdict doTestStepPreambleL();
	virtual TVerdict doTestStepL();
	virtual TVerdict doTestStepPostambleL();
	
private:
	TBool CheckSilentUntilL(const RASCliSession &aSession);	
	
private:
	TTime iSilentUntil;
	TTime iInitialTime;
	TBool iInvalidated;
	};

_LIT(KTEAlarmTestSilentUntilStep, "SilentUntilStep");

#endif // __TEALARMTESTSILENTUNTILSTEP_H__

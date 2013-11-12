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

#include "TEAlarmTestSilentStep.h"

_LIT(KInitialUTC, "InitialUTC");
_LIT(KMoveToUTC, "MoveToUTC");

TVerdict CTEAlarmTestSilentStep::doTestStepPreambleL()
	{
	SetTestStepResult(EPass);

	TVerdict verdict = CTEAlarmTestStepBase::doTestStepPreambleL();
	if(verdict != EPass)
		{
		SetTestStepResult(verdict);
		return TestStepResult();
		}

	if (!GetOffsetFromConfig(ConfigSection(), KInitialUTC, iInitialUTC))
		{
		SetTestStepResult(EFail);
		return TestStepResult();
		}

	if (!GetOffsetFromConfig(ConfigSection(), KMoveToUTC, iMoveToUTC))
		{
		SetTestStepResult(EFail);
		return TestStepResult();
		}

	return TestStepResult();
	}

TVerdict CTEAlarmTestSilentStep::doTestStepPostambleL()
	{
	TVerdict verdict = CTEAlarmTestStepBase::doTestStepPostambleL();
	if(verdict != EPass)
		{
		SetTestStepResult(verdict);
		return TestStepResult();
		}
	return TestStepResult();
	}

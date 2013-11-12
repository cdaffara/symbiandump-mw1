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

#ifndef __TEALARMTESTSILENTSTEP_H__
#define __TEALARMTESTSILENTSTEP_H__

#include <test/testexecutestepbase.h>

#include "TEAlarmTestStepBase.h"

class CTEAlarmTestSilentStep : public CTEAlarmTestStepBase
	{
public:
	virtual TVerdict doTestStepPreambleL();
	virtual TVerdict doTestStepPostambleL();
	
protected:
	TTimeIntervalSeconds iInitialUTC;
	TTimeIntervalSeconds iMoveToUTC;
	};

#endif // __TEALARMTESTSILENTSTEP_H__

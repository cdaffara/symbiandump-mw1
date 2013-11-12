// Copyright (c) 2007-2009 Nokia Corporation and/or its subsidiary(-ies).
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

#ifndef __TEALARMTESTGETALARMDATASTEP_H__
#define __TEALARMTESTGETALARMDATASTEP_H__


#include <asshdalarm.h>

#include "TEAlarmTestStepBase.h"


_LIT(KTEAlarmTestGetAlarmDataStep,"TestGetAlarmDataStep");


class CTEAlarmTestGetAlarmDataStep : public CTEAlarmTestStepBase
	{
public:
	CTEAlarmTestGetAlarmDataStep();
	~CTEAlarmTestGetAlarmDataStep();
	
	TVerdict doTestStepL();
	};


#endif // __TEALARMTESTGETALARMDATASTEP_H__

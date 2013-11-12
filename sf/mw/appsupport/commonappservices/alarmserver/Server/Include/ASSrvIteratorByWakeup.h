// Copyright (c) 2008-2009 Nokia Corporation and/or its subsidiary(-ies).
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

#ifndef __ASSRVITERATORBYWAKEUP_H__
#define __ASSRVITERATORBYWAKEUP_H__

// System includes
#include <e32base.h>

// User includes
#include "ASSrvIteratorBase.h"
#include "ASSrvDefs.h"

/**
 * Locate alarms by their wakeup flag
 */
class RASSrvIteratorByWakeup : public RASSrvIteratorBase
	{
public:
	/**
	 * Constructor
	 *
	 * @param aQueue The alarm queue containing the alarms
	 */
	RASSrvIteratorByWakeup(CASSrvAlarmQueue& aQueue);


private: // From RASSrvIteratorBase
	TBool Matches(const TASSrvAlarm& aAlarm) const;
	};

#endif  // __ASSRVITERATORBYWAKEUP_H__

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

#include "ASSrvDSTChange.h"
#include "ASSrvAlarmQueue.h"

#include <e32property.h>

// These are copies of definitions in the tz server
// they must not change there
const TInt KDSTChangePropertyKey(0x10285B32);

CASSrvDSTChange* CASSrvDSTChange::NewL(CASSrvAlarmQueue& aASSrvAlarmQueue)
	{
	CASSrvDSTChange* self = new(ELeave) CASSrvDSTChange(aASSrvAlarmQueue);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self;
	}

CASSrvDSTChange::CASSrvDSTChange(CASSrvAlarmQueue& aASSrvAlarmQueue)
	:CActive(CActive::EPriorityStandard),
	iASSrvAlarmQueue(aASSrvAlarmQueue),
	iNextDSTChangeUTC(Time::NullTTime()),
	iNextUTCOffset(0),
	iPreviousDSTChangeUTC(Time::NullTTime()),
	iPreviousUTCOffset(0)
	{
	CActiveScheduler::Add(this);
	}

void CASSrvDSTChange::ConstructL()
	{
	TInt attachError = iDSTChangeProperty.Attach(KUidSystemCategory, KDSTChangePropertyKey);
	
	if (attachError == KErrNone)
		{
		// Only subscribe if there was no error.  There should only be an error if the property
		// has not yet been defined and there was no memory to create the property.  The behaviour
		// in this fail case will be for the alarm server to not be aware of any DST change events
		// that are scheduled.
		
		iDSTChangeProperty.Subscribe(iStatus);
		SetActive();
		
		UpdatePropertyValuesL();
		}
	}

CASSrvDSTChange::~CASSrvDSTChange()
	{
	Cancel();
	iDSTChangeProperty.Close();
	}

TTime CASSrvDSTChange::NextDSTChangeUTC() const
	{
	return iNextDSTChangeUTC;
	}

TTimeIntervalMinutes CASSrvDSTChange::NextUTCOffset() const
	{
	return iNextUTCOffset;
	}

TTime CASSrvDSTChange::PreviousDSTChangeUTC() const
	{
	return iPreviousDSTChangeUTC;
	}

TTimeIntervalMinutes CASSrvDSTChange::PreviousUTCOffset() const
	{
	return iPreviousUTCOffset;
	}

void CASSrvDSTChange::UpdatePropertyValuesL()
	{
	TPckgBuf<TNextDSTChange> fetchedDSTChangeBuf;
	TInt getError = iDSTChangeProperty.Get(fetchedDSTChangeBuf);
	
	if (getError == KErrNone)
		{
		iNextDSTChangeUTC = fetchedDSTChangeBuf().iNextDSTChangeUTC;
		iNextUTCOffset = fetchedDSTChangeBuf().iNextUTCOffset;
			
		if (fetchedDSTChangeBuf().iVersion >= 2)
			{
			iPreviousDSTChangeUTC = fetchedDSTChangeBuf().iPreviousDSTChangeUTC;
			iPreviousUTCOffset = fetchedDSTChangeBuf().iPreviousUTCOffset;
			}
		}
	}

void CASSrvDSTChange::RunL()
	{	
	if (iStatus.Int() == KErrNone)
		{
		// Subscribe again before handling the change
		iDSTChangeProperty.Subscribe(iStatus);
		SetActive();
		
		UpdatePropertyValuesL();
#ifdef SYMBIAN_SYSTEM_STATE_MANAGEMENT
		// Tell the alarm queue that it may need to reset the RTC
		iASSrvAlarmQueue.HandleNextDSTChangeEventL();
#endif
		}
	}

void CASSrvDSTChange::DoCancel()
	{
	iDSTChangeProperty.Cancel();
	}



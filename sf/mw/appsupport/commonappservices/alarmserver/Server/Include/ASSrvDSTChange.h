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

#ifndef __ASSRVNEXTDSTCHANGEOBSERVER_H__
#define __ASSRVNEXTDSTCHANGEOBSERVER_H__

#include <e32std.h>
#include <e32base.h>
#include <e32property.h>

#include <asshddefs.h>

class CASSrvAlarmQueue;

/** This class is used to listen to changes to the DST changes
that are published by the time zone server.  The time of the next and
previous DST change and the next and previous offset can be retrieved.
*/
class CASSrvDSTChange : public CActive
	{
private:

	/** This is a copy of the struct used to publish the DST change in tz. We cannot 
	use the same one as we do not want a dependancy between these components
	*/
	struct TNextDSTChange
		{
		TUint8 iVersion;
		TTime iNextDSTChangeUTC;
		TInt iNextUTCOffset;
		TTime iPreviousDSTChangeUTC;
		TInt iPreviousUTCOffset;
		TInt iReserved[12];
		};
		
public:
	static CASSrvDSTChange* NewL(CASSrvAlarmQueue& aASSrvAlarmQueue);
	~CASSrvDSTChange();
	
	TTime NextDSTChangeUTC() const;
	TTimeIntervalMinutes NextUTCOffset() const;
	
	TTime PreviousDSTChangeUTC() const;
	TTimeIntervalMinutes PreviousUTCOffset() const;
	
private: // from CActive
	void RunL();
	void DoCancel();
	
private:
	CASSrvDSTChange(CASSrvAlarmQueue& aASSrvAlarmQueue);
	void ConstructL();
	
	void UpdatePropertyValuesL();

private:
	CASSrvAlarmQueue& iASSrvAlarmQueue;
	RProperty iDSTChangeProperty;
	
	TTime iNextDSTChangeUTC;
	TTimeIntervalMinutes iNextUTCOffset;
	
	TTime iPreviousDSTChangeUTC;
	TTimeIntervalMinutes iPreviousUTCOffset;
	};

#endif __ASSRVNEXTDSTCHANGEOBSERVER_H__

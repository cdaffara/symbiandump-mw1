// Copyright (c) 1999-2009 Nokia Corporation and/or its subsidiary(-ies).
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

#ifndef __ASALTREQUESTSETALARM_H__
#define __ASALTREQUESTSETALARM_H__

#include <e32base.h>

#include <asshdalarm.h>
#include "ASSrvNotificationCoordinator.h"

class RASAltClientSession;
class CASSrvAlarmQueue;
class MASSrvSessionCollection;

class CASAltRequestSetAlarm : public CActive
	{
public:								
	static CASAltRequestSetAlarm* NewL(RASAltClientSession& aSession, CASSrvNotificationCoordinator& aParent, CASSrvAlarmQueue& aQueue, MASSrvSessionCollection& aSessionCollection);
	~CASAltRequestSetAlarm ();

	void RequestSetAlarm(TAlarmId aId);
	void RequestSetAlarmCancel(TAlarmId aId);
	
private:									
	CASAltRequestSetAlarm(RASAltClientSession& aSession, CASSrvNotificationCoordinator& aParent, CASSrvAlarmQueue& aQueue, MASSrvSessionCollection& aSessionCollection);
	void ConstructL();
	
	void RequestNextAlarm(TAlarmId aId);

private:  // from CActive
	void RunL();
	void DoCancel();

private:									
	RASAltClientSession& iSession;
	CASSrvNotificationCoordinator& iParent;
	CASSrvAlarmQueue& iQueue;
	MASSrvSessionCollection& iSessionCollection;
	TASShdAlarm* iAlarmCopy;
	HBufC8* iAlarmDataCopy;
	TBool iCancelledCurrentRequest;
	RArray<TAlarmId> iRequestedAlarmIds;
	};

#endif

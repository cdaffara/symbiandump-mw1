/**
* Copyright (c) 2005-2009 Nokia Corporation and/or its subsidiary(-ies).
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


 
#ifndef __ALARMNOTIFIER_H_
#define __ALARMNOTIFIER_H_

#include <asclisession.h>
#include <asshdalarm.h>


class CAlarmNotifier : public CActive
	{
public:
	static CAlarmNotifier* NewL(RASCliSession& aAlarmServer);
	~CAlarmNotifier();
	
	void Start(const TTime aDueTime, TBool aIsUtcAlarm);
	inline const TAlarmId AlarmId() const;
private:
	CAlarmNotifier(RASCliSession& aAlarmServer);
	void ConstructL();
	
	// from CActive
	void RunL();
	void DoCancel();
	
	void InitialiseAlarm(const TTime aDueTime, TBool aIsUtcAlarm);
private:
	RASCliSession& iAlarmServer;
	TAlarmId iAlarmId;
	TASShdAlarm iAlarm;
	};

inline const TAlarmId CAlarmNotifier::AlarmId() const {return iAlarmId;}

#endif // __ALARMACTIVE_H_

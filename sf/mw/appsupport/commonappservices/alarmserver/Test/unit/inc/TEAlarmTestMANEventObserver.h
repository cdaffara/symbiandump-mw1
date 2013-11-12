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

#ifndef __TEALARMTESTMANEVENTOBSERVER_H
#define __TEALARMTESTMANEVENTOBSERVER_H


#include "TEAlarmTestMANEventEntry.h"
#include <asshdalarm.h>

class RANTestClient;


// //
//
// MMANEventHandler
//
// //

class MMANEventHandler
	{
public:
	virtual void HandleEventL(TEventEntry aEvent) = 0;
	};


// //
//
// CMANEventObserver
//
// //

class CMANEventObserver : public CActive
	{
public:
	static CMANEventObserver* NewL(const TEventEntry& aEventEntry, RANTestClient& aANTestClient, 
								MMANEventHandler& aEventHandler);

	virtual ~CMANEventObserver();
	virtual void RunL();
	virtual void DoCancel() {};
	
	void RegisterForNextEventL();

private:
	CMANEventObserver(const TEventEntry& aEventEntry, RANTestClient& aANTestClient, 
						MMANEventHandler& aEventHandler);						
	
private:
	RANTestClient&		iCAASClient;
	MMANEventHandler& 	iEventHandler;
	
	TEventEntry	iObserveEvent;
	
	TASShdAlarm	iASShdAlarm;		// used in NotifyOnAlarm
	TBuf<256>	iSoundFileName;		// used in NotifyOnSoundStart
	};


// //
//
// CMANDelayedActionHandler
//
// //

class CMANDelayedActionHandler : public CTimer
	{
public:
	static CMANDelayedActionHandler* NewL(const TEventEntry& aEventEntry, 
								MMANEventHandler& aEventHandler);

	virtual ~CMANDelayedActionHandler();
	virtual void RunL();

private:
	CMANDelayedActionHandler(const TEventEntry& aAction, MMANEventHandler& aEventHandler);						

private:
	MMANEventHandler& 	iEventHandler;
	TEventEntry	iDelayedAction;
	};




#endif	// __TEALARMTESTMANEVENTOBSERVER_H



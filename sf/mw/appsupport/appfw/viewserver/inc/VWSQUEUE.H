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


#ifndef __VWSQUEUE_H__
#define __VWSQUEUE_H__

#include <e32std.h>
#include <e32base.h>

#include "VWSDEBUG.H"
#include "VWSERVER.H"


//
// Constants.
//

const TInt KQueueSize=16;


//
// Forward declarations.
//

class CVwsEvent;
class CVwsServer;


/**
 * The CVwsEventQueue class implements an event queue for processing server and session events.
 */
class CVwsEventQueue : public CBase
	{
public:
	CVwsEventQueue();

#ifdef __DO_LOGGING__
	CVwsEventQueue(const TDesC& aName);
	void SetName(const TDesC& aName);
#endif
	~CVwsEventQueue();
	void ProcessEventL(CVwsEvent* aEvent);
	void HandleEventProcessed();
	CVwsEvent* Head() const;
	CVwsEvent* Tail() const;
	TInt Count() const;
	void HandleSessionRemoval(const TThreadId& aClientThreadId);
	void KickStart();
private:
	void DeleteHead();
	TInt AddToTail(CVwsEvent*& aEvent);
	TInt RemoveHead();
	TInt DoAddToTail(CVwsEvent* aEvent);
	TInt AddToTailIfNotPair(CVwsEvent*& aEvent);
	CVwsEvent* At(TInt aIndex);
private:
	enum TState
		{
		EEmpty,
		EProcessing
		};
private:
	CVwsEvent* iQueue[KQueueSize];
	TInt iQueueStart;
	TInt iQueueEnd;
	TInt iQueueSize;
	TState iState;
#ifdef __DO_LOGGING__
	TBuf<64> iQueueName;
#endif
	};


/**
 * The CVwsEvent class is the base class for events which can be processed by the server and session event queues
 */
class CVwsEvent : public CBase
	{
public:
	enum TType
		{
		ENormal,
		ERejectPairs
		};
public:
	CVwsEvent(TType aType,CVwsEventQueue& aQueue);
	~CVwsEvent();
	TType Type();
	void ReportEventProcessed();
private:
	friend class CVwsEventQueue;
	virtual void ProcessEventL()=0;
	virtual void HandleAddedToQueue();
	virtual void HandleLastOnQueue();
	virtual void HandleSessionRemoval(const TThreadId& aClientThreadId);
private:
	TType iType;
	CVwsEventQueue& iQueue;
	};


#endif

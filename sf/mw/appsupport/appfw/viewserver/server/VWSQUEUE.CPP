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

#ifdef SYMBIAN_ENABLE_SPLIT_HEADERS
#include "vwsinternal.h"
#include "vwsdefpartner.h"
#endif //SYMBIAN_ENABLE_SPLIT_HEADERS
#include "VWSQUEUE.H"


//
// CVwsEventQueue.
//

CVwsEventQueue::CVwsEventQueue() 
	{
	}

#ifdef __DO_LOGGING__
CVwsEventQueue::CVwsEventQueue(const TDesC& aName) : iQueueName(aName)
	{
	}

void CVwsEventQueue::SetName(const TDesC& aName)
	{
	iQueueName=aName;
	}
#endif

CVwsEventQueue::~CVwsEventQueue()
	{
	do
		{
		delete Head();
		}
	while (RemoveHead() == KErrNone);
	}

void CVwsEventQueue::ProcessEventL(CVwsEvent* aEvent)
	{
	TInt err=AddToTail(aEvent);
	if(err!=KErrNone)
		{
		if(aEvent!=NULL)
			{
			delete aEvent;
			aEvent=NULL;
			}
		User::Leave(err);
		}

	if (aEvent)
		{
		switch (iState)
			{
			case EEmpty:
				{
				iState=EProcessing;
				TRAPD(err,aEvent->ProcessEventL());
				if (err)
					{
					DeleteHead();
					iState=EEmpty;
					}
				User::LeaveIfError(err);
				}
				break;
			case EProcessing:
				// Wait until current event has finished being processed.
				break;
			default:
				ASSERT(EFalse);
			}
		}
	}

void CVwsEventQueue::HandleEventProcessed()
	{
	if (iQueueSize==1)
		{
		Head()->HandleLastOnQueue();
		DeleteHead();
		LOG3(CVwsLog::ELoud,_L("Completed event at head of %S, queue empty"),&iQueueName);
		iState=EEmpty;
		}
	else if (iQueueSize>0)
		{
		DeleteHead();
		LOG4(CVwsLog::ELoud,_L("Completed event at head of %S, processing next [queue size %d]"),&iQueueName,iQueueSize);
		// Start processing next event in the queue.
		TInt err=KErrGeneral;
		while (iQueueSize>0 && err)
				{
				TRAP(err, Head()->ProcessEventL());
				if (err)
					{
					DeleteHead();
					if (0==iQueueSize)
						{
						iState=EEmpty;
						}
					}
				}
		}
	}

CVwsEvent* CVwsEventQueue::Head() const
	{
	if (iQueueSize==0)
		return NULL;

	return iQueue[iQueueStart];
	}

CVwsEvent* CVwsEventQueue::Tail() const
	{
	if (iQueueSize==0)
		return NULL;

	return iQueue[(iQueueEnd+KQueueSize-1)%KQueueSize];
	}

TInt CVwsEventQueue::Count() const
	{
	return iQueueSize;
	}

void CVwsEventQueue::DeleteHead()
	{
	delete Head();
	RemoveHead();
	}

void CVwsEventQueue::HandleSessionRemoval(const TThreadId& aClientThreadId)
	{
	if (iQueueSize>0)
		{
		LOG4(CVwsLog::ELoud,_L("Session removed - handling removal in head event of %S [queue size %d]"),&iQueueName,iQueueSize);
		Head()->HandleSessionRemoval(aClientThreadId);
		}
	}

TInt CVwsEventQueue::AddToTail(CVwsEvent*& aEvent)
	{
	switch (aEvent->Type())
		{
		case CVwsEvent::ENormal:
			return DoAddToTail(aEvent);
		case CVwsEvent::ERejectPairs:
			return AddToTailIfNotPair(aEvent);
		default:
			ASSERT(EFalse);
			return KErrGeneral;
		}
	}

TInt CVwsEventQueue::RemoveHead()
	{
	if (iQueueSize>0)
		{
		iQueue[iQueueStart]=NULL;
		iQueueStart=(iQueueStart+1)%KQueueSize;
		iQueueSize--;
		return KErrNone;
		}

	return KErrUnderflow;
	}

TInt CVwsEventQueue::DoAddToTail(CVwsEvent* aEvent)
	{
	if (iQueueSize<KQueueSize)
		{
		iQueue[iQueueEnd]=aEvent;
		iQueueEnd=(iQueueEnd+1)%KQueueSize;
		iQueueSize++;
		aEvent->HandleAddedToQueue();
		LOG4(CVwsLog::ELoud,_L("Added event to tail of %S [queue size %d]"),&iQueueName,iQueueSize);
		return KErrNone;
		}

	return KErrOverflow;
	}

TInt CVwsEventQueue::AddToTailIfNotPair(CVwsEvent*& aEvent)
	{
	if (iQueueSize==0)
		{
		// Nothing in queue, so safe to add.
		return DoAddToTail(aEvent);
		}

	// Check for type match in queue.
	if (iQueueSize>2)
		{
		if ((Tail()->Type()==aEvent->Type()) && (At(iQueueSize-2)->Type()==aEvent->Type()) && (At(iQueueSize-3)->Type()==aEvent->Type()))
			{
			LOG3(CVwsLog::ENormal,_L("Discarded event - one already in %S"),&iQueueName);
			delete aEvent;
			aEvent=NULL;
			return KErrNone;
			}
		}

	// No match, do add to tail.
	return DoAddToTail(aEvent);
	}

CVwsEvent* CVwsEventQueue::At(TInt aIndex)
	{
	CVwsEvent* event=NULL;

	if (aIndex<iQueueSize)
		{
		event=iQueue[(iQueueStart+aIndex)%KQueueSize];
		}

	return event;
	}

void CVwsEventQueue::KickStart()
	{
	//Attempt to restart stalled q
	DeleteHead();
	iState=EEmpty;
	TInt err = KErrGeneral;
	while(iQueueSize>0 && err)
		{
		iState=EProcessing;
		TRAP(err, Head()->ProcessEventL());
		if (err)
			{
			DeleteHead();
			iState=EEmpty;
			}
		}
	}

//
// CVwsEvent.
//

CVwsEvent::CVwsEvent(TType aType,CVwsEventQueue& aQueue)
	:iType(aType),iQueue(aQueue)
	{
	}

CVwsEvent::~CVwsEvent()
	{
	}

CVwsEvent::TType CVwsEvent::Type()
	{
	return iType;
	}

void CVwsEvent::ReportEventProcessed()
	{
	iQueue.HandleEventProcessed();
	}

void CVwsEvent::HandleAddedToQueue()
	{
	}

void CVwsEvent::HandleLastOnQueue()
	{
	}

void CVwsEvent::HandleSessionRemoval(const TThreadId& /*aClientThreadId*/)
	{
	}

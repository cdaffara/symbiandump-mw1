/*
* Copyright (c) 2006-2010 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  COomClientRequestQueue.cpp.
*
*/



#include "oomclientrequestqueue.h"
#include "OomTraces.h"
#include "oomsubscribehelper.h"
#include "oompanic.h"
#include "oommemorymonitor.h"
#include <UikonInternalPSKeys.h>

const TInt KOomWatchDogStatusIdle = -1;
const TInt KClientTimeToFreeMemory = 500000; //microseconds

COomClientRequestQueue::COomClientRequestQueue(CMemoryMonitor& aMonitor)
    :iQueue(_FOFF(TClientRequest,iLink)),
    iMonitor(aMonitor)
    {
    FUNC_LOG;
    }

COomClientRequestQueue::~COomClientRequestQueue()
    {
    FUNC_LOG;
    
    if (iWatchdogStatusSubscriber)
        {
        iWatchdogStatusSubscriber->StopSubscribe();
        }
    iWatchdogStatusProperty.Close();
    delete iWatchdogStatusSubscriber;

    if (iClientRequestTimer)
        {
        iClientRequestTimer->Cancel();
        }
    delete iClientRequestTimer;
    
    TClientRequest* request;
    TSglQueIter<TClientRequest> iter(iQueue);
    iter.SetToFirst(); 
    while (iter)
        {
        request = iter++;
        iQueue.Remove(*request);
        delete request;
        };
    }

COomClientRequestQueue* COomClientRequestQueue::NewL(CMemoryMonitor& aMonitor)
    {
    FUNC_LOG;
    
    COomClientRequestQueue* self = new (ELeave) COomClientRequestQueue(aMonitor);
    CleanupStack::PushL(self);
    self->ConstructL();
    CleanupStack::Pop(self); 
    return self;
    }

void COomClientRequestQueue::ConstructL()
    {
    FUNC_LOG;
    
    TInt err = iWatchdogStatusProperty.Attach(KPSUidUikon, KUikOOMWatchdogStatus);

    TRACES1("COomClientRequestQueue::ConstructL: KUikOOMWatchdogStatus err = %d", err);
    
    err = iWatchdogStatusProperty.Set(KOomWatchDogStatusIdle);
    
    iWatchdogStatusSubscriber = new (ELeave) CSubscribeHelper(TCallBack(WatchdogStatusStatusChanged, this), iWatchdogStatusProperty);
    iWatchdogStatusSubscriber->Subscribe();

    
    iClientRequestTimer = COomClientRequestTimer::NewL(*this);    
    }

void COomClientRequestQueue::RequestFreeMemoryL(const RMessage2& aMessage)
    {
    FUNC_LOG;
    
    TClientRequest* request = new (ELeave) TClientRequest(EClientServerRequestFreeMemory, aMessage);
    CleanupStack::PushL(request);
    AddClientRequestL(*request);
    CleanupStack::Pop(request);
    }

void COomClientRequestQueue::RequestOptionalRamL(const RMessage2& aMessage)
    {
    FUNC_LOG;
    
    TClientRequest* request = new (ELeave) TClientRequest(EClientServerRequestOptionalRam, aMessage);
    CleanupStack::PushL(request);
    AddClientRequestL(*request);
    CleanupStack::Pop(request);
    }

TInt COomClientRequestQueue::WatchdogStatusStatusChanged(TAny* aPtr)
    {
    FUNC_LOG;

    COomClientRequestQueue* self = static_cast<COomClientRequestQueue*>(aPtr);
    if (self)
        {
        self->HandleWatchdogStatusCallBack();
        }
    return KErrNone;
    }

void COomClientRequestQueue::HandleWatchdogStatusCallBack()
    {
    FUNC_LOG;

    // Someone has set the key to request some free memory.
    TInt memoryRequested = 0;
    iWatchdogStatusProperty.Get(memoryRequested);

    // Try to free the RAM.
    if (memoryRequested >= 1)
        {
        TClientRequest request = TClientRequest(EPublishAndSubscribe, memoryRequested);
        TRAP_IGNORE(AddClientRequestL(request));
        }
    // Set the key back to KOomWatchDogStatusIdle to indicate we're done.
    iWatchdogStatusProperty.Set(KOomWatchDogStatusIdle);
    }

// The new request is added to the queue, then we have the following conditions: 
// 1. A client request is currently being processed
// 2. The last client request completed less than KClientTimeToFreeMemory microseconds ago -> start the timer
// 3. The timer has already been started
// 4. none of the above -> process this request
void COomClientRequestQueue::AddClientRequestL(TClientRequest& request)
    {
    FUNC_LOG;
    
    iQueue.AddLast(request);
 
    if ( !iClientRequestActive && !iClientRequestTimer->IsActive() )
        {
        TTime now;
        now.UniversalTime();
        TInt64 interval64 = (now.MicroSecondsFrom(iLastClientCompletedTime)).Int64();
        TRACES3("COomClientRequestQueue::AddClientRequestL: now = %Ld, iLastClientCompletedTime = %Ld, interval64 = %Ld",
                now.Int64(), iLastClientCompletedTime.Int64(), interval64);
               
        if ( interval64 < 0)        
            {
            //If the system time is moved backwards we lose the information about when the last request was 
            //made, so we wait for KClientTimeToFreeMemory microseconds
            iClientRequestTimer->After(TTimeIntervalMicroSeconds32(KClientTimeToFreeMemory));            
            }
        else if ( interval64 < KClientTimeToFreeMemory)
            {            
            //The last completed client is given KClientTimeToFreeMemory microseconds to allocate the memory 
            //it requested
            iClientRequestTimer->After(TTimeIntervalMicroSeconds32(interval64));
            }
        else 
            {
            StartClientRequestL();
            }        
        }
    }

void COomClientRequestQueue::StartClientRequestL()
    {    
    FUNC_LOG;
    
    iClientRequestActive = ETrue;

    TClientRequest* request = iQueue.First();
    
    RThread clientThread;
    TInt err =  (request->iRequestFreeRamMessage).Client(clientThread);
    TBool dataPaged = EFalse;
    if(err == KErrNone)
        {
        RProcess processName;
        err = clientThread.Process(processName);
        if(err == KErrNone)
            {
            dataPaged = processName.DefaultDataPaged();
            processName.Close();
            }
        clientThread.Close();
        }
    else
        {
        OomMonitorPanic(KInvalidClientRequestType);
        }    
    
    switch (request->iClientRequestType)
        {
        case EClientServerRequestOptionalRam:
            {
            TInt pluginId = request->iRequestFreeRamMessage.Int2();
            iMonitor.FreeOptionalRamL(request->iBytesRequested, pluginId, dataPaged);
            break;
            }
        case EClientServerRequestFreeMemory:
            iMonitor.RequestFreeMemoryL(request->iBytesRequested, dataPaged);
            break;
        case EPublishAndSubscribe:
            iMonitor.RequestFreeMemoryPandSL(request->iBytesRequested);
            break;
        default:
            OomMonitorPanic(KInvalidClientRequestType);
            break;
        }
    }

CMemoryMonitor& COomClientRequestQueue::Monitor()
    {
    FUNC_LOG;
    
    return iMonitor;
    }

TClientRequest::TClientRequest(TActionTriggerType aClientRequestType, TInt aBytesRequested)    
    : iClientRequestType(aClientRequestType), iBytesRequested(aBytesRequested)   
    {
    FUNC_LOG;
    }

TClientRequest::TClientRequest(TActionTriggerType aClientRequestType, const RMessage2& aRequestFreeRam)    
    : iClientRequestType(aClientRequestType), iRequestFreeRamMessage(aRequestFreeRam)
    {
    FUNC_LOG;
    
    iBytesRequested = aRequestFreeRam.Int0();
    }

void COomClientRequestQueue::ActionsCompleted(TInt aBytesFree, TBool aMemoryGood)
    {
    FUNC_LOG;
    
    if (iClientRequestActive)
        {
#ifdef _DEBUG
        TSglQueIter<TClientRequest> iter(iQueue);
        iter.SetToFirst();
        TClientRequest* req;
        while (iter)
            {
            req = iter++;
            TActionTriggerType crt = req->iClientRequestType;
            TInt bytes = req->iBytesRequested;
            TRACES2("COomClientRequestQueue::ActionsCompleted Printing Queue: Type = %d, Bytes Requested = %d", crt, bytes);
            };
#endif 

        __ASSERT_DEBUG(!iQueue.IsEmpty(), OomMonitorPanic(KClientQueueNotEmpty));        
        __ASSERT_DEBUG(!iClientRequestTimer->IsActive(), OomMonitorPanic(KClientRequestTimerActive));        

        TClientRequest* request = iQueue.First();
        RMessage2 message;
        
        switch (request->iClientRequestType)
            {
            case EClientServerRequestOptionalRam:
                message = request->iRequestFreeRamMessage;
                if (!message.IsNull())
                    {
                    TInt memoryAvailable = aBytesFree - iMonitor.GoodRamThreshold();
                    TInt minimumNeeded = message.Int1();
                    if (memoryAvailable >= minimumNeeded)
                        {
                        message.Complete(memoryAvailable);
                        }
                    else
                        {
                        message.Complete(KErrNoMemory);
                        }
                    }
                break;
            case EClientServerRequestFreeMemory:
                message = request->iRequestFreeRamMessage;
                if (!message.IsNull())
                    {
                    // If memory available is greater than the requested RAM then complete with the amount of free memory, otherwise complete with KErrNoMemory
                    message.Complete(aMemoryGood ? KErrNone : KErrNoMemory);        
                    }
                break;
            case EPublishAndSubscribe:
                // No action required for P&S key
                break; 
            default:
                OomMonitorPanic(KInvalidClientRequestType);
                break;
            }
            
        iClientRequestActive = EFalse;
        iQueue.Remove(*request);
        delete request;

        iLastClientCompletedTime.UniversalTime();
        
        if (!iQueue.IsEmpty())
            {
            //We give the client KClientTimeToFreeMemory microseconds to free the memory it requested before 
            //processing the next request
            iClientRequestTimer->After(TTimeIntervalMicroSeconds32(KClientTimeToFreeMemory));
            }
        }    
    }

void COomClientRequestQueue::RequestTimerCallbackL()
    {
    FUNC_LOG;
    
    __ASSERT_DEBUG(!iQueue.IsEmpty(), OomMonitorPanic(KClientQueueNotEmpty));        
    
    StartClientRequestL();
    }

COomClientRequestTimer* COomClientRequestTimer::NewL(COomClientRequestQueue& aQueue)
    {
    FUNC_LOG;

    COomClientRequestTimer* self = new (ELeave) COomClientRequestTimer(aQueue);
    CleanupStack::PushL(self);
    self->ConstructL();
    CleanupStack::Pop(self);
    return self;
    }

COomClientRequestTimer::COomClientRequestTimer(COomClientRequestQueue& aQueue)
: CTimer(CActive::EPriorityStandard), iClientRequestQueue(aQueue)
    {
    FUNC_LOG;
    
    CActiveScheduler::Add(this);
    }


void COomClientRequestTimer::RunL()
    {
    FUNC_LOG;
    
    iClientRequestQueue.RequestTimerCallbackL();
    }




    
    

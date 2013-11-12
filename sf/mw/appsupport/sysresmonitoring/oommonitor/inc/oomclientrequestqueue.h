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


#ifndef COOMCLIENTREQUESTQUEUE_H
#define COOMCLIENTREQUESTQUEUE_H

#include <e32std.h>
#include <e32base.h>
#include <e32property.h>

#include "oommemorymonitor.h"

class COomClientRequestQueue;
class CMemoryMonitor;
class CSubscribeHelper;

class TClientRequest
    {
public:
    TClientRequest(TActionTriggerType aClientRequestType, TInt aBytesRequested);    
    TClientRequest(TActionTriggerType aClientRequestType, const RMessage2& aRequestFreeRam);    
public:    
    TActionTriggerType iClientRequestType;
    TInt iBytesRequested;
    RMessage2 iRequestFreeRamMessage;
    TSglQueLink iLink;
    };

/*
 *  If two client requests are queued, the COomClientRequestTimer is used to wait after the first request 
 *  has been completed, to allow the first client time to allocate the memory it requested.
 *
 *  @lib oommonitor.lib
 *  @since S60 v5.0
 */
NONSHARABLE_CLASS(COomClientRequestTimer) : public CTimer
    {
public:
    static COomClientRequestTimer* NewL(COomClientRequestQueue& aQueue);
private:
    COomClientRequestTimer(COomClientRequestQueue& aQueue);
    void RunL();    
    
private: //data
    COomClientRequestQueue& iClientRequestQueue;
    };

/**
 *  Queues client requests so that the actual RAM level of the device is in a state where it can be more 
 *  accurately determined whether more actions need to be run
 *
 *  @lib oommonitor.lib
 *  @since S60 v5.0 
 */
NONSHARABLE_CLASS(COomClientRequestQueue) : public CBase
    {
public:
 
    // Constructors and destructor

    /**
     * Destructor.
     */
    ~COomClientRequestQueue();

    /**
     * Two-phased constructor.
     */
    static COomClientRequestQueue* NewL(CMemoryMonitor& aMonitor);
   
    CMemoryMonitor& Monitor();
    
    void RequestFreeMemoryL(const RMessage2& aMessage);
    void RequestOptionalRamL(const RMessage2& aMessage);
    void ActionsCompleted(TInt aBytesFree, TBool aMemoryGood);
    void RequestTimerCallbackL();
    
private:

    /**
     * Constructor for performing 1st stage construction
     */
    COomClientRequestQueue(CMemoryMonitor& aMonitor);

    /**
     * EPOC default constructor for performing 2nd stage construction
     */
    void ConstructL();
    
    static TInt WatchdogStatusStatusChanged(TAny* aPtr);
    void HandleWatchdogStatusCallBack();
    void AddClientRequestL(TClientRequest& request);
    void StartClientRequestL();    
    
    // parameters for P&S watcher.

    /**
     * The handle to the P&S property that can be used to initiate OOM Monitor actions
     */    
    RProperty iWatchdogStatusProperty;
    
    /**
     * The active object which monitors the P&S property that can be used to initiate OOM Monitor actions
     * Own.       
     */    
    CSubscribeHelper* iWatchdogStatusSubscriber;
    
    TSglQue<TClientRequest> iQueue;
    
    TBool iClientRequestActive;
    
    CMemoryMonitor& iMonitor;

    COomClientRequestTimer* iClientRequestTimer;

    TTime iLastClientCompletedTime;
    };

#endif // COOMCLIENTREQUESTQUEUE_H

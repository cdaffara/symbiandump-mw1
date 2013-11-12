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
* Description:  Main classes for Out of Memory Monitor.
*
*/


#ifndef OOMMEMORYMONITOR_H
#define OOMMEMORYMONITOR_H

#include <e32property.h>
#include <e32svr.h>
#include <w32std.h>
#include "oomglobalconfig.h"
#include "oomwindowgrouplist.h"
#include "oommonitorsession.h"

enum TActionTriggerType
    {
    ERamRotation,
    EClientServerRequestOptionalRam,
    EClientServerRequestFreeMemory,
    EPublishAndSubscribe        
    };

// ---------------------------------------------------------
// CMemoryMonitor
// ---------------------------------------------------------
//
class COutOfMemoryWatcher;
class CSubscribeHelper;
class COomMonitorPlugin;
class CMemoryMonitorServer;
class CWservEventReceiver;
class COomActionList;
class COomLogger;
class COomConfig;
class COomClientRequestQueue;

/**
 *  The main manager class for Oom Monitor. It has the following responsibility: 
 *  
 *  - initiates building the static configuration at boot time and owns the object which holds this. 
 *  - owns and drives the action list which runs actions
 *  - owns the internal representation of the window group list and an active object to monitor window group changes
 *  - owns the public API via the server and client request queue.
 *  - has an active object which monitors low RAM events
 *
 *  @lib oommonitor.lib
 *  @since S60 v5.0
 */
NONSHARABLE_CLASS(CMemoryMonitor) : public CBase
    {
public:
    static CMemoryMonitor* NewL();
    ~CMemoryMonitor();

public: // event handlers
    void FreeMemThresholdCrossedL();
    void AppNotExiting(TInt aWgId);
    void StartFreeSomeRamL(TInt aFreeRamTarget, TInt aFreeSwapSpaceTarget);
    void StartFreeSomeRamL(TInt aFreeRamTarget, TInt aFreeSwapSpaceTarget, TInt aMaxPriority);
    void FreeOptionalRamL(TInt aBytesRequested, TInt aPluginId, TBool aDataPaged); // The ID of the plugin that will clear up the allocation, used to determine the priority of the allocation
    void HandleFocusedWgChangeL();
    static const COomGlobalConfig& GlobalConfig();
    void SetPriorityBusy(TInt aWgId);
    void SetPriorityNormal(TInt aWgId);
    void SetPriorityHigh(TInt aWgId);
    void ResetTargets();
    void RequestTimerCallbackL();
    void GetFreeMemory(TInt& aCurrentFreeMemory);
    void GetFreeSwapSpace(TInt& aCurrentFreeSwapSpace);
    TActionTriggerType ActionTrigger() const;
#ifdef CLIENT_REQUEST_QUEUE
    void ActionsCompleted(TInt aBytesFree, TBool aMemoryGood);
    TInt GoodRamThreshold() const;
    TInt LowRamThreshold() const;
#endif
    void RequestFreeMemoryL(TInt aBytesRequested, TBool aDataPaged);
    void RequestFreeMemoryPandSL(TInt aBytesRequested);
    
    /*
     * Sets the RProperty associated with KOomMemoryMonitorStatusPropertyKey
     * It checks if the value is going to be changed. 
     * If the new value is the same as the old one then the property is not updated
     * If the new value is different from the previously set one then the property is update
     * 
     * @param aNewValue one of TMemoryMonitorStatusPropertyValues 
     */
    void SetMemoryMonitorStatusProperty(const TMemoryMonitorStatusPropertyValues aValue);
   
private:
    CMemoryMonitor();
    void ConstructL();
#ifndef CLIENT_REQUEST_QUEUE
    static TInt WatchdogStatusStatusChanged(TAny* aPtr);
    void HandleWatchdogStatusCallBack();
    TBool FreeMemoryAboveThreshold(TInt& aCurrentFreeMemory);    void CloseNextApp();
#endif
    void RefreshThresholds();
    
public:
    // All members are owned
    // generally useful sessions
    RFs iFs;
    RWsSession iWs;

    TBool iDataPaged;
    
private: //data

    // parameters for OOM watcher.
    TInt iLowRamThreshold;
    TInt iGoodRamThreshold;
    TInt iLowSwapThreshold;
    TInt iGoodSwapThreshold;
    TInt iCurrentRamTarget;
    TInt iCurrentSwapTarget;
    
   
    
#ifdef CLIENT_REQUEST_QUEUE
    TInt iClientBytesRequested;
#endif        
    // event receivers
    
    /**
     * The active object which monitors the kernel change notifier for changes in the low and good thresholds
     * Own
     */    
    COutOfMemoryWatcher* iOOMWatcher;
    
    
    CWservEventReceiver* iWservEventReceiver;

#ifndef CLIENT_REQUEST_QUEUE
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
#endif
    
    /**
     * The Memory Monitor Server
     * Own.       
     */
    CMemoryMonitorServer* iServer;
    
#ifdef CLIENT_REQUEST_QUEUE
    COomClientRequestQueue* iQueue;
#endif
    
#ifdef _DEBUG    
    /**
     * Oom logging tool - samples free memory for profiling
     * Own.       
     */
    COomLogger* iLogger;
#endif
    
    /**
     * A list of window group Ids, with child window groups removed such that there is one Id per application
     * Own.       
     */    
    COomWindowGroupList* iOomWindowGroupList;
    
    /**
     * The object responsible for identifying the best actions to run, and running them 
     * Own.       
     */
    COomActionList* iOomActionList;
    
    /**
     * The entire Oom Monitor configuration
     * Own.       
     */
    COomConfig* iConfig;

    /**
     * The most recent value the memory monitor status property was set to 
     */
    TInt iLastMemoryMonitorStatusProperty;
    
    TActionTriggerType iActionTrigger;
    };

#endif /*OOMMEMORYMONITOR_H*/

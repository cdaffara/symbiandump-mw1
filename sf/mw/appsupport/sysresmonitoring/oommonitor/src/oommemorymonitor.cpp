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


#include <hal.h>
#include <u32hal.h>

#include <UikonInternalPSKeys.h>

#include "oommemorymonitor.h"
#include "oommonitorplugin.h"
#include "oomsubscribehelper.h"
#include "oomconfig.h"
#include "oommemorymonitorserver.h"
#include "oomconfigparser.h"
#include "oomactionlist.h"
#include "oomlog.h"
#include "OomTraces.h"
#include "oomoutofmemorywatcher.h"
#include "oomwserveventreceiver.h"
#include "oomconstants.hrh"
#include "oomrunpluginconfig.h"
#include "oomapplicationconfig.h"
#include "oomclientrequestqueue.h"

#ifndef CLIENT_REQUEST_QUEUE
const TInt KOomWatchDogStatusIdle = -1;
#endif

// ======================================================================
// class CMemoryMonitor
// ======================================================================

// ---------------------------------------------------------
//
// ---------------------------------------------------------
//
CMemoryMonitor* CMemoryMonitor::NewL()
    { // static
    FUNC_LOG;

    CMemoryMonitor* self = new(ELeave) CMemoryMonitor();
    CleanupStack::PushL(self);
    self->ConstructL();
    CleanupStack::Pop(self);
    return self;
    }

CMemoryMonitor::CMemoryMonitor()
    {
    FUNC_LOG;

    SetMemoryMonitorTls(this);
    }

// ---------------------------------------------------------
//
// ---------------------------------------------------------
//
CMemoryMonitor::~CMemoryMonitor()
    {
    FUNC_LOG;

#ifndef CLIENT_REQUEST_QUEUE
    if (iWatchdogStatusSubscriber)
        {
        iWatchdogStatusSubscriber->StopSubscribe();
        }
    iWatchdogStatusProperty.Close();
    delete iWatchdogStatusSubscriber;
#endif
    
    delete iServer;
    delete iWservEventReceiver;
    delete iOOMWatcher;
    iFs.Close();
    iWs.Close();
    
    delete iOomWindowGroupList;
    delete iOomActionList;
    delete iConfig;
#ifdef CLIENT_REQUEST_QUEUE
    delete iQueue;
#endif
    
#ifdef _DEBUG    
    delete iLogger;
#endif
    }

// ---------------------------------------------------------
//
// ---------------------------------------------------------
//
void CMemoryMonitor::ConstructL()
    {
    FUNC_LOG;

    User::LeaveIfError(iWs.Connect());
    
    iOomWindowGroupList = COomWindowGroupList::NewL(iWs);
    
    iConfig = COomConfig::NewL(); 
    
#ifdef CLIENT_REQUEST_QUEUE
    iQueue = COomClientRequestQueue::NewL(*this);
    
    iServer = CMemoryMonitorServer::NewL(*iQueue);
#else
    iServer = CMemoryMonitorServer::NewL(*this);    
#endif
    
   // Load up threshold & OOM app lists from resource.
    User::LeaveIfError(iFs.Connect());    
    
    COomConfigParser* oomConfigParser = new (ELeave) COomConfigParser(*iConfig, iFs);
    CleanupStack::PushL(oomConfigParser);
    oomConfigParser->ParseL();
    CleanupStack::PopAndDestroy(oomConfigParser);

    iOomActionList = COomActionList::NewL(*this, *iServer, iWs, *iConfig);
    
#ifdef _DEBUG    
    iLogger = COomLogger::NewL(iWs, iFs);
#endif
    
    // Get the thresholds based on the current foreground app and the config
    RefreshThresholds();
    
    _LIT_SECURITY_POLICY_S0(KOomMemoryMonitorPolicyWrite, KOomMemoryMonitorStatusPropertyCategory.iUid);
    _LIT_SECURITY_POLICY_PASS(KOomMemoryMonitorPolicyRead);

    // Define MemoryMonitorStatusProperty. set to "above treshhold".
    TInt err = RProperty::Define(KOomMemoryMonitorStatusPropertyCategory, KOomMemoryMonitorStatusPropertyKey, RProperty::EInt, KOomMemoryMonitorPolicyRead, KOomMemoryMonitorPolicyWrite);
    TRACES1("CMemoryMonitor::ConstructL: KOomMemoryMonitorStatusProperty: Define err = %d", err);
    
    err = RProperty::Set(KOomMemoryMonitorStatusPropertyCategory, KOomMemoryMonitorStatusPropertyKey, EAboveTreshHold);
    TRACES1("CMemoryMonitor::ConstructL:  KOomMemoryMonitorStatusProperty: Set err = %d", err);            

#ifndef CLIENT_REQUEST_QUEUE
    err = iWatchdogStatusProperty.Attach(KPSUidUikon, KUikOOMWatchdogStatus);
    
    TRACES1("CMemoryMonitor::ConstructL: KUikOOMWatchdogStatus err = %d", err);
        
    err = iWatchdogStatusProperty.Set(KOomWatchDogStatusIdle);
    
    iWatchdogStatusSubscriber = new (ELeave) CSubscribeHelper(TCallBack(WatchdogStatusStatusChanged, this), iWatchdogStatusProperty);
    iWatchdogStatusSubscriber->Subscribe();
#endif
    
    
    iOOMWatcher = COutOfMemoryWatcher::NewL(*this, iLowRamThreshold, iGoodRamThreshold, iConfig->GlobalConfig().iSwapUsageMonitored, iLowSwapThreshold, iGoodSwapThreshold);
    iOOMWatcher->Start();

    iWservEventReceiver = new(ELeave) CWservEventReceiver(*this, iWs);
    iWservEventReceiver->ConstructL();
    }

const COomGlobalConfig& CMemoryMonitor::GlobalConfig()
    {
    CMemoryMonitor* globalMemoryMonitor = static_cast<CMemoryMonitor*>(Dll::Tls());
    return globalMemoryMonitor->iConfig->GlobalConfig();
    }


// ---------------------------------------------------------
//
// ---------------------------------------------------------
//
void CMemoryMonitor::FreeMemThresholdCrossedL()
    {
    FUNC_LOG;

    iActionTrigger = ERamRotation;
    StartFreeSomeRamL(iGoodRamThreshold, iGoodSwapThreshold);
    }

void CMemoryMonitor::HandleFocusedWgChangeL()
    {
    FUNC_LOG;

    TInt oldGoodRamThreshold = iGoodRamThreshold;
    TInt oldLowRamThreshold = iLowRamThreshold;
    TInt oldGoodSwapThreshold = iGoodSwapThreshold;
    TInt oldLowSwapThreshold = iLowSwapThreshold;
        
    // Refresh the low and good memory thresholds as they may have changed due to the new foreground application
    RefreshThresholds();
       
    if ((oldGoodRamThreshold != iGoodRamThreshold)
            || (oldLowRamThreshold != iLowRamThreshold)
            || (oldGoodSwapThreshold != iGoodSwapThreshold)
            || (oldLowSwapThreshold != iLowSwapThreshold))
        // If the thresholds have changed then update the memory watched
        {
        iOOMWatcher->UpdateThresholds(iLowRamThreshold, iGoodRamThreshold, iLowSwapThreshold, iGoodSwapThreshold);
        }
    
    // If the available memory is less than the low memory threshold then free some RAM
    User::CompressAllHeaps();
    TInt currentFreeRam = 0;
    HAL::Get( HALData::EMemoryRAMFree, currentFreeRam );
	TInt currentFreeSwap = 0;
	if (iConfig->GlobalConfig().iSwapUsageMonitored)
		{
        SVMSwapInfo swapInfo;
		UserSvr::HalFunction(EHalGroupVM, EVMHalGetSwapInfo, &swapInfo, 0);
		currentFreeSwap = swapInfo.iSwapFree;
		}
    
    if ((currentFreeRam < iLowRamThreshold) ||
		(iConfig->GlobalConfig().iSwapUsageMonitored && (currentFreeSwap < iLowSwapThreshold)))
        {
        iActionTrigger = ERamRotation;
        StartFreeSomeRamL(iGoodRamThreshold, iGoodSwapThreshold);
        }
    }

void CMemoryMonitor::StartFreeSomeRamL(TInt aFreeRamTarget, TInt aFreeSwapTarget)
    {
    StartFreeSomeRamL(aFreeRamTarget, aFreeSwapTarget, KOomPriorityInfinate - 1);
    }

void CMemoryMonitor::StartFreeSomeRamL(TInt aFreeRamTarget, TInt aFreeSwapTarget, TInt aMaxPriority) // The maximum priority of action to run
    {
    FUNC_LOG;

    TRACES4("MemoryMonitor::StartFreeSomeRamL: aFreeRamTarget = %d, iCurrentRamTarget = %d, aFreeSwapSpaceTarget = %d, iCurrentSwapTarget = %d", aFreeRamTarget, iCurrentRamTarget, aFreeSwapTarget, iCurrentSwapTarget);
    
    // Update the target if new target is higher. If the target is lower than the current target and memory 
    // is currently being freed then we do not want to reduce the amount of memory this operation frees.
    if (aFreeRamTarget > iCurrentRamTarget)
        {
        iCurrentRamTarget = aFreeRamTarget;
        }
    
    if (aFreeSwapTarget > iCurrentSwapTarget)
        {
        iCurrentSwapTarget = aFreeSwapTarget;
        }

    // check if there is enough free memory already.
    TInt freeMemory = 0;
    GetFreeMemory(freeMemory);
    TInt freeSwap = 0;
    if (iConfig->GlobalConfig().iSwapUsageMonitored)
        {
        GetFreeSwapSpace(freeSwap);
        }

    TRACES2("MemoryMonitor::StartFreeSomeRamL, freeMemory = %d, freeSwap = %d", freeMemory, freeSwap);
    
    if ((freeMemory >= iCurrentRamTarget) &&
        ((!iConfig->GlobalConfig().iSwapUsageMonitored) || (freeSwap >= iCurrentSwapTarget)))
        {
        if (iLastMemoryMonitorStatusProperty != EFreeingMemory)
            {
        ResetTargets();
        iOomActionList->SwitchOffPlugins();
        SetMemoryMonitorStatusProperty(EAboveTreshHold);
#ifdef CLIENT_REQUEST_QUEUE        
        iQueue->ActionsCompleted(freeMemory, ETrue);
#else        
        iServer->CloseAppsFinished(freeMemory, ETrue);
#endif        
            }
        return;
        }

#ifdef _DEBUG    
    iLogger->StartL();
#endif
	
    // Build the list of memory freeing actions
    iOomActionList->BuildActionListL(*iOomWindowGroupList, *iConfig);
    
	iOomActionList->SetCurrentTargets(iCurrentRamTarget, iCurrentSwapTarget);
    
    // Run the memory freeing actions
    iOomActionList->FreeMemory(aMaxPriority);
    }

void CMemoryMonitor::RequestFreeMemoryPandSL(TInt aBytesRequested)
    {
    FUNC_LOG;
    
    iActionTrigger = EPublishAndSubscribe;
    StartFreeSomeRamL(aBytesRequested + iLowRamThreshold, iLowSwapThreshold);
    }

void CMemoryMonitor::RequestFreeMemoryL(TInt aBytesRequested, TBool aDataPaged)
    {
    FUNC_LOG;
    
    iActionTrigger = EClientServerRequestFreeMemory;
    iDataPaged = aDataPaged;
    StartFreeSomeRamL(iLowRamThreshold, aBytesRequested + iLowSwapThreshold);
    }

void CMemoryMonitor::FreeOptionalRamL(TInt aBytesRequested, TInt aPluginId, TBool aDataPaged) // The ID of the plugin that will clear up the allocation, used to determine the priority of the allocation
    {
    FUNC_LOG;
    
    iActionTrigger = EClientServerRequestOptionalRam;

    iDataPaged = aDataPaged;
       
    // Calculate the priority of the allocation (the priority of the plugin that will clear it up - 1)
    TInt priorityOfAllocation = iConfig->GetPluginConfig(aPluginId).CalculatePluginPriority(*iOomWindowGroupList) - 1;
   
    StartFreeSomeRamL(aBytesRequested + iGoodRamThreshold, iLowSwapThreshold, priorityOfAllocation);
    }

void CMemoryMonitor::GetFreeMemory(TInt& aCurrentFreeMemory)
    {
    FUNC_LOG;

    // may cause some extra load but allows more precise action
    User::CompressAllHeaps();

    HAL::Get( HALData::EMemoryRAMFree, aCurrentFreeMemory );

    TRACES1("CMemoryMonitor::GetFreeMemory: Free RAM now %d", aCurrentFreeMemory);
    }

void CMemoryMonitor::GetFreeSwapSpace(TInt& aCurrentFreeSwapSpace)
    {
    FUNC_LOG;
    
    SVMSwapInfo swapInfo;
    UserSvr::HalFunction(EHalGroupVM, EVMHalGetSwapInfo, &swapInfo, 0);
    aCurrentFreeSwapSpace = swapInfo.iSwapFree;
        
    TRACES1("CMemoryMonitor::GetFreeSwapSpace: Free swap space now %d", aCurrentFreeSwapSpace);
    }

#ifndef CLIENT_REQUEST_QUEUE 
TInt CMemoryMonitor::WatchdogStatusStatusChanged(TAny* aPtr)
    {
    FUNC_LOG;

    CMemoryMonitor* self = STATIC_CAST(CMemoryMonitor*,aPtr);
    if (self)
        self->HandleWatchdogStatusCallBack();
    return KErrNone;
    }

// ---------------------------------------------------------
//
// ---------------------------------------------------------
//
void CMemoryMonitor::HandleWatchdogStatusCallBack()
    {
    FUNC_LOG;

    // Someone has set the key to request some free memory.
    TInt memoryRequested = 0;
    iWatchdogStatusProperty.Get(memoryRequested);

    // Try to free the RAM.
    if (memoryRequested >= 1)
        {
        iOOMWatcher->Cancel();              // Pause memory notifys.
        TRAP_IGNORE(RequestFreeMemoryPandSL(memoryRequested + iLowThreshold)); // This call could take a few seconds to do its stuff.
        iOOMWatcher->Start();               // Restarts memory monitoring.
        }
    // Set the key back to KOomWatchDogStatusIdle to indicate we're done.
    iWatchdogStatusProperty.Set(KOomWatchDogStatusIdle);
    }
#endif //CLIENT_REQUEST_QUEUE

void CMemoryMonitor::AppNotExiting(TInt aWgId)
    {
    FUNC_LOG;

    iOomActionList->AppNotExiting(aWgId);
    }


void CMemoryMonitor::RefreshThresholds()
    {
    FUNC_LOG;

    iOomWindowGroupList->Refresh();
    
    // Calculate the desired good threshold, this could be the globally configured value...
    iGoodRamThreshold = CMemoryMonitor::GlobalConfig().iGoodRamThreshold;
    iLowRamThreshold = CMemoryMonitor::GlobalConfig().iLowRamThreshold;
    iGoodSwapThreshold = CMemoryMonitor::GlobalConfig().iGoodSwapThreshold;
    iLowSwapThreshold = CMemoryMonitor::GlobalConfig().iLowSwapThreshold;
    TRACES4("CMemoryMonitor::RefreshThresholds: Global Good Ram Threshold = %d, Global Low Ram Threshold = %d, Global Good Swap Threshold = %d, Global Low Swap Threshold = %d", iGoodRamThreshold, iLowRamThreshold, iGoodSwapThreshold, iLowSwapThreshold);

#ifdef _DEBUG
    TRACES("CMemoryMonitor::RefreshThresholds: Dumping Window Group List");
    TInt wgIndex = iOomWindowGroupList->Count() - 1;
    while (wgIndex >= 0)        
        {
        TInt32 appId = iOomWindowGroupList->AppId(wgIndex, ETrue);    
        TInt wgId = iOomWindowGroupList->WgId(wgIndex).iId;
        TInt parentId = iOomWindowGroupList->WgId(wgIndex).iId;        
        TRACES4("CMemoryMonitor::RefreshThresholds: wgIndex=%d, oom uid=%x, wgId(child)=%d, parentId=%d", wgIndex, appId, wgId, parentId);    
        wgIndex--;
        }
#endif
    
    // The global value can be overridden by an app specific value
    // Find the application config entry for the foreground application
    if (iOomWindowGroupList->Count())
        {
        TUint foregroundAppId = iOomWindowGroupList->AppId(0, ETrue);
        TUid foregroundAppUid = TUid::Uid(foregroundAppId);
        
        if ( (foregroundAppUid == KUidPenInputServer || foregroundAppUid == KUidFastSwap) &&
             iOomWindowGroupList->Count() > 1 )
            {
            // pen input server puts itself to the foreground when the web browser is active
            // fast swap should not reset the thresholds for the app behind it
            foregroundAppId = iOomWindowGroupList->AppId(1, ETrue);
            }

        // If this application configuration overrides the good_ram_threshold then set it
        if (iConfig->GetApplicationConfig(foregroundAppId).iGoodRamThreshold != KOomThresholdUnset)
            {
            iGoodRamThreshold = iConfig->GetApplicationConfig(foregroundAppId).iGoodRamThreshold;
            TRACES2("CMemoryMonitor::RefreshThresholds: For foreground app %x, Good Ram Threshold = %d", foregroundAppId, iGoodRamThreshold);
            }
        // If this application configuration overrides the low_ram_threshold then set it
        if (iConfig->GetApplicationConfig(foregroundAppId).iLowRamThreshold != KOomThresholdUnset)
            {
            iLowRamThreshold = iConfig->GetApplicationConfig(foregroundAppId).iLowRamThreshold;
            TRACES2("CMemoryMonitor::RefreshThresholds: For foreground app %x, Low Ram Threshold = %d", foregroundAppId, iLowRamThreshold);
            }

        if (iConfig->GetApplicationConfig(foregroundAppId).iGoodSwapThreshold != KOomThresholdUnset)
            {
            iGoodSwapThreshold = iConfig->GetApplicationConfig(foregroundAppId).iGoodSwapThreshold;
            TRACES2("CMemoryMonitor::RefreshThresholds: For foreground app %x, Good Swap Threshold = %d", foregroundAppId, iGoodSwapThreshold);
            }
        // If this application configuration overrides the low_swap_threshold then set it
        if (iConfig->GetApplicationConfig(foregroundAppId).iLowSwapThreshold != KOomThresholdUnset)
            {
            iLowSwapThreshold = iConfig->GetApplicationConfig(foregroundAppId).iLowSwapThreshold;
            TRACES2("CMemoryMonitor::RefreshThresholds: For foreground app %x, Low Swap Threshold = %d", foregroundAppId, iLowSwapThreshold);
            }
        }
    }

// SetMemoryMonitorStatusProperty - updates the property value only if it has changed
void CMemoryMonitor::SetMemoryMonitorStatusProperty(const TMemoryMonitorStatusPropertyValues aValue)
    {
    if (iLastMemoryMonitorStatusProperty != aValue)
        {
        TInt err = RProperty::Set(KOomMemoryMonitorStatusPropertyCategory, KOomMemoryMonitorStatusPropertyKey, aValue);
        TRACES1("CMemoryMonitor::SetMemoryMonitorStatusProperty: err = %d", err);
        iLastMemoryMonitorStatusProperty = aValue;
        }
    }

void CMemoryMonitor::ResetTargets()
    {
    FUNC_LOG;

    //we reset the target when a memory free operation completes, to deal with the case 
    //where the operation was initiated with a target larger than the current good threshold
    iCurrentRamTarget = iGoodRamThreshold;
    iCurrentSwapTarget = iGoodSwapThreshold;
    iOomActionList->SetCurrentTargets(iCurrentRamTarget, iCurrentSwapTarget);
    }

void CMemoryMonitor::SetPriorityBusy(TInt aWgId)
    {
    FUNC_LOG;

    iOomWindowGroupList->SetPriorityBusy(aWgId);
    }

void CMemoryMonitor::SetPriorityNormal(TInt aWgId)
    {
    FUNC_LOG;

    iOomWindowGroupList->SetPriorityNormal(aWgId);
    }

void CMemoryMonitor::SetPriorityHigh(TInt aWgId)
    {
    iOomWindowGroupList->SetPriorityHigh(aWgId);
    }

TActionTriggerType CMemoryMonitor::ActionTrigger() const
    {
    return iActionTrigger;
    }

#ifdef CLIENT_REQUEST_QUEUE 
TInt CMemoryMonitor::GoodRamThreshold() const
    {
    return iGoodRamThreshold;
    }

TInt CMemoryMonitor::LowRamThreshold() const
    {
    return iLowRamThreshold;
    }

void CMemoryMonitor::ActionsCompleted(TInt aBytesFree, TBool aMemoryGood)
    {
    iQueue->ActionsCompleted(aBytesFree, aMemoryGood);
    }
#endif //CLIENT_REQUEST_QUEUE

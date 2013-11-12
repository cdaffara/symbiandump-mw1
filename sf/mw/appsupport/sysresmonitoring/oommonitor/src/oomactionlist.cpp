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
* Description:  Classes for executing OOM actions (e.g. closing applications and running plugins).
*
*/



#include <hal.h>
#include <u32hal.h>
#include <apgwgnam.h>

#include "oommonitorplugin.h"
#include "oommonitorplugin.hrh"
#include "oomactionlist.h"
#include "oomwindowgrouplist.h"
#include "oompanic.h"
#include "OomTraces.h"
#include "oomconstants.hrh"
#include "oommemorymonitor.h"
#include "oommemorymonitorserver.h"
#include "oomrunplugin.h"
#include "oomcloseapp.h"
#include "oomconfig.h"
#include "oomactionref.h"
#include "oomapplicationconfig.h"
#include "oomcloseappconfig.h"
#include "oomrunpluginconfig.h"

const TUid  KUidMatrixMenuApp = { 0x101F4CD2 };

template <class T>
COomPluginList<T>::COomPluginList()
    {
    FUNC_LOG;
    }

template <class T>
COomPluginList<T>::~COomPluginList()
    {
    FUNC_LOG;
    
    TInt count = iPlugins.Count();
    for (TInt ii=0; ii<count; ii++)
        {
        TPlugin& plugin = iPlugins[ii];
        if (plugin.iImpl)    // only if successfully added
            REComSession::DestroyedImplementation(plugin.iDtorUid);
        }
    iPlugins.Close();
    }

template <class T>
void COomPluginList<T>::ConstructL(TInt aInterfaceUid)
    {
    FUNC_LOG;
    
    RImplInfoPtrArray implArray;
    CleanupClosePushL(implArray);
    REComSession::ListImplementationsL(TUid::Uid(aInterfaceUid), implArray);

    TInt count = implArray.Count();
    iPlugins.ReserveL(count);

    for (TInt ii=0; ii<count; ii++)
        {
        iPlugins.AppendL(TPlugin());
        TPlugin& plugin = iPlugins[ii];
        TUid uid(implArray[ii]->ImplementationUid());
        plugin.iImpl = static_cast<T*>(REComSession::CreateImplementationL(uid, plugin.iDtorUid, NULL));
        plugin.iUid = uid.iUid;
        }
    
    CleanupStack::PopAndDestroy(&implArray);
    }

template <class T>
COomPluginList<T>::TPlugin::TPlugin()
: iImpl(0)
    {
    FUNC_LOG;
   }

template <class T>
COomPluginList<T>* COomPluginList<T>::NewL(TInt aInterfaceUid)
    {
    FUNC_LOG;
    
    COomPluginList* self = new (ELeave) COomPluginList();
    CleanupStack::PushL(self);
    self->ConstructL(aInterfaceUid);
    CleanupStack::Pop(self);
    return self;
    }
    
COomActionList* COomActionList::NewL(CMemoryMonitor& aMonitor, CMemoryMonitorServer& aServer, RWsSession& aWs, COomConfig& aConfig)
    {
    FUNC_LOG;
    
    COomActionList* self = new (ELeave) COomActionList(aMonitor, aServer, aWs);
    CleanupStack::PushL(self);
    self->ConstructL(aConfig);
    CleanupStack::Pop(self);
    return self;
    }

COomActionList::~COomActionList()
    {
    FUNC_LOG;
    
    iCloseAppActions.ResetAndDestroy(); 
    iCloseAppActions.Close();
    iRunPluginActions.ResetAndDestroy(); 
    iRunPluginActions.Close();

    iActionRefs.Close();
    
    delete iPluginList;
    delete iPluginListV2;
    }



// Creates a list of actions based on the contents of the config and the current window group list
// Any old actions will be replaced.
// This function may leave, however enough memory should be reserved for this process so that at least
// some actions have been created for freeing memory, these can then be run by calling FreeMemory.
// Note that this function will only leave in extreme circumstances, in normal usage we should have
// enough reserved memory to build the complete action list.
void COomActionList::BuildActionListL(COomWindowGroupList& aWindowGroupList, COomConfig& aConfig)
    {
    FUNC_LOG;
    
    if (iFreeingMemory)
        {
        TRACES("COomActionList::BuildActionListL Memory is currently being freed, do not build action list");
        return;
        }

    iActionRefs.Reset();
    iCurrentActionIndex = 0;
    
    aWindowGroupList.RefreshL();
    
    TInt actionsIndex = 0;
    
    // we rely on the two pluginlists not having changed since construction
    BuildPluginActionsL(*iPluginList, aWindowGroupList, aConfig, actionsIndex);
    BuildPluginActionsL(*iPluginListV2, aWindowGroupList, aConfig, actionsIndex);
    
    // Go through each item in the wglist, create an app close action for this application
    TUint wgIndex = aWindowGroupList.Count() - 1;
    
    // Fix for when fast swap has focus, or pen input server has put itself into the foreground:
    // the wg at index 1 should be considered as the foreground app.
    // stopIndex is calculated to take this into account. 
    TUint stopIndex = 0;
    TUid foregroundUid = TUid::Uid(aWindowGroupList.AppId(0));
    if ( KUidFastSwap == foregroundUid || KUidPenInputServer == foregroundUid)
        {
        stopIndex = 1;
        }
    
    while (wgIndex > stopIndex) // Don't go down to entry stopIndex because this is the foreground window
        {
        COomCloseAppConfig* appCloseConfig = NULL;

        CApaWindowGroupName* wgName = aWindowGroupList.WgName();
        __ASSERT_DEBUG(wgName, OomMonitorPanic(KInvalidWgName));    
        
        // Get the app ID for the wglist item
        // This sets the window group name
        TInt32 appId = aWindowGroupList.AppId(wgIndex);    

        if ( !appId || wgName->IsSystem() || wgName->Hidden() )
            {
            //If the UID is NULL at this point, we assume the process is not an application
            //and therefore is not a suitable candidate for closure. 
            //We also do not close system or hidden apps.
      
            //Matrix Menu is temporarily hardcoded here as a system app that can be closed
            TUid appUid = TUid::Uid(aWindowGroupList.AppId(wgIndex, ETrue));
            if ( KUidMatrixMenuApp == appUid)
                {   
                TRACES2("BuildActionListL: System App is Matrix Menu; UID = %x, wgIndex = %d", aWindowGroupList.AppId(wgIndex, ETrue),wgIndex);
                // Find the app close config for Menu App
                appCloseConfig = aConfig.GetApplicationConfig(appId).GetAppCloseConfig();
                }
            else
                {
                TRACES2("BuildActionListL: Not adding process to action list; UID = %x, wgIndex = %d", aWindowGroupList.AppId(wgIndex, ETrue),wgIndex);                
                TRACES2("BuildActionListL: IsSystem = %d, Hidden = %d", wgName->IsSystem() ? 1 : 0, wgName->Hidden() ? 1 : 0);                
                wgIndex--;
                continue;
                }
            }
        if (aWindowGroupList.IsBusy(wgIndex) || wgName->IsBusy())
            // If the application has been flagged as busy then look up the configuration for busy apps in the config file
            {
            // Find the app close config for this app ID
            appCloseConfig = aConfig.GetApplicationConfig(KOomBusyAppId).GetAppCloseConfig();
            }
        else if (aWindowGroupList.IsDynamicHighPriority(wgIndex))
            // If the application has been flagged as busy then look up the configuration for busy apps in the config file
            {
            // Find the app close config for this app ID
            appCloseConfig = aConfig.GetApplicationConfig(KOomHighPriorityAppId).GetAppCloseConfig();
            }
        else
            // If the application hasn't been flagged as busy or high priority then look up the priority according to the config
            {            
            // Find the app close config for this app ID
            appCloseConfig = aConfig.GetApplicationConfig(appId).GetAppCloseConfig();        
            }

        //If the app close config is NULL it is because there is an app specific threshold for this 
        //app but no specific close config. Use the default app close config
        if (!appCloseConfig)
            {
            appCloseConfig = aConfig.GetApplicationConfig(KOomDefaultAppId).GetAppCloseConfig();                    
            }
        
        TUint priority = appCloseConfig->CalculateCloseAppPriority(aWindowGroupList, wgIndex);
        TInt wgId = aWindowGroupList.WgId(wgIndex).iId;
        TOomSyncMode syncMode = appCloseConfig->iSyncMode;
        TInt ramEstimate = appCloseConfig->iRamEstimate;
        TActionRef ref = TActionRef(TActionRef::EAppClose, priority, syncMode, ramEstimate, wgId, wgIndex);

        //AppClose Actions should always have a unique prioirity determined by the application's z order.
        TInt err = iActionRefs.InsertInOrder(ref, ComparePriorities);
        if ((err != KErrNone) && (err != KErrAlreadyExists))
            {
            User::Leave(err);
            }
        TRACES3("BuildActionListL: Adding app to action list, Uid = %x, wgId = %d, wgIndex = %d", appId, wgId, wgIndex);                

        wgIndex--;
        }
    
    TRACES1("BuildActionListL: Action list built with %d items",iActionRefs.Count());    
    }

template <class T>
void COomActionList::BuildPluginActionsL(COomPluginList<T>& aPluginList, COomWindowGroupList& aWindowGroupList, COomConfig& aConfig, TInt& aActionsIndex)
    {
    TInt pluginIndex = aPluginList.Count();

    while (pluginIndex--)
        {
        // Get the config for this plugin
        COomRunPluginConfig& pluginConfig = aConfig.GetPluginConfig(aPluginList.Uid(pluginIndex));
        
        TActionRef::TActionType actionType;
        
        if (pluginConfig.PluginType() == EOomAppPlugin)
            {
            if (pluginConfig.CallIfTargetAppNotRunning() == EFalse)
                {
                //loop through wg group and if find can't find the target app, don't add this plugin
                TInt index = aWindowGroupList.Count() - 1;
                TUint targetApp = pluginConfig.TargetApp();
                TBool targetAppFound = EFalse;
                while (index >= 0)
                    {
                    if (aWindowGroupList.AppId(index) == targetApp)
                        {
                        targetAppFound = ETrue;
                        break;
                        }
                    index--;
                    }
                if (targetAppFound == EFalse)
                    {
                    aActionsIndex++;
                    continue;
                    }
                }
            actionType = TActionRef::EAppPlugin;
            }
        else
            {
            actionType = TActionRef::ESystemPlugin;
            }

        TInt priority = pluginConfig.CalculatePluginPriority(aWindowGroupList);
        TOomSyncMode syncMode = pluginConfig.iSyncMode;
        TInt ramEstimate = pluginConfig.iRamEstimate;
        
        __ASSERT_DEBUG(aActionsIndex < iRunPluginActions.Count(), OomMonitorPanic(KPluginArrayIndexOutOfBounds));    
        TActionRef ref = TActionRef(actionType, priority, syncMode, ramEstimate, *(iRunPluginActions[aActionsIndex]), aWindowGroupList.GetIndexFromAppId(pluginConfig.TargetApp()));

        //It is valid to have plugins with equal priority
        User::LeaveIfError(iActionRefs.InsertInOrderAllowRepeats(ref, ComparePriorities));
        
        aActionsIndex++;
        }
    }


// Execute the OOM actions according to their priority
// Run batches of OOM actions according to their sync mode
void COomActionList::FreeMemory(TInt aMaxPriority)
    {
    FUNC_LOG;
    if (iFreeingMemory)
        {
        TRACES("COomActionList::FreeMemory Memory is currently being freed, do not start any more actions");
        return;
        }
    
    iMaxPriority = aMaxPriority;
        
    TBool memoryFreeingActionRun = EFalse;
    
    // Get the configured maximum number of applications that can be closed at one time
    const COomGlobalConfig& globalConfig = CMemoryMonitor::GlobalConfig();
    TInt maxBatchSize = globalConfig.iMaxCloseAppBatch;
    TInt numberOfRunningActions = 0;
    
    TInt freeRamEstimate = 0; // The amount of free memory we expect to be free after the currently initiated operations
    HAL::Get(HALData::EMemoryRAMFree, freeRamEstimate);
    TUint64 freeSwapEstimate = 0;
    if (iSwapUsageMonitored)
        {
        SVMSwapInfo swapInfo;
        UserSvr::HalFunction(EHalGroupVM, EVMHalGetSwapInfo, &swapInfo, 0);
        freeSwapEstimate = swapInfo.iSwapFree;
        }
        
    while (iCurrentActionIndex < iActionRefs.Count() 
            && iActionRefs[iCurrentActionIndex].Priority() <= aMaxPriority)
        {
        TActionRef ref = iActionRefs[iCurrentActionIndex];
        COomAction* action = NULL;
        if (ref.Type() == TActionRef::EAppClose)
            {
            action = iCloseAppActions[numberOfRunningActions];
            static_cast<COomCloseApp*>(action)->Reconfigure(ref);
            }
        else 
            {
            action = &(ref.RunPlugin());
            }
        
        iFreeingMemory = ETrue;
        TRACES2("COomActionList::FreeMemory: Running action %d which has priority %d", iCurrentActionIndex,ref.Priority());
        
        iMonitor.SetMemoryMonitorStatusProperty(EFreeingMemory);
        
        // At the moment the actions don't make any difference between freeing
        // RAM and freeing swap. So we try to free the biggest of the two.
        // Until the plugins are updated to make a distinction between swap and RAM this is the best
        // we can do. For application close actions the amount to try to free is ignored anyway.
        TUint amountToTryToFree = 0;
		if (iCurrentRamTarget > freeRamEstimate)
			{
			amountToTryToFree = iCurrentRamTarget - freeRamEstimate;
			}
        if (iSwapUsageMonitored && (iCurrentSwapTarget > freeSwapEstimate) && ((iCurrentSwapTarget - freeSwapEstimate) > amountToTryToFree))
            {
            amountToTryToFree = iCurrentSwapTarget - freeSwapEstimate;
            }
        action->FreeMemory(amountToTryToFree, iMonitor.iDataPaged);
        memoryFreeingActionRun = ETrue;
                
        // Actions with EContinueIgnoreMaxBatchSize don't add to the tally of running actions
        if (ref.SyncMode() != EContinueIgnoreMaxBatchSize)
            numberOfRunningActions++;
        
        // Update our estimate of how much RAM we think we'll have after this operation
        freeRamEstimate += ref.RamEstimate();
         
        // Do we estimate that we are freeing enough memory (only applies if the sync mode is "esimtate" for this action)
        TBool estimatedEnoughMemoryFreed = EFalse;
        if ((ref.SyncMode() == EEstimate)
            && (freeRamEstimate >= iCurrentRamTarget))
            {
            estimatedEnoughMemoryFreed = ETrue;
            }
        
        if ((ref.SyncMode() == ECheckRam)
                || (numberOfRunningActions >= maxBatchSize)
                || estimatedEnoughMemoryFreed
                || globalConfig.ForceCheckAtPriority(iActionRefs[iCurrentActionIndex].Priority()))
            // If this actions requires a RAM check then wait for it to complete
            // Also force a check if we've reached the maximum number of concurrent operations
            // Also check if we estimate that we have already freed enough memory (assuming that the sync mode is "estimate"
            {
            // Return from the loop - we will be called back (in COomActionList::StateChanged()) when the running actions complete
            TRACES("COomActionList::FreeMemory: Exiting run action loop");
            return;
            }
        // ... otherwise continue running actions, don't wait for any existing ones to complete
        iCurrentActionIndex++;
        }

    
    if (!memoryFreeingActionRun)
        {
        // No usable memory freeing action has been found, so we give up
        TRACES("COomActionList::FreeMemory: No usable memory freeing action has been found");
        iMonitor.ResetTargets();
        TInt freeMemory = 0;
        iMonitor.GetFreeMemory(freeMemory);
        TInt freeSwap = 0;
        if (iSwapUsageMonitored)
            {
            iMonitor.GetFreeSwapSpace(freeSwap);
            }
        if ((freeMemory >= iCurrentRamTarget) &&
            ((!iSwapUsageMonitored) || (freeSwap >= iCurrentSwapTarget)))
            {
            SwitchOffPlugins();
            iMonitor.SetMemoryMonitorStatusProperty(EAboveTreshHold);
            }
        else
            {
            if (iMonitor.ActionTrigger() == EClientServerRequestOptionalRam)
                {
                //We always switch off the plugins after an optional RAM request
                SwitchOffPlugins();
                }
            iMonitor.SetMemoryMonitorStatusProperty(EBelowTreshHold);
            }
#ifdef CLIENT_REQUEST_QUEUE        
        iMonitor.ActionsCompleted(freeMemory, EFalse);
#else
        iServer.CloseAppsFinished(freeMemory, EFalse);
#endif
        }
    }

void COomActionList::SwitchOffPlugins()
    {
    FUNC_LOG;
    
    TInt actionRefIndex = iActionRefs.Count();
    
    // Go through each of the action references, if it's a plugin action then call MemoryGood on it
    // Note that this only results in a call to the plugin if FreeMemory has been called on this plugin since that last call to MemoryGood
    while (actionRefIndex--)
        {
        if ((iActionRefs[actionRefIndex].Type() == TActionRef::EAppPlugin)
                || (iActionRefs[actionRefIndex].Type() == TActionRef::ESystemPlugin))
            {
            iActionRefs[actionRefIndex].RunPlugin().MemoryGood();
            }
        }
    }

TInt COomActionList::ComparePriorities(const TActionRef& aPos1, const TActionRef& aPos2 )
    {
    FUNC_LOG;
    
    if (aPos1.Priority()< aPos2.Priority())
        {
        return -1;
        }
    if (aPos1.Priority() > aPos2.Priority())
        {
        return 1;
        }
    
    // If priorities are equal then we use hardcoded rules to determine which one is run...

    // All other actions are run in preference to application closures
	if ((aPos1.Type() != TActionRef::EAppClose)
			&& ((aPos2.Type() == TActionRef::EAppClose)))
		{
		return -1;
		}
	if ((aPos1.Type() == TActionRef::EAppClose)
			&& ((aPos2.Type() != TActionRef::EAppClose)))
		{
		return 1;
		}
	// If both actions are application closures then the Z order is used to determine which one to run (furthest back application will be closed first)
	if ((aPos1.Type() == TActionRef::EAppClose)
			&& ((aPos2.Type() == TActionRef::EAppClose)))
		{
		if (aPos1.WgIndex() < aPos2.WgIndex())
			{
			return 1;
			}
		else
			{
			return -1;
			}
		//Two Apps should not have equal window group indexes, we panic below if this is the case.
		}	
	
	// Application plugins will be run in preference to system plugins
	if ((aPos1.Type() == TActionRef::EAppPlugin)
			&& ((aPos2.Type() == TActionRef::ESystemPlugin)))
		{
		return -1;
		}	
	if ((aPos1.Type() == TActionRef::ESystemPlugin)
			&& ((aPos2.Type() == TActionRef::EAppPlugin)))
		{
		return 1;
		}
		
	// If both actions are application plugins then the Z order of the target app is used to determine which one to run (the one with the furthest back target app will be closed first)
	// If the target app is not running then the plugin is run in preference to target apps where the target app is running
	if ((aPos1.Type() == TActionRef::EAppPlugin)
			&& ((aPos2.Type() == TActionRef::EAppPlugin)))
		{
		// When the target app is not running then the plugin will be run ahead of plugins where the target app is running
		if ((aPos1.WgIndex() == KAppNotInWindowGroupList) && (aPos2.WgIndex() != KAppNotInWindowGroupList))
			{
			return -1;
			}
		if ((aPos1.WgIndex() != KAppNotInWindowGroupList) && (aPos2.WgIndex() == KAppNotInWindowGroupList))
			{
			return 1;
			}
		// If the target apps for both plugins are running then compare the Z order
		if ((aPos1.WgIndex() != KAppNotInWindowGroupList) && (aPos2.WgIndex() != KAppNotInWindowGroupList))
			{
			if (aPos1.WgIndex() < aPos2.WgIndex())
				{
				return 1;
				}
			else
				{
				return -1;
				}
			}
		// If the target app for neither plugin is running then they are of equal priority
		}
    //App Close Actions must have a unique priority.
    __ASSERT_DEBUG((aPos1.Type() != TActionRef::EAppClose) && (aPos2.Type() != TActionRef::EAppClose), OomMonitorPanic(KAppCloseActionEqualPriorities));    
    return 0;        
    }

void COomActionList::AppNotExiting(TInt aWgId)
    {
    FUNC_LOG;
    
    TInt index = iCloseAppActions.Count();
    while (index--)
        {
        COomCloseApp* action = iCloseAppActions[index];
        if ( (action->WgId() == aWgId) && (action->IsRunning()) )
            {
            TRACES1("COomCloseApp::AppNotExiting: App with window group id %d has not responded to the close event", aWgId);
            action->CloseAppEvent();            
            }
        }
    }

// From MOomActionObserver
// An action has changed state (possibly it has completed freeing memory)
void COomActionList::StateChanged()
    {
    FUNC_LOG;
    
    TBool allActionsComplete = ETrue;
    
    // Note that the actions themselves are responsible for timing out if necessary.
    TInt index = iCloseAppActions.Count();
    while ((index--) && (allActionsComplete))
        {
        if (iCloseAppActions[index]->IsRunning())
            {
            allActionsComplete = EFalse;
            }
        }

    index = iRunPluginActions.Count();
    while ((index--) && (allActionsComplete))
        {
        if (iRunPluginActions[index]->IsRunning())
            {
            allActionsComplete = EFalse;
            }
        }
    
    if (allActionsComplete)
        {
        //All immediate actions are complete and iFreeingMemory is being set to false but
        //SetMemoryMonitorStatusProperty will not change here because it is possible for the freeing actions to recommence
        //in essence, all immediate actions have been completed but not all possible actions have been processed 
        //and therefore we are still in a memory freeing state
        iFreeingMemory = EFalse;
        // If all of the actions are complete then check memory and run more if necessary
        TInt freeMemory = 0;
        iMonitor.GetFreeMemory(freeMemory);
        TInt freeSwap = 0;
        if (iSwapUsageMonitored)
            {
            iMonitor.GetFreeSwapSpace(freeSwap);
            }
        if ((freeMemory < iCurrentRamTarget) ||
            (iSwapUsageMonitored && (freeSwap < iCurrentSwapTarget)))
            // If we are still below the good-memory-threshold then continue running actions
            {
            iCurrentActionIndex++;
            
            if (iCurrentActionIndex >= iActionRefs.Count())
                {
                // There are no more actions to try, so we give up
                TRACES1("COomActionList::StateChanged: All current actions complete, below good threshold with no more actions available. freeMemory=%d", freeMemory);
                iMonitor.ResetTargets();
                if ((freeMemory >= iCurrentRamTarget) &&
                    ((!iSwapUsageMonitored) || (freeSwap >= iCurrentSwapTarget)))
                    {
                    SwitchOffPlugins();
                    iMonitor.SetMemoryMonitorStatusProperty(EAboveTreshHold);
                    }
                else
                    {
                    if (iMonitor.ActionTrigger() == EClientServerRequestOptionalRam)
                        {
                        //We always switch off the plugins after an optional RAM request
                        SwitchOffPlugins();
                        }
                    iMonitor.SetMemoryMonitorStatusProperty(EBelowTreshHold);
                    }
#ifdef CLIENT_REQUEST_QUEUE        
                iMonitor.ActionsCompleted(freeMemory, EFalse);
#else
                iServer.CloseAppsFinished(freeMemory, EFalse);
#endif
                }
            else
                {
                // There are still more actions to try, so we continue
                TRACES1("COomActionList::StateChanged: All current actions complete, running more actions. freeMemory=%d", freeMemory);
                FreeMemory(iMaxPriority);
                }
            }
        else
            {
            TRACES1("COomActionList::StateChanged: All current actions complete, memory good. freeMemory=%d", freeMemory);
            iMonitor.ResetTargets();
            SwitchOffPlugins();
            iMonitor.SetMemoryMonitorStatusProperty(EAboveTreshHold);
#ifdef CLIENT_REQUEST_QUEUE        
            iMonitor.ActionsCompleted(freeMemory, ETrue);
#else
            iServer.CloseAppsFinished(freeMemory, ETrue);
#endif
            }
        }
    
    // If some actions are not yet in the idle state then we must continue to wait for them (they will notify us of a state change via a callback)
    }

COomActionList::COomActionList(CMemoryMonitor& aMonitor, CMemoryMonitorServer& aServer, RWsSession& aWs) 
    : iWs(aWs), iMonitor(aMonitor), iServer(aServer)
    {
    FUNC_LOG;
    }

void COomActionList::ConstructL(COomConfig& aConfig)
    {
    FUNC_LOG;
    
    iCurrentActionIndex = 0;
    iSwapUsageMonitored = aConfig.GlobalConfig().iSwapUsageMonitored;
    iFreeingMemory = EFalse;
    
    // Get the list of V1 and V2 plugins available to the system
    iPluginList = COomPluginList<COomMonitorPlugin>::NewL(KOomPluginInterfaceUidValue);
    iPluginListV2 = COomPluginList<COomMonitorPluginV2>::NewL(KOomPluginInterfaceV2UidValue);
    // Go through each plugin in the plugin list, create a run-plugin action for each one
    TInt pluginIndex = iPluginList->Count();
    while (pluginIndex--)
        {
        // Get the config for this plugin
        COomRunPluginConfig& pluginConfig = aConfig.GetPluginConfig(iPluginList->Uid(pluginIndex));
        
        // Create an action acording to the config
        COomRunPlugin* action = COomRunPlugin::NewL(iPluginList->Uid(pluginIndex), pluginConfig, *this, iPluginList->Implementation(pluginIndex));
        
        iRunPluginActions.AppendL(action);
        }
    
    // Go through each V2 in the V2 plugin list, create a run-plugin action for each one
    pluginIndex = iPluginListV2->Count();
    while (pluginIndex--)
        {
        // Get the config for this plugin
        COomRunPluginConfig& pluginConfig = aConfig.GetPluginConfig(iPluginListV2->Uid(pluginIndex));
        
        // Create an action acording to the config
        COomRunPlugin* action = COomRunPlugin::NewL(iPluginList->Uid(pluginIndex), pluginConfig, *this, iPluginListV2->Implementation(pluginIndex), &(iPluginListV2->Implementation(pluginIndex)));
        
        iRunPluginActions.AppendL(action);
        }

    //allocate empty COomCloseApp objects
    TInt appCloseIndex = aConfig.GlobalConfig().iMaxCloseAppBatch;
    while (appCloseIndex--)        
        {
        COomCloseApp* action = COomCloseApp::NewL(*this, iWs);
        iCloseAppActions.AppendL(action);
        }
    }

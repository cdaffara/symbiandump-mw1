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


#ifndef OOMACTIONLIST_H_
#define OOMACTIONLIST_H_

#include <e32def.h>
#include <e32base.h>

#include "oommonitorplugin.h"

class COomWindowGroupList;
class CMemoryMonitorServer;
class COomCloseApp;
class TActionRef;
class COomRunPlugin;
class COomConfig;

/*
 * Interface for reporting a change of state in an OOM action
 * e.g. that the action has changed from a freeing-memory state to an idle state
 * 
 * @lib oommonitor.lib
 * @since S60 v5.0
 */
class MOomActionObserver
    {
public:
    virtual void StateChanged() = 0;
    };


template <class T> 
/**
 *  A class for getting instances of all of the OOM ECom plugins
 *  This class is templated because we actually need two types of list
 *  One list for V1 plugins
 *  One list for V2 plugins
 *  The functionality of each list is nearly identical, hence the templated class
 *  
 * @lib oommonitor.lib
 * @since S60 v5.0
 */
NONSHARABLE_CLASS(COomPluginList) : public CBase
    {
public:
    
    /** 
     * Two-phased constructor.
     * @param aInterfaceUid The interface of the plugin (either V1 or V2) depending on the templated type
     */
    static COomPluginList* NewL(TInt aInterfaceUid);
    inline TInt Count();
    inline T& Implementation(TInt aIndex);
    inline TInt32 Uid(TInt aIndex);
    
    ~COomPluginList();
    
private:
    COomPluginList();
    void ConstructL(TInt aInterfaceUid);
    
private: // data
    
    struct TPlugin
        {
        TPlugin();
        T* iImpl;
        TUid iDtorUid;
        TInt32 iUid;
        };
    
    RArray<TPlugin> iPlugins;
    };

/*
 * The list of possible OOM actions to be run.
 * 
 * This class is responsible for identifying the best action(s) to execute at a given time.
 * This decision is based on the properties of each action and idle time of the target apps.
 *
 *  @lib oommonitor.lib
 *  @since S60 v5.0
 */
NONSHARABLE_CLASS(COomActionList) : public CBase, public MOomActionObserver
    {
public:

    /**
     * Two-phased constructor.
     * @param aMonitor the owning Memory Monitor
     * @param aServer 
     * @param aWs a handle to a window server session, opened by the Memory Monitor
     * @param aConfig
     */
    static COomActionList* NewL(CMemoryMonitor& aMonitor, CMemoryMonitorServer& aServer, RWsSession& aWs, COomConfig& aConfig);

    ~COomActionList();
    
    /**
     * Creates a list of actions based on the contents of the config and the current window group list
     * Any old actions will be replaced.
     * 
     * @since S60 5.0
     * @param aWindowGroupList
     * @param aConfig
     */
    void BuildActionListL(COomWindowGroupList& aWindowGroupList, COomConfig& aConfig);    
    
    template <class T>
    void BuildPluginActionsL(COomPluginList<T>& aPluginList, COomWindowGroupList& aWindowGroupList, COomConfig& aConfig, TInt& aActionsIndex);
    
    /**
     * Execute the OOM actions according to their priority
     * Actions are run in batches according to their sync mode
     * 
     * @since S60 5.0
     * @param aMaxPriority The maximum priority of actions to run 
     */     
    void FreeMemory(TInt aMaxPriority);
    
    /**
     * Sets the target; the maximum prioirity up to which actions are run when an OomMonitor event 
     * occurs
     * 
     * @since S60 5.0
     * @param aCurrentRamTarget the desired amount of free unpaged memory
	 * @param aCurrentSwapTarget the desired amount of free paged memory
     */    
    inline void SetCurrentTargets(TUint aCurrentRamTarget, TUint aCurrentSwapTarget);
    
    /**
     * Switch all plugins to Off (Memory Good) state
     * Should be called when the memory situation is good, i.e. the actions run have released enough memory 
     * so that the device is above the current target.
     * It is also always called after an optional RAM request has been processed, even if insufficient
     * memory could be freed to successfully complete the request.
     * 
     * @since S60 5.0
     */     
    void SwitchOffPlugins();
        
    /**
     * Compares priorites of two actions, hard-coded rules are used to determine the order 
     * of actions with equal priority:
     * - application plugins are run in preference to system plugins
     * - appliction plugins where the target app is not running are run first
     * - Z order of the target app determines order of the rest of the application plugins (furthest back first)
     * - system plugins are run before app close actions
     * - Z order determines the prioirty of app close actions (furthest back first)
     * 
     * @since S60 5.0
     */    
    static TInt ComparePriorities(const TActionRef& aPos1, const TActionRef& aPos2 );

    /**
     * A callback from the UI framework that an applications has failed to respond to the close event.
     * Allows us to cleanup the related app close action object and move on the next available action.
     * 
     * @since S60 5.0
     * @param aWgId the window group ID of the application that has not closed.
     */    
    void AppNotExiting(TInt aWgId);
    
// from MOomActionObserver

    /**
     * 
     * from MOomActionObserver
     * 
     * An action has changed state (possibly it has completed freeing memory)
     */
    void StateChanged();    
    
private:
    
    COomActionList(CMemoryMonitor& aMonitor, CMemoryMonitorServer& aServer, RWsSession& aWs);
    
    void ConstructL(COomConfig& aConfig);
    
private: //data    
    
    RWsSession& iWs;
    
    RPointerArray<COomCloseApp> iCloseAppActions;
    RPointerArray<COomRunPlugin> iRunPluginActions;
    RArray<TActionRef> iActionRefs;
    
    TInt iCurrentActionIndex;
        
    TUint iCurrentRamTarget;
    TBool iSwapUsageMonitored;
    TUint iCurrentSwapTarget;

    /*
     * Flag specifying that a OomMonitor event is in progress.
     */
    TBool iFreeingMemory;
    
    CMemoryMonitor& iMonitor;

    /**
     * The list of V1 plugins
     * Own
     */
    COomPluginList<COomMonitorPlugin>* iPluginList;
    
    /**
     * The list of V2 plugins
     * Own
     */
    COomPluginList<COomMonitorPluginV2>* iPluginListV2;
    
    /**
     * The maximum priority of actions that should be run by an OomMonitor Event
     */
    TInt iMaxPriority; 
    
    CMemoryMonitorServer& iServer;
    
    };

#include "oomactionlist.inl"

#endif /*OOMACTIONLIST_H_*/

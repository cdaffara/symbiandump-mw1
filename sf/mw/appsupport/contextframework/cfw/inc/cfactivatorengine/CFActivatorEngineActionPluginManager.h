/*
* Copyright (c) 2008 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  CCFActivatorEngineActionPluginManager class declaration.
 *
*/


#ifndef C_CCFACTIVATORENGINEACTIONPLUGINMANAGER_H
#define C_CCFACTIVATORENGINEACTIONPLUGINMANAGER_H

#include "cfecompluginmanager.h"
#include "cfphasebase.h"
#include "CFActionCacheElement.h"
#include "cfasynccleanupcallback.h"

class CAsyncCallBack;
class CCFActionPlugIn;
class CCFActionIndication;
class CCFActionPlugInThread;
class CImplementationInformation;
class CCFActionPlugInImpl;
class MCFStarterEventHandler;

/**
 *  Actiion plug-in manager loads and maintains action plug-ins.
 *
 *  @lib -
 *  @since S60 s60 4.0
 */
NONSHARABLE_CLASS( CCFActivatorEngineActionPluginManager ): public CCFEComPluginManager,
public MCFAsyncCleanupCallback
    {
public:

    // Two phased constructors
    static CCFActivatorEngineActionPluginManager* NewL(
        RActionCacheArray& aActionCache );
    static CCFActivatorEngineActionPluginManager* NewLC(
        RActionCacheArray& aActionCache );

    // Destructor
    ~CCFActivatorEngineActionPluginManager();

public: // New methods

    /**
     * Trigger a specific action.
     * All the plug-ins are searched through. When the first
     * plug-in is found with the action identifier, it will
     * be executed.
     *
     * @since S60 4.0
     * @param aActionIndication Action indication.
     * @return ETrue if action for the identifier found.
     */
    TBool TriggerL( CCFActionIndication* aActionIndication );

    /**
     * Initializes context framework phases.
     *
     * @since S60 4.0
     * @param aPhaseId Current phase.
     * @return None.
     */
    void InitializePhaseL( CCFPhaseBase::TCFPhaseId aPhaseId );

    /**
     * "Abusing" the frienship of ActionPlugIn.
     */
    static void SetImplPointer( CCFActionPlugIn& aActionPlugIn,
        CCFActionPlugInImpl* aImpl);

    /**
     * Go through action plugins if new plugins added or removed.
     */
    void UpdatePlugInsL();

    /**
     * Add new actions to action cache.
     *
     * @since S60 3.2
     * @param aActionPluginThread Identifies the action plug-in whose actions
     * to add.
     * @return None.
     */
    void AddNewActionsToCacheL( CCFActionPlugInThread& aActionPluginThread );

        /**
        * Sets event handler to observers.
        * 
        * @since S60 3.2
        * @param aEventHandler Event handler.
        * @return None.
        */
        void SetEventHandler( MCFStarterEventHandler& aEventHandler );
        
private: // New methods

    // Initialize device starting phase
    void InitDeviceStartingPhaseL();

        // Initialize loading plugins phase
        void InitLoadingPluginsPhaseL();

    // Load plug-in
    void LoadPlugInL( TUid aUid, TInt aImplVersion );

    // Release action plug-in
    void ReleasePlugInL( CCFActionPlugInThread* aActionPlugInThread, TBool aUpgrade );

    // @see MCFAsyncCleanupCallback
    void CleanupCompleted( CCFAsyncCleanup* aCleanup );
    
    // Call back function for asynchronous plug-in loading   
    static TInt LoaderCallBack( TAny* aLoaderInfo );
        
    // Prepares plug-in loaders
    void PrepareLoaderL( CImplementationInformation& aImplementationInfo );
        
    // Execute function for plug-in load
    void ExecuteLoaders();

private:

    CCFActivatorEngineActionPluginManager( RActionCacheArray& aActionCache );
    void ConstructL();

private:

    /** List of action plug-in threads */
    RPointerArray<CCFActionPlugInThread> iActionPlugInThreads;

    /** Reference to action cache */
    RActionCacheArray& iActionCache;

    /** List of async cleanup requests */
    RPointerArray<CCFAsyncCleanup> iCleanupList;
    
    // Loaders for plug-in loading
    RPointerArray<CAsyncCallBack> iLoaders;
        
    // Counter for loaded plug-ins
    TInt iLoadedCount;
        
        /*  
         * Event handler
         * Not own. Can be NULL
         */
        MCFStarterEventHandler* iEventHandler;
// JNIA-849K7Gs
	TBool iUpdatePluginsAllowed;
    };

#endif

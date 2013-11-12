/*
* Copyright (c) 2006-2006 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  CCFActionPlugInThread class declaration.
 *
*/


#ifndef CFACTIONPLUGINTHREAD_H
#define CFACTIONPLUGINTHREAD_H

#include <e32base.h>	// For CActive, link against: euser.lib
#include <e32std.h>		// For RTimer, link against: euser.lib
#include <badesca.h>

#include "cfactionowner.h"

// FORWARD DECLARATIONS
class CCFActivatorEngineActionPluginManager;
class CImplementationInformation;
class CCFActionPlugIn;
class CCFActivatorTimedSchedulerWait;
class CCFActionIndication;
class CCFActionExecutionHandler;

// CONSTANTS
const TInt KErrBase             = 1000;
const TInt KErrReleasePlugin    = KErrBase + 1;
const TInt KErrReloadPlugin     = KErrBase + 2;

NONSHARABLE_CLASS( CCFActionPlugInThread ): public CBase,
public MCFActionOwner
    {
public:
    // Cancel and destroy
    ~CCFActionPlugInThread();

    // Two-phased constructor.
    static CCFActionPlugInThread* NewL(
        CCFActivatorEngineActionPluginManager& aActionPluginManager,
        CImplementationInformation& aImplInfo );

    // Two-phased constructor.
    static CCFActionPlugInThread* NewLC(
        CCFActivatorEngineActionPluginManager& aActionPluginManager,
        CImplementationInformation& aImplInfo );

    // Two-phased constructor.
    static CCFActionPlugInThread* NewLC( 
        CCFActivatorEngineActionPluginManager& aActionPluginManager,
        TUid aUid,
        TInt aImplVersion );

public: // New functions

    /**
     * Return implementation uid of the plug-in
     * @return TUid
     */
    TUid ImplementationUid() const;
    
    /**
     * Return the version of the implementation.
     * @return TInt.
     */
    TInt Version() const;
    
    /**
     * Set plug-in as missing state (installed/uninstalled).
     * @param aMissing ETrue if missing.
     */
    void SetMissing( TBool aMissing );
    
    /**
     * Check if plug-in is missing.
     * @return TBool.
     */
    TBool Missing() const;
    
    /**
     * Get actions from all plug-ins.
     * @param aActionList List where actions are stored.
     */
    void GetActionsL( CDesCArray& aActionList ) const;
    
    /**
     * Get action security policy.
     * @param aActionId Action which security is needed.
     * @return const TCesurityPolicy&.
     */
    const TSecurityPolicy& SecurityPolicy( const TDesC& aActionId ) const;
    
    /**
     * Releases action plug-in implementation.
     * @param aStatus Request status which is completed when the plug-in has
     *  been deleted.
     * @return None.
     */
    void ReleasePlugin( TRequestStatus& aStatus );
    
    /**
     * Cancels release plug-in request.
     * @param None.
     * @return None.
     */
    void CancelReleasePlugin();
    
    /**
     * Reloads plug-in implementation.
     * @param None.
     * @return None.
     */
    void ReloadPlugin();
    
private:

    // @see MCFActionOwner
    TCFActionOwner ActionOwner() const;

private:
    // C++ constructor
    CCFActionPlugInThread(
        CCFActivatorEngineActionPluginManager& aActionPluginManager,
        CImplementationInformation& aImplInfo );

    CCFActionPlugInThread(
	    CCFActivatorEngineActionPluginManager& aActionPluginManager,
	    TUid aUid,
        TInt aImplVersion );

    // Second-phase constructor
    void ConstructL();

public: // functions related to inter-thread communication

    // called in the main thread
    void TriggerL( CCFActionIndication* aActionIndication );
    void RunPendingActions();

    // called in the plug-in thread

    // wait for new actions to arrive to action queue
    void WaitActions( TRequestStatus& aStatus );
    void CancelActionsWait();

private: // critical section protected functions handling action queue

    // called in the main thread
    TInt AddToQueue( CCFActionIndication* aAction );

    // called in the plug-in thread
    CCFActionIndication* GetNextActionToExecute();

public: // functions called in the the plug-in thread

    // Execute actions from queue
    void NewActionsAddedToQueueL();
    
    // @see CCFActionPlugIn
    void AsyncExecutionCompleted();
    
    // Delete plug-in implementation
    void DeletePlugin();
    
    // Re-construct plug-in implementation
    void ConstructPluginL();

private: // functions called in the plug-in thread

    // Thread function for the action plug-in creation
    static TInt ThreadFunction( TAny* aSelf );

    // executed in the plug-in thread start
    void ThreadConstructL();

    // Leavable thread function
    void ThreadFunctionL();

    // executed in the plug-in thread end
    void ThreadDestruct();

    void ExecuteActionsInQueue();

private: // Owned by the main thread

    // Own: Plugin uid
    TUid iPlugInUid;

    // Own: Plugin version
    TInt iImplVersion;

    // Ref: Plugin manager
    CCFActivatorEngineActionPluginManager* iActionPluginManager;

    RThread iMainThread;

    // Own: Thread handle
    RThread iPluginThread;
    
    // Own: Missing flag
    TBool iMissing;

private: // Owned by main thread but used also by the plug-in thread

    // Own: Semaphore for signaling construction and destruction
    // finished for plug-in thread
    RSemaphore iConstructDestructSemaphore;

    // Own: a flag to signal exit condition
    TBool iExit;

    // Own: Critical section to protect action queue
    RCriticalSection iActionQueueCriticalSection;

    // pointer to request status waiting new actions
    TRequestStatus* iWaitActionsRequestStatus;

    TRequestStatus* iRunPendingActionsRequestStatus;
    
    TThreadId iMainThreadId;

private: // Created and destructed by the plugin thread

    CActiveScheduler* iScheduler;

    // Ref: Plugin instance
    CCFActionPlugIn* iPlugIn;

    // Own: Indicated actions
    RPointerArray<CCFActionIndication> iActionQueue;

    // Own: Current action
    CCFActionIndication* iCurrentAction;

    // Own: Timed scheduler wait
    CCFActivatorTimedSchedulerWait* iWait;

    // Own: An active object to listen requests from the main thread
    CCFActionExecutionHandler* iActionExecutionHandler;
    
    // Ref: Async cleanup request
    TRequestStatus* iAsyncCleanupRequest;
    };

#endif // CFACTIONPLUGINTHREAD_H


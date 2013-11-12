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


#ifndef OOMCLOSEAPP_H_
#define OOMCLOSEAPP_H_


#include <apgtask.h>

#include "oomaction.h"

class COomAppCloseTimer;
class COomAppCloseWatcher;
class TApaTask;
class TActionRef;

/*
 * The OOM action of closing an application in order to free up memory.
 *
 *  @lib oommonitor.lib
 *  @since S60 v5.0
 */
NONSHARABLE_CLASS(COomCloseApp) : public COomAction
    {
public:
    /**
     * Two-phased constructor.
     * @param aStateChangeObserver an observer to the state of the application
     * @param aWs a connected handle to a window server session
     */
    static COomCloseApp* NewL(MOomActionObserver& aStateChangeObserver, RWsSession& aWs);
        
    //from COomAction
    
    /**
     * Close the application in order to free memory
     * Call the COomAction::MemoryFreed when it is done
     * @param aBytesRequested not used for clsoe app actions
     */
    virtual void FreeMemory(TInt aBytesRequested, TBool aIsDataPaged);
    
    ~COomCloseApp();
    
    /**
     * Callback from COomAppCloseWatcher and COomAppCloseTimer
     */
    void CloseAppEvent();
    
    inline TBool IsRunning();

    /**
     * Configure, or reconfigure the COomCloseApp object
     * Action objects are reused to minimize any memory allocation when memory is low
     */
    void Reconfigure(const TActionRef& aRef);

    inline TUint WgId() const;
    
private:
    
    COomCloseApp(MOomActionObserver& aStateChangeObserver, RWsSession& aWs);
    
    void ConstructL();
    
    TUint iWgId;
        
    TBool iAppCloserRunning;
    TApaTask iCurrentTask;

    /**
     * A timeout for an app close action
     * Own
     */
    COomAppCloseTimer* iAppCloseTimer;

    /**
     * A watcher for the application closing
     * Own
     */
    COomAppCloseWatcher* iAppCloseWatcher; 
    };

#include "oomcloseapp.inl"

#endif /*OOMCLOSEAPP_H_*/

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


#include <apgtask.h>

#include "oomcloseapp.h"
#include "OomTraces.h"
#include "oomappclosetimer.h"
#include "oomappclosewatcher.h"
#include "oomactionref.h"
#include "oommemorymonitor.h"
#include "oomconstants.hrh"
#include "oompanic.h"

COomCloseApp* COomCloseApp::NewL(MOomActionObserver& aStateChangeObserver, RWsSession& aWs)
    {
    FUNC_LOG;

    COomCloseApp* self = new (ELeave) COomCloseApp(aStateChangeObserver, aWs);
    CleanupStack::PushL(self);
    self->ConstructL();
    CleanupStack::Pop(self);
    return self;
    }

// Close the application in order to free memory
// Call the COomAction::MemoryFreed when it is done
void COomCloseApp::FreeMemory(TInt, TBool aIsDataPaged)
    {
    FUNC_LOG;

    iAppCloserRunning = ETrue;
    
    // Set the TApaTask to the app
    iCurrentTask.SetWgId(iWgId);
    
    __ASSERT_DEBUG(!iAppCloseTimer->IsActive(), OomMonitorPanic(KStartingActiveCloseAppTimer));    
    __ASSERT_DEBUG(!iAppCloseWatcher->IsActive(), OomMonitorPanic(KStartingActiveAppCloseWatcher));    
    // Start a timer and the thread watcher 
    iAppCloseTimer->After(CMemoryMonitor::GlobalConfig().iMaxAppExitTime * KMicrosecondsInMillisecond);
    iAppCloseWatcher->Start(iCurrentTask);
    // Tell the app to close
    TRACES1("COomCloseApp::FreeMemory: Closing app with window group id %d",iWgId);
    
    RThread thread;
    TInt err=thread.Open(iCurrentTask.ThreadId());
    if (err == KErrNone)
        {
        RProcess process;
        err = thread.Process(process);
        if(err == KErrNone)
            {
            TBool isDataPaged = process.DefaultDataPaged();
            if((aIsDataPaged && isDataPaged) || (!aIsDataPaged && !isDataPaged ))
                {
                iCurrentTask.EndTask();
                }                    
            process.Close();
            }
        thread.Close();
        }
    }

COomCloseApp::~COomCloseApp()
    {
    FUNC_LOG;

    if (iAppCloseTimer)
        iAppCloseTimer->Cancel();
    
    if (iAppCloseWatcher)
        iAppCloseWatcher->Cancel();
    
    delete iAppCloseTimer;    
    delete iAppCloseWatcher;
    }

// Callback from COomAppCloseWatcher and COomAppCloseTimer
void COomCloseApp::CloseAppEvent()
    {
    FUNC_LOG;

    // The application has closed (or we have a timeout)
    iAppCloserRunning = EFalse;
    
    if (iAppCloseTimer)
        iAppCloseTimer->Cancel();
    if (iAppCloseWatcher)
        iAppCloseWatcher->Cancel(); 
    
    MemoryFreed(KErrNone);
    }

void COomCloseApp::Reconfigure(const TActionRef& aRef)
    {
    FUNC_LOG;

    iWgId = aRef.WgId();    
    }

void COomCloseApp::ConstructL()
    {
    FUNC_LOG;

    iAppCloseTimer = COomAppCloseTimer::NewL(*this);
    iAppCloseWatcher = new(ELeave) COomAppCloseWatcher(*this);
    }
        
COomCloseApp::COomCloseApp(MOomActionObserver& aStateChangeObserver, RWsSession& aWs)
                                : COomAction(aStateChangeObserver), iCurrentTask(aWs)
    {
    FUNC_LOG;
    }

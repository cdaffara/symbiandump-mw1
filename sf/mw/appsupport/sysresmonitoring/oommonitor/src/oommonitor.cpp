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
* Description:  Definitions of OOM Monitor.
*
*/




#include "oommonitor.h"
#include "oommemorymonitor.h"
#include "oommonitorclientserver.h"
#include "OomTraces.h"

const TInt KStackSize = 0x2000;

_LIT(KOOMWatcherThreadName, "OOM FW");


// Implements just Error() to avoid panic
NONSHARABLE_CLASS(CSimpleScheduler) : public CActiveScheduler
    {
    void Error( TInt ) const{} // From CActiveScheduler
    };

// thread function for OOM watcher
GLDEF_C TInt WatcherThreadFunction( TAny* )
    {
    FUNC_LOG;

    TInt err( KErrNone );

    CTrapCleanup* cleanup = CTrapCleanup::New();
    CActiveScheduler* scheduler = new CSimpleScheduler();
    CMemoryMonitor* oom = NULL;

    if ( !cleanup || !scheduler)
        {
        err = KErrNoMemory;
        }
    else
        {
        CActiveScheduler::Install( scheduler );
        TRAPD( err,
            {
            RThread me;
            me.SetPriority(EPriorityAbsoluteHigh);
            oom = CMemoryMonitor::NewL();
            } )

        if ( err == KErrNone )
            {
            // start the watchers
            CActiveScheduler::Start();
            }
        }

    delete cleanup;
    delete scheduler;
    delete oom;

    return err;
    }

// Creates thread for OOM watchers
EXPORT_C void CreateOOMWatcherThreadL()
    {
    FUNC_LOG;

    RThread thread;
    TInt ret = thread.Create( KOOMWatcherThreadName,
                              WatcherThreadFunction,
                              KStackSize, // stack size
                              NULL, // uses caller thread's heap
                              NULL );

    if ( ret == KErrNone )
        {
        thread.Resume();
        thread.Close();
        }

    User::LeaveIfError( ret );
    }

void PanicClient(const RMessagePtr2& aMessage,TOomMonitorClientPanic aPanic)
    {
    FUNC_LOG;

    aMessage.Panic(KMemoryMonitorServerName, aPanic);
    }

// End of file.

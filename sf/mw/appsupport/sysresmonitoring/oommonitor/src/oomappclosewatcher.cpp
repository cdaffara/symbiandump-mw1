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
* Description:  An active object which watches for app close actions successfully completing.
*
*/



#include "oomappclosewatcher.h"
#include "oomcloseapp.h"
#include "OomTraces.h"

COomAppCloseWatcher::COomAppCloseWatcher(COomCloseApp& aMonitor) : CActive(CActive::EPriorityStandard), iMonitor(aMonitor)
    {
    FUNC_LOG;

    CActiveScheduler::Add(this);
    }

COomAppCloseWatcher::~COomAppCloseWatcher()
    {
    FUNC_LOG;

    Cancel();
    }

void COomAppCloseWatcher::Start(const TApaTask& aTask)
    {
    FUNC_LOG;

    if (!IsActive())
        {
        TInt err = iThread.Open(aTask.ThreadId());
        if (err == KErrNone)
            {
            iOriginalProcessPriority = iThread.ProcessPriority();
            iThread.SetProcessPriority(EPriorityForeground);
            iThread.Logon(iStatus);
            SetActive();
            }
        else
            {
            TRequestStatus* s = &iStatus;
            User::RequestComplete(s, err);
            SetActive();
            }
        }
    }

void COomAppCloseWatcher::DoCancel()
    {
    FUNC_LOG;

    iThread.LogonCancel(iStatus);
    iThread.SetProcessPriority(iOriginalProcessPriority);
    iThread.Close();
    }

void COomAppCloseWatcher::RunL()
    {
    FUNC_LOG;

    if (iThread.Handle())
        iThread.SetProcessPriority(iOriginalProcessPriority);
    iThread.Close();
    // Experimentation shows that memory may take up to 40ms
    // to be released back to the system after app thread close.
    // Using this delay should minimise the number of apps that
    // need to be closed to recover the necessary memory.
    const TInt KAppTidyUpDelay = 40000;
    User::After(KAppTidyUpDelay);
    iMonitor.CloseAppEvent();
    }

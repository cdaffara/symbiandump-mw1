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



#include "oommemorymonitorserver.h"
#include "oommonitorclientserver.h"
#include "oommemorymonitorsession.h"
#include "oommemorymonitor.h"
#include "OomTraces.h"

#ifdef CLIENT_REQUEST_QUEUE        
CMemoryMonitorServer* CMemoryMonitorServer::NewL(COomClientRequestQueue& aQueue)
#else
CMemoryMonitorServer* CMemoryMonitorServer::NewL(CMemoryMonitor& aMonitor)
#endif
    {
    FUNC_LOG;

#ifdef CLIENT_REQUEST_QUEUE        
    CMemoryMonitorServer* self=new(ELeave) CMemoryMonitorServer(aQueue);
#else
    CMemoryMonitorServer* self=new(ELeave) CMemoryMonitorServer(aMonitor);
#endif
    CleanupStack::PushL(self);
    self->ConstructL();
    CleanupStack::Pop(self);
    return self;
    }

CMemoryMonitorServer::~CMemoryMonitorServer()
    {
    FUNC_LOG;
    }

#ifdef CLIENT_REQUEST_QUEUE        
CMemoryMonitorServer::CMemoryMonitorServer(COomClientRequestQueue& aQueue)
    :CServer2(CActive::EPriorityStandard), iQueue(aQueue)
#else
CMemoryMonitorServer::CMemoryMonitorServer(CMemoryMonitor& aMonitor)
    :CServer2(CActive::EPriorityStandard), iMonitor(aMonitor)    
#endif
    {
    FUNC_LOG;
    }

void CMemoryMonitorServer::ConstructL()
    {
    FUNC_LOG;

    StartL(KMemoryMonitorServerName);
    }

CSession2* CMemoryMonitorServer::NewSessionL(const TVersion& /*aVersion*/, const RMessage2& /*aMessage*/) const
    {
    FUNC_LOG;

    return new(ELeave) CMemoryMonitorSession();
    }

TInt CMemoryMonitorServer::RunError(TInt aError)
    {
    FUNC_LOG;

    Message().Complete(aError);
    //
    // The leave will result in an early return from CServer::RunL(), skipping
    // the call to request another message. So do that now in order to keep the
    // server running.
    ReStart();
    return KErrNone;    // handled the error fully
    }

#ifdef CLIENT_REQUEST_QUEUE        
COomClientRequestQueue& CMemoryMonitorServer::ClientRequestQueue()
    {
    FUNC_LOG;

    return iQueue;
    }
#else
CMemoryMonitor& CMemoryMonitorServer::Monitor()
    {
    FUNC_LOG;

    return iMonitor;
    }
#endif

#ifndef CLIENT_REQUEST_QUEUE        
void CMemoryMonitorServer::CloseAppsFinished(TInt aBytesFree, TBool aMemoryGood)
    {
    FUNC_LOG;

    iSessionIter.SetToFirst();
    CSession2* s;
    while ((s = iSessionIter++) != 0)
        static_cast<CMemoryMonitorSession*>(s)->CloseAppsFinished(aBytesFree, aMemoryGood);
    }
#endif

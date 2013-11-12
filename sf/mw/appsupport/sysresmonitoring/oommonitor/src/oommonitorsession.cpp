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
* Description:  Client/server interface for OOM Monitor.
*
*/


#include <coemain.h>
#include <oommonitorsession.h>
#include "oommonitorclientserver.h"
#include "oompanic.h"
#include "OomTraces.h"

EXPORT_C TInt ROomMonitorSession::Connect()
    {
    FUNC_LOG;

    return CreateSession(KMemoryMonitorServerName, TVersion(0,0,0));
    }

EXPORT_C TInt ROomMonitorSession::RequestFreeMemory(TInt aBytesRequested)
    {
    FUNC_LOG;

    TIpcArgs p(aBytesRequested);
    return SendReceive(EOomMonitorRequestFreeMemory, p);
    }

EXPORT_C TInt ROomMonitorSession::RequestOptionalRam(TInt aBytesRequested, TInt aMinimumBytesNeeded, TInt aPluginId, TInt& aBytesAvailable)
    {
    FUNC_LOG;

    TIpcArgs p(aBytesRequested, aMinimumBytesNeeded, aPluginId, aBytesAvailable);
    TInt ret = SendReceive(EOomMonitorRequestOptionalRam, p);
    if (ret >= 0)
        {
        aBytesAvailable = ret;
        ret = KErrNone;
        }
    
    return ret;
    }



EXPORT_C void ROomMonitorSession::RequestOptionalRam(TInt aBytesRequested, TInt aMinimumBytesNeeded, TInt aPluginId, TRequestStatus& aStatus)
    {
    FUNC_LOG;

    TIpcArgs p(aBytesRequested, aMinimumBytesNeeded, aPluginId);
    SendReceive(EOomMonitorRequestOptionalRam, p, aStatus);
    }

EXPORT_C void ROomMonitorSession::RequestFreeMemory(TInt aBytesRequested, TRequestStatus& aStatus)
    {
    FUNC_LOG;

    TIpcArgs p(aBytesRequested);
    SendReceive(EOomMonitorRequestFreeMemory, p, aStatus);
    }

EXPORT_C void ROomMonitorSession::CancelRequestFreeMemory()
    {
    FUNC_LOG;

    SendReceive(EOomMonitorCancelRequestFreeMemory, TIpcArgs());
    }

EXPORT_C void ROomMonitorSession::ThisAppIsNotExiting(TInt aWgId)
    {
    FUNC_LOG;

    TIpcArgs p(aWgId);
    SendReceive(EOomMonitorThisAppIsNotExiting, p);
    }


EXPORT_C void ROomMonitorSession::SetOomPriority(TOomPriority aPriority)
    {
    FUNC_LOG;

    CCoeEnv* coeEnv = CCoeEnv::Static();

    __ASSERT_DEBUG(coeEnv, OomMonitorPanic(KNoCoeEnvFound));    

    if (coeEnv)
        {
        TInt wgId = coeEnv->RootWin().Identifier();
        TIpcArgs p(wgId);
        switch (aPriority)
            {
            case EOomPriorityNormal:
                SendReceive(EOomMonitorSetPriorityNormal, p);
                break;
            case EOomPriorityHigh:
                SendReceive(EOomMonitorSetPriorityHigh, p);
                break;
            case EOomPriorityBusy:
                SendReceive(EOomMonitorSetPriorityBusy, p);
                break;
            default:
                OomMonitorPanic(KOomInvalidPriority);
                break;
            }
        }
    }



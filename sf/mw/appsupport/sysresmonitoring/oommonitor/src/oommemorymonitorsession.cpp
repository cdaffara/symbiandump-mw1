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


#include "oommonitorclientserver.h"
#include "oommemorymonitorsession.h"
#include "oommemorymonitor.h"
#include "oommemorymonitorserver.h"
#include "OomTraces.h"
#include "oomclientrequestqueue.h"

CMemoryMonitorSession::CMemoryMonitorSession()
    {
    FUNC_LOG;
    }


CMemoryMonitorSession::~CMemoryMonitorSession()
    {
    FUNC_LOG;
    }

CMemoryMonitorServer& CMemoryMonitorSession::Server()
    {
    FUNC_LOG;

    return *static_cast<CMemoryMonitorServer*>(const_cast<CServer2*>(CSession2::Server()));
    }

#ifdef CLIENT_REQUEST_QUEUE
COomClientRequestQueue& CMemoryMonitorSession::ClientRequestQueue()
    {
    FUNC_LOG;

    return Server().ClientRequestQueue();
    }
#endif

CMemoryMonitor& CMemoryMonitorSession::Monitor()
    {
    FUNC_LOG;

#ifdef CLIENT_REQUEST_QUEUE
    return ClientRequestQueue().Monitor();
#else
    return Server().Monitor();    
#endif
    }
#ifndef CLIENT_REQUEST_QUEUE
TBool CMemoryMonitorSession::IsDataPaged(const RMessage2& aMessage)
    {
    RThread clientThread;
    TInt err = aMessage.Client(clientThread);
    TBool dataPaged = EFalse;
    if(err == KErrNone)
        {
        RProcess processName;
        err = clientThread.Process(processName);
        if(err == KErrNone)
            {
            dataPaged = processName.DefaultDataPaged();
            processName.Close();
            clientThread.Close();
            }
        else
            {
            clientThread.Close();                        
            PanicClient(aMessage, EPanicIllegalFunction);
            }
        }
    else
        {
        PanicClient(aMessage, EPanicIllegalFunction);
        }
    return dataPaged;            
    }
#endif

void CMemoryMonitorSession::ServiceL(const RMessage2& aMessage)
    {
    FUNC_LOG;
        
#ifndef CLIENT_REQUEST_QUEUE
    iFunction = aMessage.Function();
#endif 
    
    switch (aMessage.Function())
        {
        case EOomMonitorRequestFreeMemory:
            if (!iRequestFreeRam.IsNull())
                PanicClient(aMessage, EPanicRequestActive);
            // message will be completed when CloseAppsFinished() is called.  
            
#ifdef CLIENT_REQUEST_QUEUE
            ClientRequestQueue().RequestFreeMemoryL(aMessage);
#else
            iRequestFreeRam = aMessage;
            Monitor().RequestFreeMemoryL(aMessage.Int0(), IsDataPaged(aMessage));
#endif
            break;

        case EOomMonitorCancelRequestFreeMemory:
            if (!iRequestFreeRam.IsNull())
                iRequestFreeRam.Complete(KErrCancel);
            aMessage.Complete(KErrNone);
            break;

        case EOomMonitorThisAppIsNotExiting:
            Monitor().AppNotExiting(aMessage.Int0());
            aMessage.Complete(KErrNone);
            break;

        case EOomMonitorRequestOptionalRam:
            if (!iRequestFreeRam.IsNull())
                PanicClient(aMessage, EPanicRequestActive);
            // message will be completed when CloseAppsFinished() is called.
            
#ifdef CLIENT_REQUEST_QUEUE
            ClientRequestQueue().RequestOptionalRamL(aMessage);
#else
            iRequestFreeRam = aMessage;
            iMinimumMemoryRequested = aMessage.Int1();
            Monitor().FreeOptionalRamL(aMessage.Int0(), aMessage.Int2(), IsDataPaged(aMessage));
#endif            
            break;
            
        case EOomMonitorSetPriorityBusy:
            Monitor().SetPriorityBusy(aMessage.Int0());
            aMessage.Complete(KErrNone);
            break;
            
        case EOomMonitorSetPriorityNormal:
               Monitor().SetPriorityNormal(aMessage.Int0());
            aMessage.Complete(KErrNone);
            break;

        case EOomMonitorSetPriorityHigh:
               Monitor().SetPriorityHigh(aMessage.Int0());
            aMessage.Complete(KErrNone);
            break;

        default:
            PanicClient(aMessage, EPanicIllegalFunction);
            break;
        }
    }

#ifndef CLIENT_REQUEST_QUEUE
void CMemoryMonitorSession::CloseAppsFinished(TInt aBytesFree, TBool aMemoryGood)
    {
    FUNC_LOG;

    if (!iRequestFreeRam.IsNull())
        {
        if (iFunction == EOomMonitorRequestOptionalRam)
            {
            TInt memoryAvailable = aBytesFree - CMemoryMonitor::GlobalConfig().iGoodRamThreshold;
            
            // If memory available is greater than the requested RAM then complete with the amount of free memory, otherwise complete with KErrNoMemory
            if (memoryAvailable >= iMinimumMemoryRequested)
                {
                iRequestFreeRam.Complete(memoryAvailable);
                }
            else
                {
                iRequestFreeRam.Complete(KErrNoMemory);
                }
               }
        else 
            iRequestFreeRam.Complete(aMemoryGood ? KErrNone : KErrNoMemory);
        }
    }
#endif

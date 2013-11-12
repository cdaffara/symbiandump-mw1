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
* Description:  OOM Monitor plug-in interface.
*
*/


#include <apgwgnam.h>
#include "oommonitorplugin.h"
#include "oommemorymonitor.h"
#include "OomTraces.h"

// TLS is used to store the CMemoryMonitor pointer, CMemoryMonitor
// being the main object in the OOM monitor thread. This allows
// plugins to access the CMemoryMonitor object easily.
EXPORT_C void SetMemoryMonitorTls(CMemoryMonitor* aMonitor)
    {
    FUNC_LOG;

    Dll::SetTls(aMonitor);
    }

CMemoryMonitor* MemoryMonitorTls()
    {
    FUNC_LOG;

    return static_cast<CMemoryMonitor*>(Dll::Tls());
    }


void OomMonitorPluginPanic(TOomMonitorPluginPanic aReason)
    {
    FUNC_LOG;

    _LIT(KCat, "OomMonitorPlugin");
    User::Panic(KCat, aReason);
    }


EXPORT_C COomMonitorPlugin::COomMonitorPlugin()
: iMemoryMonitor(MemoryMonitorTls())
    {
    FUNC_LOG;

    __ASSERT_ALWAYS(iMemoryMonitor, OomMonitorPluginPanic(EOomMonitorPluginPanic_PluginConstructedOutsideOomMonitorThread));
    }

EXPORT_C COomMonitorPlugin::~COomMonitorPlugin()
    {
    FUNC_LOG;
    }

EXPORT_C void COomMonitorPlugin::ConstructL()
    {
    FUNC_LOG;

    // CAppOomMonitorPlugin assumes ConstructL is empty
    }

EXPORT_C void COomMonitorPlugin::ExtensionInterface(TUid /*aInterfaceId*/, TAny*& /*aImplementaion*/)
    {
    FUNC_LOG;
    }

EXPORT_C RFs& COomMonitorPlugin::FsSession()
    {
    FUNC_LOG;

    return iMemoryMonitor->iFs;
    }

EXPORT_C RWsSession& COomMonitorPlugin::WsSession()
    {
    FUNC_LOG;

    return iMemoryMonitor->iWs;
    }



EXPORT_C void COomMonitorPluginV2::FreeRam()
    {
    // Note that OomMonitorV2 will not call this version of the function
    // so it does not need to be implemented in derived classes.
    }



EXPORT_C CAppOomMonitorPlugin* CAppOomMonitorPlugin::NewL(TUid aAppUid)
    {
    CAppOomMonitorPlugin* self = new(ELeave) CAppOomMonitorPlugin(aAppUid);
    // ConstructL() currently not needed
    return self;
    }

CAppOomMonitorPlugin::CAppOomMonitorPlugin(TUid aAppUid)
: iAppUid(aAppUid)
    {
    }

void CAppOomMonitorPlugin::FreeRam()
    {
    SendMessageToApp(KAppOomMonitor_FreeRam);
    }

void CAppOomMonitorPlugin::MemoryGood()
    {
    SendMessageToApp(KAppOomMonitor_MemoryGood);
    }

void CAppOomMonitorPlugin::SendMessageToApp(TInt aMessage)
    {
    RWsSession& ws = WsSession();
    TInt wgId = 0;

    do 
        {
        CApaWindowGroupName::FindByAppUid(iAppUid, ws, wgId);
        if (wgId>0)
            {
            TWsEvent event;
            event.SetType(aMessage);
            event.SetTimeNow();
            ws.SendEventToWindowGroup(wgId, event);
            }
        }
    while (wgId>0);
    }

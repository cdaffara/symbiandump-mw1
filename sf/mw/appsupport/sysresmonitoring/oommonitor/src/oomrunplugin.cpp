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

#include "oomrunplugin.h"
#include "OomTraces.h"
#include "oommemorymonitor.h"
#include "oommonitorplugin.h"
#include "oompanic.h"

COomRunPlugin* COomRunPlugin::NewL(TUint aPluginId, COomRunPluginConfig& aConfig, MOomActionObserver& aStateChangeObserver, COomMonitorPlugin& aPlugin, COomMonitorPluginV2* aV2Plugin)
    {
    FUNC_LOG;

    COomRunPlugin* self = new (ELeave) COomRunPlugin(aPluginId, aConfig, aStateChangeObserver, aPlugin, aV2Plugin);
    CleanupStack::PushL(self);
    self->ConstructL(aConfig);
    CleanupStack::Pop(self);
    return self;
    }

// Run the OOM plugin in order to free memory
// Call the COomAction::MemoryFreed when it is done
void COomRunPlugin::FreeMemory(TInt aBytesRequested, TBool)
    {
    FUNC_LOG;
    TRACES1("COomRunPlugin::FreeMemory: iPluginId = %x", iPluginId);        

    // Ask the plugin to free some memory

    // Do we have a V2 plugin, if so then use it
    if (iV2Plugin)
        iV2Plugin->FreeRam(aBytesRequested);
    else
        // If we only have a V1 plugin then use that
        iPlugin.FreeRam();
    
    iFreeMemoryCalled = ETrue;
    
    // Wait for the required time before we signal completion.
    __ASSERT_DEBUG(!iPluginWaiter->IsActive(), OomMonitorPanic(KStartingActivePluginWaiter));    
    iPluginWaiter->Start();
    }

// Call the memory good function on the plugin but...
// only if there is an outstanding FreeMemory request
void COomRunPlugin::MemoryGood()
    {
    FUNC_LOG;

    if (iFreeMemoryCalled)
        {
        iPlugin.MemoryGood();
        iFreeMemoryCalled = EFalse;
        }
    }

COomRunPlugin::~COomRunPlugin()
    {
    FUNC_LOG;

    delete iPluginWaiter;
    }

COomRunPlugin::COomRunPlugin(TUint aPluginId, COomRunPluginConfig& aConfig, MOomActionObserver& aStateChangeObserver, COomMonitorPlugin& aPlugin, COomMonitorPluginV2* aV2Plugin) : COomAction(aStateChangeObserver), iPluginId(aPluginId), iPlugin(aPlugin), iConfig(aConfig), iV2Plugin(aV2Plugin)
    {
    FUNC_LOG;
    }

void COomRunPlugin::ConstructL(COomRunPluginConfig& aPluginConfig)
    {
    FUNC_LOG;

    TInt waitDuration = CMemoryMonitor::GlobalConfig().iDefaultWaitAfterPlugin;
    
    if (aPluginConfig.WaitAfterPluginDefined())
        {
        // If the wait duration for this plugin is overridden then use the overridden value
        waitDuration = aPluginConfig.WaitAfterPlugin();
        }
    
    iPluginWaiter = COomPluginWaiter::NewL(waitDuration, *this);
    }

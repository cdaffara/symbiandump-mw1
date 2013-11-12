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
* Description:  Configuration representation classes for Out of Memory Monitor.
*
*/


#include "oomrunpluginconfig.h"
#include "oomwindowgrouplist.h"
#include "OomTraces.h"

COomRunPluginConfig* COomRunPluginConfig::NewL(TUint aPluginId, TOomPluginType aPluginType)
    {
    FUNC_LOG;

    COomRunPluginConfig* self = new (ELeave) COomRunPluginConfig(aPluginId, aPluginType);
    return self;
    }

TUint COomRunPluginConfig::CalculatePluginPriority(const COomWindowGroupList& aWindowGroupList)
    {
    FUNC_LOG;

    // Call the Priority function on the CActionConfig base class
    // This function will check if any rules match the current system state and then adjust the priority if they do
    // Rules may apply to system plugins or application plugins
    return Priority(aWindowGroupList, aWindowGroupList.GetIndexFromAppId(iTargetAppId));
    }


COomRunPluginConfig::~COomRunPluginConfig()
    {
    FUNC_LOG;
    }
    

COomRunPluginConfig::COomRunPluginConfig(TUint aPluginId, TOomPluginType aPluginType) : COomActionConfig(aPluginId), iPluginId(aPluginId), iPluginType(aPluginType)
    {
    FUNC_LOG;

    iSyncMode = EContinueIgnoreMaxBatchSize;
    iCallIfTargetAppNotRunning = ETrue;
    }

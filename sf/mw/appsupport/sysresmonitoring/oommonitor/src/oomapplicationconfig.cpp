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



#include "oomapplicationconfig.h"
#include "oomconstants.hrh"
#include "oomcloseappconfig.h"
#include "OomTraces.h"

COomApplicationConfig* COomApplicationConfig::NewL(TUint aApplicationId)
    {
    FUNC_LOG;

    COomApplicationConfig* self = new (ELeave) COomApplicationConfig(aApplicationId);
    CleanupStack::PushL(self);
    self->ConstructL();
    CleanupStack::Pop(self);
    return self;
    }

// Add a rule
// This class takes ownership of the given rule
// This rule applies to this application itself (and not a plugin associated with this application)
// The rule would usually apply to an "application close" event
void COomApplicationConfig::AddRuleL(MOomRuleConfig* aRule)
    {   
    FUNC_LOG;

   __ASSERT_ALWAYS(iCloseAppConfig, OomMonitorPanic(KRuleConfiguredBeforeApplication));
    
    iCloseAppConfig->AddRuleL(aRule);
    }

COomApplicationConfig::~COomApplicationConfig()
    {
    FUNC_LOG;

    delete iCloseAppConfig;
    }

void COomApplicationConfig::ConstructL()
    {
    FUNC_LOG;

    iGoodRamThreshold = KOomThresholdUnset;
    iLowRamThreshold = KOomThresholdUnset;  
    iGoodSwapThreshold = KOomThresholdUnset;
    iLowSwapThreshold = KOomThresholdUnset;  
    }

COomApplicationConfig::COomApplicationConfig(TUint aApplicationId) : iApplicationId(aApplicationId)
    {
    FUNC_LOG;
    }

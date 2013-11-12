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


#include "oomforegroundrule.h"
#include "oomwindowgrouplist.h"
#include "OomTraces.h"

// If the specified target app is in the foreground then apply the specified priority
COomForegroundRule::COomForegroundRule(TInt aTargetAppId, TInt aPriority) : iTargetAppId(aTargetAppId), iPriority(aPriority)
    {
    FUNC_LOG;
    }

TBool COomForegroundRule::RuleIsApplicable(const COomWindowGroupList& aWindowGroupList, TInt /*aAppIndexInWindowGroup*/) const
    {
    FUNC_LOG;

    TBool ruleIsApplicable = EFalse;
    if (aWindowGroupList.Count() > 0)
        {
        // If the target app is in the foreground then this rule is applicable
        TUint foregroundAppId = aWindowGroupList.AppId(0, ETrue);
        if (foregroundAppId == iTargetAppId)
            ruleIsApplicable = ETrue;
        }
    
    return ruleIsApplicable;
    }

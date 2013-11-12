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

#include "oomidletimerule.h"
#include "oomwindowgrouplist.h"
#include "OomTraces.h"

COomIdleTimeRule* COomIdleTimeRule::NewL(TTimeIntervalSeconds aIdleTime, TInt aPriority)
    {
    FUNC_LOG;

    COomIdleTimeRule* self = new (ELeave) COomIdleTimeRule(aIdleTime, aPriority);
    return self;
    }

TBool COomIdleTimeRule::RuleIsApplicable(const COomWindowGroupList& aWindowGroupList, TInt aAppIndexInWindowGroup) const
    {
    FUNC_LOG;

    TBool applicable = EFalse;
    if (aAppIndexInWindowGroup >= 0)
        {
        applicable = (aWindowGroupList.IdleTime(aAppIndexInWindowGroup) >= iIdleTime);
        }
    return applicable;
    }

COomIdleTimeRule::~COomIdleTimeRule()
    {
    FUNC_LOG;
    }

COomIdleTimeRule::COomIdleTimeRule(TTimeIntervalSeconds aIdleTime, TInt aPriority) : iIdleTime(aIdleTime), iPriority(aPriority)
    {
    FUNC_LOG;
    }
    

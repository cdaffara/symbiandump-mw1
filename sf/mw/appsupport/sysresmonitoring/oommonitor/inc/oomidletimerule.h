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


#ifndef OOMIDLETIMERULE_
#define OOMIDLETIMERULE_

#include <e32base.h>

#include "oomruleconfig.h"

/**
 *  A rule to modify the priority of an app close action if it has been idle for a period of time
 *
 *  @lib oommonitor.lib
 *  @since S60 v5.0
 */
NONSHARABLE_CLASS(COomIdleTimeRule) : public CBase, public MOomRuleConfig
    {
public:
    static COomIdleTimeRule* NewL(TTimeIntervalSeconds aIdleTime, TInt aPriority);
    
    TBool RuleIsApplicable(const COomWindowGroupList& aWindowGroupList, TInt aAppIndexInWindowGroup) const;

    inline TUint Priority() const;
    
    ~COomIdleTimeRule();
    
private:
    COomIdleTimeRule(TTimeIntervalSeconds aIdleTime, TInt aPriority);
    
private:
    TTimeIntervalSeconds iIdleTime; // The idle time after which to apply the given priority
    TInt iPriority; // The priority to apply after the given idle time
    };

#include "oomidletimerule.inl"

#endif /*OOMIDLETIMERULE_*/

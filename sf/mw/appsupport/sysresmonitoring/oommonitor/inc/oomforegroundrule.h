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


#ifndef OOMFOREGROUNDRULE_
#define OOMFOREGROUNDRULE_

#include <e32base.h>
#include "oomruleconfig.h"

/**
 *  A rule to modify the priority if a particular application is in the foreground
 *
 *  @lib oommonitor.lib
 *  @since S60 v5.0
 */
NONSHARABLE_CLASS(COomForegroundRule) : public CBase, public MOomRuleConfig
    {
public:
    // If the specified target app is in the foreground then apply the specified priority
    COomForegroundRule(TInt aTargetAppId, TInt aPriority);
    
    TBool RuleIsApplicable(const COomWindowGroupList& aWindowGroupList, TInt aAppIndexInWindowGroup) const;

    inline TUint Priority() const;
        
private:
    TInt iTargetAppId;
    TInt iPriority; // The priority to apply if the specified app is in the foreground
    };

#include "oomforegroundrule.inl"

#endif /*OOMFOREGROUNDRULE_*/

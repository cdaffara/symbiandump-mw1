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


#ifndef OOMRULECONFIG_
#define OOMRULECONFIG_

#include <e32std.h>

class COomWindowGroupList;

NONSHARABLE_CLASS(MOomRuleConfig)
    {
public:
    virtual TBool RuleIsApplicable(const COomWindowGroupList& aWindowGroupList, TInt aAppIndexInWindowGroup) const = 0;
    virtual TUint Priority() const = 0;
    };

#endif /*OOMRULECONFIG_*/

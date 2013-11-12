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


#ifndef OOMACTIONCONFIG_
#define OOMACTIONCONFIG_

#include <e32cmn.h>
#include <e32std.h>
#include <w32std.h>

class MOomRuleConfig;
class COomWindowGroupList;

enum TOomSyncMode
    {
    EContinue,					// Continue with the next action regardless of anything else (exluding max_batch_size)
    EEstimate,					// Continue with the next action if we estimate that we need to free more memory
    ECheckRam,					// Wait for this action to complete, then check the free RAM before continuing
    EContinueIgnoreMaxBatchSize // Continue with the next action regardless of anything else (including max_batch_size)
    };

/*
 * The base class for the configuration of all OOM actions
 * 
 * @lib oommonitor.lib
 * @since S60 v5.0
 */
NONSHARABLE_CLASS(COomActionConfig) : public CBase
    {
public:
    // Add a rule
    // This class takes ownership of the given rule
    void AddRuleL(MOomRuleConfig* aRule);    // Add the configuration for a rule (e.g. an idle time rule)
            
    virtual ~COomActionConfig();
    
    // Set the priority of this action
    // This priority will be applied if none of the attached rules can be applied
    inline void SetDefaultPriority(TUint aPriority);
    
protected:
    
    // Return the priority for this action for the idle time of the target app
    TUint Priority(const COomWindowGroupList& aWindowGroupList, TInt aAppIndexInWindowGroup) const;
    
    void ConstructL();

    COomActionConfig(TInt32 aId);
    
public:
    
    TOomSyncMode iSyncMode;
    
    TInt iRamEstimate;    // The estimated RAM saving after performing this action
    
    TInt32 iId;        // The ID of the affected component (e.g. an application ID for app closure, or a plugin ID for a plugin event)
    
    TUint iDefaultPriority;
    
protected:    
    
    RPointerArray<MOomRuleConfig> iRules;
    };

#include "oomactionconfig.inl"

#endif /*OOMACTIONCONFIG_*/

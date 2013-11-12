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


#ifndef OOMGLOBALCONFIG_
#define OOMGLOBALCONFIG_

#include <e32base.h>
#include <e32hashtab.h>

/**
 *  Class presenting the parts of the configuration that are global defaults rather than specific
 *  to the current state of the device
 *
 *  @lib oommonitor.lib
 *  @since S60 v5.0
 */
NONSHARABLE_CLASS(COomGlobalConfig) : public CBase
    {
public:
    ~COomGlobalConfig();
    
    inline void AddForceCheckPriorityL(TUint aPriority);
    
    // Return ETrue if a force check has been added for this priority, return EFalse otherwise
    inline TBool ForceCheckAtPriority(TUint aPriority) const;
    
public:
    TInt iLowRamThreshold;
    TInt iGoodRamThreshold;
    TBool iSwapUsageMonitored;
    TInt iLowSwapThreshold;
    TInt iGoodSwapThreshold;
    TUint iMaxCloseAppBatch;
    TInt iDefaultWaitAfterPlugin;
    TInt iMaxAppExitTime;
    
    RHashSet<TUint> iForceCheckPriorities;
    };

#include "oomglobalconfig.inl"

#endif /*OOMGLOBALCONFIG_*/

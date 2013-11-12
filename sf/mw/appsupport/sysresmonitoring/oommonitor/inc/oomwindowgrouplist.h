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
* Description:  A wrapper for the window group list, adding additional functionality required by OOM Monitor v2.
*
*/


#ifndef OOMWINDOWGROUPLIST_H_
#define OOMWINDOWGROUPLIST_H_

#include <e32base.h>
#include <e32hashtab.h>
#include <w32std.h>

class CApaWindowGroupName;

/**
 *  This class holds a snapshot of the window group tree. 
 *  
 *  The window group tree is collapsed (see CollapseWindowGroupTree) to remove child windows as we are only 
 *  interested in a single window group Id per application.
 *
 *  @lib oommonitor.lib
 *  @since S60 v5.0
 */
NONSHARABLE_CLASS(COomWindowGroupList) : public CBase
    {
public:
    
    static COomWindowGroupList* NewL(RWsSession& aWs);
    
    // Update the list of window groups
    void RefreshL();
    
    // Update the list of window groups, non-leaving version
    void Refresh();
    
    // Return the number of application instances in this list
    inline TUint Count() const;
    
    TUint AppId(TInt aIndex, TBool aResolveFromThread = EFalse) const;
    
    inline const RWsSession::TWindowGroupChainInfo& WgId(TInt aIndex) const;
    
    TTimeIntervalSeconds IdleTime(TInt aIndex) const;
  
    ~COomWindowGroupList();
    
    void SetPriorityBusy(TInt aWgId);
    
    void SetPriorityNormal(TInt aWgId);
    
    void SetPriorityHigh(TInt aWgId);
    
    TBool IsBusy(TInt wgIndex);
    
    // Returns ETrue if an application has registered itself as high priority at runtime
    TBool IsDynamicHighPriority(TInt wgIndex);
    
    CApaWindowGroupName* WgName() const;
    
    // Find the specificed application in the window group list and return the index
    TInt GetIndexFromAppId(TUint aAppId) const;
    
private:    

    void CollapseWindowGroupTree();
    
    void RemovePropertiesForClosedWindowsL();

    TInt FindParentIdL(TInt aWgId);

private:
    
    COomWindowGroupList(RWsSession& aWs);
    
    void ConstructL();
    
    RArray<RWsSession::TWindowGroupChainInfo> iWgIds;
    RArray<RWsSession::TWindowGroupChainInfo> iUncollapsedWgIds;
    
    enum TOomPriority
        {
        EOomPriorityNormal,
        EOomPriorityHigh,
        EOomPriorityBusy
        };
    
    class TOomWindowGroupProperties
        {
    public:
        TOomWindowGroupProperties();
        TUint32 iIdleTickTime;
        TOomPriority iDynamicPriority;
        };
        
    RHashMap<TInt, TOomWindowGroupProperties> iWgToPropertiesMapping; // A mapping between window group IDs and the properties such as idle time and the high-priority flag
    RHashSet<TInt> iExistingWindowIds; // Used locally in RemoveIdleTimesForClosedWindows(), declared globally because we need to reserve space    
    
    RWsSession& iWs;    
        
    CApaWindowGroupName* iWgName;
    HBufC* iWgNameBuf;              // owned by iWgName
    };

#include "oomwindowgrouplist.inl"

#endif /*OOMWINDOWGROUPLIST_H_*/

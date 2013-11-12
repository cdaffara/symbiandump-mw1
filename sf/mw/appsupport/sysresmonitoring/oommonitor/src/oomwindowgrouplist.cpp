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

#include <w32std.h>
#include <e32std.h>
#include <apgtask.h>
#include <apgwgnam.h>

#include "oomwindowgrouplist.h"
#include "OomTraces.h"
#include "oomconstants.hrh"
#include "oompanic.h"

_LIT(KDummyWgName, "20");
const TInt KPreallocatedSpaceForAppList = 50;

const TUint KOomTicksPerSecond = 1000;

COomWindowGroupList::TOomWindowGroupProperties::TOomWindowGroupProperties() :  iIdleTickTime(0), iDynamicPriority(EOomPriorityNormal)
    {
    FUNC_LOG;
    }

// Update the list of window groups
void COomWindowGroupList::Refresh()
    {
    FUNC_LOG;
    
#ifdef _DEBUG
    TRAPD(err, RefreshL());
    if (err)
        {
        TRACES1("COomWindowGroupList::Refresh(): RefreshL leave %d", err);
        }
#else
    TRAP_IGNORE(RefreshL());
    // Ignore any error
    // Errors are very unlikely, the only possibility is OOM errors (which should be very unlikely due to pre-created, re-reserved lists)
    // The outcome of any error is that the most foreground operations will be missing from the list
    // meaning that they will not be considered candidates for closing
#endif    
    }

// Update the list of window groups
// Should be called whenever the 
void COomWindowGroupList::RefreshL()
    {
    FUNC_LOG;

    // Refresh window group list
    // get all window groups, with info about parents
    TInt numGroups = iWs.NumWindowGroups(0);
    iWgIds.ReserveL(numGroups);
    User::LeaveIfError(iWs.WindowGroupList(0, &iWgIds));
    
    // Remove all child window groups, promote parents to foremost child position
    CollapseWindowGroupTree();
    
    // Note the current foreground window ID (if there is one)
    TBool oldForegroundWindowExists = EFalse;

    TInt oldForegroundWindowId;
    if (iWgIds.Count() > 0)
        {
        oldForegroundWindowId = iWgIds[0].iId;
        oldForegroundWindowExists = ETrue;
        }    
    
    // Cleanup the idletime hash map to remove idle times for any windows that have closed
    RemovePropertiesForClosedWindowsL();
    
    // Update the idle tick on the old foreground application (which might now be in the background)
    // This will be set to the current system tick count and will be used later to determine the idle time
    if (oldForegroundWindowExists)
        {
        TOomWindowGroupProperties* wgProperties = iWgToPropertiesMapping.Find(oldForegroundWindowId);
        if (wgProperties)
            {
            wgProperties->iIdleTickTime = User::NTickCount();
            }
        
        // If there is no idle tick entry for this window ID then it will be created in the next step...
        }
    
    TInt index = iWgIds.Count();
    
    while (index--)
        {
        // See if there is a tick count entry for each window in the list
        TOomWindowGroupProperties* wgProperties = iWgToPropertiesMapping.Find(iWgIds[index].iId);
        
        if (!wgProperties)
            {
            TOomWindowGroupProperties wgProperties;
            wgProperties.iIdleTickTime = User::NTickCount();
            // If there is no idle tick entry for this window then add one
            iWgToPropertiesMapping.InsertL(iWgIds[index].iId, wgProperties);
            }
        }
    }



void COomWindowGroupList::RemovePropertiesForClosedWindowsL()
    {
    FUNC_LOG;
    
    // First, clear the existing set of window IDs (it would be quicker to delete it BUT we have reserved memory for it and don't want to be allocating in low memory conditions)
    RHashSet<TInt>::TIter windowIdSetIter(iExistingWindowIds);
    while (windowIdSetIter.Next())
        {
        windowIdSetIter.RemoveCurrent();
        }
    
    // Create the set of existing window IDs (this saves expensive/repeated searching later on)
    TInt index = iWgIds.Count();
    while (index--)
        {
        iExistingWindowIds.InsertL(iWgIds[index].iId);
        }
    
    // Iterate the idle-time hash map - remove any items where the window no longer exists
    RHashMap<TInt, TOomWindowGroupProperties>::TIter wgToIdleIterator(iWgToPropertiesMapping);
    while (wgToIdleIterator.NextKey())
        {
        // If the current key (window ID) does not exist in the set then remove the idle-time as it is no longer relevant
        if (!iExistingWindowIds.Find(*(wgToIdleIterator.CurrentKey())))
                wgToIdleIterator.RemoveCurrent();
        }
    }


TUint COomWindowGroupList::AppId(TInt aIndex, TBool aResolveFromThread) const
    {
    FUNC_LOG;

    // get the app's details
    TPtr wgPtr(iWgNameBuf->Des());
    
    TUid uid;
    
    __ASSERT_DEBUG(aIndex < iWgIds.Count(), OomMonitorPanic(KWindowGroupArrayIndexOutOfBounds));    
    TInt wgId = iWgIds[aIndex].iId;
    
    TInt err = iWs.GetWindowGroupNameFromIdentifier(wgId, wgPtr);
    
    if (KErrNone != err)
        // If there is an error then set the UID to 0;
        {
        uid.iUid = 0;
        }
    else 
        {
        iWgName->SetWindowGroupName(iWgNameBuf); // iWgName takes ownership of iWgNameBuf
        uid = iWgName->AppUid(); // This UID comes from the app, not the mmp!
        if (aResolveFromThread && uid.iUid == 0)
            {
            TApaTask task(iWs);
            task.SetWgId(wgId);
            TThreadId threadId = task.ThreadId();
            
            TUint resolvedUid = 0;
            RThread appThread;
            TInt err = appThread.Open( threadId );
            if ( err == KErrNone )
                {
                resolvedUid = appThread.SecureId().iId;
                }
            appThread.Close();
            TRACES2("COomWindowGroupList::AppId: NULL wg UID, taking from thread; resolvedUid = %x aIndex = %d", resolvedUid, aIndex);            
            return resolvedUid;               
            }
        }
    
    return uid.iUid;
    }
    

TTimeIntervalSeconds COomWindowGroupList::IdleTime(TInt aIndex) const
    {
    FUNC_LOG;

    TUint32 currentTickCount = User::NTickCount();
        
    __ASSERT_DEBUG(aIndex < iWgIds.Count(), OomMonitorPanic(KWindowGroupArrayIndexOutOfBounds));    
    const TOomWindowGroupProperties* wgProperties = iWgToPropertiesMapping.Find(iWgIds[aIndex].iId);
    
    TTimeIntervalSeconds idleTime = 0;
    
    if (wgProperties)
        {
        // This should also handle the case where the current tick count has wrapped to a lower value than the idle tick time
        // It will only work if it has wrapped once, but
        TUint32 differenceBetweenTickCounts = currentTickCount - wgProperties->iIdleTickTime;
        idleTime = differenceBetweenTickCounts / KOomTicksPerSecond;
        }
    
    return idleTime;
    }
  


void COomWindowGroupList::CollapseWindowGroupTree()
    {
    FUNC_LOG;

    // start from the front, wg count can reduce as loop runs
    for (TInt ii=0; ii<iWgIds.Count();)
        {
        RWsSession::TWindowGroupChainInfo& info = iWgIds[ii];
        if (info.iParentId > 0)        // wg has a parent
            {
            // Look for the parent position
            TInt parentPos = ii;        // use child pos as not-found signal
            TInt count = iWgIds.Count();
            for (TInt jj=0; jj<count; jj++)
                {
                if (iWgIds[jj].iId == info.iParentId)
                    {
                    parentPos = jj;
                    break;
                    }
                }

            if (parentPos > ii)  // parent should be moved forward
                {
                iWgIds[ii] = iWgIds[parentPos];
                iWgIds.Remove(parentPos);
                }
            else if (parentPos < ii)  // parent is already ahead of child, remove child
                iWgIds.Remove(ii);
            else                    // parent not found, skip
                ii++;
            }
        else    // wg does not have a parent, skip
            ii++;
        }    
    }



COomWindowGroupList::COomWindowGroupList(RWsSession& aWs) : iWs(aWs)
    {
    FUNC_LOG;
    }



void COomWindowGroupList::ConstructL()
    {
    FUNC_LOG;

    // Reserve enough space to build an app list later.
    iWgIds.ReserveL(KPreallocatedSpaceForAppList);
    iUncollapsedWgIds.ReserveL(KPreallocatedSpaceForAppList);
    
    // Reserve enough space for the WG to idle tick mapping
    iWgToPropertiesMapping.ReserveL(KPreallocatedSpaceForAppList);
    
    // Reserve enough space for CApaWindowGroupName.
    iWgName = CApaWindowGroupName::NewL(iWs);
    iWgNameBuf = HBufC::NewL(CApaWindowGroupName::EMaxLength);
    (*iWgNameBuf) = KDummyWgName;
    iWgName->SetWindowGroupName(iWgNameBuf);    // iWgName takes ownership of iWgNameBuf
    
    RefreshL();
    }



COomWindowGroupList* COomWindowGroupList::NewL(RWsSession& aWs)
    {
    FUNC_LOG;

    COomWindowGroupList* self = new (ELeave) COomWindowGroupList(aWs);
    CleanupStack::PushL(self);
    self->ConstructL();
    CleanupStack::Pop(self);
    return self;
    }



COomWindowGroupList::~COomWindowGroupList()
    {
    FUNC_LOG;

    iWgIds.Close();
    iUncollapsedWgIds.Close();
    iWgToPropertiesMapping.Close();
    iExistingWindowIds.Close();
    delete iWgName;
    }


void COomWindowGroupList::SetPriorityBusy(TInt aWgId)
    {
    FUNC_LOG;

    Refresh();
    
    TInt parentId;
    TRAPD(err, parentId = FindParentIdL(aWgId));
    if (err)
        {
        parentId = aWgId;
        }
    
    TOomWindowGroupProperties* wgProperties = iWgToPropertiesMapping.Find(parentId);
    if (wgProperties)
        {
        wgProperties->iDynamicPriority = EOomPriorityBusy;
        }
    
    // If we can't find the window group then ignore it
    }


TInt COomWindowGroupList::FindParentIdL(TInt aWgId)
    {
    TInt numGroups = iWs.NumWindowGroups(0);
    iUncollapsedWgIds.ReserveL(numGroups);
    User::LeaveIfError(iWs.WindowGroupList(0, &iUncollapsedWgIds));

    TInt parentPos = KErrNotFound;
   
    //loop through the window group list
    for (TInt i=0; i<numGroups; i++)
        {
        //find the index for the required aWgId
        __ASSERT_DEBUG(i < iUncollapsedWgIds.Count(), OomMonitorPanic(KWindowGroupArrayIndexOutOfBounds));    
        if (iUncollapsedWgIds[i].iId == aWgId)
            {
            parentPos = i;
            break;
            }
        }

    if (parentPos >=0 )
        {
        __ASSERT_DEBUG(parentPos < iUncollapsedWgIds.Count(), OomMonitorPanic(KWindowGroupArrayIndexOutOfBounds));    
        while (iUncollapsedWgIds[parentPos].iParentId > 0)
            {
            // find the index for the parent
            for (TInt j=0; j<numGroups; j++)
                {
                __ASSERT_DEBUG(j < iUncollapsedWgIds.Count(), OomMonitorPanic(KWindowGroupArrayIndexOutOfBounds));    
                if (iUncollapsedWgIds[j].iId == iUncollapsedWgIds[parentPos].iParentId)
                    {
                    parentPos = j;
                    __ASSERT_DEBUG(parentPos < iUncollapsedWgIds.Count(), OomMonitorPanic(KWindowGroupArrayIndexOutOfBounds));    
                    break; // break out of inner loop
                    }
                }
            }
        __ASSERT_DEBUG(parentPos < iUncollapsedWgIds.Count(), OomMonitorPanic(KWindowGroupArrayIndexOutOfBounds));    
        return iUncollapsedWgIds[parentPos].iId;
        }
    else 
        {
        return KErrNotFound;
        }
    }


TBool COomWindowGroupList::IsBusy(TInt aWgIndex)
    {
    TBool isBusy = EFalse;
    __ASSERT_DEBUG(aWgIndex < iWgIds.Count(), OomMonitorPanic(KWindowGroupArrayIndexOutOfBounds));    
    TOomWindowGroupProperties* wgProperties = iWgToPropertiesMapping.Find(iWgIds[aWgIndex].iId);
    if (wgProperties)
        {
        isBusy = (wgProperties->iDynamicPriority == EOomPriorityBusy);
        }
    
    return isBusy;
    }

// Returns ETrue if an application has registered itself as high priority at runtime
TBool COomWindowGroupList::IsDynamicHighPriority(TInt aWgIndex)
    {
    FUNC_LOG;

    TBool isHighPriority = EFalse;
    __ASSERT_DEBUG(aWgIndex < iWgIds.Count(), OomMonitorPanic(KWindowGroupArrayIndexOutOfBounds));    
    TOomWindowGroupProperties* wgProperties = iWgToPropertiesMapping.Find(iWgIds[aWgIndex].iId);
    if (wgProperties)
        {
        isHighPriority = (wgProperties->iDynamicPriority == EOomPriorityHigh);
        }
    
    return isHighPriority;
    }

CApaWindowGroupName* COomWindowGroupList::WgName() const
    {
    return iWgName;
    }

void COomWindowGroupList::SetPriorityNormal(TInt aWgId)
    {
    FUNC_LOG;

    Refresh();
    
    TInt parentId;
    TRAPD(err, parentId = FindParentIdL(aWgId));
    if (err)
        {
        parentId = aWgId;
        }
    
    TOomWindowGroupProperties* wgProperties = iWgToPropertiesMapping.Find(parentId);
    if (wgProperties)
        {
        wgProperties->iDynamicPriority = EOomPriorityNormal;
        }
    
    // If we can't find the window group then ignore it
    }


void COomWindowGroupList::SetPriorityHigh(TInt aWgId)
    {
    FUNC_LOG;

    Refresh();

    TInt parentId;
    TRAPD(err, parentId = FindParentIdL(aWgId));
    if (err)
        {
        parentId = aWgId;
        }
    
    TOomWindowGroupProperties* wgProperties = iWgToPropertiesMapping.Find(parentId);
    if (wgProperties)
        {
        wgProperties->iDynamicPriority = EOomPriorityHigh;
        }
    
    // If we can't find the window group then ignore it
    }

// Find the specificed application in the window group list and return the index
TInt COomWindowGroupList::GetIndexFromAppId(TUint aAppId) const
    {
    FUNC_LOG;

    TInt indexInGroupList = Count();
    TBool appFoundInWindowGroupList = EFalse;
    
    while (indexInGroupList--)
        {
        if (AppId(indexInGroupList, ETrue) == aAppId)
            {
            appFoundInWindowGroupList = ETrue;
            break;
            }
        }
    
    if (!appFoundInWindowGroupList)
        indexInGroupList = KAppNotInWindowGroupList;

    return indexInGroupList;
    }

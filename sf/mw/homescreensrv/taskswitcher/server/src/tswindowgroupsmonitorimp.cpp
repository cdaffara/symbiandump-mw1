/*
 * Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies).
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
 * Description :
 *
 */
#include <apgwgnam.h>

#include "tswindowgroupsmonitorimp.h"
#include "tsrunningappstorageimp.h"
#include "tsresourcemanager.h"


const int KOrdinalPositionNoZOrder(-1);
// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
CTsWindowGroupsMonitor* CTsWindowGroupsMonitor::NewL(MTsResourceManager &resources)
{
    CTsWindowGroupsMonitor* self = new (ELeave) CTsWindowGroupsMonitor(resources);
    CleanupStack::PushL(self);
    self->ConstructL();
    CleanupStack::Pop(self);
    return self;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
CTsWindowGroupsMonitor::CTsWindowGroupsMonitor(MTsResourceManager &resources)
:
    CActive(EPriorityStandard),
    iResources(resources)
{
    CActiveScheduler::Add(this);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void CTsWindowGroupsMonitor::ConstructL()
{
    
    // Initial window group
    iWg = RWindowGroup (iResources.WsSession());
    User::LeaveIfError (iWg.Construct ((TUint32)&iWg, EFalse));
    iWg.SetOrdinalPosition (KOrdinalPositionNoZOrder);
    iWg.EnableReceiptOfFocus (EFalse);
    
    // Hide window
    CApaWindowGroupName* wn = CApaWindowGroupName::NewLC(iResources.WsSession());
    wn->SetHidden (ETrue);
    wn->SetWindowGroupName (iWg);
    CleanupStack::PopAndDestroy (wn);
    
    // Window group change event
    User::LeaveIfError (iWg.EnableGroupListChangeEvents());
    Subscribe();
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
CTsWindowGroupsMonitor::~CTsWindowGroupsMonitor()
{
    CActive::Cancel();
    iWg.Close();
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void CTsWindowGroupsMonitor::SubscribeL(MTsWindowGroupsObserver &observer)
{
    const TInt offset(iObservers.Find(&observer));
    KErrNotFound == offset ? iObservers.InsertL(&observer, 0) : 
                             User::Leave(KErrAlreadyExists);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void CTsWindowGroupsMonitor::Cancel(MTsWindowGroupsObserver & observer)
{
    const TInt offset(iObservers.Find(&observer));
    if (KErrNotFound != offset) {
        iObservers.Remove(offset);
    }
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void CTsWindowGroupsMonitor::RunL()
{
    User::LeaveIfError(iStatus.Int());
    ProvideEventL();
    Subscribe();
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void CTsWindowGroupsMonitor::DoCancel()
{
    if (IsActive()) {
        iResources.WsSession().EventReadyCancel();
    }
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
TInt CTsWindowGroupsMonitor::RunError(TInt error)
{
    if (!IsActive() && KErrCancel != error) {
        Subscribe();
    }
    return KErrNone;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void CTsWindowGroupsMonitor::Subscribe()
{
    iResources.WsSession().EventReady( &iStatus );
    SetActive();
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void CTsWindowGroupsMonitor::ProvideEventL()
{
    TWsEvent wsEvent;
    iResources.WsSession().GetEvent(wsEvent);
    if( EEventWindowGroupListChanged == wsEvent.Type() )
        {
        
        CTsRunningAppStorage *storage = CTsRunningAppStorage::NewLC();
        
        RArray<RWsSession::TWindowGroupChainInfo> filteredWgInfo, fullWgInfo;
        CleanupClosePushL(filteredWgInfo);
        CleanupClosePushL(fullWgInfo);
        User::LeaveIfError(iResources.WsSession().WindowGroupList(0, &filteredWgInfo));
        User::LeaveIfError(iResources.WsSession().WindowGroupList(&fullWgInfo));
        storage->HandleWindowGroupChanged(iResources, 
                                          fullWgInfo.Array(), 
                                          filteredWgInfo.Array());
        CleanupStack::PopAndDestroy( &fullWgInfo );
        CleanupStack::PopAndDestroy( &filteredWgInfo );
                
        for( TInt iter(0); iter < iObservers.Count(); ++iter ) 
            {
            iObservers[iter]->HandleWindowGroupChanged( iResources, *storage );
            }
        CleanupStack::PopAndDestroy( storage );
        
        }
}

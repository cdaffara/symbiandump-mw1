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
#include "tsrunningappstorageimp.h"
//------------------------------------------------------------------------------
CTsRunningAppStorage* CTsRunningAppStorage::NewLC()
    {
    CTsRunningAppStorage* self = new(ELeave) CTsRunningAppStorage();
    CleanupStack::PushL( self );
    return self;
    }

//------------------------------------------------------------------------------
CTsRunningAppStorage::CTsRunningAppStorage()
    {
    //No implementation required
    }

//------------------------------------------------------------------------------
CTsRunningAppStorage::~CTsRunningAppStorage()
{
    iRunningApps.ResetAndDestroy();
}

//------------------------------------------------------------------------------
void CTsRunningAppStorage::HandleWindowGroupChanged( 
               MTsResourceManager &aResources, 
               const TArray<RWsSession::TWindowGroupChainInfo> & aWindowGroups )
    {
    CTsRunningApp* app(0);
    iRunningApps.ResetAndDestroy();
    iBlockedWindowGroups.Reset();
    TRAP_IGNORE(
    for( TInt iter(0); iter < aWindowGroups.Count(); ++iter )
        {
        app = CTsRunningApp::NewLC(aResources, aWindowGroups[iter]);
        iRunningApps.AppendL(app);
        CleanupStack::Pop(app);
        }
    )//TRAP_IGNORE
    }

//------------------------------------------------------------------------------
void CTsRunningAppStorage::HandleWindowGroupChanged( 
           MTsResourceManager &aResources, 
           const TArray<RWsSession::TWindowGroupChainInfo>& aFull,
           const TArray<RWsSession::TWindowGroupChainInfo>& aFiltered)
    {
    TInt filtered(0);
    HandleWindowGroupChanged(aResources, aFiltered);
    for(TInt full(0); full < aFull.Count(); ++full)
        {
        for(filtered = 0; filtered < aFiltered.Count(); ++filtered)
            {
            if(aFull[full].iId == aFiltered[ filtered].iId)
                {
                break;
                }
            }
        if(aFiltered.Count() == filtered)
            {
            iBlockedWindowGroups.Append(aFull[full].iId);
            }
        }
    }
//------------------------------------------------------------------------------
const MTsRunningApplication& CTsRunningAppStorage::operator[] (TInt aOffset) const
    {
    return *iRunningApps[aOffset];
    }

//------------------------------------------------------------------------------
TInt CTsRunningAppStorage::Count() const
    {
    return iRunningApps.Count();
    }

//------------------------------------------------------------------------------
TInt CTsRunningAppStorage::ParentIndex( const MTsRunningApplication& aRunningApp ) const
    {
    const TInt applicationIndex(Find(aRunningApp.WindowGroupId()));
    return KErrNotFound == applicationIndex ? 
           applicationIndex : ParentIndex(applicationIndex);
    }

//------------------------------------------------------------------------------
TArray<TInt> CTsRunningAppStorage::BlockedWindowGroups() const
    {
    return iBlockedWindowGroups.Array();
    }

//------------------------------------------------------------------------------
TInt CTsRunningAppStorage::ParentIndex( TInt aOffset ) const
    {
    TInt retval(aOffset);
    if(iRunningApps[aOffset]->IsEmbeded())
        {
        const TInt parentIndex( Find(iRunningApps[aOffset]->ParentWindowGroupId(), 
                                aOffset + 1) );
        if( KErrNotFound != parentIndex )
            {
            retval = ParentIndex( parentIndex );
            }
        }
    return retval;
    }

//------------------------------------------------------------------------------
TInt CTsRunningAppStorage::Find(TInt aWindowGroupId, TInt aOffset) const
    {
    TInt retVal(KErrNotFound);
    for( TInt iter(aOffset); 
         KErrNotFound == retVal && iter < iRunningApps.Count(); 
         ++iter )
        {
        if( iRunningApps[iter]->WindowGroupId() == aWindowGroupId )
            {
            retVal = iter;
            }
        }
    return retVal;
    }



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
* Description:
*
*/
#include <tstaskmonitorglobals.h>
#include "tsstorage.h"
#include "tsmodelitemkeymsg.h"

const TInt KTsDataLimit(10);
// -----------------------------------------------------------------------------
/**
 * Two phase construction. Create and initialize storage instance.
 * @param aDataProviders - list of data providers
 * @return storage instane
 */
CTsStorage* CTsStorage::NewL(const TArray<MTsModel*> &aDataProviders)
    {
    CTsStorage* self = new(ELeave) CTsStorage();
    CleanupStack::PushL( self );
    self->ConstructL( aDataProviders );
    CleanupStack::Pop( self );
    return self;
    }

// -----------------------------------------------------------------------------
/**
 * Destructor. Function cancel subscribtion for data change notyfications
 */
CTsStorage::~CTsStorage()
    {
    for( TInt iter(0); iter < iDataProviders.Count(); ++iter )
        {
        iDataProviders[iter]->SetObserver( 0 );
        }
    iData.Close();
    }

// -----------------------------------------------------------------------------
/**
 * First phase construction.
 */
CTsStorage::CTsStorage()
    {
    //No implementation required
    }

// -----------------------------------------------------------------------------
/**
 * Second phase construction. Function make subscribtion for data changes notifications
 * @param aDataProviders - list of data providers
 */
void CTsStorage::ConstructL( const TArray<MTsModel*> &aDataProviders )
    {
    for (TInt iter(0); iter < aDataProviders.Count(); ++iter)
        {
        iDataProviders.AppendL( aDataProviders[iter] );
        aDataProviders[iter]->SetObserver( this );
        }
    }

// -----------------------------------------------------------------------------
/**
 * Interface implementation
 * @see MTsDataObserver::DataChanged()
 */
void CTsStorage::DataChanged()
    {
    ResetModel();
    }

// -----------------------------------------------------------------------------
TBool CTsStorage::IsSupported( TInt aFunction ) const
    {
    return ( OpenTaskMessage == aFunction || CloseTaskMessage == aFunction );
    }

// -----------------------------------------------------------------------------
void CTsStorage::HandleDataL( TInt aFunction, RReadStream& aDataStream )
    {
    if( !IsSupported(aFunction) ) 
        {
        User::Leave( KErrCorrupt );
        }
    CTsModelItemKeyMsg* msg = CTsModelItemKeyMsg::NewLC( aDataStream );
    OpenTaskMessage == aFunction ? LaunchL( msg->Key() ) : CloseL( msg->Key() );
    CleanupStack::PopAndDestroy( msg );
    }

// -----------------------------------------------------------------------------
/**
 * Interface implementation
 * @see MTsModelObserver::dataChanged(MTsModel &)
 */
void CTsStorage::DataChanged(MTsModel &/*model*/)
    {
    ResetModel();
    }

// -----------------------------------------------------------------------------
/**
 * Interface implementation
 * @see MTsModel::count()
 */
TInt CTsStorage::Count() const
    {
    return iData.Count();
    }

// -----------------------------------------------------------------------------
/**
 * Interface implementation
 * @see MTsModel::SetObserver(MTsModelObserver *)
 */
void CTsStorage::SetObserver( MTsModelObserver* aObserver ) 
    {
    iDataObserver = aObserver;
    }

// -----------------------------------------------------------------------------
/**
 * Interface implementation
 * @see MTsModel::DisplayNameL(TInt)
 */
const TDesC& CTsStorage::DisplayNameL( TInt aOffset ) const 
    {
    return iData[aOffset].DisplayNameL();
    }

// -----------------------------------------------------------------------------
/**
 * Interface implementation
 * @see MTsModel::IconHandleL(TInt)
 */
TInt CTsStorage::IconHandleL( TInt aOffset ) const 
    {
    return iData[aOffset].IconHandleL();
    }

// -----------------------------------------------------------------------------
/**
 * Interface implementation
 * @see MTsModel::TimestampL(TInt)
 */
TTime CTsStorage::TimestampL( TInt aOffset ) const 
    {
    return iData[aOffset].TimestampL();
    }

// -----------------------------------------------------------------------------
/**
 * Interface implementation
 * @see MTsModel::TimestampUpdateL(TInt)
 */
TTime CTsStorage::TimestampUpdateL( TInt offset ) const 
{
    return iData[offset].TimestampUpdateL();
}

// -----------------------------------------------------------------------------
/**
 * Interface implementation
 * @see MTsModel::KeyL(TInt)
 */
TTsModelItemKey CTsStorage::KeyL( TInt aoffset ) const 
    {
    return iData[aoffset].KeyL();
    }

// -----------------------------------------------------------------------------
/**
 * Interface implementation
 * @see MTsModel::IsActiveL(TInt)
 */
TBool CTsStorage::IsActiveL( TInt aOffset ) const 
    {
    return iData[aOffset].IsActiveL();
    }

// -----------------------------------------------------------------------------
/**
 * Interface implementation
 * @see MTsModel::IsClosableL(TInt)
 */
TBool CTsStorage::IsClosableL( TInt aOffset ) const 
    {
    return iData[aOffset].IsClosableL();
    }

// -----------------------------------------------------------------------------
/**
 * Interface implementation
 * @see MTsModel::IsMandatoryL(TInt) const
 */
TBool CTsStorage::IsMandatoryL( TInt aOffset ) const
    {
    return iData[aOffset].IsMandatoryL();
    }

// -----------------------------------------------------------------------------
/**
 * Interface implementation
 * @see MTsModel::CloseL(TTsModelItemKey)
 */
TBool CTsStorage::CloseL( TTsModelItemKey aKey ) const 
    {
    return FindL(aKey).CloseL();
    }

// -----------------------------------------------------------------------------
/**
 * Interface implementation
 * @see MTsModel::launchL(TTsModelItemKey)
 */
TBool CTsStorage::LaunchL(TTsModelItemKey aKey) const 
    {
    return FindL(aKey).LaunchL(); 
    }

// -----------------------------------------------------------------------------
TTsModelItem CTsStorage::FindL( TTsModelItemKey aKey ) const
    {
    for( TInt offset(0); offset < iData.Count(); ++offset ) 
        {
        if( iData[offset].KeyL() == aKey )
            {
            return iData[offset];
            }
        }
    User::Leave(KErrNotFound);
    return ItemL(0);//just avoid compilation warnings
    }

// -----------------------------------------------------------------------------
/**
 * Retrieve shallow copy of data from known data providers and sort entries
 */
TInt CTsStorage::ResetModel()
    {
    TRAPD(errNo, ResetModelL());
    return errNo;
    }

// -----------------------------------------------------------------------------
/**
 * Retrieve shallow copy of data from known data providers and sort entries
 */
void CTsStorage::ResetModelL() 
    {
    iData.Reset();
    for(TInt iter(0); iter < iDataProviders.Count(); ++iter )
        {
        PullDataL(*(iDataProviders[iter]));
        }
    ReorderDataL();
    if (0 != iDataObserver)
        {
        iDataObserver->DataChanged(*this);
        }
    }

// -----------------------------------------------------------------------------
/**
 * Retrieve shallow copy of data from source model
 * @param aSrc - source model 
 */
void CTsStorage::PullDataL( const MTsModel& aSrc )
    {
    for( TInt iter(0); iter < aSrc.Count(); ++iter )
        {
        iData.AppendL(aSrc.ItemL(iter));
        }
    }

// -----------------------------------------------------------------------------
/**
 * Sort internal data model 
 */
void CTsStorage::ReorderDataL()
    {
    for( TInt prev(0); prev < iData.Count(); ++prev ) 
        {
        for( TInt next(prev + 1); next < iData.Count(); ++next )
            {
            const TTsModelItem prevItem(iData[prev]), nextItem(iData[next]);
            if( ( !prevItem.IsMandatoryL() && nextItem.IsMandatoryL() ) ||
                ( prevItem.TimestampL() < nextItem.TimestampL() && prevItem.IsMandatoryL() == nextItem.IsMandatoryL() ) )
                {
                iData.Remove(prev);
                iData.InsertL(nextItem, prev);
                
                iData.Remove(next);
                iData.InsertL(prevItem, next);
                }
            }
        }
    TrimDataL();
    }

// -----------------------------------------------------------------------------
void CTsStorage::TrimDataL()
    {
    const TInt lastItemOffset(iData.Count() -1);
    if(KTsDataLimit <= lastItemOffset && !iData[lastItemOffset].IsMandatoryL())
        {
        iData.Remove(lastItemOffset);
        TrimDataL();
        }
    }

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
#include "tsidlist.h"

//------------------------------------------------------------------------------
CTsIdList* CTsIdList::NewLC()
    {
    CTsIdList* self = new(ELeave)CTsIdList();
    CleanupStack::PushL( self );
    return self;
    }

//------------------------------------------------------------------------------
CTsIdList* CTsIdList::NewL()
    {
    CTsIdList* self = CTsIdList::NewLC();
    CleanupStack::Pop( self );
    return self;
    }

//------------------------------------------------------------------------------
CTsIdList* CTsIdList::NewLC(RReadStream& aStream)
    {
    CTsIdList* self = CTsIdList::NewLC();
    aStream >> (*self);
    return self;
    }

//------------------------------------------------------------------------------
CTsIdList::CTsIdList()
    {
    //No implementation required
    }

//------------------------------------------------------------------------------
CTsIdList::~CTsIdList()
    {
    iIds.Reset();
    }

//------------------------------------------------------------------------------
TBool CTsIdList::IsPresent( TInt aId ) const
    {
    return KErrNotFound != iIds.Find(aId);
    }

//------------------------------------------------------------------------------
void CTsIdList::AppendL(const TInt aArray[], TInt aSize )
    {
    for( TInt iter(0); iter < aSize; ++iter )
        {
        AppendL( aArray[iter] );
        }
    }

//------------------------------------------------------------------------------
void CTsIdList::AppendL(const TArray<TInt>& aArray)
    {
    for( TInt iter(0); iter < aArray.Count(); ++iter )
        {
        AppendL( aArray[iter] );
        }
    }

//------------------------------------------------------------------------------
void CTsIdList::AppendL(TInt aId)
    {
    if( !IsPresent( aId ) )
        {
        iIds.AppendL( aId );
        }
    }

//------------------------------------------------------------------------------
void CTsIdList::Reset()
    {
    iIds.Reset();
    }

//------------------------------------------------------------------------------
TInt CTsIdList::Size() const
    {
    return ( iIds.Count() + 1 ) * sizeof( TInt );
    }

//------------------------------------------------------------------------------
void CTsIdList::ExternalizeL(RWriteStream &aStream) const
    {
    aStream.WriteInt32L(iIds.Count());
    for( TInt iter(0); iter < iIds.Count(); ++iter )
        {
        aStream.WriteInt32L(iIds[iter]);
        }
    }

//------------------------------------------------------------------------------
void CTsIdList::InternalizeL(RReadStream &aStream)
    {
    Reset();
    const TInt count( aStream.ReadInt32L());
    for( TInt iter(0); iter < count; ++iter )
        {
        AppendL( aStream.ReadInt32L() );
        }
    }

//------------------------------------------------------------------------------
TBool CTsIdList::operator ==( const CTsIdList& aList) const
    {
    TBool retVal(aList.iIds.Count() == iIds.Count());
    for( TInt offset(0); retVal && offset < iIds.Count(); ++offset )
        {
        retVal = (KErrNotFound != aList.iIds.Find(iIds[offset]));
        }
    return retVal;
    }
//------------------------------------------------------------------------------
TBool CTsIdList::operator !=( const CTsIdList& aList) const
    {
    return !(*this ==aList);
    }

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


// INCLUDES
#include "testcontextdata.h"

#include <S32MEM.H>

// CONSTRUCTION
CTestContextData* CTestContextData::NewL( const TDesC& aKey,
    const TDesC& aValue )
    {
    CTestContextData* self = CTestContextData::NewLC( aKey, aValue );
    CleanupStack::Pop();

    return self;
    }

CTestContextData* CTestContextData::NewLC( const TDesC& aKey,
    const TDesC& aValue )
    {
    CTestContextData* self = new( ELeave ) CTestContextData();
    CleanupStack::PushL( self );

    self->ConstructL( aKey, aValue );

    return self;
    }

// Destructor (virtual by CBase)
CTestContextData::~CTestContextData()
    {
    delete iKey;
    delete iValue;
    }

// Default constructor
CTestContextData::CTestContextData():
    iUid( KTestContextDataUid )
    {
    }

// Second phase construct
void CTestContextData::ConstructL( const TDesC& aKey, const TDesC& aValue )
    {
    iKey = aKey.AllocL();
    iValue = aValue.AllocL();
    }

//  METHODS

void CTestContextData::SetUid( const TUid& aUid )
    {
    iUid = aUid;
    }
    
void CTestContextData::SetKeyL( const TDesC& aKey )
    {
    HBufC* key = aKey.AllocL();
    delete iKey;
    iKey = key;
    }

void CTestContextData::SetValueL( const TDesC& aValue )
    {
    HBufC* value = aValue.AllocL();
    delete iValue;
    iValue = value;
    }
    
const TDesC& CTestContextData::Key() const
    {
    if( iKey )
        {
        return *iKey;
        }
    else
        {
        return KNullDesC;
        }
    }

const TDesC& CTestContextData::Value() const
    {
    if( iValue )
        {
        return *iValue;
        }
    else
        {
        return KNullDesC;
        }
    }

const TUid& CTestContextData::Uid() const
    {
    return iUid;
    }
    
void CTestContextData::ExternalizeL( RWriteStream& aStream )
    {
    TPtrC keyPtr( *iKey );
    aStream.WriteInt16L( keyPtr.Length() );
    aStream.WriteL( keyPtr, keyPtr.Length() );

    TPtrC valuePtr( *iValue );
    aStream.WriteInt16L( valuePtr.Length() );
    aStream.WriteL( valuePtr, valuePtr.Length() );
    }

void CTestContextData::InternalizeL( RReadStream& aStream )
    {
    TInt len = 0;
    
    len = aStream.ReadInt16L();
    HBufC* buf = HBufC::NewLC( len );
    TPtr bufPtr( buf->Des() );
    aStream.ReadL( bufPtr, len );
    delete iKey;
    iKey = buf;
    CleanupStack::Pop( buf );
    buf = NULL;
    
    len = aStream.ReadInt16L();
    buf = HBufC::NewLC( len );
    bufPtr.Set( buf->Des() );
    aStream.ReadL( bufPtr, len );
    delete iValue;
    iValue = buf;
    CleanupStack::Pop( buf );
    }

TInt CTestContextData::Size() const
    {
    TInt size = 0;
    
    size += sizeof( TInt );
    size += iKey->Size();

    size += sizeof( TInt );
    size += iValue->Size();
    
    return size;
    }

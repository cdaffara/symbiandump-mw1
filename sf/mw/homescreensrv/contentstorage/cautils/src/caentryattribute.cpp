/*
 * Copyright (c) 2008 Nokia Corporation and/or its subsidiary(-ies).
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
 * Description: Definition of entry attribute
 *
 */

// INCLUDE FILES
#include <s32mem.h>
#include "caarraycleanup.inl"
#include "caentryattribute.h"

// ================= MEMBER FUNCTIONS =======================

// ---------------------------------------------------------
// CCaEntryAttribute::~CMenuItemAttr
// ---------------------------------------------------------
//
CCaEntryAttribute::~CCaEntryAttribute()
    {
    iName.Close();
    iValue.Close();
    }

#ifdef COVERAGE_MEASUREMENT
#pragma CTC SKIP
#endif //COVERAGE_MEASUREMENT (only NewLC used in code)
// ---------------------------------------------------------
// CCaEntryAttribute::NewL
// ---------------------------------------------------------
//
EXPORT_C CCaEntryAttribute* CCaEntryAttribute::NewL( const TDesC& aName )
    {
    CCaEntryAttribute* attr = NewLC( aName );
    CleanupStack::Pop( attr );
    return attr;
    }
#ifdef COVERAGE_MEASUREMENT
#pragma CTC ENDSKIP
#endif //COVERAGE_MEASUREMENT

// ---------------------------------------------------------
// CCaEntryAttribute::NewLC
// ---------------------------------------------------------
//
EXPORT_C CCaEntryAttribute* CCaEntryAttribute::NewLC( const TDesC& aName )
    {
    CCaEntryAttribute* attr = new ( ELeave ) CCaEntryAttribute();
    CleanupStack::PushL( attr );
    attr->ConstructL( aName );
    return attr;
    }

// ---------------------------------------------------------
// CCaEntryAttribute::ConstructL
// ---------------------------------------------------------
//
void CCaEntryAttribute::ConstructL( const TDesC& aName )
    {
    iName.CreateL( aName );
    }

// ---------------------------------------------------------
// CCaEntryAttribute::ConstructL
// ---------------------------------------------------------
//
EXPORT_C void CCaEntryAttribute::SetValueL( const TDesC& aName )
    {
    if( iValue.Length() > 0 )
        {
        iValue.Close();
        }
    iValue.CreateL( aName );
    }

// ---------------------------------------------------------------------------
// CCaEntryAttribute::ExternalizeL
// ---------------------------------------------------------------------------
//
void CCaEntryAttribute::ExternalizeL( RWriteStream& aStream ) const
    {
    aStream.WriteUint32L( iName.Length() );
    aStream.WriteL( iName, iName.Length() );
    aStream.WriteUint32L( iValue.Length() );
    aStream.WriteL( iValue, iValue.Length() );
    aStream.CommitL();
    }

// ---------------------------------------------------------------------------
// CCaEntryAttribute::InternalizeL
// ---------------------------------------------------------------------------
//
void CCaEntryAttribute::InternalizeL( RReadStream& aStream )
    {
    TUint length = aStream.ReadUint32L();
    iName.Close();
    iName.CreateL( length );
    aStream.ReadL( iName, length );
    length = aStream.ReadUint32L();
    iValue.Close();
    iValue.CreateL( length );
    aStream.ReadL( iValue, length );
    }

// ---------------------------------------------------------
// RMenuSrvAttrArray::Find
// ---------------------------------------------------------
//
EXPORT_C TBool RCaEntryAttrArray::Find( const TDesC& aName, TDes& aAttrVal ) const
    {
    for( TInt i = 0; i < Count(); i++ )
        {
        if( aName.Compare( operator[]( i )->Name() ) == KErrNone )
            {
            aAttrVal = operator[]( i )->Value();
            return ETrue;
            }
        }
    return EFalse;
    }

// ---------------------------------------------------------
// RMenuSrvAttrArray::Find
// ---------------------------------------------------------
//
EXPORT_C TBool RCaEntryAttrArray::Find( const TDesC& aName, TPtrC& aAttrVal ) const
    {
    for( TInt i = 0; i < Count(); i++ )
        {
        if( aName.Compare( operator[]( i )->Name() ) == KErrNone )
            {
            aAttrVal.Set( operator[]( i )->Value() );
            return ETrue;
            }
        }
    return EFalse;
    }

// ---------------------------------------------------------
// RCaEntryAttrArray::Exist
// ---------------------------------------------------------
//
EXPORT_C TBool RCaEntryAttrArray::Exist( const TDesC& aName ) const
    {
    for( TInt i = 0; i < Count(); i++ )
        {
        if( aName.Compare( operator[]( i )->Name() ) == KErrNone )
            {
            return ETrue;
            }
        }
    return EFalse;
    }

// ---------------------------------------------------------
// RCaEntryAttrArray::RemoveAttribute
// ---------------------------------------------------------
//
EXPORT_C void RCaEntryAttrArray::RemoveAttribute( const TDesC& aAttrName )
    {
    for( TInt i = 0; i < Count(); i++ )
        {
        if( aAttrName == operator[]( i )->Name() )
            {
            delete operator[]( i );
            Remove( i );
            break;
            }
        }
    }

// ---------------------------------------------------------------------------
// RCaEntryAttrArray::ExternalizeL
// ---------------------------------------------------------------------------
//
void RCaEntryAttrArray::ExternalizeL( RWriteStream& aStream ) const
    {
    aStream.WriteUint16L( Count() );
    for( TInt i = 0; i < Count(); i++ )
        {
        operator[]( i )->ExternalizeL( aStream );
        }
    }

// ---------------------------------------------------------------------------
// RCaEntryAttrArray::InternalizeL
// ---------------------------------------------------------------------------
//
void RCaEntryAttrArray::InternalizeL( RReadStream& aStream )
    {
    ResetAndDestroy();
    TUint count = aStream.ReadUint16L();
    for( TInt i = 0; i < count; i++ )
        {
        CCaEntryAttribute* attr = CCaEntryAttribute::NewLC( KNullDesC );
        attr->InternalizeL( aStream );
        AppendL( attr );
        CleanupStack::Pop( attr );
        }
    }

//  End of File

/*
* Copyright (c) 2006 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  MPresenceInfoFieldValueEnum API object implementation.
 *
*/


#include "presenceinfofieldvalueenumimp.h"
#include "presenceinfofieldvalueenum.h"
#include "ximprbufhelpers.h"
#include "ximpobjecthelpers.h"

// ============================ MEMBER FUNCTIONS =============================

// ---------------------------------------------------------------------------
// CPresenceInfoFieldValueEnumImp::NewLC()
// ---------------------------------------------------------------------------
//
EXPORT_C CPresenceInfoFieldValueEnumImp* CPresenceInfoFieldValueEnumImp::NewLC()
    {
    CPresenceInfoFieldValueEnumImp* self = new ( ELeave ) CPresenceInfoFieldValueEnumImp();
    CleanupStack::PushL( self );
    self->ConstructL();
    return self;
    }

// ---------------------------------------------------------------------------
// CPresenceInfoFieldValueEnumImp::NewFromStreamLC()
// ---------------------------------------------------------------------------
//
XIMPIMP_IMPLEMENT_DATAOBJ_NEWFROMSTREAM( CPresenceInfoFieldValueEnumImp )

// ---------------------------------------------------------------------------
// Implement supported interface access.
// ---------------------------------------------------------------------------
//
XIMPIMP_IF_BASE_GET_INTERFACE_BEGIN( CPresenceInfoFieldValueEnumImp, MPresenceInfoFieldValueEnum )
    XIMPIMP_IF_BASE_GET_CLASS( CXIMPApiDataObjBase, this )
XIMPIMP_IF_BASE_GET_INTERFACE_END()
XIMPIMP_IF_BASE_GET_CONST_INTERFACE_BEGIN( CPresenceInfoFieldValueEnumImp, MPresenceInfoFieldValueEnum )
    XIMPIMP_IF_BASE_GET_CONST_CLASS( CXIMPApiDataObjBase, this )
XIMPIMP_IF_BASE_GET_INTERFACE_END()

// ---------------------------------------------------------------------------
// CPresenceInfoFieldValueEnumImp::~CPresenceInfoFieldValueEnumImp()
// ---------------------------------------------------------------------------
//
CPresenceInfoFieldValueEnumImp::~CPresenceInfoFieldValueEnumImp()
    {
    // nothing
    }

// ---------------------------------------------------------------------------
// CPresenceInfoFieldValueEnumImp::CPresenceInfoFieldValueEnumImp()
// ---------------------------------------------------------------------------
//
CPresenceInfoFieldValueEnumImp::CPresenceInfoFieldValueEnumImp()
    {
    // nothing
    iMaxValue = KMaxTInt32;
    iMinValue = KMinTInt32;
    }

// ---------------------------------------------------------------------------
// CPresenceInfoFieldValueEnumImp::ConstructL()
// ---------------------------------------------------------------------------
//
void CPresenceInfoFieldValueEnumImp::ConstructL()
    {
    // nothing
    }

// ---------------------------------------------------------------------------
// CPresenceInfoFieldValueEnumImp::ExternalizeL()
// ---------------------------------------------------------------------------
//
void CPresenceInfoFieldValueEnumImp::ExternalizeL( RWriteStream& aStream ) const
    {
    aStream.WriteInt32L( iMaxValue );
    aStream.WriteInt32L( iMinValue );
    aStream.WriteInt32L( iValue );
    }

// ---------------------------------------------------------------------------
// CPresenceInfoFieldValueEnumImp::InternalizeL()
// ---------------------------------------------------------------------------
//
EXPORT_C void CPresenceInfoFieldValueEnumImp::InternalizeL( RReadStream& aStream )
    {
    iMaxValue = aStream.ReadInt32L();
    iMinValue = aStream.ReadInt32L();
    iValue    = aStream.ReadInt32L();
    }

// ---------------------------------------------------------------------------
// CPresenceInfoFieldValueEnumImp::EqualsContent()
// ---------------------------------------------------------------------------
//
TBool CPresenceInfoFieldValueEnumImp::EqualsContent( 
        const CXIMPApiDataObjBase& aOtherInstance ) const
    {
    const CPresenceInfoFieldValueEnumImp* tmp =
        TXIMPGetImpClassOrPanic< const CPresenceInfoFieldValueEnumImp >::From( 
                aOtherInstance.Base() );

    TBool same = ETrue;
    TBool x = iMaxValue == tmp->iMaxValue;
    same &= x;

    x = iMinValue == tmp->iMinValue;
    same &= x;

    x = iValue == tmp->iValue;
    same &= x;

    return same;
    }

// ---------------------------------------------------------------------------
// CPresenceInfoFieldValueEnumImp::MinValue()
// ---------------------------------------------------------------------------
//
TInt CPresenceInfoFieldValueEnumImp::MinValue() const
    {
    return iMinValue;
    }

// ---------------------------------------------------------------------------
// CPresenceInfoFieldValueEnumImp::MaxValue()
// ---------------------------------------------------------------------------
//
TInt CPresenceInfoFieldValueEnumImp::MaxValue() const
    {
    return iMaxValue;
    }

// ---------------------------------------------------------------------------
// CPresenceInfoFieldValueEnumImp::Value()
// ---------------------------------------------------------------------------
//
TInt CPresenceInfoFieldValueEnumImp::Value() const
    {
    return iValue;
    }

// ---------------------------------------------------------------------------
// CPresenceInfoFieldValueEnumImp::SetValueL
// ---------------------------------------------------------------------------
//
void CPresenceInfoFieldValueEnumImp::SetValueL( TInt aValue )
    {
    User::LeaveIfError( aValue > iMaxValue ? KErrOverflow : KErrNone ); 
    User::LeaveIfError( aValue < iMinValue ? KErrUnderflow : KErrNone ); 

    iValue = aValue;
    }

// ---------------------------------------------------------------------------
// CPresenceInfoFieldValueEnumImp::SetLimits
// ---------------------------------------------------------------------------
//
void CPresenceInfoFieldValueEnumImp::SetLimits( TInt aMin, TInt aMax )
    {
    iMinValue = aMin;
    iMaxValue = aMax;
    }

// End of file

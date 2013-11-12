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
* Description:  MPresenceInfoFieldValueText API object implementation.
 *
*/


#include "presenceinfofieldvaluetextimp.h"
#include "presenceinfofieldvaluetext.h"
#include "ximprbufhelpers.h"
#include "ximpobjecthelpers.h"
#include "presenceinfo.h"

// ============================ MEMBER FUNCTIONS =============================

// ---------------------------------------------------------------------------
// CPresenceInfoFieldValueTextImp::NewLC()
// ---------------------------------------------------------------------------
//
EXPORT_C CPresenceInfoFieldValueTextImp* CPresenceInfoFieldValueTextImp::NewLC(
        TInt aMaxLength /* = NPresenceInfo::KValueLengthUnlimited */
        )
    {
    CPresenceInfoFieldValueTextImp* self = new ( ELeave ) CPresenceInfoFieldValueTextImp( aMaxLength );
    CleanupStack::PushL( self );
    self->ConstructL();
    return self;
    }

// ---------------------------------------------------------------------------
// CPresenceInfoFieldValueTextImp::NewFromStreamLC()
// ---------------------------------------------------------------------------
//
XIMPIMP_IMPLEMENT_DATAOBJ_NEWFROMSTREAM( CPresenceInfoFieldValueTextImp )

// ---------------------------------------------------------------------------
// Implement supported interface access.
// ---------------------------------------------------------------------------
//
XIMPIMP_IF_BASE_GET_INTERFACE_BEGIN( CPresenceInfoFieldValueTextImp, MPresenceInfoFieldValueText )
    XIMPIMP_IF_BASE_GET_CLASS( CXIMPApiDataObjBase, this )
XIMPIMP_IF_BASE_GET_INTERFACE_END()
XIMPIMP_IF_BASE_GET_CONST_INTERFACE_BEGIN( CPresenceInfoFieldValueTextImp, MPresenceInfoFieldValueText )
    XIMPIMP_IF_BASE_GET_CONST_CLASS( CXIMPApiDataObjBase, this )
XIMPIMP_IF_BASE_GET_INTERFACE_END()

// ---------------------------------------------------------------------------
// CPresenceInfoFieldValueTextImp::~CPresenceInfoFieldValueTextImp()
// ---------------------------------------------------------------------------
//
CPresenceInfoFieldValueTextImp::~CPresenceInfoFieldValueTextImp()
    {
    iFieldValue.Close();
    }

// ---------------------------------------------------------------------------
// CPresenceInfoFieldValueTextImp::CPresenceInfoFieldValueTextImp()
// ---------------------------------------------------------------------------
//
CPresenceInfoFieldValueTextImp::CPresenceInfoFieldValueTextImp(
        TInt aMaxLength )
    {
    iMaxLength = aMaxLength;
    }

// ---------------------------------------------------------------------------
// CPresenceInfoFieldValueTextImp::CPresenceInfoFieldValueTextImp()
// ---------------------------------------------------------------------------
//
CPresenceInfoFieldValueTextImp::CPresenceInfoFieldValueTextImp()
    {
    // nothing
    }

// ---------------------------------------------------------------------------
// CPresenceInfoFieldValueTextImp::ConstructL()
// ---------------------------------------------------------------------------
//
void CPresenceInfoFieldValueTextImp::ConstructL()
    {
    // always allocate a 0-length here, then in the setter enforce
    // the maximal length
    iFieldValue.CreateL( 0 );
    }

// ---------------------------------------------------------------------------
// CPresenceInfoFieldValueTextImp::ExternalizeL()
// ---------------------------------------------------------------------------
//
void CPresenceInfoFieldValueTextImp::ExternalizeL( RWriteStream& aStream ) const
    {
    XIMPRBuf16Helper::ExternalizeL( iFieldValue, aStream );
    aStream.WriteInt32L( iMaxLength );
    }

// ---------------------------------------------------------------------------
// CPresenceInfoFieldValueTextImp::InternalizeL()
// ---------------------------------------------------------------------------
//
EXPORT_C void CPresenceInfoFieldValueTextImp::InternalizeL( RReadStream& aStream )
    {
    XIMPRBuf16Helper::InternalizeL( iFieldValue, aStream );
    iMaxLength = aStream.ReadInt32L();
    }

// ---------------------------------------------------------------------------
// CPresenceInfoFieldValueTextImp::EqualsContent()
// ---------------------------------------------------------------------------
//
TBool CPresenceInfoFieldValueTextImp::EqualsContent( 
        const CXIMPApiDataObjBase& aOtherInstance ) const
    {
    const CPresenceInfoFieldValueTextImp* tmp =
        TXIMPGetImpClassOrPanic< const CPresenceInfoFieldValueTextImp >::From( 
                aOtherInstance.Base() );

    TBool same = ETrue;
    TBool x;
    x = 0 == iFieldValue.Compare( tmp->iFieldValue );
    same &= x;

    x = iMaxLength == tmp->iMaxLength;
    same &= x;

    return same;
    }

// ---------------------------------------------------------------------------
// CPresenceInfoFieldValueTextImp::MaxLength()
// ---------------------------------------------------------------------------
//
TInt CPresenceInfoFieldValueTextImp::MaxLength() const
    {
    return iMaxLength;
    }

// ---------------------------------------------------------------------------
// CPresenceInfoFieldValueTextImp::TextValue()
// ---------------------------------------------------------------------------
//
TPtrC CPresenceInfoFieldValueTextImp::TextValue() const
    {
    return iFieldValue;
    }

// ---------------------------------------------------------------------------
// CPresenceInfoFieldValueTextImp::SetTextValueL()
// ---------------------------------------------------------------------------
//
void CPresenceInfoFieldValueTextImp::SetTextValueL( const TDesC& aValue )
    {
    // maximal length might be enforced
    User::LeaveIfError( ( iMaxLength != NPresenceInfo::KValueLengthUnlimited ) 
            && ( iMaxLength < aValue.Length() ) ? KErrOverflow : KErrNone ); 

    HBufC16* tmp = aValue.AllocL();
    iFieldValue.Close();
    iFieldValue.Assign( tmp );
    }

// End of file

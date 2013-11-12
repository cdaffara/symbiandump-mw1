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


#include "presenceinfofieldvaluebinaryimp.h"
#include "presenceinfofieldvaluebinary.h"
#include "ximprbufhelpers.h"
#include "ximpobjecthelpers.h"

// ============================ MEMBER FUNCTIONS =============================

// ---------------------------------------------------------------------------
// CPresenceInfoFieldValueBinaryImp::NewLC()
// ---------------------------------------------------------------------------
//
EXPORT_C CPresenceInfoFieldValueBinaryImp* CPresenceInfoFieldValueBinaryImp::NewLC(
        TInt aMaxLength /* = NPresenceInfo::KPrValueLengthUnlimited */
        )
    {
    CPresenceInfoFieldValueBinaryImp* self = new ( ELeave ) CPresenceInfoFieldValueBinaryImp( aMaxLength );
    CleanupStack::PushL( self );
    self->ConstructL();
    return self;
    }

// ---------------------------------------------------------------------------
// CPresenceInfoFieldValueBinaryImp::NewFromStreamLC()
// ---------------------------------------------------------------------------
//
XIMPIMP_IMPLEMENT_DATAOBJ_NEWFROMSTREAM( CPresenceInfoFieldValueBinaryImp )

// ---------------------------------------------------------------------------
// Implement supported interface access.
// ---------------------------------------------------------------------------
//
XIMPIMP_IF_BASE_GET_INTERFACE_BEGIN( CPresenceInfoFieldValueBinaryImp, MPresenceInfoFieldValueBinary )
    XIMPIMP_IF_BASE_GET_CLASS( CXIMPApiDataObjBase, this )
XIMPIMP_IF_BASE_GET_INTERFACE_END()
XIMPIMP_IF_BASE_GET_CONST_INTERFACE_BEGIN( CPresenceInfoFieldValueBinaryImp, MPresenceInfoFieldValueBinary )
    XIMPIMP_IF_BASE_GET_CONST_CLASS( CXIMPApiDataObjBase, this )
XIMPIMP_IF_BASE_GET_INTERFACE_END()

// ---------------------------------------------------------------------------
// CPresenceInfoFieldValueBinaryImp::~CPresenceInfoFieldValueBinaryImp()
// ---------------------------------------------------------------------------
//
CPresenceInfoFieldValueBinaryImp::~CPresenceInfoFieldValueBinaryImp()
    {
    iValue.Close();
    iMimeType.Close();
    }

// ---------------------------------------------------------------------------
// CPresenceInfoFieldValueBinaryImp::CPresenceInfoFieldValueBinaryImp()
// ---------------------------------------------------------------------------
//
CPresenceInfoFieldValueBinaryImp::CPresenceInfoFieldValueBinaryImp(
        TInt aMaxLength )
    {
    iMaxLength = aMaxLength;
    }

// ---------------------------------------------------------------------------
// CPresenceInfoFieldValueBinaryImp::CPresenceInfoFieldValueBinaryImp()
// ---------------------------------------------------------------------------
//
CPresenceInfoFieldValueBinaryImp::CPresenceInfoFieldValueBinaryImp()
    {
    // nothing
    }

// ---------------------------------------------------------------------------
// CPresenceInfoFieldValueBinaryImp::ConstructL()
// ---------------------------------------------------------------------------
//
void CPresenceInfoFieldValueBinaryImp::ConstructL()
    {
    // always allocate a 0-length here, then in the setter enforce
    // the maximal length
    iValue.CreateL( 0 );
    iMimeType.CreateL( 0 );
    }

// ---------------------------------------------------------------------------
// CPresenceInfoFieldValueBinaryImp::ExternalizeL()
// ---------------------------------------------------------------------------
//
void CPresenceInfoFieldValueBinaryImp::ExternalizeL( RWriteStream& aStream ) const
    {
    XIMPRBuf8Helper::ExternalizeL( iValue, aStream );
    XIMPRBuf8Helper::ExternalizeL( iMimeType, aStream );
    aStream.WriteInt32L( iMaxLength );
    }

// ---------------------------------------------------------------------------
// CPresenceInfoFieldValueBinaryImp::InternalizeL()
// ---------------------------------------------------------------------------
//
EXPORT_C void CPresenceInfoFieldValueBinaryImp::InternalizeL( RReadStream& aStream )
    {
    XIMPRBuf8Helper::InternalizeL( iValue, aStream );
    XIMPRBuf8Helper::InternalizeL( iMimeType, aStream );
    iMaxLength = aStream.ReadInt32L();
    }

// ---------------------------------------------------------------------------
// CPresenceInfoFieldValueBinaryImp::EqualsContent()
// ---------------------------------------------------------------------------
//
TBool CPresenceInfoFieldValueBinaryImp::EqualsContent( 
        const CXIMPApiDataObjBase& aOtherInstance ) const
    {
    const CPresenceInfoFieldValueBinaryImp* tmp =
        TXIMPGetImpClassOrPanic< const CPresenceInfoFieldValueBinaryImp >::From( 
                aOtherInstance.Base() );

    TBool same = ETrue;
    TBool x;
    x = 0 == iValue.Compare( tmp->iValue );
    same &= x;
    
    x = 0 == iMimeType.Compare( tmp->iMimeType );
    same &= x;
    
    x = iMaxLength == tmp->iMaxLength;
    same &= x;

    return same;
    }

// ---------------------------------------------------------------------------
// CPresenceInfoFieldValueBinaryImp::MaxLength()
// ---------------------------------------------------------------------------
//
TInt CPresenceInfoFieldValueBinaryImp::MaxLength() const
    {
    return iMaxLength;
    }

// ---------------------------------------------------------------------------
// CPresenceInfoFieldValueBinaryImp::BinaryValue()
// ---------------------------------------------------------------------------
//
TPtrC8 CPresenceInfoFieldValueBinaryImp::BinaryValue() const
    {
    return iValue;
    }

// ---------------------------------------------------------------------------
// CPresenceInfoFieldValueBinaryImp::MimeType()
// ---------------------------------------------------------------------------
//
const TPtrC8 CPresenceInfoFieldValueBinaryImp::MimeType() const
    {
    return iMimeType;
    }

// ---------------------------------------------------------------------------
// CPresenceInfoFieldValueBinaryImp::SetBinaryValueL()
// ---------------------------------------------------------------------------
//
void CPresenceInfoFieldValueBinaryImp::SetBinaryValueL( const TDesC8& aValue )
    {
    // maximal length might be enforced
    User::LeaveIfError( ( iMaxLength != NPresenceInfo::KValueLengthUnlimited ) && 
            ( iMaxLength < aValue.Length() ) ? KErrOverflow : KErrNone ); 

    HBufC8* tmp = aValue.AllocL();
    iValue.Close();
    iValue.Assign( tmp );
    }

// ---------------------------------------------------------------------------
// CPresenceInfoFieldValueBinaryImp::SetMimeTypeL
// ---------------------------------------------------------------------------
//
void CPresenceInfoFieldValueBinaryImp::SetMimeTypeL( const TDesC8& aValue )
    {
    HBufC8* tmp = aValue.AllocL();
    iMimeType.Close();
    iMimeType.Assign( tmp );
    }

// End of file

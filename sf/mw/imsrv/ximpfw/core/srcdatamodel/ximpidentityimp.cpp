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
* Description:  MXIMPIdentity API object implementation.
*
*/


#include "ximpidentityimp.h"
#include "ximprbufhelpers.h"

// ======== MEMBER FUNCTIONS ========

// ---------------------------------------------------------------------------
// CXIMPIdentityImp::NewLC()
// ---------------------------------------------------------------------------
//
EXPORT_C CXIMPIdentityImp* CXIMPIdentityImp::NewLC()
    {
    CXIMPIdentityImp* self = new( ELeave ) CXIMPIdentityImp;
    CleanupStack::PushL( self );
    self->ConstructL();
    return self;
    }


EXPORT_C CXIMPIdentityImp* CXIMPIdentityImp::NewL()
    {
    CXIMPIdentityImp* self = CXIMPIdentityImp::NewLC();
    CleanupStack::Pop( self );
    return self;
    }


EXPORT_C CXIMPIdentityImp* CXIMPIdentityImp::NewLC( const TDesC16& aIdentity )
    {
    CXIMPIdentityImp* self = new( ELeave ) CXIMPIdentityImp;
    CleanupStack::PushL( self );
    self->ConstructL( aIdentity );
    return self;
    }

// ---------------------------------------------------------------------------
// CXIMPIdentityImp::~CXIMPIdentityImp()
// ---------------------------------------------------------------------------
//
CXIMPIdentityImp::~CXIMPIdentityImp()
    {
    iIdentity.Close();
    }


// ---------------------------------------------------------------------------
// CXIMPIdentityImp::CXIMPIdentityImp()
// ---------------------------------------------------------------------------
//
CXIMPIdentityImp::CXIMPIdentityImp()
    {
    }


// ---------------------------------------------------------------------------
// CXIMPIdentityImp::ConstructL()
// ---------------------------------------------------------------------------
//
void CXIMPIdentityImp::ConstructL()
    {
    iIdentity.CreateL( 0 );
    }

void CXIMPIdentityImp::ConstructL( const TDesC16& aIdentity )
    {
    iIdentity.CreateL( aIdentity );
    }

// ---------------------------------------------------------------------------
// CXIMPIdentityImp::NewFromStreamLC()
// ---------------------------------------------------------------------------
//
XIMPIMP_IMPLEMENT_DATAOBJ_NEWFROMSTREAM( CXIMPIdentityImp )


// ---------------------------------------------------------------------------
// Implement supported interface access.
// ---------------------------------------------------------------------------
//
XIMPIMP_IF_BASE_GET_INTERFACE_BEGIN( CXIMPIdentityImp,
                                     MXIMPIdentity )
XIMPIMP_IF_BASE_GET_INTERFACE_END()


XIMPIMP_IF_BASE_GET_CONST_INTERFACE_BEGIN( CXIMPIdentityImp,
                                           MXIMPIdentity )
XIMPIMP_IF_BASE_GET_INTERFACE_END()



// ---------------------------------------------------------------------------
// CXIMPIdentityImp::SetIdentityL()
// ---------------------------------------------------------------------------
//
void CXIMPIdentityImp::SetIdentityL( const TDesC16& aIdentity )
    {
    iIdentity.Close();
    iIdentity.CreateL( aIdentity );
    }


// ---------------------------------------------------------------------------
// CXIMPIdentityImp::Identity()
// ---------------------------------------------------------------------------
//
const TDesC16& CXIMPIdentityImp::Identity() const
    {
    return iIdentity;
    }


// ---------------------------------------------------------------------------
// CXIMPIdentityImp::ExternalizeL()
// ---------------------------------------------------------------------------
//
void CXIMPIdentityImp::ExternalizeL( RWriteStream& aStream ) const
    {
    XIMPRBuf16Helper::ExternalizeL( iIdentity, aStream );
    }


// ---------------------------------------------------------------------------
// CXIMPIdentityImp::InternalizeL()
// ---------------------------------------------------------------------------
//
EXPORT_C void CXIMPIdentityImp::InternalizeL( RReadStream& aStream )
    {
    XIMPRBuf16Helper::InternalizeL( iIdentity, aStream );
    }

// ---------------------------------------------------------------------------
// CXIMPIdentityImp::operator!=()
// ---------------------------------------------------------------------------
//
EXPORT_C TBool CXIMPIdentityImp::operator!=( const CXIMPIdentityImp& aOther ) const
    {
    if( iIdentity.Compare( aOther.iIdentity ) )
        {
        return ETrue;
        }
    else
        {
        return EFalse;
        }
    }

// ---------------------------------------------------------------------------
// CXIMPIdentityImp::EqualsContent()
// ---------------------------------------------------------------------------
//
TBool CXIMPIdentityImp::EqualsContent(
    const CXIMPApiDataObjBase& aOtherInstance ) const
    {
    const CXIMPIdentityImp* tmp =
        TXIMPGetImpClassOrPanic< const CXIMPIdentityImp >::From( aOtherInstance.Base() );

    TBool same = ETrue;

    TBool x;
    x = 0 == iIdentity.Compare( tmp->iIdentity );
    same &= x;

    return same;
    }


// ---------------------------------------------------------------------------
// CXIMPIdentityImp::Compare()
// ---------------------------------------------------------------------------
//
EXPORT_C TInt CXIMPIdentityImp::Compare( const CXIMPIdentityImp& aOther ) const
    {
    return iIdentity.Compare( aOther.iIdentity );
    }

// End of file

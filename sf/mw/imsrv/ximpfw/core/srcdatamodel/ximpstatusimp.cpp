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
* Description:  MXIMPStatus API object implementation.
*
*/

#include "ximpstatusimp.h"
#include "ximpobjecthelpers.h"
#include "ximpfeatureinfoimp.h"
#include "ximprbufhelpers.h"



// ============================ MEMBER FUNCTIONS =============================

// ---------------------------------------------------------------------------
// CXIMPStatusImp::NewLC()
// ---------------------------------------------------------------------------
//
EXPORT_C CXIMPStatusImp* CXIMPStatusImp::NewLC()
    {
    CXIMPStatusImp* self = new( ELeave ) CXIMPStatusImp;
    CleanupStack::PushL( self );
    self->ConstructL();
    return self;
    }


// ---------------------------------------------------------------------------
// CXIMPStatusImp::NewL()
// ---------------------------------------------------------------------------
//
EXPORT_C CXIMPStatusImp* CXIMPStatusImp::NewL()
    {
    CXIMPStatusImp* self = CXIMPStatusImp::NewLC();
    CleanupStack::Pop( self );
    return self;
    }



// ---------------------------------------------------------------------------
// CXIMPStatusImp::~CXIMPStatusImp()
// ---------------------------------------------------------------------------
//
CXIMPStatusImp::~CXIMPStatusImp()
    {
    iProtocolResultDescription.Close();
    }


// ---------------------------------------------------------------------------
// CXIMPStatusImp::CXIMPStatusImp()
// ---------------------------------------------------------------------------
//
CXIMPStatusImp::CXIMPStatusImp()
    {
    }


// ---------------------------------------------------------------------------
// CXIMPStatusImp::ConstructL()
// ---------------------------------------------------------------------------
//
void CXIMPStatusImp::ConstructL()
    {
    // Initialize the RBufs to zero size. Later on realloc as needed.
    iProtocolResultDescription.CreateL( 0 );
    iResultCode = KErrNone;
    iProtocolResultCode = KErrNone;
    }

// ---------------------------------------------------------------------------
// CXIMPStatusImp::NewFromStreamLC()
// ---------------------------------------------------------------------------
//
XIMPIMP_IMPLEMENT_DATAOBJ_NEWFROMSTREAM( CXIMPStatusImp )


// ---------------------------------------------------------------------------
// Implement supported interface access.
// ---------------------------------------------------------------------------
//
XIMPIMP_IF_BASE_GET_INTERFACE_BEGIN( CXIMPStatusImp, 
                                     MXIMPStatus )
XIMPIMP_IF_BASE_GET_INTERFACE_END()


XIMPIMP_IF_BASE_GET_CONST_INTERFACE_BEGIN( CXIMPStatusImp, 
                                           MXIMPStatus )
XIMPIMP_IF_BASE_GET_INTERFACE_END()



// ---------------------------------------------------------------------------
// CXIMPStatusImp::ResultCode()
// ---------------------------------------------------------------------------
TInt CXIMPStatusImp::ResultCode() const
    {
    return iResultCode;
    }


// ---------------------------------------------------------------------------
// CXIMPStatusImp::ProtocolResultCode()
// ---------------------------------------------------------------------------
TInt CXIMPStatusImp::ProtocolResultCode() const
    {
    return iProtocolResultCode;
    }

// ---------------------------------------------------------------------------
// CXIMPStatusImp::ProtocolResultDescription()
// ---------------------------------------------------------------------------
const TDesC16& CXIMPStatusImp::ProtocolResultDescription() const
    {
    return iProtocolResultDescription;
    }


// ---------------------------------------------------------------------------
// CXIMPStatusImp::SetResultCode()
// ---------------------------------------------------------------------------
void CXIMPStatusImp::SetResultCode( TInt aCode )
    {
    iResultCode = aCode;
    }


// ---------------------------------------------------------------------------
// CXIMPStatusImp::SetProtocolResultCode()
// ---------------------------------------------------------------------------
void CXIMPStatusImp::SetProtocolResultCode( TInt aCode )
    {
    iProtocolResultCode = aCode;
    }


// ---------------------------------------------------------------------------
// CXIMPStatusImp::SetProtocolResultDescriptionL()
// ---------------------------------------------------------------------------
void CXIMPStatusImp::SetProtocolResultDescriptionL(
    const TDesC16& aDescription )
    {
    XIMPRBuf16Helper::SetNewValueL( iProtocolResultDescription, aDescription );
    }


// ---------------------------------------------------------------------------
// CXIMPStatusImp::ExternalizeL()
// ---------------------------------------------------------------------------
//
void CXIMPStatusImp::ExternalizeL( RWriteStream& aStream ) const
    {
    aStream.WriteInt32L( iResultCode );
    aStream.WriteInt32L( iProtocolResultCode );
    XIMPRBuf16Helper::ExternalizeL( iProtocolResultDescription, aStream );
    }


// ---------------------------------------------------------------------------
// CXIMPStatusImp::InternalizeL()
// ---------------------------------------------------------------------------
//
EXPORT_C void CXIMPStatusImp::InternalizeL( RReadStream& aStream )
    {
    iResultCode = aStream.ReadInt32L();
    iProtocolResultCode = aStream.ReadInt32L();
    XIMPRBuf16Helper::InternalizeL( iProtocolResultDescription, aStream );
    }

// ---------------------------------------------------------------------------
// CXIMPStatusImp::EqualsContent()
// ---------------------------------------------------------------------------
//
TBool CXIMPStatusImp::EqualsContent( 
    const CXIMPApiDataObjBase& aOtherInstance ) const
    {
    const CXIMPStatusImp* tmp =
    TXIMPGetImpClassOrPanic< const CXIMPStatusImp >::From( aOtherInstance.Base() );

    TBool same = ETrue;
    TBool x;
    x = iResultCode == tmp->iResultCode;
    same &= x;

    x = iProtocolResultCode == tmp->iProtocolResultCode;
    same &= x;

    x = 0 == iProtocolResultDescription.Compare( tmp->iProtocolResultDescription );
    same &= x;

    return same;
    }


// End of file

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
* Description:  MXIMPContextClientInfo API object implementation.
*
*/

#include "ximpcontextclientinfoimp.h"
#include "ximpidentityimp.h"

// ============================ MEMBER FUNCTIONS =============================


// ---------------------------------------------------------------------------
// CXIMPContextClientInfoImp::NewLC()
// ---------------------------------------------------------------------------
//
EXPORT_C CXIMPContextClientInfoImp* CXIMPContextClientInfoImp::NewLC()
    {
    CXIMPContextClientInfoImp* self = new( ELeave ) CXIMPContextClientInfoImp;
    CleanupStack::PushL( self );
    self->ConstructL();
    return self;
    }
    

// ---------------------------------------------------------------------------
// CXIMPContextClientInfoImp::NewL()
// ---------------------------------------------------------------------------
//
EXPORT_C CXIMPContextClientInfoImp* CXIMPContextClientInfoImp::NewL()
    {
    CXIMPContextClientInfoImp* self = NewLC();
    CleanupStack::Pop( self );
    return self;
    }

// ---------------------------------------------------------------------------
// CXIMPContextClientInfoImp::NewFromStreamLC()
// ---------------------------------------------------------------------------
//
XIMPIMP_IMPLEMENT_DATAOBJ_NEWFROMSTREAM( CXIMPContextClientInfoImp )


// ---------------------------------------------------------------------------
// Implement supported interface access.
// ---------------------------------------------------------------------------
//
XIMPIMP_IF_BASE_GET_INTERFACE_BEGIN( CXIMPContextClientInfoImp, 
                                     MXIMPContextClientInfo )
XIMPIMP_IF_BASE_GET_INTERFACE_END()


XIMPIMP_IF_BASE_GET_CONST_INTERFACE_BEGIN( CXIMPContextClientInfoImp, 
                                           MXIMPContextClientInfo )
XIMPIMP_IF_BASE_GET_INTERFACE_END()

// ---------------------------------------------------------------------------
// CXIMPContextClientInfoImp::~CXIMPContextClientInfoImp()
// ---------------------------------------------------------------------------
//
CXIMPContextClientInfoImp::~CXIMPContextClientInfoImp()
    {
    delete iClientId; 
    }


// ---------------------------------------------------------------------------
// CXIMPContextClientInfoImp::CXIMPContextClientInfoImp()
// ---------------------------------------------------------------------------
//
CXIMPContextClientInfoImp::CXIMPContextClientInfoImp()
    {
    }

// ---------------------------------------------------------------------------
// CXIMPContextClientInfoImp::ConstructL()
// ---------------------------------------------------------------------------
//
void CXIMPContextClientInfoImp::ConstructL()
    {
    iClientId = CXIMPIdentityImp::NewL();
    }


// ---------------------------------------------------------------------------
// CXIMPContextClientInfoImp::ConstructL()
// ---------------------------------------------------------------------------
//
void CXIMPContextClientInfoImp::ConstructL( const TDesC16& aClientId )
    {
    iClientId = CXIMPIdentityImp::NewLC( aClientId );
    CleanupStack::Pop(); //iClientId
    }

// ---------------------------------------------------------------------------
// CXIMPContextClientInfoImp::ExternalizeL()
// ---------------------------------------------------------------------------
//
void CXIMPContextClientInfoImp::ExternalizeL( RWriteStream& aStream ) const
    {
    iClientId->ExternalizeL( aStream );
    }


// ---------------------------------------------------------------------------
// CXIMPContextClientInfoImp::InternalizeL()
// ---------------------------------------------------------------------------
//
EXPORT_C void CXIMPContextClientInfoImp::InternalizeL( RReadStream& aStream )
    {
    iClientId->InternalizeL( aStream );
    }


// ---------------------------------------------------------------------------
// CXIMPContextClientInfoImp::Match()
// ---------------------------------------------------------------------------
//
EXPORT_C TBool CXIMPContextClientInfoImp::Match( 
    const CXIMPContextClientInfoImp* aClientInfoImp ) const
    {
    return iClientId->Compare( *aClientInfoImp->iClientId ) == 0;
    }

// ---------------------------------------------------------------------------
// CXIMPContextClientInfoImp::EqualsContent()
// ---------------------------------------------------------------------------
//
TBool CXIMPContextClientInfoImp::EqualsContent( 
    const CXIMPApiDataObjBase& aOtherInstance ) const 
    {
    const CXIMPContextClientInfoImp* tmp =
    TXIMPGetImpClassOrPanic< const CXIMPContextClientInfoImp>::From( 
                                                       aOtherInstance.Base() );

    TBool same = Match( tmp ); 
    TBool x;
    x = ETrue; // do some comparison of member variable here
    same &= x;
    return same;
    }

// ---------------------------------------------------------------------------
// From MXIMPContextClientInfo class.
// CXIMPContextClientInfoImp::ClientId()
// ---------------------------------------------------------------------------
//
const MXIMPIdentity& CXIMPContextClientInfoImp::ClientId() const
    {
    return *iClientId;
    }


// ---------------------------------------------------------------------------
// From MXIMPContextClientInfo class.
// CXIMPContextClientInfoImp::SetClientIdL()
// ---------------------------------------------------------------------------
//
void CXIMPContextClientInfoImp::SetClientIdL( 
    MXIMPIdentity* aIdentity )
    {   
    CXIMPIdentityImp* tmp =
    TXIMPGetImpClassOrPanic< CXIMPIdentityImp>::From( *aIdentity );
    
    delete iClientId;
    iClientId = tmp;
    }

    
// End of file

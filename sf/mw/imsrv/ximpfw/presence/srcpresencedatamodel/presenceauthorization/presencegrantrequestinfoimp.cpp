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
* Description:  MPresenceGrantRequestInfo API object implementation.
*
*/


#include "presencegrantrequestinfoimp.h"
#include "ximpidentityimp.h"
#include "ximprbufhelpers.h"
#include "ximpobjecthelpers.h"

// ============================ MEMBER FUNCTIONS =============================

// ---------------------------------------------------------------------------
// CPresenceGrantRequestInfoImp::NewLC()
// ---------------------------------------------------------------------------
//
EXPORT_C CPresenceGrantRequestInfoImp* CPresenceGrantRequestInfoImp::NewLC(
                            const MXIMPIdentity& aIdentity,
                            const TDesC16& aDisplayName )
    {
    CPresenceGrantRequestInfoImp* self = new( ELeave ) CPresenceGrantRequestInfoImp;
    CleanupStack::PushL( self );
    self->ConstructL( aIdentity, aDisplayName );
    return self;
    }

// ---------------------------------------------------------------------------
// CPresenceGrantRequestInfoImp::NewLC()
// ---------------------------------------------------------------------------
//
EXPORT_C CPresenceGrantRequestInfoImp* CPresenceGrantRequestInfoImp::NewLC()
    {
    CPresenceGrantRequestInfoImp* self = new( ELeave ) CPresenceGrantRequestInfoImp;
    CleanupStack::PushL( self );
    self->ConstructL();
    return self;
    }


// ---------------------------------------------------------------------------
// CPresenceGrantRequestInfoImp::NewL()
// ---------------------------------------------------------------------------
//
EXPORT_C CPresenceGrantRequestInfoImp* CPresenceGrantRequestInfoImp::NewL()
    {
    CPresenceGrantRequestInfoImp* self = NewLC();
    CleanupStack::Pop( self );
    return self;
    }

// ---------------------------------------------------------------------------
// CPresenceGrantRequestfoImp::NewFromStreamLC()
// ---------------------------------------------------------------------------
//
XIMPIMP_IMPLEMENT_DATAOBJ_NEWFROMSTREAM( CPresenceGrantRequestInfoImp )

// ---------------------------------------------------------------------------
// Implement supported interface access.
// ---------------------------------------------------------------------------
//
XIMPIMP_IF_BASE_GET_INTERFACE_BEGIN( CPresenceGrantRequestInfoImp, 
                                     MPresenceGrantRequestInfo )
XIMPIMP_IF_BASE_GET_INTERFACE_END()

XIMPIMP_IF_BASE_GET_CONST_INTERFACE_BEGIN( CPresenceGrantRequestInfoImp, 
                                           MPresenceGrantRequestInfo )
XIMPIMP_IF_BASE_GET_INTERFACE_END()


// ---------------------------------------------------------------------------
// CPresenceGrantRequestInfoImp::~CPresenceGrantRequestInfoImp()
// ---------------------------------------------------------------------------
//
CPresenceGrantRequestInfoImp::~CPresenceGrantRequestInfoImp()
    {
    iDisplayName.Close();
    delete iIdentity;
    }

// ---------------------------------------------------------------------------
// CPresenceGrantRequestInfoImp::CPresenceGrantRequestInfoImp()
// ---------------------------------------------------------------------------
//
CPresenceGrantRequestInfoImp::CPresenceGrantRequestInfoImp()
    {
    }


// ---------------------------------------------------------------------------
// CPresenceGrantRequestInfoImp::ConstructL()
// ---------------------------------------------------------------------------
//
void CPresenceGrantRequestInfoImp::ConstructL()
    {
    iDisplayName.CreateL( 0 );
    iIdentity = CXIMPIdentityImp::NewLC();
    CleanupStack::Pop( iIdentity );
    }

// ---------------------------------------------------------------------------
// CPresenceGrantRequestInfoImp::ConstructL()
// ---------------------------------------------------------------------------
//
void CPresenceGrantRequestInfoImp::ConstructL(
                            const MXIMPIdentity& aIdentity,
                            const TDesC16& aDisplayName )
    {
    const CXIMPIdentityImp* sourceIdentity = static_cast< const CXIMPIdentityImp* >( &aIdentity );
    iIdentity = TXIMPObjectCloner< CXIMPIdentityImp >::CloneL( *sourceIdentity );
    iDisplayName.CreateL( aDisplayName );
    }

// ---------------------------------------------------------------------------
// CPresenceGrantRequestInfoImp::ExternalizeL()
// ---------------------------------------------------------------------------
//
void CPresenceGrantRequestInfoImp::ExternalizeL( RWriteStream& aStream ) const
    {
    iIdentity->ExternalizeL( aStream );
    XIMPRBuf16Helper::ExternalizeL( iDisplayName, aStream );
    }

// ---------------------------------------------------------------------------
// CPresenceGrantRequestInfoImp::InternalizeL()
// ---------------------------------------------------------------------------
//
EXPORT_C void CPresenceGrantRequestInfoImp::InternalizeL( RReadStream& aStream )
    {
    iIdentity->InternalizeL( aStream );
    XIMPRBuf16Helper::InternalizeL( iDisplayName, aStream );
    }

// ---------------------------------------------------------------------------
// From MPresenceGrantRequestInfo class.
// CPresenceGrantRequestInfoImp::RequestorId()
// ---------------------------------------------------------------------------
//

const MXIMPIdentity& CPresenceGrantRequestInfoImp::RequestorId() const
    {
    return *iIdentity;
    }

// ---------------------------------------------------------------------------
// From MPresenceGrantRequestInfo class.
// CPresenceGrantRequestInfoImp::RequestorDisplayName()
// ---------------------------------------------------------------------------
//
const TDesC16& CPresenceGrantRequestInfoImp::RequestorDisplayName() const
    {
    return iDisplayName;
    }

// ---------------------------------------------------------------------------
// From MPresenceGrantRequestInfo class.
// CPresenceGrantRequestInfoImp::SetRequestorIdL()
// ---------------------------------------------------------------------------
//
void CPresenceGrantRequestInfoImp::SetRequestorIdL( 
    MXIMPIdentity* aIdentity )
    {
    CXIMPIdentityImp* identityImp =
    TXIMPGetImpClassOrPanic< CXIMPIdentityImp>::From( *aIdentity );

    delete iIdentity;
    iIdentity = identityImp; 
    }


// ---------------------------------------------------------------------------
// From MPresenceGrantRequestInfo class.
// CPresenceGrantRequestInfoImp::
// ---------------------------------------------------------------------------
//
void CPresenceGrantRequestInfoImp::SetRequestorDisplayNameL( 
    const TDesC16& aDisplayName )
    {
    HBufC16* displayNameBuf = aDisplayName.AllocL();
    iDisplayName.Close();
    iDisplayName.Assign( displayNameBuf );
    }


// ---------------------------------------------------------------------------
// CPresenceGrantRequestInfoImp::SetIdentityL()
// ---------------------------------------------------------------------------
//
EXPORT_C void CPresenceGrantRequestInfoImp::SetIdentity( CXIMPIdentityImp* aIdentity )
    {
    delete iIdentity;
    iIdentity = aIdentity;
    }

// ---------------------------------------------------------------------------
// CPresenceGrantRequestInfoImp::SetDisplayName()
// ---------------------------------------------------------------------------
//
EXPORT_C void CPresenceGrantRequestInfoImp::SetDisplayName( HBufC16* aDisplayName )
    {
    iDisplayName.Close();
    iDisplayName.Assign( aDisplayName );
    }

// ---------------------------------------------------------------------------
// CPresenceGrantRequestInfoImp::IdLinearOrder()
// ---------------------------------------------------------------------------
//
EXPORT_C TInt CPresenceGrantRequestInfoImp::IdLinearOrder(
        const CPresenceGrantRequestInfoImp& aA,
        const CPresenceGrantRequestInfoImp& aB )
    {
    return aA.IdentityImp().Compare( aB.IdentityImp() );
    }
// ---------------------------------------------------------------------------
// CPresenceGrantRequestInfoImp::IdentityImp()
// ---------------------------------------------------------------------------
//
EXPORT_C const CXIMPIdentityImp& CPresenceGrantRequestInfoImp::IdentityImp() const
    {
    return *iIdentity;
    }


// ---------------------------------------------------------------------------
// CPresenceGrantRequestInfoImp::EqualsContent()
// ---------------------------------------------------------------------------
//
TBool CPresenceGrantRequestInfoImp::EqualsContent(
    const CXIMPApiDataObjBase& aOtherInstance ) const
    {
    const CPresenceGrantRequestInfoImp* tmp =
    TXIMPGetImpClassOrPanic< const CPresenceGrantRequestInfoImp >::From( 
                                                        aOtherInstance.Base() );
    
    TBool same = ETrue;
    TBool x;
    x = iIdentity->EqualsContent( *( tmp->iIdentity ) );
    same &= x;

    x = 0 == iDisplayName.Compare( tmp->iDisplayName );
    same &= x;

    // add more comparisons in a similar way...

    return same;
    }

// End of file

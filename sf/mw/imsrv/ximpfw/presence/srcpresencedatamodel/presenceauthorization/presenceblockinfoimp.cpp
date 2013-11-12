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
* Description:  MPresenceBlockInfo API object implementation.
*
*/

#include "presenceblockinfoimp.h"
#include "ximpidentityimp.h"
#include "ximprbufhelpers.h"
#include "ximpobjecthelpers.h"


// ============================ MEMBER FUNCTIONS =============================

// ---------------------------------------------------------------------------
// CPresenceBlockInfoImp::NewLC()
// ---------------------------------------------------------------------------
//
EXPORT_C CPresenceBlockInfoImp* CPresenceBlockInfoImp::NewLC(
                            const MXIMPIdentity& aIdentity,
                            const TDesC16& aDisplayName )
    {
    CPresenceBlockInfoImp* self = new( ELeave ) CPresenceBlockInfoImp;
    CleanupStack::PushL( self );
    self->ConstructL( aIdentity, aDisplayName );
    return self;
    }

// ---------------------------------------------------------------------------
// CPresenceBlockInfoImp::NewLC()
// ---------------------------------------------------------------------------
//
EXPORT_C CPresenceBlockInfoImp* CPresenceBlockInfoImp::NewLC()
    {
    CPresenceBlockInfoImp* self = new( ELeave ) CPresenceBlockInfoImp;
    CleanupStack::PushL( self );
    self->ConstructL();
    return self;
    }


// ---------------------------------------------------------------------------
// CPresenceBlockInfoImp::NewL()
// ---------------------------------------------------------------------------
//
EXPORT_C CPresenceBlockInfoImp* CPresenceBlockInfoImp::NewL()
    {
    CPresenceBlockInfoImp* self = NewLC();
    CleanupStack::Pop( self );
    return self;
    }

// ---------------------------------------------------------------------------
// CXIMPFeatureInfoImp::NewFromStreamLC()
// ---------------------------------------------------------------------------
//
XIMPIMP_IMPLEMENT_DATAOBJ_NEWFROMSTREAM( CPresenceBlockInfoImp )

// ---------------------------------------------------------------------------
// Implement supported interface access.
// ---------------------------------------------------------------------------
//
XIMPIMP_IF_BASE_GET_INTERFACE_BEGIN( CPresenceBlockInfoImp, 
                                     MPresenceBlockInfo )
XIMPIMP_IF_BASE_GET_INTERFACE_END()

XIMPIMP_IF_BASE_GET_CONST_INTERFACE_BEGIN( CPresenceBlockInfoImp, 
                                           MPresenceBlockInfo )
XIMPIMP_IF_BASE_GET_INTERFACE_END()


// ---------------------------------------------------------------------------
// CPresenceBlockInfoImp::~CPresenceBlockInfoImp()
// ---------------------------------------------------------------------------
//
CPresenceBlockInfoImp::~CPresenceBlockInfoImp()
    {
    iDisplayName.Close();
    delete iIdentity;
    }

// ---------------------------------------------------------------------------
// CPresenceBlockInfoImp::CPresenceBlockInfoImp()
// ---------------------------------------------------------------------------
//
CPresenceBlockInfoImp::CPresenceBlockInfoImp()
    {
    }


// ---------------------------------------------------------------------------
// CPresenceBlockInfoImp::ConstructL()
// ---------------------------------------------------------------------------
//
void CPresenceBlockInfoImp::ConstructL()
    {
    iDisplayName.CreateL( 0 );
    iIdentity = CXIMPIdentityImp::NewL();
    }

// ---------------------------------------------------------------------------
// CPresenceBlockInfoImp::ConstructL()
// ---------------------------------------------------------------------------
//
void CPresenceBlockInfoImp::ConstructL(
                            const MXIMPIdentity& aIdentity,
                            const TDesC16& aDisplayName )
    {
    const CXIMPIdentityImp* sourceIdentity =
    TXIMPGetImpClassOrPanic< const CXIMPIdentityImp>::From( aIdentity );

    iIdentity = TXIMPObjectCloner< CXIMPIdentityImp >::CloneL( *sourceIdentity );
    iDisplayName.CreateL( aDisplayName );
    }

// ---------------------------------------------------------------------------
// CPresenceBlockInfoImp::ExternalizeL()
// ---------------------------------------------------------------------------
//
EXPORT_C void CPresenceBlockInfoImp::ExternalizeL( RWriteStream& aStream ) const
    {
    iIdentity->ExternalizeL( aStream );
    XIMPRBuf16Helper::ExternalizeL( iDisplayName, aStream );
    }

// ---------------------------------------------------------------------------
// CPresenceBlockInfoImp::InternalizeL()
// ---------------------------------------------------------------------------
//
EXPORT_C void CPresenceBlockInfoImp::InternalizeL( RReadStream& aStream )
    {
    iIdentity->InternalizeL( aStream );
    XIMPRBuf16Helper::InternalizeL( iDisplayName, aStream );
    }

// ---------------------------------------------------------------------------
// From MPresenceBlockInfo class.
// CPresenceBlockInfoImp::BlockedEntityId()
// ---------------------------------------------------------------------------
//
const MXIMPIdentity& CPresenceBlockInfoImp::BlockedEntityId() const
    {
    return *iIdentity;
    }


// ---------------------------------------------------------------------------
// From MPresenceBlockInfo class.
// CPresenceBlockInfoImp::BlockedEntityDisplayName()
// ---------------------------------------------------------------------------
//
const TDesC16& CPresenceBlockInfoImp::BlockedEntityDisplayName() const
    {
    return iDisplayName;
    }
    

// ---------------------------------------------------------------------------
// From MPresenceBlockInfo class.
// CPresenceBlockInfoImp::SetBlockedEntityIdL()
// ---------------------------------------------------------------------------
//
void CPresenceBlockInfoImp::SetBlockedEntityIdL( 
    MXIMPIdentity* aIdentity )
    {
    CXIMPIdentityImp* identityImp =
    TXIMPGetImpClassOrPanic< CXIMPIdentityImp>::From( *aIdentity );

    delete iIdentity;
    iIdentity = identityImp; 
    }


// ---------------------------------------------------------------------------
// From MPresenceBlockInfo class.
// CPresenceBlockInfoImp::SetBlockedEntityDisplayNameL()
// ---------------------------------------------------------------------------
//
void CPresenceBlockInfoImp::SetBlockedEntityDisplayNameL( 
    const TDesC16& aDisplayName )
    {
    HBufC16* displayNameBuf = aDisplayName.AllocL();
    iDisplayName.Close();
    iDisplayName.Assign( displayNameBuf );
    }
    

// ---------------------------------------------------------------------------
// CPresenceBlockInfoImp::EqualsContent()
// ---------------------------------------------------------------------------
//
TBool CPresenceBlockInfoImp::EqualsContent( const CXIMPApiDataObjBase& aOtherInstance ) const
    {
    const CPresenceBlockInfoImp* tmp =
        TXIMPGetImpClassOrPanic< const CPresenceBlockInfoImp >::From( aOtherInstance.Base() );

    TBool same = ETrue;
    TBool x;

    x = ( iIdentity->Compare( *tmp->iIdentity ) == 0 );
    same &= x;

    x = ( iDisplayName.Compare( tmp->iDisplayName ) == 0 );
    same &= x;

    return same;    
    }

// ---------------------------------------------------------------------------
// CPresenceBlockInfoImp::SetIdentityL()
// ---------------------------------------------------------------------------
//
EXPORT_C void CPresenceBlockInfoImp::SetIdentity( CXIMPIdentityImp* aIdentity )
    {
    delete iIdentity;
    iIdentity = aIdentity;
    }

// ---------------------------------------------------------------------------
// CPresenceBlockInfoImp::SetDisplayName()
// ---------------------------------------------------------------------------
//
EXPORT_C void CPresenceBlockInfoImp::SetDisplayName( HBufC16* aDisplayName )
    {
    iDisplayName.Close();
    iDisplayName.Assign( aDisplayName );
    }

// ---------------------------------------------------------------------------
// CPresenceBlockInfoImp::IdLinearOrder()
// ---------------------------------------------------------------------------
//
EXPORT_C TInt CPresenceBlockInfoImp::IdLinearOrder(
        const CPresenceBlockInfoImp& aA,
        const CPresenceBlockInfoImp& aB )
    {
    return aA.IdentityImp().Compare( aB.IdentityImp() );
    }
// ---------------------------------------------------------------------------
// CPresenceBlockInfoImp::IdentityImp()
// ---------------------------------------------------------------------------
//
EXPORT_C const CXIMPIdentityImp& CPresenceBlockInfoImp::IdentityImp() const
    {
    return *iIdentity;
    }

// End of file

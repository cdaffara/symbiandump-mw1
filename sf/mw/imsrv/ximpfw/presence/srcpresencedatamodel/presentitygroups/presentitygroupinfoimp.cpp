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
* Description:  MPresencetityGrpouInfo API object implementation.
*
*/


#include "presentitygroupinfoimp.h"
#include "ximpidentityimp.h"
#include "ximprbufhelpers.h"
#include "ximpobjecthelpers.h"

// ============================ MEMBER FUNCTIONS =============================

// ---------------------------------------------------------------------------
// CPresentityGroupInfoImp::NewLC()
// ---------------------------------------------------------------------------
//
EXPORT_C CPresentityGroupInfoImp* CPresentityGroupInfoImp::NewLC( 
                                                const MXIMPIdentity& aIdentity, 
                                                const TDesC16& aDisplayName )
    {
    CPresentityGroupInfoImp* self = new( ELeave ) CPresentityGroupInfoImp;
    CleanupStack::PushL( self );
    self->ConstructL( aIdentity, aDisplayName );
    return self;
    }

// ---------------------------------------------------------------------------
// CPresentityGroupInfoImp::NewLC()
// ---------------------------------------------------------------------------
//
EXPORT_C CPresentityGroupInfoImp* CPresentityGroupInfoImp::NewLC()
    {
    CPresentityGroupInfoImp* self = new( ELeave ) CPresentityGroupInfoImp;
    CleanupStack::PushL( self );
    self->ConstructL();
    return self;
    }


// ---------------------------------------------------------------------------
// CPresentityGroupInfoImp::NewL()
// ---------------------------------------------------------------------------
//
EXPORT_C CPresentityGroupInfoImp* CPresentityGroupInfoImp::NewL()
    {
    CPresentityGroupInfoImp* self = NewLC();
    CleanupStack::Pop( self );
    return self;
    }

// ---------------------------------------------------------------------------
// CPresentityGroupInfoImp::NewFromStreamLC()
// ---------------------------------------------------------------------------
//
XIMPIMP_IMPLEMENT_DATAOBJ_NEWFROMSTREAM( CPresentityGroupInfoImp )

// ---------------------------------------------------------------------------
// Implement supported interface access.
// ---------------------------------------------------------------------------
//
XIMPIMP_IF_BASE_GET_INTERFACE_BEGIN( CPresentityGroupInfoImp, MPresentityGroupInfo )
XIMPIMP_IF_BASE_GET_INTERFACE_END()
XIMPIMP_IF_BASE_GET_CONST_INTERFACE_BEGIN( CPresentityGroupInfoImp, MPresentityGroupInfo )
XIMPIMP_IF_BASE_GET_INTERFACE_END()

// ---------------------------------------------------------------------------
// CPresentityGroupInfoImp::~CPresentityGroupInfoImp()
// ---------------------------------------------------------------------------
//
CPresentityGroupInfoImp::~CPresentityGroupInfoImp()
    {
    iDisplayName.Close();
    delete iIdentity;
    }

// ---------------------------------------------------------------------------
// CPresentityGroupInfoImp::CPresentityGroupInfoImp()
// ---------------------------------------------------------------------------
//
CPresentityGroupInfoImp::CPresentityGroupInfoImp()
    {
    }


// ---------------------------------------------------------------------------
// CPresentityGroupInfoImp::ConstructL()
// ---------------------------------------------------------------------------
//
void CPresentityGroupInfoImp::ConstructL()
    {
    iDisplayName.CreateL( 0 );
    iIdentity = CXIMPIdentityImp::NewLC();
    CleanupStack::Pop( iIdentity );
    }

// ---------------------------------------------------------------------------
// CPresentityGroupInfoImp::ConstructL()
// ---------------------------------------------------------------------------
//
void CPresentityGroupInfoImp::ConstructL( const MXIMPIdentity& aIdentity, 
                                              const TDesC16& aDisplayName )
    {
    const CXIMPIdentityImp* sourceIdentity =
    TXIMPGetImpClassOrPanic< const CXIMPIdentityImp >::From( aIdentity );
    iIdentity = TXIMPObjectCloner< CXIMPIdentityImp >::CloneL( *sourceIdentity );

    iDisplayName.CreateL( aDisplayName );
    }

// ---------------------------------------------------------------------------
// CPresentityGroupInfoImp::ExternalizeL()
// ---------------------------------------------------------------------------
//
void CPresentityGroupInfoImp::ExternalizeL( RWriteStream& aStream ) const
    {
    iIdentity->ExternalizeL( aStream );
    XIMPRBuf16Helper::ExternalizeL( iDisplayName, aStream );
    }

// ---------------------------------------------------------------------------
// CPresentityGroupInfoImp::InternalizeL()
// ---------------------------------------------------------------------------
//
EXPORT_C void CPresentityGroupInfoImp::InternalizeL( RReadStream& aStream )
    {
    iIdentity->InternalizeL( aStream );
    XIMPRBuf16Helper::InternalizeL( iDisplayName, aStream );
    }
    
// ---------------------------------------------------------------------------
// From MPresentityGroupInfo class.
// CPresentityGroupInfoImp::GroupId()
// ---------------------------------------------------------------------------
//
const MXIMPIdentity& CPresentityGroupInfoImp::GroupId() const
    {
    return *iIdentity;
    }


// ---------------------------------------------------------------------------
// From MPresentityGroupInfo class.
// CPresentityGroupInfoImp::GroupDisplayName()
// ---------------------------------------------------------------------------
//
const TDesC16& CPresentityGroupInfoImp::GroupDisplayName() const
    {
    return iDisplayName;
    }


// ---------------------------------------------------------------------------
// From MPresentityGroupInfo class.
// CPresentityGroupInfoImp::SetGroupIdL()
// ---------------------------------------------------------------------------
//
void CPresentityGroupInfoImp::SetGroupIdL( MXIMPIdentity* aIdentity )
    {
    CXIMPIdentityImp* identityImp =
    TXIMPGetImpClassOrPanic< CXIMPIdentityImp>::From( *aIdentity );

    delete iIdentity;
    iIdentity = identityImp; 
    }
  

// ---------------------------------------------------------------------------
// From MPresentityGroupInfo class.
// CPresentityGroupInfoImp::SetGroupDisplayNameL()
// ---------------------------------------------------------------------------
//
void CPresentityGroupInfoImp::SetGroupDisplayNameL( 
    const TDesC16& aDisplayName )
    {
    HBufC16* displayNameBuf = aDisplayName.AllocL();
    iDisplayName.Close();
    iDisplayName.Assign( displayNameBuf );
    }
    
    
// ---------------------------------------------------------------------------
// CPresentityGroupInfoImp::GroupIdLinearOrder
// ---------------------------------------------------------------------------
//
EXPORT_C TInt CPresentityGroupInfoImp::GroupIdLinearOrder( 
        const CPresentityGroupInfoImp& aA,
        const CPresentityGroupInfoImp& aB )
    {
    return aA.Identity().Compare( aB.Identity() );
    }

// ---------------------------------------------------------------------------
// CPresentityGroupInfoImp::Identity
// ---------------------------------------------------------------------------
//
EXPORT_C const CXIMPIdentityImp& CPresentityGroupInfoImp::Identity() const
    {
    return *iIdentity;
    }

// ---------------------------------------------------------------------------
// CPresentityGroupInfoImp::EqualsContent()
// ---------------------------------------------------------------------------
//
TBool CPresentityGroupInfoImp::EqualsContent( 
    const CXIMPApiDataObjBase& aOtherInstance ) const
    {
    const CPresentityGroupInfoImp* tmp =
    TXIMPGetImpClassOrPanic< const CPresentityGroupInfoImp >::From( 
                                                        aOtherInstance.Base() );
    
    TBool same = ETrue;
    TBool x;
    x = iIdentity->EqualsContent( *( tmp->iIdentity ) );
    same &= x;

    x = 0 == iDisplayName.Compare( tmp->iDisplayName );
    same &= x;

    return same;
    }

// End of file

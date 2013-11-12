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
* Description:  MPresentityGroupMemberInfo API object implementation.
*
*/


#include "presentitygroupmemberinfoimp.h"
#include "ximpidentityimp.h"
#include "ximprbufhelpers.h"
#include "ximpobjecthelpers.h"

// ============================ MEMBER FUNCTIONS =============================

// ---------------------------------------------------------------------------
// CPresentityGroupMemberInfoImp::NewLC()
// ---------------------------------------------------------------------------
//
EXPORT_C CPresentityGroupMemberInfoImp* CPresentityGroupMemberInfoImp::NewLC(
                                                const MXIMPIdentity& aIdentity,
                                                const TDesC16& aDisplayName )
    {
    CPresentityGroupMemberInfoImp* self = new( ELeave ) CPresentityGroupMemberInfoImp;
    CleanupStack::PushL( self );
    self->ConstructL( aIdentity, aDisplayName );
    return self;
    }

// ---------------------------------------------------------------------------
// CPresentityGroupMemberInfoImp::NewLC()
// ---------------------------------------------------------------------------
//
EXPORT_C CPresentityGroupMemberInfoImp* CPresentityGroupMemberInfoImp::NewLC()
    {
    CPresentityGroupMemberInfoImp* self = new( ELeave ) CPresentityGroupMemberInfoImp;
    CleanupStack::PushL( self );
    self->ConstructL();
    return self;
    }


// ---------------------------------------------------------------------------
// CPresentityGroupMemberInfoImp::NewL()
// ---------------------------------------------------------------------------
//
EXPORT_C CPresentityGroupMemberInfoImp* CPresentityGroupMemberInfoImp::NewL()
    {
    CPresentityGroupMemberInfoImp* self = NewLC();
    CleanupStack::Pop( self );
    return self;
    }
    
// ---------------------------------------------------------------------------
// CPresentityGroupMemberInfoImp::NewFromStreamLC()
// ---------------------------------------------------------------------------
//
XIMPIMP_IMPLEMENT_DATAOBJ_NEWFROMSTREAM( CPresentityGroupMemberInfoImp )

// ---------------------------------------------------------------------------
// Implement supported interface access.
// ---------------------------------------------------------------------------
//
XIMPIMP_IF_BASE_GET_INTERFACE_BEGIN( CPresentityGroupMemberInfoImp, MPresentityGroupMemberInfo )
XIMPIMP_IF_BASE_GET_INTERFACE_END()
XIMPIMP_IF_BASE_GET_CONST_INTERFACE_BEGIN( CPresentityGroupMemberInfoImp, MPresentityGroupMemberInfo )
XIMPIMP_IF_BASE_GET_INTERFACE_END()

// ---------------------------------------------------------------------------
// CPresentityGroupMemberInfoImp::~CPresentityGroupMemberInfoImp()
// ---------------------------------------------------------------------------
//
CPresentityGroupMemberInfoImp::~CPresentityGroupMemberInfoImp()
    {
    iDisplayName.Close();
    delete iIdentity;
    }

// ---------------------------------------------------------------------------
// CPresentityGroupMemberInfoImp::CPresentityGroupMemberInfoImp()
// ---------------------------------------------------------------------------
//
CPresentityGroupMemberInfoImp::CPresentityGroupMemberInfoImp()
    {
    }


// ---------------------------------------------------------------------------
// CPresentityGroupMemberInfoImp::ConstructL()
// ---------------------------------------------------------------------------
//
void CPresentityGroupMemberInfoImp::ConstructL()
    {
    iDisplayName.CreateL( 0 );
    iIdentity = CXIMPIdentityImp::NewLC();
    CleanupStack::Pop( iIdentity );
    }

// ---------------------------------------------------------------------------
// CPresentityGroupMemberInfoImp::ConstructL()
// ---------------------------------------------------------------------------
//
void CPresentityGroupMemberInfoImp::ConstructL( const MXIMPIdentity& aIdentity,
                                              const TDesC16& aDisplayName )
    {
    CXIMPIdentityImp* identity = CXIMPIdentityImp::NewLC();
    identity->SetIdentityL( aIdentity.Identity() );
    iDisplayName.CreateL( aDisplayName );
    iIdentity = identity;
    CleanupStack::Pop(); // identity
    }

// ---------------------------------------------------------------------------
// CPresentityGroupMemberInfoImp::ExternalizeL()
// ---------------------------------------------------------------------------
//
void CPresentityGroupMemberInfoImp::ExternalizeL( RWriteStream& aStream ) const
    {
    iIdentity->ExternalizeL( aStream );
    XIMPRBuf16Helper::ExternalizeL( iDisplayName, aStream );
    }

// ---------------------------------------------------------------------------
// CPresentityGroupMemberInfoImp::InternalizeL()
// ---------------------------------------------------------------------------
//
EXPORT_C void CPresentityGroupMemberInfoImp::InternalizeL( RReadStream& aStream )
    {
    iIdentity->InternalizeL( aStream );
    XIMPRBuf16Helper::InternalizeL( iDisplayName, aStream );
    }

// ---------------------------------------------------------------------------
// From MPresentityGroupMemberInfo class.
// CPresentityGroupMemberInfoImp::GroupMemberId()
// ---------------------------------------------------------------------------
//
const MXIMPIdentity& CPresentityGroupMemberInfoImp::GroupMemberId() const
    {
    return *iIdentity;
    }
    

// ---------------------------------------------------------------------------
// From MPresentityGroupMemberInfo class.
// CPresentityGroupMemberInfoImp::GroupMemberDisplayName()
// ---------------------------------------------------------------------------
//
const TDesC16& CPresentityGroupMemberInfoImp::GroupMemberDisplayName() const
    {
    return iDisplayName;
    }


// ---------------------------------------------------------------------------
// From MPresentityGroupMemberInfo class.
// CPresentityGroupMemberInfoImp::SetGroupMemberIdL()
// ---------------------------------------------------------------------------
//
void CPresentityGroupMemberInfoImp::SetGroupMemberIdL( 
    MXIMPIdentity* aIdentity )
    {
    CXIMPIdentityImp* identityImp =
    TXIMPGetImpClassOrPanic< CXIMPIdentityImp>::From( *aIdentity );

    delete iIdentity;
    iIdentity = identityImp; 
    }



// ---------------------------------------------------------------------------
// From MPresentityGroupMemberInfo class.
// CPresentityGroupMemberInfoImp::SetGroupMemberDisplayNameL()
// ---------------------------------------------------------------------------
//
void CPresentityGroupMemberInfoImp::SetGroupMemberDisplayNameL( 
    const TDesC16& aDisplayName )
    {
    HBufC16* displayNameBuf = aDisplayName.AllocL();
    iDisplayName.Close();
    iDisplayName.Assign( displayNameBuf );
    }
    


// ---------------------------------------------------------------------------
// CPresentityGroupMemberInfoImp::GroupIdLinearOrder
// ---------------------------------------------------------------------------
//
EXPORT_C TInt CPresentityGroupMemberInfoImp::GroupIdLinearOrder(
        const CPresentityGroupMemberInfoImp& aA,
        const CPresentityGroupMemberInfoImp& aB )
    {
    // identity is used as primary key
    return aA.iIdentity->Compare( *aB.iIdentity );
    }

// ---------------------------------------------------------------------------
// CPresentityGroupMemberInfoImp::Identity()
// ---------------------------------------------------------------------------
//
EXPORT_C const CXIMPIdentityImp& CPresentityGroupMemberInfoImp::IdentityImp() const
    {
    return *iIdentity;
    }

// ---------------------------------------------------------------------------
// CPresentityGroupMemberInfoImp::EqualsContent()
// ---------------------------------------------------------------------------
//
TBool CPresentityGroupMemberInfoImp::EqualsContent( 
    const CXIMPApiDataObjBase& aOtherInstance ) const
    {
    const CPresentityGroupMemberInfoImp* tmp =
    TXIMPGetImpClassOrPanic< const CPresentityGroupMemberInfoImp >::From( 
                                                        aOtherInstance.Base() );

    TBool same = ETrue;

    TBool x;
    x = iIdentity->EqualsContent( *(tmp->iIdentity) );
    same &= x;

    x = 0 == iDisplayName.Compare( tmp->iDisplayName );
    same &= x;

    return same;
    }

// End of file

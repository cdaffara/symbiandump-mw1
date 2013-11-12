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
* Description:  MPresenceWatcherInfo API object implementation.
*
*/


#include "presencewatcherinfoimp.h"
#include "ximpidentityimp.h"
#include "ximprbufhelpers.h"
#include "ximpobjecthelpers.h"

// ============================ MEMBER FUNCTIONS =============================

// ---------------------------------------------------------------------------
// CPresenceWatcherInfoImp::NewLC()
// ---------------------------------------------------------------------------
//
EXPORT_C CPresenceWatcherInfoImp* CPresenceWatcherInfoImp::NewLC(
                            const MPresenceWatcherInfo::TWatcherType aType,
                            const MXIMPIdentity& aIdentity,
                            const TDesC16& aDisplayName )
    {
    CPresenceWatcherInfoImp* self = new( ELeave ) CPresenceWatcherInfoImp;
    CleanupStack::PushL( self );
    self->ConstructL( aType, aIdentity, aDisplayName );
    return self;
    }

// ---------------------------------------------------------------------------
// CPresenceWatcherInfoImp::NewLC()
// ---------------------------------------------------------------------------
//
EXPORT_C CPresenceWatcherInfoImp* CPresenceWatcherInfoImp::NewLC()
    {
    CPresenceWatcherInfoImp* self = new( ELeave ) CPresenceWatcherInfoImp;
    CleanupStack::PushL( self );
    self->ConstructL();
    return self;
    }


// ---------------------------------------------------------------------------
// CPresenceWatcherInfoImp::NewL()
// ---------------------------------------------------------------------------
//
EXPORT_C CPresenceWatcherInfoImp* CPresenceWatcherInfoImp::NewL()
    {
    CPresenceWatcherInfoImp* self = NewLC();
    CleanupStack::Pop( self );
    return self;
    }

// ---------------------------------------------------------------------------
// CPresenceWatcherInfoImp::NewFromStreamLC()
// ---------------------------------------------------------------------------
//
XIMPIMP_IMPLEMENT_DATAOBJ_NEWFROMSTREAM( CPresenceWatcherInfoImp )


// ---------------------------------------------------------------------------
// Implement supported interface access.
// ---------------------------------------------------------------------------
//
XIMPIMP_IF_BASE_GET_INTERFACE_BEGIN( CPresenceWatcherInfoImp, 
                                     MPresenceWatcherInfo )
XIMPIMP_IF_BASE_GET_INTERFACE_END()


XIMPIMP_IF_BASE_GET_CONST_INTERFACE_BEGIN( CPresenceWatcherInfoImp, 
                                           MPresenceWatcherInfo )
XIMPIMP_IF_BASE_GET_INTERFACE_END()

// ---------------------------------------------------------------------------
// CPresenceWatcherInfoImp::~CPresenceWatcherInfoImp()
// ---------------------------------------------------------------------------
//
CPresenceWatcherInfoImp::~CPresenceWatcherInfoImp()
    {
    iDisplayName.Close();
    delete iIdentity;
    }

// ---------------------------------------------------------------------------
// CPresenceWatcherInfoImp::CPresenceWatcherInfoImp()
// ---------------------------------------------------------------------------
//
CPresenceWatcherInfoImp::CPresenceWatcherInfoImp()
    {
    }


// ---------------------------------------------------------------------------
// CPresenceWatcherInfoImp::ConstructL()
// ---------------------------------------------------------------------------
//
void CPresenceWatcherInfoImp::ConstructL()
    {
    iDisplayName.CreateL( 0 );
    iIdentity = CXIMPIdentityImp::NewLC();
    CleanupStack::Pop( iIdentity );
    }

// ---------------------------------------------------------------------------
// CPresenceWatcherInfoImp::ConstructL()
// ---------------------------------------------------------------------------
//
void CPresenceWatcherInfoImp::ConstructL(
                            const MPresenceWatcherInfo::TWatcherType aType,
                            const MXIMPIdentity& aIdentity,
                            const TDesC16& aDisplayName )
    {
    iType = aType;

    const CXIMPIdentityImp* sourceIdentity =
    TXIMPGetImpClassOrPanic< const CXIMPIdentityImp >::From( aIdentity );
    iIdentity = TXIMPObjectCloner< CXIMPIdentityImp >::CloneL( *sourceIdentity );
    
    iDisplayName.CreateL( aDisplayName );
    }

// ---------------------------------------------------------------------------
// CPresenceWatcherInfoImp::ExternalizeL()
// ---------------------------------------------------------------------------
//
void CPresenceWatcherInfoImp::ExternalizeL( RWriteStream& aStream ) const
    {
    aStream.WriteInt32L( iType );
    iIdentity->ExternalizeL( aStream );
    XIMPRBuf16Helper::ExternalizeL( iDisplayName, aStream );
    }

// ---------------------------------------------------------------------------
// CPresenceWatcherInfoImp::InternalizeL()
// ---------------------------------------------------------------------------
//
EXPORT_C void CPresenceWatcherInfoImp::InternalizeL( RReadStream& aStream )
    {
    iType = ( MPresenceWatcherInfo::TWatcherType ) aStream.ReadInt32L();
    iIdentity->InternalizeL( aStream );
    XIMPRBuf16Helper::InternalizeL( iDisplayName, aStream );
    }

// ---------------------------------------------------------------------------
// From MPresenceWatcherInfo class.
// CPresenceWatcherInfoImp::WatcherId()
// ---------------------------------------------------------------------------
//
const MXIMPIdentity& CPresenceWatcherInfoImp::WatcherId() const
    {
    return *iIdentity;
    }

// ---------------------------------------------------------------------------
// From MPresenceWatcherInfo class.
// CPresenceWatcherInfoImp::WatcherDisplayName()
// ---------------------------------------------------------------------------
//
const TDesC16& CPresenceWatcherInfoImp::WatcherDisplayName() const
    {
    return iDisplayName;
    }
    
    

// ---------------------------------------------------------------------------
// From MPresenceWatcherInfo class.
// CPresenceWatcherInfoImp::WatcherType()
// ---------------------------------------------------------------------------
//
MPresenceWatcherInfo::TWatcherType CPresenceWatcherInfoImp::WatcherType() const
    {
    return iType;
    }    
    
    
// ---------------------------------------------------------------------------
// From MPresenceWatcherInfo class.
// CPresenceWatcherInfoImp::SetWatcherIdL()
// ---------------------------------------------------------------------------
//
void CPresenceWatcherInfoImp::SetWatcherIdL( MXIMPIdentity* aIdentity )
    {
    User::LeaveIfError( aIdentity == NULL ? KErrArgument : KErrNone );

    CXIMPIdentityImp* identityImp =
    TXIMPGetImpClassOrPanic< CXIMPIdentityImp>::From( *aIdentity );

    delete iIdentity;
    iIdentity = identityImp; 
    }    


// ---------------------------------------------------------------------------
// From MPresenceWatcherInfo class.
// CPresenceWatcherInfoImp::SetWatcherDisplayNameL()
// ---------------------------------------------------------------------------
//
void CPresenceWatcherInfoImp::SetWatcherDisplayNameL( 
    const TDesC16& aDisplayName )
    {
    HBufC16* displayNameBuf = aDisplayName.AllocL();
    iDisplayName.Close();
    iDisplayName.Assign( displayNameBuf );
    }    


// ---------------------------------------------------------------------------
// From MPresenceWatcherInfo class.
// CPresenceWatcherInfoImp::SetWatcherTypeL()
// ---------------------------------------------------------------------------
//
void CPresenceWatcherInfoImp::SetWatcherTypeL( 
    MPresenceWatcherInfo::TWatcherType aType )
    {
    iType = aType;
    }    


// ---------------------------------------------------------------------------
// CPresenceWatcherInfoImp::SetIdentityL()
// ---------------------------------------------------------------------------
//
EXPORT_C void CPresenceWatcherInfoImp::SetIdentity( CXIMPIdentityImp* aIdentity )
    {
    delete iIdentity;
    iIdentity = aIdentity;
    }

// ---------------------------------------------------------------------------
// CPresenceWatcherInfoImp::SetIdentityL()
// ---------------------------------------------------------------------------
//
EXPORT_C void CPresenceWatcherInfoImp::SetDisplayName( HBufC16* aDisplayName )
    {
    iDisplayName.Close();
    iDisplayName.Assign( aDisplayName );
    }

// ---------------------------------------------------------------------------
// CPresenceWatcherInfoImp::GroupIdLinearOrder()
// ---------------------------------------------------------------------------
//
EXPORT_C TInt CPresenceWatcherInfoImp::IdLinearOrder(
        const CPresenceWatcherInfoImp& aA,
        const CPresenceWatcherInfoImp& aB )
    {
    return aA.IdentityImp().Compare( aB.IdentityImp() );
    }
// ---------------------------------------------------------------------------
// CPresenceWatcherInfoImp::Identity()
// ---------------------------------------------------------------------------
//
EXPORT_C const CXIMPIdentityImp& CPresenceWatcherInfoImp::IdentityImp() const
    {
    return *iIdentity;
    }

// ---------------------------------------------------------------------------
// CPresenceWatcherInfoImp::EqualsContent()
// ---------------------------------------------------------------------------
//
TBool CPresenceWatcherInfoImp::EqualsContent( 
    const CXIMPApiDataObjBase& aOtherInstance ) const
    {
    const CPresenceWatcherInfoImp* tmp =
    TXIMPGetImpClassOrPanic< const CPresenceWatcherInfoImp >::From( 
                                                        aOtherInstance.Base() );

    TBool same = ETrue;
    TBool x;
    x = iIdentity->EqualsContent( *( tmp->iIdentity ) );
    same &= x;

    x = 0 == iDisplayName.Compare( tmp->iDisplayName );
    same &= x;

    x = iType == iType;
    same &= x;

    // add more comparisons in a similar way...

    return same;
    }

// End of file

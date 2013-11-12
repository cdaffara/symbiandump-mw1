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
* Description:  MXIMPServiceInfo API object implementation.
*
*/

#include "ximpserviceinfoimp.h"
#include "ximprbufhelpers.h"



// ============================ MEMBER FUNCTIONS =============================


// ---------------------------------------------------------------------------
// CXIMPServiceInfoImp::NewLC()
// ---------------------------------------------------------------------------
//
EXPORT_C CXIMPServiceInfoImp* CXIMPServiceInfoImp::NewLC()
    {
    CXIMPServiceInfoImp* self = new( ELeave ) CXIMPServiceInfoImp;
    CleanupStack::PushL( self );
    self->ConstructL();
    return self;
    }
    

// ---------------------------------------------------------------------------
// CXIMPServiceInfoImp::NewL()
// ---------------------------------------------------------------------------
//
EXPORT_C CXIMPServiceInfoImp* CXIMPServiceInfoImp::NewL()
    {
    CXIMPServiceInfoImp* self = NewLC();
    CleanupStack::Pop( self );
    return self;
    }
    


// ---------------------------------------------------------------------------
// CXIMPServiceInfoImp::NewLC()
// ---------------------------------------------------------------------------
//
EXPORT_C CXIMPServiceInfoImp* CXIMPServiceInfoImp::NewLC(
                                             TUid aProtocolImplUid,
                                             const TDesC16& aServiceAddress,
                                             const TDesC16& aUsername,
                                             const TDesC16& aPassword,
                                             TInt32 aIapId )
    {
    CXIMPServiceInfoImp* self = new( ELeave ) CXIMPServiceInfoImp();
    CleanupStack::PushL( self );
    self->ConstructL( aProtocolImplUid, aServiceAddress, aUsername, aPassword, aIapId );
    return self;
    }

// ---------------------------------------------------------------------------
// CXIMPServiceInfoImp::~CXIMPServiceInfoImp()
// ---------------------------------------------------------------------------
//
CXIMPServiceInfoImp::~CXIMPServiceInfoImp()
    {
    iAddress.Close();
    iUserId.Close();
    iPassword.Close();
    }


// ---------------------------------------------------------------------------
// CXIMPServiceInfoImp::CXIMPServiceInfoImp()
// ---------------------------------------------------------------------------
//
CXIMPServiceInfoImp::CXIMPServiceInfoImp()
    {
    }


// ---------------------------------------------------------------------------
// CXIMPServiceInfoImp::ConstructL()
// ---------------------------------------------------------------------------
//
void CXIMPServiceInfoImp::ConstructL( TUid aProtocolImplUid,
                                      const TDesC16& aServiceAddress,
                                      const TDesC16& aUsername,
                                      const TDesC16& aPassword,
                                      TInt32 aIapId )
    {
    iProtocolUid = aProtocolImplUid;
    iIapId = aIapId;
    iAddress.CreateL( aServiceAddress );
    iUserId.CreateL( aUsername );
    iPassword.CreateL( aPassword );
    }


// ---------------------------------------------------------------------------
// CXIMPServiceInfoImp::ConstructL()
// ---------------------------------------------------------------------------
//
void CXIMPServiceInfoImp::ConstructL()
    {
    // Initialize RBufs to zero length, realloc later when needed
    iAddress.CreateL( 0 );
    iUserId.CreateL( 0 );
    iPassword.CreateL( 0 );
    }

// ---------------------------------------------------------------------------
// CXIMPServiceInfoImp::NewFromStreamLC()
// ---------------------------------------------------------------------------
//
XIMPIMP_IMPLEMENT_DATAOBJ_NEWFROMSTREAM( CXIMPServiceInfoImp )


// ---------------------------------------------------------------------------
// Implement supported interface access.
// ---------------------------------------------------------------------------
//
XIMPIMP_IF_BASE_GET_INTERFACE_BEGIN( CXIMPServiceInfoImp, 
                                     MXIMPServiceInfo )
XIMPIMP_IF_BASE_GET_INTERFACE_END()


XIMPIMP_IF_BASE_GET_CONST_INTERFACE_BEGIN( CXIMPServiceInfoImp, 
                                           MXIMPServiceInfo )
XIMPIMP_IF_BASE_GET_INTERFACE_END()


// ---------------------------------------------------------------------------
// From class MXIMPServiceInfo.
// CXIMPServiceInfoImp::Address()
// ---------------------------------------------------------------------------
//
const TDesC16& CXIMPServiceInfoImp::ServiceAddress() const
    {
    return iAddress;
    }


// ---------------------------------------------------------------------------
// From class MXIMPServiceInfo.
// CXIMPServiceInfoImp::UserId()
// ---------------------------------------------------------------------------
//
const TDesC16& CXIMPServiceInfoImp::UserId() const
    {
    return iUserId;
    }


// ---------------------------------------------------------------------------
// From class MXIMPServiceInfo.
// CXIMPServiceInfoImp::Password()
// ---------------------------------------------------------------------------
//
const TDesC16& CXIMPServiceInfoImp::Password() const
    {
    return iPassword;
    }


// ---------------------------------------------------------------------------
// From class MXIMPServiceInfo.
// CXIMPServiceInfoImp::IapId()
// ---------------------------------------------------------------------------
//
TInt32 CXIMPServiceInfoImp::IapId() const
    {
    return iIapId;
    }


// ---------------------------------------------------------------------------
// From class MXIMPServiceInfo.
// CXIMPServiceInfoImp::ProtocolImplUid()
// ---------------------------------------------------------------------------
//
TUid CXIMPServiceInfoImp::ProtocolImplUid() const
    {
    return iProtocolUid;
    }


// ---------------------------------------------------------------------------
// CXIMPServiceInfoImp::ExternalizeL()
// ---------------------------------------------------------------------------
//
void CXIMPServiceInfoImp::ExternalizeL( RWriteStream& aStream ) const
    {
    XIMPRBuf16Helper::ExternalizeL( iAddress, aStream );
    XIMPRBuf16Helper::ExternalizeL( iUserId, aStream );
    XIMPRBuf16Helper::ExternalizeL( iPassword, aStream );

    aStream.WriteInt32L( iProtocolUid.iUid );
    aStream.WriteInt32L( iIapId );
    }


// ---------------------------------------------------------------------------
// CXIMPServiceInfoImp::InternalizeL()
// ---------------------------------------------------------------------------
//
EXPORT_C void CXIMPServiceInfoImp::InternalizeL( RReadStream& aStream )
    {
    XIMPRBuf16Helper::InternalizeL( iAddress, aStream );
    XIMPRBuf16Helper::InternalizeL( iUserId, aStream );
    XIMPRBuf16Helper::InternalizeL( iPassword, aStream );

    iProtocolUid.iUid = aStream.ReadInt32L();
    iIapId = aStream.ReadInt32L();
    }

// ---------------------------------------------------------------------------
// CXIMPServiceInfoImp::EqualsContent()
// ---------------------------------------------------------------------------
//
TBool CXIMPServiceInfoImp::EqualsContent( 
    const CXIMPApiDataObjBase& aOtherInstance ) const
    {
    const CXIMPServiceInfoImp* tmp =
    TXIMPGetImpClassOrPanic< const CXIMPServiceInfoImp >::From( aOtherInstance.Base() );
    

    TBool same = ETrue;
    TBool x = EFalse;

    if( ServiceAddress().Compare( tmp->ServiceAddress() ) == 0 &&
        UserId().Compare( tmp->UserId() ) == 0 &&
        Password().Compare( tmp->Password() ) == 0 &&
        ProtocolImplUid() == tmp->ProtocolImplUid() )
        {
        x = ETrue;
        }
    
    same &= x;

    return same;
    }


// End of file

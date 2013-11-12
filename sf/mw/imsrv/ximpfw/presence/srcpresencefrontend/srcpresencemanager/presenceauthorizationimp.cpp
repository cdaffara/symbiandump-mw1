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
* Description:  Implementation of MPresenceAuthorization
 *
*/

#include "presenceauthorizationimp.h"
#include "presenceinfofilterimp.h"
#include "ximpobjecthelpers.h"
#include "presenceoperationdefs.h"
#include "ximpidentityimp.h"
#include "ximpcontextinternal.h"


// ============================ MEMBER FUNCTIONS =============================

// ---------------------------------------------------------------------------
// CPresenceAuthorizationImp::NewL()
// ---------------------------------------------------------------------------
//
CPresenceAuthorizationImp* CPresenceAuthorizationImp::NewL(
    MXIMPContextInternal& aContext )
    {
    CPresenceAuthorizationImp* self =
        new( ELeave ) CPresenceAuthorizationImp( aContext );

    return self;
    }


// ---------------------------------------------------------------------------
// Implement supported interface access.
// ---------------------------------------------------------------------------
//
XIMPIMP_IF_BASE_GET_INTERFACE_BEGIN( CPresenceAuthorizationImp, 
                                     MPresenceAuthorization )
XIMPIMP_IF_BASE_GET_INTERFACE_END()


XIMPIMP_IF_BASE_GET_CONST_INTERFACE_BEGIN( CPresenceAuthorizationImp, 
                                           MPresenceAuthorization )
XIMPIMP_IF_BASE_GET_INTERFACE_END()

// ---------------------------------------------------------------------------
// CPresenceAuthorizationImp::~CPresenceAuthorizationImp()
// ---------------------------------------------------------------------------
//
CPresenceAuthorizationImp::~CPresenceAuthorizationImp()
    {
    //iContext not own
    }


// ---------------------------------------------------------------------------
// CPresenceAuthorizationImp::CPresenceAuthorizationImp()
// ---------------------------------------------------------------------------
//
CPresenceAuthorizationImp::CPresenceAuthorizationImp(
    MXIMPContextInternal& aContext )
    : iContext( aContext )
    {
    }


// ---------------------------------------------------------------------------
// CPresenceAuthorizationImp::SubscribePresenceGrantRequestListL()
// ---------------------------------------------------------------------------
//
TXIMPRequestId CPresenceAuthorizationImp::SubscribePresenceGrantRequestListL()
    {
    return iContext.QueueOperationL(
                       NPresenceOps::EPrSubscribePresenceGrantRequestList,
                       KNullDesC8 );
    }

// ---------------------------------------------------------------------------
// CPresenceAuthorizationImp::UnsubscribePresenceGrantRequestListL()
// ---------------------------------------------------------------------------
//
TXIMPRequestId CPresenceAuthorizationImp::UnsubscribePresenceGrantRequestListL()
    {
    return iContext.QueueOperationL(
                       NPresenceOps::EPrUnsubscribePresenceGrantRequestList,
                       KNullDesC8 );
    }


// ---------------------------------------------------------------------------
// CPresenceAuthorizationImp::GrantPresenceForPresentityL()
// ---------------------------------------------------------------------------
//
TXIMPRequestId CPresenceAuthorizationImp::GrantPresenceForPresentityL(
    const MXIMPIdentity& aPresentityId,
    const MPresenceInfoFilter& aPif )
    {
    return QueueOperationL( NPresenceOps::EPrGrantPresenceToPresentity,
                            aPif, aPresentityId );
    }


// ---------------------------------------------------------------------------
// CPresenceAuthorizationImp::WithdrawPresenceGrantFromPresentityL()
// ---------------------------------------------------------------------------
//
TXIMPRequestId CPresenceAuthorizationImp::WithdrawPresenceGrantFromPresentityL(
    const MXIMPIdentity& aPresentityId )
    {
    HBufC8* idPack = PackPresentityLC( aPresentityId );

    TXIMPRequestId reqId =
        iContext.QueueOperationL(
                        NPresenceOps::EPrWithdrawPresenceFromPresentity,
                        *idPack );
    CleanupStack::PopAndDestroy( idPack );
    return reqId;
    }


// ---------------------------------------------------------------------------
// CPresenceAuthorizationImp::GrantPresenceForPresentityGroupMembersL()
// ---------------------------------------------------------------------------
//
TXIMPRequestId CPresenceAuthorizationImp::GrantPresenceForPresentityGroupMembersL(
    const MXIMPIdentity& aGroupId,
    const MPresenceInfoFilter& aPif )
    {
    return QueueOperationL( NPresenceOps::EPrGrantPresenceToPresentityGroupMembers,
                            aPif, aGroupId );
    }


// ---------------------------------------------------------------------------
// CPresenceAuthorizationImp::WithdrawPresenceGrantFromPresentityGroupMembersL()
// ---------------------------------------------------------------------------
//
TXIMPRequestId
    CPresenceAuthorizationImp::WithdrawPresenceGrantFromPresentityGroupMembersL(
    const MXIMPIdentity& aGroupId )
    {
    HBufC8* idPack = PackPresentityLC( aGroupId );


    TXIMPRequestId reqId =
        iContext.QueueOperationL(
                        NPresenceOps::EPrWithdrawPresenceFromPresentityGroupMembers,
                        *idPack );

    CleanupStack::PopAndDestroy( idPack );
    return reqId;
    }


// ---------------------------------------------------------------------------
// CPresenceAuthorizationImp::GrantPresenceForPresentityL()
// ---------------------------------------------------------------------------
//
TXIMPRequestId CPresenceAuthorizationImp::GrantPresenceForEveryoneL( 
    const MPresenceInfoFilter& aPif )
    {
    HBufC8* pifPack = PackPresenceInfoFilterLC( aPif );

    TXIMPRequestId reqId =
        iContext.QueueOperationL( 
                        NPresenceOps::EPrGrantPresenceToEveryone,
                        *pifPack );
    CleanupStack::PopAndDestroy( pifPack );
    return reqId;            
    }

// ---------------------------------------------------------------------------
// CPresenceAuthorizationImp::WithdrawPresenceGrantFromPresentityL()
// ---------------------------------------------------------------------------
//
TXIMPRequestId CPresenceAuthorizationImp::WithdrawPresenceGrantFromEveryoneL()
    {
    TXIMPRequestId reqId =
        iContext.QueueOperationL( 
                        NPresenceOps::EPrWithdrawPresenceFromEveryone,
                        KNullDesC8 );
    return reqId;
    }


// ---------------------------------------------------------------------------
// CPresenceAuthorizationImp::SubscribePresenceBlockListL()
// ---------------------------------------------------------------------------
//
TXIMPRequestId CPresenceAuthorizationImp::SubscribePresenceBlockListL()
    {
    // Queue the operation
    return  iContext.QueueOperationL(
                        NPresenceOps::EPrSubscribeBlockList,
                        KNullDesC8 );
    }

// ---------------------------------------------------------------------------
// CPresenceAuthorizationImp::UnsubscribePresenceBlockListL()
// ---------------------------------------------------------------------------
//
TXIMPRequestId CPresenceAuthorizationImp::UnsubscribePresenceBlockListL()
    {
    // Queue the operation
    return iContext.QueueOperationL(
                        NPresenceOps::EPrUnsubscribeBlockList,
                        KNullDesC8 );
    }


// ---------------------------------------------------------------------------
// CPresenceAuthorizationImp::BlockPresenceForPresentityL()
// ---------------------------------------------------------------------------
//
TXIMPRequestId CPresenceAuthorizationImp::BlockPresenceForPresentityL(
    const MXIMPIdentity& aPresentityId )
    {
    HBufC8* idPack = PackPresentityLC( aPresentityId );

    TXIMPRequestId reqId =
        iContext.QueueOperationL(
                        NPresenceOps::EPrBlockPresenceForPresentity,
                        *idPack );

    CleanupStack::PopAndDestroy( idPack );
    return reqId;
    }


// ---------------------------------------------------------------------------
// CPresenceAuthorizationImp::CancelPresenceBlockFromPresentityL()
// ---------------------------------------------------------------------------
//
TXIMPRequestId CPresenceAuthorizationImp::CancelPresenceBlockFromPresentityL(
    const MXIMPIdentity& aPresentityId )
    {
    HBufC8* idPack = PackPresentityLC( aPresentityId );

    TXIMPRequestId reqId =
        iContext.QueueOperationL(
                        NPresenceOps::EPrCancelPresenceBlockFromPresentity,
                        *idPack );

    CleanupStack::PopAndDestroy( idPack );
    return reqId;
    }


// ---------------------------------------------------------------------------
// CPresenceAuthorizationImp::PackPresenceDocLC()
// ---------------------------------------------------------------------------
//
HBufC8* CPresenceAuthorizationImp::PackPresenceInfoFilterLC(
    const MPresenceInfoFilter& aPif )
    {
    const CPresenceInfoFilterImp* pifImp = 
        TXIMPGetImpClassOrPanic< const CPresenceInfoFilterImp >::From( aPif );
    
    HBufC8* pack = TXIMPObjectPacker< CPresenceInfoFilterImp>::PackL( *pifImp );
    CleanupStack::PushL( pack );

    return pack;
    }

// ---------------------------------------------------------------------------
// CPresenceAuthorizationImp::QueueOperationL()
// ---------------------------------------------------------------------------
//
TXIMPRequestId CPresenceAuthorizationImp::QueueOperationL(
    NPresenceOps::TPresenceOpTypes aOperation,
    const MPresenceInfoFilter& aPif,
    const MXIMPIdentity& aIdentity )
    {
    CBufFlat* buffer = CBufFlat::NewL( 10 );
    CleanupStack::PushL( buffer );

    RBufWriteStream ws;
    CleanupClosePushL( ws );
    ws.Open( *buffer ); // CSI: 65 # Does not return a value

    // pif
    const CPresenceInfoFilterImp* pifImp =
        TXIMPGetImpClassOrPanic< const CPresenceInfoFilterImp >::From( aPif );
    pifImp->ExternalizeL( ws );


    // identity
    const CXIMPIdentityImp* identityImp =
        TXIMPGetImpClassOrPanic< const CXIMPIdentityImp >::From( aIdentity );
    identityImp->ExternalizeL( ws );

    ws.CommitL();
    CleanupStack::PopAndDestroy();  //ws

    HBufC8* packedBuf = buffer->Ptr( 0 ).AllocL();
    CleanupStack::PopAndDestroy( buffer );
    CleanupStack::PushL( packedBuf );

    // Queue the operation
    TXIMPRequestId reqId =
        iContext.QueueOperationL( aOperation, *packedBuf );
    CleanupStack::PopAndDestroy( packedBuf );

    return reqId;
    }

// ---------------------------------------------------------------------------
// CPresenceAuthorizationImp::PackPresentityLC()
// ---------------------------------------------------------------------------
//
HBufC8* CPresenceAuthorizationImp::PackPresentityLC( const MXIMPIdentity& aIdentity )
    {
    const CXIMPIdentityImp* identityImp =
            TXIMPGetImpClassOrPanic< const CXIMPIdentityImp >::From( aIdentity );
    
    HBufC8* pack = TXIMPObjectPacker< const CXIMPIdentityImp >::PackL( *identityImp );
    CleanupStack::PushL( pack );

    return pack;
    }


// End of file


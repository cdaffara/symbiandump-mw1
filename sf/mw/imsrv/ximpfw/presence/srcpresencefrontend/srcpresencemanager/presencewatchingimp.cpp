/*
* Copyright (c) 2006, 2007 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Implementation of MPresenceWatching
 *
*/

#include "presencewatchingimp.h"
#include "presenceinfofilterimp.h"
#include "ximpobjecthelpers.h"
#include "presenceoperationdefs.h"
#include "ximpidentityimp.h"
#include "ximpcontextinternal.h"


// ============================ MEMBER FUNCTIONS =============================

// ---------------------------------------------------------------------------
// CPresenceWatchingImp::NewL()
// ---------------------------------------------------------------------------
//
CPresenceWatchingImp* CPresenceWatchingImp::NewL(
    MXIMPContextInternal& aContext )
    {
    CPresenceWatchingImp* self =
        new( ELeave ) CPresenceWatchingImp( aContext );

    return self;
    }

// ---------------------------------------------------------------------------
// Implement supported interface access.
// ---------------------------------------------------------------------------
//
XIMPIMP_IF_BASE_GET_INTERFACE_BEGIN( CPresenceWatchingImp, 
                                     MPresenceWatching )
XIMPIMP_IF_BASE_GET_INTERFACE_END()


XIMPIMP_IF_BASE_GET_CONST_INTERFACE_BEGIN( CPresenceWatchingImp, 
                                           MPresenceWatching )
XIMPIMP_IF_BASE_GET_INTERFACE_END()


// ---------------------------------------------------------------------------
// CPresenceWatchingImp::~CPresenceWatchingImp()
// ---------------------------------------------------------------------------
//
CPresenceWatchingImp::~CPresenceWatchingImp()
    {
    //iContext not own
    }


// ---------------------------------------------------------------------------
// CPresenceWatchingImp::CPresenceWatchingImp()
// ---------------------------------------------------------------------------
//
CPresenceWatchingImp::CPresenceWatchingImp(
    MXIMPContextInternal& aContext )
    : iContext( aContext )
    {
    }


// ---------------------------------------------------------------------------
// CPresenceWatchingImp::SubscribePresentityPresenceL()
// ---------------------------------------------------------------------------
//
TXIMPRequestId CPresenceWatchingImp::SubscribePresentityPresenceL( 
        const MXIMPIdentity& aPresentityId,
        const MPresenceInfoFilter& aPif )
    {
    RXIMPObjOwningPtrArray< HBufC8 > packArray;
    CleanupClosePushL( packArray );

    // Pack the identity
    HBufC8* idPack = PackPresentityLC( aPresentityId );
    packArray.Append( idPack );
    CleanupStack::Pop( idPack );

    // pack the filter
    HBufC8* filterPack = PackFilterLC( aPif );
    packArray.Append( filterPack );
    CleanupStack::Pop( filterPack );

    // pack the array to a string
    HBufC8* packedBuf = TXIMPHBuf8Packer::PackArrayL( packArray );
    CleanupStack::PushL( packedBuf );

    // Queue the operation
    TXIMPRequestId reqId =
        iContext.QueueOperationL( NPresenceOps::EPrSubscribePresentityPresence,
                                   *packedBuf );

    CleanupStack::PopAndDestroy( packedBuf );
    CleanupStack::PopAndDestroy(); // packArray

    return reqId;
    }


// ---------------------------------------------------------------------------
// CPresenceWatchingImp::UnsubscribePresentityPresenceL()
// ---------------------------------------------------------------------------
//
TXIMPRequestId CPresenceWatchingImp::UnsubscribePresentityPresenceL( 
    const MXIMPIdentity& aPresentityId )
    {
    // Pack the identity
    HBufC8* idPack = PackPresentityLC( aPresentityId );
    // Queue the operation
    TXIMPRequestId reqId =
        iContext.QueueOperationL( NPresenceOps::EPrUnsubscribePresentityPresence,
                                   *idPack );
    CleanupStack::PopAndDestroy( idPack );
    return reqId;
    }


// ---------------------------------------------------------------------------
// CPresenceWatchingImp::SubscribePresentityGroupMembersPresenceL()
// ---------------------------------------------------------------------------
//
TXIMPRequestId CPresenceWatchingImp::SubscribePresentityGroupMembersPresenceL(
    const MXIMPIdentity& aGroupId,
    const MPresenceInfoFilter& aPif )
    {
    RXIMPObjOwningPtrArray< HBufC8 > packArray;
    CleanupClosePushL( packArray );

    // Pack the identity
    HBufC8* idPack = PackPresentityLC( aGroupId );
    packArray.Append( idPack );
    CleanupStack::Pop( idPack );

    // pack the filter
    HBufC8* filterPack = PackFilterLC( aPif );
    packArray.Append( filterPack );
    CleanupStack::Pop( filterPack );

    // pack the array to a string
    HBufC8* packedBuf = TXIMPHBuf8Packer::PackArrayL( packArray );
    CleanupStack::PushL( packedBuf );

    // Queue the operation
    TXIMPRequestId reqId =
        iContext.QueueOperationL( NPresenceOps::EPrSubscribePresentityGroupMembersPresence,
                                   *packedBuf );

    CleanupStack::PopAndDestroy( packedBuf );
    CleanupStack::PopAndDestroy(); // packArray

    return reqId;
    }


// ---------------------------------------------------------------------------
// CPresenceWatchingImp::UnsubscribePresentityGroupMembersPresenceL()
// ---------------------------------------------------------------------------
//
TXIMPRequestId CPresenceWatchingImp::UnsubscribePresentityGroupMembersPresenceL(
    const MXIMPIdentity& aGroupId )
    {
    // Pack the identity
    HBufC8* idPack = PackPresentityLC( aGroupId );
    // Queue the operation
    TXIMPRequestId reqId =
        iContext.QueueOperationL(
                        NPresenceOps::EPrUnsubscribePresentityGroupMembersPresence,
                        *idPack );
    CleanupStack::PopAndDestroy( idPack );
    return reqId;
    }


// ---------------------------------------------------------------------------
// CPresenceWatchingImp::PackPresentityLC()
// ---------------------------------------------------------------------------
//
HBufC8* CPresenceWatchingImp::PackPresentityLC( 
    const MXIMPIdentity& aIdentity )
    {
    const CXIMPIdentityImp* identityImp =
        TXIMPGetImpClassOrPanic< const CXIMPIdentityImp >::From( aIdentity );
    
    HBufC8* pack = TXIMPObjectPacker< const CXIMPIdentityImp >::PackL( *identityImp );
    CleanupStack::PushL( pack );

    return pack;
    }

// ---------------------------------------------------------------------------
// CPresenceWatchingImp::PackFilterLC()
// ---------------------------------------------------------------------------
//
HBufC8* CPresenceWatchingImp::PackFilterLC( 
    const MPresenceInfoFilter& aFilter )
    {
    const CPresenceInfoFilterImp* filterImp =
        TXIMPGetImpClassOrPanic< const CPresenceInfoFilterImp >::From( aFilter );
    
    HBufC8* pack = TXIMPObjectPacker< const CPresenceInfoFilterImp >::PackL( *filterImp );
    CleanupStack::PushL( pack );

    return pack;
    }


// End of file


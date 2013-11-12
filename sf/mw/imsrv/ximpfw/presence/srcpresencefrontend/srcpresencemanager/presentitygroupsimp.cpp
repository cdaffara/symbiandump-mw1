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
* Description:  Implementation of MPresenceManagement
 *
*/

#include "presentitygroupsimp.h"
#include "presenceinfoimp.h"
#include "presenceinfofilterimp.h"
#include "ximpobjecthelpers.h"
#include "presentitygroupinfoimp.h"
#include "presentitygroupmemberinfoimp.h"
#include "ximpidentityimp.h"
#include "ximpcontextinternal.h"


// ============================ MEMBER FUNCTIONS =============================

// ---------------------------------------------------------------------------
// CPresentityGroupsImp::NewL()
// ---------------------------------------------------------------------------
//
CPresentityGroupsImp* CPresentityGroupsImp::NewL( 
    MXIMPContextInternal& aContext )
    {
    CPresentityGroupsImp* self = 
        new( ELeave ) CPresentityGroupsImp( aContext );

    return self;
    }

// ---------------------------------------------------------------------------
// Implement supported interface access.
// ---------------------------------------------------------------------------
//
XIMPIMP_IF_BASE_GET_INTERFACE_BEGIN( CPresentityGroupsImp, 
                                     MPresentityGroups )
XIMPIMP_IF_BASE_GET_INTERFACE_END()

XIMPIMP_IF_BASE_GET_CONST_INTERFACE_BEGIN( CPresentityGroupsImp, 
                                           MPresentityGroups )
XIMPIMP_IF_BASE_GET_INTERFACE_END()

// ---------------------------------------------------------------------------
// CPresentityGroupsImp::~CPresentityGroupsImp()
// ---------------------------------------------------------------------------
//
CPresentityGroupsImp::~CPresentityGroupsImp()
    {
    //iContext not own
    }


// ---------------------------------------------------------------------------
// CPresentityGroupsImp::CPresentityGroupsImp()
// ---------------------------------------------------------------------------
//
CPresentityGroupsImp::CPresentityGroupsImp(
    MXIMPContextInternal& aContext )
    : iContext( aContext )
    {
    }
    
// ---------------------------------------------------------------------------
// CPresentityGroupsImp::SubscribeOwnPresenceL()
// ---------------------------------------------------------------------------
//
TXIMPRequestId CPresentityGroupsImp::SubscribePresentityGroupListL()
    {
    return iContext.QueueOperationL( NPresenceOps::EPrSubscribeGroupList,
                                      KNullDesC8 );
    }

// ---------------------------------------------------------------------------
// CPresentityGroupsImp::SubscribeOwnPresenceL()
// ---------------------------------------------------------------------------
//
TXIMPRequestId CPresentityGroupsImp::UnsubscribePresentityGroupListL()
    {
    return iContext.QueueOperationL( NPresenceOps::EPrUnsubscribeGroupList,
                                      KNullDesC8 );    
    }

// ---------------------------------------------------------------------------
// CPresentityGroupsImp::SubscribeOwnPresenceL()
// ---------------------------------------------------------------------------
//
TXIMPRequestId CPresentityGroupsImp::CreatePresentityGroupL( const MXIMPIdentity& aGroupId,
                                                                 const TDesC16& aGroupDisplayName )
    {
    return QueueOperationL( NPresenceOps::EPrCreatePresentityGroup, aGroupId, aGroupDisplayName );
    }
                                               
// ---------------------------------------------------------------------------
// CPresentityGroupsImp::DeletePresentityGroupL()
// ---------------------------------------------------------------------------
//
TXIMPRequestId CPresentityGroupsImp::DeletePresentityGroupL( const MXIMPIdentity& aGroupId )
    {
    return QueueOperationL( NPresenceOps::EPrDeletePresentityGroup, aGroupId, KNullDesC );
    }

// ---------------------------------------------------------------------------
// CPresentityGroupsImp::UpdatePresentityGroupDisplayNameL()
// ---------------------------------------------------------------------------
//
TXIMPRequestId CPresentityGroupsImp::UpdatePresentityGroupDisplayNameL( const MXIMPIdentity& aGroupId,
                                                                            const TDesC16& aGroupDisplayName )
    {
    return QueueOperationL( NPresenceOps::EPrUpdatePresentityGroupDisplayName, aGroupId, aGroupDisplayName );
    }


// ---------------------------------------------------------------------------
// From MPresentityGroups class.
// CPresentityGroupsImp::SubscribePresentityGroupContentL
// ---------------------------------------------------------------------------
//
TXIMPRequestId CPresentityGroupsImp::SubscribePresentityGroupContentL(
                                                const MXIMPIdentity& aGroupId )
    {
    HBufC8* idPack = PackIdentityLC( aGroupId );

    TXIMPRequestId reqId =
        iContext.QueueOperationL( NPresenceOps::EPrSubscribePresentityGroupContent,
                                   *idPack );
    CleanupStack::PopAndDestroy( idPack );
    return reqId;    
    }
// ---------------------------------------------------------------------------
// From MPresentityGroups class.
// CPresentityGroupsImp::UnsubscribePresentityGroupContentL
// ---------------------------------------------------------------------------
//
TXIMPRequestId CPresentityGroupsImp::UnsubscribePresentityGroupContentL(
                                                const MXIMPIdentity& aGroupId )
    {
    HBufC8* idPack = PackIdentityLC( aGroupId );

    TXIMPRequestId reqId =
        iContext.QueueOperationL( NPresenceOps::EPrUnsubscribePresentityGroupContent,
                                   *idPack );
    CleanupStack::PopAndDestroy( idPack );
    return reqId;    
    }
    
// ---------------------------------------------------------------------------
// From MPresentityGroups class.
// CPresentityGroupsImp::AddPresentityGroupMemberL
// ---------------------------------------------------------------------------
//
TXIMPRequestId CPresentityGroupsImp::AddPresentityGroupMemberL(
                                            const MXIMPIdentity& aGroupId,
                                            const MXIMPIdentity& aMemberId,
                                            const TDesC16& aMemberDisplayName )
    {
    return QueueOperationL( NPresenceOps::EPrAddPresentityGroupMember,
                            aGroupId,
                            aMemberId,
                            aMemberDisplayName );
    }
    
// ---------------------------------------------------------------------------
// From MPresentityGroups class.
// CPresentityGroupsImp::RemovePresentityGroupMemberL
// ---------------------------------------------------------------------------
//
TXIMPRequestId CPresentityGroupsImp::RemovePresentityGroupMemberL(
                                            const MXIMPIdentity& aGroupId,
                                            const MXIMPIdentity& aMemberId )
    {
    return QueueOperationL( NPresenceOps::EPrRemovePresentityGroupMember,
                            aGroupId,
                            aMemberId );
    }
    
// ---------------------------------------------------------------------------
// From MPresentityGroups class.
// CPresentityGroupsImp::UpdatePresentityGroupMemberDisplayNameL
// ---------------------------------------------------------------------------
//
TXIMPRequestId CPresentityGroupsImp::UpdatePresentityGroupMemberDisplayNameL(
                                                    const MXIMPIdentity& aGroupId,
                                                    const MXIMPIdentity& aMemberId,
                                                    const TDesC16& aMemberDisplayName )
    {
    return QueueOperationL( NPresenceOps::EPrUpdatePresentityGroupMemberDisplayName,
                            aGroupId,
                            aMemberId,
                            aMemberDisplayName );
    }

// ---------------------------------------------------------------------------
// CPresentityGroupsImp::QueueOperationL()
// ---------------------------------------------------------------------------
//
TXIMPRequestId CPresentityGroupsImp::QueueOperationL( NPresenceOps::TPresenceOpTypes aOperation,
                                                          const MXIMPIdentity& aGroupId,
                                                          const TDesC16& aGroupDisplayName )
    {
    CPresentityGroupInfoImp* groupInfo = 
                    CPresentityGroupInfoImp::NewLC( aGroupId, aGroupDisplayName );    
    HBufC8* infoPack = PackGroupInfoLC( *groupInfo );

    TXIMPRequestId reqId =
        iContext.QueueOperationL( aOperation, *infoPack );
    CleanupStack::PopAndDestroy( 2, groupInfo );

    return reqId;
    }

// ---------------------------------------------------------------------------
// CPresentityGroupsImp::QueueOperationL()
// ---------------------------------------------------------------------------
//
TXIMPRequestId CPresentityGroupsImp::QueueOperationL( NPresenceOps::TPresenceOpTypes aOperation,
                                                          const MXIMPIdentity& aGroupId,
                                                          const MXIMPIdentity& aMemberId,
                                                          const TDesC16& aMemberDisplayName )
    {
    CBufFlat* buffer = CBufFlat::NewL( 10 ); // set granularity to 10
    CleanupStack::PushL( buffer );
    
    RBufWriteStream ws;
    CleanupClosePushL( ws );
    ws.Open( *buffer ); // CSI: 65 # Does not return a value

    // group id
    const CXIMPIdentityImp* identityImp =
    TXIMPGetImpClassOrPanic< const CXIMPIdentityImp >::From( aGroupId );
    identityImp->ExternalizeL( ws );
    
    
    // member id
    identityImp =
        TXIMPGetImpClassOrPanic< const CXIMPIdentityImp >::From( aMemberId );
    identityImp->ExternalizeL( ws );
    
    
    // member display name
    ws.WriteInt32L( aMemberDisplayName.Length() );
    ws.WriteL( aMemberDisplayName );
    
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
// CPresentityGroupsImp::QueueOperationL()
// ---------------------------------------------------------------------------
//
TXIMPRequestId CPresentityGroupsImp::QueueOperationL( NPresenceOps::TPresenceOpTypes aOperation,
                                                          const MXIMPIdentity& aGroupId,
                                                          const MXIMPIdentity& aMemberId )
    {
    CBufFlat* buffer = CBufFlat::NewL( 10 ); // set granularity to 10
    CleanupStack::PushL( buffer );
    
    RBufWriteStream ws;
    CleanupClosePushL( ws );
    ws.Open( *buffer ); // CSI: 65 # Does not return a value

    // group id
    const CXIMPIdentityImp* identityImp =
        TXIMPGetImpClassOrPanic< const CXIMPIdentityImp >::From( aGroupId );
    identityImp->ExternalizeL( ws );


    // member id
    identityImp =
        TXIMPGetImpClassOrPanic< const CXIMPIdentityImp >::From( aMemberId );    
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
// CPresentityGroupsImp::PackGroupInfoLC()
// ---------------------------------------------------------------------------
//
HBufC8* CPresentityGroupsImp::PackGroupInfoLC( const CPresentityGroupInfoImp& infoImp )
    {
    HBufC8* pack = TXIMPObjectPacker< const CPresentityGroupInfoImp >::PackL( infoImp );
    CleanupStack::PushL( pack );
    return pack;
    }

// ---------------------------------------------------------------------------
// CPresentityGroupsImp::PackIdentityLC()
// ---------------------------------------------------------------------------
//
HBufC8* CPresentityGroupsImp::PackIdentityLC( const MXIMPIdentity& aIdentity )
    {
    const CXIMPIdentityImp* identityImp =
        TXIMPGetImpClassOrPanic< const CXIMPIdentityImp >::From( aIdentity );    

    HBufC8* pack = TXIMPObjectPacker< const CXIMPIdentityImp >::PackL( *identityImp );
    CleanupStack::PushL( pack );

    return pack;
    }

// End of file


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
* Description:  Test support file tool
 *
*/


#include <ximpprotocolconnectionhost.h>
#include <protocolpresenceauthorizationdatahost.h>
#include <protocolpresentitygroupsdatahost.h>
#include <protocolpresencepublishingdatahost.h>
#include <protocolpresencewatchingdatahost.h>
#include <ximpdatasubscriptionstate.h>
#include <protocolpresencedatahost.h>
#include "prfwtestfilelogichelper.h"
#include "prfwtestfilesrvmsg.h"
#include "presentitygroupinfoimp.h"
#include "presentitygroupmemberinfoimp.h"
#include "presencewatcherinfoimp.h"
#include "presencegrantrequestinfoimp.h"
#include "presenceblockinfoimp.h"
#include "ximpidentityimp.h"
#include "ximpobjecthelpers.h"
#include "prfwtestmessenger.h"
#include "ximprestrictedobjectcollectionimp.h"
#include "ximpobjectcollectionimp.h"
#include "presenceinfoimp.h"

// ======== LOCAL FUNCTIONS ========

// ======== MEMBER FUNCTIONS ========

// ---------------------------------------------------------------------------
// ?description_if_needed
// ---------------------------------------------------------------------------
//
CXIMPTestFileLogicHelper::CXIMPTestFileLogicHelper( MXIMPProtocolConnectionHost& aHost, CXIMPTestMessenger& aMessenger )
: iHost( aHost ), iMessenger( aMessenger )
    {
    }

// ---------------------------------------------------------------------------
// ?description_if_needed
// ---------------------------------------------------------------------------
//
void CXIMPTestFileLogicHelper::ConstructL()
    {
    }

// ---------------------------------------------------------------------------
// ?description_if_needed
// ---------------------------------------------------------------------------
//
EXPORT_C CXIMPTestFileLogicHelper* CXIMPTestFileLogicHelper::NewL( MXIMPProtocolConnectionHost& aHost, CXIMPTestMessenger& aMessenger  )
    {
    CXIMPTestFileLogicHelper* self = new( ELeave ) CXIMPTestFileLogicHelper( aHost, aMessenger );
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );
    return self;
    }

// ---------------------------------------------------------------------------
// ?description_if_needed
// ---------------------------------------------------------------------------
//
CXIMPTestFileLogicHelper::~CXIMPTestFileLogicHelper()
    {
    }

// ---------------------------------------------------------------------------
// ?description_if_needed
// ---------------------------------------------------------------------------
//
void CXIMPTestFileLogicHelper::HandleL( TInt aCommandType, TDesC8& aCommand )
    {
    switch( aCommandType )
        {
        case CXIMPTestFileSrvMsg::ETestSrvMsgPresenceGrantRequestList:
            {
            RXIMPObjOwningPtrArray< CPresenceGrantRequestInfoImp > array;
            CleanupClosePushL( array );
            TXIMPObjectPacker<CPresenceGrantRequestInfoImp>::UnPackArrayL( array, aCommand );
            CXIMPObjectCollectionImp* objCollection = CXIMPObjectCollectionImp::NewLC();
            for( TInt a = 0; a < array.Count(); ++a )
                {
                objCollection->AddObjectL( array[ a ] );
                array[ a ] = NULL;
                }

            iHost.ProtocolPresenceDataHost().AuthorizationDataHost().HandlePresenceGrantRequestListL( objCollection );
            CleanupStack::Pop( objCollection );
            CleanupStack::PopAndDestroy(); //array
            break;
            }
        case CXIMPTestFileSrvMsg::ETestSrvMsgPresenceGrantRequestReceived:
            {
            CPresenceGrantRequestInfoImp* infoFromServer =
                                CPresenceGrantRequestInfoImp::NewLC();
            TXIMPObjectPacker<CPresenceGrantRequestInfoImp>::UnPackL(
                                    *infoFromServer, aCommand );

            iHost.ProtocolPresenceDataHost().AuthorizationDataHost().HandlePresenceGrantRequestReceivedL( infoFromServer );
            CleanupStack::Pop( infoFromServer );
            break;
            }
        case CXIMPTestFileSrvMsg::ETestSrvMsgPresenceGrantRequestObsoleted:
            {
            CXIMPIdentityImp* idFromServer = CXIMPIdentityImp::NewLC();
            TXIMPObjectPacker<CXIMPIdentityImp>::UnPackL( *idFromServer,
                                                            aCommand );

            iHost.ProtocolPresenceDataHost().AuthorizationDataHost().HandlePresenceGrantRequestObsoletedL( idFromServer );
            CleanupStack::Pop( idFromServer );
            break;
            }
        case CXIMPTestFileSrvMsg::ETestSrvMsgPresentityGroupList:
            {
            RXIMPObjOwningPtrArray< CPresentityGroupInfoImp > array;
            CleanupClosePushL( array );
            TXIMPObjectPacker<CPresentityGroupInfoImp>::UnPackArrayL( array, aCommand );
            CXIMPObjectCollectionImp* objCollection = CXIMPObjectCollectionImp::NewLC();
            for( TInt a = 0; a < array.Count(); ++a )
                {
                objCollection->AddObjectL( array[ a ] );
                array[ a ] = NULL;
                }
            iHost.ProtocolPresenceDataHost().GroupsDataHost().HandlePresentityGroupListL( objCollection );
            CleanupStack::Pop( objCollection );
            CleanupStack::PopAndDestroy(); //array
            break;            
            }
        case CXIMPTestFileSrvMsg::ETestSrvMsgPresentityGroupCreate:
        case CXIMPTestFileSrvMsg::ETestSrvMsgPresentityGroupUpdate:
            {
            CPresentityGroupInfoImp* infoFromServer =
                                CPresentityGroupInfoImp::NewLC();
            TXIMPObjectPacker<CPresentityGroupInfoImp>::UnPackL(
                                    *infoFromServer, aCommand );

            if( aCommandType == CXIMPTestFileSrvMsg::ETestSrvMsgPresentityGroupCreate )
                {
                iHost.ProtocolPresenceDataHost().GroupsDataHost().HandlePresentityGroupCreatedL( infoFromServer );
                }
            else
                {
                iHost.ProtocolPresenceDataHost().GroupsDataHost().HandlePresentityGroupDisplayNameUpdatedL( infoFromServer );
                }
            CleanupStack::Pop( infoFromServer );
            break;
            }
        case CXIMPTestFileSrvMsg::ETestSrvMsgPresentityGroupDelete:
            {
            CXIMPIdentityImp* idFromServer = CXIMPIdentityImp::NewLC();
            TXIMPObjectPacker<CXIMPIdentityImp>::UnPackL( *idFromServer,
                                                            aCommand );
            iHost.ProtocolPresenceDataHost().GroupsDataHost().HandlePresentityGroupDeletedL( idFromServer );
            CleanupStack::Pop( idFromServer );
            break;
            }
        case CXIMPTestFileSrvMsg::ETestSrvMsgPresentityGroupMemberList:
            {
            RPointerArray< HBufC8 > bufferArray;
            CleanupClosePushL( bufferArray );
            TXIMPHBuf8Packer::UnPackArrayL( bufferArray, aCommand );
            HBufC8* groupIdBuf = bufferArray[ 0 ];
            HBufC8* array = bufferArray[ 1 ];
            CleanupStack::PopAndDestroy(); //  BufferArray
            CleanupStack::PushL( groupIdBuf );
            CleanupStack::PushL( array );

            CXIMPIdentityImp* groupId = CXIMPIdentityImp::NewLC();
            TXIMPObjectPacker< CXIMPIdentityImp >::UnPackL( *groupId, *groupIdBuf );

            RXIMPObjOwningPtrArray< CPresentityGroupMemberInfoImp > memberArray;
            CleanupClosePushL( memberArray );
            TXIMPObjectPacker<CPresentityGroupMemberInfoImp>::UnPackArrayL( memberArray, *array );
            CXIMPObjectCollectionImp* objCollection = CXIMPObjectCollectionImp::NewLC();
            for( TInt a = 0; a < memberArray.Count(); ++a )
                {
                objCollection->AddObjectL( memberArray[ a ] );
                memberArray[ a ] = NULL;
                }
            iHost.ProtocolPresenceDataHost().GroupsDataHost().HandlePresentityGroupContentL( groupId, objCollection );
            CleanupStack::Pop( objCollection );
            CleanupStack::PopAndDestroy(); //memberArray
            CleanupStack::Pop( groupId );
            CleanupStack::PopAndDestroy( 2, groupIdBuf );
            break;
            }
        case CXIMPTestFileSrvMsg::ETestCheckIfGroupMemberExists:
            {
            CXIMPIdentityImp* groupId = CXIMPIdentityImp::NewLC();
            TXIMPObjectPacker< CXIMPIdentityImp >::UnPackL( *groupId, aCommand );
            // Write to pubsub
            const MXIMPDataSubscriptionState& state = 
                iHost.ProtocolPresenceDataHost().GroupsDataHost().PresentityGroupContentDataSubscriptionState( *groupId );
            if( state.DataState() == MXIMPDataSubscriptionState::EDataAvailable )
                {
                iMessenger.SetValueFor( EXIMPPlgTestExistsGroupContentInCache, 1 );
                }
            else
                {
                iMessenger.SetValueFor( EXIMPPlgTestExistsGroupContentInCache, KErrNotFound );
                }
            CleanupStack::PopAndDestroy( groupId );
            break;
            }
        case CXIMPTestFileSrvMsg::ETestSrvMsgPresenceWatcherList:
            {
            RXIMPObjOwningPtrArray< CPresenceWatcherInfoImp > array;
            CleanupClosePushL( array );
            TXIMPObjectPacker< CPresenceWatcherInfoImp >::UnPackArrayL( array, aCommand );
            CXIMPObjectCollectionImp* objCollection = CXIMPObjectCollectionImp::NewLC();
            for( TInt a = 0; a < array.Count(); ++a )
                {
                objCollection->AddObjectL( array[ a ] );
                array[ a ] = NULL;
                }

            iHost.ProtocolPresenceDataHost().PublishingDataHost().HandlePresenceWatcherListL( objCollection );
            CleanupStack::Pop( objCollection );
            CleanupStack::PopAndDestroy(); //, array
            break;
            }
        case CXIMPTestFileSrvMsg::ETestSrvMsgPresentityPresence:
            {
            RPointerArray< HBufC8 > bufferArray;
            CleanupClosePushL( bufferArray );
            TXIMPHBuf8Packer::UnPackArrayL( bufferArray, aCommand );
            HBufC8* presenceIdBuf = bufferArray[ 0 ];
            HBufC8* presenceInfoBuf = bufferArray[ 1 ];
            CleanupStack::PopAndDestroy(); //  BufferArray
            CleanupStack::PushL( presenceIdBuf );
            CleanupStack::PushL( presenceInfoBuf );

            CXIMPIdentityImp* presenceId = CXIMPIdentityImp::NewLC();
            TXIMPObjectPacker< CXIMPIdentityImp >::UnPackL( *presenceId, *presenceIdBuf );

            CPresenceInfoImp* presenceInfo = CPresenceInfoImp::NewLC();
            TXIMPObjectPacker< CPresenceInfoImp >::UnPackL( *presenceInfo, *presenceInfoBuf );

            iHost.ProtocolPresenceDataHost().WatchingDataHost().HandleSubscribedPresentityPresenceL( presenceId, presenceInfo );
            
            CleanupStack::Pop( 2, presenceId );
            CleanupStack::PopAndDestroy( 2, presenceIdBuf );
            break;
            }
        case CXIMPTestFileSrvMsg::ETestSrvMsgPresenceBlockList:
            {
            RXIMPObjOwningPtrArray< CPresenceBlockInfoImp > array;
            CleanupClosePushL( array );
            TXIMPObjectPacker< CPresenceBlockInfoImp >::UnPackArrayL( array, aCommand );
            CXIMPObjectCollectionImp* objCollection = CXIMPObjectCollectionImp::NewLC();
            for( TInt a = 0; a < array.Count(); ++a )
                {
                objCollection->AddObjectL( array[ a ] );
                array[ a ] = NULL;
                }

            iHost.ProtocolPresenceDataHost().AuthorizationDataHost().HandlePresenceBlockListL( objCollection );
            CleanupStack::Pop( objCollection );
            CleanupStack::PopAndDestroy(); // array
            break;
            }
        case CXIMPTestFileSrvMsg::ETestSrvMsgPresenceBlocked:
            {
            CPresenceBlockInfoImp* infoFromServer =
                                CPresenceBlockInfoImp::NewLC();
            TXIMPObjectPacker<CPresenceBlockInfoImp>::UnPackL(
                                    *infoFromServer, aCommand );
            iHost.ProtocolPresenceDataHost().AuthorizationDataHost().HandlePresenceBlockedL( infoFromServer );
            CleanupStack::Pop( infoFromServer );
            break;
            }
        case CXIMPTestFileSrvMsg::ETestSrvMsgPresenceBlockCanceled:
            {
            CXIMPIdentityImp* idFromServer = CXIMPIdentityImp::NewLC();
            TXIMPObjectPacker<CXIMPIdentityImp>::UnPackL( *idFromServer,
                                                            aCommand );

            iHost.ProtocolPresenceDataHost().AuthorizationDataHost().HandlePresenceBlockCanceledL( idFromServer );

            CleanupStack::Pop( idFromServer );
            break;
            }

        case CXIMPTestFileSrvMsg::ETestSrvMsgReqParamIdentity:
            {
            CXIMPIdentityImp* groupId = CXIMPIdentityImp::NewLC();
            TXIMPObjectPacker< CXIMPIdentityImp >::UnPackL( *groupId, aCommand );
            CXIMPRestrictedObjectCollectionImp* paramCollection = CXIMPRestrictedObjectCollectionImp::NewLC();
            paramCollection->AddOrReplaceTypeL( groupId );
            iMessenger.SetReqCompleteParams( paramCollection );
            CleanupStack::Pop( 2, groupId );
            break;
            }
        default:
            {
            User::Panic( _L( "Not valid commandtype" ), 59 );
            break;
            }
        }
    }

// End of file

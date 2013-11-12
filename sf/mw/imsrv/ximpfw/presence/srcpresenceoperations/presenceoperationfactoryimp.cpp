/*
* Copyright (c) 2007 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Presence service operation factory
 *
*/


#include "presenceoperationfactoryimp.h"
#include "presenceoperationdefs.h"
#include "operationpublishownpresence.h"
#include "operationsubscribeownpresence.h"
#include "operationunsubscribeownpresence.h"
#include "operationhandleownpresence.h"
#include "operationhandlepresentitygroups.h"
#include "ximpoperationbase.h"
#include "operationsubscribegrouplist.h"
#include "operationunsubscribegrouplist.h"
#include "operationcreatepresentitygroup.h"
#include "operationsubscribepresentitypresence.h"
#include "operationunsubscribepresentitypresence.h"
#include "operationdeletepresentitygroup.h"
#include "operationupdatepresentitygroupdisplayname.h"
#include "operationhandlepresentitypresence.h"
#include "operationsubscribegroupcontent.h"
#include "operationunsubscribegroupcontent.h"
#include "operationaddpresentitygroupmember.h"
#include "operationremovepresentitygroupmember.h"
#include "operationupdatepresentitygroupmemberdisplayname.h"
#include "operationhandlepresentitygroupcontent.h"
#include "operationhandlepresentitygroupmemberadded.h"
#include "operationhandlepresentitygroupmemberremoved.h"
#include "operationhandlepresentitygroupmemberdisplaynameupdated.h"
#include "operationlaunchcleaner.h"
#include "operationsubscribewatcherlist.h"
#include "operationunsubscribewatcherlist.h"
#include "operationhandlepresencewatcherlist.h"
#include "operationsubscribepresentitygroupmemberspresence.h"
#include "operationunsubscribepresentitygroupmemberspresence.h"
#include "operationsubscribegrantrequestlist.h"
#include "operationunsubscribegrantrequestlist.h"
#include "operationhandlepresencegrantrequests.h"
#include "operationgrantpresencetopresentity.h"
#include "operationwithdrawpresencefrompresentity.h"
#include "operationgrantpresencetopresentitygroupmembers.h"
#include "operationwithdrawpresencefrompresentitygroupmembers.h"
#include "operationsubscribeblocklist.h"
#include "operationunsubscribeblocklist.h"
#include "operationblockpresenceforpresentity.h"
#include "operationcancelpresenceblockfrompresentity.h"
#include "operationhandlepresenceblocking.h"
#include "operationgrantpresencetoeveryone.h"
#include "operationwithdrawpresencefromeveryone.h"
#include "operationsetsubscriptiondata.h"
#include "ximppanics.h"
#include "ximptrace.h"

using namespace NPresenceOps;

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CXIMPOperationFactory::NewL()
// Singleton access method.
// -----------------------------------------------------------------------------
//
EXPORT_C CPresenceOperationFactory* CPresenceOperationFactory::NewL()
    {
    CPresenceOperationFactory* self = new ( ELeave ) CPresenceOperationFactory();
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );
    return self;
    }

// -----------------------------------------------------------------------------
// CXIMPOperationFactory::CXIMPOperationFactory()
// -----------------------------------------------------------------------------
//
CPresenceOperationFactory::CPresenceOperationFactory()
    {
    }

// -----------------------------------------------------------------------------
// CXIMPOperationFactory::~CXIMPOperationFactory()
// -----------------------------------------------------------------------------
//
CPresenceOperationFactory::~CPresenceOperationFactory()
    {
    #if _BullseyeCoverage
    cov_write();
    #endif
    }


// -----------------------------------------------------------------------------
// CXIMPOperationFactory::ConstructL()
// -----------------------------------------------------------------------------
//
void CPresenceOperationFactory::ConstructL()
    {
    }


// -----------------------------------------------------------------------------
// CXIMPOperationFactory::CreateOperationL()
// -----------------------------------------------------------------------------
//
CXIMPOperationBase* CPresenceOperationFactory::CreateOperationL( TInt aOperation )
    {
    CXIMPOperationBase* op = NULL;
    NPresenceOps::TPresenceOpTypes operation = ( NPresenceOps::TPresenceOpTypes ) aOperation;
    switch( aOperation )
        {

        case EPrPublishOwnPresence:
            {
            op = new ( ELeave ) COperationPublishOwnPresence();
            TRACE( _L("Operation PublishOwnPresence created") );
            break;
            }

        case EPrSubscribeOwnPresence:
            {
            op = new ( ELeave ) COperationSubscribeOwnPresence();
            TRACE( _L("Operation SubscribeOwnPresence created") );
            break;
            }

        case EPrUnsubscribeOwnPresence:
            {
            op = new ( ELeave ) COperationUnsubscribeOwnPresence();
            TRACE( _L("Operation UnsubscribeOwnPresence created") );
            break;
            }

        case EPrHandleOwnPresence:
            {
            op = new ( ELeave ) COperationHandleOwnPresence();
            TRACE( _L("Operation HandleOwnPresence created") );
            break;
            }

        case EPrSubscribeGroupList:
            {
            op = new ( ELeave ) COperationSubscribeGroupList();
            TRACE( _L("Operation UnsubscribeOwnPresence created") );
            break;
            }

        case EPrUnsubscribeGroupList:
            {
            op = new ( ELeave ) COperationUnsubscribeGroupList();
            TRACE( _L("Operation SubscribeGroupList created") );
            break;
            }

        case EPrCreatePresentityGroup:
            {
            op = new ( ELeave ) COperationCreatePresentityGroup();
            TRACE( _L("Operation CreatePresentityGroup created") );
            break;
            }

        case EPrDeletePresentityGroup:
            {
            op = new ( ELeave ) COperationDeletePresentityGroup();
            TRACE( _L("Operation DeletePresentityGroup created") );
            break;
            }

        case EPrUpdatePresentityGroupDisplayName:
            {
            op = new ( ELeave ) COperationUpdatePresentityGroupDisplayName();
            TRACE( _L("Operation UpdatePresentityGroupDisplayName created") );
            break;
            }

        case EPrSubscribePresentityGroupContent:
            {
            op = new ( ELeave ) COperationSubscribeGroupContent();
            TRACE( _L("Operation SubscribeGroupContent created") );
            break;
            }

        case EPrUnsubscribePresentityGroupContent:
            {
            op = new ( ELeave ) COperationUnsubscribeGroupContent();
            TRACE( _L("Operation UnsubscribeGroupContent created") );
            break;
            }

        case EPrAddPresentityGroupMember:
            {
            op = new ( ELeave ) COperationAddPresentityGroupMember();
            TRACE( _L("Operation AddPresentityGroupMember created") );
            break;
            }

        case EPrRemovePresentityGroupMember:
            {
            op = new ( ELeave ) COperationRemovePresentityGroupMember();
            TRACE( _L("Operation RemovePresentityGroupMember created") );
            break;
            }

        case EPrUpdatePresentityGroupMemberDisplayName:
            {
            op = new ( ELeave ) COperationUpdatePresentityGroupMemberDisplayName();
            TRACE( _L("Operation UpdatePresentityGroupMemberDisplayName created") );
            break;
            }

        case EPrSubscribePresentityPresence:
            {
            op = new ( ELeave ) COperationSubscribePresentityPresence();
            TRACE( _L("Operation SubscribePresentityPresence created") );
            break;
            }

        case EPrUnsubscribePresentityPresence:
            {
            op = new ( ELeave ) COperationUnsubscribePresentityPresence();
            TRACE( _L("Operation UnsubscribePresentityPresence created") );
            break;
            }

        case EPrSubscribePresenceWatcherList:
            {
            op = new ( ELeave ) COperationSubscribeWatcherList();
            TRACE( _L("Operation SubscribeWatcherList created") );
            break;
            }
        case EPrUnsubscribePresenceWatcherList:
            {
            op = new ( ELeave ) COperationUnsubscribeWatcherList();
            TRACE( _L("Operation UnsubscribeWatcherList created") );
            break;
            }

        case EPrHandlePresentityPresence: // Flowthrough
        case EPrHandlePresentityPresenceTerminated:
            {
            op = new ( ELeave ) COperationHandlePresentityPresence( aOperation );
            TRACE( _L("Operation HandlePresentityPresence created") );
            break;
            }

        case EPrHandlePresenceWatcherList:
            {
            op = new ( ELeave ) COperationHandlePresenceWatcherList();
            TRACE( _L("Operation HandlePresenceWatcherList created") );
            break;
            }

        case EPrHandlePresentityGroupList:
        case EPrHandlePresentityGroupCreated:
        case EPrHandlePresentityGroupDeleted:
        case EPrHandlePresentityGroupDisplayNameUpdated:        
            {
            op = new ( ELeave ) COperationHandlePresentityGroups( operation );
            TRACE( _L("Operation HandlePresentityGroups created") );
            break;
            }

        case EPrHandlePresentityGroupContent:
            {
            op = new ( ELeave ) COperationHandlePresentityGroupContent();
            TRACE( _L("Operation HandlePresentityGroupContent created") );
            break;
            }

        case EPrHandlePresentityGroupMemberAdded:
            {
            op = new ( ELeave ) COperationHandlePresentityGroupMemberAdded();
            TRACE( _L("Operation HandlePresentityGroupMemberAdded created") );
            break;
            }

        case EPrHandlePresentityGroupMemberRemoved:
            {
            op = new ( ELeave ) COperationHandlePresentityGroupMemberRemoved();
            TRACE( _L("Operation HandlePresentityGroupMemberRemoved created") );
            break;
            }

        case EPrHandlePresentityGroupMemberDisplayNameUpdated:
            {
            op = new ( ELeave ) COperationHandlePresentityGroupMemberDisplayNameUpdated();
            TRACE( _L("Operation HandlePresentityGroupMemberDisplayNameUpdated created") );
            break;
            }

        case EPrSubscribePresentityGroupMembersPresence:
            {
            op = new ( ELeave ) COperationSubscribePresentityGroupMembersPresence();
            TRACE( _L("Operation SubscribePresentityGroupMembersPresence created") );
            break;
            }

        case EPrUnsubscribePresentityGroupMembersPresence:
            {
            op = new ( ELeave ) COperationUnsubscribePresentityGroupMembersPresence();
            TRACE( _L("Operation UnsubscribePresentityGroupMembersPresence created") );
            break;
            }

        case EPrSubscribePresenceGrantRequestList:
            {
            op = new ( ELeave ) COperationSubscribeGrantRequestList();
            TRACE( _L("Operation SubscribeGrantRequestList created") );
            break;
            }

        case EPrUnsubscribePresenceGrantRequestList:
            {
            op = new ( ELeave ) COperationUnsubscribeGrantRequestList();
            TRACE( _L("Operation UnsubscribeGrantRequestList created") );
            break;
            }

        case EPrHandlePresenceGrantRequestList:
        case EPrHandlePresenceGrantRequestReceived:
        case EPrHandlePresenceGrantRequestObsoleted:
            {
            op = new ( ELeave ) COperationHandlePresenceGrantRequests( operation );
            TRACE( _L("Operation HandlePresenceGrantRequests created") );
            break;
            }
        case EPrGrantPresenceToPresentity:
            {
            op = new ( ELeave ) COperationGrantPresenceToPresentity();
            TRACE( _L("Operation GrantPresenceToPresentity created") );
            break;
            }
        case EPrWithdrawPresenceFromPresentity:
            {
            op = new ( ELeave ) COperationWithdrawPresenceFromPresentity();
            TRACE( _L("Operation WithdrawPresenceFromPresentity created") );
            break;
            }
        case EPrGrantPresenceToPresentityGroupMembers:
            {
            op = new ( ELeave ) COperationGrantPresenceToPresentityGroupMembers();
            TRACE( _L("Operation GrantPresenceToPresentityGroupMembers created") );
            break;
            }
        case EPrWithdrawPresenceFromPresentityGroupMembers:
            {
            op = new ( ELeave ) COperationWithdrawPresenceFromPresentityGroupMembers();
            TRACE( _L("Operation WithdrawPresenceFromPresentityGroupMembers created") );
            break;
            }

        case EPrGrantPresenceToEveryone:
            {
            op = new ( ELeave ) COperationGrantPresenceToEveryone();
            TRACE( _L("Operation GrantPresenceToEveryone created") );
            break;
            }
        case EPrWithdrawPresenceFromEveryone:
            {
            op = new ( ELeave ) COperationWithdrawPresenceFromEveryone();
            TRACE( _L("Operation WithdrawPresenceFromEveryone created") );
            break;
            }

        case EXIMPCleaner:
            {
            op = new ( ELeave ) COperationLaunchCleaner();
            TRACE( _L("Operation LaunchCleaner created") );
            break;
            }

        case EPrSubscribeBlockList:
            {
            op = new ( ELeave ) COperationSubscribeBlockList();
            TRACE( _L("Operation SubscribeBlockList created") );
            break;
            }
        case EPrUnsubscribeBlockList:
            {
            op = new ( ELeave ) COperationUnsubscribeBlockList();
            TRACE( _L("Operation UnsubscribeBlockList created") );
            break;
            }

        case EPrBlockPresenceForPresentity:
            {
            op = new ( ELeave ) COperationBlockPresenceForPresentity();
            TRACE( _L("Operation BlockPresenceForPresentity created") );
            break;
            }

        case EPrCancelPresenceBlockFromPresentity:
            {
            op = new ( ELeave ) COperationCancelPresenceBlockFromPresentity();
            TRACE( _L("Operation CancelPresenceBlockFromPresentity created") );
            break;
            }

        case EPrHandlePresenceBlockList:
        case EPrHandlePresenceBlocked:
        case EPrHandlePresenceBlockCanceled:
            {
            op = new ( ELeave ) COperationHandlePresenceBlocking( operation );
            TRACE( _L("Operation HandlePresenceBlocking created") );
            break;
            }

        case EPrSetOwnPresenceDataSubscriptionState:
        case EPrSetPresenceBlockDataSubscriptionState:
        case EPrSetPresenceGrantRequestDataSubscriptionState:
        case EPrSetPresenceWatcherListDataSubscriptionState:
        case EPrSetPresentityGroupListDataSubscriptionState:
        case EPrSetPresentityGroupContentDataSubscriptionState:
        case EPrSetPresentityPresenceDataSubscriptionState:
            {
            op = new ( ELeave ) COperationSetSubscriptionData( operation );
            TRACE( _L("Operation SetSubscriptionData created") );
            break;
            }
        
        // commented by pankaj.
        //XIMP_DEFAULT_CASE_UNSUPPORTED( NXIMPPrivPanic::EInvalidOperationType );
        }

    return op;
    }
// End of file



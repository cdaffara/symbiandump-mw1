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
* Description:  Implementation for managing cached presentity group information.
 *
*/


#include <ximpobjectcollection.h>
#include <presentitygroupinfo.h>
#include <ximpidentity.h>
#include <ximpdatasubscriptionstate.h>
#include <ximpstatus.h>

#include "protocolpresencedatahostimp.h"
#include "protocolpresentitygroupsdatahostimp.h"

#include "presentitygroupmemberinfoimp.h"
#include "ximpoperationbase.h"
#include "ximprestrictedobjectcollectionimp.h"
#include "ximpglobals.h"
#include "ximpoperationfactory.h"
#include "ximphost.h"
#include "presencedatacacheimp.h"
#include "presenceoperationdefs.h"
#include "ximptrace.h"

// ---------------------------------------------------------------------------
// CProtocolPresentityGroupsDataHostImp::CProtocolPresentityGroupsDataHostImp()
// ---------------------------------------------------------------------------
//
CProtocolPresentityGroupsDataHostImp::CProtocolPresentityGroupsDataHostImp( MXIMPHost& aHost )
:iHost( aHost )
    {
    }


// ---------------------------------------------------------------------------
// CProtocolPresentityGroupsDataHostImp::ConstructL()
// ---------------------------------------------------------------------------
//
void CProtocolPresentityGroupsDataHostImp::ConstructL()
    {
    }


// ---------------------------------------------------------------------------
// CProtocolPresentityGroupsDataHostImp::NewL()
// ---------------------------------------------------------------------------
//
EXPORT_C CProtocolPresentityGroupsDataHostImp* CProtocolPresentityGroupsDataHostImp::NewL( MXIMPHost& aHost )
    {
    CProtocolPresentityGroupsDataHostImp* self = new( ELeave ) CProtocolPresentityGroupsDataHostImp( aHost );
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );
    return self;
    }

// ---------------------------------------------------------------------------
// Implement supported interface access.
// ---------------------------------------------------------------------------
//
XIMPIMP_IF_BASE_GET_INTERFACE_BEGIN( CProtocolPresentityGroupsDataHostImp,
                                     MProtocolPresentityGroupsDataHost )
XIMPIMP_IF_BASE_GET_INTERFACE_END()


XIMPIMP_IF_BASE_GET_CONST_INTERFACE_BEGIN( CProtocolPresentityGroupsDataHostImp,
                                           MProtocolPresentityGroupsDataHost )
XIMPIMP_IF_BASE_GET_INTERFACE_END()

// ---------------------------------------------------------------------------
// CProtocolPresentityGroupsDataHostImp::~CProtocolPresentityGroupsDataHostImp()
// ---------------------------------------------------------------------------
//
CProtocolPresentityGroupsDataHostImp::~CProtocolPresentityGroupsDataHostImp()
    {
    }

// ---------------------------------------------------------------------------
// CProtocolPresentityGroupsDataHostImp::HandlePresentityGroupListL
// ---------------------------------------------------------------------------
//
TXIMPRequestId CProtocolPresentityGroupsDataHostImp::HandlePresentityGroupListL(
        MXIMPObjectCollection* aGroupList )
    {
    TRACE(_L("CProtocolPresentityGroupsDataHostImp::HandlePresentityGroupListL") );
    return iHost.AddNewOperationL( NPresenceOps::EPrHandlePresentityGroupList, aGroupList );
    }

// ---------------------------------------------------------------------------
// CProtocolPresentityGroupsDataHostImp::HandlePresentityGroupCreatedL
// ---------------------------------------------------------------------------
//
TXIMPRequestId CProtocolPresentityGroupsDataHostImp::HandlePresentityGroupCreatedL(
        MPresentityGroupInfo* aGroupInfo )
    {
    TRACE(_L("CProtocolPresentityGroupsDataHostImp::HandlePresentityGroupCreatedL") );
    return iHost.AddNewOperationL( NPresenceOps::EPrHandlePresentityGroupCreated, aGroupInfo );
    }

// ---------------------------------------------------------------------------
// CProtocolPresentityGroupsDataHostImp::HandlePresentityGroupDeletedL
// ---------------------------------------------------------------------------
//
TXIMPRequestId CProtocolPresentityGroupsDataHostImp::HandlePresentityGroupDeletedL(
        MXIMPIdentity* aGroupId )
    {
    TRACE(_L("CProtocolPresentityGroupsDataHostImp::HandlePresentityGroupDeletedL") );
    return iHost.AddNewOperationL( NPresenceOps::EPrHandlePresentityGroupDeleted, aGroupId );
    }

// ---------------------------------------------------------------------------
// CProtocolPresentityGroupsDataHostImp::HandlePresentityGroupDisplayNameUpdatedL
// ---------------------------------------------------------------------------
//
TXIMPRequestId CProtocolPresentityGroupsDataHostImp::HandlePresentityGroupDisplayNameUpdatedL(
        MPresentityGroupInfo* aGroupInfo )
    {
    TRACE(_L("CProtocolPresentityGroupsDataHostImp::HandlePresentityGroupDisplayNameUpdatedL") );
    return iHost.AddNewOperationL( NPresenceOps::EPrHandlePresentityGroupDisplayNameUpdated, aGroupInfo );
    }

// ---------------------------------------------------------------------------
// CProtocolPresentityGroupsDataHostImp::PresentityGroupDataSubscriptionState
// ---------------------------------------------------------------------------
//
const MXIMPDataSubscriptionState& 
    CProtocolPresentityGroupsDataHostImp::PresentityGroupListDataSubscriptionState() const
    {
    TRACE(_L("CProtocolPresentityGroupsDataHostImp::PresentityGroupListDataSubscriptionState") );
    return iHost.PresenceDataAccess().PresenceDataCache().PresentityGroupListDataSubscriptionState();
    }

// ---------------------------------------------------------------------------
// CProtocolPresentityGroupsDataHostImp::SetPresentityGroupDataSubscriptionStateL
// ---------------------------------------------------------------------------
//
TXIMPRequestId CProtocolPresentityGroupsDataHostImp::SetPresentityGroupListDataSubscriptionStateL(
            MXIMPDataSubscriptionState* aSubscriptionState, 
            MXIMPStatus* aStatus )
    {
    TRACE(_L("CProtocolPresentityGroupsDataHostImp::SetPresentityGroupListDataSubscriptionStateL") );
    return iHost.AddNewOperationL( NPresenceOps::EPrSetPresentityGroupListDataSubscriptionState, 
                                   aSubscriptionState, aStatus );
    }

// ---------------------------------------------------------------------------
// From MProtocolPresentityGroupsDataHost class.
// CProtocolPresentityGroupsDataHostImp::HandlePresentityGroupContentL
// ---------------------------------------------------------------------------
//
TXIMPRequestId CProtocolPresentityGroupsDataHostImp::HandlePresentityGroupContentL(
            MXIMPIdentity* aGroupId, MXIMPObjectCollection* aGroupMembers )
    {
    TRACE(_L("CProtocolPresentityGroupsDataHostImp::HandlePresentityGroupContentL") );
    return iHost.AddNewOperationL( NPresenceOps::EPrHandlePresentityGroupContent, aGroupId, aGroupMembers );
    }

// ---------------------------------------------------------------------------
// From MProtocolPresentityGroupsDataHost class.
// CProtocolPresentityGroupsDataHostImp::HandlePresentityGroupMemberAddedL
// ---------------------------------------------------------------------------
//
TXIMPRequestId CProtocolPresentityGroupsDataHostImp::HandlePresentityGroupMemberAddedL(
        MXIMPIdentity* aGroupId, MPresentityGroupMemberInfo* aGroupMember )
    {
    TRACE(_L("CProtocolPresentityGroupsDataHostImp::HandlePresentityGroupMemberAddedL") );
    return iHost.AddNewOperationL( NPresenceOps::EPrHandlePresentityGroupMemberAdded, aGroupId, aGroupMember );
    }

// ---------------------------------------------------------------------------
// From MProtocolPresentityGroupsDataHost class.
// CProtocolPresentityGroupsDataHostImp::HandlePresentityGroupMemberRemovedL
// ---------------------------------------------------------------------------
//
TXIMPRequestId CProtocolPresentityGroupsDataHostImp::HandlePresentityGroupMemberRemovedL(
            MXIMPIdentity* aGroupId, MXIMPIdentity* aGroupMemberId )
    {
    TRACE(_L("CProtocolPresentityGroupsDataHostImp::HandlePresentityGroupMemberRemovedL") );
    CPresentityGroupMemberInfoImp* infoImp = CPresentityGroupMemberInfoImp::NewLC(
                                                                    *aGroupMemberId, KNullDesC );
    TXIMPRequestId reqId = iHost.AddNewOperationL( 
                    NPresenceOps::EPrHandlePresentityGroupMemberRemoved, aGroupId, infoImp );
    CleanupStack::Pop( infoImp );
    delete aGroupMemberId;
    return reqId;
    }

// ---------------------------------------------------------------------------
// From MProtocolPresentityGroupsDataHost class.
// CProtocolPresentityGroupsDataHostImp::HandlePresentityGroupMemberUpdatedL
// ---------------------------------------------------------------------------
//
TXIMPRequestId CProtocolPresentityGroupsDataHostImp::HandlePresentityGroupMemberDisplayNameUpdatedL(
            MXIMPIdentity* aGroupId, MPresentityGroupMemberInfo* aGroupMember )
    {
    TRACE(_L("CProtocolPresentityGroupsDataHostImp::HandlePresentityGroupMemberDisplayNameUpdatedL") );
    return iHost.AddNewOperationL( NPresenceOps::EPrHandlePresentityGroupMemberDisplayNameUpdated, aGroupId, aGroupMember );
    }

// ---------------------------------------------------------------------------
// CProtocolPresentityGroupsDataHostImp::PresentityGroupMemberDataSubscriptionState
// ---------------------------------------------------------------------------
//
const MXIMPDataSubscriptionState& 
    CProtocolPresentityGroupsDataHostImp::PresentityGroupContentDataSubscriptionState( 
            const MXIMPIdentity& aGroupId ) const
    {
    TRACE(_L("CProtocolPresentityGroupsDataHostImp::PresentityGroupContentDataSubscriptionState") );
    return iHost.PresenceDataAccess().PresenceDataCache().PresentityGroupContentDataSubscriptionState( aGroupId );
    }

// ---------------------------------------------------------------------------
// CProtocolPresentityGroupsDataHostImp::SetPresentityGroupContentDataSubscriptionStateL
// ---------------------------------------------------------------------------
//
TXIMPRequestId 
    CProtocolPresentityGroupsDataHostImp::SetPresentityGroupContentDataSubscriptionStateL( 
            MXIMPIdentity* aGroupId, 
            MXIMPDataSubscriptionState* aSubscriptionState, 
            MXIMPStatus* aStatus )
    {
    TRACE(_L("CProtocolPresentityGroupsDataHostImp::SetPresentityGroupContentDataSubscriptionState") );
    CXIMPRestrictedObjectCollectionImp* collection = CXIMPRestrictedObjectCollectionImp::NewLC( ETrue );
    collection->AddOrReplaceTypeL( aGroupId );
    collection->AddOrReplaceTypeL( aSubscriptionState );
    collection->AddOrReplaceTypeL( aStatus );    
    TXIMPRequestId reqId = 
        iHost.AddNewOperationL( NPresenceOps::EPrSetPresentityGroupContentDataSubscriptionState,
                                collection );
    CleanupStack::Pop( collection );
    return reqId;    
    }
    

// End of file

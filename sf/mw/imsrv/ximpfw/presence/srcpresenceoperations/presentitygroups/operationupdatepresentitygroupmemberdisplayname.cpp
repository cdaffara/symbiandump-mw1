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
* Description:  Update Presentity Group Member Display Name operation
 *
*/

#include <s32mem.h>
#include <protocolpresentitygroups.h>
#include <ximpprotocolconnection.h>
#include <protocolpresencefeatures.h>

#include "operationupdatepresentitygroupmemberdisplayname.h"
#include "presentitygroupmemberinfoimp.h"
#include "ximpidentityimp.h"
#include "protocolpresentitygroupsdatahostimp.h"
#include "ximprbufhelpers.h"
#include "ximprestrictedobjectcollectionimp.h"
#include "ximphost.h"
#include "ximpstatusimp.h"
#include "ximprequestcompleteeventimp.h"
#include "ximpobjecthelpers.h"
#include "protocolpresencedatahostimp.h"

#include "ximptrace.h"
// ======== MEMBER FUNCTIONS ========

// ---------------------------------------------------------------------------
// COperationUpdatePresentityGroupMemberDisplayName::COperationUpdatePresentityGroupMemberDisplayName()
// ---------------------------------------------------------------------------
//
EXPORT_C COperationUpdatePresentityGroupMemberDisplayName::COperationUpdatePresentityGroupMemberDisplayName()
    {
    }


// ---------------------------------------------------------------------------
// COperationUpdatePresentityGroupMemberDisplayName::ConstructL
// ---------------------------------------------------------------------------
//
void COperationUpdatePresentityGroupMemberDisplayName::ConstructL( const TDesC8& aParamPck )
    {
    RDesReadStream rs;
    rs.Open( aParamPck ); // CSI: 65 # Does not return a value.
    CleanupClosePushL( rs );

    // group id
    iGroupId = CXIMPIdentityImp::NewLC();
    CleanupStack::Pop( iGroupId );
    iGroupId->InternalizeL( rs );

    // member id
    CXIMPIdentityImp* memberId = CXIMPIdentityImp::NewLC();
    memberId->InternalizeL( rs );

    // member display name
    RBuf16 dispName;
    CleanupClosePushL( dispName );
    XIMPRBuf16Helper::InternalizeL( dispName, rs );

    iMemberInfo = CPresentityGroupMemberInfoImp::NewLC( *memberId, dispName );
    CleanupStack::Pop( iMemberInfo );
    CleanupStack::PopAndDestroy( 3 ); // dispName, memberId, rs
    }

// ---------------------------------------------------------------------------
// COperationUpdatePresentityGroupMemberDisplayName::~COperationUpdatePresentityGroupMemberDisplayName()
// ---------------------------------------------------------------------------
//
COperationUpdatePresentityGroupMemberDisplayName::~COperationUpdatePresentityGroupMemberDisplayName()
    {
    delete iGroupId;
    delete iMemberInfo;
    }

// ---------------------------------------------------------------------------
// COperationUpdatePresentityGroupMemberDisplayName::ProcessL()
// ---------------------------------------------------------------------------
//
void COperationUpdatePresentityGroupMemberDisplayName::ProcessL()
    {
    TRACE(_L("COperationUpdatePresentityGroupMemberDisplayName::ProcessL()" ) );
    CXIMPOperationBase::ProcessL();

    MProtocolPresentityGroups& presentityGroups = iMyHost->GetConnection().ProtocolPresenceFeatures().PresentityGroups();

    presentityGroups.DoUpdatePresentityGroupMemberDisplayNameL(
                                                *iGroupId,
                                                iMemberInfo->GroupMemberId(),
                                                iMemberInfo->GroupMemberDisplayName(),
                                                iReqId );
    }

// ---------------------------------------------------------------------------
// COperationUpdatePresentityGroupMemberDisplayName::RequestCompleted()
// ---------------------------------------------------------------------------
//
void COperationUpdatePresentityGroupMemberDisplayName::RequestCompletedL()
    {
    TRACE(_L("COperationUpdatePresentityGroupMemberDisplayName::RequestCompletedL()" ) );
    CXIMPOperationBase::RequestCompletedL();

    MXIMPBase* object = NULL;
    iObjCollection->GetByType( object, MPresentityGroupMemberInfo::KInterfaceId );
    if( object )
        {
        CPresentityGroupMemberInfoImp* memberInfo = 
            TXIMPGetImpClassOrPanic< CPresentityGroupMemberInfoImp >::From( *object );
        delete iMemberInfo;
        iMemberInfo = memberInfo;
        }

    if ( iStatusObj->ResultCode() == KErrNone )
        {
        CXIMPIdentityImp* groupId = 
                TXIMPObjectCloner< CXIMPIdentityImp >::CloneL( *iGroupId );
        CleanupStack::PushL( groupId );
        CPresentityGroupMemberInfoImp* memberInfo = 
                TXIMPObjectCloner< CPresentityGroupMemberInfoImp >::CloneL( *iMemberInfo );
        CleanupStack::PushL( memberInfo );        
        iMyHost->PresenceDataAccess().GroupsDataAccess().HandlePresentityGroupMemberDisplayNameUpdatedL( groupId, memberInfo );
        CleanupStack::Pop( 2, groupId );
        }

    // Set information for request complete event.
    iReqCompleteEvent->AppendParamL( iGroupId );
    iGroupId = NULL;
    iReqCompleteEvent->AppendParamL( iMemberInfo );
    iMemberInfo = NULL;
    }

// ---------------------------------------------------------------------------
// COperationUpdatePresentityGroupMemberDisplayName::Type()
// ---------------------------------------------------------------------------
//
TInt COperationUpdatePresentityGroupMemberDisplayName::Type() const
    {
    return NPresenceOps::EPrUpdatePresentityGroupMemberDisplayName;
    }

// End of file

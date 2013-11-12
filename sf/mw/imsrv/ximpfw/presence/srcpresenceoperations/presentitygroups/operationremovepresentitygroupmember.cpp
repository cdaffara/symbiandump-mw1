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
* Description:  Remove Presentity Group Member operation
 *
*/

#include <s32mem.h>
#include <ximpprotocolconnection.h>
#include <protocolpresentitygroups.h>
#include <protocolpresencefeatures.h>

#include "operationremovepresentitygroupmember.h"
#include "ximpidentityimp.h"
#include "protocolpresentitygroupsdatahostimp.h"
#include "ximphost.h"
#include "ximpstatusimp.h"
#include "protocolpresencedatahostimp.h"

#include "ximptrace.h"
// ======== MEMBER FUNCTIONS ========

// ---------------------------------------------------------------------------
// COperationRemovePresentityGroupMember::COperationRemovePresentityGroupMember()
// ---------------------------------------------------------------------------
//
EXPORT_C COperationRemovePresentityGroupMember::COperationRemovePresentityGroupMember()
    {
    }


// ---------------------------------------------------------------------------
// COperationRemovePresentityGroupMember::ConstructL
// ---------------------------------------------------------------------------
//
void COperationRemovePresentityGroupMember::ConstructL( const TDesC8& aParamPck )
    {
    RDesReadStream rs;
    rs.Open( aParamPck ); // CSI: 65 # Does not return a value.
    CleanupClosePushL( rs );

    // group id
    iGroupId = CXIMPIdentityImp::NewLC();
    CleanupStack::Pop( iGroupId );
    iGroupId->InternalizeL( rs );

    // member id
    iMemberId = CXIMPIdentityImp::NewLC();
    CleanupStack::Pop( iMemberId );
    iMemberId->InternalizeL( rs );


    CleanupStack::PopAndDestroy( 1 ); // rs
    }

// ---------------------------------------------------------------------------
// COperationRemovePresentityGroupMember::~COperationRemovePresentityGroupMember()
// ---------------------------------------------------------------------------
//
COperationRemovePresentityGroupMember::~COperationRemovePresentityGroupMember()
    {
    delete iGroupId;
    delete iMemberId;
    }

// ---------------------------------------------------------------------------
// COperationRemovePresentityGroupMember::ProcessL()
// ---------------------------------------------------------------------------
//
void COperationRemovePresentityGroupMember::ProcessL()
    {
    TRACE(_L("COperationRemovePresentityGroupMember::ProcessL()" ) );
    CXIMPOperationBase::ProcessL();

    MProtocolPresentityGroups& presentityGroups = iMyHost->GetConnection().ProtocolPresenceFeatures().PresentityGroups();

    presentityGroups.DoRemovePresentityGroupMemberL( *iGroupId,
                                               *iMemberId,
                                               iReqId );
    }

// ---------------------------------------------------------------------------
// COperationRemovePresentityGroupMember::RequestCompleted()
// ---------------------------------------------------------------------------
//
void COperationRemovePresentityGroupMember::RequestCompletedL()
    {
    TRACE(_L("COperationRemovePresentityGroupMember::RequestCompletedL()" ) );
    CXIMPOperationBase::RequestCompletedL();

    if ( iStatusObj->ResultCode() == KErrNone )
        {
        iMyHost->PresenceDataAccess().GroupsDataAccess().HandlePresentityGroupMemberRemovedL( iGroupId, iMemberId );
        iGroupId = NULL;
        iMemberId = NULL;
        }
    }

// ---------------------------------------------------------------------------
// COperationRemovePresentityGroupMember::Type()
// ---------------------------------------------------------------------------
//
TInt COperationRemovePresentityGroupMember::Type() const
    {
    return NPresenceOps::EPrRemovePresentityGroupMember;
    }


// End of file

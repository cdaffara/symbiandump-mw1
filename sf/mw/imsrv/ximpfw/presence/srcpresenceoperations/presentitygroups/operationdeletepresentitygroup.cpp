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
* Description:  Concrete bind operation
 *
*/

#include <ximpprotocolconnection.h>
#include <protocolpresentitygroups.h>
#include <protocolpresencefeatures.h>

#include "operationdeletepresentitygroup.h"
#include "presentitygroupinfoimp.h"
#include "ximpobjecthelpers.h"
#include "protocolpresentitygroupsdatahostimp.h"
#include "ximphost.h"
#include "ximpstatusimp.h"
#include "ximpidentityimp.h"
#include "protocolpresencedatahostimp.h"

#include "ximptrace.h"
// ======== MEMBER FUNCTIONS ========

// ---------------------------------------------------------------------------
// COperationDeletePresentityGroup::COperationDeletePresentityGroup()
// ---------------------------------------------------------------------------
//
EXPORT_C COperationDeletePresentityGroup::COperationDeletePresentityGroup()
    {
    }


// ---------------------------------------------------------------------------
// COperationDeletePresentityGroup::ConstructL
// ---------------------------------------------------------------------------
//
void COperationDeletePresentityGroup::ConstructL( const TDesC8& aParamPck )
    {
    CPresentityGroupInfoImp* groupInfo = CPresentityGroupInfoImp::NewLC();
    TXIMPObjectPacker< CPresentityGroupInfoImp >::UnPackL( *groupInfo,
            aParamPck );
    CleanupStack::Pop( groupInfo );
    iGroupInfo = groupInfo; 
    }

// ---------------------------------------------------------------------------
// COperationDeletePresentityGroup::~COperationDeletePresentityGroup()
// ---------------------------------------------------------------------------
//
COperationDeletePresentityGroup::~COperationDeletePresentityGroup()
    {
    delete iGroupInfo;
    }

// ---------------------------------------------------------------------------
// COperationDeletePresentityGroup::ProcessL()
// ---------------------------------------------------------------------------
//
void COperationDeletePresentityGroup::ProcessL()
    {
    TRACE(_L("COperationDeletePresentityGroup::ProcessL()" ) );
    CXIMPOperationBase::ProcessL();
    
    MProtocolPresentityGroups& presentityGroups = iMyHost->GetConnection().ProtocolPresenceFeatures().PresentityGroups();

    presentityGroups.DoDeletePresentityGroupL( iGroupInfo->GroupId(), iReqId );
    }

// ---------------------------------------------------------------------------
// COperationDeletePresentityGroup::RequestCompleted()
// ---------------------------------------------------------------------------
//
void COperationDeletePresentityGroup::RequestCompletedL()
    {
    TRACE(_L("COperationDeletePresentityGroup::RequestCompletedL()" ) );
    CXIMPOperationBase::RequestCompletedL();

    if ( iStatusObj->ResultCode() == KErrNone )
        { 
        CXIMPIdentityImp* identity = CXIMPIdentityImp::NewLC( iGroupInfo->Identity().Identity() );
        iMyHost->PresenceDataAccess().GroupsDataAccess().HandlePresentityGroupDeletedL( identity );
        CleanupStack::Pop( identity );
        }
    }

// ---------------------------------------------------------------------------
// COperationDeletePresentityGroup::Type()
// ---------------------------------------------------------------------------
//
TInt COperationDeletePresentityGroup::Type() const
    {
    return NPresenceOps::EPrDeletePresentityGroup;
    }


// End of file

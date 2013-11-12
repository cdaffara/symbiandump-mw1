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

#include "operationcreatepresentitygroup.h"
#include "presentitygroupinfoimp.h"
#include "ximpobjecthelpers.h"
#include "ximprestrictedobjectcollectionimp.h"
#include "protocolpresentitygroupsdatahostimp.h"
#include "ximphost.h"
#include "ximpstatusimp.h"
#include "ximprequestcompleteeventimp.h"
#include "protocolpresencedatahostimp.h"

#include "ximptrace.h"
// ======== MEMBER FUNCTIONS ========

// ---------------------------------------------------------------------------
// COperationCreatePresentityGroup::COperationCreatePresentityGroup()
// ---------------------------------------------------------------------------
//
EXPORT_C COperationCreatePresentityGroup::COperationCreatePresentityGroup()
    {
    }


// ---------------------------------------------------------------------------
// COperationCreatePresentityGroup::ConstructL
// ---------------------------------------------------------------------------
//
void COperationCreatePresentityGroup::ConstructL( const TDesC8& aParamPck )
    {
    CPresentityGroupInfoImp* groupInfo = CPresentityGroupInfoImp::NewLC();
    TXIMPObjectPacker< CPresentityGroupInfoImp >::UnPackL( *groupInfo,
            aParamPck );
    CleanupStack::Pop( groupInfo );
    iGroupInfo = groupInfo;
    }

// ---------------------------------------------------------------------------
// COperationCreatePresentityGroup::~COperationCreatePresentityGroup()
// ---------------------------------------------------------------------------
//
COperationCreatePresentityGroup::~COperationCreatePresentityGroup()
    {
    delete iGroupInfo;
    }

// ---------------------------------------------------------------------------
// COperationCreatePresentityGroup::ProcessL()
// ---------------------------------------------------------------------------
//
void COperationCreatePresentityGroup::ProcessL()
    {
    TRACE(_L("COperationCreatePresentityGroup::ProcessL()" ) );
    CXIMPOperationBase::ProcessL();
    MProtocolPresentityGroups& presentityGroups = iMyHost->GetConnection().ProtocolPresenceFeatures().PresentityGroups();

    presentityGroups.DoCreatePresentityGroupL( iGroupInfo->GroupId(),
                                               iGroupInfo->GroupDisplayName(), iReqId );
    }

// ---------------------------------------------------------------------------
// COperationCreatePresentityGroup::RequestCompleted()
// ---------------------------------------------------------------------------
//
void COperationCreatePresentityGroup::RequestCompletedL()
    {
    TRACE(_L("COperationCreatePresentityGroup::RequestCompletedL()" ) );
    CXIMPOperationBase::RequestCompletedL();

    MXIMPBase* object = NULL;
    iObjCollection->GetByType( object, MPresentityGroupInfo::KInterfaceId );
    if( object )
        {
        CPresentityGroupInfoImp* groupInfo = 
        TXIMPGetImpClassOrPanic< CPresentityGroupInfoImp >::From( *object );
        
        delete iGroupInfo;
        iGroupInfo = groupInfo;
        }

    if ( iStatusObj->ResultCode() == KErrNone )
        {
        CPresentityGroupInfoImp* groupInfo = 
                TXIMPObjectCloner< CPresentityGroupInfoImp >::CloneL( *iGroupInfo );
        CleanupStack::PushL( groupInfo );
        iMyHost->PresenceDataAccess().GroupsDataAccess().HandlePresentityGroupCreatedL( groupInfo );
        CleanupStack::Pop( groupInfo );
        }

    // Set information for request complete event.
    iReqCompleteEvent->AppendParamL( ( CXIMPApiDataObjBase* ) iGroupInfo );
    iGroupInfo = NULL;
    }

// ---------------------------------------------------------------------------
// COperationCreatePresentityGroup::Type()
// ---------------------------------------------------------------------------
//
TInt COperationCreatePresentityGroup::Type() const
    {
    return NPresenceOps::EPrCreatePresentityGroup;
    }


// End of file

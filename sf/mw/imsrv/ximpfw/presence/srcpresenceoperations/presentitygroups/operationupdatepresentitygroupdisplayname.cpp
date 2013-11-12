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

#include "operationupdatepresentitygroupdisplayname.h"
#include "presentitygroupinfoimp.h"
#include "ximpobjecthelpers.h"
#include "ximpidentity.h"
#include "ximprestrictedobjectcollectionimp.h"
#include "protocolpresentitygroupsdatahostimp.h"
#include "ximphost.h"
#include "ximpstatusimp.h"
#include "ximprequestcompleteeventimp.h"
#include "protocolpresencedatahostimp.h"

#include "ximptrace.h"
// ======== MEMBER FUNCTIONS ========

// ---------------------------------------------------------------------------
// COperationUpdatePresentityGroupDisplayName::COperationUpdatePresentityGroupDisplayName()
// ---------------------------------------------------------------------------
//
EXPORT_C COperationUpdatePresentityGroupDisplayName::COperationUpdatePresentityGroupDisplayName()
    {
    }


// ---------------------------------------------------------------------------
// COperationUpdatePresentityGroupDisplayName::ConstructL
// ---------------------------------------------------------------------------
//
void COperationUpdatePresentityGroupDisplayName::ConstructL( const TDesC8& aParamPck )
    {
    CPresentityGroupInfoImp* groupInfo = CPresentityGroupInfoImp::NewLC();
    TXIMPObjectPacker< CPresentityGroupInfoImp >::UnPackL( *groupInfo,
            aParamPck );
    CleanupStack::Pop( groupInfo );
    iGroupInfo = groupInfo;
    }

// ---------------------------------------------------------------------------
// COperationUpdatePresentityGroupDisplayName::~COperationUpdatePresentityGroupDisplayName()
// ---------------------------------------------------------------------------
//
COperationUpdatePresentityGroupDisplayName::~COperationUpdatePresentityGroupDisplayName()
    {
    delete iGroupInfo;
    }

// ---------------------------------------------------------------------------
// COperationUpdatePresentityGroupDisplayName::ProcessL()
// ---------------------------------------------------------------------------
//
void COperationUpdatePresentityGroupDisplayName::ProcessL()
    {
    TRACE(_L("COperationUpdatePresentityGroupDisplayName::ProcessL()" ) );
    CXIMPOperationBase::ProcessL();

    MProtocolPresentityGroups& presentityGroups = iMyHost->GetConnection().ProtocolPresenceFeatures().PresentityGroups();

    presentityGroups.DoUpdatePresentityGroupDisplayNameL( iGroupInfo->GroupId(),
                                                          iGroupInfo->GroupDisplayName(), iReqId );
    }

// ---------------------------------------------------------------------------
// COperationUpdatePresentityGroupDisplayName::RequestCompleted()
// ---------------------------------------------------------------------------
//
void COperationUpdatePresentityGroupDisplayName::RequestCompletedL()
    {
    TRACE(_L("COperationUpdatePresentityGroupDisplayName::RequestCompletedL()" ) );
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
        iMyHost->PresenceDataAccess().GroupsDataAccess().HandlePresentityGroupDisplayNameUpdatedL( groupInfo );
        CleanupStack::Pop( groupInfo );
        }

    // Set information for request complete event.
    iReqCompleteEvent->AppendParamL( ( CXIMPApiDataObjBase* ) iGroupInfo );
    iGroupInfo = NULL;
    }

// ---------------------------------------------------------------------------
// COperationUpdatePresentityGroupDisplayName::Type()
// ---------------------------------------------------------------------------
//
TInt COperationUpdatePresentityGroupDisplayName::Type() const
    {
    return NPresenceOps::EPrUpdatePresentityGroupDisplayName;
    }

// End of file

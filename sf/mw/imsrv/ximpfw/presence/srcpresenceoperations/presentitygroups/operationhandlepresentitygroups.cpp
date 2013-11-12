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
* Description:  implementation of COperationHandlePresentityGroups
 *
*/


#include "operationhandlepresentitygroups.h"
#include "grouplistsubscriptionitem.h"
#include "presencedatacacheimp.h"
#include "presentitygroupinfoimp.h"
#include "ximphost.h"
#include "ximpstatusimp.h"
#include "ximppanics.h"
#include "ximprestrictedobjectcollectionimp.h"
#include "ximpobjectcollectionimp.h"
#include "ximpobjecthelpers.h"
#include "ximpidentityimp.h"
#include "protocolpresencedatahostimp.h"

#include "ximptrace.h"

// ============================ MEMBER FUNCTIONS =============================

// ---------------------------------------------------------------------------
// COperationHandlePresentityGroups::~COperationHandlePresentityGroups()
// ---------------------------------------------------------------------------
//
COperationHandlePresentityGroups::~COperationHandlePresentityGroups()
    {
    delete iGroupAddressList;
    }


// ---------------------------------------------------------------------------
// COperationHandlePresentityGroups::COperationHandlePresentityGroups()
// ---------------------------------------------------------------------------
//
EXPORT_C COperationHandlePresentityGroups::COperationHandlePresentityGroups(
        NPresenceOps::TPresenceOpTypes aMyType )
    {
    iMyType = aMyType;
    }


// ---------------------------------------------------------------------------
// COperationHandlePresentityGroups::ConstructL()
// ---------------------------------------------------------------------------
//
void COperationHandlePresentityGroups::ConstructL( const TDesC8& /* aParamPck */ )
    {
    iGroupAddressList = new ( ELeave ) RPrGrpInfoImpArray;
    }


// ---------------------------------------------------------------------------
// COperationHandlePresentityGroups::ProcessL()
// ---------------------------------------------------------------------------
//
void COperationHandlePresentityGroups::ProcessL()
    {
    TRACE(_L("COperationHandlePresentityGroups::ProcessL()") );
    CXIMPOperationBase::ProcessL();

    // recreate temporary lists
    CGroupListSubscriptionItem& groupListSubs = 
        iMyHost->PresenceDataAccess().PresenceDataCache().GroupListSubscriptionItemLC();

    // clean out temporary lists
    groupListSubs.Clean();

    switch ( iMyType )
        {
        case NPresenceOps::EPrHandlePresentityGroupList :
            {
            MXIMPBase* object = NULL;
            iObjCollection->GetByType( object, MXIMPObjectCollection::KInterfaceId );
            if( object )
                {
                CXIMPObjectCollectionImp* objectCollection = 
                        TXIMPGetImpClassOrPanic< CXIMPObjectCollectionImp >::From( *object );
                CleanupStack::PushL( objectCollection );
                TLinearOrder< CPresentityGroupInfoImp > order( CPresentityGroupInfoImp::GroupIdLinearOrder );
                TXIMPObjectMover< MPresentityGroupInfo, CPresentityGroupInfoImp >::
                                            MoveFromCollectionToArrayL( *objectCollection, *iGroupAddressList, order );
                CleanupStack::PopAndDestroy( objectCollection );        
                }
            else
                {
                User::Leave( KErrArgument );
                }

            // handling a list of groups
            groupListSubs.SetNewListL( iGroupAddressList );
            break;
            }

        case NPresenceOps::EPrHandlePresentityGroupCreated : // fall-through
        case NPresenceOps::EPrHandlePresentityGroupDisplayNameUpdated :
            {
            // handling a list of only created groups and/or
            // handling a list of groups which have display
            // name updated
            MXIMPBase* object = NULL;
            iObjCollection->GetByType( object, MPresentityGroupInfo::KInterfaceId );
            CPresentityGroupInfoImp* createdInfo = NULL;
            if( object )
                {
                createdInfo = 
                    TXIMPGetImpClassOrPanic< CPresentityGroupInfoImp >::From( *object );
                }
            else
                {
                User::Leave( KErrArgument );
                }
                        
            CleanupStack::PushL( createdInfo );
            iGroupAddressList->AppendL( createdInfo );
            CleanupStack::Pop( createdInfo );
            groupListSubs.SetCreatedListL( iGroupAddressList );
            break;
            }

        case NPresenceOps::EPrHandlePresentityGroupDeleted :
            {
            MXIMPBase* object = NULL;
            iObjCollection->GetByType( object, MXIMPIdentity::KInterfaceId );
            CXIMPIdentityImp* identity = NULL;
            if( object )
                {
                identity = TXIMPGetImpClassOrPanic< CXIMPIdentityImp >::From( *object );
                CleanupStack::PushL( identity );
                }
            else
                {
                User::Leave( KErrArgument );
                }
            CPresentityGroupInfoImp* deletedInfo = CPresentityGroupInfoImp::NewLC( *identity, KNullDesC );            
            iGroupAddressList->AppendL( deletedInfo );
            CleanupStack::Pop( deletedInfo );
            CleanupStack::PopAndDestroy( identity );
            // handling a list of only deleted groups
            groupListSubs.SetDeletedListL( iGroupAddressList );
            break;
            }

        XIMP_DEFAULT_CASE_UNSUPPORTED( NXIMPPrivPanic::EInvalidOperationType );
        }

    iGroupAddressList = NULL;
    // ownership was transferred

    // synthesize the event to all contexts
    TRAP_IGNORE( groupListSubs.SynthesiseSubscriptionEventToAllCtxsL() );
 
    CleanupStack::PopAndDestroy(); // groupListSubs

    iMyHost->HandleAdaptationRequestCompleted( iReqId, KErrNone );
    }

// ---------------------------------------------------------------------------
// COperationHandlePresentityGroups::RequestCompleted()
// ---------------------------------------------------------------------------
//
void COperationHandlePresentityGroups::RequestCompletedL()
    {
    TRACE(_L("COperationHandlePresentityGroups::RequestCompletedL()" ) );
    CXIMPOperationBase::RequestCompletedL();    
    // no special handling for error situations
    }


// ---------------------------------------------------------------------------
// COperationHandlePresentityGroups::Type()
// ---------------------------------------------------------------------------
//
TInt COperationHandlePresentityGroups::Type() const
    {
    return iMyType;
    }

// End of file

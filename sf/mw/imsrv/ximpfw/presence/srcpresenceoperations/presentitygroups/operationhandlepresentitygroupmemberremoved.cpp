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
* Description:  Handle presentity group content operations
 *
*/


#include "operationhandlepresentitygroupmemberremoved.h"
#include "ximpobjecthelpers.h"
#include "groupcontentsubscriptionitem.h"
#include "presencedatacacheimp.h"
#include "presentitygroupmemberinfoimp.h"
#include "ximpidentityimp.h"
#include "ximphost.h"
#include "ximpstatusimp.h"
#include "ximprestrictedobjectcollectionimp.h"
#include "protocolpresencedatahostimp.h"

#include "ximptrace.h"

// ============================ MEMBER FUNCTIONS =============================

// ---------------------------------------------------------------------------
// COperationHandlePresentityGroupMemberRemoved::~COperationHandlePresentityGroupMemberRemoved()
// ---------------------------------------------------------------------------
//
COperationHandlePresentityGroupMemberRemoved::~COperationHandlePresentityGroupMemberRemoved()
    {
    delete iMemberInfo;
    delete iGroupId;
    }


// ---------------------------------------------------------------------------
// COperationHandlePresentityGroupMemberRemoved::COperationHandlePresentityGroupMemberRemoved()
// ---------------------------------------------------------------------------
//
EXPORT_C COperationHandlePresentityGroupMemberRemoved::COperationHandlePresentityGroupMemberRemoved()
    {
    }


// ---------------------------------------------------------------------------
// COperationHandlePresentityGroupMemberRemoved::ConstructL()
// ---------------------------------------------------------------------------
//
void COperationHandlePresentityGroupMemberRemoved::ConstructL( const TDesC8& /*aParamPck*/ )
    {
    }


// ---------------------------------------------------------------------------
// COperationHandlePresentityGroupMemberRemoved::ProcessL()
// ---------------------------------------------------------------------------
//
void COperationHandlePresentityGroupMemberRemoved::ProcessL()
    {
    TRACE(_L("COperationHandlePresentityGroupMemberRemoved::ProcessL()") );
    CXIMPOperationBase::ProcessL();

    MXIMPBase* object = NULL;
    iObjCollection->GetByType( object, MXIMPIdentity::KInterfaceId );
    if( object )
        {
        iGroupId = TXIMPGetImpClassOrPanic< CXIMPIdentityImp >::From( *object );        
        }
    else
        {
        User::Leave( KErrArgument );
        }
        
    object = NULL;
    iObjCollection->GetByType( object, MPresentityGroupMemberInfo::KInterfaceId );
    if( object )
        {
        iMemberInfo = TXIMPGetImpClassOrPanic< CPresentityGroupMemberInfoImp >::From( *object );
        }
    else
        {
        User::Leave( KErrArgument );
        }

    if ( iMyHost->PresenceDataAccess().PresenceDataCache().GroupContentSubscriptionItemExists( *iGroupId ) )
        {
        CGroupContentSubscriptionItem& subItem =
        iMyHost->PresenceDataAccess().PresenceDataCache().GroupContentSubscriptionItemLC( *iGroupId );

        if ( subItem.GroupMemberExistsL( *iMemberInfo ) )
            {
            // clean out temporary lists
            subItem.Clean();

            RPrGrpMemInfoImpArray* removed = new ( ELeave ) RPrGrpMemInfoImpArray;
            CleanupStack::PushL( removed );
            removed->AppendL( iMemberInfo );
            iMemberInfo = NULL;
            subItem.SetRemovedListL( removed );
            CleanupStack::Pop( removed ); // ownership was transferred

            // synthesize the event to all contexts
            TRAP_IGNORE( subItem.SynthesiseSubscriptionEventToAllCtxsL() );
            }
        CleanupStack::PopAndDestroy(); // subItem
        }

    iMyHost->HandleAdaptationRequestCompleted( iReqId, KErrNone );
    }

// ---------------------------------------------------------------------------
// COperationHandlePresentityGroupMemberRemoved::RequestCompleted()
// ---------------------------------------------------------------------------
//
void COperationHandlePresentityGroupMemberRemoved::RequestCompletedL()
    {
    TRACE(_L("COperationHandlePresentityGroupMemberRemoved::RequestCompletedL()" ) );
    CXIMPOperationBase::RequestCompletedL();

    // no special handling for error situations
    }


// ---------------------------------------------------------------------------
// COperationHandlePresentityGroupMemberRemoved::Type()
// ---------------------------------------------------------------------------
//
TInt
    COperationHandlePresentityGroupMemberRemoved::Type() const
    {
    return NPresenceOps::EPrHandlePresentityGroupMemberRemoved;
    }

// End of file

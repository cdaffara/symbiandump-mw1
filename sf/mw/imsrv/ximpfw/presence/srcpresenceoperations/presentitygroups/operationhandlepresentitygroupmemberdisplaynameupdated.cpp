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


#include "operationhandlepresentitygroupmemberdisplaynameupdated.h"
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
// COperationHandlePresentityGroupMemberDisplayNameUpdated::~COperationHandlePresentityGroupMemberDisplayNameUpdated()
// ---------------------------------------------------------------------------
//
COperationHandlePresentityGroupMemberDisplayNameUpdated::~COperationHandlePresentityGroupMemberDisplayNameUpdated()
    {
    delete iMemberInfo;
    delete iGroupId;
    }


// ---------------------------------------------------------------------------
// COperationHandlePresentityGroupMemberDisplayNameUpdated::COperationHandlePresentityGroupMemberDisplayNameUpdated()
// ---------------------------------------------------------------------------
//
EXPORT_C COperationHandlePresentityGroupMemberDisplayNameUpdated::COperationHandlePresentityGroupMemberDisplayNameUpdated()
    {
    }


// ---------------------------------------------------------------------------
// COperationHandlePresentityGroupMemberDisplayNameUpdated::ConstructL()
// ---------------------------------------------------------------------------
//
void COperationHandlePresentityGroupMemberDisplayNameUpdated::ConstructL( const TDesC8& /*aParamPck*/ )
    {
    }


// ---------------------------------------------------------------------------
// COperationHandlePresentityGroupMemberDisplayNameUpdated::ProcessL()
// ---------------------------------------------------------------------------
//
void COperationHandlePresentityGroupMemberDisplayNameUpdated::ProcessL()
    {
    TRACE(_L("COperationHandlePresentityGroupMemberDisplayNameUpdated::ProcessL()") );
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

    TInt err = KErrNone;

    if ( iMyHost->PresenceDataAccess().PresenceDataCache().GroupContentSubscriptionItemExists( *iGroupId ) )
        {
        CGroupContentSubscriptionItem& subItem =
            iMyHost->PresenceDataAccess().PresenceDataCache().GroupContentSubscriptionItemLC( *iGroupId );

        // clean out temporary lists
        subItem.Clean();

        if ( subItem.GroupMemberExistsL( *iMemberInfo )  &&
             subItem.IsGroupMemberUpdatedL( *iMemberInfo ) )
            {
            RPrGrpMemInfoImpArray* updated = new ( ELeave ) RPrGrpMemInfoImpArray;
            CleanupStack::PushL( updated );
            updated->AppendL( iMemberInfo );
            iMemberInfo = NULL; // ownership was transferred

            subItem.SetAddedListL( updated );
            CleanupStack::Pop( updated ); // ownership was transferred

            // synthesize the event to all contexts
            TRAP( err, subItem.SynthesiseSubscriptionEventToAllCtxsL() );
            }
            
        CleanupStack::PopAndDestroy(); // subItem
        }

    iMyHost->HandleAdaptationRequestCompleted( iReqId, err );  // error code means subsc. synth. error
    }

// ---------------------------------------------------------------------------
// COperationHandlePresentityGroupMemberDisplayNameUpdated::RequestCompleted()
// ---------------------------------------------------------------------------
//
void COperationHandlePresentityGroupMemberDisplayNameUpdated::RequestCompletedL()
    {
    TRACE(_L("COperationHandlePresentityGroupMemberDisplayNameUpdated::RequestCompletedL()" ) );
    CXIMPOperationBase::RequestCompletedL();

    // no special handling for error situations
    }
    

// ---------------------------------------------------------------------------
// COperationHandlePresentityGroupMemberDisplayNameUpdated::Type()
// ---------------------------------------------------------------------------
//
TInt
    COperationHandlePresentityGroupMemberDisplayNameUpdated::Type() const
    {
    return NPresenceOps::EPrHandlePresentityGroupMemberDisplayNameUpdated;
    }

// End of file

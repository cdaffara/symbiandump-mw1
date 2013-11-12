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


#include "operationhandlepresentitygroupmemberadded.h"
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
// COperationHandlePresentityGroupMemberAdded::~COperationHandlePresentityGroupMemberAdded()
// ---------------------------------------------------------------------------
//
COperationHandlePresentityGroupMemberAdded::~COperationHandlePresentityGroupMemberAdded()
    {
    delete iMemberInfo;
    delete iGroupId;
    }


// ---------------------------------------------------------------------------
// COperationHandlePresentityGroupMemberAdded::COperationHandlePresentityGroupMemberAdded()
// ---------------------------------------------------------------------------
//
EXPORT_C COperationHandlePresentityGroupMemberAdded::COperationHandlePresentityGroupMemberAdded()
    {
    }


// ---------------------------------------------------------------------------
// COperationHandlePresentityGroupMemberAdded::ConstructL()
// ---------------------------------------------------------------------------
//
void COperationHandlePresentityGroupMemberAdded::ConstructL( const TDesC8& /*aParamPck*/ )
    {
    }


// ---------------------------------------------------------------------------
// COperationHandlePresentityGroupMemberAdded::ProcessL()
// ---------------------------------------------------------------------------
//
void COperationHandlePresentityGroupMemberAdded::ProcessL()
    {
    TRACE(_L("COperationHandlePresentityGroupMemberAdded::ProcessL()") );
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

    // Check if the item exits, only full subscriptions are allowed to
    // create new ones.
    if ( iMyHost->PresenceDataAccess().PresenceDataCache().GroupContentSubscriptionItemExists( *iGroupId ) )
        {
        CGroupContentSubscriptionItem& subItem =
            iMyHost->PresenceDataAccess().PresenceDataCache().GroupContentSubscriptionItemLC( *iGroupId );

        // clean out temporary lists
        subItem.Clean();

        if ( !subItem.GroupMemberExistsL( *iMemberInfo ) )
            {
            // New member
            RPrGrpMemInfoImpArray* added = new ( ELeave ) RPrGrpMemInfoImpArray;
            CleanupStack::PushL( added );
            added->AppendL( iMemberInfo );
            iMemberInfo = NULL; // ownership was transferred

            subItem.SetAddedListL( added );
            CleanupStack::Pop( added ); // ownership was transferred

            // synthesize the event to all contexts
            TRAP_IGNORE( subItem.SynthesiseSubscriptionEventToAllCtxsL() );
            }
        else if ( subItem.IsGroupMemberUpdatedL( *iMemberInfo ) )
            {
            // Existing member updated (actually this implementation might be redundant
            // because of the implementation of subcription item. Refactor if needed.
            RPrGrpMemInfoImpArray* updated = new ( ELeave ) RPrGrpMemInfoImpArray;
            CleanupStack::PushL( updated );
            updated->AppendL( iMemberInfo );
            iMemberInfo = NULL; // ownership was transferred

            subItem.SetAddedListL( updated );
            CleanupStack::Pop( updated ); // ownership was transferred

            // synthesize the event to all contexts
            TRAP_IGNORE( subItem.SynthesiseSubscriptionEventToAllCtxsL() );
            }
            
        CleanupStack::PopAndDestroy(); // subItem
        }

    iMyHost->HandleAdaptationRequestCompleted( iReqId, KErrNone );
    }

// ---------------------------------------------------------------------------
// COperationHandlePresentityGroupMemberAdded::RequestCompleted()
// ---------------------------------------------------------------------------
//
void COperationHandlePresentityGroupMemberAdded::RequestCompletedL()
    {
    TRACE(_L("COperationHandlePresentityGroupMemberAdded::RequestCompletedL()" ) );
    CXIMPOperationBase::RequestCompletedL();

    // no special handling for error situations
    }


// ---------------------------------------------------------------------------
// COperationHandlePresentityGroupMemberAdded::Type()
// ---------------------------------------------------------------------------
//
TInt
    COperationHandlePresentityGroupMemberAdded::Type() const
    {
    return NPresenceOps::EPrHandlePresentityGroupMemberAdded;
    }


// End of file

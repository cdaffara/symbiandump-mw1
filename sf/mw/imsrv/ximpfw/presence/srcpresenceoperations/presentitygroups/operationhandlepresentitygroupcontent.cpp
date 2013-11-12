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


#include "operationhandlepresentitygroupcontent.h"
#include "ximpobjecthelpers.h"
#include "groupcontentsubscriptionitem.h"
#include "presencedatacacheimp.h"
#include "presentitygroupmemberinfoimp.h"
#include "ximpidentityimp.h"
#include "ximphost.h"
#include "ximpstatusimp.h"
#include "ximprestrictedobjectcollectionimp.h"
#include "ximpobjectcollectionimp.h"
#include "protocolpresencedatahostimp.h"

#include "ximptrace.h"
// ============================ MEMBER FUNCTIONS =============================

// ---------------------------------------------------------------------------
// COperationHandlePresentityGroupContent::~COperationHandlePresentityGroupContent()
// ---------------------------------------------------------------------------
//
COperationHandlePresentityGroupContent::~COperationHandlePresentityGroupContent()
    {
    delete iGroupMemberList;
    delete iGroupId;
    }


// ---------------------------------------------------------------------------
// COperationHandlePresentityGroupContent::COperationHandlePresentityGroupContent()
// ---------------------------------------------------------------------------
//
EXPORT_C COperationHandlePresentityGroupContent::COperationHandlePresentityGroupContent()
    {
    }


// ---------------------------------------------------------------------------
// COperationHandlePresentityGroupContent::ConstructL()
// ---------------------------------------------------------------------------
//
void COperationHandlePresentityGroupContent::ConstructL( const TDesC8& /*aParamPck*/ )
    {
    iGroupMemberList = new ( ELeave ) RPrGrpMemInfoImpArray;
    }


// ---------------------------------------------------------------------------
// COperationHandlePresentityGroupContent::ProcessL()
// ---------------------------------------------------------------------------
//
void COperationHandlePresentityGroupContent::ProcessL()
    {
    TRACE(_L("COperationHandlePresentityGroupContent::ProcessL()") );
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
    iObjCollection->GetByType( object, MXIMPObjectCollection::KInterfaceId );
    if( object )
        {
        CXIMPObjectCollectionImp* objectCollection = 
                TXIMPGetImpClassOrPanic< CXIMPObjectCollectionImp >::From( *object );
        CleanupStack::PushL( objectCollection );
        TLinearOrder< CPresentityGroupMemberInfoImp > order( CPresentityGroupMemberInfoImp::GroupIdLinearOrder );
        TXIMPObjectMover< MPresentityGroupMemberInfo, CPresentityGroupMemberInfoImp >::
                                    MoveFromCollectionToArrayL( *objectCollection, *iGroupMemberList, order );
        CleanupStack::PopAndDestroy( objectCollection );        
        }
    else
        {
        User::Leave( KErrArgument );
        }

    // recreate temporary lists
    CGroupContentSubscriptionItem& subItem =
        iMyHost->PresenceDataAccess().PresenceDataCache().GroupContentSubscriptionItemLC( *iGroupId );

    // clean out temporary lists
    subItem.Clean();
    
    subItem.SetNewListL( iGroupMemberList );
    iGroupMemberList = NULL; // ownership was transferred
    
    subItem.UpdateSubscriptionStateL( CXIMPSubscriptionItemBase::ECompleteDataReceived );

    // synthesize the event to all contexts
    TRAPD( err, subItem.SynthesiseSubscriptionEventToAllCtxsL() );

    CleanupStack::PopAndDestroy(); // subItem

    err = KErrNone;
    iMyHost->HandleAdaptationRequestCompleted( iReqId, err );
    }

// ---------------------------------------------------------------------------
// COperationHandlePresentityGroupContent::RequestCompleted()
// ---------------------------------------------------------------------------
//
void COperationHandlePresentityGroupContent::RequestCompletedL()
    {
    TRACE(_L("COperationHandlePresentityGroupContent::RequestCompletedL()" ) );
    CXIMPOperationBase::RequestCompletedL();
    // no special handling for error situations
    }
    
    
// ---------------------------------------------------------------------------
// COperationHandlePresentityGroupContent::Type()
// ---------------------------------------------------------------------------
//
TInt
    COperationHandlePresentityGroupContent::Type() const
    {
    return NPresenceOps::EPrHandlePresentityGroupContent;
    }    

// End of file

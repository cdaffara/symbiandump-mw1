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
* Description:  Handle presence blocking operations
 *
*/


#include "operationhandlepresenceblocking.h"
#include "ximpobjecthelpers.h"
#include "blocklistsubscriptionitem.h"
#include "presencedatacacheimp.h"
#include "presenceblockinfoimp.h"
#include "ximpidentityimp.h"
#include "ximphost.h"
#include "ximpstatusimp.h"
#include "ximppanics.h"
#include "ximprestrictedobjectcollectionimp.h"
#include "ximpobjectcollectionimp.h"
#include "protocolpresencedatahostimp.h"

#include "ximptrace.h"
// ============================ MEMBER FUNCTIONS =============================

// ---------------------------------------------------------------------------
// COperationHandlePresenceBlocking::~COperationHandlePresenceBlocking()
// ---------------------------------------------------------------------------
//
COperationHandlePresenceBlocking::~COperationHandlePresenceBlocking()
    {
    delete iBlockList;
    delete iBlocked;
    delete iCanceledBlockId;
    }

// ---------------------------------------------------------------------------
// COperationHandlePresenceBlocking::COperationHandlePresenceBlocking()
// ---------------------------------------------------------------------------
//
EXPORT_C COperationHandlePresenceBlocking::COperationHandlePresenceBlocking(
        TInt aMyType )
    {
    iMyType = aMyType;
    }

// ---------------------------------------------------------------------------
// COperationHandlePresenceBlocking::ConstructL()
// ---------------------------------------------------------------------------
//
void COperationHandlePresenceBlocking::ConstructL( const TDesC8& /*aParamPck*/ )
    {
    iBlockList = new( ELeave ) RPrBlockInfoImpArray;
    }

// ---------------------------------------------------------------------------
// COperationHandlePresenceBlocking::ProcessL()
// ---------------------------------------------------------------------------
//
void COperationHandlePresenceBlocking::ProcessL()
    {
    TRACE(_L("COperationHandlePresenceBlocking::ProcessL()") );
    CXIMPOperationBase::ProcessL();

    CBlockListSubscriptionItem& subItem =
        iMyHost->PresenceDataAccess().PresenceDataCache().BlockListSubscriptionItemLC();
    // clean out temporary lists
    subItem.Clean();

    switch ( iMyType )
        {
        case NPresenceOps::EPrHandlePresenceBlockList:
            {
            MXIMPBase* object = NULL;
            iObjCollection->GetByType( object, MXIMPObjectCollection::KInterfaceId );
            if( object )
                {
                CXIMPObjectCollectionImp* objectCollection = 
                        TXIMPGetImpClassOrPanic< CXIMPObjectCollectionImp >::From( *object );
                CleanupStack::PushL( objectCollection );
                TLinearOrder< CPresenceBlockInfoImp > order( CPresenceBlockInfoImp::IdLinearOrder );
                TXIMPObjectMover< MPresenceBlockInfo, CPresenceBlockInfoImp >::
                                            MoveFromCollectionToArrayL( *objectCollection, *iBlockList, order );
                CleanupStack::PopAndDestroy( objectCollection );        
                }
            else
                {
                User::Leave( KErrArgument );
                }

            subItem.SetNewListL( iBlockList );
            iBlockList = NULL;
            break;
            }

        case NPresenceOps::EPrHandlePresenceBlocked:
            {
            MXIMPBase* object = NULL;
            iObjCollection->GetByType( object, MPresenceBlockInfo::KInterfaceId );
            if( object )
                {
                iBlocked = TXIMPGetImpClassOrPanic< CPresenceBlockInfoImp >::From( *object );
                }
            else
                {
                User::Leave( KErrArgument );
                }            
            subItem.SetBlockedL( iBlocked );
            iBlocked = NULL; // ownership was transferred
            break;
            }

        case NPresenceOps::EPrHandlePresenceBlockCanceled:
            {
            MXIMPBase* object = NULL;
            iObjCollection->GetByType( object, MXIMPIdentity::KInterfaceId );
            if( object )
                {
                iCanceledBlockId = TXIMPGetImpClassOrPanic< CXIMPIdentityImp >::From( *object );
                }
            else
                {
                User::Leave( KErrArgument );
                }            
            subItem.SetCanceledBlockIdL( iCanceledBlockId );
            iCanceledBlockId = NULL; // ownership was transferred
            break;
            }

        XIMP_DEFAULT_CASE_UNSUPPORTED( NXIMPPrivPanic::EInvalidOperationType );
        }

    // synthesize the event to all contexts
    TRAPD( err, subItem.SynthesiseSubscriptionEventToAllCtxsL() );

    CleanupStack::PopAndDestroy(); // subItem

    err = KErrNone;
    iMyHost->HandleAdaptationRequestCompleted( iReqId, err );
    }

// ---------------------------------------------------------------------------
// COperationHandlePresenceBlocking::RequestCompleted()
// ---------------------------------------------------------------------------
//
void COperationHandlePresenceBlocking::RequestCompletedL()
    {
    TRACE(_L("COperationHandlePresenceBlocking::RequestCompletedL()" ) );
    CXIMPOperationBase::RequestCompletedL();

    // no special handling for error situations
    }


// ---------------------------------------------------------------------------
// COperationHandlePresenceBlocking::Type()
// ---------------------------------------------------------------------------
//
TInt COperationHandlePresenceBlocking::Type() const
    {
    return iMyType;
    }

// End of file

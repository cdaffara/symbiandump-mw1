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
* Description:  Handle presence watcher list operation
 *
*/


#include "operationhandlepresencewatcherlist.h"

#include "presencedatacacheimp.h"

#include "presencewatcherinfoimp.h"
#include "watcherlistsubscriptionitem.h"
#include "ximphost.h"
#include "ximpstatusimp.h"

#include "ximpobjecthelpers.h"
#include "ximprestrictedobjectcollectionimp.h"
#include "ximpobjectcollectionimp.h"
#include "protocolpresencedatahostimp.h"

#include "ximptrace.h"

// ============================ MEMBER FUNCTIONS =============================

// ---------------------------------------------------------------------------
// COperationHandlePresenceWatcherList::~COperationHandlePresenceWatcherList()
// ---------------------------------------------------------------------------
//
COperationHandlePresenceWatcherList::~COperationHandlePresenceWatcherList()
    {
    delete iWatcherList;
    }

// ---------------------------------------------------------------------------
// COperationHandlePresenceWatcherList::COperationHandlePresenceWatcherList()
// ---------------------------------------------------------------------------
//
EXPORT_C COperationHandlePresenceWatcherList::COperationHandlePresenceWatcherList()
    {
    }

// ---------------------------------------------------------------------------
// COperationHandlePresenceWatcherList::ConstructL()
// ---------------------------------------------------------------------------
//
void COperationHandlePresenceWatcherList::ConstructL( const TDesC8& /* aParamPck */ )
    {
    iWatcherList = new ( ELeave ) RPrWatLstInfoImpArray;
    }

// ---------------------------------------------------------------------------
// From CXIMPOperationBase class.
// COperationHandlePresenceWatcherList::ProcessL()
// ---------------------------------------------------------------------------
//
void COperationHandlePresenceWatcherList::ProcessL()
    {
    TRACE(_L("COperationHandlePresenceWatcherList::ProcessL()") );
    CXIMPOperationBase::ProcessL();

    MXIMPBase* object = NULL;
    iObjCollection->GetByType( object, MXIMPObjectCollection::KInterfaceId );
    if( object )
        {
        CXIMPObjectCollectionImp* objectCollection = 
                TXIMPGetImpClassOrPanic< CXIMPObjectCollectionImp >::From( *object );
        CleanupStack::PushL( objectCollection );
        TLinearOrder< CPresenceWatcherInfoImp > order( 
                                CPresenceWatcherInfoImp::IdLinearOrder );
        TXIMPObjectMover< MPresenceWatcherInfo, CPresenceWatcherInfoImp >::MoveFromCollectionToArrayL(
                                                            *objectCollection,
                                                            *iWatcherList,
                                                            order );
        CleanupStack::PopAndDestroy( objectCollection );        
        }
    else
        {
        User::Leave( KErrArgument );
        }

    CWatcherListSubscriptionItem& subItem =
        iMyHost->PresenceDataAccess().PresenceDataCache().WatcherListSubscriptionItemLC();

    // clean out temporary lists
    subItem.Clean();

    subItem.SetNewListL( iWatcherList );
    iWatcherList = NULL; // ownership was transferred
    subItem.UpdateSubscriptionStateL(
                    CXIMPSubscriptionItemBase::ECompleteDataReceived );

    // synthesize the event to all contexts
    TRAPD( err, subItem.SynthesiseSubscriptionEventToAllCtxsL() );

    CleanupStack::PopAndDestroy(); // subItem

    err = KErrNone;
    iMyHost->HandleAdaptationRequestCompleted( iReqId, err );
    }

// ---------------------------------------------------------------------------
// From CXIMPOperationBase class.
// COperationHandlePresenceWatcherList::RequestCompleted()
// ---------------------------------------------------------------------------
//
void COperationHandlePresenceWatcherList::RequestCompletedL()
    {
    TRACE(_L("COperationHandlePresenceWatcherList::RequestCompletedL()" ) );
    CXIMPOperationBase::RequestCompletedL();

    // no special handling for error situations
    }

// ---------------------------------------------------------------------------
// From CXIMPOperationBase class.
// COperationHandlePresenceWatcherList::Type()
// ---------------------------------------------------------------------------
//
TInt COperationHandlePresenceWatcherList::Type() const
    {
    return NPresenceOps::EPrHandlePresenceWatcherList;
    }


// End of file

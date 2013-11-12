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
* Description:  Internal synthesise subscription event operation
 *
*/


#include "operationsynthesisesubscriptionevent.h"
#include "ownpresencesubscriptionitem.h"
#include "grouplistsubscriptionitem.h"
#include "groupcontentsubscriptionitem.h"
#include "presentitypresencesubscriptionitem.h"
#include "presentitygroupmemberspresencesubscriptionitem.h"
#include "watcherlistsubscriptionitem.h"
#include "grantrequestlistsubscriptionitem.h"
#include "blocklistsubscriptionitem.h"
#include "presencedatacacheimp.h"
#include "protocolpresencedatahostimp.h"
#include "ximpobjecthelpers.h"
#include "ximpidentityimp.h"
#include "ximphost.h"
#include "ximppanics.h"
#include "ximppsccontextimp.h"

#include "ximptrace.h"
// ============================ MEMBER FUNCTIONS =============================

// ---------------------------------------------------------------------------
// COperationSynthesiseSubscriptionEvent::COperationSynthesiseSubscriptionEvent()
// ---------------------------------------------------------------------------
//
EXPORT_C COperationSynthesiseSubscriptionEvent::COperationSynthesiseSubscriptionEvent(
    COperationSynthesiseSubscriptionEvent::TSubscriptionType aType, TBool aForceEvent )
    : iType( aType ), iForceEvent( aForceEvent )
    {
    }



// ---------------------------------------------------------------------------
// COperationSynthesiseSubscriptionEvent::~COperationSynthesiseSubscriptionEvent()
// ---------------------------------------------------------------------------
//
COperationSynthesiseSubscriptionEvent::~COperationSynthesiseSubscriptionEvent()
    {
    delete iIdentity;
    }

// ---------------------------------------------------------------------------
// COperationSynthesiseSubscriptionEvent::ConstructL()
// ---------------------------------------------------------------------------
//
void COperationSynthesiseSubscriptionEvent::ConstructL( const TDesC8& /*aParamPck*/ )
    {
    }


// ---------------------------------------------------------------------------
// COperationSynthesiseSubscriptionEvent::ConstructL()
// ---------------------------------------------------------------------------
//
void COperationSynthesiseSubscriptionEvent::ConstructL( const CXIMPIdentityImp& aIdentity )
    {
    iIdentity = TXIMPObjectCloner< CXIMPIdentityImp >::CloneL( aIdentity );
    }

// ---------------------------------------------------------------------------
// COperationSynthesiseSubscriptionEvent::ProcessL()
// ---------------------------------------------------------------------------
//
void COperationSynthesiseSubscriptionEvent::ProcessL()
    {
    TRACE(_L("COperationSynthesiseSubscriptionEvent::ProcessL()" ) );
    CXIMPOperationBase::ProcessL();

    MXIMPSubscriptionItem* subscriptionItem = NULL;
    switch( iType )
        {
        case EOwnPresence:
            {
            TRACE(_L("COperationSynthesiseSubscriptionEvent::ProcessL() : case EOwnPresence" ) );
            subscriptionItem = &iMyHost->PresenceDataAccess().PresenceDataCache().OwnPresenceSubscriptionItemLC();
            break;
            }
        case EPresenceBlockList:
            {
            TRACE(_L("COperationSynthesiseSubscriptionEvent::ProcessL() : case EPresenceBlockList" ) );
            subscriptionItem = &iMyHost->PresenceDataAccess().PresenceDataCache().BlockListSubscriptionItemLC();
            break;
            }
        case EPresenceGrantRequestList:
            {
            TRACE(_L("COperationSynthesiseSubscriptionEvent::ProcessL() : case EPresenceGrantRequestList" ) );
            subscriptionItem = &iMyHost->PresenceDataAccess().PresenceDataCache().GrantRequestListSubscriptionItemLC();
            break;
            }
        case EPresenceWatcherList:
            {
            TRACE(_L("COperationSynthesiseSubscriptionEvent::ProcessL() : case EPresenceWatcherList" ) );
            subscriptionItem = &iMyHost->PresenceDataAccess().PresenceDataCache().WatcherListSubscriptionItemLC();
            break;
            }
        case EPresentityGroupList:
            {
            TRACE(_L("COperationSynthesiseSubscriptionEvent::ProcessL() : case EPresentityGroupList" ) );
            subscriptionItem = &iMyHost->PresenceDataAccess().PresenceDataCache().GroupListSubscriptionItemLC();
            break;
            }
        case EPresentityGroupContent:
            {
            TRACE(_L("COperationSynthesiseSubscriptionEvent::ProcessL() : case EPresentityGroupContent" ) );
            subscriptionItem = &iMyHost->PresenceDataAccess().PresenceDataCache().GroupContentSubscriptionItemLC( *iIdentity );
            break;
            }
        case EPresentityPresence:
            {
            TRACE(_L("COperationSynthesiseSubscriptionEvent::ProcessL() : case EPresentityPresence" ) );
            subscriptionItem = &iMyHost->PresenceDataAccess().PresenceDataCache().PresentityPresenceSubscriptionItemLC( *iIdentity );
            break;
            }
        case EPresentityGroupMembersPresence:
            {
            TRACE(_L("COperationSynthesiseSubscriptionEvent::ProcessL() : case EPresentityGroupMembersPresence" ) );
            subscriptionItem = &iMyHost->PresenceDataAccess().PresenceDataCache().PresentityGroupMembersPresenceSubscriptionItemLC( *iIdentity );
            break;
            }

        
        XIMP_DEFAULT_CASE_UNSUPPORTED( NXIMPPrivPanic::EInvalidSubscriptionType );
        }

    if( iContext )
        {
        TRACE(_L("COperationSynthesiseSubscriptionEvent::ProcessL() : subscriptionItem->ActivateSubscriberL( iContext )" ) );    
        subscriptionItem->ActivateSubscriberL( iContext );
        subscriptionItem->SynthesiseSubscriptionEventToL( iContext, iForceEvent );
        }
    else
        {
        TRACE(_L("COperationSynthesiseSubscriptionEvent::ProcessL() : subscriptionItem->SynthesiseSubscriptionEventToAllCtxsL" ) );    
        subscriptionItem->SynthesiseSubscriptionEventToAllCtxsL();
        }
        
    CleanupStack::PopAndDestroy(); // subscriptionItem
    iMyHost->HandleInternalRequestCompleted( iReqId, KErrNone );
    }

// ---------------------------------------------------------------------------
// COperationSynthesiseSubscriptionEvent::RequestCompleted()
// ---------------------------------------------------------------------------
//
void COperationSynthesiseSubscriptionEvent::RequestCompletedL()
    {
    TRACE(_L("COperationSynthesiseSubscriptionEvent::RequestCompletedL()" ) );
    CXIMPOperationBase::RequestCompletedL();
    }


// ---------------------------------------------------------------------------
// COperationSynthesiseSubscriptionEvent::Type()
// ---------------------------------------------------------------------------
//
TInt
    COperationSynthesiseSubscriptionEvent::Type() const
    {
    return NPresenceOps::EPrSynthesisePresenceSubscription;
    }



// End of file

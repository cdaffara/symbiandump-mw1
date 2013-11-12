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
* Description:  Set subscription data
 *
*/


#include "operationsetsubscriptiondata.h"
#include "ximpsubscriptionitem.h"
#include "ximphost.h"
#include "ownpresencesubscriptionitem.h"
#include "grouplistsubscriptionitem.h"
#include "groupcontentsubscriptionitem.h"
#include "presentitypresencesubscriptionitem.h"
#include "presentitygroupmemberspresencesubscriptionitem.h"
#include "watcherlistsubscriptionitem.h"
#include "grantrequestlistsubscriptionitem.h"
#include "blocklistsubscriptionitem.h"
#include "ximpobjecthelpers.h"
#include "ximppanics.h"
#include "ximpidentityimp.h"
#include "ximppsccontextimp.h"
#include "ximprestrictedobjectcollectionimp.h"
#include "ximpstatusimp.h"
#include "ximpdatasubscriptionstateimp.h"
#include "presencedatacacheimp.h"
#include "protocolpresencedatahostimp.h"

#include "ximptrace.h"

// ============================ MEMBER FUNCTIONS =============================

// ---------------------------------------------------------------------------
// COperationSetSubscriptionData::COperationSetSubscriptionData()
// ---------------------------------------------------------------------------
//
EXPORT_C COperationSetSubscriptionData::COperationSetSubscriptionData(
    NPresenceOps::TPresenceOpTypes aType )
    :iType( aType )
    {
    }



// ---------------------------------------------------------------------------
// COperationSetSubscriptionData::~COperationSetSubscriptionData()
// ---------------------------------------------------------------------------
//
COperationSetSubscriptionData::~COperationSetSubscriptionData()
    {
    delete iStatus;
    delete iSubscriptionState;
    delete iIdentity;
    }

// ---------------------------------------------------------------------------
// COperationSetSubscriptionData::ConstructL()
// ---------------------------------------------------------------------------
//
void COperationSetSubscriptionData::ConstructL( const TDesC8& /*aParamPck*/ )
    {
    }

// ---------------------------------------------------------------------------
// COperationSynthesiseSubscriptionEvent::ProcessL()
// ---------------------------------------------------------------------------
//
void COperationSetSubscriptionData::ProcessL()
    {
    TRACE(_L("COperationSetSubscriptionData::ProcessL()" ) );
    CXIMPOperationBase::ProcessL();

    MXIMPBase* object = NULL;
    iObjCollection->GetByType( object, MXIMPIdentity::KInterfaceId );
    if( object )
        {
        iIdentity = TXIMPGetImpClassOrPanic< CXIMPIdentityImp >::From( *object );        
        }

    iObjCollection->GetByType( object, MXIMPDataSubscriptionState::KInterfaceId );
    if( object )
        {
        iSubscriptionState = TXIMPGetImpClassOrPanic< CXIMPDataSubscriptionStateImp >::From( *object );        
        }
    else
        {
        User::Leave( KErrArgument );
        }

    iObjCollection->GetByType( object, MXIMPDataSubscriptionState::KInterfaceId );
    if( object )
        {
        iSubscriptionState = TXIMPGetImpClassOrPanic< CXIMPDataSubscriptionStateImp >::From( *object );        
        }
    else
        {
        User::Leave( KErrArgument );
        }

    iObjCollection->GetByType( object, MXIMPStatus::KInterfaceId );
    if( object )
        {
        iStatus = TXIMPGetImpClassOrPanic< CXIMPStatusImp >::From( *object );        
        }
    
    MXIMPSubscriptionItem* subscriptionItem = NULL;
    switch( iType )
        {
        case NPresenceOps::EPrSetOwnPresenceDataSubscriptionState:
            {
            subscriptionItem = &iMyHost->PresenceDataAccess().PresenceDataCache().OwnPresenceSubscriptionItemLC();
            break;
            }
        case NPresenceOps::EPrSetPresenceBlockDataSubscriptionState:
            {
            subscriptionItem = &iMyHost->PresenceDataAccess().PresenceDataCache().BlockListSubscriptionItemLC();
            break;
            }
        case NPresenceOps::EPrSetPresenceGrantRequestDataSubscriptionState:
            {
            subscriptionItem = &iMyHost->PresenceDataAccess().PresenceDataCache().GrantRequestListSubscriptionItemLC();
            break;
            }
        case NPresenceOps::EPrSetPresenceWatcherListDataSubscriptionState:
            {
            subscriptionItem = &iMyHost->PresenceDataAccess().PresenceDataCache().WatcherListSubscriptionItemLC();
            break;
            }
        case NPresenceOps::EPrSetPresentityGroupListDataSubscriptionState:
            {
            subscriptionItem = &iMyHost->PresenceDataAccess().PresenceDataCache().GroupListSubscriptionItemLC();
            break;
            }
        case NPresenceOps::EPrSetPresentityGroupContentDataSubscriptionState:
            {
            if( !iIdentity )
                {
                User::Leave( KErrArgument );
                }
            subscriptionItem = &iMyHost->PresenceDataAccess().PresenceDataCache().GroupContentSubscriptionItemLC( *iIdentity );
            break;
            }
        case NPresenceOps::EPrSetPresentityPresenceDataSubscriptionState:
            {
            if( !iIdentity )
                {
                User::Leave( KErrArgument );
                }
            subscriptionItem = &iMyHost->PresenceDataAccess().PresenceDataCache().PresentityPresenceSubscriptionItemLC( *iIdentity );
            break;
            }

        XIMP_DEFAULT_CASE_UNSUPPORTED( NXIMPPrivPanic::EInvalidSubscriptionType );
        }

    subscriptionItem->SetDataSubscriptionStateL( *iSubscriptionState );
    iSubscriptionState = NULL;

    iMyHost->HandleAdaptationRequestCompleted( iReqId, KErrNone );
    }

// ---------------------------------------------------------------------------
// COperationSetSubscriptionData::RequestCompleted()
// ---------------------------------------------------------------------------
//
void COperationSetSubscriptionData::RequestCompletedL()
    {
    TRACE(_L("COperationSetSubscriptionData::RequestCompletedL()" ) );
    CXIMPOperationBase::RequestCompletedL();
    }


// ---------------------------------------------------------------------------
// COperationSetSubscriptionData::Type()
// ---------------------------------------------------------------------------
//
TInt
    COperationSetSubscriptionData::Type() const
    {
    return iType;
    }



// End of file

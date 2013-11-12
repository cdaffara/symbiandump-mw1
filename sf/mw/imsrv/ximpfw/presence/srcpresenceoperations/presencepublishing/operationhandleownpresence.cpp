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
* Description: implementation of COperationHandleOwnPresence 
 *
*/


#include "operationhandleownpresence.h"

#include "presenceinfoimp.h"
#include "ximpobjecthelpers.h"
#include "ownpresencesubscriptionitem.h"
#include "presencedatacacheimp.h"
#include "ximphost.h"

#include "ximprestrictedobjectcollectionimp.h"
#include "protocolpresencedatahostimp.h"

#include "ximptrace.h"

// ============================ MEMBER FUNCTIONS =============================

// ---------------------------------------------------------------------------
// COperationHandleOwnPresence::~COperationHandleOwnPresence()
// ---------------------------------------------------------------------------
//
COperationHandleOwnPresence::~COperationHandleOwnPresence()
    {
    delete iPresInfo;
    }


// ---------------------------------------------------------------------------
// COperationHandleOwnPresence::COperationHandleOwnPresence()
// ---------------------------------------------------------------------------
//
EXPORT_C COperationHandleOwnPresence::COperationHandleOwnPresence()
    {
    }


// ---------------------------------------------------------------------------
// COperationHandleOwnPresence::ConstructL()
// ---------------------------------------------------------------------------
//
void COperationHandleOwnPresence::ConstructL( const TDesC8& /* aParamPck */ )
    {
    }


// ---------------------------------------------------------------------------
// COperationHandleOwnPresence::ProcessL()
// ---------------------------------------------------------------------------
//
void COperationHandleOwnPresence::ProcessL()
    {
    TRACE(_L("COperationHandleOwnPresence::ProcessL()") );
    CXIMPOperationBase::ProcessL();

    MXIMPBase* object = NULL;
    iObjCollection->GetByType( object, MPresenceInfo::KInterfaceId );
    if ( object )
        {
        iPresInfo = TXIMPGetImpClassOrPanic< CPresenceInfoImp >::From( *object );
        }
    else
        {
        User::Leave( KErrArgument );
        }
    

    COwnPresenceSubscriptionItem& ownPresence = 
                            iMyHost->PresenceDataAccess().PresenceDataCache().OwnPresenceSubscriptionItemLC();
    ownPresence.SetPresenceInfoL( iPresInfo );
    iPresInfo = NULL;
    TRAPD( err, ownPresence.SynthesiseSubscriptionEventToAllCtxsL() );
    CleanupStack::PopAndDestroy(); // ownPresence
    iMyHost->HandleAdaptationRequestCompleted( iReqId, err );    
    }

// ---------------------------------------------------------------------------
// COperationHandleOwnPresence::RequestCompleted()
// ---------------------------------------------------------------------------
//
void COperationHandleOwnPresence::RequestCompletedL()
    {
    TRACE(_L("COperationHandleOwnPresence::RequestCompletedL()" ) );
    CXIMPOperationBase::RequestCompletedL();
    
    // no special handling for error situations
    }


// ---------------------------------------------------------------------------
// COperationHandleOwnPresence::Type()
// ---------------------------------------------------------------------------
//
TInt COperationHandleOwnPresence::Type() const
    {
    return NPresenceOps::EPrHandleOwnPresence;
    }

// End of file

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
* Description:  implementation of COperationHandlePresentityPresence
 *
*/


#include "operationhandlepresentitypresence.h"
#include "presenceinfoimp.h"
#include "ximpobjecthelpers.h"
#include "presencedatacacheimp.h"
#include "ximpidentityimp.h"
#include "presentitypresencesubscriptionitem.h"
#include "ximphost.h"

#include "ximprestrictedobjectcollectionimp.h"
#include "protocolpresencedatahostimp.h"

#include "ximptrace.h"
// ============================ MEMBER FUNCTIONS =============================

// ---------------------------------------------------------------------------
// COperationHandlePresentityPresence::~COperationHandlePresentityPresence()
// ---------------------------------------------------------------------------
//
COperationHandlePresentityPresence::~COperationHandlePresentityPresence()
    {
    delete iIdentity;
    delete iPresenceInfo;
    }

// ---------------------------------------------------------------------------
// COperationHandlePresentityPresence::COperationHandlePresentityPresence()
// ---------------------------------------------------------------------------
//
EXPORT_C COperationHandlePresentityPresence::COperationHandlePresentityPresence( TInt aOperation )
: iOperation( aOperation )
    {
    }

// ---------------------------------------------------------------------------
// COperationHandlePresentityPresence::ConstructL()
// ---------------------------------------------------------------------------
//
void COperationHandlePresentityPresence::ConstructL( const TDesC8& /* aParamPck */ )
    {
    }


// ---------------------------------------------------------------------------
// COperationHandlePresentityPresence::ProcessL()
// ---------------------------------------------------------------------------
//
void COperationHandlePresentityPresence::ProcessL()
    {
    TRACE(_L("COperationHandlePresentityPresence::ProcessL()") );
    CXIMPOperationBase::ProcessL();

    MXIMPBase* object = NULL;
    iObjCollection->GetByType( object, MXIMPIdentity::KInterfaceId );
    if ( object )
        {
        iIdentity = TXIMPGetImpClassOrPanic< CXIMPIdentityImp >::From( *object );
        }
    else
        {
        User::Leave( KErrArgument );
        }
    object = NULL;
    iObjCollection->GetByType( object, MPresenceInfo::KInterfaceId );
    if( object )
        {
        iPresenceInfo = TXIMPGetImpClassOrPanic< CPresenceInfoImp >::From( *object );
        }
    else
        {
        User::Leave( KErrArgument );
        }
    
    CPresentityPresenceSubscriptionItem& presenceItem = 
        iMyHost->PresenceDataAccess().PresenceDataCache().PresentityPresenceSubscriptionItemLC( *iIdentity );
    presenceItem.SetPresenceInfoL( iPresenceInfo );        
    iPresenceInfo = NULL;
    
    if( iOperation == NPresenceOps::EPrHandlePresentityPresenceTerminated )
        {
        //presenceItem.SetSubscriptionState( MPresenceInfo::EDataSubscriptionTerminated );
        }
    
    TRAPD( err, presenceItem.SynthesiseSubscriptionEventToAllCtxsL() );
    CleanupStack::PopAndDestroy(); // presenceItem
    iMyHost->HandleAdaptationRequestCompleted( iReqId, err );
    }

// ---------------------------------------------------------------------------
// COperationHandlePresentityPresence::RequestCompleted()
// ---------------------------------------------------------------------------
//
void COperationHandlePresentityPresence::RequestCompletedL()
    {
    TRACE(_L("COperationHandlePresentityPresence::RequestCompletedL()" ) );
    CXIMPOperationBase::RequestCompletedL();    
    // no special handling for error situations
    }


// ---------------------------------------------------------------------------
// COperationHandlePresentityPresence::Type()
// ---------------------------------------------------------------------------
//
TInt COperationHandlePresentityPresence::Type() const
    {
    return ( TInt) iOperation;
    }

// End of file

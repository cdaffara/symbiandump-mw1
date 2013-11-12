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
* Description:  Withdraw presence from presentity
 *
*/


#include <ximpprotocolconnection.h>
#include <protocolpresenceauthorization.h>
#include <protocolpresencefeatures.h>

#include "operationcancelpresenceblockfrompresentity.h"
#include "presenceblockinfoimp.h"
#include "ximpidentityimp.h"
#include "ximpobjecthelpers.h"
#include "protocolpresenceauthorizationdatahostimp.h"
#include "ximprestrictedobjectcollectionimp.h"
#include "ximphost.h"
#include "ximpstatusimp.h"
#include "protocolpresencedatahostimp.h"

#include "ximptrace.h"

// ============================ MEMBER FUNCTIONS =============================

// ---------------------------------------------------------------------------
// COperationCancelPresenceBlockFromPresentity::COperationCancelPresenceBlockFromPresentity()
// ---------------------------------------------------------------------------
//
EXPORT_C COperationCancelPresenceBlockFromPresentity::COperationCancelPresenceBlockFromPresentity()
    {
    }


// ---------------------------------------------------------------------------
// COperationCancelPresenceBlockFromPresentity::~COperationCancelPresenceBlockFromPresentity()
// ---------------------------------------------------------------------------
//
COperationCancelPresenceBlockFromPresentity::~COperationCancelPresenceBlockFromPresentity()
    {
    delete iIdentity;
    }

// ---------------------------------------------------------------------------
// COperationCancelPresenceBlockFromPresentity::ConstructL()
// ---------------------------------------------------------------------------
//
void COperationCancelPresenceBlockFromPresentity::ConstructL( const TDesC8& aParamPck )
    {
    iIdentity = CXIMPIdentityImp::NewLC();
    CleanupStack::Pop( iIdentity );
    TXIMPObjectPacker< CXIMPIdentityImp >::UnPackL( *iIdentity, aParamPck );
    }


// ---------------------------------------------------------------------------
// COperationCancelPresenceBlockFromPresentity::ProcessL()
// ---------------------------------------------------------------------------
//
void COperationCancelPresenceBlockFromPresentity::ProcessL()
    {
    TRACE(_L("COperationCancelPresenceBlockFromPresentity::ProcessL()" ) );
    CXIMPOperationBase::ProcessL();

    MProtocolPresenceAuthorization& authorization =
                            iMyHost->GetConnection().ProtocolPresenceFeatures().PresenceAuthorization();

    authorization.DoCancelPresenceBlockFromPresentityL( *iIdentity, iReqId );

    }

// ---------------------------------------------------------------------------
// COperationCancelPresenceBlockFromPresentity::RequestCompleted()
// ---------------------------------------------------------------------------
//
void COperationCancelPresenceBlockFromPresentity::RequestCompletedL()
    {
    TRACE(_L("COperationCancelPresenceBlockFromPresentity::RequestCompletedL()" ) );
    CXIMPOperationBase::RequestCompletedL();

    MXIMPBase* object = NULL;
    iObjCollection->GetByType( object,
                       MPresenceBlockInfo::KInterfaceId );
    if( object )
        {
        CPresenceBlockInfoImp* blockInfo = 
            TXIMPGetImpClassOrPanic< CPresenceBlockInfoImp >::From( *object );
        CleanupStack::PushL( blockInfo );
        delete iIdentity;
        iIdentity = NULL;
        iIdentity = TXIMPObjectCloner< CXIMPIdentityImp >::CloneL(
                                            blockInfo->IdentityImp() );
        CleanupStack::PopAndDestroy( blockInfo );
        }

    if ( iStatusObj->ResultCode() == KErrNone )
        {
        iMyHost->PresenceDataAccess().AuthorizationDataAccess().HandlePresenceBlockCanceledL( iIdentity );
        iIdentity = NULL;
        }
    SynthesiseEventL();
    }


// ---------------------------------------------------------------------------
// COperationCancelPresenceBlockFromPresentity::Type()
// ---------------------------------------------------------------------------
//
TInt
    COperationCancelPresenceBlockFromPresentity::Type() const
    {
    return NPresenceOps::EPrCancelPresenceBlockFromPresentity;
    }

// ---------------------------------------------------------------------------
// COperationCancelPresenceBlockFromPresentity::SynthesiseEventL()
// ---------------------------------------------------------------------------
//
void COperationCancelPresenceBlockFromPresentity::SynthesiseEventL()
    {
    }



// End of file

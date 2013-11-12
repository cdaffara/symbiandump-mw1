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
* Description:  Block presence for presentity operation
 *
*/


#include <ximpprotocolconnection.h>
#include <protocolpresenceauthorization.h>
#include <protocolpresencefeatures.h>

#include "operationblockpresenceforpresentity.h"
#include "presenceblockinfoimp.h"
#include "ximpidentityimp.h"
#include "protocolpresenceauthorizationdatahostimp.h"
#include "ximpobjecthelpers.h"
#include "ximprestrictedobjectcollectionimp.h"
#include "ximphost.h"
#include "ximpstatusimp.h"
#include "protocolpresencedatahostimp.h"

#include "ximptrace.h"

// ============================ MEMBER FUNCTIONS =============================

// ---------------------------------------------------------------------------
// COperationBlockPresenceForPresentity::COperationBlockPresenceForPresentity()
// ---------------------------------------------------------------------------
//
EXPORT_C COperationBlockPresenceForPresentity::COperationBlockPresenceForPresentity()
    {
    }


// ---------------------------------------------------------------------------
// COperationBlockPresenceForPresentity::~COperationBlockPresenceForPresentity()
// ---------------------------------------------------------------------------
//
COperationBlockPresenceForPresentity::~COperationBlockPresenceForPresentity()
    {
    delete iIdentity;
    delete iBlockInfo;
    }

// ---------------------------------------------------------------------------
// COperationBlockPresenceForPresentity::ConstructL()
// ---------------------------------------------------------------------------
//
void COperationBlockPresenceForPresentity::ConstructL( const TDesC8& aParamPck )
    {
    // Read params. identity
    RDesReadStream rs;
    rs.Open( aParamPck ); // CSI: 65 # Does not return a value.
    CleanupClosePushL( rs );

    // identity
    iIdentity = CXIMPIdentityImp::NewLC();
    CleanupStack::Pop( iIdentity );
    iIdentity->InternalizeL( rs );

    CleanupStack::PopAndDestroy(); // rs
    }


// ---------------------------------------------------------------------------
// COperationBlockPresenceForPresentity::ProcessL()
// ---------------------------------------------------------------------------
//
void COperationBlockPresenceForPresentity::ProcessL()
    {
    TRACE(_L("COperationBlockPresenceForPresentity::ProcessL()" ) );
    CXIMPOperationBase::ProcessL();

    MProtocolPresenceAuthorization& authorization =
                            iMyHost->GetConnection().ProtocolPresenceFeatures().PresenceAuthorization();

    authorization.DoBlockPresenceForPresentityL( *iIdentity, iReqId );

    }

// ---------------------------------------------------------------------------
// COperationBlockPresenceForPresentity::RequestCompleted()
// ---------------------------------------------------------------------------
//
void COperationBlockPresenceForPresentity::RequestCompletedL()
    {
    TRACE(_L("COperationBlockPresenceForPresentity::RequestCompletedL()" ) );
    CXIMPOperationBase::RequestCompletedL();

    MXIMPBase* object = NULL;
    iObjCollection->GetByType( object,
                       MPresenceBlockInfo::KInterfaceId );
    if( object )
        {
        CPresenceBlockInfoImp* blockInfo = 
            TXIMPGetImpClassOrPanic< CPresenceBlockInfoImp >::From( *object );
        
        delete iBlockInfo;
        iBlockInfo = blockInfo;
        }
    else // If adaptation didn't return info, wrap the id inside block info
        {
        delete iBlockInfo;
        iBlockInfo = NULL;
        iBlockInfo = CPresenceBlockInfoImp::NewLC( *iIdentity, KNullDesC );
        CleanupStack::Pop(); // iBlockinfo
        }        

    if ( iStatusObj->ResultCode() == KErrNone )
        {
        iMyHost->PresenceDataAccess().AuthorizationDataAccess().HandlePresenceBlockedL( iBlockInfo );
        iBlockInfo = NULL;
        }

    SynthesiseEventL();
    }


// ---------------------------------------------------------------------------
// COperationBlockPresenceForPresentity::Type()
// ---------------------------------------------------------------------------
//
TInt
    COperationBlockPresenceForPresentity::Type() const
    {
    return NPresenceOps::EPrBlockPresenceForPresentity;
    }

// ---------------------------------------------------------------------------
// COperationBlockPresenceForPresentity::SynthesiseEventL()
// ---------------------------------------------------------------------------
//
void COperationBlockPresenceForPresentity::SynthesiseEventL()
    {
    }



// End of file

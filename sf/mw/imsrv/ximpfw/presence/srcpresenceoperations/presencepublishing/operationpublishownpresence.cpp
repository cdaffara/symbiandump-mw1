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
* Description:  implementation of COperationPublishOwnPresence
 *
*/


#include <ximpprotocolconnection.h>
#include <protocolpresencepublishing.h>
#include <protocolpresencefeatures.h>

#include "operationpublishownpresence.h"
#include "presenceinfoimp.h"
#include "ximpobjecthelpers.h"
#include "ximphost.h"
#include "ximpstatusimp.h"

#include "ximptrace.h"

// ============================ MEMBER FUNCTIONS =============================

// ---------------------------------------------------------------------------
// COperationPublishOwnPresence::~COperationPublishOwnPresence()
// ---------------------------------------------------------------------------
//
COperationPublishOwnPresence::~COperationPublishOwnPresence()
    {
    delete iPresInfo;
    }


// ---------------------------------------------------------------------------
// COperationPublishOwnPresence::COperationPublishOwnPresence()
// ---------------------------------------------------------------------------
//
EXPORT_C COperationPublishOwnPresence::COperationPublishOwnPresence()
    {
    }


// ---------------------------------------------------------------------------
// COperationPublishOwnPresence::ConstructL()
// ---------------------------------------------------------------------------
//
void COperationPublishOwnPresence::ConstructL( const TDesC8& aParamPck )
    {
    iPresInfo = CPresenceInfoImp::NewL();
    TXIMPObjectPacker< CPresenceInfoImp >::UnPackL( *iPresInfo,
            aParamPck );
    }


// ---------------------------------------------------------------------------
// COperationPublishOwnPresence::ProcessL()
// ---------------------------------------------------------------------------
//
void COperationPublishOwnPresence::ProcessL()
    {
    TRACE(_L("COperationPublishOwnPresence::ProcessL()" ) );
    CXIMPOperationBase::ProcessL();
    
    MProtocolPresencePublishing& publishing = iMyHost->GetConnection().ProtocolPresenceFeatures().PresencePublishing();
    publishing.DoPublishOwnPresenceL( *iPresInfo, iReqId );
    }


// ---------------------------------------------------------------------------
// COperationPublishOwnPresence::RequestCompleted()
// ---------------------------------------------------------------------------
//
void COperationPublishOwnPresence::RequestCompletedL()
    {
    TRACE(_L("COperationPublishOwnPresence::RequestCompletedL()" ) );
    CXIMPOperationBase::RequestCompletedL();
    
    // no special handling for error situations
    }


// ---------------------------------------------------------------------------
// COperationPublishOwnPresence::Type()
// ---------------------------------------------------------------------------
//
TInt COperationPublishOwnPresence::Type() const
    {
    return NPresenceOps::EPrPublishOwnPresence;
    }

// End of file

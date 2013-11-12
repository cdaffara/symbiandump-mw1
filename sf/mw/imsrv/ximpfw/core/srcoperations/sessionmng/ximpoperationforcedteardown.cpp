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
* Description:  Forced teardown operation
 *
*/


#include "ximpoperationforcedteardown.h"
#include "ximphost.h"

#include "ximptrace.h"
// ============================ MEMBER FUNCTIONS =============================

// ---------------------------------------------------------------------------
// CXIMPOperationForcedTeardown::~CXIMPOperationForcedTeardown()
// ---------------------------------------------------------------------------
//
CXIMPOperationForcedTeardown::~CXIMPOperationForcedTeardown()
    {
    }

// ---------------------------------------------------------------------------
// CXIMPOperationForcedTeardown::CXIMPOperationForcedTeardown()
// ---------------------------------------------------------------------------
//
EXPORT_C CXIMPOperationForcedTeardown::CXIMPOperationForcedTeardown()
    {
    }

// ---------------------------------------------------------------------------
// CXIMPOperationForcedTeardown::ConstructL()
// ---------------------------------------------------------------------------
//
void CXIMPOperationForcedTeardown::ConstructL( const TDesC8& /* aParamPck */ )
    {
    // ignored. can't give parameters to this operation.
    }

// ---------------------------------------------------------------------------
// CXIMPOperationForcedTeardown::SetHost
// ---------------------------------------------------------------------------
//
void CXIMPOperationForcedTeardown::SetHost( MXIMPHost& aMyHost )
    {
    CXIMPOperationBase::SetHost( aMyHost );
    }

// ---------------------------------------------------------------------------
// CXIMPOperationForcedTeardown::ProcessL()
// ---------------------------------------------------------------------------
//
void CXIMPOperationForcedTeardown::ProcessL()
    {
    TRACE(_L("CXIMPOperationForcedTeardown::ProcessL()" ) );
    CXIMPOperationBase::ProcessL();

    iMyHost->ForcedTeardownL();
    iMyHost->HandleSessionMgmtRequestCompleted( iReqId, KErrNone );
    }

// ---------------------------------------------------------------------------
// CXIMPOperationForcedTeardown::RequestCompleted()
// ---------------------------------------------------------------------------
//
void CXIMPOperationForcedTeardown::RequestCompletedL()
    {
    TRACE(_L("CXIMPOperationForcedTeardown::RequestCompletedL()" ) );
    CXIMPOperationBase::RequestCompletedL();

    // no special handling for error situations
    }


// ---------------------------------------------------------------------------
// CXIMPOperationForcedTeardown::Type()
// ---------------------------------------------------------------------------
//
TInt CXIMPOperationForcedTeardown::Type() const
    {
    return NXIMPOps::EXIMPForcedTeardown;
    }

// End of file

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
* Description:  Concrete unbind operation
 *
*/


#include <e32base.h>
#include <ximpprotocolconnection.h>

#include "ximpoperationunbind.h"
#include "ximpcontextclientinfoimp.h"
#include "ximpcontextstateeventimp.h"
#include "ximpcontextstateimp.h"
#include "ximphost.h"
#include "ximppsccontextimp.h"

#include "ximptrace.h"


// ======== LOCAL FUNCTIONS ========

// ---------------------------------------------------------------------------
// ?description
// ---------------------------------------------------------------------------
//

// ======== MEMBER FUNCTIONS ========

// ---------------------------------------------------------------------------
// CXIMPOperationUnbind::CXIMPOperationUnbind()
// ---------------------------------------------------------------------------
//
EXPORT_C CXIMPOperationUnbind::CXIMPOperationUnbind()
    {
    }

// ---------------------------------------------------------------------------
// CXIMPOperationUnbind::ConstructL
// ---------------------------------------------------------------------------
//
void CXIMPOperationUnbind::ConstructL( const TDesC8& /* aParamPck */ )
    {
    iTempEvent = CXIMPContextStateEventImp::NewL();
    iTempEvent->ContextStateImp().SetStateValue( MXIMPContextState::EInactive );
    }

// ---------------------------------------------------------------------------
// CXIMPOperationUnbind::~CXIMPOperationUnbind()
// ---------------------------------------------------------------------------
//
CXIMPOperationUnbind::~CXIMPOperationUnbind()
    {
    delete iTempEvent;
    }

// ---------------------------------------------------------------------------
// CXIMPOperationUnbind::ProcessL()
// ---------------------------------------------------------------------------
//
void CXIMPOperationUnbind::ProcessL()
    {
    TRACE(_L("CXIMPOperationUnbind::ProcessL()" ) );
    CXIMPOperationBase::ProcessL();
    MXIMPProtocolConnection& connection = iMyHost->GetConnection();

    iTempEvent->ContextStateImp().SetStateValue( MXIMPContextState::EUnbinding );
    iMyHost->ActivateEventL( *iTempEvent, iContext );

    // normal case, call plugin
    connection.CloseSession( *iContext->ClientInfo(), iReqId );
    }

// ---------------------------------------------------------------------------
// CXIMPOperationUnbind::RequestCompleted()
// ---------------------------------------------------------------------------
//
void CXIMPOperationUnbind::RequestCompletedL()
    {
    TRACE(_L("CXIMPOperationUnbind::RequestCompletedL()" ) );
    CXIMPOperationBase::RequestCompletedL();
    
    iTempEvent->ContextStateImp().SetStateValue( MXIMPContextState::EInactive );
    iMyHost->ActivateEventL( *iTempEvent, iContext );    
    }

// ---------------------------------------------------------------------------
// CXIMPOperationUnbind::Type()
// ---------------------------------------------------------------------------
//
TInt CXIMPOperationUnbind::Type() const
    {
    return NXIMPOps::EXIMPUnbindContext;
    }

// -----------------------------------------------------------------------------
// CXIMPOperationBase::SetResultCode()
// -----------------------------------------------------------------------------
//
void CXIMPOperationUnbind::SetResultCode( TInt /*aResultCode*/ )
    {
    // Succeeds always.
    CXIMPOperationBase::SetResultCode( KErrNone );
    }
    
// -----------------------------------------------------------------------------
// CXIMPOperationBase::SetResultCode()
// -----------------------------------------------------------------------------
//
void CXIMPOperationUnbind::SetStatus( MXIMPStatus* aStatus )
    {
    // Succeeds always.
    CXIMPOperationBase::SetStatus( aStatus );
    CXIMPOperationBase::SetResultCode( KErrNone );
    }
    
// -----------------------------------------------------------------------------
// CXIMPOperationBase::EndOperation()
// -----------------------------------------------------------------------------
//
void CXIMPOperationUnbind::EndOperation()
    {
    CXIMPOperationBase::EndOperation();
    iMyHost->UnregisterSession( iContext );
    }

// End of file

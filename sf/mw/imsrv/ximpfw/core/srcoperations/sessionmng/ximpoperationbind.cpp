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
* Description:  Concrete bind operation
 *
*/

#include <ximpprotocolconnection.h>

#include "ximpoperationbind.h"
#include "ximpserviceinfoimp.h"
#include "ximpcontextclientinfoimp.h"
#include "ximpobjecthelpers.h"
#include "ximpcontextstateeventimp.h"
#include "ximpcontextstateimp.h"
#include "ximppsccontextimp.h"
#include "ximphost.h"
#include "ximpstatusimp.h"

#include "ximptrace.h"
// ======== MEMBER FUNCTIONS ========

// ---------------------------------------------------------------------------
// CXIMPOperationBind::CXIMPOperationBind()
// ---------------------------------------------------------------------------
//
EXPORT_C CXIMPOperationBind::CXIMPOperationBind()
    {
    }


// ---------------------------------------------------------------------------
// CXIMPOperationBind::ConstructL
// ---------------------------------------------------------------------------
//
void CXIMPOperationBind::ConstructL( const TDesC8& aParamPck )
    {
    if( iContext->IsHostAssigned() )
        {
        User::Leave( KErrAlreadyExists );
        }
    CXIMPContextClientInfoImp* clientInfo = CXIMPContextClientInfoImp::NewLC();
    CXIMPServiceInfoImp* serviceInfo = CXIMPServiceInfoImp::NewLC();

        {
        RDesReadStream rs;
        CleanupClosePushL( rs );
        rs.Open( aParamPck ); // CSI: 65 #

	    serviceInfo->InternalizeL( rs );
	    clientInfo->InternalizeL( rs );

        CleanupStack::PopAndDestroy(); // rs
        }


    CleanupStack::Pop( serviceInfo );
    iContext->SetServiceInfo( serviceInfo );

    CleanupStack::Pop( clientInfo );
    iContext->SetClientInfo( clientInfo );
    
    iTempEvent = CXIMPContextStateEventImp::NewL();
    iTempEvent->ContextStateImp().SetStateValue( MXIMPContextState::EInactive );
    }

// ---------------------------------------------------------------------------
// CXIMPOperationBind::~CXIMPOperationBind()
// ---------------------------------------------------------------------------
//
CXIMPOperationBind::~CXIMPOperationBind()
    {
    delete iTempEvent;
    }

// ---------------------------------------------------------------------------
// CXIMPOperationBind::ProcessL()
// ---------------------------------------------------------------------------
//
void CXIMPOperationBind::ProcessL()
    { 
    TRACE(_L("CXIMPOperationBind::ProcessL()" ) );   
    CXIMPOperationBase::ProcessL();

    iTempEvent->ContextStateImp().SetStateValue( MXIMPContextState::EBinding );
    iMyHost->ActivateEventL( *iTempEvent, iContext );

    MXIMPProtocolConnection& connection = iMyHost->GetConnection();
    connection.OpenSessionL( *iContext->ClientInfo(), iReqId );
    }

// ---------------------------------------------------------------------------
// CXIMPOperationBind::RequestCompleted()
// ---------------------------------------------------------------------------
//
void CXIMPOperationBind::RequestCompletedL()
    {
    TRACE(_L("CXIMPOperationBind::RequestCompletedL()" ) );
    CXIMPOperationBase::RequestCompletedL();
    
    if( iStatusObj->ResultCode() != KErrNone )
        {
        // Remove unbind, because bind did not succeed. this
        // can be done only when attempting to connect for the first time
        iContext->DestroyCachedUnbind();
        iTempEvent->ContextStateImp().SetStateValue( 
            MXIMPContextState::EInactive );
    
        iMyHost->UnregisterSession( iContext );
        }
    else
        {
        iTempEvent->ContextStateImp().SetStateValue( MXIMPContextState::EActive );
        }    
    iMyHost->ActivateEventL( *iTempEvent, iContext );
    }

// ---------------------------------------------------------------------------
// CXIMPOperationBind::Type()
// ---------------------------------------------------------------------------
//
TInt CXIMPOperationBind::Type() const
    {            
    return NXIMPOps::EXIMPBindContext;
    }

// -----------------------------------------------------------------------------
// CXIMPOperationBind::BeginOperationL
// -----------------------------------------------------------------------------
//
void CXIMPOperationBind::BeginOperation()
    {
    }
    
// -----------------------------------------------------------------------------
// CXIMPOperationBind::EndOperationL
// -----------------------------------------------------------------------------
//
void CXIMPOperationBind::EndOperation()
    {    
    }
// End of file

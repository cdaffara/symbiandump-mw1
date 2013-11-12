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

#include "ximpoperationbind2.h"
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
// CXIMPOperationBind2::CXIMPOperationBind2()
// ---------------------------------------------------------------------------
//
EXPORT_C CXIMPOperationBind2::CXIMPOperationBind2()
    {
    }


// ---------------------------------------------------------------------------
// CXIMPOperationBind2::ConstructL
// ---------------------------------------------------------------------------
//
void CXIMPOperationBind2::ConstructL( const TDesC8& aParamPck )
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
// CXIMPOperationBind2::~CXIMPOperationBind2()
// ---------------------------------------------------------------------------
//
CXIMPOperationBind2::~CXIMPOperationBind2()
    {
    delete iTempEvent;
    }

// ---------------------------------------------------------------------------
// CXIMPOperationBind2::ProcessL()
// ---------------------------------------------------------------------------
//
void CXIMPOperationBind2::ProcessL()
    { 
    TRACE(_L("CXIMPOperationBind2::ProcessL()" ) );   
    CXIMPOperationBase::ProcessL();

    iTempEvent->ContextStateImp().SetStateValue( MXIMPContextState::EBinding );
    iMyHost->ActivateEventL( *iTempEvent, iContext );

    TInt32 settingsId = iContext->ServiceInfo()->IapId();
    MXIMPProtocolConnection& connection = iMyHost->GetConnection();
    connection.OpenSessionL( settingsId, iReqId );
    }

// ---------------------------------------------------------------------------
// CXIMPOperationBind2::RequestCompleted()
// ---------------------------------------------------------------------------
//
void CXIMPOperationBind2::RequestCompletedL()
    {
    TRACE(_L("CXIMPOperationBind2::RequestCompletedL()" ) );
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
// CXIMPOperationBind2::Type()
// ---------------------------------------------------------------------------
//
TInt CXIMPOperationBind2::Type() const
    {            
    return NXIMPOps::EXIMPBindContext2;
    }

// -----------------------------------------------------------------------------
// CXIMPOperationBind2::BeginOperationL
// -----------------------------------------------------------------------------
//
void CXIMPOperationBind2::BeginOperation()
    {
    }
    
// -----------------------------------------------------------------------------
// CXIMPOperationBind2::EndOperationL
// -----------------------------------------------------------------------------
//
void CXIMPOperationBind2::EndOperation()
    {    
    }
// End of file

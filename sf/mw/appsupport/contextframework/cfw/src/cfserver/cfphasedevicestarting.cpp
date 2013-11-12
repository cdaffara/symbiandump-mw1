/*
* Copyright (c) 2006-2008 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  CFPhaseDeviceStarting class implementation.
*
*/


#include "cfphasedevicestarting.h"
#include "CFContextObjectImpl.h"
#include "cfcontextinterface.h"
#include "cfwcontextdef.h"
#include "cftrace.h"

CCFPhaseDeviceStarting* CCFPhaseDeviceStarting::NewL(
    MCFContextInterface& aCF )
    {
    FUNC_LOG;
    
    CCFPhaseDeviceStarting* self = CCFPhaseDeviceStarting::NewLC( aCF );
    CleanupStack::Pop( self );
    
    return self;
    }

CCFPhaseDeviceStarting* CCFPhaseDeviceStarting::NewLC(
    MCFContextInterface& aCF )
    {
    FUNC_LOG;
    
    CCFPhaseDeviceStarting* self = new( ELeave ) CCFPhaseDeviceStarting( aCF );
    CleanupStack::PushL( self );
    self->ConstructL();
    
    return self;
    }

CCFPhaseDeviceStarting::~CCFPhaseDeviceStarting()
    {
    FUNC_LOG;
    }

void CCFPhaseDeviceStarting::ConstructL()
    {
    FUNC_LOG;
    }

CCFPhaseDeviceStarting::CCFPhaseDeviceStarting(
    MCFContextInterface& aCF ):
    CCFPhaseBase( CCFPhaseBase::ECFDeviceStarting, aCF )
    {
    FUNC_LOG;
    }

// METHODS

//-----------------------------------------------------------------------------
// CCFPhaseDeviceStarting::ExecuteL
//-----------------------------------------------------------------------------
//
void CCFPhaseDeviceStarting::ExecuteL( TRequestStatus* aStatus )
    {
    FUNC_LOG;
    
    // Publish context that Context Framework is initializing
    CCFContextObject* context = CCFContextObject::NewLC( KCfwSource,
        KCfwSourceStatus,
        TPtrC( KCfwSourceStatusValues[EStatusInitializing] ) );
    RThread thread;
    TInt err = iCF.PublishContext( *context, thread );
    ERROR_3( err, "Failed to publish context: [%S: %S: %S]",
         &context->Source(),
         &context->Type(),
         &context->Value() );
    
    // Clean up
    thread.Close();
    CleanupStack::PopAndDestroy( context );
    
    // Complete phase immediately
    iStarterRequest = aStatus;
    Complete( KErrNone );
    }

//-----------------------------------------------------------------------------
// CCFPhaseDeviceStarting::NextPhase
//-----------------------------------------------------------------------------
//
CCFPhaseBase::TCFPhaseId CCFPhaseDeviceStarting::NextPhase() const
    {
    FUNC_LOG;
    
    return CCFPhaseBase::ECFDeviceStarted;
    }

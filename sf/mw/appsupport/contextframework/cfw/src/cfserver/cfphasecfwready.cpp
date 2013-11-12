/*
* Copyright (c) 2007-2008 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  CCFPhaseCfwReady class implementation.
*
*/


#include "cfphasecfwready.h"
#include "CFContextObjectImpl.h"
#include "cfcontextinterface.h"
#include "cfwcontextdef.h"
#include "cftrace.h"

CCFPhaseCfwReady* CCFPhaseCfwReady::NewL( MCFContextInterface& aCF )
    {
    FUNC_LOG;
    
    CCFPhaseCfwReady* self = CCFPhaseCfwReady::NewLC( aCF );
    CleanupStack::Pop( self );
    
    return self;
    }

CCFPhaseCfwReady* CCFPhaseCfwReady::NewLC( MCFContextInterface& aCF )
    {
    FUNC_LOG;
    
    CCFPhaseCfwReady* self = new( ELeave ) CCFPhaseCfwReady( aCF );
    CleanupStack::PushL( self );
    self->ConstructL();
    
    return self;
    }

CCFPhaseCfwReady::~CCFPhaseCfwReady()
    {
    FUNC_LOG;
    }

void CCFPhaseCfwReady::ConstructL()
    {
    FUNC_LOG;

    // Nothing to do    
    }

CCFPhaseCfwReady::CCFPhaseCfwReady( MCFContextInterface& aCF ):
    CCFPhaseBase( CCFPhaseBase::ECFCfwReady, aCF )
    {
    FUNC_LOG;
    }

// METHODS

//-----------------------------------------------------------------------------
// CCFPhaseCfwReady::ExecuteL
//-----------------------------------------------------------------------------
//
void CCFPhaseCfwReady::ExecuteL( TRequestStatus* aStatus )
    {
    FUNC_LOG;
    
    // Publish context that Context Framework is ready
    CCFContextObject* context = CCFContextObject::NewLC( KCfwSource,
        KCfwSourceStatus,
        TPtrC( KCfwSourceStatusValues[EStatusReady] ) );
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
// CCFPhaseCfwReady::NextPhase
//-----------------------------------------------------------------------------
//
CCFPhaseBase::TCFPhaseId CCFPhaseCfwReady::NextPhase() const
    {
    FUNC_LOG;
    
    // This ends CF starting sequence
    return CCFPhaseBase::ECFStartEnd;
    }

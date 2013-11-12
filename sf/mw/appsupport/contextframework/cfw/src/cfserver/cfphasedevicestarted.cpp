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
* Description:  CFPhaseDeviceStarted class implementation.
*
*/


#include <startupdomainpskeys.h>
#ifndef RD_STARTUP_CHANGE
#include <SysStartup.h>
#endif

#include "cfphasedevicestarted.h"
#include "cfpropertylistener.h"
#include "cftrace.h"

CCFPhaseDeviceStarted* CCFPhaseDeviceStarted::NewL(
    MCFContextInterface& aCF )
    {
    FUNC_LOG;
    
    CCFPhaseDeviceStarted* self = CCFPhaseDeviceStarted::NewLC( aCF );
    CleanupStack::Pop( self );
    
    return self;
    }

CCFPhaseDeviceStarted* CCFPhaseDeviceStarted::NewLC(
    MCFContextInterface& aCF )
    {
    FUNC_LOG;
    
    CCFPhaseDeviceStarted* self = new( ELeave ) CCFPhaseDeviceStarted( aCF );
    CleanupStack::PushL( self );
    self->ConstructL();
    
    return self;
    }

CCFPhaseDeviceStarted::~CCFPhaseDeviceStarted()
    {
    FUNC_LOG;
    
    delete iPropertyListener;
    }

void CCFPhaseDeviceStarted::ConstructL()
    {
    FUNC_LOG;
    
    iPropertyListener = CCFPropertyListener::NewL(
        KPSUidStartup,
        KPSGlobalSystemState,
        *this );
    }

CCFPhaseDeviceStarted::CCFPhaseDeviceStarted(
    MCFContextInterface& aCF ):
    CCFPhaseBase( CCFPhaseBase::ECFDeviceStarted, aCF )
    {
    FUNC_LOG;
    }

// METHODS

//-----------------------------------------------------------------------------
// CCFPhaseDeviceStarted::ExecuteL
//-----------------------------------------------------------------------------
//
void CCFPhaseDeviceStarted::ExecuteL( TRequestStatus* aStatus )
    {
    FUNC_LOG;
    
    // Store starter request
    iStarterRequest = aStatus;
    
    // Double check if we already have been started
    TInt value = 0;
    TInt err = iPropertyListener->Property().Get( value );
    if( err == KErrNone )
        {
        iStarterState = value;
        }
        
    DoComplete();
    }

//-----------------------------------------------------------------------------
// CCFPhaseDeviceStarted::NextPhase
//-----------------------------------------------------------------------------
//
CCFPhaseBase::TCFPhaseId CCFPhaseDeviceStarted::NextPhase() const
    {
    FUNC_LOG;
    
    // This ends CF starting sequence
    return CCFPhaseBase::ECFLoadingPlugins;
    }

//-----------------------------------------------------------------------------
// CCFPhaseDeviceStarted::HandlePropertyChangedL
//-----------------------------------------------------------------------------
//
void CCFPhaseDeviceStarted::HandlePropertyChangedL()
    {
    FUNC_LOG;
    
    TInt value = 0;
    TInt err = iPropertyListener->Property().Get( value );
    if( err == KErrNone )
        {
        iStarterState = value;
        
        INFO_1( "P&S key KPSUidStartup: KPSGlobalSystemState changed. New value: %d",
            iStarterState );
        
        DoComplete();
        }
    }

//-----------------------------------------------------------------------------
// CCFPhaseDeviceStarted::DoComplete
//-----------------------------------------------------------------------------
//
void CCFPhaseDeviceStarted::DoComplete()
    {
    FUNC_LOG;
    
    #ifdef RD_STARTUP_CHANGE
    if( iStarterState == ESwStateNormalRfOn ||
        iStarterState == ESwStateNormalRfOff )
    #else
    if( iStarterState == ESWStateNormal)
    #endif
        {
        INFO( "Starter in normal state... Continue initialization" );
        
        // Cancel system state API events
        iPropertyListener->Cancel();
        
        // Device started, complete phase
        if( iStarterRequest )
            {
            Complete( KErrNone );
            }
        }
    }

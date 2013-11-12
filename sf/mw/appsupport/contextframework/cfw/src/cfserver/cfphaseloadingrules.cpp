/*
* Copyright (c) 2008 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  CFPhaseLoadingRules class implementation.
*
*/


#include "cfphaseloadingrules.h"
#include "CFContextObjectImpl.h"
#include "cfcontextinterface.h"
#include "cfwcontextdef.h"
#include "cftrace.h"

// ======== MEMBER FUNCTIONS ========

// ---------------------------------------------------------------------------
// CCFPhaseLoadingRules::NewL
// ---------------------------------------------------------------------------
//
CCFPhaseLoadingRules* CCFPhaseLoadingRules::NewL(
    MCFContextInterface& aCF )
    {
    FUNC_LOG;
    
    CCFPhaseLoadingRules* self = CCFPhaseLoadingRules::NewLC( aCF );
    CleanupStack::Pop( self );
    
    return self;
    }

// ---------------------------------------------------------------------------
// CCFPhaseLoadingRules::NewLC
// ---------------------------------------------------------------------------
//
CCFPhaseLoadingRules* CCFPhaseLoadingRules::NewLC(
    MCFContextInterface& aCF )
    {
    FUNC_LOG;
    
    CCFPhaseLoadingRules* self = new( ELeave ) CCFPhaseLoadingRules( aCF );
    CleanupStack::PushL( self );
    self->ConstructL();
    
    return self;
    }

// ---------------------------------------------------------------------------
// CCFPhaseLoadingRules::~CCFPhaseLoadingRules
// ---------------------------------------------------------------------------
//
CCFPhaseLoadingRules::~CCFPhaseLoadingRules()
    {
    FUNC_LOG;
    }

// ---------------------------------------------------------------------------
// CCFPhaseLoadingRules::ConstructL
// ---------------------------------------------------------------------------
//
void CCFPhaseLoadingRules::ConstructL()
    {
    FUNC_LOG;
    }

// ---------------------------------------------------------------------------
// CCFPhaseLoadingRules::CCFPhaseLoadingRules
// ---------------------------------------------------------------------------
//
CCFPhaseLoadingRules::CCFPhaseLoadingRules(
    MCFContextInterface& aCF ):
    CCFPhaseBase( CCFPhaseBase::ECFLoadingPlugins, aCF )
    {
    FUNC_LOG;
    }

//-----------------------------------------------------------------------------
// CCFPhaseLoadingRules::ExecuteL
//-----------------------------------------------------------------------------
//
void CCFPhaseLoadingRules::ExecuteL( TRequestStatus* aStatus )
    {
    FUNC_LOG;

    // Store request
    iStarterRequest = aStatus;    
    }

//-----------------------------------------------------------------------------
// CCFPhaseLoadingRules::NextPhase
//-----------------------------------------------------------------------------
//
CCFPhaseBase::TCFPhaseId CCFPhaseLoadingRules::NextPhase() const
    {
    FUNC_LOG;
    
    return CCFPhaseBase::ECFCfwReady;
    }
    
//-----------------------------------------------------------------------------
// CCFPhaseLoadingRules::HandleEvent
//-----------------------------------------------------------------------------
//
void CCFPhaseLoadingRules::HandleEvent( MCFStarterEventHandler::TCFStarterEvents aEvent )
    {
    FUNC_LOG;
       
    if( aEvent == MCFStarterEventHandler::EContextSourcePluginsLoaded )
        {
    	  iContextSourcePluginsLoaded = ETrue;
        }
    else if(aEvent == MCFStarterEventHandler::EActionPluginsLoaded)
        {
    	  iActionPluginsLoaded = ETrue;
        }

    // Complete when context source and action plugins are loaded
    if( iContextSourcePluginsLoaded && iActionPluginsLoaded )
        {
        Complete( KErrNone );
        }    
    }    

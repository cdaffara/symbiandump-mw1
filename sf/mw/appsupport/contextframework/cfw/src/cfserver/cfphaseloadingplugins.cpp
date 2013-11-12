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
* Description:  CFPhaseLoadingPlugins class implementation.
*
*/


#include "cfphaseloadingplugins.h"
#include "CFContextObjectImpl.h"
#include "cfcontextinterface.h"
#include "cfwcontextdef.h"
#include "cftrace.h"


// ======== MEMBER FUNCTIONS ========

// ---------------------------------------------------------------------------
// CCFPhaseLoadingPlugins::NewL
// ---------------------------------------------------------------------------
//
CCFPhaseLoadingPlugins* CCFPhaseLoadingPlugins::NewL(
    MCFContextInterface& aCF )
    {
    FUNC_LOG;
    
    CCFPhaseLoadingPlugins* self = CCFPhaseLoadingPlugins::NewLC( aCF );
    CleanupStack::Pop( self );
    
    return self;
    }

// ---------------------------------------------------------------------------
// CCFPhaseLoadingPlugins::NewLC
// ---------------------------------------------------------------------------
//
CCFPhaseLoadingPlugins* CCFPhaseLoadingPlugins::NewLC(
    MCFContextInterface& aCF )
    {
    FUNC_LOG;
    
    CCFPhaseLoadingPlugins* self = new( ELeave ) CCFPhaseLoadingPlugins( aCF );
    CleanupStack::PushL( self );
    self->ConstructL();
    
    return self;
    }

// ---------------------------------------------------------------------------
// CCFPhaseLoadingPlugins::~CCFPhaseLoadingPlugins
// ---------------------------------------------------------------------------
//
CCFPhaseLoadingPlugins::~CCFPhaseLoadingPlugins()
    {
    FUNC_LOG;
    }

// ---------------------------------------------------------------------------
// CCFPhaseLoadingPlugins::ConstructL
// ---------------------------------------------------------------------------
//
void CCFPhaseLoadingPlugins::ConstructL()
    {
    FUNC_LOG;
    }

// ---------------------------------------------------------------------------
// CCFPhaseLoadingPlugins::CCFPhaseLoadingPlugins
// ---------------------------------------------------------------------------
//
CCFPhaseLoadingPlugins::CCFPhaseLoadingPlugins(
    MCFContextInterface& aCF ):
    CCFPhaseBase( CCFPhaseBase::ECFLoadingPlugins, aCF )
    {
    FUNC_LOG;
    }

//-----------------------------------------------------------------------------
// CCFPhaseLoadingPlugins::ExecuteL
//-----------------------------------------------------------------------------
//
void CCFPhaseLoadingPlugins::ExecuteL( TRequestStatus* aStatus )
    {
    FUNC_LOG;

    // Complete phase immediately
    iStarterRequest = aStatus;
    Complete( KErrNone );
    }

//-----------------------------------------------------------------------------
// CCFPhaseLoadingPlugins::NextPhase
//-----------------------------------------------------------------------------
//
CCFPhaseBase::TCFPhaseId CCFPhaseLoadingPlugins::NextPhase() const
    {
    FUNC_LOG;
    
    return CCFPhaseBase::ECFLoadingRules;
    }

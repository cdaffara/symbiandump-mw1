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
* Description:  CCFRemoveSubscriptionCleanupCmd implementation.
*
*/


// SYSTEM INCLUDES

// USER INCLUDES
#include "cfremovesubscriptioncleanupcmd.h"
#include "CFContextManager.h"
#include "cftrace.h"

// ======== MEMBER FUNCTIONS ========

// ---------------------------------------------------------------------------
// C++ constructor.
// ---------------------------------------------------------------------------
//
CCFRemoveSubscriptionCleanupCmd::CCFRemoveSubscriptionCleanupCmd(
    CCFContextManager& aContextManager )
    :   iContextManager( aContextManager )
    {
    FUNC_LOG;
    }

// ---------------------------------------------------------------------------
// Symbian two phased constructor.
// ---------------------------------------------------------------------------
//
CCFRemoveSubscriptionCleanupCmd* CCFRemoveSubscriptionCleanupCmd::NewL(
    CCFContextManager& aContextManager )
    {
    FUNC_LOG;

    CCFRemoveSubscriptionCleanupCmd* self
        = CCFRemoveSubscriptionCleanupCmd::NewLC( aContextManager );
    CleanupStack::Pop( self );
    return self;
    }

// ---------------------------------------------------------------------------
// Symbian two phased constructor.
// Leaves pointer in the cleanup stack.
// ---------------------------------------------------------------------------
//
CCFRemoveSubscriptionCleanupCmd* CCFRemoveSubscriptionCleanupCmd::NewLC(
    CCFContextManager& aContextManager )
    {
    FUNC_LOG;

    CCFRemoveSubscriptionCleanupCmd* self
        = new( ELeave ) CCFRemoveSubscriptionCleanupCmd( aContextManager );
    CleanupStack::PushL( self );
    return self;
    }

// ---------------------------------------------------------------------------
// C++ destructor.
// ---------------------------------------------------------------------------
//
CCFRemoveSubscriptionCleanupCmd::~CCFRemoveSubscriptionCleanupCmd( )
    {
    FUNC_LOG;
    }


// ---------------------------------------------------------------------------
// CCFRemoveSubscriptionCleanupCmd::ExecuteL
// ---------------------------------------------------------------------------
//
void CCFRemoveSubscriptionCleanupCmd::ExecuteL()
    {
    FUNC_LOG;

    INFO( "CCFRemoveSubscriptionCleanupCmd::ExecuteL - Cleaning removed subscriptions." );

    iContextManager.CleanupCacheElementSubscriptions();
    }

// End of file

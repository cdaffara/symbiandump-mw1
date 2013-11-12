/*
* Copyright (c) 2008-2008 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  CCFServiceBase class implementation.
 *
*/


// SYSTEM INCLUDES

// USER INCLUDES
#include "cfservicebase.h"
#include "cftrace.h"

// ======== MEMBER FUNCTIONS ========

// ---------------------------------------------------------------------------
// C++ constructor.
// ---------------------------------------------------------------------------
//
CCFServiceBase::CCFServiceBase( TCFServiceId aId,
    RCFClientSession& aSession,
    MCFListener& aListener )
    :   iSession( aSession ),
        iListener( aListener ),
        iId( aId )
    {
    FUNC_LOG;
    }

// ---------------------------------------------------------------------------
// C++ destructor.
// ---------------------------------------------------------------------------
//
CCFServiceBase::~CCFServiceBase( )
    {
    FUNC_LOG;

    // Nothing to do.
    }

// ---------------------------------------------------------------------------
// CCFServiceBase::Id
// ---------------------------------------------------------------------------
//
CCFServiceBase::TCFServiceId CCFServiceBase::Id( ) const
    {
    FUNC_LOG;
    
    return iId;
    }

// End of file

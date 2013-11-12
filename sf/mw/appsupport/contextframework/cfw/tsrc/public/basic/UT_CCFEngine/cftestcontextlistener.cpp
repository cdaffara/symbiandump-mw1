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
* Description:  CCFTestContextListener implementation.
*
*/


// SYSTEM INCLUDES

// USER INCLUDES
#include "cftestcontextlistener.h"
#include "CFContextIndication.h"

// ======== MEMBER FUNCTIONS ========

// ---------------------------------------------------------------------------
// C++ constructor.
// ---------------------------------------------------------------------------
//
CCFTestContextListener::CCFTestContextListener()
    {
    }

// ---------------------------------------------------------------------------
// Symbian 2nd phase constructor.
// ---------------------------------------------------------------------------
//
void CCFTestContextListener::ConstructL()
    {
    }

// ---------------------------------------------------------------------------
// Symbian two phased constructor.
// ---------------------------------------------------------------------------
//
CCFTestContextListener* CCFTestContextListener::NewL()
    {
    CCFTestContextListener* self = CCFTestContextListener::NewLC();
    CleanupStack::Pop( self );
    return self;
    }

// ---------------------------------------------------------------------------
// Symbian two phased constructor.
// Leaves pointer in the cleanup stack.
// ---------------------------------------------------------------------------
//
CCFTestContextListener* CCFTestContextListener::NewLC()
    {
    CCFTestContextListener* self = new( ELeave ) CCFTestContextListener;
    CleanupStack::PushL( self );
    self->ConstructL();
    return self;
    }

// ---------------------------------------------------------------------------
// C++ destructor.
// ---------------------------------------------------------------------------
//
CCFTestContextListener::~CCFTestContextListener()
    {
    delete iLastIndication;
    }


// ---------------------------------------------------------------------------
// CCFTestContextListener::ContextIndicationL
// ---------------------------------------------------------------------------
//
void CCFTestContextListener::ContextIndicationL(
    CCFContextIndication* aIndication )
    {
    if ( iLastIndication )
        {
        delete iLastIndication;
        iLastIndication = NULL;
        }
    iLastIndication = aIndication;
    }

// ---------------------------------------------------------------------------
// CCFTestContextListener::Client
// ---------------------------------------------------------------------------
//
TInt CCFTestContextListener::Client( RThread& aThread ) const
    {
    aThread = RThread();
    return KErrNone;
    }
        
// ---------------------------------------------------------------------------
// CCFTestContextListener::HandleContextSubscriptionError
// ---------------------------------------------------------------------------
//
void CCFTestContextListener::HandleContextSubscriptionError( TInt /*aError*/,
    const TDesC& /*aSource*/,
    const TDesC& /*aType*/ )
    {
    iSubscriptionError = ETrue;
    }

// End of file

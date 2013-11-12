/*
* Copyright (c) 2002 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description: 
*		Implementation of CRfsCompleter class, which is used to inform the server
*	that the Rfs operation has ended.
*
*
*/


// INCLUDE FILES

#include <e32std.h>

#include "rfsCompleter.h"
#include "rfsServer.h"
#include "rfsSession.h"


// ================= MEMBER FUNCTIONS ==========================================

// C++ default constructor can't leave
CRfsCompleter::CRfsCompleter( CRfsSession* aSession ) :
    CActive( CActive::EPriorityStandard ), iSession( aSession ) {}

// Destructor
CRfsCompleter::~CRfsCompleter()
    {
    }

// ----------------------------------------------------------------------------
// CRfsCompleter::NewL()
// ----------------------------------------------------------------------------

CRfsCompleter* CRfsCompleter::NewL( CRfsSession* aSession )
    {
    CRfsCompleter* self = new( ELeave ) CRfsCompleter( aSession );
    CActiveScheduler::Add( self );
    return self;
    }

// ----------------------------------------------------------------------------
// CRfsCompleter::RunL()
// ----------------------------------------------------------------------------

void CRfsCompleter::RunL()
    {
    iFlags |= KRfsCompletionHandled;

    if ( iSession->RequestHandled() )
        {
        iSession->CompleteMessage();
        }
    }

// ----------------------------------------------------------------------------
// CRfsCompleter::DoCancel()
// ----------------------------------------------------------------------------

void CRfsCompleter::DoCancel()
    {
    if ( !(iFlags & KRfsCompletionSent) )
        {
        TRequestStatus* status = &iStatus;
        User::RequestComplete( status, KErrCancel );
        }
    }

// ----------------------------------------------------------------------------
// CRfsCompleter::Activate()
// ----------------------------------------------------------------------------

void CRfsCompleter::Start()
    {
    iFlags = 0;
    iReturn = 0;
    iStatus = KRequestPending;
    SetActive();
    }

// ----------------------------------------------------------------------------
// CRfsCompleter::Complete()
// ----------------------------------------------------------------------------

void CRfsCompleter::Complete( TInt aReturn )
    {
    if ( !(iFlags & KRfsCompletionSent) )
        {
        iFlags |= KRfsCompletionSent;
        iReturn = aReturn;
        TRequestStatus* status = &iStatus;
        User::RequestComplete( status, KErrNone );
        }
    }

// End of File  

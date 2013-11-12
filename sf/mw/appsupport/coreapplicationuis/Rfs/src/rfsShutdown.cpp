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
*		Implements the class which will take care of shutting down the server.
*
*
*/


// INCLUDE FILES

#include <e32std.h>

#include "rfsShutdown.h"

// ================= MEMBER FUNCTIONS ==========================================

// C++ default constructor can't leave
CRfsShutdown::CRfsShutdown( TInt aPriority ) :
    CActive( aPriority ) {}

// Destructor
CRfsShutdown::~CRfsShutdown()
    {
    iTimer.Close();
    }

// -----------------------------------------------------------------------------
// CRfsShutdown::New()
// -----------------------------------------------------------------------------

CRfsShutdown* CRfsShutdown::NewL( TInt aPriority )
    {
    CRfsShutdown* timer = new( ELeave ) CRfsShutdown( aPriority );

    CleanupStack::PushL(timer);
    User::LeaveIfError( timer->iTimer.CreateLocal() );
    CActiveScheduler::Add( timer );
    CleanupStack::Pop(timer);

    return timer;
    }

// -----------------------------------------------------------------------------
// CRfsShutdown::RunL()
// -----------------------------------------------------------------------------

void CRfsShutdown::RunL()
    {
    CActiveScheduler::Stop();   // shuts down the server
    }

// -----------------------------------------------------------------------------
// CRfsShutdown::DoCancel()
// -----------------------------------------------------------------------------

void CRfsShutdown::DoCancel()
    {
    iTimer.Cancel();
    }

// -----------------------------------------------------------------------------
// CRfsShutdown::Start()
// -----------------------------------------------------------------------------

void CRfsShutdown::Start( TInt aTimeout )
    {
    iTimer.After( iStatus, aTimeout );
    SetActive();
    }

//  End of File  

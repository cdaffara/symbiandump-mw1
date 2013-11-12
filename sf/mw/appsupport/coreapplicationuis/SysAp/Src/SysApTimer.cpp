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
* Description:  CSysApTimer implementation.
*
*/


// INCLUDES

//#include <avkon.hrh>
//#include <avkon.rsg>
#include "SysAp.hrh"
#include "SysApTimer.h"



// ============================ MEMBER FUNCTIONS ==============================

// ----------------------------------------------------------------------------
// CSysApTimer::CSysApTimer()
// ----------------------------------------------------------------------------

CSysApTimer::CSysApTimer
    (
     MSysApTimerResponder& aSysApTimerResponder
     ) : CActive( EPriorityStandard ), iTimerResponder( aSysApTimerResponder ) 
    {
    CActiveScheduler::Add( this );
    }

// ----------------------------------------------------------------------------
// CSysApTimer::~CSysApTimer() 
// ----------------------------------------------------------------------------

CSysApTimer::~CSysApTimer() 
    {
    Cancel();
    }


// ----------------------------------------------------------------------------
// CSysApTimer::DoCancel() 
// ----------------------------------------------------------------------------

void CSysApTimer::DoCancel() 
    {
    iTimer.Close();
    }

// ----------------------------------------------------------------------------
// CSysApTimer::ActivateTimerL()
// ----------------------------------------------------------------------------

void CSysApTimer::ActivateTimerL( TInt aTimeoutInSeconds )
    {
    ActivateMsTimerL( 1000 * aTimeoutInSeconds );
    }

// ----------------------------------------------------------------------------
// CSysApTimer::ActivateMsTimerL()
// ----------------------------------------------------------------------------
    
void CSysApTimer::ActivateMsTimerL( TInt aTimeoutInMilliSeconds )
    {
    if ( IsActive() )
        {
        Cancel();
        }
    User::LeaveIfError( iTimer.CreateLocal() );
    iTimer.After( iStatus, 1000 * aTimeoutInMilliSeconds );
    SetActive();
    }

// ----------------------------------------------------------------------------
// CSysApTimer::RunL() 
// ----------------------------------------------------------------------------

void CSysApTimer::RunL() 
    {
    iTimer.Close();
    iTimerResponder.TimerExpiredL();
    }

// ----------------------------------------------------------------------------
// CSysApTimer::RunError()
// ----------------------------------------------------------------------------

TInt CSysApTimer::RunError( TInt /* aError */ )
    {
    return KErrNone;            
    }

// End of File


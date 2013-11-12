/*
* Copyright (c) 2006 Nokia Corporation and/or its subsidiary(-ies).
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
*
*/

#include "waitactive.h"

// ======== MEMBER FUNCTIONS ========

	
void CWaitActive::ConstructL()
    {
    CTimer::ConstructL();
    iActiveScheduler = new ( ELeave ) CActiveSchedulerWait();
    }


CWaitActive* CWaitActive::NewL()
    {
    CWaitActive* self = new( ELeave ) CWaitActive;
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop();
    return self;
    }

// ----------------------------------------------------------------------------
// CIsvTelCallAppTelephony::CIsvTelCallAppTelephony()
// Default constructor.
// ----------------------------------------------------------------------------
//
CWaitActive::CWaitActive()
    :CTimer( EPriorityStandard )
    { 
    CActiveScheduler::Add( this );
    }

// ---------------------------------------------------------------------------
// destructor
// ---------------------------------------------------------------------------
CWaitActive::~CWaitActive()
	{
    Cancel();
    delete iActiveScheduler;
	}

// ---------------------------------------------------------------------------
// destructor
// ---------------------------------------------------------------------------
void CWaitActive::Wait(TTimeIntervalMicroSeconds32 anInterval)
	{
	After(anInterval);
	iActiveScheduler->Start();
	
	}



// ----------------------------------------------------------------------------
// void CIsvTelCallAppTelephony::RunL()
// Handles the active object’s request completion event.
// ----------------------------------------------------------------------------
//
void CWaitActive::CWaitActive::RunL()
    {
	// CActiveScheduler::Stop();
    iActiveScheduler->AsyncStop();

    }


		








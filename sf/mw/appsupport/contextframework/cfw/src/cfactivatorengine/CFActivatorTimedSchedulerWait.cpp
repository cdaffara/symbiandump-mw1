/*
* Copyright (c) 2006-2006 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  CCFActivatorTimedSchedulerWait class implementation.
*
*/



#include "CFActivatorTimedSchedulerWait.h"
#include "cftrace.h"

// CONSTANTS

_LIT( KPanicCat, "ActTimSche" );

enum TPanicReason
    {
    EAlreadyStarted
    };
    
LOCAL_C void Panic( TInt aCode )
    {
    User::Panic( KPanicCat, aCode );
    }

// MEMBER FUNCTIONS

CCFActivatorTimedSchedulerWait* CCFActivatorTimedSchedulerWait::NewL()
    {
    FUNC_LOG;    

    CCFActivatorTimedSchedulerWait* self =
        CCFActivatorTimedSchedulerWait::NewLC();
    CleanupStack::Pop( self );

    return self;
    }
  
CCFActivatorTimedSchedulerWait* CCFActivatorTimedSchedulerWait::NewLC()
   {
    FUNC_LOG;    

    CCFActivatorTimedSchedulerWait* self =
        new( ELeave ) CCFActivatorTimedSchedulerWait;
    CleanupStack::PushL( self );
    self->ConstructL();

    return self;
    }
  
CCFActivatorTimedSchedulerWait::CCFActivatorTimedSchedulerWait():
    CTimer( EPriorityStandard )
    {
    FUNC_LOG;    

    // Double check if base class adds active object into scheduler
    if( !IsAdded() )
        {
        CActiveScheduler::Add( this );
        }
    }

void CCFActivatorTimedSchedulerWait::ConstructL()
    {
    FUNC_LOG;    

    // Do base constructions
    CTimer::ConstructL();
    
    // Initialize active scheduler wait
    iWait = new( ELeave ) CActiveSchedulerWait;
    }

// Destructor
CCFActivatorTimedSchedulerWait::~CCFActivatorTimedSchedulerWait()
    {
    FUNC_LOG;    

    Cancel();
    delete iWait;
    }

// METHODS

//-----------------------------------------------------------------------------
// CCFActivatorTimedSchedulerWait::Start
//-----------------------------------------------------------------------------
//
void CCFActivatorTimedSchedulerWait::Start(
    const TTimeIntervalMicroSeconds32& aInterval )
    {
    FUNC_LOG;    

    __ASSERT_ALWAYS( !IsActive(), Panic( EAlreadyStarted ) );
    
    After( aInterval );
    iWait->Start();
    }

//-----------------------------------------------------------------------------
// CCFActivatorTimedSchedulerWait::Stop
//-----------------------------------------------------------------------------
//
void CCFActivatorTimedSchedulerWait::Stop()
    {
    FUNC_LOG;    

    Cancel();
    }

//-----------------------------------------------------------------------------
// CCFActivatorTimedSchedulerWait::RunL
//-----------------------------------------------------------------------------
//
void CCFActivatorTimedSchedulerWait::RunL()
    {
    FUNC_LOG;    

    // Double check that wait really started
    if( iWait->IsStarted() )
        {
        iWait->AsyncStop();
        }
    }

//-----------------------------------------------------------------------------
// CCFActivatorTimedSchedulerWait::DoCancel
//-----------------------------------------------------------------------------
//
void CCFActivatorTimedSchedulerWait::DoCancel()
    {
    FUNC_LOG;    

    // Double check that wait really started
    CTimer::DoCancel();
    if( iWait->IsStarted() )
        {
        iWait->AsyncStop();
        }
    }

/*
* Copyright (c) 2007-2007 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  CCFDelay class implementation.
*
*/



// INCLUDE FILES
#include "cfdelay.h"
#include "cfbasicoptrace.h"


// ======== MEMBER FUNCTIONS ========

//-----------------------------------------------------------------------------
// CCFDelay::CCFDelay
// Delayed evaluation should be as exact as possible. Set high priority.
//-----------------------------------------------------------------------------
//
CCFDelay::CCFDelay( MCFDelayExpired& aDelayer )
    :   CTimer( EPriorityHigh ),
        iDelayer( aDelayer )
    {
    FUNC_LOG;
    }

//-----------------------------------------------------------------------------
// CCFDelay::ConstructL
//-----------------------------------------------------------------------------
//
void CCFDelay::ConstructL()
    {
    FUNC_LOG;

    // Construct ctimer
    CTimer::ConstructL();

    // Add into active scheduler if not already added
    if( !IsAdded() )
        {
        CActiveScheduler::Add( this );
        }
    }

//-----------------------------------------------------------------------------
// CCFDelay::NewL
//-----------------------------------------------------------------------------
//
CCFDelay* CCFDelay::NewL( MCFDelayExpired& aDelayer )
    {
    FUNC_LOG;

    CCFDelay* self = CCFDelay::NewLC( aDelayer );
    CleanupStack::Pop( self );
    return self;
    }

//-----------------------------------------------------------------------------
// CCFDelay::NewLC
//-----------------------------------------------------------------------------
//
CCFDelay* CCFDelay::NewLC( MCFDelayExpired& aDelayer )
    {
    FUNC_LOG;

    CCFDelay* self = new( ELeave ) CCFDelay( aDelayer );
    CleanupStack::PushL( self );
    self->ConstructL();
    return self;
    }

// Destructor
CCFDelay::~CCFDelay()
    {
    FUNC_LOG;

    Cancel();
    }


//-----------------------------------------------------------------------------
// CCFDelay::Delay
//-----------------------------------------------------------------------------
//
void CCFDelay::Delay( const TTimeIntervalMicroSeconds32& aDelay )
    {
    FUNC_LOG;

    TBool startDelay = ETrue;
    if ( IsActive() )
        {
        TTime currentTime;
        currentTime.HomeTime();
        TInt64 elapsedTime = currentTime.Int64() - iDelayStartTime.Int64();
        if ( aDelay.Int() < ( iDelay - elapsedTime ) )
            {
            startDelay = EFalse; // Current delay outlasts the new one
            }
        }

    if ( startDelay )
        {
        Cancel(); // Cancel ongoing timer if any

        iDelay = aDelay.Int();
        iDelayStartTime.HomeTime();
        After( aDelay ); // Start delay
        }
    }

//-----------------------------------------------------------------------------
// CCFDelay::RunL
//-----------------------------------------------------------------------------
//
void CCFDelay::RunL()
    {
    FUNC_LOG;

    iDelayer.ExpiredL();
    }

//-----------------------------------------------------------------------------
// CCFDelay::RunError
//-----------------------------------------------------------------------------
//
#ifndef ERROR_TRACE
TInt CCFDelay::RunError( TInt /*aError*/ )
#else // ERROR_TRACE defined
TInt CCFDelay::RunError( TInt aError )
#endif
    {
    FUNC_LOG;

    ERROR( aError, "Call MCFDelayExpired::ExpiredL() leave code: %d (skipped)" );

    return KErrNone; // Ignore errors
    }

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
* Description:  CVibraAction class implementation.
*
*/


#include "vibraaction.h"

#include <cfactionindication.h>

#include "vibratrace.h"
#include "vibraactionobserver.h"

// CONSTANTS

const TInt KMultiplier = 1000;

// MEMBER FUNCTIONS

CVibraAction* CVibraAction::NewL( MVibraActionObserver& aObserver )
    {
    FUNC_LOG;
    
    CVibraAction* self = CVibraAction::NewLC( aObserver );
    CleanupStack::Pop( self );
    
    return self;
    }
  
CVibraAction* CVibraAction::NewLC( MVibraActionObserver& aObserver )
    {
    FUNC_LOG;
    
    CVibraAction* self = new( ELeave ) CVibraAction( aObserver );
    CleanupStack::PushL( self );
    self->ConstructL();
    
    return self;
    }
  
// Destructor
CVibraAction::~CVibraAction()
    {
    FUNC_LOG;
    
    Cancel();
    delete iVibra;
    }

// Vibra feedback should be as exact as possible. Set high priority.
CVibraAction::CVibraAction( MVibraActionObserver& aObserver ):
    CTimer( EPriorityHigh ),
    iObserver( aObserver )
    {
    FUNC_LOG;
    
    // Add into active scheduler if not already added
    if( !IsAdded() )
        {
        CActiveScheduler::Add( this );
        }
    }
    
void CVibraAction::ConstructL()
    {
    FUNC_LOG;
    
    // Construct ctimer
    CTimer::ConstructL();
    
    // Connect to vibra client
    iVibra = CHWRMVibra::NewL( this );
    }

// METHODS

//-----------------------------------------------------------------------------
// CVibraAction::StartVibraL
//-----------------------------------------------------------------------------
//
CCFActionPlugIn::TExecutionTime CVibraAction::StartVibraL(
    const TVibraActionInfo& aVibraInfo )
    {
    FUNC_LOG;
    
    CCFActionPlugIn::TExecutionTime time = CCFActionPlugIn::ENone;
    
    INFO( "Vibra started with params:");
    INFO_1( ">> Delay: %d", aVibraInfo.iDelay );
    INFO_1( ">> Duration: %d", aVibraInfo.iDuration );
    INFO_1( ">> Intensity: %d", aVibraInfo.iIntensity );
    INFO_1( ">> Repeats: %d", aVibraInfo.iRepeats );
    INFO_1( ">> Repeat interval: %d", aVibraInfo.iRepeatInterval );
    
    // First cancel ongoing timer if any
    Cancel();
    
    // Store parameters
    iRepeats = 0;
    iInfo = aVibraInfo;
    if( iInfo.iDelay )
        {
        // We have to delay vibra
        After( MicroSeconds( iInfo.iDelay ) );
        time = CCFActionPlugIn::ELong;
        }
    else
        {
        // No delay, start right away
        time = DoRunVibraL();
        }
        
    return time;
    }

//-----------------------------------------------------------------------------
// CVibraAction::StopVibra
//-----------------------------------------------------------------------------
//
void CVibraAction::StopVibraL()
    {
    FUNC_LOG;
    
    // First cancel ongoing timer
    Cancel();
    
    // Stop vibra
    DoStopVibraL();
    
    INFO( "Vibra stopped");
    }

//-----------------------------------------------------------------------------
// CVibraAction::RunL
//-----------------------------------------------------------------------------
//
void CVibraAction::RunL()
    {
    FUNC_LOG;
    
    if( DoRunVibraL() == CCFActionPlugIn::ENone )
        {
        // All repeats executed, finnish vibra action
        iObserver.VibraActionCompletedL();
        }
    }

// ---------------------------------------------------------------------------
// CVibraAction::RunError
// ---------------------------------------------------------------------------
//
TInt CVibraAction::RunError( TInt aError )
    {
    FUNC_LOG;
    
    INFO_1( "CVibraAction::RunError: aError == %d", aError );

    // Avoid Panic in CActiveScheduler
    aError = KErrNone;

    return aError;
    }


//-----------------------------------------------------------------------------
// CVibraAction::DoCancel
//-----------------------------------------------------------------------------
//
void CVibraAction::DoCancel()
    {
    FUNC_LOG;
    
    // Stop vibra just in case
    TRAP_IGNORE( DoStopVibraL() );
    }

//-----------------------------------------------------------------------------
// CVibraAction::VibraModeChanged
//-----------------------------------------------------------------------------
//
void CVibraAction::VibraModeChanged( CHWRMVibra::TVibraModeState aStatus )
    {
    FUNC_LOG;
    
    INFO_1( "Old vibra mode: %d", iVibraModeState );
    iVibraModeState = aStatus;
    INFO_1( "New vibra mode: %d", iVibraModeState );
    }

//-----------------------------------------------------------------------------
// CVibraAction::VibraStatusChanged
//-----------------------------------------------------------------------------
//
void CVibraAction::VibraStatusChanged(
    CHWRMVibra::TVibraStatus /*aStatus*/ )
    {
    FUNC_LOG;
    
    // Nothing to do
    }

//-----------------------------------------------------------------------------
// CVibraAction::DoRunVibraL
//-----------------------------------------------------------------------------
//
CCFActionPlugIn::TExecutionTime CVibraAction::DoRunVibraL()
    {
    FUNC_LOG;

    CCFActionPlugIn::TExecutionTime time = CCFActionPlugIn::ENone;
    iVibra->StartVibraL( iInfo.iDuration, iInfo.iIntensity );
    iRepeats++;
    
    INFO_1( "Vibra activated: times = %d", iRepeats );
    
    if( iRepeats < iInfo.iRepeats )
        {
        // We have repeats
        After( MicroSeconds( iInfo.iRepeatInterval ) );
        time = CCFActionPlugIn::ELong;
        }
        
    return time;
    }

//-----------------------------------------------------------------------------
// CVibraAction::DoStopVibraL
//-----------------------------------------------------------------------------
//
void CVibraAction::DoStopVibraL()
    {
    FUNC_LOG;

    iVibra->StopVibraL();
    iRepeats = 0;
    }

//-----------------------------------------------------------------------------
// CVibraAction::MicroSeconds
//-----------------------------------------------------------------------------
//
TTimeIntervalMicroSeconds32 CVibraAction::MicroSeconds(
    TInt aMilliSeconds ) const
    {
    FUNC_LOG;
    
    return TTimeIntervalMicroSeconds32( aMilliSeconds * KMultiplier );
    }

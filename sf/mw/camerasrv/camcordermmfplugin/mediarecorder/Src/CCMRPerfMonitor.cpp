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
* Description:  Performance monitor timer
*
*/



// INCLUDES
#include "CCMRPerfMonitor.h"
#include "CCMRActiveOutput.h"
#include "CCMRVideoRecorderClient.h"
#include "CCMRVideoSettings.h"
#include "CCMRVideoHWParams.h"
#include "CCMRMediaRecorder.h"

// CONSTANTS
const TInt KCMRPerfMonitorFreq = 2000000;   // timer timeout value, 2 seconds

// MACROS
#define PMASSERT(x) __ASSERT_DEBUG(x, User::Panic(_L("CCMRPERFMONITOR"), EInternalAssertionFailure))

// Debug print macro
#ifdef _DEBUG
#include <e32svr.h>
#define PRINT(x) RDebug::Print x;
#else
#define PRINT(x)
#endif


// -----------------------------------------------------------------------------
// CCMRPerfMonitor::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CCMRPerfMonitor* CCMRPerfMonitor::NewL(
            CCMRActiveOutput* aAudioOutput,
            CCMRActiveOutput* aVideoOutput,
            CCMRVideoRecorderClient* aVideoRecorder,
            CCMRMediaRecorder* aMediaRecorder)
    {
    PRINT((_L("CCMRPerfMonitor::NewL() in ")));

    CCMRPerfMonitor* self = new (ELeave) CCMRPerfMonitor();
    CleanupStack::PushL( self );
    self->ConstructL(aAudioOutput, aVideoOutput, aVideoRecorder, aMediaRecorder);
    CleanupStack::Pop();

    PRINT((_L("CCMRPerfMonitor::NewL() out ")));
    return self;
    }

// destructor
CCMRPerfMonitor::~CCMRPerfMonitor()
    {
    // Make sure we're cancelled
	Cancel();
    }


// -----------------------------------------------------------------------------
// CCMRPerfMonitor::ConstructL
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
void CCMRPerfMonitor::ConstructL(
            CCMRActiveOutput* aAudioOutput,
            CCMRActiveOutput* aVideoOutput,
            CCMRVideoRecorderClient* aVideoRecorder,
            CCMRMediaRecorder* aMediaRecorder)
    {
    PRINT((_L("CCMRPerfMonitor::ConstructL() in ")));

    iAudioOutput = aAudioOutput;
    iVideoOutput = aVideoOutput;
    iVideoRecorder = aVideoRecorder;
    iMediaRecorder = aMediaRecorder;
    iTimedMonitoringRunning = EFalse;
    iClockSourceStarted = EFalse;
    iPerfMonPaused = EFalse;
    iStartTime = 0;
    iTimeStampWhenPaused = 0;

	CTimer::ConstructL();
    CActiveScheduler::Add( this );

    PRINT((_L("CCMRPerfMonitor::ConstructL() out ")));
    }

// -----------------------------------------------------------------------------
// CCMRPerfMonitor::StartL
// Start the timed monitor
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CCMRPerfMonitor::StartL()
    {
    PRINT((_L("CCMRPerfMonitor::Start() in")));

    if ( iMediaRecorder->AudioEnabledL() )
        {
        PRINT((_L("CCMRPerfMonitor::Start() Audio available")));
        // can't use this without audio
        PMASSERT( iAudioOutput );

        if ( !iTimedMonitoringRunning )
            {
            iTimedMonitoringRunning = ETrue;
            After( 2*KCMRPerfMonitorFreq );  // give some time to settle
            }
        }
    else
        {
        PRINT((_L("CCMRPerfMonitor::Start() No Audio, using system clock with offset: %d "), I64INT(iClockSourcePauseOffset.Int64() )));
        iClockSourceStarted = ETrue;
        iStartTime.UniversalTime();
        }
    iPerfMonPaused = EFalse;
    }

// -----------------------------------------------------------------------------
// CCMRPerfMonitor::StopL
// Stop the timed monitor
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CCMRPerfMonitor::StopL()
    {
    PRINT((_L("CCMRPerfMonitor::Stop() in ")));
    Cancel();
    iAdjustedTimeMs = 0;
    iTimeStampWhenPaused = 0;
    iStartTime = 0;
    iClockSourceStarted = EFalse;
    iPerfMonPaused = EFalse;
    PRINT((_L("CCMRPerfMonitor::Stop() out ")));
    }

// -----------------------------------------------------------------------------
// CCMRPerfMonitor::PauseL
// Stop the timed monitor, but don't reset variables
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CCMRPerfMonitor::PauseL()
    {
    PRINT((_L("CCMRPerfMonitor::Pause() in ")));
    Cancel();
    if ( iClockSourceStarted )
        {
        TTime current;
        current.UniversalTime();
        iTimeStampWhenPaused = iTimeStampWhenPaused.Int64() + current.MicroSecondsFrom(iStartTime).Int64();
        PRINT((_L("CCMRPerfMonitor::Pause() ClockSource paused at: %d "), I64INT(iTimeStampWhenPaused.Int64()) ));
        }
    else
        {
        iTimeStampWhenPaused = 0;
        }

	iPerfMonPaused = ETrue;
    PRINT((_L("CCMRPerfMonitor::Pause() in ")));
    }

// -----------------------------------------------------------------------------
// CCMRPerfMonitor::CustomInterface
// Retrieves a custom interface for the clock source.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TAny* CCMRPerfMonitor::CustomInterface(TUid /*aInterface*/)
    {
    return NULL;
    }

// -----------------------------------------------------------------------------
// CCMRPerfMonitor::Time
// Retrieves the current stream time.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TTimeIntervalMicroSeconds CCMRPerfMonitor::Time()
    {
    PRINT((_L("CCMRPerfMonitor::Time() in ")));

    // if timed monitorin is running turn it off as encoder will use clocksource to correct timestamps.
    if ( iTimedMonitoringRunning )
        {
        iTimedMonitoringRunning = EFalse;
        }

    TBool audioEnabled = EFalse;
    TRAPD(audioError, audioEnabled = iMediaRecorder->AudioEnabledL());
    if ( audioError != KErrNone )
        {
        iAudioTime = 0;
        PRINT((_L("CCMRPerfMonitor::Time() getting AudioEnabledL failed with %d"), audioError ));
        return iAudioTime;
        }

    if ( audioEnabled )
        {
        PRINT((_L("CCMRPerfMonitor::Time() Audio available, using it as clocksource") ));
        TRAPD(error, iAudioOutput->LatestTimeStampL( iAudioTime ) );
        if ( error )
            {
            iAudioTime = 0;
            }
        }
    else
        {
        if (iClockSourceStarted)
            {
            if ( iPerfMonPaused == EFalse )
	            {
                PRINT((_L("CCMRPerfMonitor::Time() No audio, using available system ClockSource") ));
    	        TTime current;
        	    current.UniversalTime();
            	iAudioTime = current.MicroSecondsFrom(iStartTime).Int64() + iTimeStampWhenPaused.Int64();
        	    }
        	else
        		{
        		iAudioTime = iTimeStampWhenPaused.Int64();
        		}
            }
        else
            {
            PRINT((_L("CCMRPerfMonitor::Time() No audio, Time() called before start!") ));
            iAudioTime = 0;
            }
        }

    PRINT((_L("CCMRPerfMonitor::Time() out, Time: %d "), I64INT(iAudioTime.Int64()) ));
    return iAudioTime;
    }

// -----------------------------------------------------------------------------
// CCMRPerfMonitor::RunL
// Main routine: check statuses & possibly adjust settings
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CCMRPerfMonitor::RunL()
    {
    PRINT((_L("CCMRPerfMonitor::RunL() in")));
    PRINT((_L("CCMRPerfMonitor::RunL() out")));
    }


// -----------------------------------------------------------------------------
// CCMRPerfMonitor::DoCancel
// Cancels the timer
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CCMRPerfMonitor::DoCancel()
    {
    // cancel the timer
    CTimer::DoCancel();
    }

// -----------------------------------------------------------------------------
// CCMRPerfMonitor::RunError
// Handle leaves from RunL
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt CCMRPerfMonitor::RunError(TInt aError)
    {
    // ignore, but since RunL left, a new timer is not set
    PRINT((_L("CCMRPerfMonitor::RunError() %d"), aError));
    aError++;//to eliminate compiler warning
    return KErrNone;
    }







// End of file

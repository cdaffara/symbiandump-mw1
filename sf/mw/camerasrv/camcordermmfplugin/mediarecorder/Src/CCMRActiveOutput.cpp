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
* Description:  Active object class for writing audio/video to CCMRMediaSink.
*                Forwards buffers from different threads to a single-thread 
*                CCMRMediaSink
*
*/


// INCLUDES
#include "CCMRActiveOutput.h"

// CONSTANTS
const TInt KCMROutputSleepTime = 10000;    // 10 ms time for other AOs to execute
const TInt KCMROutputMaxBufsPerRun = 10;   // Nr of bufs to handle in single AO run

// MACROS
// Debug print macro
#ifdef _DEBUG
#include <e32svr.h>
#define PRINT(x) RDebug::Print x
#else
#define PRINT(x)
#endif


// ============================ MEMBER FUNCTIONS ===============================


// -----------------------------------------------------------------------------
// CCMRActiveOutput::NewL
// Two-phased constructor.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
CCMRActiveOutput* CCMRActiveOutput::NewL(MCMRMediaSink* aSink, MCMRActiveOutputObserver* aObserver)
    {
    CCMRActiveOutput* self = new (ELeave) CCMRActiveOutput;
    CleanupStack::PushL(self);
    self->ConstructL( aSink, aObserver );
    CleanupStack::Pop();
    return self;
    }


// -----------------------------------------------------------------------------
// CCMRActiveOutput::CCMRActiveOutput
// Constructor
// 
// -----------------------------------------------------------------------------
//
CCMRActiveOutput::CCMRActiveOutput()
:		CActive(EPriorityStandard)
		{
		CActiveScheduler::Add( this );
		}


// -----------------------------------------------------------------------------
// CCMRActiveOutput::~CCMRActiveOutput
// Destructor
// 
// -----------------------------------------------------------------------------
//
CCMRActiveOutput::~CCMRActiveOutput()
    {
    PRINT((_L("CCMRActiveOutput::~CCMRActiveOutput() AO %x destructor in"), this));
    Cancel();
    
    iTimer.Close();
    iMutexObj.Close();
    
    iSink = NULL; // to satisfy PC Lint
    iMediaSource = NULL;
    iObserver = NULL;
    PRINT((_L("CCMRActiveOutput::~CCMRActiveOutput() AO %x destructor out"), this));
    }

// -----------------------------------------------------------------------------
// CCMRActiveOutput::ConstructL
// Symbian 2nd phase constructor.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CCMRActiveOutput::ConstructL(MCMRMediaSink* aSink, MCMRActiveOutputObserver* aObserver)
    {
    iSink = aSink;
	iObserver = aObserver;

	User::LeaveIfError( iTimer.CreateLocal() );
	User::LeaveIfError( iMutexObj.CreateLocal() );


    PRINT((_L("CCMRActiveOutput::ConstructL() created AO %x"), this));
    }


// -----------------------------------------------------------------------------
// CCMRActiveOutput::RegisterSourceL
// Register source (audio or video) for this active object
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CCMRActiveOutput::RegisterSourceL(MCMRMediaSource* aSource)
    {
    PRINT((_L("CCMRActiveOutput::RegisterSourceL() - START - this: 0x%08x, iStatus %d, IsActive: %d, iRunning: %d"), this, iStatus.Int(), IsActive(), iRunning ));
	Cancel();

    // we can safely change the source
    // save the source object handle
    PRINT((_L("CCMRActiveOutput::RegisterSourceL() AO %x for source %x"), this, aSource));
    iMediaSource = aSource;

    // enter restricted area
    iMutexObj.Wait();
    if ( iRunning && !IsActive() )
        {
        //should issue a request now, since it was not issued in Start (possible if there was no mediasource yet registered in Start)
        
        PRINT((_L("CCMRActiveOutput::RegisterSourceL() AO %x issued a data request"), this));
        RequestNewData();
        }
    // leave restricted area
    iMutexObj.Signal();

    PRINT((_L("CCMRActiveOutput::RegisterSourceL() - END - this: 0x%08x, iStatus %d, IsActive: %d, iRunning: %d"), this, iStatus.Int(), IsActive(), iRunning ));
    }


// -----------------------------------------------------------------------------
// CCMRActiveOutput::Start
// Start output by giving the first request to registered source
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CCMRActiveOutput::Start()
    {
    PRINT((_L("CCMRActiveOutput::Start() AO %x in"), this));
    if ( iMediaSource == NULL )
        {
        // no source registered yet
        PRINT((_L("CCMRActiveOutput::Start() AO %x no source!!"), this));
        return;
        }
    // enter restricted area
    iMutexObj.Wait();
    if ( !IsActive() )
        {
        PRINT((_L("CCMRActiveOutput::Start() AO %x issued a data request"), this));
    	RequestNewData();
        }
    else 
        {
        PRINT((_L("CCMRActiveOutput::Start() AO %x already active!!"), this));
        }
    iRunning = ETrue;

    // leave restricted area
    iMutexObj.Signal();
    PRINT((_L("CCMRActiveOutput::Start() AO %x out"), this));
    }

// -----------------------------------------------------------------------------
// CCMRActiveOutput::Stop
// Stops and flushes buffers
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CCMRActiveOutput::Stop()
    {
    PRINT((_L("CCMRActiveOutput::Stop() AO %x in"), this));
    iRunning = EFalse;
    Cancel();
    Flush();
    PRINT((_L("CCMRActiveOutput::Stop() AO %x out"), this));
    }

// -----------------------------------------------------------------------------
// CCMRActiveOutput::Flush
// Flushed buffers from the queue of the source
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CCMRActiveOutput::Flush()
    {
    PRINT((_L("CCMRActiveOutput::Flush() - START - this: 0x%08x, iState: %d iStatus: %d"), this, iState, iStatus.Int()));

    if ( iMediaSource == NULL )
        {
        // no source registered yet
        return;
        }
        
    if ( iBuffer ) 
        {
        // return the buffer to the source; it may have been reset in Flush
        PRINT((_L("CCMRActiveOutput::Flush() - iBuffer=0x%x"), iBuffer));
        iMediaSource->ReturnBuffer(iBuffer);
        iBuffer = NULL;
        }
    // read all buffers from the source
    CCMRMediaBuffer* buffer;
    TInt err = KErrNone;
    for (;;)
        {
        buffer = iMediaSource->GetNextBuffer();
        if ( buffer == NULL )
            {
            break;
            }

        // write the data to 3GP
        TRAP(err, iSink->WriteBufferL(buffer) )	;
        if ( err != KErrNone )
            {
            // need to flush the buffers anyway
            PRINT((_L("CCMRActiveOutput::Flush() of %x WriteBufferL left with error %d"), this, err));
            }
        else
            {
            PRINT((_L("CCMRActiveOutput::Flush() of %x wrote a buffer %x to external sink"), iMediaSource, buffer));
            }

        // return the buffer to the source
        iMediaSource->ReturnBuffer(buffer);
        buffer = NULL;
        }
        
    PRINT((_L("CCMRActiveOutput::Flush() - END - this: 0x%08x, iState: %d iStatus: %d"), this, iState, iStatus.Int()));
    }


// -----------------------------------------------------------------------------
// CCMRActiveOutput::RunL
// Main function of the active object, writes buffers from source to sink
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CCMRActiveOutput::RunL()
    {
    PRINT((_L("CCMRActiveOutput::RunL() - START - this: 0x%08x, iState: %d iStatus: %d"), this, iState, iStatus.Int()));

    if ( iMediaSource == NULL )
        {
        // no source registered yet
        return;
        }

    // issue a new request right away, this may result in one useless call for this runl if this flushes all the buffers now, but 
    // if the issuing is done after the loop here, there can be new data without completion of the request (the source thread 
    // may pre-emptively get scheduled) which is then left unread
    PRINT((_L("CCMRActiveOutput::RunL() of %x issued a new data request"), this));
    RequestNewData();

    PRINT((_L("CCMRActiveOutput::RunL() of %x started processing old buffers"), this));
    // read all buffers from the source
    TInt i = 0;
    for ( i = 0; i < KCMROutputMaxBufsPerRun; i++ )
        {
        iBuffer = iMediaSource->GetNextBuffer();
        if ( iBuffer == NULL )
            {
            PRINT((_L("CCMRActiveOutput::RunL() of %x got a NULL buffer from %x, breaking the for-loop"), this, iMediaSource));
            break;
            }

        // write the data to 3GP
        iSink->WriteBufferL(iBuffer);

        if ( iBuffer )
            {
            // return the buffer to the source; it may have been reset in Flush
            PRINT((_L("CCMRActiveOutput::RunL() of %x wrote a buffer %x of type %d from source %x to sink"), this, iBuffer, iBuffer->Type(), iMediaSource));
            iMediaSource->ReturnBuffer(iBuffer);
            iBuffer = NULL;
            }

        if ( iStatus == KErrCancel )
            {
            PRINT((_L("CCMRActiveOutput::RunL() of %x canceled while writing buffer, get out"), this));
            return;
            }
        }

    if ( iMediaSource->NumBuffersWaiting() > 0 )
        {
        // break the loop to give other AOs time to execute
        PRINT((_L("CCMRActiveOutput::RunL() of %x goes to asleep for 10 ms, # of buffers still waiting in source %d"), this, iMediaSource->NumBuffersWaiting()));
        
        // must cancel the earlier request before setting timer
        Cancel();
		RequestTimerPause();
        }
    else
        {
        // new request was issued already
        }

    PRINT((_L("CCMRActiveOutput::RunL() - END - this: 0x%08x"), this));
    }


// -----------------------------------------------------------------------------
// CCMRActiveOutput::DoCancel
// 
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CCMRActiveOutput::DoCancel()
    {
	PRINT((_L("CCMRActiveOutput::DoCancel() - START - this: 0x%08x, iState: %d, thread: %d, iStatus: %d, sc: %d"), 
	           this, iState, I64INT(RThread().Id()), iStatus.Int(), RThread().RequestCount() ));

    switch( iState )
		{
		case EStateTimerPause:
			iTimer.Cancel();
			break;
		case EStateRequestingNewData: 
			iMediaSource->RequestNewDataCancel( iStatus );			
			break;
		default:
		case EStateIdle:
			ASSERT( EFalse );
			break;
		}    
  
	iState = EStateIdle;

	PRINT((_L("CCMRActiveOutput::DoCancel() - END - this: 0x%08x, iState: %d, thread: %d, iStatus: %d, sc: %d"), 
	           this, iState, I64INT(RThread().Id()), iStatus.Int(), RThread().RequestCount() ));
    }

// -----------------------------------------------------------------------------
// CCMRActiveOutput::RunError
// Inform the observer about an error occurred in RunL
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt CCMRActiveOutput::RunError(TInt aError)
    {
    PRINT((_L("CCMRActiveOutput::RunError() of %x"), this));
    iObserver->MaooError( aError );

    // return the buffer to the source
    if ( iBuffer )
        {
        iMediaSource->ReturnBuffer(iBuffer);
        iBuffer = NULL;
        }

    return KErrNone;
    }


// -----------------------------------------------------------------------------
// CCMRActiveOutput::NumBuffersInQueueL
// Return the number of buffers waiting in the input of the active object
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt CCMRActiveOutput::NumBuffersInQueueL()
    {
    PRINT((_L("CCMRActiveOutput::NumBuffersInQueueL() of %x"), this));
    if ( !iMediaSource )
        {
        PRINT((_L("CCMRActiveOutput::NumBuffersInQueueL() of %x not ready")));
        User::Leave( KErrNotReady );
        }
    return iMediaSource->NumBuffersWaiting();
    }

// -----------------------------------------------------------------------------
// CCMRActiveOutput::LatestTimeStampL
// Return the latest time stamp from the input stream
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CCMRActiveOutput::LatestTimeStampL(TTimeIntervalMicroSeconds& aTimeStamp) const
    {
    PRINT((_L("CCMRActiveOutput::LatestTimeStampL() of %x"), this));
    if ( !iMediaSource )
        {
        PRINT((_L("CCMRActiveOutput::LatestTimeStampL() of %x not ready")));
        aTimeStamp = 0;
        }
    else
        {
        iMediaSource->LatestTimeStampL( aTimeStamp );
        }
    }

// -----------------------------------------------------------------------------
// CCMRActiveOutput::DurationL
// Return the duration of the recording
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CCMRActiveOutput::DurationL(TTimeIntervalMicroSeconds& aDuration) const
    {
    PRINT((_L("CCMRActiveOutput::DurationL() of %x"), this));
    if ( !iMediaSource )
        {
        PRINT((_L("CCMRActiveOutput::DurationL() of %x not ready")));
        User::Leave( KErrNotReady );
        }
    iMediaSource->DurationL(aDuration);
    }


// -----------------------------------------------------------------------------
// CCMRActiveOutput::SetVideoFrameSize
// Passes video frame size to mediasink
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt CCMRActiveOutput::SetVideoFrameSize(TSize aSize)
    {
    return iSink->SetVideoFrameSize( aSize );
    }

// -----------------------------------------------------------------------------
// CCMRActiveOutput::SetAverageVideoBitRate
// Passes average video bitrate to mediasink
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt CCMRActiveOutput::SetAverageVideoBitRate(TInt aBitRate)
    {
    return iSink->SetAverageVideoBitRate( aBitRate );
    }

// -----------------------------------------------------------------------------
// CCMRActiveOutput::SetMaxVideoBitRate
// Passes maximum video bitrate to mediasink
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt CCMRActiveOutput::SetMaxVideoBitRate(TInt aBitRate)
    {
    return iSink->SetMaxVideoBitRate( aBitRate );
    }

// -----------------------------------------------------------------------------
// CCMRActiveOutput::SetMaxVideoBitRate
// Passes average audio bitrate to mediasink
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt CCMRActiveOutput::SetAverageAudioBitRate(TInt aBitRate)
    {
    return iSink->SetAverageAudioBitRate( aBitRate );
    }


// -----------------------------------------------------------------------------
// CCMRActiveOutput::SetState
// 
// 
// -----------------------------------------------------------------------------
//
void CCMRActiveOutput::SetState( TState aState )
	{
    PRINT((_L("CCMRActiveOutput::SetState() changing state from %d to %d"), iState, aState ));
	iState = aState;
	}

// -----------------------------------------------------------------------------
// CCMRActiveOutput::RequestTimerPause
// 
// 
// -----------------------------------------------------------------------------
//
void CCMRActiveOutput::RequestTimerPause()
	{
	PRINT((_L("CCMRActiveOutput::RequestTimerPause() - IsActive: %d, iState: %d, iStatus: %d"), 
	           IsActive(), iState, iStatus.Int() ));
	ASSERT( !IsActive() );
	
    iTimer.After( iStatus, KCMROutputSleepTime ); // give other active objects a chance to execute and sleep 10 ms
	SetActive();
	
    SetState( CCMRActiveOutput::EStateTimerPause );
	}


// -----------------------------------------------------------------------------
// CCMRActiveOutput::RequestNewData
// 
// 
// -----------------------------------------------------------------------------
//
void CCMRActiveOutput::RequestNewData()
	{
	PRINT((_L("CCMRActiveOutput::RequestNewData() - IsActive: %d, iState: %d, iStatus: %d"), 
	           IsActive(), iState, iStatus.Int() ));
	ASSERT( !IsActive() );
	
    iMediaSource->RequestNewData(iStatus);
    SetActive();

    SetState( CCMRActiveOutput::EStateRequestingNewData );
	}
	


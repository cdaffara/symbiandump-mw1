/*
* Copyright (c) 2003 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Implementation for video recorder client class
*
*/



// INCLUDE FILES
#include "CCMRRecorderBase.h"
#include "CCMRVideoSettings.h"  // default video settings
#include "CCMRSupportedCodecs.h"
#include "CCMRVideoRecorderClient.h"
#include "CCMRVideoCodecs.h"

#include <mmf/common/mmfvideo.h>
#include <badesca.h>

// Assertion macro wrapper for code cleanup
#define VRCASSERT(x) __ASSERT_DEBUG(x, User::Panic(_L("CCMRVIDEORECORDER"), EInternalAssertionFailure)) 

// Debug print macro
#ifdef _DEBUG
#include <e32svr.h>
#define PRINT(x) RDebug::Print x
#else
#define PRINT(x)
#endif

#include "OstTraceDefinitions.h"
#ifdef OST_TRACE_COMPILER_IN_USE
#include "CCMRVideoRecorderClientTraces.h"
#endif


// ================= MEMBER FUNCTIONS =======================

// ---------------------------------------------------------
// CCMRVideoRecorderClient::NewL
// Standard Symbian OS two-phased constructor
// Returns: CCMRVideoRecorderClient*: Pointer to the newly-created 
//                              video recorder
// ---------------------------------------------------------
//
CCMRVideoRecorderClient* CCMRVideoRecorderClient::NewL()
    {

    PRINT((_L("CCMRVideoRecorderClient::NewL()"))); 

    CCMRVideoRecorderClient* self = new (ELeave) CCMRVideoRecorderClient;    
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop();

    return self;    

    }

// ---------------------------------------------------------
// CCMRVideoRecorderClient::ConstructL()
// Symbian OS Constructor
// ---------------------------------------------------------
//
void CCMRVideoRecorderClient::ConstructL()
    {

    PRINT((_L("CCMRVideoRecorderClient::ConstructL()")));    

    iVideoCodecs = CCMRVideoCodecs::NewL();

    SetState(EStateNone);

    }



// ---------------------------------------------------------
// CCMRVideoRecorderClient::~CCMRVideoRecorderClient()
// Destructor
// ---------------------------------------------------------
//
CCMRVideoRecorderClient::~CCMRVideoRecorderClient()
    {
    OstTrace0( CAMERASRV_PERFORMANCE, CCMRVIDEORECORDERCLIENT_CCMRVIDEORECORDERCLIENT, "e_CCMRVideoRecorderClient::~CCMRVideoRecorderClient 1" );

    PRINT((_L("CCMRVideoRecorderClient::~CCMRVideoRecorderClient() in")));

    // This is the counterpart to NewL & OpenL, e.g. Close & Delete

    // free all memory allocated and uninitalize & delete objects created

    delete iThreadEventMonitor;
    iThreadEventMonitor = NULL;
    // close thread
    if ( iThreadCreated )
        {
        iThreadProxy.Close();
        }

    delete iVideoCodecs;
    iVideoCodecs = NULL;

    iObserver = NULL;

    SetState(EStateNone);

    // enable for RTRT code coverage
    //#pragma attol insert _ATCPQ_DUMP(0);
    PRINT((_L("CCMRVideoRecorderClient::~CCMRVideoRecorderClient() out")));
    OstTrace0( CAMERASRV_PERFORMANCE, DUP1_CCMRVIDEORECORDERCLIENT_CCMRVIDEORECORDERCLIENT, "e_CCMRVideoRecorderClient::~CCMRVideoRecorderClient 0" );
    }

// ---------------------------------------------------------
// CCMRVideoRecorderClient::OpenL
// Opens the video recorder instance, creates submodules
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CCMRVideoRecorderClient::OpenL(MCMRVideoRecorderObserver* aObserver, 
                              CCMRActiveOutput* aOutput,
                              TInt aCameraHandle, 
                              const TDesC8& aVideoMimeType,
                              MMMFClockSource* aClockSource,
                              CCMRConfigManager* aConfig  )
    {
    PRINT((_L("CCMRVideoRecorderClient::OpenL()")));

    if ( State() != EStateNone ) 
        {
        PRINT((_L("CCMRVideoRecorderClient::OpenL() already exists")));
        User::Leave( KErrAlreadyExists );
        }

    User::LeaveIfError(iThreadProxy.CreateSubThread(iThreadCreated, aConfig));

    iThreadEventMonitor = CCMRSubThreadEventMonitor::NewL(*this, iThreadProxy); 
    iThreadEventMonitor->Start();

    VRCASSERT( aObserver );
    iObserver = aObserver;

    // set output AO to thread
    VRCASSERT( aOutput );
    User::LeaveIfError(iThreadProxy.SetOutput( aOutput ));
    
    // set clocksource
    User::LeaveIfError(iThreadProxy.SetClockSource( aClockSource));

    // set camera handle to thread
    User::LeaveIfError(iThreadProxy.SetCameraHandle( aCameraHandle ));

    // check that it is supported
    if ( iVideoCodecs->DoWeSupportThisL( aVideoMimeType ) )
        {
        User::LeaveIfError(iThreadProxy.SetCodec( aVideoMimeType ));
        //Event is signaled if state change is needed
        }
    else
        {
        // codec doesn't exist in the system
        PRINT((_L("CCMRVideoRecorderClient::OpenL() unsupported codec")));
        User::Leave( KErrNotSupported );
        }

    SetState(EStateOpen);
    iObserver->MvroStateChange( State() );

    }



// ---------------------------------------------------------
// CCMRVideoRecorderClient::PrepareL
// Prepares the recorder for recording
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CCMRVideoRecorderClient::PrepareL()
    {

    PRINT((_L("CCMRVideoRecorderClient::PrepareL() in")));

    // other state checks handled in the thread, but in this state there is no thread
    VRCASSERT( State() != EStateNone );

    SetState(EStatePreparing);
    // prepare thread
    User::LeaveIfError(iThreadProxy.Prepare());

                              
    PRINT((_L("CCMRVideoRecorderClient::PrepareL() out")));
    }


// ---------------------------------------------------------
// CCMRVideoRecorderClient::RecordL
// Starts recording
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CCMRVideoRecorderClient::RecordL()
    {

    PRINT((_L("CCMRVideoRecorderClient::Record()           ")));
    // other state checks handled in the thread, but in this state there is no thread
    VRCASSERT( State() != EStateNone );

    // start recording
    User::LeaveIfError(iThreadProxy.Record());

    SetState(EStateRecording);
    PRINT((_L("CCMRVideoRecorderClient::Record() complete           ")));
    }


// ---------------------------------------------------------
// CCMRVideoRecorderClient::StopL
// Stops recording
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CCMRVideoRecorderClient::StopL()
    {
    PRINT((_L("CCMRVideoRecorderClient::Stop() in")));
    // other state checks handled in the thread, but in this state there is no thread
    VRCASSERT( State() != EStateNone );
            
    SetState(EStateStopping);
    // Stop thread
    User::LeaveIfError(iThreadProxy.Stop());
    PRINT((_L("CCMRVideoRecorderClient::Stop() out, must wait for state change before stop completed")));
    }

// ---------------------------------------------------------
// CCMRVideoRecorderClient::RequestBuffersAndWaitEOSL
// Waits until video thread stops recording and receives EOS (end of stream) marker
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CCMRVideoRecorderClient::RequestBuffersAndWaitEOSL(TInt& aVideoEOSReached)
    {
    PRINT((_L("CCMRVideoRecorderClient::RequestBuffersAndWaitEOSL() in")));
    // other state checks handled in the thread, but in this state there is no thread
    VRCASSERT( State() != EStateNone );
            
    User::LeaveIfError(iThreadProxy.RequestBuffersAndWaitEOSL(aVideoEOSReached));
    PRINT((_L("CCMRVideoRecorderClient::RequestBuffersAndWaitEOSL() out, stop completed")));
    }


// ---------------------------------------------------------
// CCMRVideoRecorderClient::PauseL
// Pauses recording
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CCMRVideoRecorderClient::PauseL()
    {

    PRINT((_L("CCMRVideoRecorderClient::Pause() in")));
    // other state checks handled in the thread, but in this state there is no thread
    VRCASSERT( State() != EStateNone );

    // pause thread
    User::LeaveIfError(iThreadProxy.Pause());
    
    SetState(EStatePaused);
               
    PRINT((_L("CCMRVideoRecorderClient::Pause() out")));
    }

// ---------------------------------------------------------
// CCMRVideoRecorderClient::ResumeL
// Resumes recording
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CCMRVideoRecorderClient::ResumeL()
    {

    PRINT((_L("CCMRVideoRecorderClient::Resume() in")));
    // other state checks handled in the thread, but in this state there is no thread
    VRCASSERT( State() != EStateNone );

    // resume thread
    User::LeaveIfError(iThreadProxy.Resume());

    SetState(EStateRecording);
    PRINT((_L("CCMRVideoRecorderClient::Resume() out")));

    }



// Setters & Getters

// -----------------------------------------------------------------------------
// CCMRVideoRecorderClient::SetVideoCodecL
// Set video codec.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CCMRVideoRecorderClient::SetVideoCodecL(const TDesC8& aMimeType) const
    {
    // other state checks handled in the thread, but in this state there is no thread
    VRCASSERT( State() != EStateNone );

    // check that it is supported
    if ( iVideoCodecs->DoWeSupportThisL( aMimeType ) )
        {
        User::LeaveIfError(iThreadProxy.SetCodec( aMimeType ));
        //Event is signaled if state change is needed
        }
    else
        {
        // codec doesn't exist in the system
        PRINT((_L("CCMRVideoRecorderClient::OpenL() unsupported codec")));
        User::Leave( KErrNotSupported );
        }
    }

// -----------------------------------------------------------------------------
// CCMRVideoRecorderClient::GetVideoCodecL
// Get the used video codec.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CCMRVideoRecorderClient::GetVideoCodecL( TDes8& aVideoMimeType ) const
    {
    // other state checks handled in the thread, but in this state there is no thread
    VRCASSERT( State() != EStateNone );

    User::LeaveIfError(iThreadProxy.GetCodec( aVideoMimeType ));
    }


// -----------------------------------------------------------------------------
// CCMRVideoRecorderClient::GetSupportedVideoCodecsL
// Get list of supported video codecs.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CCMRVideoRecorderClient::GetSupportedVideoCodecsL( CDesC8Array& aVideoMimeTypes ) const
    {
    if ( iVideoCodecs )
        {
        iVideoCodecs->GetSupportedVideoCodecsL( aVideoMimeTypes );
        }
    }

// ---------------------------------------------------------
// CCMRVideoRecorderClient::SetTargetBitRateL
// Sets new target bitrate
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CCMRVideoRecorderClient::SetTargetBitRateL(TInt aBitRate) 
    {
    PRINT((_L("CCMRVideoRecorderClient::SetTargetBitRate, aBitrate = % d"),aBitRate));

    // other state checks handled in the thread, but in this state there is no thread
    VRCASSERT( State() != EStateNone );

    User::LeaveIfError(iThreadProxy.SetBitRate( aBitRate ));

    }

// ---------------------------------------------------------
// CCMRVideoRecorderClient::TargetBitrateL
// Gets current target bitrate
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CCMRVideoRecorderClient::TargetBitRateL(TInt& aBitRate)
    {
    PRINT((_L("CCMRVideoRecorderClient::TargetBitRate")));

    // other state checks handled in the thread, but in this state there is no thread
    VRCASSERT( State() != EStateNone );

    User::LeaveIfError(iThreadProxy.GetBitRate(aBitRate));
    }

// ---------------------------------------------------------
// CCMRVideoRecorderClient::SetFrameSizeL
// Sets new input & output frame size
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CCMRVideoRecorderClient::SetFrameSizeL(const TSize& aSize) const
    {

    PRINT((_L("CCMRVideoRecorderClient::SetFrameSize")));
    // other state checks handled in the thread, but in this state there is no thread
    VRCASSERT( State() != EStateNone );
    

    User::LeaveIfError(iThreadProxy.SetFrameSize( aSize ));
    //Event is signaled if state change is needed

    }



// ---------------------------------------------------------
// CCMRVideoRecorderClient::FrameSizeL
// Gets current frame size
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CCMRVideoRecorderClient::FrameSizeL(TSize& aSize) const
    {
    PRINT((_L("CCMRVideoRecorderClient::FrameSizeL()")));
    // other state checks handled in the thread, but in this state there is no thread
    VRCASSERT( State() != EStateNone );
                
    User::LeaveIfError(iThreadProxy.GetFrameSize( aSize )); 
                
    }


// ---------------------------------------------------------
// CCMRVideoRecorderClient::SetFrameRateL
// Sets new target frame rate
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CCMRVideoRecorderClient::SetFrameRateL(TReal32 aFrameRate) const
    {

    PRINT((_L("CCMRVideoRecorderClient::SetFrameRate")));
    // other state checks handled in the thread, but in this state there is no thread
    VRCASSERT( State() != EStateNone );


    User::LeaveIfError(iThreadProxy.SetFrameRate( aFrameRate ));

    //Event is signaled if state change is needed
    }

// ---------------------------------------------------------
// CCMRVideoRecorderClient::FrameRateL
// Get the used encoding frame rate
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CCMRVideoRecorderClient::FrameRateL(TReal32& aFrameRate) const
    {
    PRINT((_L("CCMRVideoRecorderClient::FrameRateL()")));
    // other state checks handled in the thread, but in this state there is no thread
    VRCASSERT( State() != EStateNone );
                
    User::LeaveIfError(iThreadProxy.GetFrameRate(aFrameRate));
    }

// ---------------------------------------------------------
// CCMRVideoRecorderClient::SetVideoCodingOptionsL
// Set misc video coding options
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CCMRVideoRecorderClient::SetVideoCodingOptionsL(const TCCMRVideoCodingOptions& aOptions) const
    {
    PRINT((_L("CCMRVideoRecorderClient::SetVideoCodingOptionsL()")));
    // other state checks handled in the thread, but in this state there is no thread
    VRCASSERT( State() != EStateNone );

    User::LeaveIfError(iThreadProxy.SetMiscOptions(aOptions));
    }

// ---------------------------------------------------------
// CCMRVideoRecorderClient::SetVideoRateControlOptionsL
// Set video rate control options
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CCMRVideoRecorderClient::SetVideoRateControlOptionsL(const TRateControlOptions& aOptions) const
    {
    PRINT((_L("CCMRVideoRecorderClient::SetVideoRateControlOptionsL()")));
    // other state checks handled in the thread, but in this state there is no thread
    VRCASSERT( State() != EStateNone );

    User::LeaveIfError(iThreadProxy.SetVideoRateControlOptions(aOptions));
    }

// ---------------------------------------------------------
// CCMRVideoRecorderClient::GetVideoRateControlOptionsL
// Get video rate control options
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CCMRVideoRecorderClient::GetVideoRateControlOptionsL(TRateControlOptions& aOptions) const
    {
    PRINT((_L("CCMRVideoRecorderClient::GetVideoRateControlOptionsL()")));
    // other state checks handled in the thread, but in this state there is no thread
    VRCASSERT( State() != EStateNone );

    User::LeaveIfError( iThreadProxy.GetVideoRateControlOptions(aOptions) );
    }

// ---------------------------------------------------------
// CCMRVideoRecorderClient::SetPreferredVideoEncoderL
// Set video encoder using its UID. Usage optional.
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CCMRVideoRecorderClient::SetPreferredVideoEncoderL(TUid& aEncoder)
    {
    PRINT((_L("CCMRVideoRecorderClient::SetPreferredVideoEncoderL()")));
    // other state checks handled in the thread, but in this state there is no thread
    VRCASSERT( State() != EStateNone );
    User::LeaveIfError(iThreadProxy.SetPreferredVideoEncoder(aEncoder));
    }

// ---------------------------------------------------------
// CCMRVideoRecorderClient::SetPreferredVideoEncapsulationL
// Set video encoder output format encapsulation. Usage optional.
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CCMRVideoRecorderClient::SetPreferredVideoEncapsulationL(TVideoDataUnitEncapsulation aCapsulation)
    {
    PRINT((_L("CCMRVideoRecorderClient::SetPreferredVideoEncapsulationL()")));
    // other state checks handled in the thread, but in this state there is no thread
    VRCASSERT( State() != EStateNone );
    User::LeaveIfError(iThreadProxy.SetPreferredVideoEncapsulation(aCapsulation));
    }

// ---------------------------------------------------------
// CCMRVideoRecorderClient::SetSegmentTargetSizeL
// Set video encoder target segment size. Usage optional.
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CCMRVideoRecorderClient::SetSegmentTargetSizeL(TUint aLayer, TUint aSizeBytes, TUint aSizeMacroblocks )
    {
    PRINT((_L("CCMRVideoRecorderClient::SetSegmentTargetSizeL()")));
    // other state checks handled in the thread, but in this state there is no thread
    VRCASSERT( State() != EStateNone );
    User::LeaveIfError(iThreadProxy.SetSegmentTargetSize(aLayer, aSizeBytes, aSizeMacroblocks));
    }

// ---------------------------------------------------------
// CCMRVideoRecorderClient::AdjustTimeStampsL
// Adjust time stamps of video
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CCMRVideoRecorderClient::AdjustTimeStampsL(const TInt aAdjustmentMs) const
    {
    PRINT((_L("CCMRVideoRecorderClient::AdjustTimeStampsL()")));
    // other state checks handled in the thread, but in this state there is no thread
    VRCASSERT( State() != EStateNone );
                
    User::LeaveIfError(iThreadProxy.AdjustTimeStamps(aAdjustmentMs));
    }

// ---------------------------------------------------------
// CCMRVideoRecorderClient::SetThreadPriorityL
// Set video thread priority, based on audio thread priority
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CCMRVideoRecorderClient::SetThreadPriorityL(const TThreadPriority& aAudioThreadPriority) const
    {
    User::LeaveIfError(iThreadProxy.SetThreadPriority( aAudioThreadPriority ));
    }

// ---------------------------------------------------------
// CCMRVideoRecorderClient::HandleEvent
// Handle events from video recorder thread
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CCMRVideoRecorderClient::HandleEvent(const TMMFEvent& aEvent)
    {
    PRINT((_L("CCMRVideoRecorderClient::HandleEvent(), aEvent.iEventType %d"),aEvent.iEventType.iUid));
    // note: can't use switch here since it can't compare Uids
    if ( aEvent.iEventType == KCMRCameraPrepareError )
        {
        SetState( EStateOpen );// no need to call state change
        iObserver->MvroError( aEvent.iErrorCode );
        }
    else if ( aEvent.iEventType == KCMRCameraReserveError )
        {
        SetState( EStateOpen );// no need to call state change
        iObserver->MvroError( aEvent.iErrorCode );
        }
    else if ( aEvent.iEventType == KCMRCameraCaptureError )
        {
        // assume the client will stop us
        iObserver->MvroError( aEvent.iErrorCode );
        }
    else if ( aEvent.iEventType == KCMREncoderInitError )
        {
        SetState( EStateOpen );// no need to call state change
        iObserver->MvroError( aEvent.iErrorCode );
        }
    else if ( aEvent.iEventType == KCMRRunTimeError )
        {
        // assume the client will stop us
        iObserver->MvroError( aEvent.iErrorCode );
        }
    else if ( aEvent.iEventType == KCMRPrepareComplete )
        {
        SetState( EStateReadyToRecord );
        iObserver->MvroStateChange( EStateReadyToRecord );
        }
    else if ( aEvent.iEventType == KCMRRecordingComplete )
        {
        SetState( EStateReadyToRecord );
        iObserver->MvroStateChange( EStateReadyToRecord );
        }
    else if ( aEvent.iEventType == KCMRPrepareNeeded )
        {
        if ( State() == EStateReadyToRecord )
            {
            SetState( EStateOpen );
            iObserver->MvroStateChange( EStateOpen );
            }
        }
    else if ( aEvent.iErrorCode == KErrServerTerminated )
        {
        // thread was terminated, can't use it any more
        SetState( EStateNone );
        iObserver->MvroStateChange( EStateNone );
        iObserver->MvroError( aEvent.iErrorCode );
        }
    else
        {
        // something unknown, perhaps from the system
        PRINT((_L("CCMRVideoRecorderClient::HandleEvent() unknown event")));
        iObserver->MvroError( aEvent.iErrorCode );
        }

    }


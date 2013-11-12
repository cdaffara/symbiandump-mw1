/*
* Copyright (c) 2002-2006 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Implementation for video recorder
*
*/



// INCLUDE FILES
#include "CCMRVideoRecorderClient.h"
#include "CCMRMediaRecorderImp.h"
#include "CCMRPerfMonitor.h"
#include "CCMRVideoHWParams.h"
#include "CCMRVideoCodecs.h"
#include "CCMRSupportedCodecs.h"
#include "CCMRConfigManager.h"
#include <mmf/common/mmfvideo.h>


// MACROS

// Assertion macro wrapper for code cleanup
#define MRASSERT(x) __ASSERT_DEBUG(x, User::Panic(_L("CCMRMEDIARECORDER"), EInternalAssertionFailure))


// Debug print macro
#ifdef _DEBUG
#include <e32svr.h>
#define PRINT(x) RDebug::Print x
#else
#define PRINT(x)
#endif

#include "OstTraceDefinitions.h"
#ifdef OST_TRACE_COMPILER_IN_USE
#include "CCMRMediaRecorderImpTraces.h"
#endif

// ================= MEMBER FUNCTIONS =======================


// -----------------------------------------------------------------------------
// CCMRMediaRecorderImp::CCMRMediaRecorderImp
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
CCMRMediaRecorderImp::CCMRMediaRecorderImp()
    {

    iObserver = NULL;
    iSink = NULL;
    iVideoRecorder = NULL;
    iAudioRecorder = NULL;
    iVideoRecorderPreparing = EFalse;
    iAudioRecorderPreparing = EFalse;
    iAudioEnabled = ETrue;
    iState = EStateNone;
    iErrorCode = KErrNone;

    iMaxTargetBitRate = KMMFVariableVideoBitRate; //here this means: not set (=> won't be checked when setting A/V bitrates)

    }

// -----------------------------------------------------------------------------
// CCMRMediaRecorderImp::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
EXPORT_C CCMRMediaRecorder* CCMRMediaRecorder::NewL()
    {

    CCMRMediaRecorderImp* self = new (ELeave) CCMRMediaRecorderImp();
    CleanupStack::PushL(self);
    self->ConstructL();
    CleanupStack::Pop();

    return self;

    }


// -----------------------------------------------------------------------------
// CCMRMediaRecorderImp::ConstructL
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CCMRMediaRecorderImp::ConstructL()
    {
    OstTrace0( CAMERASRV_PERFORMANCE, CCMRMEDIARECORDERIMP_CONSTRUCTL, "e_CCMRMediaRecorderImp::ConstructL 1" );
    iState = EStateNone;

    // create video recorder
    iVideoRecorder = CCMRVideoRecorderClient::NewL();
    // create audio recorder
    iAudioRecorder = CCMRAudioRecorder::NewL();
    OstTrace0( CAMERASRV_PERFORMANCE, DUP1_CCMRMEDIARECORDERIMP_CONSTRUCTL, "e_CCMRMediaRecorderImp::ConstructL 0" );
    }

// -----------------------------------------------------------------------------
// CCMRMediaRecorderImp::~CCMRMediaRecorderImp
// Destructor.
// -----------------------------------------------------------------------------
//
CCMRMediaRecorderImp::~CCMRMediaRecorderImp()
    {
    OstTrace0( CAMERASRV_PERFORMANCE, CCMRMEDIARECORDERIMP_CCMRMEDIARECORDERIMP, "e_CCMRMediaRecorderImp::~CCMRMediaRecorderImp 1" );
    PRINT((_L("CCMRMediaRecorderImp::~CCMRMediaRecorderImp() in")));
    // note, adding "this->" below disabled a PC Lint warning since it makes State explicitly to refer to this class
    if ( (this->State() == EStateRecording) || (this->State() == EStatePaused) )
        {
        // recorder must be stopped first
        PRINT((_L("CCMRMediaRecorderImp::~CCMRMediaRecorderImp() must stop first")));
        TRAPD(err, this->StopL());
        if ( err ) {}
        // we are destructing, ignore the error; error can't be KErrNotReady since our state is not stopping
        }
    // we can't enter here when in stopping-state since stop is blocking controller thread to wait
    // for audio & video threads to complete stop
    // Note that pausing & destructing can't happen at the same time since neither of them can be executed internally,
    // so we don't need to check pausing state here

    // zeroed to make PC Lint happy
    iObserver = NULL;
    iSink = NULL;
    iAudioSource = NULL;


    OstTrace0( CAMERASRV_PERFORMANCE, DUP2_CCMRMEDIARECORDERIMP_CCMRMEDIARECORDERIMP, "e_CCMRMediaRecorderImp::~CCMRMediaRecorderImp_delete_AudioRecorder 1" );
    delete iAudioRecorder;
    OstTrace0( CAMERASRV_PERFORMANCE, DUP3_CCMRMEDIARECORDERIMP_CCMRMEDIARECORDERIMP, "e_CCMRMediaRecorderImp::~CCMRMediaRecorderImp_delete_AudioRecorder 0" );
    PRINT((_L("CCMRMediaRecorderImp::~CCMRMediaRecorderImp() iAudioRecorder deleted")));
    delete iPerfMonitor;
    delete iConfig;
    OstTrace0( CAMERASRV_PERFORMANCE, DUP4_CCMRMEDIARECORDERIMP_CCMRMEDIARECORDERIMP, "e_CCMRMediaRecorderImp::~CCMRMediaRecorderImp_delete_VideoRecorder 1" );
    delete iVideoRecorder;
    OstTrace0( CAMERASRV_PERFORMANCE, DUP5_CCMRMEDIARECORDERIMP_CCMRMEDIARECORDERIMP, "e_CCMRMediaRecorderImp::~CCMRMediaRecorderImp_delete_VideoRecorder 0" );
    PRINT((_L("CCMRMediaRecorderImp::~CCMRMediaRecorderImp() iVideoRecorder deleted")));
    OstTrace0( CAMERASRV_PERFORMANCE, DUP6_CCMRMEDIARECORDERIMP_CCMRMEDIARECORDERIMP, "e_CCMRMediaRecorderImp::~CCMRMediaRecorderImp_delete_AudioOutput 1" );
    delete iAudioOutput;
    OstTrace0( CAMERASRV_PERFORMANCE, DUP7_CCMRMEDIARECORDERIMP_CCMRMEDIARECORDERIMP, "e_CCMRMediaRecorderImp::~CCMRMediaRecorderImp_delete_AudioOutput 0" );
    OstTrace0( CAMERASRV_PERFORMANCE, DUP8_CCMRMEDIARECORDERIMP_CCMRMEDIARECORDERIMP, "e_CCMRMediaRecorderImp::~CCMRMediaRecorderImp_delete_VideoOutput 1" );
    delete iVideoOutput;
    OstTrace0( CAMERASRV_PERFORMANCE, DUP9_CCMRMEDIARECORDERIMP_CCMRMEDIARECORDERIMP, "e_CCMRMediaRecorderImp::~CCMRMediaRecorderImp_delete_VideoOutput 0" );

    if ( iErrorReporter )
        {
        iErrorReporter->Cancel();
        delete iErrorReporter;
        }

    PRINT((_L("CCMRMediaRecorderImp::~CCMRMediaRecorderImp() out")));
    OstTrace0( CAMERASRV_PERFORMANCE, DUP1_CCMRMEDIARECORDERIMP_CCMRMEDIARECORDERIMP, "e_CCMRMediaRecorderImp::~CCMRMediaRecorderImp 0" );
    //#pragma attol insert _ATCPQ_DUMP(0);
    }

// -----------------------------------------------------------------------------
// CCMRMediaRecorderImp::OpenL
// Opens media recorder
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CCMRMediaRecorderImp::OpenL(MCMRMediaRecorderObserver *aObserver,
                                 MDataSource *aAudioSource,
                                 MCMRMediaSink *aSink, TInt aCameraHandle,
                                 const TDesC8& aVideoMimeType,
                                 TFourCC aAudioType)

    {
    OstTrace0( CAMERASRV_PERFORMANCE, CCMRMEDIARECORDERIMP_OPENL, "e_CCMRMediaRecorderImp::OpenL 1" );
    PRINT((_L("CCMRMediaRecorderImp::OpenL() in, thread id %x"),TInt(RThread().Id())));
    if ( State() != EStateNone )
        {
        PRINT((_L("CCMRMediaRecorderImp::OpenL() already exists")));
        User::Leave( KErrAlreadyExists );
        }

    MRASSERT(aObserver);
    iObserver = aObserver;

    MRASSERT(aSink);
    iSink = aSink;

    if ( aAudioSource == NULL )
        {
        iAudioEnabled = EFalse;
        }
    iAudioSource = aAudioSource;

    // create active output objects; must be created here so that they all are in the same thread
    iAudioOutput = CCMRActiveOutput::NewL( iSink, this );
    iVideoOutput = CCMRActiveOutput::NewL( iSink, this );

    iPerfMonitor = CCMRPerfMonitor::NewL( iAudioOutput, iVideoOutput, iVideoRecorder, this );
    iConfig = CCMRConfigManager::NewL();

    // open video recorder
    TRAPD( err, iVideoRecorder->OpenL(this, iVideoOutput, aCameraHandle, aVideoMimeType, iPerfMonitor, iConfig) );
    if ( err != KErrNone )
        {
        PRINT((_L("CCMRMediaRecorderImp::OpenL() video open failed")));
        User::Leave( err );
        }
    // Inform sink about average bit-rate
    TInt br = 0;
    TRAP( err, iVideoRecorder->TargetBitRateL(br));
    if ( err == KErrNone )
        {
        TReal videoBitrateScaler = 0;
        if ( iConfig )
        	{
             videoBitrateScaler = iConfig->PluginSettings().iCMRAvgVideoBitRateScaler;
         	}
         // make sure we never get scaler value 0 (ends up Div0 error)
        if ( videoBitrateScaler == 0 )
         	{
         	videoBitrateScaler = KCMRAvgVideoBitRateScaler;
         	}

         //inform the estimated average bitrate to sink; use a HW-specific scaler to estimate the average
        br = static_cast<TInt>( br*videoBitrateScaler );
        iSink->SetAverageVideoBitRate( br );

        // open audio recorder
        TRAP( err, iAudioRecorder->OpenL(this, iAudioSource, iAudioOutput, aAudioType, iConfig) );
        }

    if ( err != KErrNone )
        {
        PRINT((_L("CCMRMediaRecorderImp::OpenL() audio open failed")));
        User::Leave( err );
        }

    TRAP( err, iAudioRecorder->TargetBitRateL(br));
    if ( err == KErrNone )
        {
        // Inform sink about the default average bit-rate
        iSink->SetAverageAudioBitRate( br );
        }
    else
        {
        PRINT((_L("CCMRMediaRecorderImp::OpenL() audio bitrate check failed")));
        User::Leave( err );
        }

    iErrorReporter = new (ELeave) CCMRErrorReporter(aObserver);

    iState = EStateOpen;
    PRINT((_L("CCMRMediaRecorderImp::OpenL() out")));
    OstTrace0( CAMERASRV_PERFORMANCE, DUP1_CCMRMEDIARECORDERIMP_OPENL, "e_CCMRMediaRecorderImp::OpenL 0" );
    }


// Settings

// -----------------------------------------------------------------------------
// CCMRMediaRecorderImp::SetVideoCodecL
// Sets video codec (overrides the codec given in OpenL).
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CCMRMediaRecorderImp::SetVideoCodecL( const TDesC8& aVideoMimeType )
    {
    PRINT((_L("CCMRMediaRecorderImp::SetVideoCodecL()")));
    if ( iState == EStateNone )
        {
        // not opened yet
        PRINT((_L("CCMRMediaRecorderImp::SetVideoCodecL() wrong state")));
        User::Leave( KErrNotReady );
        }
    iVideoRecorder->SetVideoCodecL( aVideoMimeType );
    }

// -----------------------------------------------------------------------------
// CCMRMediaRecorderImp::GetVideoCodecL
// Gets the currently used video codec
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CCMRMediaRecorderImp::GetVideoCodecL( TDes8& aVideoMimeType )
    {
    PRINT((_L("CCMRMediaRecorderImp::GetVideoCodecL()")));
    if ( iState == EStateNone )
        {
        // not opened yet
        PRINT((_L("CCMRMediaRecorderImp::GetVideoCodecL() wrong state")));
        User::Leave( KErrNotReady );
        }
    iVideoRecorder->GetVideoCodecL( aVideoMimeType );
    }

// -----------------------------------------------------------------------------
// CCMRMediaRecorderImp::GetSupportedVideoCodecsL
// Gets the supported & installed video codecs. This can be called also before opened
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CCMRMediaRecorderImp::GetSupportedVideoCodecsL( CDesC8Array& aVideoMimeTypes )
    {
    PRINT((_L("CCMRMediaRecorderImp::GetSupportedVideoCodecsL()")));

    iVideoRecorder->GetSupportedVideoCodecsL( aVideoMimeTypes );
    }

// -----------------------------------------------------------------------------
// CCMRMediaRecorderImp::SetAudioCodecL
// Sets audio codec (overrides the codec given in OpenL).
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CCMRMediaRecorderImp::SetAudioCodecL( const TFourCC& aAudioType )
    {
    PRINT((_L("CCMRMediaRecorderImp::SetAudioCodecL()")));
    if ( iState == EStateNone )
        {
        // not opened yet
        PRINT((_L("CCMRMediaRecorderImp::SetAudioCodecL() wrong state")));
        User::Leave( KErrNotReady );
        }
    iAudioRecorder->SetAudioCodecL( aAudioType );
    }

// -----------------------------------------------------------------------------
// CCMRMediaRecorderImp::GetAudioCodecL
// Gets the currently used audio codec
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CCMRMediaRecorderImp::GetAudioCodecL( TFourCC& aAudioType )
    {
    PRINT((_L("CCMRMediaRecorderImp::GetAudioCodecL()")));
    if ( iState == EStateNone )
        {
        // not opened yet
        PRINT((_L("CCMRMediaRecorderImp::GetAudioCodecL() wrong state")));
        User::Leave( KErrNotReady );
        }
    iAudioRecorder->GetAudioCodecL( aAudioType );
    }

// -----------------------------------------------------------------------------
// CCMRMediaRecorderImp::GetSupportedAudioCodecsL
// Gets the supported & installed audio codecs. This can be called also before opened
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CCMRMediaRecorderImp::GetSupportedAudioCodecsL( RArray<TFourCC>& aAudioTypes )
    {
    PRINT((_L("CCMRMediaRecorderImp::GetSupportedAudioCodecsL()")));
    iAudioRecorder->GetSupportedAudioCodecsL( aAudioTypes );
    }

// -----------------------------------------------------------------------------
// CCMRMediaRecorderImp::SetVideoFrameRateL
// Sets video frame rate (capture rate & encoding target rate.
// This can be changed also while recording, but then affects only to encoding frame rate
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CCMRMediaRecorderImp::SetVideoFrameRateL(TReal32 aFrameRate)
    {
    PRINT((_L("CCMRMediaRecorderImp::SetVideoFrameRateL()")));
    if ( iState == EStateNone )
        {
        // not opened yet
        PRINT((_L("CCMRMediaRecorderImp::SetVideoFrameRateL() wrong state")));
        User::Leave( KErrNotReady );
        }
    iVideoRecorder->SetFrameRateL(aFrameRate);
    }

// -----------------------------------------------------------------------------
// CCMRMediaRecorderImp::VideoFrameRateL
// Gets video (encoding) frame rate (capture rate is the same or higher).
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TReal32 CCMRMediaRecorderImp::VideoFrameRateL()
    {
    PRINT((_L("CCMRMediaRecorderImp::VideoFrameRateL()")));
    if ( iState == EStateNone )
        {
        // not opened yet
        PRINT((_L("CCMRMediaRecorderImp::VideoFrameRateL() wrong state")));
        User::Leave( KErrNotReady );
        }
    iVideoRecorder->FrameRateL(iFrameRate);
    return iFrameRate;
    }

// -----------------------------------------------------------------------------
// CCMRMediaRecorderImp::SetVideoFrameSizeL
// Sets video frame size.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CCMRMediaRecorderImp::SetVideoFrameSizeL(const TSize& aSize)
    {
    PRINT((_L("CCMRMediaRecorderImp::SetVideoFrameSizeL()")));
    if ( iState == EStateNone )
        {
        // not opened yet
        PRINT((_L("CCMRMediaRecorderImp::SetVideoFrameSizeL() wrong state")));
        User::Leave( KErrNotReady );
        }
    // this can't be changed while recording; video recorder is supposed to notice if in wrong state
    // state is changed based on mvroStateChange()
    iVideoRecorder->SetFrameSizeL(aSize);
    if ( iConfig != NULL )
        {
        iConfig->SetVideoFrameSize(aSize);
        }

    // Change audio samplerate based on framesize
    TSize framesize;
    TInt samplerate = 0;
    TInt channelmode = 0;
    iVideoRecorder->FrameSizeL(framesize);


	if ( iConfig && iConfig->IsICMConfigDataAvailable() )
		{
		samplerate = iConfig->VideoQualitySettings().iAudioSamplingRate;
		channelmode = iConfig->VideoQualitySettings().iAudioChannels;
		PRINT((_L("CCMRMediaRecorderImp::SetVideoFrameSizeL() setting audio settings using ICM samplerate: %d, channels: %d"), samplerate, channelmode));
		}
	else
		{
	    if ( (framesize.iHeight >= KCMRVGAHeight) && (framesize.iWidth >= KCMRVGAWidth) )
	        {
	        samplerate = KCMRAACAudioSampleRateWithVGA;
	        channelmode = KCMRAACAudioChannelModeWithVGA;
	        PRINT((_L("CCMRMediaRecorderImp::SetVideoFrameSizeL() >=VGA Default audio to samplerate:%d channels:%d"), samplerate, channelmode));
	        }
	    else if ( (framesize.iHeight >= KCMRCIFHeight) && (framesize.iWidth >= KCMRCIFWidth) )
	        {
	        samplerate = KCMRAACAudioSampleRateWithCIF;
	        channelmode = KCMRAACAudioChannelModeWithCIF;
	        PRINT((_L("CCMRMediaRecorderImp::SetVideoFrameSizeL() CIF Default audio to samplerate:%d channels:%d"), samplerate, channelmode));
	        }
	    else if ( (framesize.iHeight >= KCMRQVGAHeight) && (framesize.iWidth >= KCMRQVGAWidth) )
	        {
	        samplerate = KCMRAACAudioSampleRateWithQVGA;
	        channelmode = KCMRAACAudioChannelModeWithQVGA;
	        PRINT((_L("CCMRMediaRecorderImp::SetVideoFrameSizeL() QVGA Default audio to samplerate:%d channels:%d"), samplerate, channelmode));
	        }
	     else
	        {
	        // AAC is not used for lower resolutions.
	        }
		}

    TFourCC currentAudioCodec;
    iAudioRecorder->GetAudioCodecL(currentAudioCodec);
    if ( currentAudioCodec == KCMRFourCCIdMPEG4AAC )
	    {
    	if ( samplerate )
        	{
        	iAudioRecorder->SetTargetSampleRateL( samplerate );
        	}
	    if ( channelmode )
	        {
	        iAudioRecorder->SetChannelModeL( channelmode );
	        }
	    }
    }


// -----------------------------------------------------------------------------
// CCMRMediaRecorderImp::GetVideoFrameSizeL
// Gets video frame size.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CCMRMediaRecorderImp::GetVideoFrameSizeL(TSize& aSize) const
    {
    PRINT((_L("CCMRMediaRecorderImp::GetVideoFrameSizeL()")));
    if ( iState == EStateNone )
        {
        // not opened yet
        PRINT((_L("CCMRMediaRecorderImp::GetVideoFrameSizeL() wrong state")));
        User::Leave( KErrNotReady );
        }
    iVideoRecorder->FrameSizeL(aSize);
    }

// -----------------------------------------------------------------------------
// CCMRMediaRecorderImp::SetVideoBitRateL
// Sets video bitrate (or KMMFVariableVideoBitRate)
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CCMRMediaRecorderImp::SetVideoBitRateL(TInt aBitRate)
    {
    PRINT((_L("CCMRMediaRecorderImp::SetVideoBitRateL()")));
    if ( iState == EStateNone )
        {
        // not opened yet
        PRINT((_L("CCMRMediaRecorderImp::SetVideoBitRateL() wrong state")));
        User::Leave( KErrNotReady );
        }

    if ( ( iMaxTargetBitRate != KMMFVariableVideoBitRate ) && ( aBitRate != KMMFVariableVideoBitRate ) )
        {
        TInt abr;
        // check that the total bit-rate (audio+video) does not exceed a given limit
        if ( iAudioRecorder != NULL )
            {
            iAudioRecorder->TargetBitRateL(abr);
            if ( iAudioEnabled && ( abr > 0 ) &&
                 ( abr + aBitRate > iMaxTargetBitRate ) )
                {
                PRINT((_L("CCMRMediaRecorderImp::SetVideoBitRateL() too high combined bitrate")));
                User::Leave(KErrArgument);
                }
            }
        }

    // this can be changed also while recording
    iVideoRecorder->SetTargetBitRateL(aBitRate);
    }

// -----------------------------------------------------------------------------
// CCMRMediaRecorderImp::VideoBitRateL
// Gets video bitrate.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt CCMRMediaRecorderImp::VideoBitRateL()
    {
    PRINT((_L("CCMRMediaRecorderImp::VideoBitRateL()")));
    if ( iState == EStateNone )
        {
        // not opened yet
        PRINT((_L("CCMRMediaRecorderImp::VideoBitRateL() wrong state")));
        User::Leave( KErrNotReady );
        }
    iVideoRecorder->TargetBitRateL(iTargetBitRate);
    return iTargetBitRate;
    }

// -----------------------------------------------------------------------------
// CCMRMediaRecorderImp::SetVideoCodingOptionsL
// Set misc video coding options
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CCMRMediaRecorderImp::SetVideoCodingOptionsL(const TCCMRVideoCodingOptions& aOptions)
    {
    PRINT((_L("CCMRMediaRecorderImp::SetVideoCodingOptionsL()")));
    if ( iState == EStateNone )
        {
        // not opened yet
        PRINT((_L("CCMRMediaRecorderImp::SetVideoCodingOptionsL() wrong state")));
        User::Leave( KErrNotReady );
        }
    iVideoRecorder->SetVideoCodingOptionsL( aOptions );
    }

// -----------------------------------------------------------------------------
// CCMRMediaRecorderImp::SetVideoRateControlOptionsL
// Set video rate control options
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CCMRMediaRecorderImp::SetVideoRateControlOptionsL(const TRateControlOptions& aOptions)
	{
    PRINT((_L("CCMRMediaRecorderImp::SetVideoCodingOptionsL() in")));
    if ( (iState == EStateNone) ||
    	 (iState == EStateOpen) ||
    	 (iState == EStatePreparing) )
    	{
        // not prepared / recording yet
        PRINT((_L("CCMRMediaRecorderImp::SetVideoRateControlOptionsL() wrong state")));
        User::Leave( KErrNotReady );
    	}
    iVideoRecorder->SetVideoRateControlOptionsL( aOptions );
    PRINT((_L("CCMRMediaRecorderImp::SetVideoCodingOptionsL() out")));
	}

// -----------------------------------------------------------------------------
// CCMRMediaRecorderImp::GetVideoRateControlOptionsL
// Get video rate control options
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CCMRMediaRecorderImp::GetVideoRateControlOptionsL(TRateControlOptions& aOptions)
    {
    PRINT((_L("CCMRMediaRecorderImp::GetVideoRateControlOptionsL() in")));
    if ( (iState == EStateNone) ||
         (iState == EStateOpen) ||
         (iState == EStatePreparing) )
        {
        // not prepared / recording yet
        PRINT((_L("CCMRMediaRecorderImp::GetVideoRateControlOptionsL() wrong state")));
        User::Leave( KErrNotReady );
        }
    iVideoRecorder->GetVideoRateControlOptionsL( aOptions );
    PRINT((_L("CCMRMediaRecorderImp::GetVideoRateControlOptionsL() out")));
    }


// -----------------------------------------------------------------------------
// CCMRMediaRecorderImp::SetAudioBitRateL
// Sets audio bitrate
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CCMRMediaRecorderImp::SetAudioBitRateL(TInt aBitRate)
    {
    PRINT((_L("CCMRMediaRecorderImp::SetAudioBitRateL()")));
    if ( iState == EStateNone )
        {
        // not opened yet
        PRINT((_L("CCMRMediaRecorderImp::SetAudioBitRateL() wrong state")));
        User::Leave( KErrNotReady );
        }

    if ( ( iMaxTargetBitRate != KMMFVariableVideoBitRate ) && ( aBitRate > 0 ) )
        {
        TInt vbr;
        // check that the total bit-rate (audio+video) does not exceed a given limit
        iVideoRecorder->TargetBitRateL(vbr);
        if ( ( vbr != KMMFVariableVideoBitRate ) &&
             (vbr + aBitRate > iMaxTargetBitRate ) )
            {
            PRINT((_L("CCMRMediaRecorderImp::SetAudioBitRateL() too high combined bitrate")));
            User::Leave(KErrArgument);
            }
        }

    iAudioRecorder->SetTargetBitRateL(aBitRate);
    }

// -----------------------------------------------------------------------------
// CCMRMediaRecorderImp::AudioBitRateL
// Gets audio bitrate
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt CCMRMediaRecorderImp::AudioBitRateL()
    {
    PRINT((_L("CCMRMediaRecorderImp::AudioBitRateL()")));
    if ( iState == EStateNone )
        {
        // not opened yet
        PRINT((_L("CCMRMediaRecorderImp::AudioBitRateL() wrong state")));
        User::Leave( KErrNotReady );
        }
    iAudioRecorder->TargetBitRateL(iTargetBitRate);
    return iTargetBitRate;
    }

// -----------------------------------------------------------------------------
// CCMRMediaRecorderImp::SetAudioEnabledL
// Sets audio enabled / disabled for the next recording.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CCMRMediaRecorderImp::SetAudioEnabledL(TBool aEnabled)
    {
    PRINT((_L("CCMRMediaRecorderImp::SetAudioEnabledL(), aEnabled: %d"),aEnabled));
    if ( iState == EStateNone )
        {
        // not opened yet
        PRINT((_L("CCMRMediaRecorderImp::SetAudioEnabledL() wrong state")));
        User::Leave( KErrNotReady );
        }

    if ( (State() == EStateRecording) || (State() == EStatePaused) )
        {
        // can't change this parameter when recording or paused.
        PRINT((_L("CCMRMediaRecorderImp::SetAudioEnabledL(), leaves, state: %d"),State()));
        User::Leave(KErrNotReady);
        }
    else
        {
        if ( iAudioEnabled && (!aEnabled) )
            {
            // audio recorder was used but should not be used any more
            iAudioRecorderPreparing = EFalse; //this should be redundant, but repeated to ensure prepare to complete

            if ( iSink )
                {
                // Inform sink about the bit-rate which is now 0
                iSink->SetAverageAudioBitRate( 0 );
                }
            }
        else if ( aEnabled )
            {
            // Enable possibly disabled audio
            TInt bitrate;
            iAudioRecorder->TargetBitRateL( bitrate );
            if ( iSink )
                {
                // Inform sink about the average bit-rate.
                iSink->SetAverageAudioBitRate( bitrate );
                }
            }

        if ( iAudioEnabled != aEnabled )
            {
            // state is changed
            iAudioEnabled = aEnabled;
            // force prepare
            iState = EStateOpen;
            // inform controller
            iObserver->MmroStateChange( iState, KErrNone );
            }
        else
            {
            // redundant call
            }

        }

    }

// -----------------------------------------------------------------------------
// CCMRMediaRecorderImp::AudioEnabledL
// Checks if audio is enabled / disabled.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TBool CCMRMediaRecorderImp::AudioEnabledL() const
    {
    PRINT((_L("CCMRMediaRecorderImp::AudioEnabledL()")));
    if ( iState == EStateNone )
        {
        // not opened yet
        PRINT((_L("CCMRMediaRecorderImp::AudioEnabledL() wrong state")));
        User::Leave( KErrNotReady );
        }
    return iAudioEnabled;
    }

// -----------------------------------------------------------------------------
// CCMRMediaRecorderImp::SetGainL
// Sets audio recording gain.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CCMRMediaRecorderImp::SetGainL(TInt aGain)
    {
    PRINT((_L("CCMRMediaRecorderImp::SetGainL()")));
    if ( iState == EStateNone )
        {
        // not opened yet
        PRINT((_L("CCMRMediaRecorderImp::SetGainL() wrong state")));
        User::Leave( KErrNotReady );
        }
    iAudioRecorder->SetGainL( aGain );
    }

// -----------------------------------------------------------------------------
// CCMRMediaRecorderImp::GainL
// Gets audio recording gain.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt CCMRMediaRecorderImp::GainL() const
    {
    PRINT((_L("CCMRMediaRecorderImp::GainL()")));
    if ( iState == EStateNone )
        {
        // not opened yet
        PRINT((_L("CCMRMediaRecorderImp::GainL() wrong state")));
        User::Leave( KErrNotReady );
        }
    return iAudioRecorder->GainL();
    }

// -----------------------------------------------------------------------------
// CCMRMediaRecorderImp::MaxGainL
// Gets max audio recording gain.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt CCMRMediaRecorderImp::MaxGainL() const
    {
    PRINT((_L("CCMRMediaRecorderImp::MaxGainL()")));
    if ( iState == EStateNone )
        {
        // not opened yet
        PRINT((_L("CCMRMediaRecorderImp::MaxGainL() wrong state")));
        User::Leave( KErrNotReady );
        }
    return iAudioRecorder->MaxGainL();
    }

// -----------------------------------------------------------------------------
// CCMRMediaRecorderImp::SetAudioPriorityL
// Set audio priority settings.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CCMRMediaRecorderImp::SetAudioPriorityL(const TMMFPrioritySettings& aPrioritySettings)
    {
    PRINT((_L("CCMRMediaRecorderImp::SetAudioPriorityL()")));
    if ( iState == EStateNone )
        {
        // not opened yet
        PRINT((_L("CCMRMediaRecorderImp::SetAudioPriorityL() wrong state")));
        User::Leave( KErrNotReady );
        }
    iAudioRecorder->SetPriorityL( aPrioritySettings );
    }


// Recording control

// -----------------------------------------------------------------------------
// CCMRMediaRecorderImp::PrepareL
// Prepares audio & video recorders for recording.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CCMRMediaRecorderImp::PrepareL()
    {
    OstTrace0( CAMERASRV_PERFORMANCE, CCMRMEDIARECORDERIMP_PREPAREL, "e_CCMRMediaRecorderImp::PrepareL 1" );
    PRINT((_L("CCMRMediaRecorderImp::PrepareL()")));

    if ( (State() != EStateOpen) && (State() != EStateReadyToRecord) )
        {
        // prepare possible only when open or prepared
        PRINT((_L("CCMRMediaRecorderImp::PrepareL() wrong state")));
        User::Leave( KErrNotReady );
        }

    iState = EStatePreparing;

    TThreadPriority audioThreadPriority = EPriorityNormal;
    // since we don't know the order of preparecomplete callbacks, both flags must be ETrue
    // before any preparations are started.
    if ( iAudioEnabled )
        {
        iAudioRecorderPreparing = ETrue;
        audioThreadPriority = iAudioRecorder->GetThreadPriority();
        }
    else
        {
        iAudioRecorderPreparing = EFalse;
        }

    iVideoRecorder->SetThreadPriorityL( audioThreadPriority );

    // start preparing video recorder, then wait for MvroStateChange
    iVideoRecorderPreparing = ETrue;
    TRAPD( err, iVideoRecorder->PrepareL() );
    if ( err != KErrNone )
        {
        // As we know the implementation of iVideoRecorder, it can leave only before any async calls.
        // Hence we can restore the previous state & leave here and there is no need to return the observer call
        iVideoRecorderPreparing = EFalse;
        PRINT((_L("CCMRMediaRecorderImp::PrepareL() video prepare failed %d"), err));
        User::Leave( err );
        }


    if ( iAudioEnabled )
        {
        // Prepare audio recorder

        TRAP(err, iAudioRecorder->PrepareL());

        if ( err == KErrNotSupported )
            {
            // No suitable audio codec found => disable audio but don't let this trouble video recording
            PRINT((_L("CCMRMediaRecorderImp::PrepareL() - iAudioRecorder->PrepareL didn't find a suitable audio codec, error code %d"),err));
            iAudioEnabled = EFalse;
            iAudioRecorderPreparing = EFalse;
            // Inform sink about the bit-rate which is now 0
            iSink->SetAverageAudioBitRate( 0 );
            }
        else if ( err != KErrNone )
            {
            // Some more serious problem => preparation of media recorder is not successful.
            // However, we cannot leave here since video recorder is preparing and hence
            // media recorder state can not be restored => error is informed in callback
            PRINT((_L("CCMRMediaRecorderImp::PrepareL() - iAudioRecorder->PrepareL failed with error code %d"),err));
            iErrorCode = err;
            iAudioRecorderPreparing = EFalse;
            }
        else
            {
            // ok
            }
        }

    OstTrace0( CAMERASRV_PERFORMANCE, DUP1_CCMRMEDIARECORDERIMP_PREPAREL, "e_CCMRMediaRecorderImp::PrepareL 0" );

    }

// -----------------------------------------------------------------------------
// CCMRMediaRecorderImp::RecordL
// Starts recording.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CCMRMediaRecorderImp::RecordL()
    {
    PRINT((_L("CCMRMediaRecorderImp::RecordL() in")));
    if ( ( State() == EStateStopping ) || ( State() == EStatePausing ) )
        {
        PRINT((_L("CCMRMediaRecorderImp::RecordL() wrong state")));
        User::Leave( KErrNotReady );
        }

    if ( (State() == EStateRecording) || (State() == EStatePaused) )
        {
        // ignore, recording already going on
        PRINT((_L("CCMRMediaRecorderImp::RecordL() ignore, recording already")));
        return;
        }

    if ( iState != EStateReadyToRecord )
        {
        PRINT((_L("CCMRMediaRecorderImp::RecordL() wrong state")));
        User::Leave(KErrNotReady);
        }

    iVideoOutput->Start();

    iVideoStopped = EFalse;
    TRAPD(err,iVideoRecorder->RecordL());

    if ( err == KErrNone )
        {
        if ( iAudioEnabled )
            {
            iAudioStopped = EFalse;
            iAudioOutput->Start();
            TRAP(err,iAudioRecorder->RecordL());

            if ( err != KErrNone )
                {
                // if audio leaved, video should be stopped
                StopL();
                }
            }
        else
            {
            // audio not recording
            iAudioStopped = ETrue;
            }
        }
    else
        {
        // Leave only now when both components are stopped (can't let audio->recordL leave)
        PRINT((_L("CCMRMediaRecorderImp::RecordL() video's recordl failed %d"), err));
        User::Leave( err );
        }

    // Start performance monitor
    iPerfMonitor->StartL();

    if ( iAudioEnabled )
        {
        // set initial lipsync adjustment
        iVideoRecorder->AdjustTimeStampsL( iAudioRecorder->AVSyncAdjustmentStart() );
        }

    //successful
    iState = EStateRecording;

    PRINT((_L("CCMRMediaRecorderImp::RecordL() out")));
    }

// -----------------------------------------------------------------------------
// CCMRMediaRecorderImp::StopL
// Stop recording.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CCMRMediaRecorderImp::StopL()
    {
    OstTrace0( CAMERASRV_PERFORMANCE, CCMRMEDIARECORDERIMP_STOPL, "e_CCMRMediaRecorderImp::StopL 1" );
    PRINT((_L("CCMRMediaRecorderImp::StopL() in")));
    if ( (State() == EStateRecording) || (State() == EStatePaused) )
        {
        TInt errVideo;
        TInt errVideoStop;        
        TInt errAudio;

        iState = EStateStopping;

        // try to free as many buffers as possible for stop / EOS marker
        iVideoOutput->Flush();
        
        if ( iAudioEnabled && (!iAudioStopped) )
            {
            // start stopping of audio (async)
            TRAP(errAudio,iAudioRecorder->StopL());
            }
        else
            {
            errAudio = KErrNone;
            }

        // try to free as many buffers for video thread as possible.
        iVideoOutput->Flush();
        
        // stop video (sync)
        TRAP(errVideo, iVideoRecorder->StopL());        

        iVideoEOSReached = EFalse;
        TTimeIntervalMicroSeconds32 sleeptime = 15000; // 15ms roughly 1 or ½ frame duration
        PRINT((_L("CCMRMediaRecorderImp::StopL() EOS sleeptime: %d"), sleeptime.Int() ));        
        while ( !iVideoEOSReached )
            {
            PRINT((_L("CCMRMediaRecorderImp::StopL() Waiting EOS / streamend, flushing controller thread buffers:")));            
            iVideoOutput->Flush();
            PRINT((_L("CCMRMediaRecorderImp::StopL() Waiting EOS / streamend, returning to video thread to return buffers to encoder:")));            
            TRAP(errVideoStop, iVideoRecorder->RequestBuffersAndWaitEOSL(iVideoEOSReached));
            if (!iVideoEOSReached)
                {
                PRINT((_L("CCMRMediaRecorderImp::StopL() sleeping %d microsecs waiting for EOS"), sleeptime.Int() ));                
                User::After(sleeptime); // wait 1 frame duration until asking again for StreamEnd again;
                }
            }
        
        iVideoStopped = ETrue;

        if ( iAudioEnabled && (!iAudioStopped) && (errAudio == KErrNone) )
            {
            // Start waiting until audiorecorder indicates it has stopped
            PRINT((_L("CCMRMediaRecorderImp::StopL() ensure that audio recorder has stopped")));
            TRAP(errAudio,iAudioRecorder->WaitUntilStoppedL());
            iAudioStopped = ETrue;
            }
        iAudioOutput->Stop();
        iAudioOutput->Flush();

        // final cleanup for video buffers.
        iVideoOutput->Stop();
        iVideoOutput->Flush();
        
        iPerfMonitor->StopL();

        iState = EStateReadyToRecord;

        if ( errVideo != KErrNone )
            {
            PRINT((_L("CCMRMediaRecorderImp::StopL() leaves, video error %d"),errVideo));
            User::Leave( errVideo );
            }
        if ( errVideoStop != KErrNone )
            {
            PRINT((_L("CCMRMediaRecorderImp::StopL() leaves, videostop error %d"),errVideoStop));
            User::Leave( errVideo );
            }
        if ( errAudio != KErrNone )
            {
            PRINT((_L("CCMRMediaRecorderImp::StopL() leaves, audio error %d"),errAudio));
            User::Leave( errAudio );
            }
        }
    else
        {
        PRINT((_L("CCMRMediaRecorderImp::StopL() already stopped, ignoring")));
        }
    PRINT((_L("CCMRMediaRecorderImp::StopL() out")));
    OstTrace0( CAMERASRV_PERFORMANCE, DUP1_CCMRMEDIARECORDERIMP_STOPL, "e_CCMRMediaRecorderImp::StopL 0" );
    }

// -----------------------------------------------------------------------------
// CCMRMediaRecorderImp::PauseL
// Pause recording.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CCMRMediaRecorderImp::PauseL()
    {
    PRINT((_L("CCMRMediaRecorderImp::PauseL() in")));
    if ( State() != EStateRecording )
        {
        return;
        }

    TInt err;

    // sync call
    TRAP( err, iVideoRecorder->PauseL() );

    if ( err != KErrNone )
        {
        //Stop both
        StopL();
        PRINT((_L("CCMRMediaRecorderImp::PauseL() video's recordl failed %d"),err));
        User::Leave( err );

        }
    else if ( iAudioEnabled )
        {
        iState = EStatePausing;

        // sync call
        TRAP( err, iAudioRecorder->PauseL());

        if ( err != KErrNone )
            {
            // stop both
            iState = EStateRecording;
            StopL();
            PRINT((_L("CCMRMediaRecorderImp::PauseL() audio's recordl failed %d"),err));
            User::Leave( err );
            }
        }
    else
        {
        // ok
        }

    iPerfMonitor->PauseL();

    //successful
    iState = EStatePaused;
    PRINT((_L("CCMRMediaRecorderImp::PauseL() out")));

    }

// -----------------------------------------------------------------------------
// CCMRMediaRecorderImp::ResumeL
// Resume recording.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CCMRMediaRecorderImp::ResumeL()
    {
    PRINT((_L("CCMRMediaRecorderImp::ResumeL() in")));
    if ( State() == EStatePausing )
        {
        PRINT((_L("CCMRMediaRecorderImp::Resume() wrong state")));
        User::Leave( KErrNotReady );
        }

    if ( State() != EStatePaused )
        {
        return;
        }

    TInt err = KErrNone;

    if ( iAudioEnabled )
        {
        TRAP(err,iAudioRecorder->ResumeL());
        if ( err != KErrNone )
            {
            // stop both
            StopL();
            PRINT((_L("CCMRMediaRecorderImp::ResumeL() audio's resumel failed %d"),err));
            User::Leave( err );
            }

        }


    TRAP( err, iVideoRecorder->ResumeL() );
    if ( err != KErrNone )
        {
        //Stop both
        StopL();
        PRINT((_L("CCMRMediaRecorderImp::ResumeL() video's resumel failed %d"),err));
        User::Leave( err );
        }
    else
        {
        // ok
        }

    iPerfMonitor->StartL();

    if ( iAudioEnabled )
        {
        // set lipsync adjustment after pause
        iVideoRecorder->AdjustTimeStampsL( iAudioRecorder->AVSyncAdjustmentResume() );
        }

    //successful
    iState = EStateRecording;
    PRINT((_L("CCMRMediaRecorderImp::ResumeL() out")));

    }

// Observer methods

// -----------------------------------------------------------------------------
// CCMRMediaRecorderImp::MvroStateChange
// Video recorder state change callback
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CCMRMediaRecorderImp::MvroStateChange(CCMRRecorderBase::TRecorderState aState)
    {
    PRINT((_L("CCMRMediaRecorderImp::MvroStateChange(), aState %d, iState %d"),aState,iState));

    switch ( State() )
        {
        case EStatePreparing:

            if ( iErrorCode != KErrNone )
                {
                // audio preparation was completed with an error
                iVideoRecorderPreparing = EFalse;
                iState = EStateOpen;
                iObserver->MmroPrepareComplete( iErrorCode );
                PRINT((_L("CCMRMediaRecorderImp::MvroStateChange(), called preparecomplete with audio errorcode %d"),iErrorCode));
                iErrorCode = KErrNone;
                }
            else if ( aState == CCMRRecorderBase::EStateReadyToRecord )
                {
                iVideoRecorderPreparing = EFalse;

                // call prepare complete -callback if both recorders are prepared
                if ( !iAudioRecorderPreparing )
                    {
                    iState = EStateReadyToRecord;
                    iObserver->MmroPrepareComplete(KErrNone);
                    }
                //else have to wait until audio is prepared
                }
            else
                {
                PRINT((_L("CCMRMediaRecorderImp::MvroStateChange() unexpected state change")));
                MRASSERT( 0 );
                }
            break;

        case EStateReadyToRecord:
            if ( aState == CCMRRecorderBase::EStateOpen )
                {
                // a setting changed video recorder to not-readytorecord state => new prepare needed
                iState = EStateOpen;
                // inform controller
                iObserver->MmroStateChange( iState, KErrNone );
                }
            // else by default state change is to record => no actions needed

            break;

        case EStateStopping:
            {
            PRINT((_L("CCMRMediaRecorderImp::MvroStateChange(), video stopped either normally by receiving EOS or by fatal error.")));            
            iVideoStopped = ETrue;
            break;
            }
        default:
            {
            if ( (aState == CCMRRecorderBase::EStateReadyToRecord)
                || (aState == CCMRRecorderBase::EStateNone) )     // none is signalled if thread was terminated
                {
                // recording stopped
                PRINT((_L("CCMRMediaRecorderImp::MvroStateChange(), video stopped unexpectedly while recording")));
                iVideoStopped = ETrue;
                }
            break;
            }
        }

    PRINT((_L("CCMRMediaRecorderImp::MvroStateChange() out")));
    }

// -----------------------------------------------------------------------------
// CCMRMediaRecorderImp::MvroError
// Video recorder error callback
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CCMRMediaRecorderImp::MvroError(TInt aError)
    {
    PRINT((_L("CCMRMediaRecorderImp::MvroError(), aError %d"),aError));

    switch ( State() )
        {
        case EStatePreparing:

            iVideoRecorderPreparing = EFalse;
            if ( !iAudioRecorderPreparing )
                {
                // Some problem happened with either in camera API or devvr initialisation.
                iObserver->MmroPrepareComplete( aError );
                iState = EStateOpen;
                }
            else
                {
                // have to wait until audio is prepared, don't send any error code yet
                iErrorCode = aError;
                }
            break;

        case EStateStopping:
            // error occurred while stopping, must wait until stop is complete before signaling the error
            PRINT((_L("CCMRMediaRecorderImp::MvroError(), error occurred while stopping, must wait until stop is complete before signaling the error")));
            iErrorCode = aError;
            break;


        default:
            {
            // all the errors are considered fatal, and the fatality is interpreted by application/user based on error code.
            PRINT((_L("CCMRMediaRecorderImp::MvroError(), forwarding error to iErrorReporter")));
            iErrorReporter->FatalError(aError);
            // assumes the controller will stop us
            }
        }

    PRINT((_L("CCMRMediaRecorderImp::MvroError() out")));
    }




// -----------------------------------------------------------------------------
// CCMRMediaRecorderImp::MaroStateChange
// Audio recorder state change callback
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CCMRMediaRecorderImp::MaroStateChange(CCMRRecorderBase::TRecorderState aState)
    {
    PRINT((_L("CCMRMediaRecorderImp::MaroStateChange(), aState %d, iState %d"),aState,iState));

    switch ( State() )
        {
        case EStatePreparing:

            if ( iErrorCode != KErrNone )
                {
                // preparation was completed with error
                iState = EStateOpen;
                iObserver->MmroPrepareComplete( iErrorCode );
                iErrorCode = KErrNone;
                iAudioRecorderPreparing = EFalse;
                }
            else if ( aState == CCMRRecorderBase::EStateReadyToRecord )
                {

                iAudioRecorderPreparing = EFalse;
                // call prepare complete -callback if both recorders are prepared
                if ( !iVideoRecorderPreparing )
                    {
                    iState = EStateReadyToRecord;
                    iObserver->MmroPrepareComplete(KErrNone);
                    }
                //else have to wait until video is prepared
                }
            else
                {
                PRINT((_L("CCMRMediaRecorderImp::MaroStateChange() unexpected state change")));
                MRASSERT( 0 );
                }

            break;

        case EStateReadyToRecord:
            if ( aState == CCMRRecorderBase::EStateOpen )
                {
                // a setting changed audio recorder to not-readytorecord state => new prepare needed
                iState = EStateOpen;
                // inform controller
                iObserver->MmroStateChange( iState, KErrNone );
                }
            // else by default state change is to record => no actions needed

            break;

        case EStatePausing:
            // waiting for audio to pause, pausing is sync and no extra actions needed here
            break;

        case EStateStopping:
            // waiting for audio to stop; should not come here
            iAudioStopped = ETrue;
            break;

        default:
            {
            if ( (aState == CCMRRecorderBase::EStateReadyToRecord)
                || (aState == CCMRRecorderBase::EStateNone) )     // none is signalled if thread was terminated
                {
                PRINT((_L("CCMRMediaRecorderImp::MaroStateChange(), audio stopped unexpectedly while recording")));
                iAudioStopped = ETrue;
                }
            break;
            }

        }

    PRINT((_L("CCMRMediaRecorderImp::MaroStateChange() out")));

    }

// -----------------------------------------------------------------------------
// CCMRMediaRecorderImp::MaroError
// Audio recorder error callback
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CCMRMediaRecorderImp::MaroError(TInt aError)
    {
    PRINT((_L("CCMRMediaRecorderImp::MaroError(), aError %d"),aError));

    switch ( State() )
        {
        case EStatePreparing:
            {
            iAudioRecorderPreparing = EFalse;
            if ( !iVideoRecorderPreparing )
                {
                // Some problem happened with audio initialisation.
                iObserver->MmroPrepareComplete( aError );
                iState = EStateOpen;
                }
            else
                {
                // have to wait until video is prepared => don't call any observer callbacks yet
                iErrorCode = aError;
                }
            break;
            }

        default:
            {
            PRINT((_L("CCMRMediaRecorderImp::MaroError(), forwarding error to iErrorReporter")));
            // all the errors are considered fatal, and the fatality is interpreted by application/user based on error code.
            // audio recorder always changes its state before signalling error
            iErrorReporter->FatalError(aError);
            }
        }
    PRINT((_L("CCMRMediaRecorderImp::MaroError() out")));
    }

// -----------------------------------------------------------------------------
// CCMRMediaRecorderImp::MaooError
// Active output error callback
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CCMRMediaRecorderImp::MaooError(TInt aError)
    {
    PRINT((_L("CCMRMediaRecorderImp::MaooError(), error %d, forwarding error to iErrorReporter"),aError));
    iErrorReporter->FatalError( aError );
    // assumes the controller will stop us
    }

// -----------------------------------------------------------------------------
// CCMRMediaRecorderImp::SetPreferredVideoEncoderL
// Set video encoder using its UID. Usage optional.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CCMRMediaRecorderImp::SetPreferredVideoEncoderL(TUid& aEncoder)
    {
    PRINT((_L("CCMRMediaRecorderImp::SetPreferredVideoEncoderL() in, UID: %d"), aEncoder.iUid));
    if ( (iState == EStateNone) )
        {
        // not prepared / recording yet
        PRINT((_L("CCMRMediaRecorderImp::SetPreferredVideoEncoderL() wrong state")));
        User::Leave( KErrNotReady );
        }
    iVideoRecorder->SetPreferredVideoEncoderL( aEncoder );
    PRINT((_L("CCMRMediaRecorderImp::SetPreferredVideoEncoderL() out")));
    }

// -----------------------------------------------------------------------------
// CCMRMediaRecorderImp::SetPreferredVideoEncapsulationL
// Set video encoder output format encapsulation. Usage optional.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CCMRMediaRecorderImp::SetPreferredVideoEncapsulationL(TVideoDataUnitEncapsulation aCapsulation)
    {
    PRINT((_L("CCMRMediaRecorderImp::SetPreferredVideoEncapsulationL() in, encapsulation: %d"), (TInt)aCapsulation));
    if ( (iState == EStateNone) )
        {
        // not prepared / recording yet
        PRINT((_L("CCMRMediaRecorderImp::SetPreferredVideoEncapsulationL() wrong state")));
        User::Leave( KErrNotReady );
        }
    iVideoRecorder->SetPreferredVideoEncapsulationL( aCapsulation );
    PRINT((_L("CCMRMediaRecorderImp::SetPreferredVideoEncapsulationL() out")));
    }

// ---------------------------------------------------------
// CCMRMediaRecorderImp::SetSegmentTargetSizeL
// Set video encoder target segment size. Usage optional.
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CCMRMediaRecorderImp::SetSegmentTargetSizeL(TUint aLayer, TUint aSizeBytes, TUint aSizeMacroblocks )
    {
    PRINT((_L("CCMRMediaRecorderImp::SetSegmentTargetSizeL() in")));
    if ( (iState == EStateNone) )
        {
        // not prepared / recording yet
        PRINT((_L("CCMRMediaRecorderImp::SetSegmentTargetSizeL() wrong state")));
        User::Leave( KErrNotReady );
        }
    iVideoRecorder->SetSegmentTargetSizeL( aLayer,  aSizeBytes, aSizeMacroblocks);
    PRINT((_L("CCMRMediaRecorderImp::SetSegmentTargetSizeL() out")));
    }

// -----------------------------------------------------------------------------
// CCMRMediaRecorderImp::CCMRErrorReporter::CCMRErrorReporter
// Default constructor
// -----------------------------------------------------------------------------
//
CCMRMediaRecorderImp::CCMRErrorReporter::CCMRErrorReporter(MCMRMediaRecorderObserver *aObserver) : CActive(EPriorityNormal), iObserver(aObserver)
    {
    CActiveScheduler::Add(this);
    iRunning = EFalse;
    }

// -----------------------------------------------------------------------------
// CCMRMediaRecorderImp::CCMRErrorReporter::~CCMRErrorReporter
// Destructor
// -----------------------------------------------------------------------------
//
CCMRMediaRecorderImp::CCMRErrorReporter::~CCMRErrorReporter()
    {
    PRINT((_L("CCMRErrorReporter::~CCMRErrorReporter")));
    Cancel();
    iErrors.Close();
    }


// -----------------------------------------------------------------------------
// CCMRMediaRecorderImp::CCMRErrorReporter::FatalError
// Report fatal error
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CCMRMediaRecorderImp::CCMRErrorReporter::FatalError(TInt aError)
    {
    if ( !IsActive() )
        {
 		iStatus=KRequestPending;
		SetActive();
		TRequestStatus *tmp=(&iStatus);
		User::RequestComplete(tmp,0);
        }
    TInt error = iErrors.Append( TCMRError(aError, EErrorFatal) );
    if ( error != KErrNone )
        {
        PRINT((_L("CCMRMediaRecorderImp::CCMRErrorReporter::FatalError() OOM error in storing error code.")));
        MRASSERT( 0 );        
        }    
    }

// -----------------------------------------------------------------------------
// CCMRMediaRecorderImp::CCMRErrorReporter::TemporaryError
// Report temporary error
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CCMRMediaRecorderImp::CCMRErrorReporter::TemporaryError(TInt aError)
    {
    if ( !IsActive() )
        {
 		iStatus=KRequestPending;
		SetActive();
		TRequestStatus *tmp=(&iStatus);
		User::RequestComplete(tmp,0);
        }
    TInt error = iErrors.Append( TCMRError(aError, EErrorTemp) );
    if ( error != KErrNone )
        {
        PRINT((_L("CCMRMediaRecorderImp::CCMRErrorReporter::TemporaryError() OOM error in storing error code.")));
        MRASSERT( 0 );        
        }
    }

// -----------------------------------------------------------------------------
// CCMRMediaRecorderImp::CCMRErrorReporter::RunL
// Serve error reporting requests
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CCMRMediaRecorderImp::CCMRErrorReporter::RunL()
    {
    PRINT((_L("CCMRMediaRecorderImp::CCMRErrorReporter::RunL() in")));
    if ( !iRunning )
        {
        // this may be called again while observer callback is running, since callback calls MR::StopL which sets activescheduler running
        iRunning = ETrue;

        while ( iErrors.Count() > 0 )
            {
            TCMRError tmp = iErrors[0];
            if ( tmp.iErrorType == EErrorFatal )
                {
                iObserver->MmroFatalError(tmp.iErrorCode);
                }
            else
                {
                iObserver->MmroTemporaryError(tmp.iErrorCode);
                }
            iErrors.Remove(0);
            }

        iErrors.Reset();
        iRunning = EFalse;
        }
    PRINT((_L("CCMRMediaRecorderImp::CCMRErrorReporter::RunL() out")));
    }

// -----------------------------------------------------------------------------
// CCMRMediaRecorderImp::CCMRErrorReporter::DoCancel
// Cancel pending requests
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CCMRMediaRecorderImp::CCMRErrorReporter::DoCancel()
    {
    // activation & request are set at the same time => nothing to cancel
    }



// End of file

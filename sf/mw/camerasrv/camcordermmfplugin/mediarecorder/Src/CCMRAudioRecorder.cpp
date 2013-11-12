/*
* Copyright (c) 2004 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Audio recorder class. Client side for audio thread
*
*/


// INCLUDE FILES

#include    "CCMRAudioRecorder.h"
#include    "CCMRSupportedCodecs.h"
#include    "CCMRAudioCodecs.h"
#include    "CCMRAudioCodecData.h"
#include    "CCMRActiveOutput.h"
#include    "CCMRVideoHWParams.h"

#include    <mmf/server/mmfdatabuffer.h>
#include    <mmf/server/mmfaudioinput.h>
#include 	<AudioPreference.h>                // For MMF audio preference definitions.

// MACROS

// Assertion macro wrapper for code cleanup
#define ARASSERT(x) __ASSERT_DEBUG(x, User::Panic(_L("CCMRAUDIORECORDER"), EInternalAssertionFailure)) 

// Debug print macro
#ifdef _DEBUG
#include <e32svr.h>

#define PRINT(x) RDebug::Print x
#else
#define PRINT(x)
#endif

#include "OstTraceDefinitions.h"
#ifdef OST_TRACE_COMPILER_IN_USE
#include "CCMRAudioRecorderTraces.h"
#endif



// ================= MEMBER FUNCTIONS =======================

// -----------------------------------------------------------------------------
// CCMRAudioRecorder::CCMRAudioRecorder
//
// Default constructor.
// -----------------------------------------------------------------------------
//
CCMRAudioRecorder::CCMRAudioRecorder()
    {
    iObserver = NULL;
    iDataSource = NULL;
    iThreadCreated = EFalse;
    }

// -----------------------------------------------------------------------------
// CCMRAudioRecorder::~CCMRAudioRecorder
//
// Destructor.
// -----------------------------------------------------------------------------
//
CCMRAudioRecorder::~CCMRAudioRecorder()
    {
    OstTrace0( CAMERASRV_PERFORMANCE, CCMRAUDIORECORDER_CCMRAUDIORECORDER, "e_CCMRAudioRecorder::~CCMRAudioRecorder 1" );
    PRINT((_L("CCMRAudioRecorder::~CCMRAudioRecorder() in")));
    
    delete iThreadEventMonitor;
    iThreadEventMonitor = NULL;

    if ( iThreadCreated )
        {
        iThreadProxy.Close();
        }
    //audio thread already logs off audio source (=> deletes it)
    PRINT((_L("CCMRAudioRecorder::~CCMRAudioRecorder() thread closed")));

    iObserver = NULL;
    iDataSource = NULL;
    iOutputAO = NULL;
    
    delete iAudioCodecs;
    iAudioCodecs = NULL;

    PRINT((_L("CCMRAudioRecorder::~CCMRAudioRecorder() out")));
    OstTrace0( CAMERASRV_PERFORMANCE, DUP1_CCMRAUDIORECORDER_CCMRAUDIORECORDER, "e_CCMRAudioRecorder::~CCMRAudioRecorder 0" );
    }


// -----------------------------------------------------------------------------
// CCMRAudioRecorder::NewL
//
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CCMRAudioRecorder* CCMRAudioRecorder::NewL()
    {
    
    CCMRAudioRecorder* self = new (ELeave) CCMRAudioRecorder;
    CleanupStack::PushL(self);
    self->ConstructL();
    CleanupStack::Pop();
    return self;
    
    }


// -----------------------------------------------------------------------------
// CCMRAudioRecorder::ConstructL
//
// Symbian 2nd phase constructor.
// -----------------------------------------------------------------------------
//
void CCMRAudioRecorder::ConstructL()
    {
    SetState(EStateNone);
    iPrioritySettings.iPriority = KAudioPriorityVideoRecording;
    iPrioritySettings.iPref =  TMdaPriorityPreference( KAudioPrefVideoRecording );

    RArray<TFourCC> audioTypes;
    CleanupClosePushL( audioTypes );
    audioTypes.Reset();
    
    iAudioCodecs = CCMRAudioCodecs::NewL();
    iAudioCodecs->GetSupportedAudioCodecsL( audioTypes );
    // audioTypes have always >= 1 element now
    if ( audioTypes.Count() > 0 )
        {
        iAudioCodecFourCC = TFourCC(audioTypes[0]);
        iAudioCodecs->SetAudioCodecL( iAudioCodecFourCC );
        CCMRRecorderBase::SetTargetBitRateL( iAudioCodecs->GetCodecDataL()->GetBitRateL() );
        }
    else
        {
        // no audio codecs exist. Must anyway create audio recorder, since mediarecorder should work also video-only and codec support is checked later
        iAudioCodecFourCC = TFourCC(' ',' ',' ',' ');
        }

    CleanupStack::PopAndDestroy( &audioTypes ); // calls also audioTypes.Close()
    }


// -----------------------------------------------------------------------------
// CCMRAudioRecorder::OpenL
//
// Open recorder. This is like audio controller's AddSource
// -----------------------------------------------------------------------------
//
void CCMRAudioRecorder::OpenL(MCMRAudioRecorderObserver *aObserver, MDataSource *aSource, 
                              CCMRActiveOutput* aOutputAO, const TFourCC& aAudioType, CCMRConfigManager* aConfig )
    {
    PRINT((_L("CCMRAudioRecorder::OpenL() in")));
    if ( State() != EStateNone ) 
        {
        PRINT((_L("CCMRAudioRecorder::OpenL() already exists")));
        User::Leave( KErrAlreadyExists );
        }
  
    // save the references/pointers
    iObserver = aObserver;
    iConfig = aConfig;
    iConfig->SetAudioCodec( iAudioCodecFourCC );

    // check the source type
    if ( aSource && aSource->DataSourceType() != KUidMmfAudioInput)
        {
        PRINT((_L("CCMRAudioRecorder::OpenL() unsupported source")));
        User::Leave(KErrNotSupported);
        }
    iDataSource = aSource;

    iOutputAO = aOutputAO;

    // create thread for audio 
    User::LeaveIfError(iThreadProxy.CreateSubThread(iThreadCreated));
    User::LeaveIfError(iThreadProxy.SetOutput(iOutputAO));

    iThreadEventMonitor = CCMRSubThreadEventMonitor::NewL(*this, iThreadProxy); 
    iThreadEventMonitor->Start();

    // add data source => we will be connected also to DevSound and then we can use e.g. gain settings
    if (iDataSource)
        {
        User::LeaveIfError(iThreadProxy.AddDataSource(iDataSource));
        }

    // set config manager to thread
    User::LeaveIfError(iThreadProxy.SetConfigManager( aConfig ));

    // SetAudioCodec requires we are at least open
    SetState( EStateOpen );
    iObserver->MaroStateChange( State() );

    SetAudioCodecL( aAudioType );
  
    PRINT((_L("CCMRAudioRecorder::OpenL() out")));
    }


// -----------------------------------------------------------------------------
// CCMRAudioRecorder::SetAudioCodecL
//
// Set audio codec to be used (overrides the one given in OpenL)
// -----------------------------------------------------------------------------
//
void CCMRAudioRecorder::SetAudioCodecL( const TFourCC& aAudioType )
    {
    PRINT((_L("CCMRAudioRecorder::SetAudioCodecL() in")));    
    if ( ( State() != EStateOpen ) && ( State() != EStateReadyToRecord ) )
        {
        PRINT((_L("CCMRAudioRecorder::SetAudioCodecL() wrong state")));
        User::Leave(KErrNotReady);
        }
    if ( aAudioType != iAudioCodecFourCC )
        {
        iAudioCodecs->SetAudioCodecL( aAudioType );
        iAudioCodecFourCC = aAudioType;
        iConfig->SetAudioCodec(iAudioCodecFourCC);

        SetState( EStateOpen );
        iObserver->MaroStateChange( State() );
        }
    PRINT((_L("CCMRAudioRecorder::SetAudioCodecL() out")));        
    }

// -----------------------------------------------------------------------------
// CCMRAudioRecorder::GetAudioCodecL
// Get the used audio codec.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CCMRAudioRecorder::GetAudioCodecL( TFourCC& aAudioType ) const
    {
    if ( (State() == EStateNone) || (iAudioCodecs->GetCodecDataL() == NULL) )
        {
        PRINT((_L("CCMRAudioRecorder::GetAudioCodecL() wrong state")));
        User::Leave(KErrNotReady);
        }

    aAudioType = iAudioCodecs->GetCodecDataL()->GetCodecFourCCL();
    }

// -----------------------------------------------------------------------------
// CCMRAudioRecorder::GetSupportedAudioCodecsL
// Get the supported & installed audio codecs.
// This can be called also when the recorder is not open
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CCMRAudioRecorder::GetSupportedAudioCodecsL( RArray<TFourCC>& aAudioTypes ) const
    {
    aAudioTypes.Reset();
    
    iAudioCodecs->GetSupportedAudioCodecsL( aAudioTypes );

    }

// -----------------------------------------------------------------------------
// CCMRAudioRecorder::SetPriorityL
//
// Set audio priority (for DevSound)
// -----------------------------------------------------------------------------
//
void CCMRAudioRecorder::SetPriorityL(const TMMFPrioritySettings& aPrioritySettings)
    {
    if ( State() == EStateNone || State() == EStateRecording || State() == EStatePaused )
        {
        PRINT((_L("CCMRAudioRecorder::SetPriorityL() wrong state")));
        User::Leave(KErrNotReady);
        }
    if ( aPrioritySettings.iPriority < EMdaPriorityMin || aPrioritySettings.iPriority > EMdaPriorityMax )
        {
        // outside allowed range
        PRINT((_L("CCMRAudioRecorder::SetPriorityL() unsupported priority")));
        User::Leave( KErrArgument );
        }

    iPrioritySettings = aPrioritySettings;
    SetState( EStateOpen );
    iObserver->MaroStateChange( State() );

    }


// -----------------------------------------------------------------------------
// CCMRAudioRecorder::SetTargetBitRateL
//
// Set audio bitrate. Note: checked only in PrepareL
// -----------------------------------------------------------------------------
//
void CCMRAudioRecorder::SetTargetBitRateL(TInt aBitRate)
    {
    PRINT((_L("CCMRAudioRecorder::SetTargetBitRateL() in")));
    if ( (( State() != EStateOpen ) && ( State() != EStateReadyToRecord )) || (iAudioCodecs->GetCodecDataL() == NULL) )
        {
        PRINT((_L("CCMRAudioRecorder::SetTargetBitRateL() wrong state")));
        User::Leave(KErrNotReady);
        }

    iAudioCodecs->GetCodecDataL()->SetBitRateL( aBitRate );
    // if we are here, the bitrate was ok, store it also here since the system assumes recorder class stores it
    CCMRRecorderBase::SetTargetBitRateL( aBitRate );

    SetState( EStateOpen );
    iObserver->MaroStateChange( State() );
    PRINT((_L("CCMRAudioRecorder::SetTargetBitRateL() out")));
    }

// -----------------------------------------------------------------------------
// CCMRAudioRecorder::SetTargetSampleRateL
//
// Set audio samplerate. AAC only.
// -----------------------------------------------------------------------------
//
void CCMRAudioRecorder::SetTargetSampleRateL(TInt aSampleRate)
    {
    PRINT((_L("CCMRAudioRecorder::SetTargetSampleRateL() in, samplerate=%d"),aSampleRate));
    if ( (( State() != EStateOpen ) && ( State() != EStateReadyToRecord )) || (iAudioCodecs->GetCodecDataL() == NULL) )
        {
        PRINT((_L("CCMRAudioRecorder::SetTargetSampleRateL() wrong state")));
        User::Leave(KErrNotReady);
        }

    iAudioCodecs->GetCodecDataL()->SetSampleRateL( aSampleRate );

    SetState( EStateOpen );
    iObserver->MaroStateChange( State() );
    PRINT((_L("CCMRAudioRecorder::SetTargetSampleRateL() out")));
    }    

// -----------------------------------------------------------------------------
// CCMRAudioRecorder::SetChannelModeL
//
// Set audio channel mode. AAC only.
// -----------------------------------------------------------------------------
//
void CCMRAudioRecorder::SetChannelModeL(TInt aChannelMode)
    {
    PRINT((_L("CCMRAudioRecorder::SetChannelModeL() in, channelmode=%d"), aChannelMode));
    if ( (( State() != EStateOpen ) && ( State() != EStateReadyToRecord )) || (iAudioCodecs->GetCodecDataL() == NULL) )
        {
        PRINT((_L("CCMRAudioRecorder::SetChannelModeL() wrong state")));
        User::Leave(KErrNotReady);
        }

    iAudioCodecs->GetCodecDataL()->SetChannelModeL( aChannelMode );

    SetState( EStateOpen );
    iObserver->MaroStateChange( State() );
    PRINT((_L("CCMRAudioRecorder::SetChannelModeL() out")));
    }

// -----------------------------------------------------------------------------
// CCMRAudioRecorder::MaxGainL
//
// Get maximum gain from audio input
// -----------------------------------------------------------------------------
//
TInt CCMRAudioRecorder::MaxGainL()
    {
    PRINT((_L("CCMRAudioRecorder::MaxGainL() in")));
    if ( State() == EStateNone )
        {
        PRINT((_L("CCMRAudioRecorder::MaxGainL() wrong state")));
        User::Leave( KErrNotReady );
        }

    // check max gain from sound device
    TInt gain;
    User::LeaveIfError( iThreadProxy.MaxGain( gain ));
    PRINT((_L("CCMRAudioRecorder::MaxGainL() almost out, max gain %d"),gain));
    return gain;
    }

// -----------------------------------------------------------------------------
// CCMRAudioRecorder::GainL
//
// Get current gain from audio input
// -----------------------------------------------------------------------------
//
TInt CCMRAudioRecorder::GainL()
    {
    PRINT((_L("CCMRAudioRecorder::GainL() in")));
    if ( State() == EStateNone )
        {
        PRINT((_L("CCMRAudioRecorder::GainL() wrong state")));
        User::Leave( KErrNotReady );
        }

    // check current gain from sound device
    TInt gain;
    User::LeaveIfError( iThreadProxy.GetGain( gain ));
    PRINT((_L("CCMRAudioRecorder::GainL() almost out, gain %d"),gain));
    return gain;
    }


// -----------------------------------------------------------------------------
// CCMRAudioRecorder::SetGainL
//
// Sets gain for audio input. Can be done before and during recording 
// and is effective immediately
// -----------------------------------------------------------------------------
//
void CCMRAudioRecorder::SetGainL(TInt aGain)
    {
    PRINT((_L("CCMRAudioRecorder::SetGainL() in")));
    if ( State() == EStateNone )
        {
        PRINT((_L("CCMRAudioRecorder::SetGainL() wrong state")));
        User::Leave( KErrNotReady );
        }
    if ( aGain >= 0 ) 
        {
        User::LeaveIfError( iThreadProxy.SetGain(aGain));

        PRINT((_L("CCMRAudioRecorder::SetGainL() out")));
        }
    else
        {
        PRINT((_L("CCMRAudioRecorder::SetGainL() out due to illegal argument: %d"),aGain));
        User::Leave( KErrArgument );
        }
    }

// -----------------------------------------------------------------------------
// CCMRAudioRecorder::GetThreadPriority
//
// Get audio thread priority
// -----------------------------------------------------------------------------
//
TThreadPriority CCMRAudioRecorder::GetThreadPriority() const
    {
    return iThreadProxy.GetThreadPriority();
    }

// -----------------------------------------------------------------------------
// CCMRAudioRecorder::PrepareL
//
// Prepare recorder for recording (freeze settings / implements PrimeL)
// -----------------------------------------------------------------------------
//
void CCMRAudioRecorder::PrepareL()
    {
    PRINT((_L("CCMRAudioRecorder::PrepareL() in    ")));

    if ( (State() != EStateOpen) && (State() != EStateReadyToRecord) )
        {
        PRINT((_L("CCMRAudioRecorder::PrepareL() wrong state")));
        User::Leave( KErrNotReady );
        }

    if ( !iThreadCreated )
        {
        PRINT((_L("CCMRAudioRecorder::PrepareL() no thread")));
        User::Leave( KErrNotReady );
        }

    // set priority settings
    User::LeaveIfError( iThreadProxy.SetPriority(iPrioritySettings));    
    
    // give handle to the audio codec
    User::LeaveIfError( iThreadProxy.SetAudioCodec( iAudioCodecs->GetCodecDataL() ) );

    // inform sink about average audio bitrate
    User::LeaveIfError( iOutputAO->SetAverageAudioBitRate(iAudioCodecs->GetCodecDataL()->GetBitRateL()) );

    // prime the audio thread
    User::LeaveIfError(iThreadProxy.Prime());
    PRINT((_L("CCMRAudioRecorder::PrepareL(), iThreadProxy.Prime() done") ));

    //no need to be in preparing state; prepare is synchronous

    SetState( EStateReadyToRecord );
    iObserver->MaroStateChange( State() );

    PRINT((_L("CCMRAudioRecorder::PrepareL() out    ")));
    }



// -----------------------------------------------------------------------------
// CCMRAudioRecorder::RecordL
//
// Start recording
// -----------------------------------------------------------------------------
//
void CCMRAudioRecorder::RecordL()
    {
    PRINT((_L("CCMRAudioRecorder::RecordL() in    ")));
    if ( (State() == EStateRecording) || (State() == EStatePaused) )
        {
        // ignore, already recording
        return;
        }
    
    if ( State() != EStateReadyToRecord )
        {
        PRINT((_L("CCMRAudioRecorder::RecordL() wrong state")));
        User::Leave(KErrNotReady);
        }

    if ( State() == EStateReadyToRecord )
        {
        User::LeaveIfError(iThreadProxy.Play());

        SetState( EStateRecording );
        iObserver->MaroStateChange( State() );
        }

    PRINT((_L("CCMRAudioRecorder::RecordL() out    ")));
    }

// -----------------------------------------------------------------------------
// CCMRAudioRecorder::StopL
//
// Stop recording
// -----------------------------------------------------------------------------
//
void CCMRAudioRecorder::StopL()
    {
    PRINT((_L("CCMRAudioRecorder::StopL() in    ")));

    if ( (State() == EStateRecording) || (State() == EStatePaused) )
        {

        User::LeaveIfError(iThreadProxy.Stop());
        SetState( EStateStopping );
        }
    PRINT((_L("CCMRAudioRecorder::StopL() out    ")));
    }

// -----------------------------------------------------------------------------
// CCMRAudioRecorder::PauseL
//
// Pause recording
// -----------------------------------------------------------------------------
//
void CCMRAudioRecorder::PauseL()
    {
    if ( State() == EStateRecording )
        {
        User::LeaveIfError(iThreadProxy.Pause());
        // pause was sync
        SetState( EStatePaused );
        iObserver->MaroStateChange( State() );
        }
    }

// -----------------------------------------------------------------------------
// CCMRAudioRecorder::ResumeL
//
// Resume recording
// -----------------------------------------------------------------------------
//
void CCMRAudioRecorder::ResumeL()
    {
    if ( State() == EStatePaused )
        {
        User::LeaveIfError(iThreadProxy.Play());

        SetState( EStateRecording );
        iObserver->MaroStateChange( State() );
        }
    }

// -----------------------------------------------------------------------------
// CCMRAudioRecorder::WaitUntilStoppedL
// Wait until audio thread has stopped recording
// 
// -----------------------------------------------------------------------------
//
void CCMRAudioRecorder::WaitUntilStoppedL()
    {
    if ( (State() == EStateRecording) || (State() == EStatePaused) || (State() == EStateStopping))
        {
        // not stopped yet
        User::LeaveIfError(iThreadProxy.WaitUntilStopped());
        // wait was sync
        SetState( EStateReadyToRecord );
        }
    }

// -----------------------------------------------------------------------------
// CCMRAudioRecorder::AVSyncAdjustmentStart
// Get default AV sync adjustment for start of recording, depending on codec type
// 
// -----------------------------------------------------------------------------
//
TInt CCMRAudioRecorder::AVSyncAdjustmentStart()
    {
    if ( iConfig && iConfig->IsICMConfigDataAvailable() )
        {
		return iConfig->VideoQualitySettings().iAVSyncStartDelay;
        }
    
    TCMRCodecType type = ECodecTypeSW;

    TRAPD(err, type = iAudioCodecs->GetCodecDataL()->GetCodecSWHWTypeL());
    ARASSERT( err == KErrNone );
    if ( err ) {}

    if ( type == ECodecTypeHW )
        {
        //HW codecs (DevSound outputs compressed data)
        return KCMRInitialVideoAudioTimeSyncHW;
        }
    else
        {
        //SW codecs (DevSound outputs PCM)
        return KCMRInitialVideoAudioTimeSyncSW;
        }

    }

// -----------------------------------------------------------------------------
// CCMRAudioRecorder::AVSyncAdjustmentResume
// Get default AV sync adjustment for resume of recording, depending on codec type
// 
// -----------------------------------------------------------------------------
//
TInt CCMRAudioRecorder::AVSyncAdjustmentResume()
    {
    if ( iConfig && iConfig->IsICMConfigDataAvailable() )
        {
		return iConfig->VideoQualitySettings().iAVSyncResumeDelay;
        }
    
    TCMRCodecType type = ECodecTypeSW;

    TRAPD(err, type = iAudioCodecs->GetCodecDataL()->GetCodecSWHWTypeL());
    ARASSERT( err == KErrNone );
    if ( err ) {}

    if ( type == ECodecTypeHW )
        {
        //HW codecs (DevSound outputs compressed data)
        return KCMRPauseVideoAudioTimeSyncHW;
        }
    else
        {
        //SW codecs (DevSound outputs PCM)
        return KCMRPauseVideoAudioTimeSyncSW;
        }

    }

// -----------------------------------------------------------------------------
// CCMRAudioRecorder::HandleEvent
// Handle event from audio thread
//
// -----------------------------------------------------------------------------
//
void CCMRAudioRecorder::HandleEvent(const TMMFEvent& aEvent)
    {
    PRINT((_L("CCMRAudioRecorder::HandleEvent(), aEvent.iEventType %d   aEvent.iErrorCode %d"), aEvent.iEventType.iUid, aEvent.iErrorCode ));

    if (aEvent.iEventType == KMMFEventCategoryPlaybackComplete)
        {
        // Lose of audio resource is signalled like this. Also some errors are signaled using this event
        // If aEvent.iErrorCode == KErrDied, dev policy kicked us out, if it is something else, some other error occurred
        // audio-thread goes to stopped state before sending this
        SetState( EStateReadyToRecord );
        iObserver->MaroStateChange( State() );
        iObserver->MaroError( aEvent.iErrorCode );
        PRINT((_L("CCMRAudioRecorder::HandleEvent(), state changed to ready and error informed") ));
        }
    else if (aEvent.iErrorCode != KErrNone ) 
        {
        if ( aEvent.iErrorCode == KErrServerTerminated )
            {
            // audio thread terminated, no need to stop etc anything
            PRINT((_L("CCMRAudioRecorder::HandleEvent(), audio thread terminated") ));
            SetState( EStateNone );
            iObserver->MaroError( aEvent.iErrorCode );
            iObserver->MaroStateChange( State() );
            }
        else
            {
            // some other event, e.g. OOM
            if ( (State() == EStateRecording) || (State() == EStatePaused) )
                { 
                // stop first
                PRINT((_L("CCMRAudioRecorder::HandleEvent(), error received but must stop first") ));
                iErrorCode = aEvent.iErrorCode;
                if ( iThreadProxy.Stop() == KErrNone )
                    {
                    // wait until stopped
                    iThreadProxy.WaitUntilStopped();
                    // wait was sync, can proceed with error handling
                    }
                }
            //inform observer about error
            SetState( EStateNone );
            iObserver->MaroError( aEvent.iErrorCode );
            iObserver->MaroStateChange( State() );
            PRINT((_L("CCMRAudioRecorder::HandleEvent(), state changed to ready and error informed") ));
            }
        }
    else
        {
        // some event without an errorcode! Don't know if this is possible
        }

    }

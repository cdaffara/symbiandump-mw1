/*
* Copyright (c) 2004-2010 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Audio input class for handling audio data and giving it to CCMRActiveOutput
*
*/


// INCLUDES
#include "CCMRAudioInput.h"
#include "CCMRAudioInputSW.h"
#include "CCMRAudioInputHW.h"
#include "CCMRActiveOutput.h"
#include "CCMRAudioCodecData.h"
#include "CCMRFifo.h"
#include "CCMRVideoHWParams.h"
#include "CCMRConfigManager.h"
#include <mmf/server/mmfaudioinput.h>
#include <AudioInput.h>
#include <AudioPreference.h>                // For MMF audio preference definitions.
#include <mmf/common/mmfaudio.h>            // For KMMFEventCategoryAudioLoadingComplete

// CONSTANTS
// Initial number of buffers allocated
const TInt KCMRNumAudioBuffers = 10;

// MACROS
#define AIASSERT(x) __ASSERT_DEBUG(x, User::Panic(_L("CCMRAUDIOINPUT"), EInternalAssertionFailure))

// Debug print macro
#ifdef _DEBUG
#include <e32svr.h>
#define PRINT(x) RDebug::Print x
#else
#define PRINT(x)
#endif

// Unused parameter macro
#define UNUSED(x) (void)x;


#include "OstTraceDefinitions.h"
#ifdef OST_TRACE_COMPILER_IN_USE
#include "CCMRAudioInputTraces.h"
#endif

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CCMRAudioInput::~CCMRAudioInput
// -----------------------------------------------------------------------------
//
CCMRAudioInput::~CCMRAudioInput()
    {
    OstTrace0( CAMERASRV_PERFORMANCE, CCMRAUDIOINPUT_CCMRAUDIOINPUT, "e_CCMRAudioInput::~CCMRAudioInput 1" );

    if ( iInputEmpty )
        {
        delete iInputEmpty;
        iInputEmpty = NULL;
        }
    if ( iInputFilled )
        {
        delete iInputFilled;
        iInputFilled = NULL;
        }
    if ( iOutputFilled )
        {
        delete iOutputFilled;
        iOutputFilled = NULL;
        }
    if ( iOutputEmptied )
        {
        delete iOutputEmptied;
        iOutputEmptied = NULL;
        }

    if ( (!iSrcBufRef) && iDevSoundBuffer )
        {
        delete iDevSoundBuffer;
        iDevSoundBuffer = NULL;
        }
    if ( iProcessingBuffer )
        {
        // we happen to have the last buffer still in process, must delete it now
        delete iProcessingBuffer;
        iProcessingBuffer = NULL;
        }

    if ( iDecConfigInfo )
        {
        delete iDecConfigInfo;
        iDecConfigInfo = NULL;
        // iOutputVideoBuffer is not used with decspecinfo
        }

    if ( iMediaSinkBuffer )
        {
        delete iMediaSinkBuffer;
        iMediaSinkBuffer = NULL;
        }

    iOutputThreadHandle.Close();

    if ( iMutexCreated )
        {
        iMutexObj.Close();
        }
    OstTrace0( CAMERASRV_PERFORMANCE, DUP1_CCMRAUDIOINPUT_CCMRAUDIOINPUT, "e_CCMRAudioInput::~CCMRAudioInput 0" );
    }

// -----------------------------------------------------------------------------
// CCMRAudioInput::ConstructL
// -----------------------------------------------------------------------------
//
void CCMRAudioInput::ConstructL(CCMRActiveOutput* aOutput, TUint aThreadId, CCMRConfigManager* aConfig )
    {
    PRINT((_L("CCMRAudioInput::ConstructL() in") ));
    iConfig = aConfig; // doesn't own
    iPrioritySettings.iPriority = KAudioPriorityVideoRecording;
    iPrioritySettings.iPref =  TMdaPriorityPreference( KAudioPrefVideoRecording );

    // Create input fifos
    // this fifo is for buffers that are available for storing data from MMFAudioInput/DevSound
    iInputEmpty = CCMRFifo::NewL(KCMRNumAudioBuffers);
    // this fifo is for buffers that are storing data from MMFAudioInput/DevSound
    iInputFilled = CCMRFifo::NewL(KCMRNumAudioBuffers);
    // this fifo is for buffers that contain encoded data and are waiting for sink to retrieve
    iOutputFilled = CCMRFifo::NewL(KCMRNumAudioBuffers);
    // this fifo is for buffers that sink has read and are available for reuse in iOutputFilled
    iOutputEmptied = CCMRFifo::NewL(KCMRNumAudioBuffers);

    iFirstTime = ETrue;
    iFirstBufferRequested = EFalse;
    iTimeStampWhenPaused = TTimeIntervalMicroSeconds(0);
    User::LeaveIfError(iOutputThreadHandle.Open(aThreadId));
    User::LeaveIfError(iMutexObj.CreateLocal());
    iMutexCreated = ETrue;

    iMediaSinkBuffer = new (ELeave) CCMRMediaBuffer();
    iConsumer = aOutput;
    iConsumer->RegisterSourceL( this );

    PRINT((_L("CCMRAudioInput::ConstructL() out") ));
    }

// -----------------------------------------------------------------------------
// CCMRAudioInput::SetCodecL
//
// Set codec
// -----------------------------------------------------------------------------
//
void CCMRAudioInput::SetCodecL( CCMRAudioCodecData* aCodecData )
    {
    PRINT((_L("CCMRAudioInput::SetCodecL() in")));
    iCodecData = aCodecData;
    PRINT((_L("CCMRAudioInput::SetCodecL() out")));
    }

// -----------------------------------------------------------------------------
// CCMRAudioInput::SourcePrimeL
// Primes the source
// -----------------------------------------------------------------------------
//
void CCMRAudioInput::SourcePrimeL()
    {
    PRINT((_L("CCMRAudioInput::SourcePrimeL()")));
    iDevSoundBuffer = NULL;
    iSendBufCount = 0;
    iFirstTime = ETrue;
    }

// -----------------------------------------------------------------------------
// CCMRAudioInput::SourceRecordL
// Start recording
// -----------------------------------------------------------------------------
//
void CCMRAudioInput::SourceRecordL()
    {
    PRINT((_L("CCMRAudioInput::SourceRecordL() in")));
    ConfigDevSoundL();
    iState = EStateRecording;
    iSamplesUpdatedAfterStart = EFalse;
    iFirstBufferRequested = EFalse;
    PRINT((_L("CCMRAudioInput::SourceRecordL() out")));
    }

// -----------------------------------------------------------------------------
// CCMRAudioInput::SourcePauseL
// Pause recording
// -----------------------------------------------------------------------------
//
void CCMRAudioInput::SourcePauseL()
    {
    PRINT((_L("CCMRAudioInput::SourcePauseL(), in")));
    iState = EStateStopping;

    // store the latest timestamp - must do it already here, since sending event may reset it and we should not touch it after reset
  	MMMFAudioInput* audioInput = static_cast<MMMFAudioInput*>(iMMFDataSource);
    CMMFDevSound* devSound = &audioInput->SoundDevice();
    TInt sampleRate = iCodecData->GetSampleRate();

	TBool firstPause = EFalse;
	if (iTimeStampWhenPaused == 0)
		{
		firstPause = ETrue;
		}

    if ( sampleRate )
        {
        TTimeIntervalMicroSeconds currentTimeStamp;
        currentTimeStamp = devSound->SamplesRecorded();
        currentTimeStamp = currentTimeStamp.Int64() * 1000000;
        currentTimeStamp = currentTimeStamp.Int64() / sampleRate;
        iTimeStampWhenPaused = iTimeStampWhenPaused.Int64() + currentTimeStamp.Int64();
        }
    else
        {
        iTimeStampWhenPaused = 0;
        }

   	TInt initialDelay = 0;
    // Add A/V sync constant to paused timestamp
	if ( iConfig && iConfig->IsICMConfigDataAvailable() )
		{
		PRINT((_L("CCMRAudioInput::SourcePauseL(), using ICM setting value for AVsync delay")));
		iTimeStampWhenPaused = iTimeStampWhenPaused.Int64() + (iConfig->VideoQualitySettings().iAVSyncResumeDelay*1000);
		initialDelay = iConfig->VideoQualitySettings().iAVSyncStartDelay;
		}
	else
		{
		initialDelay = KCMRInitialVideoAudioTimeSyncDC;
		if ( sampleRate <= KCMRPauseVideoAudioTimeSyncDC_LowSampleRateValue )
		    {
		    PRINT((_L("CCMRAudioInput::SourcePauseL(), using LowSampleRateValue")));
		    iTimeStampWhenPaused = iTimeStampWhenPaused.Int64() + (KCMRPauseVideoAudioTimeSyncDC_LowSampleRate*1000);
		    }
		else if ( sampleRate <= KCMRPauseVideoAudioTimeSyncDC_MidSampleRateValue )
			{
		    PRINT((_L("CCMRAudioInput::SourcePauseL(), using MidSampleRateValue")));
		    iTimeStampWhenPaused = iTimeStampWhenPaused.Int64() + (KCMRPauseVideoAudioTimeSyncDC_MidSampleRate*1000);
			}
		else
		    {
		    iTimeStampWhenPaused = iTimeStampWhenPaused.Int64() + (KCMRPauseVideoAudioTimeSyncDC*1000);
		    }
		}

    // Add startdelay if first pause.
    if ( firstPause )
	    {
	    // only add start delay if we haven't been paused. Only add delay after first audio available
		if ( initialDelay < 0 )
			{
			if ( iTimeStampWhenPaused > -(initialDelay*1000) )
				{
				iTimeStampWhenPaused = iTimeStampWhenPaused.Int64() + (initialDelay*1000);
				}
			else
				{
				iTimeStampWhenPaused = 0;
				}
			}
		else
			{
			iTimeStampWhenPaused = iTimeStampWhenPaused.Int64() + (initialDelay*1000);
			}
	    }
		
	// Ensure that after resume, if LatestTimeStampL() is called before UpdateTimeL() has been called it will
	// return a sensible value.
	iLatestTimeStamp = iTimeStampWhenPaused.Int64();
    // Stop audio.
    PRINT((_L("CCMRAudioInput::SourcePauseL() DevSound SourceStopL()")));
    devSound->Stop();

    if ( iOutputFilled->IsEmpty() )
        {
        PRINT((_L("CCMRAudioInput::SourcePauseL(), no data buffers queued so no chance to signal end of data; sending an event instead")));
        TMMFEvent event (KMMFEventCategoryPlaybackComplete, KErrNone);
    	iEventHandler.SendEventToClient(event);
        }

    PRINT((_L("CCMRAudioInput::SourcePauseL() out")));
    }

// -----------------------------------------------------------------------------
// CCMRAudioInput::SourceStopL
// Stop playing (recording)
// -----------------------------------------------------------------------------
//
void CCMRAudioInput::SourceStopL()
    {
    PRINT((_L("CCMRAudioInput::SourceStopL() in")));
    if ( iState != EStateStopping )
        {
        // we came here without pause => there was an error, must stop source
        PRINT((_L("CCMRAudioInput::SourceStopL() DevSound SourceStopL()")));
        MMMFAudioInput* audioInput = static_cast<MMMFAudioInput*>(iMMFDataSource);
        CMMFDevSound* devSound = &audioInput->SoundDevice();
        devSound->Stop();
        iFirstBufferRequested = EFalse;
        }
    // else data source was already stopped in pause
    iState = EStateStopped;
    PRINT((_L("CCMRAudioInput::SourceStopL() out")));
    }

// -----------------------------------------------------------------------------
// CCMRAudioInput::SetSourcePrioritySettings
// Set priority settings to source
// -----------------------------------------------------------------------------
//
void CCMRAudioInput::SetSourcePrioritySettings(const TMMFPrioritySettings& aPrioritySettings)
    {
    iPrioritySettings = aPrioritySettings;
    MMMFAudioInput* audioInput = static_cast<MMMFAudioInput*>(iMMFDataSource);
    CMMFDevSound* devSound = &audioInput->SoundDevice();
    devSound->SetPrioritySettings(aPrioritySettings);
    }

// -----------------------------------------------------------------------------
// CCMRAudioInput::SoundDevice
// Get reference to DevSound from MMFAudioInput
// -----------------------------------------------------------------------------
//
CMMFDevSound& CCMRAudioInput::SoundDevice()
    {
	MMMFAudioInput* audioInput = static_cast<MMMFAudioInput*>(iMMFDataSource);
    return audioInput->SoundDevice();
    }

// -----------------------------------------------------------------------------
// CCMRAudioInput::ResetTimeStamp
//
// Resets audio timestamp before new recording; can't be reset automatically
// in stop/play since they are used also in pause/resume when the stamp should
// not reset
// -----------------------------------------------------------------------------
//
void CCMRAudioInput::ResetTimeStamp()
    {
    iTimeStampWhenPaused = TTimeIntervalMicroSeconds(0);
    iNumFramesReceived = 0;
    iDecConfigInfoChecked = EFalse;
    iSamplesUpdatedAfterStart = EFalse;
    iLatestTimeStamp = 0;
    }

// -----------------------------------------------------------------------------
// CCMRAudioInput::SendEventToClient
// Stop & send event (error) to client
// -----------------------------------------------------------------------------
//
void CCMRAudioInput::SendEventToClient(const TMMFEvent& aEvent)
	{
    PRINT((_L("CCMRAudioInput::SendEventToClient() in, event & error, %x & %d"), aEvent.iEventType, aEvent.iErrorCode));
    TRAPD(error,SourceStopL()); // errors can be ignored, we are already reporting an error here
    if (error != KErrNone)
        {
        PRINT((_L("CCMRAudioInput::SendEventToClient() stop error: %d"), error));
        }
	iEventHandler.SendEventToClient(aEvent);
    PRINT((_L("CCMRAudioInput::SendEventToClient() out")));
	}

// -----------------------------------------------------------------------------
// CCMRAudioInput::NewBufferL
// Inform sink about new coded buffer, and store it in iOutputCoded unless there
// aren't any older buffer under processing
// -----------------------------------------------------------------------------
//
void CCMRAudioInput::NewBufferL(CMMFBuffer* aBuffer)
    {
    // enter restricted area
    iMutexObj.Wait();

    CMMFBuffer* storeBuffer = NULL;
    storeBuffer = aBuffer;

    TInt bufLength = storeBuffer->BufferSize();
    TInt frameLength = 1;   // set to 1 since nonzero needed as the initial value in the while-loop below
    TUint8* data = const_cast<TUint8*>((static_cast<CMMFDataBuffer*>(storeBuffer))->Data().Ptr());
    while ( (bufLength > 0) && (frameLength > 0) )
        {
        frameLength = iCodecData->FrameLength( data, bufLength );
        bufLength -= frameLength;
        data += frameLength;
        iNumFramesReceived++;
        iNumFramesWaiting++;
        }

    TInt error = KErrNone;
    if ( iProcessingBuffer )
        {
        // there is one under processing, store this one to fifo
        TRAP(error, iOutputFilled->PutL( reinterpret_cast<TAny*>(storeBuffer) ));
        PRINT((_L("CCMRAudioInput::NewBufferL() added %x to fifo. Fifo size %d"), storeBuffer, iOutputFilled->NumberOfItems() ));
        }
    else
        {
        // take it into use immediately
        iProcessingBuffer = static_cast<CMMFDataBuffer*>(storeBuffer);
        iProcessingIndex = 0;
        iSrcNumUnreadBytes = storeBuffer->BufferSize();
        PRINT((_L("CCMRAudioInput::NewBufferL() taken buffer into iProcessingBuffer %x, size %d"), storeBuffer, iSrcNumUnreadBytes ));
        }

    // signal to sink
    if ( ( error == KErrNone ) && iSinkRequestStatus  )
        {
        PRINT((_L("CCMRAudioInput::EmptyBufferL() RequestComplete")));
        iOutputThreadHandle.RequestComplete( iSinkRequestStatus, KErrNone );
        }
    else
        {
        // else a new request has not been issued yet; this buffer will be handled jointly with previous ones
        PRINT((_L("CCMRAudioInput::NewBufferL() skipping RequestComplete")));
        }

    // leave restricted area
    iMutexObj.Signal();

    if ( error != KErrNone )
        {
        // leave postponed here to get out of mutex
        PRINT((_L("CCMRAudioInput::NewBufferL() leave with error code %d"), error));
        User::Leave( error );
        }

    }

// -----------------------------------------------------------------------------
// CCMRAudioInput::FillBufferL
// Ask new buffers from devsound.
// -----------------------------------------------------------------------------
//
void CCMRAudioInput::FillBufferL()
    {
    PRINT((_L("CCMRAudioInput::FillBufferL() in")));
    //this is a one-shot "prime" funtion for MMFDevSound as first buffer is uninitialised
    MMMFAudioInput* audioInput = static_cast<MMMFAudioInput*>(iMMFDataSource);
    CMMFDevSound* devSound = &audioInput->SoundDevice();

    if (!iFirstBufferRequested)
        {
        PRINT((_L("CCMRAudioInput::FillBufferL() DevSound RecordInitL")));
        devSound->RecordInitL();
        iFirstBufferRequested = ETrue;
        PRINT((_L("CCMRAudioInput::FillBufferL() out")));
        return;
        }
    PRINT((_L("CCMRAudioInput::FillBufferL() DevSound RecordData")));
    devSound->RecordData();
    PRINT((_L("CCMRAudioInput::FillBufferL() out")));
    }

// -----------------------------------------------------------------------------
// CCMRAudioInput::RequestNewData
// Output active object is ready to accept new data
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CCMRAudioInput::RequestNewData(TRequestStatus& aStatus)
    {
	PRINT((_L("CCMRAudioInput::RequestNewData() - START - aStatus: 0x%08x, iSinkRequestStatus: 0x%08x"), &aStatus, iSinkRequestStatus ));
    // enter restricted area
    iMutexObj.Wait();

    iSinkRequestStatus = &aStatus;
    aStatus = KRequestPending;

    // leave restricted area
    iMutexObj.Signal();
	PRINT((_L("CCMRAudioInput::RequestNewData() - END - aStatus: 0x%08x, iSinkRequestStatus: 0x%08x"), &aStatus, iSinkRequestStatus ));
    }

// -----------------------------------------------------------------------------
// CCMRAudioInput::RequestNewDataCancel
// -----------------------------------------------------------------------------
//
void CCMRAudioInput::RequestNewDataCancel(TRequestStatus& aStatus)
    {
	PRINT((_L("CCMRAudioInput::RequestNewDataCancel() - START - aStatus: 0x%08x, iSinkRequestStatus: 0x%08x"), &aStatus, iSinkRequestStatus ));

    // enter restricted area
    iMutexObj.Wait();

	if ( &aStatus == iSinkRequestStatus )
		{
		iOutputThreadHandle.RequestComplete( iSinkRequestStatus, KErrCancel );
		}

    // leave restricted area
    iMutexObj.Signal();

	PRINT((_L("CCMRAudioInput::RequestNewDataCancel() - END - aStatus: 0x%08x, iSinkRequestStatus: 0x%08x"), &aStatus, iSinkRequestStatus ));
    }

// -----------------------------------------------------------------------------
// CCMRAudioInput::GetNextBuffer
// Output active object takes the next output buffer
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
CCMRMediaBuffer* CCMRAudioInput::GetNextBuffer()
    {
    PRINT((_L("CCMRAudioInput::GetNextBuffer() in")));

    if ( (!iDecConfigInfoChecked) && (iState == EStateRecording)  )
        {
        // check if we have to provide decoder config info first

        iDecConfigInfoChecked = ETrue;

        TRAPD(err,
            {
            iDecConfigInfo = iCodecData->GetDecoderConfigInfoLC();
            if ( iDecConfigInfo != NULL )
                {
                // must pop, otherwise TRAPD will panic
                CleanupStack::Pop(iDecConfigInfo);
                }
            }
            );
        if ( err != KErrNone )
            {
            return NULL;
            }
        if ( iDecConfigInfo )
            {
            PRINT((_L("CCMRAudioInput::GetNextBuffer() providing decoder config info first")));
            const TUint8* data = iDecConfigInfo->Ptr();
            TInt length = iDecConfigInfo->Length();
            PRINT((_L("CCMRAudioInput::GetNextBuffer() length of the info %d"), length));

            iMediaSinkBuffer->Set( TPtrC8(data,length),
                CCMRMediaBuffer::EAudioDecSpecInfo,
                length,
                EFalse,
                TTimeIntervalMicroSeconds(0) );

            // can handle only 1 output buffer at a time
            iOutputBufferInUse = ETrue;

            PRINT((_L("CCMRAudioInput::GetNextBuffer() with DecConfigInfo out")));
            return iMediaSinkBuffer;
            }
        // else the codec doesn't provide it
        }


    // enter restricted area
    iMutexObj.Wait();

    // take it from fifo
    if ( iProcessingBuffer && (!iOutputBufferInUse) )
        {

        // check frame length

        TUint8* data = const_cast<TUint8*>(iProcessingBuffer->Data().Ptr());
        data += iProcessingIndex;

        AIASSERT( iCodecData );

        TInt frameLength = iCodecData->FrameLength( data, iProcessingBuffer->BufferSize() );

        // associate the next frame from the buffer to mediabuffer
        iMediaSinkBuffer->Set( TPtrC8(data,frameLength),
            iCodecData->BufferType(),
            frameLength,
            EFalse,
            TTimeIntervalMicroSeconds((iNumFramesReceived - iNumFramesWaiting)*iCodecData->GetFrameDurationUs()) );

        iNumFramesWaiting--;

        PRINT((_L("CCMRAudioInput::GetNextBuffer() send buf of length %d"),frameLength));

        iProcessingIndex += frameLength;
        iSrcNumUnreadBytes -= frameLength;

        // leave restricted area
        iMutexObj.Signal();

        // can handle only 1 output buffer at a time
        iOutputBufferInUse = ETrue;

        PRINT((_L("CCMRAudioInput::GetNextBuffer() out")));
        return iMediaSinkBuffer;
        }
    else
        {
        // leave restricted area
        iMutexObj.Signal();

        PRINT((_L("CCMRAudioInput::GetNextBuffer() out (no data)")));
        return NULL;
        }
    }

// -----------------------------------------------------------------------------
// CCMRAudioInput::NumBuffersWaiting
// Return the number of buffers in the source
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt CCMRAudioInput::NumBuffersWaiting()
    {
    return iNumFramesWaiting;
    }

// -----------------------------------------------------------------------------
// CCMRAudioInput::LatestTimeStampL
// Return the latest time stamp from the input stream
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CCMRAudioInput::LatestTimeStampL(TTimeIntervalMicroSeconds& aTimeStamp) const
    {
  	// estimate the time since the previous call to devsound's samplesrecorded
  	if ( iSamplesUpdatedAfterStart )
  	    {
        TTime current;
        current.UniversalTime();
        aTimeStamp = iLatestTimeStamp.Int64() + current.MicroSecondsFrom(iTimeWhenSamplesUpdated).Int64();
  	    }
    else
        {
        aTimeStamp = iLatestTimeStamp.Int64();
        }

    PRINT((_L("CCMRAudioInput::LatestTimeStampL() timestamp %d"), I64INT(aTimeStamp.Int64()) ));
    }

// -----------------------------------------------------------------------------
// CCMRAudioInput::DurationL
// Return the duration of the recording
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CCMRAudioInput::DurationL(TTimeIntervalMicroSeconds& aDuration) const
    {
    aDuration = TTimeIntervalMicroSeconds(iNumFramesReceived*iCodecData->GetFrameDurationUs());
    }

// -----------------------------------------------------------------------------
// CCMRAudioInput::ReturnBuffer
// Output active object returns an emptied buffer
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CCMRAudioInput::ReturnBuffer(CCMRMediaBuffer* aBuffer)
    {
    PRINT((_L("CCMRAudioInput::ReturnBuffer() in")));
    // save it for reuse
    iMediaSinkBuffer = aBuffer;
    iOutputBufferInUse = EFalse;
    if ( iDecConfigInfo )
        {
        delete iDecConfigInfo;
        iDecConfigInfo = NULL;
        // iOutputVideoBuffer is not used with decspecinfo
        }

    if ( !iProcessingBuffer )
        {
        // we must have stopped due to pausetimer in audiothreadproxysession; no need to save the buffer, it was already saved
        return;
        }

    if ( iProcessingBuffer->LastBuffer() )
        {
        PRINT((_L("CCMRAudioInput::ReturnBuffer(), last data buffer given to sink; sending an event to inform completion")));
        TMMFEvent event (KMMFEventCategoryPlaybackComplete, KErrNone);
    	iEventHandler.SendEventToClient(event);
        }

    // enter restricted area
    iMutexObj.Wait();
    TInt err = KErrNone;
    if ( iSrcNumUnreadBytes <= 0 )
        {
        TRAP(err,iOutputEmptied->PutL( reinterpret_cast<TAny*>(iProcessingBuffer)));
        if ( err == KErrNone )
            {
            PRINT((_L("CCMRAudioInput::ReturnBuffer() save buffer %x to iOutputEmptied"),iProcessingBuffer));
            // reset
            iProcessingIndex = 0;
            iProcessingBuffer = NULL;
            if (iOutputFilled && !iOutputFilled->IsEmpty())
                {
                /* take the next into processing */
                iProcessingBuffer = reinterpret_cast<CMMFDataBuffer*>(iOutputFilled->Get());
                iProcessingIndex = 0;
                iSrcNumUnreadBytes = iProcessingBuffer->BufferSize();
                PRINT((_L("CCMRAudioInput::ReturnBuffer() started processing a new buffer %x from iOutputFilled"),iProcessingBuffer));
                }
            }
        else
            {
            // Can't store in fifo; however, aBuffer was stored to iOutputBuffer already, and iProcessingBuffer is a member
            // signal the error after got out of mutex
            PRINT((_L("CCMRAudioInput::ReturnBuffer() putting buffer to fifo failed, deallocate the buffer")));
            delete iProcessingBuffer;
            iProcessingBuffer = NULL;
            }
        }
    // leave restricted area
    iMutexObj.Signal();

    if ( err != KErrNone )
        {
        // leave postponed here to get out of mutex
        PRINT((_L("CCMRAudioInput::ReturnBuffer() report GeneralError event with error code %d"), err));
        TMMFEvent event(KMMFEventCategoryVideoRecorderGeneralError, err);
        iEventHandler.SendEventToClient(event);
        }

    PRINT((_L("CCMRAudioInput::ReturnBuffer() out")));

    }

// -----------------------------------------------------------------------------
// CCMRAudioInput::ConfigDevSoundL
// Check DevSound capabilities before configuring. If sampling rate is not
// supported by DevSound, select the next highest sampling rate supported.
// -----------------------------------------------------------------------------
//
void CCMRAudioInput::ConfigDevSoundL()
	{
    PRINT((_L("CCMRAudioInput::ConfigDevSoundL() in")));
  	MMMFAudioInput* audioInput = static_cast<MMMFAudioInput*>(iMMFDataSource);
    CMMFDevSound* devSound = &audioInput->SoundDevice();

	// Set Audio Routing API to use default mic only.
	// Create CAudioInput (errors trapped as this is optional feature from adaptation).
	PRINT((_L("CCMRAudioInput::ConfigDevSoundL(), Audio Routing in")));
	CAudioInput* audioRoutingInput = NULL;
	TRAPD(audioRoutingError, audioRoutingInput = CAudioInput::NewL( *devSound ));
	CleanupStack::PushL(audioRoutingInput);
	if ( audioRoutingError == KErrNone )
		{
		PRINT((_L("CCMRAudioInput::ConfigDevSoundL(), Audio Routing API created")));
		RArray<CAudioInput::TAudioInputPreference> inputArray;
		if ( inputArray.Append( CAudioInput::EDefaultMic ) == KErrNone )
			{
			PRINT((_L("CCMRAudioInput::ConfigDevSoundL(), Audio Routing API input array set to default mic.")));
			// Set Audio Input
			if ( audioRoutingInput )
				{
				TRAP(audioRoutingError, audioRoutingInput->SetAudioInputL( inputArray.Array( ) ));
				if ( audioRoutingError )
					{
					// if KErrNotSupported the devsound implementation probably doesn't support custom interface.
					PRINT((_L("CCMRAudioInput::ConfigDevSoundL(), Audio Routing API SetAudioInputL failed: %d"), audioRoutingError));
					}
				else
					{
					PRINT((_L("CCMRAudioInput::ConfigDevSoundL(), Audio Routing API set to default mic.")));
					}
				}
			}
		else
			{
			PRINT((_L("CCMRAudioInput::ConfigDevSoundL(), Audio Routing API input array set to default mic failed.")));
			}
		inputArray.Close();
		PRINT((_L("CCMRAudioInput::ConfigDevSoundL(), Audio Routing out")));
		}
	CleanupStack::PopAndDestroy(audioRoutingInput);

	// Query DevSound capabilities and Try to use DevSound sample rate and
	// mono/stereo capability
	TMMFCapabilities devSoundCaps = devSound->Capabilities();
	// get current config
	TMMFCapabilities devSoundConfig = devSound->Config();

	// Default PCM16
	devSoundConfig.iEncoding = EMMFSoundEncoding16BitPCM;

	// 1 = Monophonic and 2 == Stereo
    TInt channels = iCodecData->GetNumChannels();
    if ((channels == 2) && (devSoundCaps.iChannels & EMMFStereo))
        {
        PRINT((_L("CCMRAudioInput::ConfigDevSoundL() stereo recording")));
        devSoundConfig.iChannels = channels;
        }
	else if (devSoundCaps.iChannels & EMMFMono)
        {
        PRINT((_L("CCMRAudioInput::ConfigDevSoundL() mono recording")));
		devSoundConfig.iChannels = 1;
        }
    else
        {
        PRINT((_L("CCMRAudioInput::ConfigDevSoundL() unsupported channel configuration requested")));
        User::Leave( KErrNotSupported );
        }

    if ( iCodecData->GetNumChannels() != devSoundConfig.iChannels )
	    {
		iCodecData->SetChannelModeL(devSoundCaps.iChannels);
    	}

	// Check for std sample rates.
    TInt sampleRate = iCodecData->GetSampleRate();
    PRINT((_L("CCMRAudioInput::ConfigDevSoundL() got samplerate: %d from codec"), sampleRate));

    // At the moment only 8 and 16 kHz is supported. If we'll get codecs with other sampling rates, this must be modified
	if ((sampleRate == 8000) && (devSoundCaps.iRate & EMMFSampleRate8000Hz))
        {
        PRINT((_L("CCMRAudioInput::ConfigDevSoundL() using sample rate 8000")));
		devSoundConfig.iRate = EMMFSampleRate8000Hz;
        }
	else if ((sampleRate == 16000) && (devSoundCaps.iRate & EMMFSampleRate16000Hz))
        {
        PRINT((_L("CCMRAudioInput::ConfigDevSoundL() using sample rate 16000")));
		devSoundConfig.iRate = EMMFSampleRate16000Hz;
        }
	else if ((sampleRate == 24000) && (devSoundCaps.iRate & EMMFSampleRate24000Hz))
        {
        PRINT((_L("CCMRAudioInput::ConfigDevSoundL() using sample rate 24000")));
		devSoundConfig.iRate = EMMFSampleRate24000Hz;
        }
	else if ((sampleRate == 32000) && (devSoundCaps.iRate & EMMFSampleRate32000Hz))
        {
        PRINT((_L("CCMRAudioInput::ConfigDevSoundL() using sample rate 32000")));
		devSoundConfig.iRate = EMMFSampleRate32000Hz;
        }
	else if ((sampleRate == 48000) && (devSoundCaps.iRate & EMMFSampleRate48000Hz))
        {
        PRINT((_L("CCMRAudioInput::ConfigDevSoundL() using sample rate 48000")));
		devSoundConfig.iRate = EMMFSampleRate48000Hz;
        }
    else
		{
        // try to set the codec to 16k samplerate.
        if ( (sampleRate >= 16000) && (devSoundCaps.iRate & EMMFSampleRate16000Hz) )
            {
            // this one leaves if the rate is not supported
            PRINT((_L("CCMRAudioInput::ConfigDevSoundL() unsupported audio samplerate - trying 16k")));
            iCodecData->SetBitRateL( 32000 );
            iCodecData->SetSampleRateL( 16000 );
    		devSoundConfig.iRate = EMMFSampleRate16000Hz;
            }
        else if ( devSoundCaps.iRate & EMMFSampleRate8000Hz)
            {
            // this one leaves if the rate is not supported
            PRINT((_L("CCMRAudioInput::ConfigDevSoundL() unsupported audio samplerate - trying 8k")));
            iCodecData->SetBitRateL( 16000 );
            iCodecData->SetSampleRateL( 8000 );
    		devSoundConfig.iRate = EMMFSampleRate8000Hz;
            }
        else
            {
            PRINT((_L("CCMRAudioInput::ConfigDevSoundL() unsupported sample rate %d"), sampleRate));
    		User::Leave( KErrNotSupported );
            }
		}
	devSound->SetConfigL(devSoundConfig);
    PRINT((_L("CCMRAudioInput::ConfigDevSoundL() out")));
	}

// -----------------------------------------------------------------------------
// CCMRAudioInput::UpdateTimeL
//
// -----------------------------------------------------------------------------
//
void CCMRAudioInput::UpdateTimeL()
    {
    PRINT((_L("CCMRAudioInput::UpdateTimeL() in")));
    MMMFAudioInput* audioInput = static_cast<MMMFAudioInput*>(iMMFDataSource);
    CMMFDevSound* devSound = &audioInput->SoundDevice();
    TInt sampleRate = iCodecData->GetSampleRate();

    if ( sampleRate )
        {
        TInt64 samples = devSound->SamplesRecorded();
        PRINT((_L("CCMRAudioInput::UpdateTimeL() samples recorded %Ld"), samples));

        samples = samples * 1000000;
        samples = samples / sampleRate;
        iLatestTimeStamp = samples + iTimeStampWhenPaused.Int64();
        }
    else
        {
        iLatestTimeStamp = 0;
        }

    // only add start delay if we haven't been paused. Only add delay after first audio available
	if ( (iTimeStampWhenPaused == 0) && (iLatestTimeStamp != 0) )
		{
		TInt initialDelay = 0;
	    // Add A/V sync constant to timestamp
		if ( iConfig && iConfig->IsICMConfigDataAvailable() )
			{
			PRINT((_L("CCMRAudioInput::UpdateTimeL(), using ICM setting value for AVsync delay")));
			initialDelay = iConfig->VideoQualitySettings().iAVSyncStartDelay;
			}
		else
			{
			PRINT((_L("CCMRAudioInput::UpdateTimeL(), using old setting value for AVsync delay")));
			initialDelay = KCMRInitialVideoAudioTimeSyncDC;
			}

		if ( initialDelay < 0 )
			{
			if ( iLatestTimeStamp > -( initialDelay*1000) )
				{
				iLatestTimeStamp = iLatestTimeStamp.Int64() + ( initialDelay*1000 );
				}
			else
				{
				iLatestTimeStamp = 0;
				}
			}
		else
			{
			iLatestTimeStamp = iLatestTimeStamp.Int64() + ( initialDelay*1000 );
			}
		}

	if ( iLatestTimeStamp != 0 )
		{
	    iTimeWhenSamplesUpdated.UniversalTime();
	    iSamplesUpdatedAfterStart = ETrue;
		}
    PRINT((_L("CCMRAudioInput::UpdateTimeL() out")));
    }

// -----------------------------------------------------------------------------
// CCMRAudioInput::InitializeComplete
//
// -----------------------------------------------------------------------------
//
void CCMRAudioInput::InitializeComplete(TInt aError)
    {
    PRINT((_L("CCMRAudioInput::InitializeComplete() in, error:%d"), aError));
    TInt error = aError;
    if ( error == KErrNone )
        {
        MMMFAudioInput* audioInput = static_cast<MMMFAudioInput*>(iMMFDataSource);
        PRINT((_L("CCMRAudioInput::InitializeComplete() SetPrioritySettings")));
        audioInput->SoundDevice().SetPrioritySettings(iPrioritySettings);
        PRINT((_L("CCMRAudioInput::InitializeComplete() ConfigDevSoundL")));
        TRAP(error, ConfigDevSoundL());
        if (error == KErrNone)
            {
            PRINT((_L("CCMRAudioInput::InitializeComplete() AllocateInputBuffersL")));
            TRAP(error, AllocateInputBuffersL());
            }
        else
            {
            PRINT((_L("CCMRAudioInput::InitializeComplete() ConfigDevSoundL, error=%d"), error));
            }
        }
    PRINT((_L("CCMRAudioInput::InitializeComplete() SendEventToClient KMMFEventCategoryAudioLoadingComplete, error: %d"), error));
    TMMFEvent event(KMMFEventCategoryAudioLoadingComplete, error);
    iEventHandler.SendEventToClient(event);
    PRINT((_L("CCMRAudioInput::InitializeComplete() out")));
    }

// -----------------------------------------------------------------------------
// CCMRAudioInput::ToneFinished
//
// -----------------------------------------------------------------------------
//
void CCMRAudioInput::ToneFinished(TInt aError)
    {
    UNUSED(aError);
    PRINT((_L("CCMRAudioInput::ToneFinished(), error:%d"), aError));
    }

// -----------------------------------------------------------------------------
// CCMRAudioInput::BufferToBeFilled
//
// -----------------------------------------------------------------------------
//
void CCMRAudioInput::BufferToBeFilled(CMMFBuffer* /*aBuffer*/)
    {
    PRINT((_L("CCMRAudioInput::BufferToBeFilled()")));
    }

// -----------------------------------------------------------------------------
// CCMRAudioInput::PlayError
//
// -----------------------------------------------------------------------------
//
void CCMRAudioInput::PlayError(TInt aError)
    {
    UNUSED(aError);
    PRINT((_L("CCMRAudioInput::PlayError(), error:%d"), aError));
    }

// -----------------------------------------------------------------------------
// CCMRAudioInput::BufferToBeEmptied
//
// -----------------------------------------------------------------------------
//
void CCMRAudioInput::BufferToBeEmptied(CMMFBuffer* aBuffer)
    {
    PRINT((_L("CCMRAudioInput::BufferToBeEmptied()")));
    TInt error = KErrNone;
    TRAP(error, BufferFilledL(aBuffer));
    if (error != KErrNone)
        {
        TMMFEvent event (KMMFEventCategoryPlaybackComplete, error);
        SendEventToClient( event );
        }
    }

// -----------------------------------------------------------------------------
// CCMRAudioInput::RecordError
//
// -----------------------------------------------------------------------------
//
void CCMRAudioInput::RecordError(TInt aError)
    {
    PRINT((_L("CCMRAudioInput::RecordError(), error=%d"), aError));
    TMMFEvent event (KMMFEventCategoryPlaybackComplete, aError);
    iEventHandler.SendEventToClient(event);
    }

// -----------------------------------------------------------------------------
// CCMRAudioInput::ConvertError
//
// -----------------------------------------------------------------------------
//
void CCMRAudioInput::ConvertError(TInt aError)
    {
    UNUSED(aError);
    PRINT((_L("CCMRAudioInput::ConvertError(), error:%d"), aError));
    }

// -----------------------------------------------------------------------------
// CCMRAudioInput::DeviceMessage
//
// -----------------------------------------------------------------------------
//
void CCMRAudioInput::DeviceMessage(TUid /*aMessageType*/, const TDesC8& /*aMsg*/)
    {
    PRINT((_L("CCMRAudioInput::DeviceMessage()")));
    }


// End of file



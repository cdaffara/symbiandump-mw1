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
* Description:  Camcorder controller plugin
*
*/



// INCLUDE FILES
#include <mmf/common/mmfcontroller.h>
#include <mmf/common/mmfstandardcustomcommands.h>
#include <mmf/server/mmffile.h>
#include <badesca.h>    // CDesC8ArrayFlat
#include <AudioPreference.h>                // For MMF audio preference definitions.
#include <CamCControllerCustomCommands.h>
#include "CamC3GPDataSink.h"
#include "CCMRMediaRecorder.h"
#include "CamC3GPDataSinkObserver.h"
#include "CamCController.h"
#include "CCMRSupportedCodecs.h"
#include "CamCImplementationUIDs.hrh"

// LOCAL CONSTANTS AND MACROS
// Debug print macro
#if defined _DEBUG
#include <e32svr.h>
#define PRINT(x) RDebug::Print x;
#else
#define PRINT(x)
#endif

#include "OstTraceDefinitions.h"
#ifdef OST_TRACE_COMPILER_IN_USE
#include "CamCControllerTraces.h"
#endif
// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CCamCController::NewL
// Two-phased constructor.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
CCamCController* CCamCController::NewL()
    {
    PRINT((_L("CCamCController::NewL() in ")));

    CCamCController* self = new(ELeave) CCamCController();
    CleanupStack::PushL(self);
    self->ConstructL();
    CleanupStack::Pop();

    PRINT((_L("CCamCController::NewL() out ")));
    return self;
    }

// -----------------------------------------------------------------------------
// CCamCController::~CCamCController
// Destructor
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
CCamCController::~CCamCController()
    {
    OstTrace0( CAMERASRV_PERFORMANCE, CCAMCCONTROLLER_CCAMCCONTROLLER, "e_CCamCController::~CCamCController 1" );
    PRINT((_L("CCamCController::~CCamCController enter      ")));

    delete iMediaRecorder;
    iMediaRecorder = NULL;

    PRINT((_L("CCamCController::~CCamCController mediarecorder deleted      ")));

    delete iFileComposer;
    iFileComposer = NULL;

    PRINT((_L("CCamCController::~CCamCController filecomposer deleted      ")));

    delete iVideoCodec;
    iVideoCodec = NULL;

    if ( iSupportedVideoTypes )
        {
        iSupportedVideoTypes->Reset();
        delete iSupportedVideoTypes;
        iSupportedVideoTypes = NULL;
        }

    iSupportedAudioTypes.Close();
    delete iIdleStop;

    PRINT((_L("CCamCController::~CCamCController exit      ")));
    OstTrace0( CAMERASRV_PERFORMANCE, DUP1_CCAMCCONTROLLER_CCAMCCONTROLLER, "e_CCamCController::~CCamCController 0" );
    }

// -----------------------------------------------------------------------------
// CCamCController::ConstructL
// Symbian 2nd phase constructor can leave.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CCamCController::ConstructL()
    {
    OstTrace0( CAMERASRV_PERFORMANCE, CCAMCCONTROLLER_CONSTRUCTL, "e_CCamCController::ConstructL 1" );
    PRINT((_L("CCamCController::ConstructL() in")));

    iMediaRecorder = NULL;
    iFileComposer = NULL;
    iState = EStateNone;
    iVideoCodec = 0;
    iAudioCodec = 0;
    iCameraHandle = -1;
    iVideoFormat = CCamC3GPDataSink::E3GPP;
    iFileComposerReady = EFalse;
    iMRPrepareCompleteSent = EFalse;
    iForceDataSinkFileName = EFalse;
    iSavedStopError = KErrNone;
    iAsyncStop = EFalse;
    iPrioritySettings.iPriority = KAudioPriorityVideoRecording;
    iPrioritySettings.iPref =  TMdaPriorityPreference( KAudioPrefVideoRecording );

    iSupportedVideoTypes = new(ELeave) CDesC8ArrayFlat( 2 ); // 2 is the granularity of the array
    iSupportedVideoTypes->Reset();
    iSupportedAudioTypes.Reset();

    //custom command parsers
    CMMFVideoControllerCustomCommandParser* vidConPaser =
                                        CMMFVideoControllerCustomCommandParser::NewL(*this);
    CleanupStack::PushL(vidConPaser);
    AddCustomCommandParserL(*vidConPaser); //parser now owned by controller framework
    CleanupStack::Pop(vidConPaser);

    CMMFVideoRecordControllerCustomCommandParser* vidRecParser =
                                        CMMFVideoRecordControllerCustomCommandParser::NewL(*this);
    CleanupStack::PushL(vidRecParser);
    AddCustomCommandParserL(*vidRecParser); //parser now owned by controller framework
    CleanupStack::Pop(vidRecParser);

    CMMFAudioRecordDeviceCustomCommandParser* audRecParser =
                                        CMMFAudioRecordDeviceCustomCommandParser::NewL(*this);
    CleanupStack::PushL(audRecParser);
    AddCustomCommandParserL(*audRecParser); //parser now owned by controller framework
    CleanupStack::Pop(audRecParser);

    // Idle priority Active object for async video stop operation
    iIdleStop = CIdle::NewL( CActive::EPriorityIdle );

    PRINT((_L("CCamCController::ConstructL() out")));
    OstTrace0( CAMERASRV_PERFORMANCE, DUP1_CCAMCCONTROLLER_CONSTRUCTL, "e_CCamCController::ConstructL 0" );
    }

// -----------------------------------------------------------------------------
// CCamCController::AddDataSourceL
// Add data source to controller.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CCamCController::AddDataSourceL(MDataSource& aDataSource)
    {
    PRINT((_L("CCamCController::AddDataSourceL() in")));

    if( iState != EStateNone )
        {
        User::Leave( KErrNotReady );
        }

    if (!iMediaRecorder)
        {
        iMediaRecorder = CCMRMediaRecorder::NewL();
        }

    if (aDataSource.DataSourceType()==KUidMmfAudioInput)
        {
        iAudioSource = &aDataSource;
        }

    if ( ReadyToOpenSource() )
        {
        OpenSourceL();
        }

    PRINT((_L("CCamCController::AddDataSourceL() out")));
    }

// -----------------------------------------------------------------------------
// CCamCController::AddDataSinkL
// Add data sink to controller.
// (other items were commented in a header).
//
void CCamCController::AddDataSinkL(MDataSink& aDataSink)
    {
    PRINT((_L("CCamCController::AddDataSinkL() in")));

    if( iState != EStateNone )
        {
        User::Leave( KErrNotReady );
        }

    if (aDataSink.DataSinkType()==KUidMmfFileSink)
        {
        iMMFFile = STATIC_CAST(CMMFFile*, &aDataSink);
        iFileName = iMMFFile->FullName();
        iForceDataSinkFileName = EFalse;

        if (!iFileComposer)
            {
            iFileComposer = CCamC3GPDataSink::NewL(this);
            }

        if ( ReadyToOpenSource() )
            {
            OpenSourceL();
            }
        }
    else
        {
        User::Leave(KErrNotSupported);
        }

    PRINT((_L("CCamCController::AddDataSinkL() out")));
    }

// -----------------------------------------------------------------------------
// CCamCController::RemoveDataSourceL
// Remove data source from controller.
// (other items were commented in a header).
//
void CCamCController::RemoveDataSourceL(MDataSource& /*aDataSource*/)
    {
    // Not called from Video Recorder client API
    PRINT((_L("CCamCController::RemoveDataSourceL() Not Supported - leaving")));
    User::Leave( KErrNotSupported );
    }

// -----------------------------------------------------------------------------
// CCamCController::RemoveDataSinkL
// Remove data sink from controller.
// (other items were commented in a header).
//
void CCamCController::RemoveDataSinkL(MDataSink& /*aDataSink*/)
    {
    // Not called from Video Recorder client API
    PRINT((_L("CCamCController::RemoveDataSinkL() Not Supported - leaving")));
    User::Leave( KErrNotSupported );
    }

// -----------------------------------------------------------------------------
// CCamCController::ResetL
// Reset controller
// (other items were commented in a header).
//
void CCamCController::ResetL()
    {
    // Not called from Video Recorder client API
    PRINT((_L("CCamCController::ResetL() Not Supported - leaving")));
    User::Leave( KErrNotSupported );
    }

// -----------------------------------------------------------------------------
// CCamCController::PrimeL
// Primes controller.
// (other items were commented in a header).
//
void CCamCController::PrimeL()
    {
    // Preparing to record is handled by MvrcPrepareL
    }

// -----------------------------------------------------------------------------
// CCamCController::PlayL
// Start recording.
// (other items were commented in a header).
//
void CCamCController::PlayL()
    {
    OstTrace0( CAMERASRV_PERFORMANCE, CCAMCCONTROLLER_PLAYL, "e_CCamCController::PlayL 1" );
    PRINT((_L("CCamCController::PlayL() in")));

    if ( iState == EStateRecording )
        {
        OstTrace0( CAMERASRV_PERFORMANCE, DUP1_CCAMCCONTROLLER_PLAYL, "e_CCamCController::PlayL 0" );
        return;
        }

    if ( ( iState != EStatePrepared ) && ( iState != EStatePaused ) )
        {
        User::Leave(KErrNotReady);
        }

    iSavedStopError = KErrNone;

    if ( iState == EStatePrepared )
        {
        if ( iFileComposerReady == EFalse ) // to handle record, stop, record (without prepare)
            {
            if ( iMediaRecorder->AudioEnabledL() )
                {
                iMediaRecorder->GetAudioCodecL(iAudioCodec);
                }
            else
                {
                iAudioCodec = KFourCCNULL;
                }

            TInt error = KErrNone;
            if( iForceDataSinkFileName )
                {
                TRAP( error, iFileComposer->OpenFileL( iFileName, iAudioCodec, *iVideoCodec, iVideoFormat ) );
                }
            else
                {
                TRAP( error, iFileComposer->OpenFileL( iMMFFile, iAudioCodec, *iVideoCodec, iVideoFormat ) );
                }
            if ( error != KErrNone )
                {
                DoSendEventToClient(TMMFEvent(KMMFEventCategoryPlaybackComplete, error));
                return;
                }
            else
                {
                iFileComposerReady = ETrue;
                }
            }
        iMediaRecorder->RecordL();
        }
    else
        {
        iMediaRecorder->ResumeL();
        }
    iState = EStateRecording;
    PRINT((_L("CCamCController::PlayL() out")));
    OstTrace0( CAMERASRV_PERFORMANCE, DUP2_CCAMCCONTROLLER_PLAYL, "e_CCamCController::PlayL 0" );
    }

// -----------------------------------------------------------------------------
// CCamCController::PauseL
// Pause recording.
// (other items were commented in a header).
//
void CCamCController::PauseL()
    {
    PRINT((_L("CCamCController::PauseL() in")));

    if ( iState != EStateRecording )
        {
        User::Leave(KErrNotReady);
        }
    iMediaRecorder->PauseL();
    iState = EStatePaused;
    PRINT((_L("CCamCController::PauseL() out")));
    }

// -----------------------------------------------------------------------------
// CCamCController::StopL
// Stop recording.
// (other items were commented in a header).
//
void CCamCController::StopL()
    {
    OstTrace0( CAMERASRV_PERFORMANCE, CCAMCCONTROLLER_STOPL, "e_CCamCController::StopL 1" );
    PRINT((_L("CCamCController::StopL() in")));

    if ( ( iState != EStateRecording ) && ( iState != EStatePaused ) )
        {
        PRINT((_L("CCamCController::StopL() out - not recording")));
        if ( iSavedStopError != KErrNone )
            {
            User::Leave( iSavedStopError );
            }
        else
            {
            return;
            }
        }

    iState = EStateStopping;
    TInt error = KErrNone;
    TRAP( error, iMediaRecorder->StopL() );
    if ( error != KErrNone )
        {
        PRINT((_L("CCamCController::StopL() MediaRecorderStop error= %d      "), error));
        iState = EStatePrepared;
        iFileComposerReady = EFalse;
        TRAPD( ignored, iFileComposer->SinkStopL() );
        ignored++; // to remove compile warning. There has already been error, further errors are ignored.
        if ( !iAsyncStop )
	        {
    	    User::Leave( error );
        	}
        }

    PRINT((_L("CCamCController::StopL() iAsyncStop=%d      "), iAsyncStop));
	if ( iAsyncStop )
		{
        PRINT((_L("CCamCController::StopL() sending event KCamCControllerCCVideoStoppedUid, error=%d      "), error));
		DoSendEventToClient(TMMFEvent(KCamCControllerCCVideoRecordStopped, error));

        if ( !iIdleStop->IsActive() )
            {
            iIdleStop->Start( TCallBack( IdleStop, this ) );
            }

		}
    else // Sync stop
        {
        error = KErrNone;
        TRAP( error, iFileComposer->SinkStopL() );
        if ( error != KErrNone )
            {
            PRINT((_L("CCamCController::StopL() FileComposerStop error= %d      "), error));
            iState = EStatePrepared;
            iFileComposerReady = EFalse;
      	    User::Leave( error );
            }

    	if ( (iSavedStopError == KErrDiskFull) || (iSavedStopError == KErrCompletion ) || (iSavedStopError == KErrNone) )
	    	{
    	    iState = EStatePrepared;
    		}
	    else
		    {
    		// Fatal error happened, need to prepare again.
	    	iState = EStateOpen;
		    }
        iFileComposerReady = EFalse;
        }

    PRINT((_L("CCamCController::StopL() out")));
    OstTrace0( CAMERASRV_PERFORMANCE, DUP1_CCAMCCONTROLLER_STOPL, "e_CCamCController::StopL 0" );
    }

// -----------------------------------------------------------------------------
// CCamCController::PositionL
// Returns current recording position.
// (other items were commented in a header).
//
TTimeIntervalMicroSeconds CCamCController::PositionL() const
    {
     // Not called from Video Recorder client API
    return TTimeIntervalMicroSeconds(0);
    }

// -----------------------------------------------------------------------------
// CCamCController::SetPositionL
// Not supported.
// (other items were commented in a header).
//
void CCamCController::SetPositionL(const TTimeIntervalMicroSeconds& /*aPosition*/)
    {
    User::Leave(KErrNotSupported);
    }

// -----------------------------------------------------------------------------
// CCamCController::DurationL
// Current duration of recording.
// (other items were commented in a header).
//
TTimeIntervalMicroSeconds CCamCController::DurationL() const
    {
    if ( iFileComposer )
        {
        return iFileComposer->GetElapsedTime();
        }
    else
        {
        return TTimeIntervalMicroSeconds(0);
        }
    }

// -----------------------------------------------------------------------------
// CCamCController::SetPrioritySettings
// Set camcorder priority settings.
// (other items were commented in a header).
//
void CCamCController::SetPrioritySettings(const TMMFPrioritySettings& aPrioritySettings)
    {
    PRINT((_L("CCamCController::SetPrioritySettings() in")));
    TInt error = KErrNone;
    iPrioritySettings = aPrioritySettings;

    if ( iState == EStatePrepared )
        {
            iState = EStateOpen;
        }

    if ( iMediaRecorder )
        {
        if ( iState != EStateOpen )
            {
            error = KErrNotReady;
            }
        else
            {
            TRAP(error, iMediaRecorder->SetAudioPriorityL( iPrioritySettings ));
            }
        }

    if ( error != KErrNone )
        {
        DoSendEventToClient(TMMFEvent(KMMFEventCategoryVideoRecorderGeneralError, error));
        }

    PRINT((_L("CCamCController::SetPrioritySettings() out")));
    }

// -----------------------------------------------------------------------------
// CCamCController::CustomCommand
// Handle custom commands to controller.
// (other items were commented in a header).
//
void CCamCController::CustomCommand( TMMFMessage& aMessage )
    {
    OstTrace0( CAMERASRV_PERFORMANCE, CCAMCCONTROLLER_CUSTOMCOMMAND, "e_CCamCController::CustomCommand 1" );
    PRINT((_L("CCamCController::CustomCommand() in")));
    if ( aMessage.Destination().InterfaceId().iUid != KCamCUidControllerImplementation )
        {
        aMessage.Complete(KErrNotSupported);
        PRINT((_L("CCamCController::CustomCommand() KErrNotSupported out")));
        return;
        }

    TInt error = KErrNone;
    switch ( aMessage.Function() )
        {
        case ECamCControllerCCNewFilename:
            {
            PRINT((_L("CCamCController::CustomCommand() ECamCControllerCCNewFilename")));
            TRAP( error, NewFilenameL( aMessage ) );
            aMessage.Complete(error);
            break;
            }

        case ECamCControllerCCVideoStopAsync :
            {
            PRINT((_L("CCamCController::CustomCommand() ECamCControllerCCVideoStopAsync")));
            aMessage.Complete(KErrNone);
            iAsyncStop = ETrue;
            TRAPD( ignored, StopL() );
            ignored++; // to remove compile warning. There has already been error, further errors are ignored.
            break;
            }
        default:
            {
            PRINT((_L("CCamCController::CustomCommand() invalid command")));
            error = KErrNotSupported;
            aMessage.Complete(error);
            break;
            }
        }
    PRINT((_L("CCamCController::CustomCommand() out")));
    OstTrace0( CAMERASRV_PERFORMANCE, DUP1_CCAMCCONTROLLER_CUSTOMCOMMAND, "e_CCamCController::CustomCommand 0" );
    }

// -----------------------------------------------------------------------------
// CCamCController::GetNumberOfMetaDataEntriesL
// Get number of metadata entries.
// (other items were commented in a header).
//
void CCamCController::GetNumberOfMetaDataEntriesL(TInt& aNumberOfEntries)
    {
    aNumberOfEntries = 0;
    }

// -----------------------------------------------------------------------------
// CCamCController::GetMetaDataEntryL
// Returns metadata entry.
// (other items were commented in a header).
//
CMMFMetaDataEntry* CCamCController::GetMetaDataEntryL(TInt /*aIndex*/)
    {
    User::Leave( KErrNotSupported );
    return NULL;
    }

// -----------------------------------------------------------------------------
// CCamCController::MmroPrepareComplete
// Called when mediarecorder has completed prepare command.
// (other items were commented in a header).
//
void CCamCController::MmroPrepareComplete(TInt aError)
    {
    OstTrace0( CAMERASRV_PERFORMANCE, CCAMCCONTROLLER_MMROPREPARECOMPLETE, "e_CCamCController::MmroPrepareComplete 1" );
    PRINT((_L("CCamCController::MmroPrepareComplete() error= %d      "), aError));

    if ( aError == KErrNone )
        {
        iState = EStatePrepared;
        }
	if ( iFileComposerReady && !iMRPrepareCompleteSent )
		{
	    iMRPrepareCompleteSent = ETrue;
	    DoSendEventToClient(TMMFEvent(KMMFEventCategoryVideoPrepareComplete, aError));
		}
    OstTrace0( CAMERASRV_PERFORMANCE, DUP1_CCAMCCONTROLLER_MMROPREPARECOMPLETE, "e_CCamCController::MmroPrepareComplete 0" );
    }

// -----------------------------------------------------------------------------
// CCamCController::MmroStateChange
// Called when mediarecorder has changed state.
// (other items were commented in a header).
//
void CCamCController::MmroStateChange(TInt aState, TInt /*aError*/)
    {
    PRINT((_L("CCamCController::MmroStateChange()       ")));

    if ( ( aState == CCMRMediaRecorder::EStateOpen ) &&
         ( iState == EStatePrepared ) )
        {
        iState = EStateOpen;
        }
    }

// -----------------------------------------------------------------------------
// CCamCController::MmroTemporaryError
// Called when temporary error happens in mediarecorder.
// (other items were commented in a header).
//
void CCamCController::MmroTemporaryError(TInt /*aError*/)
    {
    PRINT((_L("CCamCController::MmroTemporaryError()        ")));
    }

// -----------------------------------------------------------------------------
// CCamCController::MmroFatalError
// Called when fatal error happens in mediarecorder.
// (other items were commented in a header).
//
void CCamCController::MmroFatalError(TInt aError)
    {
    PRINT((_L("CCamCController::MmroFatalError() error=%d       "), aError));

    if ( ( iState == EStateRecording ) || ( iState == EStatePaused ) )
        {
        iSavedStopError = aError;
        TInt error;
        TRAP( error, StopL() );
        PRINT(_L("CCamCController::MmroFatalError() Reporting PlaybackComplete"));
        DoSendEventToClient(TMMFEvent(KMMFEventCategoryPlaybackComplete, aError));
        }
    else if ( iState == EStatePrepared )
        {
        iState = EStateOpen;
        PRINT(_L("CCamCController::MmroFatalError() Reporting VR GeneralError"));
        DoSendEventToClient(TMMFEvent(KMMFEventCategoryVideoRecorderGeneralError, aError));
        }
    else
        {
        }
    }

// -----------------------------------------------------------------------------
// CCamCController::MfcoDiskFullL
// Called when filecomposer runs out of disk space.
// (other items were commented in a header).
//
void CCamCController::MfcoDiskFullL()
    {
    PRINT((_L("CCamCController::DiskFullL() in")));
    if ( ( iState == EStateRecording ) || ( iState == EStatePaused ) )
        {
        iSavedStopError = KErrDiskFull;
        TInt error;
        TRAP( error, StopL() );
        DoSendEventToClient(TMMFEvent(KMMFEventCategoryPlaybackComplete, KErrDiskFull));
        }
    PRINT((_L("CCamCController::DiskFullL() out")));
    }

// -----------------------------------------------------------------------------
// CCamCController::MfcoSizeLimitReachedL
// Called when filecomposer meets maximum file size.
// (other items were commented in a header).
//
void CCamCController::MfcoSizeLimitReachedL()
    {
    PRINT((_L("CCamCController::SizeLimitReachedL() in")));
    if ( iState != EStateStopping )
        {
        iSavedStopError = KErrCompletion;
        TInt error;
        TRAP( error, StopL() );
        DoSendEventToClient(TMMFEvent(KMMFEventCategoryPlaybackComplete, KErrCompletion));
        }
    PRINT((_L("CCamCController::SizeLimitReachedL() out")));
    }

// -----------------------------------------------------------------------------
// CCamCController::MvcGetVideoFrameSizeL
// Get video frame size.
// (other items were commented in a header).
//
void CCamCController::MvcGetVideoFrameSizeL(TSize& aVideoFrameSize)
    {
    if ( iMediaRecorder )
        {
        iMediaRecorder->GetVideoFrameSizeL(aVideoFrameSize);
        }
    else
        {
        User::Leave( KErrNotReady );
        }
    }

// -----------------------------------------------------------------------------
// CCamCController::MvcGetAudioCodecL
// Get audio codec.
// (other items were commented in a header).
//
void CCamCController::MvcGetAudioCodecL(TFourCC& aCodec)
    {
    if ( iMediaRecorder )
        {
        iMediaRecorder->GetAudioCodecL( aCodec );
        }
    else
        {
        User::Leave( KErrNotReady );
        }
    }

// -----------------------------------------------------------------------------
// CCamCController::MvcGetVideoBitRateL
// Get video bit rate.
// (other items were commented in a header).
//
void CCamCController::MvcGetVideoBitRateL(TInt& aBitRate)
    {
    if ( iMediaRecorder )
        {
        aBitRate = iMediaRecorder->VideoBitRateL();
        }
    else
        {
        User::Leave( KErrNotReady );
        }
    }

// -----------------------------------------------------------------------------
// CCamCController::MvcGetAudioBitRateL
// Get audio bit rate.
// (other items were commented in a header).
//
void CCamCController::MvcGetAudioBitRateL(TInt& aBitRate)
    {
    if ( iMediaRecorder )
        {
        aBitRate = iMediaRecorder->AudioBitRateL();
        }
    else
        {
        User::Leave( KErrNotReady );
        }
    }

// -----------------------------------------------------------------------------
// CCamCController::MvcSetFrameRateL
// Set video frame rate.
// (other items were commented in a header).
//
void CCamCController::MvcSetFrameRateL(TReal32 aFramesPerSecond)
    {
    if ( iMediaRecorder )
        {
        iMediaRecorder->SetVideoFrameRateL(aFramesPerSecond);
        }
    else
        {
        User::Leave( KErrNotReady );
        }
    }

// -----------------------------------------------------------------------------
// CCamCController::MvcGetFrameRateL
// Get video frame rate.
// (other items were commented in a header).
//
void CCamCController::MvcGetFrameRateL(TReal32& aFramesPerSecond)
    {
    if ( iMediaRecorder )
        {
        aFramesPerSecond = iMediaRecorder->VideoFrameRateL();
        }
    else
        {
        User::Leave( KErrNotReady );
        }
    }

// -----------------------------------------------------------------------------
// CCamCController::MvrcGetSupportedSinkAudioTypesL
// Get supported audio sink types.
// (other items were commented in a header).
//
void CCamCController::MvrcGetSupportedSinkAudioTypesL(RArray<TFourCC>& aDataTypes)
    {
    if ( iMediaRecorder )
        {
        aDataTypes.Reset();
        iMediaRecorder->GetSupportedAudioCodecsL( aDataTypes );
        }
    else
        {
        User::Leave( KErrNotReady );
        }
    }

// -----------------------------------------------------------------------------
// CCamCController::MvrcGetSupportedSinkVideoTypesL
// Get supported video sink types.
// (other items were commented in a header).
//
void CCamCController::MvrcGetSupportedSinkVideoTypesL(CDesC8Array& aDataTypes)
    {
    if ( iMediaRecorder )
        {
        aDataTypes.Reset();
        iMediaRecorder->GetSupportedVideoCodecsL( aDataTypes );
        }
    else
        {
        User::Leave( KErrNotReady );
        }
    }

// -----------------------------------------------------------------------------
// CCamCController::MvcGetVideoMimeTypeL
// Get video mime type.
// (other items were commented in a header).
//
void CCamCController::MvcGetVideoMimeTypeL(TDes8& aMimeType)
    {
    if ( iMediaRecorder )
        {
        iMediaRecorder->GetVideoCodecL( aMimeType );
        }
    else
        {
        User::Leave( KErrNotReady );
        }
    }

// -----------------------------------------------------------------------------
// CCamCController::MvrcSetVideoFormatL
// Set video format.
// (other items were commented in a header).
//
void CCamCController::MvrcSetVideoFormatL(TUid aVideoFormatUid)
    {
    PRINT((_L("CCamCController::MvrcSetVideoFormatL() in")));

    if ( iState == EStatePrepared )
        {
        iState = EStateOpen;
        }

    if ( ( aVideoFormatUid.iUid == KCamCUidRecordFormatImplementation ) ||
         ( aVideoFormatUid == KNullUid ) )
        {
        iVideoFormat = CCamC3GPDataSink::E3GPP;
        }
    else if ( aVideoFormatUid.iUid == KCamCUid3GPP2FileFormatImplementation )
        {
        iVideoFormat = CCamC3GPDataSink::E3GPP2;
        }
    else if ( aVideoFormatUid.iUid == KCamCUidMP4FileFormatImplementation )
        {
        iVideoFormat = CCamC3GPDataSink::EMPEG4;
        }
    else
        {
        User::Leave( KErrNotSupported );
        }

    if ( ReadyToOpenSource() )
        {
        OpenSourceL();
        }

    PRINT((_L("CCamCController::MvrcSetVideoFormatL() out")));
    }

// -----------------------------------------------------------------------------
// CCamCController::MvrcSetVideoCodecL
// Set video codec.
// (other items were commented in a header).
//
void CCamCController::MvrcSetVideoCodecL(const TDesC8& aVideoCodec)
    {
    PRINT((_L("CCamCController::MvrcSetVideoCodecL() in")));

    if ( iState == EStatePrepared )
        {
        iState = EStateOpen;
        }

    if ( ( iState != EStateOpen ) && ( iState != EStateNone ) )
        {
        User::Leave(KErrNotReady);
        }

    if ( iMediaRecorder )
        {
        iSupportedVideoTypes->Reset();
        iMediaRecorder->GetSupportedVideoCodecsL( *iSupportedVideoTypes );
        }
    else
        {
        User::Leave(KErrNotReady);
        }

    TBool found = EFalse;
    if ( aVideoCodec == KNullDesC8 )
        {
        iVideoCodec = aVideoCodec.AllocL();
        found = ETrue;
        }
    else
        {
        for( TInt i = 0; i < iSupportedVideoTypes->Count(); i++ )
            {
            if ( iSupportedVideoTypes->MdcaPoint(i) == aVideoCodec )
                {
                if ( iVideoCodec )
                    {
                    delete iVideoCodec;
                    iVideoCodec = NULL;
                    }
                iVideoCodec = aVideoCodec.AllocL();
                found = ETrue;
                }
            }
        }

    if ( !found )
        {
        User::Leave(KErrNotSupported);
        }

    if ( iState == EStateOpen )
        {
        iMediaRecorder->SetVideoCodecL( *iVideoCodec );
        }
    else if ( iState == EStateNone )
        {
        if ( ReadyToOpenSource() )
            {
            OpenSourceL();
            }
        }
    else
        {
        }
    PRINT((_L("CCamCController::MvrcSetVideoCodecL() out")));
    }

// -----------------------------------------------------------------------------
// CCamCController::MvrcSetAudioCodecL
// Set audio codec.
// (other items were commented in a header).
//
void CCamCController::MvrcSetAudioCodecL(TFourCC aAudioCodec)
    {
    PRINT((_L("CCamCController::MvrcSetAudioCodecL() in")));

    if ( iState == EStatePrepared )
        {
        iState = EStateOpen;
        }

    if ( ( iState != EStateOpen ) && ( iState != EStateNone ) )
        {
        User::Leave(KErrNotReady);
        }

    if ( iMediaRecorder )
        {
        iSupportedAudioTypes.Reset();
        iMediaRecorder->GetSupportedAudioCodecsL( iSupportedAudioTypes );
        }
    else
        {
        User::Leave(KErrNotReady);
        }

    TBool found = EFalse;
    if ( aAudioCodec == KFourCCNULL )
        {
        iAudioCodec = aAudioCodec;
        found = ETrue;
        }
    else
        {
        for( TInt i = 0; i < iSupportedAudioTypes.Count(); i++ )
            {
            if ( iSupportedAudioTypes[i] == aAudioCodec )
                {
                iAudioCodec = aAudioCodec;
                found = ETrue;
                }
            }
        }

    if ( !found )
        {
        User::Leave(KErrNotSupported);
        }

    if ( iState == EStateOpen )
        {
        iMediaRecorder->SetAudioCodecL( iAudioCodec );
        }
    else if ( iState == EStateNone )
        {
        if ( ReadyToOpenSource() )
            {
            OpenSourceL();
            }
        }
    else
        {
        }
    PRINT((_L("CCamCController::MvrcSetAudioCodecL() out")));
    }

// -----------------------------------------------------------------------------
// CCamCController::MvrcSetVideoBitRateL
// Set video bit rate.
// (other items were commented in a header).
//
void CCamCController::MvrcSetVideoBitRateL(TInt& aBitRate)
    {
    PRINT((_L("CCamCController::MvrcSetVideoBitRateL() in")));

    if ( iMediaRecorder )
        {
        iMediaRecorder->SetVideoBitRateL(aBitRate);
        }
    else
        {
        User::Leave( KErrNotReady );
        }
    PRINT((_L("CCamCController::MvrcSetVideoBitRateL() out")));
    }

// -----------------------------------------------------------------------------
// CCamCController::MvrcSetAudioBitRateL
// Set audio bit rate.
// (other items were commented in a header).
//
void CCamCController::MvrcSetAudioBitRateL(TInt& aBitRate)
    {
    PRINT((_L("CCamCController::MvrcSetAudioBitRateL() in")));

    if ( iState == EStatePrepared )
        {
        iState = EStateOpen;
        }

    if ( iMediaRecorder )
        {
        if ( iState != EStateOpen )
            {
            User::Leave(KErrNotReady);
            }
        iMediaRecorder->SetAudioBitRateL(aBitRate);
        }
    else
        {
        User::Leave( KErrNotReady );
        }
    PRINT((_L("CCamCController::MvrcSetAudioBitRateL() out")));
    }

// -----------------------------------------------------------------------------
// CCamCController::MvrcAddMetaDataEntryL
// Add metadata entry.
// (other items were commented in a header).
//
void CCamCController::MvrcAddMetaDataEntryL(const CMMFMetaDataEntry& /*aNewEntry*/)
    {
    User::Leave(KErrNotSupported);
    }

// -----------------------------------------------------------------------------
// CCamCController::MvrcRemoveMetaDataEntryL
// Remove metadata entry.
// (other items were commented in a header).
//
void CCamCController::MvrcRemoveMetaDataEntryL(TInt /*aIndex*/)
    {
    User::Leave(KErrArgument);
    }

// -----------------------------------------------------------------------------
// CCamCController::MvrcReplaceMetaDataEntryL
// Replace metadata entry.
// (other items were commented in a header).
//
void CCamCController::MvrcReplaceMetaDataEntryL(TInt /*aIndex*/, const CMMFMetaDataEntry& /*aNewEntry*/)
    {
    User::Leave(KErrArgument);
    }

// -----------------------------------------------------------------------------
// CCamCController::MvrcSetMaxFileSizeL
// Set maximum file size.
// (other items were commented in a header).
//
void CCamCController::MvrcSetMaxFileSizeL(TInt aMaxFileSize )
    {
    PRINT((_L("CCamCController::MvrcSetMaxFileSizeL() in")));

    if ( iState == EStatePrepared )
        {
        iState = EStateOpen;
        }

    if ( iFileComposer )
        {
        if ( iState != EStateOpen )
            {
            User::Leave(KErrNotReady);
            }

        // change sizelimit value to follow filecomposer internal handling of maximum file size
        if ( aMaxFileSize == KMMFNoMaxClipSize )
            {
            iFileComposer->SetSizeLimit(0);
            }
        else if ( aMaxFileSize < -1 )
            {
            User::Leave(KErrArgument);
            }
        else
            {
            iFileComposer->SetSizeLimit(aMaxFileSize);
            }
        }
    PRINT((_L("CCamCController::MvrcSetMaxFileSizeL() out")));
    }

// -----------------------------------------------------------------------------
// CCamCController::MvrcSetAudioEnabledL
// Set audio enabled/disabled.
// (other items were commented in a header).
//
void CCamCController::MvrcSetAudioEnabledL(TBool aEnable)
    {
    PRINT((_L("CCamCController::MvrcSetAudioEnabledL() in")));

    if ( iState == EStatePrepared )
        {
        iState = EStateOpen;
        }

    if ( iMediaRecorder )
        {
        if ( iState != EStateOpen )
            {
            User::Leave(KErrNotReady);
            }
        iMediaRecorder->SetAudioEnabledL(aEnable);
        }
    else
        {
        User::Leave( KErrNotReady );
        }
    PRINT((_L("CCamCController::MvrcSetAudioEnabledL() out")));
    }

// -----------------------------------------------------------------------------
// CCamCController::MvrcSetVideoFrameSizeL
// Set video frame size.
// (other items were commented in a header).
//
void CCamCController::MvrcSetVideoFrameSizeL(TSize aFrameSize)
    {
    PRINT((_L("CCamCController::MvrcSetVideoFrameSizeL() in")));

    if ( iState == EStatePrepared )
        {
        iState = EStateOpen;
        }

    if ( iMediaRecorder )
        {
        if ( iState != EStateOpen )
            {
            User::Leave(KErrNotReady);
            }
        iMediaRecorder->SetVideoFrameSizeL(aFrameSize);
        }
    else
        {
        User::Leave( KErrNotReady );
        }
    PRINT((_L("CCamCController::MvrcSetVideoFrameSizeL() out")));
    }

// -----------------------------------------------------------------------------
// CCamCController::MvrcPrepareL
// Prepare camcorder for recording.
// (other items were commented in a header).
//
void CCamCController::MvrcPrepareL()
    {
    OstTrace0( CAMERASRV_PERFORMANCE, CCAMCCONTROLLER_MVRCPREPAREL, "e_CCamCController::MvrcPrepareL 1" );
    PRINT((_L("CCamCController::MvrcPrepareL() in")));

    if ( iState == EStatePrepared )
        {
        DoSendEventToClient(TMMFEvent(KMMFEventCategoryVideoPrepareComplete, KErrNone));
        return;
        }

    if ( iState != EStateOpen )
        {
        DoSendEventToClient(TMMFEvent(KMMFEventCategoryVideoPrepareComplete, KErrNotReady));
        PRINT((_L("CCamCController::MvrcPrepareL() out, not ready")));
        return;
        }
	iFileComposerReady = EFalse;
    iFileComposer->SinkStopL();

    if ( iMediaRecorder->AudioEnabledL() )
        {
        iMediaRecorder->GetAudioCodecL(iAudioCodec);
        }
    else
        {
        iAudioCodec = KFourCCNULL;
        }

    TInt error = KErrNone;
    iMRPrepareCompleteSent = EFalse;
    TRAP( error, iMediaRecorder->PrepareL() );
    if ( error != KErrNone )
        {
        iMRPrepareCompleteSent = ETrue;
        DoSendEventToClient(TMMFEvent(KMMFEventCategoryVideoPrepareComplete, error));
        PRINT((_L("CCamCController::MvrcPrepareL() Mediarecorder error=%d"), error));
        return;
        }

    error = KErrNone;
    if( iForceDataSinkFileName )
        {
        TRAP( error, iFileComposer->OpenFileL( iFileName, iAudioCodec, *iVideoCodec, iVideoFormat ) );
        }
    else
        {
        TRAP( error, iFileComposer->OpenFileL( iMMFFile, iAudioCodec, *iVideoCodec, iVideoFormat ) );
        }
    if ( error != KErrNone )
        {
        iMRPrepareCompleteSent = ETrue;
        DoSendEventToClient(TMMFEvent(KMMFEventCategoryVideoPrepareComplete, error));
        PRINT((_L("CCamCController::MvrcPrepareL() Filecomposer error=%d"), error));
        return;
        }
    else
        {
        iFileComposerReady = ETrue;
        }

    PRINT((_L("CCamCController::MvrcPrepareL() out")));
    OstTrace0( CAMERASRV_PERFORMANCE, DUP1_CCAMCCONTROLLER_MVRCPREPAREL, "e_CCamCController::MvrcPrepareL 0" );
    }

// -----------------------------------------------------------------------------
// CCamCController::MvrcSetCameraHandleL
// Set camera handle.
// (other items were commented in a header).
//
void CCamCController::MvrcSetCameraHandleL(TInt aCameraHandle)
    {
    PRINT((_L("CCamCController::MvrcSetCameraHandleL() handle=%d        "), aCameraHandle));

    if ( iState == EStatePrepared )
        {
        iState = EStateOpen;
        }

    iCameraHandle = aCameraHandle;
    if ( ReadyToOpenSource() )
        {
            OpenSourceL();
        }

    PRINT((_L("CCamCController::MvrcSetCameraHandleL() out")));
    }

// -----------------------------------------------------------------------------
// CCamCController::MvrcGetRecordTimeAvailableL
// Returns available recording time.
// (other items were commented in a header).
//
void CCamCController::MvrcGetRecordTimeAvailableL(TTimeIntervalMicroSeconds& aTime)
    {
    if ( iFileComposer )
        {
        aTime =  iFileComposer->GetRemainingTimeL();
        }
    else
        {
        aTime = 0;
        }
    }

// -----------------------------------------------------------------------------
// CCamCController::MvrcGetAudioEnabledL
// Returns whether audio is enabled.
// (other items were commented in a header).
//
void CCamCController::MvrcGetAudioEnabledL(TBool& aEnabled)
    {
    PRINT((_L("CCamCController::MvrcGetAudioEnabledL() in")));

    if (iMediaRecorder)
        {
        aEnabled = iMediaRecorder->AudioEnabledL();
        }
    else
        {
        User::Leave( KErrNotReady );
        }
    PRINT((_L("CCamCController::MvrcGetAudioEnabledL() out")));
    }

// -----------------------------------------------------------------------------
// CCamCController::MardSetGainL
// Set gain.
// (other items were commented in a header).
//
void CCamCController::MardSetGainL(TInt aGain)
    {
    PRINT((_L("CCamCController::MardSetGainL() in")));

    if ( iMediaRecorder )
        {
        iMediaRecorder->SetGainL(aGain);
        }
    else
        {
        User::Leave( KErrNotReady );
        }
    PRINT((_L("CCamCController::MardSetGainL() out")));
    }

// -----------------------------------------------------------------------------
// CCamCController::MardGetMaxGainL
// Get maximum gain.
// (other items were commented in a header).
//
void CCamCController::MardGetMaxGainL(TInt& aMaxGain)
    {
    if ( iMediaRecorder )
        {
        aMaxGain = iMediaRecorder->MaxGainL();
        }
    else
        {
        User::Leave( KErrNotReady );
        }
    }

// -----------------------------------------------------------------------------
// CCamCController::MardGetGainL
// Get gain.
// (other items were commented in a header).
//
void CCamCController::MardGetGainL(TInt& aGain)
    {
    if (iMediaRecorder)
        {
        aGain = iMediaRecorder->GainL();
        }
    else
        {
        User::Leave( KErrNotReady );
        }
    }

// -----------------------------------------------------------------------------
// CCamCController::MardSetBalanceL
// Set balance. Not supported.
// (other items were commented in a header).
//
void CCamCController::MardSetBalanceL(TInt /*aBalance*/)
    {
    // Not called from Video Recorder client API
    User::Leave(KErrNotSupported);
    }

// -----------------------------------------------------------------------------
// CCamCController::MardGetBalanceL
// Get balance. Not supported.
// (other items were commented in a header).
//
void CCamCController::MardGetBalanceL(TInt& /*aBalance*/)
    {
    // Not called from Video Recorder client API
    User::Leave(KErrNotSupported);
    }

// -----------------------------------------------------------------------------
// CCamCController::NewFilename
// Handle custom commands NewFilename to controller.
// (other items were commented in a header).
//
void CCamCController::NewFilenameL( TMMFMessage& aMessage )
    {
    OstTrace0( CAMERASRV_PERFORMANCE, CCAMCCONTROLLER_NEWFILENAMEL, "e_CCamCController::NewFilenameL 1" );
    PRINT((_L("CCamCController::NewFilename() in")));

    if ( iState == EStatePrepared )
        {
        iState = EStateOpen;
        }

    if ( ( iState != EStateOpen ) && ( iState != EStateNone ) )
        {
        User::Leave( KErrNotReady );
        }

    // Get the size of the init data and create a buffer to hold it
    TInt desLength = aMessage.SizeOfData1FromClient();
    HBufC8* buf = HBufC8::NewLC(desLength);
    TPtr8 ptr = buf->Des();
    aMessage.ReadData1FromClientL(ptr);

    TMMFFileParams params;
    TPckgC<TMMFFileParams> config(params);
    config.Set(*buf);
    params = config();

    TParse parser ;
    User::LeaveIfError(parser.Set(params.iPath, NULL, NULL));
    CleanupStack::PopAndDestroy(buf);//buf

    iFileName.Copy( parser.FullName() );
    iForceDataSinkFileName = ETrue;

    PRINT((_L("CCamCController::NewFilename() out")));
    OstTrace0( CAMERASRV_PERFORMANCE, DUP1_CCAMCCONTROLLER_NEWFILENAMEL, "e_CCamCController::NewFilenameL 0" );
    }

// -----------------------------------------------------------------------------
// CCamCController::ReadyToOpenSource
// Check if controller is ready to open mediarecorder and filecomposer
// (other items were commented in a header).
//
TBool CCamCController::ReadyToOpenSource()
    {
    PRINT((_L("CCamCController::ReadyToOpenSource() in")));

    if ( ( iState == EStateNone ) &&
         ( iVideoCodec != 0 ) &&
         ( iAudioCodec != 0 ) &&
         ( iCameraHandle >= 0 ) &&
         ( iMediaRecorder ) &&
         ( iFileComposer ) )
        {
        PRINT((_L("CCamCController::ReadyToOpenSource() out, ready")));
        return ETrue;
        }
    PRINT((_L("CCamCController::ReadyToOpenSource() out, not ready")));
    return EFalse;
    }

// -----------------------------------------------------------------------------
// CCamCController::OpenSourceL
// Open mediarecorder adn filecomposer.
// (other items were commented in a header).
//
void CCamCController::OpenSourceL()
    {
    PRINT((_L("CCamCController::OpenSourceL() in")));

    TInt error = KErrNone;
    TRAP( error, iMediaRecorder->OpenL(this, iAudioSource, STATIC_CAST(MCMRMediaSink* , iFileComposer), iCameraHandle, *iVideoCodec, iAudioCodec ) );
    if ( error == KErrNone )
        {
        iState = EStateOpen;
        TRAP( error, iMediaRecorder->SetAudioPriorityL( iPrioritySettings ) );
        }
    DoSendEventToClient(TMMFEvent(KMMFEventCategoryVideoOpenComplete, error));
    PRINT((_L("CCamCController::OpenSourceL() out")));
    }

// ---------------------------------------------------------------------------
// CCamCController::IdleStop
// ---------------------------------------------------------------------------
//
TInt CCamCController::IdleStop( TAny* aCont )
    {
    CCamCController* appCont = static_cast<CCamCController*>( aCont );
    appCont->DoIdleStop();
    return KErrNone;
    }

// ---------------------------------------------------------------------------
// CCamCController::DoIdleStop
// ---------------------------------------------------------------------------
//
void CCamCController::DoIdleStop()
    {
    OstTrace0( CAMERASRV_PERFORMANCE, CCAMCCONTROLLER_DOIDLESTOP, "e_CCamCController::DoIdleStop 1" );
    PRINT((_L("CCamCController::DoIdleStop() in")));
    TInt error = KErrNone;

    TRAP( error, iFileComposer->SinkStopL() );
    if ( error != KErrNone )
        {
        PRINT((_L("CCamCController::DoIdleStop() FileComposerStop error= %d      "), error));
        iState = EStatePrepared;
        iFileComposerReady = EFalse;
        }

    PRINT((_L("CCamCController::DoIdleStop() sending event KCamCControllerCCVideoCompletedUid, error=%d      "), error));
    DoSendEventToClient(TMMFEvent(KCamCControllerCCVideoFileComposed, error));

	if ( (iSavedStopError == KErrDiskFull) || (iSavedStopError == KErrCompletion ) || (iSavedStopError == KErrNone) )
		{
    	iState = EStatePrepared;
		}
	else
		{
		// Fatal error happened, need to prepare again.
		iState = EStateOpen;
		}
    iFileComposerReady = EFalse;
    iAsyncStop = EFalse;
    PRINT((_L("CCamCController::DoIdleStop() out")));
    OstTrace0( CAMERASRV_PERFORMANCE, DUP1_CCAMCCONTROLLER_DOIDLESTOP, "e_CCamCController::DoIdleStop 0" );
    }


//  End of File

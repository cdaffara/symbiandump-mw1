/*
* Copyright (c) 2002-2008 Nokia Corporation and/or its subsidiary(-ies).
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
#include "mmf/common/mmfbase.h"
#include "CCMRRecorderBase.h"
#include "CCMRVideoSettings.h"      // default video settings
#include "CCMRSupportedCodecs.h"
#include "CCMRVideoHWParams.h"
#include "CCMRVideoRecorder.h"
#include "CCMRMediaRecorder.h"      // for bitrate control mode
#include "CCMRVideoThreadProxy.h"
#include "CCMRFifo.h"
#include "CCMRActiveOutput.h"
#include "CCMRVideoCodecDataH263.h"
#include "CCMRVideoCodecDataMPEG4.h"
#include "CCMRVideoCodecDataH264AVC.h"
#include <mmf/common/mmfvideo.h>

/*
#include "OstTraceDefinitions.h"
#ifdef OST_TRACE_COMPILER_IN_USE
#include "CCMRMDFVideoRecorderTraces.h"
#endif
*/

// MACROS
// Assertion macro wrapper for code cleanup
#define VRASSERT(x) __ASSERT_DEBUG(x, User::Panic(_L("CCMRMDFVIDEORECORDER"), EInternalAssertionFailure))

// Debug print macro
#ifdef _DEBUG
#include <e32svr.h>

#define PRINT(x) RDebug::Print x;
#else
#define PRINT(x)
#endif

#include "OstTraceDefinitions.h"
#ifdef OST_TRACE_COMPILER_IN_USE
#include "CCMRMDFVideoRecorderTraces.h"
#endif

// ============================= LOCAL FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// convertFrameRateToInterval Converts framerate to frame time interval
// (time between 2 consecutive frames).
// Returns: TInt time between 2 consecutive frames
// -----------------------------------------------------------------------------
//
static TInt convertFrameRateToInterval(TReal32 aFrameRate)
    {
    return (TInt(1E6/aFrameRate + 0.5));
    }


// -----------------------------------------------------------------------------
// convertFrameRateToInterval Converts framerate to frame time interval
// (time between 2 consecutive frames).
// Returns: TInt time between 2 consecutive frames
// -----------------------------------------------------------------------------
//
static TInt TLinearOrderFuncVideoSizeRate(const TPictureRateAndSize& aPictureRateAndSize1,
										  const TPictureRateAndSize& aPictureRateAndSize2 )
	{
	if ( (aPictureRateAndSize1.iPictureSize.iWidth == aPictureRateAndSize2.iPictureSize.iWidth) &&
		 (aPictureRateAndSize1.iPictureSize.iHeight == aPictureRateAndSize2.iPictureSize.iHeight) &&
		 (aPictureRateAndSize1.iPictureRate == aPictureRateAndSize2.iPictureRate) )
		{
		return 0;
		}

	if ( (aPictureRateAndSize1.iPictureSize.iWidth < aPictureRateAndSize2.iPictureSize.iWidth) ||
		 (aPictureRateAndSize1.iPictureSize.iHeight < aPictureRateAndSize2.iPictureSize.iHeight) ||
		 (aPictureRateAndSize1.iPictureRate < aPictureRateAndSize2.iPictureRate))
		{
		return -1;
		}
	else
		{
		return 1;
		}
	}

// ================= MEMBER FUNCTIONS =======================

// ---------------------------------------------------------
// CCMRVideoRecorder::NewL
// Two-phased constructor.
// ---------------------------------------------------------
//
CCMRVideoRecorder* CCMRVideoRecorder::NewL(MAsyncEventHandler& aEventHandler, CCMRConfigManager* aConfig )
    {
    PRINT((_L("CCMRVideoRecorder::NewL(), In: MDF")))

    CCMRVideoRecorder* self = new (ELeave) CCMRVideoRecorder(aEventHandler);
    CleanupStack::PushL( self );
    self->ConstructL(aConfig);
    CleanupStack::Pop();

    PRINT((_L("CCMRVideoRecorder::NewL(), Out: MDF")))
    return self;
    }


// ---------------------------------------------------------
// CCMRVideoRecorder::ConstructL()
// Symbian 2nd phase constructor can leave.
// ---------------------------------------------------------
//
void CCMRVideoRecorder::ConstructL(CCMRConfigManager* aConfig)
    {
    PRINT((_L("CCMRVideoRecorder::ConstructL(), In")))

    SetState(EStateNone);

    iConfig = aConfig;
    iNumCameraBuffers = iConfig->PluginSettings().iCMRNumCameraBuffers;
    // Create input fifo
    iSourceFifo = CCMRFifo::NewL(iNumCameraBuffers);
    iCodingFifo = CCMRFifo::NewL(iNumCameraBuffers);

    iOutputSinkBuffer = new (ELeave) CCMRMediaBuffer;

    iSizeIndex = -1;
    iSizeIndex420 = -1;
    iSizeIndex422 = -1;
    iSizeIndexDCEncoder = -1;
    iRateIndex = -1;
    iRateIndex420 = -1;
    iRateIndex422 = -1;
    iRateIndexDCEncoder = -1;
    iDevVideoRec = NULL;
    iEncoderHWDeviceId = 0;
    iPreProcessorHWDeviceId = 0;
    iOutputVideoBuffer = NULL;
    iVideoBufferType = CCMRMediaBuffer::EVideoH263;
    iClockSource = NULL;
    iDecSpecInfoLength = 0;
    iPreferredEncoderUID = KNullUid;
    iPreferredEncapsulationSet = EFalse;
    iFrameSize.SetSize( KCMRFrameWidth, KCMRFrameHeight );
    iSourceFrameRate = KCMRFrameRate;
    iSourceFrameInterval = convertFrameRateToInterval(iSourceFrameRate);
    iEncodingFrameRate = KCMRFrameRate;
    iMaxFrameRate4GivenSize = KCMRFrameRate;
    iRequestedFrameRate = KCMRFrameRate;
    CCMRRecorderBase::SetTargetBitRateL( KCMRTargetBitRate );

    iMinRandomAccessPeriodInSeconds = KCMRMinRandomAccessPeriod;

    iMimeType = KCMRMimeTypeH263BaselineProfile;    //copy
    iMimeType += _L8( "; level=10" );    // append
    iVideoCodecData = new (ELeave) CCMRVideoCodecDataH263(10);    // default is H.263 level 10

    iBitRateMode = EBitRateConstant;

    iTimeWhenPaused = 0;
    iTotalPausedTime = 0;

    iErrorCode = KErrNone;

    iEncoderInputQueueLength = 0;

    iNumberOfCapturedFrames = 0;
    iNumberOfEncodedFrames = 0;

    iMutexObj.CreateLocal();
    iMutexCreated = ETrue;
    iBufferReturnAO = CCMRReturnAO::NewL(this);

    iFatalError = EFalse;
    iInputEnd = EFalse;
    iStreamEnd = EFalse;

    // Direct capture
    iDirectCapture = EFalse;
    iCameraHandle = 0;
    iSkipBuffers = EFalse;
    iDriftFrameSkipCount = 0;
    iAddedFrameDurationCount = 0;
    iPreviousCameraFrameIndex = 0;

#ifdef _DEBUG
    iLastCapture = 0;
    iCumulativeEncodingTime = iCumulativeCaptureTime = 0;
    iAverageEncodingTime = iAverageCaptureTime = 0;
#endif
	iDevVideoRec = CMMFDevVideoRecord::NewL( *this );
	iAvailableVideoEncoders.Reset();
	iAvailableVideoFrameSizesRates.Reset();

	PRINT((_L("CCMRVideoRecorder::ConstructL() looking for defaul video codec encoder(s)")));
	iDevVideoRec->FindEncodersL(iMimeType, 0 /* aPreProc */, iAvailableVideoEncoders, EFalse );
	PRINT((_L("CCMRVideoRecorder::ConstructL() search found %d encoder(s)"), iAvailableVideoEncoders.Count() ));

	if ( iConfig )
		{
		iConfig->SetVideoCodec(iMimeType);
		iConfig->SetVideoPixelAspectRatio(KCMRAspectRatioNum, KCMRAspectRatioDenom);

		// fill out defaults for Rate Control from ICM.
	    iRateControlOptions.iPictureQuality = iConfig->PluginSettings().iCMRPictureQuality;
	    iRateControlOptions.iLatencyQualityTradeoff = iConfig->PluginSettings().iCMRLatencyQualityTradeoff; // latency vs. quality
	    iRateControlOptions.iQualityTemporalTradeoff = iConfig->PluginSettings().iCMRQualityTemporalTradeoff; // spatial vs. temporal quality
		}
    UpdateSupportedVideoFrameSizesRates();
    PRINT((_L("CCMRVideoRecorder::ConstructL(), Out")))
    }


// ---------------------------------------------------------
// CCMRVideoRecorder::~CCMRVideoRecorder()
// Destructor
// ---------------------------------------------------------
//
CCMRVideoRecorder::~CCMRVideoRecorder()
    {
    OstTrace0( CAMERASRV_PERFORMANCE, CCMRVIDEORECORDER_CCMRVIDEORECORDER, "e_CCMRVideoRecorder::~CCMRVideoRecorder 1" );
    PRINT((_L("CCMRVideoRecorder::~CCMRVideoRecorder(), In")));
    // This is the counterpart to NewL & OpenL, e.g. Close & Delete
    // free all memory allocated and uninitalize & delete objects created, e.g. DevVideoRecord

#if defined VIDEO_FILE_OUTPUT || defined VIDEO_BS_FILE_OUTPUT
    iOutputFile.Close();
    iFs.Close();
#endif

    // to make PC Lint happy
    iOutputVideoBuffer = NULL;
    iOutput = NULL;
    iClockSource = NULL;

    // delete DevVideoRecord instance
    OstTrace0( CAMERASRV_PERFORMANCE, DUP6_CCMRVIDEORECORDER_CCMRVIDEORECORDER, "e_CCMRVideoRecorder::~CCMRVideoRecorder_delete_DevVideoRec 1" );
    delete iDevVideoRec;
    PRINT((_L("CCMRVideoRecorder::~CCMRVideoRecorder() devvideorec deleted")));
    OstTrace0( CAMERASRV_PERFORMANCE, DUP7_CCMRVIDEORECORDER_CCMRVIDEORECORDER, "e_CCMRVideoRecorder::~CCMRVideoRecorder_delete_DevVideoRec 0" );

    iAvailableVideoFrameSizesRates.Close();
    iAvailableVideoEncoders.Close();

    delete iOutputSinkBuffer;
    iOutputSinkBuffer = NULL;

    delete iVideoCodecData;
    iVideoCodecData = NULL;

    OstTrace0( CAMERASRV_PERFORMANCE, DUP2_CCMRVIDEORECORDER_CCMRVIDEORECORDER, "e_CCMRVideoRecorder::~CCMRVideoRecorder_delete_SourceFifo 1" );
    // empty the source fifo, just in case once more; this should be done a) by encoder and b) by MdvroStreamEnd()
    MFrameBuffer* cbuffer;
    if ( iSourceFifo )
        {
        while ( !iSourceFifo->IsEmpty() )
            {
            cbuffer = reinterpret_cast<MFrameBuffer*>(iSourceFifo->Get());
            // Release camera API buffer
            cbuffer->Release();
            }
        delete iSourceFifo;
        iSourceFifo = NULL;
        }
    OstTrace0( CAMERASRV_PERFORMANCE, DUP3_CCMRVIDEORECORDER_CCMRVIDEORECORDER, "e_CCMRVideoRecorder::~CCMRVideoRecorder_delete_SourceFifo 0" );
    OstTrace0( CAMERASRV_PERFORMANCE, DUP4_CCMRVIDEORECORDER_CCMRVIDEORECORDER, "e_CCMRVideoRecorder::~CCMRVideoRecorder_delete_CodingFifo 1" );
    TVideoPicture* picture;
    if ( iCodingFifo )
        {
        // delete the empty devvr picture-holders stored in the fifo
        while ( !iCodingFifo->IsEmpty() )
            {
            picture = reinterpret_cast<TVideoPicture*>(iCodingFifo->Get());
            PRINT((_L("CCMRVideoRecorder::~CCMRVideoRecorder() deleting %x"),picture));
            delete picture;
            }
        delete iCodingFifo;
        iCodingFifo = NULL;
        }
    OstTrace0( CAMERASRV_PERFORMANCE, DUP5_CCMRVIDEORECORDER_CCMRVIDEORECORDER, "e_CCMRVideoRecorder::~CCMRVideoRecorder_delete_CodingFifo 0" );
    PRINT((_L("CCMRVideoRecorder::~CCMRVideoRecorder() fifos deleted")));

    if ( iDecSpecInfo )
        {
        // We still have MPEG-4 decoder configuration info stored
        delete iDecSpecInfo;
        iDecSpecInfo = NULL;
        }

    // delete camera
    delete iSource;
    iSource = NULL;
    PRINT((_L("CCMRVideoRecorder::~CCMRVideoRecorder() camera deleted")));

    if ( iBufferReturnAO )
        {
        iBufferReturnAO->Cancel();
        delete iBufferReturnAO;
        }

    if ( iThreadHandleOpened )
        {
        iOutputThreadHandle.Close();
        iThreadHandleOpened = EFalse;
        }

    if ( iMutexCreated )
        {
        iMutexObj.Close();
        }

    SetState(EStateNone);

    PRINT((_L("CCMRVideoRecorder::~CCMRVideoRecorder(), Out")));
    OstTrace0( CAMERASRV_PERFORMANCE, DUP1_CCMRVIDEORECORDER_CCMRVIDEORECORDER, "e_CCMRVideoRecorder::~CCMRVideoRecorder 0" );
    }


// ---------------------------------------------------------
// CCMRVideoRecorder::SetOutputL
// Sets output active object
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CCMRVideoRecorder::SetOutputL(CCMRActiveOutput* aOutput)
    {
    iOutput = aOutput;
    iOutput->RegisterSourceL( this );
    if ( iSource && iThreadHandleOpened )
        {
        SetState( EStateOpen );
        }
    }

// ---------------------------------------------------------
// CCMRVideoRecorder::SetClockSourceL
// Set clock source
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CCMRVideoRecorder::SetClockSource(MMMFClockSource* aClockSource)
    {
    iClockSource = aClockSource;
    }

// ---------------------------------------------------------
// CCMRVideoRecorder::SetOutputThreadIdL
// Sets id of the output thread
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CCMRVideoRecorder::SetOutputThreadIdL(TUint aThreadId)
    {
    User::LeaveIfError( iOutputThreadHandle.Open(aThreadId) );

    iThreadHandleOpened = ETrue;
    if ( iOutput && iSource )
        {
        SetState( EStateOpen );
        }
    }


// ---------------------------------------------------------
// CCMRVideoRecorder::SetCameraHandleL
// Sets camera handle and creates camera instance
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CCMRVideoRecorder::SetCameraHandleL(TInt aCameraHandle)
    {
    PRINT((_L("CCMRVideoRecorder::SetCameraHandleL(), In")))

    // Create video source
#ifdef VIDEO_FILE_INPUT
    iSource = CCMRVideoFileSource::NewL(this);
#else
    iSource = CCMRVideoCameraSource::NewL(this, aCameraHandle);
#endif

    // Store a camera handle for future use
    iCameraHandle = aCameraHandle;

    // get camera info
    iSource->CameraInfo(iCameraInfo);

#if defined VIDEO_FILE_OUTPUT || defined VIDEO_BS_FILE_OUTPUT
    User::LeaveIfError(iFs.Connect());
    User::LeaveIfError(iOutputFile.Replace(iFs, _L("videorec_out.bin"), EFileWrite | EFileShareExclusive));
#endif

    if ( iOutput && iThreadHandleOpened )
        {
        SetState( EStateOpen );
        }

    PRINT((_L("CCMRVideoRecorder::SetCameraHandleL(), Out")))
    }

// ---------------------------------------------------------
// CCMRVideoRecorder::SetupEncoderL
// Private helper method to select & setup the encoder
// plugin devvr must use
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CCMRVideoRecorder::SetupEncoderL()
    {
#ifdef _DEBUG
    TBuf<256> mime;
    mime.Copy(iMimeType);
    PRINT((_L("CCMRVideoRecorder::SetupEncoderL() in, video mime-type: %S "), &mime ));
#endif

    if ( iPreferredEncoderUID != KNullUid )
        {// We have preferred encoder UID from client - override encoder search and use it instead.
        PRINT((_L("CCMRVideoRecorder::SetVideoCodecL() skipping encoder search. Using API user encoder: %d "), iPreferredEncoderUID.iUid));
        iAvailableVideoEncoders.Reset();
        iAvailableVideoEncoders.AppendL(iPreferredEncoderUID);
        }
    else if ( iConfig &&
            ( iConfig->IsICMConfigDataAvailable() ) &&
            ( iConfig->VideoQualitySettings().iVideoEncoderUID != KNullUid ) )
        {// Video quality set has set UID value - override encoder search and use it instead.
        PRINT((_L("CCMRVideoRecorder::SetVideoCodecL() skipping encoder search. Using ICM config encoder: %d "), iPreferredEncoderUID.iUid));
        iAvailableVideoEncoders.Reset();
        iAvailableVideoEncoders.AppendL(iPreferredEncoderUID);
        }

    // uncompressed format structures for YUV420 planar
    TBool cameraSupports420 = ETrue;

    if ( (iSizeIndex420 < 0) || (iRateIndex420 < 0) )
        {
        cameraSupports420 = EFalse;
        }
    else
        {
        cameraSupports420 = ETrue;
        PRINT((_L("CCMRVideoRecorder::SetupEncoderL() camera supports YUV420 planar") ));
        }

    // uncompressed format structures for YUV422
    TBool cameraSupports422 = ETrue;
    if ( (iSizeIndex422 < 0) || (iRateIndex422 < 0) )
        {
        cameraSupports422 = EFalse;
        }
    else
        {
        cameraSupports422 = ETrue;
        PRINT((_L("CCMRVideoRecorder::SetupEncoderL() camera supports YUV422 interleaved") ));
        }

    // Output format
    CCompressedVideoFormat *comprFormat = CCompressedVideoFormat::NewL( iMimeType );
    CleanupStack::PushL( comprFormat );

    // index for encoder candidate. Have to go through the list to find an accerated encoder
    TInt encoderIndex = -1;
    TUncompressedVideoFormat encoderUncompFormat;

    // encoder info for retrieving capabilities
    CVideoEncoderInfo* encoderInfo = NULL;

    // this is needed if init of HW accelerated codec failed and we retry => forces to select ARM codec
    TBool alreadyFailedWithHWAccelerated = iVideoCodecHWAccelerated;
    iVideoCodecHWAccelerated = EFalse;

    TInt infoError = KErrNone;
    TBool encoderSupports422 = EFalse;
    TBool encoderSupports420 = EFalse;

    TYuvCoefficients preproInputYuvCoefficient;
    TYuvCoefficients preproOutputYuvCoefficient;
    TYuvCoefficients encoderInputYuvCoefficient;
    TUint aspectRatioNum = 0;
    TUint aspectRatioDenom = 0;
    TVideoDataUnitEncapsulation outputFormatEncapsulation = EDuElementaryStream;

    if( KCMRMimeTypeH263() == iMimeType.Left( KCMRMimeTypeH263().Length() ))
        {// H.263
        PRINT((_L("CCMRVideoRecorder::SetupEncoderL() - setting H.263 YUVs") ));
        preproInputYuvCoefficient = EYuvBt601Range1;
        preproOutputYuvCoefficient = EYuvBt601Range0;
        encoderInputYuvCoefficient = EYuvBt601Range0;
        iVideoBufferType = CCMRMediaBuffer::EVideoH263;
        // pixel aspect ratios
        if ( iConfig && iConfig->IsICMConfigDataAvailable() )
	        {
	        aspectRatioNum = iConfig->VideoQualitySettings().iVideoPixelAspectRatioNum;
	        aspectRatioDenom = iConfig->VideoQualitySettings().iVideoPixelAspectRatioDenom;
	        }
        else
	        {
	        aspectRatioNum = KCMRAspectRatioNum;
	        aspectRatioDenom = KCMRAspectRatioDenom;
	        }
        }
    else if ( KCMRMimeTypeMPEG4V() == iMimeType.Left( KCMRMimeTypeMPEG4V().Length() ))
        {// MPEG-4
        PRINT((_L("CCMRVideoRecorder::SetupEncoderL() - setting MPEG4 YUVs") ));
        preproInputYuvCoefficient = EYuvBt709Range1;
        preproOutputYuvCoefficient = EYuvBt709Range1;
        encoderInputYuvCoefficient = EYuvBt709Range1;
        // pixel aspect ratios
        if ( iConfig && iConfig->IsICMConfigDataAvailable() )
	        {
	        aspectRatioNum = iConfig->VideoQualitySettings().iVideoPixelAspectRatioNum;
	        aspectRatioDenom = iConfig->VideoQualitySettings().iVideoPixelAspectRatioDenom;
	        }
        else
	        {
	        aspectRatioNum = KCMRMPEG4AspectRatioNum;
	        aspectRatioDenom = KCMRMPEG4AspectRatioDenom;
	        }
        iVideoBufferType = CCMRMediaBuffer::EVideoMPEG4;
        }
    else
        {// H.264 AVC
        PRINT((_L("CCMRVideoRecorder::SetupEncoderL() - setting H.264 AVC YUVs") ));
        preproInputYuvCoefficient = EYuvBt709Range1;
        preproOutputYuvCoefficient = EYuvBt709Range1;
        encoderInputYuvCoefficient = EYuvBt709Range1;
        // pixel aspect ratios 1:1
        if ( iConfig && iConfig->IsICMConfigDataAvailable() )
	        {
	        aspectRatioNum = iConfig->VideoQualitySettings().iVideoPixelAspectRatioNum;
	        aspectRatioDenom = iConfig->VideoQualitySettings().iVideoPixelAspectRatioDenom;
	        }
        else
	        {
	        aspectRatioNum = KCMRMPEG4AspectRatioNum;
	        aspectRatioDenom = KCMRMPEG4AspectRatioDenom;
	        }
        // output format encapsulation
        iVideoBufferType = CCMRMediaBuffer::EVideoH264NAL;
        outputFormatEncapsulation = EDuGenericPayload;
        }

    TBool directCaptureEncoder = EFalse;
    TInt supportedEncoderInputsCount = 0;
    // find an encoder with matching capabilities
    for ( TInt i = 0 ; i < iAvailableVideoEncoders.Count(); i++ )
        {
        encoderInfo = NULL;
        PRINT((_L("CCMRVideoRecorder::SetupEncoderL() - getting info from a plugin index[%d] with Uid 0x%x"), i, iAvailableVideoEncoders[i].iUid ));
        TRAPD(error, (encoderInfo = ReadEncoderInfoL(iAvailableVideoEncoders[i])) );

        if ( encoderInfo == NULL )
            {
            PRINT((_L("CCMRVideoRecorder::SetupEncoderL() - getting info from a plugin failed, skipping") ));
            infoError = error;
            }
        else
            {
            PRINT((_L("CCMRVideoRecorder::SetupEncoderL() - got an info from a plugin, checking...") ));

            // item was popped in CheckEncoderInfoL, push it back to stack
            CleanupStack::PushL( encoderInfo );

            // check if compressed video format matches
            if ( encoderInfo->SupportsOutputFormat(*comprFormat) )   // max picture size & bit-rate was checked in the level of comprFormat
                {
                // check input format of encoder.
                // a candidate encoder supporting the required compressed format was found
                encoderSupports422 = EFalse;
                encoderSupports420 = EFalse;

                RArray<TUncompressedVideoFormat> supportedEncoderInputs = encoderInfo->SupportedInputFormats();
                supportedEncoderInputsCount = supportedEncoderInputs.Count();
                PRINT((_L("CCMRVideoRecorder::SetupEncoderL() Encoder at index[%d] supports %d input format(s), checking for support"), i, supportedEncoderInputs.Count() ));

                // Go through supported input format and look for match to important parameters.
                for(TInt j = 0; j < supportedEncoderInputsCount; j++ )
                    {
                    if ( !encoderSupports422 &&
                         cameraSupports422 &&
                         supportedEncoderInputs[j].iDataFormat == EYuvRawData &&
                         supportedEncoderInputs[j].iYuvFormat.iCoefficients == encoderInputYuvCoefficient &&
                         ( supportedEncoderInputs[j].iYuvFormat.iDataLayout == EYuvDataInterleavedLE ||
                           supportedEncoderInputs[j].iYuvFormat.iDataLayout == EYuvDataInterleavedBE ) &&
                         supportedEncoderInputs[j].iYuvFormat.iYuv2RgbMatrix == NULL &&
                         supportedEncoderInputs[j].iYuvFormat.iRgb2YuvMatrix == NULL &&
                         supportedEncoderInputs[j].iYuvFormat.iAspectRatioNum ==  aspectRatioNum &&
                         supportedEncoderInputs[j].iYuvFormat.iAspectRatioDenom ==  aspectRatioDenom &&
                         ( supportedEncoderInputs[j].iYuvFormat.iPattern == EYuv422Chroma1 ||
                         supportedEncoderInputs[j].iYuvFormat.iPattern == EYuv422Chroma2 ) )
                        {
                        // YUV 422 input format can be used with this encoder
                        encoderSupports422 = ETrue;
                        encoderUncompFormat = supportedEncoderInputs[j];
                        PRINT((_L("CCMRVideoRecorder::SetupEncoderL() Encoder at index[%d] supports YUV422 interleaved"), i ));
                        break;
                        }
                    if ( !encoderSupports420 &&
                         !encoderSupports422 &&
                         cameraSupports420 &&
                         supportedEncoderInputs[j].iDataFormat == EYuvRawData &&
                         supportedEncoderInputs[j].iYuvFormat.iCoefficients == encoderInputYuvCoefficient &&
                         supportedEncoderInputs[j].iYuvFormat.iDataLayout == EYuvDataPlanar &&
                         supportedEncoderInputs[j].iYuvFormat.iYuv2RgbMatrix == NULL &&
                         supportedEncoderInputs[j].iYuvFormat.iRgb2YuvMatrix == NULL &&
                         supportedEncoderInputs[j].iYuvFormat.iAspectRatioNum ==  aspectRatioNum &&
                         supportedEncoderInputs[j].iYuvFormat.iAspectRatioDenom ==  aspectRatioDenom &&
                         ( supportedEncoderInputs[j].iYuvFormat.iPattern == EYuv420Chroma1 ||
                           supportedEncoderInputs[j].iYuvFormat.iPattern == EYuv420Chroma2 ) )
                        {
                        // YUV 420 planar input format can be used with this encoder
                        encoderSupports420 = ETrue;
                        encoderUncompFormat = supportedEncoderInputs[j];
                        PRINT((_L("CCMRVideoRecorder::SetupEncoderL() Encoder at index[%d] supports YUV420 planar"), i ));
                        if( !cameraSupports422 )
                            {
                            break;
                            }
                        }
                    }
                PRINT((_L("CCMRVideoRecorder::SetupEncoderL() Encoder at index[%d] supports: 420[%d], 422[%d]"), i, encoderSupports420, encoderSupports422 ));

                // Check directCapture support for current codec
                directCaptureEncoder = encoderInfo->SupportsDirectCapture();
                PRINT((_L("CCMRVideoRecorder::SetupEncoderL() Encoder at index[%d] supports DirectCapture:[%d]."), i, directCaptureEncoder));

                // determine H.264 AVC encapsulation
                if ( (iVideoBufferType == CCMRMediaBuffer::EVideoH264NAL) ||
                     (iVideoBufferType == CCMRMediaBuffer::EVideoH264Bytestream) )
                    {
                    if (( iPreferredEncapsulationSet ) &&
                        ( encoderInfo->SupportedDataUnitEncapsulations() & iPreferredEncapsulation ))
                        {
                        outputFormatEncapsulation = iPreferredEncapsulation;
                        }
                    else
                        {
                        if (encoderInfo->SupportedDataUnitEncapsulations() & EDuGenericPayload)
                            {
                            PRINT((_L("CCMRVideoRecorder::SetupEncoderL() H.264 Encoder at index[%d] set to EDuGenericPayload encapsulation."), i));
                            outputFormatEncapsulation = EDuGenericPayload;
                            }
                        else
                            {
                            PRINT((_L("CCMRVideoRecorder::SetupEncoderL() H.264 Encoder at index[%d] set to EDuElementaryStream encapsulation."), i));
                            outputFormatEncapsulation = EDuElementaryStream;
                            }
                        }

                    if ( outputFormatEncapsulation == EDuGenericPayload )
                        {
                        iVideoBufferType = CCMRMediaBuffer::EVideoH264NAL;
                        }
                    else
                        {
                        iVideoBufferType = CCMRMediaBuffer::EVideoH264Bytestream;
                        }
                    }

                if (directCaptureEncoder && (supportedEncoderInputsCount != 0))
                    {
                    PRINT((_L("CCMRVideoRecorder::SetupEncoderL() Warning - Encoder at index[%d] publish input formats even though it supports directcapture - ERROR!."), i));
                    }

                if ( encoderSupports422 || encoderSupports420 || (directCaptureEncoder && (supportedEncoderInputsCount == 0)) )
                    {
                    // encoder's input format matches with camera's output
                    if ( encoderInfo->Accelerated() && (alreadyFailedWithHWAccelerated == EFalse) )
                        // assume there is only 1 HW accelerated codec, and if it was selected previously we come here only if the init failed
                        // => must use non-hw-accelerated codec
                        {
                        // no need to search for more, accelerated encoder is preferred
                        encoderIndex = i;
                        iVideoCodecHWAccelerated = ETrue;
                        iDirectCapture = directCaptureEncoder;
                        // HW accelerated is preferred regardless of the input format, but if both formats are supported, then 422 is preferred
                        // need to set the camera variables accordingly
                        if ( !(directCaptureEncoder && (supportedEncoderInputsCount == 0)) )
                            {
                            if ( encoderSupports422 )
                                {
                                PRINT((_L("CCMRVideoRecorder::SetupEncoderL() YUV422 interleaved selected as input format") ));
                                iSizeIndex = iSizeIndex422;
                                iRateIndex = iRateIndex422;
                                iVideoFormat = CCamera::EFormatYUV422;
                                }
                            else if (encoderSupports420)
                                {
                                PRINT((_L("CCMRVideoRecorder::SetupEncoderL() YUV420 planar selected as input format") ));
                                iSizeIndex = iSizeIndex420;
                                iRateIndex = iRateIndex420;
                                iVideoFormat = CCamera::EFormatYUV420Planar;
                                }
                            }
                        // check max framerate for given picture size
                        RArray<TPictureRateAndSize> rateAndSize = encoderInfo->MaxPictureRates();
                        TUint rates = rateAndSize.Count();
                        for ( TUint j = 0; j < rates; j++ )
                            {
                            if ( rateAndSize[j].iPictureSize == iFrameSize )
                                {
                                iMaxFrameRate4GivenSize = rateAndSize[j].iPictureRate;
                                break;
                                }
                            }
                        PRINT((_L("CCMRVideoRecorder::SetupEncoderL() - there is a suitable HW accelerated plugin, don't check other plugins") ));
                        CleanupStack::PopAndDestroy( encoderInfo );
                        break;
                        }
                    else if ( encoderIndex < 0 )
                        {
                        // accept also non-accelerated if no other is available
                        encoderIndex = i;
                        // encoder is selected regardless of the input format, but if both formats are supported, then 422 is preferred
                        // need to set the camera variables accordingly
                        if ( !(directCaptureEncoder && (supportedEncoderInputsCount == 0)) )
                            {
                            if ( encoderSupports422 )
                                {
                                PRINT((_L("CCMRVideoRecorder::SetupEncoderL() YUV422 interleaved selected as input format") ));
                                iSizeIndex = iSizeIndex422;
                                iRateIndex = iRateIndex422;
                                iVideoFormat = CCamera::EFormatYUV422;
                                }
                            else if (encoderSupports420)
                                {
                                PRINT((_L("CCMRVideoRecorder::SetupEncoderL() YUV420 planar selected as input format") ));
                                iSizeIndex = iSizeIndex420;
                                iRateIndex = iRateIndex420;
                                iVideoFormat = CCamera::EFormatYUV420Planar;
                                }
                            }

                        // check max framerate for given picture size
                        RArray<TPictureRateAndSize> rateAndSize = encoderInfo->MaxPictureRates();
                        TUint rates = rateAndSize.Count();

                        for ( TUint j = 0; j < rates; j++ )
                            {
                            if ( rateAndSize[j].iPictureSize == iFrameSize )
                                {
                                iMaxFrameRate4GivenSize = rateAndSize[j].iPictureRate;
                                break;
                                }
                            }
                        PRINT((_L("CCMRVideoRecorder::SetupEncoderL() - there is a suitable SW plugin.") ));
                        }
                    else
                        {
                        PRINT((_L("CCMRVideoRecorder::SetupEncoderL() - there is already similar encoder - preferring first.") ));
                        // this kind of encoder already found
                        }

                    }
                else
                    {
                    // skip this encoder, done in the following lines
                    PRINT((_L("CCMRVideoRecorder::SetupEncoderL() encoder doesn't support YUV420/YUV422 input format or isn't directcapture encoder") ));
                    }
                }
            else
                {
                PRINT((_L("CCMRVideoRecorder::SetupEncoderL() encoder at index[%d] doesn't support given compressed output format"), i ));
                }
            CleanupStack::PopAndDestroy( encoderInfo );
            }
        }

    TUncompressedVideoFormat preproInputFormat = encoderUncompFormat;
    TUncompressedVideoFormat preproOutputFormat = encoderUncompFormat;
    preproInputFormat.iYuvFormat.iCoefficients = preproInputYuvCoefficient;
    preproOutputFormat.iYuvFormat.iCoefficients = preproOutputYuvCoefficient;

    if ( encoderIndex >= 0 )
        {
        if ( iVideoCodecHWAccelerated )
            {
            iNumCameraBuffers = iConfig->PluginSettings().iCMRNumCameraBuffers;
            PRINT((_L("CCMRVideoRecorder::SetupEncoderL() - found a suitable HW accelerated video encoder")));
            }
        else
            {
            iNumCameraBuffers = iConfig->PluginSettings().iCMRNumCameraBuffersARM;
            PRINT((_L("CCMRVideoRecorder::SetupEncoderL() - found a suitable ARM video encoder")));
            }

        // if encoder doesn´t support directcapture try to find preprocessor that does.
        if ( !iDirectCapture && iVideoCodecHWAccelerated )
            {
            PRINT((_L("CCMRVideoRecorder::SetupEncoderL() Searching preprocessor.")));
            RArray<TUid> preprocessors;
            CleanupClosePushL( preprocessors );
            PRINT((_L("CCMRVideoRecorder::SetupEncoderL() Search starting")));

            TInt preproSearchErr = KErrNone;
            TRAP(preproSearchErr, iDevVideoRec->FindPreProcessorsL(EPpYuvToYuv, preprocessors));
            if (preproSearchErr == KErrNone)
                {
                PRINT((_L("CCMRVideoRecorder::SetupEncoderL() Search found %d preprocessor(s)"), preprocessors.Count() ));
                CPreProcessorInfo* preprocessorInfo = NULL;
                // find a preprocessor with matching capabilities to encoder
                for ( TInt k = 0; k < preprocessors.Count(); k++ )
                    {
                    PRINT((_L("CCMRVideoRecorder::SetupEncoderL() Checking preprocessor at index[%d] with Uid 0x%x"), k, preprocessors[k].iUid ));
                    preprocessorInfo = iDevVideoRec->PreProcessorInfoLC(preprocessors[k]);

                    if ( preprocessorInfo->SupportsInputFormat(preproInputFormat) &&
                         preprocessorInfo->SupportsOutputFormat(preproOutputFormat) &&
                         preprocessorInfo->SupportsDirectCapture() )
                        {
                        PRINT((_L("CCMRVideoRecorder::SetupEncoderL() preprocessor[%d] is suitable, setting it as preprocessor"), k ));
                        iDirectCapture = ETrue;
                        iPreProcessorHWDeviceId = iDevVideoRec->SelectPreProcessorL(preprocessors[k]);
                        PRINT((_L("CCMRVideoRecorder::SetupEncoderL() preprocessor selected")));
                        CleanupStack::PopAndDestroy(preprocessorInfo);
                        break;
                        }
                    CleanupStack::PopAndDestroy(preprocessorInfo);
                    }
                }
            else if (preproSearchErr == KErrNotFound)
                {
                PRINT((_L("CCMRVideoRecorder::SetupEncoderL() didn't find any preprocessors")));
                }
            else
                {
                PRINT((_L("CCMRVideoRecorder::SetupEncoderL() preprocessors search error: %d"), preproSearchErr));
                CleanupStack::PopAndDestroy(); //preprocessors
                User::Leave(preproSearchErr);
                }
            CleanupStack::PopAndDestroy(); //preprocessors
            PRINT((_L("CCMRVideoRecorder::SetupEncoderL() Searching preprocessor done.")));
            }
        }
    else
        {
        if ( infoError != KErrNone )
            {
            // there is an encoder but it can't be used
            PRINT((_L("CCMRVideoRecorder::SetupEncoderL() there is an encoder but it can't be used - info error=%d"), infoError ));
            User::Leave( infoError );
            }
        else
            {
            // No suitable encoder found. This should not be possible if the codec was properly set beforehand
            PRINT((_L("CCMRVideoRecorder::SetupEncoderL() No suitable encoder found.")));
            User::Leave( KErrNotSupported );
            }
        }

    iEncoderHWDeviceId = iDevVideoRec->SelectEncoderL(iAvailableVideoEncoders[encoderIndex]);
    PRINT((_L("CCMRVideoRecorder::SetupEncoderL() encoder selected")));

    // Set input format only for non-directcapture encoders
    if ( (directCaptureEncoder && (supportedEncoderInputsCount == 0)) )
        {
        // Direct capture encoders should ignore
        TUncompressedVideoFormat ignoredFormat;
        iDevVideoRec->SetInputFormatL(iEncoderHWDeviceId, ignoredFormat, iFrameSize);
        }
    else
        {
        iDevVideoRec->SetInputFormatL(iEncoderHWDeviceId, encoderUncompFormat, iFrameSize);
        }

    if ( iPreProcessorHWDeviceId )
        {
        iDevVideoRec->SetInputFormatL(iPreProcessorHWDeviceId, preproInputFormat, iFrameSize);
        }

    // Buffer options
    TEncoderBufferOptions bufferOptions;
    bufferOptions.iHrdVbvParams.Set(NULL, 0);
    bufferOptions.iHrdVbvSpec = EHrdVbvNone;
    bufferOptions.iMaxPreEncoderBufferPictures = iConfig->PluginSettings().iCMRNumCameraBuffers;
    bufferOptions.iMinNumOutputBuffers = iConfig->PluginSettings().iCMRMinNumOutputBuffers;

    bufferOptions.iMaxCodedSegmentSize =
        bufferOptions.iMaxOutputBufferSize =
        bufferOptions.iMaxCodedPictureSize = iVideoCodecData->MaxBufferLength(iFrameSize);

    iDevVideoRec->SetBufferOptionsL(bufferOptions);

    // Set output format
    iDevVideoRec->SetOutputFormatL(iEncoderHWDeviceId, *comprFormat, EDuCodedPicture,
                                   outputFormatEncapsulation, EFalse /* aSegmentationAllowed */);

    if( iPreProcessorHWDeviceId)
    {
        iDevVideoRec->SetOutputFormatL(iPreProcessorHWDeviceId, preproOutputFormat);
    }

    PRINT((_L("CCMRVideoRecorder::SetupEncoderL() MinRandomAccess= %f"), iMinRandomAccessPeriodInSeconds ));
    if ( iConfig && iConfig->IsICMConfigDataAvailable() )
        {
        iMinRandomAccessPeriodInSeconds = TReal(1)/ iConfig->VideoQualitySettings().iRandomAccessRate;
        PRINT((_L("CCMRVideoRecorder::SetupEncoderL() Set to iConfig iMinRandomAccessPeriodInSeconds= %f"), iMinRandomAccessPeriodInSeconds ));        
        iDevVideoRec->SetMinRandomAccessRate( iConfig->VideoQualitySettings().iRandomAccessRate );
        }
    else
        {
        if ( (iFrameSize.iWidth >= KCMRCIFWidth) && (iFrameSize.iHeight >= KCMRCIFHeight) )
	        {
	        iMinRandomAccessPeriodInSeconds = KCMRMinRandomAccessPeriodHighRes;
	        }
        else 
            {
            iMinRandomAccessPeriodInSeconds = KCMRMinRandomAccessPeriod;
            }
        PRINT((_L("CCMRVideoRecorder::SetupEncoderL() Set to MinRandomAccess= %f"), iMinRandomAccessPeriodInSeconds ));        
        iDevVideoRec->SetMinRandomAccessRate( TReal(1) / TReal(iMinRandomAccessPeriodInSeconds) );
        }

    CleanupStack::PopAndDestroy( comprFormat );

    // set codec specific settings
    iVideoCodecData->SetPreInitParamsL(iDevVideoRec);

    if (iDirectCapture)
        {
        TInt status = KErrNone;
        // Set the data source to a camera
        TRAP( status, iDevVideoRec->SetSourceCameraL( iCameraHandle, TReal(iSourceFrameRate)) );

        if ( status != KErrNone )
            {
            // Probably it's not supported, although directCapture was declared in the hwdevice info.
            // Leave or to give one more chance to encoder, to use src memory as a source ?
            // Yes, otherwise User::Leave(status);
            PRINT((_L("CCMRVideoRecorder::SetupEncoderL() Setting camerasource to MDF failed.")));
            }
        else
            {
            if ( iClockSource )
                {
                iDevVideoRec->SetClockSource( iClockSource );
                }
            // Ok, use direct capture
            PRINT((_L("CCMRVideoRecorder::SetupEncoderL() out")));
            return;
            }
        }

    // Set source memory is allocated & released by camera API
    iDevVideoRec->SetSourceMemoryL( TReal(iSourceFrameRate), ETrue, KCMREncodingRealTime);
    // Set the source to memory buffers
    iDirectCapture = EFalse;
    PRINT((_L("CCMRVideoRecorder::SetupEncoderL() out")));
    }


// ---------------------------------------------------------
// CCMRVideoRecorder::ReadEncoderInfoL
// Read encoder information object. The object is popped from
// stack and MUST be handled properly by the caller
// (other items were commented in a header).
// ---------------------------------------------------------
//
CVideoEncoderInfo* CCMRVideoRecorder::ReadEncoderInfoL(TUid aUid)
    {
    PRINT((_L("CCMRVideoRecorder::ReadEncoderInfoL(), In")))
    CVideoEncoderInfo* info = iDevVideoRec->VideoEncoderInfoLC( aUid );
    CleanupStack::Pop(info);

    PRINT((_L("CCMRVideoRecorder::ReadEncoderInfoL(), Out")))
    return info;
    }


// ---------------------------------------------------------
// CCMRVideoRecorder::PrepareL
// Prepares the recorder for recording
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CCMRVideoRecorder::PrepareL()
    {
    PRINT((_L("CCMRVideoRecorder::PrepareL(), In")));

    if ( (State() != EStateOpen) && (State() != EStateReadyToRecord) )
        {
        PRINT((_L("CCMRVideoRecorder::PrepareL() wrong state")));
        User::Leave( KErrNotReady );
        }

	if ( State() == EStateReadyToRecord )
		{
		// No prepare needed... already ready.
		// everything ok, inform observer that we are prepared.
		PRINT((_L("CCMRVideoRecorder::PrepareL(), Already prepared. Return right away")));
        DoSendEventToClient( KCMRPrepareComplete, KErrNone );
		return;
		}

	if (iDevVideoRec)
		{
		delete iDevVideoRec;
		iDevVideoRec = NULL;
		}

	iDevVideoRec = CMMFDevVideoRecord::NewL( *this );

    // reset error code
    iErrorCode = KErrNone;

    // If the user of the recorder API has not set valid settings,
    // use default ones

    if ( ((iSizeIndex420 < 0) && (iSizeIndex422 < 0) && (iSizeIndexDCEncoder < 0))
        || ((iSizeIndex420 >= iCameraInfo.iNumVideoFrameSizesSupported) && (iSizeIndex422 >= iCameraInfo.iNumVideoFrameSizesSupported)) )
        {
        // set default frame size
        SetFrameSizeL( TSize( KCMRFrameWidth, KCMRFrameHeight ) );
        }

    if ( ((iRateIndex420 < 0) && (iRateIndex422 < 0) && (iRateIndexDCEncoder < 0))
        || ((iRateIndex420 >= iCameraInfo.iNumVideoFrameRatesSupported) && (iRateIndex422 >= iCameraInfo.iNumVideoFrameRatesSupported)) )
        {
        // set default frame rate
        SetFrameRateL( KCMRFrameRate );
        }

    // if during previous run a new frame rate was requested; when requested it affected only on encoding rate
    if ( iRequestedFrameRate != iSourceFrameRate )
        {
        SetState(EStateOpen);//change state to make the SetFrameRate work correctly
        SetFrameRateL( iRequestedFrameRate );
        }

    // reset init completion flags
    iEncoderInitComplete = EFalse;
    iSourceInitComplete = EFalse;
    iVideoCodecHWAccelerated = EFalse;
    // Select & set parameters to video encoder
    TRAPD(err, SetupEncoderL());
    if ( err == KErrHardwareNotAvailable && iVideoCodecHWAccelerated )
        {
        // HW codec exists but can't be used, try again with ARM codec
        SetupEncoderL();
        }
    else if ( err != KErrNone )
        {
        // some other error
        PRINT((_L("CCMRVideoRecorder::PrepareL(), error [%d]"), err));
        User::Leave( err );
        }

    SetState(EStatePreparing);

    // Reserve camera for use
    // Possibly asynchronous call (if not already reserved), completion is anyway informed using this->ReserveComplete()
    // iSource has to be reserved only if we don't set hwdevice to use DirectCapture.
    if ( !iDirectCapture )
        {
        iSource->Reserve();
        }

    // Initialize DevVideoRec
    // asynchronous call, completion informed using this->MdvroInitializeComplete()
    iDevVideoRec->Initialize();

    PRINT((_L("CCMRVideoRecorder::PrepareL(), Out")));
    }


// ---------------------------------------------------------
// CCMRVideoRecorder::RecordL
// Starts recording
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CCMRVideoRecorder::RecordL()
    {
	PRINT((_L("CCMRVideoRecorder::RecordL() - START - iRequestStatus: 0x%08x"), iRequestStatus ));

    if ( (State() == EStateRecording) || (State() == EStatePaused) )
        {
        // ignore, already recording
        PRINT((_L("CCMRVideoRecorder::RecordL() ignore")));
        return;
        }

    if ( State() != EStateReadyToRecord )
        {
        PRINT((_L("CCMRVideoRecorder::RecordL() not ready")));
        User::Leave(KErrNotReady);
        }

    // check exposure setting from camera => decide framerates
    CheckExposure();

#ifdef _DEBUG
    iRecordStartTime.UniversalTime();
    iLastCapture.UniversalTime();
#endif
    iNumberOfCapturedFrames = 0;
    iNumberOfEncodedFrames = 0;
    iEncoderInputQueueLength = 0;
    iDriftFrameSkipCount = 0;
    iAddedFrameDurationCount = 0;
    iPreviousCameraFrameIndex = 0;
    iDecSpecInfoLength = 0;

    // reset pause time adjustments variables
    iTimeWhenPaused = 0;
    iTotalPausedTime = 0;
    iSystemClockDelta = 0;
    iLatestUsedTimeStamp = -iSourceFrameInterval;
    iLatestAbsoluteTimeStamp = 0;
    iNumberOfCapturedFrames = 0;
    iNumberOfVideoOutputBuffers = 0;

    if ( iVideoCodecData->DecoderConfigInfoUsed() )
        {
        // Read decoder specific information from the encoder
        PRINT((_L("CCMRVideoRecorder::RecordL() asking CodingStandardSpecificInitOutputLC from the encoder.")));
        iDecSpecInfo = iDevVideoRec->CodingStandardSpecificInitOutputLC();
        if ( iDecSpecInfo )
            {
            PRINT((_L("CCMRVideoRecorder::RecordL() CodingStandardSpecificInitOutputLC received.")));
            // it was placed to cleanupstack, pop it now since it takes some time before
            // this ptr can be destructed, and if destructed from the stack, there may
            // be newer objects in the stack at that time. The ptr is also now stored
            // to member variable which must not be in stack
            CleanupStack::Pop(iDecSpecInfo);

            // Inform the output that we have new data; it will read it in callback
            if ( iRequestStatus )
                {
				PRINT((_L("CCMRVideoRecorder::RecordL() - completing output request..." )));
                iOutputThreadHandle.RequestComplete( iRequestStatus, KErrNone );
                }
            }
        else
            {
            // This codec should have decoder specific info field in metadata, hence leave with error
            PRINT((_L("CCMRVideoRecorder::RecordL() no decoder specific info available from the encoder!?!")));
            User::Leave( KErrNotSupported );
            }
        }

    // Start encoding
    iDevVideoRec->Start();
    iSkipBuffers = EFalse;

    iInputEnd = EFalse;
    iStreamEnd = EFalse;

    // Start capturing
    // StartCapture in case, if we don't use direct capture mode
    if ( !iDirectCapture )
        {
        iSource->StartCapture();
        }

    SetState(EStateRecording);

	PRINT((_L("CCMRVideoRecorder::RecordL() - END - iRequestStatus: 0x%08x"), iRequestStatus ));
    }


// ---------------------------------------------------------
// CCMRVideoRecorder::StopL
// Stops recording
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CCMRVideoRecorder::StopL()
    {
    PRINT((_L("CCMRVideoRecorder::StopL(), In")))

    if ( (State() == EStateRecording) || (State() == EStatePaused) )
        {
        PRINT((_L("CCMRVideoRecorder::StopL(), Stop capture")));

        // Stop capturing
        // Stop capture only incase, if we don't use direct capture mode
        if ( !iDirectCapture )
            {
            iSource->StopCapture();
            PRINT((_L("CCMRVideoRecorder::StopL() camera stopped")));
            }


        // Stopping is async => state can't be "recording"
        SetState(EStateStopping);

        // Stop encoding
        if ( !iFatalError )
            {
            iInputEnd = ETrue;
            PRINT((_L("CCMRVideoRecorder::StopL(), calling DevVideo::InputEnd().")));                
            iDevVideoRec->InputEnd();
            // try to free buffers for video adaptation to complete video bitstream / EOS marker.
            PRINT((_L("CCMRVideoRecorder::StopL(), flushing written frames back to DevVideo.")));                
            iBufferReturnAO->Flush();
            
            // get all available buffers from devvideo
            PRINT((_L("CCMRVideoRecorder::StopL(), getting all encoded frames from DevVideo.")));                
            TVideoOutputBuffer* buffer = NULL;
            for (;;)
                {
                TRAPD( err, ( buffer = iDevVideoRec->NextBufferL()));
                if ( err != KErrNone || buffer == NULL)
                    {
                    break;
                    }
                // enter restricted area
                iMutexObj.Wait();
                // store
                TInt timestamp = I64INT(buffer->iCaptureTimestamp.Int64());
                PRINT((_L("CCMRVideoRecorder::StopL(), storing buffer: 0x%x, timestamp:%d"), buffer, timestamp ));                  
                iVideoOutputBufferInputQue.AddLast(*buffer);
                iNumberOfVideoOutputBuffers++;

                // leave restricted area
                iMutexObj.Signal();
                }
            }
        else
            {
            PRINT((_L("CCMRVideoRecorder::StopL(), Fatal error was reported by devVideo, next step is to delete it")));

            // set state & inform MR => it can return from stop
            SetState( EStateReadyToRecord );

            // everything ok, inform observer that we are ready for a new recording (iStopping does it too, though).
            DoSendEventToClient( KCMRRecordingComplete, KErrNone );
            }

        PRINT((_L("CCMRVideoRecorder::StopL() out, must wait for state change before stop completed")));
        }
    else
        {
        DoSendEventToClient( KCMRRecordingComplete, KErrNone );
        PRINT((_L("CCMRVideoRecorder::StopL() out, already stopped")));
        }
    }

// ---------------------------------------------------------
// CCMRVideoRecorder::RequestBuffersAndWaitEOSL
// While stopping keep requesting buffers from adaptation and waiting for EOS marker encodc signal
// (other items were commented in a header).
// ---------------------------------------------------------
//

void CCMRVideoRecorder::RequestBuffersAndWaitEOSL(TInt& aVideoEOSReached)
    {
    PRINT((_L("CCMRVideoRecorder::RequestBuffersAndWaitEOSL(), in")))
    PRINT((_L("CCMRVideoRecorder::RequestBuffersAndWaitEOSL(), flushing written frames back to DevVideo.")));                
    iBufferReturnAO->Flush();

    // get all available buffers from devvideo
    PRINT((_L("CCMRVideoRecorder::RequestBuffersAndWaitEOSL(), getting all encoded frames from DevVideo.")));                
    TVideoOutputBuffer* buffer = NULL;
    for (;;)
        {
        TRAPD( err, ( buffer = iDevVideoRec->NextBufferL()));
        if ( err != KErrNone || buffer == NULL)
            {
            break;
            }
        // enter restricted area
        iMutexObj.Wait();
        // store
        TInt timestamp = I64INT(buffer->iCaptureTimestamp.Int64());
        PRINT((_L("CCMRVideoRecorder::RequestBuffersAndWaitEOSL(), storing buffer: 0x%x, timestamp:%d"), buffer, timestamp));                  
        iVideoOutputBufferInputQue.AddLast(*buffer);
        iNumberOfVideoOutputBuffers++;

        // leave restricted area
        iMutexObj.Signal();
        }
    
    aVideoEOSReached = iStreamEnd;
    PRINT((_L("CCMRVideoRecorder::RequestBuffersAndWaitEOSL() out, aVideoEOSReached=%d"), aVideoEOSReached));
    }

// ---------------------------------------------------------
// CCMRVideoRecorder::PauseL
// Pauses recording
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CCMRVideoRecorder::PauseL()
    {
    PRINT((_L("CCMRVideoRecorder::PauseL(), In")))

    if ( State() != EStateRecording )
        {
        return;
        }

    SetState( EStatePaused );

    // Take the time stamp and later subtract it from real stamps
    iTimeWhenPaused.UniversalTime();
    PRINT((_L("CCMRVideoRecorder::PauseL() at %i:%i"),I64LOW(iTimeWhenPaused.Int64()), I64HIGH(iTimeWhenPaused.Int64()) ));

    // Adjust in the clock time changes, so we don't add it in twice during resume
    iTimeWhenPaused = iTimeWhenPaused + iSystemClockDelta;
    PRINT((_L("CCMRVideoRecorder::PauseL() at %i:%i"),I64LOW(iTimeWhenPaused.Int64()), I64HIGH(iTimeWhenPaused.Int64()) ));

    // Pause encoding
    iDevVideoRec->Pause();
    PRINT((_L("CCMRVideoRecorder::PauseL(), In")))
    }


// ---------------------------------------------------------
// CCMRVideoRecorder::ResumeL
// Resumes recording
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CCMRVideoRecorder::ResumeL()
    {
    PRINT((_L("CCMRVideoRecorder::ResumeL(), In")))

    if ( State() != EStatePaused )
        {
        return;
        }

    // Resume encoding
    iDevVideoRec->Resume();

    // measure the time we were paused; remember that this could have been Nth pause
    TTime current;
    current.UniversalTime();
    iTotalPausedTime = iTotalPausedTime.Int64() + current.MicroSecondsFrom(iTimeWhenPaused-iSystemClockDelta).Int64();
    PRINT((_L("CCMRVideoRecorder::ResumeL() at %i:%i, iTotalPausedTime now %d, iSystemClockDelta now %d"),I64LOW(current.Int64()),
                                                                                                          I64HIGH(current.Int64()),
                                                                                                          I64INT(iTotalPausedTime.Int64()),
                                                                                                          I64INT(iSystemClockDelta.Int64()) ));

    SetState( EStateRecording );
    PRINT((_L("CCMRVideoRecorder::ResumeL(), Out")))
    }


// -----------------------------------------------------------------------------
// CCMRVideoRecorder::SetVideoCodecL
// Set video codec.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CCMRVideoRecorder::SetVideoCodecL(const TDesC8& aMimeType)
    {
    PRINT((_L("CCMRVideoRecorder::SetVideoCodecL() in")));
    if ( ( State() != EStateOpen ) && ( State() != EStateReadyToRecord ) )
        {
        PRINT((_L("CCMRVideoRecorder::SetVideoCodecL() wrong state")));
        User::Leave(KErrNotReady);
        }

    if ( iMimeType == aMimeType )
        {
        // the same codec was already selected, no need to change anything
        PRINT((_L("CCMRVideoRecorder::SetVideoCodecL() out, no change needed. Requested codec already in use.")));
        return;
        }

	TInt numerator = KCMRAspectRatioNum;
	TInt denominator = KCMRAspectRatioDenom;
    TInt videoCodecLevel = 10;  // default: H.263 p0 level 10
    TBuf8<256> newMimeType;
    if ( aMimeType == KNullDesC8 )
        {
        PRINT((_L("CCMRVideoRecorder::SetVideoCodecL() Requested NULL video codec, default H.263 Baseline profile, level 10 in use.")));
        // use default (H.263 p0 level 10); theoretically this could be useful if codec is first set to smth else and then reset to default
        newMimeType = KCMRMimeTypeH263BaselineProfile;  //copy the contents
        newMimeType += _L8( "; level=10" );    //append level
        videoCodecLevel = 10;
        }
    else {
        // check the given type first
        TBuf8<256> string;
        string = KCMRMimeTypeH263;
        string += _L8( "*" );

        // the client-class checked the availability of the codec in the system already

        if ( aMimeType.MatchF( (const TDesC8& )string ) != KErrNotFound )
            {
            // H.263
            PRINT((_L("CCMRVideoRecorder::SetVideoCodecL() H.263 requested")));
            newMimeType = KCMRMimeTypeH263; //copy the contents

            if ( aMimeType.MatchF( _L8("*profile*") ) != KErrNotFound )
                {
                // profile given, check if we support it
                if ( aMimeType.MatchF( _L8("*profile=0*")) != KErrNotFound )
                    {
                    // profile 0 requested
                    PRINT((_L("CCMRVideoRecorder::SetVideoCodecL() H.263, profile 0 requested")));
                    newMimeType += _L8( "; profile=0" );    //append
                    }
                else if ( aMimeType.MatchF( _L8("*profile=3*")) != KErrNotFound )
                    {
                    // profile 3 requested
                    PRINT((_L("CCMRVideoRecorder::SetVideoCodecL() H.263, profile 3 requested")));
                    newMimeType += _L8( "; profile=3" );    //append
                    }
                else
                    {
                    // no other profiles supported
                    PRINT((_L("CCMRVideoRecorder::SetVideoCodecL() unsupported profile")));
                    User::Leave(KErrNotSupported);
                    }
                }
            else
                {
                // no profile is given => assume 0
                PRINT((_L("CCMRVideoRecorder::SetVideoCodecL() H.263, no profile requested - default to 0.")));
                newMimeType += _L8( "; profile=0" );    //append
                }

            if ( aMimeType.MatchF( _L8("*level=10*") ) != KErrNotFound )
                {
                PRINT((_L("CCMRVideoRecorder::SetVideoCodecL() H.263, level 10 requested.")));
                videoCodecLevel = 10;
                newMimeType += _L8( "; level=10" );    //append
                }
            else if ( aMimeType.MatchF( _L8("*level=20*") ) != KErrNotFound )
                {
                PRINT((_L("CCMRVideoRecorder::SetVideoCodecL() H.263, level 20 requested.")));
                videoCodecLevel = 20;
                newMimeType += _L8( "; level=20" );    //append
                }
            else if ( aMimeType.MatchF( _L8("*level=30*") ) != KErrNotFound )
                {
                PRINT((_L("CCMRVideoRecorder::SetVideoCodecL() H.263, level 30 requested.")));
                videoCodecLevel = 30;
                newMimeType += _L8( "; level=30" );    //append
                }
            else if ( aMimeType.MatchF( _L8("*level=40*") ) != KErrNotFound )
                {
                PRINT((_L("CCMRVideoRecorder::SetVideoCodecL() H.263, level 40 requested.")));
                videoCodecLevel = 40;
                newMimeType += _L8( "; level=40" );    //append
                }
            else if ( aMimeType.MatchF( _L8("*level=45*") ) != KErrNotFound )
                {
                PRINT((_L("CCMRVideoRecorder::SetVideoCodecL() H.263, level 45 requested.")));
                videoCodecLevel = 45;
                newMimeType += _L8( "; level=45" );    //append
                }
            else if ( aMimeType.MatchF( _L8("*level=50*") ) != KErrNotFound )
                {
                PRINT((_L("CCMRVideoRecorder::SetVideoCodecL() H.263, level 50 requested.")));
                videoCodecLevel = 50;
                newMimeType += _L8( "; level=50" );    //append
                }
            else if ( aMimeType.MatchF( _L8("*level*") ) != KErrNotFound )
                {
                // no other levels supported
                PRINT((_L("CCMRVideoRecorder::SetVideoCodecL() unsupported level requested.")));
                User::Leave(KErrNotSupported);
                }
            else
                {
                // if no level is given assume 10
                PRINT((_L("CCMRVideoRecorder::SetVideoCodecL() H.263, no level requested - default to 10.")));
                newMimeType += _L8( "; level=10" );    //append
                videoCodecLevel = 10;
                }

            // recreate the codec data object
            PRINT((_L("CCMRVideoRecorder::SetVideoCodecL() H.263 recreating codecdata.")));
            delete iVideoCodecData;
            iVideoCodecData = NULL;
            iVideoCodecData = new (ELeave) CCMRVideoCodecDataH263(videoCodecLevel);
            }
        else
            {
			numerator = KCMRMPEG4AspectRatioNum;
			denominator = KCMRMPEG4AspectRatioDenom;

            string = KCMRMimeTypeMPEG4V;
            string += _L8( "*" );

            if ( aMimeType.MatchF( string ) != KErrNotFound )
                {
                // MPEG-4 Visual
                PRINT((_L("CCMRVideoRecorder::SetVideoCodecL() MPEG-4 Visual requested")));
                newMimeType = KCMRMimeTypeMPEG4VSP; //copy the contents
                if ( aMimeType.MatchF( _L8("*profile-level-id=8*") ) != KErrNotFound )
                    {
                    // simple profile level 0
                    PRINT((_L("CCMRVideoRecorder::SetVideoCodecL() MPEG-4, simple profile level 0 requested.")));
                    videoCodecLevel = 0;
                    newMimeType += _L8("8");
                    }
                else if ( aMimeType.MatchF( _L8("*profile-level-id=9*") ) != KErrNotFound )
                    {
                    // simple profile level 0b
                    PRINT((_L("CCMRVideoRecorder::SetVideoCodecL() MPEG-4, simple profile level 0b requested.")));
                    videoCodecLevel = KCMRMPEG4SPLevel0B;
                    newMimeType += _L8("9");
                    }
                else if ( aMimeType.MatchF( _L8("*profile-level-id=1*") ) != KErrNotFound )
                    {
                    // simple profile level 1
                    PRINT((_L("CCMRVideoRecorder::SetVideoCodecL() MPEG-4, simple profile level 1 requested.")));
                    videoCodecLevel = 1;
                    newMimeType += _L8("1");
                    }
                else if ( aMimeType.MatchF( _L8("*profile-level-id=2*") ) != KErrNotFound )
                    {
                    // simple profile level 2
                    PRINT((_L("CCMRVideoRecorder::SetVideoCodecL() MPEG-4, simple profile level 2 requested.")));
                    videoCodecLevel = 2;
                    newMimeType += _L8("2");
                    }
                else if ( aMimeType.MatchF( _L8("*profile-level-id=3*") ) != KErrNotFound )
                    {
                    // simple profile level 3
                    PRINT((_L("CCMRVideoRecorder::SetVideoCodecL() MPEG-4, simple profile level 3 requested.")));
                    videoCodecLevel = 3;
                    newMimeType += _L8("3");
                    }
                else if ( aMimeType.MatchF( _L8("*profile-level-id=4*") ) != KErrNotFound )
                    {
                    // simple profile level 4
                    PRINT((_L("CCMRVideoRecorder::SetVideoCodecL() MPEG-4, simple profile level 4a requested.")));
                    videoCodecLevel = 4;
                    newMimeType += _L8("4");
                    }
                else if ( aMimeType.MatchF( _L8("*profile-level-id=5*") ) != KErrNotFound )
                    {
                    // simple profile level 5
                    PRINT((_L("CCMRVideoRecorder::SetVideoCodecL() MPEG-4, simple profile level 5 requested.")));
                    videoCodecLevel = 5;
                    newMimeType += _L8("5");
                    }
                else if ( aMimeType.MatchF( _L8("*profile-level-id=6*") ) != KErrNotFound )
                    {
                    // simple profile level 6
                    PRINT((_L("CCMRVideoRecorder::SetVideoCodecL() MPEG-4, simple profile level 6 requested.")));
                    videoCodecLevel = 6;
                    newMimeType += _L8("6");
                    }
                else if ( aMimeType.MatchF( _L8("*profile-level-id=*") ) != KErrNotFound )
                    {
                    // no other profile-level ids supported
                    PRINT((_L("CCMRVideoRecorder::SetVideoCodecL() unsupported profile-level requested.")));
                    User::Leave(KErrNotSupported);
                    }
                else
                    {
                    // Default is level 1
                    PRINT((_L("CCMRVideoRecorder::SetVideoCodecL() MPEG-4, no profile requested - defaulting to 1.")));
                    videoCodecLevel = 1;
                    newMimeType += _L8("1");
                    }

                // recreate the codec data object
                PRINT((_L("CCMRVideoRecorder::SetVideoCodecL() MPEG-4 recreating codecdata.")));
                delete iVideoCodecData;
                iVideoCodecData = NULL;
                iVideoCodecData = new (ELeave) CCMRVideoCodecDataMPEG4(videoCodecLevel);
                }
            else
                {
				numerator = KCMRMPEG4AspectRatioNum;
				denominator = KCMRMPEG4AspectRatioDenom;

                string = KCMRMimeTypeH264AVC;
                string += _L8( "*" );

                if ( aMimeType.MatchF( string ) != KErrNotFound )
                    {
                    // H.264/AVC
                    PRINT((_L("CCMRVideoRecorder::SetVideoCodecL() H.264/AVC requested")));
                    newMimeType = KCMRMimeTypeH264AVCProfileId; //copy the contents
                    if ( aMimeType.MatchF( _L8("*profile-level-id=42800A*") ) != KErrNotFound )
                        {
                        // baseline profile level 1
                        PRINT((_L("CCMRVideoRecorder::SetVideoCodecL() H.264/AVC, baseline profile level 1 requested.")));
                        videoCodecLevel = KCMRH264AVCCodecLevel10;
                        newMimeType += _L8("42800A");
                        }
                    else if ( aMimeType.MatchF( _L8("*profile-level-id=42900B*") ) != KErrNotFound )
                        {
                        // baseline profile level 1b
                        PRINT((_L("CCMRVideoRecorder::SetVideoCodecL() H.264/AVC, baseline profile level 1b requested.")));
                        videoCodecLevel = KCMRH264AVCCodecLevel10b;
                        newMimeType += _L8("42900B");
                        }
                    else if ( aMimeType.MatchF( _L8("*profile-level-id=42800B*") ) != KErrNotFound )
                        {
                        // baseline profile level 1.1
                        PRINT((_L("CCMRVideoRecorder::SetVideoCodecL() H.264/AVC, baseline profile level 1.1 requested.")));
                        videoCodecLevel = KCMRH264AVCCodecLevel11;
                        newMimeType += _L8("42800B");
                        }
                    else if ( aMimeType.MatchF( _L8("*profile-level-id=42800C*") ) != KErrNotFound )
                        {
                        // baseline profile level 1.2
                        PRINT((_L("CCMRVideoRecorder::SetVideoCodecL() H.264/AVC, baseline profile level 1.2 requested.")));
                        videoCodecLevel = KCMRH264AVCCodecLevel12;
                        newMimeType += _L8("42800C");
                        }
                    else if ( aMimeType.MatchF( _L8("*profile-level-id=42800D*") ) != KErrNotFound )
                        {
                        // baseline profile level 1.3
                        PRINT((_L("CCMRVideoRecorder::SetVideoCodecL() H.264/AVC, baseline profile level 1.3 requested.")));
                        videoCodecLevel = KCMRH264AVCCodecLevel13;
                        newMimeType += _L8("42800D");
                        }
                    else if ( aMimeType.MatchF( _L8("*profile-level-id=428014*") ) != KErrNotFound )
                        {
                        // baseline profile level 2
                        PRINT((_L("CCMRVideoRecorder::SetVideoCodecL() H.264/AVC, baseline profile level 2 requested.")));
                        videoCodecLevel = KCMRH264AVCCodecLevel20;
                        newMimeType += _L8("428014");
                        }
                    else if ( aMimeType.MatchF( _L8("*profile-level-id=428015*") ) != KErrNotFound )
                        {
                        // baseline profile level 2.1
                        PRINT((_L("CCMRVideoRecorder::SetVideoCodecL() H.264/AVC, baseline profile level 2.1 requested.")));
                        videoCodecLevel = KCMRH264AVCCodecLevel21;
                        newMimeType += _L8("428015");
                        }
                    else if ( aMimeType.MatchF( _L8("*profile-level-id=428016*") ) != KErrNotFound )
                        {
                        // baseline profile level 2.2
                        PRINT((_L("CCMRVideoRecorder::SetVideoCodecL() H.264/AVC, baseline profile level 2.2 requested.")));
                        videoCodecLevel = KCMRH264AVCCodecLevel22;
                        newMimeType += _L8("428016");
                        }
                    else if ( aMimeType.MatchF( _L8("*profile-level-id=42801E*") ) != KErrNotFound )
                        {
                        // baseline profile level 3
                        PRINT((_L("CCMRVideoRecorder::SetVideoCodecL() H.264/AVC, baseline profile level 3 requested.")));
                        videoCodecLevel = KCMRH264AVCCodecLevel30;
                        newMimeType += _L8("42801E");
                        }
                    else if ( aMimeType.MatchF( _L8("*profile-level-id=42801F*") ) != KErrNotFound )
                        {
                        // baseline profile level 3.1
                        PRINT((_L("CCMRVideoRecorder::SetVideoCodecL() H.264/AVC, baseline profile level 3.1 requested.")));
                        videoCodecLevel = KCMRH264AVCCodecLevel31;
                        newMimeType += _L8("42801F");
                        }
                    else if ( aMimeType.MatchF( _L8("*profile-level-id=428020*") ) != KErrNotFound )
                        {
                        // baseline profile level 3.2
                        PRINT((_L("CCMRVideoRecorder::SetVideoCodecL() H.264/AVC, baseline profile level 3.2 requested.")));
                        videoCodecLevel = KCMRH264AVCCodecLevel32;
                        newMimeType += _L8("428020");
                        }
                    else if ( aMimeType.MatchF( _L8("*profile-level-id=428028*") ) != KErrNotFound )
                        {
                        // baseline profile level 4
                        PRINT((_L("CCMRVideoRecorder::SetVideoCodecL() H.264/AVC, baseline profile level 4 requested.")));
                        videoCodecLevel = KCMRH264AVCCodecLevel40;
                        newMimeType += _L8("428028");
                        }
                    else if ( aMimeType.MatchF( _L8("*profile-level-id=4D400A*") ) != KErrNotFound )
                        {
                        // main profile level 1
                        PRINT((_L("CCMRVideoRecorder::SetVideoCodecL() H.264/AVC, main profile level 1 requested.")));
                        videoCodecLevel = KCMRH264AVCCodecLevel10;
                        newMimeType += _L8("4D400A");
                        }
                    else if ( aMimeType.MatchF( _L8("*profile-level-id=4D500B*") ) != KErrNotFound )
                        {
                        // main profile level 1b
                        PRINT((_L("CCMRVideoRecorder::SetVideoCodecL() H.264/AVC, main profile level 1b requested.")));
                        videoCodecLevel = KCMRH264AVCCodecLevel10b;
                        newMimeType += _L8("4D500B");
                        }
                    else if ( aMimeType.MatchF( _L8("*profile-level-id=4D400B*") ) != KErrNotFound )
                        {
                        // main profile level 1.1
                        PRINT((_L("CCMRVideoRecorder::SetVideoCodecL() H.264/AVC, main profile level 1.1 requested.")));
                        videoCodecLevel = KCMRH264AVCCodecLevel11;
                        newMimeType += _L8("4D400B");
                        }
                    else if ( aMimeType.MatchF( _L8("*profile-level-id=4D400C*") ) != KErrNotFound )
                        {
                        // main profile level 1.2
                        PRINT((_L("CCMRVideoRecorder::SetVideoCodecL() H.264/AVC, main profile level 1.2 requested.")));
                        videoCodecLevel = KCMRH264AVCCodecLevel12;
                        newMimeType += _L8("4D400C");
                        }
                    else if ( aMimeType.MatchF( _L8("*profile-level-id=4D400D*") ) != KErrNotFound )
                        {
                        // main profile level 1.3
                        PRINT((_L("CCMRVideoRecorder::SetVideoCodecL() H.264/AVC, main profile level 1.3 requested.")));
                        videoCodecLevel = KCMRH264AVCCodecLevel13;
                        newMimeType += _L8("4D400D");
                        }
                    else if ( aMimeType.MatchF( _L8("*profile-level-id=4D4014*") ) != KErrNotFound )
                        {
                        // main profile level 2
                        PRINT((_L("CCMRVideoRecorder::SetVideoCodecL() H.264/AVC, main profile level 2 requested.")));
                        videoCodecLevel = KCMRH264AVCCodecLevel20;
                        newMimeType += _L8("4D4014");
                        }
                    else if ( aMimeType.MatchF( _L8("*profile-level-id=4D4015*") ) != KErrNotFound )
                        {
                        // main profile level 2.1
                        PRINT((_L("CCMRVideoRecorder::SetVideoCodecL() H.264/AVC, main profile level 2.1 requested.")));
                        videoCodecLevel = KCMRH264AVCCodecLevel21;
                        newMimeType += _L8("4D4015");
                        }
                    else if ( aMimeType.MatchF( _L8("*profile-level-id=4D4016*") ) != KErrNotFound )
                        {
                        // main profile level 2.2
                        PRINT((_L("CCMRVideoRecorder::SetVideoCodecL() H.264/AVC, main profile level 2.2 requested.")));
                        videoCodecLevel = KCMRH264AVCCodecLevel22;
                        newMimeType += _L8("4D4016");
                        }
                    else if ( aMimeType.MatchF( _L8("*profile-level-id=4D401E*") ) != KErrNotFound )
                        {
                        // main profile level 3
                        PRINT((_L("CCMRVideoRecorder::SetVideoCodecL() H.264/AVC, main profile level 3 requested.")));
                        videoCodecLevel = KCMRH264AVCCodecLevel30;
                        newMimeType += _L8("4D401E");
                        }
                    else if ( aMimeType.MatchF( _L8("*profile-level-id=4D401F*") ) != KErrNotFound )
                        {
                        // main profile level 3.1
                        PRINT((_L("CCMRVideoRecorder::SetVideoCodecL() H.264/AVC, main profile level 3.1 requested.")));
                        videoCodecLevel = KCMRH264AVCCodecLevel31;
                        newMimeType += _L8("4D401F");
                        }
                    else if ( aMimeType.MatchF( _L8("*profile-level-id=4D4020*") ) != KErrNotFound )
                        {
                        // main profile level 3.2
                        PRINT((_L("CCMRVideoRecorder::SetVideoCodecL() H.264/AVC, main profile level 3.2 requested.")));
                        videoCodecLevel = KCMRH264AVCCodecLevel32;
                        newMimeType += _L8("4D4020");
                        }
                    else if ( aMimeType.MatchF( _L8("*profile-level-id=4D4028*") ) != KErrNotFound )
                        {
                        // main profile level 4
                        PRINT((_L("CCMRVideoRecorder::SetVideoCodecL() H.264/AVC, main profile level 4 requested.")));
                        videoCodecLevel = KCMRH264AVCCodecLevel40;
                        newMimeType += _L8("4D4028");
                        }
                    else if ( aMimeType.MatchF( _L8("*profile-level-id=64400A*") ) != KErrNotFound )
                        {
                        // high profile level 1
                        PRINT((_L("CCMRVideoRecorder::SetVideoCodecL() H.264/AVC, high profile level 1 requested.")));
                        videoCodecLevel = KCMRH264AVCCodecLevel10;
                        newMimeType += _L8("64400A");
                        }
                    else if ( aMimeType.MatchF( _L8("*profile-level-id=644009*") ) != KErrNotFound )
                        {
                        // high profile level 1b
                        PRINT((_L("CCMRVideoRecorder::SetVideoCodecL() H.264/AVC, high profile level 1b requested.")));
                        videoCodecLevel = KCMRH264AVCCodecLevel10b;
                        newMimeType += _L8("644009");
                        }
                    else if ( aMimeType.MatchF( _L8("*profile-level-id=64400B*") ) != KErrNotFound )
                        {
                        // high profile level 1.1
                        PRINT((_L("CCMRVideoRecorder::SetVideoCodecL() H.264/AVC, high profile level 1.1 requested.")));
                        videoCodecLevel = KCMRH264AVCCodecLevel11;
                        newMimeType += _L8("64400B");
                        }
                    else if ( aMimeType.MatchF( _L8("*profile-level-id=64400C*") ) != KErrNotFound )
                        {
                        // high profile level 1.2
                        PRINT((_L("CCMRVideoRecorder::SetVideoCodecL() H.264/AVC, high profile level 1.2 requested.")));
                        videoCodecLevel = KCMRH264AVCCodecLevel12;
                        newMimeType += _L8("64400C");
                        }
                    else if ( aMimeType.MatchF( _L8("*profile-level-id=64400D*") ) != KErrNotFound )
                        {
                        // high profile level 1.3
                        PRINT((_L("CCMRVideoRecorder::SetVideoCodecL() H.264/AVC, high profile level 1.3 requested.")));
                        videoCodecLevel = KCMRH264AVCCodecLevel13;
                        newMimeType += _L8("64400D");
                        }
                    else if ( aMimeType.MatchF( _L8("*profile-level-id=644014*") ) != KErrNotFound )
                        {
                        // high profile level 2
                        PRINT((_L("CCMRVideoRecorder::SetVideoCodecL() H.264/AVC, high profile level 2 requested.")));
                        videoCodecLevel = KCMRH264AVCCodecLevel20;
                        newMimeType += _L8("644014");
                        }
                    else if ( aMimeType.MatchF( _L8("*profile-level-id=644015*") ) != KErrNotFound )
                        {
                        // high profile level 2.1
                        PRINT((_L("CCMRVideoRecorder::SetVideoCodecL() H.264/AVC, high profile level 2.1 requested.")));
                        videoCodecLevel = KCMRH264AVCCodecLevel21;
                        newMimeType += _L8("644015");
                        }
                    else if ( aMimeType.MatchF( _L8("*profile-level-id=644016*") ) != KErrNotFound )
                        {
                        // high profile level 2.2
                        PRINT((_L("CCMRVideoRecorder::SetVideoCodecL() H.264/AVC, high profile level 2.2 requested.")));
                        videoCodecLevel = KCMRH264AVCCodecLevel22;
                        newMimeType += _L8("644016");
                        }
                    else if ( aMimeType.MatchF( _L8("*profile-level-id=64401E*") ) != KErrNotFound )
                        {
                        // high profile level 3
                        PRINT((_L("CCMRVideoRecorder::SetVideoCodecL() H.264/AVC, high profile level 3 requested.")));
                        videoCodecLevel = KCMRH264AVCCodecLevel30;
                        newMimeType += _L8("64401E");
                        }
                    else if ( aMimeType.MatchF( _L8("*profile-level-id=64401F*") ) != KErrNotFound )
                        {
                        // high profile level 3.1
                        PRINT((_L("CCMRVideoRecorder::SetVideoCodecL() H.264/AVC, high profile level 3.1 requested.")));
                        videoCodecLevel = KCMRH264AVCCodecLevel31;
                        newMimeType += _L8("64401F");
                        }
                    else if ( aMimeType.MatchF( _L8("*profile-level-id=644020*") ) != KErrNotFound )
                        {
                        // high profile level 3.2
                        PRINT((_L("CCMRVideoRecorder::SetVideoCodecL() H.264/AVC, high profile level 3.2 requested.")));
                        videoCodecLevel = KCMRH264AVCCodecLevel32;
                        newMimeType += _L8("644020");
                        }
                    else if ( aMimeType.MatchF( _L8("*profile-level-id=644028*") ) != KErrNotFound )
                        {
                        // high profile level 4
                        PRINT((_L("CCMRVideoRecorder::SetVideoCodecL() H.264/AVC, high profile level 4 requested.")));
                        videoCodecLevel = KCMRH264AVCCodecLevel40;
                        newMimeType += _L8("644028");
                        }
                    else if ( aMimeType.MatchF( _L8("*profile-level-id=*") ) != KErrNotFound )
                        {
                        // no other profile-level ids supported
                        PRINT((_L("CCMRVideoRecorder::SetVideoCodecL() unsupported profile-level requested.")));
                        User::Leave(KErrNotSupported);
                        }
                    else
                        {
                        // Default is level 1
                        PRINT((_L("CCMRVideoRecorder::SetVideoCodecL() H.264/AVC, no profile requested - defaulting to 1.")));
                        videoCodecLevel = KCMRH264AVCCodecLevel10;
                        newMimeType += _L8("42800A");
                        }

                    // recreate the codec data object
                    PRINT((_L("CCMRVideoRecorder::SetVideoCodecL() H.264/AVC recreating codecdata.")));
                    delete iVideoCodecData;
                    iVideoCodecData = NULL;
                    iVideoCodecData = new (ELeave) CCMRVideoCodecDataH264AVC(videoCodecLevel);
                    }
                else
                    {
                    PRINT((_L("CCMRVideoRecorder::SetVideoCodecL() Unknown mimetype requested.")));
                    // unknown mimetype
                    }
                }
            }
        }

    // successfully interpreted the input mime type
    if ( newMimeType != iMimeType )
	    {
        iMimeType = newMimeType; //copy the contents
		PRINT((_L("CCMRVideoRecorder::SetVideoCodecL() new supported video codec requested - updating available encoders.")));
		iAvailableVideoEncoders.Reset();
        if ( iPreferredEncoderUID != KNullUid )
            {// We have preferred encoder UID from client - override encoder search and use it instead.
            PRINT((_L("CCMRVideoRecorder::SetVideoCodecL() skipping encoder search. Using API user encoder: %d "), iPreferredEncoderUID.iUid));
            iAvailableVideoEncoders.AppendL(iPreferredEncoderUID);
            }
        else if ( iConfig &&
                ( iConfig->IsICMConfigDataAvailable() ) &&
                ( iConfig->VideoQualitySettings().iVideoEncoderUID != KNullUid ) )
            {// Video quality set has set UID value - override encoder search and use it instead.
            PRINT((_L("CCMRVideoRecorder::SetVideoCodecL() skipping encoder search. Using ICM config encoder: %d "), iPreferredEncoderUID.iUid));
            iAvailableVideoEncoders.AppendL(iPreferredEncoderUID);
            }
        else
            {
            iDevVideoRec->FindEncodersL(iMimeType, 0 /* aPreProc */, iAvailableVideoEncoders, EFalse );
            PRINT((_L("CCMRVideoRecorder::SetVideoCodecL() search found %d encoder(s)"), iAvailableVideoEncoders.Count() ));
            }
        
        if ( iConfig )
            {
            iConfig->SetVideoCodec(iMimeType);
            iConfig->SetVideoPixelAspectRatio(numerator, denominator);
            }

        UpdateSupportedVideoFrameSizesRates();

        // user has to call this->PrepareL() now that a setting has been changed !!!
        // allow to start recording only when the state is "EReadyToRecord"
        PRINT((_L("CCMRVideoRecorder::SetVideoCodecL() codec set, prepare needed before recording.")));
        SetState(EStateOpen);
        DoSendEventToClient( KCMRPrepareNeeded, iErrorCode );   // use iErrorCode in case we had stored error; it may be KErrNone too
        iErrorCode = KErrNone;
	    }

    PRINT((_L("CCMRVideoRecorder::SetVideoCodecL() out")));
    }

// -----------------------------------------------------------------------------
// CCMRVideoRecorder::GetVideoCodec
// Get the used video codec.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CCMRVideoRecorder::GetVideoCodecL( TDes8& aVideoMimeType ) const
    {
    aVideoMimeType = iMimeType;
    }


// ---------------------------------------------------------
// CCMRVideoRecorder::SetTargetBitRateL
// Sets new target bitrate
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CCMRVideoRecorder::SetTargetBitRateL(TInt aBitRate)
    {
    PRINT((_L("CCMRVideoRecorder::SetTargetBitRate, aBitrate = % d"),aBitRate));

    if ( aBitRate == KMMFVariableVideoBitRate )
        {
        // Variable rate requested; normalize the bit-rate (temporarily only in this function)
        // but set a mode
        aBitRate = iVideoCodecData->MaxBitRate();
        iBitRateMode = EBitRateVariable;
        }
    else
        {
        iBitRateMode = EBitRateConstant;
        }

    // check that values are reasonable; check max bitrate only when the level is expected to be used for MMS
    if ( (aBitRate < KCMRMinAcceptedBitRate) || ((iVideoCodecData->LevelForMMS()) && (aBitRate > iVideoCodecData->MaxBitRate())) )
        {
        PRINT((_L("CCMRVideoRecorder::SetTargetBitRateL() illegal bitrate")));
        User::Leave(KErrArgument);
        }

    // call base class implementation
    CCMRRecorderBase::SetTargetBitRateL(aBitRate);

    // inform the max bitrate to the sink
    User::LeaveIfError( iOutput->SetMaxVideoBitRate( aBitRate ) );

    //inform the estimated average bitrate to sink; use a HW-specific scaler to estimate the average
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

    TInt br = static_cast<TInt>(aBitRate*videoBitrateScaler);
    User::LeaveIfError( iOutput->SetAverageVideoBitRate( br ) );

    if ( StateRequiresDynamicSetting() )
        {
        // we have prepared => new value must be given via dynamic methods
        FillRateControlOptions( iRateControlOptions );
        iDevVideoRec->SetRateControlOptions(0, iRateControlOptions); // only base layer (layer 0) supported
        }
    else
        {
        // we are not recording or prepared => value will be given forward in prepare
        }
    }


// ---------------------------------------------------------
// CCMRVideoRecorder::TargetBitRateL
// Gets current target bitrate
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CCMRVideoRecorder::TargetBitRateL(TInt& aBitRate)
    {
    PRINT((_L("CCMRVideoRecorder::TargetBitRate")));


    if ( iBitRateMode == EBitRateConstant )
        {
        aBitRate = iTargetBitRate;
        }
    else
        {
        aBitRate = KMMFVariableVideoBitRate;
        }
    }


// ---------------------------------------------------------
// CCMRVideoRecorder::SetFrameSizeL
// Sets new input & output frame size
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CCMRVideoRecorder::SetFrameSizeL(const TSize& aSize)
    {

    PRINT((_L("CCMRVideoRecorder::SetFrameSizeL(), In, size: %dx%d"), aSize.iWidth, aSize.iHeight ));

    // allow settings only when in "open" or "readytorecord" states
    if ( ( State() != EStateOpen ) && ( State() != EStateReadyToRecord ) )
        {
        PRINT((_L("CCMRVideoRecorder::SetFrameSizeL() wrong state")));
        User::Leave(KErrNotReady);
        }

    // check that size is acceptable
    if ( !iVideoCodecData->MaxFrameSize( aSize ) )
        {
        PRINT((_L("CCMRVideoRecorder::SetFrameSizeL() illegal frame size")));
        User::Leave(KErrArgument);//could be also KErrNotSupported
        }

    TSize size;
    TInt i = 0;
    TInt j = 0;
   	TInt k = 0;

    PRINT((_L("CCMRVideoRecorder::SetFrameSizeL() looking match for frame size: %d x %d from camera"), aSize.iWidth, aSize.iHeight ));

    // Some explanation of the logic here. Camera API must support the same rates for all formats, since there
    // is just a single variable in the info class to check the number of rates.
    // But the indices may differ and that's why we need enumerations
    if ( iCameraInfo.iVideoFrameFormatsSupported & CCamera::EFormatYUV420Planar )
        {
        // check YUV420 planar
        for ( i = 0; i < iCameraInfo.iNumVideoFrameSizesSupported; i++ )
            {
            iSource->EnumerateVideoFrameSizes(size, i, CCamera::EFormatYUV420Planar);
            PRINT((_L("CCMRVideoRecorder::SetFrameSizeL() Camera EFormatYUV420Planar %d x %d found from camera"), size.iWidth, size.iHeight ));

            if ( size == aSize )
                {
                PRINT((_L("CCMRVideoRecorder::SetFrameSizeL() Camera EFormatYUV420Planar %d x %d Matched."), size.iWidth, size.iHeight ));
                iSizeIndex420 = i;
                break;
                }
            }
        }
    else
        {
        i = iCameraInfo.iNumVideoFrameSizesSupported;
        PRINT((_L("CCMRVideoRecorder::SetFrameSizeL() Camera doesn't support EFormatYUV420Planar")));
        }

    if ( iCameraInfo.iVideoFrameFormatsSupported & CCamera::EFormatYUV422 )
        {
        // check YUV422 interleaved
        for ( j = 0; j < iCameraInfo.iNumVideoFrameSizesSupported; j++ )
            {
            iSource->EnumerateVideoFrameSizes(size, j, CCamera::EFormatYUV422 );
            PRINT((_L("CCMRVideoRecorder::SetFrameSizeL() Camera EFormatYUV422 %d x %d found from camera"), size.iWidth, size.iHeight ));

            if ( size == aSize )
                {
                PRINT((_L("CCMRVideoRecorder::SetFrameSizeL() Camera EFormatYUV422 %d x %d Matched."), size.iWidth, size.iHeight ));
                iSizeIndex422 = j;
                break;
                }
            }
        }
    else
        {
        j = iCameraInfo.iNumVideoFrameSizesSupported;
        PRINT((_L("CCMRVideoRecorder::SetFrameSizeL() Camera doesn't support EFormatYUV422")));
        }

	// Search if we have direct capture encoder available -> no need to care about camera supported sizes.
	if ( iAvailableVideoFrameSizesRates.Count() )
		{
        // check direct capture encoder size array
        for ( k = 0; k < iAvailableVideoFrameSizesRates.Count(); k++ )
            {
            // All target resolutions that are below higher resolution found from encoders are accepted.
            if ( (iAvailableVideoFrameSizesRates[k].iPictureSize.iWidth >= aSize.iWidth) &&
            	 (iAvailableVideoFrameSizesRates[k].iPictureSize.iHeight >= aSize.iHeight) )
                {
                PRINT((_L("CCMRVideoRecorder::SetFrameSizeL() DirectCapture encoder %d x %d Matched."), aSize.iWidth, aSize.iHeight ));
				iSizeIndexDCEncoder = k;
                break;
                }
            }
		}
	else
		{
		PRINT((_L("CCMRVideoRecorder::SetFrameSizeL() no directcapture encoder available for given size.")));
		}

    if ( (i == iCameraInfo.iNumVideoFrameSizesSupported) &&
    	 (j == iCameraInfo.iNumVideoFrameSizesSupported) &&
    	 (k == iAvailableVideoFrameSizesRates.Count()) )
        {
        // Requested size not supported by camera API or the directcapture encoder
        PRINT((_L("CCMRVideoRecorder::SetFrameSizeL(), Camera and/or direct capture encoder does not support this resolution")));
        User::Leave(KErrNotSupported);
        }

    iFrameSize = aSize;
    iOutput->SetVideoFrameSize(aSize);

    // user has to call this->PrepareL() now that a setting has been changed !!!
    // allow to start recording only when the state is "EReadyToRecord"
    SetState(EStateOpen);
    DoSendEventToClient( KCMRPrepareNeeded, iErrorCode );   // use iErrorCode in case we had stored error; it may be KErrNone too
    iErrorCode = KErrNone;

    // update MaxFrameRate to new frame size
    iMaxFrameRate4GivenSize = iVideoCodecData->MaxFrameRate( iFrameSize );

    // framerate is tied with framesize; if it was set before update it accordingly
    // this may be overridden by user e.g. with a higher rate; this sets the rates <= previous encoding rate
    if ( (iRateIndex420 >= 0) || (iRateIndex422 >= 0) || (iRateIndexDCEncoder >= 0) )
        {
        // must use iEncodingFrameRate, since other rates may raise encoding rate higher than wanted; this allows camera frame rate to be higher
        TInt success = KErrNone;
        do
            {
            TRAP( success, SetFrameRateL( iEncodingFrameRate ));
            if ( success == KErrNone )
                {
                break;
                }
            // if picture size was increased, we may need to decrease framerate
            iEncodingFrameRate--;
            } while ( iEncodingFrameRate > 0 );
            if ( success != KErrNone )
                {
                // This framesize - framerate combination not possible
                PRINT((_L("CCMRVideoRecorder::SetFrameSizeL() illegal frame size - rate combination")));
                User::Leave(success);
                }
        }

    PRINT((_L("CCMRVideoRecorder::SetFrameSizeL(), Out")));
    }


// ---------------------------------------------------------
// CCMRVideoRecorder::FrameSizeL
// Gets current frame size
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CCMRVideoRecorder::FrameSizeL(TSize& aSize) const
    {
    PRINT((_L("CCMRVideoRecorder::FrameSizeL()")));
    aSize = iFrameSize;
    }


// ---------------------------------------------------------
// CCMRVideoRecorder::SetFrameRateL
// Sets new target frame rate
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CCMRVideoRecorder::SetFrameRateL(TReal32 aFrameRate)
    {
    PRINT((_L("CCMRVideoRecorder::SetFrameRate(), requested rate %f"), aFrameRate));

    // Size has to be set before setting frame rate !!!
    if ( (State() == EStateNone) || ((iSizeIndex420 < 0) && (iSizeIndex422 < 0) && (iSizeIndexDCEncoder < 0)) )
        {
        PRINT((_L("CCMRVideoRecorder::SetFrameRateL() wrong state")));
        User::Leave(KErrNotReady);
        }

    // check that values are reasonable
    if ( (aFrameRate <= 0) || (aFrameRate > iVideoCodecData->MaxFrameRate(iFrameSize)) )
        {
        PRINT((_L("CCMRVideoRecorder::SetFrameSizeL() illegal frame rate")));
        User::Leave(KErrArgument);
        }

    // encoding frame rate is always the requested one
    if ( aFrameRate <= iMaxFrameRate4GivenSize ) // actually iMaxFrameRate4GivenSize is valid only after prepare
        {
        // requested rate looks ok
        iEncodingFrameRate = aFrameRate;
        }
    else if ( iEncodingFrameRate < iMaxFrameRate4GivenSize )
        {
        // too high rate requested, but the current rate is still lower than max possible, so we can set the rate higher anyway
        iEncodingFrameRate = static_cast<TReal32>(iMaxFrameRate4GivenSize);
        PRINT((_L("CCMRVideoRecorder::SetFrameRate(), requested rate is too high for the used encoder, but set it to this rate: %f"), iMaxFrameRate4GivenSize));
        }
    else
        {
        // The change has no effect; the used encoder is already set to max possible rate and can't be configured for the requested rate
        // for now camera api capability is not checked; e.g. if H.263 level is 20 => framerate could be 30fps for QCIF but if max encoder
        // impl. rate is 10fps, we don't leave, but just ignore
        PRINT((_L("CCMRVideoRecorder::SetFrameRate(), requested rate is too high for the used encoder, max rate %f"), iMaxFrameRate4GivenSize));
        return;
        }

    if ( StateRequiresDynamicSetting() )
        {
        // we are running, change only encoding rate now

        // store the requested rate also, to be taken into use in the next prepare
        if ( !iNightMode )
            {
            iRequestedFrameRate = aFrameRate;
            }

        if ( iEncodingFrameRate > iSourceFrameRate )
            {
            // can't be higher than capture rate, limit it
            iEncodingFrameRate = iSourceFrameRate;
            }

        // set the rate control params
        FillRateControlOptions( iRateControlOptions );
        iDevVideoRec->SetRateControlOptions(0, iRateControlOptions); // only base layer (layer 0) supported
        }
    else
        {
        // we are in initialization phase, change also camera rate

        TReal32 rate;
        TInt i;

        iRateIndex420 = -1; //reset just in case, needed to determine if the loop below was successful
        iRateIndex422 = -1; //reset just in case, needed to determine if the loop below was successful
        iRateIndexDCEncoder = -1;

        // Some explanation of the logic here. Camera API must support the same rates for all formats, since there
        // is just a single variable in the info class to check the number of rates.
        // But the indices may differ and that's why we need enumerations
        // Since we prefer YUV422, it can override the member variables.

        if ( iCameraInfo.iVideoFrameFormatsSupported & CCamera::EFormatYUV420Planar )
            {
            // check YUV420 planar

            // search for an exact match from camera's supported rates
            for ( i = 0; i < iCameraInfo.iNumVideoFrameRatesSupported; i++ )
                {

                iSource->EnumerateVideoFrameRates(rate, i, CCamera::EFormatYUV420Planar, iSizeIndex420,
                                                  CCamera::EExposureAuto);

                if ( rate == aFrameRate )
                    {
                    iRateIndex420 = i;
                    iSourceFrameRate = aFrameRate;
                    iSourceFrameInterval = convertFrameRateToInterval(iSourceFrameRate);
                    PRINT((_L("CCMRVideoRecorder::SetFrameRate, asked framerate found")));
                    break;
                    }
                else if ( rate > aFrameRate )
                    {
                    // this is higher but could be used (encoder can match the rates)
                    // Check the other rates too if a better is found
                    iRateIndex420 = i;
                    iSourceFrameRate = rate;
                    iSourceFrameInterval = convertFrameRateToInterval(iSourceFrameRate);
                    PRINT((_L("CCMRVideoRecorder::SetFrameRate, a higher source framerate found")));
                    }
                }
            }

        if ( iCameraInfo.iVideoFrameFormatsSupported & CCamera::EFormatYUV422 )
            {
            // check YUV422 interleaved

            // search for an exact match from camera's supported rates
            for ( i = 0; i < iCameraInfo.iNumVideoFrameRatesSupported; i++ )
                {
                iSource->EnumerateVideoFrameRates(rate, i, CCamera::EFormatYUV422, iSizeIndex422,
                                                  CCamera::EExposureAuto);

                if ( rate == aFrameRate )
                    {
                    iRateIndex422 = i;
                    iSourceFrameRate = aFrameRate;
                    iSourceFrameInterval = convertFrameRateToInterval(iSourceFrameRate);
                    PRINT((_L("CCMRVideoRecorder::SetFrameRate, asked framerate found")));
                    break;
                    }
                else if ( rate > aFrameRate )
                    {
                    // this is higher but could be used (encoder can match the rates)
                    // Check the other rates too if a better is found
                    iRateIndex422 = i;
                    iSourceFrameRate = rate;
                    iSourceFrameInterval = convertFrameRateToInterval(iSourceFrameRate);
                    PRINT((_L("CCMRVideoRecorder::SetFrameRate, a higher source framerate found")));
                    }
                }
            }

        if ( iAvailableVideoFrameSizesRates.Count() && (iSizeIndexDCEncoder >= 0) && (iSizeIndexDCEncoder < iAvailableVideoFrameSizesRates.Count()) )
            {
            // check Direct capture encoder supported rates.

            // search for an exact match from encoders supported rates
            for ( i = 0; i < iAvailableVideoFrameSizesRates.Count(); i++ )
                {
                if ( (iAvailableVideoFrameSizesRates[i].iPictureRate == aFrameRate) &&
                	 (iAvailableVideoFrameSizesRates[i].iPictureSize == iAvailableVideoFrameSizesRates[iSizeIndexDCEncoder].iPictureSize) )
                    {
                    iRateIndexDCEncoder = i;
                    iSizeIndexDCEncoder = i;
                    iSourceFrameRate = aFrameRate;
                    iSourceFrameInterval = convertFrameRateToInterval(iSourceFrameRate);
                    PRINT((_L("CCMRVideoRecorder::SetFrameRate, asked framerate found")));
                    break;
                    }
                else if ( (iAvailableVideoFrameSizesRates[i].iPictureRate >= aFrameRate) &&
                	 	  (iAvailableVideoFrameSizesRates[i].iPictureSize.iWidth >= iAvailableVideoFrameSizesRates[iSizeIndexDCEncoder].iPictureSize.iWidth ) &&
                	 	  (iAvailableVideoFrameSizesRates[i].iPictureSize.iHeight >= iAvailableVideoFrameSizesRates[iSizeIndexDCEncoder].iPictureSize.iHeight ) )
                    {
                    // this is higher but could be used (encoder can match the rates)
                    // Check the other rates too if a better is found
                    iRateIndexDCEncoder = i;
                    iSizeIndexDCEncoder = i;
                    iSourceFrameRate = aFrameRate;
                    iSourceFrameInterval = convertFrameRateToInterval(iSourceFrameRate);
                    PRINT((_L("CCMRVideoRecorder::SetFrameRate, a higher source framerate found: %dx%d at %f"), iAvailableVideoFrameSizesRates[i].iPictureSize.iWidth,
                    																							iAvailableVideoFrameSizesRates[i].iPictureSize.iHeight,
                    																							iAvailableVideoFrameSizesRates[i].iPictureRate ));
                    }
                }
            }


        if ( (iRateIndex420 < 0) && (iRateIndex422 < 0) && (iRateIndexDCEncoder < 0) )
            {
            // there is no suitable frame rate available, not even a higher one, return error
            PRINT((_L("CCMRVideoRecorder::SetFrameRate, no suitable framerate found from Camera API and/or direct capture encoder.")));
            User::Leave(KErrNotSupported);
            }

        // set to the actual rate to prevent trying set the rate again from PrepareL
        iRequestedFrameRate = iSourceFrameRate;

        // user has to call this->PrepareL() now that a setting has been changed !!!
        // allow to start recording only when the state is "EReadyToRecord"
        if ( State() != EStateOpen )
            {
            SetState(EStateOpen);
            DoSendEventToClient( KCMRPrepareNeeded, iErrorCode );   // use iErrorCode in case we had stored error; it may be KErrNone too
            iErrorCode = KErrNone;
            }
        }
    }


// ---------------------------------------------------------
// CCMRVideoRecorder::FrameRateL
// Get the used encoding frame rate
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CCMRVideoRecorder::FrameRateL(TReal32& aFrameRate) const
    {
    PRINT((_L("CCMRVideoRecorder::FrameRateL()")));
    aFrameRate = iEncodingFrameRate;
    }


// ---------------------------------------------------------
// CCMRVideoRecorder::SetVideoCodingOptionsL
// Set misc video coding options
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CCMRVideoRecorder::SetVideoCodingOptionsL(const TCCMRVideoCodingOptions& aOptions)
    {
    if ( ( State() != EStateOpen ) && ( State() != EStateReadyToRecord ) )
        {
        PRINT((_L("CCMRVideoRecorder::SetVideoCodingOptionsL() wrong state")));
        User::Leave( KErrNotReady );
        }
    if ( iMinRandomAccessPeriodInSeconds != KCMRUseDefault )
        {
        iMinRandomAccessPeriodInSeconds = aOptions.iMinRandomAccessPeriodInSeconds;
        }

    iVideoCodecData->SetVideoCodingOptionsL(aOptions);
    }


// ---------------------------------------------------------
// CCMRVideoRecorder::SetVideoRateControlOptionsL
// Set video rate control options
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CCMRVideoRecorder::SetVideoRateControlOptionsL(const TRateControlOptions& aOptions)
    {
    PRINT((_L("CCMRVideoRecorder::SetVideoRateControlOptionsL() in")));
    PRINT((_L("CCMRVideoRecorder::SetVideoRateControlOptionsL() TBitrateControlType iControl: %d"), aOptions.iControl));
    PRINT((_L("CCMRVideoRecorder::SetVideoRateControlOptionsL() iBitrate: %d"), aOptions.iBitrate));
    PRINT((_L("CCMRVideoRecorder::SetVideoRateControlOptionsL() iPictureQuality: %d"), aOptions.iPictureQuality));
    PRINT((_L("CCMRVideoRecorder::SetVideoRateControlOptionsL() iPictureRate: %f"), aOptions.iPictureRate));
    PRINT((_L("CCMRVideoRecorder::SetVideoRateControlOptionsL() iQualityTemporalTradeoff: %f"), aOptions.iQualityTemporalTradeoff));
    PRINT((_L("CCMRVideoRecorder::SetVideoRateControlOptionsL() iLatencyQualityTradeoff: %f"), aOptions.iLatencyQualityTradeoff));

    if ( !StateRequiresDynamicSetting() )
    	{
        PRINT((_L("CCMRVideoRecorder::SetVideoRateControlOptionsL() illegal state for dynamic settings")));
    	User::Leave(KErrNotReady);
    	}

	// Handle new bitrate value:
    if ( (aOptions.iBitrate < KCMRMinAcceptedBitRate) || ((iVideoCodecData->LevelForMMS()) && (aOptions.iBitrate > iVideoCodecData->MaxBitRate())) )
        {// check that values are reasonable; check max bitrate only when the level is expected to be used for MMS
        PRINT((_L("CCMRVideoRecorder::SetVideoRateControlOptionsL() illegal bitrate")));
        User::Leave(KErrArgument);
        }
    CCMRRecorderBase::SetTargetBitRateL(aOptions.iBitrate);
    // inform the max bitrate to the sink
    User::LeaveIfError( iOutput->SetMaxVideoBitRate( aOptions.iBitrate ) );
    //inform the estimated average bitrate to sink; use a HW-specific scaler to estimate the average
    TReal videoBitrateScaler = 0;
    if ( iConfig )
    	{
        videoBitrateScaler = iConfig->PluginSettings().iCMRAvgVideoBitRateScaler;
    	}
    if ( videoBitrateScaler == 0 )
    	{// make sure we never get scaler value 0 (ends up Div0 error)
    	videoBitrateScaler = KCMRAvgVideoBitRateScaler;
    	}
    TInt br = static_cast<TInt>(aOptions.iBitrate*videoBitrateScaler);
    User::LeaveIfError( iOutput->SetAverageVideoBitRate( br ) );


    //Handle new framerate value:
    if ( (aOptions.iPictureRate <= 0) || (aOptions.iPictureRate > iVideoCodecData->MaxFrameRate(iFrameSize)) )
        {// check that values are reasonable
        PRINT((_L("CCMRVideoRecorder::SetVideoRateControlOptionsL() illegal frame rate, %f"), aOptions.iPictureRate));
        User::Leave(KErrArgument);
        }
    if ( aOptions.iPictureRate <= iMaxFrameRate4GivenSize ) // actually iMaxFrameRate4GivenSize is valid only after prepare
        {// encoding frame rate is always the requested one
        iEncodingFrameRate = aOptions.iPictureRate;        // requested rate looks ok
        }
    else if ( iEncodingFrameRate < iMaxFrameRate4GivenSize )
        {// too high rate requested, but the current rate is still lower than max possible, so we can set the rate higher anyway
        iEncodingFrameRate = static_cast<TReal32>(iMaxFrameRate4GivenSize);
        PRINT((_L("CCMRVideoRecorder::SetVideoRateControlOptionsL(), requested rate is too high for the used encoder, but set it to this rate: %f"), iMaxFrameRate4GivenSize));
        }
    if ( !iNightMode )
        {// store the requested rate also, to be taken into use in the next prepare
        iRequestedFrameRate = aOptions.iPictureRate;
        }
    if ( iEncodingFrameRate > iSourceFrameRate )
        {// can't be higher than capture rate, limit it
        iEncodingFrameRate = iSourceFrameRate;
        }

    iRateControlOptions = aOptions;
    iRateControlOptions.iPictureRate = iEncodingFrameRate;
    if ( iDevVideoRec )
    	{
        PRINT((_L("CCMRVideoRecorder::SetVideoRateControlOptionsL() settings stored and sending them to Devvideo.")));
        iDevVideoRec->SetRateControlOptions(0, iRateControlOptions); // only base layer (layer 0) supported
    	}
    else
    	{
        PRINT((_L("CCMRVideoRecorder::SetVideoRateControlOptionsL() no DevVideoRec! Leaving KErrNotReady -18")));
    	User::Leave(KErrNotReady);
    	}
    PRINT((_L("CCMRVideoRecorder::SetVideoRateControlOptionsL() out")));
    }

// ---------------------------------------------------------
// CCMRVideoRecorder::GetVideoRateControlOptionsL
// Get video rate control options
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CCMRVideoRecorder::GetVideoRateControlOptionsL(TRateControlOptions& aOptions)
    {
    PRINT((_L("CCMRVideoRecorder::GetVideoRateControlOptionsL() in")));
    PRINT((_L("CCMRVideoRecorder::GetVideoRateControlOptionsL() TBitrateControlType iControl: %d"), iRateControlOptions.iControl));
    PRINT((_L("CCMRVideoRecorder::GetVideoRateControlOptionsL() iBitrate: %d"), iRateControlOptions.iBitrate));
    PRINT((_L("CCMRVideoRecorder::GetVideoRateControlOptionsL() iPictureQuality: %d"), iRateControlOptions.iPictureQuality));
    PRINT((_L("CCMRVideoRecorder::GetVideoRateControlOptionsL() iPictureRate: %f"), iRateControlOptions.iPictureRate));
    PRINT((_L("CCMRVideoRecorder::GetVideoRateControlOptionsL() iQualityTemporalTradeoff: %f"), iRateControlOptions.iQualityTemporalTradeoff));
    PRINT((_L("CCMRVideoRecorder::GetVideoRateControlOptionsL() iLatencyQualityTradeoff: %f"), iRateControlOptions.iLatencyQualityTradeoff));

    if ( !StateRequiresDynamicSetting() )
        {
        PRINT((_L("CCMRVideoRecorder::GetVideoRateControlOptionsL() illegal state for dynamic settings")));
        User::Leave(KErrNotReady);
        }
    aOptions = iRateControlOptions;
    PRINT((_L("CCMRVideoRecorder::GetVideoRateControlOptionsL() out")));
    }


// ---------------------------------------------------------
// CCMRVideoRecorder::SetPreferredVideoEncoderL
// Set video encoder using its UID. Usage optional.
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CCMRVideoRecorder::SetPreferredVideoEncoderL(TUid& aEncoder)
    {
    PRINT((_L("CCMRVideoRecorder::SetPreferredVideoEncoderL() in")));
    iPreferredEncoderUID = aEncoder;

    if ( iPreferredEncoderUID != KNullUid )
        {// We have preferred encoder UID from client - override encoder search and use it instead.
        PRINT((_L("CCMRVideoRecorder::SetPreferredVideoEncoderL() skipping encoder search. Using API user encoder: %d "), iPreferredEncoderUID.iUid));
        iAvailableVideoEncoders.Reset();
        iAvailableVideoEncoders.AppendL(iPreferredEncoderUID);
        UpdateSupportedVideoFrameSizesRates();
        }

    PRINT((_L("CCMRVideoRecorder::SetPreferredVideoEncoderL() out")));
    }

// ---------------------------------------------------------
// CCMRVideoRecorder::SetPreferredVideoEncapsulationL
// Set video encoder output format encapsulation. Usage optional.
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CCMRVideoRecorder::SetPreferredVideoEncapsulationL(TVideoDataUnitEncapsulation aCapsulation)
    {
    PRINT((_L("CCMRVideoRecorder::SetPreferredVideoEncapsulationL() in")));
    iPreferredEncapsulation = aCapsulation;
    iPreferredEncapsulationSet = ETrue;
    PRINT((_L("CCMRVideoRecorder::SetPreferredVideoEncapsulationL() out")));
    }

// ---------------------------------------------------------
// CCMRVideoRecorder::SetSegmentTargetSize
// Set video segment target size
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CCMRVideoRecorder::SetSegmentTargetSizeL(TUint aLayer, TUint aSizeBytes, TUint aSizeMacroblocks)
    {
    PRINT((_L("CCMRVideoRecorder::SetSegmentTargetSize() in")));
    PRINT((_L("CCMRVideoRecorder::SetSegmentTargetSize() aLayer: %d"), aLayer));
    PRINT((_L("CCMRVideoRecorder::SetSegmentTargetSize() aSizeBytes: %d"), aSizeBytes));
    PRINT((_L("CCMRVideoRecorder::SetSegmentTargetSize() aSizeMacroblocks: %d"), aSizeMacroblocks));

    if ( !StateRequiresDynamicSetting() )
        {
        PRINT((_L("CCMRVideoRecorder::SetSegmentTargetSize() illegal state for dynamic settings")));
        User::Leave(KErrNotReady);
        }

    if ( iDevVideoRec )
        {
        PRINT((_L("CCMRVideoRecorder::SetSegmentTargetSize() sending SetSegmentTargetSize to Devvideo.")));
        iDevVideoRec->SetSegmentTargetSize(aLayer, aSizeBytes, aSizeMacroblocks); // only base layer (layer 0) supported
        }
    else
        {
        PRINT((_L("CCMRVideoRecorder::SetSegmentTargetSize() no DevVideoRec! Leaving KErrNotReady -18")));
        User::Leave(KErrNotReady);
        }
    PRINT((_L("CCMRVideoRecorder::SetSegmentTargetSize() out")));
    }

// ---------------------------------------------------------
// CCMRVideoRecorder::AdjustTimeStampsL
// Adjust time stamps of video
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CCMRVideoRecorder::AdjustTimeStampsL(const TInt aAdjustmentMs)
    {
    if ( State() != EStateRecording )
        {
        PRINT((_L("CCMRVideoRecorder::AdjustTimeStampsL() wrong state")));
        User::Leave( KErrNotReady );
        }

    iAdjustmentTimeUs = TInt64( aAdjustmentMs * 1000 );

    PRINT((_L("CCMRVideoRecorder::AdjustTimeStampsL() by %d us"),I64INT(iAdjustmentTimeUs)));
    }


// ---------------------------------------------------------
// CCMRVideoRecorder::MvsoReserveComplete
// Called by camera API when camera API has been reserved
// for exclusive use
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CCMRVideoRecorder::MvsoReserveComplete(TInt aError)
    {
    PRINT((_L("CCMRVideoRecorder::MvsoReserveComplete() in")));

    // Should not be called, if we use direct capture mode
    if (iDirectCapture)
        {
        VRASSERT(0);
        }

    // need to set this already here if returns unsuccesfully, to enable error handling when encoder init completes
    iSourceInitComplete = ETrue;

    if ( iEncoderInitComplete && (iErrorCode != KErrNone) )
        {
        // there was an error in the encoder initialisation, but the reporting was
        // waiting for the completion of the source initialisation => report it now
        PRINT((_L("CCMRVideoRecorder::MvsoReserveComplete() but error in video encoder: %d"), iErrorCode));
        DoSendEventToClient( KCMREncoderInitError, iErrorCode );
        iErrorCode = KErrNone;
        return;
        }

    if ( aError != KErrNone )
        {
        if ( iEncoderInitComplete )
            {
            PRINT((_L("CCMRVideoRecorder::MvsoReserveComplete() with error: %d"), aError));
            DoSendEventToClient( KCMRCameraReserveError, aError );
            }
        else
            {
            // report only when encoder is completed too
            PRINT((_L("CCMRVideoRecorder::MvsoReserveComplete() with error: %d, but must wait for encoder init completion"), aError));
            iErrorCode = aError;
            }
        return;
        }

    TInt error;

    TRAP(error, iSource->PrepareCaptureL(iVideoFormat, iSizeIndex, iRateIndex,
                                         iNumCameraBuffers, KCMRNumFramesInCameraBuffer));

    PRINT((_L("CCMRVideoRecorder::MvsoReserveComplete() camera prepared, error %d"),error));
    if ( error != KErrNone )
        {
        if ( iEncoderInitComplete )
            {
            DoSendEventToClient( KCMRCameraPrepareError, error );
            }
        else
            {
            // report only when encoder is completed too
            iErrorCode = error;
            }
        return;
        }


    if ( iEncoderInitComplete )
        {
        SetState(EStateReadyToRecord);

        // everything ok, inform observer that we are prepared.
        DoSendEventToClient( KCMRPrepareComplete, KErrNone );
        }

    PRINT((_L("CCMRVideoRecorder::MvsoReserveComplete() out")));
    }


// ---------------------------------------------------------
// CCMRVideoRecorder::MvsoFrameBufferReady
// Called by camera API when a frame (or several frames)
// has been captured
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CCMRVideoRecorder::MvsoFrameBufferReady(MFrameBuffer* aFrameBuffer, TInt aError)
    {
    // Should not be called, if we use direct capture mode
    if (iDirectCapture)
        {
        VRASSERT(0);
        }

    if ( aError != KErrNone || aFrameBuffer == NULL )
        {
        // some errors, inform observer and return
        DoSendEventToClient( KCMRCameraCaptureError, aError );
        if ( aFrameBuffer )
            {
            // in case of errors, aFrameBuffer may be NULL
            aFrameBuffer->Release();
            }
        PRINT((_L("CCMRVideoRecorder::MvsoFrameBufferReady with error %d"), aError));
        return;
        }

    PRINT((_L("CCMRVideoRecorder::MvsoFrameBufferReady in %x, frame & time & systemdrift\t%d\t%d\t%d"),aFrameBuffer,
                                                                                       aFrameBuffer->iIndexOfFirstFrameInBuffer,
                                                                                       I64INT(aFrameBuffer->iElapsedTime.Int64()),
                                                                                       I64INT(iSystemClockDelta.Int64())
                                                                                       ));

#ifdef VIDEO_FILE_OUTPUT

    // Write the frame to a file for testing purposes
    TInt error = KErrNone;
    TRAPD(error2, error = iOutputFile.Write( *(aFrameBuffer->DataL(0)) ));

    // release frame
    aFrameBuffer->Release();

    if (error != KErrNone || error2 != KErrNone)
        {
        DoSendEventToClient( KCMRRunTimeError, error );
        }


#else
    // the real deal

    if ( (State() != EStateRecording) || (iErrorCode != KErrNone) )
        {
        // we are paused or stopping or there was an error => return the framebuffer immediately
        // if system clock has been changed during pause take that account in future frame timestamp calculations.
        if ( (((aFrameBuffer->iElapsedTime.Int64() + iSystemClockDelta.Int64()) < iLatestAbsoluteTimeStamp.Int64()) ||
             ((aFrameBuffer->iElapsedTime.Int64() + iSystemClockDelta.Int64()) > (iLatestAbsoluteTimeStamp.Int64() + 2*iSourceFrameInterval))) &&
             (aFrameBuffer->iIndexOfFirstFrameInBuffer == iPreviousCameraFrameIndex + 1) )
             {
             // something has changed system clock during pause, add cumulatively to drift delta.
             iSystemClockDelta = (iLatestAbsoluteTimeStamp.Int64() + iSourceFrameInterval) -
                                 (aFrameBuffer->iElapsedTime.Int64() + iSystemClockDelta.Int64())
                                 + iSystemClockDelta.Int64();

             PRINT((_L("CCMRVideoRecorder::MvsoFrameBufferReady() System time change: aFrameBuffer->iElapsedTime = %d, iSystemClockDelta = %d, iLatestAbsoluteTimeStamp = %d, iSourceFrameInterval = %d"),
                                                                                              I64INT(aFrameBuffer->iElapsedTime.Int64()),
                                                                                              I64INT(iSystemClockDelta.Int64()),
                                                                                              I64INT(iLatestAbsoluteTimeStamp.Int64()),
                                                                                              iSourceFrameInterval));
             }

        iLatestAbsoluteTimeStamp = aFrameBuffer->iElapsedTime.Int64() + iSystemClockDelta.Int64();
        iPreviousCameraFrameIndex = aFrameBuffer->iIndexOfFirstFrameInBuffer;
        aFrameBuffer->Release();
        PRINT((_L("CCMRVideoRecorder::MvsoFrameBufferReady() not recording, skip this one")));
        return;
        }


#if ( defined (__WINS__) || defined (__WINSCW__) )
    if ( iEncoderInputQueueLength > 0 )
        {
        // Too many pictures already in the encoder's input queue - skip this one. In Wins the criteria is more strict since performance is lower
        PRINT((_L("CCMRVideoRecorder::MvsoFrameBufferReady() - encoder already has one frame under processing, skip this one")));
        iPreviousCameraFrameIndex = aFrameBuffer->iIndexOfFirstFrameInBuffer;
        aFrameBuffer->Release();
        return;
        }
#else
    if ( iEncoderInputQueueLength >= KCMRMaxPreEncoderBufferPictures )
        {
        // Too many pictures already in the encoder's input queue - skip this one
        PRINT((_L("CCMRVideoRecorder::MvsoFrameBufferReady() - Encoder input/output queue(s) already full, must skip a captured frame")));
        iPreviousCameraFrameIndex = aFrameBuffer->iIndexOfFirstFrameInBuffer;
        aFrameBuffer->Release();
        return;
        }
#endif

    // Regulate timestamp to avoid too small or too large variations

    // use frame index instead of associated buffer timestamp, since timestamps may be
    // less accurate and may cause rounding problems when converted to e.g. H.263 TRs
    TInt64 frameTimeStamp;
    frameTimeStamp = TInt64((aFrameBuffer->iIndexOfFirstFrameInBuffer - iDriftFrameSkipCount + iAddedFrameDurationCount) * 1E6/iSourceFrameRate + 0.5);   // use accurate division here since rounded value in multiplication multiplies also the rounding error

    // However, relying purely on frameindex causes drifting if the used source framerate value here is not exactly
    // the camera input rate. Hence we need to do some adjustments.
    // The frameindex may also be exclude skipped frames, but that should considered a bug in camera drivers, although the Camera API spec is not fully unambiguous about this
    TInt64 expectedNewTimeStamp;
    if ( iNumberOfCapturedFrames > 0 )
        {
        expectedNewTimeStamp = iLatestAbsoluteTimeStamp.Int64() + iSourceFrameInterval;
        PRINT((_L("CCMRVideoRecorder::MvsoFrameBufferReady(), expectedNewTimeStamp (abs)\t%d\t"),I64INT(expectedNewTimeStamp)));
        }
    else
        {
        //first frame
        expectedNewTimeStamp = 0;
        iDriftFrameSkipCount = 0;
        iAddedFrameDurationCount = 0;
        iPreviousCameraFrameIndex = 0;
        }

    TInt frameDropCount = aFrameBuffer->iIndexOfFirstFrameInBuffer - iPreviousCameraFrameIndex - 1;
    iPreviousCameraFrameIndex = aFrameBuffer->iIndexOfFirstFrameInBuffer;
    //
    // Handle case where camera has dropped frames.
    //
    if ( frameDropCount > 0 )
        {// Camera dropped frame
        PRINT((_L("CCMRVideoRecorder::MvsoFrameBufferReady(), Camera dropped %d frame(s), drift from expected: \t%d\t"),
                                                frameDropCount,
                                                I64INT((frameTimeStamp - expectedNewTimeStamp ))));
        frameTimeStamp = expectedNewTimeStamp + (frameDropCount*iSourceFrameInterval);
        }
	//
	// Handle case where camera frame timestamp is more than previous encoded timestamp + specified camera frame duration.
	//
    else if ( aFrameBuffer->iElapsedTime.Int64() + iSystemClockDelta.Int64() > expectedNewTimeStamp )
        {
        PRINT((_L("CCMRVideoRecorder::MvsoFrameBufferReady(), adjusted timestamp since it was greater than expected timestamp and we have delayed timestamps earlier\t%d\t"),I64INT((expectedNewTimeStamp - frameTimeStamp))));
        // we have adjusted timestamps more to positive direction (delayed timestamps), try to compensate it now
        // if camera frames have drifted more than 1 frame duration increase frame time stamp with one frame duration (as if frame dropped).
        if ((aFrameBuffer->iElapsedTime.Int64() + iSystemClockDelta.Int64() - expectedNewTimeStamp) > iSourceFrameInterval )
        	{
        	iAddedFrameDurationCount++;
        	PRINT((_L("CCMRVideoRecorder::MvsoFrameBufferReady(), Camera timestamp has drifted more than 1 frame duration - adding 1 frame duration to timestamp from now on!\t%d\t"), iAddedFrameDurationCount ));
        	expectedNewTimeStamp = iLatestAbsoluteTimeStamp.Int64() + 2*iSourceFrameInterval;
            PRINT((_L("CCMRVideoRecorder::MvsoFrameBufferReady(), new expectedNewTimeStamp (abs)\t%d\t"),I64INT(expectedNewTimeStamp)));
        	}
        frameTimeStamp = expectedNewTimeStamp;
        }
	//
	// Handles case where camera frame timestamp is less than previous encoded frame timestamp
	// * happens when Camera is producing higher FPS that target FPS causing frame timestamp to drift below calculated cumulative recording time.
	//
    else if ( aFrameBuffer->iElapsedTime.Int64() + iSystemClockDelta.Int64() < iLatestAbsoluteTimeStamp.Int64() )
        {
        // frame with the timestamp of this frame was already encoded
        PRINT((_L("CCMRVideoRecorder::MvsoFrameBufferReady(), drift between expected and frame buffer timestamp more than 1 frame duration, dropping frame!")));
        iDriftFrameSkipCount++;
        aFrameBuffer->Release();
        return;
        }
	//
	// Handle case where (camera frame index * framerate) is less than camera frame timestamp - specified camera frame duration
	// * happens when Camera is skipping frames without extra increase in frame index count.
	//
    else if ( frameTimeStamp < ( (aFrameBuffer->iElapsedTime.Int64() + iSystemClockDelta.Int64()) - iSourceFrameInterval ) )
        {
        PRINT((_L("CCMRVideoRecorder::MvsoFrameBufferReady(), adjusted timestamp since frame counter was too small compared to elapsed time: difference\t%d\t"),I64INT(aFrameBuffer->iElapsedTime.Int64() + iSystemClockDelta.Int64() - frameTimeStamp)));
        // frame numbers & elapsed time do not match, probably camera is skipping frames but doesn't know it / doesn't inform it to us
        frameTimeStamp = Max( aFrameBuffer->iElapsedTime.Int64() + iSystemClockDelta.Int64(), expectedNewTimeStamp );
        }
    //
    // Handle case where (camera frame index * framerate) is less than calculated estimated timestamp value for currect frame
    // (based on previous encoded frame timestamp + specified camera frame duration)
    //
    else if ( frameTimeStamp < expectedNewTimeStamp )
        {
        // timestamp should not be too close to the previous timestamp (video codecs set max for framerate <=> min for ts diff)
        PRINT((_L("CCMRVideoRecorder::MvsoFrameBufferReady(), adjusted timestamp since it was smaller than expected timestamp, difference\t%d\t"),I64INT(expectedNewTimeStamp - frameTimeStamp)));
        frameTimeStamp = expectedNewTimeStamp;
        }
    else
        {
        // leave it as it is
        }

    // for now we've been playing with absolute times, excluding all pauses & adjustments. Save this value as reference for determining timestamp for the next frame
    iLatestAbsoluteTimeStamp = frameTimeStamp;

    if ( iAdjustmentTimeUs != 0 )
        {
        // at the moment, initial and pause values are in opposite direction (+/-)
        // for initial adjustment:
        // if audio is ahead (iAdjustmentTimeUs > 0) => video should fast forward (video timestamps should be increased),
        // if audio is behind (iAdjustmentTimeUs < 0) => video should "pause" for a while (video timestamps should be decreased)
        // note that we modify here the iPausedTime which is subtracted from aFrameBuffer->iElapsedTime, so these
        // changes affect kind of reverse way

        if ( ( frameTimeStamp - (iTotalPausedTime.Int64() - iAdjustmentTimeUs) ) >= (iLatestUsedTimeStamp.Int64() + iSourceFrameInterval) )
            {
            // iAdjustmentTimeUs > 0 or < 0 but not too much < 0 so that the resulting timestamp won't be < (latest time stamp + sourceframeinterval)
            iTotalPausedTime = iTotalPausedTime.Int64() - iAdjustmentTimeUs;
            PRINT((_L("CCMRVideoRecorder::MvsoFrameBufferReady(), adjusted paused time by %d us"), I64INT(iAdjustmentTimeUs)));
            iAdjustmentTimeUs = 0;
            }
        else
            {
            // iAdjustmentTimeUs < 0 here, we should "pause" video (skip frames) until we reach the required time
            // Skipping is needed to avoid the timestamps to skip back over already captured frames.
            PRINT((_L("CCMRVideoRecorder::MvsoFrameBufferReady(), skipped a frame due to time adjustment request %d; buffer->elapsedTime %d; latest time stamp %d"), I64INT(iAdjustmentTimeUs), I64INT(aFrameBuffer->iElapsedTime.Int64() + iSystemClockDelta.Int64()), I64INT(iLatestUsedTimeStamp.Int64()) ));
            aFrameBuffer->Release();
            return;
            }
        }

    // subtract the time we were paused
    // (MSL assumes devvideorecord/hw device does this but we do it here)
    TTimeIntervalMicroSeconds iPrevUsedTimeStamp = iLatestUsedTimeStamp;
    iLatestUsedTimeStamp = frameTimeStamp - iTotalPausedTime.Int64();

    // Increment timestamp in case we dropped frames during pause
    while(( iLatestUsedTimeStamp < iPrevUsedTimeStamp ) && ( iNumberOfCapturedFrames > 0 ))
        {
        PRINT((_L("CCMRVideoRecorder::MvsoFrameBufferReady() incrementing timestamp by one frame")));
        iLatestUsedTimeStamp = TTimeIntervalMicroSeconds( iLatestUsedTimeStamp.Int64() + iSourceFrameInterval );
        }

    PRINT((_L("CCMRVideoRecorder::MvsoFrameBufferReady(), iTotalPausedTime \t%d"), I64INT(iTotalPausedTime.Int64()) ));
    PRINT((_L("CCMRVideoRecorder::MvsoFrameBufferReady(), iLatestUsedTimeStamp (relative) & drift from elapsed time\t%d"), I64INT(iLatestUsedTimeStamp.Int64()) ));

    EncodeFrame(aFrameBuffer);

    PRINT((_L("CCMRVideoRecorder::MvsoFrameBufferReady, out")));
    }


// ---------------------------------------------------------
// CCMRVideoRecorder::EncodeFrame
// Encodes the given video frame.
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CCMRVideoRecorder::EncodeFrame(MFrameBuffer* aFrameBuffer)
    {
    PRINT((_L("CCMRVideoRecorder::EncodeFrame, in")));

    // Take a TVideoPicture into use
    TVideoPicture* outputPicture = NULL;
    if ( iCodingFifo->IsEmpty() )
        {
        // allocate a new
        outputPicture = new TVideoPicture;
        if ( outputPicture == NULL )
            {
            // out of memory, stop operation
            DoSendEventToClient( KCMRRunTimeError, KErrNoMemory );
            aFrameBuffer->Release();
            return;
            }
        }
    else
        {
        // take an old one from the queue
        outputPicture = reinterpret_cast<TVideoPicture*>(iCodingFifo->Get());
        }

    iNumberOfCapturedFrames++;

#ifdef _DEBUG
    TTime current;
    current.UniversalTime();
    TInt elapsedMs = I64INT((current.MicroSecondsFrom(iLastCapture).Int64())) / 1000;
    iCumulativeCaptureTime += elapsedMs;
    iAverageCaptureTime = TReal(iCumulativeCaptureTime) / TReal(iNumberOfCapturedFrames);

    PRINT((_L("CCMRVideoRecorder::EncodeFrame() recording time %d, ms since last: %d, average capture time: %4.2f"),
        (I64INT(current.MicroSecondsFrom(iRecordStartTime).Int64())) / 1000,
        elapsedMs,
        iAverageCaptureTime));

    iLastCapture = current;
    iEncodingStartTime.UniversalTime();
#endif

    // wrap captured frame to encoder input buffer
    outputPicture->iData.iDataFormat = EYuvRawData;
    outputPicture->iData.iDataSize.iWidth = iFrameSize.iWidth;
    outputPicture->iData.iDataSize.iHeight = iFrameSize.iHeight;
    TRAPD(err, (outputPicture->iData.iRawData = (TPtr8*)aFrameBuffer->DataL(0)));
    if ( err != KErrNone )
        {
        // error, stop operation
        DoSendEventToClient( KCMRRunTimeError, err );
        aFrameBuffer->Release();
        delete outputPicture;
        return;
        }

    outputPicture->iTimestamp = iLatestUsedTimeStamp;
    outputPicture->iOptions = TVideoPicture::ETimestamp;
    outputPicture->iUser = this;

#ifdef _DEBUG__
    TTime current;
    current.UniversalTime();


    PRINT((_L("CCMRVideoRecorder::EncodeFrame()\t%d"),
        (I64INT(current.MicroSecondsFrom(iRecordStartTime).Int64())) ));
#endif


    // Put the captured buffer to iSourceFifo
    TRAP( err, iSourceFifo->PutL( reinterpret_cast<TAny*>(aFrameBuffer) ));
    if ( err != KErrNone )
        {
        // out of memory, stop operation
        DoSendEventToClient( KCMRRunTimeError, err );
        aFrameBuffer->Release();
        delete outputPicture;
        return;
        }


    // give the frame to devVideoRec for encoding
    iEncoderInputQueueLength++;
    TRAP( err, iDevVideoRec->WritePictureL( outputPicture ) );
    if ( err != KErrNone )
        {
        // error, stop operation, aFrameBuffer will be released from fifo when stopped
        DoSendEventToClient( KCMRRunTimeError, err );
        return;
        }

    PRINT((_L("CCMRVideoRecorder::EncodeFrame, picture %x written to devvr"),outputPicture));

#endif // #else if !VIDEO_FILE_OUTPUT
    }


// ---------------------------------------------------------
// CCMRVideoRecorder::MdvroInitializeComplete
// Called by DevVideoRecord when its initalization is complete
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CCMRVideoRecorder::MdvroInitializeComplete(TInt aError)
    {
    PRINT((_L("CCMRVideoRecorder::MdvroInitializeComplete(), error[%d]"), aError));

    if ( iDirectCapture )
        {
        // Since direct capture is used, it should be true. Actual init CAPI status is
        // included to aError now.
        iSourceInitComplete = ETrue;
        }

    iEncoderInitComplete = ETrue;

    if ( iSourceInitComplete && (iErrorCode != KErrNone) )
        {
        // there was an error in the source initialisation, but the reporting was
        // waiting for the completion of the encoder initialisation => report it now
        PRINT((_L("CCMRVideoRecorder::MdvroInitializeComplete() but camera reservation had error: %d"), iErrorCode));
        DoSendEventToClient( KCMRCameraReserveError, iErrorCode );
        iErrorCode = KErrNone;
        return;
        }

    if ( aError != KErrNone )
        {
        if ( aError == KErrHardwareNotAvailable && iVideoCodecHWAccelerated )
            {
            PRINT((_L("CCMRVideoRecorder::MdvroInitializeComplete() - initialization of HW accelerated video encoder failed, retrying with ARM codec")));
            // hw accel codec and init failed due to resource problems => retry with non-hw-accelerated
            iEncoderInitComplete = EFalse;
            PRINT((_L("CCMRVideoRecorder::MdvroInitializeComplete() - initialization of HW accelerated video encoder failed, delete devvideo")));
            if (iDevVideoRec)
                {
                delete iDevVideoRec;
                iDevVideoRec = NULL;
                }
            TRAPD(err, iDevVideoRec = CMMFDevVideoRecord::NewL( *this ));
            if ( err == KErrNone )
                {
                PRINT((_L("CCMRVideoRecorder::MdvroInitializeComplete() - initialization of HW accelerated video encoder failed, new devvideo created")));
                TRAP(err,SetupEncoderL());
                if ( err == KErrNone )
                    {
                    // ok, continue waiting for the callback
                    return;
                    }
                else
                    {
                    // store the last error, to be used in the operations below
                    aError = err;
                    }
                }
            else
                {
                // store the last error, to be used in the operations below
                PRINT((_L("CCMRVideoRecorder::MdvroInitializeComplete() - initialization of HW accelerated video encoder failed, new devvideo creation failed.")));
                aError = err;
                }
            }

        if ( iSourceInitComplete )
            {
            PRINT((_L("CCMRVideoRecorder::MdvroInitializeComplete() with error: %d"), aError));
            DoSendEventToClient( KCMREncoderInitError, aError );
            }
        else
            {
            // report only when source is completed too
            PRINT((_L("CCMRVideoRecorder::MdvroInitializeComplete() with error: %d, but must wait for camera reserve completion"), aError));
            iErrorCode = aError;
            }
        return;
        }

    if ( aError == KErrNone )
        {
        // If Encoder initialized Ok, give other run-time settings
        // 1. Give dynamic settings to encoder after initializing
        if ( !iVideoCodecHWAccelerated && ( iDevVideoRec->NumComplexityLevels(iEncoderHWDeviceId) > 0 ) )
            {
            iVideoComplexity = iConfig->PluginSettings().iVideoComplexitySetting;
            iDevVideoRec->SetComplexityLevel( iEncoderHWDeviceId, iVideoComplexity );
            }
        // HW codec-specific configuration is HW dependent, and hence outside the scope of Series 60

        // 2. Rate control options
        if ( iEncodingFrameRate > iMaxFrameRate4GivenSize )
            {
            iEncodingFrameRate = static_cast<TReal32>(iMaxFrameRate4GivenSize);
            }

        FillRateControlOptions( iRateControlOptions );
        iDevVideoRec->SetRateControlOptions(0, iRateControlOptions); // only base layer (layer 0) supported
        TRAPD(err,iVideoCodecData->SetPostInitParamsL(iDevVideoRec));
        if ( err ) {}
        // ignore the error, having some post init setting is not crucial

        }

    if ( iSourceInitComplete )
        {
        SetState(EStateReadyToRecord);

        // everything ok, inform observer that we are prepared.
        DoSendEventToClient( KCMRPrepareComplete, KErrNone );
        }
    }


// ---------------------------------------------------------
// CCMRVideoRecorder::MdvroReturnPicture
// Called by DevVideoRecord when it does not need the given
// input frame anymore
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CCMRVideoRecorder::MdvroReturnPicture(TVideoPicture* aPicture)
    {
#ifdef _DEBUG
    TTime current;
    current.UniversalTime();


    PRINT((_L("CCMRVideoRecorder::MdvroReturnPicture() time now\t%d"),
        (I64INT(current.MicroSecondsFrom(iRecordStartTime).Int64())) ));
#endif

    // take oldest frame from fifo
    VRASSERT( !iSourceFifo->IsEmpty() );// if empty, for some reason the buffer was already returned!!

    MFrameBuffer* buffer = reinterpret_cast<MFrameBuffer*>(iSourceFifo->Get());

    // check that its the oldest stored captured picture
    TPtr8* tmp = NULL;
    TRAPD(error, (tmp = static_cast<TPtr8*>(buffer->DataL(0))));
    if ( tmp != aPicture->iData.iRawData )
        {
        // pictures are returned in different order than they were sent
		RPointerArray<MFrameBuffer> buffers;
        TInt bufError = buffers.Append(buffer);
        VRASSERT( bufError == KErrNone );
        
        TUint i;
        for ( i = 1; i < iNumCameraBuffers; i++ )
            {
            bufError = buffers.Append(reinterpret_cast<MFrameBuffer*>(iSourceFifo->Get()));
            VRASSERT( bufError == KErrNone );

            TRAP(error, (tmp = static_cast<TPtr8*>(buffers[i]->DataL(0))));
            if ( tmp == aPicture->iData.iRawData )
                {
                // Found it! Put the retrieved buffers back to the queue
                TInt err = KErrNone;
                TUint j;
                for ( j = 0; j < i; j++ )
                    {
                    TRAP( err, iSourceFifo->PutL( reinterpret_cast<TAny*>(buffers[j]) ));
                    // This can't really fail since we just got the buffers from the fifo and in that case PutL doesn't allocate anything => can't leave
                    VRASSERT( err == KErrNone );
                    buffers[j] = NULL;
                    }
                break;
                }
            }
        VRASSERT( i < iNumCameraBuffers );
        buffer = buffers[i];
        buffers.Close();
        }

    // release captured buffer
    PRINT((_L("CCMRVideoRecorder::MdvroReturnPicture() releasing %x"), buffer ));
    buffer->Release();
    buffer = NULL;

    iEncoderInputQueueLength--;

    // save the picture holder to fifo
    TRAPD( err, iCodingFifo->PutL( reinterpret_cast<TAny*>(aPicture) ));
    if ( err != KErrNone )
        {
        // OOM, inform error and stop operation
        DoSendEventToClient( KCMRRunTimeError, err );
        delete aPicture;
        return;
        }
    PRINT((_L("CCMRVideoRecorder::MdvroReturnPicture() stored %x"), aPicture ));
    }


// ---------------------------------------------------------
// CCMRVideoRecorder::MdvroSupplementalInfoSent
// Called by DevVideoRecord when supplemental info (e.g.,
// VOL header) has been sent to the encoder
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CCMRVideoRecorder::MdvroSupplementalInfoSent()
    {
    PRINT((_L("CCMRVideoRecorder::MdvroSupplementalInfoSent()")));
    User::Panic(_L("CCMRVIDEORECORDER"), KErrGeneral);
    }


// ---------------------------------------------------------
// CCMRVideoRecorder::MdvroNewBuffers
// Called by DevVideoRecord when new bitstream buffers are
// available for retrieval
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CCMRVideoRecorder::MdvroNewBuffers()
    {
	PRINT((_L("CCMRVideoRecorder::MdvroNewBuffers() - START - iRequestStatus: 0x%08x"), iRequestStatus ));
    PRINT((_L("CCMRVideoRecorder::MdvroNewBuffers(), NumDataBuffers[%d]"), iDevVideoRec->NumDataBuffers() ));
    iNumberOfEncodedFrames++;

    TVideoOutputBuffer* buffer = NULL;
    for (;;)
        {
        TRAPD( err, ( buffer = iDevVideoRec->NextBufferL()));

        if ( err != KErrNone )
            {
            return;
            }
        if ( buffer == NULL )
            {
            break;
            }
/*      TInt bufferSize = buffer->iData.Length();
        TInt bufferTimeStamp = I64INT(buffer->iCaptureTimestamp.Int64());
        OstTraceExt2( TRACE_PERFORMANCE, CCMRVIDEORECORDER_MDVRONEWBUFFERS, "CCMRVideoRecorder::MdvroNewBuffers %d %d", bufferSize, bufferTimeStamp );
*/        
        // enter restricted area
        iMutexObj.Wait();
        // store
        PRINT((_L("CCMRVideoRecorder::MdvroNewBuffers(), storing buffer: 0x%x, timestamp: high: %u low: %u"), buffer, I64HIGH(buffer->iCaptureTimestamp.Int64()),I64LOW(buffer->iCaptureTimestamp.Int64())));
        iVideoOutputBufferInputQue.AddLast(*buffer);
        iNumberOfVideoOutputBuffers++;

        // leave restricted area
        iMutexObj.Signal();
        }

    // enter restricted area
    iMutexObj.Wait();
    if ( iRequestStatus )
        {
    	PRINT((_L("CCMRVideoRecorder::MdvroNewBuffers() - completing output request..." )));
        iOutputThreadHandle.RequestComplete( iRequestStatus, KErrNone );
        }
    else
        {
        // else a new request has not been issued yet
    	PRINT((_L("CCMRVideoRecorder::MdvroNewBuffers() - skipping RequestComplete" )));
        }
    // leave restricted area
    iMutexObj.Signal();

	PRINT((_L("CCMRVideoRecorder::MdvroNewBuffers() - END - iRequestStatus: 0x%08x"), iRequestStatus ));
    }


// ---------------------------------------------------------
// CCMRVideoRecorder::MdvroFatalError
// Called by devVideoRecord when a fatal error has occurred
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CCMRVideoRecorder::MdvroFatalError(TInt aError)
    {
    PRINT((_L("CCMRVideoRecorder::MdvroFatalError(), error[%d]"), aError));

    // Since now don't touch DevVideo that expects only destruction from the client, otherwise panics..
    iFatalError = ETrue;

    // Cancel returning buffers to DevVideo
    iBufferReturnAO->Cancel();

    if ( iDevVideoRec )
    	{
    	delete iDevVideoRec;
    	iDevVideoRec = NULL;
    	}

    if ( State() == EStateStopping )
        {
        // error occurred while stopping => carry out the rest of stopping & inform client
        SetState( EStateReadyToRecord );
        DoSendEventToClient( KCMRRecordingComplete, aError );
        }
    else
        {
        // inform client
        DoSendEventToClient( KCMRRunTimeError, aError );
        iErrorCode = aError;
        }
    }


// ---------------------------------------------------------
// CCMRVideoRecorder::MdvroStreamEnd
// Called by devVideoRecord when all pictures have been processed
// (after InputEnd is called)
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CCMRVideoRecorder::MdvroStreamEnd()
    {
    PRINT((_L("CCMRVideoRecorder::MdvroStreamEnd() in")));

    // MdvroStreamEnd was called by devVideo without InputEnd
    VRASSERT( iInputEnd );

    // get all available buffers from devvideo
    PRINT((_L("CCMRVideoRecorder::MdvroStreamEnd(), getting all encoded frames from DevVideo.")));                
    TVideoOutputBuffer* buffer = NULL;
    for (;;)
        {
        TRAPD( err, ( buffer = iDevVideoRec->NextBufferL()));
        if ( err != KErrNone || buffer == NULL)
            {
            break;
            }
        // enter restricted area
        iMutexObj.Wait();
        // store
        PRINT((_L("CCMRVideoRecorder::MdvroStreamEnd(), storing buffer: 0x%x, timestamp:%d"), buffer, I64INT(buffer->iCaptureTimestamp.Int64())));                  
        iVideoOutputBufferInputQue.AddLast(*buffer);
        iNumberOfVideoOutputBuffers++;

        // leave restricted area
        iMutexObj.Signal();
        }
    
    PRINT((_L("CCMRVideoRecorder::MdvroStreamEnd(), flushing written frames back to DevVideo.")));                
    iBufferReturnAO->Flush();    
    
    // stop
    if ( iDevVideoRec )
    	{
        PRINT((_L("CCMRVideoRecorder::MdvroStreamEnd() iDevVideoRec->Stop called")));    	
        iDevVideoRec->Stop();
    	}

    iStreamEnd = ETrue;
    
    // set state & inform MR => it can return from stop
    SetState( EStateReadyToRecord );

    // everything ok, inform observer that we are ready for a new recording (iStopping does it too, though).
    DoSendEventToClient( KCMRRecordingComplete, KErrNone );

    PRINT((_L("CCMRVideoRecorder::MdvroStreamEnd() out")));
    }


// -----------------------------------------------------------------------------
// CCMRVideoRecorder::RequestNewData
// Output active object is ready to accept new data
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CCMRVideoRecorder::RequestNewData(TRequestStatus& aStatus)
    {
	PRINT((_L("CCMRVideoRecorder::RequestNewData() - START - aStatus: 0x%08x, iRequestStatus: 0x%08x"), &aStatus, iRequestStatus ));

    // enter restricted area
    iMutexObj.Wait();

    iRequestStatus = &aStatus;
	aStatus = KRequestPending;

    // leave restricted area
    iMutexObj.Signal();

	PRINT((_L("CCMRVideoRecorder::RequestNewData() - END - aStatus: 0x%08x, iRequestStatus: 0x%08x"), &aStatus, iRequestStatus ));
    }


// -----------------------------------------------------------------------------
// CCMRVideoRecorder::RequestNewDataCancel
//
//
// -----------------------------------------------------------------------------
//
void CCMRVideoRecorder::RequestNewDataCancel(TRequestStatus& aStatus)
    {
	PRINT((_L("CCMRVideoRecorder::RequestNewDataCancel() - START - aStatus: 0x%08x, iRequestStatus: 0x%08x"), &aStatus, iRequestStatus ));

    // enter restricted area
    iMutexObj.Wait();

	if ( &aStatus == iRequestStatus )
		{
		iOutputThreadHandle.RequestComplete( iRequestStatus, KErrCancel );
		}

    // leave restricted area
    iMutexObj.Signal();

	PRINT((_L("CCMRVideoRecorder::RequestNewDataCancel() - END - aStatus: 0x%08x, iRequestStatus: 0x%08x"), &aStatus, iRequestStatus ));
	}


// -----------------------------------------------------------------------------
// CCMRVideoRecorder::GetNextBuffer
// Output active object takes the next output buffer
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
CCMRMediaBuffer* CCMRVideoRecorder::GetNextBuffer()
    {
    PRINT((_L("CCMRVideoRecorder::GetNextBuffer() in") ));
    if ( iDevVideoRec == NULL || iFatalError )
        {
        // probably we have not been opened yet
        PRINT((_L("CCMRVideoRecorder::GetNextBuffer(), We have not been opened yet, or there's an error from devVideo")));
        return NULL;
        }

    if ( iDecSpecInfo )
        {
        // we have MPEG-4 decoder configuration information, it must be processed first
        PRINT((_L("CCMRVideoRecorder::GetNextBuffer() iDecSpecInfo") ));
        iRemoveHeader = ETrue;
        const TUint8* data = iDecSpecInfo->Ptr();
        iDecSpecInfoLength = iDecSpecInfo->Length();

        CCMRMediaBuffer::TBufferType decSpecType = CCMRMediaBuffer::EVideoMPEG4DecSpecInfo;
        if ( iVideoBufferType == CCMRMediaBuffer::EVideoH264NAL )
            {// H.264 AVC NAL / GenericPayload encapsulation
            decSpecType = CCMRMediaBuffer::EVideoH264NALDecSpecInfo;
            }
        else if ( iVideoBufferType == CCMRMediaBuffer::EVideoH264Bytestream )
            {
            decSpecType = CCMRMediaBuffer::EVideoH264BytestreamDecSpecInfo;
            }

        // wrap the relevant data to CCMRMediaBuffer; iOutputSinkBuffer can't be != NULL here since it is created in ConstructL
        iOutputSinkBuffer->Set( TPtrC8(data,iDecSpecInfoLength),
                                decSpecType,
                                iDecSpecInfoLength,
                                EFalse,
                                TTimeIntervalMicroSeconds(0) );

        PRINT((_L("CCMRVideoRecorder::GetNextBuffer() decoder specific info to sink sent")));
        return iOutputSinkBuffer;
        }

    // enter restricted area
    iMutexObj.Wait();
    if (iNumberOfVideoOutputBuffers > 0)
        {
        iOutputVideoBuffer = iVideoOutputBufferInputQue.First();

        // Remove the picture from the list
        if ( iOutputVideoBuffer != NULL )
            {
            iOutputVideoBuffer->iLink.Deque();
            iNumberOfVideoOutputBuffers--;
            }
        // leave restricted area
        iMutexObj.Signal();
        }
    else
        {
        // leave restricted area
        PRINT((_L("CCMRVideoRecorder::GetNextBuffer()returning null, leave restricted area, iNumberOfVideoOutputBuffers == 0 %x"), this));
        iMutexObj.Signal();
        return NULL;
        }

    PRINT((_L("CCMRVideoRecorder::GetNextBuffer() got [0x%x] from devvr"), iOutputVideoBuffer ));

    if ( iOutputVideoBuffer != NULL )
        {
        PRINT((_L("CCMRVideoRecorder::GetNextBuffer() processing iOutputVideoBuffer") ));
        TBool outputMediaBufferSet = EFalse;
        if ( iRemoveHeader )
            {
            // check if we need to remove VOS+VO+VOL header from the bitstream, since it is stored in metadata, and
            // having it in 2 places may cause interoperability problems
            PRINT((_L("CCMRVideoRecorder::GetNextBuffer() remove extra header info") ));
            iRemoveHeader = EFalse;
            if ( iVideoBufferType == CCMRMediaBuffer::EVideoMPEG4 )
                {// MPEG-4
                RemoveSeqHeader( reinterpret_cast<TDesC8*>(&iOutputVideoBuffer->iData) );
                }
            else if ( iVideoBufferType == CCMRMediaBuffer::EVideoH264NAL )
                {// H.264 AVC NAL /  EDuGenericPayload / NAL encapsulation
                // Removes SPS & PPS from first frame from encoder to avoid situation where its both in
                // .mp4 file metadata and in bitstream of video track.
                // Make sure buffer contains only one frame and rewrite encapsulation to make sure its ok.
                TRAPD( error, RemoveNalDecSpecInfoHeaderL( reinterpret_cast<TDesC8*>(&iOutputVideoBuffer->iData) ) );
                if ( error != KErrNone )
                    {
                    PRINT((_L("CCMRVideoRecorder::GetNextBuffer() RemoveNalDecSpecInfoHeaderL error %d"), error ));
                    DoSendEventToClient( KCMRRunTimeError, error );
                    return NULL;
                    }
                outputMediaBufferSet = ETrue;
                }
            else if ( iVideoBufferType == CCMRMediaBuffer::EVideoH264Bytestream )
                {// H.264 AVC NAL /  EDuElementarystream / Bytestream
                // Removes SPS & PPS from first frame from encoder to avoid situation where its both in
                // .mp4 file metadata and in bitstream of video track.
                RemoveByteStreamDecSpecInfoHeader( reinterpret_cast<TDesC8*>(&iOutputVideoBuffer->iData) );
                outputMediaBufferSet = ETrue;
                }
            }

        if ( !outputMediaBufferSet )
        	{
            // wrap the relevant data to CCMRMediaBuffer
	        iOutputSinkBuffer->Set( iOutputVideoBuffer->iData,
									iVideoBufferType,
									iOutputVideoBuffer->iData.Length(),
									iOutputVideoBuffer->iRandomAccessPoint,
									iOutputVideoBuffer->iCaptureTimestamp );
        	}
        PRINT((_L("CCMRVideoRecorder::GetNextBuffer() iOutputSinkBuffer set") ));

#ifdef _DEBUG__
            TTime current;
            current.UniversalTime();
            TInt elapsedMs = (I64INT(current.MicroSecondsFrom(iEncodingStartTime).Int64())) / 1000;
            iCumulativeEncodingTime += elapsedMs;
            iAverageEncodingTime = TReal(iCumulativeEncodingTime) / TReal(iNumberOfEncodedFrames);


            PRINT((_L("CCMRVideoRecorder::GetNextBuffer() %d, time consumed: %d, average encoding time: %4.2f"),
                (I64INT(current.MicroSecondsFrom(iRecordStartTime).Int64())) / 1000,
                (I64INT(current.MicroSecondsFrom(iEncodingStartTime).Int64())) / 1000,
                iAverageEncodingTime));
#endif

#ifdef VIDEO_BS_FILE_OUTPUT
            // Write the frame to a file for testing purposes; have to ignore possible errors
            iOutputFile.Write(videoBuffer->iData);
#endif

        PRINT((_L("CCMRVideoRecorder::GetNextBuffer() a buffer to sink sent with timestamp high: %u low: %u"),I64HIGH(iOutputVideoBuffer->iCaptureTimestamp.Int64()),I64LOW(iOutputVideoBuffer->iCaptureTimestamp.Int64())));        
        return iOutputSinkBuffer;
        }
    else
        {
        PRINT((_L("CCMRVideoRecorder::GetNextBuffer() returning NULL - no new buffers in queue") ));
        // no more new buffers in queue
        return NULL;
        }
    }


// -----------------------------------------------------------------------------
// CCMRVideoRecorder::NumBuffersWaiting
// Return the number of buffers in the source
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt CCMRVideoRecorder::NumBuffersWaiting()
    {
    if (iFatalError)
        {
        // NumDataBuffers() cannot be called since fatalError was reported
        return 0;
        }

    if ( iDecSpecInfo )
        {
        // decspecinfo is not counted in iNumberOfVideoOutputBuffers
        return (iNumberOfVideoOutputBuffers+1);
        }
    else
        {
        return iNumberOfVideoOutputBuffers;
        }
    }


// -----------------------------------------------------------------------------
// CCMRVideoRecorder::LatestTimeStampL
// Return the latest time stamp from the input stream
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CCMRVideoRecorder::LatestTimeStampL(TTimeIntervalMicroSeconds& aTimeStamp) const
    {
    aTimeStamp = iLatestUsedTimeStamp;
    }

// -----------------------------------------------------------------------------
// CCMRVideoRecorder::DurationL
// Get duration of the recording (in practice the same as the latest time stamp)
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CCMRVideoRecorder::DurationL(TTimeIntervalMicroSeconds& aDuration) const
    {
    LatestTimeStampL( aDuration );
    }

// -----------------------------------------------------------------------------
// CCMRVideoRecorder::ReturnBuffer
// Output active object returns an emptied buffer
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CCMRVideoRecorder::ReturnBuffer(CCMRMediaBuffer* aBuffer)
    {
    PRINT((_L("CCMRVideoRecorder::ReturnBuffer() buffers 0x%x, 0x%x"),aBuffer, iOutputSinkBuffer));
    VRASSERT( iOutputSinkBuffer == aBuffer );

    if ( iFatalError )
        {
        PRINT((_L("CCMRVideoRecorder::ReturnBuffer(), don't return anything, since there's an error from devVideo")));
        iOutputVideoBuffer = NULL;
        return;
        }

    iOutputSinkBuffer = aBuffer;
    if ( iDecSpecInfo )
        {
        delete iDecSpecInfo;
        iDecSpecInfo = NULL;
        // iOutputVideoBuffer is not used with decspecinfo
        }
    else
        {
        // normal case; iOutputVideoBuffer != NULL if functions are called in correct order
        PRINT((_L("CCMRVideoRecorder::ReturnBuffer() iOutputVideoBuffer 0x%x"), iOutputVideoBuffer));
        VRASSERT( iOutputVideoBuffer != NULL );
        // store the buffer for sending from this thread
        iBufferReturnAO->EnqueueReturnBuffer(iOutputVideoBuffer);

        iOutputVideoBuffer = NULL;
        }
    }



// -----------------------------------------------------------------------------
// CCMRVideoRecorder::FillRateControlOptions
// Fill DevVideoRecord rate control options structure
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CCMRVideoRecorder::FillRateControlOptions( TRateControlOptions& aRC )
    {
    TRAPD(error,CCMRRecorderBase::TargetBitRateL((TInt&)(aRC.iBitrate)));

    // error can be ignored, it will never be != KErrNone at this stage
    if (error != KErrNone)
        {
        VRASSERT( error == KErrNone );
        }

    if ( iBitRateMode == EBitRateConstant )
        {
        aRC.iControl = EBrControlStream;
        }
    else
        {
        // Variable bitrate; rc.iBitrate is probably not needed but it gives now the upper limit
        aRC.iControl = EBrControlNone;
        }

    aRC.iPictureRate = iEncodingFrameRate;
    // these are filled already in constructor:
    //aRC.iPictureQuality
    //aRC.iLatencyQualityTradeoff
    //aRC.iQualityTemporalTradeoff
    PRINT((_L("CCMRVideoRecorder::FillRateControlOptions() - framerate %f, bitrate %d"), aRC.iPictureRate, aRC.iBitrate));
    }


// -----------------------------------------------------------------------------
// CCMRVideoRecorder::CheckExposure
// Check exposure from camera and adjust framerates if needed
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CCMRVideoRecorder::CheckExposure()
    {
    if ( iSource->GetExposure() == CCamera::EExposureNight )
        {
        iNightMode = ETrue;
        iSourceFrameRate = iConfig->PluginSettings().iVideoNightFrameRate;
        iSourceFrameInterval = convertFrameRateToInterval(iSourceFrameRate);

        // new framerate should be set to the encoder too
        TRAPD(error, SetFrameRateL( iSourceFrameRate ));

        // error can be ignored, it will never be != KErrNone here
        if (error != KErrNone)
            {
            VRASSERT( error == KErrNone );
            }
        }
    else
        {
        // other modes don't affect to framerates, except if we switch back from night mode to normal
        if ( iNightMode )
            {
            iSourceFrameRate = iRequestedFrameRate;
            iSourceFrameInterval = convertFrameRateToInterval(iSourceFrameRate);

            // new framerate should be set to the encoder too
            TRAPD(error,SetFrameRateL( iSourceFrameRate ));

            // error can be ignored, it will never be != KErrNone here
            if (error != KErrNone)
                {
                VRASSERT( error == KErrNone );
                }

            iNightMode = EFalse;
            }
        }

    }


// -----------------------------------------------------------------------------
// CCMRVideoRecorder::UpdateSupportedFrameSizesRates
// Update supported video frames and sizes table from ECAM and MDF encoders.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CCMRVideoRecorder::UpdateSupportedVideoFrameSizesRates()
	{
	PRINT((_L("CCMRVideoRecorder::UpdateSupportedFrameSizesRates() in")));
	iAvailableVideoFrameSizesRates.Reset();

	if ( iAvailableVideoEncoders.Count() )
		{
	    for ( TInt i = 0 ; i < iAvailableVideoEncoders.Count(); i++ )
	        {
		    // encoder info for retrieving capabilities
		    CVideoEncoderInfo* encoderInfo = NULL;
	        PRINT((_L("CCMRVideoRecorder::UpdateSupportedFrameSizesRates() - getting info from a plugin index[%d] with Uid 0x%x"), i, iAvailableVideoEncoders[i].iUid ));
	        TRAPD(error, (encoderInfo = ReadEncoderInfoL(iAvailableVideoEncoders[i])) );

	        if ( (encoderInfo != NULL) && (error == KErrNone))
	            {
                // Check directCapture support for current codec
				if ( encoderInfo->SupportsDirectCapture() )
					{
			        // check max framerate for given picture size
			        RArray<TPictureRateAndSize> rateAndSize = encoderInfo->MaxPictureRates();
			        for ( TUint j = 0; j < rateAndSize.Count(); j++ )
			            {
			            TPictureRateAndSize newSizeRate = rateAndSize[j];
						TInt addError = iAvailableVideoFrameSizesRates.InsertInOrder(newSizeRate, TLinearOrder<TPictureRateAndSize>(TLinearOrderFuncVideoSizeRate) );
						if (addError == KErrNone)
							{
							PRINT((_L("CCMRVideoRecorder::UpdateSupportedFrameSizesRates() - Added size: %d x %d, rate: %f"),
																					rateAndSize[j].iPictureSize.iWidth,
																					rateAndSize[j].iPictureSize.iHeight,
																					rateAndSize[j].iPictureRate));
							}
						}
					}
				delete encoderInfo;
	            }
	        }
		}
	PRINT((_L("CCMRVideoRecorder::UpdateSupportedFrameSizesRates() out")));
	}

// -----------------------------------------------------------------------------
// CCMRVideoRecorder::DoSendEventToClient
// Send event to client
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt CCMRVideoRecorder::DoSendEventToClient(TUid aEventType, TInt aErrorCode)
    {
    PRINT((_L("CCMRVideoRecorder::DoSendEventToClient(), aEventType %d"),aEventType.iUid));
    TMMFEvent event(aEventType, aErrorCode);
    return iEventHandler.SendEventToClient(event);
    }


// -----------------------------------------------------------------------------
// CCMRVideoRecorder::ReturnBufferToDevVR
// CCMRReturnAO uses this function to return buffer to DevVideoRecord
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CCMRVideoRecorder::ReturnBufferToDevVR(TVideoOutputBuffer* aBuffer)
    {
    PRINT((_L("CCMRVideoRecorder::ReturnBufferToDevVR() in buffer: 0x%x, timestamp: high: %u low: %u"), aBuffer, I64HIGH(aBuffer->iCaptureTimestamp.Int64()),I64LOW(aBuffer->iCaptureTimestamp.Int64())));
    if (iDevVideoRec)
    	{
        iDevVideoRec->ReturnBuffer(aBuffer);
    	}
    PRINT((_L("CCMRVideoRecorder::ReturnBufferToDevVR() out")));
    }


// -----------------------------------------------------------------------------
// CCMRVideoRecorder::RemoveSeqHeader( TDesC8* aVideoBuffer )
// RemoveSeqHeader remove MPEG4 decoder configuration info (VOS+VO+VOL header)
// from the 1st video packet (it is saved in metadata, and duplication not allowed)
// Returns: TInt number of bytes removed
// -----------------------------------------------------------------------------
//
TInt CCMRVideoRecorder::RemoveSeqHeader( TDesC8* aVideoBuffer )
    {
    // MPEG-4 VOP header == the 1st element in the buffer to not remove
    const TUint8 KHeader[4] = {0x00, 0x00, 0x01, 0xb6};
    const TUint KSampleLength = 4;

    TPtr8* buffer = reinterpret_cast<TPtr8*>(aVideoBuffer);
    // Bitstream start point
    const TUint8* data = buffer->Ptr();
    TUint headerLength;
    TBool iFound = EFalse;

    // Search for VOP start code
    for ( headerLength = 0; headerLength < (buffer->Length() - (KSampleLength - 1)); headerLength++ )
        {
        if ( (data[headerLength] == KHeader[0])
            && (data[headerLength+1] == KHeader[1])
            && (data[headerLength+2] == KHeader[2])
            && (data[headerLength+3] == KHeader[3]) )
            {
            // VOP start code is found => video data starts from here
            iFound = ETrue;
            break;
            }
        }

    if ( (headerLength > 0) && iFound )
        {
        // adjust ptr & length depending which is longer VOP header location or earlier iDecSpecInfoLength length
        if ( iDecSpecInfoLength && (iDecSpecInfoLength <= headerLength ) )
	        {
	        buffer->Delete(0,iDecSpecInfoLength);
	        }
        else
	        {
			buffer->Delete(0,headerLength);
	        }
        }
    return headerLength;
    }

// -----------------------------------------------------------------------------
// CCMRVideoRecorder::RemoveNalDecSpecInfoHeaderL( TDesC8* aVideoBuffer )
// Removes decoder configuration info (SPS & PPS) from first NAL encapsulated H.264/AVC video buffer
// from encoder to avoid situation where its both in .mp4 file metadata and in bitstream of video track.
// Makes sure buffer contains only one frame and rewrite encapsulation to make sure its ok.
// -----------------------------------------------------------------------------
//
TInt CCMRVideoRecorder::RemoveNalDecSpecInfoHeaderL( TDesC8* aVideoBuffer )
	{
	// H.264 AVC NAL /  EDuGenericPayload / NAL encapsulation

    // Get reference to the buffer
    TPtr8* ptr = reinterpret_cast<TPtr8*>(aVideoBuffer);
    // Bitstream start point
    TPtr8& buffer(*ptr);
    TInt frameStart = 0;
    TInt frameSize = 0;
    TInt offset = 0;
    TInt bufType = 0;
    TInt i=0;
    TInt totalNALLength = 0;
    TInt firstCopiedNAL = 0;
    TInt nalCount = 0;
    RArray<TInt> nalSizes;
    RArray<TInt> nalStarts;

    PRINT((_L("CCMRVideoRecorder::RemoveNalDecSpecInfoHeaderL() in, length: %d "), buffer.Length() ));

	// There should be enough data for the NAL header + frame
    if ( buffer.Length() > 12 )
        {
        // Offset to the end and get NAL unit count
        offset = buffer.Length()-4; //last 4 bytes are the NAL unit count
        nalCount = TInt(buffer[offset]) +
                  (TInt(buffer[offset + 1]) << 8) +
                  (TInt(buffer[offset + 2]) << 16) +
                  (TInt(buffer[offset + 3]) << 24);

        for(i=0; i<nalCount; i++)
            {//go through all NAL units in buffer
            // Offset to the start of NAL Unit infos
            offset = buffer.Length()-4-(8*nalCount); // 4 is the NAL unit count at end of buffer, 8 bytes used per NAL Unit for FrameStartOffset and FrameSize.

            // Get frame start offset
            offset += 8*i;
            frameStart = TInt(buffer[offset]) +
                        (TInt(buffer[offset + 1]) << 8) +
                        (TInt(buffer[offset + 2]) << 16) +
                        (TInt(buffer[offset + 3]) << 24);

            PRINT((_L("CCMRVideoRecorder::RemoveNalDecSpecInfoHeaderL() NAL unit %d frame start: %d "), i, frameStart ));

            // Get frame size
            offset += 4;
            frameSize = TInt(buffer[offset]) +
                       (TInt(buffer[offset + 1]) << 8) +
                       (TInt(buffer[offset + 2]) << 16) +
                       (TInt(buffer[offset + 3]) << 24);

           PRINT((_L("CCMRVideoRecorder::RemoveNalDecSpecInfoHeaderL() NAL unit %d frame size: %d "), i, frameSize ));
           bufType = buffer[frameStart] & 0x1F;
           PRINT((_L("CCMRVideoRecorder::RemoveNalDecSpecInfoHeaderL() NAL unit %d type: %d "), i, bufType ));
           if ( bufType != 7 && //SPS
                bufType != 8 )//PPS
               {
               // we found first NAL unit that isn't SPS or PPS
               nalSizes.AppendL(frameSize);
               if (firstCopiedNAL==0)
                   {
                   firstCopiedNAL = frameStart;
                   }
               nalStarts.AppendL(frameStart-firstCopiedNAL);               
               totalNALLength = frameStart+frameSize-firstCopiedNAL;
               PRINT((_L("CCMRVideoRecorder::RemoveNalDecSpecInfoHeaderL() new total length: %d, padding: %d "), totalNALLength, totalNALLength%4 ));
               totalNALLength += totalNALLength%4;
               }
           }

        // The buffer should have enough space for the new NAL header

        // We need to write a new NAL header just after the frame end
        PRINT((_L("CCMRVideoRecorder::RemoveNalDecSpecInfoHeaderL() writing new header.")));
        offset = firstCopiedNAL + totalNALLength;
        
        if ( (offset + nalSizes.Count()*8 + 4) > buffer.Length() ) 
            {
            PRINT((_L("CCMRVideoRecorder::RemoveNalDecSpecInfoHeaderL() Fatal error, cannot create header, non-align 32bit encoder output buffer.")));
            VRASSERT(0);            
            }

        for (TInt j=0; j<nalSizes.Count(); j++)
            {
            PRINT((_L("CCMRVideoRecorder::RemoveNalDecSpecInfoHeaderL() new header, unit: %d, start: %d, size: %d."), j, nalStarts[j], nalSizes[j] ));            
            // Write NAL unit start position
            buffer[offset + 0] = TUint8(nalStarts[j] & 0xff);
            buffer[offset + 1] = TUint8((nalStarts[j] >> 8) & 0xff);
            buffer[offset + 2] = TUint8((nalStarts[j] >> 16) & 0xff);
            buffer[offset + 3] = TUint8((nalStarts[j] >> 24) & 0xff);

            // Write NAL unit size
            offset +=4;
            buffer[offset + 0] = TUint8(nalSizes[j] & 0xff);
            buffer[offset + 1] = TUint8((nalSizes[j] >> 8) & 0xff);
            buffer[offset + 2] = TUint8((nalSizes[j] >> 16) & 0xff);
            buffer[offset + 3] = TUint8((nalSizes[j] >> 24) & 0xff);

            offset +=4;
            }

        // Write the number of NAL units
        buffer[offset + 0] = TUint8(nalSizes.Count() & 0xff);;
        buffer[offset + 1] = TUint8((nalSizes.Count() >> 8) & 0xff);;
        buffer[offset + 2] = TUint8((nalSizes.Count() >> 16) & 0xff);
        buffer[offset + 3] = TUint8((nalSizes.Count() >> 24) & 0xff);
        }

    // Get a pointer to the position where the frame starts
    TPtrC8 tmp( (buffer.Ptr() + firstCopiedNAL), (totalNALLength + (nalSizes.Count()*8) + 4) );

    // Set output media buffer
    iOutputSinkBuffer->Set( tmp, CCMRMediaBuffer::EVideoH264NAL,
                            tmp.Length(), iOutputVideoBuffer->iRandomAccessPoint,
                            iOutputVideoBuffer->iCaptureTimestamp );

    nalSizes.Close();
    nalStarts.Close();
    return firstCopiedNAL;
    }

// -----------------------------------------------------------------------------
// CCMRVideoRecorder::RemoveByteStreamDecSpecInfoHeader( TDesC8* aVideoBuffer )
// Removes decoder configuration info (SPS & PPS) from first bytestream encapsulated H.264/AVC video buffer
// from encoder to avoid situation where its both in .mp4 file metadata and in bitstream of video track.
// -----------------------------------------------------------------------------
//
TInt CCMRVideoRecorder::RemoveByteStreamDecSpecInfoHeader( TDesC8* aVideoBuffer )
	{// H.264 AVC NAL /  EDuElementarystream / Bytestream
    TInt skippedBytes = -1;
    TInt bufType = 0;

    // Get reference to the buffer
    TPtr8* buffer = reinterpret_cast<TPtr8*>(aVideoBuffer);
    // Bitstream start point
    const TUint8* data = buffer->Ptr();

    PRINT((_L("CCMRVideoRecorder::RemoveByteStreamDecSpecInfoHeader() in, length: %d "), buffer->Length() ));

    // check buffer for bytestream header and possible decSpecInfo (SPS & PPS)
    for (TInt i=0; i+4< buffer->Length(); i++)
    	{
    	// check for bytestream encapsulation [00 00 00 01]
    	if ( data[i] == 0 &&
    		 data[i+1] == 0 &&
    		 data[i+2] == 0 &&
    		 data[i+3] == 1 )
    		{
    	    PRINT((_L("CCMRVideoRecorder::RemoveByteStreamDecSpecInfoHeader() 0001 header at: %d "), i ));
    		// search buffer that isn't SPS or PPS
    		bufType = data[i+4] & 0x1F;
    		PRINT((_L("CCMRVideoRecorder::RemoveByteStreamDecSpecInfoHeader() buffer type: %d "), bufType ));
    		if ( (bufType == 7 || //SPS
    			  bufType == 8) && //PPS
    			  skippedBytes == -1 )
    		    {// we need to skip SPS and PPS so activate next if clause
    			skippedBytes = 0;
    		    }
    		if ( bufType != 7 && //SPS
                 bufType != 8 && //PPS
                 skippedBytes == 0)
    		    {// found first non SPS / PPS buffer content -> stop stripping here
    			skippedBytes = i;
    		    PRINT((_L("CCMRVideoRecorder::RemoveByteStreamDecSpecInfoHeader() skipping: %d bytes"), skippedBytes ));
    			}
    		}
    	}
/*
    if (skippedBytes)
        {
        for (TInt j=0; j<skippedBytes;j++)
            {
            PRINT((_L("CCMRVideoRecorder::RemoveByteStreamDecSpecInfoHeader() skipping %d: %x "), j, data[j] ));
            }
        }
*/
    // Get a pointer to the position where the frame starts
    TPtrC8 tmp(buffer->Ptr() + skippedBytes, buffer->Length() - skippedBytes);

    // Set output media buffer
    iOutputSinkBuffer->Set( tmp, CCMRMediaBuffer::EVideoH264Bytestream,
                            tmp.Length(), iOutputVideoBuffer->iRandomAccessPoint,
                            iOutputVideoBuffer->iCaptureTimestamp );

    return skippedBytes;
	}

// ---------------------------------------------------------
// CCMRVideoRecorder::CCMRReturnAO::NewL
// Two-phased constructor.
// ---------------------------------------------------------
//
CCMRVideoRecorder::CCMRReturnAO* CCMRVideoRecorder::CCMRReturnAO::NewL(CCMRVideoRecorder* aHost)
    {
    PRINT((_L("CCMRVideoRecorder::CCMRReturnAO::NewL(), In")))

    CCMRReturnAO* self = new (ELeave) CCMRReturnAO(aHost);
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop();

    PRINT((_L("CCMRVideoRecorder::CCMRReturnAO::NewL(), Out")))
    return self;
    }

// -----------------------------------------------------------------------------
// CCMRVideoRecorder::CCMRReturnAO::CCMRReturnAO
// Default constructor for CCMRReturnAO
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
CCMRVideoRecorder::CCMRReturnAO::CCMRReturnAO(CCMRVideoRecorder* aHost) :
        CActive(CActive::EPriorityStandard),
        iVideoOutputBufferReturnQue(_FOFF(TVideoOutputBuffer,iLink)),
        iVideoOutputBufferReturnQueIter(iVideoOutputBufferReturnQue),
        iHost(aHost)
    {
    CActiveScheduler::Add(this);
    }

// ---------------------------------------------------------
// CCMRVideoRecorder::CCMRReturnAO::ConstructL()
// Symbian 2nd phase constructor can leave.
// ---------------------------------------------------------
//
void CCMRVideoRecorder::CCMRReturnAO::ConstructL()
    {
    PRINT((_L("CCMRVideoRecorder::CCMRReturnAO::ConstructL() in")));
    User::LeaveIfError(iMutexObj.CreateLocal());
    iMutexCreated = ETrue;
    User::LeaveIfError(iVideoThreadHandle.Open(RThread().Id()));
    iThreadHandleOpened = ETrue;

    iStatus = KRequestPending;
    SetActive();
    PRINT((_L("CCMRVideoRecorder::CCMRReturnAO::ConstructL() out")));
    }

// -----------------------------------------------------------------------------
// CCMRVideoRecorder::CCMRReturnAO::~CCMRReturnAO
// Destructor for CCMRReturnAO
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
CCMRVideoRecorder::CCMRReturnAO::~CCMRReturnAO()
    {
    PRINT((_L("CCMRVideoRecorder::CCMRReturnAO::~CCMRReturnAO")));
    if ( iMutexCreated )
        {
        iMutexObj.Close();
        }
    if ( iThreadHandleOpened )
        {
        iVideoThreadHandle.Close();
        }
    }

// -----------------------------------------------------------------------------
// CCMRVideoRecorder::CCMRReturnAO::EnqueueReturnBuffer
// Enqueue TVideoOutputBuffer from controller thread to be returned to
// DevVideoRecord from this thread
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CCMRVideoRecorder::CCMRReturnAO::EnqueueReturnBuffer(TVideoOutputBuffer* aBuffer)
    {
    PRINT((_L("CCMRVideoRecorder::CCMRReturnAO::EnqueueReturnBuffer() in aBuffer=0x%x "), aBuffer));
    // enter restricted area
    iMutexObj.Wait();
    iVideoOutputBufferReturnQue.AddLast(*aBuffer);
    if ( iStatus == KRequestPending )
        {
        TRequestStatus* tmp = &iStatus;
        iVideoThreadHandle.RequestComplete(tmp, KErrNone);
        }
    // leave restricted area
    iMutexObj.Signal();
    PRINT((_L("CCMRVideoRecorder::CCMRReturnAO::EnqueueReturnBuffer() out")));
    }


// -----------------------------------------------------------------------------
// CCMRVideoRecorder::CCMRReturnAO::Flush
// Flush content of return queueu to DevVideo
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CCMRVideoRecorder::CCMRReturnAO::Flush()
    {
    PRINT((_L("CCMRVideoRecorder::CCMRReturnAO::Flush() in.")));
    iMutexObj.Wait();
    while (!iVideoOutputBufferReturnQue.IsEmpty())
        {
        // take the oldest buffer
        TVideoOutputBuffer* tmp = iVideoOutputBufferReturnQue.First();
        // Remove the picture from the list
        tmp->iLink.Deque();
        // send it to DevVR using the CCMRVideoRecorder object
        iHost->ReturnBufferToDevVR(tmp);
        tmp = NULL;
        }
    // leave restricted area
    iMutexObj.Signal();
    PRINT((_L("CCMRVideoRecorder::CCMRReturnAO::Flush() out")));
    }

// -----------------------------------------------------------------------------
// CCMRVideoRecorder::CCMRReturnAO::RunL
// Returns the queued buffers
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CCMRVideoRecorder::CCMRReturnAO::RunL()
    {
    PRINT((_L("CCMRVideoRecorder::CCMRReturnAO::RunL() - iStatus: %d"), iStatus.Int() ));
    SetActive();
    // enter restricted area
    iMutexObj.Wait();
    iStatus = KRequestPending;
    while (!iVideoOutputBufferReturnQue.IsEmpty())
        {
        // take the oldest buffer
        TVideoOutputBuffer* tmp = iVideoOutputBufferReturnQue.First();
        // Remove the picture from the list
        tmp->iLink.Deque();
        // send it to DevVR using the CCMRVideoRecorder object
        iHost->ReturnBufferToDevVR(tmp);
        tmp = NULL;
        }
    // leave restricted area
    iMutexObj.Signal();
    PRINT((_L("CCMRVideoRecorder::CCMRReturnAO::RunL() out")));
    }

// -----------------------------------------------------------------------------
// CCMRVideoRecorder::CCMRReturnAO::RunError
// Handles errors from RunL
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt CCMRVideoRecorder::CCMRReturnAO::RunError(TInt aError)
    {
    PRINT((_L("CCMRVideoRecorder::CCMRReturnAO::RunError")));
    // currently RunL can't leave, but keep this here to remind the possible need in the future
    if (aError != KErrNone)
        {
        PRINT((_L("CCMRVideoRecorder::CCMRReturnAO::RunError() error %d"),aError));
        }
    return KErrNone;
    }

// -----------------------------------------------------------------------------
// CCMRVideoRecorder::CCMRReturnAO::DoCancel
// Cancels the active object
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CCMRVideoRecorder::CCMRReturnAO::DoCancel()
    {
    // Cancel the request
    PRINT((_L("CCMRVideoRecorder::CCMRReturnAO::DoCancel() in")));
    if ( iStatus == KRequestPending )
        {
        TRequestStatus *stat = &iStatus;
        User::RequestComplete(stat, KErrCancel);
        }
    PRINT((_L("CCMRVideoRecorder::CCMRReturnAO::DoCancel() out")));
    }

// End of file

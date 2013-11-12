/*
* Copyright (c) 2003-2008 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Video thread proxy class
*
*/

#ifndef CCMRVIDEOTHREADPROXY_H
#define CCMRVIDEOTHREADPROXY_H

// INCLUDES
#include "CCMRSubThread.h"
#include <mmf/devvideo/devvideoconstants.h>


// MACROS


// DATA TYPES

/**
* Video thread messages ids
*/ 
enum TCMRVideoSubThreadMessageIds
    {
    ECMRVideoThreadLoadVideoRecorder = ECMRThreadLastCommonMsgId+1,  //combine IDs with TCMRSubThreadMessageIds
    ECMRVideoThreadSetCameraHandle,
    ECMRVideoThreadSetOutput,
    ECMRVideoThreadSetOutputThreadId,
    ECMRVideoThreadSetBitRate,
    ECMRVideoThreadGetBitRate,
    ECMRVideoThreadSetFrameSize,
    ECMRVideoThreadGetFrameSize,
    ECMRVideoThreadSetFrameRate,
    ECMRVideoThreadGetFrameRate,
    ECMRVideoThreadSetCodec,
    ECMRVideoThreadGetCodec,
    ECMRVideoThreadSetMiscOptions,
    ECMRVideoThreadAdjustTimeStamps,
    ECMRVideoThreadPrepare,
    ECMRVideoThreadRecord,
    ECMRVideoThreadStop,
    ECMRVideoThreadPause,
    ECMRVideoThreadResume,
    ECMRVideoThreadSetClockSource,
    ECMRVideoThreadSetVideoRateControlOptions,
    ECMRVideoThreadGetVideoRateControlOptions,
    ECMRVideoThreadSetPreferredVideoEncoder,
    ECMRVideoThreadSetPreferredVideoEncapsulation,
    ECMRVideoRequestBuffersAndThreadWaitEOS,
    ECMRVideoThreadSetSegmentTargetSize
    };

// CONSTANTS
// error codes for events sent from video thread to client
const TUid KCMRCameraPrepareError = {0x0001};
const TUid KCMRCameraReserveError = {0x0002};
const TUid KCMRCameraCaptureError = {0x0004};
const TUid KCMREncoderInitError = {0x0008};
const TUid KCMRRunTimeError = {0x0010};
const TUid KCMRPrepareComplete = {0x0020};
const TUid KCMRRecordingComplete = {0x0040};
const TUid KCMRPrepareNeeded = {0x0080};


// FORWARD DECLARATIONS
class CCMRActiveOutput;
class TCCMRVideoCodingOptions;
class MMMFClockSource;
class CCMRConfigManager;
class TRateControlOptions;

//CLASS DECLARATIONS

/**
*  Video thread proxy class
*
*  @lib CAMCMEDIARECORDER.LIB
*  @since 2.1
*/
class RCMRVideoThreadProxy : public RCMRSubThreadBase
    {
    public: // Constructor
        /**
        * C++ default constructor.
        */
        RCMRVideoThreadProxy() : RCMRSubThreadBase(KCMRThreadProxyShutdownTimeout) {};
    public: // new functions
        /**                     
        * Create thread
        * @since 2.1
        * @param  TBool& aThreadCreated; thread may be created but session not necessarily so return value doesn't tell all
        * @return TInt error code
        */
        TInt CreateSubThread(TBool& aThreadCreated, CCMRConfigManager* aConfig );
        /**                     
        * Set camera handle
        * @since 2.1
        * @param  TInt aCameraHandle
        * @return TInt error code
        */
        TInt SetCameraHandle(TInt aCameraHandle) const;
        /**                     
        * Set Set config manager.
        * @since 3.2
        * @param  TInt aConfig
        * @return TInt error code
        */
        TInt SetConfigManager(CCMRConfigManager* aConfig) const;        
        /**                     
        * Set output active object
        * @since 2.1
        * @param  CCMRActiveOutput* aOutput
        * @return TInt error code
        */
        TInt SetOutput(CCMRActiveOutput* aOutput) const;
        /**                     
        * Set clock source
        * @since 3.0
        * @param  MMMFClockSource* aClockSource
        * @return TInt error code
        */
        TInt SetClockSource(MMMFClockSource* aClockSource) const;
        /**                     
        * Set target bitrate
        * @since 2.1
        * @param  TInt aBitRate
        * @return TInt error code
        */
        TInt SetBitRate(TInt aBitRate) const;
        /**                     
        * Get current target bitrate
        * @since 2.1
        * @param  TInt& aBitRate
        * @return TInt error code
        */
        TInt GetBitRate(TInt& aBitRate) const;
        /**                     
        * Set video frame size (width&height)
        * @since 2.1
        * @param  const TSize& aSize
        * @return TInt error code
        */
        TInt SetFrameSize(const TSize& aSize) const;
        /**                     
        * Get current video frame size (width&height)
        * @since 2.1
        * @param  TSize& aSize
        * @return TInt error code
        */
        TInt GetFrameSize(TSize& aSize) const;
        /**                     
        * Set video framerate
        * @since 2.1
        * @param  TReal32 aFrameRate
        * @return TInt error code
        */
        TInt SetFrameRate(TReal32 aFrameRate) const;
        /**                     
        * Get current video framerate
        * @since 2.1
        * @param  TReal32& aFrameRate
        * @return TInt error code
        */
        TInt GetFrameRate(TReal32& aFrameRate) const;
        /**                     
        * Set video codec MIME-type
        * @since 2.1
        * @param  const TDesC8& aMimeType
        * @return TInt error code
        */
        TInt SetCodec(const TDesC8& aMimeType) const;
        /**                     
        * Get current video codec MIME-type
        * @since 2.1
        * @param  TDes8& aMimeType
        * @return TInt error code
        */
        TInt GetCodec(TDes8& aMimeType) const;
        /**                     
        * Set video options
        * @since 2.1
        * @param  const TCCMRVideoCodingOptions& aOptions
        * @return TInt error code
        */
        TInt SetMiscOptions(const TCCMRVideoCodingOptions& aOptions) const;
        
        /**                     
        * Set video rate control options
        * @since 3.2
        * @param  const TRateControlOptions& aOptions
        * @return TInt error code
        */        
        TInt SetVideoRateControlOptions(const TRateControlOptions& aOptions) const;
        
        /**
        * Get video rate control options
        * @since 3.2
        * @param  const TRateControlOptions& aOptions
        * @return TInt error code
        */
        TInt GetVideoRateControlOptions(TRateControlOptions& aOptions) const;

        /**
        * Set video encoder using its UID. Usage optional.
        * This overrides Media Recorder internal search for encoder based on set video mime type ( SetVideoCodecL() ).
        *
        * @since 3.2.3
        * @param    "aEncoder"  "Video encoder UID."
        * @return void
        */
        TInt SetPreferredVideoEncoder(TUid& aEncoder);

        /**
        * Set video encoder output format encapsulation. Usage optional.
        * This overrides Media Recorder internal default preferences for TVideoDataUnitEncapsulation
        * if used encoder supports multiple encapsulations.
        *
        * Defaults are:
        *   H.263 and MPEG-4 part 2 : EDuElementaryStream
        *               H.264 / AVC : EDuGenericPayload
        *
        * @since 3.2.3
        * @param    "aCapsulation"  "Encapsulation for coded video data units."
        * @return void
        */
        TInt SetPreferredVideoEncapsulation(TVideoDataUnitEncapsulation aCapsulation);

        /**
        * Set video encoder target segment size. Usage optional.
        * @since 5.2
        * @param  TUint aLayer              Layer number
        * @param  TUint aSizeBytes          Segment target size in bytes
        * @param  TUint aSizeMacroblocks    Segment target size in number of macroblocks per segment
        * @return TBool
        */            
        TInt SetSegmentTargetSize(TUint aLayer, TUint aSizeBytes, TUint aSizeMacroblocks );        
        
        /**
        * Adjust video time stamps (+ or -)
        * @since 2.1
        * @param  const TInt aAdjustmentMs
        * @return TInt error code
        */
        TInt AdjustTimeStamps(const TInt aAdjustmentMs) const;

        /**
        * Set video thread priority
        * @since 2.8
        * @param  const TThreadPriority& audioThreadPriority
        * @return TInt error code
        */
        TInt SetThreadPriority(const TThreadPriority& audioThreadPriority) const;

        /**                     
        * Prepare for recording (freeze settings)
        * @since 2.1
        * @return TInt error code
        */
        TInt Prepare() const;
        /**                     
        * Start playing (recording)
        * @since 2.1
        * @return TInt error code
        */
        TInt Record() const;
        /**                     
        * Stop video recorder
        * @since 2.1
        * @return TInt error code
        */
        TInt Stop() const;
        /**                     
        * Used while stopping. Requests encoded video frames from adaptation and waits for the streamend / EOS notification.
        * @since 5.2
        * @return TInt error code
        */
        TInt RequestBuffersAndWaitEOSL(TInt& aVideoEOSReached) const;
        /**                     
        * Pause video recorder
        * @since 2.1
        * @return TInt error code
        */
        TInt Pause() const;
        /**                     
        * Resume video recorder
        * @since 2.1
        * @return TInt error code
        */
        TInt Resume() const;
        /**                     
        * Close thread
        * @since 2.1
        * @return TInt error code
        */
        void Close();
    };


#endif      
            
// End of File




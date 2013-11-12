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
* Description:  Header file for video recorder class
*
*/


#ifndef CCMRVIDEORECORDER_H
#define CCMRVIDEORECORDER_H

// INCLUDES

#include "CCMRRecorderBase.h"
#include "CCMRVideoSource.h"
#include "CCMRMediaSource.h"
#include <mmf/devvideo/devvideorecord.h>


// FORWARD DECLARATIONS
class CCMRFifo;
class CCMRActiveOutput;
class TCCMRVideoCodingOptions;
class CCMRVideoCodecData;
class CVideoEncoderInfo;


// this is set in .mmp
#ifdef VIDEO_FILE_INPUT
// use test file input
#include "ccmrvideofilesource.h"

#else
// use actual camera API
#include "CCMRVideoCameraSource.h"
#endif

// this is set in .mmp
// Write captured frames OR video bitstream to a file
#if defined VIDEO_FILE_OUTPUT || defined VIDEO_BS_FILE_OUTPUT
#include <f32file.h>
#endif

// FORWARD DECLARATIONS
class MAsyncEventHandler;
class CCMRConfigManager;


// CLASS DECLARATION


/**
*  Video recorder class
*
*  @lib CAMCMEDIARECORDER.LIB
*  @since 2.1
*/
class CCMRVideoRecorder : public CCMRRecorderBase, public MCMRVideoSourceObserver, 
                          public MMMFDevVideoRecordObserver, public MCMRMediaSource

    {
    public:
        // Video bit-rate control mode
        enum TCMRVideoBitRateMode
            {
            EBitRateConstant,
            EBitRateVariable
            };

    public:
        /**
        *  Internal class for returning buffers to devvideo using active object
        *  Needed since devvideo can't handle calls from multiple threads
        */
        class CCMRReturnAO : public CActive
            {
            public: // Constructors and destructor
                     
                /**
                * Two-phased constructor.
                */
                static CCMRReturnAO* NewL(CCMRVideoRecorder* aHost);
                /**
                * Destructor.
                */
                virtual ~CCMRReturnAO(); 

            public: // New functions
            
                /**
                * Enqueue TVideoOutputBuffer from controller thread to be returned to 
                * DevVideoRecord from this thread
                */
                void EnqueueReturnBuffer(TVideoOutputBuffer* aBuffer);
                
                /**
                * Flush TVideoOutputBuffer's from video thread to DevVideoRecord
                */                
                void Flush();

            protected: // from baseclass

                /**
                * From CActive Does the required action (calls the observer)
                */
                void RunL();
                /**
                * From CActive Cancels pending actions
                */
                void DoCancel();
                
                /**
                * From CActive Handles leaves from RunL
                */
                TInt RunError(TInt aError);

            private: // constructors
                
                /**
                * By default ConstructL is private.
                */
                void ConstructL();
                
                /**
                * C++ default constructor.
                */
                CCMRReturnAO(CCMRVideoRecorder* aHost);

            private: // data
            
                // Queue for buffers to be returned
                TDblQue<TVideoOutputBuffer> iVideoOutputBufferReturnQue;
                TDblQueIter<TVideoOutputBuffer> iVideoOutputBufferReturnQueIter;
                
                // "host" object used to actually return the buffer, it owns the DevVideoRecord object
                CCMRVideoRecorder* iHost;
                // Mutex
                TBool iMutexCreated;
                RMutex iMutexObj; 
                // Video thread handle; needed since AO may be activated from controller thread
                RThread iVideoThreadHandle;
                TBool iThreadHandleOpened;
            };
    
    public:  // Constructors and destructor
        
                        
        /**
        * Two-phased constructor.
        */
        
        static CCMRVideoRecorder* NewL(MAsyncEventHandler& aEventHandler, CCMRConfigManager* aConfig );

        /**
        * Destructor.
        */

        virtual ~CCMRVideoRecorder();               

    public:   // Constants        

        enum TErrorCode
            {
            EInternalAssertionFailure = -10030,
            };
        
    public: // New functions
        
        /**
        * Sets output active object
        * @since 2.1
        * @param CCMRActiveOutput* aOutput
        * @return void
        */
        void SetOutputL(CCMRActiveOutput* aOutput);
        /**
        * Sets clock source
        * @since 3.0
        * @param MMMFClockSource* aClockSource
        * @return void
        */
        void SetClockSource(MMMFClockSource* aClockSource);
        /**
        * Sets camera handle & create camera instance
        * @since 2.1
        * @param TInt aCameraHandle
        * @return void
        */
        void SetCameraHandleL(TInt aCameraHandle);

        /**
        * Sets id of the output thread
        * @since 2.1
        * @param TUint aThreadId
        * @return void
        */
        void SetOutputThreadIdL(TUint aThreadId);
               
        /**
        * Sets video Mime type (codec)
        * @since 2.1
        * @param aMimeType   video codec Mime type
        * @return void
        */
        void SetVideoCodecL(const TDesC8& aMimeType);

        /**
        * Get the currently used video codec
        * @since 2.1
        * @param  aVideoMimeType    Used codec as mime type
        * @return TInt Error code
        */
        void GetVideoCodecL( TDes8& aVideoMimeType ) const;


        /**
        * Sets new input & output frame size
        * @since 2.1
        * @param aSize New size
        * @return void
        */
        void SetFrameSizeL(const TSize& aSize);
        
        /**
        * Gets current input & output video frame size
        * @since 2.1
        * @param aSize Output parameter for the size
        * @return void
        */
        void FrameSizeL(TSize& aSize) const;
        
        /**
        * Sets new target encoding frame rate
        * @since 2.1
        * @param aFrameRate New frame rate
        * @return TInt Error code     
        */
        void SetFrameRateL(TReal32 aFrameRate);
        
        /**
        * Get current target encoding frame rate
        * @since 2.1
        * @return TReal32 Frame rate
        */
        void FrameRateL(TReal32& aFrameRate) const;

        /**
        * Set misc video coding options
        * @since 2.1
        * @param  aOptions      video coding options
        * @return void
        */
        void SetVideoCodingOptionsL(const TCCMRVideoCodingOptions& aOptions);
        
        /**                     
        * Set video rate control options
        * @since 3.2
        * @param  const TRateControlOptions& aOptions
        * @return void
        */        
        void SetVideoRateControlOptionsL(const TRateControlOptions& aOptions);   
        
        /**                     
        * Get video rate control options
        * @since 3.2
        * @param  const TRateControlOptions& aOptions
        * @return void
        */
        void GetVideoRateControlOptionsL(TRateControlOptions& aOptions);

        /**
        * Set video encoder using its UID. Usage optional.
        * This overrides Media Recorder internal search for encoder based on set video mime type ( SetVideoCodecL() ).
        * @since 3.2.3
        * @param    "aEncoder"  "Video encoder UID."
        * @return void
        */
        void SetPreferredVideoEncoderL(TUid& aEncoder);

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
        void SetPreferredVideoEncapsulationL(TVideoDataUnitEncapsulation aCapsulation);

        /**                     
        * Set video segment target size
        * @since 5.2
        * @param  TUint aLayer              Layer number
        * @param  TUint aSizeBytes          Segment target size in bytes
        * @param  TUint aSizeMacroblocks    Segment target size in number of macroblocks per segment
        * @return void
        */     
        void SetSegmentTargetSizeL(TUint aLayer, TUint aSizeBytes, TUint aSizeMacroblocks);        
        
        /**
        * Adjust time stamps of video
        * @since 2.1
        * @param  const TInt aAdjustmentMs time in ms, can be positive or negative
        * @return void
        */
        void AdjustTimeStampsL(const TInt aAdjustmentMs);
        
        /**                     
        * Return buffer to DevVideoRecord
        * @param  TVideoOutputBuffer* aBuffer returned buffer
        * @return void
        */                
        void ReturnBufferToDevVR(TVideoOutputBuffer* aBuffer);

        /**
        * Used while stopping. Requests encoded video frames from adaptation and waits for the streamend / EOS notification.
        * @since 5.2
        * @return void
        */
        void RequestBuffersAndWaitEOSL(TInt& aVideoEOSReached);  

    public: // Functions from base classes

        /**
        * From CCMRRecorderBase Set new target bitrate
        */
        void SetTargetBitRateL(TInt aBitRate);

        /**
        * From CCMRRecorderBase Get current target bitrate
        */
        void TargetBitRateL(TInt& aBitRate);

        /**
        * From CCMRRecorderBase Prepare for recording
        */
        void PrepareL();

        /**
        * From CCMRRecorderBase Start recording
        */
        void RecordL();

        /**
        * From CCMRRecorderBase Stops recording (async, client must wait for state change)
        */
        void StopL();

        /**
        * From CCMRRecorderBase Pause recording
        */
        void PauseL();

        /**
        * From CCMRRecorderBase Resume recording
        */
        void ResumeL();     

        /**
        * From MDEVVRVideoRecordObserver Return a video picture to the devVideo interface
        * @param aPicture Pointer to the returned picture        
        */
        void MdvroReturnPicture(TVideoPicture* aPicture);

        /**
        * From MDEVVRVideoRecordObserver Supplemental info sent to encoder
        */
        void MdvroSupplementalInfoSent();

        /**
        * From MDEVVRVideoRecordObserver New buffers available for retrieval
        */
        void MdvroNewBuffers();

        /**
        * From MDEVVRVideoRecordObserver Fatal error has occurred
        */
        void MdvroFatalError(TInt aError);

        /**
        * From MDEVVRVideoRecordObserver Initialisation has been completed
        * @param aError Error code
        */
        void MdvroInitializeComplete(TInt aError);

        /**
        * From MDEVVRVideoRecordObserver All pictures have been processed.
        */
        void MdvroStreamEnd();

        /**
        * From MCMRVideoSourceObserver Video source has been reserved
        */
        void MvsoReserveComplete(TInt aError);

        /**
        * From MCMRVideoSourceObserver A video frame has been captured
        */
        void MvsoFrameBufferReady(MFrameBuffer* aFrameBuffer,TInt aError);
        

        /**
        * From MCMRMediaSource Output active object is ready to accept new data
        */
        void RequestNewData(TRequestStatus& aStatus);

        /**
        * From MCMRMediaSource Output active object is cancelling new data request
        */
        void RequestNewDataCancel(TRequestStatus& aStatus);

        /**
        * From MCMRMediaSource Output active object takes the next output buffer
        */
        CCMRMediaBuffer* GetNextBuffer();

        /**
        * From MCMRMediaSource Return the number of buffers in the source
        */
        TInt NumBuffersWaiting();

        /**
        * From MCMRMediaSource Return the latest time stamp from the input stream
        */
        void LatestTimeStampL(TTimeIntervalMicroSeconds& aTimeStamp) const;

        /**
        * From MCMRMediaSource Return the duration of the recording
        */
        void DurationL(TTimeIntervalMicroSeconds& aDuration) const;

        /**
        * From MCMRMediaSource Output active object returns an emptied buffer
        */
        void ReturnBuffer(CCMRMediaBuffer* aBuffer);

    private: // constructors
        
        /**
        * C++ default constructor.
        */
        CCMRVideoRecorder(MAsyncEventHandler& aEventHandler) : iEventHandler(aEventHandler),
        	                iVideoOutputBufferInputQue(_FOFF(TVideoOutputBuffer,iLink)),
	                        iVideoOutputBufferInputQueIter(iVideoOutputBufferInputQue)
            {};

        /**
        * By default EPOC constructor is private.
        */
        void ConstructL( CCMRConfigManager* aConfig );  
        

    private: // new functions

        /**
        * Select encoder plugin used in devvr
        * @return void
        */
        void SetupEncoderL();

        /**
        * Read encoder plugin information object from plugin with given Uid
        * @param TUid aUid
        * @return CDEVVRVideoEncoderInfo* information object
        */
        CVideoEncoderInfo* ReadEncoderInfoL(TUid aUid);
        
        /**
        * Encode the given video frame
        * @param aFrameBuffer The buffer to be encoded
        */
        void EncodeFrame(MFrameBuffer* aFrameBuffer);

        /**
        * Fills DevVideoRecord rate control options structure
        * @return void
        */
        void FillRateControlOptions( TRateControlOptions& aRC );

        /**
        * Send event to client
        * @return error code
        */
        TInt DoSendEventToClient(TUid aEventType, TInt aErrorCode);

        /**
        * Check exposure setting from camera and decice framerates based on it
        * @return void
        */
        void CheckExposure();
        
        /**
        * Update available video frames and sizes table from ECAM and MDF encoders.
        * @return void
        */
        void UpdateSupportedVideoFrameSizesRates();
        
        /**
		* RemoveSeqHeader remove MPEG4 decoder configuration info (VOS+VO+VOL header)
		* from the 1st video packet (it is saved in metadata, and duplication not allowed)
        * @return number of bytes removed
        */        
        TInt RemoveSeqHeader( TDesC8* aVideoBuffer );
        
        /**
        * Removes decoder configuration info (SPS & PPS) from first NAL encapsulated H.264/AVC video buffer
        * from encoder to avoid situation where its both in .mp4 file metadata and in bitstream of video track.
        * @return number of bytes removed
        */
        TInt RemoveNalDecSpecInfoHeaderL( TDesC8* aVideoBuffer );
        
        /**
        * Removes decoder configuration info (SPS & PPS) from first bytestream encapsulated H.264/AVC video buffer
        * from encoder to avoid situation where its both in .mp4 file metadata and in bitstream of video track.
        * @return number of bytes removed
        */
        TInt RemoveByteStreamDecSpecInfoHeader( TDesC8* aVideoBuffer );

    private:    // Data                 

        // event handler
        MAsyncEventHandler& iEventHandler;

        // source for raw frames        
#ifdef VIDEO_FILE_INPUT
        // File input test
        CCMRVideoFileSource *iSource;    
#else
        // Symbian Onboard Camera API handle
        CCMRVideoCameraSource *iSource;  
#endif

        // Video source -specific

        // output format from source
        CCamera::TFormat iVideoFormat;  
        // source initialization done ?
        TBool iSourceInitComplete;  
        // camera properties
        TCameraInfo iCameraInfo;    
        // size index from enumeration, the used one and then saved ones for both supported formats YUV420 planar and YUV422 interleaved
        TInt iSizeIndex;
        TInt iSizeIndex420;
        TInt iSizeIndex422;
        // rate index from enumeration, the used one and then saved ones for both supported formats YUV420 planar and YUV422 interleaved
        TInt iRateIndex;
        TInt iRateIndex420;
        TInt iRateIndex422;
        // Encoder UID from user - override for encoder search
        TUid iPreferredEncoderUID;
        // Video output data unit format encapsulation from user - override for internal default.
        TBool iPreferredEncapsulationSet;
        TVideoDataUnitEncapsulation iPreferredEncapsulation;

        // DevVideoRecord -specific
        // handle
        CMMFDevVideoRecord* iDevVideoRec;
        // encoder initialization done ?
        TBool iEncoderInitComplete;         
        // Mime type for encoded data
        TBuf8<256> iMimeType;                
        // Array of available encoders for given video mime type.
        RArray<TUid> iAvailableVideoEncoders;                
        // device ID for the encoder        
        THwDeviceId iEncoderHWDeviceId;  
        // device ID for the preprocessor        
        THwDeviceId iPreProcessorHWDeviceId;
        // output buffer pointer (from encoder)
        TVideoOutputBuffer* iOutputVideoBuffer;
       	// video encoder output buffer type
       	CCMRMediaBuffer::TBufferType iVideoBufferType;
       	// available frame sizes and rates for given video codec.
       	RArray<TPictureRateAndSize> iAvailableVideoFrameSizesRates;
		// Index of iAvailableVideoFrameSizesRates that matches current set video frame size & rate
       	TInt iSizeIndexDCEncoder;
		// Index of iAvailableVideoFrameSizesRates that matches current set video frame size & rate       	
		TInt iRateIndexDCEncoder;

        TDblQue<TVideoOutputBuffer> iVideoOutputBufferInputQue;
        TDblQueIter<TVideoOutputBuffer> iVideoOutputBufferInputQueIter;
        TInt iNumberOfVideoOutputBuffers;
        
        // class for codec-specific data
        CCMRVideoCodecData* iVideoCodecData;

        CCMRReturnAO* iBufferReturnAO;
        
        // time between random access points
        TReal iMinRandomAccessPeriodInSeconds;

        // ETrue if we are using HW accelerated video encoder => affects e.g. on default settings such as framerate
        TBool iVideoCodecHWAccelerated;

        // clocksource
        MMMFClockSource* iClockSource;
        // Config manager
        // Doesn't own.
        CCMRConfigManager* iConfig;
        // the sink for data
        CCMRActiveOutput* iOutput;
        // output mediasink buffer
        CCMRMediaBuffer* iOutputSinkBuffer;

        // FIFO for maintaining source buffers
        CCMRFifo *iSourceFifo;  
        // FIFO for maintaining buffers being encoded
        CCMRFifo *iCodingFifo;  
        // Number of pictures in the encoder input queue at the moment
        TUint iEncoderInputQueueLength;
        // Number of camera buffers
        TUint iNumCameraBuffers;

        // input/output frame dimensions
        TSize iFrameSize;    
        // capture frame rate in frames per second
        TReal32 iSourceFrameRate;  
        // average time between 2 successive captured frames (==timeunit/framerate)
        TInt iSourceFrameInterval;
        // target encoding frame rate in frames per second
        TReal32 iEncodingFrameRate;
        // max encoding frame rate in frames per second the used encoder promises to produce
        TReal iMaxFrameRate4GivenSize;
        // capture & target encoding frame rate for the next recording if given while recording
        TReal32 iRequestedFrameRate;

        // ETrue if camera's exposure is set to night mode
        TBool iNightMode;

        // constant or variable bit-rate?
        TInt iBitRateMode;

        // Error code saved while waiting for a completion of an async operation
        TInt iErrorCode;

        // thread handle for output active object
        RThread iOutputThreadHandle;
        // Status info for output active objct
        TRequestStatus* iRequestStatus;

        // ETrue if thread handle was opened
        TBool iThreadHandleOpened;

        // Video encoder complexity level
        TInt iVideoComplexity;

        // Time stamp adjustment after pause
        TTime iTimeWhenPaused;
        TTimeIntervalMicroSeconds iTotalPausedTime;
        TTimeIntervalMicroSeconds iSystemClockDelta;

        // Timestamp of the latest buffer given to devvideorecord
        TTimeIntervalMicroSeconds iLatestUsedTimeStamp;
        TTimeIntervalMicroSeconds iLatestAbsoluteTimeStamp;

        // Required adjustment for the timestamp in microseconds, 
        // will be included in iTotalPausedTime when possible
        TInt64 iAdjustmentTimeUs;

        // Mutex used to use variables safely in case of use from other thread
        RMutex      iMutexObj; 
        TBool       iMutexCreated;

        // MPEG-4 decoder configuration information
        HBufC8* iDecSpecInfo;
        // MPEG-4 decoder configuration information data length
        TInt iDecSpecInfoLength;
        // Flag to remove decoder configuration info from the 1st video packet (it is saved in metadata, and duplication not allowed)
        TBool iRemoveHeader;

        // Indicates fatal error from devVideo
        TBool iFatalError;

        // Indicates input end call to devVideo
        TBool iInputEnd;
        
        // Indicates stream end call from devVideo
        TBool iStreamEnd;
        
        // Indicates when to skip buffers between inputend -> streamend to avoid parent thread lockup.
        TBool iStoppingSkipBuffers;

        // For statistics
        TInt iNumberOfCapturedFrames;
        TInt iNumberOfEncodedFrames;
        
        // Direct capture
        TBool iDirectCapture;
        
        // Camera handle
        TInt iCameraHandle;

        // Skip buffer flag
        TBool iSkipBuffers;
        
        // Number of input camera frames skipped because of frame drift from camera.
        TInt iDriftFrameSkipCount;
        // Number of input camera frames that has had their frame duration increased to compensate
        // camera timestamp drift from expected.
        TUint iAddedFrameDurationCount;
        
        // Index of previous camera frame
        TInt iPreviousCameraFrameIndex;
        
        // TRateControlOptions received from client/ICM
        TRateControlOptions iRateControlOptions;
           
#if defined VIDEO_FILE_OUTPUT || defined VIDEO_BS_FILE_OUTPUT
        RFs iFs;
        RFile iOutputFile;
#endif           

#ifdef _DEBUG
        TTime iRecordStartTime;
        
        TTime iLastCapture;        
        TInt iCumulativeCaptureTime;
        TReal iAverageCaptureTime; // per frame

        TTime iEncodingStartTime;  // per frame
        TInt iCumulativeEncodingTime;
        TReal iAverageEncodingTime; // per frame
#endif

    };  
    
#endif      // CCMRVIDEORECORDER_H
    
// End of File
    
    
    
    
    

    
    
    

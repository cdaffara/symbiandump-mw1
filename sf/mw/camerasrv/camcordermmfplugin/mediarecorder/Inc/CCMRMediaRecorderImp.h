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
* Description:  Media recorder implementation header
*
*/


#ifndef CCMRMEDIARECORDERIMP_H
#define CCMRMEDIARECORDERIMP_H

//  INCLUDES
#include "CCMRMediaRecorder.h"
#include "CCMRVideoRecorderClient.h"
#include "CCMRAudioRecorder.h"
#include "CCMRActiveOutput.h"

// FORWARD DECLARATION
class CCMRPerfMonitor;
class CCMRConfigManager;


// CLASS DECLARATION

/**
*  Media recorder implementation
*
*  @lib CAMCMEDIARECORDER.LIB
*  @since 2.1
*/

class CCMRMediaRecorderImp : public CCMRMediaRecorder, public MCMRVideoRecorderObserver,
                             public MCMRAudioRecorderObserver, public MCMRActiveOutputObserver
    {

    public:  // Constants

        // Error codes
        enum TErrorCode
            {
            EInternalAssertionFailure = -10000
            };

    public:  // Constructors and destructor
        
        /**
        * C++ default constructor.
        */
        CCMRMediaRecorderImp();

        /**
        * Symbian 2nd phase constructor
        */
        void ConstructL();                
        
        /**
        * Destructor.
        */
        ~CCMRMediaRecorderImp();    

    public: // new functions

        /**
        *  Internal class for reporting errors to observer using active object
        *  Needed to break the callback chain since controller may call MR::StopL from this callback
        */
        class CCMRErrorReporter : public CActive
            {
            public: // Constants
                enum TErrorType
                    {
                    EErrorFatal,
                    EErrorTemp
                    };

                class TCMRError
                    {
                    public:
                        TCMRError(TInt aError, TErrorType aType) : iErrorCode(aError), iErrorType(aType) {};

                    public: //data
                        // Error code from the client to be passed
                        TInt iErrorCode;
                        // Error types (fatal/temporary)
                        TErrorType iErrorType;
                    };


            public: // Constructors and destructor
                /**
                * C++ default constructor.
                */
                CCMRErrorReporter(MCMRMediaRecorderObserver *aObserver);
                /**
                * Destructor.
                */
                ~CCMRErrorReporter();

            public: // New functions
                /**
                * Report fatal error to the observer
                */
                void FatalError(TInt aError);
                /**
                * Report temporary error to the observer
                */
                void TemporaryError(TInt aError);

            protected: // from baseclass

                /**
                * From CActive Does the required action (calls the observer)
                */
                void RunL();
                /**
                * From CActive Cancels pending actions
                */
                void DoCancel();

            private: // data
                // List of error codes from the client to be passed
                RArray<TCMRError> iErrors;
                // Observer for whom to report the error
                MCMRMediaRecorderObserver *iObserver;
                // ETrue if RunL of the active object is running => another AO can't change the contents of the object
                TBool iRunning;
            };

    public: // Functions from base classes
        
        /**
        * From CCMRMediaRecorder 
        */        
        void OpenL(MCMRMediaRecorderObserver *aObserver, 
                   MDataSource *aAudioSource, 
                   MCMRMediaSink *aSink, TInt aCameraHandle, 
                   const TDesC8& aVideoMimeType = KNullDesC8,  // video codec to use     
                   TFourCC aAudioType = KFourCCNULL); // audio codec to use
        
        /**
        * From CCMRMediaRecorder Return current state
        */ 
        inline TRecorderState State() { return iState; }
        
        // Settings
        
        /**
        * From CCMRMediaRecorder Set video codec to be used (overrides the one given in OpenL)
        */
        void SetVideoCodecL( const TDesC8& aVideoMimeType );

        /**
        * From CCMRMediaRecorder Get the used video codec
        */
        void GetVideoCodecL( TDes8& aVideoMimeType );

        /**
        * From CCMRMediaRecorder Get the supported & installed video codecs
        */
        void GetSupportedVideoCodecsL( CDesC8Array& aVideoMimeTypes );

        /**
        * From CCMRMediaRecorder Set audio codec to be used (overrides the one given in OpenL)
        */
        void SetAudioCodecL( const TFourCC& aAudioType );
        
        /**
        * From CCMRMediaRecorder Get the used audio codec
        */
        void GetAudioCodecL( TFourCC& aAudioType );
        
        /**
        * From CCMRMediaRecorder Get the supported & installed audio codecs
        */
        void GetSupportedAudioCodecsL( RArray<TFourCC>& aAudioTypes );

        /**
        * From CCMRMediaRecorder Set new maximum total (audio+video) bitrate
        */
        inline void SetMaxTotalBitRateL(TInt aBitRate) { iMaxTargetBitRate = aBitRate; }

        /**
        * From CCMRMediaRecorder Get current maximum total (audio+video) bitrate
        */
        inline TInt MaxTotalBitRateL() const { return iMaxTargetBitRate; }

        /**
        * From CCMRMediaRecorder Set new video frame rate
        */
        void SetVideoFrameRateL(TReal32 aFrameRate);
        
        /**
        * From CCMRMediaRecorder Get current video frame rate
        */
        TReal32 VideoFrameRateL();

        /**
        * From CCMRMediaRecorder Set new video frame size
        */
        void SetVideoFrameSizeL(const TSize& aSize);

        /**
        * From CCMRMediaRecorder Get current video frame size
        */
        void GetVideoFrameSizeL(TSize& aSize) const;

        /**
        * From CCMRMediaRecorder Set new target video bitrate
        */
        void SetVideoBitRateL(TInt aBitRate);

        /**
        * From CCMRMediaRecorder Get current target video bitrate
        */
        TInt VideoBitRateL();

        /**
        * From CCMRMediaRecorder Set misc video coding options
        */
        void SetVideoCodingOptionsL(const TCCMRVideoCodingOptions& aOptions);

        /**
        * From CCMRMediaRecorder Set video rate control options
        */
    	void SetVideoRateControlOptionsL(const TRateControlOptions& aOptions);        
        
        /**
        * From CCMRMediaRecorder Get video rate control options
        */
        void GetVideoRateControlOptionsL(TRateControlOptions& aOptions);        
    	
        /**
        * From CCMRMediaRecorder Set new target audio bitrate
        */
        void SetAudioBitRateL(TInt aBitRate);

        /**
        * From CCMRMediaRecorder Gets the current target audio bitrate  
        */
        TInt AudioBitRateL();

        /**
        * From CCMRMediaRecorder Toggles audio recording on/off
        */
        void SetAudioEnabledL(TBool aEnabled);

        /**
        * From CCMRMediaRecorder Query whether audio recording is enabled
        */
        TBool AudioEnabledL() const;                
        
        /**
        * From CCMRMediaRecorder Set new audio recording input gain
        */
        void SetGainL(TInt aGain);

        /**
        * From CCMRMediaRecorder Get current audio recording gain
        */
        TInt GainL() const;

        /**
        * From CCMRMediaRecorder Get maximum audio recording gain
        */
        TInt MaxGainL() const;   
        
        /**
        * From CCMRMediaRecorder Set audio priority settings
        */
        void SetAudioPriorityL(const TMMFPrioritySettings& aPrioritySettings);


        // Recording control
     
        /**
        * From CCMRMediaRecorder Prepares the recorder for recording
        */
        void PrepareL();

        /**
        * From CCMRMediaRecorder Starts recording audio and video
        */
        void RecordL();
      
        /**
        * From CCMRMediaRecorder Stops recording audio and video
        */
        void StopL();
       
        /**
        * From CCMRMediaRecorder Pauses recording
        */
        void PauseL();

        /**
        * From CCMRMediaRecorder Resumes recording
        */
        void ResumeL();

        /**
        * From CCMRMediaRecorder Called when video recorder state has changed
        */
        void MvroStateChange(CCMRRecorderBase::TRecorderState aState);

        /**
        * From CCMRMediaRecorder Called when an error has occurred in the video recorder
        */
        void MvroError(TInt aError);    


        /**
        * From MCMRAudioRecorderObserver Informs that audio recorder has changed its state
        */
        void MaroStateChange(CCMRRecorderBase::TRecorderState aState);

        /**
        * From MCMRAudioRecorderObserver Informs about error in audio recorder
        */
        void MaroError(TInt aError);

        /**
        * From MCMRActiveOutputObserver Informs about error in active output
        */
        void MaooError(TInt aError);

        // Video encoder overrides

        /**
        * Set video encoder using its UID. Usage optional.
        * This overrides Media Recorder internal search for encoder based on set video mime type ( SetVideoCodecL() ).
        *
        * @since 3.2.3
        * @param    "aEncoder"  "Video encoder UID."
        * @return void
        */
        void SetPreferredVideoEncoderL(TUid& aEncoder);

        /**
        * Set video encoder output format encapsulation. Usage optional.
        * This overrides Media Recorder internal default preferences for TVideoDataUnitEncapsulation with H.264/AVC video output.
        * if used encoder supports multiple encapsulations.
        *
        * Default for H.264 / AVC is EDuGenericPayload
        * Possible values: EDuGenericPayload or EDuElementaryStream
        *
        * @since 3.2.3
        * @param    "aCapsulation"  "Encapsulation for coded video data units."
        * @return void
        */
        void SetPreferredVideoEncapsulationL(TVideoDataUnitEncapsulation aCapsulation);
        
        /**
        * Set video encoder target segment size. Usage optional.
        * @since 5.2
        * @param  TUint aLayer              Layer number
        * @param  TUint aSizeBytes          Segment target size in bytes
        * @param  TUint aSizeMacroblocks    Segment target size in number of macroblocks per segment
        * @return TBool
        */            
        void SetSegmentTargetSizeL(TUint aLayer, TUint aSizeBytes, TUint aSizeMacroblocks );        

    private:  // Data

        // internal state
        TRecorderState iState;

        // observer for callbacks
        MCMRMediaRecorderObserver* iObserver;

        // media sink, e.g. 3GP file composer
        MCMRMediaSink* iSink;

        // active objects taking care of video and audio output
        CCMRActiveOutput* iAudioOutput;
        CCMRActiveOutput* iVideoOutput;

        // MMF audio input, given as parameter
        MDataSource *iAudioSource;

        // video recorder
        CCMRVideoRecorderClient* iVideoRecorder; 

        // audio recorder
        CCMRAudioRecorder* iAudioRecorder; 

        // is video recording being prepared ?
        TBool iVideoRecorderPreparing;  

        // is audio recording being prepared ?
        TBool iAudioRecorderPreparing;  

        // audio recording flag (on/off)
        TBool iAudioEnabled;  

        // Max bitrate, -1 if not set (=> won't be checked when setting A/V bitrates)       
        TInt iMaxTargetBitRate;  
        // target bitrate, used only to store the actual value when asked from audio/videorecorder
        TInt iTargetBitRate;
        // video frame rate, used only to store the actual value when asked from videorecorder
        TReal32 iFrameRate;

        // Error code saved while waiting for a completion of an async operation
        TInt iErrorCode;

        // Flags used when waiting async stoppings to complete
        TBool iVideoStopped;
        TBool iAudioStopped;
        
        // Status for EOS / streamend marker
        TInt iVideoEOSReached;

        // Active object to report errors to the observer. Needed to break the callback chain since the observer may call e.g. MR::Stop in the callback
        CCMRErrorReporter* iErrorReporter;

        // Performance monitor object
        CCMRPerfMonitor *iPerfMonitor;
        
        // Mediarecorder / CamcorderMMFPlugin config manager.
        CCMRConfigManager* iConfig;
    };


#endif // CCMRMEDIARECORDERIMP_H

// End of file

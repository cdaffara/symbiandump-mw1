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
* Description:  Header file for video recorder client class
*
*/


#ifndef CCMRVIDEORECORDERCLIENT_H
#define CCMRVIDEORECORDERCLIENT_H

// INCLUDES
#include "CCMRRecorderBase.h"
#include "CCMRVideoThreadProxy.h"

// FORWARD DECLARATION
class CCMRVideoCodecs;
class CCMRActiveOutput;
class TCCMRVideoCodingOptions;
class CDesC8Array;
class MMMFClockSource;
class TRateControlOptions;

// CLASS DECLARATION

/**
*  Video recorder observer interface  
*
*  @lib CAMCMEDIARECORDER.LIB
*  @since 2.1
*/
class MCMRVideoRecorderObserver
    {
    public:

        /**
        * Informs about a state change
        * @since 2.1
        * @param aState New state
        * @return void
        */
        virtual void MvroStateChange(CCMRRecorderBase::TRecorderState aState) = 0;

        /**
        * Informs about an error
        * @since 2.1
        * @param aError Error code
        * @return void
        */
        virtual void MvroError(TInt aError) = 0;
    };


// CLASS DECLARATION

/**
*  Video recorder client class
*
*  @lib CAMCMEDIARECORDER.LIB
*  @since 2.1
*/
class CCMRVideoRecorderClient : public CCMRRecorderBase, public MCMRSubThreadEventMonitorObserver
                          

    {
    public:
        // Video bit-rate control mode
        enum TCMRVideoBitRateMode
            {
            EBitRateConstant,
            EBitRateVariable
            };

    public:  // Constructors and destructor
        
        /**
        * C++ default constructor.
        */
        inline CCMRVideoRecorderClient() {};
                        
        /**
        * Two-phased constructor.
        */
        
        static CCMRVideoRecorderClient* NewL();

        /**
        * Destructor.
        */

        virtual ~CCMRVideoRecorderClient();               

    public:   // Constants        

        enum TErrorCode
            {
            EInternalAssertionFailure = -10040,
            };
        
    public: // New functions
        
        /**
        * Open the video recorder
        * @since 2.1
        * @param aObserver      Observer instance
        * @param aOutput        Output active object
        * @param aCameraHandle  Camera handle (index)
        * @param aVideoMimeType Video Mime-type
        * @param aClockSource   Clock source from audio timestamps
        * @return void
        */
        void OpenL(MCMRVideoRecorderObserver* aObserver, CCMRActiveOutput* aOutput, 
            TInt aCameraHandle, const TDesC8& aVideoMimeType, MMMFClockSource* aClockSource, CCMRConfigManager* aConfig );
                
        /**
        * Sets video Mime type (codec)
        * @since 2.1
        * @param aMimeType   video codec Mime type
        * @return void
        */
        void SetVideoCodecL(const TDesC8& aMimeType) const;

        /**
        * Get the currently used video codec
        * @since 2.1
        * @param  aVideoMimeType    Used codec as mime type
        * @return TInt Error code
        */
        void GetVideoCodecL( TDes8& aVideoMimeType ) const;

        /**
        * Get supported & installed video codecs.
        * This can be called also when the recorder is not open
        * @since 2.1
        * @param  aVideoMimeTypes   List of supported & installed video codecs as mime type
        * @return void
        */
        void GetSupportedVideoCodecsL( CDesC8Array& aVideoMimeTypes ) const;

        /**
        * Sets new input & output frame size
        * @since 2.1
        * @param aSize New size
        * @return void
        */
        void SetFrameSizeL(const TSize& aSize) const;
        
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
        void SetFrameRateL(TReal32 aFrameRate) const;
        
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
        void SetVideoCodingOptionsL(const TCCMRVideoCodingOptions& aOptions) const;

        /**
        * Set video rate control options
        * @since 3.2
        * @param  aOptions      video rate control options
        * @return void
        */        
        void SetVideoRateControlOptionsL(const TRateControlOptions& aOptions) const;        

        /**
        * Get video rate control options
        * @since 3.2
        * @param  aOptions      video rate control options
        * @return void
        */        
        void GetVideoRateControlOptionsL(TRateControlOptions& aOptions) const;               
        
        /**
        * Adjust time stamps of video
        * @since 2.1
        * @param  const TInt aAdjustmentMs time in ms, can be positive or negative
        * @return void
        */
        void AdjustTimeStampsL(const TInt aAdjustmentMs) const;

        /**
        * Set video thread priority
        * @since 2.8
        * @param  const TThreadPriority& aAudioThreadPriority
        * @return void
        */
        void SetThreadPriorityL(const TThreadPriority& aAudioThreadPriority) const;

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
        * Set video encoder target segment size. Usage optional.
        * @since 5.2
        * @param  TUint aLayer              Layer number
        * @param  TUint aSizeBytes          Segment target size in bytes
        * @param  TUint aSizeMacroblocks    Segment target size in number of macroblocks per segment
        * @return TBool
        */            
        void SetSegmentTargetSizeL(TUint aLayer, TUint aSizeBytes, TUint aSizeMacroblocks );        

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
        * From CCMRRecorderBase Stop recording (async => must wait for state change)
        */
        void StopL();
        
        /**
        * Used while stopping. Requests encoded video frames from adaptation and waits for the streamend / EOS notification.
        */
        void RequestBuffersAndWaitEOSL(TInt& aVideoEOSReached);          

        /**
        * From CCMRRecorderBase Pause recording
        */
        void PauseL();

        /**
        * From CCMRRecorderBase Resume recording
        */
        void ResumeL();     

        /**
        * From MCMRSubThreadEventMonitorObserver Handle event
        */
        void HandleEvent(const TMMFEvent& aEvent);

    private:
        
        /**
        * By default EPOC constructor is private.
        */
        void ConstructL();  
        

    private:    // Data                 



        // level of video, e.g. H.263 level 10
        TInt iVideoCodecLevel;

        // the observer for the video recorder
        MCMRVideoRecorderObserver* iObserver;  

        // video thread
        RCMRVideoThreadProxy iThreadProxy;

        // ETrue if iThreadProxy was opened => can be closed
        TBool iThreadCreated;

        // Thread event monitor
        CCMRSubThreadEventMonitor* iThreadEventMonitor;

        // Maintains list of supported & installed video codecs
        CCMRVideoCodecs* iVideoCodecs;

    };  
    
#endif      // CCMRVIDEORECORDERCLIENT_H
    
// End of File
    
    
    
    
    

    
    
    

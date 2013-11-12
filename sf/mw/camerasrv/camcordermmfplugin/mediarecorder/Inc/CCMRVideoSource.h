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
* Description:  Interface definition for a video source
*                this is "hiding" the Symbian on-board camera API, included
*                so that each and every definition doesn't need to be duplicated here
*
*/


#ifndef CCMRVIDEOSOURCE_H
#define CCMRVIDEOSOURCE_H

// INCLUDES
#include <ecam.h>



// CLASS DECLARATION

/**
*  Video source observer interface  
*
*  @lib CAMCMEDIARECORDER.LIB
*  @since 2.1
*/
class MCMRVideoSourceObserver
    {
    public:
    
        /**
        * Camera reservation completed
        * @since 2.1
        * @param TInt aError
        * @return void
        */
        virtual void MvsoReserveComplete(TInt aError) = 0;
        /**
        * Frame buffer from camera
        * @since 2.1
        * @param MFrameBuffer* aFrameBuffer
        * @param TInt aError
        * @return void
        */
        virtual void MvsoFrameBufferReady(MFrameBuffer* aFrameBuffer,TInt aError ) = 0;        
    };


// CLASS DECLARATION

/**
*  Abstract video source interface
*
*  @lib CAMCMEDIARECORDER.LIB
*  @since 2.1
*/

class MCMRVideoSource
    {

    public:  // Constants
         
        // States the source can be in
        enum TVideoSourceState
            {
                EStateNone = 0,
                EStateReady,   // == stopped
                EStateCapturing
            };


    public:  // New functions

        /**
        * Gets info from the camera
        * @since 2.1
        * @param TCameraInfo Info (See Ecam.h)
        * @return void
        */
        virtual void CameraInfo(TCameraInfo& aInfo) = 0;

        /**
        * Returns the current output frame size
        * @since 2.1
        * @param aSize Output parameter for the size
        * @return void        
        */
        virtual void GetFrameSize(TSize& aSize) = 0;

        /**
        * Returns the current output frame rate
        * @since 2.1
        * @param void
        * @return TReal32 Frame rate
        */
        virtual TReal32 FrameRate() = 0;

        /**
        * Returns the number of buffers the source is using
        * @since 2.1
        * @param void
        * @return TInt Number of buffers in use
        */
        virtual TInt BuffersInUse() = 0;        

        /**
        * Returns the number of frames in one buffer
        * @since 2.1
        * @param void
        * @return TInt The number of frames in one buffer
        */
        virtual TInt FramesPerBuffer() = 0;

        /**
        * Queries whether video capture is active
        * @since 2.1
        * @param void
        * @return TBool ETrue if capture is active
        */
        virtual TBool CaptureActive() = 0;

        /**
        * Reserves the source for exclusive use
        * @since 2.1
        * @param void
        * @return void
        */
        virtual void Reserve() = 0;

        /**
        * Releases the source for others to use it
        * @since 2.1
        * @param void
        * @return void
        */
        virtual void Release() = 0;             
        
        /**
        * Enumerates through available video frame sizes using the aSizeIndex -parameter
        * @since 2.1
        * @param aSize Output parameter for the enumerated size
        * @param aSizeIndex Size index, range 0 <= aSizeIndex <= cameraInfo.NumFrameSizesSupported - 1)
        * @param aFormat Output video format to be used
        * @return void
        */
        virtual void EnumerateVideoFrameSizes(TSize& aSize,TInt aSizeIndex,
            CCamera::TFormat aFormat) const = 0;

        /**
        * Enumerates through available video frame rates using the aRateIndex -parameter
        * @since 2.1
        * @param aRate Output parameter for the enumerated rate
        * @param aRateIndex Rate index, range 0 <= aRateIndex <= cameraInfo.NumFrameRatesSupported - 1)
        * @param aFormat Output video format to be used
        * @param aSizeIndex Index of the frame size to be used (from EnumerateVideoFrameSizes)
        * @param aExposure Exposure mode to be used (See Ecam.h)
        * @return void
        */
        virtual void EnumerateVideoFrameRates(TReal32& aRate,TInt aRateIndex,
            CCamera::TFormat aFormat, TInt aSizeIndex, CCamera::TExposure aExposure) const = 0;     
        
        /**
        * Prepares the source for capturing video
        * @since 2.1
        * @param aFormat Output video format to be used
        * @param aSizeIndex Index of the frame size to be used (from EnumerateVideoFrameSizes)
        * @param aRateIndex Index of the frame rate to be used (from EnumerateVideoFrameRates)
        * @param aBuffersToUse Number of frame buffers to use
        * @param aFramesPerBuffer Number of frames in one buffer
        * @return void
        */
        virtual void PrepareCaptureL(CCamera::TFormat aFormat,TInt aSizeIndex,
            TInt aRateIndex,TInt aBuffersToUse,TInt aFramesPerBuffer)=0;
        
        /**
        * Prepares the source for capturing video
        * @since 2.1
        * @param aFormat Output video format to be used
        * @param aSizeIndex Index of the frame size to be used (from EnumerateVideoFrameSizes)
        * @param aRateIndex Index of the frame rate to be used (from EnumerateVideoFrameRates)
        * @param aBuffersToUse Number of frame buffers to use
        * @param aFramesPerBuffer Number of frames in one buffer
        * @param aClipRect The frames will be clipped to the given rectangle        
        * @return void
        */
        virtual void PrepareCaptureL(CCamera::TFormat aFormat, TInt aSizeIndex,
            TInt aRateIndex,TInt aBuffersToUse,TInt aFramesPerBuffer,
            const TRect& aClipRect) = 0;
        
        /**
        * Starts capturing video
        * @since 2.1
        * @param void
        * @return void
        */
        virtual void StartCapture() = 0;        

        /**
        * Stops capturing video
        * @since 2.1
        * @param void
        * @return void
        */
        virtual void StopCapture() = 0;            

        /**
        * Get current exposure setting
        * @since 2.1
        * @param void
        * @return TExposure Current exposure setting
        */
        virtual CCamera::TExposure GetExposure() const = 0;
    };


#endif // CCMRVIDEOSOURCE_H

// End of file

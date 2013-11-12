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
* Description:  Interface for video camera source using the Symbian onboard camera API                 
*
*/


#ifndef CCMRVIDEOCAMERASOURCE_H
#define CCMRVIDEOCAMERASOURCE_H

//  INCLUDES
#include <ecam.h>  // Symbian onboard camera API header
#include "CCMRVideoSource.h"

// CLASS DECLARATION

/**
*  Symbian camera API wrapper
*
*  @lib CAMCMEDIARECORDER.LIB
*  @since 2.1
*/
class CCMRVideoCameraSource : public CBase, public MCMRVideoSource, public MCameraObserver, public MCameraObserver2
    {
    
    public:  // Constructors and destructor
        
        /**
        * C++ default constructor.
        */
        CCMRVideoCameraSource(MCMRVideoSourceObserver* aObserver, TInt aCameraHandle);        
        
        /**
        * Two-phased constructor.
        */
        
        static CCMRVideoCameraSource* NewL(MCMRVideoSourceObserver* aObserver, TInt aCameraHandle);
                                  
        /**
        * Destructor.
        */
        ~CCMRVideoCameraSource();           

    public:  // Constants


    public: // New functions
        
        /**
        * Returns the current state of the camera source
        * @since 2.1
        * @param void 
        * @return TVideoSourceState State (from CCMRVideoSource.h)
        */
        TVideoSourceState State();
        
    public: // Functions from base classes               

        /**
        * From MCMRVideoSource Get camera info        
        */
        void CameraInfo(TCameraInfo& aInfo);
        
        /**
        * From MCMRVideoSource Get frame size        
        */      
        void GetFrameSize(TSize& aSize);
        
        /**
        * From MCMRVideoSource Get frame rate        
        */
        TReal32 FrameRate();
        
        /**
        * From MCMRVideoSource Get no. of frame buffers in use                
        */
        TInt BuffersInUse();
        
        /**
        * From MCMRVideoSource Get no. of frames in one buffer
        */
        TInt FramesPerBuffer();
        
        /**
        * From MCMRVideoSource Query whether capture is active        
        */       
        TBool CaptureActive();
        
        /**
        * From MCMRVideoSource Reserve camera                
        */
        void Reserve();
        
        /**
        * From MCMRVideoSource Release camera       
        */
        void Release();               
        
        /**
        * From MCMRVideoSource Enumarates frame sizes
        */
        void EnumerateVideoFrameSizes(TSize& aSize,TInt aSizeIndex,
            CCamera::TFormat aFormat) const;
        
        /**
        * From MCMRVideoSource Enumarates frame rates        
        */
        void EnumerateVideoFrameRates(TReal32& aRate,TInt aRateIndex,CCamera::TFormat aFormat, 
            TInt aSizeIndex, CCamera::TExposure aExposure) const;       
        
        /**
        * From MCMRVideoSource Prepares camera for capturing
        */
        void PrepareCaptureL(CCamera::TFormat aFormat,TInt aSizeIndex,
            TInt aRateIndex,TInt aBuffersToUse,TInt aFramesPerBuffer);
                
        /**
        * From MCMRVideoSource Prepares camera for capturing
        */
        void PrepareCaptureL(CCamera::TFormat aFormat, TInt aSizeIndex,
            TInt aRateIndex,TInt aBuffersToUse,TInt aFramesPerBuffer,
            const TRect& aClipRect);
        
        /**
        * From MCMRVideoSource Starts capturing
        */
        void StartCapture();
        
        /**
        * From MCMRVideoSource Stops capturing
        */
        void StopCapture();           

        /**
        * From MCMRVideoSource Get current exposure setting
        */
        CCamera::TExposure GetExposure() const;

        /**
        * From MCameraObserver Informs that reserve has completed
        */
        void ReserveComplete(TInt aError);

        /**
        * From MCameraObserver Informs that frame buffer is ready
        */
        void FrameBufferReady(MFrameBuffer* aFrameBuffer,TInt aError);

        /**
        * From MCameraObserver Informs that Power on has completed
        */
        void PowerOnComplete( TInt aError );

        /**
        * From MCameraObserver Informs that Viewfinder frame is ready => not applicable..
        */
        inline void ViewFinderFrameReady( CFbsBitmap& /*aFrame*/ )
            { User::Panic(_L("CCMRVIDEOCAMERASOURCE"), KErrNotSupported); }

        /**
        * From MCameraObserver Informs that still image is ready => not applicable..                
        */
        inline void ImageReady( CFbsBitmap* /*aBitmap*/,HBufC8* /*aData*/,TInt /*aError*/ ) 
            { User::Panic(_L("CCMRVIDEOCAMERASOURCE"), KErrNotSupported); }

        /**
        * From MCameraObserver2 Informs that client should handle a event
        */
        void HandleEvent(const TECAMEvent& aEvent);

        /**
        * From MCameraObserver2 Informs that viewfinder frame is ready
        */
        void ViewFinderReady(MCameraBuffer& aCameraBuffer,TInt aError);

        /**
        * From MCameraObserver2 Informs that a new captured image is ready
        */
        void ImageBufferReady(MCameraBuffer& aCameraBuffer,TInt aError);
    
        /**
        * From MCameraObserver2 Informs that a new captured video is ready
        */
        void VideoBufferReady(MCameraBuffer& aCameraBuffer,TInt aError);

    private:  // New functions
        
        /**
        * By default EPOC constructor is private.
        */
        void ConstructL();      
               
    private:    // Data

        // internal state
        TVideoSourceState iState;  
        // is the camera reserved for us?
        TBool iReserved;  
        // the observer
        MCMRVideoSourceObserver* iObserver;                                      
        // camera index
        TInt iCameraHandle; 
        // the Camera
        CCamera* iCamera;   
        // ETrue if the camera was opened as duplicate using a given handle
        TBool iDuplicateInstance;
    };


#endif // CCMRVIDEOCAMERASOURCE_H

// End of file
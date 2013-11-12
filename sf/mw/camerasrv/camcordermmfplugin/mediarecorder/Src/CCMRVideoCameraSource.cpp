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
* Description:  Implementation for camera API wrapper
*
*/


// INCLUDE FILES
#include    "CCMRVideoCameraSource.h"
#include    "CCMRFrameBuffer.h"

#ifdef _DEBUG
#include <e32svr.h>
#define PRINT(x) RDebug::Print x
#else
#define PRINT(x)
#endif

#include "OstTraceDefinitions.h"
#ifdef OST_TRACE_COMPILER_IN_USE
#include "CCMRVideoCameraSourceTraces.h"
#endif



// ================= MEMBER FUNCTIONS =======================




// ---------------------------------------------------------
// CCMRVideoCameraSource::CCMRVideoCameraSource
// C++ default constructor can NOT contain any code, that
// might leave.
// ---------------------------------------------------------
//
CCMRVideoCameraSource::CCMRVideoCameraSource(MCMRVideoSourceObserver* aObserver, TInt aCameraHandle)
    {
    
    iObserver = aObserver;
    iCameraHandle = aCameraHandle;
    // zero-initialize all variables

    iCamera = NULL;
    }


// ---------------------------------------------------------
// CCMRVideoCameraSource::ConstructL()
// Symbian 2nd phase constructor can leave.
// ---------------------------------------------------------
//
void CCMRVideoCameraSource::ConstructL()
    {
    
    iReserved = EFalse;

    if ( iCameraHandle == 0 ) 
        {
        // camera handle not available, should only happen in development phase?
        // Create a new Camera API implementation object, if supported
        TRAPD( err, iCamera = CCamera::New2L( static_cast<MCameraObserver2&>(*this), 0 /*index*/, 0 /*Priority*/ ) );
        if ( err )
            {
            PRINT( ( _L( "CCMRVideoCameraSource::ConstructL() CCamera::New2L return code=%d" ), err ) ); 

            // Create old Camera API implementation object.
            iCamera = CCamera::NewL( static_cast<MCameraObserver&>(*this), 0 /*Index*/ );
            }
        else
            {
            PRINT( ( _L( "CCMRVideoCameraSource::ConstructL() using new MCameraObserver2" )) ); 
            }
        iDuplicateInstance = EFalse;
        }
    else
        {
        // Normal case, duplicate camera instance
        // Create a new Camera API duplicate object, if supported
        TRAPD( err, iCamera = CCamera::NewDuplicate2L( static_cast<MCameraObserver2&>(*this), iCameraHandle ) );
        if ( err )
            {
            PRINT( ( _L( "CCMRVideoCameraSource::ConstructL() CCamera::NewDuplicate2L return code=%d" ), err ) ); 

            // Create old Camera API duplicate object.
            iCamera = CCamera::NewDuplicateL( static_cast<MCameraObserver&>(*this), iCameraHandle );
            }
        else
            {
            PRINT( ( _L( "CCMRVideoCameraSource::ConstructL() using duplicated MCameraObserver2" )) ); 
            }
        iDuplicateInstance = ETrue;
        }
   
    iState = EStateNone;

    }

// -----------------------------------------------------------------------------
// CCMRVideoCameraSource::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CCMRVideoCameraSource* CCMRVideoCameraSource::NewL(MCMRVideoSourceObserver* aObserver, TInt aCameraHandle)
    {
    CCMRVideoCameraSource* self = new (ELeave) CCMRVideoCameraSource(aObserver, aCameraHandle);
    
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop();

    return self;
    }


// ---------------------------------------------------------
// CCMRVideoCameraSource::~CCMRVideoCameraSource()
// Destructor
// ---------------------------------------------------------
//
CCMRVideoCameraSource::~CCMRVideoCameraSource()
    {   
    OstTrace0( CAMERASRV_PERFORMANCE, CCMRVIDEOCAMERASOURCE_CCMRVIDEOCAMERASOURCE, "e_CCMRVideoCameraSource::~CCMRVideoCameraSource 1" );
    // verify that camera has been released
    if ( iReserved && iCamera )
        {
        iCamera->Release();
        }

    iObserver = NULL;
    delete iCamera;
    OstTrace0( CAMERASRV_PERFORMANCE, DUP1_CCMRVIDEOCAMERASOURCE_CCMRVIDEOCAMERASOURCE, "e_CCMRVideoCameraSource::~CCMRVideoCameraSource 0" );
    }

// ---------------------------------------------------------
// CCMRVideoCameraSource::GetFrameSize
// Get frame size from camera
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CCMRVideoCameraSource::GetFrameSize(TSize& aSize)
   {
   iCamera->GetFrameSize(aSize);
   }

// ---------------------------------------------------------
// CCMRVideoCameraSource::CameraInfo
// Get camera info
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CCMRVideoCameraSource::CameraInfo(TCameraInfo& aInfo)
    {
    iCamera->CameraInfo(aInfo);
    }

// ---------------------------------------------------------
// CCMRVideoCameraSource::FrameRate
// Get frame rate from camera
// (other items were commented in a header).
// ---------------------------------------------------------
//
TReal32 CCMRVideoCameraSource::FrameRate()
    {
    return iCamera->FrameRate();
    }

// ---------------------------------------------------------
// CCMRVideoCameraSource::BuffersInUse
// Get number of buffers used by camera
// (other items were commented in a header).
// ---------------------------------------------------------
//
TInt CCMRVideoCameraSource::BuffersInUse()
   {
   return iCamera->BuffersInUse();
   }

// ---------------------------------------------------------
// CCMRVideoCameraSource::FramesPerBuffer
// Get number of frames per buffer used by camera
// (other items were commented in a header).
// ---------------------------------------------------------
//
TInt CCMRVideoCameraSource::FramesPerBuffer()
   {
   return iCamera->FramesPerBuffer();
   }

// ---------------------------------------------------------
// CCMRVideoCameraSource::ReserveComplete
// Camera observer callback informs reserve was completed
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CCMRVideoCameraSource::ReserveComplete(TInt aError)
   {   
   iReserved = ETrue;
   if ( iDuplicateInstance || (aError != KErrNone) )
       {
       // no need to switch power on
       iObserver->MvsoReserveComplete(aError);
       }
   else 
       {
       // we have the main instance so we must also switch the power on
       // this hides the poweron from the user and combines reserve & poweron
       iCamera->PowerOn();
       }
   }

// ---------------------------------------------------------
// CCMRVideoCameraSource::PowerOnComplete
// Camera observer callback informs poweron was completed
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CCMRVideoCameraSource::PowerOnComplete(TInt aError)
    {
    // we hide the poweron from the user and combine reserve & poweron
    iObserver->MvsoReserveComplete(aError);
    }

// ---------------------------------------------------------
// CCMRVideoCameraSource::FrameBufferReady
// Camera observer callback informs a video buffer is ready
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CCMRVideoCameraSource::FrameBufferReady(MFrameBuffer* aFrameBuffer,TInt aError)
   {
   iObserver->MvsoFrameBufferReady(aFrameBuffer, aError);
   }

// ---------------------------------------------------------
// CCMRVideoCameraSource::Reserve
// Reservers camera, result is informed in callback
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CCMRVideoCameraSource::Reserve()
   {

   if (iReserved) 
       {
       // reserved already, call observer to indicate to caller that source is ready
       iObserver->MvsoReserveComplete( KErrNone );
       return;
       }

   iCamera->Reserve();

   }

// ---------------------------------------------------------
// CCMRVideoCameraSource::Release
// Release camera for other use
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CCMRVideoCameraSource::Release()
   {   
   iCamera->Release();
   }

// ---------------------------------------------------------
// CCMRVideoCameraSource::EnumerateVideoFrameSizes
// Asks camera to enumerate video frame sizes
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CCMRVideoCameraSource::EnumerateVideoFrameSizes(TSize& aSize,TInt aSizeIndex,
                                                     CCamera::TFormat aFormat) const
   {

   iCamera->EnumerateVideoFrameSizes(aSize, aSizeIndex, aFormat);

   }

// ---------------------------------------------------------
// CCMRVideoCameraSource::EnumerateVideoFrameRates
// Asks camera to enumerate video frame rates
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CCMRVideoCameraSource::EnumerateVideoFrameRates(TReal32& aRate,TInt aRateIndex, CCamera::TFormat aFormat, 
                                                     TInt aSizeIndex, CCamera::TExposure /*aExposure*/) const
   {
 
   iCamera->EnumerateVideoFrameRates(aRate, aRateIndex, aFormat, aSizeIndex /* Exposure == Auto */);

   }



// ---------------------------------------------------------
// CCMRVideoCameraSource::PrepareCaptureL
// Prepares camera for video capture
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CCMRVideoCameraSource::PrepareCaptureL(CCamera::TFormat aFormat,TInt aSizeIndex,
                                          TInt aRateIndex,TInt aBuffersToUse, TInt aFramesPerBuffer)
   {

   // NOTE: Trap this in video recorder !!!
   iCamera->PrepareVideoCaptureL(aFormat, aSizeIndex, aRateIndex, aBuffersToUse, aFramesPerBuffer);   

   iState = EStateReady;

   }



// ---------------------------------------------------------
// CCMRVideoCameraSource::PrepareCaptureL
// Prepares camera for video capture
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CCMRVideoCameraSource::PrepareCaptureL(CCamera::TFormat aFormat, TInt aSizeIndex, TInt aRateIndex,
                                            TInt aBuffersToUse,TInt aFramesPerBuffer,const TRect& aClipRect)        
   {

   // NOTE: Trap this in video recorder !!!
   iCamera->PrepareVideoCaptureL(aFormat, aSizeIndex, aRateIndex, aBuffersToUse, aFramesPerBuffer, aClipRect);

   iState = EStateReady;

   }

// ---------------------------------------------------------
// CCMRVideoCameraSource::StartCapture
// Start video capture from camera
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CCMRVideoCameraSource::StartCapture()
   {
   iCamera->StartVideoCapture();
   iState = EStateCapturing;
   }

// ---------------------------------------------------------
// CCMRVideoCameraSource::StopCapture
// Stop video capture from camera
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CCMRVideoCameraSource::StopCapture()
   {
   iCamera->StopVideoCapture();
   iState = EStateReady;
   }

// ---------------------------------------------------------
// CCMRVideoCameraSource::StopCapture
// Check if capture is active
// (other items were commented in a header).
// ---------------------------------------------------------
//
TBool CCMRVideoCameraSource::CaptureActive()
   {
   return iCamera->VideoCaptureActive();
   }

// ---------------------------------------------------------
// Get current exposure setting
//
// ---------------------------------------------------------
//
CCamera::TExposure CCMRVideoCameraSource::GetExposure() const
    {
    return iCamera->Exposure();
    }

// -----------------------------------------------------------------------------
// CCMRVideoCameraSource::HandleEvent
// MCameraObserver2 call-back handler
// -----------------------------------------------------------------------------
//
void CCMRVideoCameraSource::HandleEvent( const TECAMEvent& aEvent)
    {
    PRINT(( _L( "CCMRVideoCameraSource::HandleEvent() entering, type=%x, err=%d" ), aEvent.iEventType.iUid, aEvent.iErrorCode ));

    if ( aEvent.iEventType == KUidECamEventCameraNoLongerReserved )
        {
        PRINT(( _L( "CCMRVideoCameraSource::HandleEvent() KUidECamEventCameraNoLongerReserved" ) ));
        }
    else if ( aEvent.iEventType == KUidECamEventPowerOnComplete )
        {
        PRINT(( _L( "CCMRVideoCameraSource::HandleEvent() KUidECamEventPowerOnComplete" ) ));
        PowerOnComplete( aEvent.iErrorCode );
        }
    else if ( aEvent.iEventType == KUidECamEventReserveComplete )
       {
       PRINT(( _L( "CCMRVideoCameraSource::HandleEvent() KUidECamEventReserveComplete" ) ));
       ReserveComplete( aEvent.iErrorCode );
       }

    PRINT(( _L( "CCMRVideoCameraSource::HandleEvent() returning" ) ));
    }

// -----------------------------------------------------------------------------
// CCMRVideoCameraSource::ViewFinderReady
// MCameraObserver2 call-back handler
// -----------------------------------------------------------------------------
//
void CCMRVideoCameraSource::ViewFinderReady( MCameraBuffer& aCameraBuffer, TInt aError)
    {
    PRINT(( _L( "CCMRVideoCameraSource::ViewFinderReady() NOT HANDLED err=%d, NumFrames=%d" ), aError, aCameraBuffer.NumFrames() ));

    if ( !aError )
        {
	    aCameraBuffer.Release();
        }
    }

// -----------------------------------------------------------------------------
// CCMRVideoCameraSource::ImageBufferReady
// MCameraObserver2 call-back handler
// -----------------------------------------------------------------------------
//
void CCMRVideoCameraSource::ImageBufferReady( MCameraBuffer& aCameraBuffer, TInt aError)
    {
    PRINT(( _L( "CCMRVideoCameraSource::ImageBufferReady() NOT HANDLED, err=%d, NumFrames=%d" ), aError, aCameraBuffer.NumFrames() ));
	
    if ( !aError )
        {
	    aCameraBuffer.Release();
        }
    }
    
// -----------------------------------------------------------------------------
// CCMRVideoCameraSource::VideoBufferReady
// MCameraObserver2 call-back handler
// -----------------------------------------------------------------------------
//
void CCMRVideoCameraSource::VideoBufferReady( MCameraBuffer& aCameraBuffer, TInt aError)
    {
    PRINT(( _L( "CCMRVideoCameraSource::VideoBufferReady() entering, err=%d, NumFrames=%d" ), aError, aCameraBuffer.NumFrames() ));
    CCMRFrameBuffer* buffer = NULL;
	
    if ( !aError )
        {
        // Create a new wrapper class to make conversion from MCameraBuffer to MFrameBuffer
        
        TRAPD( err, buffer = CCMRFrameBuffer::NewL( &aCameraBuffer ));
        if ( err )
            {
            PRINT(( _L( "CCMRVideoCameraSource::VideoBufferReady() CCMRFrameBuffer::NewL failed, err=%d" ), err ));
            aCameraBuffer.Release();
            aError = err;
            }
        }
    // Call old Camera-API observer handler
    FrameBufferReady( buffer, aError );
    
    PRINT(( _L( "CCMRVideoCameraSource::VideoBufferReady() returning" )));
    }



//  End of File  

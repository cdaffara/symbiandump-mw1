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
* Description:  Camcorder MMF API Test DLL
*
*/



// INCLUDE FILES

#include "CamcTestClient_7.h"
#include "camctest.h"
#include <AudioPreference.h>

// EXTERNAL DATA STRUCTURES

// EXTERNAL FUNCTION PROTOTYPES 
 
// CONSTANTS

// MACROS

#ifdef _DEBUG
#include <e32svr.h>
#define PRINT(x) RDebug::Print x
#else
#define PRINT(x)
#endif

// LOCAL CONSTANTS AND MACROS

// MODULE DATA STRUCTURES

// LOCAL FUNCTION PROTOTYPES

// FORWARD DECLARATIONS



CCamcTestClient_7::CCamcTestClient_7 () : CTimer(CActive::EPriorityHigh)
    {
    }


CCamcTestClient_7::~CCamcTestClient_7 ()
    {
    Cancel();
    
    delete iWaitScheduler;
    delete iCamc;   
    iCamc = NULL;
    delete iCamera;
    iCamera= NULL;
    delete iCamera2;
    iCamera2= NULL;
    }

CCamcTestClient_7* CCamcTestClient_7::NewL()
    {
    CCamcTestClient_7* self = new(ELeave) CCamcTestClient_7;
    CleanupStack::PushL(self);
    self->ConstructL( );
    CleanupStack::Pop(self);
    return self;
    }

void CCamcTestClient_7::ConstructL( )
    {
    iCamc = CVideoRecorderUtility::NewL( *this, KAudioPriorityVideoRecording, EMdaPriorityPreferenceTimeAndQuality );
    // It takes some time to free the camera.
    // Wait that it is free.
    TInt err;
    do
        {
        // Create a new Camera API implementation object, if supported
        TRAP( err, iCamera = CCamera::New2L( static_cast<MCameraObserver2&>(*this), 0 /*index*/, 0 /*Priority*/ ) );
        if ( err == KErrNotSupported )
            {
            PRINT( ( _L( "CCamcTestClient_7::ConstructL() CCamera::New2L return code=%d" ), err ) ); 

            // Create old Camera API implementation object.
            TRAP( err, iCamera = CCamera::NewL( static_cast<MCameraObserver&>(*this), 0 /*Index*/ ) );
            }
        else if ( !err )
            {
            PRINT( ( _L( "CCamcTestClient_7::ConstructL() using new MCameraObserver2" )) ); 
            }
        }
        while (err != KErrNone);
    iOpenReady = EFalse;
    iPrepareReady = EFalse;
    iRecordingReady = EFalse;
    iPaused = EFalse;
    
    CTimer::ConstructL();
    iTimeout = 0;
    iClosed = 0;
    CActiveScheduler::Add(this);
    }


void CCamcTestClient_7::Start_Active_ObjectL ( TestClient_7_Actions aAction )
    {
    
    // This reserve the camera, starts the active object.

    iAction = aAction;    //iAction might be set to NULL after action has been executed
    //	iTimeout = aTimeout;
    iTimeout = (TInt) 1E6; /* 1 Second */
    
    iWaitScheduler = new (ELeave) CActiveSchedulerWait;	
    // Reserve the Camera.
    iCamera->Reserve();
	iWaitScheduler->Start();
    
    RDebug::Print(_L("CamcTest_7::Start_Active_ObjectL PowerOn Completed starting CActiveScheduler") );
    CActiveScheduler::Start();
    

    // The Active Scheduler has finished.
    // Any Errors?

    if (iError)
        {
        RDebug::Print(_L("CamcTest_7::Start_Active_ObjectL leaves with %d"), iError);
        User::Leave(iError);
        }
    }


void CCamcTestClient_7::RunL()
    {
    
    // Trap RunL, to catch if the AO leaves.
    // A Walkaround...
    // RunError() should be used in this case, but
    // Apparently, it is not possible to use the AO RunError() virtual function
    // for that purpose, it is not called.
    
    TRAPD( err,RunLTrappedL() );
    if ( err )
        {
        RDebug::Print(_L("CamcTest_7::RunTrappedL leaves with %d"), err);
        SomethingInActiveObjectIsLeaving(err);
        }
    }

void CCamcTestClient_7::RunLTrappedL()
    {
    if ( iOpenReady ) 
        {
        iOpenReady = EFalse;
        
        // Set default parameters. They can be overwritten depending on the test case. 
        iCamc->SetVideoFrameSizeL(TSize(176,144));
        iCamc->SetVideoFrameRateL(TReal32(15));
        iCamc->SetVideoBitRateL(TInt(50000));
        iCamc->SetAudioEnabledL(ETrue); 

        RDebug::Print(_L("iCamc->Prepare1"));
        iCamc->Prepare();
        }
    else if ( iPrepareReady ) 
        {
        RDebug::Print(_L("iCamc->Record1"));
        iPrepareReady = EFalse;
        iCamc->Record();
        CTimer::After( iTimeout );
        }
    
    else if ( iRecordingReady )
        {
        RDebug::Print(_L("CamcTest_7::RunTrapped iRecordingReady") );
        iRecordingReady = EFalse;
        
        iClosed=1;
        
        }
    else if ( iPaused ) //resume
        {
        RDebug::Print(_L("iCamc->Record1"));
        iPaused = EFalse;
        iCamc->Record();
        
        }
    else //timer
        {
        
        switch(iAction)
            {
            case K_Tc7_ReleaseCameraWhileRecording:
                {
                // We are now recording
                iCamera->Release();
                iAction = K_Tc7_none;
                CTimer::After( (TInt) 10E6 );
                break;
                }
            case K_Tc7_PowerOffCameraWhileRecording:
                {
                // We are now recording
                RDebug::Print(_L("Powering off the camera"));
                iCamera->PowerOff();
                iAction = K_Tc7_none;  
                CTimer::After( (TInt) 20E6 );
                break;
                }
            case K_Tc7_ReleaseCameraCloseRecorderUseCamera:
                {
                // We are now recording
                switch(iCameraReleasedState)
                    {       
                    case ENothingReleased:
                        {
                        iCamera->Release();
                        CTimer::After( (TInt) 1E6 );
                        iCameraReleasedState = ECameraReleased;
                        break;    
                        }
                    case ECameraReleased:
                        {
                        iCamc->Close();   
                        CTimer::After( (TInt) 1E6 );
                        iCameraReleasedState = ECamcStopped;
                        break;                   
                        }
                    case ECamcStopped:
                        {
                        // Try to use the camera   
                        // Create a new Camera API implementation object, if supported
                        TRAPD( err, iCamera2 = CCamera::New2L( static_cast<MCameraObserver2&>(*this), 0 /*index*/, 0 /*Priority*/ ) );
                        if ( err )
                            {
                            PRINT( ( _L( "CCamcTestClient_7::RunLTrappedL() CCamera::New2L return code=%d" ), err ) ); 

                            // Create old Camera API implementation object.
                            iCamera2 = CCamera::NewL( static_cast<MCameraObserver&>(*this), 0 /*Index*/ );
                            }
                        else 
                            {
                            PRINT( ( _L( "CCamcTestClient_7::RunLTrappedL() using new MCameraObserver2" )) ); 
                            }
                        iCamera2->Reserve();
                        iAction = K_Tc7_none;
                        break;
                        }
                    default:
                        break;
                    }
                break;
                }

            default:
                {
                RDebug::Print(_L("CamcTest_7::RunTrappedL default"));
                iCamc->Close();
                // Simulates that iRecording has been completed through the 
                // MvruoRecordComplete callback
                iRecordingReady=ETrue;	
                iCamera->Release();
                CTimer::After( (TInt) 1E6 );
                }
            }
        

        
        }
    
    
    if ( iClosed )
        {
        RDebug::Print(_L("CamcTest_7::RunTrappedL iClosed"));
        iCamc->Close();
        CActiveScheduler::Stop();
        }
    }



void CCamcTestClient_7::DoCancel()
    {
    RDebug::Print(_L("CamcTest_7::DoCancel") );
    iCamc->Stop();
    iCamc->Close();
    //delete iCamc;
    //iCamc = NULL;
    }

////////////////////////////////////////////////////////
// MVideoRecorderUtilityObserver Callback functions

void CCamcTestClient_7::MvruoOpenComplete( TInt aError )
    {
    if (aError)
        {
        RDebug::Print(_L("CamcTest_7::MvruoOpenComplete leaves with %d"), aError );        
        SomethingInActiveObjectIsLeaving(aError);
        }
    else
        {
        iOpenReady = ETrue;
        RunL(); // RunL is trapped, cannot actually leave.
        }
    }

void CCamcTestClient_7::MvruoPrepareComplete( TInt aError )
    {    
    if (aError)
        {
         RDebug::Print(_L("CamcTest_7::MvruoPrepareComplete leaves with %d"), aError );
         SomethingInActiveObjectIsLeaving(aError);
        }
    else
        {
        iPrepareReady = ETrue;
        RunL(); // RunL is trapped, cannot actually leave.
        }
    }

void CCamcTestClient_7::MvruoRecordComplete( TInt aError )
    {
    if (aError)
        {
         RDebug::Print(_L("CamcTest_7::MvruoRecordComplete leaves with %d"), aError );
         SomethingInActiveObjectIsLeaving(aError);
        }
    else
        {
        iRecordingReady = ETrue;
        RunL(); // RunL is trapped, cannot actually leave.
        }
    }

void CCamcTestClient_7::MvruoEvent(const TMMFEvent& /*aEvent*/) 
    {
    /* Should be never called ... */  
    SomethingInActiveObjectIsLeaving(99);
    }


////////////////////////////////////////////////////////
// MCameraObserver Callback functions


void CCamcTestClient_7::ReserveComplete(TInt aError)
    {

    // The Camera is now reserved for us.

    if (aError)
        {
            SomethingInActiveObjectIsLeaving(aError);  
        }

    if (iCamera2 && iWaitScheduler->IsStarted())
        {
        iCamera2->PowerOn();
        }
    else if (iCamera && iWaitScheduler->IsStarted())
        {
        iCamera->PowerOn();
        }
    
    }

void CCamcTestClient_7::PowerOnComplete(TInt aError)
    {
    if (aError)
        {
        SomethingInActiveObjectIsLeaving(aError);
        }

    if (iWaitScheduler->IsStarted())
	    {
    	 iWaitScheduler->AsyncStop();
    	}  
    	     
    // If no errors, continue tests.
    switch(iAction)
        {
        case K_Tc7_ReleaseCameraWhileRecording:
        case K_Tc7_PowerOffCameraWhileRecording: 
        case K_Tc7_ReleaseCameraCloseRecorderUseCamera:
            {
              
            RDebug::Print(_L("CamcTest_7::case1 OpenFileL in"));
            // Start the Camcorder.
            TUid controllerUid = {TC7_CONTROLLER_UID};	
            TUid formatUid = {TC7_VIDEO_FORMAT_UID};
            TFileName file;
            AddDriveLetterToPath(_L("recordQCIF.3gp"), file);
            TRAPD(err, iCamc->OpenFileL(
                file,	                     	// File
                iCamera->Handle(),				//CameraHandle
                controllerUid,					//Controller UID
                formatUid						//Format UID
                ));

            if (err)
                {
                RDebug::Print(_L("CamcTest_7::case1 OpenFileL leaves with %d"), err); //FIXME
                SomethingInActiveObjectIsLeaving(err);
                }
            RDebug::Print(_L("CamcTest_7::case1 OpenFileL out"));
            break;
            }
        case K_Tc7_OpenFileWhenCameraIsNotAvailable:
            {  
           
            // Start the Camcorder.
            TUid controllerUid = {TC7_CONTROLLER_UID};	
            TUid formatUid = {TC7_VIDEO_FORMAT_UID};
   
            RDebug::Print(_L("CamcTest_7::case2 OpenFileL in"));
            TFileName file;
            AddDriveLetterToPath(_L("recordQCIF.3gp"), file);
            TRAPD(err, iCamc->OpenFileL(
                file,	
                0,			    // CameraHandle = 0 ==> Get a valid camera
                                // There shouldn't be any left.
                controllerUid,					
                formatUid						
                ));

            if (err)
                {
                RDebug::Print(_L("CamcTest_7::case2 OpenFileL leaves with %d"), err); //FIXME
                SomethingInActiveObjectIsLeaving(err);
                }
            RDebug::Print(_L("CamcTest_7::case2 OpenFileL out"));
            break;
            }
        case K_Tc7_OpenFileWithVideoFormatNULL :
            {
            // Start the Camcorder.
            TUid controllerUid = {TC7_CONTROLLER_UID};	
   
            TFileName file;
            AddDriveLetterToPath(_L("recordQCIF.3gp"), file);
            TRAPD(err, iCamc->OpenFileL(
                file,	
                 iCamera->Handle(),			 
                controllerUid,					
                KNullUid	  // VideoFormat NULL					
                ));

            if (err)
                {
                    SomethingInActiveObjectIsLeaving(err);
                }

            break;
            }
        default:
            CTimer::After( (TInt) 1E6 );   
        }
        
        
    }
void CCamcTestClient_7::ViewFinderFrameReady(CFbsBitmap& /*aFrame*/)
    {    

    // do nothing Here
    }
void CCamcTestClient_7::ImageReady(CFbsBitmap* /*aBitmap */,HBufC8* /*aData*/,TInt aError)
    {
    if (aError)
        {
        SomethingInActiveObjectIsLeaving(aError);
        }

    // If no errors, do nothing
    }
void CCamcTestClient_7::FrameBufferReady(MFrameBuffer* /*aFrameBuffer*/,TInt aError )
    {
    if (aError)
        {
        SomethingInActiveObjectIsLeaving(aError);
        }
    
    // If no errors, do nothing
    }


void CCamcTestClient_7::SomethingInActiveObjectIsLeaving (TInt aError)
    {
    // Something in the active object is leaving
    // or a fatal error has been found.

    // Stop the active object scheduler.
        iError=aError;
        iCamc->Stop();
        CActiveScheduler::Stop();
    }


// -----------------------------------------------------------------------------
// CCamcTestClient_7::HandleEvent
// MCameraObserver2 call-back handler
// -----------------------------------------------------------------------------
//
void CCamcTestClient_7::HandleEvent( const TECAMEvent& aEvent)
    {
    PRINT(( _L( "CCamcTestClient_7::HandleEvent() entering, type=%x, err=%d" ), aEvent.iEventType.iUid, aEvent.iErrorCode ));

    if ( aEvent.iEventType == KUidECamEventCameraNoLongerReserved )
        {
        PRINT(( _L( "CCamcTestClient_7::HandleEvent() KUidECamEventCameraNoLongerReserved" ) ));
        }
    else if ( aEvent.iEventType == KUidECamEventPowerOnComplete )
        {
        PRINT(( _L( "CCamcTestClient_7::HandleEvent() KUidECamEventPowerOnComplete" ) ));
        PowerOnComplete( aEvent.iErrorCode );
        }
    else if ( aEvent.iEventType == KUidECamEventReserveComplete )
       {
       PRINT(( _L( "CCamcTestClient_7::HandleEvent() KUidECamEventReserveComplete" ) ));
       ReserveComplete( aEvent.iErrorCode );
       }

    PRINT(( _L( "CCamcTestClient_7::HandleEvent() returning" ) ));
    }

// -----------------------------------------------------------------------------
// CCamcTestClient_7::ViewFinderReady
// MCameraObserver2 call-back handler
// -----------------------------------------------------------------------------
//
void CCamcTestClient_7::ViewFinderReady( MCameraBuffer& aCameraBuffer, TInt aError)
    {
    PRINT(( _L( "CCamcTestClient_7::ViewFinderReady() NOT HANDLED err=%d, NumFrames=%d" ), aError, aCameraBuffer.NumFrames() ));
    CFbsBitmap *buffer = NULL;
    if ( !aError )
        {
	    aCameraBuffer.Release();
        }
    ViewFinderFrameReady( *buffer );
    }

// -----------------------------------------------------------------------------
// CCamcTestClient_7::ImageBufferReady
// MCameraObserver2 call-back handler
// -----------------------------------------------------------------------------
//
void CCamcTestClient_7::ImageBufferReady( MCameraBuffer& aCameraBuffer, TInt aError)
    {
    PRINT(( _L( "CCamcTestClient_7::ImageBufferReady() BUFFER NOT HANDLED, err=%d, NumFrames=%d" ), aError, aCameraBuffer.NumFrames() ));
	
    if ( !aError )
        {
	    aCameraBuffer.Release();
        }
    ImageReady( NULL, NULL, aError );
    }
    
// -----------------------------------------------------------------------------
// CCamcTestClient_7::VideoBufferReady
// MCameraObserver2 call-back handler
// -----------------------------------------------------------------------------
//
void CCamcTestClient_7::VideoBufferReady( MCameraBuffer& aCameraBuffer, TInt aError)
    {
    PRINT(( _L( "CCamcTestClient_7::VideoBufferReady() entering, err=%d, NumFrames=%d" ), aError, aCameraBuffer.NumFrames() ));
    MFrameBuffer* buffer = NULL;
	
    if ( !aError )
        {
        aCameraBuffer.Release();
        }
    // Call old Camera-API observer handler
    FrameBufferReady( buffer, aError );
    
    PRINT(( _L( "CCamcTestClient_7::VideoBufferReady() returning" )));
    }




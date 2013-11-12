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

#include "camctestclient_8.h"
#include <e32def.h>
#include <AudioPreference.h>


// EXTERNAL DATA STRUCTURES

// EXTERNAL FUNCTION PROTOTYPES 
 
// CONSTANTS

// MACROS

// LOCAL CONSTANTS AND MACROS

// MODULE DATA STRUCTURES

// LOCAL FUNCTION PROTOTYPES

// FORWARD DECLARATIONS

CCamcTestClient_8::CCamcTestClient_8 () : CTimer(CActive::EPriorityHigh)
    {
    }


CCamcTestClient_8::~CCamcTestClient_8 ()
    {
    Cancel();

    delete iCamc;   
    }

CCamcTestClient_8* CCamcTestClient_8::NewL(const TDesC &aFileName, TInt *aError ,
                                           TInt * aNumberOfOpenedCamc, TInt aCameraHandle )
    {
    CCamcTestClient_8* self = new(ELeave) CCamcTestClient_8;
    CleanupStack::PushL(self);
    self->ConstructL( aFileName, aError , aNumberOfOpenedCamc, aCameraHandle );
    CleanupStack::Pop(self);
    return self;
    }

void CCamcTestClient_8::ConstructL( const TDesC &aFileName , TInt *aError ,
                                   TInt * aNumberOfOpenedCamc, TInt aCameraHandle )
    {
    iCamc = CVideoRecorderUtility::NewL( *this, KAudioPriorityVideoRecording, EMdaPriorityPreferenceTimeAndQuality );
    iOpenReady = EFalse;
    iPrepareReady = EFalse;
    iRecordingReady = EFalse;
    iPaused = EFalse;
    iError = aError;

    // Start the Camcorder.
    TUid controllerUid = {TC8_CONTROLLER_UID};	
    TUid formatUid = {TC8_VIDEO_FORMAT_UID};
    iFileName = aFileName;
    iNumberOfOpenedCamc = aNumberOfOpenedCamc;
    // Register to the number of openCam
    (*aNumberOfOpenedCamc)++; 

    iCamc->OpenFileL(
        aFileName,		                //File
        aCameraHandle,				     //CameraHandle
        controllerUid,					//Controller UID
        formatUid						//Format UID
        );

    CTimer::ConstructL();
    iTimeout = 0;
    iClosed = 0;
    CActiveScheduler::Add(this);
    }


void CCamcTestClient_8::Start_Active_Object ( TestClient_8_Actions aAction,
                                            CCamcTestClient_8 * aClient )
    {
    
    // This reserve the camera, starts the active object.

    iAction = aAction;    //iAction might be set to NULL after action has been executed
    //	iTimeout = aTimeout;
    iTimeout = (TInt) 1E6; /* 1 Second */

    iOtherClient = aClient;

    }


void CCamcTestClient_8::RunL()
    {
    
    // Trap RunL, to catch if the AO leaves.
    // A Walkaround...
    // RunError() should be used in this case, but
    // Apparently, it is not possible to use the AO RunError() virtual function
    // for that purpose, it is not called.
    
    TRAPD( err,RunLTrappedL() );
    if ( err )
        {
        SomethingInActiveObjectIsLeaving(err);
        }
    }

void CCamcTestClient_8::RunLTrappedL()
    {
    TBool closeNow = EFalse;
    TBuf<128> str;
    str = iFileName;
    if ( iOpenReady ) 
        {         
#ifdef _DEBUG
        str.Append(_L(": Opened"));
        RDebug::Print(str); 
#endif
        iOpenReady = EFalse;
        
        // Set default parameters. They can be overwritten depending on the test case. 
        iCamc->SetVideoFrameSizeL(TSize(176,144));
        iCamc->SetVideoFrameRateL(TReal32(15));
        iCamc->SetVideoBitRateL(TInt(50000));
        iCamc->SetAudioEnabledL(ETrue); 

        switch (iAction)
            {  
            case K_Tc8_MultipleInstance002_AO2:
                {
                // Wait that the instance 1 is in prepared state.
                WaitForOtherObject();
                break;
                }
            case K_Tc8_MultipleInstance004_AO2:
                {
                // Wait that the instance 1 is recording
                WaitForOtherObject();
                break;
                }
            case K_Tc8_MultipleInstance005_AO2:
                {
                // Wait that the instance 1 has finished recording
                WaitForOtherObject();
                break;          
                }
            case K_Tc8_MultipleInstance006_AO2:
                {
                // Wait that the instance 1 is in paused state
                WaitForOtherObject();
                break;
                }
            default:
                {
                iCamc->Prepare();   
                break;
                }
            }
        }
    else if ( iPrepareReady ) 
        {
#ifdef _DEBUG
        str.Append(_L(": Prepared"));
        RDebug::Print(str); 
#endif
        iPrepareReady = EFalse;

        switch (iAction)
             {
            case K_Tc8_MultipleInstance001_AO1:   
            case K_Tc8_MultipleInstance002_AO1: 
            case K_Tc8_MultipleInstance003_AO1:
                {
                // Wait forever in that state.
                ReachedWaitingState();
                break;           
                }
            case K_Tc8_MultipleInstance001_AO2:
                {
                // If both instance are prepared, test is passed.
                WaitForOtherObject();
                break;
                }
            default:
                {
                // Start recording
                iCamc->Record();
                CTimer::After( 5 * TInt(1E6) );
                }
            }
        }
    
    else if ( iRecordingReady )
        {
#ifdef _DEBUG
        str.Append(_L(": Recording Ready"));
        RDebug::Print(str); 
#endif
        iRecordingReady = EFalse;
        switch (iAction)
            {
            case K_Tc8_MultipleInstance005_AO1:  
                {
                if (!iRecordingReadyTimer)
                    {
                    RDebug::Print(_L("CamcTest_8::Multiple5 iRecordingReady in"));
                    // It takes some time to free the camera.
                    // Give 10 seconds 
                      iRecordingReady = ETrue;  
                      iRecordingReadyTimer = ETrue;
                      CTimer::After ( 10 * TInt (1E6) );
                    }
                else
                    {
                    RDebug::Print(_L("CamcTest_8::Multiple5 ReachedWaitingState in"));
                    // Wait forever
                    ReachedWaitingState();   
                    }
                break; 
                }
            default:
                break;
            }
        }
    else if ( iPaused ) //resume
        {
#ifdef _DEBUG
        str.Append(_L(": Paused"));
        RDebug::Print(str); 
#endif
        iPaused = EFalse;
        switch (iAction)
            {
            case K_Tc8_MultipleInstance006_AO1:
                {
                // Wait forever in that state.
                ReachedWaitingState();
                break;     
                }
            default:
                {
                iCamc->Record();
                break;
                }
            }
        }
    else if (  iEmittedRequest )
        {       
#ifdef _DEBUG
        str.Append(_L(": Waiting is over"));
        RDebug::Print(str); 
#endif
        // The request has been fulfilled
        iEmittedRequest = EFalse;
        switch (iAction)
            {
            case K_Tc8_MultipleInstance002_AO2:
                {
                // Instance 1 has reached the prepared state.
                // Close both.
                iOtherClient->Close();
                closeNow = ETrue;
                break;          
                }
            case K_Tc8_MultipleInstance004_AO2:
                {
                // Instance 1 is now recording.
                // Prepare the 2nd camcorder.
                iCamc->Prepare();
                break;
                }
            case K_Tc8_MultipleInstance005_AO2:
                {
                // Instance 1 has finished recording.
                // Prepare the 2nd camcorder.
                RDebug::Print(_L("CamcTest_8::Multiple5 iCamc->Prepare in"));

                iCamc->Prepare();
                break;       
                }
            case K_Tc8_MultipleInstance006_AO2:
                {
                // Instance 1 is now paused.
                // Prepare the 2nd camcorder.
                iCamc->Prepare();
                break;
                }
            default:
                {
                __ASSERT_ALWAYS(0,User::Panic(_L("Test Client"),1));
                break;
                }
                
            }
        }
    else if ( iClosed )
        {           
#ifdef _DEBUG
        str.Append(_L(": Closed"));
        RDebug::Print(str); 
#endif
        (*iNumberOfOpenedCamc)--;
        if (*iNumberOfOpenedCamc == 0)
            {
            // Everything is closed, we can stop the active scheduler    
            CActiveScheduler::Stop();
            }
        }
    else //timer
        {
#ifdef _DEBUG
        str.Append(_L(": Recording"));
        RDebug::Print(str); 
#endif
        switch (iAction)
            {
            case K_Tc8_MultipleInstance004_AO1:
                // Wait forever in that state.
                {
                ReachedWaitingState();
                break;     
                }
            case K_Tc8_MultipleInstance006_AO1:
                {
                iCamc->PauseL();
                iPaused = ETrue;
                CTimer::After( (TInt) 1E6 ); // Pause for 1 sec.
                break;
                }
            default:
                {
                iCamc->Stop();
                iRecordingReady = ETrue;
                CTimer::After( (TInt) 1E6 );
                }
            }
        }
    
    
    if (closeNow)
        {
        iCamc->Close();
        CTimer::After( (TInt) 1E6 );
        iClosed = ETrue;
        }
    
    }



void CCamcTestClient_8::DoCancel()
    {
    iCamc->Stop();
    iCamc->Close();
    delete iCamc;
    iCamc = NULL;
    }

////////////////////////////////////////////////////////
// MVideoRecorderUtilityObserver Callback functions

void CCamcTestClient_8::MvruoOpenComplete( TInt aError )
    {
    if (aError)
        {        
        SomethingInActiveObjectIsLeaving(aError);
        }
    else
        {
        iOpenReady = ETrue;
        RunL(); // RunL is trapped, cannot actually leave.
        }
    }

void CCamcTestClient_8::MvruoPrepareComplete( TInt aError )
    {    
    if (aError)
        {
         SomethingInActiveObjectIsLeaving(aError);
        }
    else
        {
        iPrepareReady = ETrue;
        RunL(); // RunL is trapped, cannot actually leave.
        }
    }

void CCamcTestClient_8::MvruoRecordComplete( TInt aError )
    {
    if (aError)
        {
         SomethingInActiveObjectIsLeaving(aError);
        }
    else
        {
        iRecordingReady = ETrue;
        RunL(); // RunL is trapped, cannot actually leave.
        }
    }

void CCamcTestClient_8::MvruoEvent(const TMMFEvent& /*aEvent*/) 
    {
    /* Should be never called ... */  
    SomethingInActiveObjectIsLeaving(99);
    }


void CCamcTestClient_8::Close()
    {
    // We are requested by an other object to close.
        iCamc->Close();
        CTimer::After( (TInt) 1E6 );
        iClosed = ETrue;
    }


void CCamcTestClient_8::ReachedWaitingState()
    {
    // We have reach a state were we wait for the other object to generate a 
    // TellMeWhenYourInCorrectState request.

    iReachedWaitingState = ETrue;
    if (iRequestPending)
        {
        // Serve the request
       // TRequestStatus* status = &iReceivedRequest;
        User::RequestComplete(iReceivedRequest, KErrNone);
        }

    }


void CCamcTestClient_8::WaitForOtherObject()
    {
    // One object (the client) generates a request to the other object
    iOtherClient->TellMeWhenYouAreInCorrectState(iStatus);
    SetActive();
    if (iStatus != KRequestPending)
        {
        // If the client was already in correct state, we complete the request ourselves.
        TRequestStatus * aReq = &iStatus;
        User::RequestComplete(aReq, KErrNone);
        }
    iEmittedRequest = ETrue;
    }

void CCamcTestClient_8::TellMeWhenYouAreInCorrectState(TRequestStatus &aRequest)
    {
    if (iReachedWaitingState)
        {
        // Already waiting !
        aRequest = 0; // Something else then KRequestPending.
        iRequestPending = EFalse;
        }
    else
        {
        aRequest = KRequestPending;    
        iRequestPending = ETrue;        
        }
    iReceivedRequest= &aRequest;
    }


void CCamcTestClient_8::SomethingInActiveObjectIsLeaving (TInt aError)
    {
    // Something in the active object is leaving
    // or a fatal error has been found.

    // Stop the active object scheduler.
        *iError=aError;
        iCamc->Stop();
        CActiveScheduler::Stop();
    }
    
    




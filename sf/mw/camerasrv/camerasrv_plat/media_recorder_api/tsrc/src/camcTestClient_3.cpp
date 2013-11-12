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

#include "camctestclient_3.h"
#include "camcTest.h"
#include <AudioPreference.h>

// EXTERNAL DATA STRUCTURES

// EXTERNAL FUNCTION PROTOTYPES 
 
// CONSTANTS

// MACROS

// LOCAL CONSTANTS AND MACROS

// MODULE DATA STRUCTURES

// LOCAL FUNCTION PROTOTYPES

// FORWARD DECLARATIONS


CCamcTestClient_3::CCamcTestClient_3 () : CTimer(CActive::EPriorityHigh)
    {
    }


CCamcTestClient_3::~CCamcTestClient_3 ()
    {
    Cancel();
    delete iCamc;
    }

CCamcTestClient_3* CCamcTestClient_3::NewL(TInt aCameraHandle)
    {
    CCamcTestClient_3* self = new(ELeave) CCamcTestClient_3;
    CleanupStack::PushL(self);
    self->ConstructL(aCameraHandle );
    CleanupStack::Pop(self);
    return self;
    }

void CCamcTestClient_3::ConstructL(TInt aCameraHandle)
    {
    iCamc = CVideoRecorderUtility::NewL( *this, KAudioPriorityVideoRecording, EMdaPriorityPreferenceTimeAndQuality );
    iOpenReady = EFalse;
    iPrepareReady = EFalse;
    iRecordingReady = EFalse;
    iPaused = EFalse;
    
    CTimer::ConstructL();
    iTimeout = 0;
    iClosed = 0;
    CActiveScheduler::Add( this );
    Start_Active_ObjectL ( K_Tc3_none, aCameraHandle );
    }


void CCamcTestClient_3::Start_Active_ObjectL ( TestClient3Actions aAction, TInt aCameraHandle )
    {
    
    TInt camHandle = aCameraHandle;
    TUid controllerUid = {0x101F8503};	//implementation uid
    TUid formatUid = {0};
    
    
    iAction = aAction;                  //iAction might be set to NULL after action has been executed
    iSavedAction = aAction;             //SavedAction will never be changed.
    //	iTimeout = aTimeout;
    iTimeout = 1500000;
    
    TFileName file;
    AddDriveLetterToPath(_L("recordQCIF.3gp"),file);
    TRAPD(err,
        iCamc->OpenFileL( file, camHandle, 
        controllerUid,
        formatUid,
        //TFourCC( '2', '6', '3', 'B' ),
        _L8("video/H263-2000"),
        TFourCC(' ', 'A', 'M', 'R') ));
    
    if (err)
        {
        User::Leave(99);
        }
    iOpenReady = EFalse;
    
    // Main part of program is a wait loop
    // This function completes when the scheduler stops
    CActiveScheduler::Start();
    
    if ( iError )
        {
        User::Leave( iError );
        }
    }


void CCamcTestClient_3::RunL()
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

void CCamcTestClient_3::RunLTrappedL()
    { 
    if ( iOpenReady ) 
        {
        iOpenReady = EFalse;
        
        // Set default parameters. They can be overwritten depending on the test case. 
        iCamc->SetVideoFrameSizeL(TSize(176,144));
        iCamc->SetVideoFrameRateL(TReal32(15));
        iCamc->SetVideoBitRateL(TInt(50000));
        iCamc->SetAudioEnabledL(ETrue); 
        
        iCamc->Prepare();
        }
    else if ( iPrepareReady ) 
        {
        
        iPrepareReady = EFalse;
        iCamc->Record();
        CTimer::After( iTimeout );
        }
    
    else if ( iRecordingReady )
        {
        
        iRecordingReady = EFalse;
        iCamc->Close();
        iClosed =  ETrue;
        
        }
    else if ( iPaused ) //resume
        {
        iPaused = EFalse;
        iCamc->Record();
        
        }
    else //timer
        {
        
        iCamc->Stop();
        // Simulates that iRecording has been completed through the 
        // MvruoRecordComplete callback
        iRecordingReady=ETrue;				
        CTimer::After( 1000 );
        
        }
    
    
    if ( iClosed )
        {
        iCamc->Stop();
        CActiveScheduler::Stop();
        }
    }



void CCamcTestClient_3::DoCancel()
    {
    iCamc->Stop();
    iCamc->Close();
    delete iCamc;
    iCamc = NULL;
    }

void CCamcTestClient_3::MvruoOpenComplete( TInt aError )
    {
    if (aError)
        {        
        SomethingInActiveObjectIsLeaving(aError);
        }
    else
        {
        iOpenReady = ETrue;
        RunL();  // RunL is trapped, cannot actually leave.
        }
    }

void CCamcTestClient_3::MvruoPrepareComplete( TInt aError )
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

void CCamcTestClient_3::MvruoRecordComplete( TInt aError )
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

void CCamcTestClient_3::MvruoEvent(const TMMFEvent& /*aEvent*/) 
    {
    /* Should be never called ... */  
    SomethingInActiveObjectIsLeaving(99);
    }

void CCamcTestClient_3::SomethingInActiveObjectIsLeaving (TInt aError)
    {
    // Something in the active object is leaving
    // or a fatal error has been found.

    // Stop the active object scheduler.
        iError=aError;
        iCamc->Stop();
        CActiveScheduler::Stop();
    }




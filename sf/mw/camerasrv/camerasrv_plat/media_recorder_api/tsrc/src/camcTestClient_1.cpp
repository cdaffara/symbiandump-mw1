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
#include "camctestclient_1.h"
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




CCamcTestClient_1::CCamcTestClient_1 () : CTimer(CActive::EPriorityHigh)
    {
    }



CCamcTestClient_1::~CCamcTestClient_1 ()
    {
    Cancel();
    iCamc->Close();
    delete iCamc;
    }

CCamcTestClient_1* CCamcTestClient_1::NewL( )
    {
    CCamcTestClient_1* self = new(ELeave) CCamcTestClient_1;
    CleanupStack::PushL(self);
    self->ConstructL();
    CleanupStack::Pop(self);
    return self;
    }

void CCamcTestClient_1::ConstructL( )
    {
    iCamc = CVideoRecorderUtility::NewL( *this, KAudioPriorityVideoRecording, EMdaPriorityPreferenceTimeAndQuality );
    iOpenReady = EFalse;
    iPrepareReady = EFalse;
    iRecordingReady = EFalse;
    iPaused = EFalse;
    
    iNrPauses = 0;
    iPauseCount = 0;
    iTimeout = 0;
    
    CTimer::ConstructL();
    
    CActiveScheduler::Add(this);
    }

void CCamcTestClient_1::Test_OpenFileL(const TDesC& aFileName, 
                                       TInt aCameraHandle, 
                                       TUid aControllerUid, 
                                       TUid aVideoFormat,
                                       const TDesC8 & aVideoType,
                                       TFourCC aAudioType)
    {
    TFileName file;
    AddDriveLetterToPath(aFileName,file);
    iCamc->OpenFileL( file, aCameraHandle, aControllerUid, aVideoFormat,aVideoType, aAudioType);
    }

void CCamcTestClient_1::Test_OpenFileL(const TDesC& aFileName, 
                                       TInt aCameraHandle, 
                                       TUid aControllerUid, 
                                       TUid aVideoFormat)
    {
    TFileName file;
    AddDriveLetterToPath(aFileName,file);
    iCamc->OpenFileL( file, aCameraHandle, aControllerUid, aVideoFormat);
    }

void CCamcTestClient_1::Test_OpenDesL(TDes8& aDescriptor, 
                                      TInt aCameraHandle, 
                                      TUid aControllerUid, 
                                      TUid aVideoFormat,
                                      const TDesC8 & aVideoType,
                                      TFourCC aAudioType)
    {
    iCamc->OpenDesL( aDescriptor, aCameraHandle, aControllerUid, aVideoFormat,aVideoType, aAudioType);
    }

void CCamcTestClient_1::Test_OpenUrlL(const TDesC& aUrl, 
                                      TInt aCameraHandle, 
                                      TUid aControllerUid, 
                                      TUid aVideoFormat,
                                      const TDesC8 & aVideoType,
                                      TFourCC aAudioType)
    {
    iCamc->OpenUrlL( aUrl, 0, aCameraHandle, aControllerUid, aVideoFormat,aVideoType, aAudioType);
    }

TInt CCamcTestClient_1::WaitForCallbackWithErrors()
    {
    iStopWhenOpenComplete = ETrue;
    CActiveScheduler::Start();
    return(iError);
    }


void CCamcTestClient_1::RunL()
    {
    // Trap RunL, to catch if the AO leaves.
    // A Walkaround...
    // RunError() should be used in this case, but
    // Apparently, it is not possible to use the AO RunError() virtual function
    // for that purpose, it is not called.
    
    TRAPD( err,
           RunLTrappedL() );
    if ( err )
        {
        SomethingInActiveObjectIsLeaving(err);
        }
    }

void CCamcTestClient_1::RunLTrappedL()
    {
    TBuf<100> aaa;
    aaa.Format(_L("RunL iOpenReady:%i iPrepareReady:%i iRecordingReady:%i iPaused:%i"), 
        iOpenReady, iPrepareReady, iRecordingReady, iPaused);
    if ( iOpenReady ) 
        {
        iOpenReady = EFalse;

        iCamc->SetVideoFrameSizeL(TSize(176,144));

        iCamc->SetVideoFrameRateL(TReal32(15));
        iCamc->SetVideoBitRateL(TInt(50000));
        iCamc->SetAudioEnabledL(EFalse);
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
        
        CActiveScheduler::Stop();
        
        }
    else if ( iPaused ) //resume
        {
        iPaused = EFalse;
        iCamc->Record();
        
        }
    else //timer
        {
        if ( iPauseCount < iNrPauses )
            {
            iPaused = ETrue;
            iPauseCount++;
            iCamc->PauseL();
            //add timer
            CTimer::After( iTimeout );
            }
        else 
            {
            iCamc->Stop();
            
            CActiveScheduler::Stop();
            }
        }
    }

void CCamcTestClient_1::DoCancel()
    {
    iCamc->Stop();
    iCamc->Close();
    delete iCamc;
    iCamc = NULL;
    }

void CCamcTestClient_1::MvruoOpenComplete( TInt aError )
    {
    if ( iStopWhenOpenComplete )
        {
        iError = aError;
        CActiveScheduler::Stop();
        return;
        }

    if (aError)
        {        
        SomethingInActiveObjectIsLeaving(aError);
        }
    else
        {
        iOpenReady = ETrue;
        RunL(); // RunL is trapped, it cannot leave.
        }
    }

void CCamcTestClient_1::MvruoPrepareComplete( TInt aError )
    {    
    TBuf<128> str;
    str.Format(_L("CCamcTestClient_1::MvruoPrepareComplete %i"), aError);
    RDebug::Print(str); 
    if (aError)
        {
         SomethingInActiveObjectIsLeaving(aError);
        }
    else
        {
        iPrepareReady = ETrue;
        RunL(); // RunL is trapped, it cannot leave.
        }
    }

void CCamcTestClient_1::MvruoRecordComplete( TInt aError )
    {    
    TBuf<128> str;
    str.Format(_L("CCamcTestClient_1::MvruoRecordComplete %i"), aError);
    RDebug::Print(str); 
    if (aError)
        {
         SomethingInActiveObjectIsLeaving(aError);
        }
    else
        {
        iRecordingReady = ETrue;
        RunL(); // RunL is trapped, it cannot leave.
        }
    }

void CCamcTestClient_1::MvruoEvent(const TMMFEvent& /*aEvent*/) 
    {   
    RDebug::Print(_L("CCamcTestClient_1::MvruoEvent"));
    /* Should be never called ... */  
    SomethingInActiveObjectIsLeaving(99);
    }

void CCamcTestClient_1::SomethingInActiveObjectIsLeaving (TInt aError)
    {
    // Something in the active object is leaving
    // or a fatal error has been found.

    TBuf<128> str;
    str.Format(_L("CCamcTestClient_1::Something In Active Object is leaving %i"), aError);
    RDebug::Print(str);   

    // Stop the active object scheduler.
        iError=aError;
        iCamc->Stop();
        CActiveScheduler::Stop();
    }





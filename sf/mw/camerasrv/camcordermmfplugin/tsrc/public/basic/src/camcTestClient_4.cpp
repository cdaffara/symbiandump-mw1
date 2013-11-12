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

#include "CamcTestClient_4.h"
#include "camctest.h"
#include <AudioPreference.h>

// EXTERNAL DATA STRUCTURES

// EXTERNAL FUNCTION PROTOTYPES 
 
// CONSTANTS

const TInt KMaxVideoLength = 3600;

// MACROS

// LOCAL CONSTANTS AND MACROS

// MODULE DATA STRUCTURES

// LOCAL FUNCTION PROTOTYPES

// FORWARD DECLARATIONS


CCamcTestClient_4::CCamcTestClient_4 () : CTimer(CActive::EPriorityHigh)
    {
    }


CCamcTestClient_4::~CCamcTestClient_4 ()
    {
    Cancel();
    delete iCamc;
    }

CCamcTestClient_4* CCamcTestClient_4::NewL()
    {
    CCamcTestClient_4* self = new(ELeave) CCamcTestClient_4;
    CleanupStack::PushL(self);
    self->ConstructL( );
    CleanupStack::Pop(self);
    return self;
    }

void CCamcTestClient_4::ConstructL( )
    {
    iCamc = CVideoRecorderUtility::NewL( *this, KAudioPriorityVideoRecording, EMdaPriorityPreferenceTimeAndQuality );
    iOpenReady = EFalse;
    iPrepareReady = EFalse;
    iRecordingReady = EFalse;
    iPaused = EFalse;
    
    CTimer::ConstructL();
    iTimeout = 0;
    iClosed = 0;
    CActiveScheduler::Add(this);
    }


void CCamcTestClient_4::Start_Active_ObjectL ( TestClient4Actions aAction, TInt aCameraHandle )
    {
    
    TInt camHandle = aCameraHandle;
    TUid controllerUid = {0x101F8503};	//implementation uid
    TUid formatUid = {0};
    
    
    iAction = aAction;                  //iAction might be set to NULL after action has been executed
    iSavedAction = aAction;             //SavedAction will never be changed.
    
    TFileName file;
    AddDriveLetterToPath(_L("recordQCIF.3gp"),file);
    TRAPD(err,
        iCamc->OpenFileL(file, camHandle, 
        controllerUid,
        formatUid,
        //TFourCC( '2', '6', '3', 'B' ),
        _L8("video/H263-2000"),
        TFourCC(' ', 'A', 'M', 'R') ));
    
    if (err)
        {
        User::Leave(K4UnexpectedErr);
        }
    
    iOpenReady = EFalse;
    
    // Main part of program is a wait loop
    // This function completes when the scheduler stops
    CActiveScheduler::Start();
    
    // We continue here when the AO scheduler has been stopped.
    if ( iError )
        {
        User::Leave( iError );
        }
    }

void CCamcTestClient_4::RunL()
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

void CCamcTestClient_4::RunLTrappedL()
    {
    
    if ( iClosed )
        {
        iCamc->Stop();
        CActiveScheduler::Stop();
        }
    else
        if ( iOpenReady ) 
            {
            iOpenReady = EFalse;
            
            // Set default parameters. They can be overwritten depending on the test case. 
            iCamc->SetVideoFrameSizeL(TSize(176,144));
            iCamc->SetVideoFrameRateL(TReal32(5));
            iCamc->SetVideoBitRateL(TInt(TC4_VIDEO_BIT_RATE));
            iCamc->SetAudioBitRateL(TInt(TC4_AUDIO_BIT_RATE));
            iCamc->SetAudioEnabledL(ETrue); 
            switch (iAction)
                {
                case K4RecordTimeAvailSizeLimit:
                    {
                    iCamc->SetMaxClipSizeL(TC4_FILE_SIZE_LIMIT) ; 
                    break;
                    }
                case K4RecordTimeAvailNotRecordingSizeLimit:
                    {
                    iCamc->SetMaxClipSizeL(TC4_FILE_SIZE_LIMIT) ; 
                    break;
                    }
                default:
                    break;
                }
            
            iCamc->Prepare();
            }
        else if ( iPrepareReady ) 
            {    
            switch (iAction)
                {
                case K4RecordTimeAvailNotRecordingSizeLimit:
                    {
                    // Compare if set size limit is correctly returned by 
                    // a RecordTimeAvailableL      
                    TTimeIntervalMicroSeconds timeRemaining = iCamc->RecordTimeAvailable();
                    TInt timeRemainingInSec = (I64LOW (( TInt64 )(timeRemaining.Int64() /1E6)));
                    
                    // Value must be proportional to size limit
                    TInt calculatedDuration = (TInt)
                       ((float)TC4_FILE_SIZE_LIMIT) * 8 / (TC4_VIDEO_BIT_RATE+TC4_AUDIO_BIT_RATE);
                    // Allows 5% error margin:
                    if ( ( timeRemainingInSec > calculatedDuration*1.05 ) ||
                        ( timeRemainingInSec < calculatedDuration*0.95 ) )
                        {          
                        User::Leave( K4ValueNotWithinRange ) ; 
                        }          
                    break;
                    }
                case K4RecordTimeAvailNotRecordingNoSizeLimit:
                    {
                    // Compare if default size limit is correctly returned by 
                    // a RecordTimeAvailableL      
                    TTimeIntervalMicroSeconds timeRemaining = iCamc->RecordTimeAvailable();
                    TInt64 timeRemainingInSec = timeRemaining.Int64()/1E6;
                    
                    // Duration value depends on available disk drive space.
                    // Get the free space of the disk drive:
                    RFs fsSession;
                    User::LeaveIfError(fsSession.Connect());
                    TVolumeInfo volumeInfo;
                    TInt err = fsSession.Volume(volumeInfo,EDriveC);
                    fsSession.Close();
                    if (err)
                        {
                        User::Leave( KErrGeneral) ;
                        }
                    // Possible duration = available disk space/videobitrate
                    TInt64 calculatedDuration = volumeInfo.iFree / TInt64((TC4_VIDEO_BIT_RATE+TC4_AUDIO_BIT_RATE)/8);
                    
                    // If there is more space than the max length of the video -> 
                    // set the calculatedDuration to max length
                    if ( calculatedDuration > KMaxVideoLength) 
                        {
                        calculatedDuration = KMaxVideoLength;
                        }
                    
                    RDebug::Print(_L("CCamcTest_4::RecordTimeAvailable4 timeRemaining is %d (Should be %d)"), timeRemainingInSec, calculatedDuration );
                    // Allows 5% error margin:
                    if ( ( timeRemainingInSec > calculatedDuration*1.05 ) ||
                        ( timeRemainingInSec < calculatedDuration*0.95 ) )
                        {          
                        User::Leave( K4ValueNotWithinRange ) ; 
                        }
                    break;
                    
                    }
                default:
                    break;
                }
            iPrepareReady = EFalse;
            
            iCamc->Record();
            
            // Set a timer 
            // Must be >10 sec to avoid the uncertainties of the bitrates 
            // at the beginning of a new recording.
            CTimer::After( (TInt) (20*1E6) );

            }
        
        else if ( iRecordingReady )
            {
            
            iRecordingReady = EFalse;
            iCamc->Close();
            iClosed = ETrue;
            CTimer::After( 1500 ); // Give time to the AO to close, will callback RunL after.
            
            }
        else if ( iPaused ) //resume
            {
            
            iPaused = EFalse;
            iCamc->Record();
            }
        else //timer
            {
            
            TTimeIntervalMicroSeconds timeRemaining = iCamc->RecordTimeAvailable();
#ifdef _DEBUG
            TBuf<128> str;
            str.Format(_L("Time remaining %x %x"), I64HIGH(timeRemaining.Int64()),
                I64LOW(timeRemaining.Int64()) );
            RDebug::Print(str);   
#endif

            if ( iAction != K4RecordTimeAvailUntilDiskFull )
                {
                if (( iNumberOfTimeOut != 0) && (iOldTimeRemaining <= timeRemaining))
                    {
                   User::Leave(K4ValueNotDecreasingErr);
                    }
                }

            iOldTimeRemaining = timeRemaining;
            
            iNumberOfTimeOut++;
            
            switch (iAction)
                {
                case K4RecordTimeAvailSizeLimit:
                    {
                    // Do nothing, stop callback should be called when
                    // size limit is reached.
                    CTimer::After( (TInt) 15E6 );     // Wait 15 sec	
                    break;
                    }
                case K4RecordTimeAvailNoSizeLimit:
                    {
                    // After 50 timers calls, stop the test
                    // It is normal behavior.
                    if (iNumberOfTimeOut == 15)
                        {
                        iCamc->Close();
                        iClosed = ETrue;
                        }
                    CTimer::After( (TInt) 15E6 );     // Wait 15 sec	
                    break;
                    }


            case K4RecordTimeAvailUntilDiskFull:
                {
                //TTimeIntervalMicroSeconds timeRemaining = iCamc->RecordTimeAvailable();
                iTimeRemainingInSec = timeRemaining.Int64()/1E6;
                CTimer::After( (TInt) (1*1E6) );
                break;
                }

            default:
                    // Stop the recording
                    iCamc->Close();
                    iClosed = ETrue;
                    CTimer::After( (TInt) 15E6 );     // Wait 15 sec	
                }
            }
        
        
    }
    
    
    
    void CCamcTestClient_4::DoCancel()
        {
        iCamc->Stop();
        iCamc->Close();
        delete iCamc;
        iCamc = NULL;
        }
    
    
    void CCamcTestClient_4::MvruoOpenComplete( TInt aError )
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
    
    void CCamcTestClient_4::MvruoPrepareComplete( TInt aError )
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
    
    void CCamcTestClient_4::MvruoRecordComplete( TInt aError )
        {
    
        switch (iAction)
            {
            case K4RecordTimeAvailUntilDiskFull:
                {
#ifdef _DEBUG
                TBuf<128> str;
                str.Format(_L("K4RecordTimeAvailUntilDiskFull : Disk Full ! %i"), iTimeRemainingInSec);
                RDebug::Print(str);   
#endif
                // KErrCompletion if there is more space than needed for 60 min recording.
                // Otherwise KErrDiskFull
                if ( (aError !=  KErrDiskFull) || (aError != KErrCompletion ) )
                    {
                    User::Leave(K4ErrExpectingADifferentValue);
                    }
                if ( iTimeRemainingInSec > 5 ) 
                    {
                    User::Leave(K4ValueNotWithinRange);
                    }
                break;
                }
            }

        if ( (aError) && (aError != KErrCompletion) )
            {
            SomethingInActiveObjectIsLeaving(aError);
            }
        else
            {
            iRecordingReady = ETrue;
            
            if ( !IsActive() )
                {
                RunL(); // RunL is trapped, cannot actually leave.
                }
            }
        }
    
    void CCamcTestClient_4::MvruoEvent(const TMMFEvent& /*aEvent*/) 
        {
        /* Should be never called ... */  
        SomethingInActiveObjectIsLeaving(99);
        }
    
    void CCamcTestClient_4::SomethingInActiveObjectIsLeaving (TInt aError)
        {
        // Something in the active object is leaving
        // or a fatal error has been found.
        
        // Stop the active object scheduler.
#ifdef _DEBUG
            TBuf<128> str;
            str.Format(_L("Something is leaving %i"), aError);
            RDebug::Print(str);   
#endif
        iError=aError;
        iCamc->Stop();
        CActiveScheduler::Stop();
        }
    
    
    

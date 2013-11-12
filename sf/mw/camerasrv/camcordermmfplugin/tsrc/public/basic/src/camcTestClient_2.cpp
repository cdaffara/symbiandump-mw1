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

#include "CamcTestClient_2.h"
#include <e32svr.h>
#include <AudioPreference.h>
#include "camctest.h"

// EXTERNAL DATA STRUCTURES

// EXTERNAL FUNCTION PROTOTYPES 
 
// CONSTANTS

// MACROS

// LOCAL CONSTANTS AND MACROS

// MODULE DATA STRUCTURES

// LOCAL FUNCTION PROTOTYPES

// FORWARD DECLARATIONS



CCamcTestClient_2::CCamcTestClient_2 () : CTimer(CActive::EPriorityHigh)
    { 
    RDebug::Print(_L("CamCTestClient 2: Constructor"));
    }


CCamcTestClient_2::~CCamcTestClient_2 ()
    {
#ifdef _DEBUG
    RDebug::Print(_L("CamCTestClient 2: Destructor Start"));
#endif
    Cancel();
    delete iCamc;
#ifdef _DEBUG
    RDebug::Print(_L("CamCTestClient 2: Destructor End"));
#endif
    }

CCamcTestClient_2* CCamcTestClient_2::NewL()
    {
    CCamcTestClient_2* self = new(ELeave) CCamcTestClient_2;
    CleanupStack::PushL(self);
    self->ConstructL( );
    CleanupStack::Pop(self);
    return self;
    }

void CCamcTestClient_2::ConstructL( )
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


void CCamcTestClient_2::Start_Active_ObjectL ( TestClient2Actions aAction, TInt aCameraHandle )
    {

    TInt camHandle = aCameraHandle;
    TUid controllerUid = {0x101F8503};	//implementation uid
    TUid formatUid = {0};
    TUid formatUidMP4 = {0x101F873D};
    
    
    iAction = aAction;                  //iAction might be set to NULL after action has been executed
    iSavedAction = aAction;             //SavedAction will never be changed.
    //	iTimeout = aTimeout;
    iTimeout = 1500000;
    switch ( iAction )
        {
        case  KcloseWhenControllerNotReady :
            {
            iCamc->Close();
            CTimer::After( 1000 );	// Give some time for all Active objects to react to this
            // Command
            break;
            }
            
        case KprepareWhenControllerNotReady :
            {
            iCamc->Prepare();
            CTimer::After( 1000 );	// Give some time for all Active objects to react to this
            // Command
            break;
            }
            
        case KrecordWhenControllerNotReady :
            {
            iCamc->Record();	
            CTimer::After( 1000 );	// Give some time for all Active objects to react to this
            // Command
            break;
            }
            
        case KSetPriorityLWhenControllerNotReady :
            {
            iCamc->SetPriorityL(EMdaPriorityNormal,EMdaPriorityPreferenceTime );	
            CTimer::After( 1000 );	// Give some time for all Active objects to react to this
            // Command
            break;
            }
        case KSetVideoFrameRateLWhenControllerNotReady :
            {
            iCamc->SetVideoFrameRateL( TReal32(TC2_VIDEO_FRAME_RATE) );
            CTimer::After( 1000 );	// Give some time for all Active objects to react to this
            // Command
            break;
            }
        case KSetVideoFrameSizeLWhenControllerNotReady :
            {
            iCamc->SetVideoFrameSizeL( TC2_VIDEO_FRAME_SIZE );
            CTimer::After( 1000 );	// Give some time for all Active objects to react to this
            // Command
            break;
            }
        case KSetVideoBitRateLWhenControllerNotReady :
            {
            iCamc->SetVideoBitRateL( TC2_VIDEO_BIT_RATE  );
            CTimer::After( 1000 );	// Give some time for all Active objects to react to this
            // Command
            break;
            }
        case KSetAudioBitRateLWhenControllerNotReady :
            {
            iCamc->SetAudioBitRateL( TC2_AUDIO_BIT_RATE );
            CTimer::After( 1000 );	// Give some time for all Active objects to react to this
            // Command
            break;
            }
        case KSetAudioEnabledLWhenControllerNotReady :
            {
            iCamc->SetAudioEnabledL( TC2_AUDIO_ENABLED );
            CTimer::After( 1000 );	// Give some time for all Active objects to react to this
            // Command
            break;
            }
        case KSetMaxClipSizeLWhenControllerNotReady :
            {
            iCamc->SetMaxClipSizeL( 10000 );
            CTimer::After( 1000 );	// Give some time for all Active objects to react to this
            // Command
            break;
            }
        case KSetVideoTypeLWhenControllerNotReady :
            {
            iCamc->SetVideoTypeL( TC2_VIDEO_TYPE );
            CTimer::After( 1000 );	// Give some time for all Active objects to react to this
            // Command
            break;
            }
        case KSetAudioTypeLWhenControllerNotReady :
            {
            iCamc->SetAudioTypeL(  TC2_AUDIO_TYPE );
            CTimer::After( 1000 );	// Give some time for all Active objects to react to this
            // Command
            break;
            }
        case KSetVideoFrameSizeLWhenControllerReady_MPEG4 :
        case KcloseWhenPrepareReadyUsingAACandMPEG4 :
            {
            TFileName file;
            AddDriveLetterToPath(_L("recordCIF.mp4"), file);
            TRAPD(err,
                iCamc->OpenFileL( file, 
                camHandle, 
                controllerUid,
                formatUidMP4,
                _L8("video/mp4v-es; profile-level-id=3"),
                TFourCC(' ', 'A', 'A', 'C') ));
            
            if (err)
                {
                User::Leave(99);
                }
            iOpenReady = EFalse;
            
            break;
            }
        default:
            {
            TFileName file;
            AddDriveLetterToPath(_L("recordQCIF.3gp"), file);
            TRAPD(err,
                iCamc->OpenFileL( file, 
                camHandle, 
                controllerUid,
                formatUid,
                _L8("video/H263-2000"),
                TFourCC(' ', 'A', 'M', 'R') ));
            
            if (err)
                {
                User::Leave(99);
                }
            iOpenReady = EFalse;
            
            break;
            }
        }
    // Main part of program is a wait loop
    // This function completes when the scheduler stops
    CActiveScheduler::Start();
    if ( iError )
        {
        User::Leave( iError );
        }
    }

void CCamcTestClient_2::RunL()
    {
    
    // Trap RunL, to catch if the AO leaves.
    // A Walkaround...
    // RunError() should be used in this case, but
    // Apparently, it is not possible to use the AO RunError() virtual function
    // for that purpose, it is not called.
#ifdef _DEBUG
    RDebug::Print(_L("CamCTestClient 2:RunL"));
#endif
    TRAPD( err,
        RunLTrappedL() );
    if ( err )
        {
        SomethingInActiveObjectIsLeaving(err);
        }
    }

void CCamcTestClient_2::RunLTrappedL()
    {
    TBool prepare = EFalse;
    if ( iOpenReady ) 
        {
        iOpenReady = EFalse;
        
        // Set default parameters. They can be overwritten depending on the test case. 
        iCamc->SetVideoFrameSizeL(TSize(176,144));
        iCamc->SetVideoFrameRateL(TReal32(15));
        iCamc->SetVideoBitRateL(TInt(50000));
        iCamc->SetAudioEnabledL(ETrue);

        switch ( iAction )
            {
            case  KcloseWhenControllerReady:
                {
                iCamc->Close();		
                iClosed = 1;
                break;
                }
                
            case  KprepareWhenControllerReady:
                {
                prepare = ETrue;
                break;
                }
                
            case KrecordWhenControllerReady:
                {
                iCamc->Record();
                break;
                }
                
            case KSetPriorityLWhenControllerReady :
                {
                iCamc->SetPriorityL(EMdaPriorityNormal,EMdaPriorityPreferenceTime );
                prepare = ETrue;
                break;
                }
            case KSetVideoFrameRateLWhenControllerReady :
                {
                iCamc->SetVideoFrameRateL( TReal32(TC2_VIDEO_FRAME_RATE) );
                prepare = ETrue;
                break;
                }
            case KSetVideoFrameSizeLWhenControllerReady :
                {
                iCamc->SetVideoFrameSizeL( TC2_VIDEO_FRAME_SIZE );
                prepare = ETrue;
                break;
                }
            case KSetVideoFrameSizeLWhenControllerReady_MPEG4 :
                {
                iCamc->SetVideoFrameSizeL( TC2_CIF_VIDEO_FRAME_SIZE );
                prepare = ETrue;
                break;
                }
            case KSetVideoBitRateLWhenControllerReady :
                {
                iCamc->SetVideoBitRateL( TC2_VIDEO_BIT_RATE );	
                prepare = ETrue;
                break;
                }
            case KSetAudioBitRateLWhenControllerReady :
                {
                iCamc->SetAudioBitRateL( TC2_AUDIO_BIT_RATE  );	
                prepare = ETrue;
                break;
                }
            case KSetAudioEnabledLWhenControllerReady :
                {
                iCamc->SetAudioEnabledL( TC2_AUDIO_ENABLED );	
                prepare = ETrue;
                break;
                }
            case KSetMaxClipSizeLWhenControllerReady :
                {
                iCamc->SetMaxClipSizeL( 10000 );	
                prepare = ETrue;
                break;
                }
            case KSetVideoTypeLWhenControllerReady :
                {
                iCamc->SetVideoTypeL( TC2_VIDEO_TYPE );
                prepare = ETrue;
                break;
                }
            case KSetAudioTypeLWhenControllerReady :
                {
                iCamc->SetAudioTypeL( TC2_AUDIO_TYPE );
                prepare = ETrue;
                break;
                }
            default:
                {
                prepare = ETrue;
                break;
                }
            }
        if ( prepare )
            {
            iCamc->Prepare();
            }
        }
    else if ( iPrepareReady ) 
        {
        TBool record = EFalse;
        
        switch ( iAction )
            {
            case KcloseWhenPrepareReady:
                {
                iCamc->Close();		
                iClosed = 1;
                break;
                }
            case KcloseWhenPrepareReadyUsingAACandMPEG4:
                {
                iCamc->Close();		
                iClosed = 1;
                break;
                }

            case  KprepareWhenPrepareReady:
                {
                iAction = Knone;
                iPrepareReady = EFalse;
                iCamc->Prepare();
                break;
                }
            case  KprepareWhenPrepareReadySetVideoAttributes:
                {
                iAction = Knone;
                iPrepareReady = EFalse;
                iCamc->SetVideoFrameSizeL( TSize(176,144) );
                iCamc->SetVideoFrameRateL( TReal32(15) );
                iCamc->SetVideoBitRateL( TInt(50000) );
                iCamc->SetAudioEnabledL( EFalse );
                iCamc->Prepare();
                break;
                }
            case KrecordWhenPrepareReady:
                {
                record = ETrue;
                break;
                }
            case KSetPriorityLWhenPrepareReady :
                {
                if (!iSecondTime)
                    {
                    iCamc->SetPriorityL(EMdaPriorityNormal,EMdaPriorityPreferenceTime );
                    record = EFalse;
                    iCamc->Prepare();
                    iSecondTime = ETrue;
                    }
                else
                    {
                    record= ETrue;
                    }
                break;
                }
            case KSetVideoFrameRateLWhenPrepareReady :
                {
                iCamc->SetVideoFrameRateL( TReal32(TC2_VIDEO_FRAME_RATE) );
                record = ETrue;
                break;
                }
            case KSetVideoFrameSizeLWhenPrepareReady :
                {
                iCamc->SetVideoFrameSizeL( TC2_VIDEO_FRAME_SIZE );
                record = ETrue;
                break;
                }
            case KSetVideoBitRateLWhenPrepareReady :
                {
                iCamc->SetVideoBitRateL( TC2_VIDEO_BIT_RATE );
                record = ETrue;
                break;
                }
            case KSetAudioBitRateLWhenPrepareReady :
                {
                iCamc->SetAudioBitRateL( TC2_AUDIO_BIT_RATE );
                record = EFalse;
                iCamc->Prepare();
                iAction = Knone;
                break;
                }
            case KSetAudioEnabledLWhenPrepareReady :
                {
                iCamc->SetAudioEnabledL( TC2_AUDIO_ENABLED );	
                record = ETrue;
                break;
                }
            case KSetMaxClipSizeLWhenPrepareReady :
                {
                iCamc->SetMaxClipSizeL( 10000 );	
                record = ETrue;
                break;
                }
            case KSetVideoTypeLWhenPrepareReady :
                {
                iCamc->SetVideoTypeL( TC2_VIDEO_TYPE  );   
                record = ETrue;
                break;
                }
            case KSetAudioTypeLWhenPrepareReady :
                {
                iCamc->SetAudioTypeL( TC2_AUDIO_TYPE );   
                record = ETrue;
                break;
                }   
            case KSetMaxClipSizeLWhenControllerReady :
                {
                record = ETrue;
                iTimeout = 40 * 1E6; // Wait for 10 seconds.
                break;
                }
            default:
                {
                record = ETrue;
                break;
                }
            }
        if ( record )
            {
            iPrepareReady = EFalse;
            iCamc->Record();
            CTimer::After( iTimeout );
            }
        }
    
    else if ( iRecordingReady )
        {
        TBool closeCam = ETrue;
        switch ( iAction )
            {
            case KcloseWhenRecordingReady:
                {
                iAction = Knone;
                iCamc->Close();		
                closeCam = EFalse;     // False because we want to stop the Active scheduler
                                       // yet
                CTimer::After( 10000 );
                break;
                }
            case  KprepareWhenRecordingReady:
                {
                iAction = Knone;
                iRecordingReady = EFalse;
                iCamc->Prepare();
                closeCam = EFalse;
                // Should start a new recording
                break;
                }
            case  KrecordWhenRecordingReady:
                {
                iAction = Knone;
                iRecordingReady = EFalse;
                iCamc->Record();
                closeCam = EFalse;
                CTimer::After( (TInt) (10*1E6) );    // Starts a new recording
                break;
                }
            default:
                break;
            }
        // ////////////////////////////////////////////////////////////////
        // Check here if the settings were taken into use
        // (For functions with a get)
        
        switch ( iSavedAction )
            {
            case KSetPriorityLWhenPrepareReady :
            case KSetPriorityLWhenRecording:
            case KSetPriorityLWhenControllerReady :	
                {
                TInt priority;
                TMdaPriorityPreference pref;
                iCamc->GetPriorityL(priority, pref);
                __ASSERT_ALWAYS( priority == EMdaPriorityNormal,
                    User::Leave( KErrGetNotEqualToValuePreviouslySet ));
                __ASSERT_ALWAYS( pref == EMdaPriorityPreferenceTime, 
                    User::Leave( KErrGetNotEqualToValuePreviouslySet )); 
                break;
                }          
            
            case KSetVideoFrameRateLWhenPrepareReady :
            case KSetVideoFrameRateLWhenRecording:
            case KSetVideoFrameRateLWhenControllerReady :	
                {
                TReal32 frameRate;
                frameRate = iCamc->VideoFrameRateL();
                __ASSERT_ALWAYS( frameRate ==  TC2_VIDEO_FRAME_RATE ,
                    User::Leave( KErrGetNotEqualToValuePreviouslySet ));
                break;
                }
                
            case KSetVideoFrameSizeLWhenPrepareReady :   // Should have left already
            case KSetVideoFrameSizeLWhenRecording:       // Should have left already
                User::Panic(_L("Should never come here"),0);
                break;
            case KSetVideoFrameSizeLWhenControllerReady :
                {
                TSize frameSize;
                iCamc->GetVideoFrameSizeL( frameSize );
                __ASSERT_ALWAYS( frameSize==  TC2_VIDEO_FRAME_SIZE ,
                    User::Leave( KErrGetNotEqualToValuePreviouslySet ));
                break;
                }           
            case KSetVideoFrameSizeLWhenControllerReady_MPEG4 :
                {
                TSize cifFrameSize;
                iCamc->GetVideoFrameSizeL( cifFrameSize );
                __ASSERT_ALWAYS( cifFrameSize==  TC2_CIF_VIDEO_FRAME_SIZE ,
                    User::Leave( KErrGetNotEqualToValuePreviouslySet ));
                break;
                }           
            case KSetVideoBitRateLWhenPrepareReady :
            case KSetVideoBitRateLWhenRecording:
            case KSetVideoBitRateLWhenControllerReady :	
                {
                TInt bitRate;
                bitRate = iCamc->VideoBitRateL();
                __ASSERT_ALWAYS( bitRate ==  TC2_VIDEO_BIT_RATE ,
                    User::Leave( KErrGetNotEqualToValuePreviouslySet ));
                break;
                }
            case KSetAudioBitRateLWhenPrepareReady :
            case KSetAudioBitRateLWhenControllerReady :	
                {
                TInt bitRate;
                bitRate = iCamc->AudioBitRateL();
                __ASSERT_ALWAYS( bitRate ==  TC2_AUDIO_BIT_RATE ,
                    User::Leave( KErrGetNotEqualToValuePreviouslySet ));
                break;
                }
            case KSetVideoTypeLWhenPrepareReady :  // Should have left already
            case KSetVideoTypeLWhenRecording:      // Should have left already     
                User::Panic(_L("Should never come here"),0);
                break;
            case KSetVideoTypeLWhenControllerReady :	
                {
                 
                TPtrC8 videoType = iCamc->VideoFormatMimeType();
         

                TPtrC8 expectedVideoType[TC2_VIDEO_TYPES_ARRAY_LENGTH] = TC2_VIDEO_TYPES_ARRAY ;
                TBool found = 0;
                for (TInt i=0; i<TC2_VIDEO_TYPES_ARRAY_LENGTH ; i++)
                    {
                    if ( expectedVideoType[i] == videoType)
                        {
                        found = 1;
                        }
                    }
                __ASSERT_ALWAYS( found,
                       User::Leave( KErrGetNotEqualToValuePreviouslySet ));
                break;
                }
            case KSetAudioTypeLWhenPrepareReady :  // Should have left already
            case KSetAudioTypeLWhenRecording:      // Should have left already
               User::Panic(_L("Should never come here"),0);
               break;
            case KSetAudioTypeLWhenControllerReady :	
                {
                TFourCC audioType;
                audioType = iCamc->AudioTypeL();
                __ASSERT_ALWAYS( audioType ==  TC2_AUDIO_TYPE ,
                    User::Leave( KErrGetNotEqualToValuePreviouslySet ));
                break;
                }
            default:
                break;
            }
        
        if (closeCam)
            {
            iRecordingReady = EFalse;

            // Close will panic if called here...
            iCamc->Close();
            CActiveScheduler::Stop();

            }
        }
    else if ( iPaused ) //resume
        {
        iPaused = EFalse;
        iCamc->Record();
        
        }
    else //timer
        {
        switch ( iAction )
            {
            case KcloseWhenRecording:
                {
                iCamc->Close();		
                iClosed = 1;
                break;
                }
            case KprepareWhenRecording:
                {
                // Should leave.
                iAction = Knone;
                iCamc->Prepare();
                break;
                }
            case KrecordWhenRecording:
                {
                // Should be ignored
                iAction = Knone;
                iCamc->Record();
                CTimer::After ( (TInt) 1E6 ); // Wait 1 Second
                break;
                }
            case KSetPriorityLWhenRecording :
                {
                iAction = Knone;
                iCamc->SetPriorityL( EMdaPriorityNormal,EMdaPriorityPreferenceTime );	
                CTimer::After( iTimeout );
                break;
                }
            case KSetVideoFrameRateLWhenRecording:
                {
                iAction = Knone;
                iCamc->SetVideoFrameRateL( TReal32( TC2_VIDEO_FRAME_RATE ) );
                CTimer::After( iTimeout );
                break;
                }
            case KSetVideoFrameSizeLWhenRecording :
                {
                iCamc->SetVideoFrameSizeL( TC2_VIDEO_FRAME_SIZE );
                break;
                }
            case KSetVideoBitRateLWhenRecording :
                {
                iAction = Knone;
                iCamc->SetVideoBitRateL( TC2_VIDEO_BIT_RATE );
                CTimer::After( iTimeout );
                break;
                }
            case KSetAudioBitRateLWhenRecording :
                {
                iAction = Knone;   
                // Should leave
                iCamc->SetAudioBitRateL( TC2_AUDIO_BIT_RATE  );
                // If it has left yet, problem !
                User::Leave(KErrCorrupt);
                CTimer::After( iTimeout );
                break;
                }
            case KSetAudioEnabledLWhenRecording :
                {
                iAction = Knone;
                iCamc->SetAudioEnabledL( TC2_AUDIO_ENABLED );
                CTimer::After( iTimeout );
                break; 
                }
            case KSetMaxClipSizeLWhenRecording:
                {
                iAction = Knone;
                iCamc->SetMaxClipSizeL( 10000 );
                CTimer::After( iTimeout );
                break;
                }
            case KSetVideoTypeLWhenRecording:
                {
                iAction = Knone;
                // Should leave
                iCamc->SetVideoTypeL( TC2_VIDEO_TYPE  ); 
                // If it has left yet, problem !
                User::Leave(KErrCorrupt);
                CTimer::After( iTimeout );
                break;
                }
            case KSetAudioTypeLWhenRecording:
                {
                iAction = Knone;  
                // Should leave
                iCamc->SetAudioTypeL( TC2_AUDIO_TYPE ); 
                // If it has left yet, problem !
                User::Leave(KErrCorrupt); 
                CTimer::After( iTimeout );
                break;
                }
                
            default:
                {
                iCamc->Stop();
                // Simulates that iRecording has been completed through the 
                // MvruoRecordComplete callback
                iRecordingReady=ETrue;				
                CTimer::After( 1000 );
                break;
                }
            }
        }
    
    
    if ( iClosed )
        {
        iCamc->Stop();
        CActiveScheduler::Stop();
        }
}



void CCamcTestClient_2::DoCancel()
    {
    iCamc->Stop();
    iCamc->Close();
    delete iCamc;
    iCamc = NULL;
    }

void CCamcTestClient_2::MvruoOpenComplete( TInt aError )
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

void CCamcTestClient_2::MvruoPrepareComplete( TInt aError )
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

void CCamcTestClient_2::MvruoRecordComplete( TInt aError )
    {  
#ifdef _DEBUG
    RDebug::Print(_L("CamCTestClient 2:Record Complete"));
#endif

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

void CCamcTestClient_2::MvruoEvent(const TMMFEvent& /*aEvent*/) 
    {
    /* Should be never called ... */  
    SomethingInActiveObjectIsLeaving(99);
    }

void CCamcTestClient_2::SomethingInActiveObjectIsLeaving (TInt aError)
    {
    // Something in the active object is leaving
    // or a fatal error has been found.
#ifdef _DEBUG
    RDebug::Print(_L("CamCTestClient 2:SomethingInActiveObjectIsLeaving"));
#endif
    // Stop the active object scheduler.
    iError=aError;
    iCamc->Stop();
    CActiveScheduler::Stop();
    }

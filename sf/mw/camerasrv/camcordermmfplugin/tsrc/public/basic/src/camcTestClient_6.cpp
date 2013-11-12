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

#include "CamcTestClient_6.h"
#include "badesca.h"
#include "camctest.h"
#include <AudioPreference.h>

// EXTERNAL DATA STRUCTURES

// EXTERNAL FUNCTION PROTOTYPES 
 
// CONSTANTS

// MACROS

// LOCAL CONSTANTS AND MACROS

// MODULE DATA STRUCTURES

// LOCAL FUNCTION PROTOTYPES

// FORWARD DECLARATIONS


CCamcTestClient_6::CCamcTestClient_6 () : CTimer(CActive::EPriorityHigh)
    {
    }


CCamcTestClient_6::~CCamcTestClient_6 ()
    {
    Cancel();
    delete iCamc;
    }

CCamcTestClient_6* CCamcTestClient_6::NewL()
    {
    CCamcTestClient_6* self = new(ELeave) CCamcTestClient_6;
    CleanupStack::PushL(self);
    self->ConstructL( );
    CleanupStack::Pop(self);
    return self;
    }

void CCamcTestClient_6::ConstructL( )
    {
    iCamc = CVideoRecorderUtility::NewL( *this, KAudioPriorityVideoRecording, EMdaPriorityPreferenceTimeAndQuality );
    iOpenReady = EFalse;
    iPrepareReady = EFalse;
    iRecordingReady = EFalse;
    iPaused = EFalse;
    iResumeRecording = EFalse;
    
       CTimer::ConstructL();
    iTimeout = 0;
    iClosed = 0;
    CActiveScheduler::Add(this);
    }


void CCamcTestClient_6::Start_Active_ObjectL ( TestClient6Actions aAction, TInt aCameraHandle )
    {
    
    TInt camHandle = aCameraHandle;
    TUid controllerUid = {0x101F8503};	//implementation uid
    TUid formatUid = {0};
    
    iAction = aAction;                  //iAction might be set to NULL after action has been executed
    iSavedAction = aAction;             //SavedAction will never be changed.
    //	iTimeout = aTimeout;
    iTimeout = 1500000;
    
    switch ( iAction )
        {
        case K_Tc6_VideoTypeLWhenOpenWithoutSpecifyingVideoTypeAndNotRecording :
        case K_Tc6_AudioTypeLWhenOpenWithoutSpecifyingAudioTypeAndNotRecording :
        case K_Tc6_SetVideoTypeLWhenOpenWithoutSpecifyingVideoTypeAndNotRecording :
        case K_Tc6_SetAudioTypeLWhenOpenWithoutSpecifyingAudioTypeAndNotRecording :
            {
            TFileName file;
            AddDriveLetterToPath(_L("recordQCIF.3gp"),file);
            TRAPD(err,
                iCamc->OpenFileL( file, camHandle, 
                controllerUid,
                formatUid ));
            
            if (err)
                {
                User::Leave(99);
                }
            break;    
            }
            
        case K_Tc6_VideoTypeLWhenOpenWithNULLAndNotRecording :
            {
            TFileName file;
            AddDriveLetterToPath(_L("recordQCIF.3gp"),file);
            TRAPD(err,
                iCamc->OpenFileL( file, camHandle, 
                controllerUid,
                formatUid,
                _L8("") ));
            
            if (err)
                {
                User::Leave(99);
                }
            break;   
            }
            
        default:
            {
            switch ( iAction )
                {
                case K_Tc6_ControllerImplementationInformationWhenNotOpen:
                    {
                    iCamc->ControllerImplementationInformationL();
                    // Test case should have left already.    
                    
                    // There is a problem if we haven't left already
                    User::Leave(KErrCorrupt);
                    break;
                    }
                case K_Tc6_VideoTypeLWhenOpenWithMimeAndNotRecordingNewFormat:
                    {
                    TFileName file;
                    AddDriveLetterToPath(_L("recordQCIF.3gp"),file);
                    TRAPD(err,
                        iCamc->OpenFileL( file, camHandle, 
                        controllerUid,
                        formatUid,
                        _L8("video/mp4v-es"),
                        TFourCC(' ', 'A', 'M', 'R') ));
                    
                    if (err)
                        {
                        User::Leave(99);
                        }
                    break;
                    }
                case K_Tc6_ControllerImplementationInformationWhenOpenNew_MPEG4:
                case K_Tc6_SetVideoTypeLWithProfile3:
                    {
                    TFileName file;
                    AddDriveLetterToPath(_L("recordQCIF.mp4"),file);
                    TRAPD(err,
                        iCamc->OpenFileL( file, camHandle, 
                        controllerUid,
                        formatUid,
                        _L8("video/mp4v-es; profile-level-id=3"),
                        TFourCC(' ', 'A', 'M', 'R') ));
                    
                    if (err)
                        {
                        User::Leave(99);
                        }
                    break;
                    }

                default:
                    {
                    TFileName file;
                    AddDriveLetterToPath(_L("recordQCIF.3gp"),file);
                    TRAPD(err,
                        iCamc->OpenFileL( file, camHandle, 
                        controllerUid,
                        formatUid,
                        _L8("video/H263-2000"),
                        TFourCC(' ', 'A', 'M', 'R') ));
                    
                    if (err)
                        {
                        User::Leave(99);
                        }
                    break;
                    }
                }
/*            TFileName file;
            AddDriveLetterToPath(_L("recordQCIF.3gp"),file);
            TRAPD(err,
                iCamc->OpenFileL( file, camHandle, 
                controllerUid,
                formatUid,
                _L8("video/H263-2000"),
                TFourCC(' ', 'A', 'M', 'R') ));
            
            if (err)
                {
                User::Leave(99);
                }*/
            }
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

void CCamcTestClient_6::RunL()
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

void CCamcTestClient_6::RunLTrappedL()
    {
    
    if ( iOpenReady ) 
        {
        iOpenReady = EFalse;
        
        // Set default parameters. They can be overwritten depending on the test case. 
        iCamc->SetVideoFrameSizeL(TSize(176,144));
        iCamc->SetVideoFrameRateL(TReal32(15));
        iCamc->SetVideoBitRateL(TInt(50000));
        iCamc->SetAudioEnabledL(EFalse); 
        
        TBool prepare = ETrue;
        switch ( iAction )
            {
            case K_Tc6_SetAudioEnabledLWhenControllerReady :
                {
                iCamc->SetAudioEnabledL( ETrue );	
                break;
                }
            case K_Tc6_SetMaxClipSizeLWhenControllerReady :
                {
                // -55 is invalide size
                iCamc->SetMaxClipSizeL( -55 );	
                
                // Should have left already
                // Don't go on, to prevent a panic
                User::Leave(K_Tc6_ErrExpectingToLeave);
                break;
                }                
            case K_Tc6_VideoTypeLWhenOpenWithMimeAndNotRecording :   
                {
                
                TPtrC8 returnedVideoType = iCamc->VideoFormatMimeType();
                TBool found = 0;
                TPtrC8 expectedVideoType[TC6_VIDEO_TYPES_ARRAY_LENGTH] = TC6_VIDEO_TYPES_ARRAY ;
                for (TInt i=0; i<TC6_VIDEO_TYPES_ARRAY_LENGTH ; i++)
                    {
                    if (expectedVideoType[i] == returnedVideoType)
                        {
                        found = 1;
                        }
                    }
                if (!found)
                    {
                    User::Leave(K_Tc6_ErrExpectingADifferentValue);
                    }
                break;
                }
            case K_Tc6_VideoTypeLWhenOpenWithMimeAndNotRecordingNewFormat :   
                {
                
                TPtrC8 returnedVideoType = iCamc->VideoFormatMimeType();
                TBool found = 0;
                TPtrC8 expectedVideoType[TC6_VIDEO_TYPES_ARRAY_LENGTH_NEW] = TC6_VIDEO_TYPES_ARRAY_NEW ;
                for (TInt i=0; i<TC6_VIDEO_TYPES_ARRAY_LENGTH_NEW ; i++)
                    {
                    if (expectedVideoType[i] == returnedVideoType)
                        {
                        found = 1;
                        }
                    }
                if (!found)
                    {
                    User::Leave(K_Tc6_ErrExpectingADifferentValue);
                    }
                break;
                }
            case K_Tc6_VideoTypeLWhenOpenWithoutSpecifyingVideoTypeAndNotRecording :
            case K_Tc6_VideoTypeLWhenOpenWithNULLAndNotRecording :
                {
                TPtrC8 returnedVideoType = iCamc->VideoFormatMimeType();
                TBool found = 0;
                TPtrC8 expectedVideoType[TC6_VIDEO_TYPES_ARRAY_LENGTH] = TC6_VIDEO_TYPES_ARRAY ;
                for (TInt i=0; i<TC6_VIDEO_TYPES_ARRAY_LENGTH ; i++)
                    {
                    if (expectedVideoType[i] == returnedVideoType)
                        {
                        found = 1;
                        }
                    }
                if (!found)
                    {
                    User::Leave(K_Tc6_ErrExpectingADifferentValue);
                    }
                break;
                }
                
            case K_Tc6_AudioTypeLWhenOpenWithfourCCAndNotRecording :
            case K_Tc6_AudioTypeLWhenOpenWithoutSpecifyingAudioTypeAndNotRecording :
                {
                TFourCC expectedAudioType = TFourCC( ' ', 'A', 'M', 'R' );
                TFourCC returnedAudioType = iCamc->AudioTypeL(  );
                if (expectedAudioType != returnedAudioType)
                    {
                    User::Leave(K_Tc6_ErrExpectingADifferentValue);
                    }
                break;
                }                           
            case K_Tc6_SetVideoTypeLWhenOpenWithoutSpecifyingVideoTypeAndNotRecording:
                {
                iCamc->SetVideoTypeL( TC6_VIDEO_TYPE );
                break;
                }                
            case K_Tc6_SetVideoTypeLWhenNotRecording :
                {
                iCamc->SetVideoTypeL( TC6_VIDEO_TYPE_WRONG );
                break;
                }               
            case K_Tc6_SetVideoTypeLWithProfile :
                {
                iCamc->SetVideoTypeL( TC6_VIDEO_TYPE_PROFILE0 );
                break;
                }         
            case K_Tc6_SetVideoTypeLWithProfile8 :
                {
                iCamc->SetVideoTypeL( TC6_VIDEO_TYPE_PROFILE8 );

                TPtrC8 returnedVideoType = iCamc->VideoFormatMimeType();
                TBool found = 0;
                TPtrC8 expectedVideoType[TC6_VIDEO_TYPES_ARRAY_LENGTH_NEW] = TC6_VIDEO_TYPES_ARRAY_NEW ;
                for (TInt i=0; i<TC6_VIDEO_TYPES_ARRAY_LENGTH_NEW ; i++)
                    {
                    if (expectedVideoType[i] == returnedVideoType)
                        {
                        found = 1;
                        }
                    }
                if (!found)
                    {
                    User::Leave(K_Tc6_ErrExpectingADifferentValue);
                    }
                
                break;
                }         
            case K_Tc6_SetVideoTypeLWithProfile3 :
                {
                iCamc->SetVideoTypeL( TC6_VIDEO_TYPE_PROFILE3 );

                TPtrC8 returnedVideoType = iCamc->VideoFormatMimeType();
                TBool found = 0;
                TPtrC8 expectedVideoType[TC6_VIDEO_TYPES_ARRAY_LENGTH_NEW] = TC6_VIDEO_TYPES_ARRAY_NEW ;
                for (TInt i=0; i<TC6_VIDEO_TYPES_ARRAY_LENGTH_NEW ; i++)
                    {
                    if (expectedVideoType[i] == returnedVideoType)
                        {
                        found = 1;
                        }
                    }
                if (!found)
                    {
                    User::Leave(K_Tc6_ErrExpectingADifferentValue);
                    }
                
                // set CIF size since level = 3
                iCamc->SetVideoFrameSizeL( TC6_CIF_VIDEO_FRAME_SIZE );

                break;
                }         
            case K_Tc6_SetAudioTypeLWhenOpenWithoutSpecifyingAudioTypeAndNotRecording :
                {
                iCamc->SetAudioTypeL( TC6_AUDIO_TYPE );
                break;
                }       
            case K_Tc6_SetAudioTypeLWhenOpenWithfourCCAndNotRecording_AAC :
                {
                iCamc->SetAudioTypeL( TC6_AAC_AUDIO_TYPE );
                break;
                }            
            case K_Tc6_SetAudioTypeLWhenNotRecording :
                {
                iCamc->SetAudioTypeL( TC6_AUDIO_TYPE_WRONG );
                break;
                }
            case K_Tc6_ControllerImplementationInformationWhenOpen:
                { 
                const CMMFControllerImplementationInformation & returnedResult = iCamc->ControllerImplementationInformationL();
                CheckControllerImplementationInformationL ( returnedResult ); 
                break;
                }
            case K_Tc6_ControllerImplementationInformationWhenOpenNew:
                { 
                const CMMFControllerImplementationInformation & returnedResult = iCamc->ControllerImplementationInformationL();
                CheckControllerImplementationInformationNewL ( returnedResult ); 
                break;
                }
            case K_Tc6_ControllerImplementationInformationWhenOpenNew_MPEG4:
                { 
                const CMMFControllerImplementationInformation & returnedResult = iCamc->ControllerImplementationInformationL();
                CheckControllerImplementationInformationNewMPEG4L ( returnedResult ); 

                // set CIF size since level = 3
                iCamc->SetVideoFrameSizeL( TC6_CIF_VIDEO_FRAME_SIZE );

                break;
                }
            case K_Tc6_DurationWhenRecording:
                {
                // Use a more realistic frame rate
                // The duration test is quite affected by AO scheduling.
                iCamc->SetVideoFrameRateL(TReal32(5));
#if ( defined ( __WINS__ ) || defined ( __WINSCW__) )
                // Give some help to the wins case.
                iCamc->SetAudioEnabledL(EFalse); 
#endif
                break;
                }

            default:
                break;
            }
            if ( prepare )           
                {
                iCamc->Prepare();
                }
        }
        else if ( iPrepareReady ) 
            {    
            switch (iAction)
                {
                case K_Tc6_SetAudioEnabledLWhenControllerReady :
                    {
                    //Compare
                    TBool aAudioEnabled;
                    aAudioEnabled = iCamc->AudioEnabledL();
                    //aSetAudioEnable should be the same with previously set
                    if (!aAudioEnabled )
                        {
                        User::Leave(K_Tc6_ErrExpectingADifferentValue);
                        }
                    break;
                    }
                case K_Tc6_SetVideoTypeLWhenOpenWithoutSpecifyingVideoTypeAndNotRecording :
                    //Compare
                    {
                    TPtrC8 returnedSetVideoType = iCamc->VideoFormatMimeType();
                    TBool found = 0;
                    TPtrC8 expectedVideoType[TC6_VIDEO_TYPES_ARRAY_LENGTH] = TC6_VIDEO_TYPES_ARRAY ;
                    for (TInt i=0; i<TC6_VIDEO_TYPES_ARRAY_LENGTH ; i++)
                        {
                        if (expectedVideoType[i] == returnedSetVideoType)
                            {
                            found = 1;
                            }
                        }
                    if (!found)
                        {
                        User::Leave(K_Tc6_ErrExpectingADifferentValue);
                        }
                    
                    break;
                    }                
                case K_Tc6_SetAudioTypeLWhenOpenWithoutSpecifyingAudioTypeAndNotRecording :
                    //Compare
                    {
                    TFourCC returnedSetAudioType;
                    returnedSetAudioType = iCamc->AudioTypeL( );
                    if (returnedSetAudioType != TC6_AUDIO_TYPE)
                        {
                        User::Leave(K_Tc6_ErrExpectingADifferentValue);
                        }
                    break;
                    }               
                case K_Tc6_SetAudioTypeLWhenOpenWithfourCCAndNotRecording_AAC :
                    //Compare
                    {
                    TFourCC returnedSetAudioType;
                    returnedSetAudioType = iCamc->AudioTypeL( );
                    if (returnedSetAudioType != TC6_AAC_AUDIO_TYPE)
                        {
                        User::Leave(K_Tc6_ErrExpectingADifferentValue);
                        }
                    break;
                    }
                case K_Tc6_DurationWhenNotRecording:
                    {
                    TTimeIntervalMicroSeconds aValue = iCamc->DurationL();
                    TTimeIntervalMicroSeconds expected (0);
                    //aValue should always be 0. 
                    if (aValue != expected)
                        {
                        User::Leave(K_Tc6_ErrExpectingADifferentValue);
                        }
                    break;
                    }
                default:
                    break;
                }
            
            switch (iAction)
                {   
                case K_Tc6_SetMaxClipSizeLWhenPrepareReady :
                    {
                    iCamc->SetMaxClipSizeL( TC6_FILE_SIZE_LIMIT );	
                    iAction = K_Tc6_none;
                    iCamc->Prepare();
                    break;
                    }
                case K_Tc6_RecordWhenPrepareComplete:
                    {
                    iPrepareReady = EFalse;
                    iCamc->Record();
                    break;
                    }
                case K_Tc6_DurationWhenPaused:
                    {
                    iPrepareReady = EFalse;
                    iCamc->Record();
                    CTimer::After(5 * TInt32 ( 1E6 ) ); // Record for 5 seconds.
                    break;
                    }
                default:
                    {
                    iPrepareReady = EFalse;
                    iCamc->Record();
                    if (iSavedAction != K_Tc6_SetMaxClipSizeLWhenPrepareReady)
                        {
                        CTimer::After( iTimeout );
                        }
                    }
                }
        }
        
        else if ( iRecordingReady )
            {
            switch (iAction)
                {
                case K_Tc6_PauseWhenStopped:
                    {
                    iCamc->PauseL();
                    iAction = K_Tc6_none;
                    break;
                    }
                default:   
                    iRecordingReady = EFalse;                    
                    iCamc->Close();
                    iClosed = ETrue;
                }
            CTimer::After( 1000 );        
            }
        
        else if ( iPaused ) //resume
            {
            switch (iAction)
                {
                case K_Tc6_SetVideoFrameSizeLWhenPaused:
                    {
                    iCamc->SetVideoFrameSizeL( TC6_VIDEO_FRAME_SIZE );
                    break;
                    }
                case K_Tc6_RecordWhenPaused:
                    {
                    iCamc->Record();
                    CTimer::After( 10 * TInt32 ( 1E6 ) );
                    iPaused = EFalse;
                    iAction = K_Tc6_none;         
                    break;
                    }
                case K_Tc6_DurationWhenPaused:
                    {
                    iNumberOfTimeAsked++;
                    TTimeIntervalMicroSeconds timeDuration = iCamc->DurationL();                   
                    if (( iNumberOfTimeAsked != 0) && (iOldDuration > timeDuration))
                        {
                        User::Leave(K_Tc6_ErrExpectingADifferentValue);
                        }
                    
                    if (iNumberOfTimeAsked <3)
                        {
                        CTimer::After( 1 * TInt32 ( 1E6 ) );
                        }
                    else
                        {
                        iPaused = EFalse;
                        iCamc->Record();
                        iResumeRecording = ETrue;
                        CTimer::After( 1 * TInt32 ( 1E6 ) );  
                        }
                    iOldDuration = timeDuration;
                    break;
                    }
                default:
                    iPaused = EFalse;
                    iCamc->Record();
                }
            }
        
        else //timer
            {
            switch (iAction)
                {
                case K_Tc6_SetVideoFrameSizeLWhenPaused:
                    {
                    iCamc->PauseL();
                    iPaused = ETrue;
                    CTimer::After( 30 * TInt32 ( 1E6 ) );
                    break;
                    }
                case K_Tc6_RecordWhenPaused:
                    {
                    iCamc->PauseL();
                    iPaused = ETrue;
                    CTimer::After( 10 * TInt32 ( 1E6 ) );
                    break;
                    }
                case K_Tc6_VideoTypeLWhenOpenWithMimeAndRecording :
                    {
                    TPtrC8 returnedVideoType = iCamc->VideoFormatMimeType();
                    TBool found = 0;
                    TPtrC8 expectedVideoType[TC6_VIDEO_TYPES_ARRAY_LENGTH] = TC6_VIDEO_TYPES_ARRAY ;
                    for (TInt i=0; i<TC6_VIDEO_TYPES_ARRAY_LENGTH ; i++)
                        {
                        if (expectedVideoType[i] == returnedVideoType)
                            {
                            found = 1;
                            }
                        }
                    if (!found)
                        {
                        User::Leave(K_Tc6_ErrExpectingADifferentValue);
                        }
                    
                    
                    CTimer::After( 1 * TInt32 ( 1E6 ) );
                    iAction = K_Tc6_none; 
                    break;
                    }               
                case K_Tc6_AudioTypeLWhenOpenWithfourCCAndRecording :
                    {
                    TFourCC expectedAudioType = TFourCC( ' ', 'A', 'M', 'R' );
                    TFourCC returnedAudioType = iCamc->AudioTypeL(  );
                    if (expectedAudioType != returnedAudioType)
                        {
                        User::Leave(K_Tc6_ErrExpectingADifferentValue);
                        }
                    CTimer::After( 1 * TInt32 ( 1E6 ) );
                    iAction = K_Tc6_none; 
                    break;
                    }
                case K_Tc6_DurationWhenRecording:
                    {
                    TTimeIntervalMicroSeconds timeDuration = iCamc->DurationL();                   
#ifdef _DEBUG
                    TBuf<128> str;
                    str.Format(_L("Asking for time duration %i:%i"), I64HIGH(timeDuration.Int64()),
                        I64LOW(timeDuration.Int64()));
                    RDebug::Print(str);    
#endif
                    if (( iNumberOfTimeOut != 0) && (iOldTimeDuration > timeDuration))
                        {
                        RDebug::Print(_L("Leaving because not increasing"));
                        User::Leave(K_Tc6_ValueNotIncreasingErr);
                        }
                    
                    TInt64 expected_duration;
                    TTime time;
                    time.UniversalTime(); // Get the real elapsed time
                    
                    if (iNumberOfTimeOut == 0)
                        {
                        // First wait period is 1.5 seconds.
                        // The AO are quite inaccurate, especially at the beginning
                        // We don't check the first run.
                        }
                    else
                        {
                        // Next wait period are 1 seconds
                        // But are not accurate, because of low AO priority.
                        TInt64 delta_time  = time.Int64() - iPreviousTime ;
                        expected_duration = iOldTimeDuration.Int64() + delta_time;
                        
                        TTimeIntervalMicroSeconds kTimeLimit( expected_duration );
                       
                        TTimeIntervalMicroSeconds kTimeMargin( 300000 ); //0.3 second
                        
                        if ( ( timeDuration > kTimeLimit.Int64() + kTimeMargin.Int64() ) ||
                            ( timeDuration < kTimeLimit.Int64() - kTimeMargin.Int64() ) ) 
                            {     
                            RDebug::Print(_L("Leaving because not within margins"));
                            TBuf<128> aBuf;
                            aBuf.Format(_L("Actually elapsed time since last call: %i \nexpected duration: %i:%i "),
                                I64LOW(delta_time),I64HIGH(expected_duration),
                                I64LOW(expected_duration));
                            RDebug::Print(aBuf);
                            User::Leave(K_Tc6_ErrExpectingADifferentValue); 
                            }
                        }
                    
                    
                    iPreviousTime = time.Int64();
                    iOldTimeDuration = timeDuration;
                    
                    
                    if (iNumberOfTimeOut >20)
                        {
                        iCamc->Stop();
                        // Simulates that iRecording has been completed through the 
                        // MvruoRecordComplete callback
                        iRecordingReady=ETrue;				
                        CTimer::After( 1 * TInt32 ( 1E6 ) );
                        }
                    else
                        {
                        // Wait  a second and ask for the duration again.
                        CTimer::After( 1 * TInt32 ( 1E6 ) ); /* a second */
                        }
                    iNumberOfTimeOut++;
                    break;
                    }
                case K_Tc6_DurationWhenPaused:
                    {
                    if (iResumeRecording)
                        {
                        TTimeIntervalMicroSeconds timeDuration = iCamc->DurationL();                   
                        #ifdef _DEBUG
                        TBuf<128> str;
                        str.Format(_L("Time duration %x:%i"), I64HIGH(timeDuration.Int64()),
                            I64LOW(timeDuration.Int64()));
                        RDebug::Print(str);    
                        #endif

                        TInt64 delta_time = 1E6;
                       
                        TInt64 expected_duration = iOldDuration.Int64() + delta_time;
                        
                        TTimeIntervalMicroSeconds kTimeLimit( expected_duration );
                       
                        TTimeIntervalMicroSeconds kTimeMargin( 300000 ); //0.3 second
                        
                        if ( ( timeDuration > kTimeLimit.Int64() + kTimeMargin.Int64() ) ||
                           ( timeDuration < kTimeLimit.Int64() - kTimeMargin.Int64() ) ) 
                            { 
                            User::Leave(K_Tc6_ErrExpectingADifferentValue); 
                            }            
                        
                        iAction = K_Tc6_none; 
                        iCamc->Stop();
                        iRecordingReady=ETrue;				
                        CTimer::After( 1 * TInt32 ( 1E6 ) );
                        }
                    else
                        {
                        iCamc->PauseL();
                        iRecordingReady = EFalse;
                        CTimer::After( 5 * TInt32 ( 1E6 ) ); // Pause for 5 seconds.
                        iPaused=ETrue;
                        iResumeRecording = EFalse;
                        }
                    break;
                    }
                default:
                    iCamc->Stop();
                    // Simulates that iRecording has been completed through the 
                    // MvruoRecordComplete callback
                    iRecordingReady=ETrue;				
                    CTimer::After( 1 * TInt32 ( 1E6 ) );
            }
        }
        
        if ( iClosed )
            {
            iCamc->Stop();
            CActiveScheduler::Stop();
            }
        
    }
    
void CCamcTestClient_6::CheckControllerImplementationInformationL(const CMMFControllerImplementationInformation & aReturnedResult )
    {
    // Check if returned controller implementation information correspond
    // to what we expect
    
    
    // Check for the supported play formats:
    {
    const RMMFFormatImplInfoArray& formatArray = aReturnedResult.PlayFormats();
    TInt nbItems;
    nbItems = formatArray.Count();
    
    // Video recorder doesn't support any Play formats.
    if ( nbItems != 0 )
        {
        User::Leave(K_Tc6_ErrExpectingADifferentValue); 
        }
    
    }
    // Check for the supported Recorded Formats
        
    {
    const RMMFFormatImplInfoArray& formatArray = aReturnedResult.RecordFormats();
    TInt nbItems;
    nbItems = formatArray.Count();
    
    if ( nbItems !=1 )
        {
        User::Leave(K_Tc6_ErrExpectingADifferentValue); 
        }
    
    TBuf<100> name;
    // Format name
    name = formatArray[0]->DisplayName();
    if ((name != _L("3GPP File Format")))
        {
        User::Leave(K_Tc6_ErrExpectingADifferentValue); 
        }
    
    //Format UID
    name = formatArray[0]->Uid().Name();
    if (name != _L("[101f8504]"))
        {
        User::Leave(K_Tc6_ErrExpectingADifferentValue); 
        }
    
    //Format Supplier   
    name = formatArray[0]->Supplier();              
    if (name != _L("Nokia"))
        {
        User::Leave(K_Tc6_ErrExpectingADifferentValue); 
        }
    //Format Version
    if (formatArray[0]->Version() != 0)           
        {
        User::Leave(K_Tc6_ErrExpectingADifferentValue); 
        }
    
    // Supported Medias UID by the Format
    {
        
    const RArray<TUid> & supportedUids = formatArray[0]->SupportedMediaIds();
    // Only one format UID is currently supported
    if (supportedUids.Count() != 1)
        {
        User::Leave(K_Tc6_ErrExpectingADifferentValue); 
        }
    name = supportedUids[0].Name();
    if (name != _L("[101f5d08]"))
        {
        User::Leave(K_Tc6_ErrExpectingADifferentValue); 
        }
    
    }
    
    //Supported Files extensions
    {
    const CDesC8Array & names =
        formatArray[0]->SupportedFileExtensions();
    if (names.Count() != 1)
        {
        User::Leave(K_Tc6_ErrExpectingADifferentValue); 
        }
    if (names[0] != _L8(".3gp"))
        {
        User::Leave(K_Tc6_ErrExpectingADifferentValue); 
        }
    }
    //Supported Mime types
    {
    const CDesC8Array & names =
        formatArray[0]->SupportedMimeTypes();
    if (names.Count() != 1)
        {
        User::Leave(K_Tc6_ErrExpectingADifferentValue); 
        }
    if (names[0] != _L8("video/3gpp"))
        {
        User::Leave(K_Tc6_ErrExpectingADifferentValue); 
        }
    
    }   
    }
}
    
   
void CCamcTestClient_6::CheckControllerImplementationInformationNewL(const CMMFControllerImplementationInformation & aReturnedResult )
    {
    // Check if returned controller implementation information correspond
    // to what we expect
    
    
    // Check for the supported play formats:
    {
    const RMMFFormatImplInfoArray& formatArray = aReturnedResult.PlayFormats();
    TInt nbItems;
    nbItems = formatArray.Count();
    
    // Video recorder doesn't support any Play formats.
    if ( nbItems != 0 )
        {
        User::Leave(K_Tc6_ErrExpectingADifferentValue); 
        }
    
    }
    // Check for the supported Recorded Formats
        
    {
    const RMMFFormatImplInfoArray& formatArray = aReturnedResult.RecordFormats();
    TInt nbItems;
    nbItems = formatArray.Count();
    
    if ( nbItems !=2 )
        {
        User::Leave(K_Tc6_ErrExpectingADifferentValue); 
        }
    
    TBuf<100> name;
    // Format name
    name = formatArray[0]->DisplayName();
    if ((name != _L("3GPP File Format"))) 
        {
        User::Leave(K_Tc6_ErrExpectingADifferentValue); 
        }

    name = formatArray[1]->DisplayName();
    if ((name != _L("3GPP2 File Format"))) 
        {
        User::Leave(K_Tc6_ErrExpectingADifferentValue); 
        }
    
    //Format UID
    name = formatArray[0]->Uid().Name();
    if (name != _L("[101f8504]"))
        {
        User::Leave(K_Tc6_ErrExpectingADifferentValue); 
        }
    name = formatArray[1]->Uid().Name();
//    if (name != _L("[101F86D6]"))
//        {
//        User::Leave(K_Tc6_ErrExpectingADifferentValue); 
//        }
    
    //Format Supplier   
    name = formatArray[0]->Supplier();              
    if (name != _L("Nokia"))
        {
        User::Leave(K_Tc6_ErrExpectingADifferentValue); 
        }

    name = formatArray[1]->Supplier();              
    if (name != _L("Nokia"))
        {
        User::Leave(K_Tc6_ErrExpectingADifferentValue); 
        }
    //Format Version
    if (formatArray[0]->Version() != 0)           
        {
        User::Leave(K_Tc6_ErrExpectingADifferentValue); 
        }

    if (formatArray[1]->Version() != 0)           
        {
        User::Leave(K_Tc6_ErrExpectingADifferentValue); 
        }
 
    
    // Supported Medias UID by the Format
    {        
    const RArray<TUid> & supportedUids = formatArray[0]->SupportedMediaIds();
    // 3gpp
    if (supportedUids.Count() != 1)
        {
        User::Leave(K_Tc6_ErrExpectingADifferentValue); 
        }
    name = supportedUids[0].Name();
    if (name != _L("[101f5d08]"))
        {
        User::Leave(K_Tc6_ErrExpectingADifferentValue); 
        }    
    }
    {
    const RArray<TUid> & supportedUids = formatArray[1]->SupportedMediaIds();
    // 3gpp2
    if (supportedUids.Count() != 1)
        {
        User::Leave(K_Tc6_ErrExpectingADifferentValue); 
        }
    name = supportedUids[0].Name();
    if (name != _L("[101f5d08]"))
        {
        User::Leave(K_Tc6_ErrExpectingADifferentValue); 
        }    
    }
    

    //Supported Files extensions
    {
    // 3gpp
    const CDesC8Array & names =
        formatArray[0]->SupportedFileExtensions();
    if (names.Count() != 1)
        {
        User::Leave(K_Tc6_ErrExpectingADifferentValue); 
        }
    if (names[0] != _L8(".3gp"))
        {
        User::Leave(K_Tc6_ErrExpectingADifferentValue); 
        }
    }
    {
    // 3gpp2
    const CDesC8Array & names =
        formatArray[1]->SupportedFileExtensions();
    if (names.Count() != 1)
        {
        User::Leave(K_Tc6_ErrExpectingADifferentValue); 
        }
    if (names[0] != _L8(".3g2"))
        {
        User::Leave(K_Tc6_ErrExpectingADifferentValue); 
        }
    }

    
    //Supported Mime types
    {
    // 3gpp
    const CDesC8Array & names =
        formatArray[0]->SupportedMimeTypes();
    if (names.Count() != 1)
        {
        User::Leave(K_Tc6_ErrExpectingADifferentValue); 
        }
    if (names[0] != _L8("video/3gpp"))
        {
        User::Leave(K_Tc6_ErrExpectingADifferentValue); 
        }
    }  
    {
    // 3gpp2
    const CDesC8Array & names =
        formatArray[1]->SupportedMimeTypes();
    if (names.Count() != 1)
        {
        User::Leave(K_Tc6_ErrExpectingADifferentValue); 
        }
    if (names[0] != _L8("video/3gpp2"))
        {
        User::Leave(K_Tc6_ErrExpectingADifferentValue); 
        }    
    }  

    }
}
    
   
void CCamcTestClient_6::CheckControllerImplementationInformationNewMPEG4L(const CMMFControllerImplementationInformation & aReturnedResult )
    {
    // Check if returned controller implementation information correspond
    // to what we expect
    
    
    // Check for the supported play formats:
    {
    const RMMFFormatImplInfoArray& formatArray = aReturnedResult.PlayFormats();
    TInt nbItems;
    nbItems = formatArray.Count();
    
    // Video recorder doesn't support any Play formats.
    if ( nbItems != 0 )
        {
        User::Leave(K_Tc6_ErrExpectingADifferentValue); 
        }
    
    }
    // Check for the supported Recorded Formats
        
    {
    const RMMFFormatImplInfoArray& formatArray = aReturnedResult.RecordFormats();
    TInt nbItems;
    nbItems = formatArray.Count();
    
    if ( nbItems !=3 )
        {
        User::Leave(K_Tc6_ErrExpectingADifferentValue); 
        }
    
    TBuf<100> name;
    // Format name
    name = formatArray[0]->DisplayName();
    if ((name != _L("3GPP File Format"))) 
        {
        User::Leave(K_Tc6_ErrExpectingADifferentValue); 
        }

    name = formatArray[1]->DisplayName();
    if ((name != _L("3GPP2 File Format"))) 
        {
        User::Leave(K_Tc6_ErrExpectingADifferentValue); 
        }
 
    name = formatArray[2]->DisplayName();
    if ((name != _L("MPEG-4 File Format"))) 
        {
        User::Leave(K_Tc6_ErrExpectingADifferentValue); 
        }
    
    //Format UID
    name = formatArray[0]->Uid().Name();
    if (name != _L("[101f8504]"))
        {
        User::Leave(K_Tc6_ErrExpectingADifferentValue); 
        }
    name = formatArray[1]->Uid().Name();
    name = formatArray[2]->Uid().Name();
//    if (name != _L("[101F86D6]"))
//        {
//        User::Leave(K_Tc6_ErrExpectingADifferentValue); 
//        }
    
    //Format Supplier   
    name = formatArray[0]->Supplier();              
    if (name != _L("Nokia"))
        {
        User::Leave(K_Tc6_ErrExpectingADifferentValue); 
        }

    name = formatArray[1]->Supplier();              
    if (name != _L("Nokia"))
        {
        User::Leave(K_Tc6_ErrExpectingADifferentValue); 
        }
    
    name = formatArray[2]->Supplier();              
    if (name != _L("Nokia"))
        {
        User::Leave(K_Tc6_ErrExpectingADifferentValue); 
        }

    //Format Version
    if (formatArray[0]->Version() != 0)           
        {
        User::Leave(K_Tc6_ErrExpectingADifferentValue); 
        }

    if (formatArray[1]->Version() != 0)           
        {
        User::Leave(K_Tc6_ErrExpectingADifferentValue); 
        }
 
    if (formatArray[2]->Version() != 0)           
        {
        User::Leave(K_Tc6_ErrExpectingADifferentValue); 
        }
    
    // Supported Medias UID by the Format
    {        
    const RArray<TUid> & supportedUids = formatArray[0]->SupportedMediaIds();
    // 3gpp
    if (supportedUids.Count() != 1)
        {
        User::Leave(K_Tc6_ErrExpectingADifferentValue); 
        }
    name = supportedUids[0].Name();
    if (name != _L("[101f5d08]"))
        {
        User::Leave(K_Tc6_ErrExpectingADifferentValue); 
        }    
    }
    {
    const RArray<TUid> & supportedUids = formatArray[1]->SupportedMediaIds();
    // 3gpp2
    if (supportedUids.Count() != 1)
        {
        User::Leave(K_Tc6_ErrExpectingADifferentValue); 
        }
    name = supportedUids[0].Name();
    if (name != _L("[101f5d08]"))
        {
        User::Leave(K_Tc6_ErrExpectingADifferentValue); 
        }    
    }
    {
    const RArray<TUid> & supportedUids = formatArray[2]->SupportedMediaIds();
    // mp4
    if (supportedUids.Count() != 1)
        {
        User::Leave(K_Tc6_ErrExpectingADifferentValue); 
        }
    name = supportedUids[0].Name();
    if (name != _L("[101f5d08]"))
        {
        User::Leave(K_Tc6_ErrExpectingADifferentValue); 
        }    
    }
    

    //Supported Files extensions
    {
    // 3gpp
    const CDesC8Array & names =
        formatArray[0]->SupportedFileExtensions();
    if (names.Count() != 1)
        {
        User::Leave(K_Tc6_ErrExpectingADifferentValue); 
        }
    if (names[0] != _L8(".3gp"))
        {
        User::Leave(K_Tc6_ErrExpectingADifferentValue); 
        }
    }
    {
    // 3gpp2
    const CDesC8Array & names =
        formatArray[1]->SupportedFileExtensions();
    if (names.Count() != 1)
        {
        User::Leave(K_Tc6_ErrExpectingADifferentValue); 
        }
    if (names[0] != _L8(".3g2"))
        {
        User::Leave(K_Tc6_ErrExpectingADifferentValue); 
        }
    }
    {
    // mp4
    const CDesC8Array & names =
        formatArray[2]->SupportedFileExtensions();
    if (names.Count() != 1)
        {
        User::Leave(K_Tc6_ErrExpectingADifferentValue); 
        }
    if (names[0] != _L8(".mp4"))
        {
        User::Leave(K_Tc6_ErrExpectingADifferentValue); 
        }
    }

    
    //Supported Mime types
    {
    // 3gpp
    const CDesC8Array & names =
        formatArray[0]->SupportedMimeTypes();
    if (names.Count() != 1)
        {
        User::Leave(K_Tc6_ErrExpectingADifferentValue); 
        }
    if (names[0] != _L8("video/3gpp"))
        {
        User::Leave(K_Tc6_ErrExpectingADifferentValue); 
        }
    }  
    {
    // 3gpp2
    const CDesC8Array & names =
        formatArray[1]->SupportedMimeTypes();
    if (names.Count() != 1)
        {
        User::Leave(K_Tc6_ErrExpectingADifferentValue); 
        }
    if (names[0] != _L8("video/3gpp2"))
        {
        User::Leave(K_Tc6_ErrExpectingADifferentValue); 
        }    
    }  
    {
    // mp4
    const CDesC8Array & names =
        formatArray[2]->SupportedMimeTypes();
    if (names.Count() != 1)
        {
        User::Leave(K_Tc6_ErrExpectingADifferentValue); 
        }
    if (names[0] != _L8("video/mp4"))
        {
        User::Leave(K_Tc6_ErrExpectingADifferentValue); 
        }    
    }  

    }
}

void CCamcTestClient_6::DoCancel()
    {
    iCamc->Stop();
    iCamc->Close();
    delete iCamc;
    iCamc = NULL;
    }

void CCamcTestClient_6::MvruoOpenComplete( TInt aError )
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

void CCamcTestClient_6::MvruoPrepareComplete( TInt aError )
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

void CCamcTestClient_6::MvruoRecordComplete( TInt aError )
    {
    
    if (iAction == K_Tc6_RecordWhenPrepareComplete )
        {
        if ( (aError ==  KErrDiskFull) || (aError == KErrCompletion ) ) 
            {
            // KErrCompletion if there is more space than needed for 60 min recording.
            // Otherwise KErrDiskFull
            aError = KErrNone;
            }
        else
            {
            aError = KErrGeneral;
            }
        }
    
    if ( ( aError ) && ( aError != KErrCompletion ) )
        {
        SomethingInActiveObjectIsLeaving(aError);
        }
    else
        {
        iRecordingReady = ETrue;
        RunL(); // RunL is trapped, cannot actually leave.
        }
    }

void CCamcTestClient_6::MvruoEvent(const TMMFEvent& /*aEvent*/) 
    {
    /* Should be never called ... */  
    SomethingInActiveObjectIsLeaving(99);
    }

void CCamcTestClient_6::SomethingInActiveObjectIsLeaving (TInt aError)
    {
    // Something in the active object is leaving
    // or a fatal error has been found.
    
    // Stop the active object scheduler.
    iError=aError;
    iCamc->Stop();
    CActiveScheduler::Stop();
    }

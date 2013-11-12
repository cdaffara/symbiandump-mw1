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

#include "camctestclient_5.h"
#include "badesca.h"
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



CCamcTestClient_5::CCamcTestClient_5 () : CTimer(CActive::EPriorityHigh)
    {
    }


CCamcTestClient_5::~CCamcTestClient_5 ()
    {
    Cancel();
    delete iCamc;
    delete iMMFMetaDataEntry;
    }

CCamcTestClient_5* CCamcTestClient_5::NewL()
    {
    CCamcTestClient_5* self = new(ELeave) CCamcTestClient_5;
    CleanupStack::PushL(self);
    self->ConstructL( );
    CleanupStack::Pop(self);
    return self;
    }

void CCamcTestClient_5::ConstructL( )
    {
    iCamc = CVideoRecorderUtility::NewL( *this, KAudioPriorityVideoRecording, EMdaPriorityPreferenceTimeAndQuality );
    iOpenReady = EFalse;
    iPrepareReady = EFalse;
    iRecordingReady = EFalse;
    iPaused = EFalse;
    iPrepare2ndTime = EFalse;
    
    // Construct the MetaDataEntry
    iMMFMetaDataEntry = CMMFMetaDataEntry::NewL();

    CTimer::ConstructL();
    iTimeout = 0;
    iClosed = 0;
    CActiveScheduler::Add(this);
    }


void CCamcTestClient_5::Start_Active_ObjectL ( TestClient5Actions aAction, TInt aCameraHandle )
    {
    
    iUiLevelCameraHandle = aCameraHandle;
    TUid controllerUid = {0x101F8503};	//implementation uid
    TUid formatUid = {0};
    TUid formatUidG2 = {0x101F86D6};
    TUid formatUidMP4 = {0x101F873D};
    
    iAction = aAction;                  //iAction might be set to NULL after action has been executed
    iSavedAction = aAction;             //SavedAction will never be changed.
    //	iTimeout = aTimeout;
    iTimeout = 1500000;

     if ( iAction == K_Tc5_StopWhenRecordingAfterOpenFileNew )
        {
        TFileName file;
        AddDriveLetterToPath(_L("recordQCIF.3g2"), file);
        
        TRAPD(err,
            iCamc->OpenFileL( file, 
            iUiLevelCameraHandle, 
            controllerUid,
            formatUidG2,
            _L8("video/mp4v-es"),
            TC5_AUDIO_TYPE_1 ));
        
        if (err)
            {
            User::Leave(99);
            }
        }
    else if ( iAction == K_Tc5_StopWhenRecordingAfterOpenFileNew_MPEG4 )
        {
        TFileName file;
        AddDriveLetterToPath(_L("recordQCIF.mp4"), file);
        
        TRAPD(err,
            iCamc->OpenFileL( file, 
            iUiLevelCameraHandle, 
            controllerUid,
            formatUidMP4,
            _L8("video/mp4v-es; profile-level-id=3"),
            TC5_AUDIO_TYPE_3 ));
        
        if (err)
            {
            User::Leave(99);
            }
        }
    else
        {       
        TFileName file;
        AddDriveLetterToPath(_L("recordQCIF.3gp"), file);
        
        TRAPD(err,
            iCamc->OpenFileL( file, 
            iUiLevelCameraHandle, 
            controllerUid,
            formatUid,
            _L8("video/H263-2000"),
            TFourCC(' ', 'A', 'M', 'R') ));
        
        if (err)
            {
            User::Leave(99);
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

void CCamcTestClient_5::RunL()
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

void CCamcTestClient_5::RunLTrappedL()
    {
    
     if ( iClosed )
        {
        iCamc->Stop();
        CActiveScheduler::Stop();
        return;
        }

    if ( iOpenReady ) 

        {

        iOpenReady = EFalse;

        switch (iAction)
            {
            case K_Tc5_GetPriorityWhenNotSet:
                {
                TInt priority;
                TMdaPriorityPreference pref;
                iCamc->GetPriorityL(priority, pref);
                // Currently we always set priority to KAudioPriorityVideoRecording to avoid problems caused by keypad tones if keys pressed during testing
                if (priority != KAudioPriorityVideoRecording || pref != EMdaPriorityPreferenceTimeAndQuality)
                    {
                    User::Leave(KErrExpectingADifferentValue);
                    }
                break;
                }

            case K_Tc5_VideoFrameRateWhenNotSet:
                {
                TReal32 aValue;
                aValue = iCamc->VideoFrameRateL();
                if (aValue != 15.0)
                    {
                    User::Leave(KErrExpectingADifferentValue);
                    }
                break;
                }
 
            case K_Tc5_VideoBitRateWhenNotSet:
                {
                TInt aBit;
                aBit = iCamc->VideoBitRateL();
                if (aBit != 64000)
                    {
                    User::Leave(KErrExpectingADifferentValue);
                    
                    }
                break;
                }

            case K_Tc5_AudioBitRateWhenNotSet:
                {
                TInt aAudioBit;
                aAudioBit = iCamc->AudioBitRateL();
                if (aAudioBit <= 0)
                    {
                    User::Leave(KErrExpectingADifferentValue);
                    }
                break;
                }
 
            case K_Tc5_AudioEnabledWhenNotSet:
                {
                
                TBool aEnabled;
                aEnabled = iCamc->AudioEnabledL();
                // By default, audio are enabled.
                if ( !aEnabled )
                    {
                    User::Leave(KErrExpectingADifferentValue);
                    }
                break;
                }

            case K_Tc5_GetVideoFrameSizeWhenNotSet:
                {
                TSize aFrame;
                TSize expectedFrameSize(176,144);

                 iCamc->GetVideoFrameSizeL(aFrame);
                 if (aFrame != expectedFrameSize)
                    {
                    
                    User::Leave(KErrExpectingADifferentValue);
                    
                    }
                break;
                }
            case K_Tc5_GainWhenNotSet:
                {
                TInt aGain;
                aGain = iCamc->GainL();
                if ((aGain < 0) || (aGain >= 100))
                    {
                    User::Leave(KErrExpectingADifferentValue);
                    
                    }
                break;
                }

            default:
                break;
            }

        // Set default parameters. They can be overwritten depending on the test case. 
        iCamc->SetVideoFrameSizeL(TSize(176,144));
        iCamc->SetVideoFrameRateL(TReal32(15));
        iCamc->SetVideoBitRateL(TInt(50000));
        iCamc->SetAudioEnabledL(ETrue); 

        switch (iAction)
            {
           
            case K_Tc5_NumberOfMetaDataEntriesWhenNotRecording:
                {
                TInt aValue;
                aValue = iCamc->NumberOfMetaDataEntriesL();
                // aValue should always be 0.
                if (aValue != 0)
                    {
                    User::Leave(KErrExpectingADifferentValue);
                    }
                break;
                }

            case K_Tc5_MetaDataEntryWhenNotRecording:
                {
                // Index 2 dosen't exist, should leave
                iCamc->MetaDataEntryL( 2 );
                // There is a problem if we haven't left already
                User::Leave(KErrExpectingADifferentValue);
                break;
                }

            case K_Tc5_AddMetaDataEntryWhenNotRecording:
                {
                // 
                iCamc->AddMetaDataEntryL( *iMMFMetaDataEntry );      
                break;
                }

            case K_Tc5_RemoveMetaDataEntryWhenNotRecording:
                {
                // Index 2 dosen't exist, should leave
                iCamc->RemoveMetaDataEntryL( 2 );
                break;
                }

            case K_Tc5_ReplaceMetaDataEntryWhenNotRecording:
                {
                // Index 2 dosen't exist, should leave
                iCamc->ReplaceMetaDataEntryL( 2, *iMMFMetaDataEntry );
                break;
                }

            case K_Tc5_SetPriorityWhenNotRecording:
                {
                // Set Invalid Priority.
                iCamc->SetPriorityL( EMdaPriorityMin-100,EMdaPriorityPreferenceTime );
                break;
                }
 
            case K_Tc5_SetVideoFrameRateWhenOpenNotPrepared:
                {
                // 
                iCamc->SetVideoFrameRateL( TReal32(TC5_VIDEO_FRAME_RATE_WRONG) );
                break;
                }

            case K_Tc5_SetVideoBitRateWhenOpenNotPrepared:
                {
                // 
                iCamc->SetVideoBitRateL( TC5_VIDEO_BIT_RATE_WRONG );
                break;
                }

            case K_Tc5_SetAudioBitRateWhenOpenNotPrepared:
                {
                // 
                iCamc->SetAudioBitRateL( TC5_AUDIO_BIT_RATE_WRONG );
                break;
                }

            case K_Tc5_SetAudioEnabledWhenOpenNotPrepared:
                {
                // 
                iCamc->SetAudioEnabledL( TC5_AUDIO_ENABLED );
                break; 
                }

            case K_Tc5_SetVideoFrameSizeWhenOpenNotPrepared:
                {
                // 
                iCamc->SetVideoFrameSizeL( TC5_VIDEO_FRAME_SIZE_WRONG );
                break;
                }

            case K_Tc5_SetMaxClipSizeLWhenNotRecording:
                {
                iCamc->SetMaxClipSizeL( -10 ); // A wrong value	              
                break;
                }

            case K_Tc5_SetGainWhenNotRecording:
                {
                // 
                iCamc->SetGainL( 1 );
                break;
                }

            case K_Tc5_SetGainWhenNotRecordingAndSetWrong:
                {
                //
                TInt aSetGain;
                aSetGain = iCamc->MaxGainL();
                RDebug::Print(_L("CCamcTest_5::SetGain2 MaxGain is %d "), aSetGain );
                iCamc->SetGainL( aSetGain+1 );
                break; 
                }
                
            case K_Tc5_MaxGainWhenNotRecording:
                {
                TInt aGain;
                aGain = iCamc->MaxGainL();
                RDebug::Print(_L("CCamcTest_5::MaxGain MaxGain is %d (Should be 0-100)"), aGain );
                if ((aGain <= 0) || (aGain >= 100))
                    {
                    User::Leave(KErrExpectingADifferentValue);          
                    }
                break;
                }

            case K_Tc5_StopWhenNotRecording:
                {
                iCamc->Stop();
                //iAction = K_Tc5_none;
                break;
                }
            case K_Tc5_StopWhenRecordingAfterOpenFileNew_MPEG4:
                {
                iCamc->SetVideoFrameSizeL( TC5_CIF_VIDEO_FRAME_SIZE );
                break;
                }

            default:
                break;
            }

        iCamc->Prepare();
        }

    else if ( iPrepareReady ) 

        {      
        TBool prepare = ETrue;
        switch (iAction)
            {
            //Compare
            case K_Tc5_SetAudioEnabledWhenOpenNotPrepared:
                {
                TInt aEnabled;
                aEnabled = iCamc->AudioEnabledL();

                TBool ok = TC5_AUDIO_ENABLED; // To avoid a warning.
                if ( ( aEnabled && (!ok) ) || ( (!aEnabled) && ok ) ) 
                    {
                    User::Leave(KErrExpectingADifferentValue);
                    }

                break;
                }

            //Compare
            case K_Tc5_SetGainWhenNotRecording:
                {
                TInt aGain;
                aGain = iCamc->GainL();
                RDebug::Print(_L("CCamcTest_5::SetGain1 Gain is %d (Should be 1)"), aGain );
                // aSetGain should be the same with previously set 
                if (aGain != 1)
                    {
                    User::Leave(KErrExpectingADifferentValue);
                    
                    }
                break;
                }

            case K_Tc5_StopWhenRecordingNewPrepareCorrect:
                {
                if ( iPrepare2ndTime )

                    {
                    // If we are here, the 2nd prepare was ready before the 
                    // 2s timeout ---> Test passed
                    iCamc->Stop();
                    CActiveScheduler::Stop();
                    prepare =EFalse;
                    }
                break;
                }
                
            case K_Tc5_GetSupportedVideoTypesWhenOpenNotRecording:
                {
                CDesC8ArrayFlat * spprtVideoTypes = new (ELeave) CDesC8ArrayFlat (1);
                CleanupStack::PushL(spprtVideoTypes);
                iCamc->GetSupportedVideoTypesL( *spprtVideoTypes );
                TInt index;
                if (spprtVideoTypes->Count() < 2)
                    {
                    User::Leave(KErrExpectingADifferentValue);
                    }         
                if ( spprtVideoTypes->Find(_L8("video/H263-2000"), index ) == KErrNotFound )
                    {
                    User::Leave(KErrExpectingADifferentValue);
                    }
                
                if (spprtVideoTypes->Find(_L8("video/H263-2000; profile=0") ,index ) == KErrNotFound )
                    {
                    User::Leave(KErrExpectingADifferentValue);
                    }
                CleanupStack::PopAndDestroy();
                break;                
                }
                
            case K_Tc5_GetSupportedVideoTypesWhenOpenNotRecordingNew:
                {
                CDesC8ArrayFlat * spprtVideoTypes = new (ELeave) CDesC8ArrayFlat (1);
                CleanupStack::PushL(spprtVideoTypes);
                iCamc->GetSupportedVideoTypesL( *spprtVideoTypes );
                TInt index;
                if (spprtVideoTypes->Count() < 4)
                    {
                    User::Leave(KErrExpectingADifferentValue);
                    }         
                if ( spprtVideoTypes->Find(_L8("video/H263-2000"), index ) == KErrNotFound )
                    {
                    User::Leave(KErrExpectingADifferentValue);
                    }
                
                if (spprtVideoTypes->Find(_L8("video/H263-2000; profile=0") ,index ) == KErrNotFound )
                    {
                    User::Leave(KErrExpectingADifferentValue);
                    }

                if ( spprtVideoTypes->Find(_L8("video/mp4v-es"), index ) == KErrNotFound )
                    {
                    User::Leave(KErrExpectingADifferentValue);
                    }
                
                if (spprtVideoTypes->Find(_L8("video/mp4v-es; profile-level-id=8") ,index ) == KErrNotFound )
                    {
                    User::Leave(KErrExpectingADifferentValue);
                    }
                CleanupStack::PopAndDestroy();
                break;
                }
            case K_Tc5_GetSupportedAudioTypesWhenOpenNotRecordingAMR:
                {
                RArray<TFourCC> spprtAudioTypes;
                spprtAudioTypes.Reset();
                iCamc->GetSupportedAudioTypesL( spprtAudioTypes );
                if (spprtAudioTypes.Count() != 1)
                    {
                    spprtAudioTypes.Close();
                    User::Leave(KErrExpectingADifferentValue);
                    }         
                
                if (spprtAudioTypes.Find(TC5_AUDIO_TYPE_1) == KErrNotFound )
                    {
                    spprtAudioTypes.Close();
                    User::Leave(KErrExpectingADifferentValue);
                    }
                spprtAudioTypes.Close();
                break;                
                }
            case K_Tc5_GetSupportedAudioTypesWhenOpenNotRecordingAAC:
                {
                RArray<TFourCC> spprtAudioTypes;
                spprtAudioTypes.Reset();
                iCamc->GetSupportedAudioTypesL( spprtAudioTypes );
                if (spprtAudioTypes.Count() != 2)
                    {
                    spprtAudioTypes.Close();
                    User::Leave(KErrExpectingADifferentValue);
                    }         
                if ( spprtAudioTypes.Find( TC5_AUDIO_TYPE_1 ) == KErrNotFound )
                    {
                    spprtAudioTypes.Close();
                    User::Leave(KErrExpectingADifferentValue);
                    }
                
                if (spprtAudioTypes.Find( TC5_AUDIO_TYPE_3 ) == KErrNotFound )
                    {
                    spprtAudioTypes.Close();
                    User::Leave(KErrExpectingADifferentValue);
                    }
                spprtAudioTypes.Close();
                break;                
                }
                
            default:
                break;
            }
        if (prepare)
            {
            iPrepareReady = EFalse;
            iCamc->Record();
            CTimer::After( iTimeout );
            }
        }
    
    else if ( iRecordingReady )
        {
        switch (iAction)
            {
            
            //Compare
            case K_Tc5_SetGainWhenRecording:
                {
                TInt aGain;
                aGain = iCamc->GainL();
                RDebug::Print(_L("CCamcTest_5::SetGain3 Gain is %d (Should be 1)"), aGain );
                if (aGain != 1)
                    {
                    User::Leave(KErrExpectingADifferentValue);
                    
                    }
                break;
                }
            
            default:
                break;
            }
        iRecordingReady = EFalse;
        iCamc->Close();
        iClosed =1;
        CTimer::After( 1000 );
        
        }
    else if ( iPaused ) //resume
        {
        iPaused = EFalse;
        iCamc->Record();
        CTimer::After(10 * TInt32 ( 1E6 ) ); // Continue recording for 10 seconds
                                             // Then stop the recording
        }
    else //timer
        {
        TBool stopRecording = ETrue;

        switch (iAction)
            {
            case K_Tc5_NumberOfMetaDataEntriesWhenRecording:
                {
                TInt aValue;
                aValue = iCamc->NumberOfMetaDataEntriesL();
                // aValue should always be 0.
                if (aValue != 0)
                    {
                    User::Leave(KErrExpectingADifferentValue);
                    }
                break;
                }

            case K_Tc5_MetaDataEntryWhenRecording:
                {
                // Index 2 dosen't exist, should leave
                iCamc->MetaDataEntryL( 2 );  
                // There is a problem if we haven't left already
                User::Leave(KErrExpectingADifferentValue);       
                break;
                }

            case K_Tc5_AddMetaDataEntryWhenRecording:
                {
                // 
                iCamc->AddMetaDataEntryL( *iMMFMetaDataEntry );             
                break;
                }

            case K_Tc5_RemoveMetaDataEntryWhenRecording:
                {
                // Index 2 dosen't exist, should leave
                iCamc->RemoveMetaDataEntryL( 2 );
                break;
                }

            case K_Tc5_ReplaceMetaDataEntryWhenRecording:
                {
                // Index 2 dosen't exist, should leave
                iCamc->ReplaceMetaDataEntryL( 2, *iMMFMetaDataEntry );
                break;
                }

            case K_Tc5_PauseWhenRecording:
                {
                iCamc->PauseL();
                stopRecording = EFalse;
                iAction=K_Tc5_none;     
                CTimer::After(2 * TInt32 ( 1E6 ) ); // Pause for 2 seconds.
                iPaused=ETrue;
                break;
                }

            case K_Tc5_SetGainWhenRecording:
                {
                // 
                iCamc->SetGainL( 1 );
                break; 
                }

            case K_Tc5_MaxGainWhenRecording:
                {
                TInt aGain;
                aGain = iCamc->MaxGainL();
                if ((aGain <= 0) || (aGain >= 100))
                    {
                    User::Leave(KErrExpectingADifferentValue);
                    
                    }
                break;
                }
                
            case K_Tc5_StopWhenRecording:
                {
                //iCamc->Stop();
                stopRecording = ETrue;
                //iAction = K_Tc5_none;
                break;
                }

            case K_Tc5_StopWhenRecordingNewPrepareCorrect:
                {
                if ( iPrepare2ndTime )
                    {
                    // If we are here, the 2nd prepare was NOT ready before the 
                    // 2s timeout ---> Test failed
                    User::Leave(KErrExpectingADifferentValue);
                    }
                stopRecording = ETrue;
                break;
                }

            case K_Tc5_StopWhenRecordingAfterOpenFile:
                {
                //iCamc->Stop();
                stopRecording = ETrue;
                //iAction = K_Tc5_none;
                break;
                }
            case K_Tc5_StopWhenRecordingAfterOpenFileNew:
            case K_Tc5_StopWhenRecordingAfterOpenFileNew_MPEG4:
                {
                //iCamc->Stop();
                stopRecording = ETrue;
                //iAction = K_Tc5_none;
                break;
                }
            default:
                break;
            }
        if (stopRecording)
            {
            
            iCamc->Stop();
            // Simulates that iRecording has been completed through the 
            // MvruoRecordComplete callback
            iRecordingReady=ETrue;
            
            switch (iAction)
                {
                case K_Tc5_StopWhenRecordingNewPrepareCorrect:
                    {
                    iCamc->Close();
                    TUid controllerUid = {0x101F8503};	//implementation uid
                    TUid formatUid = {0};
                    TRAPD(err,iCamc->OpenFileL( _L("c:\\recordQCIF_2.3gp"), 
                        iUiLevelCameraHandle, 
                        controllerUid,
                        formatUid ));
                    
                    User::LeaveIfError(err);

                    CTimer::After(2 * TInt32 ( 1E6 ) ); // Wait 2 seconds
                    iPrepare2ndTime = ETrue;
                    break;
                    }
                    
                default:
                    {
                    CTimer::After( 1000 );
                    break;
                    }
                }
            }    
        
        switch (iAction)
            {
            case K_Tc5_PauseWhenStopped:
                {
                iCamc->PauseL();
                break;
                }
            default:
                break;
            }
        }

    }



void CCamcTestClient_5::DoCancel()
    {
    iCamc->Stop();
    iCamc->Close();
    delete iCamc;
    iCamc = NULL;
    }


void CCamcTestClient_5::MvruoOpenComplete( TInt aError )
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

void CCamcTestClient_5::MvruoPrepareComplete( TInt aError )
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

void CCamcTestClient_5::MvruoRecordComplete( TInt aError )
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

void CCamcTestClient_5::MvruoEvent(const TMMFEvent& /*aEvent*/) 
    {
    /* Should be never called ... */  
    SomethingInActiveObjectIsLeaving(99);
    }

void CCamcTestClient_5::SomethingInActiveObjectIsLeaving (TInt aError)
    {
    // Something in the active object is leaving
    // or a fatal error has been found.

    // Stop the active object scheduler.
        iError=aError;
        iCamc->Stop();
        CActiveScheduler::Stop();
    }




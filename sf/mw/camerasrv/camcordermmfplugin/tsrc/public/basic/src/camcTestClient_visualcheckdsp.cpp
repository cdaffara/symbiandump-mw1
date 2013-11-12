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

#include "camctestclient_visualcheckdsp.h"
#include <AudioPreference.h>


// EXTERNAL DATA STRUCTURES

// EXTERNAL FUNCTION PROTOTYPES 
 
// CONSTANTS

// MACROS

// LOCAL CONSTANTS AND MACROS

// MODULE DATA STRUCTURES

// LOCAL FUNCTION PROTOTYPES

// FORWARD DECLARATIONS



CCamcTestClient_visualcheckdsp::CCamcTestClient_visualcheckdsp () : CTimer(CActive::EPriorityHigh)
    {
    }


CCamcTestClient_visualcheckdsp::~CCamcTestClient_visualcheckdsp ()
    {
#ifdef _DEBUG
    RDebug::Print(_L("CamCTestClient VisualCheck: Destructor Start"));
#endif
    Cancel();
    delete iCamc;
#ifdef _DEBUG
    RDebug::Print(_L("CamCTestClient VisualCheck: Destructor Start"));
#endif
    }

CCamcTestClient_visualcheckdsp* CCamcTestClient_visualcheckdsp::NewL()
    {
    CCamcTestClient_visualcheckdsp* self = new(ELeave) CCamcTestClient_visualcheckdsp;
    CleanupStack::PushL(self);
    self->ConstructL( );
    CleanupStack::Pop(self);
    return self;
    }

void CCamcTestClient_visualcheckdsp::ConstructL( )
    {
    iCamc = CVideoRecorderUtility::NewL( *this, KAudioPriorityVideoRecording, EMdaPriorityPreferenceTimeAndQuality );
    iOpenReady = EFalse;
    iPrepareReady = EFalse;
    iRecordingReady = EFalse;
    iPaused = EFalse;
    i2ndTime = EFalse;
    iDoublePause = EFalse;
    iStartRecording2ndTime = EFalse;
    
    CTimer::ConstructL();
    iTimeout = 0;
    iClosed = 0;
    CActiveScheduler::Add(this);
    }


void CCamcTestClient_visualcheckdsp::Start_Active_ObjectL ( TestClientVCdspActions aAction, TInt aCameraHandle )
    {
    
    iUiLevelCameraHandle = aCameraHandle;
    TUid controllerUid = {0x101F8503};	//implementation uid
    TUid formatUid = {0};
    TUid formatUidG2 = {0x101F86D6};
    TUid formatUidMP4 = {0x101F873D};
    
    
    iAction = aAction;                  //iAction might be set to NULL after action has been executed
    TBuf<50> fileName;
    
#if ( defined (__WINS__) || defined (__WINSCW__) )
    _LIT(KFileName1,"c:\\recordQCIF_dsp_1.3gp");
    _LIT(KFileName2,"c:\\recordQCIF_dsp_2.3gp");
    _LIT(KFileName3,"c:\\recordQCIF_dsp_3.3gp");
    _LIT(KFileName4,"c:\\recordQCIF_dsp_4.3gp");
    _LIT(KFileName5,"c:\\recordQCIF_dsp_5.3gp");
    _LIT(KFileName6,"c:\\recordQCIF_dsp_6.3gp");
    _LIT(KFileName7,"c:\\recordQCIF_dsp_7.3gp");
    _LIT(KFileName8,"c:\\recordQCIF_dsp_8.3gp");
    _LIT(KFileName9_a,"c:\\recordQCIF_dsp_9_a.3gp");
    _LIT(KFileName9_b,"c:\\recordQCIF_dsp_9_b.3gp");
    _LIT(KFileName10,"c:\\recordQCIF_dsp_10.3gp");
    _LIT(KFileName11,"c:\\recordQCIF_dsp_11.3gp");
    _LIT(KFileName12,"c:\\recordQCIF_dsp_12.3gp");
    _LIT(KFileName14,"c:\\recordQCIF_dsp_14.3gp");
    _LIT(KFileName15,"c:\\recordQCIF_dsp_15.3gp");
    _LIT(KFileName16,"c:\\recordQCIF_dsp_16.3gp");
    _LIT(KFileName17,"c:\\recordQCIF_dsp_17.3gp");
    _LIT(KFileName17_A,"c:\\recordQCIF_dsp_17_a.3gp");
    _LIT(KFileName18,"c:\\recordQCIF_dsp_18.3g2");
    _LIT(KFileName19,"c:\\recordQCIF_dsp_19.3g2");
    _LIT(KFileName20,"c:\\recordQCIF_dsp_20.3g2");
    _LIT(KFileName21,"c:\\recordQCIF_dsp_21.mp4");
#else 
    _LIT(KFileName1,"E:\\recordQCIF_dsp_1.3gp");
    _LIT(KFileName2,"E:\\recordQCIF_dsp_2.3gp");
    _LIT(KFileName3,"E:\\recordQCIF_dsp_3.3gp");
    _LIT(KFileName4,"E:\\recordQCIF_dsp_4.3gp");
    _LIT(KFileName5,"E:\\recordQCIF_dsp_5.3gp");
    _LIT(KFileName6,"E:\\recordQCIF_dsp_6.3gp");
    _LIT(KFileName7,"E:\\recordQCIF_dsp_7.3gp");
    _LIT(KFileName8,"E:\\recordQCIF_dsp_8.3gp");
    _LIT(KFileName9_a,"E:\\recordQCIF_dsp_9_a.3gp");
    _LIT(KFileName9_b,"E:\\recordQCIF_dsp_9_b.3gp");
    _LIT(KFileName10,"E:\\recordQCIF_dsp_10.3gp");
    _LIT(KFileName11,"E:\\recordQCIF_dsp_11.3gp");
    _LIT(KFileName12,"E:\\recordQCIF_dsp_12.3gp");
    _LIT(KFileName14,"E:\\recordQCIF_dsp_14.3gp");
    _LIT(KFileName15,"C:\\recordQCIF_dsp_15.3gp");
    _LIT(KFileName16,"E:\\recordQCIF_dsp_16.3gp");
    _LIT(KFileName17,"E:\\recordQCIF_dsp_17.3gp");
    _LIT(KFileName17_A,"E:\\recordQCIF_dsp_17_a.3gp");
    _LIT(KFileName18,"E:\\recordQCIF_dsp_18.3g2");
    _LIT(KFileName19,"E:\\recordQCIF_dsp_19.3g2");
    _LIT(KFileName20,"E:\\recordQCIF_dsp_20.3g2");
    _LIT(KFileName21,"E:\\recordQCIF_dsp_21.mp4");
#endif
    switch(iAction)
        {
        case K_VC_dsp_test_case_1:
            {
            fileName = KFileName1;
            break;
            }
        case K_VC_dsp_test_case_2:
            {
            fileName = KFileName2;
            break;
            }
        case K_VC_dsp_test_case_3:
            {
            fileName = KFileName3;
            break;
            }
        case K_VC_dsp_test_case_4:
            {
            fileName = KFileName4;
            break;
            }
        case K_VC_dsp_test_case_5:
            {
            fileName = KFileName5;
            break;
            }
        case K_VC_dsp_test_case_6:
            {
            fileName = KFileName6;
            break;
            }
        case K_VC_dsp_test_case_7:
            {
            fileName = KFileName7;
            break;
            }
        case K_VC_dsp_test_case_8:
            {
            fileName = KFileName8;
            break;
            }
        case K_VC_dsp_test_case_9_a:
            {
            fileName = KFileName9_a;
            break;
            }
        case K_VC_dsp_test_case_9_b:
            {
            fileName = KFileName9_b;
            break;
            }
        case K_VC_dsp_test_case_10:
            {
            fileName = KFileName10;
            break;
            }
        case K_VC_dsp_test_case_11:
            {
            fileName = KFileName11;
            break;
            }
        case K_VC_dsp_test_case_12:
            {
            fileName = KFileName12;
            break;
            }
        case K_VC_dsp_test_case_14:
            {
            fileName = KFileName14;
            break;
            }
        case K_VC_dsp_test_case_15:
            {
            fileName = KFileName15;
            break;
            }
        case K_VC_dsp_test_case_16:
            {
            fileName = KFileName16;
            break;
            }
        case K_VC_dsp_test_case_17:
            {
            fileName = KFileName17;
            break;
            }
        case K_VC_dsp_test_case_17_a:
            {
            fileName = KFileName17_A;
            break;
            }
        case K_VC_dsp_test_case_18:
            {
            fileName = KFileName18;
            break;
            }
        case K_VC_dsp_test_case_19:
            {
            fileName = KFileName19;
            break;
            }
        case K_VC_dsp_test_case_20:
            {
            fileName = KFileName20;
            break;
            }
        case K_VC_dsp_test_case_21:
            {
            fileName = KFileName21;
            break;
            }
        default:
            break;
        }
    
    TInt err = KErrNone;
    switch ( iAction )
        {
        case K_VC_dsp_test_case_9_a :
            TRAP(err,
                iCamc->OpenFileL( fileName, iUiLevelCameraHandle, 
                controllerUid,
                formatUid,
                _L8("video/H263-2000; profile=0; level=45"),
                TFourCC(' ', 'A', 'M', 'R') ));
            break;
        case K_VC_dsp_test_case_17 :
        case K_VC_dsp_test_case_17_a :
            TRAP(err,
                iCamc->OpenFileL( fileName, iUiLevelCameraHandle, 
                controllerUid,
                formatUid,
                _L8("video/mp4v-es"),
                TFourCC(' ', 'A', 'M', 'R') ));
            break;
        case K_VC_dsp_test_case_18:
            TRAP(err,
                iCamc->OpenFileL( fileName, iUiLevelCameraHandle, 
                controllerUid,
                formatUidG2,
                _L8("video/H263-2000"),
                TFourCC(' ', 'A', 'M', 'R') ));
            break;
        case K_VC_dsp_test_case_19:
            TRAP(err,
                iCamc->OpenFileL( fileName, iUiLevelCameraHandle, 
                controllerUid,
                formatUidG2,
                _L8("video/mp4v-es"),
                TFourCC(' ', 'Q', '1', '3') ));
            break;
        case K_VC_dsp_test_case_20:
            TRAP(err,
                iCamc->OpenFileL( fileName, iUiLevelCameraHandle, 
                controllerUid,
                formatUidG2,
                _L8("video/H263-2000"),
                TFourCC(' ', 'Q', '1', '3') ));
            break;
        case K_VC_dsp_test_case_21:
            TRAP(err,
                iCamc->OpenFileL( fileName, iUiLevelCameraHandle, 
                controllerUid,
                formatUidMP4,
                _L8("video/mp4v-es; profile-level-id=3"),
                TFourCC(' ', 'A', 'A', 'C') ));
            break;
        default:
            {
            TRAP(err,
                iCamc->OpenFileL( fileName, iUiLevelCameraHandle, 
                controllerUid,
                formatUid,
                _L8("video/H263-2000"),
                TFourCC(' ', 'A', 'M', 'R') ));
            }
        }
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
    
    void CCamcTestClient_visualcheckdsp::RunL()
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
    
    void CCamcTestClient_visualcheckdsp::RunLTrappedL()
        {
#ifdef _DEBUG
        RDebug::Print(_L("CamCTestClient VisualCheck:RunL"));
#endif
        if ( iOpenReady ) 
            {
            iOpenReady = EFalse;
            
            switch(iAction)
                {  
                case K_VC_dsp_test_case_1:
                    {
                    break; 
                    }
                case K_VC_dsp_test_case_2:
                    {
                    iCamc->SetVideoFrameSizeL(TSize(128,96));
                    iCamc->SetVideoFrameRateL(TReal32(5));
                    iCamc->SetVideoBitRateL(TInt(28000));
                    iCamc->SetAudioEnabledL(ETrue); 
                    iCamc->SetAudioBitRateL(12200); 
                    iCamc->SetMaxClipSizeL(95000);
                    break; 
                    }
                case K_VC_dsp_test_case_3:
                    {
                    iCamc->SetVideoFrameSizeL(TSize(128,96));
                    iCamc->SetVideoFrameRateL(TReal32(7.5));
                    iCamc->SetVideoBitRateL(TInt(28000));
                    iCamc->SetAudioEnabledL(ETrue); 
                    iCamc->SetAudioBitRateL(6700); 
                    iCamc->SetMaxClipSizeL(95000);
                    break; 
                    }
                case K_VC_dsp_test_case_4:
                    {
                    iCamc->SetVideoFrameSizeL(TSize(128,96));
                    iCamc->SetVideoFrameRateL(TReal32(15));
                    iCamc->SetVideoBitRateL(TInt(28000));
                    iCamc->SetAudioEnabledL(ETrue); 
                    iCamc->SetAudioBitRateL(5150); 
                    iCamc->SetMaxClipSizeL(250000);
                    break; 
                    }
                case K_VC_dsp_test_case_5:
                    {
                    iCamc->SetVideoFrameSizeL(TSize(176,144));
                    iCamc->SetVideoFrameRateL(TReal32(5));
                    iCamc->SetVideoBitRateL(TInt(42000));
                    iCamc->SetAudioEnabledL(ETrue); 
                    iCamc->SetAudioBitRateL(7400); 
                    iCamc->SetMaxClipSizeL(95000);
                    break;
                    }
                case K_VC_dsp_test_case_6:
                    {
                    iCamc->SetVideoFrameSizeL(TSize(176,144));
                    iCamc->SetVideoFrameRateL(TReal32(7.5));
                    iCamc->SetVideoBitRateL(TInt(42000));
                    iCamc->SetAudioEnabledL(ETrue); 
                    iCamc->SetAudioBitRateL(4750); 
                    iCamc->SetMaxClipSizeL(95000);
                    break;
                    }
                case K_VC_dsp_test_case_7:
                    {
                    iCamc->SetVideoFrameSizeL(TSize(176,144));
                    iCamc->SetVideoFrameRateL(TReal32(15));
                    iCamc->SetVideoBitRateL(TInt(42000));
                    iCamc->SetAudioEnabledL(ETrue); 
                    iCamc->SetAudioBitRateL(12200); 
                    break;
                    }
                case K_VC_dsp_test_case_8:
                    {
                    iCamc->SetVideoFrameSizeL(TSize(176,144));
                    iCamc->SetVideoFrameRateL(TReal32(7.5));
                    iCamc->SetVideoBitRateL(TInt(64000));
                    iCamc->SetAudioEnabledL(ETrue); 
                    iCamc->SetAudioBitRateL(12200); 
                    iCamc->SetMaxClipSizeL(250000);
                    break;              
                    }
                case K_VC_dsp_test_case_9_a:
                    {
                    iCamc->SetVideoFrameSizeL(TSize(176,144));
                    iCamc->SetVideoFrameRateL(TReal32(15));
                    iCamc->SetVideoBitRateL(TInt(128000));
                    iCamc->SetAudioEnabledL(ETrue); 
                    iCamc->SetAudioBitRateL(12200); 
                    break;
                    }
                case K_VC_dsp_test_case_9_b:
                    {
                    iCamc->SetVideoFrameSizeL(TSize(176,144));
                    iCamc->SetVideoFrameRateL(TReal32(15));
                    iCamc->SetVideoBitRateL(TInt(64000));
                    iCamc->SetAudioEnabledL(ETrue); 
                    iCamc->SetAudioBitRateL(12200); 
                    break;
                    }
                case K_VC_dsp_test_case_10:
                    {
                    iCamc->SetVideoFrameSizeL(TSize(176,144));
                    iCamc->SetVideoFrameRateL(TReal32(7.5));
                    iCamc->SetVideoBitRateL(TInt(42000));    
                    iCamc->SetAudioEnabledL(EFalse); 
                    iCamc->SetMaxClipSizeL(95000);
                    break;
                    }
                case K_VC_dsp_test_case_11:
                    {
                    iCamc->SetVideoFrameSizeL(TSize(176,144));
                    iCamc->SetVideoFrameRateL(TReal32(15));
                    iCamc->SetVideoBitRateL(TInt(64000));
                    iCamc->SetAudioEnabledL(EFalse); 
                    iCamc->SetMaxClipSizeL(500000);
                    break;
                    }
                case K_VC_dsp_test_case_12:
                    {
                    iCamc->SetVideoFrameSizeL(TSize(128,96));
                    iCamc->SetVideoFrameRateL(TReal32(15));
                    iCamc->SetVideoBitRateL(KMMFVariableVideoBitRate);
                    iCamc->SetMaxClipSizeL(95000);
                    break;
                    }
                case K_VC_dsp_test_case_15:
                    {
                    iCamc->SetVideoFrameSizeL(TSize(176,144));
                    iCamc->SetVideoFrameRateL(TReal32(15));
                    iCamc->SetVideoBitRateL(KMMFVariableVideoBitRate);
                    break;
                    }    
                case K_VC_dsp_test_case_16:
                    {
                    iCamc->SetVideoFrameSizeL(TSize(176,144));
                    iCamc->SetVideoFrameRateL(TReal32(10));
                    iCamc->SetVideoBitRateL(TInt(64000));
                    iCamc->SetAudioEnabledL(ETrue); 
                    iCamc->SetAudioBitRateL(12200); 
                    break;
                    }    
                case K_VC_dsp_test_case_17:
                    {
                    iCamc->SetVideoFrameSizeL(TSize(128,96));
                    iCamc->SetVideoFrameRateL(TReal32(15));
                    iCamc->SetVideoBitRateL(TInt(64000));
                    iCamc->SetAudioEnabledL(ETrue); 
                    iCamc->SetAudioBitRateL(12200); 
                    break;
                    }    
                case K_VC_dsp_test_case_17_a:
                    {
                    iCamc->SetVideoFrameSizeL(TSize(128,96));
                    iCamc->SetVideoFrameRateL(TReal32(15));
                    iCamc->SetVideoBitRateL(TInt(64000));
                    iCamc->SetAudioEnabledL(ETrue); 
                    iCamc->SetAudioBitRateL(12200); 
                    iCamc->SetMaxClipSizeL(500000);
                    break;
                    }
                case K_VC_dsp_test_case_18:
                    {
                    iCamc->SetVideoFrameSizeL(TSize(176,144));
                    iCamc->SetVideoFrameRateL(TReal32(10));
                    iCamc->SetVideoBitRateL(TInt(64000));
                    iCamc->SetAudioEnabledL(ETrue); 
                    iCamc->SetAudioBitRateL(12200); 
                    break;
                    }    
                case K_VC_dsp_test_case_19:
                    {
                    iCamc->SetVideoFrameSizeL(TSize(176,144));
                    iCamc->SetVideoFrameRateL(TReal32(10));
                    iCamc->SetVideoBitRateL(TInt(64000));
                    iCamc->SetAudioEnabledL(ETrue); 
                    iCamc->SetMaxClipSizeL(300000);
                    //bitrate setting TBD
                    iCamc->SetAudioBitRateL(6800); 
                    break;
                    }
                case K_VC_dsp_test_case_20:
                    {
                    iCamc->SetVideoFrameSizeL(TSize(176,144));
                    iCamc->SetVideoFrameRateL(TReal32(15));
                    iCamc->SetVideoBitRateL(TInt(64000));
                    iCamc->SetAudioEnabledL(ETrue); 
                    //bitrate setting TBD
                    iCamc->SetAudioBitRateL(6800); 
                    break;
                    }
                case K_VC_dsp_test_case_21:
                    {
                    iCamc->SetVideoFrameSizeL(TSize(352,288));
                    iCamc->SetVideoFrameRateL(TReal32(15));
                    iCamc->SetVideoBitRateL(TInt(384000));
                    iCamc->SetAudioEnabledL(ETrue); 
                    iCamc->SetAudioBitRateL(32000); 
                    break;
                    }
                default:
                    break;
            }
            
            
            
            iCamc->Prepare();
        }
        else if ( iPrepareReady ) 
            {
            iPrepareReady = EFalse;    
            iCamc->Record();
            switch(iAction)
                {
                case K_VC_dsp_test_case_1:
                    {
                    CTimer::After( 30 * TInt32 ( 1E6 ) );  // Records for 30 sec
                    break;
                    }
                case K_VC_dsp_test_case_2:
                case K_VC_dsp_test_case_3:
                case K_VC_dsp_test_case_4:
                case K_VC_dsp_test_case_5:
                case K_VC_dsp_test_case_6:
                case K_VC_dsp_test_case_19:
                    {
                    // The timeout will be generated by the SetMaxClipSize.
                    break;
                    }
                case K_VC_dsp_test_case_14:
                    {
                    CTimer::After( 1 * TInt32 ( 1E6 ) );
                    break;
                    }
                case K_VC_dsp_test_case_16:
                case K_VC_dsp_test_case_17:
                case K_VC_dsp_test_case_17_a:
                case K_VC_dsp_test_case_18:
                    {
                    CTimer::After( 10 * TInt32 ( 1E6 ) );  // Records for 10 sec
                    iStartRecording2ndTime = EFalse;
                    break;
                    }
                case K_VC_dsp_test_case_12:
                    {
                    CTimer::After( 4 * TInt32 ( 1E6 ) );  // Records for 4 sec
                    iStartRecording2ndTime = EFalse;
                    }
                    break;
                case K_VC_dsp_test_case_20:
                    {
                    CTimer::After( 15 * TInt32 ( 1E6 ) );  // Records for 15 sec + pause + another 15 sec
                    iStartRecording2ndTime = EFalse;
                    break;
                    }
                case K_VC_dsp_test_case_21:
                    {
                    CTimer::After( 30 * TInt32 ( 1E6 ) );  // Records for 30 sec + pause + another 30 sec
                    iStartRecording2ndTime = EFalse;
                    break;
                    }

                default:
                    {
                    CTimer::After( 10 * TInt32 ( 1E6 ) );
                    break;          
                    }
                }
            }
        else if ( iRecordingReady )
            {
            iRecordingReady = EFalse;
            iCamc->Close();
            iClosed = ETrue;
            CTimer::After(1000);
            }
        else if (iClosed)
            {
            CActiveScheduler::Stop();
            }
        else if ( iPaused ) //resume
            {
            switch(iAction)
                {
                case K_VC_dsp_test_case_7:
                    {
                    iCamc->Record();
                    CTimer::After( 50 * TInt32 ( 1E6 ) ); // Records for 50 sec.
                    iPaused = EFalse;
                    iAction = K_VC_dsp_none;         
                    break;
                    }
                case K_VC_dsp_test_case_9_a: 
                    {
                    if ( iDoublePause )
                        {             
                        iCamc->Record();
                        CTimer::After( 2*60 * TInt32 ( 1E6 ) ); // Record 2 minutes
                        iPaused = EFalse;
                        iAction = K_VC_dsp_none;
                        }
                    else 
                        {
                        iCamc->Record();
                        CTimer::After(  2*60 * TInt32 ( 1E6 ) ); // Record 2 minutes
                        iPaused = EFalse;
                        iDoublePause = ETrue;
                        }
                    break;
                    }
                case K_VC_dsp_test_case_9_b: 
                    {
                    if ( iDoublePause )
                        {             
                        iCamc->Record();
                        CTimer::After( 2*60 * TInt32 ( 1E6 ) ); // Record 2 minutes
                        iPaused = EFalse;
                        iAction = K_VC_dsp_none;
                        }
                    else 
                        {
                        iCamc->Record();
                        CTimer::After(  2*60 * TInt32 ( 1E6 ) ); // Record 2 minutes
                        iPaused = EFalse;
                        iDoublePause = ETrue;
                        }
                    break;
                    }
                case K_VC_dsp_test_case_12:
                case K_VC_dsp_test_case_19:
                    {
                    iCamc->Record();
                    iPaused = EFalse;  
                    break;
                    }
                case K_VC_dsp_test_case_20:
                    {
                    iCamc->Record();
                    CTimer::After( 15 * TInt32 ( 1E6 ) ); // Records for 15 sec.
                    iPaused = EFalse;  
                    iAction = K_VC_dsp_none;
                    break;
                    }
                case K_VC_dsp_test_case_21:
                    {
                    iCamc->Record();
                    CTimer::After( 30 * TInt32 ( 1E6 ) ); // Records for 30 sec.
                    iPaused = EFalse;  
                    iAction = K_VC_dsp_none;
                    break;
                    }
                case K_VC_dsp_test_case_15:
                    {
                    if ( iDoublePause )
                        {             
                        iCamc->Record();
                        iAction = K_VC_dsp_none;
                        // Record until disk full
                        }
                    else 
                        {
                        iCamc->Record();
                        CTimer::After(   2*60 * TInt32 ( 1E6 ) ); // Record 2 minutes
                        iPaused = EFalse;
                        iDoublePause = ETrue;
                        }
                    break;
                    }
                default:
                    break;
                }
            }
        else //timer
            {
            switch(iAction)
                {     
                case K_VC_dsp_test_case_7:
                    {
                    iCamc->PauseL();
                    iPaused = ETrue;
                    CTimer::After( 5 * TInt32 ( 1E6 ) ); // Pause 5 sec
                    break;
                    }
                case K_VC_dsp_test_case_8: 
                    {
                    if ( i2ndTime )
                        {
                        // Volume down
                        iCamc->SetGainL( 1 );
                        iAction = K_VC_dsp_none;
                        }
                    else 
                        {                
                        // Volume Up
                        TInt aSetGain;
                        aSetGain = iCamc->MaxGainL();
                        iCamc->SetGainL( aSetGain );
                        CTimer::After( 15 * TInt32 ( 1E6 ) );
                        i2ndTime = ETrue;            
                        }
                    break;
                    }
                case K_VC_dsp_test_case_9_a:      
                case K_VC_dsp_test_case_9_b:      
                case K_VC_dsp_test_case_15:
                    {
                    iCamc->PauseL();
                    iPaused = ETrue;          
                    
                    if ( i2ndTime )
                        {
                        CTimer::After( 30 * TInt32 ( 1E6 ) ); // Pauses for 30s
                        }
                    else 
                        {                
                        CTimer::After( 10 * TInt32 ( 1E6 ) ); // Pauses for 10s
                        i2ndTime = ETrue;            
                        }
                    break;
                    }
                case K_VC_dsp_test_case_10:
                    {
                    iCamc->SetVideoBitRateL( VIDEO_BIT_RATE_10  );
                    break;
                    }
                case K_VC_dsp_test_case_11:
                case K_VC_dsp_test_case_17:
                case K_VC_dsp_test_case_17_a:
                    {
                    iCamc->SetVideoBitRateL( VIDEO_BIT_RATE_11  );
                    iAction = K_VC_dsp_none;
                    break;
                    }
                case K_VC_dsp_test_case_12:
                case K_VC_dsp_test_case_19:
                case K_VC_dsp_test_case_20:
                case K_VC_dsp_test_case_21:
                    {
                    iCamc->PauseL();
                    iPaused = ETrue;
                    CTimer::After( 10 * TInt32 ( 1E6 ) );
                    break;
                    }
                case K_VC_dsp_test_case_16:
                    {
                    if(iStartRecording2ndTime)
                        {
                        iPrepareReady = ETrue;    
                        CTimer::After( 10 * TInt32 ( 1E6 ) );
                        iAction = K_VC_dsp_none;    
                        }
                    else
                        {
                        iCamc->Stop();
                        CTimer::After( 3 * TInt32 ( 1E6 ) );
                        iStartRecording2ndTime = ETrue;
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
        
}

void CCamcTestClient_visualcheckdsp::DoCancel()
    {
    iCamc->Stop();
    iCamc->Close();
    delete iCamc;
    iCamc = NULL;
    }

void CCamcTestClient_visualcheckdsp::MvruoOpenComplete( TInt aError )
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

void CCamcTestClient_visualcheckdsp::MvruoPrepareComplete( TInt aError )
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

void CCamcTestClient_visualcheckdsp::MvruoRecordComplete( TInt aError )
    {
#ifdef _DEBUG
    RDebug::Print(_L("CamCTestClient VisualCheck:Record Complete"));
#endif
    if ((aError) && (aError != KErrCompletion))
        {
        SomethingInActiveObjectIsLeaving(aError);
        }
    else
        {
        iRecordingReady = ETrue;
        RunL(); // RunL is trapped, cannot actually leave.
        }
    }

void CCamcTestClient_visualcheckdsp::MvruoEvent(const TMMFEvent& /*aEvent*/) 
    {
    /* Should be never called ... */  
    SomethingInActiveObjectIsLeaving(99);
    }

void CCamcTestClient_visualcheckdsp::SomethingInActiveObjectIsLeaving (TInt aError)
    {
    
#ifdef _DEBUG
    RDebug::Print(_L("CamCTestClient VisualCheck: SomethingInActiveObjectIsLeaving "));
#endif
    // Something in the active object is leaving
    // or a fatal error has been found.
    
    // Stop the active object scheduler.
    iError=aError;
    iCamc->Stop();
    CActiveScheduler::Stop();
    }




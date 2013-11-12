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

#include "camctestclient_visualcheck.h"
#include <AudioPreference.h>



// EXTERNAL DATA STRUCTURES

// EXTERNAL FUNCTION PROTOTYPES 
 
// CONSTANTS

// MACROS

// LOCAL CONSTANTS AND MACROS

// MODULE DATA STRUCTURES

// LOCAL FUNCTION PROTOTYPES

// FORWARD DECLARATIONS



CCamcTestClient_visualcheck::CCamcTestClient_visualcheck () : CTimer(CActive::EPriorityHigh)
    {
    }


CCamcTestClient_visualcheck::~CCamcTestClient_visualcheck ()
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

CCamcTestClient_visualcheck* CCamcTestClient_visualcheck::NewL()
    {
    CCamcTestClient_visualcheck* self = new(ELeave) CCamcTestClient_visualcheck;
    CleanupStack::PushL(self);
    self->ConstructL( );
    CleanupStack::Pop(self);
    return self;
    }

void CCamcTestClient_visualcheck::ConstructL( )
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


void CCamcTestClient_visualcheck::Start_Active_ObjectL ( TestClientVCActions aAction )
    {
    
    TInt camHandle = 0;
    TUid controllerUid = {0x101F8503};	//implementation uid
    TUid formatUid = {0};
    TUid formatUidG2 = {0x101F86D6};
    
    
    iAction = aAction;                  //iAction might be set to NULL after action has been executed
    TBuf<50> fileName;
    
#if ( defined (__WINS__) || defined (__WINSCW__) )
    _LIT(KFileName101,"c:\\recordQCIF_101.3gp");
    _LIT(KFileName102,"c:\\recordQCIF_102.3gp");
    _LIT(KFileName103,"c:\\recordQCIF_103.3gp");
    _LIT(KFileName104,"c:\\recordQCIF_104.3gp");
    _LIT(KFileName105,"c:\\recordQCIF_105.3gp");
    _LIT(KFileName106,"c:\\recordQCIF_106.3gp");
    _LIT(KFileName107,"c:\\recordQCIF_107.3gp");
    _LIT(KFileName108_a,"c:\\recordQCIF_108_a.3gp");
    _LIT(KFileName108_b,"c:\\recordQCIF_108_b.3gp");
    _LIT(KFileName109,"c:\\recordQCIF_109.3gp");
    _LIT(KFileName110,"c:\\recordQCIF_110.3gp");
    _LIT(KFileName111,"c:\\recordQCIF_111.3gp");
    _LIT(KFileName112,"c:\\recordQCIF_112.3gp");
    _LIT(KFileName113,"c:\\recordQCIF_113.3gp");
    _LIT(KFileName114,"c:\\recordQCIF_114.3gp");
    _LIT(KFileName115,"c:\\recordQCIF_115.3gp");
    _LIT(KFileName116,"c:\\recordQCIF_116.3gp");
    _LIT(KFileName117,"c:\\recordQCIF_117.3g2");
    _LIT(KFileName118,"c:\\recordQCIF_118.3g2");
    _LIT(KFileName119,"c:\\recordQCIF_119.3gp");
    _LIT(KFileName120,"c:\\recordQCIF_120.3g2");
    _LIT(KFileName121,"c:\\recordQCIF_121.mp4");
#else 
    _LIT(KFileName101,"E:\\recordQCIF_101.3gp");
    _LIT(KFileName102,"E:\\recordQCIF_102.3gp");
    _LIT(KFileName103,"E:\\recordQCIF_103.3gp");
    _LIT(KFileName104,"E:\\recordQCIF_104.3gp");
    _LIT(KFileName105,"E:\\recordQCIF_105.3gp");
    _LIT(KFileName106,"E:\\recordQCIF_106.3gp");
    _LIT(KFileName107,"E:\\recordQCIF_107.3gp");
    _LIT(KFileName108_a,"E:\\recordQCIF_108_a.3gp");
    _LIT(KFileName108_b,"E:\\recordQCIF_108_b.3gp");
    _LIT(KFileName109,"E:\\recordQCIF_109.3gp");
    _LIT(KFileName110,"E:\\recordQCIF_110.3gp");
    _LIT(KFileName111,"E:\\recordQCIF_111.3gp");
    _LIT(KFileName112,"E:\\recordQCIF_112.3gp");
    _LIT(KFileName113,"E:\\recordQCIF_113.3gp");
    _LIT(KFileName114,"E:\\recordQCIF_114.3gp");
    _LIT(KFileName115,"C:\\recordQCIF_115.3gp");
    _LIT(KFileName116,"E:\\recordQCIF_116.3gp");
    _LIT(KFileName117,"E:\\recordQCIF_117.3g2");
    _LIT(KFileName118,"E:\\recordQCIF_118.3g2");
    _LIT(KFileName119,"E:\\recordQCIF_119.3gp");
    _LIT(KFileName120,"E:\\recordQCIF_120.3g2");
    _LIT(KFileName121,"E:\\recordQCIF_121.mp4");
#endif
    switch(iAction)
        {
        case K_VC_test_case_101:
            {
            fileName = KFileName101;
            break;
            }
        case K_VC_test_case_102:
            {
            fileName = KFileName102;
            break;
            }
        case K_VC_test_case_103:
            {
            fileName = KFileName103;
            break;
            }
        case K_VC_test_case_104:
            {
            fileName = KFileName104;
            break;
            }
        case K_VC_test_case_105:
            {
            fileName = KFileName105;
            break;
            }
        case K_VC_test_case_106:
            {
            fileName = KFileName106;
            break;
            }
        case K_VC_test_case_107:
            {
            fileName = KFileName107;
            break;
            }
        case K_VC_test_case_108_a:
            {
            fileName = KFileName108_a;
            break;
            }
        case K_VC_test_case_108_b:
            {
            fileName = KFileName108_b;
            break;
            }
        case K_VC_test_case_109:
            {
            fileName = KFileName109;
            break;
            }
        case K_VC_test_case_110:
            {
            fileName = KFileName110;
            break;
            }
        case K_VC_test_case_111:
            {
            fileName = KFileName111;
            break;
            }
        case K_VC_test_case_112:
            {
            fileName = KFileName112;
            break;
            }
        case K_VC_test_case_113:
            {
            fileName = KFileName113;
            break;
            }
        case K_VC_test_case_114:
            {
            fileName = KFileName114;
            break;
            }
        case K_VC_test_case_115:
            {
            fileName = KFileName115;
            break;
            }
        case K_VC_test_case_116:
            {
            fileName = KFileName116;
            break;
            }
        case K_VC_test_case_117:
            {
            fileName = KFileName117;
            break;
            }
        case K_VC_test_case_118:
            {
            fileName = KFileName118;
            break;
            }
        case K_VC_test_case_119:
            {
            fileName = KFileName119;
            break;
            }
        case K_VC_test_case_120:
            {
            fileName = KFileName120;
            break;
            }
        case K_VC_test_case_121:
            {
            fileName = KFileName121;
            break;
            }
        default:
            break;
        }
    
        switch(iAction)
            {
            case K_VC_test_case_108_a:
                {
                TRAPD(err,
                    iCamc->OpenFileL( fileName, camHandle, 
                    controllerUid,
                    formatUid,
                    //TFourCC( '2', '6', '3', 'B' ),
                    _L8("video/H263-2000; profile=0; level=45"),
                    TFourCC(' ', 'A', 'M', 'R') ));
                if (err)
                    {
                    User::Leave(99);
                    }
                break;
                }
            case K_VC_test_case_117:
                {
                TRAPD(err,
                    iCamc->OpenFileL( fileName, camHandle, 
                    controllerUid,
                    formatUidG2,
                    //TFourCC( '2', '6', '3', 'B' ),
                    _L8("video/H263-2000"),
                    TFourCC(' ', 'A', 'M', 'R') ));
                if (err)
                    {
                    User::Leave(99);
                    }
                break;
                }
            case K_VC_test_case_118:
            case K_VC_test_case_120:
                {
                TRAPD(err,
                    iCamc->OpenFileL( fileName, camHandle, 
                    controllerUid,
                    formatUidG2,
                    //TFourCC( '2', '6', '3', 'B' ),
                    _L8("video/mp4v-es"),
                    TFourCC(' ', 'A', 'M', 'R') ));
                if (err)
                    {
                    User::Leave(99);
                    }
                break;
                }
            case K_VC_test_case_119:
                {
                TRAPD(err,
                    iCamc->OpenFileL( fileName, camHandle, 
                    controllerUid,
                    formatUid,
                    //TFourCC( '2', '6', '3', 'B' ),
                    _L8("video/mp4v-es"),
                    TFourCC(' ', 'A', 'M', 'R') ));
                if (err)
                    {
                    User::Leave(99);
                    }
                break;
                }
            case K_VC_test_case_121:
                {
                TRAPD(err,
                    iCamc->OpenFileL( fileName, camHandle, 
                    controllerUid,
                    formatUid,
                    //TFourCC( '2', '6', '3', 'B' ),
                    _L8("video/mp4v-es; profile-level-id=3"),
                    TFourCC(' ', 'A', 'A', 'C') ));
                if (err)
                    {
                    User::Leave(99);
                    }
                break;
                }
            default:
                {
                TRAPD(err,
                    iCamc->OpenFileL( fileName, camHandle, 
                    controllerUid,
                    formatUid,
                    //TFourCC( '2', '6', '3', 'B' ),
                    _L8("video/H263-2000"),
                    TFourCC(' ', 'A', 'M', 'R') ));
                if (err)
                    {
                    User::Leave(99);
                    }
                break;          
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
    
    void CCamcTestClient_visualcheck::RunL()
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
    
    void CCamcTestClient_visualcheck::RunLTrappedL()
        {
#ifdef _DEBUG
        RDebug::Print(_L("CamCTestClient VisualCheck:RunL"));
#endif
        if ( iOpenReady ) 
            {
            iOpenReady = EFalse;
            
            switch(iAction)
                {  
                case K_VC_test_case_101:
                    {
                    break; 
                    }
                case K_VC_test_case_102:
                    {
                    iCamc->SetVideoFrameSizeL(TSize(128,96));
                    iCamc->SetVideoFrameRateL(TReal32(3));
                    iCamc->SetVideoBitRateL(TInt(28000));
                    iCamc->SetAudioEnabledL(ETrue); 
                    iCamc->SetAudioBitRateL(12200); 
                    iCamc->SetMaxClipSizeL(95000);
                    break; 
                    }
                case K_VC_test_case_103:
                    {
                    iCamc->SetVideoFrameSizeL(TSize(128,96));
                    iCamc->SetVideoFrameRateL(TReal32(5));
                    iCamc->SetVideoBitRateL(TInt(28000));
                    iCamc->SetAudioEnabledL(ETrue); 
                    iCamc->SetAudioBitRateL(6700); 
                    iCamc->SetMaxClipSizeL(95000);
                    break; 
                    }
                case K_VC_test_case_104:
                    {
                    iCamc->SetVideoFrameSizeL(TSize(128,96));
                    iCamc->SetVideoFrameRateL(TReal32(10));
                    iCamc->SetVideoBitRateL(TInt(28000));
                    iCamc->SetAudioEnabledL(ETrue); 
                    iCamc->SetAudioBitRateL(5150); 
                    iCamc->SetMaxClipSizeL(250000);
                    break; 
                    }
                case K_VC_test_case_105:
                    {
                    iCamc->SetVideoFrameSizeL(TSize(176,144));
                    iCamc->SetVideoFrameRateL(TReal32(1));
                    iCamc->SetVideoBitRateL(TInt(42000));
                    iCamc->SetAudioEnabledL(ETrue); 
                    iCamc->SetAudioBitRateL(7400); 
                    iCamc->SetMaxClipSizeL(95000);
                    break;
                    }
                case K_VC_test_case_106:
                    {
                    iCamc->SetVideoFrameSizeL(TSize(176,144));
                    iCamc->SetVideoFrameRateL(TReal32(3));
                    iCamc->SetVideoBitRateL(TInt(42000));
                    iCamc->SetAudioEnabledL(ETrue); 
                    iCamc->SetAudioBitRateL(4750); 
                    iCamc->SetMaxClipSizeL(95000);
                    break;
                    }
                case K_VC_test_case_107:
                    {
                    iCamc->SetVideoFrameSizeL(TSize(176,144));
                    iCamc->SetVideoFrameRateL(TReal32(5));
                    iCamc->SetVideoBitRateL(TInt(42000));
                    iCamc->SetAudioEnabledL(ETrue); 
                    iCamc->SetAudioBitRateL(12200); 
                    break;
                    }
                case K_VC_test_case_108_a:
                    {
                    iCamc->SetVideoFrameSizeL(TSize(176,144));
                    iCamc->SetVideoFrameRateL(TReal32(8));
                    iCamc->SetVideoBitRateL(TInt(128000));
                    iCamc->SetAudioEnabledL(ETrue); 
                    iCamc->SetAudioBitRateL(12200); 
                    iCamc->SetMaxClipSizeL(250000);
                    break;              
                    }
                case K_VC_test_case_108_b:
                    {
                    iCamc->SetVideoFrameSizeL(TSize(176,144));
                    iCamc->SetVideoFrameRateL(TReal32(1));
                    iCamc->SetVideoBitRateL(TInt(64000));
                    iCamc->SetAudioEnabledL(ETrue); 
                    iCamc->SetAudioBitRateL(12200); 
                    iCamc->SetMaxClipSizeL(250000);
                    break;              
                    }
                case K_VC_test_case_109:
                    {
                    iCamc->SetVideoFrameSizeL(TSize(176,144));
                    iCamc->SetVideoFrameRateL(TReal32(3));
                    iCamc->SetVideoBitRateL(TInt(64000));
                    iCamc->SetAudioEnabledL(ETrue); 
                    iCamc->SetAudioBitRateL(12200); 
                    break;
                    }
                case K_VC_test_case_110:
                    {
                    iCamc->SetVideoFrameSizeL(TSize(176,144));
                    iCamc->SetVideoFrameRateL(TReal32(5));
                    iCamc->SetVideoBitRateL(TInt(42000));    
                    iCamc->SetAudioEnabledL(EFalse); 
                    iCamc->SetMaxClipSizeL(95000);
                    break;
                    }
                case K_VC_test_case_111:
                    {
                    iCamc->SetVideoFrameSizeL(TSize(176,144));
                    iCamc->SetVideoFrameRateL(TReal32(5));
                    iCamc->SetVideoBitRateL(TInt(64000));
                    iCamc->SetAudioEnabledL(EFalse); 
                    iCamc->SetMaxClipSizeL(500000);
                    break;
                    }
                case K_VC_test_case_112:
                    {
                    iCamc->SetVideoFrameSizeL(TSize(128,96));
                    iCamc->SetVideoFrameRateL(TReal32(5));
                    iCamc->SetVideoBitRateL(KMMFVariableVideoBitRate);
                    iCamc->SetMaxClipSizeL(95000);
                    break;
                    }
                case K_VC_test_case_113:
                    {
                    iCamc->SetMaxClipSizeL(50);
                    break; 
                    }
                case K_VC_test_case_114:
                    {
                    break; 
                    }
                case K_VC_test_case_115:
                    {
                    iCamc->SetVideoFrameSizeL(TSize(176,144));
                    iCamc->SetVideoFrameRateL(TReal32(5));
                    iCamc->SetVideoBitRateL(KMMFVariableVideoBitRate);
                    break;
                    }    
                case K_VC_test_case_116:
                    {
                    iCamc->SetVideoFrameSizeL(TSize(176,144));
                    iCamc->SetVideoFrameRateL(TReal32(5));
                    iCamc->SetVideoBitRateL(TInt(64000));
                    iCamc->SetAudioEnabledL(ETrue); 
                    iCamc->SetAudioBitRateL(12200); 
                    break;
                    }    
                case K_VC_test_case_117:
                    {
                    iCamc->SetVideoFrameSizeL(TSize(176,144));
                    iCamc->SetVideoFrameRateL(TReal32(5));
                    iCamc->SetVideoBitRateL(TInt(64000));
                    iCamc->SetMaxClipSizeL(95000);
                    break;
                    }    
                case K_VC_test_case_118:
                    {
                    iCamc->SetVideoFrameSizeL(TSize(176,144));
                    iCamc->SetVideoFrameRateL(TReal32(5));
                    iCamc->SetVideoBitRateL(TInt(64000));
                    iCamc->SetAudioEnabledL(ETrue); 
                    iCamc->SetAudioBitRateL(12200); 
                    break;
                    }    
                case K_VC_test_case_119:
                    {
                    iCamc->SetVideoFrameSizeL(TSize(176,144));
                    iCamc->SetVideoFrameRateL(TReal32(7));
                    iCamc->SetVideoBitRateL(TInt(64000));
                    iCamc->SetMaxClipSizeL(95000);
                    iCamc->SetAudioEnabledL(ETrue); 
                    iCamc->SetAudioBitRateL(5150); 
                    break;
                    }    
                 case K_VC_test_case_120:
                    {
                    iCamc->SetVideoFrameSizeL(TSize(128,96));
                    iCamc->SetVideoFrameRateL(TReal32(10));
                    iCamc->SetVideoBitRateL(TInt(64000));
                    iCamc->SetMaxClipSizeL(500000);
                    break;
                    }    
                case K_VC_test_case_121:
                    {
                    iCamc->SetVideoFrameSizeL(TSize(352,288));
                    iCamc->SetVideoFrameRateL(TReal32(15));
                    iCamc->SetVideoBitRateL(TInt(384000));
                    iCamc->SetAudioEnabledL(ETrue); 
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
                case K_VC_test_case_101:
                    {
                    CTimer::After( 30 * TInt32 ( 1E6 ) );  // Records for 30 sec
                    break;
                    }
                case K_VC_test_case_102:
                case K_VC_test_case_103:
                case K_VC_test_case_104:
                case K_VC_test_case_105:
                case K_VC_test_case_106:
                case K_VC_test_case_108_a:
                case K_VC_test_case_117:
                case K_VC_test_case_119:
                case K_VC_test_case_113:
                    {
                    // The timeout will be generated by the SetMaxClipSize.
                    break;
                    }
                case K_VC_test_case_114:
                    {
                    CTimer::After( 1 * TInt32 ( 1E6 ) );  // Records for 1 sec
                    break;
                    }
                case K_VC_test_case_116:
                    {
                    CTimer::After( 10 * TInt32 ( 1E6 ) );  // Records for 10 sec
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
                case K_VC_test_case_107:
                    {
                    iCamc->Record();
                    CTimer::After( 50 * TInt32 ( 1E6 ) ); // Records for 50 sec.
                    iPaused = EFalse;
                    iAction = K_VC_none;         
                    break;
                    }
                case K_VC_test_case_109: 
                case K_VC_test_case_118: 
                    {
                    if ( iDoublePause )
                        {             
                        iCamc->Record();
                        CTimer::After( 2*60 * TInt32 ( 1E6 ) ); // Record 2 minutes
                        iPaused = EFalse;
                        iAction = K_VC_none;
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
                case K_VC_test_case_112:
                    {
                    iCamc->Record();
                    iPaused = EFalse;  
                    break;
                    }
                case K_VC_test_case_115:
                    {
                    if ( iDoublePause )
                        {             
                        iCamc->Record();
                        iAction = K_VC_none;
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
                case K_VC_test_case_121:
                    {
                    iCamc->Record();
                    CTimer::After( 50 * TInt32 ( 1E6 ) ); // Records for 50 sec.
                    iPaused = EFalse;
                    iAction = K_VC_none;         
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
                case K_VC_test_case_107:
                    {
                    iCamc->PauseL();
                    iPaused = ETrue;
                    CTimer::After( 5 * TInt32 ( 1E6 ) ); // Pause 5 sec
                    break;
                    }
                case K_VC_test_case_108_a: 
                    {
                    if ( i2ndTime )
                        {
                        // Volume down
                        iCamc->SetGainL( 1 );
                        iAction = K_VC_none;
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
                case K_VC_test_case_108_b: 
                    {
                    if ( i2ndTime )
                        {
                        // Volume down
                        iCamc->SetGainL( 1 );
                        iAction = K_VC_none;
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
                case K_VC_test_case_109:      
                case K_VC_test_case_115:
                case K_VC_test_case_118:
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
                case K_VC_test_case_110:
                    {
                    iCamc->SetVideoBitRateL( TCVC_VIDEO_BIT_RATE_10  );
                    break;
                    }
                case K_VC_test_case_111:
                    {
                    iCamc->SetVideoBitRateL( TCVC_VIDEO_BIT_RATE_11  );
                    iAction = K_VC_none;
                    break;
                    }
                case K_VC_test_case_112:
                    {
                    iCamc->PauseL();
                    iPaused = ETrue;
                    CTimer::After( 10 * TInt32 ( 1E6 ) );
                    break;
                    }
                case K_VC_test_case_116:
                    {
                    if(iStartRecording2ndTime)
                        {
                        iPrepareReady = ETrue;    
                        CTimer::After( 10 * TInt32 ( 1E6 ) );
                        iAction = K_VC_none;    
                        }
                    else
                        {
                        iCamc->Stop();
                        CTimer::After( 3 * TInt32 ( 1E6 ) );
                        iStartRecording2ndTime = ETrue;
                        }
                    break;
                    }
                case K_VC_test_case_120:
                    {
                    iCamc->SetVideoBitRateL( TCVC_VIDEO_BIT_RATE_20  );
                    iAction = K_VC_none;
                    break;
                    }
                case K_VC_test_case_121:
                    {
                    iCamc->PauseL();
                    iPaused = ETrue;
                    CTimer::After( 5 * TInt32 ( 1E6 ) ); // Pause 5 sec
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

void CCamcTestClient_visualcheck::DoCancel()
    {
    iCamc->Stop();
    iCamc->Close();
    delete iCamc;
    iCamc = NULL;
    }


void CCamcTestClient_visualcheck::MvruoOpenComplete( TInt aError )
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

void CCamcTestClient_visualcheck::MvruoPrepareComplete( TInt aError )
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

void CCamcTestClient_visualcheck::MvruoRecordComplete( TInt aError )
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

void CCamcTestClient_visualcheck::MvruoEvent(const TMMFEvent& /*aEvent*/) 
    {
    /* Should be never called ... */  
    SomethingInActiveObjectIsLeaving(99);
    }

void CCamcTestClient_visualcheck::SomethingInActiveObjectIsLeaving (TInt aError)
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




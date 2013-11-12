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

#include "camctestclient_9.h"
#include <CamCControllerCustomCommands.h>
#include <mmf/common/mmfstandardcustomcommands.h>
#include <mmf/server/mmffile.h>
#include <AudioPreference.h>
#include "camcTest.h"


// EXTERNAL DATA STRUCTURES

// EXTERNAL FUNCTION PROTOTYPES 
 
// CONSTANTS

const TInt KMaxVideoLength = 3600;

// MACROS
// Debug print macro
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


CCamcTestClient_9::CCamcTestClient_9 () : CTimer(CActive::EPriorityHigh)
    {
    }


CCamcTestClient_9::~CCamcTestClient_9 ()
    {
    Cancel();
    delete iCamc;
    }

CCamcTestClient_9* CCamcTestClient_9::NewL()
    {
    CCamcTestClient_9* self = new(ELeave) CCamcTestClient_9;
    CleanupStack::PushL(self);
    self->ConstructL( );
    CleanupStack::Pop(self);
    return self;
    }

void CCamcTestClient_9::ConstructL( )
    {
    iCamc = CVideoRecorderUtility::NewL( *this, KAudioPriorityVideoRecording, EMdaPriorityPreferenceTimeAndQuality );
    iOpenReady = EFalse;
    iPrepareReady = EFalse;
    iRecordingReady = EFalse;
    iPaused = EFalse;
    iNewFileName = EFalse;
    iRecord2ndTime = EFalse;
    
    CTimer::ConstructL();
    iTimeout = 0;
    iClosed = 0;
    CActiveScheduler::Add(this);
    }


void CCamcTestClient_9::Start_Active_ObjectL ( TestClient9Actions aAction, TInt aCameraHandle )
    {
    
    TFileName file;
    TInt camHandle = aCameraHandle;
    TUid controllerUid = {0x101F8503};	//implementation uid
    TUid formatUid = {0};
    TUid formatUidG2 = {0x101F86D6};
    TUid formatUidMP4 = {0x101F873D};
    
    
    iAction = aAction;                  //iAction might be set to NULL after action has been executed
    iSavedAction = aAction;             //SavedAction will never be changed.
    //	iTimeout = aTimeout;
    iTimeout = 1500000;
    
    if ( iAction == K_TC9_NewFileNameWhenRecordSecondTimeDiffFileFormat )
        {
        AddDriveLetterToPath(_L("recordQCIF.3g2"), file);
        
        TRAPD(err,
            iCamc->OpenFileL( file, camHandle, 
            controllerUid,
            formatUidG2,
            _L8("video/H263-2000"),
            TFourCC(' ', 'A', 'M', 'R') ));
        
        if (err)
            {
            User::Leave(99);
            }
        }
    else if ( iAction == K_TC9_NewFileNameWhenRecordSecondTimeMPEG4FileFormat )
        {
        AddDriveLetterToPath(_L("recordQCIF.mp4"), file);
        
        TRAPD(err,
            iCamc->OpenFileL( file, camHandle, 
            controllerUid,
            formatUidMP4,
            _L8("video/mp4v-es; profile-level-id=3"),
            TFourCC(' ', 'A', 'A', 'C') ));
        
        if (err)
            {
            User::Leave(99);
            }
        }
    else
        {       
        AddDriveLetterToPath(_L("recordQCIF.3gp"), file);
        
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

void CCamcTestClient_9::RunL()
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

void CCamcTestClient_9::RunLTrappedL()
    {
    if ( iOpenReady )  
        { 
        iOpenReady = EFalse;
        switch ( iAction )
            {
            case K_Tc9_PrepareWhenControllerReadyWhitoutSetting:
            case K_Tc9_PrepareWhenControllerReadyOnlyVideoBitRateSetting:
            case K_Tc9_PrepareWhenControllerReadyOnlyVideoFrameSizeSetting:
                {
                break;
                }
            case K_TC9_NewFileNameWhenInternalStop:
                {
                iCamc->SetMaxClipSizeL(TC9_FILE_SIZE_LIMIT) ; 
                break;
                }
            case K_TC9_NewFileNameWhenRecordSecondTimeMPEG4FileFormat:
                {
                iCamc->SetVideoFrameSizeL(TSize(352,288));
                iCamc->SetVideoFrameRateL(TReal32(15));
                iCamc->SetVideoBitRateL( TC9_VIDEO_BIT_RATE );
                iCamc->SetAudioBitRateL(TInt( TC9_AUDIO_BIT_RATE_AAC ));
                break;
                }
            default:
                {
                // Set default parameters. They can be overwritten depending on the test case. 
                iCamc->SetVideoFrameSizeL(TSize(176,144));
                iCamc->SetVideoFrameRateL(TReal32(15));
                iCamc->SetVideoBitRateL( TC9_VIDEO_BIT_RATE );
                iCamc->SetAudioBitRateL(TInt( TC9_AUDIO_BIT_RATE ));
                break;
                }

            }
        
        switch ( iAction )
            {
            case  K_Tc9_PrepareWhenControllerReadyWhitoutSetting:
                {
                break;
                }
            case K_Tc9_PrepareWhenControllerReadyOnlyVideoBitRateSetting:
                {
                iCamc->SetVideoBitRateL(TInt(50000));
                break;
                }
            case K_Tc9_PrepareWhenControllerReadyOnlyVideoFrameSizeSetting:
                {
                iCamc->SetVideoFrameSizeL(TSize(176,144));
                break;
                }
            case K_Tc9_SetMaxClipSizeLWhenControllerReadySize0:
                {
                iCamc->SetMaxClipSizeL( 0 );
                break;
                }
            case K_Tc9_SetMaxClipSizeLWhenControllerReadyKMMFNoMaxClipSize:
                {
                iCamc->SetMaxClipSizeL( KMMFNoMaxClipSize );
                break;
                }
            case K_Tc9_OpenFileInAFolderNonExistent:
                {               
                AddDriveLetterToPath(_L("non-existent-folder\\recordQCIF.3gp"), iFileName);
                TMMFFileParams params;
                params.iPath = iFileName;
                TPckgC<TMMFFileParams> pckg(params);
                    
                TUid controllerUid = {0x101F8503};	// controller implementation interface uid
                TInt ret = iCamc->CustomCommandSync( TMMFMessageDestination( controllerUid, KMMFObjectHandleController ), ECamCControllerCCNewFilename, pckg, KNullDesC8);
                if (ret != 0)
                    {
                    User::Leave(K_Tc9_ValueNotExpected);
                    }
                iAction = K_Tc9_none;
                break;
                }
            default:
                {
                break;
                }
            }
        iCamc->Prepare();
            
        }
    else if ( iPrepareReady ) 
        {
        TBool record = EFalse;
        switch ( iAction )
            {
            case K_Tc9_SetMaxClipSizeLWhenControllerReadySize0:
            case K_Tc9_SetMaxClipSizeLWhenControllerReadyKMMFNoMaxClipSize:
                {
                iPrepareReady = EFalse;
                iCamc->Record();
                record = EFalse;
                break;
                }
            case K_TC9_NewFileNameWhenPreparedNotRecording:
                {
                if (!iNewFileName)
                    {
                    AddDriveLetterToPath(_L("NewFileName.3gp"),iFileName);
                    
                    TMMFFileParams params;
                    params.iPath = iFileName;
                    TPckgC<TMMFFileParams> pckg(params);
                    
                    TUid controllerUid = {0x101F8503};	// controller implementation interface uid
                    TInt ret = iCamc->CustomCommandSync( TMMFMessageDestination( controllerUid, KMMFObjectHandleController ), ECamCControllerCCNewFilename, pckg, KNullDesC8);
                    if (ret != 0)
                        {
                        User::Leave(K_Tc9_ValueNotExpected);
                        }

                    iCamc->Prepare();
                    iAction = K_Tc9_none;
                    }
                break;
                }
            case K_TC9_NewFileNameWhenInternalStop :
                {
                iPrepareReady = EFalse;
                iCamc->Record(); // No timer need, will eventually stop because of filesize limit.
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
        TBool close = EFalse;
        iRecordingReady = EFalse;

        TMMFFileParams params;
        switch ( iAction )
            {
            case K_Tc9_RecordTimeAvailWhenRecordingReady:
                {
                // Compare if default size limit is correctly returned by 
                // a RecordTimeAvailableL      
                TTimeIntervalMicroSeconds timeRemaining = iCamc->RecordTimeAvailable();
                TInt64 timeRemainingInSec = timeRemaining.Int64()/1E6; //Max time is 3600s = 60min
                
                PRINT((_L("CCamCTest_9::RecordTimeAvailable5: timeRemainingInSec: %d "), I64INT(timeRemainingInSec  )));
                
                
                
                // Duration value depends on available disk drive space.
                // Get the free space of the disk drive:

                RFs fsSession;
                fsSession.Connect();
                TVolumeInfo volumeInfo;
                TInt err = fsSession.Volume(volumeInfo,EDriveC);
                fsSession.Close();
                if (err)
                    {
                    User::Leave( KErrGeneral) ;
                    }

                // Possible duration = available disk space/videobitrate
                TInt64 calculatedDuration = volumeInfo.iFree / TInt64((TC9_VIDEO_BIT_RATE+TC9_AUDIO_BIT_RATE)/8);
                
                // If there is more space than the max length of the video -> 
                // set the calculatedDuration to max length
                if ( calculatedDuration > KMaxVideoLength) 
                    {
                    calculatedDuration = KMaxVideoLength;
                    }
                
                RDebug::Print(_L("CCamcTest_9::RecordTimeAvailable5 timeRemaining is %d "), I64INT(timeRemainingInSec ));
                RDebug::Print(_L("CCamcTest_9::RecordTimeAvailable5 calculatedDuration is %d "), I64INT(calculatedDuration ) );
                // Allows 5% error margin:
                if ( ( timeRemainingInSec > calculatedDuration*1.05 ) ||
                    ( timeRemainingInSec < calculatedDuration*0.95 ) )
                    {
                    RDebug::Print(_L("CCamcTest_9::RecordTimeAvailable5 is leaving, because timeRemaining != Calculated ") );          
                    User::Leave( K_Tc9_ValueNotWithinRange ) ; 
                    }
                close = ETrue;
                break;
                }
                
            case K_TC9_NewFileNameWhenRecordSecondTime : 
                {
                if (!iRecord2ndTime)
                    {
                    iRecord2ndTime =ETrue;
                    AddDriveLetterToPath(_L("NewFileName.3gp"), iFileName );
                    
                    params.iPath = iFileName;
                    TPckgC<TMMFFileParams> pckg(params);
                    
                    TUid controllerUid = {0x101F8503};	// controller implementation interface uid
                    TInt ret = iCamc->CustomCommandSync( TMMFMessageDestination( controllerUid, KMMFObjectHandleController ), ECamCControllerCCNewFilename, pckg, KNullDesC8);
                    if (ret != 0)
                        {
                        User::Leave(K_Tc9_ValueNotExpected);
                        }
                    iCamc->Prepare();
                    iAction = K_Tc9_none;
                    }
                break;          
                }

            case K_TC9_NewFileNameWhenRecordSecondTimeDiffFileFormat : 
                {
                if (!iRecord2ndTime)
                    {
                    iRecord2ndTime =ETrue;
                    AddDriveLetterToPath(_L("NewFileName.3g2"), iFileName );
                    
                    params.iPath = iFileName;
                    TPckgC<TMMFFileParams> pckg(params);
                    
                    TUid controllerUid = {0x101F8503};	// controller implementation interface uid
                    TInt ret = iCamc->CustomCommandSync( TMMFMessageDestination( controllerUid, KMMFObjectHandleController ), ECamCControllerCCNewFilename, pckg, KNullDesC8);
                    if (ret != 0)
                        {
                        User::Leave(K_Tc9_ValueNotExpected);
                        }
                    iCamc->Prepare();
                    iAction = K_Tc9_none;
                    }
                break;          
                }

            case K_TC9_NewFileNameWhenRecordSecondTimeMPEG4FileFormat : 
                {
                if (!iRecord2ndTime)
                    {
                    iRecord2ndTime =ETrue;
                    AddDriveLetterToPath(_L("NewFileName.mp4"), iFileName );
                    
                    params.iPath = iFileName;
                    TPckgC<TMMFFileParams> pckg(params);
                    
                    TUid controllerUid = {0x101F8503};	// controller implementation interface uid
                    TInt ret = iCamc->CustomCommandSync( TMMFMessageDestination( controllerUid, KMMFObjectHandleController ), ECamCControllerCCNewFilename, pckg, KNullDesC8);
                    if (ret != 0)
                        {
                        User::Leave(K_Tc9_ValueNotExpected);
                        }
                    iCamc->Prepare();
                    iAction = K_Tc9_none;
                    }
                break;          
                }

            case K_TC9_NewFileNameWhenInternalStop : 
                {             
                if (!iRecord2ndTime)
                    {
                    iRecord2ndTime =ETrue;
                    AddDriveLetterToPath(_L("NewFileName.3gp"), iFileName );

                    params.iPath = iFileName;
                    TPckgC<TMMFFileParams> pckg(params);
                    
                    TUid controllerUid = {0x101F8503};	// controller implementation interface uid
                    TInt ret = iCamc->CustomCommandSync( TMMFMessageDestination( controllerUid, KMMFObjectHandleController ), ECamCControllerCCNewFilename, pckg, KNullDesC8);
                    if (ret != 0)
                        {
                        User::Leave(K_Tc9_ValueNotExpected);
                        }

                    iCamc->Prepare();
                    }
                else
                    {
                    close = ETrue;
                    }
                break;

                }               
            default:
                {
                close = ETrue;
                break;
                }
            }
        if (close)
            {
            iCamc->Close();
            iClosed = ETrue;
            CTimer::After( 1000 );
            }
        }

    else 
        {
          RunLContinuedL();
        }
        
    if ( iClosed )
        {
        iCamc->Stop();
        CActiveScheduler::Stop();
        }
    }

void CCamcTestClient_9::RunLContinuedL()
// This function exist to skip a MS VC6 bug/feature
// Without this, VC6 would complains about stack abuse. 
    {
        if ( iPaused ) //resume
        {
        switch ( iAction )
            {
            case K_TC9_NewFileNameWhenPaused : 
                {
                    
                AddDriveLetterToPath(_L("NewFileName.3gp"), iFileName );
                    
                iAction=K_Tc9_none;     
                
                TMMFFileParams params;
                params.iPath = iFileName;
                TPckgC<TMMFFileParams> pckg(params);
                
                TUid controllerUid = {0x101F8503};	// controller implementation interface uid
                TInt err = iCamc->CustomCommandSync( TMMFMessageDestination( controllerUid, KMMFObjectHandleController ), ECamCControllerCCNewFilename, pckg, KNullDesC8);
                if (err)
                    {
                    User::Leave(err);
                    }
                break;
                }
            default:   
                iPaused = EFalse;
                iCamc->Record();
            }
        }
    
    else //timer
        {
        switch ( iAction )
            {
            case K_TC9_NewFileNameWhenRecording : 
                {
                AddDriveLetterToPath(_L("NewFileName.3gp"), iFileName );
                
                TMMFFileParams params;
                params.iPath = iFileName;
                TPckgC<TMMFFileParams> pckg(params);
                
                TUid controllerUid = {0x101F8503};	// controller implementation interface uid
                TInt err = iCamc->CustomCommandSync( TMMFMessageDestination( controllerUid, KMMFObjectHandleController ), ECamCControllerCCNewFilename, pckg, KNullDesC8);
                if (err)
                    {
                    User::Leave(err);
                    }
                break;
                }            
            case K_TC9_NewFileNameWhenPaused:
                {
                iCamc->PauseL();
                CTimer::After(2 * TInt32 ( 1E6 ) ); // Pause for 2 seconds.
                iPaused=ETrue;
                break;
                }
            default:
                iCamc->Stop();        // Simulates that iRecording has been completed through the 
                // MvruoRecordComplete callback
                iRecordingReady=ETrue;				
                CTimer::After( 1000 );
            }       
        }
    
    
    }



void CCamcTestClient_9::DoCancel()
    {
    iCamc->Stop();
    iCamc->Close();
    delete iCamc;
    iCamc = NULL;
    }


void CCamcTestClient_9::MvruoOpenComplete( TInt aError )
    {
    if (aError)
        {        
        SomethingInActiveObjectIsLeaving(aError);
        }
    else
        {
        iOpenReady = ETrue;
        RunL();
        }
    }

void CCamcTestClient_9::MvruoPrepareComplete( TInt aError )
    {    
    if (aError)
        {
         SomethingInActiveObjectIsLeaving(aError);
        }
    else
        {
        iPrepareReady = ETrue;
        RunL();
        }
    }

void CCamcTestClient_9::MvruoRecordComplete( TInt aError )
    {
    switch( iAction)
        {
        case K_Tc9_SetMaxClipSizeLWhenControllerReadySize0 :
        case K_Tc9_SetMaxClipSizeLWhenControllerReadyKMMFNoMaxClipSize :
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
            break;
            }
        case K_TC9_NewFileNameWhenInternalStop :
            {
            if (aError == KErrCompletion)
                {
                // That's expected case.
                aError = KErrNone;
                }
            else
                {
                aError = KErrGeneral;
                }
            break;
            }
        }

    if (aError)
        {
         SomethingInActiveObjectIsLeaving(aError);
        }
    else
        {
        iRecordingReady = ETrue;
        RunL();
        }
    }

void CCamcTestClient_9::MvruoEvent(const TMMFEvent& /*aEvent*/) 
    {
    /* Should be never called ... */  
    SomethingInActiveObjectIsLeaving(99);
    }

void CCamcTestClient_9::SomethingInActiveObjectIsLeaving (TInt aError)
    {
    // Something in the active object is leaving
    // or a fatal error has been found.

    // Stop the active object scheduler.
        iError=aError;
        iCamc->Stop();
        CActiveScheduler::Stop();
    }



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
* Description:  Camcorder MMF API Test DLL, State transition tests
*
*/



// INCLUDE FILES
#include "camcTest_2.h"
#include "camcTest.h"


// EXTERNAL DATA STRUCTURES

// EXTERNAL FUNCTION PROTOTYPES 
 
// CONSTANTS

// MACROS

// LOCAL CONSTANTS AND MACROS

// MODULE DATA STRUCTURES

// LOCAL FUNCTION PROTOTYPES

// FORWARD DECLARATIONS


CCamcTest_2::CCamcTest_2 ()
    {
    }


CCamcTest_2::~CCamcTest_2 ()
    {
    delete iCamcTestClient;
    iCamcTestClient = NULL;
    }


void CCamcTest_2::setUpL ()
    {
#ifdef _DEBUG
    RDebug::Print(_L("CamCTest 2: SetUpL"));
#endif
    iCamcTestClient = CCamcTestClient_2::NewL( );

    iWaitScheduler = new (ELeave) CActiveSchedulerWait;	

    // Create a new Camera API implementation object, if supported
    TRAPD( err, iUiLevelCamera = CCamera::New2L( static_cast<MCameraObserver2&>(*this), 0 /*index*/, 0 /*Priority*/ ) );
    if ( err )
        {
        PRINT( ( _L( "CCamcTest_2::setUpL CCamera::New2L return code=%d" ), err ) ); 
        // Create old Camera API implementation object.
        iUiLevelCamera = CCamera::NewL( static_cast<MCameraObserver&>(*this), 0 /*Index*/ );
        }
    else 
       {
       PRINT( ( _L( "CCamcTest_2::setUp() using new MCameraObserver2" )) ); 
       }

	iUiLevelCameraHandle = iUiLevelCamera->Handle();
	iUiLevelCamera->Reserve();
	iWaitScheduler->Start();    
    }


void CCamcTest_2::tearDown ()
    {
#ifdef _DEBUG
    RDebug::Print(_L("CamCTest 2: TearDown Start"));
#endif
    delete iCamcTestClient;
#ifdef _DEBUG
    RDebug::Print(_L("CamCTest 2: TearDown Stop"));
#endif
    iCamcTestClient = NULL;
    delete iWaitScheduler;
    delete iUiLevelCamera;    
	REComSession::FinalClose();
    }


//
// Own test functions.

// //////////////////////////////////////////////////////////////////
//                       Close tests
// //////////////////////////////////////////////////////////////////


void CCamcTest_2::Close_001_L()
    {
    ///////////////////////////////////////
    //   ID: CAMC_API.CLOSE_001
    //   Action :
    //        Close when controller is open
    //   Output : 
    //        OK
    ///////////////////////////////////////
    iCamcTestClient->Start_Active_ObjectL(
        CCamcTestClient_2::KcloseWhenControllerReady, iUiLevelCameraHandle);
    }

void CCamcTest_2::Close_002_L()
    {
    ///////////////////////////////////////
    //   ID: CAMC_API.CLOSE_002
    //  Action :
    //      Close before the controller has been open
    //   Output : 
    //        OK
    ///////////////////////////////////////    
    
    iCamcTestClient->Start_Active_ObjectL(
        CCamcTestClient_2::KcloseWhenControllerNotReady, iUiLevelCameraHandle);
    }

void CCamcTest_2::Close_003_L()
    {
    ///////////////////////////////////////
    //   ID: CAMC_API.CLOSE_003
    //   Action :
    //        Close when the controller is in prepared state 
    //   Output : 
    //        OK
    ///////////////////////////////////////   
    
    iCamcTestClient->Start_Active_ObjectL(
        CCamcTestClient_2::KcloseWhenPrepareReady, iUiLevelCameraHandle);
    }

void CCamcTest_2::Close_004_L()
    {
    ///////////////////////////////////////
    //   ID: CAMC_API.CLOSE_004
    //   Action :
    //        Close after Recording is ready
    //   Output : 
    //        OK
    ///////////////////////////////////////    
    
    iCamcTestClient->Start_Active_ObjectL(
        CCamcTestClient_2::KcloseWhenRecordingReady, iUiLevelCameraHandle);
    }

void CCamcTest_2::Close_005_L()
    {
    ///////////////////////////////////////
    //   ID: CAMC_API.CLOSE_005
    //   Action :
    //        Close when Recording
    //   Output : 
    //        OK
    ///////////////////////////////////////    
    
    iCamcTestClient->Start_Active_ObjectL(
        CCamcTestClient_2::KcloseWhenRecording, iUiLevelCameraHandle);

    RFile file; 
    RFs fsSession;
    fsSession.Connect();
    TInt err;
    TFileName fileName;

    AddDriveLetterToPath(_L("recordQCIF.3gp"), fileName);
    if ( err = file.Open(fsSession,fileName,EFileShareAny) )
        {
        file.Close();
        fsSession.Close();
        assertTIntsEqualL( KErrNone, err );
        }
    
    AddDriveLetterToPath(_L("CamcorderTmpDir"), fileName);
    if ( err = file.Open(fsSession,fileName,EFileShareAny) )
        {
        file.Close();
        fsSession.Close();
        assertTIntsEqualL( KErrNotFound, err );
        }

    AddDriveLetterToPath(_L("CamcorderTMP"), fileName);
    if ( err = file.Open(fsSession,fileName,EFileShareAny) )
        {
        file.Close();
        fsSession.Close();
        assertTIntsEqualL( KErrNotFound, err );
        }

    file.Close();
    fsSession.Close();
    }


void CCamcTest_2::Close_007_L()
    {
    ///////////////////////////////////////
    //   ID: CAMC_API.CLOSE_007
    //   Action :
    //        Close when the controller is in prepared state 
    //   Output : 
    //        OK
    ///////////////////////////////////////   
    
    iCamcTestClient->Start_Active_ObjectL(
        CCamcTestClient_2::KcloseWhenPrepareReadyUsingAACandMPEG4, iUiLevelCameraHandle);
    }



// //////////////////////////////////////////////////////////////////
//                       Prepare tests
// //////////////////////////////////////////////////////////////////


void CCamcTest_2::Prepare_001_L()
    {
    ///////////////////////////////////////
    //   ID: CAMC_API.PREPARE_001
    //   Action :
    //        Prepare when controller is open
    //   Output : 
    //        OK
    ///////////////////////////////////////    
    
    iCamcTestClient->Start_Active_ObjectL(
        CCamcTestClient_2::KprepareWhenControllerReady, iUiLevelCameraHandle);
    }

void CCamcTest_2::Prepare_002_L()
    {
    ///////////////////////////////////////
    //   ID: CAMC_API.PREPARE_002
    //   Action :
    //        Prepare before the controller has been open
    //   Output : 
    //        OK
    //////////////////////////////////////  
    
    TRAPD(err, iCamcTestClient->Start_Active_ObjectL(
        CCamcTestClient_2::KprepareWhenControllerNotReady, iUiLevelCameraHandle));
    assertTIntsEqualL( KErrNotReady, err );
    }

void CCamcTest_2::Prepare_003_L()
    {
    ///////////////////////////////////////
    //   ID: CAMC_API.PREPARE_003
    //   Action :
    //        Call Prepare() when already prepared
    //	      No video attributes set before calling the 2nd prepare.
    //   Output : 
    //        OK
    //////////////////////////////////////   
    
    iCamcTestClient->Start_Active_ObjectL(
        CCamcTestClient_2::KprepareWhenPrepareReady, iUiLevelCameraHandle);
    }

void CCamcTest_2::Prepare_004_L()
    {
    ///////////////////////////////////////
    //   ID: CAMC_API.PREPARE_004
    //   Action :
    //        Prepare after Recording is ready
    //   Output : 
    //        OK
    ///////////////////////////////////////   
    
    iCamcTestClient->Start_Active_ObjectL(
        CCamcTestClient_2::KprepareWhenRecordingReady, iUiLevelCameraHandle);
    }

void CCamcTest_2::Prepare_005_L()
    {
    ///////////////////////////////////////
    //   ID: CAMC_API.PREPARE_005
    //   Action :
    //       Prepare when Recording
    //   Output : 
    //        Should leave with KErrNotReady
    ///////////////////////////////////////    
    
    TRAPD(err, iCamcTestClient->Start_Active_ObjectL(
        CCamcTestClient_2::KprepareWhenRecording, iUiLevelCameraHandle));
    assertTIntsEqualL( KErrNotReady, err );
    }

void CCamcTest_2::Prepare_006_L()
    {
    ///////////////////////////////////////
    //   ID: CAMC_API.PREPARE_006
    //   Action :
    //        Call Prepare() when already prepared
    //		Set the video attributes before calling the 2nd prepare.
    //   Output : 
    //        OK
    ///////////////////////////////////////    
    
    iCamcTestClient->Start_Active_ObjectL(
        CCamcTestClient_2::KprepareWhenPrepareReadySetVideoAttributes, iUiLevelCameraHandle);
    }



// //////////////////////////////////////////////////////////////////
//                       Record tests
// //////////////////////////////////////////////////////////////////


void CCamcTest_2::Record_001_L()
    {
    ///////////////////////////////////////
    //   ID: CAMC_API.RECORD_001
    //   Action :
    //        Record when controller is open, but not prepared.
    //   Output : 
    //        Leave KErrNotReady
    ///////////////////////////////////////   
    
    TRAPD(err, iCamcTestClient->Start_Active_ObjectL(
        CCamcTestClient_2::KrecordWhenControllerReady, iUiLevelCameraHandle));
    assertTIntsEqualL( KErrNotReady, err );
    }

void CCamcTest_2::Record_002_L()
    {
    ///////////////////////////////////////
    //   ID: CAMC_API.RECORD_002
    //   Action :
    //        Record before the controller has been open
    //   Output : 
    //        Leave KErrNotReady
    ///////////////////////////////////////   
    
    TRAPD(err, iCamcTestClient->Start_Active_ObjectL(
        CCamcTestClient_2::KrecordWhenControllerNotReady, iUiLevelCameraHandle)); 
    assertTIntsEqualL( KErrNotReady, err );
    }

void CCamcTest_2::Record_003_L()
    {
    ///////////////////////////////////////
    //   ID: CAMC_API.RECORD_003
    //   Action :
    //        Record when controller is prepared
    //   Output : 
    //        OK, file is recorded
    ///////////////////////////////////////    
    
    iCamcTestClient->Start_Active_ObjectL(
        CCamcTestClient_2::KrecordWhenPrepareReady, iUiLevelCameraHandle);
    }

void CCamcTest_2::Record_004_L()
    {
    ///////////////////////////////////////
    //   ID: CAMC_API.RECORD_004
    //   Action :
    //        Record when 1st recording is over.
    //		No preparation.
    //		
    //   Output : 
    //        Leave
    ///////////////////////////////////////   
    
    iCamcTestClient->Start_Active_ObjectL(
        CCamcTestClient_2::KrecordWhenRecordingReady, iUiLevelCameraHandle);
    }

void CCamcTest_2::Record_005_L()
    {
    ///////////////////////////////////////
    //   ID: CAMC_API.RECORD_005
    //   Action :
    //        Record when already recording.
    //		
    //   Output : 
    //        Ignores Record command
    ///////////////////////////////////////   
    
    iCamcTestClient->Start_Active_ObjectL(
        CCamcTestClient_2::KrecordWhenRecording, iUiLevelCameraHandle);
    }


// //////////////////////////////////////////////////////////////////
//                       SetPriorityL tests
// //////////////////////////////////////////////////////////////////


void CCamcTest_2::SetPriorityL_001_L()
    {
    ///////////////////////////////////////
    //   ID: CAMC_API.SETPRIORITYL_001
    //   Action :
    //        SetPriorityL when controller is open, but not prepared.
    //   Output : 
    //        OK
    //////////////////////////////////////   
    
    iCamcTestClient->Start_Active_ObjectL(
        CCamcTestClient_2::KSetPriorityLWhenControllerReady, iUiLevelCameraHandle);
    }

void CCamcTest_2::SetPriorityL_002_L()
    {
    ///////////////////////////////////////
    //   ID: CAMC_API.SETPRIORITYL_002
    //   Action :
    //        SetPriorityL before the controller has been open
    //   Output : 
    //        Leave with KErrNotReady
    ///////////////////////////////////////    
    
    TRAPD(err, iCamcTestClient->Start_Active_ObjectL(
        CCamcTestClient_2::KSetPriorityLWhenControllerNotReady, iUiLevelCameraHandle));
    assertTIntsEqualL( KErrNotReady, err );
    }

void CCamcTest_2::SetPriorityL_003_L()
    {
    ///////////////////////////////////////
    //   ID: CAMC_API.SETPRIORITYL_003
    //   Action :
    //        SetPriorityL when prepare is ready
    //        Call prepare
    //        Ask for the value of priority
    //   Output : 
    //        OK
    //////////////////////////////////////    
    
    iCamcTestClient->Start_Active_ObjectL(
        CCamcTestClient_2::KSetPriorityLWhenPrepareReady, iUiLevelCameraHandle);
    }

void CCamcTest_2::SetPriorityL_004_L()
    {
    ///////////////////////////////////////
    //   ID: CAMC_API.SETPRIORITYL_004
    //   Action :
    //        SetPriorityL while recording
    //		
    //   Output : 
    //        Leaves with KErrNotReady
    ////////////////////////////////////////    
    
    TRAPD(err, iCamcTestClient->Start_Active_ObjectL(
        CCamcTestClient_2::KSetPriorityLWhenRecording, iUiLevelCameraHandle)  );
    assertTIntsEqualL( KErrNotReady, err );
    }



// //////////////////////////////////////////////////////////////////
//                       SetVideoFrameRateL tests
// //////////////////////////////////////////////////////////////////


void CCamcTest_2::SetVideoFrameRateL_001_L()
    {
    ///////////////////////////////////////
    //   ID: CAMC_API.SETVIDEOFRAMERATEL_001
    //   Action :
    //        SetVideoFrameRateL when controller is open, but not prepared.
    //   Output : 
    //        OK
    ////////////////////////////////////////    
    
    iCamcTestClient->Start_Active_ObjectL(
        CCamcTestClient_2::KSetVideoFrameRateLWhenControllerReady, iUiLevelCameraHandle);
    }

void CCamcTest_2::SetVideoFrameRateL_002_L()
    {
    ///////////////////////////////////////
    //   ID: CAMC_API.SETVIDEOFRAMERATEL_002
    //   Action :
    //        SetVideoFrameRateL before the controller has been open
    //   Output : 
    //        Leave with KErrNotReady
    ///////////////////////////////////////    
    
    TRAPD(err, iCamcTestClient->Start_Active_ObjectL(
        CCamcTestClient_2::KSetVideoFrameRateLWhenControllerNotReady, iUiLevelCameraHandle));
    assertTIntsEqualL( KErrNotReady, err );
    }

void CCamcTest_2::SetVideoFrameRateL_003_L()
    {
    ///////////////////////////////////////
    //   ID: CAMC_API.SETVIDEOFRAMERATEL_003
    //   Action :
    //        SetVideoFrameRateL when prepare is ready
    //   Output : 
    //        OK
    ///////////////////////////////////////    
    
    iCamcTestClient->Start_Active_ObjectL(
        CCamcTestClient_2::KSetVideoFrameRateLWhenPrepareReady, iUiLevelCameraHandle);
    }

void CCamcTest_2::SetVideoFrameRateL_004_L()
    {
    ///////////////////////////////////////
    //   ID: CAMC_API.SETVIDEOFRAMERATEL_004
    //   Action :
    //        SetVideoFrameRateL while recording
    //		
    //   Output : 
    //        OK
    ///////////////////////////////////////    
    
    iCamcTestClient->Start_Active_ObjectL(
        CCamcTestClient_2::KSetVideoFrameRateLWhenRecording, iUiLevelCameraHandle);
    }




// //////////////////////////////////////////////////////////////////
//                       SetVideoFrameSizeL tests
// //////////////////////////////////////////////////////////////////


void CCamcTest_2::SetVideoFrameSizeL_001_L()
    {
    ///////////////////////////////////////
    //   ID: CAMC_API.SETVIDEOFRAMESIZEL_001
    //   Action :
    //        SetVideoFrameSizeL when controller is open, but not prepared.
    //   Output : 
    //        OK
    //////////////////////////////////////   
    
    iCamcTestClient->Start_Active_ObjectL(
        CCamcTestClient_2::KSetVideoFrameSizeLWhenControllerReady, iUiLevelCameraHandle);
    }

void CCamcTest_2::SetVideoFrameSizeL_002_L()
    {
    ///////////////////////////////////////
    //   ID: CAMC_API.SETVIDEOFRAMESIZEL_002
    //   Action :
    //        SetVideoFrameSizeL before the controller has been open
    //   Output : 
    //        Leave with KErrNotReady
    //////////////////////////////////////    
    
    TRAPD(err, iCamcTestClient->Start_Active_ObjectL(
        CCamcTestClient_2::KSetVideoFrameSizeLWhenControllerNotReady, iUiLevelCameraHandle));
    assertTIntsEqualL( KErrNotReady, err );
    }

void CCamcTest_2::SetVideoFrameSizeL_003_L()
    {
    ///////////////////////////////////////
    //   ID: CAMC_API.SETVIDEOFRAMESIZEL_003
    //   Action :
    //        SetVideoFrameSizeL when prepare is ready
    //        Start Recording
    //   Output : 
    //        Leave with KErrNotReady?
    ///////////////////////////////////////    
    
    TRAPD(err, iCamcTestClient->Start_Active_ObjectL(
        CCamcTestClient_2::KSetVideoFrameSizeLWhenPrepareReady, iUiLevelCameraHandle));
    assertTIntsEqualL( KErrNotReady, err );
    }

void CCamcTest_2::SetVideoFrameSizeL_004_L()
    {
    ///////////////////////////////////////
    //   ID: CAMC_API.SETVIDEOFRAMESIZEL_004
    //   Action :
    //        SetVideoFrameSizeL while recording
    //		
    //   Output : 
    //        Leaves with KErrNotReady
    ///////////////////////////////////////    
    
    TRAPD(err, iCamcTestClient->Start_Active_ObjectL(
        CCamcTestClient_2::KSetVideoFrameSizeLWhenRecording, iUiLevelCameraHandle));
    assertTIntsEqualL( KErrNotReady, err );
    }

void CCamcTest_2::SetVideoFrameSizeL_007_L()
    {
    ///////////////////////////////////////
    //   ID: CAMC_API.SETVIDEOFRAMESIZEL_007
    //   Action :
    //        SetVideoFrameSizeL when controller is open, but not prepared.
    //   Output : 
    //        OK
    //////////////////////////////////////   
    
    iCamcTestClient->Start_Active_ObjectL(
        CCamcTestClient_2::KSetVideoFrameSizeLWhenControllerReady_MPEG4, iUiLevelCameraHandle);
    }


// //////////////////////////////////////////////////////////////////
//                       SetVideoBitRateL tests
// //////////////////////////////////////////////////////////////////


void CCamcTest_2::SetVideoBitRateL_001_L()
    {
    ///////////////////////////////////////
    //   ID: CAMC_API.SETVIDEOBITRATEL_001
    //   Action :
    //        SetVideoBitRateL when controller is open, but not prepared.
    //   Output : 
    //        OK
    ///////////////////////////////////////   
    
    iCamcTestClient->Start_Active_ObjectL(
        CCamcTestClient_2::KSetVideoBitRateLWhenControllerReady, iUiLevelCameraHandle);
    }

void CCamcTest_2::SetVideoBitRateL_002_L()
    {
    ///////////////////////////////////////
    //   ID: CAMC_API.SETVIDEOBITRATEL_002
    //   Action :
    //        SetVideoBitRateL before the controller has been open
    //   Output : 
    //        Leave with KErrNotReady
    ///////////////////////////////////////    
    
    TRAPD(err, iCamcTestClient->Start_Active_ObjectL(
        CCamcTestClient_2::KSetVideoBitRateLWhenControllerNotReady, iUiLevelCameraHandle));
    assertTIntsEqualL( KErrNotReady, err );
    }

void CCamcTest_2::SetVideoBitRateL_003_L()
    {
    ///////////////////////////////////////
    //   ID: CAMC_API.SETVIDEOBITRATEL_003
    //   Action :
    //        SetVideoBitRateL when prepare is ready
    //   Output : 
    //        OK
    ///////////////////////////////////////    
    
    iCamcTestClient->Start_Active_ObjectL(
        CCamcTestClient_2::KSetVideoBitRateLWhenPrepareReady, iUiLevelCameraHandle);
    }

void CCamcTest_2::SetVideoBitRateL_004_L()
    {
    ///////////////////////////////////////
    //   ID: CAMC_API.SETVIDEOBITRATEL_004
    //   Action :
    //        SetVideoBitRateL while recording
    //		
    //   Output : 
    //        OK
    ///////////////////////////////////////   
    
    iCamcTestClient->Start_Active_ObjectL(
        CCamcTestClient_2::KSetVideoBitRateLWhenRecording, iUiLevelCameraHandle);
    }



// //////////////////////////////////////////////////////////////////
//                       SetAudioBitRateL tests
// //////////////////////////////////////////////////////////////////


void CCamcTest_2::SetAudioBitRateL_001_L()
    {
    ///////////////////////////////////////
    //   ID: CAMC_API.SETAUDIOBITRATEL_001
    //   Action :
    //        SetAudioBitRateL when controller is open, but not prepared.
    //   Output : 
    //        OK
    ///////////////////////////////////////    
    
    iCamcTestClient->Start_Active_ObjectL(
        CCamcTestClient_2::KSetAudioBitRateLWhenControllerReady, iUiLevelCameraHandle);
    }

void CCamcTest_2::SetAudioBitRateL_002_L()
    {
    ///////////////////////////////////////
    //   ID: CAMC_API.SETAUDIOBITRATEL_002
    //   Action :
    //        SetAudioBitRateL before the controller has been open
    //   Output : 
    //        Leave with KErrNotReady
    //////////////////////////////////////    
    
    TRAPD(err, iCamcTestClient->Start_Active_ObjectL(
        CCamcTestClient_2::KSetAudioBitRateLWhenControllerNotReady, iUiLevelCameraHandle));
    assertTIntsEqualL( KErrNotReady, err );
    }

void CCamcTest_2::SetAudioBitRateL_003_L()
    {
    ///////////////////////////////////////
    //   ID: CAMC_API.SETAUDIOBITRATEL_003
    //   Action :
    //        SetAudioBitRateL when prepare is ready
    //        Call prepare
    //        Ask for bit rate.
    //   Output : 
    //        Bit rate is same as set.
    ///////////////////////////////////////    
    
    iCamcTestClient->Start_Active_ObjectL(
        CCamcTestClient_2::KSetAudioBitRateLWhenPrepareReady, iUiLevelCameraHandle);
    }

void CCamcTest_2::SetAudioBitRateL_004_L()
    {
    ///////////////////////////////////////
    //   ID: CAMC_API.SETAUDIOBITRATEL_004
    //   Action :
    //        SetAudioBitRateL while recording
    //		
    //   Output : 
    //        Leave with KErrNotReady
    //////////////////////////////////////    
    
    TRAPD(err, iCamcTestClient->Start_Active_ObjectL(
        CCamcTestClient_2::KSetAudioBitRateLWhenRecording, iUiLevelCameraHandle));
    assertTIntsEqualL( KErrNotReady, err );

    }


// //////////////////////////////////////////////////////////////////
//                       SetAudioEnabledL tests
// //////////////////////////////////////////////////////////////////


void CCamcTest_2::SetAudioEnabledL_001_L()
    {
    ///////////////////////////////////////
    //   ID: CAMC_API.SETAUDIOENABLEDL_001
    //   Action :
    //        SetAudioEnabledL when controller is open, but not prepared.
    //   Output : 
    //        OK
    //////////////////////////////////////   
    
    iCamcTestClient->Start_Active_ObjectL(
        CCamcTestClient_2::KSetAudioEnabledLWhenControllerReady, iUiLevelCameraHandle);
    }

void CCamcTest_2::SetAudioEnabledL_002_L()
    {
    ///////////////////////////////////////
    //   ID: CAMC_API.SETAUDIOENABLEDL_002
    //   Action :
    //        SetAudioEnabledL before the controller has been open
    //   Output : 
    //        Leave with KErrNotReady
    /////////////////////////////////////    
    
    TRAPD(err, iCamcTestClient->Start_Active_ObjectL(
        CCamcTestClient_2::KSetAudioEnabledLWhenControllerNotReady, iUiLevelCameraHandle));
    assertTIntsEqualL( KErrNotReady, err );
    }

void CCamcTest_2::SetAudioEnabledL_003_L()
    {
    ///////////////////////////////////////
    //   ID: CAMC_API.SETAUDIOENABLEDL_003
    //   Action :
    //        SetAudioEnabledL when prepare is ready
    //   Output : 
    //        Leave with KErrNotReady
    ////////////////////////////////////////    
    
    TRAPD(err,iCamcTestClient->Start_Active_ObjectL(
        CCamcTestClient_2::KSetAudioEnabledLWhenPrepareReady, iUiLevelCameraHandle));
    assertTIntsEqualL( KErrNotReady, err );
    }

void CCamcTest_2::SetAudioEnabledL_004_L()
    {
    ///////////////////////////////////////
    //   ID: CAMC_API.SETAUDIOENABLEDL_004
    //   Action :
    //        SetAudioEnabledL while recording
    //		
    //   Output : 
    //        Leave with KErrNotReady
    ///////////////////////////////////////    
    
    TRAPD(err,iCamcTestClient->Start_Active_ObjectL(
        CCamcTestClient_2::KSetAudioEnabledLWhenRecording, iUiLevelCameraHandle));
    assertTIntsEqualL( KErrNotReady, err );
    }



// //////////////////////////////////////////////////////////////////
//                       SetMaxClipSizeL tests
// //////////////////////////////////////////////////////////////////

void CCamcTest_2::SetMaxClipSizeL_001_L()
    {
    ///////////////////////////////////////
    //   ID: CAMC_API.SETMAXCLIPSIZEL_001
    //   Action :
    //        SetMaxClipSizeL when controller is open, but not prepared.
    //   Output : 
    //        Size is reached. KErrCompletion is received
    //////////////////////////////////////   
    
    TRAPD(err, iCamcTestClient->Start_Active_ObjectL(
        CCamcTestClient_2::KSetMaxClipSizeLWhenControllerReady, iUiLevelCameraHandle));
    assertTIntsEqualL( KErrCompletion, err );
    }


void CCamcTest_2::SetMaxClipSizeL_002_L()
    {
    ///////////////////////////////////////
    //   ID: CAMC_API.SETMAXCLIPSIZEL_002
    //   Action :
    //        SetMaxClipSizeL before the controller has been open
    //   Output : 
    //        Leave with KErrNotReady
    //////////////////////////////////////    
    
    TRAPD(err, iCamcTestClient->Start_Active_ObjectL(
        CCamcTestClient_2::KSetMaxClipSizeLWhenControllerNotReady, iUiLevelCameraHandle));
    assertTIntsEqualL( KErrNotReady, err );
    }

void CCamcTest_2::SetMaxClipSizeL_003_L()
    {
    ///////////////////////////////////////
    //   ID: CAMC_API.SETMAXCLIPSIZEL_003
    //   Action :
    //        SetMaxClipSizeL when prepare is ready
    //   Output : 
    //        Leave with KErrNotReady
    ///////////////////////////////////////    
    
    TRAPD(err,iCamcTestClient->Start_Active_ObjectL(
        CCamcTestClient_2::KSetMaxClipSizeLWhenPrepareReady, iUiLevelCameraHandle));
    assertTIntsEqualL( KErrNotReady, err );
    }

void CCamcTest_2::SetMaxClipSizeL_004_L()
    {
    ///////////////////////////////////////
    //   ID: CAMC_API.SETMAXCLIPSIZEL_004
    //   Action :
    //        SetMaxClipSizeL while recording
    //		
    //   Output : 
    //        Leave with KErrNotReady
    //////////////////////////////////////    
    
    TRAPD(err,iCamcTestClient->Start_Active_ObjectL(
        CCamcTestClient_2::KSetMaxClipSizeLWhenRecording, iUiLevelCameraHandle));
    assertTIntsEqualL( KErrNotReady, err );
    }


// //////////////////////////////////////////////////////////////////
//                       SetVideoTypeL tests
// //////////////////////////////////////////////////////////////////


void CCamcTest_2::SetVideoTypeL_001_L()
    {
    ///////////////////////////////////////
    //   ID: CAMC_API.SETVIDEOTYPEL_001
    //   Action :
    //        SetVideoTypeL when controller is open, but not prepared.
    //   Output : 
    //        OK
    ///////////////////////////////////////   
    
    iCamcTestClient->Start_Active_ObjectL(
        CCamcTestClient_2::KSetVideoTypeLWhenControllerReady, iUiLevelCameraHandle);
    }

void CCamcTest_2::SetVideoTypeL_002_L()
    {
    ///////////////////////////////////////
    //   ID: CAMC_API.SETVIDEOTYPEL_002
    //   Action :
    //        SetVideoTypeL before the controller has been open
    //   Output : 
    //        Leave with KErrNotReady
    ///////////////////////////////////////    
    
    TRAPD(err, iCamcTestClient->Start_Active_ObjectL(
        CCamcTestClient_2::KSetVideoTypeLWhenControllerNotReady, iUiLevelCameraHandle));
    assertTIntsEqualL( KErrNotReady, err );
    }

void CCamcTest_2::SetVideoTypeL_003_L()
    {
    ///////////////////////////////////////
    //   ID: CAMC_API.SETVIDEOTYPEL_003
    //   Action :
    //        SetVideoTypeL when prepare is ready
    //        Record
    //   Output : 
    //        Leave with KErrNotReady
    ///////////////////////////////////////    
    
    TRAPD(err,iCamcTestClient->Start_Active_ObjectL(
        CCamcTestClient_2::KSetVideoTypeLWhenPrepareReady, iUiLevelCameraHandle));
    assertTIntsEqualL( KErrNotReady, err );
    }

void CCamcTest_2::SetVideoTypeL_004_L()
    {
    ///////////////////////////////////////
    //   ID: CAMC_API.SETVIDEOTYPEL_004
    //   Action :
    //        SetVideoTypeL while recording
    //		
    //   Output : 
    //        Leave with KErrNotReady
    //////////////////////////////////////    
    
    TRAPD(err,iCamcTestClient->Start_Active_ObjectL(
        CCamcTestClient_2::KSetVideoTypeLWhenRecording, iUiLevelCameraHandle));
    assertTIntsEqualL( KErrNotReady, err );
    }


// //////////////////////////////////////////////////////////////////
//                       SetAudioTypeL tests
// //////////////////////////////////////////////////////////////////


void CCamcTest_2::SetAudioTypeL_001_L()
    {
    ///////////////////////////////////////
    //   ID: CAMC_API.SETAUDIOTYPEL_001
    //   Action :
    //        SetAudioTypeL when controller is open, but not prepared.
    //   Output : 
    //        OK
    /////////////////////////////////////// 
    
    iCamcTestClient->Start_Active_ObjectL(
        CCamcTestClient_2::KSetAudioTypeLWhenControllerReady, iUiLevelCameraHandle);
    }

void CCamcTest_2::SetAudioTypeL_002_L()
    {
    ///////////////////////////////////////
    //   ID: CAMC_API.SETAUDIOTYPEL_002
    //   Action :
    //        SetAudioTypeL before the controller has been open
    //   Output : 
    //        Leave with KErrNotReady
    ///////////////////////////////////////   
    
    TRAPD(err, iCamcTestClient->Start_Active_ObjectL(
        CCamcTestClient_2::KSetAudioTypeLWhenControllerNotReady, iUiLevelCameraHandle));
    assertTIntsEqualL( KErrNotReady, err );
    }

void CCamcTest_2::SetAudioTypeL_003_L()
    {
    ///////////////////////////////////////
    //   ID: CAMC_API.SETAUDIOTYPEL_003
    //   Action :
    //        SetAudioTypeL when prepare is ready
    //        Record
    //   Output : 
    //        Leave with KErrNotReady
    ///////////////////////////////////////   
    
    TRAPD(err,iCamcTestClient->Start_Active_ObjectL(
        CCamcTestClient_2::KSetAudioTypeLWhenPrepareReady, iUiLevelCameraHandle));
    assertTIntsEqualL( KErrNotReady, err );
    }

void CCamcTest_2::SetAudioTypeL_004_L()
    {
    ///////////////////////////////////////
    //   ID: CAMC_API.SETAUDIOTYPEL_004
    //   Action :
    //        SetAudioTypeL while recording
    //		
    //   Output : 
    //        Leave with KErrNotReady
    ///////////////////////////////////////    
    
    TRAPD(err,iCamcTestClient->Start_Active_ObjectL(
        CCamcTestClient_2::KSetAudioTypeLWhenRecording, iUiLevelCameraHandle));
    assertTIntsEqualL( KErrNotReady, err );
    }



// //////////////////////////////////////////////////////////////////
//   Dummy Camera API callback functions
// //////////////////////////////////////////////////////////////////
void CCamcTest_2::ReserveComplete(TInt /*aError*/)
    {
    RDebug::Print(_L("CCamcTest_2::ReserveComplete"));
        
    // UI level camera is being initialized        
    if (iWaitScheduler->IsStarted())
	    {
    	 iUiLevelCamera->PowerOn();
    	 return;
	    }
    }

void CCamcTest_2::PowerOnComplete(TInt /*aError*/)
    {   
    RDebug::Print(_L("CCamcTest_2::PowerOnComplete"));

    // UI level camera is being initialized
    if (iWaitScheduler->IsStarted())
	    {
    	 iWaitScheduler->AsyncStop();
    	 return;
    	}        
    }

void CCamcTest_2::ViewFinderFrameReady(CFbsBitmap& /*aFrame*/)
    {
    }
void CCamcTest_2::ImageReady(CFbsBitmap* /*aBitmap */,HBufC8* /*aData*/,TInt /*aError*/)
    {    
    }
void CCamcTest_2::FrameBufferReady(MFrameBuffer* /*aFrameBuffer*/,TInt /*aError*/ )
    {
    }

// -----------------------------------------------------------------------------
// CCamcTest_2::HandleEvent
// MCameraObserver2 call-back handler
// -----------------------------------------------------------------------------
//
void CCamcTest_2::HandleEvent( const TECAMEvent& aEvent)
    {
    PRINT(( _L( "CCamcTest_2::HandleEvent() entering, type=%x, err=%d" ), aEvent.iEventType.iUid, aEvent.iErrorCode ));

    if ( aEvent.iEventType == KUidECamEventCameraNoLongerReserved )
        {
        PRINT(( _L( "CCamcTest_2::HandleEvent() KUidECamEventCameraNoLongerReserved" ) ));
        }
    else if ( aEvent.iEventType == KUidECamEventPowerOnComplete )
        {
        PRINT(( _L( "CCamcTest_2::HandleEvent() KUidECamEventPowerOnComplete" ) ));
        PowerOnComplete( aEvent.iErrorCode );
        }
    else if ( aEvent.iEventType == KUidECamEventReserveComplete )
       {
       PRINT(( _L( "CCamcTest_2::HandleEvent() KUidECamEventReserveComplete" ) ));
       ReserveComplete( aEvent.iErrorCode );
       }

    PRINT(( _L( "CCamcTestClient_7::HandleEvent() returning" ) ));
    }

// -----------------------------------------------------------------------------
// CCamcTest_2::ViewFinderReady
// MCameraObserver2 call-back handler
// -----------------------------------------------------------------------------
//
void CCamcTest_2::ViewFinderReady( MCameraBuffer& aCameraBuffer, TInt aError)
    {
    PRINT(( _L( "CCamcTest_2::ViewFinderReady() NOT HANDLED err=%d, NumFrames=%d" ), aError, aCameraBuffer.NumFrames() ));
    CFbsBitmap *buffer = NULL;
    if ( !aError )
        {
	    aCameraBuffer.Release();
        }
    ViewFinderFrameReady( *buffer );
    }

// -----------------------------------------------------------------------------
// CCamcTest_2::ImageBufferReady
// MCameraObserver2 call-back handler
// -----------------------------------------------------------------------------
//
void CCamcTest_2::ImageBufferReady( MCameraBuffer& aCameraBuffer, TInt aError)
    {
    PRINT(( _L( "CCamcTest_2::ImageBufferReady() BUFFER NOT HANDLED, err=%d, NumFrames=%d" ), aError, aCameraBuffer.NumFrames() ));
	
    if ( !aError )
        {
	    aCameraBuffer.Release();
        }
    ImageReady( NULL, NULL, aError );
    }
    
// -----------------------------------------------------------------------------
// CCamcTest_2::VideoBufferReady
// MCameraObserver2 call-back handler
// -----------------------------------------------------------------------------
//
void CCamcTest_2::VideoBufferReady( MCameraBuffer& aCameraBuffer, TInt aError)
    {
    PRINT(( _L( "CCamcTest_2::VideoBufferReady() entering, err=%d, NumFrames=%d" ), aError, aCameraBuffer.NumFrames() ));
    MFrameBuffer* buffer = NULL;
	
    if ( !aError )
        {
        aCameraBuffer.Release();
        }
    // Call old Camera-API observer handler
    FrameBufferReady( buffer, aError );
    
    PRINT(( _L( "CCamcTest_2::VideoBufferReady() returning" )));
    }



//
// An own static function to collect the test functions into one 
// suite of tests. The framework will run the tests and free the
// memory allocated for the test suite.
// 
MTest* CCamcTest_2::suiteL ()
    {
    // Always use NewL (Do not use NewLC) !!!
    CTestSuite *suite = CTestSuite::NewL(_L8("State Transition tests (CamcTest2)"));
    CleanupStack::PushL( suite );
        
    suite->addTestL(CTestCaller<CCamcTest_2>::NewL(_L8("CAMC_API.CLOSE_001"), &CCamcTest_2::Close_001_L));
    suite->addTestL(CTestCaller<CCamcTest_2>::NewL(_L8("CAMC_API.CLOSE_002"), &CCamcTest_2::Close_002_L));
    suite->addTestL(CTestCaller<CCamcTest_2>::NewL(_L8("CAMC_API.CLOSE_003"), &CCamcTest_2::Close_003_L)); 
    suite->addTestL(CTestCaller<CCamcTest_2>::NewL(_L8("CAMC_API.CLOSE_004"), &CCamcTest_2::Close_004_L)); 
    suite->addTestL(CTestCaller<CCamcTest_2>::NewL(_L8("CAMC_API.CLOSE_005"), &CCamcTest_2::Close_005_L));
#ifdef MP4_FILE_FORMAT_SUPPORTED
#if ((!defined __WINS__) || (!defined __WINSCW__)) // AAC supported only in HW
    suite->addTestL(CTestCaller<CCamcTest_2>::NewL(_L8("CAMC_API.CLOSE_007"), &CCamcTest_2::Close_007_L)); 
#endif
#endif
    
    suite->addTestL(CTestCaller<CCamcTest_2>::NewL(_L8("CAMC_API.PREPARE_001"), &CCamcTest_2::Prepare_001_L));
    suite->addTestL(CTestCaller<CCamcTest_2>::NewL(_L8("CAMC_API.PREPARE_002"), &CCamcTest_2::Prepare_002_L));
    suite->addTestL(CTestCaller<CCamcTest_2>::NewL(_L8("CAMC_API.PREPARE_003"), &CCamcTest_2::Prepare_003_L));
    suite->addTestL(CTestCaller<CCamcTest_2>::NewL(_L8("CAMC_API.PREPARE_004"), &CCamcTest_2::Prepare_004_L));
    suite->addTestL(CTestCaller<CCamcTest_2>::NewL(_L8("CAMC_API.PREPARE_005"), &CCamcTest_2::Prepare_005_L));
    suite->addTestL(CTestCaller<CCamcTest_2>::NewL(_L8("CAMC_API.PREPARE_006"), &CCamcTest_2::Prepare_006_L));
   
    suite->addTestL(CTestCaller<CCamcTest_2>::NewL(_L8("CAMC_API.RECORD_001"), &CCamcTest_2::Record_001_L));
    suite->addTestL(CTestCaller<CCamcTest_2>::NewL(_L8("CAMC_API.RECORD_002"), &CCamcTest_2::Record_002_L));
    suite->addTestL(CTestCaller<CCamcTest_2>::NewL(_L8("CAMC_API.RECORD_003"), &CCamcTest_2::Record_003_L));
    
    suite->addTestL(CTestCaller<CCamcTest_2>::NewL(_L8("CAMC_API.RECORD_004"), &CCamcTest_2::Record_004_L));
    suite->addTestL(CTestCaller<CCamcTest_2>::NewL(_L8("CAMC_API.RECORD_005"), &CCamcTest_2::Record_005_L));
    suite->addTestL(CTestCaller<CCamcTest_2>::NewL(_L8("CAMC_API.SETPRIORITYL_001"), &CCamcTest_2::SetPriorityL_001_L));
    suite->addTestL(CTestCaller<CCamcTest_2>::NewL(_L8("CAMC_API.SETPRIORITYL_002"), &CCamcTest_2::SetPriorityL_002_L));
    suite->addTestL(CTestCaller<CCamcTest_2>::NewL(_L8("CAMC_API.SETPRIORITYL_003"), &CCamcTest_2::SetPriorityL_003_L));
    suite->addTestL(CTestCaller<CCamcTest_2>::NewL(_L8("CAMC_API.SETPRIORITYL_004"), &CCamcTest_2::SetPriorityL_004_L));
    
  
    suite->addTestL(CTestCaller<CCamcTest_2>::NewL(_L8("CAMC_API.SETVIDEOFRAMERATEL_001"), &CCamcTest_2::SetVideoFrameRateL_001_L));
    suite->addTestL(CTestCaller<CCamcTest_2>::NewL(_L8("CAMC_API.SETVIDEOFRAMERATEL_002"), &CCamcTest_2::SetVideoFrameRateL_002_L));     
    suite->addTestL(CTestCaller<CCamcTest_2>::NewL(_L8("CAMC_API.SETVIDEOFRAMERATEL_003"), &CCamcTest_2::SetVideoFrameRateL_003_L));
    suite->addTestL(CTestCaller<CCamcTest_2>::NewL(_L8("CAMC_API.SETVIDEOFRAMERATEL_004"), &CCamcTest_2::SetVideoFrameRateL_004_L));
    
    suite->addTestL(CTestCaller<CCamcTest_2>::NewL(_L8("CAMC_API.SETVIDEOFRAMESIZEL_001"), &CCamcTest_2::SetVideoFrameSizeL_001_L));
    suite->addTestL(CTestCaller<CCamcTest_2>::NewL(_L8("CAMC_API.SETVIDEOFRAMESIZEL_002"), &CCamcTest_2::SetVideoFrameSizeL_002_L));
    suite->addTestL(CTestCaller<CCamcTest_2>::NewL(_L8("CAMC_API.SETVIDEOFRAMESIZEL_003"), &CCamcTest_2::SetVideoFrameSizeL_003_L));
    suite->addTestL(CTestCaller<CCamcTest_2>::NewL(_L8("CAMC_API.SETVIDEOFRAMESIZEL_004"), &CCamcTest_2::SetVideoFrameSizeL_004_L));

#ifdef MP4_FILE_FORMAT_SUPPORTED
#if ((!defined __WINS__) || (!defined __WINSCW__)) // AAC supported only in HW
    suite->addTestL(CTestCaller<CCamcTest_2>::NewL(_L8("CAMC_API.SETVIDEOFRAMESIZEL_007"), &CCamcTest_2::SetVideoFrameSizeL_007_L));
#endif
#endif
   
    
    suite->addTestL(CTestCaller<CCamcTest_2>::NewL(_L8("CAMC_API.SETVIDEOBITRATEL_001"), &CCamcTest_2::SetVideoBitRateL_001_L));
    suite->addTestL(CTestCaller<CCamcTest_2>::NewL(_L8("CAMC_API.SETVIDEOBITRATEL_002"), &CCamcTest_2::SetVideoBitRateL_002_L));
    suite->addTestL(CTestCaller<CCamcTest_2>::NewL(_L8("CAMC_API.SETVIDEOBITRATEL_003"), &CCamcTest_2::SetVideoBitRateL_003_L));
    suite->addTestL(CTestCaller<CCamcTest_2>::NewL(_L8("CAMC_API.SETVIDEOBITRATEL_004"), &CCamcTest_2::SetVideoBitRateL_004_L));

    
    suite->addTestL(CTestCaller<CCamcTest_2>::NewL(_L8("CAMC_API.SETAUDIOBITRATEL_001"), &CCamcTest_2::SetAudioBitRateL_001_L));
    suite->addTestL(CTestCaller<CCamcTest_2>::NewL(_L8("CAMC_API.SETAUDIOBITRATEL_002"), &CCamcTest_2::SetAudioBitRateL_002_L));
    suite->addTestL(CTestCaller<CCamcTest_2>::NewL(_L8("CAMC_API.SETAUDIOBITRATEL_003"), &CCamcTest_2::SetAudioBitRateL_003_L));
    suite->addTestL(CTestCaller<CCamcTest_2>::NewL(_L8("CAMC_API.SETAUDIOBITRATEL_004"), &CCamcTest_2::SetAudioBitRateL_004_L));
    
    suite->addTestL(CTestCaller<CCamcTest_2>::NewL(_L8("CAMC_API.SETAUDIOENABLEDL_001"), &CCamcTest_2::SetAudioEnabledL_001_L));
    suite->addTestL(CTestCaller<CCamcTest_2>::NewL(_L8("CAMC_API.SETAUDIOENABLEDL_002"), &CCamcTest_2::SetAudioEnabledL_002_L));
    suite->addTestL(CTestCaller<CCamcTest_2>::NewL(_L8("CAMC_API.SETAUDIOENABLEDL_003"), &CCamcTest_2::SetAudioEnabledL_003_L));
    suite->addTestL(CTestCaller<CCamcTest_2>::NewL(_L8("CAMC_API.SETAUDIOENABLEDL_004"), &CCamcTest_2::SetAudioEnabledL_004_L));
    

    suite->addTestL(CTestCaller<CCamcTest_2>::NewL(_L8("CAMC_API.SETMAXCLIPSIZEL_001"), &CCamcTest_2::SetMaxClipSizeL_001_L));
    suite->addTestL(CTestCaller<CCamcTest_2>::NewL(_L8("CAMC_API.SETMAXCLIPSIZEL_002"), &CCamcTest_2::SetMaxClipSizeL_002_L));
    suite->addTestL(CTestCaller<CCamcTest_2>::NewL(_L8("CAMC_API.SETMAXCLIPSIZEL_003"), &CCamcTest_2::SetMaxClipSizeL_003_L));
    suite->addTestL(CTestCaller<CCamcTest_2>::NewL(_L8("CAMC_API.SETMAXCLIPSIZEL_004"), &CCamcTest_2::SetMaxClipSizeL_004_L));
    

    suite->addTestL(CTestCaller<CCamcTest_2>::NewL(_L8("CAMC_API.SETVIDEOTYPEL_001"), &CCamcTest_2::SetVideoTypeL_001_L));
    suite->addTestL(CTestCaller<CCamcTest_2>::NewL(_L8("CAMC_API.SETVIDEOTYPEL_002"), &CCamcTest_2::SetVideoTypeL_002_L));
    suite->addTestL(CTestCaller<CCamcTest_2>::NewL(_L8("CAMC_API.SETVIDEOTYPEL_003"), &CCamcTest_2::SetVideoTypeL_003_L));  
    suite->addTestL(CTestCaller<CCamcTest_2>::NewL(_L8("CAMC_API.SETVIDEOTYPEL_004"), &CCamcTest_2::SetVideoTypeL_004_L));

    suite->addTestL(CTestCaller<CCamcTest_2>::NewL(_L8("CAMC_API.SETAUDIOTYPEL_001"), &CCamcTest_2::SetAudioTypeL_001_L));
    suite->addTestL(CTestCaller<CCamcTest_2>::NewL(_L8("CAMC_API.SETAUDIOTYPEL_002"), &CCamcTest_2::SetAudioTypeL_002_L));
    suite->addTestL(CTestCaller<CCamcTest_2>::NewL(_L8("CAMC_API.SETAUDIOTYPEL_003"), &CCamcTest_2::SetAudioTypeL_003_L));
    suite->addTestL(CTestCaller<CCamcTest_2>::NewL(_L8("CAMC_API.SETAUDIOTYPEL_004"), &CCamcTest_2::SetAudioTypeL_004_L));
    
    CleanupStack::Pop( suite );
    return suite;
    }
    

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
#include "camcTest_5.h"
#include "camcTest.h"
#include <bldvariant.hrh>


// EXTERNAL DATA STRUCTURES

// EXTERNAL FUNCTION PROTOTYPES 
 
// CONSTANTS

// MACROS

// LOCAL CONSTANTS AND MACROS

// MODULE DATA STRUCTURES

// LOCAL FUNCTION PROTOTYPES

// FORWARD DECLARATIONS


CCamcTest_5::CCamcTest_5 ()
    {
    }

CCamcTest_5::~CCamcTest_5 ()
    {
    delete iCamcTestClient;
    iCamcTestClient = NULL;
    }


void CCamcTest_5::setUpL ()
    {
    iCamcTestClient = CCamcTestClient_5::NewL( );
    
    iWaitScheduler = new (ELeave) CActiveSchedulerWait;	

    // Create a new Camera API implementation object, if supported
    TRAPD( err, iUiLevelCamera = CCamera::New2L( static_cast<MCameraObserver2&>(*this), 0 /*index*/, 0 /*Priority*/ ) );
    if ( err )
        {
        PRINT( ( _L( "CCamcTest_5::setUpL CCamera::New2L return code=%d" ), err ) ); 
        // Create old Camera API implementation object.
        iUiLevelCamera = CCamera::NewL( static_cast<MCameraObserver&>(*this), 0 /*Index*/ );
        }
    else 
       {
       PRINT( ( _L( "CCamcTest_5::setUp() using new MCameraObserver2" )) ); 
       }

	iUiLevelCameraHandle = iUiLevelCamera->Handle();
	iUiLevelCamera->Reserve();
	iWaitScheduler->Start();    
    }


void CCamcTest_5::tearDown ()
    {
    delete iCamcTestClient;
    iCamcTestClient = NULL;
    delete iWaitScheduler;
    delete iUiLevelCamera;        
	REComSession::FinalClose();
    }


//
// Own test functions.
//

// //////////////////////////////////////////////////////////////////
//                       Memory Failure tests
// //////////////////////////////////////////////////////////////////

/******************************/
/*NUMBER OF META DATA ENTRIES */
/******************************/

void CCamcTest_5::NumberOfMetaDataEntriesL_001_L()
    {
    ///////////////////////////////////////
	//   ID: CAMC_API.NUMBEROFMETADATAENTRIESL_001
    //   Action :
    //       Call NumberOfMetaDataEntriesL() when not recording
    //   Output : 
    //       Function should retrun 0.
    //   Precondition :
    //       Not recording
    ///////////////////////////////////////

        iCamcTestClient->Start_Active_ObjectL(K_Tc5_NumberOfMetaDataEntriesWhenNotRecording, iUiLevelCameraHandle);
    }

void CCamcTest_5::NumberOfMetaDataEntriesL_002_L()
    {
    ///////////////////////////////////////
	//   ID: CAMC_API.NUMBEROFMETADATAENTRIESL_002
    //   Action :
    //       Call NumberOfMetaDataEntriesL() when recording
    //   Output : 
    //       Function should retrun 0.
    //   Precondition :
    //       Recording
    ///////////////////////////////////////

        iCamcTestClient->Start_Active_ObjectL(K_Tc5_NumberOfMetaDataEntriesWhenRecording, iUiLevelCameraHandle);
    }

/*******************************/
/*      META DATA ENTRY        */
/*******************************/

void CCamcTest_5::MetaDataEntryL_001_L()
    {
    ///////////////////////////////////////
    //   ID: CAMC_API.METADATAENTRYL_001
    //   Action :
    //        Call MetaDataEntryL() when not recording
    //   Output : 
    //        Leave with KErrNotSupported
    ///////////////////////////////////////    
    
    TRAPD(err, iCamcTestClient->Start_Active_ObjectL(K_Tc5_MetaDataEntryWhenNotRecording, iUiLevelCameraHandle));
    assertTIntsEqualL( KErrNotSupported, err );
    }

void CCamcTest_5::MetaDataEntryL_002_L()
    {
    ///////////////////////////////////////
    //   ID: CAMC_API.METADATAENTRYL_002
    //   Action :
    //        Call MetaDataEntryL() when recording
    //   Output : 
    //        Leave with KErrNotSupported
    ///////////////////////////////////////    
    
    TRAPD(err, iCamcTestClient->Start_Active_ObjectL(K_Tc5_MetaDataEntryWhenRecording, iUiLevelCameraHandle));
    assertTIntsEqualL( KErrNotSupported, err );
    }

/*******************************/
/*    ADD META DATA ENTRY      */
/*******************************/

void CCamcTest_5::AddMetaDataEntryL_001_L()
    {
    ///////////////////////////////////////
    //   ID: CAMC_API.ADDMETADATAENTRYL_001
    //   Action :
    //        Call AddMetaDataEntryL() when not recording
    //   Output : 
    //        Leave with KErrArgument
    ///////////////////////////////////////    
    
    TRAPD(err, iCamcTestClient->Start_Active_ObjectL(K_Tc5_AddMetaDataEntryWhenNotRecording, iUiLevelCameraHandle));
    assertTIntsEqualL( KErrNotSupported, err );
    }

void CCamcTest_5::AddMetaDataEntryL_002_L()
    {
    ///////////////////////////////////////
    //   ID: CAMC_API.ADDMETADATAENTRYL_002
    //   Action :
    //        Call AddMetaDataEntryL() when recording
    //   Output : 
    //        Leave with KErrArgument
    ///////////////////////////////////////    
    
    TRAPD(err, iCamcTestClient->Start_Active_ObjectL(K_Tc5_AddMetaDataEntryWhenRecording, iUiLevelCameraHandle));
    assertTIntsEqualL( KErrNotSupported, err );
    }

/********************************/
/*    REMOVE META DATA ENTRY    */
/********************************/

void CCamcTest_5::RemoveMetaDataEntryL_001_L()
    {
    ///////////////////////////////////////
    //   ID: CAMC_API.REMOVEMETADATAENTRYL_001
    //   Action :
    //        Call RemoveMetaDataEntryL() when not recording
    //   Output : 
    //        Leave with KErrArgument
    ///////////////////////////////////////    
    
    TRAPD(err, iCamcTestClient->Start_Active_ObjectL(K_Tc5_RemoveMetaDataEntryWhenNotRecording, iUiLevelCameraHandle));
    assertTIntsEqualL( KErrArgument, err );
    }

void CCamcTest_5::RemoveMetaDataEntryL_002_L()
    {
    ///////////////////////////////////////
    //   ID: CAMC_API.REMOVEMETADATAENTRYL_002
    //   Action :
    //        Call RemoveMetaDataEntryL() when recording
    //   Output : 
    //        Leave with KErrArgument
    ///////////////////////////////////////    
    
    TRAPD(err, iCamcTestClient->Start_Active_ObjectL(K_Tc5_RemoveMetaDataEntryWhenRecording, iUiLevelCameraHandle));
    assertTIntsEqualL( KErrArgument, err );
    }

/*********************************/
/*    REPLACE META DATA ENTRY    */
/*********************************/

void CCamcTest_5::ReplaceMetaDataEntryL_001_L()
    {
    ///////////////////////////////////////
    //   ID: CAMC_API.REPLACEMETADATAENTRYL_001
    //   Action :
    //        Call ReplaceMetaDataEntryL() when not recording
    //   Output : 
    //        Leave with KErrArgument
    ///////////////////////////////////////    
    
    TRAPD(err, iCamcTestClient->Start_Active_ObjectL(K_Tc5_ReplaceMetaDataEntryWhenNotRecording, iUiLevelCameraHandle));
    assertTIntsEqualL( KErrArgument, err );
    }

void CCamcTest_5::ReplaceMetaDataEntryL_002_L()
    {
    ///////////////////////////////////////
    //   ID: CAMC_API.REPLACEMETADATAENTRYL_002
    //   Action :
    //        Call ReplaceMetaDataEntryL() when recording
    //   Output : 
    //        Leave with KErrArgument
    ///////////////////////////////////////    
    
    TRAPD(err, iCamcTestClient->Start_Active_ObjectL(K_Tc5_ReplaceMetaDataEntryWhenRecording, iUiLevelCameraHandle));
    assertTIntsEqualL( KErrArgument, err );
    }

/**********************************/
/*        SET PRIORITY            */
/**********************************/

void CCamcTest_5::SetPriorityL_005_L()
    {
    ///////////////////////////////////////
    //   ID: CAMC_API.SETPRIORITYL_005
    //   Action :
    //        Set wrong SetPriorityL() 
    //   Output : 
    //        Leave with KErrArgument
    ///////////////////////////////////////    
    
    TRAPD(err, iCamcTestClient->Start_Active_ObjectL(K_Tc5_SetPriorityWhenNotRecording, iUiLevelCameraHandle));
    assertTIntsEqualL( KErrArgument, err );
    }

/**********************************/
/*         GET PRIORITY           */
/**********************************/

void CCamcTest_5::GetPriorityL_001_L()
    {
    ///////////////////////////////////////
	//   ID: CAMC_API.GETPRIORITYL_001
    //   Action :
    //       Call GetPriorityL() when not set
    //   Output : 
    //       
    //   Precondition :
    //       Not set
    ///////////////////////////////////////

        iCamcTestClient->Start_Active_ObjectL(K_Tc5_GetPriorityWhenNotSet, iUiLevelCameraHandle);
    }

/**********************************/
/*         SET VIDEO FRAME RATE   */
/**********************************/

void CCamcTest_5::SetVideoFrameRateL_005_L()
    {
    ///////////////////////////////////////
    //   ID: CAMC_API.SETVIDEOFRAMERATEL_005
    //   Action :
    //        Set wrong SetVideoFrameRateL()
    //   Output : 
    //        Leave with KErrArgument
    ///////////////////////////////////////    
    
    TRAPD(err, iCamcTestClient->Start_Active_ObjectL(K_Tc5_SetVideoFrameRateWhenOpenNotPrepared, iUiLevelCameraHandle));
    assertTIntsEqualL( KErrArgument, err );
    }

/**********************************/
/*         VIDEO FRAME RATE       */
/**********************************/

void CCamcTest_5::VideoFrameRateL_001_L()
    {
    ///////////////////////////////////////
	//   ID: CAMC_API.VIDEOFRAMERATEL_001
    //   Action :
    //       Call VideoFrameRateL() when not set
    //   Output : 
    //       Function should retrun 0.
    //   Precondition :
    //       
    ///////////////////////////////////////

        iCamcTestClient->Start_Active_ObjectL(K_Tc5_VideoFrameRateWhenNotSet, iUiLevelCameraHandle);
    }

/**********************************/
/*        SET VIDEO BIT RATE      */
/**********************************/

void CCamcTest_5::SetVideoBitRateL_005_L()
    {
    ///////////////////////////////////////
    //   ID: CAMC_API.SETVIDEOBITRATEL_005
    //   Action :
    //        Set wrong SetVideoBitRateL()
    //   Output : 
    //        Leave with KErrArgument
    ///////////////////////////////////////    
    
    TRAPD(err, iCamcTestClient->Start_Active_ObjectL(K_Tc5_SetVideoBitRateWhenOpenNotPrepared, iUiLevelCameraHandle));
    assertTIntsEqualL( KErrArgument, err );
    }

/**********************************/
/*         VIDEO BIT RATE         */
/**********************************/

void CCamcTest_5::VideoBitRateL_001_L()
    {
    ///////////////////////////////////////
	//   ID: CAMC_API.VIDEOBITRATEL_001
    //   Action :
    //       Call VideoBitRateL() when not set
    //   Output : 
    //       Function should retrun 0.
    //   Precondition :
    //       
    ///////////////////////////////////////


        iCamcTestClient->Start_Active_ObjectL(K_Tc5_VideoBitRateWhenNotSet, iUiLevelCameraHandle);

    }

/**********************************/
/*        SET AUDIO BIT RATE      */
/**********************************/

void CCamcTest_5::SetAudioBitRateL_005_L()
    {
    ///////////////////////////////////////
    //   ID: CAMC_API.SETAUDIOBITRATEL_005
    //   Action :
    //        Set wrong SetAudioBitRateL()
    //   Output : 
    //        Leave with KErrArgument
    ///////////////////////////////////////    
    
    TRAPD(err, iCamcTestClient->Start_Active_ObjectL(K_Tc5_SetAudioBitRateWhenOpenNotPrepared, iUiLevelCameraHandle));
    assertTIntsEqualL( KErrArgument, err );
    }

/**********************************/
/*        AUDIO BIT RATE          */
/**********************************/

void CCamcTest_5::AudioBitRateL_001_L()
    {
    ///////////////////////////////////////
	//   ID: CAMC_API.AUDIOBITRATEL_001
    //   Action :
    //       Call AudioBitRateL() when not set
    //   Output : 
    //       Function should retrun 0.
    //   Precondition :
    //       
    ///////////////////////////////////////

        iCamcTestClient->Start_Active_ObjectL(K_Tc5_AudioBitRateWhenNotSet, iUiLevelCameraHandle);
    }

/**********************************/
/*        SET AUDIO ENABLED       */
/**********************************/

void CCamcTest_5::SetAudioEnabledL_005_L()
    {
    ///////////////////////////////////////
    //   ID: CAMC_API.SETAUDIOENABLEDL_005
    //   Action :
    //        SetAudioEnabledL when controller is open, but not prepared.
    //   Output : 
    //        OK
    //////////////////////////////////////   
    
    iCamcTestClient->Start_Active_ObjectL(K_Tc5_SetAudioEnabledWhenOpenNotPrepared, iUiLevelCameraHandle);
    }

/**********************************/
/*        AUDIO ENABLED           */
/**********************************/

void CCamcTest_5::AudioEnabledL_001_L()
    {
    ///////////////////////////////////////
    //   ID: CAMC_API.AUDIOENABLEDL_001
    //   Action :
    //        AudioEnabledL when not set
    //   Output : 
    //        
    //////////////////////////////////////   
    
    iCamcTestClient->Start_Active_ObjectL(K_Tc5_AudioEnabledWhenNotSet, iUiLevelCameraHandle);
    }

/**********************************/
/*        SET VIDEO FRAME SIZE    */
/**********************************/

void CCamcTest_5::SetVideoFrameSizeL_006_L()
    {
    ///////////////////////////////////////
    //   ID: CAMC_API.SETVIDEOFRAMESIZEL_006
    //   Action :
    //        Set wrong SetVideoFrameSizeL()
    //   Output : 
    //        Leave with KErrArgument
    ///////////////////////////////////////    
    
    TRAPD(err, iCamcTestClient->Start_Active_ObjectL(K_Tc5_SetVideoFrameSizeWhenOpenNotPrepared, iUiLevelCameraHandle));
    assertTIntsEqualL( KErrArgument, err );
    }

/**********************************/
/*        GET VIDEO FRAME SIZE    */
/**********************************/

void CCamcTest_5::GetVideoFrameSizeL_001_L()
    {
    ///////////////////////////////////////
	//   ID: CAMC_API.GETVIDEOFRAMESIZEL_001
    //   Action :
    //       Call GetVideoFrameSizeL() when not set
    //   Output : 
    //       Function should retrun 0.
    //   Precondition :
    //       
    ///////////////////////////////////////

        iCamcTestClient->Start_Active_ObjectL(K_Tc5_GetVideoFrameSizeWhenNotSet, iUiLevelCameraHandle);
    }

/**********************************/
/*            PAUSE               */
/**********************************/

void CCamcTest_5::PauseL_001_L()
    {
    ///////////////////////////////////////
    //   ID: CAMC_API.PAUSEL_001
    //   Action :
    //        Pauses recording per request when recording
    //   Output : 
    //        Leave?
    ///////////////////////////////////////   
    
    iCamcTestClient->Start_Active_ObjectL(K_Tc5_PauseWhenRecording, iUiLevelCameraHandle);
    }

void CCamcTest_5::PauseL_002_L()
    {
    ///////////////////////////////////////
    //   ID: CAMC_API.PAUSEL_001
    //   Action :
    //        Ignores request to pause when not recording
    //   Output : 
    //        Leave?
    ///////////////////////////////////////   
    
    iCamcTestClient->Start_Active_ObjectL(K_Tc5_PauseWhenStopped, iUiLevelCameraHandle);
    }

/**********************************/
/*        SET MAX CLIP SIZE    */
/**********************************/

void CCamcTest_5::SetMaxClipSizeL_005_L()
    {
    ///////////////////////////////////////
    //   ID: CAMC_API.SETMAXCLIPSIZEL_005
    //   Action :
    //        Set invalid size for SetMaxClipSizeL when not recording
    //   Output : 
    //        Leave with KErrArgument
    //////////////////////////////////////    
    
    TRAPD(err, iCamcTestClient->Start_Active_ObjectL(K_Tc5_SetMaxClipSizeLWhenNotRecording, iUiLevelCameraHandle));
    assertTIntsEqualL( KErrArgument, err );
    }

/**********************************/
/*          SET GAIN              */
/**********************************/

void CCamcTest_5::SetGainL_001_L()
    {
    ///////////////////////////////////////
    //   ID: CAMC_API.SETGAINL_001
    //   Action :
    //        Set acceptable & ask, SetGainL when not recording.
    //   Output : 
    //        compare
    //////////////////////////////////////   
    
    iCamcTestClient->Start_Active_ObjectL(K_Tc5_SetGainWhenNotRecording, iUiLevelCameraHandle);
    }

void CCamcTest_5::SetGainL_002_L()
    {
    ///////////////////////////////////////
    //   ID: CAMC_API.SETGAINL_002
    //   Action :
    //        Set wrong SetGainL()
    //   Output : 
    //        The value is corrected inside the Video Recorder, 
    //        the user is not warned of his error.
    ///////////////////////////////////////    
    
    TRAPD(err, iCamcTestClient->Start_Active_ObjectL(K_Tc5_SetGainWhenNotRecordingAndSetWrong, iUiLevelCameraHandle));
    assertTIntsEqualL( KErrNone, err );
    }

void CCamcTest_5::SetGainL_003_L()
    {
    ///////////////////////////////////////
    //   ID: CAMC_API.SETGAINL_003
    //   Action :
    //        Set acceptable & ask, SetGainL when recording.
    //   Output : 
    //        compare
    //////////////////////////////////////   
    
    iCamcTestClient->Start_Active_ObjectL(K_Tc5_SetGainWhenRecording, iUiLevelCameraHandle);
    }

/**********************************/
/*              GAIN              */
/**********************************/

void CCamcTest_5::GainL_001_L()
    {
    ///////////////////////////////////////
	//   ID: CAMC_API.GAINL_001
    //   Action :
    //       Call GainL() when not set
    //   Output : 
    //       Function should retrun 0.
    //   Precondition :
    //       
    ///////////////////////////////////////

        iCamcTestClient->Start_Active_ObjectL(K_Tc5_GainWhenNotSet, iUiLevelCameraHandle);
    }

/**********************************/
/*           MAX GAIN             */
/**********************************/

void CCamcTest_5::MaxGainL_001_L()
    {
    ///////////////////////////////////////
	//   ID: CAMC_API.MAXGAINL_001
    //   Action :
    //       Call MaxGainL() when not recording
    //   Output : 
    //       Function should retrun 0???
    //   Precondition :
    //       
    ///////////////////////////////////////

        iCamcTestClient->Start_Active_ObjectL(K_Tc5_MaxGainWhenNotRecording, iUiLevelCameraHandle);
    }

void CCamcTest_5::MaxGainL_002_L()
    {
    ///////////////////////////////////////
	//   ID: CAMC_API.MAXGAINL_002
    //   Action :
    //       Call MaxGainL() when recording
    //   Output : 
    //       Function should retrun 0???
    //   Precondition :
    //       
    ///////////////////////////////////////

        iCamcTestClient->Start_Active_ObjectL(K_Tc5_MaxGainWhenRecording, iUiLevelCameraHandle);
    }

/**********************************/
/*             STOP               */
/**********************************/

void CCamcTest_5::Stop_001_L()
    {
    ///////////////////////////////////////
    //   ID: CAMC_API.STOP_001
    //   Action :
    //        Call stop when recording
    //   Output : 
    //        Video Recording is stopped.
    ///////////////////////////////////////   
    
    iCamcTestClient->Start_Active_ObjectL(K_Tc5_StopWhenRecording, iUiLevelCameraHandle);
    }

void CCamcTest_5::Stop_002_L()
    {
    ///////////////////////////////////////
    //   ID: CAMC_API.STOP_002
    //   Action :
    //        Call Stop when not recording or paused
    //   Output : 
    //        Nothing
    ///////////////////////////////////////   

    iCamcTestClient->Start_Active_ObjectL(K_Tc5_StopWhenNotRecording, iUiLevelCameraHandle);
    }

void CCamcTest_5::Stop_003_L()
    {
    ///////////////////////////////////////
    //   ID: CAMC_API.STOP_003
    //   Action :
    //        Stop + start timer + close + prepare
    //   Output : 
    //        New prepare complete, Prepare ready in 2 seconds
    ///////////////////////////////////////   

    iCamcTestClient->Start_Active_ObjectL(K_Tc5_StopWhenRecordingNewPrepareCorrect, iUiLevelCameraHandle);
    }

void CCamcTest_5::Stop_004_L()
    {
    ///////////////////////////////////////
    //   ID: CAMC_API.STOP_004
    //   Action :
    //        Stop + close + open file
    //   Output : 
    //        File can be opened for reading
    ///////////////////////////////////////   

    iCamcTestClient->Start_Active_ObjectL(K_Tc5_StopWhenRecordingAfterOpenFile, iUiLevelCameraHandle);

/* File can be opened for reading */
    RFile file; 
    RFs fsSession;
    fsSession.Connect();
    TFileName fileName;
    AddDriveLetterToPath(_L("recordQCIF.3gp"),fileName);
    TInt err = file.Open(fsSession,fileName,EFileShareAny);    
    file.Close();
    fsSession.Close();
    assertTIntsEqualL( KErrNone, err );
    }

void CCamcTest_5::Stop_005_L()
    {
    ///////////////////////////////////////
    //   ID: CAMC_API.STOP_005
    //   Action :
    //        Stop + close + open file
    //   Output : 
    //        File can be opened for reading
    ///////////////////////////////////////   

    iCamcTestClient->Start_Active_ObjectL(K_Tc5_StopWhenRecordingAfterOpenFileNew, iUiLevelCameraHandle);

/* File can be opened for reading */
    RFile file; 
    RFs fsSession;
    fsSession.Connect();
    TFileName fileName;
    AddDriveLetterToPath(_L("recordQCIF.3g2"),fileName);
    TInt err = file.Open(fsSession,fileName,EFileShareAny);    
    file.Close();
    fsSession.Close();
    assertTIntsEqualL( KErrNone, err );
    }

void CCamcTest_5::Stop_007_L()
    {
    ///////////////////////////////////////
    //   ID: CAMC_API.STOP_007
    //   Action :
    //        Stop + close + open file
    //   Output : 
    //        File can be opened for reading
    ///////////////////////////////////////   

    iCamcTestClient->Start_Active_ObjectL(K_Tc5_StopWhenRecordingAfterOpenFileNew_MPEG4, iUiLevelCameraHandle);

/* File can be opened for reading */
    RFile file; 
    RFs fsSession;
    fsSession.Connect();
    TFileName fileName;
    AddDriveLetterToPath(_L("recordQCIF.mp4"),fileName);
    TInt err = file.Open(fsSession,fileName,EFileShareAny);    
    file.Close();
    fsSession.Close();
    assertTIntsEqualL( KErrNone, err );
    }

void CCamcTest_5::GetSupportedVideoTypes_001_L()
    {
    ///////////////////////////////////////
    //   ID: CAMC_API.GETSUPPORTEDVIDEOTYPESL_001
    //   Action :
    //        GetSupportedVideoTypesL when open, not recording
    //   Output : 
    //        must be sensible
    ///////////////////////////////////////   
    
    iCamcTestClient->Start_Active_ObjectL(K_Tc5_GetSupportedVideoTypesWhenOpenNotRecording, iUiLevelCameraHandle);
    }

void CCamcTest_5::GetSupportedVideoTypes_002_L()
    {
    ///////////////////////////////////////
    //   ID: CAMC_API.GETSUPPORTEDVIDEOTYPESL_002
    //   Action :
    //        GetSupportedVideoTypesL when open, not recording
    //   Output : 
    //        must be sensible
    ///////////////////////////////////////   
    
    iCamcTestClient->Start_Active_ObjectL(K_Tc5_GetSupportedVideoTypesWhenOpenNotRecordingNew, iUiLevelCameraHandle);
    }

void CCamcTest_5::GetSupportedAudioTypes_001_L()
    {
    ///////////////////////////////////////
    //   ID: CAMC_API.GETSUPPORTEDAUDIOTYPESL_001
    //   Action :
    //        GetSupportedAudioTypesL when open, not recording
    //   Output : 
    //        must be sensible
    ///////////////////////////////////////   
    
    iCamcTestClient->Start_Active_ObjectL(K_Tc5_GetSupportedAudioTypesWhenOpenNotRecordingAMR, iUiLevelCameraHandle);
    }

void CCamcTest_5::GetSupportedAudioTypes_003_L()
    {
    ///////////////////////////////////////
    //   ID: CAMC_API.GETSUPPORTEDAUDIOTYPESL_003
    //   Action :
    //        GetSupportedAudioTypesL when open, not recording
    //   Output : 
    //        must be sensible
    ///////////////////////////////////////   
    
    iCamcTestClient->Start_Active_ObjectL(K_Tc5_GetSupportedAudioTypesWhenOpenNotRecordingAAC, iUiLevelCameraHandle);
    }
    
    

// //////////////////////////////////////////////////////////////////
//   Dummy Camera API callback functions
// //////////////////////////////////////////////////////////////////
void CCamcTest_5::ReserveComplete(TInt /*aError*/)
    {
    RDebug::Print(_L("CCamcTest_5::ReserveComplete"));
        
    // UI level camera is being initialized        
    if (iWaitScheduler->IsStarted())
	    {
    	 iUiLevelCamera->PowerOn();
    	 return;
	    }
    }

void CCamcTest_5::PowerOnComplete(TInt /*aError*/)
    {   
    RDebug::Print(_L("CCamcTest_5::PowerOnComplete"));
        
    // UI level camera is being initialized
    if (iWaitScheduler->IsStarted())
	    {
    	 iWaitScheduler->AsyncStop();
    	 return;
    	}        
    }

void CCamcTest_5::ViewFinderFrameReady(CFbsBitmap& /*aFrame*/)
    {
    }
void CCamcTest_5::ImageReady(CFbsBitmap* /*aBitmap */,HBufC8* /*aData*/,TInt /*aError*/)
    {    
    }
void CCamcTest_5::FrameBufferReady(MFrameBuffer* /*aFrameBuffer*/,TInt /*aError*/ )
    {
    }
  
// -----------------------------------------------------------------------------
// CCamcTest_5::HandleEvent
// MCameraObserver2 call-back handler
// -----------------------------------------------------------------------------
//
void CCamcTest_5::HandleEvent( const TECAMEvent& aEvent)
    {
    PRINT(( _L( "CCamcTest_5::HandleEvent() entering, type=%x, err=%d" ), aEvent.iEventType.iUid, aEvent.iErrorCode ));

    if ( aEvent.iEventType == KUidECamEventCameraNoLongerReserved )
        {
        PRINT(( _L( "CCamcTest_5::HandleEvent() KUidECamEventCameraNoLongerReserved" ) ));
        }
    else if ( aEvent.iEventType == KUidECamEventPowerOnComplete )
        {
        PRINT(( _L( "CCamcTest_5::HandleEvent() KUidECamEventPowerOnComplete" ) ));
        PowerOnComplete( aEvent.iErrorCode );
        }
    else if ( aEvent.iEventType == KUidECamEventReserveComplete )
       {
       PRINT(( _L( "CCamcTest_5::HandleEvent() KUidECamEventReserveComplete" ) ));
       ReserveComplete( aEvent.iErrorCode );
       }

    PRINT(( _L( "CCamcTestClient_7::HandleEvent() returning" ) ));
    }

// -----------------------------------------------------------------------------
// CCamcTest_5::ViewFinderReady
// MCameraObserver2 call-back handler
// -----------------------------------------------------------------------------
//
void CCamcTest_5::ViewFinderReady( MCameraBuffer& aCameraBuffer, TInt aError)
    {
    PRINT(( _L( "CCamcTest_5::ViewFinderReady() NOT HANDLED err=%d, NumFrames=%d" ), aError, aCameraBuffer.NumFrames() ));
    CFbsBitmap *buffer = NULL;
    if ( !aError )
        {
	    aCameraBuffer.Release();
        }
    ViewFinderFrameReady( *buffer );
    }

// -----------------------------------------------------------------------------
// CCamcTest_5::ImageBufferReady
// MCameraObserver2 call-back handler
// -----------------------------------------------------------------------------
//
void CCamcTest_5::ImageBufferReady( MCameraBuffer& aCameraBuffer, TInt aError)
    {
    PRINT(( _L( "CCamcTest_5::ImageBufferReady() BUFFER NOT HANDLED, err=%d, NumFrames=%d" ), aError, aCameraBuffer.NumFrames() ));
	
    if ( !aError )
        {
	    aCameraBuffer.Release();
        }
    ImageReady( NULL, NULL, aError );
    }
    
// -----------------------------------------------------------------------------
// CCamcTest_5::VideoBufferReady
// MCameraObserver2 call-back handler
// -----------------------------------------------------------------------------
//
void CCamcTest_5::VideoBufferReady( MCameraBuffer& aCameraBuffer, TInt aError)
    {
    PRINT(( _L( "CCamcTest_5::VideoBufferReady() entering, err=%d, NumFrames=%d" ), aError, aCameraBuffer.NumFrames() ));
    MFrameBuffer* buffer = NULL;
	
    if ( !aError )
        {
        aCameraBuffer.Release();
        }
    // Call old Camera-API observer handler
    FrameBufferReady( buffer, aError );
    
    PRINT(( _L( "CCamcTest_5::VideoBufferReady() returning" )));
    }




//
// An own static function to collect the test functions into one 
// suite of tests. The framework will run the tests and free the
// memory allocated for the test suite.
// 

MTest* CCamcTest_5::suiteL ()
    {
    // Always use NewL (Do not use NewLC) !!!
    CTestSuite *suite = CTestSuite::NewL(_L8("CCamcTest_5"));
    CleanupStack::PushL( suite );

    suite->addTestL(CTestCaller<CCamcTest_5>::NewL(_L8("CAMC_API.NUMBEROFMETADATAENTRIESL_001"), &NumberOfMetaDataEntriesL_001_L));
    suite->addTestL(CTestCaller<CCamcTest_5>::NewL(_L8("CAMC_API.NUMBEROFMETADATAENTRIESL_002"), &NumberOfMetaDataEntriesL_002_L));
 
    suite->addTestL(CTestCaller<CCamcTest_5>::NewL(_L8("CAMC_API.METADATAENTRYL_001"), &MetaDataEntryL_001_L));
    suite->addTestL(CTestCaller<CCamcTest_5>::NewL(_L8("CAMC_API.METADATAENTRYL_002"), &MetaDataEntryL_002_L));

    suite->addTestL(CTestCaller<CCamcTest_5>::NewL(_L8("CAMC_API.ADDMETADATAENTRYL_001"), &AddMetaDataEntryL_001_L));
    suite->addTestL(CTestCaller<CCamcTest_5>::NewL(_L8("CAMC_API.ADDMETADATAENTRYL_002"), &AddMetaDataEntryL_002_L));

    suite->addTestL(CTestCaller<CCamcTest_5>::NewL(_L8("CAMC_API.REMOVEMETADATAENTRYL_001"), &RemoveMetaDataEntryL_001_L));   
    suite->addTestL(CTestCaller<CCamcTest_5>::NewL(_L8("CAMC_API.REMOVEMETADATAENTRYL_002"), &RemoveMetaDataEntryL_002_L));
    
    suite->addTestL(CTestCaller<CCamcTest_5>::NewL(_L8("CAMC_API.REPLACEMETADATAENTRYL_001"), &ReplaceMetaDataEntryL_001_L));
    suite->addTestL(CTestCaller<CCamcTest_5>::NewL(_L8("CAMC_API.REPLACEMETADATAENTRYL_002"), &ReplaceMetaDataEntryL_002_L));

    suite->addTestL(CTestCaller<CCamcTest_5>::NewL(_L8("CAMC_API.SETPRIORITYL_005"), &SetPriorityL_005_L));     
    suite->addTestL(CTestCaller<CCamcTest_5>::NewL(_L8("CAMC_API.GETPRIORITYL_001"), &GetPriorityL_001_L));

    suite->addTestL(CTestCaller<CCamcTest_5>::NewL(_L8("CAMC_API.SETVIDEOFRAMERATEL_005"), &SetVideoFrameRateL_005_L));
    suite->addTestL(CTestCaller<CCamcTest_5>::NewL(_L8("CAMC_API.VIDEOFRAMERATEL_001"), &VideoFrameRateL_001_L));

    suite->addTestL(CTestCaller<CCamcTest_5>::NewL(_L8("CAMC_API.SETVIDEOBITRATEL_005"), &SetVideoBitRateL_005_L));
    suite->addTestL(CTestCaller<CCamcTest_5>::NewL(_L8("CAMC_API.VIDEOBITRATEL_001"), &VideoBitRateL_001_L));

    suite->addTestL(CTestCaller<CCamcTest_5>::NewL(_L8("CAMC_API.SETAUDIOBITRATEL_005"), &SetAudioBitRateL_005_L));
    suite->addTestL(CTestCaller<CCamcTest_5>::NewL(_L8("CAMC_API.AUDIOBITRATEL_001"), &AudioBitRateL_001_L));
 
    suite->addTestL(CTestCaller<CCamcTest_5>::NewL(_L8("CAMC_API.SETAUDIOENABLEDL_005"), &SetAudioEnabledL_005_L));
    suite->addTestL(CTestCaller<CCamcTest_5>::NewL(_L8("CAMC_API.AUDIOENABLEDL_001"), &AudioEnabledL_001_L));

    suite->addTestL(CTestCaller<CCamcTest_5>::NewL(_L8("CAMC_API.SETVIDEOFRAMESIZEL_006"), &SetVideoFrameSizeL_006_L));
    suite->addTestL(CTestCaller<CCamcTest_5>::NewL(_L8("CAMC_API.GETVIDEOFRAMESIZEL_001"), &GetVideoFrameSizeL_001_L));

    suite->addTestL(CTestCaller<CCamcTest_5>::NewL(_L8("CAMC_API.PAUSEL_001"), &PauseL_001_L));
    suite->addTestL(CTestCaller<CCamcTest_5>::NewL(_L8("CAMC_API.PAUSEL_002"), &PauseL_002_L));

    suite->addTestL(CTestCaller<CCamcTest_5>::NewL(_L8("CAMC_API.SETMAXCLIPSIZEL_005"), &SetMaxClipSizeL_005_L));
     
    suite->addTestL(CTestCaller<CCamcTest_5>::NewL(_L8("CAMC_API.SETGAINL_001"), &SetGainL_001_L));
    suite->addTestL(CTestCaller<CCamcTest_5>::NewL(_L8("CAMC_API.SETGAINL_002"), &SetGainL_002_L));
    suite->addTestL(CTestCaller<CCamcTest_5>::NewL(_L8("CAMC_API.SETGAINL_003"), &SetGainL_003_L));
    
    suite->addTestL(CTestCaller<CCamcTest_5>::NewL(_L8("CAMC_API.GAINL_001"), &GainL_001_L));
   
    suite->addTestL(CTestCaller<CCamcTest_5>::NewL(_L8("CAMC_API.MAXGAINL_001"), &MaxGainL_001_L));
    suite->addTestL(CTestCaller<CCamcTest_5>::NewL(_L8("CAMC_API.MAXGAINL_002"), &MaxGainL_002_L));
 
    suite->addTestL(CTestCaller<CCamcTest_5>::NewL(_L8("CAMC_API.STOP_001"), &Stop_001_L));
    suite->addTestL(CTestCaller<CCamcTest_5>::NewL(_L8("CAMC_API.STOP_002"), &Stop_002_L));
    suite->addTestL(CTestCaller<CCamcTest_5>::NewL(_L8("CAMC_API.STOP_003"), &Stop_003_L));
    suite->addTestL(CTestCaller<CCamcTest_5>::NewL(_L8("CAMC_API.STOP_004"), &Stop_004_L));
    
#ifdef __MPEG4_VIDEO_ENCODING
    suite->addTestL(CTestCaller<CCamcTest_5>::NewL(_L8("CAMC_API.STOP_005"), &Stop_005_L));
#endif
#ifndef __MPEG4_VIDEO_ENCODING
    suite->addTestL(CTestCaller<CCamcTest_5>::NewL(_L8("CAMC_API.GETSUPPORTEDVIDEOTYPESL_001"), &GetSupportedVideoTypes_001_L));
#else
    suite->addTestL(CTestCaller<CCamcTest_5>::NewL(_L8("CAMC_API.GETSUPPORTEDVIDEOTYPESL_002"), &GetSupportedVideoTypes_002_L));
#endif

#if ((!defined __WINS__) || (!defined __WINSCW__)) // AAC supported only in HW
    // MP4 AAC + AMR
    suite->addTestL(CTestCaller<CCamcTest_5>::NewL(_L8("CAMC_API.GETSUPPORTEDAUDIOTYPESL_003"), &GetSupportedAudioTypes_003_L));
    suite->addTestL(CTestCaller<CCamcTest_5>::NewL(_L8("CAMC_API.STOP_007"), &Stop_007_L));
#endif
    // AMR-only
    suite->addTestL(CTestCaller<CCamcTest_5>::NewL(_L8("CAMC_API.GETSUPPORTEDAUDIOTYPESL_001"), &GetSupportedAudioTypes_001_L));

    CleanupStack::Pop( suite );
    return suite;
    }


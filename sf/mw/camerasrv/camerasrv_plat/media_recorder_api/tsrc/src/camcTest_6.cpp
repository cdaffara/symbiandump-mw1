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
#include "camcTest_6.h"
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

CCamcTest_6::CCamcTest_6 ()
    {
    }



CCamcTest_6::~CCamcTest_6 ()
    {
    delete iCamcTestClient;
    iCamcTestClient = NULL;
    }


void CCamcTest_6::setUpL ()
    {
    iCamcTestClient = CCamcTestClient_6::NewL( );
    
    iWaitScheduler = new (ELeave) CActiveSchedulerWait;	

    // Create a new Camera API implementation object, if supported
    TRAPD( err, iUiLevelCamera = CCamera::New2L( static_cast<MCameraObserver2&>(*this), 0 /*index*/, 0 /*Priority*/ ) );
    if ( err )
        {
        PRINT( ( _L( "CCamcTest_6::setUpL CCamera::New2L return code=%d" ), err ) ); 
        // Create old Camera API implementation object.
        iUiLevelCamera = CCamera::NewL( static_cast<MCameraObserver&>(*this), 0 /*Index*/ );
        }
    else 
       {
       PRINT( ( _L( "CCamcTest_6::setUp() using new MCameraObserver2" )) ); 
       }

	iUiLevelCameraHandle = iUiLevelCamera->Handle();
	iUiLevelCamera->Reserve();
	iWaitScheduler->Start();    
    }


void CCamcTest_6::tearDown ()
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

/***************************/
/*           PAUSE         */
/***************************/

void CCamcTest_6::PauseL_003_L()
    {
    ///////////////////////////////////////
    //   ID: CAMC_API.PAUSEL_003
    //   Action :
    //        Call pause when stopped
    //   Output : 
    //        Leave?
    ///////////////////////////////////////   
    
    iCamcTestClient->Start_Active_ObjectL(K_Tc6_PauseWhenStopped, iUiLevelCameraHandle);
    }

/**************************/
/*  SET VIDEO FRAME SIZE  */
/**************************/

void CCamcTest_6::SetVideoFrameSizeL_005_L()
    {
    ///////////////////////////////////////
    //   ID: CAMC_API.SETVIDEOFRAMESIZEL_005
    //   Action :
    //        SetVideoFrameSizeL when paused
    //		
    //   Output : 
    //        Leaves with KErrNotReady
    ///////////////////////////////////////    
    
    TRAPD(err, iCamcTestClient->Start_Active_ObjectL(K_Tc6_SetVideoFrameSizeLWhenPaused, iUiLevelCameraHandle));
    assertTIntsEqualL( KErrNotReady, err );
    }

/**************************/
/*    SET AUDIO ENABLED   */
/**************************/

void CCamcTest_6::SetAudioEnabledL_005_L()
    {
    ///////////////////////////////////////
    //   ID: CAMC_API.SETAUDIOENABLEDL_005
    //   Action :
    //        SetAudioEnabledL when controller is open, but not prepared.
    //   Output : 
    //        OK
    //////////////////////////////////////   
    
    iCamcTestClient->Start_Active_ObjectL(K_Tc6_SetAudioEnabledLWhenControllerReady, iUiLevelCameraHandle);
    }

/**************************/
/*   SET MAX CLIP SIZE    */
/**************************/ 

void CCamcTest_6::SetMaxClipSizeL_006_L()
    {
    ///////////////////////////////////////
    //   ID: CAMC_API.SETMAXCLIPSIZEL_006
    //   Action :
    //        Set invalid size for SetMaxClipSizeL when controller open
    //   Output : 
    //        Leave with KErrNotReady??
    //////////////////////////////////////    
    
    TRAPD(err, iCamcTestClient->Start_Active_ObjectL(K_Tc6_SetMaxClipSizeLWhenControllerReady, iUiLevelCameraHandle));
    assertTIntsEqualL( KErrArgument, err );
    }

// Allows a 10% margin for the filesize
#define TC6_ALLOWED_MARGIN 0.1 

void CCamcTest_6::SetMaxClipSizeL_007_L()
    {
    ///////////////////////////////////////
    //   ID: CAMC_API.SETMAXCLIPSIZEL_007
    //   Action :
    //        SetMaxClipSizeL when prepare is ready
    //   Output : 
    //        Check file size 
    ///////////////////////////////////////    
    

    iCamcTestClient->Start_Active_ObjectL(K_Tc6_SetMaxClipSizeLWhenPrepareReady, iUiLevelCameraHandle);
    
/* Check if size is according to previously set Size limit */
    RFile file; 
    TInt filesize;
    RFs fsSession;
    fsSession.Connect();
    TFileName fileName;
    AddDriveLetterToPath(_L("recordQCIF.3gp"),fileName);
    TInt err;
    if ( err = file.Open(fsSession,fileName,EFileShareAny) )
        {
        fsSession.Close();
        }
    assertTIntsEqualL( KErrNone, err );

    err = file.Size(filesize);
    file.Close();
    fsSession.Close();
    if ( ( filesize > TC6_FILE_SIZE_LIMIT * (1 + TC6_ALLOWED_MARGIN ) ) ||
        ( filesize < TC6_FILE_SIZE_LIMIT * (1 - TC6_ALLOWED_MARGIN ) ) )
        {
        assertL(0);
        }
    }

/**************************/
/*       RECORD           */
/**************************/ 

void CCamcTest_6::Record_006_L()
    {
    ///////////////////////////////////////
    //   ID: CAMC_API.RECORD_006
    //   Action :
    //        Record when PrepareComplete, no size limit
    //   Output : 
    //        OK
    ///////////////////////////////////////    
    
    iCamcTestClient->Start_Active_ObjectL(K_Tc6_RecordWhenPrepareComplete, iUiLevelCameraHandle);
    }

void CCamcTest_6::Record_007_L()
    {
    ///////////////////////////////////////
    //   ID: CAMC_API.RECORD_007
    //   Action :
    //        Record when paused
    //   Output : 
    //        OK, No error in callback ???
    ///////////////////////////////////////    
    
    iCamcTestClient->Start_Active_ObjectL(K_Tc6_RecordWhenPaused, iUiLevelCameraHandle);
    }

/**************************/
/*       VIDEO TYPE       */
/**************************/ 

void CCamcTest_6::VideoTypeL_001_L()
    {
    ///////////////////////////////////////
    //   ID: CAMC_API.VIDEOTYPEL_001
    //   Action :
    //        Call VideoType when opened with specified type, not recording
    //   Output : 
    //        compare
    ///////////////////////////////////////    
    
    iCamcTestClient->Start_Active_ObjectL(K_Tc6_VideoTypeLWhenOpenWithMimeAndNotRecording, iUiLevelCameraHandle);
    }

void CCamcTest_6::VideoTypeL_002_L()
    {
    ///////////////////////////////////////
    //   ID: CAMC_API.VIDEOTYPEL_002
    //   Action :
    //        Call VideoType when opened Without Specifying VideoType, not recording
    //   Output : 
    //        must be sensible ??
    ///////////////////////////////////////    
    
    iCamcTestClient->Start_Active_ObjectL(K_Tc6_VideoTypeLWhenOpenWithoutSpecifyingVideoTypeAndNotRecording, iUiLevelCameraHandle);
    }

void CCamcTest_6::VideoTypeL_003_L()
    {
    ///////////////////////////////////////
    //   ID: CAMC_API.VIDEOTYPEL_003
    //   Action :
    //        Call VideoType when opened with specified type, recording
    //   Output : 
    //        compare
    ///////////////////////////////////////    
    
    iCamcTestClient->Start_Active_ObjectL(K_Tc6_VideoTypeLWhenOpenWithMimeAndRecording, iUiLevelCameraHandle);
    }

void CCamcTest_6::VideoTypeL_004_L()
    {
    ///////////////////////////////////////
    //   ID: CAMC_API.VIDEOTYPEL_004
    //   Action :
    //        Call VideoType when open with NULL, not recording. 
    //   Output : 
    //        compare (seted acceptable)
    ///////////////////////////////////////    
    
    iCamcTestClient->Start_Active_ObjectL(K_Tc6_VideoTypeLWhenOpenWithNULLAndNotRecording, iUiLevelCameraHandle);
    }

void CCamcTest_6::VideoTypeL_005_L()
    {
    ///////////////////////////////////////
    //   ID: CAMC_API.VIDEOTYPEL_005
    //   Action :
    //        Call VideoType 
    //   Output : 
    //        compare
    ///////////////////////////////////////    
    
    iCamcTestClient->Start_Active_ObjectL(K_Tc6_VideoTypeLWhenOpenWithMimeAndNotRecordingNewFormat, iUiLevelCameraHandle);
    }

/**************************/
/*       AUDIO TYPE       */
/**************************/ 

void CCamcTest_6::AudioTypeL_001_L()
    {
    ///////////////////////////////////////
    //   ID: CAMC_API.AUDIOTYPEL_001
    //   Action :
    //        Call AudioType when open with fourCC, not recording
    //   Output : 
    //        compare
    ///////////////////////////////////////    
    
    iCamcTestClient->Start_Active_ObjectL(K_Tc6_AudioTypeLWhenOpenWithfourCCAndNotRecording, iUiLevelCameraHandle);
    }

void CCamcTest_6::AudioTypeL_002_L()
    {
    ///////////////////////////////////////
    //   ID: CAMC_API.AUDIOTYPEL_002
    //   Action :
    //        Call AudioType when open without specifying the audio type, not recording
    //   Output : 
    //        must be sensible ??
    ///////////////////////////////////////    
    
    iCamcTestClient->Start_Active_ObjectL(K_Tc6_AudioTypeLWhenOpenWithoutSpecifyingAudioTypeAndNotRecording, iUiLevelCameraHandle);
    }

void CCamcTest_6::AudioTypeL_003_L()
    {
    ///////////////////////////////////////
    //   ID: CAMC_API.AUDIOTYPEL_003
    //   Action :
    //        Call AudioType when open with fourCC, recording
    //   Output : 
    //        compare
    ///////////////////////////////////////    
    
    iCamcTestClient->Start_Active_ObjectL(K_Tc6_AudioTypeLWhenOpenWithfourCCAndRecording, iUiLevelCameraHandle);
    }

/**************************/
/*     SET VIDEO TYPE     */
/**************************/ 

void CCamcTest_6::SetVideoTypeL_005_L()
    {
    ///////////////////////////////////////
    //   ID: CAMC_API.SETVIDEOTYPEL_005
    //   Action :
    //        Call SetVideoType when open without specifying the videoType. 
    //   Output : 
    //        compare (seted acceptable)
    ///////////////////////////////////////    
    
    iCamcTestClient->Start_Active_ObjectL(K_Tc6_SetVideoTypeLWhenOpenWithoutSpecifyingVideoTypeAndNotRecording, iUiLevelCameraHandle);
    }

void CCamcTest_6::SetVideoTypeL_006_L()
    {
    ///////////////////////////////////////
    //   ID: CAMC_API.SETVIDEOTYPEL_006
    //   Action : 
    //        SetVideoTypeL called with "video/mp4v-es; profile=8" 
    //   Output : 
    //        compare (seted acceptable)
    ///////////////////////////////////////    
    
    iCamcTestClient->Start_Active_ObjectL(K_Tc6_SetVideoTypeLWithProfile8, iUiLevelCameraHandle);
    }

void CCamcTest_6::SetVideoTypeL_007_L()
    {
    ///////////////////////////////////////
    //   ID: CAMC_API.SETVIDEOTYPEL_007
    //   Action :
    //        SetVideoType when not recording, set wrong 
    //   Output : 
    //        Leave with KErrArgument
    ///////////////////////////////////////    
    
    TRAPD(err, iCamcTestClient->Start_Active_ObjectL(K_Tc6_SetVideoTypeLWhenNotRecording, iUiLevelCameraHandle));
    assertTIntsEqualL( KErrNotSupported, err );
    }

void CCamcTest_6::SetVideoTypeL_008_L()
    {
    ///////////////////////////////////////
    //   ID: CAMC_API.SETVIDEOTYPEL_008
    //   Action :
    //        SetVideoTypeL called with "video/H263-2000; profile=0" 
    //   Output : 
    //        compare (seted acceptable)
    ///////////////////////////////////////    
    
    iCamcTestClient->Start_Active_ObjectL(K_Tc6_SetVideoTypeLWithProfile, iUiLevelCameraHandle);
    }

void CCamcTest_6::SetVideoTypeL_009_L()
    {
    ///////////////////////////////////////
    //   ID: CAMC_API.SETVIDEOTYPEL_009
    //   Action :
    //        SetVideoTypeL called with "video/mp4v-es; profile-level-id=3" 
    //   Output : 
    //        compare (seted acceptable)
    ///////////////////////////////////////    
    
    iCamcTestClient->Start_Active_ObjectL(K_Tc6_SetVideoTypeLWithProfile3, iUiLevelCameraHandle);
    }

/**************************/
/*     SET AUDIO TYPE     */
/**************************/ 

void CCamcTest_6::SetAudioTypeL_005_L()
    {
    ///////////////////////////////////////
    //   ID: CAMC_API.SETAUDIOTYPEL_005
    //   Action :
    //        Call SetAudioType when open Without Specifying Audio Type, not recording. 
    //   Output : 
    //        compare (seted acceptable)
    ///////////////////////////////////////    
    
    iCamcTestClient->Start_Active_ObjectL(K_Tc6_SetAudioTypeLWhenOpenWithoutSpecifyingAudioTypeAndNotRecording, iUiLevelCameraHandle);
    }

void CCamcTest_6::SetAudioTypeL_007_L()
    {
    ///////////////////////////////////////
    //   ID: CAMC_API.SETAUDIOTYPEL_007
    //   Action :
    //        SetAudioType when not recording, set wrong 
    //   Output : 
    //        Leave with KErrArgument
    ///////////////////////////////////////    
    
    TRAPD(err, iCamcTestClient->Start_Active_ObjectL(K_Tc6_SetAudioTypeLWhenNotRecording, iUiLevelCameraHandle));
    assertTIntsEqualL( KErrNotSupported, err );
    }

void CCamcTest_6::SetAudioTypeL_008_L()
    {
    ///////////////////////////////////////
    //   ID: CAMC_API.SETAUDIOTYPEL_008
    //   Action :
    //        Call SetAudioType when open with fourCC, not recording. 
    //   Output : 
    //        compare (seted different acceptable)
    ///////////////////////////////////////    
    
    iCamcTestClient->Start_Active_ObjectL(K_Tc6_SetAudioTypeLWhenOpenWithfourCCAndNotRecording_AAC, iUiLevelCameraHandle);
    }


/***************************************/
/*CONTROLLER IMPLEMENTATION INFORMATION*/
/***************************************/

void CCamcTest_6::ControllerImplementationInformationL_001_L()
    {
    ///////////////////////////////////////
    //   ID: CAMC_API.CONTROLLERIMPLEMENTATIONINFORMATIONL_001
    //   Action :
    //        ControllerImplementationInformationL when not open
    //   Output : 
    //        Leave with KErrArgument
    ///////////////////////////////////////   
    
    TRAPD(err, iCamcTestClient->Start_Active_ObjectL(K_Tc6_ControllerImplementationInformationWhenNotOpen, iUiLevelCameraHandle));
    assertTIntsEqualL( KErrNotReady, err );
    }

void CCamcTest_6::ControllerImplementationInformationL_002_L()
    {
    ///////////////////////////////////////
    //   ID: CAMC_API.CONTROLLERIMPLEMENTATIONINFORMATIONL_002
    //   Action :
    //        ControllerImplementationInformationL when open
    //   Output : 
    //        must be sensible
    ///////////////////////////////////////   
    
    iCamcTestClient->Start_Active_ObjectL(K_Tc6_ControllerImplementationInformationWhenOpen, iUiLevelCameraHandle);
    }

void CCamcTest_6::ControllerImplementationInformationL_003_L()
    {
    ///////////////////////////////////////
    //   ID: CAMC_API.CONTROLLERIMPLEMENTATIONINFORMATIONL_003
    //   Action :
    //        ControllerImplementationInformationL when open
    //   Output : 
    //        must be sensible
    ///////////////////////////////////////   
    
    iCamcTestClient->Start_Active_ObjectL(K_Tc6_ControllerImplementationInformationWhenOpenNew, iUiLevelCameraHandle);
    }

void CCamcTest_6::ControllerImplementationInformationL_004_L()
    {
    ///////////////////////////////////////
    //   ID: CAMC_API.CONTROLLERIMPLEMENTATIONINFORMATIONL_004
    //   Action :
    //        ControllerImplementationInformationL when open
    //   Output : 
    //        must be sensible
    ///////////////////////////////////////   
    
    iCamcTestClient->Start_Active_ObjectL(K_Tc6_ControllerImplementationInformationWhenOpenNew_MPEG4, iUiLevelCameraHandle);
    }



/**************************/
/*         DURATION       */
/**************************/ 

void CCamcTest_6::DurationL_001_L()
    {
    ///////////////////////////////////////
    //   ID: CAMC_API.DURATIONL_001
    //   Action :
    //        DurationL when recording
    //   Output : 
    //        Must be close to timeout*n time
    ///////////////////////////////////////   
    
    iCamcTestClient->Start_Active_ObjectL(K_Tc6_DurationWhenRecording, iUiLevelCameraHandle);
    }

void CCamcTest_6::DurationL_002_L()
    {
    ///////////////////////////////////////
    //   ID: CAMC_API.DURATIONL_002
    //   Action :
    //        DurationL when not recording
    //   Output : 
    //        Value must be 0
    ///////////////////////////////////////   
    
    iCamcTestClient->Start_Active_ObjectL(K_Tc6_DurationWhenNotRecording, iUiLevelCameraHandle);
    }

void CCamcTest_6::DurationL_003_L()
    {
    ///////////////////////////////////////
    //   ID: CAMC_API.DURATIONL_003
    //   Action :
    //        Recording paused then resumed 
    //   Output : 
    //        Pause, Resume and Ask Duration frequently using timer
    ///////////////////////////////////////   
    
    iCamcTestClient->Start_Active_ObjectL(K_Tc6_DurationWhenPaused, iUiLevelCameraHandle);
    }


/**************************/
/*     Registration       */
/**************************/ 


void CCamcTest_6::MMFRegistration_001_L()
    {
    ///////////////////////////////////////
    //   ID: CAMC_API.MMFREGISTRATION_001
    //   Action :
    //        Check that camcorder registration was sucessfull.
    //        Ask for Controller UID, Version, Supplier, Supported media UID
    //        and Formats.
    //   Output : 
    //        must be sensible
    ///////////////////////////////////////   
    
    /////////////////////////////////////////////////////
    // Retrieve a list of possible controllers from ECOM
    CMMFControllerPluginSelectionParameters * cSelect = CMMFControllerPluginSelectionParameters::NewLC();
    CMMFFormatSelectionParameters * fSelect = CMMFFormatSelectionParameters::NewLC();

    fSelect->SetMatchToMimeTypeL(_L8("video/3gpp"));
    cSelect->SetRequiredRecordFormatSupportL(*fSelect);
    
    RMMFControllerImplInfoArray controllers;
    CleanupResetAndDestroyPushL(controllers);
    cSelect->ListImplementationsL(controllers);
    
    // Only one Controller is expected to be found
    // This might change in the future.
    if (controllers.Count() !=1 )
        {
        User::Leave(K_Tc6_ErrExpectingADifferentValue);
        }
    iCamcTestClient->CheckControllerImplementationInformationL ( *controllers[0] );

    CleanupStack::PopAndDestroy(3);

    }

void CCamcTest_6::MMFRegistration_002_L()
    {
    ///////////////////////////////////////
    //   ID: CAMC_API.MMFREGISTRATION_002
    //   Action :
    //        Check that camcorder registration was sucessfull.
    //        Ask for :Controller UID, Version, Supplier, 
    //        Supported Media UIDs and Formats
    //   Output : 
    //        must be sensible
    ///////////////////////////////////////   
    
    /////////////////////////////////////////////////////
    // Retrieve a list of possible controllers from ECOM
    CMMFControllerPluginSelectionParameters * cSelect = CMMFControllerPluginSelectionParameters::NewLC();
    CMMFFormatSelectionParameters * fSelect = CMMFFormatSelectionParameters::NewLC();

    fSelect->SetMatchToMimeTypeL(_L8("video/3gpp2"));
    cSelect->SetRequiredRecordFormatSupportL(*fSelect);
    
    RMMFControllerImplInfoArray controllers;
    CleanupResetAndDestroyPushL(controllers);
    cSelect->ListImplementationsL(controllers);
    
    // Only one Controller is expected to be found
    // This might change in the future.
    if (controllers.Count() !=1 )
        {
        User::Leave(K_Tc6_ErrExpectingADifferentValue);
        }
    iCamcTestClient->CheckControllerImplementationInformationNewL ( *controllers[0] );

    CleanupStack::PopAndDestroy(3);

    }

void CCamcTest_6::MMFRegistration_003_L()
    {
    ///////////////////////////////////////
    //   ID: CAMC_API.MMFREGISTRATION_003
    //   Action :
    //        Check that camcorder registration was sucessfull.
    //        Ask for Controller UID, Version, Supplier, Supported media UID
    //        and Formats.
    //   Output : 
    //        must be sensible
    ///////////////////////////////////////   
    
    /////////////////////////////////////////////////////
    // Retrieve a list of possible controllers from ECOM
    CMMFControllerPluginSelectionParameters * cSelect = CMMFControllerPluginSelectionParameters::NewLC();
    CMMFFormatSelectionParameters * fSelect = CMMFFormatSelectionParameters::NewLC();

    fSelect->SetMatchToMimeTypeL(_L8("video/3gpp"));
    cSelect->SetRequiredRecordFormatSupportL(*fSelect);
    
    RMMFControllerImplInfoArray controllers;
    CleanupResetAndDestroyPushL(controllers);
    cSelect->ListImplementationsL(controllers);
    
    // Only one Controller is expected to be found
    // This might change in the future.
    if (controllers.Count() !=1 )
        {
        User::Leave(K_Tc6_ErrExpectingADifferentValue);
        }
    iCamcTestClient->CheckControllerImplementationInformationNewL ( *controllers[0] );

    CleanupStack::PopAndDestroy(3);

    }

void CCamcTest_6::MMFRegistration_004_L()
    {
    ///////////////////////////////////////
    //   ID: CAMC_API.MMFREGISTRATION_004
    //   Action :
    //        Check that camcorder registration was sucessfull.
    //        Ask for Controller UID, Version, Supplier, Supported media UID
    //        and Formats.
    //   Output : 
    //        must be sensible
    ///////////////////////////////////////   
    
    /////////////////////////////////////////////////////
    // Retrieve a list of possible controllers from ECOM
    CMMFControllerPluginSelectionParameters * cSelect = CMMFControllerPluginSelectionParameters::NewLC();
    CMMFFormatSelectionParameters * fSelect = CMMFFormatSelectionParameters::NewLC();

    fSelect->SetMatchToMimeTypeL(_L8("video/mp4"));
    cSelect->SetRequiredRecordFormatSupportL(*fSelect);
    
    RMMFControllerImplInfoArray controllers;
    CleanupResetAndDestroyPushL(controllers);
    cSelect->ListImplementationsL(controllers);
    
    // Only one Controller is expected to be found
    // This might change in the future.
    if (controllers.Count() !=1 )
        {
        User::Leave(K_Tc6_ErrExpectingADifferentValue);
        }
    iCamcTestClient->CheckControllerImplementationInformationNewMPEG4L ( *controllers[0] );

    CleanupStack::PopAndDestroy(3);

    }


void CCamcTest_6::MMFRegistration_005_L()
    {
    ///////////////////////////////////////
    //   ID: CAMC_API.MMFREGISTRATION_005
    //   Action :
    //        Check that camcorder registration was sucessfull.
    //        Ask for Controller UID, Version, Supplier, Supported media UID
    //        and Formats.
    //   Output : 
    //        must be sensible
    ///////////////////////////////////////   
    
    /////////////////////////////////////////////////////
    // Retrieve a list of possible controllers from ECOM
    CMMFControllerPluginSelectionParameters * cSelect = CMMFControllerPluginSelectionParameters::NewLC();
    CMMFFormatSelectionParameters * fSelect = CMMFFormatSelectionParameters::NewLC();

    fSelect->SetMatchToMimeTypeL(_L8("video/3gpp2"));
    cSelect->SetRequiredRecordFormatSupportL(*fSelect);
    
    RMMFControllerImplInfoArray controllers;
    CleanupResetAndDestroyPushL(controllers);
    cSelect->ListImplementationsL(controllers);
    
    // Only one Controller is expected to be found
    // This might change in the future.
    if (controllers.Count() !=1 )
        {
        User::Leave(K_Tc6_ErrExpectingADifferentValue);
        }
    iCamcTestClient->CheckControllerImplementationInformationNewMPEG4L ( *controllers[0] );

    CleanupStack::PopAndDestroy(3);

    }


void CCamcTest_6::MMFRegistration_006_L()
    {
    ///////////////////////////////////////
    //   ID: CAMC_API.MMFREGISTRATION_006
    //   Action :
    //        Check that camcorder registration was sucessfull.
    //        Ask for Controller UID, Version, Supplier, Supported media UID
    //        and Formats.
    //   Output : 
    //        must be sensible
    ///////////////////////////////////////   
    
    /////////////////////////////////////////////////////
    // Retrieve a list of possible controllers from ECOM
    CMMFControllerPluginSelectionParameters * cSelect = CMMFControllerPluginSelectionParameters::NewLC();
    CMMFFormatSelectionParameters * fSelect = CMMFFormatSelectionParameters::NewLC();

    fSelect->SetMatchToMimeTypeL(_L8("video/3gpp"));
    cSelect->SetRequiredRecordFormatSupportL(*fSelect);
    
    RMMFControllerImplInfoArray controllers;
    CleanupResetAndDestroyPushL(controllers);
    cSelect->ListImplementationsL(controllers);
    
    // Only one Controller is expected to be found
    // This might change in the future.
    if (controllers.Count() !=1 )
        {
        User::Leave(K_Tc6_ErrExpectingADifferentValue);
        }
    iCamcTestClient->CheckControllerImplementationInformationNewMPEG4L ( *controllers[0] );

    CleanupStack::PopAndDestroy(3);

    }



// //////////////////////////////////////////////////////////////////
//   Dummy Camera API callback functions
// //////////////////////////////////////////////////////////////////
void CCamcTest_6::ReserveComplete(TInt /*aError*/)
    {
    RDebug::Print(_L("CCamcTest_6::ReserveComplete"));
        
    // UI level camera is being initialized        
    if (iWaitScheduler->IsStarted())
	    {
    	 iUiLevelCamera->PowerOn();
    	 return;
	    }
    }

void CCamcTest_6::PowerOnComplete(TInt /*aError*/)
    {   
    RDebug::Print(_L("CCamcTest_6::PowerOnComplete"));

    // UI level camera is being initialized
    if (iWaitScheduler->IsStarted())
	    {
    	 iWaitScheduler->AsyncStop();
    	 return;
    	}        
    }

void CCamcTest_6::ViewFinderFrameReady(CFbsBitmap& /*aFrame*/)
    {
    }
void CCamcTest_6::ImageReady(CFbsBitmap* /*aBitmap */,HBufC8* /*aData*/,TInt /*aError*/)
    {    
    }
void CCamcTest_6::FrameBufferReady(MFrameBuffer* /*aFrameBuffer*/,TInt /*aError*/ )
    {
    }

  
// -----------------------------------------------------------------------------
// CCamcTest_6::HandleEvent
// MCameraObserver2 call-back handler
// -----------------------------------------------------------------------------
//
void CCamcTest_6::HandleEvent( const TECAMEvent& aEvent)
    {
    PRINT(( _L( "CCamcTest_6::HandleEvent() entering, type=%x, err=%d" ), aEvent.iEventType.iUid, aEvent.iErrorCode ));

    if ( aEvent.iEventType == KUidECamEventCameraNoLongerReserved )
        {
        PRINT(( _L( "CCamcTest_6::HandleEvent() KUidECamEventCameraNoLongerReserved" ) ));
        }
    else if ( aEvent.iEventType == KUidECamEventPowerOnComplete )
        {
        PRINT(( _L( "CCamcTest_6::HandleEvent() KUidECamEventPowerOnComplete" ) ));
        PowerOnComplete( aEvent.iErrorCode );
        }
    else if ( aEvent.iEventType == KUidECamEventReserveComplete )
       {
       PRINT(( _L( "CCamcTest_6::HandleEvent() KUidECamEventReserveComplete" ) ));
       ReserveComplete( aEvent.iErrorCode );
       }

    PRINT(( _L( "CCamcTestClient_7::HandleEvent() returning" ) ));
    }

// -----------------------------------------------------------------------------
// CCamcTest_6::ViewFinderReady
// MCameraObserver2 call-back handler
// -----------------------------------------------------------------------------
//
void CCamcTest_6::ViewFinderReady( MCameraBuffer& aCameraBuffer, TInt aError)
    {
    PRINT(( _L( "CCamcTest_6::ViewFinderReady() NOT HANDLED err=%d, NumFrames=%d" ), aError, aCameraBuffer.NumFrames() ));
    CFbsBitmap *buffer = NULL;
    if ( !aError )
        {
	    aCameraBuffer.Release();
        }
    ViewFinderFrameReady( *buffer );
    }

// -----------------------------------------------------------------------------
// CCamcTest_6::ImageBufferReady
// MCameraObserver2 call-back handler
// -----------------------------------------------------------------------------
//
void CCamcTest_6::ImageBufferReady( MCameraBuffer& aCameraBuffer, TInt aError)
    {
    PRINT(( _L( "CCamcTest_6::ImageBufferReady() BUFFER NOT HANDLED, err=%d, NumFrames=%d" ), aError, aCameraBuffer.NumFrames() ));
	
    if ( !aError )
        {
	    aCameraBuffer.Release();
        }
    ImageReady( NULL, NULL, aError );
    }
    
// -----------------------------------------------------------------------------
// CCamcTest_6::VideoBufferReady
// MCameraObserver2 call-back handler
// -----------------------------------------------------------------------------
//
void CCamcTest_6::VideoBufferReady( MCameraBuffer& aCameraBuffer, TInt aError)
    {
    PRINT(( _L( "CCamcTest_6::VideoBufferReady() entering, err=%d, NumFrames=%d" ), aError, aCameraBuffer.NumFrames() ));
    MFrameBuffer* buffer = NULL;
	
    if ( !aError )
        {
        aCameraBuffer.Release();
        }
    // Call old Camera-API observer handler
    FrameBufferReady( buffer, aError );
    
    PRINT(( _L( "CCamcTest_6::VideoBufferReady() returning" )));
    }



/*********************************************************************************************/


MTest* CCamcTest_6::suiteL ()
    {
    // Always use NewL (Do not use NewLC) !!!
    CTestSuite *suite = CTestSuite::NewL(_L8("CCamcTest_6")); 
    CleanupStack::PushL( suite );

    suite->addTestL(CTestCaller<CCamcTest_6>::NewL(_L8("CAMC_API.PAUSEL_003"), &CCamcTest_6::PauseL_003_L));

    suite->addTestL(CTestCaller<CCamcTest_6>::NewL(_L8("CAMC_API.SETVIDEOFRAMESIZEL_005"), &CCamcTest_6::SetVideoFrameSizeL_005_L));

    suite->addTestL(CTestCaller<CCamcTest_6>::NewL(_L8("CAMC_API.SETAUDIOENABLEDL_005"), &CCamcTest_6::SetAudioEnabledL_005_L));

    suite->addTestL(CTestCaller<CCamcTest_6>::NewL(_L8("CAMC_API.SETMAXCLIPSIZEL_006"), &CCamcTest_6::SetMaxClipSizeL_006_L));
    suite->addTestL(CTestCaller<CCamcTest_6>::NewL(_L8("CAMC_API.SETMAXCLIPSIZEL_007"), &CCamcTest_6::SetMaxClipSizeL_007_L));

    // Record_006, only to be tested on HW
    // Removed from MuDo tests
#if !( defined (__WINS__) || defined (__WINSCW__) )
//    suite->addTestL(CTestCaller<CCamcTest_6>::NewL(_L8("CAMC_API.RECORD_006"), &CCamcTest_6::Record_006_L));
#endif
    suite->addTestL(CTestCaller<CCamcTest_6>::NewL(_L8("CAMC_API.RECORD_007"), &CCamcTest_6::Record_007_L));

    suite->addTestL(CTestCaller<CCamcTest_6>::NewL(_L8("CAMC_API.VIDEOTYPEL_001"), &CCamcTest_6::VideoTypeL_001_L));
    suite->addTestL(CTestCaller<CCamcTest_6>::NewL(_L8("CAMC_API.VIDEOTYPEL_002"), &CCamcTest_6::VideoTypeL_002_L));
    suite->addTestL(CTestCaller<CCamcTest_6>::NewL(_L8("CAMC_API.VIDEOTYPEL_003"), &CCamcTest_6::VideoTypeL_003_L));
    suite->addTestL(CTestCaller<CCamcTest_6>::NewL(_L8("CAMC_API.VIDEOTYPEL_004"), &CCamcTest_6::VideoTypeL_004_L));
   
#ifdef __MPEG4_VIDEO_ENCODING
    suite->addTestL(CTestCaller<CCamcTest_6>::NewL(_L8("CAMC_API.VIDEOTYPEL_005"), &CCamcTest_6::VideoTypeL_005_L));
#endif

    suite->addTestL(CTestCaller<CCamcTest_6>::NewL(_L8("CAMC_API.AUDIOTYPEL_001"), &CCamcTest_6::AudioTypeL_001_L));
    suite->addTestL(CTestCaller<CCamcTest_6>::NewL(_L8("CAMC_API.AUDIOTYPEL_002"), &CCamcTest_6::AudioTypeL_002_L));
    suite->addTestL(CTestCaller<CCamcTest_6>::NewL(_L8("CAMC_API.AUDIOTYPEL_003"), &CCamcTest_6::AudioTypeL_003_L));
 
    suite->addTestL(CTestCaller<CCamcTest_6>::NewL(_L8("CAMC_API.SETVIDEOTYPEL_005"), &CCamcTest_6::SetVideoTypeL_005_L));
   
#ifdef __MPEG4_VIDEO_ENCODING
    suite->addTestL(CTestCaller<CCamcTest_6>::NewL(_L8("CAMC_API.SETVIDEOTYPEL_006"), &CCamcTest_6::SetVideoTypeL_006_L));
#endif
    suite->addTestL(CTestCaller<CCamcTest_6>::NewL(_L8("CAMC_API.SETVIDEOTYPEL_007"), &CCamcTest_6::SetVideoTypeL_007_L));
    suite->addTestL(CTestCaller<CCamcTest_6>::NewL(_L8("CAMC_API.SETVIDEOTYPEL_008"), &CCamcTest_6::SetVideoTypeL_008_L));

#ifdef MP4_FILE_FORMAT_SUPPORTED
#if ((!defined __WINS__) || (!defined __WINSCW__)) // AAC supported only in HW
    suite->addTestL(CTestCaller<CCamcTest_6>::NewL(_L8("CAMC_API.SETVIDEOTYPEL_009"), &CCamcTest_6::SetVideoTypeL_009_L));
#endif
#endif
    
    
    suite->addTestL(CTestCaller<CCamcTest_6>::NewL(_L8("CAMC_API.SETAUDIOTYPEL_005"), &CCamcTest_6::SetAudioTypeL_005_L));
    suite->addTestL(CTestCaller<CCamcTest_6>::NewL(_L8("CAMC_API.SETAUDIOTYPEL_007"), &CCamcTest_6::SetAudioTypeL_007_L));   

#ifdef MP4_FILE_FORMAT_SUPPORTED
#if ((!defined __WINS__) || (!defined __WINSCW__)) // AAC supported only in HW
    suite->addTestL(CTestCaller<CCamcTest_6>::NewL(_L8("CAMC_API.SETAUDIOTYPEL_008"), &CCamcTest_6::SetAudioTypeL_008_L));
#endif
#endif

    suite->addTestL(CTestCaller<CCamcTest_6>::NewL(_L8("CAMC_API.CONTROLLERIMPLEMENTATIONINFORMATIONL_001"), &CCamcTest_6::ControllerImplementationInformationL_001_L));   
#ifndef MP4_FILE_FORMAT_SUPPORTED
    suite->addTestL(CTestCaller<CCamcTest_6>::NewL(_L8("CAMC_API.CONTROLLERIMPLEMENTATIONINFORMATIONL_003"), &CCamcTest_6::ControllerImplementationInformationL_003_L));   
#else
    suite->addTestL(CTestCaller<CCamcTest_6>::NewL(_L8("CAMC_API.CONTROLLERIMPLEMENTATIONINFORMATIONL_004"), &CCamcTest_6::ControllerImplementationInformationL_004_L));   
#endif

#ifndef MP4_FILE_FORMAT_SUPPORTED
    suite->addTestL(CTestCaller<CCamcTest_6>::NewL(_L8("CAMC_API.MMFREGISTRATION_002"), &CCamcTest_6::MMFRegistration_002_L));   
    suite->addTestL(CTestCaller<CCamcTest_6>::NewL(_L8("CAMC_API.MMFREGISTRATION_003"), &CCamcTest_6::MMFRegistration_003_L));   
#else
    suite->addTestL(CTestCaller<CCamcTest_6>::NewL(_L8("CAMC_API.MMFREGISTRATION_004"), &CCamcTest_6::MMFRegistration_004_L));   
    suite->addTestL(CTestCaller<CCamcTest_6>::NewL(_L8("CAMC_API.MMFREGISTRATION_005"), &CCamcTest_6::MMFRegistration_005_L));   
    suite->addTestL(CTestCaller<CCamcTest_6>::NewL(_L8("CAMC_API.MMFREGISTRATION_006"), &CCamcTest_6::MMFRegistration_006_L));   
#endif

    suite->addTestL(CTestCaller<CCamcTest_6>::NewL(_L8("CAMC_API.DURATIONL_001"), &CCamcTest_6::DurationL_001_L));   
    suite->addTestL(CTestCaller<CCamcTest_6>::NewL(_L8("CAMC_API.DURATIONL_002"), &CCamcTest_6::DurationL_002_L));   
    suite->addTestL(CTestCaller<CCamcTest_6>::NewL(_L8("CAMC_API.DURATIONL_003"), &CCamcTest_6::DurationL_003_L));   

    CleanupStack::Pop( suite );
    return suite;
    }


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
#include "camcTest_9.h"
#include "camcTest.h"
#include <bldvariant.hrh>


// EXTERNAL DATA STRUCTURES

// EXTERNAL FUNCTION PROTOTYPES 
 
// CONSTANTS

// MACROS

// LOCAL CONSTANTS AND MACROS

// Allows a 10% margin for the filesize
#define TC9_ALLOWED_MARGIN ( TC9_FILE_SIZE_LIMIT * 0.1 )

// MODULE DATA STRUCTURES

// LOCAL FUNCTION PROTOTYPES

// FORWARD DECLARATIONS

CCamcTest_9::CCamcTest_9 ()
    {
    }



CCamcTest_9::~CCamcTest_9 ()
    {
    delete iCamcTestClient;
    iCamcTestClient = NULL;
    }


void CCamcTest_9::setUpL ()
    {
        {
        iCamcTestClient = CCamcTestClient_9::NewL( );
        }
    // Do nothing in the setUp to get Memory Failure functionality
    iWaitScheduler = new (ELeave) CActiveSchedulerWait;	

    // Create a new Camera API implementation object, if supported
    TRAPD( err, iUiLevelCamera = CCamera::New2L( static_cast<MCameraObserver2&>(*this), 0 /*index*/, 0 /*Priority*/ ) );
    if ( err )
        {
        PRINT( ( _L( "CCamcTest_9::setUpL CCamera::New2L return code=%d" ), err ) ); 
        // Create old Camera API implementation object.
        iUiLevelCamera = CCamera::NewL( static_cast<MCameraObserver&>(*this), 0 /*Index*/ );
        }
    else 
       {
       PRINT( ( _L( "CCamcTest_9::setUp() using new MCameraObserver2" )) ); 
       }

	iUiLevelCameraHandle = iUiLevelCamera->Handle();
	iUiLevelCamera->Reserve();
	iWaitScheduler->Start();     
    }


void CCamcTest_9::tearDown ()
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
//                       Prepare tests
// //////////////////////////////////////////////////////////////////

void CCamcTest_9::Prepare_007_L()
    {
    ///////////////////////////////////////
    //   ID: CAMC_API.PREPARE_007
    //   Action :
    //        Prepare when controller is open.
    //        Call prepare, without setting anything (using all defaults)
    //   Output : 
    //        OK
    ///////////////////////////////////////    
    
    iCamcTestClient->Start_Active_ObjectL(K_Tc9_PrepareWhenControllerReadyWhitoutSetting, iUiLevelCameraHandle);
    }
    
void CCamcTest_9::Prepare_008_L()
    {
    ///////////////////////////////////////
    //   ID: CAMC_API.PREPARE_008
    //   Action :
    //        Prepare when controller is open. Call prepare, setting 
    //        only VideoBitRate (using defaults for the other parameters)
    //   Output : 
    //        OK
    ///////////////////////////////////////    
    
    iCamcTestClient->Start_Active_ObjectL(K_Tc9_PrepareWhenControllerReadyOnlyVideoBitRateSetting, iUiLevelCameraHandle);
    }

void CCamcTest_9::Prepare_009_L()
    {
    ///////////////////////////////////////
    //   ID: CAMC_API.PREPARE_009
    //   Action :
    //        Prepare when controller is open. Call prepare, setting 
    //        only VideoFrameSize (using defaults for the other parameters)
    //   Output : 
    //        OK
    ///////////////////////////////////////    
    
    iCamcTestClient->Start_Active_ObjectL(K_Tc9_PrepareWhenControllerReadyOnlyVideoFrameSizeSetting, iUiLevelCameraHandle);
    }

void CCamcTest_9::RecordTimeAvailableL_005_L()
    {
    ///////////////////////////////////////
	//   ID: CAMC_API.RECORDTIMEAVAILABLEL_005
    //   Action :    
    //       Asking RecordTimeAvailable()
    //       Once Recording is ready
    //   Output : 
    //       Value must be sensible
    ///////////////////////////////////////
    
    iCamcTestClient->Start_Active_ObjectL( K_Tc9_RecordTimeAvailWhenRecordingReady, iUiLevelCameraHandle );
    }

/**************************/
/*   SET MAX CLIP SIZE    */
/**************************/ 

void CCamcTest_9::SetMaxClipSizeL_008_L()
    {
    ///////////////////////////////////////
    //   ID: CAMC_API.SETMAXCLIPSIZEL_008
    //   Action :
    //        SetMaxClipSizeL when controller is open. Set size = 0
    //   Output : 
    //        Must record until disk full.
    ///////////////////////////////////////    
    
    iCamcTestClient->Start_Active_ObjectL(K_Tc9_SetMaxClipSizeLWhenControllerReadySize0, iUiLevelCameraHandle);
    }

void CCamcTest_9::SetMaxClipSizeL_009_L()
    {
    ///////////////////////////////////////
    //   ID: CAMC_API.SETMAXCLIPSIZEL_009
    //   Action :
    //        SetMaxClipSizeL when controller is open. Set size = KMMFNoMaxClipSize
    //   Output : 
    //        Must record until disk full.
    ///////////////////////////////////////    
    
    iCamcTestClient->Start_Active_ObjectL(K_Tc9_SetMaxClipSizeLWhenControllerReadyKMMFNoMaxClipSize, iUiLevelCameraHandle);
    }

/**************************/
/*     NEW FILE NAME      */
/**************************/ 

void CCamcTest_9::NewFileName_001_L()
    {
    ///////////////////////////////////////
    //   ID: CAMC_API.NEWFILENAME_001
    //   Action :
    //        Set NewFileName when prepared, not recording 
    //   Output : 
    //        New video, which have new name.
    ///////////////////////////////////////    
    TInt err;

    RFile file; 
    RFs fsSession;
    fsSession.Connect();
    
    TFileName fileName;
    AddDriveLetterToPath(_L("NewFileName.3gp"),fileName);
    err = fsSession.Delete( fileName );

    if ( ( err != KErrNone ) && ( err != KErrNotFound ) )
        {
        User::Leave(KErrGeneral);
        }

    iCamcTestClient->Start_Active_ObjectL(K_TC9_NewFileNameWhenPreparedNotRecording, iUiLevelCameraHandle);

    err = file.Open(fsSession,fileName,EFileShareAny);    
    file.Close();
    fsSession.Close();
    assertTIntsEqualL( KErrNone, err );
    }

void CCamcTest_9::NewFileName_002_L()
    {
    ///////////////////////////////////////
    //   ID: CAMC_API.NEWFILENAME_002
    //   Action :
    //        Set NewFileName when recorded ready 
    //   Output : 
    //        Record two videos, and the second one have new name.
    ///////////////////////////////////////    
   
    RFile file; 
    RFs fsSession;
    fsSession.Connect();
    TInt err;
    TFileName fileName;
    AddDriveLetterToPath(_L("recordQCIF.3gp"),fileName);
    TFileName newfile;
    AddDriveLetterToPath(_L("NewFileName.3gp"),newfile);
    err = fsSession.Delete( fileName ); 
    if ( ( err != KErrNone ) && ( err != KErrNotFound ) )
        {
        User::Leave(KErrGeneral);
        }

    err = fsSession.Delete( newfile );
    if ( ( err != KErrNone ) && ( err != KErrNotFound ) )
        {
        User::Leave(KErrGeneral);
        }

    iCamcTestClient->Start_Active_ObjectL(K_TC9_NewFileNameWhenRecordSecondTime, iUiLevelCameraHandle);


    err = file.Open(fsSession,fileName,EFileShareAny);
    assertTIntsEqualL( KErrNone, err );
    err = file.Open(fsSession,newfile,EFileShareAny);
    assertTIntsEqualL( KErrNone, err );

    file.Close();
    fsSession.Close();
    }

void CCamcTest_9::NewFileName_003_L()
    {
    ///////////////////////////////////////
    //   ID: CAMC_API.NEWFILENAME_003
    //   Action :
    //        Set NewFileName when recording 
    //   Output : 
    //        Leave.
    ///////////////////////////////////////    
    
    TRAPD(err, iCamcTestClient->Start_Active_ObjectL(K_TC9_NewFileNameWhenRecording, iUiLevelCameraHandle));
    assertTIntsEqualL( KErrNotReady, err );
    }

void CCamcTest_9::NewFileName_004_L()
    {
    ///////////////////////////////////////
    //   ID: CAMC_API.NEWFILENAME_004
    //   Action :
    //        Set NewFileName when paused 
    //   Output : 
    //        Leave.
    ///////////////////////////////////////    
    
    TRAPD(err, iCamcTestClient->Start_Active_ObjectL(K_TC9_NewFileNameWhenPaused, iUiLevelCameraHandle));
    assertTIntsEqualL( KErrNotReady, err );
    }

void CCamcTest_9::NewFileName_005_L()
    {
    ///////////////////////////////////////
    //   ID: CAMC_API.NEWFILENAME_005
    //   Action :
    //        One video is recorded. It ends because of filesize reached.
    //        A second video is recorded, changing the filename using custom
    //        command, no OpenFile(). 
    //   Output : 
    //        FileSize of both video is ok.
    ///////////////////////////////////////    
       
    RFile file; 
    TInt filesize,err;
    RFs fsSession;
    fsSession.Connect();
    TFileName fileName1, fileName2;
  
    TRAPD(leavingCode,
        {
        AddDriveLetterToPath(_L("recordQCIF.3gp"),fileName1);
        err = fsSession.Delete( fileName1 ); 
        AddDriveLetterToPath(_L("NewFileName.3gp"),fileName2);
        err = fsSession.Delete( fileName2 ); 
        
        // Start the test case
        iCamcTestClient->Start_Active_ObjectL(K_TC9_NewFileNameWhenInternalStop, iUiLevelCameraHandle);
        
        // Check if size of files is according to previously set Size limit 
        err = file.Open(fsSession,fileName1,EFileShareAny);
        assertTIntsEqualL( KErrNone, err );
        err = file.Size(filesize);
        file.Close();
        
        if ( ( filesize > TC9_FILE_SIZE_LIMIT + TC9_ALLOWED_MARGIN ) ||
            (filesize < TC9_FILE_SIZE_LIMIT - TC9_ALLOWED_MARGIN ) )
            {
            assertL(0);
            }

        err = file.Open(fsSession,fileName2,EFileShareAny);
        assertTIntsEqualL( KErrNone, err );
        err = file.Size(filesize);
        file.Close();
        
        if ( ( filesize > TC9_FILE_SIZE_LIMIT + TC9_ALLOWED_MARGIN ) ||
            (filesize < TC9_FILE_SIZE_LIMIT - TC9_ALLOWED_MARGIN ) )
            {
            assertL(0);
            }
        } ); //End of TRAPD
    
    fsSession.Close();
    User::LeaveIfError(leavingCode);
    }

void CCamcTest_9::NewFileName_006_L()
    {
    ///////////////////////////////////////
    //   ID: CAMC_API.NEWFILENAME_006
    //   Action :
    //        Custom command sent, prepare(), and record() 
    //   Output : 
    //        2nd recorded file has the name 
    //        requested in the custom command.
    ///////////////////////////////////////    
   
    RFile file; 
    RFs fsSession;
    fsSession.Connect();
    TInt err;
    TFileName fileName;
    AddDriveLetterToPath(_L("recordQCIF.3g2"),fileName);
    TFileName newfile;
    AddDriveLetterToPath(_L("NewFileName.3g2"),newfile);
    err = fsSession.Delete( fileName ); 
    if ( ( err != KErrNone ) && ( err != KErrNotFound ) )
        {
        User::Leave(KErrGeneral);
        }

    err = fsSession.Delete( newfile );
    if ( ( err != KErrNone ) && ( err != KErrNotFound ) )
        {
        User::Leave(KErrGeneral);
        }

    iCamcTestClient->Start_Active_ObjectL(K_TC9_NewFileNameWhenRecordSecondTimeDiffFileFormat, iUiLevelCameraHandle);


    err = file.Open(fsSession,fileName,EFileShareAny);
    assertTIntsEqualL( KErrNone, err );
    err = file.Open(fsSession,newfile,EFileShareAny);
    assertTIntsEqualL( KErrNone, err );

    file.Close();
    fsSession.Close();
    }

void CCamcTest_9::NewFileName_007_L()
    {
    ///////////////////////////////////////
    //   ID: CAMC_API.NEWFILENAME_007
    //   Action :
    //        Custom command sent, prepare(), and record() 
    //   Output : 
    //        2nd recorded file has the name 
    //        requested in the custom command.
    ///////////////////////////////////////    
   
    RFile file; 
    RFs fsSession;
    fsSession.Connect();
    TInt err;
    TFileName fileName;
    AddDriveLetterToPath(_L("recordQCIF.mp4"),fileName);
    TFileName newfile;
    AddDriveLetterToPath(_L("NewFileName.mp4"),newfile);
    err = fsSession.Delete( fileName ); 
    if ( ( err != KErrNone ) && ( err != KErrNotFound ) )
        {
        User::Leave(KErrGeneral);
        }

    err = fsSession.Delete( newfile );
    if ( ( err != KErrNone ) && ( err != KErrNotFound ) )
        {
        User::Leave(KErrGeneral);
        }

    iCamcTestClient->Start_Active_ObjectL(K_TC9_NewFileNameWhenRecordSecondTimeMPEG4FileFormat, iUiLevelCameraHandle);


    err = file.Open(fsSession,fileName,EFileShareAny);
    assertTIntsEqualL( KErrNone, err );
    err = file.Open(fsSession,newfile,EFileShareAny);
    assertTIntsEqualL( KErrNone, err );

    file.Close();
    fsSession.Close();
    }



/*****************************/
/*      OPEN FILE            */
/*****************************/

void CCamcTest_9::OpenFileL_016_L()
    {
    
    ///////////////////////////////////////
    //	 ID: CAMC_API.OPENFILEL_016
    //    
    //   Action :FileOpen with FileName "foo/filename.3gp", ie a folder
    //             that doesn't exists. Then normal recording procedure.
    //	        
    //   Output :Folder is created, video is correctly recorded. 
    //             (Existence of the video checked)
    ///////////////////////////////////////   

    iCamcTestClient->Start_Active_ObjectL(K_Tc9_OpenFileInAFolderNonExistent, iUiLevelCameraHandle);  
    
    RFile file; 
    RFs fsSession;
    fsSession.Connect();
    TInt err;
    TFileName fileName;
    AddDriveLetterToPath(_L("non-existent-folder\\recordQCIF.3gp"), fileName);
    if ( err = file.Open(fsSession,fileName,EFileShareAny) )
        {
        fsSession.Close();
        }
    assertTIntsEqualL( KErrNone, err );

    file.Close();
    fsSession.Close();
    }

//
// An own static function to collect the test functions into one 
// suite of tests. The framework will run the tests and free the
// memory allocated for the test suite.
// 


// //////////////////////////////////////////////////////////////////
//   Dummy Camera API callback functions
// //////////////////////////////////////////////////////////////////
void CCamcTest_9::ReserveComplete(TInt /*aError*/)
    {
    RDebug::Print(_L("CCamcTest_9::ReserveComplete"));
        
    // UI level camera is being initialized        
    if (iWaitScheduler->IsStarted())
	    {
    	 iUiLevelCamera->PowerOn();
    	 return;
	    }
    }

void CCamcTest_9::PowerOnComplete(TInt /*aError*/)
    {   
    RDebug::Print(_L("CCamcTest_9::PowerOnComplete"));

    // UI level camera is being initialized
    if (iWaitScheduler->IsStarted())
	    {
    	 iWaitScheduler->AsyncStop();
    	 return;
    	}        
    }

void CCamcTest_9::ViewFinderFrameReady(CFbsBitmap& /*aFrame*/)
    {
    }
void CCamcTest_9::ImageReady(CFbsBitmap* /*aBitmap */,HBufC8* /*aData*/,TInt /*aError*/)
    {    
    }
void CCamcTest_9::FrameBufferReady(MFrameBuffer* /*aFrameBuffer*/,TInt /*aError*/ )
    {
    }

// -----------------------------------------------------------------------------
// CCamcTest_9::HandleEvent
// MCameraObserver2 call-back handler
// -----------------------------------------------------------------------------
//
void CCamcTest_9::HandleEvent( const TECAMEvent& aEvent)
    {
    PRINT(( _L( "CCamcTest_9::HandleEvent() entering, type=%x, err=%d" ), aEvent.iEventType.iUid, aEvent.iErrorCode ));

    if ( aEvent.iEventType == KUidECamEventCameraNoLongerReserved )
        {
        PRINT(( _L( "CCamcTest_9::HandleEvent() KUidECamEventCameraNoLongerReserved" ) ));
        }
    else if ( aEvent.iEventType == KUidECamEventPowerOnComplete )
        {
        PRINT(( _L( "CCamcTest_9::HandleEvent() KUidECamEventPowerOnComplete" ) ));
        PowerOnComplete( aEvent.iErrorCode );
        }
    else if ( aEvent.iEventType == KUidECamEventReserveComplete )
       {
       PRINT(( _L( "CCamcTest_9::HandleEvent() KUidECamEventReserveComplete" ) ));
       ReserveComplete( aEvent.iErrorCode );
       }

    PRINT(( _L( "CCamcTestClient_7::HandleEvent() returning" ) ));
    }

// -----------------------------------------------------------------------------
// CCamcTest_9::ViewFinderReady
// MCameraObserver2 call-back handler
// -----------------------------------------------------------------------------
//
void CCamcTest_9::ViewFinderReady( MCameraBuffer& aCameraBuffer, TInt aError)
    {
    PRINT(( _L( "CCamcTest_9::ViewFinderReady() NOT HANDLED err=%d, NumFrames=%d" ), aError, aCameraBuffer.NumFrames() ));
    CFbsBitmap *buffer = NULL;
    if ( !aError )
        {
	    aCameraBuffer.Release();
        }
    ViewFinderFrameReady( *buffer );
    }

// -----------------------------------------------------------------------------
// CCamcTest_9::ImageBufferReady
// MCameraObserver2 call-back handler
// -----------------------------------------------------------------------------
//
void CCamcTest_9::ImageBufferReady( MCameraBuffer& aCameraBuffer, TInt aError)
    {
    PRINT(( _L( "CCamcTest_9::ImageBufferReady() BUFFER NOT HANDLED, err=%d, NumFrames=%d" ), aError, aCameraBuffer.NumFrames() ));
	
    if ( !aError )
        {
	    aCameraBuffer.Release();
        }
    ImageReady( NULL, NULL, aError );
    }
    
// -----------------------------------------------------------------------------
// CCamcTest_9::VideoBufferReady
// MCameraObserver2 call-back handler
// -----------------------------------------------------------------------------
//
void CCamcTest_9::VideoBufferReady( MCameraBuffer& aCameraBuffer, TInt aError)
    {
    PRINT(( _L( "CCamcTest_9::VideoBufferReady() entering, err=%d, NumFrames=%d" ), aError, aCameraBuffer.NumFrames() ));
    MFrameBuffer* buffer = NULL;
	
    if ( !aError )
        {
        aCameraBuffer.Release();
        }
    // Call old Camera-API observer handler
    FrameBufferReady( buffer, aError );
    
    PRINT(( _L( "CCamcTest_9::VideoBufferReady() returning" )));
    }


MTest* CCamcTest_9::suiteL ()
    {
    // Always use NewL (Do not use NewLC) !!!
    CTestSuite *suite = CTestSuite::NewL(_L8("CCamcTest_9"));
    CleanupStack::PushL( suite );

    suite->addTestL(CTestCaller<CCamcTest_9>::NewL(_L8("CAMC_API.PREPARE_007"), &CCamcTest_9::Prepare_007_L));
    suite->addTestL(CTestCaller<CCamcTest_9>::NewL(_L8("CAMC_API.PREPARE_008"), &CCamcTest_9::Prepare_008_L));
    suite->addTestL(CTestCaller<CCamcTest_9>::NewL(_L8("CAMC_API.PREPARE_009"), &CCamcTest_9::Prepare_009_L));
    
    suite->addTestL(CTestCaller<CCamcTest_9>::NewL(_L8("CAMC_API.RECORDTIMEAVAILABLEL_005"), &CCamcTest_9::RecordTimeAvailableL_005_L));
    
    // SetMaxClipSizeL_008 and SetMaxClipSizeL_009, only to be tested on HW
#if !( defined (__WINS__) || defined (__WINSCW__) )
    suite->addTestL(CTestCaller<CCamcTest_9>::NewL(_L8("CAMC_API.SETMAXCLIPSIZEL_008"), &CCamcTest_9::SetMaxClipSizeL_008_L));
    suite->addTestL(CTestCaller<CCamcTest_9>::NewL(_L8("CAMC_API.SETMAXCLIPSIZEL_009"), &CCamcTest_9::SetMaxClipSizeL_009_L));
#endif
    
    suite->addTestL(CTestCaller<CCamcTest_9>::NewL(_L8("CAMC_API.NEWFILENAME_001"), &CCamcTest_9::NewFileName_001_L));
    suite->addTestL(CTestCaller<CCamcTest_9>::NewL(_L8("CAMC_API.NEWFILENAME_002"), &CCamcTest_9::NewFileName_002_L));
    suite->addTestL(CTestCaller<CCamcTest_9>::NewL(_L8("CAMC_API.NEWFILENAME_003"), &CCamcTest_9::NewFileName_003_L));
    suite->addTestL(CTestCaller<CCamcTest_9>::NewL(_L8("CAMC_API.NEWFILENAME_004"), &CCamcTest_9::NewFileName_004_L));
    suite->addTestL(CTestCaller<CCamcTest_9>::NewL(_L8("CAMC_API.NEWFILENAME_005"), &CCamcTest_9::NewFileName_005_L));
    suite->addTestL(CTestCaller<CCamcTest_9>::NewL(_L8("CAMC_API.NEWFILENAME_006"), &CCamcTest_9::NewFileName_006_L));
    
#ifdef MP4_FILE_FORMAT_SUPPORTED
#if ((!defined __WINS__) || (!defined __WINSCW__)) // AAC supported only in HW
    suite->addTestL(CTestCaller<CCamcTest_9>::NewL(_L8("CAMC_API.NEWFILENAME_007"), &CCamcTest_9::NewFileName_007_L));
#endif
#endif

    suite->addTestL(CTestCaller<CCamcTest_9>::NewL(_L8("CAMC_API.OPENFILEL_016"), &CCamcTest_9::OpenFileL_016_L));
   
    CleanupStack::Pop( suite );
    return suite;
    }


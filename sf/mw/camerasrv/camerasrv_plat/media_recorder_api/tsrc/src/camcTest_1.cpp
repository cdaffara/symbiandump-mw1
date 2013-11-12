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
#include "camcTest_1.h"
#include <bldvariant.hrh>


// EXTERNAL DATA STRUCTURES

// EXTERNAL FUNCTION PROTOTYPES 

// CONSTANTS

// MACROS
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



CCamcTest_1::CCamcTest_1 ()
    {
    }



CCamcTest_1::~CCamcTest_1 ()
    {
    delete iCamcTestClient;
    iCamcTestClient = NULL;
    }


void CCamcTest_1::setUpL ()
    {
    iCamcTestClient = CCamcTestClient_1::NewL( );
    iWaitScheduler = new (ELeave) CActiveSchedulerWait;	

    // Create a new Camera API implementation object, if supported
    TRAPD( err, iUiLevelCamera = CCamera::New2L( static_cast<MCameraObserver2&>(*this), 0 /*index*/, 0 /*Priority*/ ) );
    if ( err )
        {
        PRINT( ( _L( "CCamcTest_1::setUpL CCamera::New2L return code=%d" ), err ) ); 
        // Create old Camera API implementation object.
        iUiLevelCamera = CCamera::NewL( static_cast<MCameraObserver&>(*this), 0 /*Index*/ );
        }
    else 
       {
       PRINT( ( _L( "CCamcTest_1::setUp() using new MCameraObserver2" )) ); 
       }


	iUiLevelCameraHandle = iUiLevelCamera->Handle();
	iUiLevelCamera->Reserve();
	iWaitScheduler->Start();
    }


void CCamcTest_1::tearDown ()
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
//                       OpenFileL tests
// //////////////////////////////////////////////////////////////////

void CCamcTest_1::OpenFileL_001_L()
    {
    ///////////////////////////////////////
    //   ID: CAMC_API.OPENFILEL_001
    //   Action :
    //       CtrUid = OK
    //   Output : 
    //       OpenFile = OK
    //   Precondition :
    //       CVideoRecorderUtility is ready
    ///////////////////////////////////////
    
    TUid controllerUid = {CONTROLLER_UID};	
    TUid formatUid = {VIDEO_FORMAT_UID};
    
    iCamcTestClient->Test_OpenFileL(
        _L("recordQCIF.3gp"),		// File
        iUiLevelCameraHandle,		//CameraHandle
        controllerUid,					//Controller UID
        formatUid,      				//Format UID
        MIME_VIDEO_FORMAT,
        NO_AUDIO_FOURCC  				//Audio Type
        );
    
    // Succeed, if it doesn't leave.
    }


void CCamcTest_1::OpenFileL_002_L()
    {
    
    ///////////////////////////////////////
    //   ID: CAMC_API.OPENFILEL_002
    //   
    //     Action :
    //        CtrUid = None (0x0)
    //        FileName = OK
    //     Output : 
    //        OpenFile Leaves with KErrNotFound
    //        or Callback is called with error msg
    //    Precondition :
    //        CVideoRecorderUtility is ready
    ///////////////////////////////////////   
    
    TUid controllerUid = {0x0};	
    TUid formatUid = {VIDEO_FORMAT_UID};
    
    TRAPD(err, iCamcTestClient->Test_OpenFileL(
        _L("recordQCIF.3gp"),		// File
        iUiLevelCameraHandle,								//CameraHandle
        controllerUid,					//Controller UID
        formatUid,						//Format UID
        MIME_VIDEO_FORMAT,
        NO_AUDIO_FOURCC				    //Audio Type
        ));

    // Function may leave with KErrNotFound, that's ok
    if (err==KErrNotFound)
        return;
    // Otherwise, the MvruoOpenComplete should return with an error

    assertTIntsEqualL( KErrNotFound, iCamcTestClient->WaitForCallbackWithErrors() );

    }

void CCamcTest_1::OpenFileL_003_L()
    {
    
    ///////////////////////////////////////
    //   ID: CAMC_API.OPENFILEL_003
    //   
    //     Action :
    //        CtrUid = none
    //        FileName = Extension not OK to find the controller
    //     Output : 
    //        OpenFile = Leave with KErrArgument 
    //     Precondition :
    //        CVideoRecorderUtility is ready
    ///////////////////////////////////////    
    
    ////////////////////////////////////////
    // Due to functionality changes, Test case is not relevant anymore
    ////////////////////////////////////////
    assertL(0);
    
    //TUid controllerUid = {0};	
    //TUid formatUid = {0};
    
    //TRAPD(err,
    //    iCamcTestClient->Test_OpenFileL(
    //   _L("c:\\recordQCIF.txt"),		// File
    //    0,								//CameraHandle
    //    controllerUid,					//Controller UID
    //    formatUid,						//Format UID
    //    //TFourCC('2', '6', '3', 'B'),	//Video Type
    //    _L8("video/3gpp"),
    //    KMMFFourCCCodeNULL				//Audio Type
    //    ));
    
    // OpenFile should not succeed...
    // assertL (err==KErrArgument);
    }

void CCamcTest_1::OpenFileL_004_L()
    {
    
    ///////////////////////////////////////
    //	   ID: CAMC_API.OPENFILEL_004
    //    
    //     Action :
    //	        CtrUid  = incorrect
    //     Output : 
    //         OpenFile = Leaves 
    //           or MvruoOpenComplete return with an error
    //         
    //     Precondition :
    //        CVideoRecorderUtility is ready
    ///////////////////////////////////////   
    
    TUid controllerUid = {0x101F5023};   // This is a real Controller UID
    // The Rubik Audio Controller
    TUid formatUid = {VIDEO_FORMAT_UID};
    
    TRAPD(err, iCamcTestClient->Test_OpenFileL(
        _L("recordQCIF.3gp"),		// File
        iUiLevelCameraHandle,								//CameraHandle
        controllerUid,					//Controller UID
        formatUid,						//Format UID
        MIME_VIDEO_FORMAT,
        NO_AUDIO_FOURCC				    //Audio Type
        ));

    // Function may leave with KErrNotFound, that's ok
    if (err==KErrNotFound)
        return;
    // Otherwise, the MvruoOpenComplete should return with an error

    assertTIntsEqualL( KErrNotFound, iCamcTestClient->WaitForCallbackWithErrors() );
    
    }

void CCamcTest_1::OpenFileL_005_L()
    {
    ///////////////////////////////////////
    //   ID: CAMC_API.OPENFILEL_005
    //   Action :
    //       videoFormat ok
    //   Output : 
    //       No leave
    //   Precondition :
    //       CVideoRecorderUtility is ready
    ///////////////////////////////////////
    
    TUid controllerUid = {CONTROLLER_UID};	
    TUid formatUid = {VIDEO_FORMAT_UID};
    
    iCamcTestClient->Test_OpenFileL(
        _L("recordQCIF.3gp"),		// File
        iUiLevelCameraHandle,								//CameraHandle
        controllerUid,					//Controller UID
        formatUid,      				//Format UID
        NEW_MIME_VIDEO_FORMAT,
        NO_AUDIO_FOURCC  				//Audio Type
        );
    
    // Succeed, if it doesn't leave.
    }

void CCamcTest_1::OpenFileL_006_L()
    {
    ///////////////////////////////////////
    //  ID: CAMC_API.OPENFILEL_006
    //
    //   Action :
    //      Incorrect Video format UID
    //   Output : 
    //      OpenFile = Leaves with KErrNotSupported
    //           or MvruoOpenComplete return with an error
    //   Precondition :
    //      CVideoRecorderUtility is ready
    ///////////////////////////////////////
    
    
    TUid controllerUid = {CONTROLLER_UID};	
    TUid formatUid = {0x101f5c51};  // This is an audio Format, not supported
    // by the video controller.
    
    TRAPD(err, iCamcTestClient->Test_OpenFileL(
        _L("recordQCIF.3gp"),		// File
        iUiLevelCameraHandle,								//CameraHandle
        controllerUid,					//Controller UID
        formatUid,						//Format UID
        MIME_VIDEO_FORMAT,
        NO_AUDIO_FOURCC				    //Audio Type
        ));
    // Function may leave with KErrNotSupported, that's ok
    if (err==KErrNotSupported)
        return;
    // Otherwise, the MvruoOpenComplete should return with an error

    assertTIntsEqualL( KErrNotSupported, iCamcTestClient->WaitForCallbackWithErrors() );

    }


void CCamcTest_1::OpenFileL_008_L()
    {
    ///////////////////////////////////////
    //   ID: CAMC_API.OPENFILEL_008
    //   
    //     Action :
    //        Video Mime Type Supported
    //     Output : 
    //        OpenFile = OK
    //     Precondition :
    //        CVideoRecorderUtility is ready
    ///////////////////////////////////////
    
    TUid controllerUid = {CONTROLLER_UID};	
    TUid formatUid = {VIDEO_FORMAT_UID};
    
    iCamcTestClient->Test_OpenFileL(
        _L("recordQCIF.3gp"),		// File
        iUiLevelCameraHandle,								//CameraHandle
        controllerUid,					//Controller UID
        formatUid,      				//Format UID
        MIME_VIDEO_FORMAT,
        NO_AUDIO_FOURCC  				//Audio Type
        );
    
    // Succeed, if it doesn't leave.
    }

void CCamcTest_1::OpenFileL_009_L()
    {
    ///////////////////////////////////////
    //  ID: CAMC_API.OPENFILEL_009
    //  
    //    Action :
    //       Video Mime Type = Not Present
    //    Output : 
    //       OpenFile = OK
    //    Precondition :
    //       CVideoRecorderUtility is ready
    ///////////////////////////////////////
    
    TUid controllerUid = {CONTROLLER_UID};	
    TUid formatUid = {VIDEO_FORMAT_UID};
    
    iCamcTestClient->Test_OpenFileL(
        _L("recordQCIF.3gp"),		// File
        iUiLevelCameraHandle,								//CameraHandle
        controllerUid,					//Controller UID
        formatUid      			    	//Format UID
        );
    }

void CCamcTest_1::OpenFileL_010_L()
    {
    ///////////////////////////////////////
    //  ID: CAMC_API.OPENFILEL_010
    //
    //  Action :
    //    VideoMimeType Unsupported
    //  Output : 
    //    OpenFile Leaves
    //           or MvruoOpenComplete return with an error
    //  Precondition :
    //    CVideoRecorderUtility is ready
    ///////////////////////////////////////  
    
    
    TUid controllerUid = {CONTROLLER_UID};	
    TUid formatUid = {VIDEO_FORMAT_UID};
    
    TRAPD(err, iCamcTestClient->Test_OpenFileL(
        _L("recordQCIF.3gp"),		// File
        iUiLevelCameraHandle,								//CameraHandle
        controllerUid,					//Controller UID
        formatUid,						//Format UID
        _L8("video/3gpp"),              //Usupported type...
        NO_AUDIO_FOURCC				    //Audio Type
        ));
    
     // Function may leave with KErrNotSupported, that's ok
    if (err==KErrNotSupported)
        return;
    // Otherwise, the MvruoOpenComplete should return with an error

    assertTIntsEqualL( KErrNotSupported, iCamcTestClient->WaitForCallbackWithErrors() );
 
    }


void CCamcTest_1::OpenFileL_011_L()
    {   
    ///////////////////////////////////////
    //  ID: CAMC_API.OPENFILEL_011
    //
    //  Action :
    //    Give a valid Camera Handler
    //  Output : 
    //    OK
    //  Precondition :
    //    Camera already used by someone else
    ///////////////////////////////////////  
    
    // Create a new Camera API implementation object, if supported
    TRAPD( err, iCamera = CCamera::New2L( static_cast<MCameraObserver2&>(*this), 0 /*index*/, 0 /*Priority*/ ) );
    if ( err )
        {
        PRINT( ( _L( "CCamcTest_1::OpenFileL_011_L CCamera::New2L return code=%d" ), err ) ); 
        // Create old Camera API implementation object.
        iCamera = CCamera::NewL( static_cast<MCameraObserver&>(*this), 0 /*Index*/ );
        }
    else 
       {
       PRINT( ( _L( "CCamcTest_1::OpenFileL_011_L() using new MCameraObserver2" )) ); 
       }
    
    
    CleanupStack::PushL(iCamera);	
    
    TUid controllerUid = {CONTROLLER_UID};	
    TUid formatUid = {VIDEO_FORMAT_UID};
    
    iCamcTestClient->Test_OpenFileL(
        _L("recordQCIF.3gp"),		// File
        iCamera->Handle(),				//CameraHandle
        controllerUid,					//Controller UID
        formatUid,						//Format UID
        MIME_VIDEO_FORMAT,
        NO_AUDIO_FOURCC				    //Audio Type
        );
    
    CleanupStack::PopAndDestroy();
    
    }

void CCamcTest_1::OpenFileL_017_L()
    {
    ///////////////////////////////////////
    //   ID: CAMC_API.OPENFILEL_017
    //   Action :
    //       videoFormat ok
    //   Output : 
    //       No leave
    //   Precondition :
    //       CVideoRecorderUtility is ready
    ///////////////////////////////////////
    
    TUid controllerUid = {CONTROLLER_UID};	
    TUid formatUid = {VIDEO_FORMAT_UID_NEW};
    
    iCamcTestClient->Test_OpenFileL(
        _L("recordQCIF.3g2"),		// File
        iUiLevelCameraHandle,								//CameraHandle
        controllerUid,					//Controller UID
        formatUid,      				//Format UID
        MIME_VIDEO_FORMAT,
        NO_AUDIO_FOURCC  				//Audio Type
        );
    
    // Succeed, if it doesn't leave.
    }

void CCamcTest_1::OpenFileL_018_L()
    {
    ///////////////////////////////////////
    //   ID: CAMC_API.OPENFILEL_018
    //   Action :
    //       videoFormat ok
    //   Output : 
    //       No leave
    //   Precondition :
    //       CVideoRecorderUtility is ready
    ///////////////////////////////////////
    
    TUid controllerUid = {CONTROLLER_UID};	
    TUid formatUid = {VIDEO_FORMAT_UID_NEW};
    
    iCamcTestClient->Test_OpenFileL(
        _L("recordQCIF.3g2"),		// File
        iUiLevelCameraHandle,								//CameraHandle
        controllerUid,					//Controller UID
        formatUid,      				//Format UID
        NEW_MIME_VIDEO_FORMAT,
        NO_AUDIO_FOURCC  				//Audio Type
        );
    
    // Succeed, if it doesn't leave.
    }

void CCamcTest_1::OpenFileL_020_L()
    {
    ///////////////////////////////////////
    //   ID: CAMC_API.OPENFILEL_020
    //   Action :
    //       videoFormat ok
    //   Output : 
    //       No leave
    //   Precondition :
    //       CVideoRecorderUtility is ready
    ///////////////////////////////////////
    
    TUid controllerUid = {CONTROLLER_UID};	
    TUid formatUid = {VIDEO_FORMAT_UID_MP4};
    
    iCamcTestClient->Test_OpenFileL(
        _L("recordQCIF.mp4"),		// File
        iUiLevelCameraHandle,								//CameraHandle
        controllerUid,					//Controller UID
        formatUid,      				//Format UID
        MPEG4_MIME_VIDEO_FORMAT,
        ACC_AUDIO_FORMAT  				//Audio Type
        );
    
    // Succeed, if it doesn't leave.
    }

void CCamcTest_1::OpenFileL_021_L()
    {
    ///////////////////////////////////////
    //   ID: CAMC_API.OPENFILEL_021
    //   Action :
    //       videoFormat Unsupported
    //   Output : 
    //    OpenFile Leaves
    //           or MvruoOpenComplete return with an error
    //   Precondition :
    //       CVideoRecorderUtility is ready
    ///////////////////////////////////////
    
    TUid controllerUid = {CONTROLLER_UID};	
    TUid formatUid = {VIDEO_FORMAT_UID_MP4};
    
    TRAPD(err, iCamcTestClient->Test_OpenFileL(
        _L("recordQCIF.mp4"),		// File
        iUiLevelCameraHandle,								//CameraHandle
        controllerUid,					//Controller UID
        formatUid,      				//Format UID
        UNSUPPORTED_VIDEO_FORMAT,
        AMR_AUDIO_FORMAT  				//Audio Type
        ));

    // Function may leave with KErrNotSupported, that's ok
    if (err==KErrNotSupported)
        return;
    // Otherwise, the MvruoOpenComplete should return with an error

    assertTIntsEqualL( KErrNotSupported, iCamcTestClient->WaitForCallbackWithErrors() );

    }




// //////////////////////////////////////////////////////////////////
//                       OpenDescL tests
// //////////////////////////////////////////////////////////////////
void CCamcTest_1::OpenDesL_001_L()
    {
    ///////////////////////////////////////
    //   ID: CAMC_API.OPENDESCL_001
    //   
    //     Action :
    //        Call to openDesc
    //     Output : 
    //        OpenDesc Leaves, unsupported.
    //           or MvruoOpenComplete return with an error
    //     Precondition :
    //        CVideoRecorderUtility is ready
    /////////////////////////////////////// 
    
    TUid controllerUid = {0x101F8503};   
    TUid formatUid = {0};
    TBuf8 <100> myDescriptor;	 
    TRAPD(err,iCamcTestClient->Test_OpenDesL(
        myDescriptor,		// Descriptor
        iUiLevelCameraHandle,								//CameraHandle
        controllerUid,					//Controller UID
        formatUid,						//Format UID
        _L8("video/H263-2000"),
        KMMFFourCCCodeNULL				//Audio Type
        ));
    
    // Function may leave with KErrNotSupported, that's ok
    if (err==KErrNotSupported)
        return;
    // Otherwise, the MvruoOpenComplete should return with an error

    assertTIntsEqualL( KErrNotSupported, iCamcTestClient->WaitForCallbackWithErrors() );
 
    }

// //////////////////////////////////////////////////////////////////
//                       OpenUrlL tests
// //////////////////////////////////////////////////////////////////
void CCamcTest_1::OpenUrlL_001_L()
    {
    ///////////////////////////////////////
    //   ID: CAMC_API.OPENURLL_001
    //   
    //     Action :
    //        Call to openUrl
    //     Output : 
    //        OpenDesc Leaves, unsupported.
    //           or MvruoOpenComplete return with an error
    //  Precondition :
    //        CVideoRecorderUtility is ready
    /////////////////////////////////////// 
    
    TUid controllerUid = {0x101F8503};   
    TUid formatUid = {0};	 
    TRAPD(err,iCamcTestClient->Test_OpenUrlL(
        _L("http:\\nokia.com"),		    // Descriptor
        iUiLevelCameraHandle,								//CameraHandle
        controllerUid,					//Controller UID
        formatUid,						//Format UID
        _L8("video/H263-2000"),
        KMMFFourCCCodeNULL				//Audio Type
        ));
    
     // Function may leave with KErrNotSupported, that's ok
    if (err==KErrNotSupported)
        return;
    // Otherwise, the MvruoOpenComplete should return with an error

    assertTIntsEqualL( KErrNotSupported, iCamcTestClient->WaitForCallbackWithErrors() );
 
    }




// //////////////////////////////////////////////////////////////////
//   Dummy Camera API callback functions
// //////////////////////////////////////////////////////////////////

void CCamcTest_1::ReserveComplete(TInt aError)
    {
    RDebug::Print(_L("CCamcTest_1::ReserveComplete"));
    if (aError)
        {
        SomethingInActiveObjectIsLeaving(aError);  
        return;
        }
        
    // UI level camera is being initialized        
    if (iWaitScheduler->IsStarted())
	    {
    	 iUiLevelCamera->PowerOn();
	    }
    }

void CCamcTest_1::PowerOnComplete(TInt aError)
    {   
    RDebug::Print(_L("CCamcTest_1::PowerOnComplete"));
    if (aError)
        {
        SomethingInActiveObjectIsLeaving(aError);  
        return;
        } 
        
    // UI level camera is being initialized
    if (iWaitScheduler->IsStarted())
    	{
    	 iWaitScheduler->AsyncStop();
	    }        
	}

void CCamcTest_1::ViewFinderFrameReady(CFbsBitmap& /*aFrame*/)
    {
    }
void CCamcTest_1::ImageReady(CFbsBitmap* /*aBitmap */,HBufC8* /*aData*/,TInt aError)
    {    
    TBuf<128> str;
    str.Format(_L("CCamcTest_1::ImageReady %i"), aError);
    RDebug::Print(str);   
    }
void CCamcTest_1::FrameBufferReady(MFrameBuffer* /*aFrameBuffer*/,TInt aError )
    {
    TBuf<128> str;
    str.Format(_L("CCamcTest_1::FrameBufferReady %i"), aError);
    RDebug::Print(str);  
    }

void CCamcTest_1::SomethingInActiveObjectIsLeaving (TInt aError)
    {
    // Something in the active object is leaving
    // or a fatal error has been found.  
    
    TBuf<128> str;
    str.Format(_L("CCamcTest_1::Something In Active Object is leaving %i"), aError);
    RDebug::Print(str);   

    // Stop the active object scheduler.
    iError=aError;
    CActiveScheduler::Stop();
    }

// -----------------------------------------------------------------------------
// CCamcTest_1::HandleEvent
// MCameraObserver2 call-back handler
// -----------------------------------------------------------------------------
//
void CCamcTest_1::HandleEvent( const TECAMEvent& aEvent)
    {
    PRINT(( _L( "CCamcTest_1::HandleEvent() entering, type=%x, err=%d" ), aEvent.iEventType.iUid, aEvent.iErrorCode ));

    if ( aEvent.iEventType == KUidECamEventCameraNoLongerReserved )
        {
        PRINT(( _L( "CCamcTest_1::HandleEvent() KUidECamEventCameraNoLongerReserved" ) ));
        }
    else if ( aEvent.iEventType == KUidECamEventPowerOnComplete )
        {
        PRINT(( _L( "CCamcTest_1::HandleEvent() KUidECamEventPowerOnComplete" ) ));
        PowerOnComplete( aEvent.iErrorCode );
        }
    else if ( aEvent.iEventType == KUidECamEventReserveComplete )
       {
       PRINT(( _L( "CCamcTest_1::HandleEvent() KUidECamEventReserveComplete" ) ));
       ReserveComplete( aEvent.iErrorCode );
       }

    PRINT(( _L( "CCamcTestClient_7::HandleEvent() returning" ) ));
    }

// -----------------------------------------------------------------------------
// CCamcTest_1::ViewFinderReady
// MCameraObserver2 call-back handler
// -----------------------------------------------------------------------------
//
void CCamcTest_1::ViewFinderReady( MCameraBuffer& aCameraBuffer, TInt aError)
    {
    PRINT(( _L( "CCamcTest_1::ViewFinderReady() NOT HANDLED err=%d, NumFrames=%d" ), aError, aCameraBuffer.NumFrames() ));
    CFbsBitmap *buffer = NULL;
    if ( !aError )
        {
	    aCameraBuffer.Release();
        }
    ViewFinderFrameReady( *buffer );
    }

// -----------------------------------------------------------------------------
// CCamcTest_1::ImageBufferReady
// MCameraObserver2 call-back handler
// -----------------------------------------------------------------------------
//
void CCamcTest_1::ImageBufferReady( MCameraBuffer& aCameraBuffer, TInt aError)
    {
    PRINT(( _L( "CCamcTest_1::ImageBufferReady() BUFFER NOT HANDLED, err=%d, NumFrames=%d" ), aError, aCameraBuffer.NumFrames() ));
	
    if ( !aError )
        {
	    aCameraBuffer.Release();
        }
    ImageReady( NULL, NULL, aError );
    }
    
// -----------------------------------------------------------------------------
// CCamcTest_1::VideoBufferReady
// MCameraObserver2 call-back handler
// -----------------------------------------------------------------------------
//
void CCamcTest_1::VideoBufferReady( MCameraBuffer& aCameraBuffer, TInt aError)
    {
    PRINT(( _L( "CCamcTest_1::VideoBufferReady() entering, err=%d, NumFrames=%d" ), aError, aCameraBuffer.NumFrames() ));
    MFrameBuffer* buffer = NULL;
	
    if ( !aError )
        {
        aCameraBuffer.Release();
        }
    // Call old Camera-API observer handler
    FrameBufferReady( buffer, aError );
    
    PRINT(( _L( "CCamcTest_1::VideoBufferReady() returning" )));
    }


//
// An own static function to collect the test functions into one 
// suite of tests. The framework will run the tests and free the
// memory allocated for the test suite.
// 

MTest* CCamcTest_1::suiteL ()
    {
    // Always use NewL (Do not use NewLC) !!!
    CTestSuite *suite = CTestSuite::NewL(_L8("CCamcTest_1"));
    CleanupStack::PushL( suite );

    suite->addTestL(CTestCaller<CCamcTest_1>::NewL(_L8("CAMC_API.OPENFILEL_001"), &CCamcTest_1::OpenFileL_001_L));
    suite->addTestL(CTestCaller<CCamcTest_1>::NewL(_L8("CAMC_API.OPENFILEL_002"), &CCamcTest_1::OpenFileL_002_L));
    // OPENFILEL_003 is not a valid test case.
    suite->addTestL(CTestCaller<CCamcTest_1>::NewL(_L8("CAMC_API.OPENFILEL_004"), &CCamcTest_1::OpenFileL_004_L));
    // OPENFILEL_005 is not relevent in current configuration.
      
#ifdef __MPEG4_VIDEO_ENCODING
    suite->addTestL(CTestCaller<CCamcTest_1>::NewL(_L8("CAMC_API.OPENFILEL_005"), &CCamcTest_1::OpenFileL_005_L));
#endif
    suite->addTestL(CTestCaller<CCamcTest_1>::NewL(_L8("CAMC_API.OPENFILEL_006"), &CCamcTest_1::OpenFileL_006_L));
    suite->addTestL(CTestCaller<CCamcTest_1>::NewL(_L8("CAMC_API.OPENFILEL_008"), &CCamcTest_1::OpenFileL_008_L));
    suite->addTestL(CTestCaller<CCamcTest_1>::NewL(_L8("CAMC_API.OPENFILEL_009"), &CCamcTest_1::OpenFileL_009_L));
    suite->addTestL(CTestCaller<CCamcTest_1>::NewL(_L8("CAMC_API.OPENFILEL_010"), &CCamcTest_1::OpenFileL_010_L));
    suite->addTestL(CTestCaller<CCamcTest_1>::NewL(_L8("CAMC_API.OPENFILEL_011"), &CCamcTest_1::OpenFileL_011_L));
  
    suite->addTestL(CTestCaller<CCamcTest_1>::NewL(_L8("CAMC_API.OPENFILEL_017"), &CCamcTest_1::OpenFileL_017_L));
#ifdef __MPEG4_VIDEO_ENCODING
    suite->addTestL(CTestCaller<CCamcTest_1>::NewL(_L8("CAMC_API.OPENFILEL_018"), &CCamcTest_1::OpenFileL_018_L));   
#endif
#ifdef MP4_FILE_FORMAT_SUPPORTED
    suite->addTestL(CTestCaller<CCamcTest_1>::NewL(_L8("CAMC_API.OPENFILEL_020"), &CCamcTest_1::OpenFileL_020_L));   
    suite->addTestL(CTestCaller<CCamcTest_1>::NewL(_L8("CAMC_API.OPENFILEL_021"), &CCamcTest_1::OpenFileL_021_L));   
#endif
    
    suite->addTestL(CTestCaller<CCamcTest_1>::NewL(_L8("CAMC_API.OPENDESCL_001"), &CCamcTest_1::OpenDesL_001_L));
    suite->addTestL(CTestCaller<CCamcTest_1>::NewL(_L8("CAMC_API.OPENURLL_001"), &CCamcTest_1::OpenUrlL_001_L));
    
    CleanupStack::Pop( suite );
    return suite;
    }


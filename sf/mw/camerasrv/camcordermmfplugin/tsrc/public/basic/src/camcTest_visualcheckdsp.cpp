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
#include "camcTest_visualcheckdsp.h"


// EXTERNAL DATA STRUCTURES

// EXTERNAL FUNCTION PROTOTYPES 
 
// CONSTANTS

// MACROS

// LOCAL CONSTANTS AND MACROS

// MODULE DATA STRUCTURES

// LOCAL FUNCTION PROTOTYPES

// FORWARD DECLARATIONS


CCamcTest_visualcheckdsp::CCamcTest_visualcheckdsp ()
    {
    }



CCamcTest_visualcheckdsp::~CCamcTest_visualcheckdsp ()
    {
    delete iCamcTestClient;
    iCamcTestClient = NULL;
    }


void CCamcTest_visualcheckdsp::setUpL ()
    {
#ifdef _DEBUG
      RDebug::Print(_L("CamCTest VisualCheckDsp: SetUpL"));
#endif
      iCamcTestClient = CCamcTestClient_visualcheckdsp::NewL( );

    // Do nothing in the setUp to get Memory Failure functionality
    iWaitScheduler = new (ELeave) CActiveSchedulerWait;	

    // Create a new Camera API implementation object, if supported
    TRAPD( err, iUiLevelCamera = CCamera::New2L( static_cast<MCameraObserver2&>(*this), 0 /*index*/, 0 /*Priority*/ ) );
    if ( err )
        {
        PRINT( ( _L( "CCamcTest_visualcheckdsp::setUpL CCamera::New2L return code=%d" ), err ) ); 
        // Create old Camera API implementation object.
        iUiLevelCamera = CCamera::NewL( static_cast<MCameraObserver&>(*this), 0 /*Index*/ );
        }
    else 
       {
       PRINT( ( _L( "CCamcTest_visualcheckdsp::setUp() using new MCameraObserver2" )) ); 
       }


	iUiLevelCameraHandle = iUiLevelCamera->Handle();
	iUiLevelCamera->Reserve();
	iWaitScheduler->Start();    
    
    }


void CCamcTest_visualcheckdsp::tearDown ()
    {
#ifdef _DEBUG
    RDebug::Print(_L("CamCTest VisualCheckDsp: TearDown Start"));
#endif
    delete iCamcTestClient;

    iCamcTestClient = NULL;
    delete iWaitScheduler;
    delete iUiLevelCamera;  
	REComSession::FinalClose();
#ifdef _DEBUG
    RDebug::Print(_L("CamCTest VisualCheckDsp: TearDown Stop"));
#endif
    }


//
// Own test functions.
//

// //////////////////////////////////////////////////////////////////
//                       visual check tests
// //////////////////////////////////////////////////////////////////

void CCamcTest_visualcheckdsp::VisualCheck_001_L()
    {
    ///////////////////////////////////////
	//   ID: CAMC_API.VISUALCHECK_001
    //   Action :
    //       Format:              Default
    //       Video:               Default
    //       Video Bit-rate:      Default
    //       Video Frame-Rate:    Default
    //       Picture size:        Default
    //       Audio:               Default
    //       Audio Bit-rate:      Default
    //       Size limit (kbytes): None
    //       Length:              30s
    //       Dynamic change 1:    None
    //       Dynamic change 2:    None
    //   Output : 
    //       The video clip is ok, and inspected _visually by a (human) operator.
    //   Precondition :
    //       None
    ///////////////////////////////////////

    iCamcTestClient->Start_Active_ObjectL(K_VC_dsp_test_case_1, iUiLevelCameraHandle);
    }

void CCamcTest_visualcheckdsp::VisualCheck_002_L()
    {
    ///////////////////////////////////////
	//   ID: CAMC_API.VISUALCHECK_002
    //   Action :
    //       Format:              3gp
    //       Video:               263b
    //       Video Bit-rate:      28000
    //       Video Frame-Rate:    5
    //       Picture size:        subQCIF
    //       Audio:               AMR
    //       Audio Bit-rate:      12.2
    //       Size limit (kbytes): 95
    //       Length:              -
    //       Dynamic change 1:    None
    //       Dynamic change 2:    None
    //   Output : 
    //       The video clip is ok, and inspected _visually by a (human) operator.
    //   Precondition :
    //       None
    ///////////////////////////////////////

    iCamcTestClient->Start_Active_ObjectL(K_VC_dsp_test_case_2, iUiLevelCameraHandle);
    }

void CCamcTest_visualcheckdsp::VisualCheck_003_L()
    {
    ///////////////////////////////////////
	//   ID: CAMC_API.VISUALCHECK_003
    //   Action :
    //       Format:              3gp
    //       Video:               263b
    //       Video Bit-rate:      28000
    //       Video Frame-Rate:    7.5
    //       Picture size:        subQCIF
    //       Audio:               AMR
    //       Audio Bit-rate:      6.7
    //       Size limit (kbytes): 95
    //       Length:              -
    //       Dynamic change 1:    None
    //       Dynamic change 2:    None
    //   Output : 
    //       The video clip is ok, and inspected _visually by a (human) operator.
    //   Precondition :
    //       None
    ///////////////////////////////////////

    iCamcTestClient->Start_Active_ObjectL(K_VC_dsp_test_case_3, iUiLevelCameraHandle);
    }

void CCamcTest_visualcheckdsp::VisualCheck_004_L()
    {
    ///////////////////////////////////////
	//   ID: CAMC_API.VISUALCHECK_004
    //   Action :
    //       Format:              3gp
    //       Video:               263b
    //       Video Bit-rate:      28000
    //       Video Frame-Rate:    15
    //       Picture size:        subQCIF
    //       Audio:               AMR
    //       Audio Bit-rate:      5.15
    //       Size limit (kbytes): 250
    //       Length:              -
    //       Dynamic change 1:    None
    //       Dynamic change 2:    None
    //   Output : 
    //       The video clip is ok, and inspected _visually by a (human) operator.
    //   Precondition :
    //       None
    ///////////////////////////////////////

    iCamcTestClient->Start_Active_ObjectL(K_VC_dsp_test_case_4, iUiLevelCameraHandle);
    }

void CCamcTest_visualcheckdsp::VisualCheck_005_L()
    {
    ///////////////////////////////////////
	//   ID: CAMC_API.VISUALCHECK_005
    //   Action :
    //       Format:              3gp
    //       Video:               263b
    //       Video Bit-rate:      42000
    //       Video Frame-Rate:    5
    //       Picture size:        QCIF
    //       Audio:               AMR
    //       Audio Bit-rate:      7.4
    //       Size limit (kbytes): 95
    //       Length:              -
    //       Dynamic change 1:    None
    //       Dynamic change 2:    None
    //   Output : 
    //       The video clip is ok, and inspected _visually by a (human) operator.
    //   Precondition :
    //       None
    ///////////////////////////////////////

    iCamcTestClient->Start_Active_ObjectL(K_VC_dsp_test_case_5, iUiLevelCameraHandle);
    }

void CCamcTest_visualcheckdsp::VisualCheck_006_L()
    {
    ///////////////////////////////////////
	//   ID: CAMC_API.VISUALCHECK_006
    //   Action :
    //       Format:              3gp
    //       Video:               263b
    //       Video Bit-rate:      42000
    //       Video Frame-Rate:    7.5
    //       Picture size:        QCIF
    //       Audio:               AMR
    //       Audio Bit-rate:      4.75
    //       Size limit (kbytes): 95
    //       Length:              -
    //       Dynamic change 1:    None
    //       Dynamic change 2:    None
    //   Output : 
    //       The video clip is ok, and inspected _visually by a (human) operator.
    //   Precondition :
    //       None
    ///////////////////////////////////////

    iCamcTestClient->Start_Active_ObjectL(K_VC_dsp_test_case_6, iUiLevelCameraHandle);
    }

void CCamcTest_visualcheckdsp::VisualCheck_007_L()
    {
    ///////////////////////////////////////
	//   ID: CAMC_API.VISUALCHECK_007
    //   Action :
    //       Format:              3gp
    //       Video:               263b
    //       Video Bit-rate:      42000
    //       Video Frame-Rate:    15
    //       Picture size:        QCIF
    //       Audio:               AMR
    //       Audio Bit-rate:      12.2
    //       Size limit (kbytes): None
    //       Length:              60s
    //       Dynamic change 1:    Pause + Resume
    //       Dynamic change 2:    None
    //   Output : 
    //       The video clip is ok, and inspected _visually by a (human) operator.
    //   Precondition :
    //       None
    ///////////////////////////////////////

    iCamcTestClient->Start_Active_ObjectL(K_VC_dsp_test_case_7, iUiLevelCameraHandle);
    }

void CCamcTest_visualcheckdsp::VisualCheck_008_L()
    {
    ///////////////////////////////////////
	//   ID: CAMC_API.VISUALCHECK_008
    //   Action :
    //       Format:              3gp
    //       Video:               263b
    //       Video Bit-rate:      64000
    //       Video Frame-Rate:    7.5
    //       Picture size:        QCIF
    //       Audio:               AMR
    //       Audio Bit-rate:      12.2
    //       Size limit (kbytes): 250
    //       Length:              -
    //       Dynamic change 1:    Volume up
    //       Dynamic change 2:    Volume down
    //   Output : 
    //       The video clip is ok, and inspected _visually by a (human) operator.
    //   Precondition :
    //       None
    ///////////////////////////////////////

    iCamcTestClient->Start_Active_ObjectL(K_VC_dsp_test_case_8, iUiLevelCameraHandle);
    }

void CCamcTest_visualcheckdsp::VisualCheck_009_A_L()
    {
    ///////////////////////////////////////
	//   ID: CAMC_API.VISUALCHECK_009_A
    //   Action :
    //       Format:              3gp
    //       Video:               263b
    //       Video Bit-rate:      128000
    //       Video Frame-Rate:    15
    //       Picture size:        QCIF
    //       Audio:               AMR
    //       Audio Bit-rate:      12.2
    //       Size limit (kbytes): None
    //       Length:              5min
    //       Dynamic change 1:    Pause + Resume
    //       Dynamic change 2:    Pause + Resume
    //   Output : 
    //       The video clip is ok, and inspected _visually by a (human) operator.
    //   Precondition :
    //       None
    ///////////////////////////////////////

    iCamcTestClient->Start_Active_ObjectL(K_VC_dsp_test_case_9_a, iUiLevelCameraHandle);
    }

void CCamcTest_visualcheckdsp::VisualCheck_009_B_L()
    {
    ///////////////////////////////////////
	//   ID: CAMC_API.VISUALCHECK_009_B
    //   Action :
    //       Format:              3gp
    //       Video:               263b
    //       Video Bit-rate:      64000
    //       Video Frame-Rate:    15
    //       Picture size:        QCIF
    //       Audio:               AMR
    //       Audio Bit-rate:      12.2
    //       Size limit (kbytes): None
    //       Length:              5min
    //       Dynamic change 1:    Pause + Resume
    //       Dynamic change 2:    Pause + Resume
    //   Output : 
    //       The video clip is ok, and inspected _visually by a (human) operator.
    //   Precondition :
    //       None
    ///////////////////////////////////////

    iCamcTestClient->Start_Active_ObjectL(K_VC_dsp_test_case_9_b, iUiLevelCameraHandle);
    }

void CCamcTest_visualcheckdsp::VisualCheck_010_L()
    {
    ///////////////////////////////////////
	//   ID: CAMC_API.VISUALCHECK_010
    //   Action :
    //       Format:              3gp
    //       Video:               263b
    //       Video Bit-rate:      42000
    //       Video Frame-Rate:    7.5
    //       Picture size:        QCIF
    //       Audio:               None
    //       Audio Bit-rate:      -
    //       Size limit (kbytes): 95
    //       Length:              -
    //       Dynamic change 1:    Bit-rate 64000
    //       Dynamic change 2:    None
    //   Output : 
    //       The video clip is ok, and inspected _visually by a (human) operator.
    //   Precondition :
    //       None
    ///////////////////////////////////////

    iCamcTestClient->Start_Active_ObjectL(K_VC_dsp_test_case_10, iUiLevelCameraHandle);
    }

void CCamcTest_visualcheckdsp::VisualCheck_011_L()
    {
    ///////////////////////////////////////
	//   ID: CAMC_API.VISUALCHECK_011
    //   Action :
    //       Format:              3gp
    //       Video:               263b
    //       Video Bit-rate:      64000
    //       Video Frame-Rate:    15
    //       Picture size:        QCIF
    //       Audio:               None
    //       Audio Bit-rate:      -
    //       Size limit (kbytes): 500
    //       Length:              -
    //       Dynamic change 1:    Bit-rate 28000
    //       Dynamic change 2:    None
    //   Output : 
    //       The video clip is ok, and inspected _visually by a (human) operator.
    //   Precondition :
    //       None
    ///////////////////////////////////////

    iCamcTestClient->Start_Active_ObjectL(K_VC_dsp_test_case_11, iUiLevelCameraHandle);
    }

void CCamcTest_visualcheckdsp::VisualCheck_012_L()
    {
    ///////////////////////////////////////
	//   ID: CAMC_API.VISUALCHECK_012
    //   Action :
    //       Format:              3gp
    //       Video:               263b
    //       Video Bit-rate:      var
    //       Video Frame-Rate:    15
    //       Picture size:        subQCIF
    //       Audio:               -
    //       Audio Bit-rate:      -
    //       Size limit (kbytes): 95
    //       Length:              -
    //       Dynamic change 1:    Pause + Resume
    //       Dynamic change 2:    None
    //   Output : 
    //       The video clip is ok, and inspected _visually by a (human) operator.
    //   Precondition :
    //       None
    ///////////////////////////////////////

    iCamcTestClient->Start_Active_ObjectL(K_VC_dsp_test_case_12, iUiLevelCameraHandle);
    }

void CCamcTest_visualcheckdsp::VisualCheck_013_L()
    {
    ///////////////////////////////////////
	//   ID: CAMC_API.VISUALCHECK_013
    //   Action :
    //       Format:              Default
    //       Video:               Default
    //       Video Bit-rate:      Default
    //       Video Frame-Rate:    Default
    //       Picture size:        Default
    //       Audio:               Default
    //       Audio Bit-rate:      Default
    //       Size limit (kbytes): 50 bytes
    //       Length:              None
    //       Dynamic change 1:    None
    //       Dynamic change 2:    None
    //   Output : 
    //       The video clip is ok, and inspected _visually by a (human) operator.
    //   Precondition :
    //       None
    //
    //   !!! Obsolete Test !!!
    //
    ///////////////////////////////////////

    iCamcTestClient->Start_Active_ObjectL(K_VC_dsp_test_case_13, iUiLevelCameraHandle);
    }

void CCamcTest_visualcheckdsp::VisualCheck_014_L()
    {
    ///////////////////////////////////////
	//   ID: CAMC_API.VISUALCHECK_014
    //   Action :
    //       Format:              Default
    //       Video:               Default
    //       Video Bit-rate:      Default
    //       Video Frame-Rate:    Default
    //       Picture size:        Default
    //       Audio:               Default
    //       Audio Bit-rate:      Default
    //       Size limit (kbytes): None
    //       Length:              1s
    //       Dynamic change 1:    None
    //       Dynamic change 2:    None
    //   Output : 
    //       The video clip is ok, and inspected _visually by a (human) operator.
    //   Precondition :
    //       None
    ///////////////////////////////////////

    iCamcTestClient->Start_Active_ObjectL(K_VC_dsp_test_case_14, iUiLevelCameraHandle);
    }

void CCamcTest_visualcheckdsp::VisualCheck_015_L()
    {
    ///////////////////////////////////////
	//   ID: CAMC_API.VISUALCHECK_015
    //   Action :
    //       Format:              3gp
    //       Video:               263b
    //       Video Bit-rate:      var
    //       Video Frame-Rate:    15
    //       Picture size:        QCIF
    //       Audio:               -
    //       Audio Bit-rate:      -
    //       Size limit (kbytes): None
    //       Length:              Until disk "full"
    //       Dynamic change 1:    Pause + Resume
    //       Dynamic change 2:    Pause + Resume
    //   Output : 
    //       The video clip is ok, and inspected _visually by a (human) operator.
    //   Precondition :
    //       None
    ///////////////////////////////////////

    TRAPD(err, iCamcTestClient->Start_Active_ObjectL(K_VC_dsp_test_case_15, iUiLevelCameraHandle));
    assertTIntsEqualL( KErrDiskFull, err );
    }

void CCamcTest_visualcheckdsp::VisualCheck_016_L()
    {
    ///////////////////////////////////////
	//   ID: CAMC_API.VISUALCHECK_016
    //   Action :
    //       Format:              3gp
    //       Video:               263b
    //       Video Bit-rate:      64000
    //       Video Frame-Rate:    10
    //       Picture size:        QCIF
    //       Audio:               AMR
    //       Audio Bit-rate:      12.2
    //       Size limit (kbytes): None
    //       Length:              -
    //       Dynamic change 1:    Stop Recording + Start a new record without prepare
    //       Dynamic change 2:    None
    //   Output : 
    //       The video clip is ok, and inspected _visually by a (human) operator.
    //   Precondition :
    //       None
    ///////////////////////////////////////

    iCamcTestClient->Start_Active_ObjectL(K_VC_dsp_test_case_16, iUiLevelCameraHandle);
    }

void CCamcTest_visualcheckdsp::VisualCheck_017_L()
    {
    ///////////////////////////////////////
	//   ID: CAMC_API.VISUALCHECK_017
    //   Action :
    //       Format:              3gp
    //       Video:               MPEG4
    //       Video Bit-rate:      64000
    //       Video Frame-Rate:    15
    //       Picture size:        subQCIF
    //       Audio:               AMR
    //       Audio Bit-rate:      12.2
    //       Size limit (kbytes): None
    //       Length:              10 s
    //       Dynamic change 1:    Bitrate to 28000
    //       Dynamic change 2:    None
    //   Output : 
    //       The video clip is ok, and inspected _visually by a (human) operator.
    //   Precondition :
    //       None
    ///////////////////////////////////////

    iCamcTestClient->Start_Active_ObjectL(K_VC_dsp_test_case_17, iUiLevelCameraHandle);
    }

void CCamcTest_visualcheckdsp::VisualCheck_017_A_L()
    {
    ///////////////////////////////////////
	//   ID: CAMC_API.VISUALCHECK_017_A
    //   Action :
    //       Format:              3gp
    //       Video:               MPEG4
    //       Video Bit-rate:      64000
    //       Video Frame-Rate:    15
    //       Picture size:        subQCIF
    //       Audio:               AMR
    //       Audio Bit-rate:      12.2
    //       Size limit (kbytes): 500
    //       Length:              10 s
    //       Dynamic change 1:    Bitrate to 28000
    //       Dynamic change 2:    None
    //   Output : 
    //       The video clip is ok, and inspected _visually by a (human) operator.
    //   Precondition :
    //       None
    ///////////////////////////////////////

    iCamcTestClient->Start_Active_ObjectL(K_VC_dsp_test_case_17_a, iUiLevelCameraHandle);
    }

void CCamcTest_visualcheckdsp::VisualCheck_018_L()
    {
    ///////////////////////////////////////
	//   ID: CAMC_API.VISUALCHECK_018
    //   Action :
    //       Format:              3g2
    //       Video:               263b
    //       Video Bit-rate:      64000
    //       Video Frame-Rate:    10
    //       Picture size:        QCIF
    //       Audio:               AMR
    //       Audio Bit-rate:      12.2
    //       Size limit (kbytes): None
    //       Length:              10 s
    //       Dynamic change 1:    None
    //       Dynamic change 2:    None
    //   Output : 
    //       The video clip is ok, and inspected _visually by a (human) operator.
    //   Precondition :
    //       None
    ///////////////////////////////////////

    iCamcTestClient->Start_Active_ObjectL(K_VC_dsp_test_case_18, iUiLevelCameraHandle);
    }

void CCamcTest_visualcheckdsp::VisualCheck_021_L()
    {
    ///////////////////////////////////////
	//   ID: CAMC_API.VISUALCHECK_021
    //   Action :
    //       Format:              mp4
    //       Video:               video/mp4v-es; profile-level-id=3
    //       Video Bit-rate:      384000
    //       Video Frame-Rate:    15
    //       Picture size:        CIF
    //       Audio:               AAC
    //       Audio Bit-rate:      32000
    //       Size limit (kbytes): None
    //       Length:              60 s
    //       Dynamic change 1:    Pause + Resume
    //       Dynamic change 2:    None
    //   Output : 
    //       The video clip is ok, and inspected _visually by a (human) operator.
    //   Precondition :
    //       None
    ///////////////////////////////////////

    iCamcTestClient->Start_Active_ObjectL(K_VC_dsp_test_case_21, iUiLevelCameraHandle);
    }
// //////////////////////////////////////////////////////////////////
//   Dummy Camera API callback functions
// //////////////////////////////////////////////////////////////////
void CCamcTest_visualcheckdsp::ReserveComplete(TInt /*aError*/)
    {
    RDebug::Print(_L("CCamcTest_5::ReserveComplete"));
        
    // UI level camera is being initialized        
    if (iWaitScheduler->IsStarted())
	    {
    	 iUiLevelCamera->PowerOn();
    	 return;
	    }
    }

void CCamcTest_visualcheckdsp::PowerOnComplete(TInt /*aError*/)
    {   
    RDebug::Print(_L("CCamcTest_5::PowerOnComplete"));
        
    // UI level camera is being initialized
    if (iWaitScheduler->IsStarted())
	    {
    	 iWaitScheduler->AsyncStop();
    	 return;
    	}        
    }

void CCamcTest_visualcheckdsp::ViewFinderFrameReady(CFbsBitmap& /*aFrame*/)
    {
    }
void CCamcTest_visualcheckdsp::ImageReady(CFbsBitmap* /*aBitmap */,HBufC8* /*aData*/,TInt /*aError*/)
    {    
    }
void CCamcTest_visualcheckdsp::FrameBufferReady(MFrameBuffer* /*aFrameBuffer*/,TInt /*aError*/ )
    {
    }


// -----------------------------------------------------------------------------
// CCamcTest_visualcheckdsp::HandleEvent
// MCameraObserver2 call-back handler
// -----------------------------------------------------------------------------
//
void CCamcTest_visualcheckdsp::HandleEvent( const TECAMEvent& aEvent)
    {
    PRINT(( _L( "CCamcTest_visualcheckdsp::HandleEvent() entering, type=%x, err=%d" ), aEvent.iEventType.iUid, aEvent.iErrorCode ));

    if ( aEvent.iEventType == KUidECamEventCameraNoLongerReserved )
        {
        PRINT(( _L( "CCamcTest_visualcheckdsp::HandleEvent() KUidECamEventCameraNoLongerReserved" ) ));
        }
    else if ( aEvent.iEventType == KUidECamEventPowerOnComplete )
        {
        PRINT(( _L( "CCamcTest_visualcheckdsp::HandleEvent() KUidECamEventPowerOnComplete" ) ));
        PowerOnComplete( aEvent.iErrorCode );
        }
    else if ( aEvent.iEventType == KUidECamEventReserveComplete )
       {
       PRINT(( _L( "CCamcTest_visualcheckdsp::HandleEvent() KUidECamEventReserveComplete" ) ));
       ReserveComplete( aEvent.iErrorCode );
       }

    PRINT(( _L( "CCamcTestClient_7::HandleEvent() returning" ) ));
    }

// -----------------------------------------------------------------------------
// CCamcTest_visualcheckdsp::ViewFinderReady
// MCameraObserver2 call-back handler
// -----------------------------------------------------------------------------
//
void CCamcTest_visualcheckdsp::ViewFinderReady( MCameraBuffer& aCameraBuffer, TInt aError)
    {
    PRINT(( _L( "CCamcTest_visualcheckdsp::ViewFinderReady() NOT HANDLED err=%d, NumFrames=%d" ), aError, aCameraBuffer.NumFrames() ));
    CFbsBitmap *buffer = NULL;
    if ( !aError )
        {
	    aCameraBuffer.Release();
        }
    ViewFinderFrameReady( *buffer );
    }

// -----------------------------------------------------------------------------
// CCamcTest_visualcheckdsp::ImageBufferReady
// MCameraObserver2 call-back handler
// -----------------------------------------------------------------------------
//
void CCamcTest_visualcheckdsp::ImageBufferReady( MCameraBuffer& aCameraBuffer, TInt aError)
    {
    PRINT(( _L( "CCamcTest_visualcheckdsp::ImageBufferReady() BUFFER NOT HANDLED, err=%d, NumFrames=%d" ), aError, aCameraBuffer.NumFrames() ));
	
    if ( !aError )
        {
	    aCameraBuffer.Release();
        }
    ImageReady( NULL, NULL, aError );
    }
    
// -----------------------------------------------------------------------------
// CCamcTest_visualcheckdsp::VideoBufferReady
// MCameraObserver2 call-back handler
// -----------------------------------------------------------------------------
//
void CCamcTest_visualcheckdsp::VideoBufferReady( MCameraBuffer& aCameraBuffer, TInt aError)
    {
    PRINT(( _L( "CCamcTest_visualcheckdsp::VideoBufferReady() entering, err=%d, NumFrames=%d" ), aError, aCameraBuffer.NumFrames() ));
    MFrameBuffer* buffer = NULL;
	
    if ( !aError )
        {
        aCameraBuffer.Release();
        }
    // Call old Camera-API observer handler
    FrameBufferReady( buffer, aError );
    
    PRINT(( _L( "CCamcTest_visualcheckdsp::VideoBufferReady() returning" )));
    }






//
// An own static function to collect the test functions into one 
// suite of tests. The framework will run the tests and free the
// memory allocated for the test suite.
// 

MTest* CCamcTest_visualcheckdsp::suiteL ()
    {
    // Always use NewL (Do not use NewLC) !!!
    CTestSuite *suite = CTestSuite::NewL(_L8("CCamcTest_visualcheckdsp"));
    CleanupStack::PushL( suite );

    suite->addTestL(CTestCaller<CCamcTest_visualcheckdsp>::NewL(_L8("CAMC_API.VISUALCHECK_001"), &VisualCheck_001_L));
    suite->addTestL(CTestCaller<CCamcTest_visualcheckdsp>::NewL(_L8("CAMC_API.VISUALCHECK_002"), &VisualCheck_002_L));
    suite->addTestL(CTestCaller<CCamcTest_visualcheckdsp>::NewL(_L8("CAMC_API.VISUALCHECK_003"), &VisualCheck_003_L));
    suite->addTestL(CTestCaller<CCamcTest_visualcheckdsp>::NewL(_L8("CAMC_API.VISUALCHECK_004"), &VisualCheck_004_L));
    suite->addTestL(CTestCaller<CCamcTest_visualcheckdsp>::NewL(_L8("CAMC_API.VISUALCHECK_005"), &VisualCheck_005_L));
    suite->addTestL(CTestCaller<CCamcTest_visualcheckdsp>::NewL(_L8("CAMC_API.VISUALCHECK_006"), &VisualCheck_006_L));
    suite->addTestL(CTestCaller<CCamcTest_visualcheckdsp>::NewL(_L8("CAMC_API.VISUALCHECK_007"), &VisualCheck_007_L));
    suite->addTestL(CTestCaller<CCamcTest_visualcheckdsp>::NewL(_L8("CAMC_API.VISUALCHECK_008"), &VisualCheck_008_L));
    suite->addTestL(CTestCaller<CCamcTest_visualcheckdsp>::NewL(_L8("CAMC_API.VISUALCHECK_009_A"), &VisualCheck_009_A_L));
    //Old Test VisualCheck_009
    //suite->addTestL(CTestCaller<CCamcTest_visualcheckdsp>::NewL(_L8("CAMC_API.VISUALCHECK_009_B"), &VisualCheck_009_B_L));
    suite->addTestL(CTestCaller<CCamcTest_visualcheckdsp>::NewL(_L8("CAMC_API.VISUALCHECK_010"), &VisualCheck_010_L));
    suite->addTestL(CTestCaller<CCamcTest_visualcheckdsp>::NewL(_L8("CAMC_API.VISUALCHECK_011"), &VisualCheck_011_L));
    suite->addTestL(CTestCaller<CCamcTest_visualcheckdsp>::NewL(_L8("CAMC_API.VISUALCHECK_012"), &VisualCheck_012_L));
    // Test 113 considered as obsolete.
    //suite->addTestL(CTestCaller<CCamcTest_visualcheckdsp>::NewL(_L8("CAMC_API.VISUALCHECK_013"), &VisualCheck_013_L));
    suite->addTestL(CTestCaller<CCamcTest_visualcheckdsp>::NewL(_L8("CAMC_API.VISUALCHECK_014"), &VisualCheck_014_L));
    suite->addTestL(CTestCaller<CCamcTest_visualcheckdsp>::NewL(_L8("CAMC_API.VISUALCHECK_016"), &VisualCheck_016_L));

#if !( defined (__WINS__) || defined (__WINSCW__) )
 //   suite->addTestL(CTestCaller<CCamcTest_visualcheckdsp>::NewL(_L8("CAMC_API.VISUALCHECK_015"), &VisualCheck_015_L));
#endif   

#ifdef __MPEG4_VIDEO_ENCODING
//    suite->addTestL(CTestCaller<CCamcTest_visualcheckdsp>::NewL(_L8("CAMC_API.VISUALCHECK_017"), &VisualCheck_017_L));
    suite->addTestL(CTestCaller<CCamcTest_visualcheckdsp>::NewL(_L8("CAMC_API.VISUALCHECK_017_A"), &VisualCheck_017_A_L));
#endif
#ifdef MP4_FILE_FORMAT_SUPPORTED
    suite->addTestL(CTestCaller<CCamcTest_visualcheckdsp>::NewL(_L8("CAMC_API.VISUALCHECK_021"), &VisualCheck_021_L));
#endif

    CleanupStack::Pop( suite );    
    return suite;
    }


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
#include "camcTest_visualcheck.h"
#include <bldvariant.hrh>


// EXTERNAL DATA STRUCTURES

// EXTERNAL FUNCTION PROTOTYPES 
 
// CONSTANTS

// MACROS

// LOCAL CONSTANTS AND MACROS

// MODULE DATA STRUCTURES

// LOCAL FUNCTION PROTOTYPES

// FORWARD DECLARATIONS


CCamcTest_visualcheck::CCamcTest_visualcheck ()
    {
    }



CCamcTest_visualcheck::~CCamcTest_visualcheck ()
    {
    delete iCamcTestClient;
    iCamcTestClient = NULL;
    }


void CCamcTest_visualcheck::setUpL ()
    {
#ifdef _DEBUG
      RDebug::Print(_L("CamCTest VisualCheck: SetUpL"));
#endif
      iCamcTestClient = CCamcTestClient_visualcheck::NewL( );
    // Do nothing in the setUp to get Memory Failure functionality
    }


void CCamcTest_visualcheck::tearDown ()
    {
#ifdef _DEBUG
    RDebug::Print(_L("CamCTest VisualCheck: TearDown Start"));
#endif
    delete iCamcTestClient;
#ifdef _DEBUG
    RDebug::Print(_L("CamCTest VisualCheck: TearDown Stop"));
#endif
    iCamcTestClient = NULL;
	REComSession::FinalClose();
    }


//
// Own test functions.
//

// //////////////////////////////////////////////////////////////////
//           Visual check tests for NON HW accelerated encoder
// //////////////////////////////////////////////////////////////////

void CCamcTest_visualcheck::VisualCheck_101_L()
    {
    ///////////////////////////////////////
	//   ID: CAMC_API.VISUALCHECK_101
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

    iCamcTestClient->Start_Active_ObjectL(K_VC_test_case_101);
    }

void CCamcTest_visualcheck::VisualCheck_102_L()
    {
    ///////////////////////////////////////
	//   ID: CAMC_API.VISUALCHECK_102
    //   Action :
    //       Format:              3gp
    //       Video:               263b
    //       Video Bit-rate:      28000
    //       Video Frame-Rate:    3
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

    iCamcTestClient->Start_Active_ObjectL(K_VC_test_case_102);
    }

void CCamcTest_visualcheck::VisualCheck_103_L()
    {
    ///////////////////////////////////////
	//   ID: CAMC_API.VISUALCHECK_103
    //   Action :
    //       Format:              3gp
    //       Video:               263b
    //       Video Bit-rate:      28000
    //       Video Frame-Rate:    5
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

    iCamcTestClient->Start_Active_ObjectL(K_VC_test_case_103);
    }

void CCamcTest_visualcheck::VisualCheck_104_L()
    {
    ///////////////////////////////////////
	//   ID: CAMC_API.VISUALCHECK_104
    //   Action :
    //       Format:              3gp
    //       Video:               263b
    //       Video Bit-rate:      28000
    //       Video Frame-Rate:    10
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

    iCamcTestClient->Start_Active_ObjectL(K_VC_test_case_104);
    }

void CCamcTest_visualcheck::VisualCheck_105_L()
    {
    ///////////////////////////////////////
	//   ID: CAMC_API.VISUALCHECK_105
    //   Action :
    //       Format:              3gp
    //       Video:               263b
    //       Video Bit-rate:      42000
    //       Video Frame-Rate:    1
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

    iCamcTestClient->Start_Active_ObjectL(K_VC_test_case_105);
    }

void CCamcTest_visualcheck::VisualCheck_106_L()
    {
    ///////////////////////////////////////
	//   ID: CAMC_API.VISUALCHECK_106
    //   Action :
    //       Format:              3gp
    //       Video:               263b
    //       Video Bit-rate:      42000
    //       Video Frame-Rate:    3
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

    iCamcTestClient->Start_Active_ObjectL(K_VC_test_case_106);
    }

void CCamcTest_visualcheck::VisualCheck_107_L()
    {
    ///////////////////////////////////////
	//   ID: CAMC_API.VISUALCHECK_107
    //   Action :
    //       Format:              3gp
    //       Video:               263b
    //       Video Bit-rate:      42000
    //       Video Frame-Rate:    5
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

    iCamcTestClient->Start_Active_ObjectL(K_VC_test_case_107);
    }

void CCamcTest_visualcheck::VisualCheck_108_A_L()
    {
    ///////////////////////////////////////
	//   ID: CAMC_API.VISUALCHECK_108_A
    //   Action :
    //       Format:              3gp
    //       Video:               263b
    //       Video Bit-rate:      128000
    //       Video Frame-Rate:    8
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

    iCamcTestClient->Start_Active_ObjectL(K_VC_test_case_108_a);
    }

void CCamcTest_visualcheck::VisualCheck_108_B_L()
    {
    ///////////////////////////////////////
	//   ID: CAMC_API.VISUALCHECK_108_B
    //   Action :
    //       Format:              3gp
    //       Video:               263b
    //       Video Bit-rate:      64000
    //       Video Frame-Rate:    1
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

    iCamcTestClient->Start_Active_ObjectL(K_VC_test_case_108_b);
    }

void CCamcTest_visualcheck::VisualCheck_109_L()
    {
    ///////////////////////////////////////
	//   ID: CAMC_API.VISUALCHECK_109_B
    //   Action :
    //       Format:              3gp
    //       Video:               263b
    //       Video Bit-rate:      64000
    //       Video Frame-Rate:    3
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

    iCamcTestClient->Start_Active_ObjectL(K_VC_test_case_109);
    }

void CCamcTest_visualcheck::VisualCheck_110_L()
    {
    ///////////////////////////////////////
	//   ID: CAMC_API.VISUALCHECK_110
    //   Action :
    //       Format:              3gp
    //       Video:               263b
    //       Video Bit-rate:      42000
    //       Video Frame-Rate:    5
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

    iCamcTestClient->Start_Active_ObjectL(K_VC_test_case_110);
    }

void CCamcTest_visualcheck::VisualCheck_111_L()
    {
    ///////////////////////////////////////
	//   ID: CAMC_API.VISUALCHECK_111
    //   Action :
    //       Format:              3gp
    //       Video:               263b
    //       Video Bit-rate:      64000
    //       Video Frame-Rate:    5
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

    iCamcTestClient->Start_Active_ObjectL(K_VC_test_case_111);
    }

void CCamcTest_visualcheck::VisualCheck_112_L()
    {
    ///////////////////////////////////////
	//   ID: CAMC_API.VISUALCHECK_112
    //   Action :
    //       Format:              3gp
    //       Video:               263b
    //       Video Bit-rate:      var
    //       Video Frame-Rate:    5
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

    iCamcTestClient->Start_Active_ObjectL(K_VC_test_case_112);
    }

void CCamcTest_visualcheck::VisualCheck_113_L()
    {
    ///////////////////////////////////////
	//   ID: CAMC_API.VISUALCHECK_113
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

    iCamcTestClient->Start_Active_ObjectL(K_VC_test_case_113);
    }

void CCamcTest_visualcheck::VisualCheck_114_L()
    {
    ///////////////////////////////////////
	//   ID: CAMC_API.VISUALCHECK_114
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

    iCamcTestClient->Start_Active_ObjectL(K_VC_test_case_114);
    }

void CCamcTest_visualcheck::VisualCheck_115_L()
    {
    ///////////////////////////////////////
	//   ID: CAMC_API.VISUALCHECK_115
    //   Action :
    //       Format:              3gp
    //       Video:               263b
    //       Video Bit-rate:      var
    //       Video Frame-Rate:    5
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

    TRAPD(err, iCamcTestClient->Start_Active_ObjectL(K_VC_test_case_115));
    assertTIntsEqualL( KErrDiskFull, err );
    }

void CCamcTest_visualcheck::VisualCheck_116_L()
    {
    ///////////////////////////////////////
	//   ID: CAMC_API.VISUALCHECK_116
    //   Action :
    //       Format:              3gp
    //       Video:               263b
    //       Video Bit-rate:      64000
    //       Video Frame-Rate:    5
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

    iCamcTestClient->Start_Active_ObjectL(K_VC_test_case_116);
    }

void CCamcTest_visualcheck::VisualCheck_117_L()
    {
    ///////////////////////////////////////
	//   ID: CAMC_API.VISUALCHECK_117
    //   Action :
    //       Format:              3g2
    //       Video:               263b
    //       Video Bit-rate:      64000
    //       Video Frame-Rate:    5
    //       Picture size:        QCIF
    //       Audio:               -
    //       Audio Bit-rate:      -
    //       Size limit (kbytes): 95
    //       Length:              -
    //       Dynamic change 1:    None
    //       Dynamic change 2:    None
    //   Output : 
    //       The video clip is ok, and inspected _visually by a (human) operator.
    //   Precondition :
    //       None
    ///////////////////////////////////////

    iCamcTestClient->Start_Active_ObjectL(K_VC_test_case_117);
    }

void CCamcTest_visualcheck::VisualCheck_118_L()
    {
    ///////////////////////////////////////
	//   ID: CAMC_API.VISUALCHECK_118
    //   Action :
    //       Format:              3g2
    //       Video:               MPEG4
    //       Video Bit-rate:      64000
    //       Video Frame-Rate:    5
    //       Picture size:        QCIF
    //       Audio:               AMR
    //       Audio Bit-rate:      12.2
    //       Size limit (kbytes): None
    //       Length:              5min
    //       Dynamic change 1:    Pause + resume
    //       Dynamic change 2:    Pause + resume
    //   Output : 
    //       The video clip is ok, and inspected _visually by a (human) operator.
    //   Precondition :
    //       None
    ///////////////////////////////////////

    iCamcTestClient->Start_Active_ObjectL(K_VC_test_case_118);
    }

void CCamcTest_visualcheck::VisualCheck_119_L()
    {
    ///////////////////////////////////////
	//   ID: CAMC_API.VISUALCHECK_119
    //   Action :
    //       Format:              3gp
    //       Video:               MPEG4
    //       Video Bit-rate:      64000
    //       Video Frame-Rate:    7
    //       Picture size:        QCIF
    //       Audio:               AMR
    //       Audio Bit-rate:      5,15
    //       Size limit (kbytes): 95
    //       Length:              -
    //       Dynamic change 1:    None
    //       Dynamic change 2:    None
    //   Output : 
    //       The video clip is ok, and inspected _visually by a (human) operator.
    //   Precondition :
    //       None
    ///////////////////////////////////////

    iCamcTestClient->Start_Active_ObjectL(K_VC_test_case_119);
    }

void CCamcTest_visualcheck::VisualCheck_120_L()
    {
    ///////////////////////////////////////
	//   ID: CAMC_API.VISUALCHECK_120
    //   Action :
    //       Format:              3g2
    //       Video:               MPEG4
    //       Video Bit-rate:      64000
    //       Video Frame-Rate:    10
    //       Picture size:        subQCIF
    //       Audio:               -
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

    iCamcTestClient->Start_Active_ObjectL(K_VC_test_case_120);
    }

void CCamcTest_visualcheck::VisualCheck_121_L()
    {
    ///////////////////////////////////////
	//   ID: CAMC_API.VISUALCHECK_121
    //   Action :
    //       Format:              mp4
    //       Video:               video/mp4v-es; profile-level-id=3
    //       Video Bit-rate:      384000
    //       Video Frame-Rate:    15
    //       Picture size:        CIF
    //       Audio:               AAC
    //       Audio Bit-rate:      None
    //       Size limit (kbytes): None
    //       Length:              60s
    //       Dynamic change 1:    Pause + Resume
    //       Dynamic change 2:    None
    //   Output : 
    //       The video clip is ok, and inspected _visually by a (human) operator.
    //   Precondition :
    //       None
    ///////////////////////////////////////

    iCamcTestClient->Start_Active_ObjectL(K_VC_test_case_121);
    }



//
// An own static function to collect the test functions into one 
// suite of tests. The framework will run the tests and free the
// memory allocated for the test suite.
// 

MTest* CCamcTest_visualcheck::suiteL ()
    {
    // Always use NewL (Do not use NewLC) !!!
    CTestSuite *suite = CTestSuite::NewL(_L8("CCamcTest_visualcheck"));
    CleanupStack::PushL( suite );

    suite->addTestL(CTestCaller<CCamcTest_visualcheck>::NewL(_L8("CAMC_API.VISUALCHECK_101"), &VisualCheck_101_L));
    suite->addTestL(CTestCaller<CCamcTest_visualcheck>::NewL(_L8("CAMC_API.VISUALCHECK_102"), &VisualCheck_102_L));
    suite->addTestL(CTestCaller<CCamcTest_visualcheck>::NewL(_L8("CAMC_API.VISUALCHECK_103"), &VisualCheck_103_L));
    suite->addTestL(CTestCaller<CCamcTest_visualcheck>::NewL(_L8("CAMC_API.VISUALCHECK_104"), &VisualCheck_104_L));
    suite->addTestL(CTestCaller<CCamcTest_visualcheck>::NewL(_L8("CAMC_API.VISUALCHECK_105"), &VisualCheck_105_L));
    suite->addTestL(CTestCaller<CCamcTest_visualcheck>::NewL(_L8("CAMC_API.VISUALCHECK_106"), &VisualCheck_106_L));
    suite->addTestL(CTestCaller<CCamcTest_visualcheck>::NewL(_L8("CAMC_API.VISUALCHECK_107"), &VisualCheck_107_L));
    suite->addTestL(CTestCaller<CCamcTest_visualcheck>::NewL(_L8("CAMC_API.VISUALCHECK_108_A"), &VisualCheck_108_A_L));
    //Old Test VisualCheck_108
    //suite->addTestL(CTestCaller<CCamcTest_visualcheck>::NewL(_L8("CAMC_API.VISUALCHECK_108_B"), &VisualCheck_108_B_L));
    suite->addTestL(CTestCaller<CCamcTest_visualcheck>::NewL(_L8("CAMC_API.VISUALCHECK_109"), &VisualCheck_109_L));
    suite->addTestL(CTestCaller<CCamcTest_visualcheck>::NewL(_L8("CAMC_API.VISUALCHECK_110"), &VisualCheck_110_L));
    suite->addTestL(CTestCaller<CCamcTest_visualcheck>::NewL(_L8("CAMC_API.VISUALCHECK_111"), &VisualCheck_111_L));
    suite->addTestL(CTestCaller<CCamcTest_visualcheck>::NewL(_L8("CAMC_API.VISUALCHECK_112"), &VisualCheck_112_L));
    // Test 113 considered as obsolete.
    //  suite->addTestL(CTestCaller<CCamcTest_visualcheck>::NewL(_L8("CAMC_API.VISUALCHECK_113"), &VisualCheck_113_L));
    suite->addTestL(CTestCaller<CCamcTest_visualcheck>::NewL(_L8("CAMC_API.VISUALCHECK_114"), &VisualCheck_114_L));
  
#if !( defined (__WINS__) || defined (__WINSCW__) )
     suite->addTestL(CTestCaller<CCamcTest_visualcheck>::NewL(_L8("CAMC_API.VISUALCHECK_115"), &VisualCheck_115_L));
#endif   
     
     suite->addTestL(CTestCaller<CCamcTest_visualcheck>::NewL(_L8("CAMC_API.VISUALCHECK_116"), &VisualCheck_116_L));

     suite->addTestL(CTestCaller<CCamcTest_visualcheck>::NewL(_L8("CAMC_API.VISUALCHECK_117"), &VisualCheck_117_L));
     
#ifdef __MPEG4_VIDEO_ENCODING
    suite->addTestL(CTestCaller<CCamcTest_visualcheck>::NewL(_L8("CAMC_API.VISUALCHECK_118"), &VisualCheck_118_L));
#endif

#ifdef __MPEG4_VIDEO_ENCODING
    suite->addTestL(CTestCaller<CCamcTest_visualcheck>::NewL(_L8("CAMC_API.VISUALCHECK_119"), &VisualCheck_119_L));
#endif

#ifdef __MPEG4_VIDEO_ENCODING
    suite->addTestL(CTestCaller<CCamcTest_visualcheck>::NewL(_L8("CAMC_API.VISUALCHECK_120"), &VisualCheck_120_L));
#endif  
    
#ifdef MP4_FILE_FORMAT_SUPPORTED
    suite->addTestL(CTestCaller<CCamcTest_visualcheck>::NewL(_L8("CAMC_API.VISUALCHECK_121"), &VisualCheck_121_L));
#endif

    CleanupStack::Pop( suite );
    return suite;
    }


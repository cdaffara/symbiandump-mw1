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
#include "camcTest_7.h"

// EXTERNAL DATA STRUCTURES

// EXTERNAL FUNCTION PROTOTYPES 
 
// CONSTANTS

// MACROS

// LOCAL CONSTANTS AND MACROS

// MODULE DATA STRUCTURES

// LOCAL FUNCTION PROTOTYPES

// FORWARD DECLARATIONS

///////////////////////////////////////////////////////////////////
// Tests for Cooperation with Camera API
//
///////////////////////////////////////////////////////////////////

CCamcTest_7::CCamcTest_7 ()
    {
    }



CCamcTest_7::~CCamcTest_7 ()
    {
    delete iCamcTestClient;
    iCamcTestClient = NULL;
    }


void CCamcTest_7::setUpL ()
    {
    iCamcTestClient = CCamcTestClient_7::NewL( );
    }


void CCamcTest_7::tearDown ()
    {   
    delete iCamcTestClient;
    iCamcTestClient = NULL;
	REComSession::FinalClose();
    }


//
// Own test functions.
//

void CCamcTest_7::OpenFileL_007_L()
    {
    
    ///////////////////////////////////////
    //  ID: CAMC_API.OPENFILEL_007
    //
    //   Action :
    //      Video Format UID = NULL
	//   Output : 
    //      Recording works, Doesn't leave.
    //   Precondition :
    //      CVideoRecorderUtility is ready
    /////////////////////////////////////// 
    
    iCamcTestClient->Start_Active_ObjectL( K_Tc7_OpenFileWithVideoFormatNULL );
    }

void CCamcTest_7::OpenFileL_013_L()
    {
    ///////////////////////////////////////
	//   ID: CAMC_API.OPENFILE_13
    //   Action :
    //       Application release the camera while Camcorder still
    //       records
    //   Output : 
    //       Recording continues till the end.
    //   Precondition :
    //       Camera Reserved, CamC recording
    ///////////////////////////////////////
    
    iCamcTestClient->Start_Active_ObjectL(K_Tc7_ReleaseCameraWhileRecording);
 
    }

void CCamcTest_7::OpenFileL_014_L()
    {
    ///////////////////////////////////////
	//   ID: CAMC_API.OPENFILE_014
    //   Action :
    //       Application powers off the camera while Camcorder still
    //       records
    //   Output : 
    //       Recording stops with KErrNotReady.
    //   Precondition :
    //       Camera Reserved, CamC recording
    ///////////////////////////////////////
    
    TRAPD(err, 
        iCamcTestClient->Start_Active_ObjectL( K_Tc7_PowerOffCameraWhileRecording ) );
    assertTIntsEqualL( KErrNotReady, err );

    }

void CCamcTest_7::OpenFileL_015_L()
    {
    ///////////////////////////////////////
	//   ID: CAMC_API.OPENFILE_015
    //   Action :
    //       Camera is used by an other application,
    //       Call to OpenFileL()
    //   Output : 
    //       Should leave.
    //   Precondition :
    //       Camera Reserved by an other application.
    ///////////////////////////////////////
    
    TRAPD(err,iCamcTestClient->Start_Active_ObjectL( K_Tc7_OpenFileWhenCameraIsNotAvailable ));
    assertTIntsEqualL( KErrInUse, err );
    
    }

void CCamcTest_7::Close_006_L()
    {
    ///////////////////////////////////////
	//   ID: CAMC_API.CLOSE_006
    //   Action :
    //       Application release the camera while Camcorder still
    //       records
    //       Camcorder is Closed
    //   Output : 
    //       The Camera can be used by somebody else.
    //   Precondition :
    //       Camera Reserved, CamC recording
    ///////////////////////////////////////
    
    iCamcTestClient->Start_Active_ObjectL( K_Tc7_ReleaseCameraCloseRecorderUseCamera);

    
    // Succeed, if it doesn't leave.
    }
//
// An own static function to collect the test functions into one 
// suite of tests. The framework will run the tests and free the
// memory allocated for the test suite.
// 
MTest* CCamcTest_7::suiteL ()
    {
    // Always use NewL (Do not use NewLC) !!!
    CTestSuite *suite = CTestSuite::NewL(_L8("CCamcTest_7"));   
    CleanupStack::PushL( suite );
    
    suite->addTestL(CTestCaller<CCamcTest_7>::NewL(_L8("CAMC_API.OPENFILEL_007"), &CCamcTest_7::OpenFileL_007_L));
    suite->addTestL(CTestCaller<CCamcTest_7>::NewL(_L8("CAMC_API.OPENFILEL_013"), &CCamcTest_7::OpenFileL_013_L));
    suite->addTestL(CTestCaller<CCamcTest_7>::NewL(_L8("CAMC_API.OPENFILEL_014"), &CCamcTest_7::OpenFileL_014_L));
    suite->addTestL(CTestCaller<CCamcTest_7>::NewL(_L8("CAMC_API.OPENFILEL_015"), &CCamcTest_7::OpenFileL_015_L));
// Removed from MuDo tests
//    suite->addTestL(CTestCaller<CCamcTest_7>::NewL(_L8("CAMC_API.CLOSE_006"), &CCamcTest_7::Close_006_L));

    CleanupStack::Pop( suite );
    return suite;
    }


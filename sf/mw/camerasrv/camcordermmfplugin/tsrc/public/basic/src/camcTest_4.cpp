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
#include "camcTest_4.h"
#include "camcTest.h"

// EXTERNAL DATA STRUCTURES

// EXTERNAL FUNCTION PROTOTYPES 
 
// CONSTANTS

// MACROS

// LOCAL CONSTANTS AND MACROS

// MODULE DATA STRUCTURES

// LOCAL FUNCTION PROTOTYPES

// FORWARD DECLARATIONS


///////////////////////////////////////////////////////////////////
// Automated API Tests 
//
///////////////////////////////////////////////////////////////////

CCamcTest_4::CCamcTest_4 ()
    {
    }



CCamcTest_4::~CCamcTest_4 ()
    {
    delete iCamcTestClient;
    iCamcTestClient = NULL;
    }


void CCamcTest_4::setUpL ()
    {
    iCamcTestClient = CCamcTestClient_4::NewL( );

    iWaitScheduler = new (ELeave) CActiveSchedulerWait;	

    // Create a new Camera API implementation object, if supported
    TRAPD( err, iUiLevelCamera = CCamera::New2L( static_cast<MCameraObserver2&>(*this), 0 /*index*/, 0 /*Priority*/ ) );
    if ( err )
        {
        PRINT( ( _L( "CCamcTest_4::setUpL CCamera::New2L return code=%d" ), err ) ); 
        // Create old Camera API implementation object.
        iUiLevelCamera = CCamera::NewL( static_cast<MCameraObserver&>(*this), 0 /*Index*/ );
        }
    else 
       {
       PRINT( ( _L( "CCamcTest_4::setUp() using new MCameraObserver2" )) ); 
       }

	iUiLevelCameraHandle = iUiLevelCamera->Handle();
	iUiLevelCamera->Reserve();
	iWaitScheduler->Start();       
    }


void CCamcTest_4::tearDown ()
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
//                       RecordTimeAvailableL tests
// //////////////////////////////////////////////////////////////////

// Allows a 10% margin for the filesize
#define TC4_ALLOWED_MARGIN ( TC4_FILE_SIZE_LIMIT * 0.1 )

void CCamcTest_4::RecordTimeAvailableL_001_L()
    {
    ///////////////////////////////////////
	//   ID: CAMC_API.RECORDTIMEAVAILABLEL_001
    //   Action :
    //       Ask time available frequently using timer
    //   Output : 
    //       Value is decreasing all the time
    //   Precondition :
    //       File Size limit set.
    //       Recording
    ///////////////////////////////////////


    iCamcTestClient->Start_Active_ObjectL( 
        CCamcTestClient_4::K4RecordTimeAvailSizeLimit, iUiLevelCameraHandle );

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

    if ( ( filesize > TC4_FILE_SIZE_LIMIT + TC4_ALLOWED_MARGIN ) ||
        (filesize < TC4_FILE_SIZE_LIMIT - TC4_ALLOWED_MARGIN ) )
        {
        assertL(0);
        }
    }


void CCamcTest_4::RecordTimeAvailableL_002_L()
    {
    ///////////////////////////////////////
	//   ID: CAMC_API.RECORDTIMEAVAILABLEL_002
    //   Action :
    //       Ask time available frequently using timer
    //   Output : 
    //       Value is decreasing all the time
    //   Precondition :
    //       File Size limit is NOT set.
    //       Recording
    ///////////////////////////////////////

    iCamcTestClient->Start_Active_ObjectL( 
        CCamcTestClient_4::K4RecordTimeAvailNoSizeLimit, iUiLevelCameraHandle  );

    }

void CCamcTest_4::RecordTimeAvailableL_003_L()
    {
    ///////////////////////////////////////
	//   ID: CAMC_API.RECORDTIMEAVAILABLEL_003
    //   Action :
    //       Not recording, size limit set
    //       Asking RecordTimeAvailable()
    //   Output : 
    //       Value is proportional to size limit set
    //   Precondition :
    //       File Size limit is set.
    ///////////////////////////////////////

    iCamcTestClient->Start_Active_ObjectL( 
        CCamcTestClient_4::K4RecordTimeAvailNotRecordingSizeLimit, iUiLevelCameraHandle  );

    }

void CCamcTest_4::RecordTimeAvailableL_004_L()
    {
    ///////////////////////////////////////
	//   ID: CAMC_API.RECORDTIMEAVAILABLEL_004
    //   Action :    
    //       Asking RecordTimeAvailable()
    //   Output : 
    //       Value is acceptable
    //   Precondition :    
    //       Not recording, size limit NOT set
    ///////////////////////////////////////
    
    iCamcTestClient->Start_Active_ObjectL( 
        CCamcTestClient_4::K4RecordTimeAvailNotRecordingNoSizeLimit, iUiLevelCameraHandle  );

    }

void CCamcTest_4::RecordTimeAvailableL_006_L()
    {
    ///////////////////////////////////////
	//   ID: CAMC_API.RECORDTIMEAVAILABLEL_006
    //   Action :    
    //       Asking RecordTimeAvailable(), every seconds,
    //       and keep in memory last value for Time Remaining
    //   Output : 
    //       Value is acceptable. Last value of Time Remaining <= 5 sec.
    //   Precondition :    
    //       Recording, size limit NOT set
    ///////////////////////////////////////
    
    TRAPD(err, iCamcTestClient->Start_Active_ObjectL( 
        CCamcTestClient_4::K4RecordTimeAvailUntilDiskFull, iUiLevelCameraHandle  ));
    assertTIntsEqualL( KErrDiskFull, err );

    }
//
// An own static function to collect the test functions into one 
// suite of tests. The framework will run the tests and free the
// memory allocated for the test suite.
// 

// //////////////////////////////////////////////////////////////////
//   Dummy Camera API callback functions
// //////////////////////////////////////////////////////////////////
void CCamcTest_4::ReserveComplete(TInt /*aError*/)
    {
    RDebug::Print(_L("CCamcTest_4::ReserveComplete"));
        
    // UI level camera is being initialized        
    if (iWaitScheduler->IsStarted())
	    {
    	 iUiLevelCamera->PowerOn();
    	 return;
	    }
    }

void CCamcTest_4::PowerOnComplete(TInt /*aError*/)
    {   
    RDebug::Print(_L("CCamcTest_4::PowerOnComplete"));
        
    // UI level camera is being initialized
    if (iWaitScheduler->IsStarted())
	    {
    	 iWaitScheduler->AsyncStop();
    	 return;
    	}        
    }

void CCamcTest_4::ViewFinderFrameReady(CFbsBitmap& /*aFrame*/)
    {
    }
void CCamcTest_4::ImageReady(CFbsBitmap* /*aBitmap */,HBufC8* /*aData*/,TInt /*aError*/)
    {    
    }
void CCamcTest_4::FrameBufferReady(MFrameBuffer* /*aFrameBuffer*/,TInt /*aError*/ )
    {
    }

// -----------------------------------------------------------------------------
// CCamcTest_4::HandleEvent
// MCameraObserver2 call-back handler
// -----------------------------------------------------------------------------
//
void CCamcTest_4::HandleEvent( const TECAMEvent& aEvent)
    {
    PRINT(( _L( "CCamcTest_4::HandleEvent() entering, type=%x, err=%d" ), aEvent.iEventType.iUid, aEvent.iErrorCode ));

    if ( aEvent.iEventType == KUidECamEventCameraNoLongerReserved )
        {
        PRINT(( _L( "CCamcTest_4::HandleEvent() KUidECamEventCameraNoLongerReserved" ) ));
        }
    else if ( aEvent.iEventType == KUidECamEventPowerOnComplete )
        {
        PRINT(( _L( "CCamcTest_4::HandleEvent() KUidECamEventPowerOnComplete" ) ));
        PowerOnComplete( aEvent.iErrorCode );
        }
    else if ( aEvent.iEventType == KUidECamEventReserveComplete )
       {
       PRINT(( _L( "CCamcTest_4::HandleEvent() KUidECamEventReserveComplete" ) ));
       ReserveComplete( aEvent.iErrorCode );
       }

    PRINT(( _L( "CCamcTestClient_7::HandleEvent() returning" ) ));
    }

// -----------------------------------------------------------------------------
// CCamcTest_4::ViewFinderReady
// MCameraObserver2 call-back handler
// -----------------------------------------------------------------------------
//
void CCamcTest_4::ViewFinderReady( MCameraBuffer& aCameraBuffer, TInt aError)
    {
    PRINT(( _L( "CCamcTest_4::ViewFinderReady() NOT HANDLED err=%d, NumFrames=%d" ), aError, aCameraBuffer.NumFrames() ));
    CFbsBitmap *buffer = NULL;
    if ( !aError )
        {
	    aCameraBuffer.Release();
        }
    ViewFinderFrameReady( *buffer );
    }

// -----------------------------------------------------------------------------
// CCamcTest_4::ImageBufferReady
// MCameraObserver2 call-back handler
// -----------------------------------------------------------------------------
//
void CCamcTest_4::ImageBufferReady( MCameraBuffer& aCameraBuffer, TInt aError)
    {
    PRINT(( _L( "CCamcTest_4::ImageBufferReady() BUFFER NOT HANDLED, err=%d, NumFrames=%d" ), aError, aCameraBuffer.NumFrames() ));
	
    if ( !aError )
        {
	    aCameraBuffer.Release();
        }
    ImageReady( NULL, NULL, aError );
    }
    
// -----------------------------------------------------------------------------
// CCamcTest_4::VideoBufferReady
// MCameraObserver2 call-back handler
// -----------------------------------------------------------------------------
//
void CCamcTest_4::VideoBufferReady( MCameraBuffer& aCameraBuffer, TInt aError)
    {
    PRINT(( _L( "CCamcTest_4::VideoBufferReady() entering, err=%d, NumFrames=%d" ), aError, aCameraBuffer.NumFrames() ));
    MFrameBuffer* buffer = NULL;
	
    if ( !aError )
        {
        aCameraBuffer.Release();
        }
    // Call old Camera-API observer handler
    FrameBufferReady( buffer, aError );
    
    PRINT(( _L( "CCamcTest_4::VideoBufferReady() returning" )));
    }



MTest* CCamcTest_4::suiteL ()
    {
    // Always use NewL (Do not use NewLC) !!!
    CTestSuite *suite = CTestSuite::NewL(_L8("CCamcTest_4"));
    CleanupStack::PushL( suite );

    suite->addTestL(CTestCaller<CCamcTest_4>::NewL(_L8("CAMC_API.RECORDTIMEAVAILABLE_001"), &RecordTimeAvailableL_001_L));
#if ! (defined (__WINS__) || defined (__WINSCW__) )
    suite->addTestL(CTestCaller<CCamcTest_4>::NewL(_L8("CAMC_API.RECORDTIMEAVAILABLE_002"), &RecordTimeAvailableL_002_L));
#endif
    suite->addTestL(CTestCaller<CCamcTest_4>::NewL(_L8("CAMC_API.RECORDTIMEAVAILABLE_003"), &RecordTimeAvailableL_003_L));
    suite->addTestL(CTestCaller<CCamcTest_4>::NewL(_L8("CAMC_API.RECORDTIMEAVAILABLE_004"), &RecordTimeAvailableL_004_L));

    // RecordTimeAvailableL_006, only to be tested on HW
    // Removed from MuDo tests
#if ! (defined (__WINS__) || defined (__WINSCW__) )
//    suite->addTestL(CTestCaller<CCamcTest_4>::NewL(_L8("CAMC_API.RECORDTIMEAVAILABLE_006"), &RecordTimeAvailableL_006_L));
#endif
  
    CleanupStack::Pop( suite );
    return suite;
    }



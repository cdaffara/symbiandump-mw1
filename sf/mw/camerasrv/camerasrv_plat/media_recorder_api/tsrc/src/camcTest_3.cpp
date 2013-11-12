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
* Description:  Camcorder MMF API Test DLL, Tests for memory Failures
*
*/



// INCLUDE FILES
#include "camcTest_3.h"

// EXTERNAL DATA STRUCTURES

// EXTERNAL FUNCTION PROTOTYPES 
 
// CONSTANTS

// MACROS

// LOCAL CONSTANTS AND MACROS

// MODULE DATA STRUCTURES

// LOCAL FUNCTION PROTOTYPES

// FORWARD DECLARATIONS


///////////////////////////////////////////////////////////////////
// Tests for memory Failures
//
// Should be used together with CppUnit Memory Failure option.
//
///////////////////////////////////////////////////////////////////

CCamcTest_3::CCamcTest_3 ()
    {
    }



CCamcTest_3::~CCamcTest_3 ()
    {
    delete iCamcTestClient;
    iCamcTestClient = NULL;
    }


void CCamcTest_3::setUpL ()
    {
    // Do nothing in the setUp to get Memory Failure functionality
    }


void CCamcTest_3::tearDown ()
    {
    delete iCamcTestClient;
    iCamcTestClient = NULL;
	REComSession::FinalClose();
    }


//
// Own test functions.
//

// //////////////////////////////////////////////////////////////////
//                       Memory Failure tests
// //////////////////////////////////////////////////////////////////

void CCamcTest_3::NewL_001_L()
    {
    ///////////////////////////////////////
	//   ID: CAMC_API.NEWL_001
    //   Action :
    //       CppUnit will generate all possible Memory Failure
    //       Problems.
    //   Output : 
    //       Function should leave gracefully.
    //   Precondition :
    //       None
    ///////////////////////////////////////
    

    iWaitScheduler = new (ELeave) CActiveSchedulerWait;	
    // Create a new Camera API implementation object, if supported
    TRAPD( err, iUiLevelCamera = CCamera::New2L( static_cast<MCameraObserver2&>(*this), 0 /*index*/, 0 /*Priority*/ ) );
    if ( err )
        {
        PRINT( ( _L( "CCamcTest_3::setUpL CCamera::New2L return code=%d" ), err ) ); 
        // Create old Camera API implementation object.
        iUiLevelCamera = CCamera::NewL( static_cast<MCameraObserver&>(*this), 0 /*Index*/ );
        }
    else 
       {
       PRINT( ( _L( "CCamcTest_3::setUp() using new MCameraObserver2" )) ); 
       }
	iUiLevelCameraHandle = iUiLevelCamera->Handle();
	iUiLevelCamera->Reserve();
	iWaitScheduler->Start();    

    iCamcTestClient = CCamcTestClient_3::NewL( iUiLevelCameraHandle );
    
    delete iWaitScheduler;
    delete iUiLevelCamera;       
    // Succeed, if it doesn't leave.
    }



// //////////////////////////////////////////////////////////////////
//   Dummy Camera API callback functions
// //////////////////////////////////////////////////////////////////
void CCamcTest_3::ReserveComplete(TInt /*aError*/)
    {
    RDebug::Print(_L("CCamcTest_3::ReserveComplete"));
        
    // UI level camera is being initialized        
    if (iWaitScheduler->IsStarted())
	    {
    	 iUiLevelCamera->PowerOn();
    	 return;
	    }
    }

void CCamcTest_3::PowerOnComplete(TInt /*aError*/)
    {   
    RDebug::Print(_L("CCamcTest_3::PowerOnComplete"));

    // UI level camera is being initialized
    if (iWaitScheduler->IsStarted())
	    {
    	 iWaitScheduler->AsyncStop();
    	 return;
    	}        
    }

void CCamcTest_3::ViewFinderFrameReady(CFbsBitmap& /*aFrame*/)
    {
    }
void CCamcTest_3::ImageReady(CFbsBitmap* /*aBitmap */,HBufC8* /*aData*/,TInt /*aError*/)
    {    
    }
void CCamcTest_3::FrameBufferReady(MFrameBuffer* /*aFrameBuffer*/,TInt /*aError*/ )
    {
    }

// -----------------------------------------------------------------------------
// CCamcTest_3::HandleEvent
// MCameraObserver2 call-back handler
// -----------------------------------------------------------------------------
//
void CCamcTest_3::HandleEvent( const TECAMEvent& aEvent)
    {
    PRINT(( _L( "CCamcTest_3::HandleEvent() entering, type=%x, err=%d" ), aEvent.iEventType.iUid, aEvent.iErrorCode ));

    if ( aEvent.iEventType == KUidECamEventCameraNoLongerReserved )
        {
        PRINT(( _L( "CCamcTest_3::HandleEvent() KUidECamEventCameraNoLongerReserved" ) ));
        }
    else if ( aEvent.iEventType == KUidECamEventPowerOnComplete )
        {
        PRINT(( _L( "CCamcTest_3::HandleEvent() KUidECamEventPowerOnComplete" ) ));
        PowerOnComplete( aEvent.iErrorCode );
        }
    else if ( aEvent.iEventType == KUidECamEventReserveComplete )
       {
       PRINT(( _L( "CCamcTest_3::HandleEvent() KUidECamEventReserveComplete" ) ));
       ReserveComplete( aEvent.iErrorCode );
       }

    PRINT(( _L( "CCamcTestClient_7::HandleEvent() returning" ) ));
    }

// -----------------------------------------------------------------------------
// CCamcTest_3::ViewFinderReady
// MCameraObserver2 call-back handler
// -----------------------------------------------------------------------------
//
void CCamcTest_3::ViewFinderReady( MCameraBuffer& aCameraBuffer, TInt aError)
    {
    PRINT(( _L( "CCamcTest_3::ViewFinderReady() NOT HANDLED err=%d, NumFrames=%d" ), aError, aCameraBuffer.NumFrames() ));
    CFbsBitmap *buffer = NULL;
    if ( !aError )
        {
	    aCameraBuffer.Release();
        }
    ViewFinderFrameReady( *buffer );
    }

// -----------------------------------------------------------------------------
// CCamcTest_3::ImageBufferReady
// MCameraObserver2 call-back handler
// -----------------------------------------------------------------------------
//
void CCamcTest_3::ImageBufferReady( MCameraBuffer& aCameraBuffer, TInt aError)
    {
    PRINT(( _L( "CCamcTest_3::ImageBufferReady() BUFFER NOT HANDLED, err=%d, NumFrames=%d" ), aError, aCameraBuffer.NumFrames() ));
	
    if ( !aError )
        {
	    aCameraBuffer.Release();
        }
    ImageReady( NULL, NULL, aError );
    }
    
// -----------------------------------------------------------------------------
// CCamcTest_3::VideoBufferReady
// MCameraObserver2 call-back handler
// -----------------------------------------------------------------------------
//
void CCamcTest_3::VideoBufferReady( MCameraBuffer& aCameraBuffer, TInt aError)
    {
    PRINT(( _L( "CCamcTest_3::VideoBufferReady() entering, err=%d, NumFrames=%d" ), aError, aCameraBuffer.NumFrames() ));
    MFrameBuffer* buffer = NULL;
	
    if ( !aError )
        {
        aCameraBuffer.Release();
        }
    // Call old Camera-API observer handler
    FrameBufferReady( buffer, aError );
    
    PRINT(( _L( "CCamcTest_3::VideoBufferReady() returning" )));
    }


//
// An own static function to collect the test functions into one 
// suite of tests. The framework will run the tests and free the
// memory allocated for the test suite.
// 

MTest* CCamcTest_3::suiteL ()
    {
    // Always use NewL (Do not use NewLC) !!!
    CTestSuite *suite = CTestSuite::NewL(_L8("CCamcTest_3"));
    CleanupStack::PushL( suite );
    
    suite->addTestL(CTestCaller<CCamcTest_3>::NewL(_L8("CAMC_API.NEWL_001"), &CCamcTest_3::NewL_001_L));
    CleanupStack::Pop( suite );
    return suite;
    }


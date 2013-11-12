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
#include "camcTest_8.h"
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
// Tests for Cooperation with Camera API
//
///////////////////////////////////////////////////////////////////

CCamcTest_8::CCamcTest_8 ()
    {
    }



CCamcTest_8::~CCamcTest_8 ()
    {
    delete iCamcTestClient1; // To mute PCLINT
    iCamcTestClient1 = NULL;
    delete iCamcTestClient2; // To mute PCLINT
    iCamcTestClient2 = NULL;
    }


void CCamcTest_8::setUpL ()
    {
    iWaitScheduler = new (ELeave) CActiveSchedulerWait;	
    // Create a new Camera API implementation object, if supported
    TRAPD( err, iUiLevelCamera = CCamera::New2L( static_cast<MCameraObserver2&>(*this), 0 /*index*/, 0 /*Priority*/ ) );
    if ( err )
        {
        PRINT( ( _L( "CCamcTest_8::setUpL CCamera::New2L return code=%d" ), err ) ); 
        // Create old Camera API implementation object.
        iUiLevelCamera = CCamera::NewL( static_cast<MCameraObserver&>(*this), 0 /*Index*/ );
        }
    else 
       {
       PRINT( ( _L( "CCamcTest_8::setUp() using new MCameraObserver2" )) ); 
       }

	iUiLevelCameraHandle = iUiLevelCamera->Handle();
	iUiLevelCamera->Reserve();
	iWaitScheduler->Start();        
    }


void CCamcTest_8::tearDown ()
    {   
    delete iWaitScheduler;
    delete iUiLevelCamera;    
        
	REComSession::FinalClose();
    }


//
// Own test functions.
//

void CCamcTest_8::MultipleInstance_001_L()
    {
    
    ///////////////////////////////////////
    //  ID: CAMC_API.MULTIPLEINSTANCE_001
    //
    //   Action :
    //      1 Camcorder is in Prepared state
    //      A 2nd Camcorder is instanciated with same file name.
	//   Output : 
    //      Leave with KErrInUse
    /////////////////////////////////////// 
    TInt numberOfCamcOpen = 0;

    TFileName file;
    AddDriveLetterToPath(_L("recordQCIF1.3gp"),file);

    iCamcTestClient1 = CCamcTestClient_8::NewL( file,
        &iError,&numberOfCamcOpen, iUiLevelCameraHandle);
    iCamcTestClient2 = CCamcTestClient_8::NewL( file,
        &iError,&numberOfCamcOpen, iUiLevelCameraHandle);


    iCamcTestClient1->Start_Active_Object( K_Tc8_MultipleInstance001_AO1 ,
        iCamcTestClient2);    
    iCamcTestClient2->Start_Active_Object( K_Tc8_MultipleInstance001_AO2 ,
        iCamcTestClient1); 

    // Start the active scheduler
    CActiveScheduler::Start();

    delete iCamcTestClient1;      
    iCamcTestClient1 = NULL;
    delete iCamcTestClient2; 
    iCamcTestClient2 = NULL;
    assertTIntsEqualL( KErrInUse, iError );

    }


void CCamcTest_8::MultipleInstance_002_L()
    {
    
    ///////////////////////////////////////
    //  ID: CAMC_API.MULTIPLEINSTANCE_002
    //
    //   Action :
    //      1 Camcorder is in Prepared state
    //      A 2nd Camcorder is instanciated with different file name.
    //   Output : 
    //      OK
    /////////////////////////////////////// 
    TInt numberOfCamcOpen = 0;

    TFileName file;
    AddDriveLetterToPath(_L("recordQCIF1.3gp"),file);
    iCamcTestClient1 = CCamcTestClient_8::NewL( file,
        &iError, &numberOfCamcOpen, iUiLevelCameraHandle);

    AddDriveLetterToPath(_L("recordQCIF2.3gp"),file);
    iCamcTestClient2 = CCamcTestClient_8::NewL( file ,
        &iError, &numberOfCamcOpen, iUiLevelCameraHandle);


    iCamcTestClient1->Start_Active_Object( K_Tc8_MultipleInstance002_AO1 ,
        iCamcTestClient2);    
    iCamcTestClient2->Start_Active_Object( K_Tc8_MultipleInstance002_AO2 ,
        iCamcTestClient1); 

    // Start the active scheduler
    CActiveScheduler::Start();
    assertTIntsEqualL( KErrNone, iError );
    delete iCamcTestClient1;      
    iCamcTestClient1 = NULL;
    delete iCamcTestClient2;   
    iCamcTestClient2 = NULL;
    }

void CCamcTest_8::MultipleInstance_003_L()
    {
    
    ///////////////////////////////////////
    //  ID: CAMC_API.MULTIPLEINSTANCE_003
    //
    //   Action :
    //      1 Camcorder is in Prepared state
    //      A 2nd Camcorder calls prepare.
    //   Output : 
    //      Leave with KErrInUse
    /////////////////////////////////////// 
    TInt numberOfCamcOpen = 0;

    TFileName file;
    AddDriveLetterToPath(_L("recordQCIF1.3gp"),file);
    iCamcTestClient1 = CCamcTestClient_8::NewL( file,
        &iError, &numberOfCamcOpen, iUiLevelCameraHandle);
    
    AddDriveLetterToPath(_L("recordQCIF2.3gp"),file);
    iCamcTestClient2 = CCamcTestClient_8::NewL( file,
        &iError, &numberOfCamcOpen, iUiLevelCameraHandle);


    iCamcTestClient1->Start_Active_Object( K_Tc8_MultipleInstance003_AO1 ,
        iCamcTestClient2);    
    iCamcTestClient2->Start_Active_Object( K_Tc8_MultipleInstance003_AO2 ,
        iCamcTestClient1); 

    // Start the active scheduler
    CActiveScheduler::Start();

    delete iCamcTestClient1;       
    iCamcTestClient1 = NULL;
    delete iCamcTestClient2;   
    iCamcTestClient2 = NULL;
    assertTIntsEqualL( KErrInUse, iError );
    }



void CCamcTest_8::MultipleInstance_004_L()
    {
    
    ///////////////////////////////////////
    //  ID: CAMC_API.MULTIPLEINSTANCE_004
    //
    //   Action :
    //      1 Camcorder is Recording
    //      A 2nd Camcorder calls prepare.
    //   Output : 
    //      Leave with KErrInUse
    /////////////////////////////////////// 
    TInt numberOfCamcOpen = 0;

    TFileName file;
    AddDriveLetterToPath(_L("recordQCIF1.3gp"),file);
    iCamcTestClient1 = CCamcTestClient_8::NewL( file,
        &iError, &numberOfCamcOpen, iUiLevelCameraHandle);
    
    AddDriveLetterToPath(_L("recordQCIF2.3gp"),file);
    iCamcTestClient2 = CCamcTestClient_8::NewL( file,
        &iError, &numberOfCamcOpen, iUiLevelCameraHandle);


    iCamcTestClient1->Start_Active_Object( K_Tc8_MultipleInstance004_AO1 ,
        iCamcTestClient2);    
    iCamcTestClient2->Start_Active_Object( K_Tc8_MultipleInstance004_AO2 ,
        iCamcTestClient1); 

    // Start the active scheduler
    CActiveScheduler::Start();

    delete iCamcTestClient1; 
    iCamcTestClient1 = NULL;
    delete iCamcTestClient2; 
    iCamcTestClient2 = NULL;
    assertTIntsEqualL( KErrInUse, iError );
    }



void CCamcTest_8::MultipleInstance_005_L()
    {
    
    ///////////////////////////////////////
    //  ID: CAMC_API.MULTIPLEINSTANCE_005
    //
    //   Action :
    //      1 Camcorder has finished Recording
    //      A 2nd Camcorder calls prepare.
    //   Output : 
    //      Leaves with KErrInUse
    /////////////////////////////////////// 
    TInt numberOfCamcOpen = 0;

    TFileName file;
    AddDriveLetterToPath(_L("recordQCIF1.3gp"),file);
    iCamcTestClient1 = CCamcTestClient_8::NewL( file,
        &iError, &numberOfCamcOpen, iUiLevelCameraHandle);

    AddDriveLetterToPath(_L("recordQCIF2.3gp"),file);
    iCamcTestClient2 = CCamcTestClient_8::NewL( file ,
        &iError, &numberOfCamcOpen, iUiLevelCameraHandle);

    iCamcTestClient1->Start_Active_Object( K_Tc8_MultipleInstance005_AO1 ,
        iCamcTestClient2);    
    iCamcTestClient2->Start_Active_Object( K_Tc8_MultipleInstance005_AO2 ,
        iCamcTestClient1); 

    // Start the active scheduler
    CActiveScheduler::Start();
    
    delete iCamcTestClient1; 
    iCamcTestClient1 = NULL;
    delete iCamcTestClient2; 
    iCamcTestClient2 = NULL;
    assertTIntsEqualL( KErrInUse, iError );
    assertTIntsEqualL( KErrHardwareNotAvailable, iError );
    }


void CCamcTest_8::MultipleInstance_006_L()
    {
    
    ///////////////////////////////////////
    //  ID: CAMC_API.MULTIPLEINSTANCE_006
    //
    //   Action :
    //      1 Camcorder is Recording
    //      A 2nd Camcorder calls prepare.
    //   Output : 
    //      Leave with KErrInUse
    /////////////////////////////////////// 
    TInt numberOfCamcOpen = 0;

    
    TFileName file;
    AddDriveLetterToPath(_L("recordQCIF1.3gp"),file);

    iCamcTestClient1 = CCamcTestClient_8::NewL( file,
        &iError, &numberOfCamcOpen, iUiLevelCameraHandle);

    AddDriveLetterToPath(_L("recordQCIF2.3gp"),file);
    iCamcTestClient2 = CCamcTestClient_8::NewL( file,
        &iError, &numberOfCamcOpen, iUiLevelCameraHandle);


    iCamcTestClient1->Start_Active_Object( K_Tc8_MultipleInstance006_AO1 ,
        iCamcTestClient2);    
    iCamcTestClient2->Start_Active_Object( K_Tc8_MultipleInstance006_AO2 ,
        iCamcTestClient1); 

    // Start the active scheduler
    CActiveScheduler::Start();

    delete iCamcTestClient1;  
    iCamcTestClient1 = NULL;      
    delete iCamcTestClient2; 
    iCamcTestClient2 = NULL;
    assertTIntsEqualL( KErrInUse, iError );
    }
    
// //////////////////////////////////////////////////////////////////
//   Dummy Camera API callback functions
// //////////////////////////////////////////////////////////////////
void CCamcTest_8::ReserveComplete(TInt /*aError*/)
    {
    RDebug::Print(_L("CCamcTest_8::ReserveComplete"));
        
    // UI level camera is being initialized        
    if (iWaitScheduler->IsStarted())
	    {
    	 iUiLevelCamera->PowerOn();
    	 return;
	    }
    }

void CCamcTest_8::PowerOnComplete(TInt /*aError*/)
    {   
    RDebug::Print(_L("CCamcTest_8::PowerOnComplete"));

    // UI level camera is being initialized
    if (iWaitScheduler->IsStarted())
	    {
    	 iWaitScheduler->AsyncStop();
    	 return;
    	}        
    }

void CCamcTest_8::ViewFinderFrameReady(CFbsBitmap& /*aFrame*/)
    {
    }
void CCamcTest_8::ImageReady(CFbsBitmap* /*aBitmap */,HBufC8* /*aData*/,TInt /*aError*/)
    {    
    }
void CCamcTest_8::FrameBufferReady(MFrameBuffer* /*aFrameBuffer*/,TInt /*aError*/ )
    {
    }
    
  
// -----------------------------------------------------------------------------
// CCamcTest_8::HandleEvent
// MCameraObserver2 call-back handler
// -----------------------------------------------------------------------------
//
void CCamcTest_8::HandleEvent( const TECAMEvent& aEvent)
    {
    PRINT(( _L( "CCamcTest_8::HandleEvent() entering, type=%x, err=%d" ), aEvent.iEventType.iUid, aEvent.iErrorCode ));

    if ( aEvent.iEventType == KUidECamEventCameraNoLongerReserved )
        {
        PRINT(( _L( "CCamcTest_8::HandleEvent() KUidECamEventCameraNoLongerReserved" ) ));
        }
    else if ( aEvent.iEventType == KUidECamEventPowerOnComplete )
        {
        PRINT(( _L( "CCamcTest_8::HandleEvent() KUidECamEventPowerOnComplete" ) ));
        PowerOnComplete( aEvent.iErrorCode );
        }
    else if ( aEvent.iEventType == KUidECamEventReserveComplete )
       {
       PRINT(( _L( "CCamcTest_8::HandleEvent() KUidECamEventReserveComplete" ) ));
       ReserveComplete( aEvent.iErrorCode );
       }

    PRINT(( _L( "CCamcTestClient_7::HandleEvent() returning" ) ));
    }

// -----------------------------------------------------------------------------
// CCamcTest_8::ViewFinderReady
// MCameraObserver2 call-back handler
// -----------------------------------------------------------------------------
//
void CCamcTest_8::ViewFinderReady( MCameraBuffer& aCameraBuffer, TInt aError)
    {
    PRINT(( _L( "CCamcTest_8::ViewFinderReady() NOT HANDLED err=%d, NumFrames=%d" ), aError, aCameraBuffer.NumFrames() ));
    CFbsBitmap *buffer = NULL;
    if ( !aError )
        {
	    aCameraBuffer.Release();
        }
    ViewFinderFrameReady( *buffer );
    }

// -----------------------------------------------------------------------------
// CCamcTest_8::ImageBufferReady
// MCameraObserver2 call-back handler
// -----------------------------------------------------------------------------
//
void CCamcTest_8::ImageBufferReady( MCameraBuffer& aCameraBuffer, TInt aError)
    {
    PRINT(( _L( "CCamcTest_8::ImageBufferReady() BUFFER NOT HANDLED, err=%d, NumFrames=%d" ), aError, aCameraBuffer.NumFrames() ));
	
    if ( !aError )
        {
	    aCameraBuffer.Release();
        }
    ImageReady( NULL, NULL, aError );
    }
    
// -----------------------------------------------------------------------------
// CCamcTest_8::VideoBufferReady
// MCameraObserver2 call-back handler
// -----------------------------------------------------------------------------
//
void CCamcTest_8::VideoBufferReady( MCameraBuffer& aCameraBuffer, TInt aError)
    {
    PRINT(( _L( "CCamcTest_8::VideoBufferReady() entering, err=%d, NumFrames=%d" ), aError, aCameraBuffer.NumFrames() ));
    MFrameBuffer* buffer = NULL;
	
    if ( !aError )
        {
        aCameraBuffer.Release();
        }
    // Call old Camera-API observer handler
    FrameBufferReady( buffer, aError );
    
    PRINT(( _L( "CCamcTest_8::VideoBufferReady() returning" )));
    }




MTest* CCamcTest_8::suiteL ()
    {
    // Always use NewL (Do not use NewLC) !!!
    CTestSuite *suite = CTestSuite::NewL(_L8("CCamcTest_8"));   
    CleanupStack::PushL( suite );

    suite->addTestL(CTestCaller<CCamcTest_8>::NewL(_L8("CAMC_API.MULTIPLEINSTANCE_001"), &MultipleInstance_001_L));
    
    suite->addTestL(CTestCaller<CCamcTest_8>::NewL(_L8("CAMC_API.MULTIPLEINSTANCE_002"), &MultipleInstance_002_L));
    suite->addTestL(CTestCaller<CCamcTest_8>::NewL(_L8("CAMC_API.MULTIPLEINSTANCE_003"), &MultipleInstance_003_L));
    suite->addTestL(CTestCaller<CCamcTest_8>::NewL(_L8("CAMC_API.MULTIPLEINSTANCE_004"), &MultipleInstance_004_L));
    suite->addTestL(CTestCaller<CCamcTest_8>::NewL(_L8("CAMC_API.MULTIPLEINSTANCE_005"), &MultipleInstance_005_L));
    suite->addTestL(CTestCaller<CCamcTest_8>::NewL(_L8("CAMC_API.MULTIPLEINSTANCE_006"), &MultipleInstance_006_L));
    
    CleanupStack::Pop( suite );
    return suite;
    }


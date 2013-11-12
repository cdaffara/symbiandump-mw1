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


#ifndef __CAMCTEST_3_H
#define __CAMCTEST_3_H

//  INCLUDES


#include <e32base.h>
#include <ecam.h>
#include "camcTest.h"


#include <videorecorder.h>
#include "camctestclient_3.h"

// CONSTANTS

// MACROS
#ifdef _DEBUG
#include <e32svr.h>
#define PRINT(x) RDebug::Print x
#else
#define PRINT(x)
#endif

// DATA TYPES

// FUNCTION PROTOTYPES

// FORWARD DECLARATIONS

// CLASS DECLARATION

class CCamcTest_3 : public CTestCase, public MCameraObserver, public MCameraObserver2
    {
    public:
        
        CCamcTest_3 ();
        ~CCamcTest_3 ();
        
        // Allocate the resources for one test function
        void setUpL ();
        
        // Free the resources reserved in setUpL()
        void tearDown ();
        
        // A function to collect and return a suite of tests
        static MTest* suiteL ();
       
        // Camera API callbacks
        void ReserveComplete(TInt aError);
        void PowerOnComplete(TInt aError);
        void ViewFinderFrameReady(CFbsBitmap& aFrame);
        void ImageReady(CFbsBitmap* aBitmap,HBufC8* aData,TInt aError);	
        void FrameBufferReady(MFrameBuffer* aFrameBuffer,TInt aError);        

        // Camera API V2 callbacks
        void HandleEvent(const TECAMEvent& aEvent);
        void ViewFinderReady(MCameraBuffer& aCameraBuffer,TInt aError);
        void ImageBufferReady(MCameraBuffer& aCameraBuffer,TInt aError);
        void VideoBufferReady(MCameraBuffer& aCameraBuffer,TInt aError);
       
    protected:
        
        // Own test functions that use assertions and may leave:
        void NewL_001_L();
       
        
    private:
        
        CCamcTestClient_3 * iCamcTestClient;

        TInt iUiLevelCameraHandle;
        CCamera *iUiLevelCamera;
    	CActiveSchedulerWait *iWaitScheduler;         
    };

#endif

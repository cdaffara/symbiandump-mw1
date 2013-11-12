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


#ifndef __CAMCTEST_VISUALCHECKDSP_H
#define __CAMCTEST_VISUALCHECKDSP_H

//  INCLUDES

#include <e32base.h>
#include "camcTest.h"

#include <ecam.h>
#include <videorecorder.h>
#include "camctestclient_visualcheckdsp.h"

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



class CCamcTest_visualcheckdsp : public CTestCase, public MCameraObserver, public MCameraObserver2
    {
    public:
        
        CCamcTest_visualcheckdsp ();
        ~CCamcTest_visualcheckdsp ();
        
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

       void VisualCheck_001_L();
       void VisualCheck_002_L();
       void VisualCheck_003_L();
       void VisualCheck_004_L();
       void VisualCheck_005_L();
       void VisualCheck_006_L();
       void VisualCheck_007_L();
       void VisualCheck_008_L();
       void VisualCheck_009_A_L();
       void VisualCheck_009_B_L();
       void VisualCheck_010_L();
       void VisualCheck_011_L();
       void VisualCheck_012_L();
       void VisualCheck_013_L();
       void VisualCheck_014_L();
       void VisualCheck_015_L();
       void VisualCheck_016_L();
       void VisualCheck_017_L();
       void VisualCheck_017_A_L();
       void VisualCheck_018_L();
       void VisualCheck_019_L();
       void VisualCheck_020_L();
       void VisualCheck_021_L();

    private:
        
        CCamcTestClient_visualcheckdsp * iCamcTestClient;
        
        TInt iUiLevelCameraHandle;
        CCamera *iUiLevelCamera;
        CActiveSchedulerWait *iWaitScheduler; 
    };

#endif

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


#ifndef __CAMCTEST_9_H
#define __CAMCTEST_9_H

//  INCLUDES

#include <e32base.h>
#include <ecam.h>
#include "camcTest.h"

#include <videorecorder.h>
#include "camctestclient_9.h"

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



class CCamcTest_9 : public CTestCase, public MCameraObserver, public MCameraObserver2
    {
    public:
        
        CCamcTest_9 ();
        ~CCamcTest_9 ();
        
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
        void Prepare_007_L();
        void Prepare_008_L();
        void Prepare_009_L();
        void RecordTimeAvailableL_005_L();
        void SetMaxClipSizeL_008_L();
        void SetMaxClipSizeL_009_L();
        void NewFileName_001_L();
        void NewFileName_002_L();
        void NewFileName_003_L();
        void NewFileName_004_L();
        void NewFileName_005_L();
        void NewFileName_006_L();
        void NewFileName_007_L();
        void OpenFileL_016_L();
        
    private:
        
        CCamcTestClient_9 * iCamcTestClient;
        
        TInt iUiLevelCameraHandle;
        CCamera *iUiLevelCamera;
    	CActiveSchedulerWait *iWaitScheduler;           
        
    };

#endif

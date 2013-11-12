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


#ifndef __CAMCTEST_2_H
#define __CAMCTEST_2_H

//  INCLUDES

#include <e32base.h>
#include "camcTest.h"
#include <ecam.h>
#include <videorecorder.h>

#include "camctestclient_2.h"

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


class CCamcTest_2 : public CTestCase, public MCameraObserver, public MCameraObserver2
    {
    public:
        
        CCamcTest_2 ();
        ~CCamcTest_2 ();
        
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
        void Close_001_L();
        void Close_002_L();
        void Close_003_L();
        void Close_004_L();
        void Close_005_L();
        void Close_007_L();
        
        void Prepare_001_L();
        void Prepare_002_L();
        void Prepare_003_L();
        void Prepare_004_L();
        void Prepare_005_L();
        void Prepare_006_L();
        
        void Record_001_L();
        void Record_002_L();
        void Record_003_L();
        void Record_004_L();
        void Record_005_L();
                
        void SetPriorityL_001_L();
        void SetPriorityL_002_L();
        void SetPriorityL_003_L();
        void SetPriorityL_004_L();
        
        
        void SetVideoFrameRateL_001_L();
        void SetVideoFrameRateL_002_L();
        void SetVideoFrameRateL_003_L();
        void SetVideoFrameRateL_004_L();
        
        void SetVideoFrameSizeL_001_L();
        void SetVideoFrameSizeL_002_L();
        void SetVideoFrameSizeL_003_L();
        void SetVideoFrameSizeL_004_L();
        void SetVideoFrameSizeL_007_L();
        
        void SetVideoBitRateL_001_L();
        void SetVideoBitRateL_002_L();
        void SetVideoBitRateL_003_L();
        void SetVideoBitRateL_004_L();
        
        
        void SetAudioBitRateL_001_L();
        void SetAudioBitRateL_002_L();
        void SetAudioBitRateL_003_L();
        void SetAudioBitRateL_004_L();
        
        void SetAudioEnabledL_001_L();
        void SetAudioEnabledL_002_L();
        void SetAudioEnabledL_003_L();
        void SetAudioEnabledL_004_L();
        
        
        void SetMaxClipSizeL_001_L();
        void SetMaxClipSizeL_002_L();
        void SetMaxClipSizeL_003_L();
        void SetMaxClipSizeL_004_L();
        
        
        void SetVideoTypeL_001_L();
        void SetVideoTypeL_002_L();
        void SetVideoTypeL_003_L();
        void SetVideoTypeL_004_L();
        
        void SetAudioTypeL_001_L();
        void SetAudioTypeL_002_L();
        void SetAudioTypeL_003_L();
        void SetAudioTypeL_004_L();
        
    private:
        CCamcTestClient_2 * iCamcTestClient;
        
        TInt iUiLevelCameraHandle;
        CCamera *iUiLevelCamera;
    	CActiveSchedulerWait *iWaitScheduler;         
    };

#endif

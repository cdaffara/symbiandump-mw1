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


#ifndef __CAMCTEST_6_H
#define __CAMCTEST_6_H

//  INCLUDES


#include <e32base.h>
#include <ecam.h>
#include "camcTest.h"

#include <videorecorder.h>
#include "camctestclient_6.h"


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


class CCamcTest_6 : public CTestCase, public MCameraObserver, public MCameraObserver2
    {
    public:
        
        CCamcTest_6 ();
        ~CCamcTest_6 ();
        
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
        void PauseL_003_L();

        void SetVideoFrameSizeL_005_L();

        void SetAudioEnabledL_005_L();

        void SetMaxClipSizeL_006_L();
        void SetMaxClipSizeL_007_L();

        void Record_006_L();
        void Record_007_L();

        void VideoTypeL_001_L();
        void VideoTypeL_002_L();
        void VideoTypeL_003_L();
        void VideoTypeL_004_L();
        void VideoTypeL_005_L();

        void AudioTypeL_001_L();
        void AudioTypeL_002_L();
        void AudioTypeL_003_L();

        void SetVideoTypeL_005_L();
        void SetVideoTypeL_006_L();
        void SetVideoTypeL_007_L();
        void SetVideoTypeL_008_L();
        void SetVideoTypeL_009_L();

        void SetAudioTypeL_005_L();
        void SetAudioTypeL_006_L();
        void SetAudioTypeL_007_L();
        void SetAudioTypeL_008_L();

        void ControllerImplementationInformationL_001_L();
        void ControllerImplementationInformationL_002_L();
        void ControllerImplementationInformationL_003_L();
        void ControllerImplementationInformationL_004_L();

        void MMFRegistration_001_L();
        void MMFRegistration_002_L();
        void MMFRegistration_003_L();
        void MMFRegistration_004_L();
        void MMFRegistration_005_L();
        void MMFRegistration_006_L();

        void DurationL_001_L();
        void DurationL_002_L();
        void DurationL_003_L();
                    
    private:
        
        CCamcTestClient_6 * iCamcTestClient;

        TInt iUiLevelCameraHandle;
        CCamera *iUiLevelCamera;
    	CActiveSchedulerWait *iWaitScheduler;          
    };

#endif

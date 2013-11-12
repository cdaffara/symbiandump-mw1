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



#ifndef __CAMCTEST_1_H
#define __CAMCTEST_1_H


//  INCLUDES
#include <e32base.h>
#include "camcTest.h"
#include <videorecorder.h>
#include <ecam.h>
#include "camctestclient_1.h"

// CONSTANTS

#define CONTROLLER_UID 0x101f8503
#define VIDEO_FORMAT_UID 0x101f8504
#define VIDEO_FORMAT_UID_NEW 0x101F86D6
#define VIDEO_FORMAT_UID_MP4 0x101F873D

#define MIME_VIDEO_FORMAT _L8("video/H263-2000")
#define NEW_MIME_VIDEO_FORMAT _L8("video/mp4v-es")
#define MPEG4_MIME_VIDEO_FORMAT _L8("video/mp4v-es; profile-level-id=3")
#define NO_AUDIO_FOURCC KMMFFourCCCodeNULL
#define ACC_AUDIO_FORMAT TFourCC( ' ','A','A','C' )
#define AMR_AUDIO_FORMAT TFourCC( ' ','A','M','R' )
#define UNSUPPORTED_VIDEO_FORMAT _L8("video/unSupport")

// MACROS

// DATA TYPES

// FUNCTION PROTOTYPES

// FORWARD DECLARATIONS

// CLASS DECLARATION

class CCamcTest_1 : public CTestCase, public MCameraObserver, public MCameraObserver2
    {
    public:
        
        CCamcTest_1 ();
        ~CCamcTest_1 ();
        
        // Allocate the resources for one test function
        void setUpL ();
        
        // Free the resources reserved in setUpL()
        void tearDown ();
        
        // A function to collect and return a suite of tests
        static MTest* suiteL ();
        
    protected:
        
        // Own test functions that use assertions and may leave:
        void OpenFileL_001_L();
        void OpenFileL_002_L();
        void OpenFileL_003_L();
        void OpenFileL_004_L();	
        void OpenFileL_005_L();
        
        void OpenFileL_006_L();
        void OpenFileL_008_L();
        void OpenFileL_009_L();	
        void OpenFileL_010_L();
        
        void OpenFileL_011_L();
        void OpenFileL_017_L();
        void OpenFileL_018_L();
        void OpenFileL_019_L();
        void OpenFileL_020_L();
        void OpenFileL_021_L();
        
        void OpenDesL_001_L();
        void OpenUrlL_001_L();
                
    private:       
        
        enum TCamCTest_1_Actions
            {
            KCamcorderInDuplicatedMode
            };

        CCamcTestClient_1 * iCamcTestClient;
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


        // Variables and functions for Camera API 
        void SomethingInActiveObjectIsLeaving (TInt aError);
        CCamera *iCamera; 
        TCamCTest_1_Actions iTestCase;
        TInt iError;
        
        TInt iUiLevelCameraHandle;
        CCamera *iUiLevelCamera;
    	CActiveSchedulerWait *iWaitScheduler;        
    };

#endif

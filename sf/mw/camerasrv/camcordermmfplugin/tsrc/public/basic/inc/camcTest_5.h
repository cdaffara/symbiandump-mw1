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


#ifndef __CAMCTEST_5_H
#define __CAMCTEST_5_H

//  INCLUDES


#include <e32base.h>
#include <ecam.h>
#include "Camctest.h"

#include <videorecorder.h>
#include "camctestclient_5.h"

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




class CCamcTest_5 : public CTestCase, public MCameraObserver, public MCameraObserver2
    {
    public:
        
        CCamcTest_5 ();
        ~CCamcTest_5 ();
        
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
        void NumberOfMetaDataEntriesL_001_L();
        void NumberOfMetaDataEntriesL_002_L();
        void MetaDataEntryL_001_L();
        void MetaDataEntryL_002_L();
        void AddMetaDataEntryL_001_L();
        void AddMetaDataEntryL_002_L();
        void RemoveMetaDataEntryL_001_L();
        void RemoveMetaDataEntryL_002_L();
        void ReplaceMetaDataEntryL_001_L();
        void ReplaceMetaDataEntryL_002_L();
        void SetPriorityL_005_L();
        void GetPriorityL_001_L();
        void SetVideoFrameRateL_005_L();
        void VideoFrameRateL_001_L();
        void SetVideoBitRateL_005_L();
        void VideoBitRateL_001_L();
        void SetAudioBitRateL_005_L();
        void AudioBitRateL_001_L();
        void SetAudioEnabledL_005_L();
        void AudioEnabledL_001_L();
        void SetVideoFrameSizeL_006_L();
        void GetVideoFrameSizeL_001_L();
        void PauseL_001_L();
        void PauseL_002_L();
        void SetMaxClipSizeL_005_L();
        void SetGainL_001_L();
        void SetGainL_002_L();
        void SetGainL_003_L();
        void GainL_001_L();
        void MaxGainL_001_L();
        void MaxGainL_002_L();
        void Stop_001_L();
        void Stop_002_L();
        void Stop_003_L();
        void Stop_004_L();
        void Stop_005_L();
        void Stop_006_L();
        void Stop_007_L();
        void GetSupportedVideoTypes_001_L();
        void GetSupportedVideoTypes_002_L();
        void GetSupportedAudioTypes_001_L();
        void GetSupportedAudioTypes_002_L();
        void GetSupportedAudioTypes_003_L();

    private:
        
        CCamcTestClient_5 * iCamcTestClient;
        
        TInt iUiLevelCameraHandle;
        CCamera *iUiLevelCamera;
    	CActiveSchedulerWait *iWaitScheduler;         
        
    };

#endif

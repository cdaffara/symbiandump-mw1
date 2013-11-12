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


#ifndef __CAMCTESTCLIENT_VISUALCHECKDSP_H
#define __CAMCTESTCLIENT_VISUALCHECKDSP_H


//  INCLUDES

#include <e32base.h>
#include "camcTest.h"

#include <mmf/common/mmfvideo.h>
#include <videorecorder.h>


// CONSTANTS

#define VIDEO_BIT_RATE_10 TInt(64000)
#define VIDEO_BIT_RATE_11 TInt(28000) 

// MACROS

// DATA TYPES

enum TestClientVCdspActions
    {
    K_VC_dsp_none,
        K_VC_dsp_test_case_1,
        K_VC_dsp_test_case_2,
        K_VC_dsp_test_case_3,
        K_VC_dsp_test_case_4,
        K_VC_dsp_test_case_5,
        K_VC_dsp_test_case_6,
        K_VC_dsp_test_case_7,
        K_VC_dsp_test_case_8,
        K_VC_dsp_test_case_9_a,
        K_VC_dsp_test_case_9_b,
        K_VC_dsp_test_case_10,
        K_VC_dsp_test_case_11,
        K_VC_dsp_test_case_12,
        K_VC_dsp_test_case_13,
        K_VC_dsp_test_case_14,
        K_VC_dsp_test_case_15,
        K_VC_dsp_test_case_16,
        K_VC_dsp_test_case_17,
        K_VC_dsp_test_case_17_a,
        K_VC_dsp_test_case_18,
        K_VC_dsp_test_case_19,
        K_VC_dsp_test_case_20,
        K_VC_dsp_test_case_21
    };


// FUNCTION PROTOTYPES

// FORWARD DECLARATIONS

// CLASS DECLARATION




class CCamcTestClient_visualcheckdsp : public CTimer /* also CActive*/, public MVideoRecorderUtilityObserver
{
public:
    static CCamcTestClient_visualcheckdsp* NewL( );
	~CCamcTestClient_visualcheckdsp ();

	void Start_Active_ObjectL ( TestClientVCdspActions aAction, TInt aCameraHandle );

    //from observer
    void MvruoOpenComplete(TInt aError);
    void MvruoPrepareComplete(TInt aError);
    void MvruoRecordComplete(TInt aError);
    void MvruoEvent(const TMMFEvent& /*aEvent*/);
    void SomethingInActiveObjectIsLeaving (TInt aError);

    

protected:

private:

	CCamcTestClient_visualcheckdsp ();
    void ConstructL( );

    //from CActive
    void RunL();
    void RunLTrappedL();
    void DoCancel();

    CVideoRecorderUtility* iCamc;
	
	TInt  iTimeout;

    TBool iOpenReady;
    TBool iPrepareReady;
    TBool iRecordingReady;
    TBool iPaused;
    TBool iClosed;
    TBool i2ndTime;
    TBool iDoublePause;
    TBool iStartRecording2ndTime;
    TestClientVCdspActions iAction;
    TInt iError;
    
     TInt iUiLevelCameraHandle;
     
};


#endif

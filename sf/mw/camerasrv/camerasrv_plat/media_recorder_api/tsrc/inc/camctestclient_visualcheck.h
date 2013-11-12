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


#ifndef __CAMCTESTCLIENT_VISUALCHECK_H
#define __CAMCTESTCLIENT_VISUALCHECK_H


//  INCLUDES

#include <e32base.h>
#include "camcTest.h"

#include <mmf/common/mmfvideo.h>
#include <videorecorder.h>


// CONSTANTS

#define TCVC_VIDEO_BIT_RATE_10 TInt(64000)
#define TCVC_VIDEO_BIT_RATE_11 TInt(28000) 
#define TCVC_VIDEO_BIT_RATE_20 TInt(28000) 

// MACROS

// DATA TYPES

enum TestClientVCActions
    {
    K_VC_none,
        K_VC_test_case_101,
        K_VC_test_case_102,
        K_VC_test_case_103,
        K_VC_test_case_104,
        K_VC_test_case_105,
        K_VC_test_case_106,
        K_VC_test_case_107,
        K_VC_test_case_108_a,
        K_VC_test_case_108_b,
        K_VC_test_case_109,
        K_VC_test_case_110,
        K_VC_test_case_111,
        K_VC_test_case_112,
        K_VC_test_case_113,
        K_VC_test_case_114,
        K_VC_test_case_115,
        K_VC_test_case_116,
        K_VC_test_case_117,
        K_VC_test_case_118,
        K_VC_test_case_119,
        K_VC_test_case_120,
        K_VC_test_case_121
    };


// FUNCTION PROTOTYPES

// FORWARD DECLARATIONS

// CLASS DECLARATION




class CCamcTestClient_visualcheck : public CTimer /* also CActive*/, public MVideoRecorderUtilityObserver
{
public:
    static CCamcTestClient_visualcheck* NewL( );
    ~CCamcTestClient_visualcheck ();

    void Start_Active_ObjectL ( TestClientVCActions aAction );

    //from observer
    void MvruoOpenComplete(TInt aError);
    void MvruoPrepareComplete(TInt aError);
    void MvruoRecordComplete(TInt aError);
    void MvruoEvent(const TMMFEvent& /*aEvent*/);
    void SomethingInActiveObjectIsLeaving (TInt aError);


protected:

private:

    CCamcTestClient_visualcheck ();
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
    TestClientVCActions iAction;
    TInt iError;
};


#endif

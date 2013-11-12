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


#ifndef __CAMCTESTCLIENT_7_H
#define __CAMCTESTCLIENT_7_H


//  INCLUDES

#include <e32base.h>
#include "camcTest.h"

#include <videorecorder.h>
#include <ecam.h>


// CONSTANTS

#define TC7_CONTROLLER_UID 0x101f8503
#define TC7_VIDEO_FORMAT_UID 0x101f8504

// MACROS

// DATA TYPES

enum TestClient_7_Actions
    {
	K_Tc7_none,
    K_Tc7_ReleaseCameraWhileRecording,
    K_Tc7_PowerOffCameraWhileRecording,
    K_Tc7_ReleaseCameraCloseRecorderUseCamera,
    K_Tc7_OpenFileWhenCameraIsNotAvailable,
    K_Tc7_OpenFileWithVideoFormatNULL
    };

enum TCameraReleasedState
    {
    ENothingReleased,
    ECamcStopped,
    ECameraReleased
    };

// FUNCTION PROTOTYPES

// FORWARD DECLARATIONS

// CLASS DECLARATION


class CCamcTestClient_7 : public CTimer /* also CActive*/,
                          public MVideoRecorderUtilityObserver,
                          public MCameraObserver,
                          public MCameraObserver2

{
public:
    static CCamcTestClient_7* NewL( );
	~CCamcTestClient_7 ();

	void Start_Active_ObjectL ( TestClient_7_Actions aAction );

protected:
    //from MVideoRecorder observer
    void MvruoOpenComplete(TInt aError);
    void MvruoPrepareComplete(TInt aError);
    void MvruoRecordComplete(TInt aError);
    void MvruoEvent(const TMMFEvent& /*aEvent*/);
    void SomethingInActiveObjectIsLeaving (TInt aError);

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

private:

	CCamcTestClient_7 ();
    void ConstructL( );

    //from CActive
    void RunL();
    void RunLTrappedL();
    void DoCancel();

    CVideoRecorderUtility* iCamc;
	CCamera *iCamera, *iCamera2;
	CActiveSchedulerWait *iWaitScheduler;   

	TInt  iTimeout;

    TBool iOpenReady;
    TBool iPrepareReady;
    TBool iRecordingReady;
    TBool iPaused;
    TBool iClosed;
    TestClient_7_Actions iAction;
    TInt iError;
    TInt iCameraReleasedState;
};


#endif

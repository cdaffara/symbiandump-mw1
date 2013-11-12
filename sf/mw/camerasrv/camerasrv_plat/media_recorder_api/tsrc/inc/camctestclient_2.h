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


#ifndef __CAMCTESTCLIENT_2_H
#define __CAMCTESTCLIENT_2_H


//  INCLUDES

#include <e32base.h>
#include "camcTest.h"

#include <videorecorder.h>


// CONSTANTS

#define TC2_AUDIO_TYPE TFourCC(' ', 'A', 'M', 'R')
#define TC2_VIDEO_FRAME_RATE 10
#define TC2_VIDEO_FRAME_SIZE TSize(128,96)
#define TC2_CIF_VIDEO_FRAME_SIZE TSize(352,288)
#define TC2_VIDEO_BIT_RATE TInt(40000) 
#define TC2_AUDIO_BIT_RATE TInt(12200)
#define TC2_AUDIO_ENABLED EFalse
#define TC2_VIDEO_TYPE _L8("video/H263-2000")

#define TC2_VIDEO_TYPES_ARRAY  \
    {     _L8("video/H263-2000"), \
          _L8("video/H263-2000; profile=0"), \
          _L8("video/H263-2000; profile=0; level=10") }
#define TC2_VIDEO_TYPES_ARRAY_LENGTH 3

// MACROS

// DATA TYPES

// FUNCTION PROTOTYPES

// FORWARD DECLARATIONS

// CLASS DECLARATION 


class CCamcTestClient_2 : public CTimer /* also CActive*/, public MVideoRecorderUtilityObserver
{

public:
    
  
enum TestClient2Actions
    {
	Knone,
    KcloseWhenControllerReady,
	KcloseWhenControllerNotReady,
	KcloseWhenPrepareReady,
	KcloseWhenRecordingReady,
	KcloseWhenRecording,
    KcloseWhenPrepareReadyUsingAACandMPEG4,

	KprepareWhenControllerReady,
	KprepareWhenControllerNotReady,
	KprepareWhenPrepareReady,
	KprepareWhenRecordingReady,
	KprepareWhenRecording,
	KprepareWhenPrepareReadySetVideoAttributes,

	KrecordWhenControllerReady,
	KrecordWhenControllerNotReady,
	KrecordWhenPrepareReady,
	KrecordWhenRecordingReady,
	KrecordWhenRecording,

	KSetPriorityLWhenControllerReady,
	KSetPriorityLWhenControllerNotReady,	
	KSetPriorityLWhenPrepareReady,
	KSetPriorityLWhenRecording,

	KSetVideoFrameRateLWhenControllerReady,
	KSetVideoFrameRateLWhenControllerNotReady,	
	KSetVideoFrameRateLWhenPrepareReady,
	KSetVideoFrameRateLWhenRecording,
	
	KSetVideoFrameSizeLWhenControllerReady,
	KSetVideoFrameSizeLWhenControllerReady_MPEG4,
	KSetVideoFrameSizeLWhenControllerNotReady,	
	KSetVideoFrameSizeLWhenPrepareReady,
	KSetVideoFrameSizeLWhenRecording,

	KSetVideoBitRateLWhenControllerReady,
	KSetVideoBitRateLWhenControllerNotReady,	
	KSetVideoBitRateLWhenPrepareReady,
	KSetVideoBitRateLWhenRecording,
	
	KSetAudioBitRateLWhenControllerReady,
	KSetAudioBitRateLWhenControllerNotReady,	
	KSetAudioBitRateLWhenPrepareReady,
    KSetAudioBitRateLWhenRecording,
    
   	KSetAudioEnabledLWhenControllerReady,
    KSetAudioEnabledLWhenControllerNotReady,	
    KSetAudioEnabledLWhenPrepareReady,
    KSetAudioEnabledLWhenRecording,
    
    KSetMaxClipSizeLWhenControllerReady,
    KSetMaxClipSizeLWhenControllerNotReady,	
    KSetMaxClipSizeLWhenPrepareReady,
    KSetMaxClipSizeLWhenRecording,
    
    KSetVideoTypeLWhenControllerReady,
    KSetVideoTypeLWhenControllerNotReady,	
    KSetVideoTypeLWhenPrepareReady,
    KSetVideoTypeLWhenRecording,
    
    KSetAudioTypeLWhenControllerReady,
    KSetAudioTypeLWhenControllerNotReady,	
    KSetAudioTypeLWhenPrepareReady,
    KSetAudioTypeLWhenRecording
    };

    static CCamcTestClient_2* NewL( );
	~CCamcTestClient_2 ();

	void Start_Active_ObjectL(TestClient2Actions aAction, TInt aCameraHandle );

    //from observer
    void MvruoOpenComplete(TInt aError);
    void MvruoPrepareComplete(TInt aError);
    void MvruoRecordComplete(TInt aError);
    void MvruoEvent(const TMMFEvent& /*aEvent*/);
    void SomethingInActiveObjectIsLeaving (TInt aError);

protected:

private:


enum TestClient2LeavingCodes
    {
    KErrGetNotEqualToValuePreviouslySet = 98
    };

	CCamcTestClient_2 ();
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
    TestClient2Actions iAction;
    TestClient2Actions iSavedAction; //iSaved Action will never be changed.
    TInt iError;
    TBool iSecondTime;
};



#endif

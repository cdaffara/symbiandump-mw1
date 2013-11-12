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


#ifndef __CAMCTESTCLIENT_6_H
#define __CAMCTESTCLIENT_6_H


//  INCLUDES

#include <e32base.h>
#include <e32def.h>
#include "camcTest.h"

#include <mmf/common/mmfcontrollerpluginresolver.h>
#include <videorecorder.h>


// CONSTANTS

#define TC6_VIDEO_FRAME_SIZE TSize(128,96)

// Equivalent video types.
#define TC6_VIDEO_TYPES_ARRAY  \
    {     _L8("video/H263-2000"), \
          _L8("video/H263-2000; profile=0"), \
          _L8("video/H263-2000; profile=0; level=10") }
#define TC6_VIDEO_TYPES_ARRAY_NEW  \
    {     _L8("video/mp4v-es"), \
          _L8("video/mp4v-es; profile-level-id=8"), \
	      _L8("video/mp4v-es; profile-level-id=2"), \
	      _L8("video/mp4v-es; profile-level-id=3"), \
          _L8("video/mp4v-es; profile-level-id=1") }

#define TC6_VIDEO_TYPES_ARRAY_LENGTH 3
#define TC6_VIDEO_TYPES_ARRAY_LENGTH_NEW 5


#define TC6_VIDEO_TYPE _L8("video/H263-2000")
#define TC6_VIDEO_TYPE_PROFILE0 _L8("video/H263-2000; profile=0")
#define TC6_VIDEO_TYPE_PROFILE8 _L8("video/mp4v-es; profile-level-id=8")
#define TC6_VIDEO_TYPE_PROFILE3 _L8("video/mp4v-es; profile-level-id=3")
#define TC6_VIDEO_TYPE_WRONG _L8("video/H263-2000; profile=5")
// There is no other type supported currently,
// TC6_VIDEO_TYPE_DIFF = TC6_VIDEO_TYPE
//#define TC6_VIDEO_TYPE_DIFF _L8("video/mp4v-es")
#define TC6_AUDIO_TYPE TFourCC( ' ', 'A', 'M', 'R' )
#define TC6_AUDIO_TYPE_WRONG TFourCC( 0,0,0,0 )
#define TC6_AAC_AUDIO_TYPE TFourCC( ' ','A','A','C' )
#define TC6_FILE_SIZE_LIMIT 100000 // bytes

#define TC6_CIF_VIDEO_FRAME_SIZE TSize(352,288)

// MACROS

// DATA TYPES
enum TestClient6Actions
    {
	K_Tc6_none,
    K_Tc6_PauseWhenStopped,
    K_Tc6_SetVideoFrameSizeLWhenPaused,
    K_Tc6_SetAudioEnabledLWhenControllerReady,
    K_Tc6_SetMaxClipSizeLWhenControllerReady,
    K_Tc6_SetMaxClipSizeLWhenPrepareReady,
    K_Tc6_RecordWhenPrepareComplete,
    K_Tc6_RecordWhenPaused,
    K_Tc6_VideoTypeLWhenOpenWithMimeAndNotRecording,
    K_Tc6_VideoTypeLWhenOpenWithNULLAndNotRecording,
    K_Tc6_VideoTypeLWhenOpenWithMimeAndRecording,
    K_Tc6_VideoTypeLWhenOpenWithoutSpecifyingVideoTypeAndNotRecording,
    K_Tc6_AudioTypeLWhenOpenWithfourCCAndNotRecording,
    K_Tc6_AudioTypeLWhenOpenWithoutSpecifyingAudioTypeAndNotRecording,
    K_Tc6_AudioTypeLWhenOpenWithfourCCAndRecording,
    K_Tc6_SetVideoTypeLWhenOpenWithoutSpecifyingVideoTypeAndNotRecording,
    K_Tc6_SetVideoTypeLWhenOpenWithMimeAndNotRecording,
    K_Tc6_VideoTypeLWhenOpenWithMimeAndNotRecordingNewFormat,
    K_Tc6_SetVideoTypeLWhenNotRecording,
    K_Tc6_SetVideoTypeLWithProfile,
    K_Tc6_SetVideoTypeLWithProfile8,
    K_Tc6_SetVideoTypeLWithProfile3,
    K_Tc6_SetAudioTypeLWhenOpenWithoutSpecifyingAudioTypeAndNotRecording,
    K_Tc6_SetAudioTypeLWhenOpenWithfourCCAndNotRecording_AAC,
    K_Tc6_SetAudioTypeLWhenNotRecording,
    K_Tc6_ControllerImplementationInformationWhenNotOpen,
    K_Tc6_ControllerImplementationInformationWhenOpen, 
    K_Tc6_ControllerImplementationInformationWhenOpenNew, 
    K_Tc6_ControllerImplementationInformationWhenOpenNew_MPEG4, 
    K_Tc6_DurationWhenRecording,
    K_Tc6_DurationWhenNotRecording,
    K_Tc6_DurationWhenPaused
    };

enum TestClient6LeavingCodes
    {
    K_Tc6_ErrExpectingADifferentValue = 101,
    K_Tc6_ValueNotIncreasingErr,
    K_Tc6_ErrExpectingToLeave
    };

// FUNCTION PROTOTYPES

// FORWARD DECLARATIONS

// CLASS DECLARATION



class CCamcTestClient_6 : public CTimer /* also CActive*/, public MVideoRecorderUtilityObserver
{
public:
    static CCamcTestClient_6* NewL( );
	~CCamcTestClient_6 ();

	void Start_Active_ObjectL(TestClient6Actions aAction, TInt aCameraHandle );

    //from observer
    void MvruoOpenComplete(TInt aError);
    void MvruoPrepareComplete(TInt aError);
    void MvruoRecordComplete(TInt aError);
    void MvruoEvent(const TMMFEvent& /*aEvent*/);
    void SomethingInActiveObjectIsLeaving (TInt aError);
    void CheckControllerImplementationInformationL (const CMMFControllerImplementationInformation & aReturnedResult );
    void CheckControllerImplementationInformationNewL (const CMMFControllerImplementationInformation & aReturnedResult );
    void CheckControllerImplementationInformationNewMPEG4L (const CMMFControllerImplementationInformation & aReturnedResult );

protected:

private:

	CCamcTestClient_6 ();
    void ConstructL ();
   
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
    TestClient6Actions iAction;
    TestClient6Actions iSavedAction; //iSaved Action will never be changed.
    TInt iError;
    TInt iNumberOfTimeOut;
    TInt64 iPreviousTime;
    TTimeIntervalMicroSeconds iOldTimeDuration;
    TBool iResumeRecording;
    TInt iNumberOfTimeAsked;
    TTimeIntervalMicroSeconds iOldDuration;
};

#endif

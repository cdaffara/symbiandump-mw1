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


#ifndef __CAMCTESTCLIENT_5_H
#define __CAMCTESTCLIENT_5_H


//  INCLUDES

#include <e32base.h>
#include "camcTest.h"

#include <videorecorder.h>


// CONSTANTS

#define TC5_VIDEO_FRAME_RATE_WRONG 0
#define TC5_VIDEO_BIT_RATE_WRONG TInt(-50) // Voluntarily Incoherent value
#define TC5_AUDIO_BIT_RATE_WRONG TInt(1) // Voluntarily Incoherent value
#define TC5_AUDIO_ENABLED EFalse
#define TC5_VIDEO_FRAME_SIZE_WRONG TSize(128,0) // Voluntarily Incoherent value
#define TC5_CIF_VIDEO_FRAME_SIZE TSize(352,288)

// AMR
#define TC5_AUDIO_TYPE_1 TFourCC( ' ', 'A', 'M', 'R' )
//AAC
#define TC5_AUDIO_TYPE_3 TFourCC( ' ', 'A', 'A', 'C' )


// MACROS

// DATA TYPES

enum TestClient5Actions
    {
	K_Tc5_none,
	K_Tc5_NumberOfMetaDataEntriesWhenNotRecording,
	K_Tc5_NumberOfMetaDataEntriesWhenRecording,
    K_Tc5_MetaDataEntryWhenNotRecording,
    K_Tc5_MetaDataEntryWhenRecording,
    K_Tc5_AddMetaDataEntryWhenNotRecording,
    K_Tc5_AddMetaDataEntryWhenRecording,
    K_Tc5_RemoveMetaDataEntryWhenNotRecording,
    K_Tc5_RemoveMetaDataEntryWhenRecording,
    K_Tc5_ReplaceMetaDataEntryWhenNotRecording,
    K_Tc5_ReplaceMetaDataEntryWhenRecording,
    K_Tc5_SetPriorityWhenNotRecording,
    K_Tc5_GetPriorityWhenNotSet,
    K_Tc5_SetVideoFrameRateWhenOpenNotPrepared,
    K_Tc5_VideoFrameRateWhenNotSet,
    K_Tc5_SetVideoBitRateWhenOpenNotPrepared,
    K_Tc5_VideoBitRateWhenNotSet,
    K_Tc5_SetAudioBitRateWhenOpenNotPrepared,
    K_Tc5_AudioBitRateWhenNotSet,
    K_Tc5_SetAudioEnabledWhenOpenNotPrepared,
    K_Tc5_AudioEnabledWhenNotSet,
    K_Tc5_SetVideoFrameSizeWhenOpenNotPrepared,
    K_Tc5_GetVideoFrameSizeWhenNotSet,
    K_Tc5_PauseWhenRecording,
    K_Tc5_PauseWhenStopped,
    K_Tc5_SetMaxClipSizeLWhenNotRecording,
    K_Tc5_SetGainWhenNotRecording,
    K_Tc5_SetGainWhenNotRecordingAndSetWrong,
    K_Tc5_SetGainWhenRecording,
    K_Tc5_GainWhenNotSet,
    K_Tc5_MaxGainWhenNotRecording,
    K_Tc5_MaxGainWhenRecording,
    K_Tc5_StopWhenRecording,
    K_Tc5_StopWhenNotRecording,
    K_Tc5_StopWhenRecordingNewPrepareCorrect,
    K_Tc5_StopWhenRecordingAfterOpenFile,
    K_Tc5_StopWhenRecordingAfterOpenFileNew,
    K_Tc5_StopWhenRecordingAfterOpenFileNew_MPEG4,
    K_Tc5_GetSupportedVideoTypesWhenOpenNotRecording,
    K_Tc5_GetSupportedVideoTypesWhenOpenNotRecordingNew,
    K_Tc5_GetSupportedAudioTypesWhenOpenNotRecordingAMR,
    K_Tc5_GetSupportedAudioTypesWhenOpenNotRecordingAAC
    };

enum TestClient5LeavingCodes
    {
    KErrExpectingADifferentValue = 101
    };

// FUNCTION PROTOTYPES

// FORWARD DECLARATIONS

// CLASS DECLARATION

class CCamcTestClient_5 : public CTimer /* also CActive*/, public MVideoRecorderUtilityObserver
{
public:
    static CCamcTestClient_5* NewL( );
	~CCamcTestClient_5 ();

	void Start_Active_ObjectL(TestClient5Actions aAction, TInt aCameraHandle );

    //from observer
    void MvruoOpenComplete(TInt aError);
    void MvruoPrepareComplete(TInt aError);
    void MvruoRecordComplete(TInt aError);
    void MvruoEvent(const TMMFEvent& /*aEvent*/);
    void SomethingInActiveObjectIsLeaving (TInt aError);
    
protected:

private:

	CCamcTestClient_5 ();
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
    TBool iPrepare2ndTime;

    TestClient5Actions iAction;
    TestClient5Actions iSavedAction; //iSaved Action will never be changed.
    TInt iError;
    CMMFMetaDataEntry *iMMFMetaDataEntry;
    TInt iUiLevelCameraHandle;
};


#endif

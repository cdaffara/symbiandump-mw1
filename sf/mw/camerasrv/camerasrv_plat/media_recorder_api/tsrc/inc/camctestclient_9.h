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


#ifndef __CAMCTESTCLIENT_9_H
#define __CAMCTESTCLIENT_9_H


//  INCLUDES

#include <e32base.h>
#include <e32def.h>
#include "camcTest.h"

#include <videorecorder.h>


// CONSTANTS

#define TC9_VIDEO_BIT_RATE TInt(40000) 
#define TC9_AUDIO_BIT_RATE TInt(12200)
#define TC9_FILE_SIZE_LIMIT 50000
#define TC9_AUDIO_BIT_RATE_AAC TInt(32000)

// MACROS

// DATA TYPES

enum TestClient9Actions
    {
	K_Tc9_none,
    K_Tc9_PrepareWhenControllerReadyWhitoutSetting,
    K_Tc9_PrepareWhenControllerReadyOnlyVideoBitRateSetting,
    K_Tc9_PrepareWhenControllerReadyOnlyVideoFrameSizeSetting,
    K_Tc9_RecordTimeAvailWhenRecordingReady,
    K_Tc9_SetMaxClipSizeLWhenControllerReadySize0,
    K_Tc9_SetMaxClipSizeLWhenControllerReadyKMMFNoMaxClipSize,
    K_TC9_NewFileNameWhenPreparedNotRecording,
    K_TC9_NewFileNameWhenRecordSecondTime,
    K_TC9_NewFileNameWhenRecordSecondTimeDiffFileFormat,
    K_TC9_NewFileNameWhenRecordSecondTimeMPEG4FileFormat,
    K_TC9_NewFileNameWhenRecording,
    K_TC9_NewFileNameWhenPaused,
    K_TC9_NewFileNameWhenInternalStop,
    K_Tc9_OpenFileInAFolderNonExistent
    };

enum TestClient9LeavingCodes
    {
    K_Tc9_ValueNotWithinRange = 101,
    K_Tc9_ValueNotExpected
    };

// FUNCTION PROTOTYPES

// FORWARD DECLARATIONS

// CLASS DECLARATION



class CCamcTestClient_9 : public CTimer /* also CActive*/, public MVideoRecorderUtilityObserver
{
public:
    static CCamcTestClient_9* NewL( );
	~CCamcTestClient_9 ();

	void Start_Active_ObjectL ( TestClient9Actions aAction, TInt aCameraHandle );

    //from observer
    void MvruoOpenComplete(TInt aError);
    void MvruoPrepareComplete(TInt aError);
    void MvruoRecordComplete(TInt aError);
    void MvruoEvent(const TMMFEvent& /*aEvent*/);
    void SomethingInActiveObjectIsLeaving (TInt aError);

protected:

private:

	CCamcTestClient_9 ();
    void ConstructL( );

    //from CActive
    void RunL();
    void RunLTrappedL();
    void RunLContinuedL();
    void DoCancel();

    CVideoRecorderUtility* iCamc;
	
	TInt  iTimeout;

    TBool iOpenReady;
    TBool iPrepareReady;
    TBool iRecordingReady;
    TBool iPaused;
    TBool iClosed;
    TestClient9Actions iAction;
    TestClient9Actions iSavedAction; //iSaved Action will never be changed.
    TInt iError;
    TTimeIntervalMicroSeconds iOldTimeRemaining;
    TBool iNewFileName;
    TBool iRecord2ndTime;
    TBuf<256> iFileName;
};


#endif

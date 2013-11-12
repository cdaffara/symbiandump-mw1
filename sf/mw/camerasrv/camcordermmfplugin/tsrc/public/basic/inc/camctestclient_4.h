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


#ifndef __CAMCTESTCLIENT_4_H
#define __CAMCTESTCLIENT_4_H

//  INCLUDES

#include <e32base.h>
#include <e32def.h>
#include "Camctest.h"

#include <videorecorder.h>


// CONSTANTS

#define TC4_FILE_SIZE_LIMIT 250000 //250k bytes
#define TC4_VIDEO_BIT_RATE TInt(40000) 
#define TC4_AUDIO_BIT_RATE TInt(12200)

// MACROS

// DATA TYPES

// FUNCTION PROTOTYPES

// FORWARD DECLARATIONS

// CLASS DECLARATION



class CCamcTestClient_4 : public CTimer /* also CActive*/, public MVideoRecorderUtilityObserver
{
public:
    
    enum TestClient4Actions
        {
        K4RecordTimeAvailSizeLimit,
        K4RecordTimeAvailNoSizeLimit,
        K4RecordTimeAvailNotRecordingSizeLimit,  
        K4RecordTimeAvailNotRecordingNoSizeLimit,
        K4RecordTimeAvailUntilDiskFull
        };
    
    enum TestClient4LeavingCodes
        {
        K4UnexpectedErr = 99,
        K4ValueNotDecreasingErr,
        K4ValueNotWithinRange,
        K4ErrExpectingADifferentValue = 101
        };

    static CCamcTestClient_4* NewL( );
	~CCamcTestClient_4 ();

	void Start_Active_ObjectL(TestClient4Actions aAction, TInt aCameraHandle );

    //from observer
    void MvruoOpenComplete(TInt aError);
    void MvruoPrepareComplete(TInt aError);
    void MvruoRecordComplete(TInt aError);
    void MvruoEvent(const TMMFEvent& /*aEvent*/);
    void SomethingInActiveObjectIsLeaving (TInt aError);


protected:

private:

	CCamcTestClient_4 ();
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
    TestClient4Actions iAction;
    TestClient4Actions iSavedAction; //iSaved Action will never be changed.
    TInt iError;
    TInt iNumberOfTimeOut;
    TTimeIntervalMicroSeconds iOldTimeRemaining;
    TInt64 iTimeRemainingInSec;
};

#endif

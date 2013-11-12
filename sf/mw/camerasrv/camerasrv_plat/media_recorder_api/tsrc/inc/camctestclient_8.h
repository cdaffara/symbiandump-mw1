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


#ifndef __CAMCTESTCLIENT_8_H
#define __CAMCTESTCLIENT_8_H


//  INCLUDES

#include <e32base.h>
#include "camcTest.h"

#include <videorecorder.h>
#include <ecam.h>


// CONSTANTS

#define TC8_CONTROLLER_UID 0x101f8503
#define TC8_VIDEO_FORMAT_UID 0x101f8504

// MACROS

// DATA TYPES

enum TestClient_8_Actions
    {
    K_Tc8_MultipleInstance001_AO1,    
    K_Tc8_MultipleInstance001_AO2,

    K_Tc8_MultipleInstance002_AO1,    
    K_Tc8_MultipleInstance002_AO2,

    K_Tc8_MultipleInstance003_AO1,    
    K_Tc8_MultipleInstance003_AO2,
    
    K_Tc8_MultipleInstance004_AO1,    
    K_Tc8_MultipleInstance004_AO2,

    K_Tc8_MultipleInstance005_AO1,    
    K_Tc8_MultipleInstance005_AO2,

    K_Tc8_MultipleInstance006_AO1,    
    K_Tc8_MultipleInstance006_AO2

    };


// FUNCTION PROTOTYPES

// FORWARD DECLARATIONS

// CLASS DECLARATION



class CCamcTestClient_8 : public CTimer /* also CActive*/,
                          public MVideoRecorderUtilityObserver

{
public:
    static CCamcTestClient_8* NewL( const TDesC & aFileName, TInt *aError,
        TInt * aNumberOfOpenedCamc, TInt aCameraHandle);
	~CCamcTestClient_8 ();

	void Start_Active_Object(TestClient_8_Actions aAction,
                             CCamcTestClient_8 * aClient);
    void TellMeWhenYouAreInCorrectState(TRequestStatus &aRequest);
    void Close();


protected:
    //from MVideoRecorder observer
    void MvruoOpenComplete(TInt aError);
    void MvruoPrepareComplete(TInt aError);
    void MvruoRecordComplete(TInt aError);
    void MvruoEvent(const TMMFEvent& /*aEvent*/);

// Own functions
    void WaitForOtherObject();
    void SomethingInActiveObjectIsLeaving (TInt aError);

private:

	CCamcTestClient_8 ();
    void ConstructL( const TDesC &aFileName , TInt *aError,
        TInt * aNumberOfOpenedCamc, TInt aCameraHandle);

    //from CActive
    void RunL();
    void RunLTrappedL();
    void DoCancel();
    void ReachedWaitingState();

    CVideoRecorderUtility * iCamc;
    CCamcTestClient_8 * iOtherClient;
	TInt  iTimeout;

    TBool iOpenReady;
    TBool iPrepareReady;
    TBool iRecordingReady;
    TBool iPaused;
    TBool iClosed;
    TBool iReachedWaitingState;
    TestClient_8_Actions iAction;
    TInt *iError;
    TRequestStatus *iReceivedRequest;
    TBool iRequestPending; // The "server" has been requested for some thing,
                           // but it hasn't answered yet.  
    TBool iEmittedRequest; // The "client" has been requesting some thing.

    TBuf<100> iFileName;

    TInt *iNumberOfOpenedCamc;
    TBool iRecordingReadyTimer;
};

#endif

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


#ifndef __CAMCTESTCLIENT_3_H
#define __CAMCTESTCLIENT_3_H


//  INCLUDES


#include <e32base.h>
#include "Camctest.h"

#include <videorecorder.h>

// CONSTANTS

// MACROS

// DATA TYPES

// FUNCTION PROTOTYPES

// FORWARD DECLARATIONS

// CLASS DECLARATION





class CCamcTestClient_3 : public CTimer /* also CActive*/, public MVideoRecorderUtilityObserver
{
public:
    

    static CCamcTestClient_3* NewL(TInt aCameraHandle );
    ~CCamcTestClient_3 ();




protected:

private:

    enum TestClient3Actions
        {
        K_Tc3_none
        };
    CCamcTestClient_3 ();
    void ConstructL(TInt aCameraHandle );

    void Start_Active_ObjectL(TestClient3Actions aAction, TInt aCameraHandle );

    //from CActive
    void RunL();
    void RunLTrappedL();
    void DoCancel();

    //from observer
    void MvruoOpenComplete(TInt aError);
    void MvruoPrepareComplete(TInt aError);
    void MvruoRecordComplete(TInt aError);
    void MvruoEvent(const TMMFEvent& /*aEvent*/);
    void SomethingInActiveObjectIsLeaving (TInt aError);

    CVideoRecorderUtility* iCamc;
	
	TInt  iTimeout;

    TBool iOpenReady;
    TBool iPrepareReady;
    TBool iRecordingReady;
    TBool iPaused;
    TBool iClosed;
    TestClient3Actions iAction;
    TestClient3Actions iSavedAction; //iSaved Action will never be changed.
    TInt iError;
};

#endif

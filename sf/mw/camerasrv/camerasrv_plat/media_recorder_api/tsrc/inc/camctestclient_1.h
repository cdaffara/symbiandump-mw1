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


#ifndef __CAMCTESTCLIENT_1_H
#define __CAMCTESTCLIENT_1_H


//  INCLUDES

#include <e32base.h>
#include "camcTest.h"

#include <videorecorder.h>


// CONSTANTS

// MACROS

// DATA TYPES

// FUNCTION PROTOTYPES

// FORWARD DECLARATIONS

// CLASS DECLARATION

class CCamcTestClient_1 : public CTimer /* also CActive*/, public MVideoRecorderUtilityObserver
{
public:

    static CCamcTestClient_1* NewL( );
    ~CCamcTestClient_1 ();
	

    void Test_OpenFileL(const TDesC& aFileName, 
                        TInt aCameraHandle, 
                        TUid aControllerUid, 
                        TUid aVideoFormat,
                        const TDesC8 & aVideoType,
                        TFourCC aAudioType);
		
	void Test_OpenFileL(const TDesC& aFileName, 
									 TInt aCameraHandle, 
									 TUid aControllerUid, 
									 TUid aVideoFormat);


	void Test_OpenDesL(TDes8& aDescriptor, 
									 TInt aCameraHandle, 
									 TUid aControllerUid, 
									 TUid aVideoFormat,
									 const TDesC8 & aVideoType,
									 TFourCC aAudioType);
	
	void Test_OpenUrlL(const TDesC& aUrl, 
									 TInt aCameraHandle, 
									 TUid aControllerUid, 
									 TUid aVideoFormat,
									 const TDesC8 & aVideoType,
									 TFourCC aAudioType);
    
    TInt WaitForCallbackWithErrors();

    //from observer
    void MvruoOpenComplete(TInt aError);
    void MvruoPrepareComplete(TInt aError);
    void MvruoRecordComplete(TInt aError);
    void MvruoEvent(const TMMFEvent& /*aEvent*/);
    void SomethingInActiveObjectIsLeaving (TInt aError);


protected:


private:

	CCamcTestClient_1 ();
    void ConstructL();


    //from CActive
    void RunL();
    void RunLTrappedL();
    void DoCancel();

    CVideoRecorderUtility* iCamc;

    TInt  iNrPauses;
    TInt  iPauseCount;
	TInt  iTimeout;

    TBool iOpenReady;
    TBool iPrepareReady;
    TBool iRecordingReady;
    TBool iPaused;
    TInt iError;

    TBool iStopWhenOpenComplete;

};

#endif

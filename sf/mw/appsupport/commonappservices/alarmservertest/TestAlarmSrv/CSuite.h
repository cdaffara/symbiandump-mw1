// Copyright (c) 2003-2009 Nokia Corporation and/or its subsidiary(-ies).
// All rights reserved.
// This component and the accompanying materials are made available
// under the terms of "Eclipse Public License v1.0"
// which accompanies this distribution, and is available
// at the URL "http://www.eclipse.org/legal/epl-v10.html".
//
// Initial Contributors:
// Nokia Corporation - initial contribution.
//
// Contributors:
//
// Description:
// This file contains the CTestAlarmServer class.
// that represents our Test Server.This file also 
// contains the TASStartupSignaller class used for
// starting up the Alarm Server.
// 
//

#if (!defined CAP_Alarm_SERVER_H_)
#define CAP_Alarm_SERVER_H_

#include <e32base.h>
#include <e32svr.h>

#include "f32file.h"
#include "e32test.h"

#include <test/testexecutestepbase.h>
#include <test/testexecuteserverbase.h>

// Alarm Server headers. For starting Alarm Server, building the 
// correct parameters for IPCs etc.
#include "asshddefs.h"
#include "ascliclientutils.h"
#include "asshdalarm.h"
#include "asclisession.h"

// The two ways to get thru
#define API_RETVALUE_SERVERPANIC				0
#define API_RETVALUE_NOCAPERROR					0

// The only way to get rejected
#define API_RETVALUE_PERMISSIONDENIED			1
#define DEBUG_ONLY								1
#define DYNAMIC_IPC								2

// No. of Message slots
const TInt KAlarmServerAsynchronousSlotCount =	5;

class  CTestAlarmServer : public CTestServer
	{
public:
	static CTestAlarmServer* NewL();
	virtual CTestStep* CreateTestStep(const TDesC& aStepName);
	};

class CCapabilityTestStep : public CTestStep, public RSessionBase
	{

public:
	enum TDbgFns  
		{
		MarkHeapStart, 
		MarkHeapEnd, 
		CheckHeap, 
		FailNext, 
		ResetFailNext
		};
public:
	// This is for the scheduler test framework
	virtual TVerdict doTestStepPreambleL( void );
	virtual TVerdict doTestStepPostambleL( void );
	virtual enum TVerdict doTestStepL();

	// Stuff that derived classes need to implement
	virtual TVersion Version()=0;
	virtual TInt Exec_SendReceiveL()=0;

	TInt StartServer() ;
	
	TVerdict MainThread();

	TVerdict GetVerdict(TInt aAPIretValue);

 	TVerdict GetVerdict(TExitType aExit,TInt aInitRetValue, TInt aApiRetValue);

 	TInt TestDebugHeap(TInt* aDbgIPCNo);
 	
 	static TAlarmId GetAlarmId();

	// from CStep.h
	#define SR_MESSAGE_TYPE_CHNGED		2
	#define SR_MESSAGE_ID_CHNGED		10
	#define SR_ServerName_CHNGED		_L("FLogger server")

	#define SR_CAPTEST_INVERSE			0	

	CActiveScheduler*		iTestScheduler;
	
	/** The Server Name (eg: CommServer, EtelServer,FLogger server, etc)*/
	TBuf<100>				iSrServerName;	

	/** iServerPanic is a unique name from Server,but always truncated to KMaxExitCategoryName*/
	TBuf<100>				iServerPanic;

	/** Following flags influence inverse tests*/
	TBool					iExpectRejection;
	
	TUint32  				iStepCap;

	/** Is it Async or sync?*/
	TInt					iSrMessageType;

	/** It holds the IPC number*/
	TInt					iSrMessageId;

	/** Holds the cap mask for the message*/
	TUint32					iSrMessageMask;

	/** We name the child thread appended by the IPC_Number it tests*/
	TBuf<100>				iSrChildThread;		

	/** The flag informs the main thread whether the connection to the server was established*/
	volatile TBool			iSessionCreated;

	/** To find out if an Async message was completed and if then with what result?*/
	TRequestStatus			iSrRequestStatus;

	/** For an Sync message, nResult_SR get you the return value*/
	TInt					iResultSr;	

	/** Hold the retusn value from "CreateSession" API*/
	TInt					iResultServer;

	TInt					iResultC32;
		
 	TInt					iOptions;
   	
	/** The Child thread object*/
	RThread					iChildThread;	
	};

#endif /* CAP_Alarm_SERVER_H_ */


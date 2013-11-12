// Copyright (c) 2005-2009 Nokia Corporation and/or its subsidiary(-ies).
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
// Contains declaration of CTestMultipleAlarmsServer class, which represents
// the TEF test server
// 
//

#ifndef __TEST_MULTIPLE_ALARMS_SERVER_H__
#define __TEST_MULTIPLE_ALARMS_SERVER_H__

// User Includes

// System Includes
#include <test/testexecuteserverbase.h>
#include <test/testexecutestepbase.h>
#include <calsession.h>
#include <calentryview.h>
#include <uikon/eikalsrv.h>
#include <asshddefs.h>

/*@{*/
// Literal constant defined for calendar file
_LIT(KTestCalFile, "c:testcal.vcs");
/*@}*/

class CAlarmControlsManager;

/**
The test server.
@internalTechnology
@test
*/
class CTestMultipleAlarmsServer : public CTestServer
	{
public:
	// Construction
	static	CTestMultipleAlarmsServer*	NewL();

	// Destruction
	~CTestMultipleAlarmsServer();

	// Base class pure virtual
	virtual	CTestStep* 					CreateTestStep(const TDesC& aStepName);
	
	// Other functions used by the test steps
	RThread& 							AltSrvThread();
	CAlarmControlsManager*& 			AlarmControlsManager();
	CEikServAlarmAlertServer*&			AlertServer();
	TInt& 								CurrentMaxAlarms();
	TInt 								CurrentMaxAlarms() const;
	TInt& 								NoOfCurrentlyNotifyingAlarms();
	TAlarmId& 							CurrentlyPlayingAlarmId();
	TBool 								LogSessionShared();
	
private:
	void 								ConstructL();
	
private:
	CAlarmControlsManager* 				iAlmCtrlsMgr;
	CEikServAlarmAlertServer*			iAlertServer;
	TInt 								iCurrentMaxAlarms;	
	TInt 								iNoOfCurrentlyNotifyingAlarms;
	TAlarmId 							iCurrentlyPlayingAlarmId;
	RThread 							iAltSrvThread;
	TBool 								iLogSessionShared;
};

#endif		// __TEST_MULTIPLE_ALARMS_SERVER_H__


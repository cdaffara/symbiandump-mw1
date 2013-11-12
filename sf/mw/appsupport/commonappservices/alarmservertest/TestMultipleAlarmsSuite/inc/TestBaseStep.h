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
// Contains declaration of CTestBaseStep class, from which all the test steps
// are derived
// 
//

#ifndef __TEST_BASE_STEP_H__
#define __TEST_BASE_STEP_H__

// User Includes
#include "TestMultipleAlarmsServer.h"

/*@{*/
// Literal constant defined for identifying step name
_LIT(KTestBaseStep, "TestBaseStep");
/*@}*/

/*@{*/
// Literal constants defined for ini keys
_LIT(KIniMaxAlarms, "maxalarms");
_LIT(KIniAfter, "after");
_LIT(KIniAt, "at");
_LIT(KIniNumOfEntries, "numofentries");
_LIT(KIniSubsections, "subsections");
_LIT(KIniTypeOfEntry, "typeofentry");
_LIT(KIniGUId, "guid");
_LIT(KIniTypeOfAD, "typeofad");
_LIT(KIniContentType, "contenttype");
_LIT(KIniUrl, "url");
_LIT(KIniMime, "mime");
_LIT(KIniFileName, "filename");
_LIT(KIniAlarmMessage, "message");
_LIT(KIniAlarmSoundName, "alarmsoundname");
_LIT(KIniIsPlaying, "isplaying");
_LIT(KIniIsShowing, "isshowing");
_LIT(KIniIsSnoozed, "issnoozed");
_LIT(KIniExpectedCalls, "expectedcalls");
_LIT(KIniAlarmMinsFromNow, "alarmminsfromnow");
_LIT(KIniMinsToSnooze, "minstosnooze");
_LIT(KIniDataReadTime, "datareadtime");
_LIT(KIniNotificationTime, "notificationtime");
_LIT(KIniClearAll, "clearall");
/*@}*/	

/*@{*/
// Literal constant identifying a flag in the ini file
_LIT(KYes, "yes");
/*@}*/	

/**
The Base Test Step from which all other test steps are derived
@internalTechnology
@test
*/
class CTestBaseStep : public CTestStep
	{
public:
	// Construction
	CTestBaseStep(CTestMultipleAlarmsServer& aTestServer);
	
	// Base class virtual
	TVerdict 					doTestStepPostambleL();

	CTestMultipleAlarmsServer*	TestServer();
protected:
	// Functions useful to sub-classes
	TBool 						ReadStringsFromConfig(TRefByValue<const TDesC> aSection, ...);
	TBool 						ReadIntsFromConfig(TRefByValue<const TDesC> aSection, ...);
	void						IniProblem();
	void 						PrintIfError(const TInt& aError);
	void 						PrintIfError(const TDesC& aMessage, const TInt& aError);
	void 						GetAlarmContentFromFileL(const TDesC& aFileName, HBufC8*& aAlarmContentBuf);
	TBool 						GetBool(const TPtrC& aString);
	TReal32 					GetTReal32(const TPtrC& aString);
	void 						CheckAltSrvThreadDeathStatus();
	void 						StartActiveSchedAndWaitL(TRequestStatus& aStatus);
	void 						CheckTimeDifference(const TTime& aStartTime, const TTime& aEndTime, const TReal32 aMaxSecondsLimit);
	CCalSession* 				CreateAndInitializeCalSessionL(const TBool& aCreateNewFile = EFalse);
	CCalEntryView* 				CreateEntryViewL(CCalSession* aCalSession);
	
private:	
	/** Test server reference for sharing data between test steps */
	CTestMultipleAlarmsServer& iTestServer;
	};

#endif		// __TEST_BASE_STEP_H__

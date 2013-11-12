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
// Contains declaration of CTestAssociatedDataStep class, which is used for testing the 
// integrity of the rich data associated with an alarm.
// 
//

#ifndef __TEST_ASSOCIATED_DATA_STEP_H__
#define __TEST_ASSOCIATED_DATA_STEP_H__

// User Includes
#include "TestBaseStep.h"

// System Includes
#include <calcontent.h>

/*@{*/
// Literal constant defined for identifying step name
_LIT(KTestAssociatedDataStep, "TestAssociatedDataStep");

/*@}*/

/**
Test the integrity of the rich data associated with an alarm
@internalTechnology
@test
*/
class CTestAssociatedDataStep : public CTestBaseStep
	{
public:
	CTestAssociatedDataStep(CTestMultipleAlarmsServer& aTestServer);
	~CTestAssociatedDataStep();

	// TEF pure virtual
	TVerdict 				doTestStepL();
	
	// TEF virtual
	TVerdict 				doTestStepPreambleL();
	
	TBool 					GetEntryIdL(const TPtrC& aAlarmMessage, CCalEntryId*& aEntryId);
	TBool 					GetCalEntryL(CCalEntryId* aEntryId, CCalEntry*& aCalEntry, CCalSession* aCalSession);
	CCalContent* 			GetAlarmContentL(const CCalEntry* aCalEntry);

private:
	void 					TestAssociatedDataL(const TPtrC& aAlarmMessage, const TPtrC& aTypeOfAD);
	TBool 					FindCalEntryL(const RPointerArray<CCalEntry> aEntryList, const TCalTime& aRecurrenceId, CCalEntry*& aFoundEntry);
	void 					DoTestAssociatedDataL(CCalContent* aAlarmContent , const TPtrC& aTypeOfAD);
	void 					CheckAlarmContentValuesL(CCalContent*& aAlarmContent, CCalContent::TDisposition aExpectedDisposition, const TDesC& aExpectedMimeType, const TDesC8& aExpectedContent);
	void 					TestStoreFileNameL(CCalEntryId* aEntryId);
	void 					TestAlarmSoundNameL(const TPtrC& aAlarmMessage);

private:
	CActiveScheduler*		iActiveScheduler;
	};

#endif		// __TEST_ASSOCIATED_DATA_STEP_H__

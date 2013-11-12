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
// Contains declaration of CTestCreateCalEntriesStep class, which is used for 
// creating armed calendar entries
// 
//

#ifndef __TEST_CREATE_CAL_ENTRIES_STEP_H__
#define __TEST_CREATE_CAL_ENTRIES_STEP_H__

// User Includes
#include "TestBaseStep.h"

// System includes
#include <calentryview.h>

/*@{*/
// Literal constant defined for identifying step name
_LIT(KTestCreateCalEntriesStep, "TestCreateCalEntriesStep");
/*@}*/

/*@{*/
// Literal constants defined for error messages
_LIT(KCalSessionError, "Error occured while creating cal session: %D");
_LIT(KEntryViewError, "Error occured while creating entry view: %D");
_LIT(KCreateEntriesError, "Error occured while creating entries: %D");
/*@}*/

/*@{*/
// Literal constants defined for constructing sub-section names
_LIT(KDot, ".");
_LIT(KEntry, "entry");
/*@}*/

/*@{*/
// Literal constants that identify the type of entry in the in file
_LIT(KTodo, "todo");
_LIT(KEvent, "event");
_LIT(KReminder, "reminder");
_LIT(KAnniversary, "anniv");
/*@}*/	

/**
Test step to create a set of armed calendar entries
@internalTechnology
@test
*/
class CTestCreateCalEntriesStep : public CTestBaseStep
	{
public:
	// Construction
	CTestCreateCalEntriesStep(CTestMultipleAlarmsServer& aTestServer);
	
	// TEF pure virtual
	TVerdict 				doTestStepL();

protected:
	void 					CreateCalEntriesL();
		
private:
	void 					FillCalEntriesFromSubSectionsL(RPointerArray<CCalEntry>& aEntryArray, const TInt& aNumOfEntries);
	void 					FillCalEntriesAtWillL(RPointerArray<CCalEntry>& aEntryArray, const TInt& aNumOfEntries);
	void 					GetSubSectionName(HBufC*& aSubSectionName, const TInt& aSubSectionNo);
	TBool 					GetCalEntryDetails(const TDesC& aSectionName, TPtrC& aTypeOfEntry, TPtrC& aGUId, TPtrC& aAlarmMessage, TInt& aAlarmMinsFromNow);
	CCalEntry* 				CreateCalEntryL(const CCalEntry::TType& aTypeOfEntry, const TDesC& aGUId, const TDesC& aAlarmMessage);
	void 					SetCalTimesL(CCalEntry*& aEntry, const TTime aNow, const TInt& aAlarmMinsFromNow);
	CCalAlarm* 				CreateCalAlarmL(const TDesC& aSectionName, const TInt& aAlarmMinsFromNow);
	void 					SetAlarmContentL(const TDesC& aSectionName, CCalAlarm*& aCalAlarm);
	void 					DoSetAlarmContentL(const TPtrC& aTypeOfAD, const TDesC& aSectionName, CCalAlarm*& aCalAlarm);
	void					CompareAndPrintResult(const TInt& aTriedNum, const TInt& aActualNum);
	CCalEntry::TType 		GetEntryTypeL(const TDesC& aTypeString);
	
private:
	TCalTime				iCalStartTime;
	TCalTime				iCalEndTime;
	};

#endif		// __TEST_CREATE_CAL_ENTRIES_STEP_H__

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
// Contains implementation of CTestAssociatedDataStep class
// 
//

/**
 @file
 @internalTechnology 
*/

// User Include
#include "TestAssociatedDataStep.h"
#include "DummyAlarmControl.h"

// Syatem Includes
#include <uikon/eikalsup.h>
#include <calalarm.h>

/**
Constructor. Sets the test step name. Testserver reference passed to make use 
of TEF's method of sharing data between test steps
@internalTechnology
@test
*/
CTestAssociatedDataStep::CTestAssociatedDataStep(CTestMultipleAlarmsServer& aTestServer) 
: CTestBaseStep(aTestServer)
	{
	//Call base class method to set human readable name for test step
	SetTestStepName(KTestAssociatedDataStep);
	}
	
/**
Destructor
@internalTechnology
@test
*/
CTestAssociatedDataStep::~CTestAssociatedDataStep()
	{
	delete iActiveScheduler;
	}
	
/**
Base class virtual. Installs the active scheduler
@return		EPass or EFail indicating the result of the test step.
@internalTechnology
@test
*/
TVerdict CTestAssociatedDataStep::doTestStepPreambleL()
	{
	iActiveScheduler = 	new (ELeave) CActiveScheduler;
	CActiveScheduler::Install(iActiveScheduler);
	return TestStepResult();
	}

/**
Base class pure virtual.
@return		EPass or EFail indicating the result of the test step.
@internalTechnology
@test
*/
TVerdict CTestAssociatedDataStep::doTestStepL()
	{
	TPtrC alarmMessage;
	if(!ReadStringsFromConfig(ConfigSection(), &KIniAlarmMessage(), &alarmMessage, NULL))
		{
		IniProblem();
		}
	else
		{
		TPtrC typeOfAD;
		TRAPD(error, TestAlarmSoundNameL(alarmMessage));
		PrintIfError(error);
		if(!ReadStringsFromConfig(ConfigSection(), &KIniTypeOfAD(), &typeOfAD, NULL))
			{
			INFO_PRINTF2(_L("%S field not found in ini. Assuming there is no associated data for the entry"), &KIniTypeOfAD);
			}
		else
			{
			TRAP(error, TestAssociatedDataL(alarmMessage, typeOfAD));
			PrintIfError(error);
			}	
		}
	return TestStepResult();	
	}
	
/**
Retreives the alarm control object and tests the associated data of the alarm 
whose message is the one that is passed as parameter
@param aAlarmMessage Message to identify the alarm control object
@param aTypeOfAD The type of Associated Data, i.e. url or mime
@internalTechnology
@test
*/
void CTestAssociatedDataStep::TestAssociatedDataL(const TPtrC& aAlarmMessage, const TPtrC& aTypeOfAD)
	{
	CCalEntryId* entryId = NULL;
	if(!GetEntryIdL(aAlarmMessage, entryId))
		{
		ERR_PRINTF2(_L("No associated data attached with the alarm %S ... Failing the test"), &aAlarmMessage);
		SetTestStepResult(EFail);
		}
	else
		{
		CCalEntry* calEntry = NULL;
		CleanupStack::PushL(entryId);
		
		TestStoreFileNameL(entryId);
		
		CCalSession* calSession = CreateAndInitializeCalSessionL(EFalse);
		CleanupStack::PushL(calSession);
		
		if(!GetCalEntryL(entryId, calEntry, calSession))
			{
			ERR_PRINTF2(_L("The calendar entry was not found for %S. Failing the test..."), &aAlarmMessage);
			SetTestStepResult(EFail);
			}
		else
			{
			CleanupStack::PushL(calEntry);
			CCalAlarm* alarm = calEntry->AlarmL();
			if(!alarm->AlarmAction())
				{
				ERR_PRINTF2(_L("Alarm content is NULL for %S. Failing the test..."), &aAlarmMessage);
				SetTestStepResult(EFail);
				}
			else
				{
				CleanupStack::PushL(alarm);
				DoTestAssociatedDataL(alarm->AlarmAction(), aTypeOfAD);
				CleanupStack::PopAndDestroy(alarm);
				}	
			CleanupStack::PopAndDestroy(calEntry);	
			}
		CleanupStack::PopAndDestroy(2, entryId);	// calSession and entryId
		}	
	}
	
/**
Retreives the raw data from the alert server and constructs the CCalEntryId
that contains the details about the entry, which owns the alarm corresponding 
to the message passed as parameter.
@param aAlarmMessage The message associated with the alarm
@param aEntryId Pointer to the object that will hold the data describing the 
entry
@return The sucess or failure. EFalse if entry does not have associated data
@internalTechnology
@test
*/
TBool CTestAssociatedDataStep::GetEntryIdL(const TPtrC& aAlarmMessage, CCalEntryId*& aEntryId)
	{
	CDummyAlarmControl* alarmControl = dynamic_cast<CDummyAlarmControl*>(TestServer()->AlarmControlsManager()->GetAlarmControlObjectL(aAlarmMessage));
	if(alarmControl->AlarmObject().HasAssociatedData())
		{
		// Get the data from the supervisor
		HBufC8* data = HBufC8::NewL(alarmControl->Supervisor()->AlarmData().Length());
		*data = alarmControl->Supervisor()->AlarmData();
		CleanupStack::PushL(data);
		
		aEntryId = CCalEntryId::NewL(data);
		
		// CCalEntryId takes ownership
		CleanupStack::Pop(data);
		return ETrue;
		}
		
	return EFalse;
	}

/**
Retreives the calendar entry corresponding to the data contained in the
CCalEntryId passed as parameter
@param aEntryId The pointer to the object that contains the id details
of the entry
@param aCalEntry The pointer to hold the cal-entry if found
@return The sucess or failure of the find operation
@internalTechnology
@test
*/
TBool CTestAssociatedDataStep::GetCalEntryL(CCalEntryId* aEntryId, CCalEntry*& aCalEntry, CCalSession* aCalSession)
	{
	// Get the calendar entry.
	RPointerArray<CCalEntry> entryList;
	
	CCalEntryView* calEntryView = CreateEntryViewL(aCalSession);
	CleanupStack::PushL(calEntryView);
	
	calEntryView->FetchL(aEntryId->IdL(), entryList);
	
	TBool found = FindCalEntryL(entryList, aEntryId->RecurrenceIdL(), aCalEntry);
	
	CleanupStack::PopAndDestroy(calEntryView);
	return found;
	}
	
/**
Not used, to be removed
Retreives the data associated with the entry passed as paraemeter
@param aCalEntry The pointer to the cal-entry.
@return A pointer to the CCalAlarmContent
@internalTechnology
@test
*/
CCalContent* CTestAssociatedDataStep::GetAlarmContentL(const CCalEntry* aCalEntry)
	{
	// Get the rich data
	return aCalEntry->AlarmL()->AlarmAction();
	}
	
/**
Retreives the data associated with the entry passed as paraemeter and checks it
@param aAlarmContent The pointer to the rich data.
@param aTypeOfAD The type of Associated Data, i.e. url or mime
@internalTechnology
@test
*/
void CTestAssociatedDataStep::DoTestAssociatedDataL(CCalContent* aAlarmContent , const TPtrC& aTypeOfAD)
	{
	TPtrC mimeType;
	if(ReadStringsFromConfig(ConfigSection(), &KIniContentType(), &mimeType, NULL) == EFalse)
		{
		IniProblem();
		return;
		};

	HBufC8* content8 = NULL;
	CCalContent::TDisposition disposition = CCalContent::EDispositionUnknown;
	if(aTypeOfAD.Compare(KIniUrl) == 0)
		{
		TPtrC url;
		if(ReadStringsFromConfig(ConfigSection(), &KIniUrl(), &url, NULL) == EFalse)
			{
			IniProblem();
			return;
			};
		// Convert read url from unicode to 8-bit data.
		content8 = HBufC8::NewL(url.Length());
		content8->Des().Copy(url);
		disposition = CCalContent::EDispositionUrl;
		}
	else if(aTypeOfAD.Compare(KIniMime) == KErrNone)
		{
		TPtrC fileName;
		if(ReadStringsFromConfig(ConfigSection(), &KIniFileName(), &fileName, NULL) == EFalse)
			{
			IniProblem();
			return;
			};
		GetAlarmContentFromFileL(fileName, content8);
		disposition = CCalContent::EDispositionInline;
		}
	CleanupStack::PushL(content8);
	
	CheckAlarmContentValuesL(aAlarmContent, disposition, mimeType, *content8);
	CleanupStack::PopAndDestroy(content8);
	}

/**
Checks data associated with the entry passed as paraemeter against a set of 
expected values
@param aAlarmContent The reference to a pointer to the rich data
@param aExpectedValueType Expected content-value type
@param aExpectedContentType Expected content type
@param aExpectedDataContent8 Expected data content in an 8-bit descriptor
@internalTechnology
@test
*/
void CTestAssociatedDataStep::CheckAlarmContentValuesL(CCalContent*& aAlarmContent, CCalContent::TDisposition aExpectedDisposition, const TDesC& aExpectedMimeType, const TDesC8& aExpectedContent)
	{
	INFO_PRINTF3(_L("Testing content disposition. Expected = %D, Actual = %D."), aExpectedDisposition, aAlarmContent->Disposition());
	TEST1(aAlarmContent->Disposition() == aExpectedDisposition, ETrue);
	
	// Convert 8-bit MIME type to unicode for logging...
	HBufC* mimeType = HBufC::NewLC(aAlarmContent->MimeType().Length());
	mimeType->Des().Copy(aAlarmContent->MimeType());
	INFO_PRINTF3(_L("Testing MIME type. Expected = %S, Actual = %S."), &aExpectedMimeType, mimeType);
	TEST1(mimeType->Compare(aExpectedMimeType) == KErrNone, ETrue);
	CleanupStack::PopAndDestroy(mimeType);

	INFO_PRINTF1(_L("Testing data content"));
	TEST1(aAlarmContent->Content().Compare(aExpectedContent) == KErrNone, ETrue);
	}

/**
Finds the calendar entry checking for the sequence no. and recurrence ids
@param aEntryList The list of entries to be checked
@param aRecurrenceId The recurrence id of the entry
@param aFoundEntry The reference to a pointer to the entry which will 
be set to point to the found entry
@internalTechnology
@return Success status
@test
*/
TBool CTestAssociatedDataStep::FindCalEntryL(RPointerArray<CCalEntry> aEntryList, const TCalTime& aRecurrenceId, CCalEntry*& aFoundEntry)
	{
	TInt index = 0;
	while(aEntryList.Count() > 0)
		{
		aFoundEntry = aEntryList[index];
		aEntryList.Remove(index);
		TCalTime calTime = aFoundEntry->RecurrenceIdL();
		if (calTime.TimeUtcL() == aRecurrenceId.TimeUtcL())
			{
			++index;
			}
		else
			{
			delete aFoundEntry;
			aFoundEntry = NULL;
			}
		}
	aEntryList.Reset();	
	return (aFoundEntry ? ETrue : EFalse);
	}

/**
Tests whether the Store File Name of the calendar entry is as expected
@param aEntryId Pointer to the object that has the entry details
@internalTechnology
@test
*/
void CTestAssociatedDataStep::TestStoreFileNameL(CCalEntryId* aEntryId)
	{
	TPtrC storeFileName(aEntryId->StoreFileNameL());
	INFO_PRINTF3(_L("Testing store file name. Expected = %S, Actual  = %S."), &KTestCalFile, &storeFileName);
	TEST1(storeFileName.Compare(KTestCalFile) == KErrNone, ETrue);
	}
	
/**
Tests whether the Alarm Sound Name of the alarm is as expected
@param aAlarmMessage The message associated with the alarm
@internalTechnology
@test
*/
void CTestAssociatedDataStep::TestAlarmSoundNameL(const TPtrC& aAlarmMessage)
	{
	TPtrC alarmSoundName;
	CDummyAlarmControl* alarmControl = dynamic_cast<CDummyAlarmControl*>(TestServer()->AlarmControlsManager()->GetAlarmControlObjectL(aAlarmMessage));
	
	if(ReadStringsFromConfig(ConfigSection(), &KIniAlarmSoundName(), &alarmSoundName, NULL))
		{
		INFO_PRINTF3(_L("Testing alarm sound name. Expected = %S, Actual  = %S."), &alarmSoundName, &(alarmControl->AlarmSoundName()));
		TEST1(alarmSoundName.Compare(alarmControl->AlarmSoundName()) == KErrNone, ETrue);
		}
	else
		{
		INFO_PRINTF1(_L("Testing alarm sound name to be NULL descriptor"));
		TEST1(KNullDesC().Compare(alarmControl->AlarmSoundName()) == KErrNone, ETrue);
		}	
	}
	

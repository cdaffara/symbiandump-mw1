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
// Contains implementation of CTestCreateCalEntriesStep class
// 
//

/**
 @file
 @internalTechnology 
*/

// User Include
#include "TestCreateCalEntriesStep.h"
#include "ProgressCallBack.h"

// System Includes
#include <calalarm.h>
#include <calcontent.h>

/**
Constructor. Sets the test step name. Testserver reference passed to make use 
of TEF's method of sharing data between test steps
@internalTechnology
@test
*/
CTestCreateCalEntriesStep::CTestCreateCalEntriesStep(CTestMultipleAlarmsServer& aTestServer) 
: CTestBaseStep(aTestServer)
	{
	//Call base class method to set human readable name for test step
	SetTestStepName(KTestCreateCalEntriesStep);
	}
	
/**
Call back to destroy the pointer array
@param aPtr Pointer to the pointer array
@internalTechnology
@test
*/
LOCAL_C void DestroyPointerArray(TAny* aPtr)
	{
	RPointerArray<CCalEntry>* pointerArray = static_cast<RPointerArray<CCalEntry>*> (aPtr);
	pointerArray->ResetAndDestroy();
	pointerArray->Close();
	}
	
/**
Base class pure virtual.
@return		EPass or EFail indicating the result of the test step.
@internalTechnology
@test
*/
TVerdict CTestCreateCalEntriesStep::doTestStepL()
	{
	CActiveScheduler* sched = new(ELeave) CActiveScheduler;
	CActiveScheduler::Install(sched);
	
	TRAPD(error, CreateCalEntriesL());
	PrintIfError(KCreateEntriesError(), error);

	delete sched;	
	return TestStepResult();	
	}

/**
Creates the calendar entries
@internalTechnology
@test
*/
void CTestCreateCalEntriesStep::CreateCalEntriesL()
	{
	TInt numOfEntries = 0;
	TPtrC subsections;

	CCalSession* calSession = CreateAndInitializeCalSessionL(ETrue);
	CleanupStack::PushL(calSession);
	CCalEntryView* calEntryView = CreateEntryViewL(calSession);
	CleanupStack::PushL(calEntryView);
	
	ReadIntsFromConfig(ConfigSection(), &KIniNumOfEntries(), &numOfEntries, NULL);
	ReadStringsFromConfig(ConfigSection(), &KIniSubsections(), &subsections, NULL);
	if(numOfEntries <= 0)
		{
		IniProblem();
		}
	else
		{
		RPointerArray<CCalEntry> entryArray;
		CleanupStack::PushL(TCleanupItem(DestroyPointerArray, &entryArray));
		
		// Fill the array with entries
		if(subsections.Compare(KYes) == KErrNone)
			{
			FillCalEntriesFromSubSectionsL(entryArray, numOfEntries);
			}
		else
			{
			FillCalEntriesAtWillL(entryArray, numOfEntries);	
			}	
		
		// Store the entries
		TInt actualNumOfEntries;
		calEntryView->StoreL(entryArray, actualNumOfEntries);
		// Check result
		CompareAndPrintResult(numOfEntries, actualNumOfEntries);
		CleanupStack::PopAndDestroy(&entryArray);
		}	
	CleanupStack::PopAndDestroy(2, calSession);	// calEntryView and calSession
	}
	
/**
Constructs the sub-section name
@param aSubSectionName The descriptor which is to hold the sub-sec name
@param aSubSectionNo The sub section number
@internalTechnology
@test
*/
void CTestCreateCalEntriesStep::GetSubSectionName(HBufC*& aSubSectionName, const TInt& aSubSectionNo)
	{
	aSubSectionName->Des().Copy(ConfigSection());
	aSubSectionName->Des().Append(KDot);
	aSubSectionName->Des().Append(KEntry);
	aSubSectionName->Des().AppendNum(aSubSectionNo);
	}
	
	
/**
Reads the ini sub-sections, initializes the calendar entries and fills the pointer 
array passed as parameter
@param aEntryArray Pointer array to be filled with the entries
@param aNumOfEntries No. of entries to be created, so as to loop through the
ini sub-sections
@internalTechnology
@test
*/
void CTestCreateCalEntriesStep::FillCalEntriesFromSubSectionsL(RPointerArray<CCalEntry>& aEntryArray, const TInt& aNumOfEntries)
	{
	TPtrC typeOfEntry;
	TPtrC gUId;
	TPtrC alarmMessage;
	TInt alarmMinsFromNow = 0;
	
	TTime now;
	now.HomeTime();
				
	// Create a HBufC enough to hold the 'sub-section' name
	// which looks like "<section-name>.entry<#>"
	HBufC* sectionName = HBufC::NewLC(ConfigSection().Length() + KEntry().Length() + 4);
	for(TInt sub = 1; sub <= aNumOfEntries; ++sub)
		{
		GetSubSectionName(sectionName, sub);
		if(GetCalEntryDetails(*sectionName, typeOfEntry, gUId, alarmMessage, alarmMinsFromNow))
			{
			CCalEntry* entry = CreateCalEntryL(GetEntryTypeL(typeOfEntry), gUId, alarmMessage);
			CleanupStack::PushL(entry);
			if(entry != NULL)	
				{
				aEntryArray.AppendL(entry);
				}
			// The memory pointed to by entry will be destroyed when this
			// array is destroyed, which is already in the CleanupStack	
			CleanupStack::Pop(entry);	
			
			// Set alarm
			SetCalTimesL(entry, now, alarmMinsFromNow);
			CCalAlarm* calAlarm = CreateCalAlarmL(*sectionName, alarmMinsFromNow);
			CleanupStack::PushL(calAlarm);

			entry->SetAlarmL(calAlarm);

			CleanupStack::PopAndDestroy(calAlarm);
			}
		else
			{
			IniProblem(); 
			// We still dont return as other sub-sections might be proper.
			// The test step result will be EFail though
			}	
		}
	CleanupStack::PopAndDestroy(sectionName);	
	}
	
/**
Creates a set of test entries. Can be used to create a large no. of
entries arbitrarily without having to take the pain of mentioning the
details of all the entries in the ini file.
@param aEntryArray Pointer array to be filled with the entries
@param aNumOfEntries No. of entries to be created, so as to loop through the
ini sub-sections
@internalTechnology
@test
*/
void CTestCreateCalEntriesStep::FillCalEntriesAtWillL(RPointerArray<CCalEntry>& aEntryArray, const TInt& aNumOfEntries)
	{
	_LIT(KTestGUId, "testguid%D@test.com");
	_LIT(KTestMessage, "testentry%D");
		
	CCalEntry::TType typeOfEntry;
	TBuf<30> gUId;
	TBuf<15> alarmMessage;
	TInt alarmMinsFromNow = 1;
	
	// In this case, same calalarm will be used by all the entries
	CCalAlarm* calAlarm = CreateCalAlarmL(ConfigSection(), alarmMinsFromNow);
	CleanupStack::PushL(calAlarm);
	
	TTime now;
	now.HomeTime();
	
	for(TInt num = 1; num <= aNumOfEntries; ++num)
		{
		// typeofentry will alternate between the 5 types of entries
		typeOfEntry = static_cast<CCalEntry::TType>(num % CCalEntry::EAnniv); 
		
		gUId.Format(KTestGUId, num);
		alarmMessage.Format(KTestMessage, num);

		CCalEntry* entry = CreateCalEntryL(typeOfEntry, gUId, alarmMessage);
		CleanupStack::PushL(entry);
		aEntryArray.AppendL(entry);
		
		// The memory pointed to by entry will be destroyed when the
		// array is destroyed, which is already in the CleanupStack	
		CleanupStack::Pop(entry);	
				
		SetCalTimesL(entry, now, alarmMinsFromNow);
		
		entry->SetAlarmL(calAlarm);
		
		}
	CleanupStack::PopAndDestroy(calAlarm);	
	}
	
/**
Reads the specified values from ini
@param aSectionName The section name to read from
@param aTypeOfEntry Will hold the type of entry (appt/entry/todo/event)
@param aGUId Will hold the GUID of the entry
@param aAlarmMessage Will hold the message associated with the alarm
@param aAlarmMinsFromNow Will hold the minutes from now, when the alarm has to 
expire
@internalTechnology
@test
*/
TBool CTestCreateCalEntriesStep::GetCalEntryDetails(const TDesC& aSectionName, TPtrC& aTypeOfEntry, TPtrC& aGUId, TPtrC& aAlarmMessage, TInt& aAlarmMinsFromNow)
	{
	if(ReadStringsFromConfig(aSectionName, &KIniTypeOfEntry(), &aTypeOfEntry, 
												  &KIniGUId(), &aGUId, 
												  &KIniAlarmMessage(), &aAlarmMessage, NULL)
	&& ReadIntsFromConfig(aSectionName, &KIniAlarmMinsFromNow(), &aAlarmMinsFromNow, NULL))
		{
		return ETrue;
		}
	return EFalse;	
	}
	
/**
Creates a calendar entry
@param aTypeOfEntry The type of entry
@param aGUId The GUID of the entry
@param aAlarmMessage The Summary of the entry
@internalTechnology
@test
*/
CCalEntry* CTestCreateCalEntriesStep::CreateCalEntryL(const CCalEntry::TType& aTypeOfEntry, const TDesC& aGUId, const TDesC& aAlarmMessage)
	{
	HBufC8* gUID8 = HBufC8::NewLC(aGUId.Length());
	gUID8->Des().Copy(aGUId);
	CCalEntry* entry = CCalEntry::NewL(aTypeOfEntry, gUID8, CCalEntry::EMethodNone, 0);
	CleanupStack::Pop(gUID8);
	CleanupStack::PushL(entry);

	// The summary will be displayed when alarm is playing.
	// This summary is considered as the one that uniquely identifies the 
	// entry/alarm for the purpose of this testing. Hence the message of
	// each entry in the ini file must be unique.
	entry->SetSummaryL(aAlarmMessage);
	
	CleanupStack::Pop(entry);
	return entry;
	}

/**
Sets the start and end time of the entry and the alarm, all relative to the
current time, so that the alarms are imminent ones
@param aEntry Reference to a pointer to the entry
@param aNow The time value, on which the calculation is to be based upon
@param aAlarmMinsFromNow The number of minutes after which we desire the alarm 
to expire. For example if aAlarmMinsFromNow = 1, then start time of the entry will
be calculated as 2 minutes from NOW and alarm offset will be 1 minute before 
the start time
@internalTechnology
@test
*/
void CTestCreateCalEntriesStep::SetCalTimesL(CCalEntry*& aEntry, const TTime aNow, const TInt& aAlarmMinsFromNow)
	{
	iCalStartTime.SetTimeLocalL(aNow + TTimeIntervalMinutes(2 * aAlarmMinsFromNow));
	if(aEntry->EntryTypeL() == CCalEntry::ETodo)
		{
		iCalEndTime = iCalStartTime; 
		}
	else
		{
		iCalEndTime.SetTimeLocalL(iCalStartTime.TimeLocalL() + TTimeIntervalHours(1));
		}
		
	aEntry->SetStartAndEndTimeL(iCalStartTime, iCalEndTime);
	}
	
/**
Creates the cal-alarm
@param aSectionName Section name in the ini
@param aAlarmMinsFromNow The number of minutes after which we desire the alarm 
to expire. 
@return Pointer to the cal-alarm
@internalTechnology
@test
*/
CCalAlarm* CTestCreateCalEntriesStep::CreateCalAlarmL(const TDesC& aSectionName, const TInt& aAlarmMinsFromNow)
	{
	TPtrC aAlarmSoundName;
	CCalAlarm* calAlarm = CCalAlarm::NewL();
	CleanupStack::PushL(calAlarm);
	calAlarm->SetTimeOffset(aAlarmMinsFromNow);
	
	if(ReadStringsFromConfig(aSectionName, &KIniAlarmSoundName(), &aAlarmSoundName, NULL))
		{
		calAlarm->SetAlarmSoundNameL(aAlarmSoundName);
		}
	
	SetAlarmContentL(aSectionName, calAlarm);
	
	CleanupStack::Pop(calAlarm);
	return calAlarm;
	}
	
/**
Attaches the associated data for the alarm if require ini field exists, 
else simply returns
@param aSectionName Section name in the ini
@param aCalAlarm Reference to a pointer to the CalAlarm
@internalTechnology
@test
*/
void CTestCreateCalEntriesStep::SetAlarmContentL(const TDesC& aSectionName, CCalAlarm*& aCalAlarm)
	{
	TPtrC typeOfAD;
	if(!ReadStringsFromConfig(aSectionName, &KIniTypeOfAD(), &typeOfAD, NULL))
		{// No associated data to be attached
		return;
		}
	else
		{
		INFO_PRINTF1(_L("Attaching associated data..."));
		DoSetAlarmContentL(typeOfAD, aSectionName, aCalAlarm);
		}	
	}
	
/**
Attaches the associated data for the alarm
If the type of AD is URL, it attaches the URL value passed from the ini
If it is MIME, then it expects a filename from the ini file, from which data will
be read into a descriptor and attached with the alarm
@param aTypeOfAD Type of associated data, can be either url or mime
@param aSectionName Section name in the ini
@param aCalAlarm Reference to a pointer to the CalAlarm
@internalTechnology
@test
*/
void CTestCreateCalEntriesStep::DoSetAlarmContentL(const TPtrC& aTypeOfAD, const TDesC& aSectionName, CCalAlarm*& aCalAlarm)	
	{
	TPtrC mimeType;
	if(!ReadStringsFromConfig(aSectionName, &KIniContentType(), &mimeType, NULL))
		{
		IniProblem();
		return;
		};
	// Convert read MIME type from unicode to 8-bit data.
	HBufC8* mimeType8 = HBufC8::NewL(mimeType.Length());
	mimeType8->Des().Copy(mimeType);
	CleanupStack::PushL(mimeType8);

	HBufC8* content8 = NULL;
	CCalContent::TDisposition disposition = CCalContent::EDispositionUnknown;
	if(aTypeOfAD.Compare(KIniUrl) == 0)
		{
		TPtrC url;
		if(!ReadStringsFromConfig(aSectionName, &KIniUrl(), &url, NULL))
			{
			CleanupStack::PopAndDestroy(mimeType8);
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
		if(!ReadStringsFromConfig(aSectionName, &KIniFileName(), &fileName, NULL))
			{
			CleanupStack::PopAndDestroy(mimeType8);
			IniProblem();
			return;
			};
		GetAlarmContentFromFileL(fileName, content8);
		disposition = CCalContent::EDispositionInline;
		}
	CleanupStack::PushL(content8);

	CCalContent* alarmAction = CCalContent::NewL();
	CleanupStack::PushL(alarmAction);
	alarmAction->SetContentL(content8, mimeType8, disposition); //Takes ownership of content8 & mimeType8
	CleanupStack::Pop(alarmAction);
	CleanupStack::Pop(content8);
	CleanupStack::Pop(mimeType8);
	
	aCalAlarm->SetAlarmAction(alarmAction); // Takes ownership of alarmAction
	}

/**
Returns the appropriate enumeration based on the entry type string passed
@param aTypeString The entry type string
@return The appropriate enumeration 
@internalTechnology
@test
*/
CCalEntry::TType CTestCreateCalEntriesStep::GetEntryTypeL(const TDesC& aTypeString)
	{
	CCalEntry::TType entryType = CCalEntry::EAppt;
	HBufC* typeBuf = aTypeString.AllocL();
	typeBuf->Des().LowerCase();
	if(typeBuf->Des().Compare(KTodo) == KErrNone)
		{
		entryType = CCalEntry::ETodo;
		}
	else if(typeBuf->Des().Compare(KEvent) == KErrNone)
		{
		entryType = CCalEntry::EEvent;
		}
	else if(typeBuf->Des().Compare(KReminder) == KErrNone)
		{
		entryType = CCalEntry::EReminder;
		}	
	else if(typeBuf->Des().Compare(KAnniversary) == KErrNone)
		{
		entryType = CCalEntry::EAnniv;
		}	
	delete typeBuf;
	return entryType;
	}
	
/**
Compares the no. of entries we tried to create with that of the actual
entries created and prints the result
@param aTriedNum The no. of entries we tried to create
@param aActualNum The no. of successful entries actually created
@internalTechnology
@test
*/
void CTestCreateCalEntriesStep::CompareAndPrintResult(const TInt& aTriedNum, const TInt& aActualNum)
	{
	INFO_PRINTF3(_L("Num of entries tried to create = %D, actually created = %D"), aTriedNum, aActualNum);
	if(aTriedNum != aActualNum)
		{
		INFO_PRINTF1(_L("Could not create one or more entries...Failing the test"));
		SetTestStepResult(EFail);
		}
	}

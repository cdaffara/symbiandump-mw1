// Copyright (c) 2004-2009 Nokia Corporation and/or its subsidiary(-ies).
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
//

#include "DstIntUtils.h"
#include <tz.h>

#include <caldataexchange.h>
#include <caldataformat.h>
#include <s32file.h>

TInt CDstIntUtils::SetHomeTimeZoneL(const TDesC8& aLocation, const RTz &aServer)
	{
	CTzId *timezoneid = CTzId::NewL(aLocation);
	CleanupStack::PushL(timezoneid);
	TInt err;
	TRAP(err, aServer.SetTimeZoneL(*timezoneid));
	CleanupStack::PopAndDestroy(timezoneid);

	return err;
	}

TInt CDstIntUtils::CheckHomeTimeZoneL(const RTz &aServer, const TDesC8& aLocation)
	{
	TInt err = KErrBadName;
	CTzId *timezoneid = aServer.GetTimeZoneIdL();
	if(timezoneid->TimeZoneNameID() == aLocation)
		{
		err = KErrNone;
		}
	delete timezoneid;
	return err;
	}


void CDstIntUtils::AppendTimeParamStringL(TTime aTime, TDes& aParamString)
	{
	// This writes TTime according to a format specification, and this takes produces proper
	// month and day values (ie they start with 1)
	TBuf<15> dateString;
	_LIT(KDateFormat, "%F%Y%M%D:%H%T%S");
	aTime.FormatL(dateString, KDateFormat);
	aParamString.Append(dateString);
	}

void ResetAndDestroyArrayOfEntries(TAny* aObject)
	{
	RPointerArray<CCalEntry>* array=static_cast<RPointerArray<CCalEntry>*>(aObject);
	if (array)
		{
		array->ResetAndDestroy();
		}
	}

void CDstIntUtils::OpenAgendaFileL(CCalSession& aSession, const TDesC& aFilename)
	{
	// Create the agenda file if does not exist
	TRAPD(err, aSession.CreateCalFileL(aFilename));
	if (err != KErrAlreadyExists)
		{
		User::LeaveIfError(err);
		}
		
	// Now open it	
	aSession.OpenL(aFilename);
	}


TCalLocalUid CDstIntUtils::ImportVCalL(RFs& aFs, CCalSession& aSession, CCalEntryView& aEntryView, const TDesC& aFileName)
	{
	RFileReadStream readStream;
	User::LeaveIfError(readStream.Open(aFs, aFileName, EFileRead));
	CleanupClosePushL(readStream);

	// Create ptr array for new entries
	RPointerArray<CCalEntry> entryArray;
	CleanupStack::PushL(TCleanupItem(ResetAndDestroyArrayOfEntries, &entryArray));

	CCalDataExchange* dataExchange = CCalDataExchange::NewL(aSession);
	CleanupStack::PushL(dataExchange);
	dataExchange->ImportL(KUidVCalendar, readStream, entryArray);

	TInt elements = entryArray.Count();
	TCalLocalUid id = 0;

	if(elements > 0)
		{
		CCalEntry* entry = entryArray[0];
		// add the first entry only
		while (entryArray.Count() > 1)
			{
			delete entryArray[1];
			entryArray.Remove(1);
			}
		TInt success = 0;
		aEntryView.StoreL(entryArray, success);
		id = entryArray[0]->LocalUidL();
		}

	CleanupStack::PopAndDestroy(dataExchange);
	CleanupStack::PopAndDestroy();	// ResetAndDestroyArrayOfEntries(entryArray)
	CleanupStack::PopAndDestroy();	// readStream.Close()

	return id;
	}


TInt CDstIntUtils::CheckAgendaEntryL(CCalEntryView& aEntryView, const TCalLocalUid& aId, const TTime& aStart, const TTime& aEnd)
	{
	TInt err = KErrArgument;
	CCalEntry* entry = aEntryView.FetchL(aId);
	CleanupStack::PushL(entry);

	TTime start = entry->StartTimeL().TimeLocalL();
	TTime end   = entry->EndTimeL().TimeLocalL();

#ifdef _DEBUG
	TDateTime startDateTime = start.DateTime();
	TDateTime aStartDateTime = aStart.DateTime();
	TDateTime endDateTime = end.DateTime();
	TDateTime aEndDateTime = aEnd.DateTime();
#endif

	if((aStart == start) && (aEnd == end))
		{
		err = KErrNone;
		}
	
	CleanupStack::PopAndDestroy(entry);
	return err;
	}

void CDstIntUtils::GenerateVCalL(RFs& aFs, const TDesC& aFileName, const TDesC& aStartTime, const TDesC& aEndTime)
	{
	RFile file;
	file.Replace(aFs, aFileName, EFileWrite);

	TBuf8<64> buf;

	file.Write(_L8("BEGIN:VCALENDAR\r\n"));
	file.Write(_L8("VERSION:1.0\r\n"));
	file.Write(_L8("BEGIN:VEVENT\r\n"));
	file.Write(_L8("CATEGORIES:MEETING\r\n"));
	file.Write(_L8("DTSTART:"));
	buf.Copy(aStartTime);
	file.Write(buf);
	file.Write(_L8("\r\n"));
	file.Write(_L8("DTEND:"));
	buf.Copy(aEndTime);
	file.Write(buf);
	file.Write(_L8("\r\n"));
	file.Write(_L8("SUMMARY:TestMeeting\r\n"));
	file.Write(_L8("DESCRIPTION:Test\r\n"));
	file.Write(_L8("END:VEVENT\r\n"));
	file.Write(_L8("END:VCALENDAR\r\n"));

	file.Flush();
	file.Close();
	}

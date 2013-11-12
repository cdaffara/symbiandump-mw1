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

/**
 @file ImportVCal.cpp
*/

#include "ImportVcal.h"
#include "DstIntUtils.h"

_LIT(KVCalFilename, "c:\\dstscripts\\vcalgen.vcs");

CImportVCal::~CImportVCal()
	{
	iTimeZoneServer.Close();
	}

CImportVCal::CImportVCal()
	{
	SetTestStepName(KImportVCal);
	}

void CImportVCal::PrepareTestEnvironmentL()
	{
	// Read parameters
	_LIT(KTagDTSTART,	"DTSTART");
	_LIT(KTagDTEND,		"DTEND");
	_LIT(KTagTimeStart,	"TimeStart");
	_LIT(KTagTimeEnd,	"TimeEnd");

	TPtrC	  strPtr;
	
	// Read parameters
	GetStringFromConfig(ConfigSection(), KTagLocation, strPtr);
	iLocation.Copy(strPtr);
	GetIntFromConfig(ConfigSection(), KTagLocationId, iLocationId);

	GetStringFromConfig(ConfigSection(), KTagDTSTART, strPtr);
	iDtStart.Copy(strPtr);
	GetStringFromConfig(ConfigSection(), KTagDTEND, strPtr);
	iDtEnd.Copy(strPtr);
	GetStringFromConfig(ConfigSection(), KTagTimeStart, strPtr);
	iTimeStart = CDstStep::ReadTimeParamStringL(strPtr);
	GetStringFromConfig(ConfigSection(), KTagTimeEnd, strPtr);
	iTimeEnd = CDstStep::ReadTimeParamStringL(strPtr);	
	}

TInt CImportVCal::CreateAndRunTestUnitL()
	{
	TInt err = KErrNone;
	
	// Instantiate Converter
	User::LeaveIfError(iTimeZoneServer.Connect());
  	err = CDstIntUtils::SetHomeTimeZoneL(iLocation, iTimeZoneServer);
  	if(err == KErrNone)
  		{
		// Instantiate File server
		RFs fs;
		User::LeaveIfError(fs.Connect());
		CleanupClosePushL(fs);
		
		// Instantiate Agenda Server and Model
		CCalSession* session = CCalSession::NewL();
		CleanupStack::PushL(session);

		// Get the default name for the agenda file
		const TDesC& KDefaultFilename = session->DefaultFileNameL();

		CDstIntUtils::OpenAgendaFileL(*session, KDefaultFilename);
		
		CCalEntryView* entryView = CCalEntryView::NewL(*session, *this);
		CleanupStack::PushL(entryView);
		
		CActiveScheduler::Start();	// wait for entry view to finish construction	
		
		// Load the vCal and check it for correct conversion
		CDstIntUtils::GenerateVCalL(fs, KVCalFilename, iDtStart, iDtEnd);
		TCalLocalUid id = CDstIntUtils::ImportVCalL(fs, *session, *entryView, KVCalFilename);
		
		if(id == 0)
			{
			_LIT(KNullIdTxt, "Null ID returned when attempting vCal import");
			err = KErrGeneral;
			LogCheck(KNullIdTxt, err);
			}
		else
			{
			err = CDstIntUtils::CheckAgendaEntryL(*entryView, id, iTimeStart, iTimeEnd);
			if(err != KErrNone)
				{
				_LIT(KBadConversion, "Bad conversion detected");
				LogCheck(KBadConversion, err);
				}
			
			RArray<TCalLocalUid> uids;
			CleanupClosePushL(uids);
			uids.AppendL(id);
			TInt success = 0;
			entryView->DeleteL(uids, success);	
			CleanupStack::PopAndDestroy(&uids);
			}
		
		fs.Delete(KVCalFilename);
		
		// Destroy
		CleanupStack::PopAndDestroy(entryView);
		CleanupStack::PopAndDestroy(session);
		CleanupStack::PopAndDestroy(); // fs.Close()
  		}
  	else
  		{
  		_LIT(KInvalidTimeZone, "Invalid time zone");
  		LogCheck(KInvalidTimeZone, err);
  		}
		
	return err;
	}
	
	
void CImportVCal::Progress(TInt )
	{
	}

void CImportVCal::Completed(TInt )
	{
	CActiveScheduler::Stop();
	}
	
TBool CImportVCal::NotifyProgress()
	{
	return EFalse;
	}

void CImportVCal::DestroyTestUnit()
	{
	iTimeZoneServer.Close();
	}

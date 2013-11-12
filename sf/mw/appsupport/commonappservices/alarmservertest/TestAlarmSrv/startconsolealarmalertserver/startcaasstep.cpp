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
//

#include "startcaasstep.h"
#include <test/testexecutelog.h>

#include <asaltdefs.h>
#include <consolealarmalertservermain.h>


const TUid KServerUid2={0x1000008D};
const TUid KServerUid3={0x101F502A};
const TUidType serverUid(KNullUid,KServerUid2,KServerUid3);
_LIT(KConsoleAlarmAlertServerImg,"ConsoleAlarmAlertServer");	

_LIT(KCAASAlreadyRunning, "ConsoleAlarmAlertServer is already running");	
_LIT(KCAASStartSuccess, "ConsoleAlarmAlertServer started successfully");
_LIT(KCAASLoadFail, "ConsoleAlarmAlertServer failed to load");
_LIT(KCAASFailAfterStartupAttempt, \
	"Failed to create a RASAltClientSession even after a CAAS startup attempt");


CStartCAASStep::~CStartCAASStep()
	{
	}


CStartCAASStep::CStartCAASStep()
	{
	SetTestStepName(KStepNameStartCAAS);
	}


TVerdict CStartCAASStep::doTestStepPreambleL()
	{
	SetTestStepResult(EFail);
	return TestStepResult();
	}


TVerdict CStartCAASStep::doTestStepL()
	{
	// Check if CAAS is already running
	if ( IsCAASRunning() )
		{
		// If it is, then we are fine: return EPass
		INFO_PRINTF1(KCAASAlreadyRunning);		
		SetTestStepResult(EPass);		
		return TestStepResult();		
		}

	// If not, then try to start it once
	if ( StartCAASL() != EPass )
		{
		// here server image couldn't be loaded
		INFO_PRINTF1(KCAASLoadFail);
		SetTestStepResult(EFail);
		return TestStepResult();		
		}
	
	// As everything's fine until now, check if it is running this time
	if ( !IsCAASRunning() )
		{
		INFO_PRINTF1(KCAASFailAfterStartupAttempt);
		SetTestStepResult(EFail);
		return TestStepResult();
		}
	
	INFO_PRINTF1(KCAASStartSuccess);		
	SetTestStepResult(EPass); 

	return TestStepResult();
	}


TVerdict CStartCAASStep::doTestStepPostambleL()
	{
	return TestStepResult();
	}


/**
 *	CStartCAASStep::StartCAAS()
 *
 *	Start the ConsoleAlarmAlertServer
 *	@return		EPass if ok, or EFail otherwise
 */
TVerdict CStartCAASStep::StartCAASL()
	{
	// Start the ConsoleAlarmAlertServer


	RProcess server;
	TInt r=server.Create(KConsoleAlarmAlertServerImg,KNullDesC,serverUid);

	TRequestStatus status;
	server.Rendezvous(status);
	if (status != KRequestPending)	
		{
		server.Kill(0);		// abort startup
		return EFail;
		}

	server.Resume();	// logon OK - start the server

	User::After(1000000);	// wait 1 sec while CAAS kicks off
	//User::WaitForRequest(status);	// alternate code to wait for CAAS to start


	return EPass;
	}
	
	
TBool CStartCAASStep::IsCAASRunning()
	{
	TFullName name;
	TFindServer finder(KAlarmAlertServerName);
	return  (finder.Next(name) == KErrNone);
	}

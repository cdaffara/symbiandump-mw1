// Copyright (c) 1999-2009 Nokia Corporation and/or its subsidiary(-ies).
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

#include <consolealarmalertservermain.h>
#include <asaltdefs.h>
#include "ConsoleAlarmAlertServer.h"

TInt ConsoleAlarmAlertServer::StartServerThreadFunction(TAny* instructionSet)
	{
	__UHEAP_MARK;
	//
	CTrapCleanup* cleanup = CTrapCleanup::New();
	if	(!cleanup)
		User::Invariant();
	//
	TRAPD(err, StartServerL(instructionSet));
	delete cleanup;
	//
	__UHEAP_MARKEND;
	return err;
	}

void ConsoleAlarmAlertServer::StartServerL(TAny* instructionSet)
	{
	CActiveScheduler* scheduler = new CActiveScheduler();
	CleanupStack::PushL(scheduler);
	CActiveScheduler::Install(scheduler);
	//

	CConsoleAlarmAlertServer * tmp = CConsoleAlarmAlertServer::NewLC();
	tmp->AddResponseArray(instructionSet);
	RProcess::Rendezvous(KErrNone);
	CActiveScheduler::Start();
	//
	CleanupStack::PopAndDestroy(2, scheduler);
	}

GLDEF_C TInt E32Main()
	{
	TBuf<256> cmdline;
	RArray< TInt > buff;
	TInt i, len, ret = KErrNone;

	User::CommandLine(cmdline);
	len = User::CommandLineLength();
	if (len)
		{
		for(i=0;i<len;i++)
			{
		    ret = buff.Append(cmdline[i]);
		    if (ret != KErrNone)
		    	{
		    	return ret;
		    	}
			}
		ret = ConsoleAlarmAlertServer::StartServerThreadFunction(&buff);
        }
    else
		{
		ret = ConsoleAlarmAlertServer::StartServerThreadFunction();
        }

	return ret;
	}

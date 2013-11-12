// Copyright (c) 1997-2009 Nokia Corporation and/or its subsidiary(-ies).
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

#include "crectestlogger.h"

_LIT(KTxtLogPath, "t_recog");
_LIT(KTxtLogFilename, "t_recogtest.log");

CRecTestLogger* CRecTestLogger::NewL()
	{
	CRecTestLogger* self = new(ELeave) CRecTestLogger();
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self;
	}
	
CRecTestLogger::~CRecTestLogger()
	{
	iLogger.CloseLog();
	iLogger.Close();
	}
	
CRecTestLogger::CRecTestLogger()
	{
	}
	
void CRecTestLogger::ConstructL()
	{
	User::LeaveIfError(iLogger.Connect());
	iLogger.SetDateAndTime(ETrue, ETrue);
	iLogger.CreateLog(KTxtLogPath(), KTxtLogFilename(), EFileLoggingModeOverwrite);
	_LIT8(KTxtLogHeader, "Starting Application-Protocols Recogniser Tests...");
	iLogger.Write(KTxtLogHeader());
	}

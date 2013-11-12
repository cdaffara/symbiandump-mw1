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

#include "VWSDEBUG.H"

#ifdef __DO_LOGGING__


//#define VIEWSRV_SHOW_TRACE		
#ifdef VIEWSRV_SHOW_TRACE
void ShowTrace(TRefByValue<const TDesC> aFmt,...)
	{
	RDebug::Print(aFmt);
	}
#else
void ShowTrace(TRefByValue<const TDesC> /*aFmt*/,...)
	{
	}
#endif

//
// Constants.
//

const TInt KMaxTraceLength=256;


//
// CVwsLog.
//

void CVwsLog::StartLogL()
	{
	CVwsLog* self=new(ELeave) CVwsLog;
	TRAPD(err,self->ConstructL());
	if (!err)
		{
		Dll::SetTls(self);
		CVwsLog::Log(CVwsLog::ENormal,_L("View Server log created"));
		}
	}

void CVwsLog::ShutdownLog()
	{
	CVwsLog::Log(CVwsLog::ENormal,_L("View Server log shutdown"));
	CVwsLog* self=STATIC_CAST(CVwsLog*,Dll::Tls());
	ASSERT(self);
	delete self;
	Dll::SetTls(NULL);
	}

CVwsLog::CVwsLog() : iType(EFile),iLevel(ELoud)
	{
	}

CVwsLog::~CVwsLog()
	{
	iLogFile.Close();
	iFs.Close();
	}

void CVwsLog::ConstructL()
	{
	TUint att;
	
	RBuf dataLogs;
	RBuf viewLog;
	RBuf viewLogOne;
	RBuf viewLogTwo;
	
	TChar sysDrive = RFs::GetSystemDriveChar();
	_LIT(KDataLogs,":\\Data\\logs");
	_LIT(KlogView,":\\Data\\logs\\viewsrv.txt");
	_LIT(KLogViewOne,":\\Data\\logs\\viewsrv1.txt");
	_LIT(KLogViewTwo,":\\Data\\logs\\viewsrv2.txt");
	TInt maxSizeofFileName = KDataLogs().Length() + 1;
	dataLogs.CreateL(maxSizeofFileName);
	dataLogs.CleanupClosePushL();
	dataLogs.Append(sysDrive);
	dataLogs.Append(KDataLogs());
	
	maxSizeofFileName = KlogView().Length() + 1;
	viewLog.CreateL(maxSizeofFileName);
	viewLog.CleanupClosePushL();
	viewLog.Append(sysDrive);
	viewLog.Append(KlogView());
	
	maxSizeofFileName = KLogViewOne().Length() + 1;
	viewLogOne.CreateL(maxSizeofFileName);
	viewLogOne.CleanupClosePushL();
	viewLogOne.Append(sysDrive);
	viewLogOne.Append(KLogViewOne());
	
	maxSizeofFileName = KLogViewTwo().Length() + 1;
	viewLogTwo.CreateL(maxSizeofFileName);
	viewLogTwo.CleanupClosePushL();
	viewLogTwo.Append(sysDrive);
	viewLogTwo.Append(KLogViewTwo());

	// If the log file can be opened,logging is active
	if (iFs.Connect() == KErrNone && iFs.Att(dataLogs,att) == KErrNone)
		{
		// Make a copy of previous log file
		iFs.Delete(viewLogTwo);
		iFs.Rename(viewLogOne,viewLogTwo);
		iFs.Rename(viewLog,viewLogOne);

		if (iLogFile.Replace(iFs, viewLog, EFileShareAny | EFileWrite | EFileStreamText) == KErrNone)
			{
			iActive = ETrue;
			}

		TUint16 feffInt=0xFEFF;
		User::LeaveIfError(iLogFile.Write(TPtrC8((TUint8 *)&feffInt,2)));
		iEol16=TPtrC16((TUint16 *)_S("\r\n"));
		iEol.Set((TUint8 *)iEol16.Ptr(),iEol16.Size());
		}

	CleanupStack::PopAndDestroy(&viewLogTwo);
	CleanupStack::PopAndDestroy(&viewLogOne);
	CleanupStack::PopAndDestroy(&viewLog);
	CleanupStack::PopAndDestroy(&dataLogs);
	}

void CVwsLog::SetLevel(TLevel aLogLevel)
	{
	iLevel=aLogLevel;
	}

void CVwsLog::SetType(TLogType aType)
	{
	iType=aType;
	}

void CVwsLog::Log(TLevel aLogLevel,TRefByValue<const TDesC> aFmt,...)
	{
	CVwsLog* self=STATIC_CAST(CVwsLog*,Dll::Tls());
	ASSERT(self);
	VA_LIST list;
	VA_START(list,aFmt);
	TBuf<KMaxTraceLength> buf;
	buf.FormatList(aFmt,list);
	self->DoLog(aLogLevel,buf);
	}

void CVwsLog::DoLog(TLevel aLogLevel,const TDesC& aOutput)
	{
	if (aLogLevel>iLevel)
		{
		return;
		}

	switch (iType)
		{
		case EFile:
			LogToFile(aLogLevel,aOutput);
			break;
		case ERDebug:
			LogToRDebug(aOutput);
			break;
		case EBoth:
			LogToFile(aLogLevel,aOutput);
			LogToRDebug(aOutput);
			break;
		case ENeither:
			break;
		}
	}

void CVwsLog::LogToFile(TLevel aLogLevel,const TDesC& aOutput)
	{
	if (iActive)
		{
		for (TInt ii=0;ii<aLogLevel;ii++)
			{
			TUint16 tab=0x0009;
			iLogFile.Write(TPtrC8((TUint8*)&tab,2));
			}
		iLogFile.Write(TPtrC8((TUint8 *)aOutput.Ptr(),aOutput.Size()));
		iLogFile.Write(iEol);
		iLogFile.Flush();
		}
	}

void CVwsLog::LogToRDebug(const TDesC& aOutput)
	{
	TBuf<KMaxTraceLength> buf;
	buf.Insert(0,_L("VIEWSRV: "));
	buf.Append(aOutput);
	ShowTrace(buf);
	}

#endif

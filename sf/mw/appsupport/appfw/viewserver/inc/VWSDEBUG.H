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

#ifndef __VWSDEBUG_H__
#define __VWSDEBUG_H__

#include <e32svr.h>
#include <f32file.h>

#ifdef _DEBUG
#define __DO_LOGGING__
#endif

#ifndef __DO_LOGGING__

#define LOG1(_p1)
#define LOG2(_p1,_p2)
#define LOG3(_p1,_p2,_p3)
#define LOG4(_p1,_p2,_p3,_p4)
#define LOG5(_p1,_p2,_p3,_p4,_p5)
#define LOG6(_p1,_p2,_p3,_p4,_p5,_p6)
#define LOG7(_p1,_p2,_p3,_p4,_p5,_p6,_p7)
#define LOG8(_p1,_p2,_p3,_p4,_p5,_p6,_p7,_p8)

#else

#define LOG1(_p1) CVwsLog::Log(_p1)
#define LOG2(_p1,_p2) CVwsLog::Log(_p1,_p2)
#define LOG3(_p1,_p2,_p3) CVwsLog::Log(_p1,_p2,_p3)
#define LOG4(_p1,_p2,_p3,_p4) CVwsLog::Log(_p1,_p2,_p3,_p4)
#define LOG5(_p1,_p2,_p3,_p4,_p5) CVwsLog::Log(_p1,_p2,_p3,_p4,_p5)
#define LOG6(_p1,_p2,_p3,_p4,_p5,_p6) CVwsLog::Log(_p1,_p2,_p3,_p4,_p5,_p6)
#define LOG7(_p1,_p2,_p3,_p4,_p5,_p6,_p7) CVwsLog::Log(_p1,_p2,_p3,_p4,_p5,_p6,_p7)
#define LOG8(_p1,_p2,_p3,_p4,_p5,_p6,_p7,_p8) CVwsLog::Log(_p1,_p2,_p3,_p4,_p5,_p6,_p7,_p8)


/**
 * The CVwsLog class implements debug logging for the view server.
 */ 
NONSHARABLE_CLASS(CVwsLog) : public CBase
	{
public:
	enum TLogType
		{
		EFile,
		ERDebug,
		EBoth,
		ENeither
		};
	enum TLevel
		{
		ESpecial,
		EQuiet,
		ENormal,
		ELoud,
		EShouting
		};
public:
	static void StartLogL();
	static void ShutdownLog();
	static void Log(TLevel aLogLevel,TRefByValue<const TDesC> aFmt,...);
	void SetLevel(TLevel aLogLevel);
	void SetType(TLogType aType);
private:
	CVwsLog();
	~CVwsLog();
	void ConstructL();
	void DoLog(TLevel aLogLevel,const TDesC& aOutput);
	void LogToFile(TLevel aLogLevel,const TDesC& aOutput);
	void LogToRDebug(const TDesC& aOutput);
private:
	RFs iFs;
	RFile iLogFile;
	TLogType iType;
	TBool iActive;
	TLevel iLevel;
	TPtrC8 iEol;
	TBuf16<2> iEol16;
	};

#endif
#endif

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

#ifndef __CRECTESTLOGGER_H__
#define __CRECTESTLOGGER_H__

#include <e32base.h>
#include <flogger.h>

class CRecTestLogger : public CBase
	{
public:
	static CRecTestLogger* NewL();
	virtual ~CRecTestLogger();
	
	inline RFileLogger& Logger();
	
private:
	CRecTestLogger();
	void ConstructL();
	
private:
	RFileLogger iLogger;
	};

inline
RFileLogger& CRecTestLogger::Logger()
	{
	return iLogger;
	}

#endif // __CRECTESTLOGGER_H__

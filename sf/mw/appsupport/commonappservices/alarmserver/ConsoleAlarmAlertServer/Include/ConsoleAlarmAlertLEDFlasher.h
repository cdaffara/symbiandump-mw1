/**
* Copyright (c) 1999-2009 Nokia Corporation and/or its subsidiary(-ies).
* All rights reserved.
* This component and the accompanying materials are made available
* under the terms of "Eclipse Public License v1.0"
* which accompanies this distribution, and is available
* at the URL "http://www.eclipse.org/legal/epl-v10.html".
*
* Initial Contributors:
* Nokia Corporation - initial contribution.
*
* Contributors:
*
* Description:
*
*/



#ifndef __CONSOLEALARMALERTLEDFLASHER_H__
#define __CONSOLEALARMALERTLEDFLASHER_H__

#include <e32base.h>

class CConsoleAlarmAlertLEDFlasher : public CTimer
	{
public:
	static CConsoleAlarmAlertLEDFlasher* NewL();

private:
	CConsoleAlarmAlertLEDFlasher();

public:
	TInt Start();
	void Stop();

private:	// from CActive
	void RunL();

private:
	TInt SetRedLedOn(TBool aOn);

private:
	TBool iLedOn;
	};

#endif

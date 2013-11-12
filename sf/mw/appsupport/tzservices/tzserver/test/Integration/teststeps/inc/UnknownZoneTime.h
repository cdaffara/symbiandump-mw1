/**
* Copyright (c) 2008-2009 Nokia Corporation and/or its subsidiary(-ies).
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



#ifndef __UNKNOWN_ZONE_TIME_H__
#define __UNKNOWN_ZONE_TIME_H__

#include <tz.h>
#include <test/testexecutestepbase.h>

_LIT(KUnknownZoneTime, "UnknownZoneTime");
		
class CUnknownZoneTime : public CTestStep
	{
public:
	static CUnknownZoneTime* NewL();
	~CUnknownZoneTime();
	
	TVerdict doTestStepPreambleL();
	TVerdict doTestStepL();
	TVerdict doTestStepPostambleL();
	
private:
	CUnknownZoneTime();
	void ConstructL();
	void TestSetUnknownZoneTimeNoPersistL();
	void TestSetUnknownZoneTimePersistL();
	
private:
	RTz iTz;
	};

#endif // __UNKNOWN_ZONE_TIME_H__

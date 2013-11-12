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



#ifndef __TIME_ZONE_OFFSET_CHECK_H
#define __TIME_ZONE_OFFSET_CHECK_H

#include <tz.h>
#include <test/testexecutestepbase.h>

// Name of the test step.
_LIT(KTimeZoneOffsetCheckStep, "TimeZoneOffsetCheck");

const TInt KCitynameLength = 64;

class CTimeZoneOffsetCheck : public CTestStep
	{
public:
	CTimeZoneOffsetCheck();
	~CTimeZoneOffsetCheck();
		
private:
	// from CTestStep
	TVerdict doTestStepL();
	TVerdict doTestStepPreambleL();
	TVerdict doTestStepPostambleL();
	
private:
	RTz iTimeZoneServer;
	//This is the expected old offset that is read from the configuration file
	TInt iOldOffset;
	//This is the expected new offset that is read from the configuration file
	TInt iNewOffset;
	//time zone to be set
	TBuf8<KCitynameLength> iLocation;
	};

#endif

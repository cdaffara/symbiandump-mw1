/**
* Copyright (c) 2004-2009 Nokia Corporation and/or its subsidiary(-ies).
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




#ifndef __UTCOFFSET_H_
#define __UTCOFFSET_H_

#include <tz.h>
#include <test/testexecutestepbase.h>

// String for the server create test step code
_LIT(KUTCOffset, "UTCOffset");

const TInt KMaxCityNameLength = 64;

class CUTCOffset : public CTestStep
	{
public:
	CUTCOffset();
	~CUTCOffset();
		
private:
	// from CTestStep
	TVerdict doTestStepL();
	TVerdict doTestStepPreambleL();
	TVerdict doTestStepPostambleL();
	
private:
	RTz iTimeZoneServer;
	//This is the expected utc offset that is read from the configuration file
	TTimeIntervalSeconds iUTCOffset;
	//time zone to be set
	TBuf8<KMaxCityNameLength> iLocation;
	// Time that is read from the configuration file
	TTime iTime;
	};
	
#endif //__UTCOFFSET_H_

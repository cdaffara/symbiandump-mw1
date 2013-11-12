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




#ifndef __ISDAYLIGHTLSAVINGON_H_
#define __ISDAYLIGHTLSAVINGON_H_

#include <tz.h>
#include <test/testexecutestepbase.h>

// String for the server create test step code
_LIT(KIsDaylightSavingOn, "IsDaylightSavingOn");

const TInt KCityNameLength = 64;

class CIsDaylightSavingOn : public CTestStep
	{
public:
	CIsDaylightSavingOn();
	~CIsDaylightSavingOn();
		
private:
	// from CTestStep
	TVerdict doTestStepL();
	TVerdict doTestStepPreambleL();
	TVerdict doTestStepPostambleL();
	
private:
	RTz iTimeZoneServer;

	//time zone to be set
	TBuf8<KCityNameLength> iLocation;
	// Time that is read from the configuration file
	TTime iTime;
	TTime iChangedTime;
	};
	
#endif //__ISDAYLIGHTLSAVINGON_H_

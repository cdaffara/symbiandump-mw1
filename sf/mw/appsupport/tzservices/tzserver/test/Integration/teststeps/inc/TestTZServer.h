/**
* Copyright (c) 2006-2009 Nokia Corporation and/or its subsidiary(-ies).
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



#ifndef __TEST_TZ_SERVER_H__
#define __TEST_TZ_SERVER_H__

#include <tz.h>
#include <test/testexecutestepbase.h>

/** constants */
const TInt KMaxLengthString = 100;
const TInt KRepeatApiNumber = 11;

/** literals */
_LIT(KTestTZServer, "TestTZServer");
_LIT(KTZTest,		"test");
_LIT(KNumericIds,			"numericids");
_LIT(KOffset,				"offsets");
_LIT(KDaylightSaving,	 	"daylightSaving");
_LIT(KTime,					"time");

enum TTZTestNumber
		{
		ETZTest1 = 1,
		ETZTest2 = 2,
		ETZTest3 = 3,
		ETZTest4 = 4,
		ETZTest5 = 5,
		ETZTest6 = 6,
		ETZTest7 = 7,
		ETZTest8 = 8,
		ETZTest9 = 9,
		};
		
// Class CTestTZServer  - This class executes TZ server tests
class CTestTZServer : public CTestStep
	{
public:
	CTestTZServer();
	~CTestTZServer();
	TVerdict doTestStepL();

private:
	void OOMGetOffsetTZIdsL();
	void OOMTimeConversionsL();
	void OOMGetCurrentTimeZoneL();
	void OOMSetCurrentTimeZoneL();
	void TimeZoneNumericIdL();
	void OOMDaylightSavingL();
	void TZConverterL();
	void MulitpleTZConverterL();
	void MulitpleTZServerL();
	void TokenizeStringL(const TDesC& aString, RArray<TPtrC>& aList, TChar aSeparator = ',');
	void StartHeapCheck(const TInt aFailAt);
	TBool ErrorProcess(const TInt aErr);
	void EndHeapCheck();
	
private:
	TInt iIntTestNumber;
	TBool iDaylightSaving;
	TInt iIntNumericId;
	TBuf<KMaxLengthString> iField;
	TInt iFailAt;
	};

#endif // __TEST_TZ_SERVER_H__

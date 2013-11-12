/*
* Copyright (c) 2008 Nokia Corporation and/or its subsidiary(-ies).
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


/**
@file
@test
@internalComponent
*/

#ifndef tmsteststep_
#define tmsteststep_

#include <test/testexecutestepbase.h>
#include <w32std.h>
#include <flogger.h>
#include <badesca.h> 

// default name that a test will be called if not specifically set in the calling object
_LIT(KUnknownSYMTestCaseIDName, "@SYMTestCaseID Unnamed test");
_LIT(KUndefinedSYMTestCaseIDName, "@SYMTestCaseID is not defined");
_LIT(KNotATestSYMTestCaseIDName, "This is not a test");

class CTmsTestStep : public CTestStep
	{
public:
	IMPORT_C CTmsTestStep();
	IMPORT_C virtual ~CTmsTestStep();
	IMPORT_C void CloseTMSGraphicsStep();
	IMPORT_C void MQCTest(TBool aCondition, const TText8* aFile, TInt aLine);
	IMPORT_C void MQCTestL(TBool aCondition, const TText8* aFile, TInt aLine);
	IMPORT_C void SetTestStepID(const TDesC& aStepName);
	IMPORT_C void SetOverallTestStepID(const TDesC& aStepName);
	IMPORT_C void RecordTestResultL();
	IMPORT_C void MultipleResultsForSameID(TBool aShowMultipleResults);

	static void DebugLogL(const TDesC8 &aText, const TDesC8 &aText1, TInt aPos);
	static TInt DebugLogPosL();
protected:
	static TInt ReadNextLineL( RFile &aFile, TDes8 &aLine );
	void CleanUpAndWriteResults();
	TBuf8<KMaxTestExecuteNameLength> iCurrentTestStepName;
	TBool iStepPassFail;
	TBool iShowMultipleResults;
	CDesCArrayFlat* iTestStepNames;
	RArray<TInt> iTestStepPositions;
	};

#endif /*tmsteststep_*/

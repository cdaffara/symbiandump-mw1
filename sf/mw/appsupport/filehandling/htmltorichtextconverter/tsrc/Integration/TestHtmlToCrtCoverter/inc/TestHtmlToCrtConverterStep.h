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



#ifndef TESTHTMLTOCRTCONVERTER_STEP_H
#define TESTHTMLTOCRTCONVERTER_STEP_H

#include "TestHtmlToCrtConverterSuiteServer.h"
#include "CHtmlToCrtConverter.h"
#include "CHtmlToCrtConvActive.h"

#include <e32std.h>
#include <e32test.h>
#include <f32file.h>
#include <e32math.h>

_LIT(KSourceFile,								"sourcefile");
_LIT(KTargetFile,								"targetfile");
_LIT(KNumberOfFilesConverted,					"Number of files Converted is %d");
_LIT(KAllocTestForSyncConversion,				"alloctestforsyncconversion");
_LIT(KAllocTestForAsyncConversion,				"alloctestforasyncconversion");
_LIT(KPerformanceTest,							"performance");
_LIT(KSynchronousConversion,					"syncconversion");
_LIT(KAsyncConversion,							"asyncconversion");

_LIT(KInfoTimeTakenForConversion,				"Time taken for converting the files synchronously is %fsecs");

_LIT(KErrConversionNotSuccessful,				"Conversion is not successful");

/**
Test Step that Converts the text file which is in HTML format to rich text format.
(1) convert files synchronously/asynchronously
(2) synchronous/asynchronous oom test
*/
class CTestHtmlToCrtConverterStep : public CTestStep
	{
public:
	CTestHtmlToCrtConverterStep();
	~CTestHtmlToCrtConverterStep();

	virtual TVerdict	doTestStepL();

private:
	void	SynchronousL(const TDesC& aSourceFile, TDesC& aTargetFile);
	void	AsynchronousL(const TDesC& aSourceFile, TDesC& aTargetFile);
	void	OOMSynchronousL(const TDesC& aSourceFile, TDesC& aTargetFile);
	void	OOMAsynchronousL(const TDesC& aSourceFile, TDesC& aTargetFile);
	void	PerformanceTestForSynchronousL(const TDesC& aSourceFile, TDesC& aTargetFile);
	void	TokenizeStringL(const TDesC& aString, RArray<TPtrC>& aList, TChar aSeparator = ',');
	TReal32	ComputeTimeDifference(const TTime& aStartTime, const TTime& aEndTime);
	};

_LIT(KTestHtmlToCrtConverterStep,"TestHtmlToCrtConverterStep");

#endif

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



#ifndef TESTHTMLTOCRTCONVERTER_BUFFER_STEP_H
#define TESTHTMLTOCRTCONVERTER_BUFFER_STEP_H

#include "TestHtmlToCrtConverterSuiteServer.h"
#include "CHtmlToCrtConvBuffer.h"
#include <charconv.h>

#include <e32std.h>
#include <e32test.h>

_LIT(KSourceText,								"source");
_LIT(KGetPartOfBuffer,							"getpartofbuffer");
_LIT(KStartPosition,							"startposition");
_LIT(KEndPosition,								"endposition");
_LIT(KInfoComparisionSuccessful,				"Unicode conversion is Successful");

_LIT(KErrInComparison,							"Unicode conversion is not Successful");
_LIT(KErrInConversion,							"Error : Buffer is not converted");
_LIT(KErrInPartOfBufferUnicode,					"Error : Error in get part of buffer unicode");

const TInt KMaxLengthString = 512;

class CHtmlToCrtConvBuffer;
/**
* Test Step that compares unicode buffers obtained by
*(1) passing the entire narrow buffer to CCnvCharacterSetConverter::ConvertToUnicode
*(2) using CHtmlToCrtConvBuffer::ReadCharacterL
*/
class CTestHtmlToCrtConverterBufferStep : public CTestStep
	{
public:
	CTestHtmlToCrtConverterBufferStep();
	~CTestHtmlToCrtConverterBufferStep();

	TVerdict	doTestStepL();

private:
	void	GetCharacterSetL(const TDesC8& aSource);
	void	ConvertBufferDirectly(const TDesC8& aSource, TDes& aUnicode);
	void	ConvertBufferUsingCHtmlToCrtConvBufferL(const TDesC8& aSource, TDes& aUnicode, TDes& aPartOfBuffer);
	TInt	DoComparisonL(const TDes8& aSource);
	void	ComparisonForVariousCharsetsL();
	void	GetPartOfConvertedBufferL(CHtmlToCrtConvBuffer* aBuffer, TDes& aPartOfBuffer);
	void	ComparePartOfBufferUnicode(const TDesC& aPartOfBuffer, const TDesC& aDirectUnicode);
	
public:
	RFs	iFs;
	CCnvCharacterSetConverter* iCnvCharacterSetConverter;
	TInt iStartPosition;
	TInt iEndPosition;
	TBool iGetPartOfBuffer;
	TInt iOffset;
	};

_LIT(KTestHtmlToCrtConverterBufferStep,"TestHtmlToCrtConverterBufferStep");

#endif

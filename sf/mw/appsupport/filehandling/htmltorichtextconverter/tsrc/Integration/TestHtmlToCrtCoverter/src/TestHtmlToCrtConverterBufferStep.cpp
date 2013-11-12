// Copyright (c) 2006-2009 Nokia Corporation and/or its subsidiary(-ies).
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

#include "TestHtmlToCrtConverterBufferStep.h"

CTestHtmlToCrtConverterBufferStep::CTestHtmlToCrtConverterBufferStep()
: iGetPartOfBuffer(EFalse)
/**
 * Constructor
 */
	{
	// **MUST** call SetTestStepName in the constructor as the controlling
	// framework uses the test step name immediately following construction to set
	// up the step's unique logging ID.
	SetTestStepName(KTestHtmlToCrtConverterBufferStep);
	}
	
CTestHtmlToCrtConverterBufferStep::~CTestHtmlToCrtConverterBufferStep()
/**
 * Destructor
 */
	{
	delete iCnvCharacterSetConverter;
	iFs.Close();
	}

/**
Base class pure virtual.
@return		EPass or EFail indicating the result of the test step.
@test
*/
TVerdict CTestHtmlToCrtConverterBufferStep::doTestStepL()
	{
	User::LeaveIfError(iFs.Connect());

	// CCnvCharacterSetConverter - Converts text between Unicode and other character sets.
	iCnvCharacterSetConverter = CCnvCharacterSetConverter::NewL();
	ComparisonForVariousCharsetsL();

	return TestStepResult();
	}
	

/** Compare the various character sets
@test
*/		
void CTestHtmlToCrtConverterBufferStep::ComparisonForVariousCharsetsL()
	{
	// Get different character sets from the ini file
	TPtrC	sourceData;
	GetStringFromConfig(ConfigSection(), KSourceText, sourceData);
	
	HBufC8*	tempSource = HBufC8::NewLC(sourceData.Length());
	tempSource->Des().Copy(sourceData);
	TPtr8	source(tempSource->Des());
	
	// Compare the source text with the text converted by the characterset converter
	if ( DoComparisonL(source) == 0 )
		{
		INFO_PRINTF1(KInfoComparisionSuccessful);
		}
	else
		{
		ERR_PRINTF1(KErrInComparison);
		SetTestStepResult(EFail);
		}
	CleanupStack::PopAndDestroy(tempSource);
	}
	
/** converts the Non Unicode character set present in source to unicode using CCnvCharacterSetConverter 
and stores them in the buffer, Converts the same source ie. Non Unicode character set to unicode using 
CHtmlToCrtConverter Class and compares both the unicode characters.
@param	aSource Contains different character set
@return	TInt
@test
*/
TInt CTestHtmlToCrtConverterBufferStep::DoComparisonL(const TDes8& aSource)
	{
	TBuf<KMaxLengthString> directUnicode;
	TBuf<KMaxLengthString> bufferUnicode;
	TBuf<KMaxLengthString> partOfBufferUnicode;

	GetCharacterSetL(aSource);
	ConvertBufferDirectly(aSource, directUnicode);
	ConvertBufferUsingCHtmlToCrtConvBufferL(aSource, bufferUnicode, partOfBufferUnicode);
	// Compare the two buffers and returns the result
	TInt	comparisionResult = directUnicode.Compare(bufferUnicode);
	
	// If iGetPartOfBuffer is set, then get the specified part based on the requirement from the user
	if ( iGetPartOfBuffer )
		{
		// Get the part of the buffer unicode and compare it with the expected value.
		ComparePartOfBufferUnicode(partOfBufferUnicode, directUnicode);	
		}
	
	return comparisionResult;
	}

/** Compare the non unicode character set to unicode in the buffer
@param	aSource Contains different character set
@test
*/
void CTestHtmlToCrtConverterBufferStep::GetCharacterSetL(const TDesC8& aSource)
	{
	// Array containing all of the character sets for which conversion is available
	CArrayFix<CCnvCharacterSetConverter::SCharacterSet>*	arrayOfCharacterSetsAvailable = CCnvCharacterSetConverter::CreateArrayOfCharacterSetsAvailableLC(iFs);
	TInt	confidence;
	TUint	characterSetIdentifier;

	// Attempts to determine the character set of the sample text from those supported on the phone
	iCnvCharacterSetConverter->AutoDetectCharacterSetL(confidence, characterSetIdentifier, *arrayOfCharacterSetsAvailable, aSource);
	// Specifies the character set to convert to or from
	iCnvCharacterSetConverter->PrepareToConvertToOrFromL(characterSetIdentifier, *arrayOfCharacterSetsAvailable, iFs);
	CleanupStack::PopAndDestroy(arrayOfCharacterSetsAvailable);
	}

/** Converts the text encoded in a non-Unicode character set into the Unicode character set 
@param	aSource The non-Unicode source text to be converted
@param	aUnicode Contains the converted text in the Unicode character 
@test
*/
void CTestHtmlToCrtConverterBufferStep::ConvertBufferDirectly(const TDesC8& aSource, TDes& aUnicode)
	{
	TInt	state = CCnvCharacterSetConverter::KStateDefault;
	// Converts text encoded in a non-Unicode character set into the Unicode character set 
	TInt	returnValue = iCnvCharacterSetConverter->ConvertToUnicode(aUnicode, aSource, state);

	// return value is 0 if entire buffer converted
	if( returnValue != KErrNone )
		{
		ERR_PRINTF1(KErrInConversion);
		SetTestStepResult(EFail);
		}
	}

/** Converts the text encoded in a non-Unicode character set into the Unicode character set 
@param	aSource The non-Unicode source text to be converted
@param	aUnicode Contains the converted text in the Unicode character
@param  aPartOfBuffer return value of the getpartofconvertedbuffer
@test
*/
void CTestHtmlToCrtConverterBufferStep::ConvertBufferUsingCHtmlToCrtConvBufferL(const TDesC8& aSource, TDes& aUnicode, TDes& aPartOfBuffer)
	{
	// write source buffer to source file for passing to CHtmlToCrtConvBuffer::NewL()
	_LIT(KSourceFile,"c:\\BufferTestSourceFile");
	TBufC<KMaxLengthString>	sourceFile = KSourceFile();
	RFile	source;
	User::LeaveIfError(source.Replace(iFs, sourceFile, EFileWrite));
	TESTL(source.Write(aSource) == KErrNone);
	source.Close();
	
	// Coverts the text encoded in a non-Unicode character set into the unicode character set using CHtmlToCrtConvBuffer
	CHtmlToCrtConvBuffer*	buffer = CHtmlToCrtConvBuffer::NewL(sourceFile, iFs, *iCnvCharacterSetConverter);
	CleanupStack::PushL(buffer);

	TChar	character;
	TInt	position = 0;
	TBool	endOfBuffer = EFalse;
	// Reads the converted Unicode character set and appends it in aUnicode character by character.
	while( buffer->ReadCharacterL(character, position, endOfBuffer) )
		{
		aUnicode.Append(character);
		}

	GetPartOfConvertedBufferL(buffer, aPartOfBuffer);

	CleanupStack::PopAndDestroy(buffer);
	}

/** Gets the specified part of the unicode based on the requirement from the user
@param	aBuffer Pointer to CHtmlToCrtConvBuffer
@param	aPartOfUnicode Contains the return value of GetPartOfBufferL()
@test
*/
void CTestHtmlToCrtConverterBufferStep::GetPartOfConvertedBufferL(CHtmlToCrtConvBuffer* aBuffer, TDes& aPartOfBuffer)
	{
	GetBoolFromConfig(ConfigSection(), KGetPartOfBuffer, iGetPartOfBuffer);
	if ( iGetPartOfBuffer == EFalse )
		{
		return;
		}
		
	GetIntFromConfig(ConfigSection(), KStartPosition, iStartPosition);
	GetIntFromConfig(ConfigSection(), KEndPosition, iEndPosition);
	TPtrC	partOfBufferUnicode;
	if ( iEndPosition == 0 )
		{
		// Gets the part of the buffer from the start position till the end
		aBuffer->GetToEndOfBufferL(partOfBufferUnicode, iStartPosition);
		aPartOfBuffer.Copy(partOfBufferUnicode);	
		}
	else if ( (iStartPosition != 0) && (iEndPosition != 0) )
		{
		// Get the specified part of the buffer returned from CHtmlToCrtConverterBuffer
		aBuffer->GetPartOfBufferL(partOfBufferUnicode, iStartPosition, iEndPosition);
		aPartOfBuffer.Copy(partOfBufferUnicode);
		}
	else if ( iEndPosition != 0 )
		{
		TBuf8<KMaxLengthString>	partOfBufferUnicode;
		// Get the sample of text from the file and returns it in the partOfBufferUnicode
		aBuffer->GetSampleOfTextFromFileL(partOfBufferUnicode, iEndPosition, 0);
		aPartOfBuffer.Copy(partOfBufferUnicode);
		}

	}

/** Compares the value that is returned from GetPartOfBufferL and the expected value
@param	aDirectUnicode expected value
@param	aPartOfUnicode Contains the return value of GetPartOfBufferL()
@test
*/
void CTestHtmlToCrtConverterBufferStep::ComparePartOfBufferUnicode(const TDesC& aPartOfBuffer, const TDesC& aDirectUnicode)
	{
	TPtrC	expectedPartOfBuffer;
	// Compares the returned values with the expected
	if ( iEndPosition == 0 )
		{
		expectedPartOfBuffer.Set(aDirectUnicode.Mid(iStartPosition));	
		}
	else if ( (iStartPosition != 0) && (iEndPosition != 0) )
		{
		expectedPartOfBuffer.Set(aDirectUnicode.Mid(iStartPosition,(iEndPosition - iStartPosition + 1)));	
		}
	else if ( iEndPosition != 0 )
		{
		expectedPartOfBuffer.Set(aDirectUnicode.Mid(0,iEndPosition));	
		}
	// Compare both the values
	TInt	result = aPartOfBuffer.Compare(expectedPartOfBuffer);
	if ( result != 0 )
		{
		ERR_PRINTF1(KErrInPartOfBufferUnicode);
		SetTestStepResult(EFail);
		}
	}


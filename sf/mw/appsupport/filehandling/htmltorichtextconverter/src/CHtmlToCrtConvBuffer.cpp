// Copyright (c) 2001-2009 Nokia Corporation and/or its subsidiary(-ies).
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

#include "CHtmlToCrtConvBuffer.h"
#include "CHtmlToCrtConvAssertDefines.h"

const TInt KReadBufferSize = 256;

CHtmlToCrtConvBuffer::CHtmlToCrtConvBuffer(CCnvCharacterSetConverter& aCnvCharacterSetConverter)
:iCnvCharacterSetConverter(aCnvCharacterSetConverter)
	{
	}

CHtmlToCrtConvBuffer* CHtmlToCrtConvBuffer::NewL(const TDesC& aFile, RFs& aFsSession, CCnvCharacterSetConverter& aCnvCharacterSetConverter)
	{
	CHtmlToCrtConvBuffer* self=new(ELeave) CHtmlToCrtConvBuffer(aCnvCharacterSetConverter);
	CleanupStack::PushL(self);
	self->ConstructL(aFile, aFsSession);
	CleanupStack::Pop(self);
	return self;
	}

void CHtmlToCrtConvBuffer::ConstructL(const TDesC& aFile, RFs& aFsSession)
	{
	User::LeaveIfError(iFile.Open(aFsSession, aFile, EFileShareReadersOnly));
	iSourceBuffer=HBufC8::NewL(KReadBufferSize);
	iUnicodeBuffer=HBufC16::NewL(KReadBufferSize);
	}

CHtmlToCrtConvBuffer::~CHtmlToCrtConvBuffer()
	{
	iFile.Close();
	delete iSourceBuffer;
	delete iUnicodeBuffer;
	}

//===================================================================================================
//ReadCharacterL - returns EFalse if no character read, ie when end of file and end of buffer reached
//===================================================================================================
TBool CHtmlToCrtConvBuffer::ReadCharacterL(TChar& aCharacter, TInt& aBufferPosition, TBool& aEndOfBuffer)
	{
	ASSERT((iBufferPosition>=0 && iBufferPosition<=KReadBufferSize));

	aEndOfBuffer=EFalse;
	if (iBufferPosition < iUnicodeBuffer->Length())
		//character in buffer
		{
		aBufferPosition=iBufferPosition;
		aCharacter=(*iUnicodeBuffer)[iBufferPosition++];
		if (iBufferPosition==iUnicodeBuffer->Length())
			{
			aEndOfBuffer=ETrue;
			}
		return ETrue;
		}
	else
		//no characters remaining in buffer
		{
		aBufferPosition=0;
		if (FillBufferL())
			//file was not empty
			{
			aCharacter=(*iUnicodeBuffer)[iBufferPosition++];
			if (iBufferPosition==iUnicodeBuffer->Length())
				{
				aEndOfBuffer=ETrue;
				}
			return ETrue;
			}
		else
			//file was empty
			{
			aEndOfBuffer=ETrue;
			return EFalse;
			}
		}
	}

void CHtmlToCrtConvBuffer::GetPartOfBufferL(TPtrC16& aPartOfBuffer, TInt aStartPosition, TInt aEndPosition) const
	{
	TPtrC16 partOfBuffer(iUnicodeBuffer->Mid(aStartPosition,(aEndPosition - aStartPosition + 1)));
	aPartOfBuffer.Set(partOfBuffer);
	}

void CHtmlToCrtConvBuffer::GetToEndOfBufferL(TPtrC16& aPartOfBuffer, TInt aStartPosition) const
	{
	TPtrC16 partOfBuffer(iUnicodeBuffer->Mid(aStartPosition));
	aPartOfBuffer.Set(partOfBuffer);
	}

void CHtmlToCrtConvBuffer::GetSampleOfTextFromFileL(TDes8& aSample, TInt aLength, TInt aOffset)
	{
	TInt fileSize=0;
	User::LeaveIfError(iFile.Size(fileSize));

	//adjust aOffset - used to get sample of text from file, so should read aLength chars
	if(fileSize<aLength)
		aOffset=0;
	else if(fileSize<aOffset+aLength)
		aOffset=fileSize-aLength;

	User::LeaveIfError(iFile.Read(aOffset, aSample, aLength));
	ResetL();
	}

void CHtmlToCrtConvBuffer::ResetL()
	{
	//set read file position to start of file
	TInt pos=0;
	User::LeaveIfError(iFile.Seek(ESeekStart, pos));
	}

TBool CHtmlToCrtConvBuffer::FillBufferL()
	{
	iBufferPosition = 0;
	//iSourceBuffer contains any remaining unconverted characters from last call to ConvertToUnicode.
	TPtr8 sourcePtr(iSourceBuffer->Des());
	TInt sourceLength=iSourceBuffer->Length();
	TBool endOfFileReached=EFalse;

	//newBuffer gets characters from the file to fill the remaining space in iSourceBuffer
	HBufC8* newBuffer=HBufC8::NewLC(KReadBufferSize-sourceLength);
	TPtr8 newBufferPtr(newBuffer->Des());
	User::LeaveIfError(iFile.Read(newBufferPtr, KReadBufferSize-sourceLength));
	sourcePtr.Append(newBufferPtr);
	CleanupStack::PopAndDestroy(newBuffer);

	sourceLength=sourcePtr.Length();
	if(sourceLength < KReadBufferSize)
		{
		endOfFileReached=ETrue;
		if(sourcePtr.Length()==0)
			{
			return EFalse;
			}
		}

	TPtr16 unicodePtr(iUnicodeBuffer->Des());
	unicodePtr.Zero();

	DoConvertBufferL(sourcePtr, unicodePtr, sourceLength, endOfFileReached);

	return ETrue;
	}

//=============================================================================
//DoConvertBufferL - sets iUnicodeBuffer to contain the converted characters,
//and iSourceBuffer to contain only the unconverted characters.
//=============================================================================
void CHtmlToCrtConvBuffer::DoConvertBufferL(TPtr8& aSourcePtr, TPtr16& aUnicodePtr, TInt& aSourceLength, TBool& aEndOfFile)
	{
	HBufC16* tempUnicode=HBufC16::NewLC(aSourceLength);
	TPtr16 tempUnicodePtr(tempUnicode->Des());

	//returnValue = number of unconverted characters at end of buffer
	TInt returnValue=iCnvCharacterSetConverter.ConvertToUnicode(tempUnicodePtr, aSourcePtr, iState);

	if(!aEndOfFile)
		{
		if(returnValue==CCnvCharacterSetConverter::EErrorIllFormedInput)
			User::Leave(KErrCorrupt);
		}
	if(returnValue<0)//future-proof against 'TError' expanding
		User::Leave(KErrGeneral);

	aUnicodePtr.Append(tempUnicodePtr);
	CleanupStack::PopAndDestroy(tempUnicode);
	
	//set iSourceBuffer to contain only the unconverted characters
	aSourcePtr.Copy(aSourcePtr.Right(returnValue));
	aSourceLength=aSourcePtr.Length();
	}

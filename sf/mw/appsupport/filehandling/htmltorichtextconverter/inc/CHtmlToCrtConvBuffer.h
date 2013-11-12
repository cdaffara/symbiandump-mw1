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

#include <e32std.h>
#include <e32base.h>
#include <f32file.h>
#include <charconv.h>

class CHtmlToCrtConvBuffer : public CBase
	{
	public:
		static CHtmlToCrtConvBuffer* NewL(const TDesC& aFile, RFs& aFsSession, CCnvCharacterSetConverter& aCnvCharacterSetConverter);
		~CHtmlToCrtConvBuffer();

	/**
		@fn		TBool ReadCharacterL(TChar& aCharacter, TInt& aBufferPosition, TBool& aEndOfBuffer)		
		Intended Usage	: gets next character from the file
		@param	aCharacter the character which was read
		@param	aBufferPosition	the current buffer position
		@param	aEndOfBuffer	ETrue if end of buffer reached, else EFalse
		@return	TBool ETrue if character read, else EFalse
	 */
		TBool ReadCharacterL(TChar& aCharacter, TInt& aBufferPosition, TBool& aEndOfBuffer);
		void GetPartOfBufferL(TPtrC16& aPartOfBuffer, TInt aStartPosition, TInt aEndPosition) const;
		void GetToEndOfBufferL(TPtrC16& aPartOfBuffer, TInt aStartPosition) const;
	/**
		@fn		GetSampleOfTextFromFileL(TDes8& aSample, TInt aLength, TInt aOffset)
		Intended Usage	:	reads the specified number of characters from the file at a specified offset 
		@param	aSample	descriptor into which characters are read
		@param	aLength	number of characters to be read
		@param	aOffset	offset from start of file
		@return	void
	 */
		void GetSampleOfTextFromFileL(TDes8& aSample, TInt aLength, TInt aOffset);
		void ResetL();

	private:
		CHtmlToCrtConvBuffer(CCnvCharacterSetConverter& aCnvCharacterSetConverter);
		void ConstructL(const TDesC& aFile, RFs& aFsSession);
	/**
		@fn		TBool FillBufferL()
		Intended Usage	:	used to read characters from the file into the buffer 
		@return	TBool EFalse if no characters read from file, else Etrue
	 */
		TBool FillBufferL();
		void DoConvertBufferL(TPtr8& aSourcePtr, TPtr16& aUnicodePtr, TInt& aLength, TBool& aEndOfFile);

	private:
		CCnvCharacterSetConverter& iCnvCharacterSetConverter;
		TInt iState;
		RFile iFile;
		HBufC8* iSourceBuffer;
		HBufC16* iUnicodeBuffer;
		TInt iBufferPosition;
	};

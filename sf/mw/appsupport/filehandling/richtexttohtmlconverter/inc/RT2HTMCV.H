// Copyright (c) 1997-2009 Nokia Corporation and/or its subsidiary(-ies).
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
// Converter Architecture wrappers for CRT -> HTML conversion
// 
//

#ifndef __HTML_2_CRT_CONV_H__
#define __HTML_2_CRT_CONV_H__

const TUid KCrt2HTMLConverterUid = {0x1000071c};

#include <txtmrtsr.h>

//-------------------------------------------------------------------------------------------------
// Fwd Decl.
class TOutputSink;

//-------------------------------------------------------------------------------------------------
// Class CCrtToHTMLConverter.
// The CConverterBase2 derived class which is responsable for performing the conversion operation.
// 1. Only supports File->File conversions
// 2. Does not support embedded objects (ConArc has no mechanism to request a filename to convert the 
//    embedded object into)
// 3. The conversion is performed on a paragraph by paragraph basis

class CCrtToHTMLConverter : public CConverterBase2, MRichTextStoreResolver
{
	public: // from CConverterBase2
		void ConvertL(const TFileName& aSourceFile, const TFileName& aTargetFile,  MConverterUiObserver* aObserver = NULL);
		void ConvertObjectL(RReadStream& aReadStream, RWriteStream& aWriteStream,  MConverterUiObserver* aObserver=NULL);  // Leaves with KErrNotSupported
		void ConvertAL(const TFileName& aSourceFile, const TFileName& aTargetFile, MConverterUiObserver* aObserver=NULL); // Leaves with KErrNotSupported
		void ConvertObjectAL(RReadStream& aReadStream, RWriteStream& aWriteStream, MConverterUiObserver* aObserver=NULL); // Leaves with KErrNotSupported

		TBool DoConvertL();
		TUid  Uid();
		TInt  Capabilities();
		void  CancelConvert();

		CCrtToHTMLConverter();
		~CCrtToHTMLConverter();
	
		// From MRichTextStoreResolver
		const CStreamStore& StreamStoreL(TInt /*aPos*/)const { return *iStore; };
		static CConverterBase2* NewL();
	private:
		void ResetMembers();
		void RestoreInputTextL(const TFileName& aSourceFile);
		void PrepForConversionL(const TFileName& aSourceFile, const TFileName& aTargetFile);
		void PrepForConversionL(RReadStream& aReadStream, RWriteStream& aWriteStream);

		// Output
		void StartDocumentL();
		void EndDocumentL();
		void TranslateL		(const TDesC& aBuf);
		void WriteTagL(const TDesC8& aTagText);
		void WriteContentL(const TDesC& aText);
		// Processing
		void ProcessParagraphFormatL(const CParaFormat* aParaFormat, TBool aClosePara = ETrue);
		void ProcessParagraphTextL(TInt aPos, TInt aLength);

		void OpenCharFormatL (const TCharFormatMask& aMask, const TCharFormat& aFormat);
		void CloseCharFormatL(const TCharFormatMask& aMask, const TCharFormat& aFormat);
		void DiffCharFormats(const TCharFormat& aFormatA, const TCharFormat& aFormatB, TCharFormatMask& aMask);

	private:
		CParaFormatLayer*  iParaLayer;		// The global paragraph format layer
		CCharFormatLayer*  iCharLayer;		// The global character format layer
		CRichText*		   iInputText; 		// The input rich text
		HBufC*			   iInternalFile;	// Interanl file copy for the input text

		CDirectFileStore*  iStore;			// The Root stream from which we are reading

		RFs				   iFs;				// Our file server session
		RFileWriteStream   iFileStream;
		RWriteStream&	   iHTMLOut;	   	// Somewhere to write our output to

		TBullet* iBullet;					// Pointer to any current bullet object. Null if we're not processing a list.
		TInt     iIndent;					// Indent level (Number of <BLOCKQUOTE> tags to insert)
	
		TInt     iParaRemain;				// How many paragraphs remain to be processed
		TInt	 iParaCount;				// How many paragraphs are there 
	    TInt	 iParaNum;					// Which paragraph are we processing now (-1 if we haven't started processing yet)
	    TInt	 iParaLen;					// How long is the current paragraph
		TInt	 iParaPos;					// What is the offset of the start of the curent paragraph from the start of the input text

		TBool    iInsertBlankDivClose;		// Flag indicates if the current paragraph was blank
		TInt     iStyleIndex;				// Index into the RichText style list, or -1
		TInt	 iOldFmtCount;				// Checks if this loop has previously been entered

	    MConverterUiObserver* iObserver;		// Our observer object (or NULL if no-one is watching)
	};

enum TTextToHTMLPanic
	{
	EHTMLTagTooSmall,
	EHTMLBeyondLastPara,
	EHTMLNotAPicture
	};

void Panic(TTextToHTMLPanic aPanic);

#endif

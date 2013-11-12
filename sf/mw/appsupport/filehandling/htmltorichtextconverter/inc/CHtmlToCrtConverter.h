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
//

#ifndef __CHTMLTOCRTCONVERTER_H__
#define __CHTMLTOCRTCONVERTER_H__

#include <e32base.h>
#include <conarc.h>
#include <barsc.h>
#include <charconv.h>

#include "MHtmlToCrtConvResourceFile.h"

class CRichText;
class CParaFormatLayer;
class CCharFormatLayer;
class CHtmlToCrtConvParser;
class CHtmlToCrtConvBuffer;
class CHtmlToCrtConvCharsetConvert;

class CHtmlToCrtConverter : public CConverterBase2, public MHtmlToCrtConvResourceFile
	{
	public:
		enum TConverterState
			{
			EGetCharsetFromPassword,
			EGetCharsetFromMetaTag,
			EGetCharsetUsingAutoDetect,
			EPrepareForConvertToUnicode,
			EDoOneStepOfParsing,
			EFinishedParsing
			};

	public:
		CHtmlToCrtConverter();
		~CHtmlToCrtConverter();

	//from CConverterBase
	public:
		void ConvertL(const TFileName& aSourceFile, const TFileName& aTargetFile, MConverterUiObserver* aObserver=NULL);
		void ConvertAL(const TFileName& aSourceFile, const TFileName& aTargetFile, MConverterUiObserver* aObserver=NULL);
		TBool DoConvertL();
		TUid Uid();
		TInt Capabilities();
		void CancelConvert();

		static CConverterBase2* NewL();


	private:
		void PrepForConversionL(const TDesC& aSourceFile, const TDesC& aTargetFile);
		void LocateAndOpenResourceFileL();
		void Cleanup();
		void FinaliseConversionL();

		//from MHtmlToCrtConvResourceFile
		HBufC* ReadResourceHBufCLC(TInt aResourceId);
		TRgb ReadResourceTRgbLC(TInt aResourceId);

	private:
		CRichText* iText;
		CParaFormatLayer* iParaFormatLayer;
		CCharFormatLayer* iCharFormatLayer;
		CHtmlToCrtConvBuffer* iBuffer;
		CHtmlToCrtConvParser* iParser;
		CHtmlToCrtConvCharsetConvert* iCharsetConvert;
		CCnvCharacterSetConverter* iCnvCharacterSetConverter;

		MConverterUiObserver* iObserver;
		HBufC* iSourceFileName;
		HBufC* iTargetFileName;
		RFs iFsSession;
		RResourceFile iResourceFile;
		TConverterState iConverterState;
	};

#endif

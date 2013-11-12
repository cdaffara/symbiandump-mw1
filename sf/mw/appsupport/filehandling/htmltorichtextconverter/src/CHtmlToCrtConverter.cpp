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

#include "CHtmlToCrtConverter.h"
#include "CHtmlToCrtConvParser.h"
#include "CHtmlToCrtConvBuffer.h"
#include "CHtmlToCrtConvCharsetConvert.h"
#include <txtrich.h>
#include <bautils.h>
#include <s32file.h>
#include <ecom/implementationproxy.h>

#ifdef __CHTMLTOCRTCONV_COPY_TO_CLIPBOARD__
	#include <baclipb.h>
#endif

const TUid KUidCHtmlToCrtConverter={0x1000a90e};

CHtmlToCrtConverter::CHtmlToCrtConverter()
	{
	}

CHtmlToCrtConverter::~CHtmlToCrtConverter()
	{
	Cleanup();
	}

void CHtmlToCrtConverter::ConvertL(const TFileName& aSourceFile, const TFileName& aTargetFile, MConverterUiObserver* aObserver)
	{
	iObserver=aObserver;
	PrepForConversionL(aSourceFile, aTargetFile);
	while(DoConvertL())
		{
		}
	}

void CHtmlToCrtConverter::ConvertAL(const TFileName& aSourceFile, const TFileName& aTargetFile, MConverterUiObserver* aObserver)
	{
	iObserver=aObserver;
	PrepForConversionL(aSourceFile, aTargetFile);
	}

TBool CHtmlToCrtConverter::DoConvertL()
	{
	TBool moreProcessingRequired=ETrue;
	TBool moreParsingRequired=ETrue;
	switch(iConverterState)
		{
		case EGetCharsetFromPassword:
			if(iCharsetConvert->GetCharSetFromPasswordL())
				{
				iConverterState=EPrepareForConvertToUnicode;
				}
			else
				iConverterState=EGetCharsetFromMetaTag;
			break;
		case EGetCharsetFromMetaTag:
			if(iCharsetConvert->GetCharSetFromMetaTagL())
				{
				iConverterState=EPrepareForConvertToUnicode;
				}
			else
				iConverterState=EGetCharsetUsingAutoDetect;
			break;
		case EGetCharsetUsingAutoDetect:
			iCharsetConvert->GetCharSetUsingAutoDetectL();
			iConverterState=EPrepareForConvertToUnicode;
			break;
		case EPrepareForConvertToUnicode:
			iCharsetConvert->PrepareForConvertToUnicodeL();
			iConverterState=EDoOneStepOfParsing;
			break;
		case EDoOneStepOfParsing:
			moreParsingRequired=iParser->DoOneStepL();
			if(!moreParsingRequired)
				{
				iConverterState=EFinishedParsing;
				}
			break;
		case EFinishedParsing:
			FinaliseConversionL();
			moreProcessingRequired=EFalse;
			break;
		default:
			break;
		}
	return moreProcessingRequired;
	}

TUid CHtmlToCrtConverter::Uid()
	{
	return KUidCHtmlToCrtConverter;
	}

TInt CHtmlToCrtConverter::Capabilities()
	{
	return EConvertsFiles;
	}

void CHtmlToCrtConverter::CancelConvert()
	{
	Cleanup();
	}

void CHtmlToCrtConverter::PrepForConversionL(const TDesC& aSourceFile, const TDesC& aTargetFile)
	{
	Cleanup();
	iParaFormatLayer=CParaFormatLayer::NewL();
	iCharFormatLayer=CCharFormatLayer::NewL();
	iText=CRichText::NewL(iParaFormatLayer, iCharFormatLayer);

	iSourceFileName=aSourceFile.AllocL();
	iTargetFileName=aTargetFile.AllocL();
	User::LeaveIfError(iFsSession.Connect());
	LocateAndOpenResourceFileL();
	iCnvCharacterSetConverter=CCnvCharacterSetConverter::NewL();
	iBuffer=CHtmlToCrtConvBuffer::NewL(*iSourceFileName, iFsSession, *iCnvCharacterSetConverter);
	iCharsetConvert=CHtmlToCrtConvCharsetConvert::NewL(*iBuffer, iFsSession, *iCnvCharacterSetConverter, *this, iObserver);
	iParser=CHtmlToCrtConvParser::NewL(*iText, *iBuffer, *this);
	if (iObserver)
		{
		TEntry entry;
		User::LeaveIfError(iFsSession.Entry(aSourceFile, entry));
		iObserver->MaxSteps((entry.iSize / KCharsProcessedInOneStep), 0);
		}
	}

void CHtmlToCrtConverter::LocateAndOpenResourceFileL()
	{
	_LIT(KResourceFileName, "chtmltocrtconverter.rsc");
	_LIT(KPath,"\\resource\\convert\\");

	TFileName* resourceFileName=new(ELeave) TFileName(KResourceFileName);
	CleanupStack::PushL(resourceFileName);
	BaflUtils::NearestLanguageFile(iFsSession, *resourceFileName);
	TFileName* fullPathToResourceFile=new(ELeave) TFileName;
	CleanupStack::PushL(fullPathToResourceFile);

#ifdef __CHTMLTOCRTCONV_TESTCODE__
	//test code uses TFindFile to scan for resource file
	TFindFile findFile(iFsSession);
	User::LeaveIfError(findFile.FindByDir(*resourceFileName, KPath)==KErrNone);
	(*fullPathToResourceFile)=findFile.File();
#else
	//use Dll::FileName to get drive letter for converter, then append "\system\convert\'resource file name'"
	Dll::FileName(*fullPathToResourceFile);
	fullPathToResourceFile->SetLength(2);
	fullPathToResourceFile->Append(KPath);
	fullPathToResourceFile->Append(*resourceFileName);
#endif

	iResourceFile.OpenL(iFsSession, *fullPathToResourceFile);
	CleanupStack::PopAndDestroy(2);//fullPathToResourceFile, resourceFileName
	iResourceFile.ConfirmSignatureL(0);
	}

HBufC* CHtmlToCrtConverter::ReadResourceHBufCLC(TInt aResourceId)
	{
	HBufC8* resource=iResourceFile.AllocReadLC(aResourceId);
	TResourceReader resourceReader;
	resourceReader.SetBuffer(resource);

	HBufC* resourceText=resourceReader.ReadHBufCL();
	CleanupStack::PopAndDestroy(resource);
	CleanupStack::PushL(resourceText);
	return resourceText;
	}

TRgb CHtmlToCrtConverter::ReadResourceTRgbLC(TInt aResourceId)
	{
	HBufC8* resource=iResourceFile.AllocReadLC(aResourceId);
	TResourceReader resourceReader;
	resourceReader.SetBuffer(resource);

	TRgb resourceColor=0;
	TInt color=resourceReader.ReadUint8();
	resourceColor.SetRed(color);
	color=resourceReader.ReadUint8();
	resourceColor.SetGreen(color);
	color=resourceReader.ReadUint8();
	resourceColor.SetBlue(color);

	CleanupStack::PopAndDestroy(resource);
	return resourceColor;
	}

void CHtmlToCrtConverter::Cleanup()
	{
	delete iCharsetConvert;
	iCharsetConvert=NULL;
	delete iParser;
	iParser=NULL;
	delete iBuffer;
	iBuffer=NULL;
	delete iCnvCharacterSetConverter;
	iCnvCharacterSetConverter=NULL;
	delete iText;
	iText=NULL;
	delete iParaFormatLayer;
	iParaFormatLayer=NULL;
	delete iCharFormatLayer;
	iCharFormatLayer=NULL;
	delete iSourceFileName;
	iSourceFileName=NULL;
	delete iTargetFileName;
	iTargetFileName=NULL;
	iResourceFile.Close();
	iFsSession.Close();
	}

void CHtmlToCrtConverter::FinaliseConversionL()
	{
	TInt error=iFsSession.MkDirAll(*iTargetFileName);
	if(!(error==KErrNone || error==KErrAlreadyExists))
		User::Leave(error);

	CDirectFileStore* directFileStore=CDirectFileStore::ReplaceLC(iFsSession, *iTargetFileName, EFileWrite);
	directFileStore->SetTypeL(KDirectFileStoreLayoutUid);

	CStreamDictionary* dictionary=CStreamDictionary::NewLC();
	if (iText->DocumentLength())
		{
		iText->CopyToStoreL(*directFileStore, *dictionary, 0, iText->DocumentLength());
		}

	RStoreWriteStream storeWriteStream;
	TStreamId streamId=storeWriteStream.CreateLC(*directFileStore);
	storeWriteStream << *dictionary;
	storeWriteStream.CommitL();

	CleanupStack::PopAndDestroy(2); //storeWriteStream, dictionary
	directFileStore->SetRootL(streamId);
	directFileStore->CommitL();
	CleanupStack::PopAndDestroy(directFileStore);

#ifdef __CHTMLTOCRTCONV_COPY_TO_CLIPBOARD__
	CClipboard* clipboard=CClipboard::NewForWritingLC(iFsSession);
	if (iText->DocumentLength())
		{
		iText->CopyToStoreL((clipboard->Store()), (clipboard->StreamDictionary()), 0, iText->DocumentLength());
		}
	clipboard->CommitL();
	CleanupStack::PopAndDestroy(clipboard);
#endif
	}

CConverterBase2* CHtmlToCrtConverter::NewL()
	{
	CConverterBase2* newConverter=new (ELeave) CHtmlToCrtConverter();
	return newConverter;
	}

const TImplementationProxy ImplementationTable[] =
    {
	IMPLEMENTATION_PROXY_ENTRY(0x1000a90e,CHtmlToCrtConverter::NewL)
    };

EXPORT_C const TImplementationProxy* ImplementationGroupProxy(TInt& aTableCount)
    {
    aTableCount = sizeof(ImplementationTable) / sizeof(TImplementationProxy);
    return ImplementationTable;
    }


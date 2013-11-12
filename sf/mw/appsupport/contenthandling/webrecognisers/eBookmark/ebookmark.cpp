// Copyright (c) 2000-2009 Nokia Corporation and/or its subsidiary(-ies).
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

#include <apmrec.h>
#include <apmstd.h>
#include <f32file.h>
#include "ebookmark.h"

#ifdef __UI_FRAMEWORKS_V2__
#include <ecom/implementationproxy.h>
#endif //__UI_FRAMEWORKS_V2__

const TInt KMimeEBookmarkRecognizerValue=0x10008ED4;
const TUid KUidEBookmarkRecognizer={KMimeEBookmarkRecognizerValue};
const TInt KNumMimeTypes=1; // text/vnd.symbian.ebookmark

_LIT8(KEBookmarkMimeType, "text/vnd.symbian.ebookmark");


// eBookmark datafields needed for recognition
_LIT8(KFieldBeginMarker, "BEGIN:EBOOKMARK");
_LIT8(KFieldUrlMarker, "URL:");

_LIT(KEBookmarkFileExtension, ".eBm");

const TInt KUidEBookmarkRecognizerStringLength = 255;


CEBookmarkRecognizer::CEBookmarkRecognizer()
	:CApaDataRecognizerType(KUidEBookmarkRecognizer,CApaDataRecognizerType::ENormal)
	{
	iConfidence = ENotRecognized;
	iCountDataTypes = KNumMimeTypes;
	}

TUint CEBookmarkRecognizer::PreferredBufSize()
	{
	return KUidEBookmarkRecognizerStringLength;
	}

TDataType CEBookmarkRecognizer::SupportedDataTypeL(TInt aIndex) const
	{
	__ASSERT_ALWAYS(aIndex>=0 && aIndex<KNumMimeTypes,User::Invariant());
	return TDataType(KEBookmarkMimeType);
	}

void CEBookmarkRecognizer::DoRecognizeL(const TDesC& aName, const TDesC8& aBuffer)
	{
	iConfidence = ENotRecognized;


	TInt beginFieldPos = aBuffer.FindF(KFieldBeginMarker);
	TInt urlFieldPos = aBuffer.FindF(KFieldUrlMarker);

	if (beginFieldPos >= 0 && urlFieldPos > beginFieldPos)
		// must find fields BEGIN: and URL:
		{
		iDataType=TDataType(KEBookmarkMimeType);

	    const TInt positionOfDot=aName.LocateReverse('.');
	    if ((positionOfDot>=0) &&
			(aName.Mid(positionOfDot).CompareF(KEBookmarkFileExtension) == 0))
			iConfidence = ECertain;
		else
			iConfidence = EProbable;

		}

	}


#ifdef __UI_FRAMEWORKS_V2__

const TImplementationProxy ImplementationTable[]=
	{
    IMPLEMENTATION_PROXY_ENTRY(0x10008ED4,CEBookmarkRecognizer::NewL)
	};

EXPORT_C const TImplementationProxy* ImplementationGroupProxy(TInt& aTableCount)
    {
    aTableCount = sizeof(ImplementationTable) / sizeof(TImplementationProxy);
    return ImplementationTable;
    }

CApaDataRecognizerType* CEBookmarkRecognizer::NewL()
	{
    return new (ELeave) CEBookmarkRecognizer();
    }

#else

EXPORT_C CApaDataRecognizerType* CreateRecognizer()
// The gate function - ordinal 1
//
	{
	CApaDataRecognizerType* thing=new CEBookmarkRecognizer();
	return thing; // NULL if new failed
	}

#endif //__UI_FRAMEWORKS_V2__


#ifndef EKA2
GLDEF_C TInt E32Dll(TDllReason /*aReason*/)
//
// DLL entry point
//
	{
	return KErrNone;
	}
#endif // EKA2

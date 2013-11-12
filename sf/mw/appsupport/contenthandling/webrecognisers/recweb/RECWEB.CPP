// Copyright (c) 2003-2009 Nokia Corporation and/or its subsidiary(-ies).
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
#include "RECWEB.H"

#ifdef __UI_FRAMEWORKS_V2__
#include <ecom/implementationproxy.h>
#endif //__UI_FRAMEWORKS_V2__

const TInt KMimeWebRecognizerValue=0x10001315;
const TUid KUidMimeWebRecognizer={KMimeWebRecognizerValue};
const TInt KWebNumMimeTypes=2;
const TInt KWebRecogniserBufferSize=2048;
_LIT8(KHtmlMimeType,"text/html");
_LIT8(KXmlMimeType,"text/xml");

CApaWebRecognizer::CApaWebRecognizer()
:	CApaDataRecognizerType(KUidMimeWebRecognizer,CApaDataRecognizerType::ENormal)
	// All these mime types have reasonable recognition
	{
	iCountDataTypes=KWebNumMimeTypes;
	}

TUint CApaWebRecognizer::PreferredBufSize()
	{
	return KWebRecogniserBufferSize;
	}

TDataType CApaWebRecognizer::SupportedDataTypeL(TInt aIndex) const
	{
	__ASSERT_DEBUG(aIndex>=0 && aIndex<KWebNumMimeTypes,User::Invariant());
	switch (aIndex)
		{
	case 0:
		return TDataType(KHtmlMimeType);
	default:
		return TDataType(KXmlMimeType);
		}
	}

void CApaWebRecognizer::DoRecognizeL(const TDesC& aName, const TDesC8& aBuffer)
	{
	iConfidence = ENotRecognized;

	const TInt positionOfLastDot=aName.LocateReverse('.');

    if (positionOfLastDot>=0)
		{//check the extension
		const TPtrC ext=aName.Mid(positionOfLastDot);
		_LIT(KDotHtml,".html");
		_LIT(KDotHtm,".htm");
		_LIT(KDotShtml,".shtml");
		_LIT(KDotShtm,".shtm");
		_LIT(KDotXhtml,".xhtml");
		_LIT(KDotXml,".xml");

		if (ext.CompareF(KDotHtml)==0 || ext.CompareF(KDotHtm)==0
			|| ext.CompareF(KDotShtml)==0 || ext.CompareF(KDotShtm)==0
			|| ext.CompareF(KDotXhtml)==0)
			{
			iDataType=TDataType(KHtmlMimeType);
			iConfidence=EProbable;
			}
		else if (ext.CompareF(KDotXml)==0)
			{
			iDataType=TDataType(KXmlMimeType);
			iConfidence=EProbable;
			}
		}

	if (aBuffer.Length() > 0 && iConfidence == ENotRecognized)
		{ //don't check data if extension is recognised, otherwise check data file
		TInt htmlConfidence = 0;
		TInt xmlConfidence = 0;

													// Description				        Valid in
													//								HTML  XHTML  XML
													//
		_LIT8(KXml, "<?xml");						// Header info					      X      X
		_LIT8(KEmbeddedXml, "<xml");				// Embbeded XML content			X     X
		_LIT8(KXmlScheme, "<xs:");					// Schema						             X
		_LIT8(KHtml, "<html");						// HTML document				X     X
		_LIT8(KBody, "<body");						// Body element					X     X
		_LIT8(KTitle, "<title");					// Document title				X     X
		_LIT8(KHead, "<head");						// Information about document	X     X
		_LIT8(KDocType, "<!doctype");				// Document type				X     X      X
		_LIT8(KHtmlDocType, "<!DOCTYPE html");		// H/XHTML document				X     X
		_LIT8(KXmlStyle, "<?xml-");					// Stylesheet					             X

		if (aBuffer.FindF(KXml) >=0)
			{
			++xmlConfidence;
			}

		if (aBuffer.FindF(KEmbeddedXml) >=0)
			{
			++htmlConfidence;
			}

		if (aBuffer.FindF(KXmlScheme) >=0)
			{
			++xmlConfidence;
			}

		if (aBuffer.FindF(KHtml) >=0)
			{
			++htmlConfidence;
			}

		if (aBuffer.FindF(KBody) >=0)
			{
			++htmlConfidence;
			}

		if (aBuffer.FindF(KTitle) >=0)
			{
			++htmlConfidence;
			}

		if (aBuffer.FindF(KHead) >=0)
			{
			++htmlConfidence;
			}

		if (aBuffer.FindF(KDocType) >=0)
			{
			++xmlConfidence;
			++htmlConfidence;
			}

		if (aBuffer.FindF(KHtmlDocType) >=0)
			{
			++htmlConfidence;
			}

		if (aBuffer.FindF(KXmlStyle) >=0)
			{
			++xmlConfidence;
			}

		// Use the xmlConfidence and htmlConfidence values and decide on the type.
		// The datatype will be set to text/xml if 2 or more xml tags match.
		// If there are 3 or more html tags the data type is overridden to text/html
		// as if we have xml and html tags, it is likely that it is an xhtml file
		// which has the text.html datatype. If there are more than 3 html tags
		// the confidence is increased.
		// If there is an extension but it doesn't match data, knock down confidence.
		if(xmlConfidence>1)
			{
			iDataType=TDataType(KXmlMimeType);
			//knock down confidence if extension is not recognised
			iConfidence=(positionOfLastDot>=0) ? EPossible : EProbable;
			}

		if(htmlConfidence>2)
			{
			iDataType=TDataType(KHtmlMimeType);
			//knock down confidence if extension is not recognised
			iConfidence=(positionOfLastDot>=0) ? EPossible : EProbable;
			}

		if(htmlConfidence>3)
			iConfidence=EProbable;
		}
	}

#ifdef __UI_FRAMEWORKS_V2__

const TImplementationProxy ImplementationTable[]=
	{
    IMPLEMENTATION_PROXY_ENTRY(0x10001315,CApaWebRecognizer::NewL)
	};

EXPORT_C const TImplementationProxy* ImplementationGroupProxy(TInt& aTableCount)
    {
    aTableCount = sizeof(ImplementationTable) / sizeof(TImplementationProxy);
    return ImplementationTable;
    }

CApaDataRecognizerType* CApaWebRecognizer::NewL()
	{
    return new (ELeave) CApaWebRecognizer();
    }

#else

EXPORT_C CApaDataRecognizerType* CreateRecognizer()
// The gate function - ordinal 1
//
	{
	CApaDataRecognizerType* self = new CApaWebRecognizer();
	return self; // NULL if new failed
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

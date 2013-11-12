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
// RECScheme.CPP
// 
//

#include <apmrec.h>
#include <apmstd.h>
#include <f32file.h>
#include "weburlrec.h"

#ifdef __UI_FRAMEWORKS_V2__
#include <ecom/implementationproxy.h>
#endif //__UI_FRAMEWORKS_V2__

const TInt KMimeWebUrlRecognizerValue = 0x100064DE;
const TUid KUidWebUrlRecognizer = {KMimeWebUrlRecognizerValue};
const TInt KSchemeNumMimeTypes = 6; // http, https, file, mailto, sms, mms are recognised by this plugin

_LIT( KHttpScheme , "http" );       _LIT8( KHttpUrlDataType , "X-Epoc-Url/http" ) ;		//may be either web or wap
_LIT( KHttpsScheme , "https" );     _LIT8( KHttpsUrlDataType , "X-Epoc-Url/https" );	//may be either web or wap
_LIT( KFileScheme , "file" );       _LIT8( KFileUrlDataType , "X-Epoc-Url/file" );		//may be either web or wap
//Messaging schemes
_LIT( KMailtoScheme , "mailto" );	_LIT8( KMailtoUrlDataType , "X-Epoc-Url/mailto" ); 
_LIT( KSmsScheme , "sms" );			_LIT8( KSmsUrlDataType , "X-Epoc-Url/sms" );
_LIT( KMmsScheme , "mms" );			_LIT8( KMmsUrlDataType , "X-Epoc-Url/mms" );

const TInt KWebUrlRecognizerStringLength = 255;


CApaSchemeRecognizer::CApaSchemeRecognizer()
	:CApaDataRecognizerType(KUidWebUrlRecognizer,CApaDataRecognizerType::ENormal)
	{
	iConfidence = ENotRecognized;
	iCountDataTypes = KSchemeNumMimeTypes;
	}

TUint CApaSchemeRecognizer::PreferredBufSize()
	{
	return KWebUrlRecognizerStringLength;
	}

TDataType CApaSchemeRecognizer::SupportedDataTypeL(TInt aIndex) const
	// this is a special supported data type X-EPOC-Scheme for urls
	{
	__ASSERT_DEBUG(aIndex>=0 && aIndex<KSchemeNumMimeTypes,User::Invariant());//assert if out of range

	switch (aIndex)
		{
	case 0:
		return TDataType(KHttpUrlDataType);
	case 1:
		return TDataType(KHttpsUrlDataType);
	case 2:
		return TDataType(KFileUrlDataType);
	case 3:
		return TDataType(KMailtoUrlDataType);
	case 4:
		return TDataType(KSmsUrlDataType);
	case 5:
		return TDataType(KMmsUrlDataType);
	default:
		User::Leave(KErrNotSupported);
		}
	return TDataType();
	}

void CApaSchemeRecognizer::DoRecognizeL(const TDesC& aName, const TDesC8& /*aBuffer*/)
	{
	iConfidence = ECertain;
	if (FindScheme(KHttpScheme, aName))
		{
		iDataType = TDataType(KHttpUrlDataType);
		return;
		}
	if (FindScheme(KHttpsScheme, aName))
		{
		iDataType = TDataType(KHttpsUrlDataType);
		return;
		}
	if (FindScheme(KFileScheme, aName))
		{
		iDataType = TDataType(KFileUrlDataType);
		return;
		}
	if (FindScheme(KMailtoScheme, aName))
		{
		iDataType = TDataType(KMailtoUrlDataType);
		return;
		}
	if (FindScheme(KSmsScheme, aName))
		{
		iDataType = TDataType(KSmsUrlDataType);
		return;
		}
	if (FindScheme(KMmsScheme, aName))
		{
		iDataType = TDataType(KMmsUrlDataType);
		return;
		}
	iConfidence = ENotRecognized;
	}

TBool CApaSchemeRecognizer::FindScheme(const TDesC& aScheme, const TDesC& aUrl)
	{
	TInt colonPos = aUrl.Locate(':');
	TInt schemePos = KErrNotFound;
	schemePos = aUrl.FindF(aScheme);

	if (schemePos >= 0 && aScheme.Length() == colonPos) // we have found a scheme that is the bit before the ':'
        {
		return ETrue;
        }

	return EFalse;
	}


#ifdef __UI_FRAMEWORKS_V2__

const TImplementationProxy ImplementationTable[] =
	{
    IMPLEMENTATION_PROXY_ENTRY(0x100064DE,CApaSchemeRecognizer::NewL)
	};

EXPORT_C const TImplementationProxy* ImplementationGroupProxy(TInt& aTableCount)
    {
    aTableCount = sizeof(ImplementationTable) / sizeof(TImplementationProxy);
    return ImplementationTable;
    }

CApaDataRecognizerType* CApaSchemeRecognizer::NewL()
	{
    return new (ELeave) CApaSchemeRecognizer();
    }

#else

EXPORT_C CApaDataRecognizerType* CreateRecognizer()
// The gate function - ordinal 1
//
	{
	CApaDataRecognizerType* thing = new CApaSchemeRecognizer();
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

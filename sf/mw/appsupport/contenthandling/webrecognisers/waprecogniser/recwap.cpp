// Copyright (c) 2002-2009 Nokia Corporation and/or its subsidiary(-ies).
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

// header
#include "recwap.h"

// other includes
#include <wapmime.h> // constants
#include <f32file.h> // TParse

#ifdef __UI_FRAMEWORKS_V2__
#include <ecom/implementationproxy.h>
#endif //__UI_FRAMEWORKS_V2__


//
// CApaWapRecognizer
//
CApaWapRecognizer::CApaWapRecognizer()
:CApaDataRecognizerType(KUidMimeWapRecognizer,CApaDataRecognizerType::ENormal)
	{
	// All these mime types have reasonable recognition
	iCountDataTypes=KWapNumMimeTypes;
	}

TUint CApaWapRecognizer::PreferredBufSize()
	{
	// no buffer recognition yet
	return 0;
	}

TDataType CApaWapRecognizer::SupportedDataTypeL(TInt aIndex) const
	{
	__ASSERT_DEBUG(aIndex>=0 && aIndex<KWapNumMimeTypes,User::Invariant());
	switch (aIndex)
		{
	case 0:
		return TDataType(KWMLMimeType);
	case 1:
		return TDataType(KWMLCMimeType);
	case 2:
		return TDataType(KWMLDTDMimeType);
	case 3:
		return TDataType(KWBXMLMimeType);
	case 4:
	default:
		return TDataType(KWMLSCMimeType);
		}
	}

void CApaWapRecognizer::DoRecognizeL(const TDesC& aName, const TDesC8& /*aBuffer*/)
	{
	if (aName.Length()<3)
		return;

	TInt dotPos = aName.LocateReverse('.');
	if (dotPos == KErrNotFound)
		return;
	TInt extLength = aName.Length() - dotPos;

	HBufC* ext = aName.Right(extLength).AllocL();
	CleanupStack::PushL(ext);

	if (ext->CompareF(KDotWML)==0)
		{
		iDataType=TDataType(KWMLMimeType);
		iConfidence=EProbable;
		}
	else if (ext->CompareF(KDotWMLC)==0)
		{
		iDataType=TDataType(KWMLCMimeType);
		iConfidence=EProbable;
		}
	else if (ext->CompareF(KDotWMLSC)==0)
		{
		iDataType=TDataType(KWMLSCMimeType);
		iConfidence=EProbable;
		}
	else if (ext->CompareF(KDotWMLS)==0)
		{
		iDataType=TDataType(KWMLSMimeType);
		iConfidence=EProbable;
		}
	else if (ext->CompareF(KDotWBXML)==0)
		{
		iDataType=TDataType(KWBXMLMimeType);
		iConfidence=EProbable;
		}
	else if (ext->CompareF(KDotWDTD)==0)
		{
		iDataType=TDataType(KWMLDTDMimeType);
		iConfidence=EProbable;
		}
	CleanupStack::PopAndDestroy();	// ext
	}

#ifdef __UI_FRAMEWORKS_V2__

const TImplementationProxy ImplementationTable[]=
	{
	IMPLEMENTATION_PROXY_ENTRY(0x1000515E,CApaWapRecognizer::NewL)
	};

EXPORT_C const TImplementationProxy* ImplementationGroupProxy(TInt& aTableCount)
    {
    aTableCount = sizeof(ImplementationTable) / sizeof(TImplementationProxy);
    return ImplementationTable;
    }

CApaDataRecognizerType* CApaWapRecognizer::NewL()
	{
    return new (ELeave) CApaWapRecognizer();
    }

#else

EXPORT_C CApaDataRecognizerType* CreateRecognizer()
//
// The gate function - ordinal 1
//
	{
	CApaDataRecognizerType* thing=new CApaWapRecognizer();
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
#endif

// Copyright (c) 2010 Nokia Corporation and/or its subsidiary(-ies).
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
// This recognizer is part of the supporting test code for T_ForceRegStep.CPP
// 
//

/**
 @file
 @internalComponent - Internal Symbian test code
*/

#include <e32std.h>
#include <e32base.h>
#include <apmstd.h>
#include <apmrec.h>
#include <f32file.h>

#include <apfrec.h>
#include <ecom/implementationproxy.h> 

_LIT8(KLit8_DataType_ForceRegApp1, "x-epoc/forcregapp1");
_LIT8(KLit8_DataType_ForceRegApp2, "x-epoc/forcregapp2");
_LIT8(KLit8_DataType_ForceRegApp3, "x-epoc/forcregapp3");
_LIT8(KLit8_DataType_ForceRegApp4, "x-epoc/forcregapp4");

const TUid KUidForceRegAppRecognizer={0xA0001010};
const TUint KImplForceRegAppRecognizer=0xA0001010;

enum TMimeTypes
    {
    EMimeType1 = 0,
    EMimeType2,
    EMimeType3,
    EMimeType4,
    EMimeLast
    };

_LIT(KLitMimeExtension1, ".FRG1");
_LIT(KLitMimeExtension2, ".FRG2");
_LIT(KLitMimeExtension3, ".FRG3");
_LIT(KLitMimeExtension4, ".FRG4");


// CTForceRegAppRec definition

class CTForceRegAppRec : public CApaDataRecognizerType
	{
public:
	CTForceRegAppRec();
	static CApaDataRecognizerType* CreateRecognizerL();
private:
	// from CApaDataRecognizerType
	virtual TUint PreferredBufSize();
	virtual TDataType SupportedDataTypeL(TInt aIndex) const;
	virtual void DoRecognizeL(const TDesC& aName, const TDesC8& aBuffer);
	};


// CTForceRegAppRec implementation

CTForceRegAppRec::CTForceRegAppRec() 
	:CApaDataRecognizerType(KUidForceRegAppRecognizer, EHigh)
	{
	iCountDataTypes = EMimeLast;
	} 

TUint CTForceRegAppRec::PreferredBufSize()
	{
	return 0;
	}

TDataType CTForceRegAppRec::SupportedDataTypeL(TInt aIndex) const
	{
	if (aIndex == EMimeType1)
		return TDataType(KLit8_DataType_ForceRegApp1);
    
    else if (aIndex == EMimeType2)
		return TDataType(KLit8_DataType_ForceRegApp2);
    
    else if (aIndex == EMimeType3)
		return TDataType(KLit8_DataType_ForceRegApp3);
    
    else if (aIndex == EMimeType4)
		return TDataType(KLit8_DataType_ForceRegApp4);
    
    else
        return TDataType(KNullDesC8);
    }
 

void CTForceRegAppRec::DoRecognizeL(const TDesC& aName, const TDesC8&)
	{

	// Compare if the file extension is known
	if (aName.Length() < 5)
		{
    	iDataType = TDataType(KNullDesC8); 	
    	iConfidence = ENotRecognized;
		return;
		}

	if (aName.Right(5).CompareF(KLitMimeExtension1) == 0)
		{
		iDataType = TDataType(KLit8_DataType_ForceRegApp1);
		iConfidence = ECertain;
		}
	else if (aName.Right(5).CompareF(KLitMimeExtension2) == 0)
		{
		iDataType = TDataType(KLit8_DataType_ForceRegApp2);
		iConfidence = ECertain;
		}
	else if (aName.Right(5).CompareF(KLitMimeExtension3) == 0)
		{
		iDataType = TDataType(KLit8_DataType_ForceRegApp3);
		iConfidence = ECertain;
		}
	else if (aName.Right(5).CompareF(KLitMimeExtension4) == 0)
		{
		iDataType = TDataType(KLit8_DataType_ForceRegApp4);
		iConfidence = ECertain;
		}
    else
    	{
    	iDataType = TDataType(KNullDesC8); 	
    	iConfidence = ENotRecognized;
    	}
	}

// stand-alone functions

CApaDataRecognizerType* CTForceRegAppRec::CreateRecognizerL()
	{
	return new (ELeave) CTForceRegAppRec();
	}

const TImplementationProxy ImplementationTable[] = 
    {
	IMPLEMENTATION_PROXY_ENTRY(KImplForceRegAppRecognizer, CTForceRegAppRec::CreateRecognizerL)
	};

EXPORT_C const TImplementationProxy* ImplementationGroupProxy(TInt& aTableCount)
    {
    aTableCount = sizeof(ImplementationTable) / sizeof(TImplementationProxy);
    return ImplementationTable;
    }


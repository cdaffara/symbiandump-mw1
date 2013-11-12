// Copyright (c) 2005-2009 Nokia Corporation and/or its subsidiary(-ies).
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

/**
 @file
 @internalComponent - Internal Symbian test code  
*/

#include <conarc.h>
#include <ecom/ecom.h>
#include <ecom/implementationproxy.h>
#include "TTestEcomConverter.hrh" 

const TUid KUidTTestEcomConverter={KTestEcomConvImplementationUid};

#include <e32std.h>
#include <e32uid.h>

class CTestEcomConverter : public CConverterBase2
	{
public: // from CConverterBase2
	static CConverterBase2* NewL();
	CTestEcomConverter();
	~CTestEcomConverter();
	void Release();
	void ConvertL(const TFileName& aSourceFile, const TFileName& aTargetFile, MConverterUiObserver* aObserver=NULL);
	void ConvertObjectL(RReadStream& aReadStream, RWriteStream& aWriteStream,   MConverterUiObserver* aObserver=NULL);
	void ConvertAL(const TFileName& , const TFileName&, MConverterUiObserver* ) {}
	void ConvertObjectAL(RReadStream& , RWriteStream&, MConverterUiObserver* ) {}
	TBool DoConvertL() { return EFalse; }
	TUid Uid();
	};


CConverterBase2* CTestEcomConverter::NewL()
	{
	CConverterBase2* testEcomConverter=new (ELeave) CTestEcomConverter();
	return testEcomConverter;
	}

const TImplementationProxy ImplementationTable[] = 
    {
	IMPLEMENTATION_PROXY_ENTRY(KTestEcomConvImplementationUid,CTestEcomConverter::NewL)
    };

EXPORT_C const TImplementationProxy* ImplementationGroupProxy(TInt& aTableCount)
    {
    aTableCount = sizeof(ImplementationTable) / sizeof(TImplementationProxy);
    return ImplementationTable;
    }

//
// class CTestEcomConverter
// 

void CTestEcomConverter::ConvertL(const TFileName& , const TFileName& ,  MConverterUiObserver* )
	{
	User::InfoPrint(_L("TTESTECOMCONVERTER file conversion"));
	}

void CTestEcomConverter::ConvertObjectL(RReadStream& , RWriteStream& ,   MConverterUiObserver* )
	{
	User::InfoPrint(_L("TTESTECOMCONVERTER byte stream conversion"));
	}

TUid CTestEcomConverter::Uid()
	{
	return KUidTTestEcomConverter;
	}

CTestEcomConverter::CTestEcomConverter()
	{}

CTestEcomConverter::~CTestEcomConverter()
	{
	}

GLDEF_C TInt E32Dll(
					)
	{
	return(KErrNone);
	}    



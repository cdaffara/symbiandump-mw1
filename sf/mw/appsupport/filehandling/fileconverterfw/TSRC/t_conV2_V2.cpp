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
// A converter module used for testing the version 2 CNF format
// 
//

/**
 @file
 @internalComponent - Internal Symbian test code
*/

#include <conarc.h>

#include <ecom/ecom.h>
#include <ecom/implementationproxy.h>
#include "T_ConV2.hrh" 

//const TUid KUidTConV2 = {0x10004c42};
const TUid KUidTConV2={KTConV2ImplementationUid};

#include <e32std.h>
#include <e32uid.h>

// CTestConverterV2
class CTestConverterV2 : public CConverterBase2
	{
public:
	static CConverterBase2* NewL();
	~CTestConverterV2();
	void Release();

	CTestConverterV2();
	// from CConverterBase2
public:
	void ConvertL(const TFileName& aSourceFile, const TFileName& aTargetFile,  MConverterUiObserver* aObserver=NULL);
	void ConvertObjectL(RReadStream& aReadStream, RWriteStream& aWriteStream,  MConverterUiObserver* aObserver=NULL);
	TUid Uid();
	};



CConverterBase2* CTestConverterV2::NewL()
	{
	CConverterBase2* ttestconV2=new (ELeave) CTestConverterV2();
	return ttestconV2;
	}
	
const TImplementationProxy ImplementationTable[] = 
    {
	IMPLEMENTATION_PROXY_ENTRY(KTConV2ImplementationUid,CTestConverterV2::NewL)
    };

EXPORT_C const TImplementationProxy* ImplementationGroupProxy(TInt& aTableCount)
    {
    aTableCount = sizeof(ImplementationTable) / sizeof(TImplementationProxy);
    return ImplementationTable;
    }
    
//
// class CTestConverterV2
// 
void CTestConverterV2::ConvertL(const TFileName& , const TFileName& ,  MConverterUiObserver* )
	{
	User::InfoPrint(_L("CTestConverterV2 file conversion"));
	}

void CTestConverterV2::ConvertObjectL(RReadStream& , RWriteStream& ,  MConverterUiObserver* )
	{
	User::InfoPrint(_L("CTestConverterV2 byte stream conversion"));
	}

TUid CTestConverterV2::Uid()
	{
	return KUidTConV2;
	}

CTestConverterV2::CTestConverterV2()
	{
	}

CTestConverterV2::~CTestConverterV2()
	{}
	
// Entry point
GLDEF_C TInt E32Dll(
					)
	{
	return KErrNone;
	}    


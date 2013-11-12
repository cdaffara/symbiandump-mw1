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
// A converter module used for testing a CNV with wrong middle UID
// 
//

/**
 @file
 @internalComponent - Internal Symbian test code
*/

#include <conarc.h>
#include <ecom/ecom.h>
#include <ecom/implementationproxy.h>
#include "t_convBadUid.hrh" 

const TUid KUidTConvBadUid = {KTBadUidConvImplementationUid};

#include <e32std.h>
#include <e32uid.h>

// CTestConverterBadUid
class CTestConverterBadUid : public CConverterBase2
	{
public:
	static CConverterBase2* NewL();
	CTestConverterBadUid();
	~CTestConverterBadUid();
	void Release();
	// from CConverterBase2
public:
	void ConvertL(const TFileName& aSourceFile, const TFileName& aTargetFile,  MConverterUiObserver* aObserver=NULL);
	void ConvertObjectL(RReadStream& aReadStream, RWriteStream& aWriteStream,  MConverterUiObserver* aObserver=NULL);
	TUid Uid();
	};



CConverterBase2* CTestConverterBadUid::NewL()
	{
	CConverterBase2* testBadUidConverter=new (ELeave) CTestConverterBadUid();
	return testBadUidConverter;
	}

const TImplementationProxy ImplementationTable[] = 
    {
	IMPLEMENTATION_PROXY_ENTRY(KTBadUidConvImplementationUid,CTestConverterBadUid::NewL)
    };

EXPORT_C const TImplementationProxy* ImplementationGroupProxy(TInt& aTableCount)
    {
    aTableCount = sizeof(ImplementationTable) / sizeof(TImplementationProxy);
    return ImplementationTable;
    }
    
//
// class CTestConverterBadUid
// 
void CTestConverterBadUid::ConvertL(const TFileName& , const TFileName& ,  MConverterUiObserver* )
	{
	User::InfoPrint(_L("CTestConverterBadUid file conversion"));
	}

void CTestConverterBadUid::ConvertObjectL(RReadStream& , RWriteStream& ,  MConverterUiObserver* )
	{
	User::InfoPrint(_L("CTestConverterBadUid byte stream conversion"));
	}

TUid CTestConverterBadUid::Uid()
	{
	return KUidTConvBadUid;
	}

CTestConverterBadUid::CTestConverterBadUid()
	{
	}
	
CTestConverterBadUid::~CTestConverterBadUid()
	{
	}

// Entry point
GLDEF_C TInt E32Dll(
					)
	{
	return KErrNone;
	}    


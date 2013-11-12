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
#include "TCon2.hrh" 

//const TUid KUidTCON2={100124};
const TUid KUidTcon2={KTCon2ImplementationUid};

#include <e32std.h>
#include <e32uid.h>

class CTCON2 : public CConverterBase2
	{
public: // from CConverterBase2
	static CConverterBase2* NewL();
	CTCON2();
	~CTCON2();
	void Release();
	void ConvertL(const TFileName& aSourceFile, const TFileName& aTargetFile,  MConverterUiObserver* aObserver=NULL);
	void ConvertObjectL(RReadStream& aReadStream, RWriteStream& aWriteStream,  MConverterUiObserver* aObserver=NULL);
	TUid Uid();
	};


CConverterBase2* CTCON2::NewL()
	{
	CConverterBase2* tcon2=new (ELeave) CTCON2();
	return tcon2;
	}
	
const TImplementationProxy ImplementationTable[] = 
    {
	IMPLEMENTATION_PROXY_ENTRY(KTCon2ImplementationUid,CTCON2::NewL)
    };

EXPORT_C const TImplementationProxy* ImplementationGroupProxy(TInt& aTableCount)
    {
    aTableCount = sizeof(ImplementationTable) / sizeof(TImplementationProxy);
    return ImplementationTable;
    }


//
// class CTCON2
// 

void CTCON2::ConvertL(const TFileName& , const TFileName& ,  MConverterUiObserver* )
	{
	User::InfoPrint(_L("TCON2 file conversion"));
	}

void CTCON2::ConvertObjectL(RReadStream& , RWriteStream& ,  MConverterUiObserver* )
	{
	User::InfoPrint(_L("TCON2 byte stream conversion"));
	}

TUid CTCON2::Uid()
	{
	return KUidTcon2;
	}

CTCON2::CTCON2()
    {}

CTCON2::~CTCON2()
	{}

GLDEF_C TInt E32Dll(
					)
	{
	return(KErrNone);
	}    


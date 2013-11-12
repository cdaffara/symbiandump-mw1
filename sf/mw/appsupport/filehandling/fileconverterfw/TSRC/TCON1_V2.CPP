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
#include "TCon1.hrh" 

//const TUid KUidTcon1={100123};//put in hrh file
const TUid KUidTcon1={KTCon1ImplementationUid};

#include <e32std.h>
#include <e32uid.h>

class CTcon1 : public CConverterBase2 //changed base class
	{
public: // from CConverterBase2
	static CConverterBase2* NewL();
	CTcon1();
	~CTcon1();
	void Release();
	void ConvertL(const TFileName& aSourceFile, const TFileName& aTargetFile, MConverterUiObserver* aObserver=NULL);
	void ConvertObjectL(RReadStream& aReadStream, RWriteStream& aWriteStream,   MConverterUiObserver* aObserver=NULL);
	void ConvertAL(const TFileName& , const TFileName&, MConverterUiObserver* ) {}
	void ConvertObjectAL(RReadStream& , RWriteStream&, MConverterUiObserver* ) {}
	TBool DoConvertL() { return EFalse; }
	TUid Uid();
	};


CConverterBase2* CTcon1::NewL()
	{
	CConverterBase2* tcon1=new (ELeave) CTcon1();
	return tcon1;
	}
	
const TImplementationProxy ImplementationTable[] = 
    {
	IMPLEMENTATION_PROXY_ENTRY(KTCon1ImplementationUid,CTcon1::NewL)
    };

EXPORT_C const TImplementationProxy* ImplementationGroupProxy(TInt& aTableCount)
    {
    aTableCount = sizeof(ImplementationTable) / sizeof(TImplementationProxy);
    return ImplementationTable;
    }



//
// class CTcon1
// 

void CTcon1::ConvertL(const TFileName& , const TFileName& ,  MConverterUiObserver* )
	{
	User::InfoPrint(_L("TCON1 file conversion"));
	}

void CTcon1::ConvertObjectL(RReadStream& , RWriteStream& ,   MConverterUiObserver* )
	{
	User::InfoPrint(_L("TCON1 byte stream conversion"));
	}

TUid CTcon1::Uid()
	{
	return KUidTcon1;
	}

CTcon1::CTcon1()
	{}

CTcon1::~CTcon1()
	{}

GLDEF_C TInt E32Dll(
					)
	{
	return(KErrNone);
	}   


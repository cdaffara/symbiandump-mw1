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
#include "TCON3_V2.H"
#include <ecom/ecom.h>
#include <ecom/implementationproxy.h>
#include "TCon3.hrh" 

const TUid KUidTcon3={KTCon3ImplementationUid};

#include <e32std.h>
#include <e32uid.h>

// Implementation of virtual interface classes
class CTcon3Extended1Impl : public CTcon3Extended1
	{
public:
	void TestFunction1();
	void FileExtension(TDes& aDes);
	};


class CTcon3Extended2Impl : public CTcon3Extended2
	{
public:
	void TestFunction2(TDes& aDes);
	};


GLDEF_C TInt E32Dll(
					)
	{
	return(KErrNone);
	}    

CConverterBase2* CTcon3::NewL()
	{
	CConverterBase2* tcon3=new (ELeave) CTcon3();
	return tcon3;
	}
	
const TImplementationProxy ImplementationTable[] = 
    {
	IMPLEMENTATION_PROXY_ENTRY(KTCon3ImplementationUid,CTcon3::NewL)
    };

EXPORT_C const TImplementationProxy* ImplementationGroupProxy(TInt& aTableCount)
    {
    aTableCount = sizeof(ImplementationTable) / sizeof(TImplementationProxy);
    return ImplementationTable;
    }
    
//
// class CTcon3
// 

CTcon3::CTcon3()
	{}

CTcon3::~CTcon3()
	{
	}

TUid CTcon3::Uid()
	{
	return KUidTcon3;
	}

void CTcon3::ExtendedInterfaceL(TUid aInterfaceUid, CBase*& aInterface)
	{
	switch (aInterfaceUid.iUid)
		{
	case KUid1:
		aInterface = new(ELeave) (CTcon3Extended1Impl);
		break;
	case KUid2:
		aInterface = new(ELeave) (CTcon3Extended2Impl);
		break;
	default:
		aInterface = NULL;
		break;
		}
	}


// Extended Interface classes

/** Sample implementation of an extended function - this could
    be anything sensisble used to extend the connarc interface
*/
void CTcon3Extended1Impl::FileExtension(TDes& aDes)
	{
	_LIT(KFileExt, "doc");
	aDes.Append(KFileExt);
	}


void CTcon3Extended2Impl::TestFunction2(TDes& aDes)
	{
	_LIT(KFuncName, "TestFunction2");
	aDes.Append(KFuncName);	
	}

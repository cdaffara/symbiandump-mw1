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
// TCon3 is a simple test conarc plug-in used by the test program TExtInterface
// The idea of this plug in is to provide several 'extended interfaces' which get
// instantiated via CConverterBase2::ExtendedInterfaceL
// These extended interfaces are defined by several abstract base classes below. 
// Derived clases provide concrete implementations of these classes in TCon3.cpp 
// 
//

/**
 @file
 @internalComponent - Internal Symbian test code
*/


const TInt KUid1=120125;
const TUid KInterfaceUid1={KUid1};
const TInt KUid2=120126;
const TUid KInterfaceUid2={KUid2};
const TInt KUid3=120127;
const TUid KInterfaceUid3={KUid3};

class CTcon3 : public CConverterBase2 //changed base class
	{
public: // from CConverterBase2
	static CConverterBase2* NewL();
	CTcon3();
	~CTcon3();
	void Release();
	void ConvertAL(const TFileName& , const TFileName&, MConverterUiObserver* ) {}
	void ConvertObjectAL(RReadStream& , RWriteStream&, MConverterUiObserver* ) {}
	TBool DoConvertL() { return EFalse; }
	TUid Uid();
	void ExtendedInterfaceL(TUid aInterfaceUid, CBase*& aInterface);
	};


/** Abstract base classes define the 'extended' interfaces that are used by
    CConverterBase2::ExtendedInterfaceL, which will instantiate concrete
    implemenations of classes that derive from these abstract base classes
*/
class CTcon3Extended1 : public CBase
	{
public:
	virtual void FileExtension(TDes& aDes)=0;
	};

class CTcon3Extended2 : public CBase
	{
public:
	virtual void TestFunction2(TDes& aDes)=0;
	};

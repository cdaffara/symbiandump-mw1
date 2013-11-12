// Copyright (c) 1997-2009 Nokia Corporation and/or its subsidiary(-ies).
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

#ifndef __APAID_PARTNER_H__
#define __APAID_PARTNER_H__

#if !defined(__E32BASE_H__)
#include <e32base.h>
#endif
#if !defined(__APADEF_H__)
#include <apadef.h>
#endif

#include "apmstd.h"

// classes referenced:
class RReadStream;
class RWriteStream;

/**
@internalTechnology
@released 
*/
IMPORT_C TPtrC NameApaServServerThread();


/** 
The uid for the Open service.

@publishedPartner
@released
*/
const TUid KOpenServiceUid = { 0x10208DCA };

/** Application service information.

Encapsulates a service UID and associated opaque data.

An instance of this class provides information about
a specific implementation of the service identified by
the encapsulated service UID.

@publishedPartner
@released
*/
class TApaAppServiceInfo
	{
public:
	IMPORT_C TApaAppServiceInfo();
	IMPORT_C TApaAppServiceInfo(TUid aUid, CArrayFixFlat<TDataTypeWithPriority>* aDataTypes, HBufC8* aOpaqueData);
	IMPORT_C void ExternalizeL(RWriteStream& aStream) const;
	IMPORT_C void InternalizeL(RReadStream& aStream);
	IMPORT_C void Release();
	IMPORT_C CArrayFixFlat<TDataTypeWithPriority>& DataTypes();
	IMPORT_C TUid Uid() const;
	IMPORT_C const CArrayFixFlat<TDataTypeWithPriority>& DataTypes() const;
	IMPORT_C const TDesC8& OpaqueData() const;
private:
	TUid iUid;
	CArrayFixFlat<TDataTypeWithPriority>* iDataTypes;
	HBufC8* iOpaqueData;
	TInt iTApaAppServiceInfo;
	};

/** Application service information array.

Owns an array of TApaAppServiceInfo objects.

@see TApaAppServiceInfo
@publishedPartner
@released
*/
class CApaAppServiceInfoArray : public CBase
	{
protected:
	IMPORT_C CApaAppServiceInfoArray();
public:
	/** Provides access to the encapsulated array of
	TApaAppServiceInfo objects.

	@return A generic array of TApaAppServiceInfo objects. */
	virtual TArray<TApaAppServiceInfo> Array()=0;
private:
	IMPORT_C virtual void CApaAppServiceInfoArray_Reserved1();
	IMPORT_C virtual void CApaAppServiceInfoArray_Reserved2();
private:
	TInt iCApaAppServiceInfoArray_Reserved1;
	};

#endif

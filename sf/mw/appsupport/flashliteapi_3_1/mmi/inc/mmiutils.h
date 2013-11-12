/*
* Copyright (c) 2006 Nokia Corporation and/or its subsidiary(-ies).
* All rights reserved.
* This component and the accompanying materials are made available
* under the terms of "Eclipse Public License v1.0"
* which accompanies this distribution, and is available
* at the URL "http://www.eclipse.org/legal/epl-v10.html".
*
* Initial Contributors:
* Nokia Corporation - initial contribution.
*
* Contributors:
*
* Description:  Defines the utility interface class.
*
*/



#ifndef __MMI_UTILS_H__
#define __MMI_UTILS_H__

#include <e32base.h>
#include <eikenv.h>

class TMMIKeyValuePair
{
public:
	const TDesC* iKey;
	const TDesC* iVal;
};

const TUint8 KDataRequestSuccess = 1;
const TUint8 KDataRequestFail = 0;

class MMIUtils
{
protected: // this class is not for instantiation
	inline MMIUtils() {}
public:

	virtual CEikonEnv* GetEikonEnv() = 0;


	virtual void SendEvent(RPointerArray<TDesC>& aArgs) = 0;

	virtual TInt GetIntFromDescL(const TDesC& aDesc) = 0;
	
	virtual short SetReturnStringFromKeyValuePairsL(const CArrayFix<TMMIKeyValuePair>* aArray, const TAny* aRetVal) = 0;
	virtual short SetReturnStringFromIntL(TInt aInt, const TAny* aRetVal) = 0;
	virtual short SetReturnStringFromDes16L(const TDesC& aDes, const TAny* aRetVal) = 0;

	// data request functions
	virtual void DataRequestSetStringProperty(const TAny* aRequestHandle, const TDesC& aKey, const TDesC& aValue) = 0;
	virtual void DataRequestSetNumberProperty(const TAny* aRequestHandle, const TDesC& aKey, TInt aValue) = 0;
	virtual TAny* DataRequestCreateObjectProperty(const TAny* aRequestHandle, const TDesC& aKey) = 0;
	virtual void DataRequestSetComplete(const TAny* aRequestHandle, TUint8 aSuccess) = 0;

};

class MObjectOwner
{
public:
	virtual void DeleteObject(CBase* aObject)=0;
};

class TValueKeyPairParser
{
public:
	inline TValueKeyPairParser(const TDesC& aString) :iContainer(aString),iLastWasKey(EFalse) { iRemaining=iContainer.Length(); }

public:
	inline TPtrC NextKey()
	{
		ASSERT(!iLastWasKey);
		iLastWasKey=ETrue;
		return GetNext(TChar('='));
	}
	inline TPtrC NextValue()
	{
		ASSERT(iLastWasKey);
		iLastWasKey=EFalse;
		return GetNext(TChar('&'));
	}
	TBool Done() const
	{
		return(iRemaining==0);
	}
private:
	inline TPtrC GetNext(TChar aChar)
	{
		TPtrC temp(iContainer.Right(iRemaining));
		TInt index=temp.Locate(aChar);
		TBool found = ETrue;
		
		if(index==KErrNotFound)
		{
			index=temp.Length(); // i.e. the end
			found = EFalse;
		}
		
		if (found)
			{
			iRemaining-=(index+1); // +1 to count for '=' or '&' char which is thrown away
			}
			else
			{
			iRemaining-=(index);	
			}
		return TPtrC(temp.Left(index));
	}
private:
	const TPtrC iContainer;
	TInt iRemaining;
	TBool iLastWasKey;
};

#endif // __MMI_UTILS__

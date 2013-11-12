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
* Description:  Defines the abstract base class for all types of MMI 
				 content specific extensions.
*
*/


#ifndef __MMI_DELEGATEBASE_H__
#define __MMI_DELEGATEBASE_H__

#include <e32base.h>
#include <ecom\ecomresolverparams.h>
#include <ecom\ecom.h>

//forward definitions
class MMIUtils; //mmiutils.h
struct FI_ReturnString;

/**
* The CMMI_Delegate class is an abstract base class for all types of MMI content specific extensions.
*/
class CMMI_Delegate : public CBase
{
public:
enum TDoItLOperationOpCodes
	{
		EOpCodePauseState = 0,
		EOpCodeSMCheckNeeded = 100
	};


public:
	static CMMI_Delegate* NewL(const TDesC8& aMatch, MMIUtils* aMMIUtils);

	~CMMI_Delegate() { REComSession::DestroyedImplementation (iDtor_ID_Key); }

public:

	virtual const TDesC8* Name() const = 0;

	virtual void Initialize(MMIUtils* /*aMMIUtils*/) = 0;
	virtual short GetL(const RPointerArray<TDesC>& aArgs, struct FI_ReturnString* /*retVal*/) = 0;

	virtual short GetVarsL(const RPointerArray<TDesC>& aArgs, struct FI_ReturnString* /*retVal*/) = 0;

	virtual short SetL(const RPointerArray<TDesC>& aArgs, const TDesC& /*aValue*/) =0;

	virtual void SetVarsL(const RPointerArray<TDesC>& aArgs, const TDesC& /*aValue*/, short& /*aRetVal*/) =0;

	virtual short SendL(const TDesC& aVerbArg, const RPointerArray<TDesC>& aArgs) =0;

	virtual short RequestDataL(const RPointerArray<TDesC>& /*aArgs*/, const TAny* /*aDataRequestHandle*/ )=0;

	virtual short CancelRequestL(const TAny* /*aDataRequestHandle*/) = 0;

	virtual void PlayerDestroyed() = 0;
	virtual TBool RegisterExtendedEvent(const TDesC& aEventName) = 0;
	virtual TBool UnregisterExtendedEvent(const TDesC& aEventName) = 0;
	virtual TAny* ExecuteFunctionL(TInt aOpCode, TAny* aParam1, TAny* aParam2, TAny* aParam3, TAny* aParam4) = 0;

private:
	TUid iDtor_ID_Key;

};

#endif // __MMI_DELEGATEBASE_H__
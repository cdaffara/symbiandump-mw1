/*
* Copyright (c) 2006-2010 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  
*
*/




#include <w32std.h>
#include "CDummyApplicationHandle.h"

CCDummyApplicationHandle::CCDummyApplicationHandle(TUid aUid)
: iUid(aUid)
	{
	// No implementation required
	}

CCDummyApplicationHandle::~CCDummyApplicationHandle()
	{
	if(iMsgQueue.Handle())
		{
		iMsgQueue.SendBlocking(0);
		iMsgQueue.Close();
		}
	iProcess.Close();
	}

CCDummyApplicationHandle* CCDummyApplicationHandle::NewLC(TUid aUid, TInt aExtraMemoryAllocation)
	{
	CCDummyApplicationHandle* self = new (ELeave) CCDummyApplicationHandle(aUid);
	CleanupStack::PushL(self);
	self->ConstructL(aExtraMemoryAllocation);
	return self;
	}

CCDummyApplicationHandle* CCDummyApplicationHandle::NewL(TUid aUid, TInt aExtraMemoryAllocation)
	{
	CCDummyApplicationHandle* self = CCDummyApplicationHandle::NewLC(aUid, aExtraMemoryAllocation);
	CleanupStack::Pop(); // self;
	return self;
	}

void CCDummyApplicationHandle::ConstructL(TInt aExtraMemoryAllocation)
	{
	TBuf<28> params;
	params.Format(_L("uid=%08x alloc=%x"), iUid, aExtraMemoryAllocation);
	User::LeaveIfError(iProcess.Create(_L("t_oomdummyapp_0xE6CFBA00.exe"), params));
	User::LeaveIfError(iMsgQueue.CreateGlobal(KNullDesC, 4));
	User::LeaveIfError(iProcess.SetParameter(15, iMsgQueue));
	iProcess.Resume();
	}

void CCDummyApplicationHandle::SendMessage(TInt aMessage)
	{
	iMsgQueue.SendBlocking(aMessage);
	}

TBool CCDummyApplicationHandle::CompareTo(const TUid* aKey, const CCDummyApplicationHandle& aValue)
	{
	return aValue.iUid == *aKey;
	}

void CCDummyApplicationHandle::BringToForeground()
	{
	SendMessage(1);
	}

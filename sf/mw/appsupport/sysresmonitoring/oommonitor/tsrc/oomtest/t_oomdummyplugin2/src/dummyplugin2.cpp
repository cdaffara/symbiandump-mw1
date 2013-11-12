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




#include <e32property.h>
#include <e32cmn.h>
#include "dummyplugin2.h"
#include "debug.h"
#include "t_oomdummyplugin_properties.h"


CDummyOomPluginV2* CDummyOomPluginV2::NewL(TInt aInstance)
	{
	CDummyOomPluginV2* self = new(ELeave)CDummyOomPluginV2(aInstance);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self;
	}

CDummyOomPluginV2::CDummyOomPluginV2(TInt aInstance) :
iInstance(aInstance)
	{
	
	}

void CDummyOomPluginV2::ConstructL()
	{
	TRACE_FUNC_ENTRY;
	//connect to alloc server
	User::LeaveIfError(iAllocServer.Connect());
	TRACE_FUNC_EXIT;
	}

CDummyOomPluginV2::~CDummyOomPluginV2()
	{
	TRACE_FUNC;
	}

void CDummyOomPluginV2::FreeRam(TInt aBytesRequested)
	{
	TRACE_FUNC;
	iLowMemoryCallCount++;
	RProperty::Set(KUidOomPropertyCategory, iInstance + KOOMDummyPluginLowMemoryCount, iLowMemoryCallCount);
	TInt err = RProperty::Set(KUidOomPropertyCategory, iInstance + KOOMDummyPluginBytesRequested, aBytesRequested);
	iAllocServer.MemoryLow(TUid::Uid(iInstance));
	}

void CDummyOomPluginV2::MemoryGood()
	{
	TRACE_FUNC;
	iGoodMemoryCallCount++;
	RProperty::Set(KUidOomPropertyCategory, iInstance + KOOMDummyPluginGoodMemoryCount, iGoodMemoryCallCount);
	iAllocServer.MemoryGood(TUid::Uid(iInstance));
	}

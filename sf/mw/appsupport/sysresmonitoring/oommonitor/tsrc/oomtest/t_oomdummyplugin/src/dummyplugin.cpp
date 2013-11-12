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
#include "dummyplugin.h"
#include "debug.h"
#include "t_oomdummyplugin_properties.h"

CDummyOomPlugin* CDummyOomPlugin::NewL(TInt aInstance)
	{
	CDummyOomPlugin* self = new(ELeave)CDummyOomPlugin(aInstance);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self;
	}

CDummyOomPlugin::CDummyOomPlugin(TInt aInstance) :
iInstance(aInstance)
	{
	
	}

void CDummyOomPlugin::ConstructL()
	{
	TRACE_FUNC_ENTRY;
	//connect to alloc server
	User::LeaveIfError(iAllocServer.Connect());
	TRACE_FUNC_EXIT;
	}

CDummyOomPlugin::~CDummyOomPlugin()
	{
	TRACE_FUNC;
	}

void CDummyOomPlugin::FreeRam()
	{
	TRACE_FUNC;
	iLowMemoryCallCount++;
	RProperty::Set(KUidOomPropertyCategory, iInstance + KOOMDummyPluginLowMemoryCount, iLowMemoryCallCount);
	iAllocServer.MemoryLow(TUid::Uid(iInstance));
	}

void CDummyOomPlugin::MemoryGood()
	{
	TRACE_FUNC;
	iGoodMemoryCallCount++;
	RProperty::Set(KUidOomPropertyCategory, iInstance + KOOMDummyPluginGoodMemoryCount, iGoodMemoryCallCount);
	iAllocServer.MemoryGood(TUid::Uid(iInstance));
	}

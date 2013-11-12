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




#include <ecom\implementationproxy.h>
#include "dummyplugin2.h"

#define DECLARE_CREATE_INSTANCEV2(UID) CDummyOomPluginV2* CreateInstance##UID() { return CDummyOomPluginV2::NewL(UID); }
#define IMPLEMENTATION_INSTANCEV2(UID) IMPLEMENTATION_PROXY_ENTRY(UID, CreateInstance##UID)

//ECOM factory functions
DECLARE_CREATE_INSTANCEV2(0x10286A3C)
DECLARE_CREATE_INSTANCEV2(0x10286A3D)

// Define the private interface UIDs
const TImplementationProxy ImplementationTable[] =
	{
	IMPLEMENTATION_INSTANCEV2(0x10286A3C),
	IMPLEMENTATION_INSTANCEV2(0x10286A3D)
	};

EXPORT_C const TImplementationProxy* ImplementationGroupProxy(TInt& aTableCount)
	{
	aTableCount = sizeof(ImplementationTable) / sizeof(TImplementationProxy);

	return ImplementationTable;
	}

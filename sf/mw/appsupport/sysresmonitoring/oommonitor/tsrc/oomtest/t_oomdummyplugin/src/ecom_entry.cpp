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
#include "dummyplugin.h"

#define DECLARE_CREATE_INSTANCE(UID) CDummyOomPlugin* CreateInstance##UID() { return CDummyOomPlugin::NewL(UID); }
#define IMPLEMENTATION_INSTANCE(UID) IMPLEMENTATION_PROXY_ENTRY(UID, CreateInstance##UID)

// #define DECLARE_CREATE_INSTANCEV2(UID) CDummyOomPluginV2* CreateInstance##UID() { return CDummyOomPluginV2::NewL(UID); }
// #define IMPLEMENTATION_INSTANCEV2(UID) IMPLEMENTATION_PROXY_ENTRY(UID, CreateInstance##UID)

//ECOM factory functions
DECLARE_CREATE_INSTANCE(0x10286A34)
DECLARE_CREATE_INSTANCE(0x10286A35)
DECLARE_CREATE_INSTANCE(0x10286A36)
DECLARE_CREATE_INSTANCE(0x10286A37)
DECLARE_CREATE_INSTANCE(0x10286A38)
DECLARE_CREATE_INSTANCE(0x10286A39)
DECLARE_CREATE_INSTANCE(0x10286A3A)
DECLARE_CREATE_INSTANCE(0x10286A3B)
// DECLARE_CREATE_INSTANCEV2(0x10286A3C)
// DECLARE_CREATE_INSTANCEV2(0x10286A3D)

// Define the private interface UIDs
const TImplementationProxy ImplementationTable[] =
	{
	IMPLEMENTATION_INSTANCE(0x10286A34),
	IMPLEMENTATION_INSTANCE(0x10286A35),
	IMPLEMENTATION_INSTANCE(0x10286A36),
	IMPLEMENTATION_INSTANCE(0x10286A37),
	IMPLEMENTATION_INSTANCE(0x10286A38),
	IMPLEMENTATION_INSTANCE(0x10286A39),
	IMPLEMENTATION_INSTANCE(0x10286A3A),
	IMPLEMENTATION_INSTANCE(0x10286A3B)// ,
	// IMPLEMENTATION_INSTANCEV2(0x10286A3C),
	// IMPLEMENTATION_INSTANCEV2(0x10286A3D)
	};

EXPORT_C const TImplementationProxy* ImplementationGroupProxy(TInt& aTableCount)
	{
	aTableCount = sizeof(ImplementationTable) / sizeof(TImplementationProxy);

	return ImplementationTable;
	}

/*
* Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies).
* All rights reserved.
*
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU Lesser General Public License as published by
* the Free Software Foundation, version 2.1 of the License.
* 
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU Lesser General Public License for more details.
*
* You should have received a copy of the GNU Lesser General Public License
* along with this program.  If not, 
* see "http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html/".
*
*     This class export ECom interface implementations.
*
*/




#include <e32std.h>

#include <ecom/implementationproxy.h>



#include "BrowserRfsPlugin.h"



// Map the interface UIDs to implementation factory functions

const TImplementationProxy ImplementationTable[] =

    {

    	IMPLEMENTATION_PROXY_ENTRY(0x200169EA,	CBrowserRfsPlugin::NewL )

    };



// Exported proxy for instantiation method resolution

EXPORT_C const TImplementationProxy* ImplementationGroupProxy(TInt& aTableCount)

    {

    aTableCount = sizeof(ImplementationTable) / sizeof(TImplementationProxy);



    return ImplementationTable;

    }




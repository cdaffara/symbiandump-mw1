/*
* Copyright (c) 2007-2009 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  ECOM implementation table
 *
*/

#include <e32base.h>
#include <e32std.h>
#include <ecom/ecom.h>
#include <ecom/implementationproxy.h>
#include "hsappkeyhandleruids.hrh"
#include "hsappkeyplugin.h"

const TImplementationProxy AppkeyPluginImplementationTable[] =
    {
    IMPLEMENTATION_PROXY_ENTRY( KUIDAPPKEYPLUGINIMPL1, CHsAppKeyPlugin::NewL )
    };

// ---------------------------------------------------------------------------
//
// ---------------------------------------------------------------------------
//
EXPORT_C const TImplementationProxy* ImplementationGroupProxy( TInt& aTableCount )
    {
    aTableCount = sizeof( AppkeyPluginImplementationTable ) / sizeof( TImplementationProxy );

    return AppkeyPluginImplementationTable;
    }

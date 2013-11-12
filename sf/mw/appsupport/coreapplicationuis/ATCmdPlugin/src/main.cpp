
/*
 * Copyright (c) 2008-2009 Nokia Corporation and/or its subsidiary(-ies).
 * All rights reserved.
 * This component and the accompanying materials are made available
 * under the terms of "Eclipse Public License v1.0"
 * which accompanies this distribution, and is available
 * at the URL "http://www.eclipse.org/legal/epl-v10.html".
 * Initial Contributors:
 * Nokia Corporation - initial contribution.
 *
 * Contributors:
 * Description :
 *
 */



// INCLUDES
#include <e32base.h>
#include <ecom/implementationproxy.h>
#include "atcmdpluginuids.hrh"
#include "atcmdplugin.h"

// ============================= LOCAL FUNCTIONS  =============================

// ============================= MEMBER FUNCTIONS =============================

// ========================= OTHER EXPORTED FUNCTIONS =========================

// ----------------------------------------------------------------------------
// ImplementationTable
// Ecom plug-in implementation table
// Status : Approved
// ----------------------------------------------------------------------------
//
const TImplementationProxy ImplementationTable[] =
	{
	IMPLEMENTATION_PROXY_ENTRY( KUidATCmdEcomImpl1, CATCmdPluginEcom::NewL ),
	};

// ----------------------------------------------------------------------------
// ImplementationGroupProxy
// Instance of implementation proxy
// (exported)
// Status : Approved
// ----------------------------------------------------------------------------
//
EXPORT_C const TImplementationProxy* ImplementationGroupProxy( TInt& aTableCount )
	{
	aTableCount = sizeof( ImplementationTable ) / sizeof( TImplementationProxy );

	return ImplementationTable;
	}

//  End of File  

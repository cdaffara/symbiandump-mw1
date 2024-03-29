/*
* Copyright (c) 2002-2004 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:   This class contains the ECom specific implementations for
*                startuptask plugin.
*
*/


// INCLUDE FILES
#include <e32std.h>
#include <ecom/implementationproxy.h>

#include "StartupTaskAdd.h"
#include "StartupTaskRemove.h"
#include "SWInstUid.h"

using namespace SwiUI;

// CONSTANTS

const TImplementationProxy KImplementationTable[] =
    {
    IMPLEMENTATION_PROXY_ENTRY( KSWInstStartupAddImplUid, 
                                CStartupTaskAdd::NewL ),
    IMPLEMENTATION_PROXY_ENTRY( KSWInstStartupRemoveImplUid, 
                                CStartupTaskRemove::NewL )
    };

// ========================== OTHER EXPORTED FUNCTIONS =========================

// -----------------------------------------------------------------------------
// ImplementationGroupProxy
// Returns the filters implemented in this DLL
// Returns: The filters implemented in this DLL
// -----------------------------------------------------------------------------
//
EXPORT_C const TImplementationProxy* ImplementationGroupProxy( TInt& aTableCount )
    {
    aTableCount = sizeof( KImplementationTable ) / sizeof( TImplementationProxy );
    return KImplementationTable;
    }

//  End of File  

/*
* Copyright (c) 2002-2006 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Camcorder controller plugin DLL entry point and implementation table
*
*/



// INCLUDE FILES
#include "CamCImplementationUIDs.hrh"
#include "CamCController.h"

// CONSTANTS
// -----------------------------------------------------------------------------
// ImplementationTable
// Exported proxy for instantiation method resolution.
// -----------------------------------------------------------------------------
//
const TImplementationProxy ImplementationTable[] =
    {
    	IMPLEMENTATION_PROXY_ENTRY(KCamCUidControllerImplementation, CCamCController::NewL)
    };

// -----------------------------------------------------------------------------
// ImplementationGroupProxy 
// Returns implementation table and count to ECOM
// Returns: ImplementationTable: pointer to implementation table
// -----------------------------------------------------------------------------
//
EXPORT_C const TImplementationProxy* ImplementationGroupProxy(
    TInt& aTableCount)  // reference used to return implementation count
    {
    aTableCount = sizeof(ImplementationTable) / sizeof(TImplementationProxy);
    return ImplementationTable;
    }


//  End of File

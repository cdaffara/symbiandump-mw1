/*
* Copyright (c) 2006 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  GSServer is responsible for initializing the TV
*                driver during the boot and always when the cable is connected.
*
*/


// INCLUDE FILES
#include "GSServerClient.h"
#include <e32svr.h>

// EXTERNAL DATA STRUCTURES

// EXTERNAL FUNCTION PROTOTYPES  

// CONSTANTS

// MACROS

// LOCAL CONSTANTS AND MACROS

// MODULE DATA STRUCTURES

// LOCAL FUNCTION PROTOTYPES

// FORWARD DECLARATIONS


// ============================= LOCAL FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// RGSServerClient::RGSServerClient
// Empty default constructor.
// -----------------------------------------------------------------------------
//
EXPORT_C RGSServerClient::RGSServerClient()
    {
    }


// -----------------------------------------------------------------------------
// RGSServerClient::Connect
// Connect to the server, standard stuff.
// -----------------------------------------------------------------------------
//
EXPORT_C TInt RGSServerClient::Connect()
    {
    return KErrNone;
    }


// -----------------------------------------------------------------------------
// RGSServerClient::Close
// Close the connection to the server, just call base class.
// -----------------------------------------------------------------------------
//
EXPORT_C void RGSServerClient::Close()
    {
    RSessionBase::Close();
    }


//  End of File

/*
* Copyright (c) 2004 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  CSysApBtController stub implementation
 *
*/


// INCLUDES

#include "SysApBtControllerStub.h"

MSysApBtController* CreateSysApBtControllerL( CSysApAppUi& aSysApAppUi )
    {
    return CSysApBtController::NewL( aSysApAppUi );
    }

// ============================ MEMBER FUNCTIONS ==============================

// ----------------------------------------------------------------------------
// CSysApBtController::NewL() 
// ----------------------------------------------------------------------------
CSysApBtController* CSysApBtController::NewL( CSysApAppUi& )
    {
    CSysApBtController* self = new(ELeave) CSysApBtController;
    return self;
    }

// ----------------------------------------------------------------------------
// CSysApBtController::CSysApBtController() 
// ----------------------------------------------------------------------------
CSysApBtController::CSysApBtController()
    {

    }

// ----------------------------------------------------------------------------
// CSysApBtController::~CSysApBtController() 
// ----------------------------------------------------------------------------

CSysApBtController::~CSysApBtController()
    {

    }

// ----------------------------------------------------------------------------
// CSysApBtController::SetPowerState()
// ----------------------------------------------------------------------------
TInt CSysApBtController::SetPowerState( TBool /* aBtState */ )
    {
    return KErrNone;
    }

// End of File




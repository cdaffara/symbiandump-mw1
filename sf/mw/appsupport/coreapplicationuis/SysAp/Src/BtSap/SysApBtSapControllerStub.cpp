/*
* Copyright (c) 2005 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  CSysApBtSapController stub implementation
 *
*/


// INCLUDES

#include "SysApBtSapControllerStub.h"

MSysApBtSapController* CreateSysApBtSapControllerL( CSysApAppUi& aSysApAppUi )
    {
    return CSysApBtSapController::NewL( aSysApAppUi );
    }

// ============================ MEMBER FUNCTIONS ==============================

// ----------------------------------------------------------------------------
// CSysApBtSapController::NewL() 
// ----------------------------------------------------------------------------
CSysApBtSapController* CSysApBtSapController::NewL( CSysApAppUi& )
    {
    CSysApBtSapController* self = new(ELeave) CSysApBtSapController;
    return self;
    }

// ----------------------------------------------------------------------------
// CSysApBtSapController::CSysApBtSapController() 
// ----------------------------------------------------------------------------
CSysApBtSapController::CSysApBtSapController()
    {

    }

// ----------------------------------------------------------------------------
// CSysApBtSapController::~CSysApBtSapController() 
// ----------------------------------------------------------------------------

CSysApBtSapController::~CSysApBtSapController()
    {

    }

// ----------------------------------------------------------------------------
// CSysApBtSapController::BtSapEnabled()
// ----------------------------------------------------------------------------
TBool CSysApBtSapController::BtSapEnabled()
    {
    return EFalse;
    }

// ----------------------------------------------------------------------------
// CSysApBtSapController::SimApplicationsClosed()
// ----------------------------------------------------------------------------
void CSysApBtSapController::SimApplicationsClosed()
    {

    }

// ----------------------------------------------------------------------------
// CSysApBtSapController::Disconnect()
// ----------------------------------------------------------------------------
TInt CSysApBtSapController::Disconnect()
    {
    return KErrNone;
    }

// ----------------------------------------------------------------------------
// CSysApBtSapController::SwitchingToOffline()
// ----------------------------------------------------------------------------
void CSysApBtSapController::SwitchingToOffline()
    {

    }

// ----------------------------------------------------------------------------
// CSysApBtSapController::GetBtSapDeviceNameL()
// ----------------------------------------------------------------------------
HBufC* CSysApBtSapController::GetBtSapDeviceNameL()
    {
    return NULL;
    }

// End of File




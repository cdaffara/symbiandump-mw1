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
* Description:  CSysApOfflineModeController stub implementation
 *
*/


// INCLUDES

#include "SysApOfflineModeControllerStub.h"
class CSysApAppUi;

MSysApOfflineModeController* CreateSysApOfflineModeControllerL( CSysApAppUi& aSysApAppUi )
    {
    return CSysApOfflineModeController::NewL( aSysApAppUi );
    }

// ============================ MEMBER FUNCTIONS ==============================

// ----------------------------------------------------------------------------
// CSysApOfflineModeController::NewL() 
// ----------------------------------------------------------------------------
CSysApOfflineModeController* CSysApOfflineModeController::NewL( CSysApAppUi& )
    {
    CSysApOfflineModeController* self = new(ELeave) CSysApOfflineModeController;
    return self;
    }

// ----------------------------------------------------------------------------
// CSysApOfflineModeController::OfflineModeActive() 
// ----------------------------------------------------------------------------
TBool CSysApOfflineModeController::OfflineModeActive()
    {
    return EFalse;
    }

// ----------------------------------------------------------------------------
// CSysApOfflineModeController::CSysApOfflineModeController() 
// ----------------------------------------------------------------------------
CSysApOfflineModeController::CSysApOfflineModeController()
    {
    // Empty stub implementation
    }

// ----------------------------------------------------------------------------
// CSysApOfflineModeController::~CSysApOfflineModeController()
// ----------------------------------------------------------------------------

CSysApOfflineModeController::~CSysApOfflineModeController()
    {
    // Empty stub implementation
    }

// ----------------------------------------------------------------------------
// CSysApOfflineModeController::SwitchFromOnlineToOfflineModeL()
// ----------------------------------------------------------------------------

void CSysApOfflineModeController::SwitchFromOnlineToOfflineModeL()
    {
    // Empty stub implementation
    }

// ----------------------------------------------------------------------------
// CSysApOfflineModeController::SwitchFromOfflineToOnlineModeL()
// ----------------------------------------------------------------------------

void CSysApOfflineModeController::SwitchFromOfflineToOnlineModeL()
    {
    // Empty stub implementation
    }

// ----------------------------------------------------------------------------
// CSysApOfflineModeController::CheckOfflineModeInitialStatusesL()
// ----------------------------------------------------------------------------

void CSysApOfflineModeController::CheckOfflineModeInitialStatusesL()
    {
    // Empty stub implementation
    }

// ----------------------------------------------------------------------------
// CSysApOfflineModeController::DoNotActivateBt()
// ----------------------------------------------------------------------------
void CSysApOfflineModeController::DoNotActivateBt()
    {
    // Empty stub implementation
    }

// ----------------------------------------------------------------------------
// CSysApOfflineModeController::DoNotActivateRF()
// ----------------------------------------------------------------------------
void CSysApOfflineModeController::DoNotActivateRF()
    {
    // Empty stub implementation
    }

// ----------------------------------------------------------------------------
// CSysApOfflineModeController::MustBtBeActivated()
// ----------------------------------------------------------------------------
TBool CSysApOfflineModeController::MustBtBeActivated()
    {
    return EFalse;
    }

// ----------------------------------------------------------------------------
// CSysApOfflineModeController::GoOnlineIfOkL()
// ----------------------------------------------------------------------------
void CSysApOfflineModeController::GoOnlineIfOkL()
    {
    // Empty stub implementation
    }

// End of File




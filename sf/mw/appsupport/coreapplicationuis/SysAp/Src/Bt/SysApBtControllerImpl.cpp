/*
* Copyright (c) 2004-2007 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  CSysApBtController implementation
 *
*/


// INCLUDES

#include "SysApBtControllerImpl.h"
#include "SysApAppUi.h"

MSysApBtController* CreateSysApBtControllerL( CSysApAppUi& aSysApAppUi )
    {
    return CSysApBtController::NewL( aSysApAppUi );
    }

// ============================ MEMBER FUNCTIONS ==============================

// ----------------------------------------------------------------------------
// CSysApBtController::NewL() 
// ----------------------------------------------------------------------------
CSysApBtController* CSysApBtController::NewL( CSysApAppUi& aSysApAppUi )
    {
    TRACES( RDebug::Print( _L("CSysApBtController::NewL") ) );
    CSysApBtController* self = new(ELeave) CSysApBtController( aSysApAppUi );
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop();
    return self;
    }

// ----------------------------------------------------------------------------
// CSysApBtController::CSysApBtController() 
// ----------------------------------------------------------------------------
CSysApBtController::CSysApBtController( CSysApAppUi& aSysApAppUi ) :
  iSysApAppUi( aSysApAppUi )
    {
    }

// ----------------------------------------------------------------------------
// CSysApBtController::ConstructL() 
// ----------------------------------------------------------------------------
void CSysApBtController::ConstructL()
    {
    TRACES( RDebug::Print( _L("CSysApBtController::ConstructL") ) );

    iBTEngSettings = CBTEngSettings::NewL();
    }

// ----------------------------------------------------------------------------
// CSysApBtController::~CSysApBtController() 
// ----------------------------------------------------------------------------

CSysApBtController::~CSysApBtController()
    {
    TRACES( RDebug::Print( _L("CSysApBtController::~CSysApBtController") ) );

    delete iBTEngSettings;
    }

// ----------------------------------------------------------------------------
// CSysApBtController::SetPowerState()
// ----------------------------------------------------------------------------
TInt CSysApBtController::SetPowerState( TBool aBtState )
    {
    TRACES( RDebug::Print( _L("CSysApBtController::SetPowerState: aBtState=%d"), aBtState ) );

    TBTPowerStateValue powerState = aBtState ? EBTPowerOn : EBTPowerOff;
    
    return iBTEngSettings->SetPowerState( powerState );
    }

// End of File




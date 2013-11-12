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
* Description:  CSysApCenRepBtObserver implementation.
 *
*/


// INCLUDE FILES
#include "SysApCenRepBTObserver.h"
#include "SysApAppUi.h"

// ========================== MEMBER FUNCTIONS ================================

// ----------------------------------------------------------------------------
// CSysApCenRepBtObserver* CSysApCenRepBtObserver::NewL()
// ----------------------------------------------------------------------------

CSysApCenRepBtObserver* CSysApCenRepBtObserver::NewL( CSysApAppUi& aSysApAppUi )
    {       
    TRACES( RDebug::Print( _L("CSysApCenRepBtObserver::NewL" ) ) );
    CSysApCenRepBtObserver* self = new ( ELeave ) CSysApCenRepBtObserver( aSysApAppUi );
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop(); //self
    return self;
    }

// ----------------------------------------------------------------------------
// CSysApCenRepBtObserver::CSysApCenRepBtObserver( CSysApAppUi& aSysApAppUi )
// ----------------------------------------------------------------------------

CSysApCenRepBtObserver::CSysApCenRepBtObserver( CSysApAppUi& aSysApAppUi  )
    : iSysApAppUi( aSysApAppUi )
    {
    TRACES( RDebug::Print( _L("CSysApCenRepBtObserver::CSysApCenRepBtObserver" ) ) );
    }

// ----------------------------------------------------------------------------
// CSysApCenRepBtObserver::~CSysApCenRepBtObserver()
// ----------------------------------------------------------------------------

CSysApCenRepBtObserver::~CSysApCenRepBtObserver()
    {
    TRACES( RDebug::Print( _L("~CSysApCenRepBtObserver") ) );
    if ( iBTPowerStateHandler )
        {
        iBTPowerStateHandler->StopListening();
        }
    delete iBTPowerStateHandler;
    delete iSession;
    }

// ----------------------------------------------------------------------------
// CSysApCenRepBtObserver::ConstructL()
// ----------------------------------------------------------------------------

void CSysApCenRepBtObserver::ConstructL()
    {
    TRACES( RDebug::Print( _L("CSysApCenRepBtObserver::ConstructL: trying CRepository::NewL( KCRUidBluetoothPowerState )") ) );
    iSession = CRepository::NewL( KCRUidBluetoothPowerState );
    
    iBTPowerStateHandler = 
        CCenRepNotifyHandler::NewL( *this, 
                                    *iSession, 
                                    CCenRepNotifyHandler::EIntKey, 
                                    KBTPowerState );
    iBTPowerStateHandler->StartListeningL();
    }

// ----------------------------------------------------------------------------
// CSysApCenRepBtObserver::HandleNotifyInt()
// ----------------------------------------------------------------------------

void CSysApCenRepBtObserver::HandleNotifyInt( TUint32 aId, TInt aNewValue  )
    {
    TRACES( RDebug::Print( _L("CSysApCenRepBtObserver::HandleNotifyInt(), aId=0x%x, aNewValue=%d" ), aId, aNewValue ) );
    
    if ( aId == KBTPowerState )
        {
        if( aNewValue == KBluetoothModulePowerModeOn )
            {
            // Disable re-activating BT if in off-line mode
            if ( iSysApAppUi.OfflineModeActive() )
                {
                iSysApAppUi.DoNotActivateBt();
                }
            }
        }
    }

// ----------------------------------------------------------------------------
// CSysApCenRepBtObserver::HandleNotifyError()
// ----------------------------------------------------------------------------

void CSysApCenRepBtObserver::HandleNotifyError( TUint32 /* aId */, TInt /* error */, CCenRepNotifyHandler* /* aHandler */ )
    {
    TRACES( RDebug::Print( _L("CSysApCenRepBtObserver::HandleNotifyError()" ) ) );
    }

// ----------------------------------------------------------------------------
// CSysApCenRepBtObserver::BluetoothPowerMode()  
// ----------------------------------------------------------------------------

TInt CSysApCenRepBtObserver::BluetoothPowerMode() const
    {
    TInt btPowerState;
    
    if ( iSession->Get( KBTPowerState, btPowerState ) < 0 )
        {
        TRACES( RDebug::Print( _L("CSysApCenRepBtObserver::BluetoothPowerMode ERROR: btPowerState: %d"), btPowerState ) );
        btPowerState = EBTPowerOff;
        }
    return btPowerState;
    }
    

// End of File


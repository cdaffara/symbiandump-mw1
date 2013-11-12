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
* Description:  CSysApCenRepLogsObserver implementation.
 *
*/


// INCLUDE FILES
#include "SysApCenRepLogsObserver.h"
#include "SysApAppUi.h"

// ========================== MEMBER FUNCTIONS ================================

// ----------------------------------------------------------------------------
// CSysApCenRepLogsObserver* CSysApCenRepLogsObserver::NewL()
// ----------------------------------------------------------------------------

CSysApCenRepLogsObserver* CSysApCenRepLogsObserver::NewL( CSysApAppUi& aSysApAppUi )
    {       
    TRACES( RDebug::Print( _L("CSysApCenRepLogsObserver::NewL" ) ) );
    CSysApCenRepLogsObserver* self = new ( ELeave ) CSysApCenRepLogsObserver( aSysApAppUi );
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );
    return self;
    }

// ----------------------------------------------------------------------------
// CSysApCenRepLogsObserver::CSysApCenRepLogsObserver( CSysApAppUi& aSysApAppUi )
// ----------------------------------------------------------------------------

CSysApCenRepLogsObserver::CSysApCenRepLogsObserver( CSysApAppUi& aSysApAppUi  )
    : iSysApAppUi( aSysApAppUi ),
      iUiReady( EFalse ),
      iSimChangedCheckDone( EFalse )
    {
    TRACES( RDebug::Print( _L("CSysApCenRepLogsObserver::CSysApCenRepLogsObserver" ) ) );
    }

// ----------------------------------------------------------------------------
// CSysApCenRepLogsObserver::~CSysApCenRepLogsObserver()
// ----------------------------------------------------------------------------

CSysApCenRepLogsObserver::~CSysApCenRepLogsObserver()
    {
    TRACES( RDebug::Print( _L("~CSysApCenRepLogsObserver") ) );
    if ( iNewMissedCallsHandler )
        {
        iNewMissedCallsHandler->StopListening();
        }
    delete iNewMissedCallsHandler;
    delete iSession;
    }

// ----------------------------------------------------------------------------
// CSysApCenRepLogsObserver::ConstructL()
// ----------------------------------------------------------------------------

void CSysApCenRepLogsObserver::ConstructL()
    {
    const TUid KCRUidLogs = {0x101F874E};
    /**
    * Informs the Logs application about the amount of new missed calls.
    * Integer type
    **/
    const TUint32 KLogsNewMissedCalls = 0x00000006;
    TRAPD( err, iSession = CRepository::NewL( KCRUidLogs ) );
    TRACES( RDebug::Print( _L("CSysApCenRepLogsObserver::ConstructL: err=%d (KCRUidLogs)"), err ) );
    User::LeaveIfError( err );
    iNewMissedCallsHandler = 
        CCenRepNotifyHandler::NewL( *this, 
                                    *iSession, 
                                    CCenRepNotifyHandler::EIntKey, 
                                    KLogsNewMissedCalls );
                                    iNewMissedCallsHandler->StartListeningL();
    
    iSession->Get(KLogsNewMissedCalls, iMissedCallsValue);
    }

// ----------------------------------------------------------------------------
// CSysApCenRepLogsObserver::HandleNotifyInt()
// ----------------------------------------------------------------------------
void CSysApCenRepLogsObserver::HandleNotifyInt( TUint32 /* aId */, TInt aNewValue )
    {
    TRACES( RDebug::Print( _L("CSysApCenRepLogsObserver::HandleNotifyInt: aNewValue=%d"),  aNewValue ) );
    
    iMissedCallsValue = aNewValue;
    
    TRAPD( err, UpdateMissedCallsIndicatorL() );
        
    if ( err )
        {
        TRACES( RDebug::Print( _L("CSysApCenRepLogsObserver::HandleNotifyInt: err=%d" ), err ) );
        }
    }

// ----------------------------------------------------------------------------
// CSysApCenRepLogsObserver::HandleNotifyError()
// ----------------------------------------------------------------------------
void CSysApCenRepLogsObserver::HandleNotifyError( TUint32 /* aId */, TInt /* error */, CCenRepNotifyHandler* /* aHandler */ )
    {
    TRACES( RDebug::Print( _L("CSysApCenRepLogsObserver::HandleNotifyError()" ) ) );
    }
    
    
// ----------------------------------------------------------------------------
// CSysApCenRepLogsObserver::HandleUiReady()
// ----------------------------------------------------------------------------
void CSysApCenRepLogsObserver::HandleUiReadyL()
    {
    TRACES( RDebug::Print( _L("CSysApCenRepLogsObserver::HandleUiReady()" ) ) ); 
    
    if ( !iUiReady ) // handle only once during device startup
        {
        iUiReady = ETrue;
            
        UpdateMissedCallsIndicatorL();
        }
    }

// ----------------------------------------------------------------------------
// CSysApCenRepLogsObserver::HandleSimChangedCheckDone()
// ----------------------------------------------------------------------------
void CSysApCenRepLogsObserver::HandleSimChangedCheckDoneL()
    {
    TRACES( RDebug::Print( _L("CSysApCenRepLogsObserver::HandleSimChangedCheckDone()" ) ) );
    
    if ( !iSimChangedCheckDone ) // handle only once during device startup
        {
        iSimChangedCheckDone = ETrue;
            
        UpdateMissedCallsIndicatorL();
        }
    }

// ----------------------------------------------------------------------------
// CSysApCenRepLogsObserver::UpdateMissedCallsIndicatorL()
// ----------------------------------------------------------------------------
void CSysApCenRepLogsObserver::UpdateMissedCallsIndicatorL()
    {
    TRACES( RDebug::Print( _L("CSysApCenRepLogsObserver::UpdateMissedCallsIndicatorL(): iMissedCallsValue=%d, UI flags=%d" ),
                iMissedCallsValue, iSimChangedCheckDone && iUiReady ) );
    
    if ( iSimChangedCheckDone && iUiReady )
        {
//        TInt newState( iMissedCallsValue > 0 ? EAknIndicatorStateOn : EAknIndicatorStateOff );
        
//        iSysApAppUi.SetIndicatorStateL( EAknIndicatorMissedCalls, newState );
        }
    }

// End of File


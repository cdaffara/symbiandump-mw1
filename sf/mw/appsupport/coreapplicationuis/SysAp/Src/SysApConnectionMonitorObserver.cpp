/*
* Copyright (c) 2005-2007 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  CSysApConnectionMonitorObserver implementation.
*
*/


// INCLUDE FILES

#include "SysApConnectionMonitorObserver.h"
#include "SysAp.hrh"
#include "SysApAppUi.h"

// ========================= MEMBER FUNCTIONS ================================

// ----------------------------------------------------------------------------
// CSysApConnectionMonitorObserver::NewL()
// ----------------------------------------------------------------------------

CSysApConnectionMonitorObserver* CSysApConnectionMonitorObserver::NewL( CSysApAppUi& aSysApAppUi )
    {       
    TRACES( RDebug::Print( _L("CSysApConnectionMonitorObserver::NewL") ) );
    CSysApConnectionMonitorObserver* self = new ( ELeave ) CSysApConnectionMonitorObserver( aSysApAppUi );
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop(); //self
    return self;
    }

// ----------------------------------------------------------------------------
// CSysApConnectionMonitorObserver::CSysApConnectionMonitorObserver() 
// ----------------------------------------------------------------------------

CSysApConnectionMonitorObserver::CSysApConnectionMonitorObserver( CSysApAppUi& aSysApAppUi ) 
: 	CActive(EPriorityStandard),
    iSysApAppUi( aSysApAppUi ),
	iBearerValue( EBearerUnknown )
	{
	CActiveScheduler::Add(this);
    }

// ----------------------------------------------------------------------------
// CSysApConnectionMonitorObserver::CSysApConnectionMonitorObserver( )
// ----------------------------------------------------------------------------

CSysApConnectionMonitorObserver::~CSysApConnectionMonitorObserver()
    {
    TRACES( RDebug::Print( _L("CSysApConnectionMonitorObserver::~CSysApConnectionMonitorObserver") ) );
    
    iConnMon.CancelNotifications();

    Cancel();

    iConnMon.Close();
    }

// ----------------------------------------------------------------------------
// CSysApConnectionMonitorObserver::ConstructL()
// ----------------------------------------------------------------------------

void CSysApConnectionMonitorObserver::ConstructL( )
    {
    TRACES( RDebug::Print( _L("CSysApConnectionMonitorObserver::ConstructL") ) );
    User::LeaveIfError( iConnMon.ConnectL() );
    
    // Get initial bearer
    TRACES( RDebug::Print(_L("CSysApConnectionMonitorObserver::ConstructL - Getting initial bearer value" ) ) );
    iConnMon.GetIntAttribute( EBearerIdGPRS, 0, KBearer, iBearerValue, iStatus );
    SetActive();
    
    iConnMon.NotifyEventL( *this );
    }


// -----------------------------------------------------------------------------
// CSysApConnectionMonitorObserver::RunL
// Handle initial bearer value get
// -----------------------------------------------------------------------------
//
void CSysApConnectionMonitorObserver::RunL()
    {
    TRACES( RDebug::Print(_L("CSysApConnectionMonitorObserver::RunL - Initial bearer value received, status: %d, value: %d" ), iStatus.Int(), iBearerValue ) );
    }

// -----------------------------------------------------------------------------
// CSysApConnectionMonitorObserver::DoCancel
// Handle cancel order on this active object.
// -----------------------------------------------------------------------------
//
void CSysApConnectionMonitorObserver::DoCancel()
    {
    if ( IsActive() )
        {
        iConnMon.CancelAsyncRequest(EConnMonGetIntAttribute);
        }
    }

// ----------------------------------------------------------------------------
// CSysApConnectionMonitorObserver::EventL()
// ----------------------------------------------------------------------------
//
void CSysApConnectionMonitorObserver::EventL( const CConnMonEventBase &aConnMonEvent )
    {
    switch ( aConnMonEvent.EventType() )
		{
		case EConnMonBearerChange:
            {
            CConnMonBearerChange* eventBearerChange;
			eventBearerChange = ( CConnMonBearerChange* ) &aConnMonEvent;
			iBearerValue = eventBearerChange->Bearer();
			TRACES( RDebug::Print( _L("CSysApConnectionMonitorObserver::EventL: iBearerValue=%d"), iBearerValue ) );
            iSysApAppUi.SetSignalIndicatorL();
		    }
            break;
        
 		default:
			break;
		}; 

    }
    
// ----------------------------------------------------------------------------
// CSysApConnectionMonitorObserver::GetBearerValue()
// ----------------------------------------------------------------------------

TInt CSysApConnectionMonitorObserver::GetBearerValue()
    {
    return iBearerValue;
    }    
    

// End of File






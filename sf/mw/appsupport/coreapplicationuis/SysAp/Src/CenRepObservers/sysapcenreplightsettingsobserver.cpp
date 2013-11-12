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
* Description:  A Central Repository observer for getting notifications on light
*                settings changes.
*
*/


#include <e32cmn.h>
#include <hwrmlightdomaincrkeys.h>
#include "sysapcenreplightsettingsobserver.h"
#include "SysAp.hrh"
#include "SysApAppUi.h"

// ======== MEMBER FUNCTIONS ========

// ---------------------------------------------------------------------------
// CSysApCenRepLightSettingsObserver::CSysApCenRepLightSettingsObserver
// ---------------------------------------------------------------------------
//
CSysApCenRepLightSettingsObserver::CSysApCenRepLightSettingsObserver( CSysApAppUi& aSysApAppUi )
    : iSysApAppUi( aSysApAppUi ),
      iSession( NULL ),
      iDisplayLightsTimeoutHandler( NULL )
    {
    TRACES( RDebug::Print( _L("CSysApCenRepLightSettingsObserver::CSysApCenRepLightSettingsObserver()" ) ) );
    }

// ---------------------------------------------------------------------------
// CSysApCenRepLightSettingsObserver::ConstructL
// ---------------------------------------------------------------------------
//
void CSysApCenRepLightSettingsObserver::ConstructL()
    {
    TRACES( RDebug::Print( _L("CSysApCenRepLightSettingsObserver::ConstructL()" ) ) );
    
    iSession = CRepository::NewL( KCRUidLightSettings );

    iDisplayLightsTimeoutHandler = CCenRepNotifyHandler::NewL( *this, 
                                                               *iSession, 
                                                               CCenRepNotifyHandler::EIntKey, 
                                                               KDisplayLightsTimeout );
    iDisplayLightsTimeoutHandler->StartListeningL();
    }

// ---------------------------------------------------------------------------
// CSysApCenRepLightSettingsObserver::NewL
// ---------------------------------------------------------------------------
//
CSysApCenRepLightSettingsObserver* CSysApCenRepLightSettingsObserver::NewL( CSysApAppUi& aSysApAppUi )
    {
    TRACES( RDebug::Print( _L("CSysApCenRepLightSettingsObserver::NewL()" ) ) );
    CSysApCenRepLightSettingsObserver* self = new( ELeave ) CSysApCenRepLightSettingsObserver( aSysApAppUi);
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );
    return self;
    }

// ---------------------------------------------------------------------------
// CSysApCenRepLightSettingsObserver::~CSysApCenRepLightSettingsObserver
// ---------------------------------------------------------------------------
//
CSysApCenRepLightSettingsObserver::~CSysApCenRepLightSettingsObserver()
    {
    TRACES( RDebug::Print( _L("CSysApCenRepLightSettingsObserver::~CSysApCenRepLightSettingsObserver()" ) ) );
    if ( iDisplayLightsTimeoutHandler )
        { 
        iDisplayLightsTimeoutHandler->StopListening();
        }
    delete iDisplayLightsTimeoutHandler;
    delete iSession;
    }

// ---------------------------------------------------------------------------
// CSysApCenRepLightSettingsObserver::GetLightsTimeout
// ---------------------------------------------------------------------------
//
TInt CSysApCenRepLightSettingsObserver::GetLightsTimeout() const
    {
    TRACES( RDebug::Print( _L("CSysApCenRepLightSettingsObserver::GetLightsTimeout()" ) ) );
    
    TInt timeout( 0 );
    
    TInt err = iSession->Get( KDisplayLightsTimeout, timeout );

    if ( err )
        {
        TRACES( RDebug::Print( _L("CSysApCenRepLightSettingsObserver::GetLightsTimeout: err=%d" ), err ) );
        timeout = KDefaultLightsTimeout;
        }
        
    return timeout;
    }

// ---------------------------------------------------------------------------
// From class MCenRepNotifyHandlerCallback.
// CSysApCenRepLightSettingsObserver::HandleNotifyInt
// ---------------------------------------------------------------------------
//
void CSysApCenRepLightSettingsObserver::HandleNotifyInt( TUint32 aId, TInt aNewValue )
    {
    TRACES( RDebug::Print( _L("CSysApCenRepLightSettingsObserver::HandleNotifyInt( aId=%d, aNewValue=%d"), aId, aNewValue ) );
    switch ( aId )
        {
        case KDisplayLightsTimeout:
            TRAPD( err, iSysApAppUi.DoLightsTimeoutChangedL( aNewValue ) );
            if ( err )
                {
                TRACES( RDebug::Print( _L("CSysApCenRepLightSettingsObserver::HandleNotifyInt: err=%d" ), err ) );
                }
            break;
        default:
            TRACES( RDebug::Print( _L("CSysApCenRepLightSettingsObserver::HandleNotifyInt: unknown id" ) ) );
            break;
        }
    }
  
// ---------------------------------------------------------------------------
// From class MCenRepNotifyHandlerCallback.
// CSysApCenRepLightSettingsObserver::HandleNotifyError
// ---------------------------------------------------------------------------
//
void CSysApCenRepLightSettingsObserver::HandleNotifyError( TUint32 /*aId*/, TInt /*error*/, CCenRepNotifyHandler* /*aHandler*/ )
    {
    TRACES( RDebug::Print( _L("CSysApCenRepLightSettingsObserver::HandleNotifyError()" ) ) );
    }

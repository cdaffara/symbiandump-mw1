/*
* Copyright (c) 2007 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Observes changes in audio output routing.
*
*/


#include "sysapaudioroutingobserver.h"
#include "SysAp.hrh"
#include "SysApAppUi.h"

// ---------------------------------------------------------------------------
// CSysApAudioRoutingObserver
// ---------------------------------------------------------------------------
//
CSysApAudioRoutingObserver::CSysApAudioRoutingObserver( CSysApAppUi& aSysApAppUi ) :  
                                                            iTelephonyAudioRouting( NULL ),
                                                            iSysApAppUi( aSysApAppUi )
    {
    }


// ---------------------------------------------------------------------------
// ConstructL
// ---------------------------------------------------------------------------
//
void CSysApAudioRoutingObserver::ConstructL()
    {
    TRACES( RDebug::Print( _L("CSysApAudioRoutingObserver::ConstructL") ) );
    iTelephonyAudioRouting = CTelephonyAudioRouting::NewL( *this );
    }


// ---------------------------------------------------------------------------
// NewL
// ---------------------------------------------------------------------------
//
CSysApAudioRoutingObserver* CSysApAudioRoutingObserver::NewL( CSysApAppUi& aSysApAppUi )
    {
    TRACES( RDebug::Print( _L("CSysApAudioRoutingObserver::NewL") ) );
    CSysApAudioRoutingObserver* self = new( ELeave ) CSysApAudioRoutingObserver( aSysApAppUi );
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );
    return self;
    }

// ---------------------------------------------------------------------------
// ~CSysApAudioRoutingObserver
// ---------------------------------------------------------------------------
//
CSysApAudioRoutingObserver::~CSysApAudioRoutingObserver()
    {
    TRACES( RDebug::Print( _L("CSysApAudioRoutingObserver::~CSysApAudioRoutingObserver") ) );
    delete iTelephonyAudioRouting;
    }

// ---------------------------------------------------------------------------
// IhfEnabled
// ---------------------------------------------------------------------------
//
TBool CSysApAudioRoutingObserver::IhfEnabled() const
    {
    CTelephonyAudioRouting::TAudioOutput audioOutput = iTelephonyAudioRouting->Output();
    
    TRACES( RDebug::Print( _L("CSysApAudioRoutingObserver::IhfEnabled: audioOutput=%d"), audioOutput ) );
    
    return ( audioOutput == CTelephonyAudioRouting::ELoudspeaker );
    
    }

// ---------------------------------------------------------------------------
// From class MTelephonyAudioRoutingObserver.
// AvailableOutputsChanged
// ---------------------------------------------------------------------------
//    
void CSysApAudioRoutingObserver::AvailableOutputsChanged( CTelephonyAudioRouting& /*aTelephonyAudioRouting*/ )
    {
    TRACES( RDebug::Print( _L("CSysApAudioRoutingObserver::AvailableOutputsChanged") ) );
    }

// ---------------------------------------------------------------------------
// From class MTelephonyAudioRoutingObserver.
// OutputChanged
// ---------------------------------------------------------------------------
//
void CSysApAudioRoutingObserver::OutputChanged( CTelephonyAudioRouting& /*aTelephonyAudioRouting*/ )
    {
    TRACES( RDebug::Print( _L("CSysApAudioRoutingObserver::OutputChanged") ) );
    TRAPD( err, 
           iSysApAppUi.SetIhfIndicatorL();
           iSysApAppUi.SetHacIndicatorL();
         );
    
    if ( err != KErrNone )
        {
        TRACES( RDebug::Print( _L("CSysApAudioRoutingObserver::OutputChanged: err=%d"), err ) );
        }
    }

// ---------------------------------------------------------------------------
// From class MTelephonyAudioRoutingObserver.
// SetOutputComplete
// ---------------------------------------------------------------------------
//  
void CSysApAudioRoutingObserver::SetOutputComplete( CTelephonyAudioRouting& /*aTelephonyAudioRouting*/, TInt /*aError*/ )
    {
    TRACES( RDebug::Print( _L("CSysApAudioRoutingObserver::SetOutputComplete") ) );
    }

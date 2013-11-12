/*
* Copyright (c) 2009-2010 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Implementation of CSysApSatObserver.
*
*/



// INCLUDE FILES
#include <rsatrefresh.h>
#include <rsatsession.h>
#include <e32debug.h>
#include "SysApSatObserver.h"
#include "SysAp.hrh"

// Constants

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CSysApSatObserver::CSysApSatObserver
// -----------------------------------------------------------------------------
//
CSysApSatObserver::CSysApSatObserver( MSysApSatObserver& aObserver ): 
    iObserver ( aObserver )
    {
    }

// -----------------------------------------------------------------------------
// CSysApSatObserver::ConstructL
// -----------------------------------------------------------------------------
//
void CSysApSatObserver::ConstructL( TInt aFile )
    {
    TRACES( RDebug::Print( _L( "CSysApSatObserver::ConstructL" ) ) );
    
    iSatSession = new ( ELeave ) RSatSession;
    iSatSession->ConnectL();
    iSatRefresh = new ( ELeave ) RSatRefresh( *this );
    iSatRefresh->OpenL( *iSatSession );     
    
    TSatRefreshFiles file;
    file.Append( aFile );
    iSatRefresh->NotifyFileChangeL( file );
    }

// -----------------------------------------------------------------------------
// CSysApSatObserver::NewL
// -----------------------------------------------------------------------------
//
CSysApSatObserver* CSysApSatObserver::NewL( MSysApSatObserver& aObserver, TInt aFile )
    {
    TRACES( RDebug::Print( _L( "CSysApSatObserver::NewL" ) ) );
    
    CSysApSatObserver* self = new (ELeave) CSysApSatObserver( aObserver );
    CleanupStack::PushL( self );
    self->ConstructL( aFile );
    CleanupStack::Pop( self );
    return self;
    }

// -----------------------------------------------------------------------------
// Destructor
// -----------------------------------------------------------------------------
//
CSysApSatObserver::~CSysApSatObserver()
    {
    TRACES( RDebug::Print( _L( "CSysApSatObserver::~CSysApSatObserver" ) ) );
    
    if ( iSatRefresh )
        {
        iSatRefresh->Cancel();  
        iSatRefresh->Close();
        }
    delete iSatRefresh;
    
    if ( iSatSession )
        {
        iSatSession->Close();
        }
    delete iSatSession;
    }

// -----------------------------------------------------------------------------
// CSysApSatObserver::AllowRefresh
// -----------------------------------------------------------------------------
//
TBool CSysApSatObserver::AllowRefresh( TSatRefreshType /*aType*/, 
    const TSatRefreshFiles& /*aFiles*/ )
    {
    // Refresh is always allowed
    TRACES( RDebug::Print( _L( "CSysApSatObserver::AllowRefresh" ) ) );

    return ETrue;
    }
 
// -----------------------------------------------------------------------------
// CSysApSatObserver::Refresh
// -----------------------------------------------------------------------------
//     
void CSysApSatObserver::Refresh( TSatRefreshType /*aType*/, const TSatRefreshFiles& /*aFiles*/ )
    {
    TRACES( RDebug::Print( _L( "CSysApSatObserver::Refresh" ) ) );
    
    TRAP_IGNORE( iObserver.SimFileChangedL() );
    
    // Notify SAT Server that refresh initiated file read is done
    iSatRefresh->RefreshEFRead( EFalse );
    }

//  End of File  

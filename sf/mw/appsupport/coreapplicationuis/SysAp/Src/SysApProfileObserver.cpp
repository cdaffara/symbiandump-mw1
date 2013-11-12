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
* Description:  CSysApProfileObserver implementation.
*
*/


// INCLUDE FILES
#include "SysApProfileObserver.h"
#include "SysApAppUi.h"

// ========================== MEMBER FUNCTIONS ================================

// ----------------------------------------------------------------------------
// CSysApProfileObserver* CSysApProfileObserver::NewL()
// ----------------------------------------------------------------------------

CSysApProfileObserver* CSysApProfileObserver::NewL( CSysApAppUi& aSysApAppUi )
    {       
    TRACES( RDebug::Print( _L("CSysApProfileObserver::NewL" ) ) );
    CSysApProfileObserver* self = new ( ELeave ) CSysApProfileObserver( aSysApAppUi );
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop(); //self
    return self;
    }

// ----------------------------------------------------------------------------
// CSysApProfileObserver::CSysApProfileObserver( CSysApAppUi& aSysApAppUi )
// ----------------------------------------------------------------------------

CSysApProfileObserver::CSysApProfileObserver( CSysApAppUi& aSysApAppUi  )
    : iSysApAppUi( aSysApAppUi )
    {
    TRACES( RDebug::Print( _L("CSysApProfileObserver::CSysApProfileObserver" ) ) );
    }

// ----------------------------------------------------------------------------
// CSysApProfileObserver::~CSysApProfileObserver()
// ----------------------------------------------------------------------------

CSysApProfileObserver::~CSysApProfileObserver()
    {
    TRACES( RDebug::Print( _L("~CSysApProfileObserver") ) );
    delete iHandler;
    }

// ----------------------------------------------------------------------------
// CSysApProfileObserver::ConstructL()
// ----------------------------------------------------------------------------

void CSysApProfileObserver::ConstructL()
    {
    TRACES( RDebug::Print( _L("CSysApProfileObserver::ConstructL()") ) );
    iHandler = CProfileChangeNotifyHandler::NewL( this );
    }

// ----------------------------------------------------------------------------
// CSysApProfileObserver::HandleActiveProfileEventL()
// ----------------------------------------------------------------------------
void CSysApProfileObserver::HandleActiveProfileEventL( TProfileEvent aProfileEvent, TInt aProfileId )
    {
    TRACES( RDebug::Print( _L("CSysApProfileObserver::HandleActiveProfileEventL(): aProfileId=%d"), aProfileId ) );
    if ( aProfileEvent == EProfileNewActiveProfile )
        {
        iSysApAppUi.HandleProfileChangedL( aProfileId );
        }
    else if ( aProfileEvent == EProfileActiveProfileModified )
        {
        iSysApAppUi.CheckSilentModeL();
        }
    }

// End of File


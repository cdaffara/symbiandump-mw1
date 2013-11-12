/*
* Copyright (c) 2002-2007 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  CSysApPowerKeyMenuObserver implementation.
*
*/


// INCLUDES
#include "SysApPowerKeyMenuObserver.h"
#include "SysApAppUi.h"
#include <avkondomainpskeys.h> // KAknPowerMenuStatus

// ============================ MEMBER FUNCTIONS ==============================

// ----------------------------------------------------------------------------
// CSysApPowerKeyMenuObserver::CSysApPowerKeyMenuObserver( CSysApAppUi& aSysApAppUi )
// ----------------------------------------------------------------------------

CSysApPowerKeyMenuObserver::CSysApPowerKeyMenuObserver( CSysApAppUi& aSysApAppUi ) 
    : CActive( EPriorityStandard ), 
    iSysApAppUi( aSysApAppUi )
    {
    CActiveScheduler::Add(this);
    }

// ----------------------------------------------------------------------------
// CSysApPowerKeyMenuObserver::~CSysApPowerKeyMenuObserver() 
// ----------------------------------------------------------------------------

CSysApPowerKeyMenuObserver::~CSysApPowerKeyMenuObserver() 
    {
    Cancel() ;
    }


// ----------------------------------------------------------------------------
// CSysApPowerKeyMenuObserver::DoCancel() 
// ----------------------------------------------------------------------------

void CSysApPowerKeyMenuObserver::DoCancel() 
    {
    RProperty::Set( KPSUidAvkonDomain, KAknPowerMenuStatus, EPowerMenuNotOpen );
    }

// ----------------------------------------------------------------------------
// CSysApPowerKeyMenuObserver::Start()
// ----------------------------------------------------------------------------

void CSysApPowerKeyMenuObserver::Start()
    {
    if( !IsActive() )
        {
        RProperty::Set( KPSUidAvkonDomain, KAknPowerMenuStatus, EPowerMenuOpen ); 
        SetActive();
        }
    }

// ----------------------------------------------------------------------------
// CSysApPowerKeyMenuObserver::RunL() 
// ----------------------------------------------------------------------------

void CSysApPowerKeyMenuObserver::RunL() 
    {
    TRACES( RDebug::Print( _L( "CSysApPowerKeyMenuObserver::RunL: started, %d selected" ), iStatus.Int() ) );
    RProperty::Set( KPSUidAvkonDomain, KAknPowerMenuStatus, EPowerMenuNotOpen );
    iSysApAppUi.PowerKeyPopUpMenuSelectionDoneL( iStatus.Int() );
    TRACES( RDebug::Print( _L( "CSysApPowerKeyMenuObserver::RunL: completed" ) ) );
    }

// ----------------------------------------------------------------------------
// CSysApPowerKeyMenuObserver::RunError( TInt aError )
// ----------------------------------------------------------------------------

TInt CSysApPowerKeyMenuObserver::RunError( TInt aError )
    {
    TRACES( RDebug::Print( _L( "CSysApPowerKeyMenuObserver::RunError: %d started" ), aError ) );
    if( aError == KErrNoMemory || aError == KErrDiskFull )
        {
        return aError;            
        }
    else 
        {
        return KErrNone;
        }
    }

// End of File

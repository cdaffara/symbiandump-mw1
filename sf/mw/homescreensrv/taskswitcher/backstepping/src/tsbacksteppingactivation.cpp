/*
* Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:
*
*/

#include <startupdomainpskeys.h>
#include <flogger.h>
#include <w32std.h>

#include "tsbacksteppingactivation.h"
#include "tsbackstepping.h"

 
// -----------------------------------------------------------------------------
// CBacksteppingActivation::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
EXPORT_C CTsBacksteppingActivation* CTsBacksteppingActivation::NewL( 
                                            MTsWindowGroupsMonitor &aMonitor )
{
    CTsBacksteppingActivation* self = 
        new( ELeave ) CTsBacksteppingActivation( aMonitor );
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );
    return self;
}

// -----------------------------------------------------------------------------
// CTsBacksteppingActivation::CTsBacksteppingActivation
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
CTsBacksteppingActivation::CTsBacksteppingActivation( 
                                            MTsWindowGroupsMonitor &aMonitor ) 
:
    CActive( EPriorityLow ),
    iMonitor( aMonitor )
    {
    CActiveScheduler::Add( this );
    }

// -----------------------------------------------------------------------------
// CTsBacksteppingActivation::ConstructL()
// -----------------------------------------------------------------------------
//
void CTsBacksteppingActivation::ConstructL() 
    {
    User::LeaveIfError( iProperty.Attach( KPSUidStartup, KPSIdlePhase1Ok ) );
    iStatus = KRequestPending;
    SetActive();
    TRequestStatus* status( &iStatus );
    User::RequestComplete( status, KErrNone );
    }

// -----------------------------------------------------------------------------
// CTsBacksteppingActivation::~CTsBacksteppingActivation
// Destructor
// -----------------------------------------------------------------------------
//
EXPORT_C CTsBacksteppingActivation::~CTsBacksteppingActivation()
    {
    Cancel();
    delete iEngine;
    iProperty.Close();
    }

// -----------------------------------------------------------------------------
// CBacksteppingActivation::DoCancel
// 
// -----------------------------------------------------------------------------
//
void CTsBacksteppingActivation::DoCancel()
    {
    if( IsActive() && iProperty.Handle() ) 
        {
        iProperty.Cancel();
        }
    }

// -----------------------------------------------------------------------------
/**
 * @see CActive.
 */
void CTsBacksteppingActivation::RunL()
{
    User::LeaveIfError( iStatus.Int() );
    TInt state( ~EIdlePhase1Ok );
    User::LeaveIfError( iProperty.Get( state ) );
    if( EIdlePhase1Ok == state )
        {
        if( 0 == iEngine )
            {
            iEngine = CTsBackstepping::NewL( iMonitor );
            }
        } 
    else
        {
        delete iEngine;
        iEngine = 0;
        }
    Subscribe();
    }

// -----------------------------------------------------------------------------
/**
 * @see CActive::RunError(TInt)
 */
TInt CTsBacksteppingActivation::RunError( TInt aError )
    {
    if (KErrCancel != aError) 
        {
        Subscribe();
        }
    return KErrNone;
    }

// -----------------------------------------------------------------------------
void CTsBacksteppingActivation::Subscribe()
    {
    iProperty.Subscribe( iStatus );
    SetActive();
    }
// end of file

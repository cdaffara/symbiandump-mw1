/*
* Copyright (c) 2002-2004 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  This singleton class takes care of creation of different other
*                classes needed in PECProfiles.
*
*/



// INCLUDE FILES
#include    "MT_CSchedulerUtility.h"
#include    <e32svr.h>

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// MT_CSchedulerUtility::MT_CSchedulerUtility
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
MT_CSchedulerUtility::MT_CSchedulerUtility()
    {
    RDebug::Print( _L( "Digia:MT_CSchedulerUtility::MT_CSchedulerUtility()" ) );
    }

// -----------------------------------------------------------------------------
// MT_CSchedulerUtility::ConstructL
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void MT_CSchedulerUtility::ConstructL()
    {
    iWait = new ( ELeave ) CActiveSchedulerWait;
    }

// -----------------------------------------------------------------------------
// MT_CSchedulerUtility::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
MT_CSchedulerUtility* MT_CSchedulerUtility::NewL()
    {
    MT_CSchedulerUtility* self = new( ELeave ) MT_CSchedulerUtility;
    
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop();

    return self;
    }

    
// Destructor
MT_CSchedulerUtility::~MT_CSchedulerUtility()
    {
    Stop();
    delete iWait;
    Dll::SetTls( NULL );
    RDebug::Print( _L( "Digia:MT_CSchedulerUtility::~MT_CSchedulerUtility()" ) );
    }


// -----------------------------------------------------------------------------
// MT_CSchedulerUtility::InstanceL
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
MT_CSchedulerUtility* MT_CSchedulerUtility::InstanceL()
    {
    TAny* instance = Dll::Tls();
    if( instance == NULL )
        {
        instance = MT_CSchedulerUtility::NewL();
        TInt err( Dll::SetTls( instance ) );
        if( err )
            {
            delete static_cast<MT_CSchedulerUtility*>( instance );
            User::Leave( err );
            }
        }
    return static_cast<MT_CSchedulerUtility*>( instance );
    }

void MT_CSchedulerUtility::Release()
    {
    TAny* instance = Dll::Tls();
    if( instance != NULL )
        {
        delete ( static_cast<MT_CSchedulerUtility*>( instance ) );
        }
    }

// -----------------------------------------------------------------------------
// MT_CSchedulerUtility::Start
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void MT_CSchedulerUtility::Start()
    {
    if( !iWait->IsStarted() )
        {
        iWait->Start();
        }
    }

// -----------------------------------------------------------------------------
// MT_CSchedulerUtility::Stop
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void MT_CSchedulerUtility::Stop()
    {
    if( iWait->IsStarted() )
        {
        iWait->AsyncStop();
        }
    }

//  End of File  


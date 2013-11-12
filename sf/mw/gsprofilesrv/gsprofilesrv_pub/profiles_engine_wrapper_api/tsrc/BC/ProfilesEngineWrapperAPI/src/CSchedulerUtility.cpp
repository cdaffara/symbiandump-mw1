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
* Description:        
*
*/









// INCLUDE FILES
#include    "CSchedulerUtility.h"
#include    <e32svr.h>

// ============================ LOCAL DEFINITIONS ================= =============
//static CSchedulerUtility* iInstance;
//CSchedulerUtility* CSchedulerUtility::iInstance;
// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CSchedulerUtility::CSchedulerUtility
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
CSchedulerUtility::CSchedulerUtility()
    {
    RDebug::Print( _L( "Digia:CSchedulerUtility::CSchedulerUtility()" ) );
    }

// -----------------------------------------------------------------------------
// CSchedulerUtility::ConstructL
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CSchedulerUtility::ConstructL()
    {
    iWait = new ( ELeave ) CActiveSchedulerWait;
    }

// -----------------------------------------------------------------------------
// CSchedulerUtility::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CSchedulerUtility* CSchedulerUtility::NewL()
    {
    CSchedulerUtility* self = new( ELeave ) CSchedulerUtility;
    
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop();

    return self;
    }

    
// Destructor
CSchedulerUtility::~CSchedulerUtility()
    {
    Stop();
    delete iWait;
    RDebug::Print( _L( "Digia:CSchedulerUtility::~CSchedulerUtility()" ) );
    }


// -----------------------------------------------------------------------------
// CSchedulerUtility::InstanceL
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
/*CSchedulerUtility* CSchedulerUtility::InstanceL()
    {
    if( iInstance == NULL )
        {
        iInstance = CSchedulerUtility::NewL();
        }
    return iInstance;
    }
*/
/*void CSchedulerUtility::Release()
    {
    delete iInstance;
    iInstance = NULL;
    }
*/
// -----------------------------------------------------------------------------
// CSchedulerUtility::Start
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CSchedulerUtility::Start()
    {
    if( !iWait->IsStarted() )
        {
        iWait->Start();
        }
    }

// -----------------------------------------------------------------------------
// CSchedulerUtility::Stop
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CSchedulerUtility::Stop()
    {
    if( iWait->IsStarted() )
        {
        iWait->AsyncStop();
        }
    }

//  End of File  


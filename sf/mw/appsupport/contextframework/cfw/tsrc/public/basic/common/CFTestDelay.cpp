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
* Description:  ?Description
*
*/


#include "CFTestDelay.h"

namespace
    {
    _LIT( KPanicCat, "TestDelay" );

    enum TPanicReason
        {
        EAlreadyStarted
        };

    LOCAL_C void Panic( TInt aCode )
        {
        User::Panic( KPanicCat, aCode );
        }
    }

CCFTestDelay* CCFTestDelay::NewL()
    {
    CCFTestDelay* self =
        CCFTestDelay::NewLC();
    CleanupStack::Pop();

    return self;
    }

CCFTestDelay* CCFTestDelay::NewLC()
   {
    CCFTestDelay* self =
        new( ELeave ) CCFTestDelay;
    CleanupStack::PushL( self );
    self->ConstructL();

    return self;
    }

CCFTestDelay::CCFTestDelay():
    CTimer( EPriorityStandard )
    {
    // Double check if base class adds active object into scheduler
    if( !IsAdded() )
        {
        CActiveScheduler::Add( this );
        }
    }

void CCFTestDelay::ConstructL()
    {
    // Do base constructions
    CTimer::ConstructL();

    // Initialize active scheduler wait
    iWait = new( ELeave ) CActiveSchedulerWait;
    }

// Destructor
CCFTestDelay::~CCFTestDelay()
    {
    Cancel();
    delete iWait;
    }

// METHODS

//-----------------------------------------------------------------------------
// CCFTestDelay::Start
//-----------------------------------------------------------------------------
//
void CCFTestDelay::Start(
    const TTimeIntervalMicroSeconds32& aInterval )
    {
    __ASSERT_ALWAYS( !IsActive(), Panic( EAlreadyStarted ) );

    After( aInterval );
    iWait->Start();
    }

//-----------------------------------------------------------------------------
// CCFTestDelay::Stop
//-----------------------------------------------------------------------------
//
void CCFTestDelay::Stop()
    {
    Cancel();
    }

//-----------------------------------------------------------------------------
// CCFTestDelay::RunL
//-----------------------------------------------------------------------------
//
void CCFTestDelay::RunL()
    {
    // Double check that wait really started
    if( iWait->IsStarted() )
        {
        iWait->AsyncStop();
        }
    }

//-----------------------------------------------------------------------------
// CCFTestDelay::DoCancel
//-----------------------------------------------------------------------------
//
void CCFTestDelay::DoCancel()
    {
    // Double check that wait really started
    CTimer::DoCancel();
    if( iWait->IsStarted() )
        {
        iWait->AsyncStop();
        }
    }

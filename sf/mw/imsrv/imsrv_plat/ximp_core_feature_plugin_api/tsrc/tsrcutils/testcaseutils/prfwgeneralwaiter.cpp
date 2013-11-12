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



#include <e32std.h>

#include "prfwgeneralwaiter.h"
#include "prfwwaitobserver.h"



// ======== LOCAL FUNCTIONS ========


// ======== MEMBER FUNCTIONS ========


// ---------------------------------------------------------------------------
// ?description_if_needed
// ---------------------------------------------------------------------------
//
CXIMPTestGeneralWaiter::CXIMPTestGeneralWaiter( T_MWaitObserver* aObserver ) :
    CActive( EPriorityNormal ),
    iObserver( aObserver )
    {
    CActiveScheduler::Add( this );
    }


// ---------------------------------------------------------------------------
// ?description_if_needed
// ---------------------------------------------------------------------------
//
void CXIMPTestGeneralWaiter::ConstructL()
    {
    iTimer.CreateLocal();
    }


// ---------------------------------------------------------------------------
// ?description_if_needed
// ---------------------------------------------------------------------------
//
EXPORT_C CXIMPTestGeneralWaiter* CXIMPTestGeneralWaiter::NewL( T_MWaitObserver* aObserver )
    {
    CXIMPTestGeneralWaiter* self = new( ELeave ) CXIMPTestGeneralWaiter( aObserver );
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );
    return self;
    }


// ---------------------------------------------------------------------------
// ?description_if_needed
// ---------------------------------------------------------------------------
//
CXIMPTestGeneralWaiter::~CXIMPTestGeneralWaiter()
    {
    iTimer.Close();
    }


// ---------------------------------------------------------------------------
// ?implementation_description
// ---------------------------------------------------------------------------
//
EXPORT_C void CXIMPTestGeneralWaiter::WaitForL( TInt aSeconds )
    {
    TTimeIntervalMicroSeconds32 timeToWait( aSeconds * 1000000 );
    iTimer.After( iStatus, timeToWait );
    SetActive();
    }


void CXIMPTestGeneralWaiter::RunL()
    {
    iObserver->WaitCompleted( KErrNone );
    }


void CXIMPTestGeneralWaiter::DoCancel()
    {
    
    }


TInt CXIMPTestGeneralWaiter::RunErrorL( TInt /* aError */ )
    {
    iObserver->WaitCompleted( KErrGeneral );
    return 0;
    }


// End of file

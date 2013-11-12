/*
* Copyright (c) 2004 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  CStartupSubscriber implementation.
 *
*/


// INCLUDES
#include "StartupAppUi.h"
#include "StartupSubscriber.h"
#include "startup.hrh"
#include <e32svr.h>

// CONSTANTS

// ============================= MEMBER FUNCTIONS =============================

// ----------------------------------------------------------------------------
// CStartupSubscriber::NewL()
// ----------------------------------------------------------------------------
CStartupSubscriber* CStartupSubscriber::NewL( 
                            MStartupPropertyResponder& aStartupPropertyResponder,
                            const TUid& aCategory, 
                            TUint aKey )
    {
    CStartupSubscriber* self = new (ELeave) CStartupSubscriber( aStartupPropertyResponder,
                                                                aCategory, 
                                                                aKey );
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop(); //self
    return self;
    }

// ----------------------------------------------------------------------------
// CStartupSubscriber::ConstructL()
// ----------------------------------------------------------------------------
void CStartupSubscriber::ConstructL()
    {
    CActiveScheduler::Add( this );
    iProperty.Attach( iCategory, iKey );
    }

// ----------------------------------------------------------------------------
// CStartupSubscriber::Subscribe()
// ----------------------------------------------------------------------------
void CStartupSubscriber::Subscribe()
    {
    iProperty.Subscribe( iStatus );
    SetActive();
    }

// ----------------------------------------------------------------------------
// CStartupSubscriber::CStartupSubscriber()
// ----------------------------------------------------------------------------
CStartupSubscriber::CStartupSubscriber( MStartupPropertyResponder& aStartupPropertyResponder, 
                                        const TUid& aCategory, 
                                        TUint aKey ) :
    CActive( EPriorityStandard ),
    iStartupPropertyResponder( aStartupPropertyResponder ),
    iCategory( aCategory),
    iKey( aKey )
    {
    }

// ----------------------------------------------------------------------------
// CStartupSubscriber::RunL()
// ----------------------------------------------------------------------------
void CStartupSubscriber::RunL()
    {
    Subscribe();
    iStartupPropertyResponder.HandlePropertyChangedL( iCategory, iKey );
    }

// ----------------------------------------------------------------------------
// CStartupSubscriber::DoCancel()
// ----------------------------------------------------------------------------
void CStartupSubscriber::DoCancel()
    {
    iProperty.Cancel();
    }

// ----------------------------------------------------------------------------
// CStartupSubscriber::RunError()
// ----------------------------------------------------------------------------
TInt CStartupSubscriber::RunError( TInt aError )
    {    
    return aError;
    }

// ----------------------------------------------------------------------------
// CStartupSubscriber::~CStartupSubscriber()
// ----------------------------------------------------------------------------
CStartupSubscriber::~CStartupSubscriber()
    {
    Cancel();
    iProperty.Close();
    }

// End of File




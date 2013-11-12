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
* Description:  COutOfDiskSubscriber implementation.
 *
*/


// INCLUDES
#include "outofdisksubscriber.h"
#include "OodTraces.h"

// CONSTANTS

// ============================= MEMBER FUNCTIONS =============================

// ----------------------------------------------------------------------------
// COutOfDiskSubscriber::NewL()
// ----------------------------------------------------------------------------
COutOfDiskSubscriber* COutOfDiskSubscriber::NewL( 
                            MOutOfDiskPropertyResponder& aOutOfDiskPropertyResponder,
                            const TUid& aCategory, 
                            TUint aKey )
    {
    COutOfDiskSubscriber* self = new (ELeave) COutOfDiskSubscriber( aOutOfDiskPropertyResponder,
                                                                    aCategory, 
                                                                    aKey );
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );
    return self;
    }

// ----------------------------------------------------------------------------
// COutOfDiskSubscriber::ConstructL()
// ----------------------------------------------------------------------------
void COutOfDiskSubscriber::ConstructL()
    {
    CActiveScheduler::Add( this );
    iProperty.Attach( iCategory, iKey );
    }

// ----------------------------------------------------------------------------
// COutOfDiskSubscriber::Subscribe()
// ----------------------------------------------------------------------------
void COutOfDiskSubscriber::Subscribe()
    {
    iProperty.Subscribe( iStatus );
    SetActive();
    }

// ----------------------------------------------------------------------------
// COutOfDiskSubscriber::COutOfDiskSubscriber()
// ----------------------------------------------------------------------------
COutOfDiskSubscriber::COutOfDiskSubscriber( MOutOfDiskPropertyResponder& 
                                                aOutOfDiskPropertyResponder, 
                                            const TUid& aCategory, 
                                            TUint aKey ) :
    CActive( EPriorityStandard ),
    iOutOfDiskPropertyResponder( aOutOfDiskPropertyResponder ),
    iCategory( aCategory),
    iKey( aKey )
    {
    }

// ----------------------------------------------------------------------------
// COutOfDiskSubscriber::RunL()
// ----------------------------------------------------------------------------
void COutOfDiskSubscriber::RunL()
    {
    Subscribe();
    iOutOfDiskPropertyResponder.HandlePropertyChangedL( iCategory, iKey );
    }

// ----------------------------------------------------------------------------
// COutOfDiskSubscriber::DoCancel()
// ----------------------------------------------------------------------------
void COutOfDiskSubscriber::DoCancel()
    {
    iProperty.Cancel();
    }

// ----------------------------------------------------------------------------
// COutOfDiskSubscriber::RunError()
// ----------------------------------------------------------------------------
TInt COutOfDiskSubscriber::RunError( TInt aError )
    {    
    return aError;
    }

// ----------------------------------------------------------------------------
// COutOfDiskSubscriber::~COutOfDiskSubscriber()
// ----------------------------------------------------------------------------
COutOfDiskSubscriber::~COutOfDiskSubscriber()
    {
    Cancel();
    iProperty.Close();
    }

// End of File

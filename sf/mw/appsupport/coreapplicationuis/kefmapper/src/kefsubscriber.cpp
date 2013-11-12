/*
* Copyright (c) 2006 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Property subscriber implementation.
*
*/


// INCLUDES
#include "kefsubscriber.h"
#include "keflogger.h"

// ============================= MEMBER FUNCTIONS =============================

// ----------------------------------------------------------------------------
// CKefSubscriber::NewL
// ----------------------------------------------------------------------------
//
CKefSubscriber* CKefSubscriber::NewL( 
        MKefPropertyResponder& aKefPropertyResponder, 
        const TUid& aCategory, 
        const TUint aKey )
    {
    CKefSubscriber* self = 
        new (ELeave) CKefSubscriber( 
            aKefPropertyResponder, aCategory, aKey );
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop(); //self
    return self;
    }

// ----------------------------------------------------------------------------
// CKefSubscriber::ConstructL
// ----------------------------------------------------------------------------
//
void CKefSubscriber::ConstructL()
    {   
    CActiveScheduler::Add( this );   
    
    TInt err = iProperty.Attach( iCategory, iKey );
    if ( err == KErrNone )
        {        
        Subscribe();
        }
    else
        {
        __KEFLOGSTRING1("CKefSubscriber::ConstrucL() Problem attaching to The Key %d", err);
        User::Leave( err );
        }
    }

// ----------------------------------------------------------------------------
// CKefSubscriber::Subscribe
// ----------------------------------------------------------------------------
//
void CKefSubscriber::Subscribe()
    {
    Cancel();
    iProperty.Subscribe( iStatus );
    SetActive();
    }

// ----------------------------------------------------------------------------
// CKefSubscriber::CKefSubscriber
// ----------------------------------------------------------------------------
//
CKefSubscriber::CKefSubscriber( 
    MKefPropertyResponder& aKefPropertyResponder, 
    const TUid& aCategory, 
    const TUint aKey ) 
    : CActive( EPriorityStandard ),
      iKefPropertyResponder( aKefPropertyResponder ),
      iCategory( aCategory),
      iKey( aKey )
    {
    }

// ----------------------------------------------------------------------------
// CKefSubscriber::RunL
// ----------------------------------------------------------------------------
//
void CKefSubscriber::RunL()
    {
    if ( iStatus == KErrNone )
        {        
        Subscribe(); // Subscribe again.
        }
    
    iKefPropertyResponder.HandlePropertyChangedL( iCategory, iKey );
    }

// ----------------------------------------------------------------------------
// CKefSubscriber::DoCancel
// ----------------------------------------------------------------------------
//
void CKefSubscriber::DoCancel()
    {
    iProperty.Cancel();
    }

// ----------------------------------------------------------------------------
// CKefSubscriber::RunError
// ----------------------------------------------------------------------------
//
TInt CKefSubscriber::RunError( TInt aError )
    {    
    __KEFLOGSTRING1("CKefSubscriber::RunError: error: %d", aError );
    (void)aError; // for the compiler if the logging is not defined.
    return KErrNone;
    }

// ----------------------------------------------------------------------------
// CKefSubscriber::~CKefSubscriber
// ----------------------------------------------------------------------------
//
CKefSubscriber::~CKefSubscriber()
    {
    Cancel();
    iProperty.Close();
    }

// End of File

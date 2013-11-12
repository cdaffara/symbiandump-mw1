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
* Description:  CSysApSubscriber implementation.
*
*/


// INCLUDES
#include "SysApAppUi.h"
#include "SysApSubscriber.h"
#include "SysAp.hrh"
#include <e32svr.h>

// CONSTANTS

// ============================= MEMBER FUNCTIONS =============================

// ----------------------------------------------------------------------------
// CSysApSubscriber::NewL()
// ----------------------------------------------------------------------------
CSysApSubscriber* CSysApSubscriber::NewL( MSysApPropertyResponder& aSysApPropertyResponder, const TUid& aCategory, const TUint aKey )
    {
    CSysApSubscriber* self = new (ELeave) CSysApSubscriber( aSysApPropertyResponder, aCategory, aKey );
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop(); //self
    return self;
    }

// ----------------------------------------------------------------------------
// CSysApSubscriber::ConstructL()
// ----------------------------------------------------------------------------
void CSysApSubscriber::ConstructL()
    {
    CActiveScheduler::Add( this );
    iProperty.Attach( iCategory, iKey );
    }

// ----------------------------------------------------------------------------
// CSysApSubscriber::Subscribe()
// ----------------------------------------------------------------------------
void CSysApSubscriber::Subscribe()
    {
    iProperty.Subscribe( iStatus );
    SetActive();
    }

// ----------------------------------------------------------------------------
// CSysApSubscriber::CSysApSubscriber()
// ----------------------------------------------------------------------------
CSysApSubscriber::CSysApSubscriber( MSysApPropertyResponder& aSysApPropertyResponder, const TUid& aCategory, const TUint aKey ) :
    CActive( EPriorityStandard ),
    iSysApPropertyResponder( aSysApPropertyResponder ),
    iCategory( aCategory),
    iKey( aKey )
    {
    }

// ----------------------------------------------------------------------------
// CSysApSubscriber::RunL()
// ----------------------------------------------------------------------------
void CSysApSubscriber::RunL()
    {
    Subscribe();
    iSysApPropertyResponder.HandlePropertyChangedL( iCategory, iKey );
    }

// ----------------------------------------------------------------------------
// CSysApSubscriber::DoCancel()
// ----------------------------------------------------------------------------
void CSysApSubscriber::DoCancel()
    {
    iProperty.Cancel();
    }

// ----------------------------------------------------------------------------
// CSysApSubscriber::RunError()
// ----------------------------------------------------------------------------
TInt CSysApSubscriber::RunError( TInt aError )
    {    
    TRACES( RDebug::Print( _L("CSysApSubscriber::RunError: error: %d"), aError ) );
    return aError;
    }

// ----------------------------------------------------------------------------
// CSysApSubscriber::~CSysApSubscriber()
// ----------------------------------------------------------------------------
CSysApSubscriber::~CSysApSubscriber()
    {
    Cancel();
    iProperty.Close();
    }

// End of File




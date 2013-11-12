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
* Description:  CStartupMediatorPluginSubscriber implementation.
 *
*/


// INCLUDES
#include "StartupMediatorPluginSubscriber.h"
#include <e32svr.h>

// CONSTANTS

// ============================= MEMBER FUNCTIONS =============================

// ----------------------------------------------------------------------------
// CStartupMediatorPluginSubscriber::NewL()
// ----------------------------------------------------------------------------
CStartupMediatorPluginSubscriber* CStartupMediatorPluginSubscriber::NewL( 
                            MStartupMediatorPluginPropertyResponder& aStartupMediatorPluginPropertyResponder,
                            const TUid& aCategory, 
                            TUint aKey )
    {
    CStartupMediatorPluginSubscriber* self = new (ELeave) CStartupMediatorPluginSubscriber( aStartupMediatorPluginPropertyResponder,
                                                                                            aCategory, 
                                                                                            aKey );
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop(); //self
    return self;
    }

// ----------------------------------------------------------------------------
// CStartupMediatorPluginSubscriber::ConstructL()
// ----------------------------------------------------------------------------
void CStartupMediatorPluginSubscriber::ConstructL()
    {
    CActiveScheduler::Add( this );
    iProperty.Attach( iCategory, iKey );
    }

// ----------------------------------------------------------------------------
// CStartupMediatorPluginSubscriber::Subscribe()
// ----------------------------------------------------------------------------
void CStartupMediatorPluginSubscriber::Subscribe()
    {
    iProperty.Subscribe( iStatus );
    SetActive();
    }

// ----------------------------------------------------------------------------
// CStartupMediatorPluginSubscriber::CStartupMediatorPluginSubscriber()
// ----------------------------------------------------------------------------
CStartupMediatorPluginSubscriber::CStartupMediatorPluginSubscriber( MStartupMediatorPluginPropertyResponder& aStartupMediatorPluginPropertyResponder, 
                                        const TUid& aCategory, 
                                        TUint aKey ) :
    CActive( EPriorityStandard ),
    iStartupMediatorPluginPropertyResponder( aStartupMediatorPluginPropertyResponder ),
    iCategory( aCategory),
    iKey( aKey )
    {
    }

// ----------------------------------------------------------------------------
// CStartupMediatorPluginSubscriber::RunL()
// ----------------------------------------------------------------------------
void CStartupMediatorPluginSubscriber::RunL()
    {
    Subscribe();
    iStartupMediatorPluginPropertyResponder.HandlePropertyChangedL( iCategory, iKey );
    }

// ----------------------------------------------------------------------------
// CStartupMediatorPluginSubscriber::DoCancel()
// ----------------------------------------------------------------------------
void CStartupMediatorPluginSubscriber::DoCancel()
    {
    iProperty.Cancel();
    }

// ----------------------------------------------------------------------------
// CStartupMediatorPluginSubscriber::RunError()
// ----------------------------------------------------------------------------
TInt CStartupMediatorPluginSubscriber::RunError( TInt aError )
    {    
    return aError;
    }

// ----------------------------------------------------------------------------
// CStartupMediatorPluginSubscriber::~CStartupMediatorPluginSubscriber()
// ----------------------------------------------------------------------------
CStartupMediatorPluginSubscriber::~CStartupMediatorPluginSubscriber()
    {
    Cancel();
    iProperty.Close();
    }

// End of File

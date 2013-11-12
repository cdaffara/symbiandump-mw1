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
* Description:  Implementation of CProEngProfileActivationDelegate.
*
*/



// INCLUDE FILES
#include    "CProEngProfileActivationDelegate.h"
#include    <centralrepository.h>
#include    <MProEngProfileActivationObserver.h>
#include    <ProfileEnginePrivatePSKeys.h>  // KProEngActiveProfileChanged
#include    <ProfileEngineSDKCRKeys.h> // KCRUidProfileEngine etc.

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CProEngProfileActivationDelegate::CProEngProfileActivationDelegate
// -----------------------------------------------------------------------------
//
CProEngProfileActivationDelegate::CProEngProfileActivationDelegate(
        MProEngProfileActivationObserver& aObserver )
    : CProEngPubSubObserverBase( KProEngActiveProfileChanged ),
      iObserver( aObserver )
    {
    }

// -----------------------------------------------------------------------------
// CProEngProfileActivationDelegate::ConstructL
// -----------------------------------------------------------------------------
//
void CProEngProfileActivationDelegate::ConstructL()
    {
    iRepository = CRepository::NewL( KCRUidProfileEngine );
    }

// -----------------------------------------------------------------------------
// CProEngProfileActivationDelegate::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CProEngProfileActivationDelegate* CProEngProfileActivationDelegate::NewL(
        MProEngProfileActivationObserver& aObserver )
    {
    CProEngProfileActivationDelegate* self =
        new ( ELeave ) CProEngProfileActivationDelegate( aObserver );

    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );

    return self;
    }

// -----------------------------------------------------------------------------
// CProEngProfileActivationDelegate::~CProEngProfileActivationDelegate
// -----------------------------------------------------------------------------
//
CProEngProfileActivationDelegate::~CProEngProfileActivationDelegate()
    {
    delete iRepository;
    }

// -----------------------------------------------------------------------------
// CProEngProfileActivationDelegate::NotifyObserverL
// -----------------------------------------------------------------------------
//
void CProEngProfileActivationDelegate::NotifyObserverL()
    {
    // Read the id of the current active profile and pass it to the observer
    TInt profileId( 0 );
    User::LeaveIfError( iRepository->Get( KProEngActiveProfile, profileId ) );
    iObserver.HandleProfileActivatedL( profileId );
    }

// -----------------------------------------------------------------------------
// CProEngProfileActivationDelegate::NotifyError
// -----------------------------------------------------------------------------
//
void CProEngProfileActivationDelegate::NotifyError( TInt aError )
    {
    iObserver.HandleProfileActivationNotificationError( aError );
    }

//  End of File


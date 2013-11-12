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
* Description:  Implementation of CProEngProfileEventDelegate
*
*/



// INCLUDE FILES
#include    "CProEngProfileEventDelegate.h"
#include    <MProEngProfileObserver.h>

namespace
    {
    // CONSTANTS
    // Profile ID is encoded in the first byte of a Profiles Engine CR key
    const TInt KProEngProfileIdShift( 24 );
    const TUint32 KProEngProfileIdMask( 0xFF000000 );
    }

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CProEngProfileEventDelegate::CProEngProfileEventDelegate
// -----------------------------------------------------------------------------
//
CProEngProfileEventDelegate::CProEngProfileEventDelegate(
        TUint32 aPartialCenRepKey,
        TUint32 aKeyMask,
        MProEngProfileObserver& aObserver )
    : CProEngCenRepObserverBase( aPartialCenRepKey, aKeyMask ),
      iObserver( aObserver )
    {
    }

// -----------------------------------------------------------------------------
// CProEngProfileEventDelegate::NewL
// -----------------------------------------------------------------------------
//
CProEngProfileEventDelegate* CProEngProfileEventDelegate::NewL(
          TInt aProfileId,
          MProEngProfileObserver& aObserver )
    {
    CProEngProfileEventDelegate* self = new ( ELeave )
        CProEngProfileEventDelegate( ( aProfileId << KProEngProfileIdShift ),
                                     KProEngProfileIdMask, aObserver );

    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );

    return self;
    }

// -----------------------------------------------------------------------------
// CProEngProfileEventDelegate::NotifyObserverL
// -----------------------------------------------------------------------------
//
void CProEngProfileEventDelegate::NotifyObserverL()
    {
    iObserver.HandleProfileModifiedL( ProfileId() );
    }

// -----------------------------------------------------------------------------
// CProEngProfileEventDelegate::NotifyError
// -----------------------------------------------------------------------------
//
void CProEngProfileEventDelegate::NotifyError( TInt aError )
    {
    iObserver.HandleProfileNotificationError( aError );
    }

// -----------------------------------------------------------------------------
// CProEngProfileEventDelegate::NotifyError
// -----------------------------------------------------------------------------
//
TInt CProEngProfileEventDelegate::ProfileId() const
    {
    return ( iKey >> KProEngProfileIdShift );
    }

//  End of File


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
* Description:  Implementation of CProEngProfileNameArrayEventDelegate.
*
*/



// INCLUDE FILES
#include    "CProEngProfileNameArrayEventDelegate.h"
#include    <MProEngProfileNameArrayObserver.h>

namespace
    {
    // CONSTANTS
    // Conjunction of KProEngProfileName (for any profile) and KProEngUsedIds
    // (see the key values in ProfileEnginePrivateCRKeys.h). This is the
    // partial key used in NotifyRequest() call made by the parent class.
    const TUint32 KProEngPartialNameAndDynamicProfilesKey( 0x00000002 );
    // This mask is used to filter out the key for KProEngActiveRingingType when
    // requesting notifications of name modifications and profile creations and
    // deletions. The 1's in mask means that the bits in those positions of the
    // partial Central Repository key given in NotifyRequest() are significant.
    const TUint32 KProEngNameAndDynamicProfilesKeyMask( 0x40FFFFFF );
    }

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CProEngProfileNameArrayEventDelegate::CProEngProfileNameArrayEventDelegate
// -----------------------------------------------------------------------------
//
CProEngProfileNameArrayEventDelegate::CProEngProfileNameArrayEventDelegate(
        TUint32 aPartialCenRepKey,
        TUint32 aKeyMask,
        MProEngProfileNameArrayObserver& aObserver )
    : CProEngCenRepObserverBase( aPartialCenRepKey, aKeyMask ),
      iObserver( aObserver )
    {
    }

// -----------------------------------------------------------------------------
// CProEngProfileNameArrayEventDelegate::NewL
// -----------------------------------------------------------------------------
//
CProEngProfileNameArrayEventDelegate* CProEngProfileNameArrayEventDelegate::NewL
        ( MProEngProfileNameArrayObserver& aObserver )
          
    {
    CProEngProfileNameArrayEventDelegate* self = new ( ELeave )
        CProEngProfileNameArrayEventDelegate(
                KProEngPartialNameAndDynamicProfilesKey,
                KProEngNameAndDynamicProfilesKeyMask,
                aObserver );

    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );

    return self;
    }

// -----------------------------------------------------------------------------
// CProEngProfileNameArrayEventDelegate::NotifyObserverL
// -----------------------------------------------------------------------------
//
void CProEngProfileNameArrayEventDelegate::NotifyObserverL()
    {
    iObserver.HandleProfileNameArrayModificationL();
    }

// -----------------------------------------------------------------------------
// CProEngProfileNameArrayEventDelegate::NotifyError
// -----------------------------------------------------------------------------
//
void CProEngProfileNameArrayEventDelegate::NotifyError( TInt aError )
    {
    iObserver.HandleProfileNameArrayNotificationError( aError );
    }

//  End of File


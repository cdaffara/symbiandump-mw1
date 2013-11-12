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
* Description:  Implementation of CProEngActiveProfileDelegate
*
*/



// INCLUDE FILES
#include    "CProEngActiveProfileDelegate.h"
#include    <ProfileEnginePrivatePSKeys.h> // KProEngActiveProfileModified 
#include    <MProEngActiveProfileObserver.h>

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CProEngActiveProfileDelegate::CProEngActiveProfileDelegate
// -----------------------------------------------------------------------------
//
CProEngActiveProfileDelegate::CProEngActiveProfileDelegate(
        MProEngActiveProfileObserver& aObserver )
    : CProEngPubSubObserverBase( KProEngActiveProfileModified ),
      iObserver( aObserver )
    {
    }

// -----------------------------------------------------------------------------
// CProEngActiveProfileDelegate::NotifyObserverL
// -----------------------------------------------------------------------------
//
void CProEngActiveProfileDelegate::NotifyObserverL()
    {
    iObserver.HandleActiveProfileModifiedL();
    }

// -----------------------------------------------------------------------------
// CProEngActiveProfileDelegate::NotifyError
// -----------------------------------------------------------------------------
//
void CProEngActiveProfileDelegate::NotifyError( TInt aError )
    {
    iObserver.HandleActiveProfileNotificationError( aError );
    }

//  End of File


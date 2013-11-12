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
* Description:  Implementation of CProEngPubSubObserverBase.
*
*/



// INCLUDE FILES
#include    "CProEngPubSubObserverBase.h"
#include    <ProfileEnginePrivatePSKeys.h>

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CProEngPubSubObserverBase::CProEngPubSubObserverBase
// -----------------------------------------------------------------------------
//
CProEngPubSubObserverBase::CProEngPubSubObserverBase( TUint32 aPubSubKey )
    : CProEngObserverBase( aPubSubKey )
    {
    iAttatched = EFalse;
    }

// Destructor
CProEngPubSubObserverBase::~CProEngPubSubObserverBase()
    {
    Cancel();
    iProperty.Close();
    }

// -----------------------------------------------------------------------------
// CProEngPubSubObserverBase::RequestNotification
// -----------------------------------------------------------------------------
//
TInt CProEngPubSubObserverBase::RequestNotification()
    {
    TInt err = KErrNone;
    
    if ( !iAttatched )
        {
        err = iProperty.Attach( KPSUidProfileEngine, iKey );
        if ( err == KErrNone )
            {
            iAttatched = ETrue;
            }
        }

    if( err == KErrNone )
        {
        iProperty.Subscribe( iStatus );
        SetActive();
        }

    return err;
    }

// -----------------------------------------------------------------------------
// CProEngPubSubObserverBase::DoCancel
// -----------------------------------------------------------------------------
//
void CProEngPubSubObserverBase::DoCancel()
    {
    iProperty.Cancel();
    }

//  End of File


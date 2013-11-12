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
* Description:  Implementation of CProEngCenRepObserverBase.
*
*/



// INCLUDE FILES
#include    "CProEngCenRepObserverBase.h"
#include    <centralrepository.h>
#include    <ProfileEnginePrivatePSKeys.h>
#include    <ProfileEngineSDKCRKeys.h>

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CProEngCenRepObserverBase::CProEngCenRepObserverBase
// -----------------------------------------------------------------------------
//
CProEngCenRepObserverBase::CProEngCenRepObserverBase( TUint32 aPartialCenRepKey,
                                                      TUint32 aKeyMask )
    : CProEngObserverBase( aPartialCenRepKey ), iKeyMask( aKeyMask )
    {
    }

// Destructor
CProEngCenRepObserverBase::~CProEngCenRepObserverBase()
    {
    Cancel();
    delete iRepository;
    }

// -----------------------------------------------------------------------------
// CProEngCenRepObserverBase::ConstructL
// -----------------------------------------------------------------------------
//
void CProEngCenRepObserverBase::ConstructL()
    {
    iRepository = CRepository::NewL( KCRUidProfileEngine );
    }

// -----------------------------------------------------------------------------
// CProEngCenRepObserverBase::RequestNotification
// -----------------------------------------------------------------------------
//
TInt CProEngCenRepObserverBase::RequestNotification()
    {
    TInt result( iRepository->NotifyRequest( iKey, iKeyMask, iStatus ) );
    if( result == KErrNone )
        {
        SetActive();
        }

    return result;
    }

// -----------------------------------------------------------------------------
// CProEngCenRepObserverBase::DoCancel
// -----------------------------------------------------------------------------
//
void CProEngCenRepObserverBase::DoCancel()
    {
    iRepository->NotifyCancel( iKey, iKeyMask );
    }

//  End of File


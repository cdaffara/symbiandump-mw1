/*
* Copyright (c) 2002 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Implementation of the CProfilePttSettingsImpl.
*
*/



// INCLUDE FILES
#include "CProfilePttSettingsImpl.h"
#include "ProfileEngUtils.h"
#include "ProfileEnginePrivateCRKeys.h"
#include <centralrepository.h>

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CProfilePttSettingsImpl::ConstructL
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CProfilePttSettingsImpl::ConstructL()
    {
    iPttRingingTone = KNullDesC().AllocL();
    }

// -----------------------------------------------------------------------------
// CProfilePttSettingsImpl::ConstructL
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CProfilePttSettingsImpl::ConstructL(
    const MProfilePttSettings& aProfilePttSettings )
    {
    iPttRingingTone = aProfilePttSettings.PttRingingTone().AllocL();
    iPttStatus = aProfilePttSettings.PttStatus();
    }

// -----------------------------------------------------------------------------
// CProfilePttSettingsImpl::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CProfilePttSettingsImpl* CProfilePttSettingsImpl::NewL()
    {
    CProfilePttSettingsImpl* self = new( ELeave ) CProfilePttSettingsImpl;

    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop();

    return self;
    }

// -----------------------------------------------------------------------------
// CProfilePttSettingsImpl::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CProfilePttSettingsImpl* CProfilePttSettingsImpl::NewL(
    const MProfilePttSettings& aProfilePttSettings )
    {
    CProfilePttSettingsImpl* self = new( ELeave ) CProfilePttSettingsImpl;

    CleanupStack::PushL( self );
    self->ConstructL( aProfilePttSettings );
    CleanupStack::Pop();

    return self;
    }


// Destructor
CProfilePttSettingsImpl::~CProfilePttSettingsImpl()
    {
    delete iPttRingingTone;
    }


// -----------------------------------------------------------------------------
// CProfilePttSettingsImpl::InternalizeL
//
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CProfilePttSettingsImpl::InternalizeL( CRepository& aCenRep,
                                            TInt aProfileId )
    {
    delete iPttRingingTone;
    iPttRingingTone = NULL;

    TFileName toneFile;

    // PTT ringing tone
    User::LeaveIfError(
            aCenRep.Get( ProfileEngUtils::ResolveKey( KProEngPttTone,
            aProfileId ), toneFile ) );
    iPttRingingTone = toneFile.AllocL();

    // PTT status
    TInt status( 0 );
    User::LeaveIfError(
            aCenRep.Get( ProfileEngUtils::ResolveKey( KProEngPttStatus,
            aProfileId ), status ) );
    iPttStatus = TProfilePttStatus( status );
    }

// -----------------------------------------------------------------------------
// CProfilePttSettingsImpl::ExternalizeL
//
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CProfilePttSettingsImpl::ExternalizeL( CRepository& aCenRep,
                                            TInt aProfileId ) const
    {
    // PTT Ringing tone
    User::LeaveIfError(
            aCenRep.Set( ProfileEngUtils::ResolveKey( KProEngPttTone,
            aProfileId ), *iPttRingingTone ) );

    // PTT status
    User::LeaveIfError(
            aCenRep.Set( ProfileEngUtils::ResolveKey( KProEngPttStatus,
            aProfileId ), iPttStatus ) );

    }

// -----------------------------------------------------------------------------
// CProfilePttSettingsImpl::PttRingingTone
//
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
const TDesC& CProfilePttSettingsImpl::PttRingingTone() const
    {
    return *iPttRingingTone;
    }

// -----------------------------------------------------------------------------
// CProfilePttSettingsImpl::SetPttRingingToneL
//
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CProfilePttSettingsImpl::SetPttRingingToneL( const TDesC& aPttRingingTone )
    {
    HBufC* tmp = aPttRingingTone.Left( KMaxFileName ).AllocL();
    delete iPttRingingTone;
    iPttRingingTone = tmp;
    }

// -----------------------------------------------------------------------------
// CProfilePttSettingsImpl::PttStatus
//
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TProfilePttStatus CProfilePttSettingsImpl::PttStatus() const
    {
    return iPttStatus;
    }

// -----------------------------------------------------------------------------
// CProfilePttSettingsImpl::PttStatus
//
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CProfilePttSettingsImpl::SetPttStatus( TProfilePttStatus aPttStatus )
    {
    iPttStatus = aPttStatus;
    }

//  End of File


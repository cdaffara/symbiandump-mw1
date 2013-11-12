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
* Description:  Implementation of the CProfilePresenceImpl.
*                This implementation is used when presence is supported.
*
*/



// INCLUDE FILES
#include "CProfilePresenceImpl.h"
#include "ProfileEnginePrivateCRKeys.h"
#include "ProfileEngUtils.h"
#include <centralrepository.h>
#include <bautils.h>
#include <barsc.h>
#include <ProfileEng.rsg>
#include <ProfileEng.hrh>
#include <ProfileEngineConstants.h>

// EXTERNAL DATA STRUCTURES
// EXTERNAL FUNCTION PROTOTYPES
// CONSTANTS
const TInt KProfileMaxStatusMessageLength( PROFILES_MAX_STATUS_MSG_LENGTH );

// MACROS
// LOCAL CONSTANTS AND MACROS
// MODULE DATA STRUCTURES
// LOCAL FUNCTION PROTOTYPES
// FORWARD DECLARATIONS

// ============================= LOCAL FUNCTIONS ===============================

// ============================ MEMBER FUNCTIONS ===============================


// -----------------------------------------------------------------------------
// CProfileNameImpl::CProfilePresenceImpl
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
CProfilePresenceImpl::CProfilePresenceImpl()
    {
    }

// -----------------------------------------------------------------------------
// CProfilePresenceImpl::ConstructL
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CProfilePresenceImpl::ConstructL()
    {
    iStatusMessage = KNullDesC().AllocL();
    }

// -----------------------------------------------------------------------------
// CProfilePresenceImpl::ConstructL
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CProfilePresenceImpl::ConstructL(
    const MProfilePresence& aPresence )
    {
    iStatusMessage = aPresence.StatusMessage().AllocL();
    iAvailabilityAttribute = aPresence.AvailabilityAttribute();
    }

// -----------------------------------------------------------------------------
// CProfilePresenceImpl::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CProfilePresenceImpl* CProfilePresenceImpl::NewL()
    {
    CProfilePresenceImpl* self = new( ELeave ) CProfilePresenceImpl;

    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop();

    return self;
    }

// -----------------------------------------------------------------------------
// CProfilePresenceImpl::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CProfilePresenceImpl* CProfilePresenceImpl::NewL(
    const MProfilePresence& aPresence )
    {
    CProfilePresenceImpl* self = new( ELeave ) CProfilePresenceImpl;

    CleanupStack::PushL( self );
    self->ConstructL( aPresence );
    CleanupStack::Pop();

    return self;
    }

// Destructor
CProfilePresenceImpl::~CProfilePresenceImpl()
    {
    delete iStatusMessage;
    }

// -----------------------------------------------------------------------------
// CProfilePresenceImpl::InternalizeL
//
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CProfilePresenceImpl::InternalizeL( CRepository& aCenRep,
                                           TInt aProfileId )
    {
    delete iStatusMessage;
    iStatusMessage = NULL;

    // Status message
    TBuf< KProfileMaxStatusMessageLength > buf;
    aCenRep.Get( ProfileEngUtils::ResolveKey( KProEngStatusMsg,
                 aProfileId ), buf );
    iStatusMessage = buf.AllocL();

    // AvailabilityAttribute
    TInt tmpAttr( 0 );
    aCenRep.Get( ProfileEngUtils::ResolveKey( KProEngAvailAttribute,
                 aProfileId ), tmpAttr );
    iAvailabilityAttribute = tmpAttr;
    }

// -----------------------------------------------------------------------------
// CProfilePresenceImpl::ExternalizeL
//
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CProfilePresenceImpl::ExternalizeL( CRepository& aCenRep,
                                           TInt aProfileId ) const
    {
    aCenRep.Set( ProfileEngUtils::ResolveKey( KProEngStatusMsg,
                 aProfileId ), *iStatusMessage );
    aCenRep.Set( ProfileEngUtils::ResolveKey( KProEngAvailAttribute,
                 aProfileId ), ( TInt ) iAvailabilityAttribute );
    }

// -----------------------------------------------------------------------------
// CProfilePresenceImpl::AvailabilityAttribute
//
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TUint32 CProfilePresenceImpl::AvailabilityAttribute() const
    {
    return iAvailabilityAttribute;
    }

// -----------------------------------------------------------------------------
// CProfilePresenceImpl::SetAvailabilityAttribute
//
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CProfilePresenceImpl::SetAvailabilityAttribute(
    const TUint32 aAvailabilityAttribute )
    {
    iAvailabilityAttribute = aAvailabilityAttribute;
    }

// -----------------------------------------------------------------------------
// CProfilePresenceImpl::StatusMessage
//
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
const TDesC& CProfilePresenceImpl::StatusMessage() const
    {
    return *iStatusMessage;
    }

// -----------------------------------------------------------------------------
// CProfilePresenceImpl::SetStatusMessageL
//
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CProfilePresenceImpl::SetStatusMessageL(
    const TDesC& aStatusMessage )
    {
    HBufC* tmp = aStatusMessage.Left( KProfileMaxStatusMessageLength ).AllocL();
    delete iStatusMessage;
    iStatusMessage = tmp;
    }

// ========================== OTHER EXPORTED FUNCTIONS =========================

//  End of File

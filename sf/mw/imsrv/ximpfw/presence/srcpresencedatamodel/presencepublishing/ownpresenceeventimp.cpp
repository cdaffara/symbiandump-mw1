/*
* Copyright (c) 2006 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  MOwnPresenceEvent API object implementation.
 *
*/

#include "ownpresenceeventimp.h"
#include "presenceinfoimp.h"
#include "ximpobjecthelpers.h"
#include "ximpdatasubscriptionstateimp.h"
#include <s32strm.h>


// ---------------------------------------------------------------------------
// COwnPresenceEventImp::NewLC()
// ---------------------------------------------------------------------------
//
EXPORT_C COwnPresenceEventImp* COwnPresenceEventImp::NewLC(
    CXIMPDataSubscriptionStateImp* aState )
    {
    COwnPresenceEventImp* self = new( ELeave ) COwnPresenceEventImp();
    CleanupStack::PushL( self );
    self->ConstructL( aState );
    return self;
    }

// ---------------------------------------------------------------------------
// COwnPresenceEventImp::NewL()
// ---------------------------------------------------------------------------
//
EXPORT_C COwnPresenceEventImp* COwnPresenceEventImp::NewL(
    CXIMPDataSubscriptionStateImp* aState )
    {
    COwnPresenceEventImp* self = NewLC( aState );
    CleanupStack::Pop( self );
    return self;
    }


// ---------------------------------------------------------------------------
// COwnPresenceEventImp::NewFromStreamLC()
// ---------------------------------------------------------------------------
//
CXIMPApiEventBase* COwnPresenceEventImp::NewFromStreamLC( RReadStream& aStream )
    {
    COwnPresenceEventImp* self = new( ELeave ) COwnPresenceEventImp;
    CleanupStack::PushL( self );
    self->ConstructL( aStream );
    return self;
    }

// ---------------------------------------------------------------------------
// COwnPresenceEventImp::~COwnPresenceEventImp()
// ---------------------------------------------------------------------------
//
COwnPresenceEventImp::~COwnPresenceEventImp()
    {
    delete iPresenceInfo;
    delete iSubscriptionState;
    }


// ---------------------------------------------------------------------------
// COwnPresenceEventImp::COwnPresenceEventImp()
// ---------------------------------------------------------------------------
//
COwnPresenceEventImp::COwnPresenceEventImp()
    {
    }

// ---------------------------------------------------------------------------
// COwnPresenceEventImp::ConstructL()
// ---------------------------------------------------------------------------
//
void COwnPresenceEventImp::ConstructL( CXIMPDataSubscriptionStateImp* aState )
    {
    iSubscriptionState = aState;
    iPresenceInfo = CPresenceInfoImp::NewL();
    }

// ---------------------------------------------------------------------------
// COwnPresenceEventImp::ConstructL()
// ---------------------------------------------------------------------------
//
void COwnPresenceEventImp::ConstructL( RReadStream& aStream )
    {
    iSubscriptionState = CXIMPDataSubscriptionStateImp::NewL();
    iPresenceInfo = CPresenceInfoImp::NewL();
    InternalizeL( aStream );
    }

// ---------------------------------------------------------------------------
// Implement supported interface access.
// ---------------------------------------------------------------------------
//
XIMPIMP_IF_BASE_GET_INTERFACE_BEGIN( COwnPresenceEventImp,
                                     MOwnPresenceEvent )
    XIMPIMP_IF_BASE_GET_CLASS( CXIMPApiEventBase, this )
XIMPIMP_IF_BASE_GET_INTERFACE_END()


XIMPIMP_IF_BASE_GET_CONST_INTERFACE_BEGIN( COwnPresenceEventImp,
                                           MOwnPresenceEvent )
    XIMPIMP_IF_BASE_GET_CONST_CLASS( CXIMPApiEventBase, this )                                            
XIMPIMP_IF_BASE_GET_INTERFACE_END()



// ---------------------------------------------------------------------------
// Implement methods from base event interface and base event class
// ---------------------------------------------------------------------------
//
XIMPIMP_IMPLEMENT_EVENT_BASE_METHODS( COwnPresenceEventImp,
                                      MOwnPresenceEvent )

// ---------------------------------------------------------------------------
// COwnPresenceEventImp::EqualsContent()
// ---------------------------------------------------------------------------
//
TBool COwnPresenceEventImp::EqualsContent( 
    const CXIMPApiEventBase& aOtherInstance ) const
    {
    const COwnPresenceEventImp* tmp =
    TXIMPGetImpClassOrPanic< const COwnPresenceEventImp>::From( aOtherInstance.Base() );


    TBool same = ETrue;

    TBool x = iPresenceInfo->EqualsContent( *tmp->iPresenceInfo );
    same &= x;
    
    x = iSubscriptionState->EqualsContent( *tmp->iSubscriptionState );
    same &= x;

    return same;
    }

// ---------------------------------------------------------------------------
// From class CXIMPApiEventBase.
// COwnPresenceEventImp::ExternalizeL()
// ---------------------------------------------------------------------------
//
void COwnPresenceEventImp::ExternalizeL( RWriteStream& aStream ) const
    {
    iPresenceInfo->ExternalizeL( aStream );
    iSubscriptionState->ExternalizeL( aStream );
    }


// ---------------------------------------------------------------------------
// COwnPresenceEventImp::InternalizeL()
// ---------------------------------------------------------------------------
//
void COwnPresenceEventImp::InternalizeL( RReadStream& aStream )
    {
    iPresenceInfo->InternalizeL( aStream );
    iSubscriptionState->InternalizeL( aStream );
    }


// ---------------------------------------------------------------------------
// From class MOwnPresenceEvent.
// COwnPresenceEventImp::Status()
// ---------------------------------------------------------------------------
//
const MXIMPDataSubscriptionState&
    COwnPresenceEventImp::DataSubscriptionState() const
    {
    return *iSubscriptionState;
    }


// ---------------------------------------------------------------------------
// From class MOwnPresenceEvent.
// COwnPresenceEventImp::SubscribedPresence()
// ---------------------------------------------------------------------------
//
const MPresenceInfo& 
    COwnPresenceEventImp::SubscribedPresence() const
    {
    return *iPresenceInfo;
    }

// ---------------------------------------------------------------------------
// COwnPresenceEventImp::SetPresenceInfo
// ---------------------------------------------------------------------------
//
void COwnPresenceEventImp::SetPresenceInfo( CPresenceInfoImp* aInfoImp )
    {
    delete iPresenceInfo;
    iPresenceInfo = aInfoImp;
    }

// End of file


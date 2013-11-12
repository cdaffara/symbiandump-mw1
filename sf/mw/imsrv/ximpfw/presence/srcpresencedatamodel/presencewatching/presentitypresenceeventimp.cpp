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
* Description:  MPresentityPresenceEvent API object implementation.
 *
*/

#include "presentitypresenceeventimp.h"
#include "presenceinfoimp.h"
#include "ximpidentityimp.h"
#include "ximpdatasubscriptionstateimp.h"
#include "ximpobjecthelpers.h"
#include <s32strm.h>



// ---------------------------------------------------------------------------
// CPresentityPresenceEventImp::NewLC()
// ---------------------------------------------------------------------------
//
EXPORT_C CPresentityPresenceEventImp* CPresentityPresenceEventImp::NewLC(
    CXIMPDataSubscriptionStateImp* aState )
    {
    CPresentityPresenceEventImp* self = new( ELeave ) CPresentityPresenceEventImp();
    CleanupStack::PushL( self );
    self->ConstructL( aState );
    return self;
    }

// ---------------------------------------------------------------------------
// CPresentityPresenceEventImp::NewL()
// ---------------------------------------------------------------------------
//
EXPORT_C CPresentityPresenceEventImp* CPresentityPresenceEventImp::NewL(
    CXIMPDataSubscriptionStateImp* aState )
    {
    CPresentityPresenceEventImp* self = NewLC( aState );
    CleanupStack::Pop( self );
    return self;
    }

// ---------------------------------------------------------------------------
// CPresentityPresenceEventImp::NewFromStreamLC()
// ---------------------------------------------------------------------------
//
CXIMPApiEventBase* CPresentityPresenceEventImp::NewFromStreamLC( RReadStream& aStream )
    {
    CPresentityPresenceEventImp* self = new( ELeave ) CPresentityPresenceEventImp;
    CleanupStack::PushL( self );
    self->ConstructL( aStream );
    return self;
    }

// ---------------------------------------------------------------------------
// CPresentityPresenceEventImp::~CPresentityPresenceEventImp()
// ---------------------------------------------------------------------------
//
CPresentityPresenceEventImp::~CPresentityPresenceEventImp()
    {
    delete iPresentityIdentity;
    delete iSubscriptionState;
    delete iPresenceInfo;
    }


// ---------------------------------------------------------------------------
// CPresentityPresenceEventImp::CPresentityPresenceEventImp()
// ---------------------------------------------------------------------------
//
CPresentityPresenceEventImp::CPresentityPresenceEventImp()
    {
    }


// ---------------------------------------------------------------------------
// CPresentityPresenceEventImp::ConstructL()
// ---------------------------------------------------------------------------
//
void CPresentityPresenceEventImp::ConstructL( CXIMPDataSubscriptionStateImp* aState )
    {
    iPresentityIdentity = CXIMPIdentityImp::NewL();
    iPresenceInfo = CPresenceInfoImp::NewL();
    
    iSubscriptionState = aState;
    }

// ---------------------------------------------------------------------------
// CPresentityGroupContentEventImp::ConstructL()
// ---------------------------------------------------------------------------
//
void CPresentityPresenceEventImp::ConstructL( RReadStream& aStream )
    {
    iPresentityIdentity = CXIMPIdentityImp::NewL();
    iPresenceInfo = CPresenceInfoImp::NewL();
    iSubscriptionState = CXIMPDataSubscriptionStateImp::NewL();
    InternalizeL( aStream );
    }

// ---------------------------------------------------------------------------
// Implement supported interface access.
// ---------------------------------------------------------------------------
//
XIMPIMP_IF_BASE_GET_INTERFACE_BEGIN( CPresentityPresenceEventImp,
                                     MPresentityPresenceEvent )
    XIMPIMP_IF_BASE_GET_CLASS( CXIMPApiEventBase, this )
XIMPIMP_IF_BASE_GET_INTERFACE_END()

XIMPIMP_IF_BASE_GET_CONST_INTERFACE_BEGIN( CPresentityPresenceEventImp,
                                           MPresentityPresenceEvent )
    XIMPIMP_IF_BASE_GET_CONST_CLASS( CXIMPApiEventBase, this )                                            
XIMPIMP_IF_BASE_GET_INTERFACE_END()



// ---------------------------------------------------------------------------
// Implement methods from base event interface and base event class
// ---------------------------------------------------------------------------
//
XIMPIMP_IMPLEMENT_EVENT_BASE_METHODS( CPresentityPresenceEventImp,
                                      MPresentityPresenceEvent )

// ---------------------------------------------------------------------------
// CPresentityPresenceEventImp::EqualsContent
// ---------------------------------------------------------------------------
//
TBool CPresentityPresenceEventImp::EqualsContent( 
    const CXIMPApiEventBase& aOtherInstance ) const 
    {
    const CPresentityPresenceEventImp* tmp =
    TXIMPGetImpClassOrPanic< const CPresentityPresenceEventImp >::From( 
                                                        aOtherInstance.Base() );
    TBool same = ETrue;
    TBool x = iPresentityIdentity->EqualsContent( *tmp->iPresentityIdentity );
    same &= x;

    x = iSubscriptionState->EqualsContent( *tmp->iSubscriptionState );
    same &= x;

    x = iPresenceInfo->EqualsContent( *tmp->iPresenceInfo );
    same &= x;

    return same;
    }

// ---------------------------------------------------------------------------
// From class CXIMPApiEventBase
// CPresentityPresenceEventImp::ExternalizeL
// ---------------------------------------------------------------------------
//
void CPresentityPresenceEventImp::ExternalizeL( RWriteStream& aStream ) const
    {
    iPresentityIdentity->ExternalizeL( aStream );
    iSubscriptionState->ExternalizeL( aStream );
    iPresenceInfo->ExternalizeL( aStream );
    }



// ---------------------------------------------------------------------------
// CPresentityPresenceEventImp::InternalizeL()
// ---------------------------------------------------------------------------
//
void CPresentityPresenceEventImp::InternalizeL( RReadStream& aStream )
    {
    iPresentityIdentity->InternalizeL( aStream );
    iSubscriptionState->InternalizeL( aStream );
    iPresenceInfo->InternalizeL( aStream );
    }


// ---------------------------------------------------------------------------
// From class MPresentityPresenceEvent.
// CPresentityPresenceEventImp::PresentityId()
// ---------------------------------------------------------------------------
//
const MXIMPIdentity& CPresentityPresenceEventImp::PresentityId() const
    {
    return *iPresentityIdentity;
    }


// ---------------------------------------------------------------------------
// From class MPresentityPresenceEvent.
// CPresentityPresenceEventImp::SubscribedPresence()
// ---------------------------------------------------------------------------
//
const MPresenceInfo& 
    CPresentityPresenceEventImp::SubscribedPresence() const
    {
    return *iPresenceInfo;
    }
    

// ---------------------------------------------------------------------------
// From class MPresentityPresenceEvent.
// COwnPresenceEventImp::Status()
// ---------------------------------------------------------------------------
//
const MXIMPDataSubscriptionState&
    CPresentityPresenceEventImp::DataSubscriptionState() const
    {
    return *iSubscriptionState;
    }

// ---------------------------------------------------------------------------
// COwnPresenceEventImp::SetPresenceInfo
// ---------------------------------------------------------------------------
//
void CPresentityPresenceEventImp::SetPresenceInfo( CPresenceInfoImp* aInfoImp )
    {
    delete iPresenceInfo;
    iPresenceInfo = aInfoImp;
    }

// ---------------------------------------------------------------------------
// COwnPresenceEventImp::SetIdentityL()
// ---------------------------------------------------------------------------
//
void CPresentityPresenceEventImp::SetIdentityL( const CXIMPIdentityImp& aIdentity )
    {
    CXIMPIdentityImp* id = TXIMPObjectCloner< CXIMPIdentityImp >::CloneL( aIdentity );
    delete iPresentityIdentity;
    iPresentityIdentity = id;
    }


// End of file


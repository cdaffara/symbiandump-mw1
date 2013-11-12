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
* Description:  MXIMPDataSubscriptionState API object implementation.
*
*/

#include "ximpdatasubscriptionstateimp.h"
#include "ximptrace.h"

// ============================ MEMBER FUNCTIONS =============================

// ---------------------------------------------------------------------------
// CXIMPDataSubscriptionStateImp::NewLC()
// ---------------------------------------------------------------------------
//
EXPORT_C CXIMPDataSubscriptionStateImp* CXIMPDataSubscriptionStateImp::NewLC()
    {
    CXIMPDataSubscriptionStateImp* self = new( ELeave ) CXIMPDataSubscriptionStateImp;
    CleanupStack::PushL( self );
    self->ConstructL();
    return self;
    }


// ---------------------------------------------------------------------------
// CXIMPDataSubscriptionStateImp::NewL()
// ---------------------------------------------------------------------------
//
EXPORT_C CXIMPDataSubscriptionStateImp* CXIMPDataSubscriptionStateImp::NewL()
    {
    CXIMPDataSubscriptionStateImp* self = CXIMPDataSubscriptionStateImp::NewLC();
    CleanupStack::Pop( self );
    return self;
    }



// ---------------------------------------------------------------------------
// CXIMPDataSubscriptionStateImp::~CXIMPDataSubscriptionStateImp()
// ---------------------------------------------------------------------------
//
CXIMPDataSubscriptionStateImp::~CXIMPDataSubscriptionStateImp()
    {
    }


// ---------------------------------------------------------------------------
// CXIMPDataSubscriptionStateImp::CXIMPDataSubscriptionStateImp()
// ---------------------------------------------------------------------------
//
CXIMPDataSubscriptionStateImp::CXIMPDataSubscriptionStateImp()
    {
    iSubscriptionState = MXIMPDataSubscriptionState::ESubscriptionInactive;
    iDataState = MXIMPDataSubscriptionState::EDataUnavailable;
    }


// ---------------------------------------------------------------------------
// CXIMPDataSubscriptionStateImp::ConstructL()
// ---------------------------------------------------------------------------
//
void CXIMPDataSubscriptionStateImp::ConstructL()
    {
    }


// ---------------------------------------------------------------------------
// CXIMPDataSubscriptionStateImp::NewFromStreamLC()
// ---------------------------------------------------------------------------
//
XIMPIMP_IMPLEMENT_DATAOBJ_NEWFROMSTREAM( CXIMPDataSubscriptionStateImp )


// ---------------------------------------------------------------------------
// Implement supported interface access.
// ---------------------------------------------------------------------------
//
XIMPIMP_IF_BASE_GET_INTERFACE_BEGIN( CXIMPDataSubscriptionStateImp,
                                     MXIMPDataSubscriptionState )
XIMPIMP_IF_BASE_GET_INTERFACE_END()


XIMPIMP_IF_BASE_GET_CONST_INTERFACE_BEGIN( CXIMPDataSubscriptionStateImp,
                                           MXIMPDataSubscriptionState )
XIMPIMP_IF_BASE_GET_INTERFACE_END()



// ---------------------------------------------------------------------------
// From class MXIMPDataSubscriptionState.
// CXIMPDataSubscriptionStateImp::SubscriptionState()
// ---------------------------------------------------------------------------
//
MXIMPDataSubscriptionState::TSubscriptionState
    CXIMPDataSubscriptionStateImp::SubscriptionState() const
    {
    return iSubscriptionState;
    }


// ---------------------------------------------------------------------------
// From class MXIMPDataSubscriptionState.
// CXIMPDataSubscriptionStateImp::DataState()
// ---------------------------------------------------------------------------
//
MXIMPDataSubscriptionState::TDataState
    CXIMPDataSubscriptionStateImp::DataState() const
    {
    return iDataState;
    }


// ---------------------------------------------------------------------------
// From class MXIMPDataSubscriptionState.
// CXIMPDataSubscriptionStateImp::SetSubscriptionState()
// ---------------------------------------------------------------------------
//
void CXIMPDataSubscriptionStateImp::SetSubscriptionStateL(
    MXIMPDataSubscriptionState::TSubscriptionState aSubscriptionState )
    {
    SetSubscriptionState( aSubscriptionState );
    }


// ---------------------------------------------------------------------------
// From class MXIMPDataSubscriptionState.
// CXIMPDataSubscriptionStateImp::SetDataState()
// ---------------------------------------------------------------------------
//
void CXIMPDataSubscriptionStateImp::SetDataStateL(
    MXIMPDataSubscriptionState::TDataState aDataState )
    {
    SetDataState( aDataState );
    }


// ---------------------------------------------------------------------------
// From class MXIMPDataSubscriptionState.
// CXIMPDataSubscriptionStateImp::SetSubscriptionState()
// ---------------------------------------------------------------------------
//
void CXIMPDataSubscriptionStateImp::SetSubscriptionState(
    MXIMPDataSubscriptionState::TSubscriptionState aSubscriptionState )
    {
    if ( MXIMPDataSubscriptionState::ESubscriptionActive == aSubscriptionState )
        {
        TRACE( _L("CXIMPDataSubscriptionStateImp::SetSubscriptionState ESubscriptionActive" ) );
        }
    else if ( MXIMPDataSubscriptionState::ESubscriptionInactive == aSubscriptionState )
        {
        TRACE( _L("CXIMPDataSubscriptionStateImp::SetSubscriptionState ESubscriptionInactive" ) );
        }
    else 
        {
        TRACE( _L("CXIMPDataSubscriptionStateImp::SetSubscriptionState XXX" ) );
        }
    iSubscriptionState = aSubscriptionState;
    }


// ---------------------------------------------------------------------------
// From class MXIMPDataSubscriptionState.
// CXIMPDataSubscriptionStateImp::SetDataState()
// ---------------------------------------------------------------------------
//
void CXIMPDataSubscriptionStateImp::SetDataState(
    MXIMPDataSubscriptionState::TDataState aDataState )
    {
    iDataState = aDataState;
    }


// ---------------------------------------------------------------------------
// CXIMPDataSubscriptionStateImp::ExternalizeL()
// ---------------------------------------------------------------------------
//
void CXIMPDataSubscriptionStateImp::ExternalizeL( RWriteStream& aStream ) const
    {
    aStream.WriteInt32L( iSubscriptionState );
    aStream.WriteInt32L( iDataState );
    }


// ---------------------------------------------------------------------------
// CXIMPDataSubscriptionStateImp::InternalizeL()
// ---------------------------------------------------------------------------
//
EXPORT_C void CXIMPDataSubscriptionStateImp::InternalizeL( RReadStream& aStream )
    {
    iSubscriptionState =
        (MXIMPDataSubscriptionState::TSubscriptionState) aStream.ReadInt32L();

    iDataState =
        (MXIMPDataSubscriptionState::TDataState) aStream.ReadInt32L();
    }


// ---------------------------------------------------------------------------
// CXIMPDataSubscriptionStateImp::EqualsContent()
// ---------------------------------------------------------------------------
//
TBool CXIMPDataSubscriptionStateImp::EqualsContent(
    const CXIMPApiDataObjBase& aOtherInstance ) const
    {
    const CXIMPDataSubscriptionStateImp* tmp =
    TXIMPGetImpClassOrPanic< const CXIMPDataSubscriptionStateImp >::From(
                                                        aOtherInstance.Base() );

    TBool totallySame = ETrue;
    TBool x;
    x = iSubscriptionState == tmp->iSubscriptionState;
    totallySame &= x;

    x = iDataState == tmp->iDataState;
    totallySame &= x;
    
    return totallySame;
    }


// End of file

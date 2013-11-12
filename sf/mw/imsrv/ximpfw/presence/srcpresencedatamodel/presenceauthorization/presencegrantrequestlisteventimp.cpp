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
* Description:  Presence watcher list event implementation.
 *
*/

#include "presencegrantrequestlisteventimp.h"
#include "presencegrantrequestinfoimp.h"
#include "ximpobjecthelpers.h"
#include "ximpidentityimp.h"
#include "ximpdatasubscriptionstateimp.h"
#include <s32strm.h>


// ---------------------------------------------------------------------------
// CPresenceGrantRequestListEventImp::NewLC()
// ---------------------------------------------------------------------------
//
EXPORT_C CPresenceGrantRequestListEventImp* CPresenceGrantRequestListEventImp::NewLC(
        RPrGrntReqInfoImpArray* aNew,
        RPrGrntReqInfoImpArray* aPending,
        RPrGrntReqInfoImpArray* aObsoleted,
        CXIMPDataSubscriptionStateImp* aState
        )
    {
    CPresenceGrantRequestListEventImp* self = new( ELeave ) CPresenceGrantRequestListEventImp();
    CleanupStack::PushL( self );
    self->ConstructL( aNew, aPending, aObsoleted, aState );
    return self;
    }

// ---------------------------------------------------------------------------
// CPresenceGrantRequestListEventImp::NewFromStreamLC()
// ---------------------------------------------------------------------------
//
CXIMPApiEventBase* CPresenceGrantRequestListEventImp::NewFromStreamLC( RReadStream& aStream )
    {
    CPresenceGrantRequestListEventImp* self = new( ELeave ) CPresenceGrantRequestListEventImp;
    CleanupStack::PushL( self );
    self->ConstructL( aStream );
    return self;
    }

// ---------------------------------------------------------------------------
// CPresenceGrantRequestListEventImp::~CPresenceGrantRequestListEventImp()
// ---------------------------------------------------------------------------
//
CPresenceGrantRequestListEventImp::~CPresenceGrantRequestListEventImp()
    {
    if( iConstructedFromStream )
        {
        delete iNewRequests;
        delete iPending;
        delete iObsoleted;
        iPending = NULL;
        }
    if( iOwnsPending )
        {
        delete iPending;
        }

    delete iSubscriptionState;
    }


// ---------------------------------------------------------------------------
// CPresenceGrantRequestListEventImp::CPresenceGrantRequestListEventImp()
// ---------------------------------------------------------------------------
//
CPresenceGrantRequestListEventImp::CPresenceGrantRequestListEventImp()
    {
    }


// ---------------------------------------------------------------------------
// CPresenceGrantRequestListEventImp::ConstructL()
// ---------------------------------------------------------------------------
//
void CPresenceGrantRequestListEventImp::ConstructL(
        RPrGrntReqInfoImpArray* aNew,
        RPrGrntReqInfoImpArray* aPending,
        RPrGrntReqInfoImpArray* aObsoleted,
        CXIMPDataSubscriptionStateImp* aState )
    {
    iNewRequests = aNew;
    if( aPending )
        {
        iPending = aPending;
        }
    else
        {
        iPending = new ( ELeave ) RPrGrntReqInfoImpArray;
        iOwnsPending = ETrue;
        }
    iObsoleted = aObsoleted;
    iSubscriptionState = aState;
    }

// ---------------------------------------------------------------------------
// CPresenceGrantRequestListEventImp::ConstructL()
// ---------------------------------------------------------------------------
//
void CPresenceGrantRequestListEventImp::ConstructL( RReadStream& aStream )
    {
    iConstructedFromStream = ETrue;

    iNewRequests = new ( ELeave ) RPrGrntReqInfoImpArray;
    iPending = new ( ELeave ) RPrGrntReqInfoImpArray;
    iObsoleted = new ( ELeave ) RPrGrntReqInfoImpArray;
    iSubscriptionState = CXIMPDataSubscriptionStateImp::NewL();

    FillArrayFromStreamL( *iNewRequests, aStream );
    FillArrayFromStreamL( *iPending, aStream );
    FillArrayFromStreamL( *iObsoleted, aStream );
    iSubscriptionState->InternalizeL( aStream );
    }

// ---------------------------------------------------------------------------
// Implement supported interface access.
// ---------------------------------------------------------------------------
//
XIMPIMP_IF_BASE_GET_INTERFACE_BEGIN( CPresenceGrantRequestListEventImp,
                                     MPresenceGrantRequestListEvent )
    XIMPIMP_IF_BASE_GET_CLASS( CXIMPApiEventBase, this )
XIMPIMP_IF_BASE_GET_INTERFACE_END()


XIMPIMP_IF_BASE_GET_CONST_INTERFACE_BEGIN( CPresenceGrantRequestListEventImp,
                                           MPresenceGrantRequestListEvent )
    XIMPIMP_IF_BASE_GET_CONST_CLASS( CXIMPApiEventBase, this )                                            
XIMPIMP_IF_BASE_GET_INTERFACE_END()

// ---------------------------------------------------------------------------
// Implement methods from base event interface and base event class
// ---------------------------------------------------------------------------
//
XIMPIMP_IMPLEMENT_EVENT_BASE_METHODS( CPresenceGrantRequestListEventImp,
                                      MPresenceGrantRequestListEvent )

// ---------------------------------------------------------------------------
// CPresenceGrantRequestListEventImp::EqualsContent()
// ---------------------------------------------------------------------------
//
TBool CPresenceGrantRequestListEventImp::EqualsContent( 
    const CXIMPApiEventBase& aOtherInstance ) const
    {
    const CPresenceGrantRequestListEventImp* tmp =
    TXIMPGetImpClassOrPanic< const CPresenceGrantRequestListEventImp >::From( 
                                                        aOtherInstance.Base() );
    
    TBool same = ETrue;
    TBool x = TXIMPEqualsContent< CPresenceGrantRequestInfoImp >::Array( *iNewRequests, *( tmp->iNewRequests ) );
    same &= x;

    x = TXIMPEqualsContent< CPresenceGrantRequestInfoImp >::Array( *iPending, *( tmp->iPending ) );
    same &= x;

    x = TXIMPEqualsContent< CPresenceGrantRequestInfoImp >::Array( *iObsoleted, *( tmp->iObsoleted ) );
    same &= x;

    x = iSubscriptionState->EqualsContent( *tmp->iSubscriptionState );
    same &= x;

    return same;
    }

// ---------------------------------------------------------------------------
// From class CXIMPApiEventBase.
// CPresenceGrantRequestListEventImp::ExternalizeL()
// ---------------------------------------------------------------------------
//
void CPresenceGrantRequestListEventImp::ExternalizeL( RWriteStream& aStream ) const
    {
    WriteArrayToStreamL( *iNewRequests, aStream );
    WriteArrayToStreamL( *iPending, aStream );
    WriteArrayToStreamL( *iObsoleted, aStream );
    iSubscriptionState->ExternalizeL( aStream );
    }



// ---------------------------------------------------------------------------
// From MPresenceGrantRequestListEvent class.
// CPresenceGrantRequestListEventImp::NewRequestsCount()
// ---------------------------------------------------------------------------
//
TInt CPresenceGrantRequestListEventImp::NewRequestsCount() const
    {
    return iNewRequests->Count();
    }

// ---------------------------------------------------------------------------
// From MPresenceGrantRequestListEvent class.
// CPresenceGrantRequestListEventImp::NewRequest()
// ---------------------------------------------------------------------------
//
const MPresenceGrantRequestInfo&
            CPresenceGrantRequestListEventImp::NewRequest( TInt aIndex ) const
    {
    return *(*iNewRequests)[ aIndex ];
    }
    
    
// ---------------------------------------------------------------------------
// From MPresenceGrantRequestListEvent class.
// CPresenceGrantRequestListEventImp::DisappearedRequestsCount()
// ---------------------------------------------------------------------------
//
TInt CPresenceGrantRequestListEventImp::DisappearedRequestsCount() const
    {
    return iObsoleted->Count();
    }

// ---------------------------------------------------------------------------
// From MPresenceGrantRequestListEvent class.
// CPresenceGrantRequestListEventImp::DisappearedRequest()
// ---------------------------------------------------------------------------
//
const MPresenceGrantRequestInfo&
    CPresenceGrantRequestListEventImp::DisappearedRequest( TInt aIndex ) const
    {
    return *(*iObsoleted)[ aIndex ];
    }
    

// ---------------------------------------------------------------------------
// From MPresenceGrantRequestListEvent class.
// CPresenceGrantRequestListEventImp::CurrentRequestsCount()
// ---------------------------------------------------------------------------
//
TInt CPresenceGrantRequestListEventImp::CurrentRequestsCount() const
    {
    return iPending->Count();
    }

// ---------------------------------------------------------------------------
// From MPresenceGrantRequestListEvent class.
// CPresenceGrantRequestListEventImp::CurrentRequest()
// ---------------------------------------------------------------------------
//
const MPresenceGrantRequestInfo&
    CPresenceGrantRequestListEventImp::CurrentRequest( TInt aIndex ) const
    {
    return *(*iPending)[ aIndex ];
    }





// ---------------------------------------------------------------------------
// From class MPresenceGrantRequestListEvent.
// CPresenceGrantRequestListEventImp::DataSubscriptionState()
// ---------------------------------------------------------------------------
//
const MXIMPDataSubscriptionState&
    CPresenceGrantRequestListEventImp::DataSubscriptionState() const
    {
    return *iSubscriptionState;
    }


// ---------------------------------------------------------------------------
// CPresenceGrantRequestListEventImp::WriteArrayToStreamL()
// ---------------------------------------------------------------------------
//
void CPresenceGrantRequestListEventImp::WriteArrayToStreamL(
        RPrGrntReqInfoImpArray& aArray, RWriteStream& aStream ) const
    {
    TInt count = aArray.Count();
    aStream.WriteInt32L( count );
        {
        for( TInt a = 0; a < count; ++a )
            {
            aArray[ a ]->ExternalizeL( aStream );
            }
        }
    }


// ---------------------------------------------------------------------------
// CPresenceGrantRequestListEventImp::FillArrayFromStreamL
// ---------------------------------------------------------------------------
//
void CPresenceGrantRequestListEventImp::FillArrayFromStreamL(
        RPrGrntReqInfoImpArray& aArray,
        RReadStream& aStream )
    {
    TInt count = aStream.ReadInt32L();
    for( TInt a = 0; a < count; ++a )
        {
        // the stream MUST be in order (since it was externalized by this class)
        CPresenceGrantRequestInfoImp* newInfo = CPresenceGrantRequestInfoImp::NewLC();
        newInfo->InternalizeL( aStream );
        aArray.AppendL( newInfo );
        CleanupStack::Pop(); // newInfo
        }
    }

// End of file


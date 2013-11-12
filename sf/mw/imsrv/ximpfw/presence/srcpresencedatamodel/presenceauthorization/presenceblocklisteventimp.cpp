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
* Description:  Presence block list event implementation.
 *
*/

#include "presenceblocklisteventimp.h"
#include "presenceblockinfoimp.h"
#include "ximpobjecthelpers.h"
#include "ximpidentityimp.h"
#include "ximpdatasubscriptionstateimp.h"
#include <s32strm.h>


// ---------------------------------------------------------------------------
// CPresenceBlockListEventImp::NewLC()
// ---------------------------------------------------------------------------
//
EXPORT_C CPresenceBlockListEventImp* CPresenceBlockListEventImp::NewLC(
        RPrBlockInfoImpArray* aAdded,
        RPrBlockInfoImpArray* aPending,
        RPrBlockInfoImpArray* aRemoved,
        RPrBlockInfoImpArray* aUpdated,
        CXIMPDataSubscriptionStateImp* aState
        )
    {
    CPresenceBlockListEventImp* self = new( ELeave ) CPresenceBlockListEventImp();
    CleanupStack::PushL( self );
    self->ConstructL( aAdded, aPending, aRemoved, aUpdated, aState );
    return self;
    }

// ---------------------------------------------------------------------------
// CPresenceBlockListEventImp::NewFromStreamLC()
// ---------------------------------------------------------------------------
//
CXIMPApiEventBase* CPresenceBlockListEventImp::NewFromStreamLC( RReadStream& aStream )
    {
    CPresenceBlockListEventImp* self = new( ELeave ) CPresenceBlockListEventImp;
    CleanupStack::PushL( self );
    self->ConstructL( aStream );
    return self;
    }

// ---------------------------------------------------------------------------
// CPresenceBlockListEventImp::~CPresenceBlockListEventImp()
// ---------------------------------------------------------------------------
//
CPresenceBlockListEventImp::~CPresenceBlockListEventImp()
    {
    if( iConstructedFromStream )
        {
        delete iAdded;
        delete iCurrent;
        delete iRemoved;
        delete iUpdated;
        iCurrent = NULL; // Must be null for next branch.
        }

    if( iOwnsCurrent )
        {
        delete iCurrent;
        }

    delete iSubscriptionState;
    }


// ---------------------------------------------------------------------------
// CPresenceBlockListEventImp::CPresenceBlockListEventImp()
// ---------------------------------------------------------------------------
//
CPresenceBlockListEventImp::CPresenceBlockListEventImp()
    {
    }


// ---------------------------------------------------------------------------
// CPresenceBlockListEventImp::ConstructL()
// ---------------------------------------------------------------------------
//
void CPresenceBlockListEventImp::ConstructL(
        RPrBlockInfoImpArray* aAdded,
        RPrBlockInfoImpArray* aPending,
        RPrBlockInfoImpArray* aRemoved,
        RPrBlockInfoImpArray* aUpdated,
        CXIMPDataSubscriptionStateImp* aState )
    {
    iAdded = aAdded;
    if( aPending )
        {
        iCurrent = aPending;
        }
    else
        {
        iCurrent = new ( ELeave ) RPrBlockInfoImpArray;
        iOwnsCurrent = ETrue;
        }
    iRemoved = aRemoved;
    iUpdated = aUpdated;
    iSubscriptionState = aState;
    }

// ---------------------------------------------------------------------------
// CPresenceBlockListEventImp::ConstructL()
// ---------------------------------------------------------------------------
//
void CPresenceBlockListEventImp::ConstructL( RReadStream& aStream )
    {
    iConstructedFromStream = ETrue;

    iAdded = new ( ELeave ) RPrBlockInfoImpArray;
    iCurrent = new ( ELeave ) RPrBlockInfoImpArray;
    iRemoved = new ( ELeave ) RPrBlockInfoImpArray;
    iUpdated = new ( ELeave ) RPrBlockInfoImpArray;
    iSubscriptionState = CXIMPDataSubscriptionStateImp::NewL();

    FillArrayFromStreamL( *iAdded, aStream );
    FillArrayFromStreamL( *iCurrent, aStream );
    FillArrayFromStreamL( *iRemoved, aStream );
    FillArrayFromStreamL( *iUpdated, aStream );
    
    iSubscriptionState->InternalizeL( aStream );
    }

// ---------------------------------------------------------------------------
// Implement supported interface access.
// ---------------------------------------------------------------------------
//
XIMPIMP_IF_BASE_GET_INTERFACE_BEGIN( CPresenceBlockListEventImp,
                                     MPresenceBlockListEvent )
    XIMPIMP_IF_BASE_GET_CLASS( CXIMPApiEventBase, this )
XIMPIMP_IF_BASE_GET_INTERFACE_END()


XIMPIMP_IF_BASE_GET_CONST_INTERFACE_BEGIN( CPresenceBlockListEventImp,
                                           MPresenceBlockListEvent )
    XIMPIMP_IF_BASE_GET_CONST_CLASS( CXIMPApiEventBase, this )                                            
XIMPIMP_IF_BASE_GET_INTERFACE_END()



// ---------------------------------------------------------------------------
// Implement methods from base event interface and base event class
// ---------------------------------------------------------------------------
//
XIMPIMP_IMPLEMENT_EVENT_BASE_METHODS( CPresenceBlockListEventImp,
                                      MPresenceBlockListEvent )


// ---------------------------------------------------------------------------
// CPresenceBlockListEventImp::EqualsContent()
// ---------------------------------------------------------------------------
//
TBool CPresenceBlockListEventImp::EqualsContent( 
    const CXIMPApiEventBase& aOtherInstance ) const
    {
    const CPresenceBlockListEventImp* tmp =
    TXIMPGetImpClassOrPanic< const CPresenceBlockListEventImp>::From( 
                                                       aOtherInstance.Base() );
    TBool same = ETrue;
    TBool x = TXIMPEqualsContent< CPresenceBlockInfoImp >::Array( *iAdded, *( tmp->iAdded ) );
    same &= x;

    x = TXIMPEqualsContent< CPresenceBlockInfoImp >::Array( *iCurrent, *( tmp->iCurrent ) );
    same &= x;

    x = TXIMPEqualsContent< CPresenceBlockInfoImp >::Array( *iRemoved, *( tmp->iRemoved ) );
    same &= x;

    x = TXIMPEqualsContent< CPresenceBlockInfoImp >::Array( *iUpdated, *( tmp->iUpdated ) );
    same &= x;
    
    x = iSubscriptionState->EqualsContent( *tmp->iSubscriptionState );
    same &= x;

    return same;
    }

// ---------------------------------------------------------------------------
// From class CXIMPApiEventBase.
// CPresenceBlockListEventImp::ExternalizeL()
// ---------------------------------------------------------------------------
//
void CPresenceBlockListEventImp::ExternalizeL( RWriteStream& aStream ) const
    {
    WriteArrayToStreamL( *iAdded, aStream );
    WriteArrayToStreamL( *iCurrent, aStream );
    WriteArrayToStreamL( *iRemoved, aStream );
    WriteArrayToStreamL( *iUpdated, aStream );
    iSubscriptionState->ExternalizeL( aStream );
    }


// ---------------------------------------------------------------------------
// From MPresenceBlockListEvent class.
// CPresenceBlockListEventImp::NewBlocksCount()
// ---------------------------------------------------------------------------
//
TInt CPresenceBlockListEventImp::NewBlocksCount() const
    {
    return iAdded->Count();
    }
    

// ---------------------------------------------------------------------------
// From MPresenceBlockListEvent class.
// CPresenceBlockListEventImp::NewBlock()
// ---------------------------------------------------------------------------
//
const MPresenceBlockInfo&
            CPresenceBlockListEventImp::NewBlock( TInt aIndex ) const
    {
    return *(*iAdded)[ aIndex ];
    }
    
// ---------------------------------------------------------------------------
// From MPresenceBlockListEvent class.
// CPresenceBlockListEventImp::UpdatedBlocksCount()
// ---------------------------------------------------------------------------
//
TInt CPresenceBlockListEventImp::UpdatedBlocksCount() const
    {
    return iUpdated->Count();
    }


// ---------------------------------------------------------------------------
// From MPresenceBlockListEvent class.
// CPresenceBlockListEventImp::UpdatedBlock()
// ---------------------------------------------------------------------------
//
const MPresenceBlockInfo& 
    CPresenceBlockListEventImp::UpdatedBlock( TInt aIndex ) const
    {
    return *(*iUpdated)[ aIndex ];
    }
    

// ---------------------------------------------------------------------------
// From MPresenceBlockListEvent class.
// CPresenceBlockListEventImp::DisappearedBlocksCount()
// ---------------------------------------------------------------------------
//
TInt CPresenceBlockListEventImp::DisappearedBlocksCount() const
    {
    return iRemoved->Count();
    }
    

// ---------------------------------------------------------------------------
// From MPresenceBlockListEvent class.
// CPresenceBlockListEventImp::DisappearedBlock()
// ---------------------------------------------------------------------------
//
const MPresenceBlockInfo& 
    CPresenceBlockListEventImp::DisappearedBlock( TInt aIndex ) const
    {
    return *(*iRemoved)[ aIndex ];
    }
    

// ---------------------------------------------------------------------------
// From MPresenceBlockListEvent class.
// CPresenceBlockListEventImp::CurrentBlocksCount()
// ---------------------------------------------------------------------------
//
TInt CPresenceBlockListEventImp::CurrentBlocksCount() const
    {
    return iCurrent->Count();
    }

// ---------------------------------------------------------------------------
// From MPresenceBlockListEvent class.
// CPresenceBlockListEventImp::CurrentBlock()
// ---------------------------------------------------------------------------
//
const MPresenceBlockInfo&
    CPresenceBlockListEventImp::CurrentBlock( TInt aIndex ) const
    {
    return *(*iCurrent)[ aIndex ];
    }


// ---------------------------------------------------------------------------
// From class MPresenceBlockListEvent.
// CPresenceBlockListEventImp::DataSubscriptionState()
// ---------------------------------------------------------------------------
//
const MXIMPDataSubscriptionState&
    CPresenceBlockListEventImp::DataSubscriptionState() const
    {
    return *iSubscriptionState;
    }


// ---------------------------------------------------------------------------
// CPresenceBlockListEventImp::WriteArrayToStreamL()
// ---------------------------------------------------------------------------
//
void CPresenceBlockListEventImp::WriteArrayToStreamL(
        RPrBlockInfoImpArray& aArray, RWriteStream& aStream ) const
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
// CPresenceBlockListEventImp::FillArrayFromStreamL
// ---------------------------------------------------------------------------
//
void CPresenceBlockListEventImp::FillArrayFromStreamL(
        RPrBlockInfoImpArray& aArray,
        RReadStream& aStream )
    {
    TInt count = aStream.ReadInt32L();
    for( TInt a = 0; a < count; ++a )
        {
        // the stream MUST be in order (since it was externalized by this class)
        CPresenceBlockInfoImp* newInfo = CPresenceBlockInfoImp::NewLC();
        newInfo->InternalizeL( aStream );
        aArray.AppendL( newInfo );
        CleanupStack::Pop(); // newInfo
        }
    }



// End of file


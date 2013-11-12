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

#include "presencewatcherlisteventimp.h"
#include "presencewatcherinfoimp.h"
#include "ximpobjecthelpers.h"
#include "ximpidentityimp.h"
#include "ximpdatasubscriptionstateimp.h"
#include <s32strm.h>


// ---------------------------------------------------------------------------
// CPresenceWatcherListEventImp::NewLC()
// ---------------------------------------------------------------------------
//
EXPORT_C CPresenceWatcherListEventImp* CPresenceWatcherListEventImp::NewLC(
        RPrWatLstInfoImpArray* aNew,
        RPrWatLstInfoImpArray* aCurrent,
        RPrWatLstInfoImpArray* aDisappeared,
        CXIMPDataSubscriptionStateImp* aState
        )
    {
    CPresenceWatcherListEventImp* self = new( ELeave ) CPresenceWatcherListEventImp();
    CleanupStack::PushL( self );
    self->ConstructL( aNew, aCurrent, aDisappeared, aState );
    return self;
    }

// ---------------------------------------------------------------------------
// CPresenceWatcherListEventImp::NewFromStreamLC()
// ---------------------------------------------------------------------------
//
CXIMPApiEventBase* CPresenceWatcherListEventImp::NewFromStreamLC( RReadStream& aStream )
    {
    CPresenceWatcherListEventImp* self = new( ELeave ) CPresenceWatcherListEventImp;
    CleanupStack::PushL( self );
    self->ConstructL( aStream );
    return self;
    }

// ---------------------------------------------------------------------------
// CPresenceWatcherListEventImp::~CPresenceWatcherListEventImp()
// ---------------------------------------------------------------------------
//
CPresenceWatcherListEventImp::~CPresenceWatcherListEventImp()
    {
    if( iConstructedFromStream )
        {
        delete iNewWatchers;
        delete iCurrent;
        delete iDisappeared;
        iCurrent = NULL;
        }
    if( iOwnsCurrent )
        {
        delete iCurrent;
        }

    delete iSubscriptionState;
    }


// ---------------------------------------------------------------------------
// CPresenceWatcherListEventImp::CPresenceWatcherListEventImp()
// ---------------------------------------------------------------------------
//
CPresenceWatcherListEventImp::CPresenceWatcherListEventImp()
    {
    }


// ---------------------------------------------------------------------------
// CPresenceWatcherListEventImp::ConstructL()
// ---------------------------------------------------------------------------
//
void CPresenceWatcherListEventImp::ConstructL(
        RPrWatLstInfoImpArray* aNew,
        RPrWatLstInfoImpArray* aCurrent,
        RPrWatLstInfoImpArray* aDisappeared,
        CXIMPDataSubscriptionStateImp* aState )
    {
    iNewWatchers = aNew;
    if( iCurrent )
        {
        iCurrent = aCurrent;
        }
    else
        {
        iCurrent = new ( ELeave ) RPrWatLstInfoImpArray;
        iOwnsCurrent = ETrue;
        }
    iDisappeared = aDisappeared;
    iSubscriptionState = aState;
    }

// ---------------------------------------------------------------------------
// CPresenceWatcherListEventImp::ConstructL()
// ---------------------------------------------------------------------------
//
void CPresenceWatcherListEventImp::ConstructL( RReadStream& aStream )
    {
    iConstructedFromStream = ETrue;

    iNewWatchers = new ( ELeave ) RPrWatLstInfoImpArray;
    iCurrent = new ( ELeave ) RPrWatLstInfoImpArray;
    iDisappeared = new ( ELeave ) RPrWatLstInfoImpArray;
    iSubscriptionState = CXIMPDataSubscriptionStateImp::NewL();

    FillArrayFromStreamL( *iNewWatchers, aStream );
    FillArrayFromStreamL( *iCurrent, aStream );
    FillArrayFromStreamL( *iDisappeared, aStream );
    
    iSubscriptionState->InternalizeL( aStream );
    }

// ---------------------------------------------------------------------------
// Implement supported interface access.
// ---------------------------------------------------------------------------
//
XIMPIMP_IF_BASE_GET_INTERFACE_BEGIN( CPresenceWatcherListEventImp,
                                     MPresenceWatcherListEvent )
    XIMPIMP_IF_BASE_GET_CLASS( CXIMPApiEventBase, this )
XIMPIMP_IF_BASE_GET_INTERFACE_END()

XIMPIMP_IF_BASE_GET_CONST_INTERFACE_BEGIN( CPresenceWatcherListEventImp,
                                           MPresenceWatcherListEvent )
    XIMPIMP_IF_BASE_GET_CONST_CLASS( CXIMPApiEventBase, this )                                            
XIMPIMP_IF_BASE_GET_INTERFACE_END()



// ---------------------------------------------------------------------------
// Implement methods from base event interface and base event class
// ---------------------------------------------------------------------------
//
XIMPIMP_IMPLEMENT_EVENT_BASE_METHODS( CPresenceWatcherListEventImp,
                                      MPresenceWatcherListEvent )

// ---------------------------------------------------------------------------
// CPresenceWatcherListEventImp::EqualsContent()
// ---------------------------------------------------------------------------
//
TBool CPresenceWatcherListEventImp::EqualsContent( 
    const CXIMPApiEventBase& aOtherInstance ) const
    {
    const CPresenceWatcherListEventImp* tmp =
    TXIMPGetImpClassOrPanic< const CPresenceWatcherListEventImp >::From( 
                                                        aOtherInstance.Base() );
    TBool same = ETrue;
    TBool x = TXIMPEqualsContent< CPresenceWatcherInfoImp >::Array( *iNewWatchers, *( tmp->iNewWatchers ) );
    same &= x;

    x = TXIMPEqualsContent< CPresenceWatcherInfoImp >::Array( *iCurrent, *( tmp->iCurrent ) );
    same &= x;

    x = TXIMPEqualsContent< CPresenceWatcherInfoImp >::Array( *iDisappeared, *( tmp->iDisappeared ) );
    same &= x;

    x = iSubscriptionState->EqualsContent( *tmp->iSubscriptionState );
    same &= x;

    return same;
    }

// ---------------------------------------------------------------------------
// From class CXIMPApiEventBase.
// CPresenceWatcherListEventImp::ExternalizeL()
// ---------------------------------------------------------------------------
//
void CPresenceWatcherListEventImp::ExternalizeL( RWriteStream& aStream ) const
    {
    WriteArrayToStreamL( *iNewWatchers, aStream );
    WriteArrayToStreamL( *iCurrent, aStream );
    WriteArrayToStreamL( *iDisappeared, aStream );
    iSubscriptionState->ExternalizeL( aStream );
    }


// ---------------------------------------------------------------------------
// From MPresenceWatcherListEvent class.
// CPresenceWatcherListEventImp::NewWatchersCount()
// ---------------------------------------------------------------------------
//
TInt CPresenceWatcherListEventImp::NewWatchersCount() const
    {
    return iNewWatchers->Count();
    }

// ---------------------------------------------------------------------------
// From MPresenceWatcherListEvent class.
// CPresenceWatcherListEventImp::NewWatcher()
// ---------------------------------------------------------------------------
//
const MPresenceWatcherInfo&
            CPresenceWatcherListEventImp::NewWatcher( TInt aIndex ) const
    {
    return *(*iNewWatchers)[ aIndex ];
    }

// ---------------------------------------------------------------------------
// From MPresenceWatcherListEvent class.
// CPresenceWatcherListEventImp::DisappearedWatchersCount()
// ---------------------------------------------------------------------------
//
TInt CPresenceWatcherListEventImp::DisappearedWatchersCount() const
    {
    return iDisappeared->Count();
    }

// ---------------------------------------------------------------------------
// From MPresenceWatcherListEvent class.
// CPresenceWatcherListEventImp::DisappearedWatcher()
// ---------------------------------------------------------------------------
//
const MPresenceWatcherInfo&
            CPresenceWatcherListEventImp::DisappearedWatcher( TInt aIndex ) const
    {
    return *(*iDisappeared)[ aIndex ];
    }


// ---------------------------------------------------------------------------
// From MPresenceWatcherListEvent class.
// CPresenceWatcherListEventImp::CurrentWatchersCount()
// ---------------------------------------------------------------------------
//
TInt CPresenceWatcherListEventImp::CurrentWatchersCount() const
    {
    return iCurrent->Count();
    }

// ---------------------------------------------------------------------------
// From MPresenceWatcherListEvent class.
// CPresenceWatcherListEventImp::CurrentWatcher()
// ---------------------------------------------------------------------------
//
const MPresenceWatcherInfo&
            CPresenceWatcherListEventImp::CurrentWatcher( TInt aIndex ) const
    {
    return *(*iCurrent)[ aIndex ];
    }






// ---------------------------------------------------------------------------
// From class MPresenceWatcherListEvent.
// CPresenceWatcherListEventImp::DataSubscriptionState()
// ---------------------------------------------------------------------------
//
const MXIMPDataSubscriptionState&
    CPresenceWatcherListEventImp::DataSubscriptionState() const
    {
    return *iSubscriptionState;
    }


// ---------------------------------------------------------------------------
// CPresenceWatcherListEventImp::WriteArrayToStreamL()
// ---------------------------------------------------------------------------
//
void CPresenceWatcherListEventImp::WriteArrayToStreamL(
        RPrWatLstInfoImpArray& aArray, RWriteStream& aStream ) const
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
// CPresenceWatcherListEventImp::FillArrayFromStreamL
// ---------------------------------------------------------------------------
//
void CPresenceWatcherListEventImp::FillArrayFromStreamL(
        RPrWatLstInfoImpArray& aArray,
        RReadStream& aStream )
    {
    TInt count = aStream.ReadInt32L();
    for( TInt a = 0; a < count; ++a )
        {
        // the stream MUST be in order (since it was externalized by this class)
        CPresenceWatcherInfoImp* newInfo = CPresenceWatcherInfoImp::NewLC();
        newInfo->InternalizeL( aStream );
        aArray.AppendL( newInfo );
        CleanupStack::Pop(); // newInfo
        }
    }


// End of file


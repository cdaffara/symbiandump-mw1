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

#include "presentitygrouplisteventimp.h"
#include "presentitygroupinfoimp.h"
#include "ximpobjecthelpers.h"
#include "ximpdatasubscriptionstateimp.h"
#include "ximpidentityimp.h"
#include <s32strm.h>


// ---------------------------------------------------------------------------
// CPresentityGroupListEventImp::NewLC()
// ---------------------------------------------------------------------------
//
EXPORT_C CPresentityGroupListEventImp* CPresentityGroupListEventImp::NewLC(
        RPrGrpInfoImpArray* aCreated,
        RPrGrpInfoImpArray* aUpdated,
        RPrGrpInfoImpArray* aDeleted,
        CXIMPDataSubscriptionStateImp* aState )
    {
    CPresentityGroupListEventImp* self = new( ELeave ) CPresentityGroupListEventImp();
    CleanupStack::PushL( self );
    self->ConstructL( aCreated, aUpdated, aDeleted, aState );
    return self;
    }

// ---------------------------------------------------------------------------
// CPresentityGroupListEventImp::NewLC()
// ---------------------------------------------------------------------------
//
EXPORT_C CPresentityGroupListEventImp* CPresentityGroupListEventImp::NewLC(
        RPrGrpInfoImpArray* aCreated,
        RPrGrpInfoImpArray* aUpdated,
        RPrGrpInfoImpArray* aDeleted,
        RPrGrpInfoImpArray* aCurrent,
        CXIMPDataSubscriptionStateImp* aState
        )
    {
    CPresentityGroupListEventImp* self = new( ELeave ) CPresentityGroupListEventImp();
    CleanupStack::PushL( self );
    if( aCurrent )
        {
        self->ConstructL( aCreated, aUpdated, aDeleted, aCurrent, aState );
        }
    else
        {
        self->ConstructL( aCreated, aUpdated, aDeleted, aState );
        }
    return self;
    }
    
// ---------------------------------------------------------------------------
// CPresentityGroupListEventImp::NewLC()
// ---------------------------------------------------------------------------
//
EXPORT_C CPresentityGroupListEventImp* CPresentityGroupListEventImp::NewL(
        RPrGrpInfoImpArray* aCreated,
        RPrGrpInfoImpArray* aUpdated,
        RPrGrpInfoImpArray* aDeleted,
        RPrGrpInfoImpArray* aCurrent,
        CXIMPDataSubscriptionStateImp* aState
        )
    {
    CPresentityGroupListEventImp* self = NewLC( aCreated, aUpdated, aDeleted, aCurrent, aState );
    CleanupStack::Pop( self );
    return self;
    }
    
// ---------------------------------------------------------------------------
// CPresentityGroupListEventImp::NewFromStreamLC()
// ---------------------------------------------------------------------------
//
CXIMPApiEventBase* CPresentityGroupListEventImp::NewFromStreamLC( RReadStream& aStream )
    {
    CPresentityGroupListEventImp* self = new( ELeave ) CPresentityGroupListEventImp;
    CleanupStack::PushL( self );
    self->ConstructL( aStream );
    return self;
    }

// ---------------------------------------------------------------------------
// CPresentityGroupListEventImp::~CPresentityGroupListEventImp()
// ---------------------------------------------------------------------------
//
CPresentityGroupListEventImp::~CPresentityGroupListEventImp()
    {
    if( iConstructedFromStream )
        {
        delete iCreatedGroups;
        delete iUpdatedGroups;
        delete iDeletedGroups;
        delete iCurrentGroups;
        iCurrentGroups = NULL;
        }

    if ( iOwnsCurrentGroupsArray )
        {
        delete iCurrentGroups;
        }

    delete iSubscriptionState;
    }


// ---------------------------------------------------------------------------
// CPresentityGroupListEventImp::CPresentityGroupListEventImp()
// ---------------------------------------------------------------------------
//
CPresentityGroupListEventImp::CPresentityGroupListEventImp()
    {
    }


// ---------------------------------------------------------------------------
// CPresentityGroupListEventImp::ConstructL()
// ---------------------------------------------------------------------------
//
void CPresentityGroupListEventImp::ConstructL(
        RPrGrpInfoImpArray* aCreated,
        RPrGrpInfoImpArray* aUpdated,
        RPrGrpInfoImpArray* aDeleted,
        CXIMPDataSubscriptionStateImp* aState )
    {
    iCreatedGroups = aCreated;
    iUpdatedGroups = aUpdated;
    iDeletedGroups = aDeleted;

    iCurrentGroups = new ( ELeave ) RPrGrpInfoImpArray;
    iOwnsCurrentGroupsArray = ETrue;
    iSubscriptionState = aState;
    }

// ---------------------------------------------------------------------------
// CPresentityGroupListEventImp::ConstructL()
// ---------------------------------------------------------------------------
//
void CPresentityGroupListEventImp::ConstructL(
        RPrGrpInfoImpArray* aCreated,
        RPrGrpInfoImpArray* aUpdated,
        RPrGrpInfoImpArray* aDeleted,
        RPrGrpInfoImpArray* aCurrent,
        CXIMPDataSubscriptionStateImp* aState
        )
    {
    iCreatedGroups = aCreated;
    iUpdatedGroups = aUpdated;
    iDeletedGroups = aDeleted;
    iCurrentGroups = aCurrent;
    iSubscriptionState = aState;
    }

// ---------------------------------------------------------------------------
// CPresentityGroupListEventImp::ConstructL()
// ---------------------------------------------------------------------------
//
void CPresentityGroupListEventImp::ConstructL( RReadStream& aStream )
    {
    iConstructedFromStream = ETrue;

    iCreatedGroups = new ( ELeave ) RPrGrpInfoImpArray;
    iUpdatedGroups = new ( ELeave ) RPrGrpInfoImpArray;
    iDeletedGroups = new ( ELeave ) RPrGrpInfoImpArray;
    iCurrentGroups = new ( ELeave ) RPrGrpInfoImpArray;

    FillArrayFromStreamL( *iCreatedGroups, aStream );
    FillArrayFromStreamL( *iUpdatedGroups, aStream );
    FillArrayFromStreamL( *iDeletedGroups, aStream );
    FillArrayFromStreamL( *iCurrentGroups, aStream );

    iSubscriptionState = CXIMPDataSubscriptionStateImp::NewL();
    iSubscriptionState->InternalizeL( aStream );
    }

// ---------------------------------------------------------------------------
// Implement supported interface access.
// ---------------------------------------------------------------------------
//
XIMPIMP_IF_BASE_GET_INTERFACE_BEGIN( CPresentityGroupListEventImp,
                                     MPresentityGroupListEvent )
    XIMPIMP_IF_BASE_GET_CLASS( CXIMPApiEventBase, this )
XIMPIMP_IF_BASE_GET_INTERFACE_END()

XIMPIMP_IF_BASE_GET_CONST_INTERFACE_BEGIN( CPresentityGroupListEventImp,
                                           MPresentityGroupListEvent )
    XIMPIMP_IF_BASE_GET_CONST_CLASS( CXIMPApiEventBase, this )                                            
XIMPIMP_IF_BASE_GET_INTERFACE_END()


// ---------------------------------------------------------------------------
// Implement methods from base event interface and base event class
// ---------------------------------------------------------------------------
//
XIMPIMP_IMPLEMENT_EVENT_BASE_METHODS( CPresentityGroupListEventImp,
                                      MPresentityGroupListEvent )


// ---------------------------------------------------------------------------
// CPresentityGroupListEventImp::EqualsContent()
// ---------------------------------------------------------------------------
//
TBool CPresentityGroupListEventImp::EqualsContent( 
    const CXIMPApiEventBase& aOtherInstance ) const
    {
    const CPresentityGroupListEventImp* tmp =
    TXIMPGetImpClassOrPanic< const CPresentityGroupListEventImp >::From( 
                                                        aOtherInstance.Base() );
    
    TBool same = ETrue;

    TBool x;
    x = CompareArrays( iCreatedGroups, tmp->iCreatedGroups );
    same &= x;

    x = CompareArrays( iUpdatedGroups, tmp->iUpdatedGroups );
    same &= x;

    x = CompareArrays( iDeletedGroups, tmp->iDeletedGroups );
    same &= x;
    
    x = CompareArrays( iCurrentGroups, tmp->iCurrentGroups );
    same &= x;

    x = iSubscriptionState->EqualsContent( *tmp->iSubscriptionState );
    same &= x;

    return same;
    }

// ---------------------------------------------------------------------------
// CPresentityGroupListEventImp::CompareArrays
// ---------------------------------------------------------------------------
//
TBool CPresentityGroupListEventImp::CompareArrays(
        RPrGrpInfoImpArray* aA, RPrGrpInfoImpArray* aB )
    {
    TBool same = ETrue;

    TBool x;
    x = aA->Count() == aB->Count();   // counts must match
    same &= x;

    if ( ! same )
        {
        // don't bother checking further if counts mismatch
        return same;
        }

    // now check whole array, arrays have equal length
    for ( TInt i = 0; i < aA->Count(); i++ )
        {
        x = 0 == ( (*aA)[ i ]->GroupId().Identity().Compare(
                    (*aB)[ i ]->GroupId().Identity() ) );
        same &= x;

        x = 0 == ( (*aA)[ i ]->GroupDisplayName().Compare(
                    (*aB)[ i ]->GroupDisplayName() ) );
        same &= x;
        }

    return same;
    }

// ---------------------------------------------------------------------------
// From class CXIMPApiEventBase.
// CPresentityGroupListEventImp::ExternalizeL()
// ---------------------------------------------------------------------------
//
void CPresentityGroupListEventImp::ExternalizeL( RWriteStream& aStream ) const
    {
    WriteArrayToStreamL( *iCreatedGroups, aStream );
    WriteArrayToStreamL( *iUpdatedGroups, aStream );
    WriteArrayToStreamL( *iDeletedGroups, aStream );
    WriteArrayToStreamL( *iCurrentGroups, aStream );

    iSubscriptionState->ExternalizeL( aStream );
    }


// ---------------------------------------------------------------------------
// From class MPresentityGroupListEvent.
// CPresentityGroupListEventImp::NewGroupsCount()
// ---------------------------------------------------------------------------
//
TInt CPresentityGroupListEventImp::NewGroupsCount() const
    {
    return iCreatedGroups->Count();
    }

// ---------------------------------------------------------------------------
// From class MPresentityGroupListEvent.
// CPresentityGroupListEventImp::NewGroup()
// ---------------------------------------------------------------------------
//
const MPresentityGroupInfo&
            CPresentityGroupListEventImp::NewGroup( TInt aIndex ) const
    {
    return *(*iCreatedGroups)[ aIndex ];
    }

// ---------------------------------------------------------------------------
// From class MPresentityGroupListEvent.
// CPresentityGroupListEventImp::UpdatedGroupsCount()
// ---------------------------------------------------------------------------
//
TInt CPresentityGroupListEventImp::UpdatedGroupsCount() const
    {
    return iUpdatedGroups->Count();
    }

// ---------------------------------------------------------------------------
// From class MPresentityGroupListEvent.
// CPresentityGroupListEventImp::UpdatedGroup()
// ---------------------------------------------------------------------------
//
const MPresentityGroupInfo&
        CPresentityGroupListEventImp::UpdatedGroup( TInt aIndex ) const
    {
    return *(*iUpdatedGroups)[ aIndex ];
    }

// ---------------------------------------------------------------------------
// From class MPresentityGroupListEvent.
// CPresentityGroupListEventImp::DisappearedGroupsCount()
// ---------------------------------------------------------------------------
//
TInt CPresentityGroupListEventImp::DisappearedGroupsCount() const
    {
    return iDeletedGroups->Count();
    }

// ---------------------------------------------------------------------------
// From class MPresentityGroupListEvent.
// CPresentityGroupListEventImp::DisappearedGroup()
// ---------------------------------------------------------------------------
//
const MPresentityGroupInfo&
CPresentityGroupListEventImp::DisappearedGroup( TInt aIndex ) const
    {
    return *(*iDeletedGroups)[ aIndex ];
    }

// ---------------------------------------------------------------------------
// From class MPresentityGroupListEvent.
// CPresentityGroupListEventImp::CurrentGroupsCount()
// ---------------------------------------------------------------------------
//
TInt CPresentityGroupListEventImp::CurrentGroupsCount() const
    {
    return iCurrentGroups->Count();
    }

// ---------------------------------------------------------------------------
// From class MPresentityGroupListEvent.
// CPresentityGroupListEventImp::CurrentGroup()
// ---------------------------------------------------------------------------
//
const MPresentityGroupInfo& 
    CPresentityGroupListEventImp::CurrentGroup( TInt aIndex ) const
    {
    return *(*iCurrentGroups)[ aIndex ];
    }

// ---------------------------------------------------------------------------
// From class MPresentityGroupListEvent.
// CPresentityGroupListEventImp::Status()
// ---------------------------------------------------------------------------
//
const MXIMPDataSubscriptionState&
    CPresentityGroupListEventImp::DataSubscriptionState() const
    {
    return *iSubscriptionState;
    }

// ---------------------------------------------------------------------------
// CPresentityGroupListEventImp::WriteArrayToStreamL()
// ---------------------------------------------------------------------------
//
void CPresentityGroupListEventImp::WriteArrayToStreamL(
        RPrGrpInfoImpArray& aArray, RWriteStream& aStream ) const
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
// CPresentityGroupListEventImp::FillArrayFromStreamL
// ---------------------------------------------------------------------------
//
void CPresentityGroupListEventImp::FillArrayFromStreamL(
        RPrGrpInfoImpArray& aArray,
        RReadStream& aStream )
    {
    TInt count = aStream.ReadInt32L();
    for( TInt a = 0; a < count; ++a )
        {
        // the stream MUST be in order (since it was externalized by this class)
        CPresentityGroupInfoImp* newInfo = CPresentityGroupInfoImp::NewLC();
        newInfo->InternalizeL( aStream );
        aArray.AppendL( newInfo );
        CleanupStack::Pop(); // newInfo
        }
    }

// End of file


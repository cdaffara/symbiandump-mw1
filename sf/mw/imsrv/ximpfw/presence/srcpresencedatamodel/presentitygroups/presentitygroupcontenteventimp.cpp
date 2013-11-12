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
* Description:  MPresentityGroupContentEvent API object implementation.
 *
*/

#include "presentitygroupcontenteventimp.h"
#include "presentitygroupmemberinfoimp.h"
#include "ximpidentityimp.h"
#include "ximpdatasubscriptionstateimp.h"
#include "ximpobjecthelpers.h"
#include <s32strm.h>


// ---------------------------------------------------------------------------
// CPresentityGroupContentEventImp::NewLC()
// ---------------------------------------------------------------------------
//
EXPORT_C CPresentityGroupContentEventImp* CPresentityGroupContentEventImp::NewLC(
        CXIMPIdentityImp& aGroupId,
        RPrGrpMemInfoImpArray* aAdded,
        RPrGrpMemInfoImpArray* aUpdated,
        RPrGrpMemInfoImpArray* aRemoved,
        CXIMPDataSubscriptionStateImp* aState
        )
    {
    CPresentityGroupContentEventImp* self = new( ELeave ) CPresentityGroupContentEventImp();
    CleanupStack::PushL( self );
    self->ConstructL( aGroupId, aAdded, aUpdated, aRemoved, aState );
    return self;
    }

// ---------------------------------------------------------------------------
// CPresentityGroupContentEventImp::NewLC()
// ---------------------------------------------------------------------------
//
EXPORT_C CPresentityGroupContentEventImp* CPresentityGroupContentEventImp::NewLC(
        CXIMPIdentityImp& aGroupId,
        RPrGrpMemInfoImpArray* aAdded,
        RPrGrpMemInfoImpArray* aUpdated,
        RPrGrpMemInfoImpArray* aRemoved,
        RPrGrpMemInfoImpArray* aCurrent,
        CXIMPDataSubscriptionStateImp* aState
        )
    {
    CPresentityGroupContentEventImp* self = new( ELeave ) CPresentityGroupContentEventImp();
    CleanupStack::PushL( self );
    if( aCurrent )
        {
        self->ConstructL( aGroupId, aAdded, aUpdated, aRemoved, aCurrent, aState );
        }
    else
        {
        self->ConstructL( aGroupId, aAdded, aUpdated, aRemoved, aState );
        }
    return self;
    }
// ---------------------------------------------------------------------------
// CPresentityGroupContentEventImp::NewL()
// ---------------------------------------------------------------------------
//
EXPORT_C CPresentityGroupContentEventImp* CPresentityGroupContentEventImp::NewL(
        CXIMPIdentityImp& aGroupId,
        RPrGrpMemInfoImpArray* aAdded,
        RPrGrpMemInfoImpArray* aUpdated,
        RPrGrpMemInfoImpArray* aRemoved,
        RPrGrpMemInfoImpArray* aCurrent,
        CXIMPDataSubscriptionStateImp* aState
        )
    {
    CPresentityGroupContentEventImp* self = 
        NewLC( aGroupId, aAdded, aUpdated, aRemoved, aCurrent, aState );
    CleanupStack::Pop( self );
    return self;
    }

// ---------------------------------------------------------------------------
// CPresentityGroupContentEventImp::NewFromStreamLC()
// ---------------------------------------------------------------------------
//
CXIMPApiEventBase* CPresentityGroupContentEventImp::NewFromStreamLC( RReadStream& aStream )
    {
    CPresentityGroupContentEventImp* self = new( ELeave ) CPresentityGroupContentEventImp();
    CleanupStack::PushL( self );
    self->ConstructL( aStream );
    return self;
    }

// ---------------------------------------------------------------------------
// CPresentityGroupContentEventImp::~CPresentityGroupContentEventImp()
// ---------------------------------------------------------------------------
//
CPresentityGroupContentEventImp::~CPresentityGroupContentEventImp()
    {
    delete iGroupId;

    if( iConstructedFromStream )
        {
        delete iAddedMembers;
        delete iUpdatedMembers;
        delete iRemovedMembers;
        delete iCurrentMembers;
        iCurrentMembers = NULL;  // must be null to skip double deletion below
        }

    if ( iOwnsCurrentMembersArray )
        {
        delete iCurrentMembers;
        }

    delete iSubscriptionState;
    }

// ---------------------------------------------------------------------------
// CPresentityGroupContentEventImp::CPresentityGroupContentEventImp()
// ---------------------------------------------------------------------------
//
CPresentityGroupContentEventImp::CPresentityGroupContentEventImp()
    {
    }


// ---------------------------------------------------------------------------
// CPresentityGroupContentEventImp::ConstructL()
// ---------------------------------------------------------------------------
//
void CPresentityGroupContentEventImp::ConstructL(
        CXIMPIdentityImp& aGroupId,
        RPrGrpMemInfoImpArray* aAdded,
        RPrGrpMemInfoImpArray* aUpdated,
        RPrGrpMemInfoImpArray* aRemoved,
        CXIMPDataSubscriptionStateImp* aState
         )
    {
    iGroupId = TXIMPObjectCloner< CXIMPIdentityImp >::CloneL( aGroupId );
    iAddedMembers = aAdded;
    iUpdatedMembers = aUpdated;
    iRemovedMembers = aRemoved;

    iCurrentMembers = new ( ELeave ) RPrGrpMemInfoImpArray;
    iOwnsCurrentMembersArray = ETrue;
    
    iSubscriptionState = aState;
    }

// ---------------------------------------------------------------------------
// CPresentityGroupContentEventImp::ConstructL()
// ---------------------------------------------------------------------------
//
void CPresentityGroupContentEventImp::ConstructL(
        CXIMPIdentityImp& aGroupId,
        RPrGrpMemInfoImpArray* aAdded,
        RPrGrpMemInfoImpArray* aUpdated,
        RPrGrpMemInfoImpArray* aRemoved,
        RPrGrpMemInfoImpArray* aCurrent,
        CXIMPDataSubscriptionStateImp* aState )
    {
    iGroupId = TXIMPObjectCloner< CXIMPIdentityImp >::CloneL( aGroupId );
    iAddedMembers = aAdded;
    iUpdatedMembers = aUpdated;
    iRemovedMembers = aRemoved;
    iCurrentMembers = aCurrent;

    iSubscriptionState = aState;
    }

// ---------------------------------------------------------------------------
// CPresentityGroupContentEventImp::ConstructL()
// ---------------------------------------------------------------------------
//
void CPresentityGroupContentEventImp::ConstructL( RReadStream& aStream )
    {
    iConstructedFromStream = ETrue;
    iAddedMembers = new ( ELeave ) RPrGrpMemInfoImpArray;
    iUpdatedMembers = new ( ELeave ) RPrGrpMemInfoImpArray;
    iRemovedMembers = new ( ELeave ) RPrGrpMemInfoImpArray;
    iCurrentMembers = new ( ELeave ) RPrGrpMemInfoImpArray;

    iGroupId = CXIMPIdentityImp::NewLC();
    CleanupStack::Pop( 1 ); // iGroupId

    iGroupId->InternalizeL( aStream );
    FillArrayFromStreamL( *iAddedMembers, aStream );
    FillArrayFromStreamL( *iUpdatedMembers, aStream );
    FillArrayFromStreamL( *iRemovedMembers, aStream );
    FillArrayFromStreamL( *iCurrentMembers, aStream );


    iSubscriptionState = CXIMPDataSubscriptionStateImp::NewL();
    iSubscriptionState->InternalizeL( aStream );
    }

// ---------------------------------------------------------------------------
// Implement supported interface access.
// ---------------------------------------------------------------------------
//
XIMPIMP_IF_BASE_GET_INTERFACE_BEGIN( CPresentityGroupContentEventImp,
                                     MPresentityGroupContentEvent )
    XIMPIMP_IF_BASE_GET_CLASS( CXIMPApiEventBase, this )
XIMPIMP_IF_BASE_GET_INTERFACE_END()

XIMPIMP_IF_BASE_GET_CONST_INTERFACE_BEGIN( CPresentityGroupContentEventImp,
                                           MPresentityGroupContentEvent )
    XIMPIMP_IF_BASE_GET_CONST_CLASS( CXIMPApiEventBase, this )                                            
XIMPIMP_IF_BASE_GET_INTERFACE_END()



// ---------------------------------------------------------------------------
// Implement methods from base event interface and base event class
// ---------------------------------------------------------------------------
//
XIMPIMP_IMPLEMENT_EVENT_BASE_METHODS( CPresentityGroupContentEventImp,
                                      MPresentityGroupContentEvent )


// ---------------------------------------------------------------------------
// CPresentityGroupContentEventImp::EqualsContent()
// ---------------------------------------------------------------------------
//
TBool CPresentityGroupContentEventImp::EqualsContent( 
    const CXIMPApiEventBase& aOtherInstance ) const
    {
    const CPresentityGroupContentEventImp* tmp =
    TXIMPGetImpClassOrPanic< const CPresentityGroupContentEventImp >::From( 
                                                        aOtherInstance.Base() );

    TBool same = ETrue;

    TBool x = 0 == GroupId().Identity().Compare( tmp->GroupId().Identity() );
    same &= x;

    x = CompareArrays( iAddedMembers, tmp->iAddedMembers );
    same &= x;

    x = CompareArrays( iUpdatedMembers, tmp->iUpdatedMembers );
    same &= x;

    x = CompareArrays( iRemovedMembers, tmp->iRemovedMembers );
    same &= x;

    x = CompareArrays( iCurrentMembers, tmp->iCurrentMembers );
    same &= x;

    x = iSubscriptionState->EqualsContent( *tmp->iSubscriptionState );
    same &= x;

    return same;
    }

// ---------------------------------------------------------------------------
// CPresentityGroupContentEventImp::CompareArrays
// ---------------------------------------------------------------------------
//
TBool CPresentityGroupContentEventImp::CompareArrays(
        RPrGrpMemInfoImpArray* aA, RPrGrpMemInfoImpArray* aB )
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
        x = 0 == ( (*aA)[ i ]->IdentityImp().Compare(
                    (*aB)[ i ]->IdentityImp() ) );
        same &= x;

        x = 0 == ( (*aA)[ i ]->GroupMemberDisplayName().Compare(
                    (*aB)[ i ]->GroupMemberDisplayName() ) );
        same &= x;
        }

    return same;
    }

// ---------------------------------------------------------------------------
// From class CXIMPApiEventBase.
// CPresentityGroupContentEventImp::ExternalizeL()
// ---------------------------------------------------------------------------
//
void CPresentityGroupContentEventImp::ExternalizeL( RWriteStream& aStream ) const
    {
    iGroupId->ExternalizeL( aStream );
    WriteArrayToStreamL( *iAddedMembers, aStream );
    WriteArrayToStreamL( *iUpdatedMembers, aStream );
    WriteArrayToStreamL( *iRemovedMembers, aStream );
    WriteArrayToStreamL( *iCurrentMembers, aStream );

    iSubscriptionState->ExternalizeL( aStream );
    }


// ---------------------------------------------------------------------------
// From class MPresentityGroupContentEvent.
// CPresentityGroupContentEventImp::GroupId()
// ---------------------------------------------------------------------------
//
const MXIMPIdentity& CPresentityGroupContentEventImp::GroupId() const
    {
    return *iGroupId;
    }

// ---------------------------------------------------------------------------
// From class MPresentityGroupContentEvent.
// CPresentityGroupContentEventImp::NewMembersCount()
// ---------------------------------------------------------------------------
//
TInt CPresentityGroupContentEventImp::NewMembersCount() const
    {
    return iAddedMembers->Count();
    }

// ---------------------------------------------------------------------------
// From class MPresentityGroupContentEvent.
// CPresentityGroupContentEventImp::NewMember()
// ---------------------------------------------------------------------------
//
const MPresentityGroupMemberInfo& 
    CPresentityGroupContentEventImp::NewMember( TInt aIndex ) const
    {
    return *(*iAddedMembers)[ aIndex ];
    }

// ---------------------------------------------------------------------------
// From class MPresentityGroupContentEvent.
// CPresentityGroupContentEventImp::UpdatedMembersCount()
// ---------------------------------------------------------------------------
//
TInt CPresentityGroupContentEventImp::UpdatedMembersCount() const
    {
    return iUpdatedMembers->Count();
    }

// ---------------------------------------------------------------------------
// From class MPresentityGroupContentEvent.
// CPresentityGroupContentEventImp::UpdatedMember()
// ---------------------------------------------------------------------------
//
const MPresentityGroupMemberInfo& 
    CPresentityGroupContentEventImp::UpdatedMember( TInt aIndex ) const
    {
    return *(*iUpdatedMembers)[ aIndex ];
    }

// ---------------------------------------------------------------------------
// From class MPresentityGroupContentEvent.
// CPresentityGroupContentEventImp::DisappearedMembersCount()
// ---------------------------------------------------------------------------
//
TInt CPresentityGroupContentEventImp::DisappearedMembersCount() const
    {
    return iRemovedMembers->Count();
    }

// ---------------------------------------------------------------------------
// From class MPresentityGroupContentEvent.
// CPresentityGroupContentEventImp::DisappearedMember()
// ---------------------------------------------------------------------------
//
const MPresentityGroupMemberInfo& 
    CPresentityGroupContentEventImp::DisappearedMember( TInt aIndex ) const
    {
    return *(*iRemovedMembers)[ aIndex ];
    }

// ---------------------------------------------------------------------------
// From class MPresentityGroupContentEvent.
// CPresentityGroupContentEventImp::CurrentMembersCount()
// ---------------------------------------------------------------------------
//
TInt CPresentityGroupContentEventImp::CurrentMembersCount() const
    {
    return iCurrentMembers->Count();
    }

// ---------------------------------------------------------------------------
// From class MPresentityGroupContentEvent.
// CPresentityGroupContentEventImp::CurrentMember()
// ---------------------------------------------------------------------------
//
const MPresentityGroupMemberInfo& 
    CPresentityGroupContentEventImp::CurrentMember( TInt aIndex ) const
    {
    return *(*iCurrentMembers)[ aIndex ];
    }

// ---------------------------------------------------------------------------
// From class MPresentityGroupContentEvent.
// CPresentityGroupContentEventImp::Status()
// ---------------------------------------------------------------------------
//
const MXIMPDataSubscriptionState&
    CPresentityGroupContentEventImp::DataSubscriptionState() const
    {
    return *iSubscriptionState;
    }


// ---------------------------------------------------------------------------
// CPresentityGroupContentEventImp::WriteArrayToStreamL()
// ---------------------------------------------------------------------------
//
void CPresentityGroupContentEventImp::WriteArrayToStreamL(
        RPrGrpMemInfoImpArray& aArray, RWriteStream& aStream ) const
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
// CPresentityGroupContentEventImp::FillArrayFromStreamL
// ---------------------------------------------------------------------------
//
void CPresentityGroupContentEventImp::FillArrayFromStreamL(
        RPrGrpMemInfoImpArray& aArray,
        RReadStream& aStream )
    {
    TInt count = aStream.ReadInt32L();
    for( TInt a = 0; a < count; ++a )
        {
        // the stream MUST be in order (since it was externalized by this class)
        CPresentityGroupMemberInfoImp* newInfo = CPresentityGroupMemberInfoImp::NewLC();
        newInfo->InternalizeL( aStream );
        aArray.AppendL( newInfo );
        CleanupStack::Pop(); // newInfo
        }
    }

// End of file


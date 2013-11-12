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
* Description:  Presence Service Connection group member subscription implementation.
*
*/


#include <badesca.h>

#include "groupcontentsubscriptionitem.h"
#include "presenceinfofilterimp.h"
#include "ximpitemparent.h"
#include "ximppanics.h"
#include "ximppsccontext.h"
#include "documentutils.h"
#include "presentitygroupcontenteventimp.h"
#include "presencetypehelpers.h"
#include "ximpobjecthelpers.h"
#include "presentitygroupmemberinfoimp.h"
#include "ximpidentityimp.h"
#include "groupcontenteventobserver.h"

#include "ximptrace.h"
// ---------------------------------------------------------------------------
// CGroupContentSubscriptionItem::CGroupContentSubscriptionItem()
// ---------------------------------------------------------------------------
//
CGroupContentSubscriptionItem::CGroupContentSubscriptionItem( MXIMPItemParentBase& aParent )
: CXIMPSubscriptionItemBase( aParent )
    {
    }


// ---------------------------------------------------------------------------
// CGroupContentSubscriptionItem::ConstructL()
// ---------------------------------------------------------------------------
//
void CGroupContentSubscriptionItem::ConstructL( const CXIMPIdentityImp& aGroupId )
    {
    BaseConstructL();
    
    iGroupId = TXIMPObjectCloner< CXIMPIdentityImp >::CloneL( aGroupId );

    // empty lists must always exist
    iCurrentMembers = new ( ELeave) RPrGrpMemInfoImpArray;

    iAdded = new ( ELeave ) RPrGrpMemInfoImpArray;
    iUpdated = new ( ELeave ) RPrGrpMemInfoImpArray;
    iRemoved = new ( ELeave ) RPrGrpMemInfoImpArray;
    }


// ---------------------------------------------------------------------------
// CGroupContentSubscriptionItem::NewLC()
// ---------------------------------------------------------------------------
//
CGroupContentSubscriptionItem*
        CGroupContentSubscriptionItem::NewLC( MXIMPItemParentBase& aParent,
                                                     const CXIMPIdentityImp& aGroupId )
    {
    CGroupContentSubscriptionItem* self =
                new( ELeave ) CGroupContentSubscriptionItem( aParent );
    CleanupClosePushL( *self );
    self->ConstructL( aGroupId );
    return self;
    }


// ---------------------------------------------------------------------------
// CGroupContentSubscriptionItem::~CGroupContentSubscriptionItem()
// ---------------------------------------------------------------------------
//
CGroupContentSubscriptionItem::~CGroupContentSubscriptionItem()
    {
        MXIMPItemParent* presCache = static_cast<MXIMPItemParent*>(iParent.GetInterface(PRESENCE_ITEM_PARENT));
    presCache->RemoveMe( this );


    if ( iCurrentMembers )
        {
        iCurrentMembers->Close();
        }
    delete iCurrentMembers;

    Clean();
    delete iAdded;
    delete iUpdated;
    delete iRemoved;

    delete iGroupId;

    iEventObservers.Close();
    }


// ---------------------------------------------------------------------------
// CGroupContentSubscriptionItem::AddOrRenewSubscriberL()
// ---------------------------------------------------------------------------
//
EXPORT_C void CGroupContentSubscriptionItem::AddOrRenewSubscriberL(
        MXIMPPscContext* aContext )
    {
    TInt index = iContexts.Find( aContext );
    if( index == KErrNotFound )
        {
        iContexts.AppendL( aContext );
        User::LeaveIfError( Open() );
        }
    }

// ---------------------------------------------------------------------------
// CGroupContentSubscriptionItem::GroupId()
// ---------------------------------------------------------------------------
//
EXPORT_C const CXIMPIdentityImp& CGroupContentSubscriptionItem::GroupId() const
    {
    return *iGroupId;
    }

// ---------------------------------------------------------------------------
// CGroupContentSubscriptionItem::Order()
// ---------------------------------------------------------------------------
//
EXPORT_C TInt CGroupContentSubscriptionItem::Order(
                        const CGroupContentSubscriptionItem& aA,
                        const CGroupContentSubscriptionItem& aB )
    {
    return IdentityOrder( aA.iGroupId, aB );
    }
// ---------------------------------------------------------------------------
// CPresentityPresenceSubscriptionItem::Order()
// ---------------------------------------------------------------------------
//
EXPORT_C TInt CGroupContentSubscriptionItem::IdentityOrder(
                        const CXIMPIdentityImp* aA,
                        const CGroupContentSubscriptionItem& aB )
    {
    return aA->Compare( *aB.iGroupId );
    }

// ---------------------------------------------------------------------------
// From MXIMPSubscriptionItem class.
// CGroupContentSubscriptionItem::SynthesiseSubscriptionEventTo()
// ---------------------------------------------------------------------------
//
void CGroupContentSubscriptionItem::SynthesiseSubscriptionEventToL(
        MXIMPPscContext* aContext, TBool aForceEvent )
    {
    TRACE_1( _L("CGroupContentSubscriptionItem::SynthesiseSubscriptionEventTo() aForce=%d"), aForceEvent );
    CXIMPDataSubscriptionStateImp* status = StatusLC( aContext );

    if( status->DataState() == MXIMPDataSubscriptionState::EDataAvailable || aForceEvent )
        {
        CPresentityGroupContentEventImp* newEvent =
            CPresentityGroupContentEventImp::NewL( *iGroupId,
                    iAdded,
                    iUpdated,
                    iRemoved,
                    aForceEvent ? iCurrentMembers : NULL,
                    status
                    );
        CleanupStack::Pop(); //status
        CleanupStack::PushL( newEvent );
        MXIMPItemParent* presCache = static_cast<MXIMPItemParent*>(iParent.GetInterface(PRESENCE_ITEM_PARENT));
        presCache->AddEventL( *newEvent, aContext );
        }        
    CleanupStack::PopAndDestroy(); // status || newEvent. Depending on branch.
    }

// ---------------------------------------------------------------------------
// CGroupContentSubscriptionItem::SynthesiseSubscriptionEventToAllCtxsL()
// ---------------------------------------------------------------------------
//
void CGroupContentSubscriptionItem::SynthesiseSubscriptionEventToAllCtxsL()
{

TRACE_1( _L("CGroupContentSubscriptionItem::SynthesiseSubscriptionEventToAllCtxsL() iEventObservers.Count()=%d"), iEventObservers.Count() );
TRACE_1( _L("CGroupContentSubscriptionItem::SynthesiseSubscriptionEventToAllCtxsL() iAdded->Count()=%d"), iAdded->Count() );
TRACE_1( _L("CGroupContentSubscriptionItem::SynthesiseSubscriptionEventToAllCtxsL() iUpdated->Count()=%d"), iUpdated->Count() );
TRACE_1( _L("CGroupContentSubscriptionItem::SynthesiseSubscriptionEventToAllCtxsL() iRemoved->Count()=%d"), iRemoved->Count() );

    TInt count = iEventObservers.Count();
    if( count && iAdded->Count() )
    {
        for( TInt a = 0; a < count; ++a )
        {
        TRACE_1( _L("CGroupContentSubscriptionItem::SynthesiseSubscriptionEventToAllCtxsL() observer = iEventObservers[ a ] =%d"), a );
            MGroupContentEventObserver* observer = iEventObservers[ a ];
        TInt error ( KErrNone );
        TRAP( error, observer->HandleSynthesiseL( *iAdded ););
        TRACE_1( _L("CGroupContentSubscriptionItem::SynthesiseSubscriptionEventToAllCtxsL()  TRAPD( error, observer->HandleSynthesiseL =%d"), error );
        }
    }

  TRACE( _L("CGroupContentSubscriptionItem::SynthesiseSubscriptionEventToAllCtxsL - Call to CXIMPSubscriptionItemBase::SynthesiseSubscriptionEventToAllCtxsL Now ") );
    CXIMPSubscriptionItemBase::SynthesiseSubscriptionEventToAllCtxsL();
}

// ---------------------------------------------------------------------------
// CGroupContentSubscriptionItem::SetNewListL()
// ---------------------------------------------------------------------------
//
EXPORT_C void CGroupContentSubscriptionItem::SetNewListL(
        RPrGrpMemInfoImpArray* aMemberList )
    {
    TRACE_1( _L("CGroupContentSubscriptionItem::SetNewListL() aMemberList Count=%d"), aMemberList->Count() );
    // we will eventually take ownership to this
    RPrGrpMemInfoImpArray* tmp = aMemberList;
    CleanupStack::PushL( tmp );

    // the list is already sorted. when a copy was made in datacacheimp, the
    // InsertInOrder method was used

    // The below algorithm has complexity of (roughly):
    //   - aMemberList length M
    //   - iCurrentMembers final length N
    //
    //   Final complexity:
    //     M*(    1     // indexed accessor
    //          + logN  // find (ordered)
    //          + logN  // insert in order (find+insert)
    //          + O(1)  // delete
    //          + O(1)  // remove
    //        ) + O(1)  // updating iRemoved and iCurrentMembers
    //     = M(2logN+3) + 2MlogN+3M = O(cMlogM).
    //
    // Should be fast enough. The complexity is not fully accurate because
    // array size grows as we insert into it.
    //
    TLinearOrder<CPresentityGroupMemberInfoImp>
        linearOrder( CPresentityGroupMemberInfoImp::GroupIdLinearOrder );

    for ( TInt i = 0; i < tmp->Count(); i++ )
        {
        // compare against current list
        CPresentityGroupMemberInfoImp* info = (*tmp)[ i ];
        TInt pos = iCurrentMembers->FindInOrder( info, linearOrder );

        if ( pos == KErrNotFound )
            {
            // not found in current list
            // so must be a fresh created list name
            iAdded->InsertInOrderL( info, linearOrder );
            }
        else
            {
            // found in current list, so it must be an updated list name
            iUpdated->InsertInOrderL( info, linearOrder );

            delete (*iCurrentMembers)[ pos ];
            iCurrentMembers->Remove( pos );

            // we must remove the found ones from iCurrentMembers,
            // otherwise we will not know what was left. and finding out
            // the removed items will be difficult.
            }
        }

    // what's left in iCurrentMembers contains the deleted ones.
    delete iRemoved;
    iRemoved = iCurrentMembers;

    // the given list becomes the new list
    iCurrentMembers = tmp;
    CleanupStack::Pop( tmp );
    iSubscriptionState->SetDataStateL( MXIMPDataSubscriptionState::EDataAvailable );
    InformObserversL();
    }


// ---------------------------------------------------------------------------
// CGroupContentSubscriptionItem::SetAddedListL()
// ---------------------------------------------------------------------------
//
EXPORT_C void CGroupContentSubscriptionItem::SetAddedListL(
        RPrGrpMemInfoImpArray* aMemberList )
    {
    TRACE_1( _L("CGroupContentSubscriptionItem::SetAddedListL() aMemberList Count=%d"), aMemberList->Count() );
    // we took ownership to the given list
    RPrGrpMemInfoImpArray* tmp = aMemberList;
    CleanupDeletePushL( tmp );

    // see also SetNewListL.

    // the list is already sorted. when a copy was made in datacacheimp, the
    // InsertInOrder method was used
    TLinearOrder<CPresentityGroupMemberInfoImp>
        linearOrder( CPresentityGroupMemberInfoImp::GroupIdLinearOrder );

    // update the list of created and updated groups
    for ( TInt i = 0; i < tmp->Count(); i++ )
        {
        // compare against current list
        CPresentityGroupMemberInfoImp* info = (*tmp)[ i ];
        TInt pos = iCurrentMembers->FindInOrder( info, linearOrder );

        if ( pos == KErrNotFound )
            {
            // not found in current list
            // so must be a fresh created list name.
            // this cannot come when the HandleDisplayNameUpdatedListL method
            // is called.
            iCurrentMembers->InsertInOrderL( info, linearOrder );
            ( *tmp )[ i ] = NULL;
            iAdded->InsertInOrderL( info, linearOrder );
            }
        else
            {
            // This can be checked if really had changed if needed. Now
            // we trust server that changes are reasonable.
            
            // change the display name of the updated group in the current list
            (*iCurrentMembers)[ pos ]->SetGroupMemberDisplayNameL( info->GroupMemberDisplayName() );

            // found in current list, so it must be an updated list name
            iUpdated->InsertInOrderL( (*iCurrentMembers)[ pos ], linearOrder );
            }
        }

    // current list may get updated display name to the existing elements,
    // otherwise the list is unchanged. list of deleted groups stays empty.
    // updated and created lists were updated. the input parameter list can be
    // deleted
    CleanupStack::PopAndDestroy( tmp );
    iSubscriptionState->SetDataStateL( MXIMPDataSubscriptionState::EDataAvailable );

    InformObserversL();
    }

// ---------------------------------------------------------------------------
// CGroupContentSubscriptionItem::SetRemovedListL()
// ---------------------------------------------------------------------------
//
EXPORT_C void CGroupContentSubscriptionItem::SetRemovedListL(
        RPrGrpMemInfoImpArray* aMemberList )
    {
    TRACE_1( _L("CGroupContentSubscriptionItem::SetRemovedListL() aMemberList Count=%d"), aMemberList->Count() );
    // we will eventually take ownership to this
    RPrGrpMemInfoImpArray* tmp = aMemberList;
    CleanupStack::PushL( tmp );

    // see also SetNewListL.

    // the list is already sorted. when a copy was made in datacacheimp, the
    // InsertInOrder method was used
    TLinearOrder<CPresentityGroupMemberInfoImp>
        linearOrder( CPresentityGroupMemberInfoImp::GroupIdLinearOrder );

    // remove the deleted ones from the current list
    for ( TInt i = 0; i < tmp->Count(); i++ )
        {
        // compare against current list
        CPresentityGroupMemberInfoImp* info = (*tmp)[ i ];
        TInt pos = iCurrentMembers->FindInOrder( info, linearOrder );

        // found in current list, so it must be a deleted list name
        if ( pos != KErrNotFound )
            {
            // remove from current list
            delete (*iCurrentMembers)[ pos ];
            iCurrentMembers->Remove( pos );
            }
        }

    // the given list becomes the new list of
    // removed ones
    delete iRemoved;
    iRemoved = tmp;
    CleanupStack::Pop( tmp );
    iSubscriptionState->SetDataStateL( MXIMPDataSubscriptionState::EDataAvailable );
    InformObserversL();
    }

// ---------------------------------------------------------------------------
// CGroupContentSubscriptionItem::Clean()
// ---------------------------------------------------------------------------
//
EXPORT_C void CGroupContentSubscriptionItem::Clean()
    {
    TRACE( _L("CGroupContentSubscriptionItem::Clean") );
    // empty the lists.
    // the lists must stay valid for use

    // iAdded and iUpdated are collection from currentlist. Do not delete items.
    // iRemoved is only place for items. Delete those.

    iAdded->Reset();
    iUpdated->Reset();
    iRemoved->Close();
    }


// ---------------------------------------------------------------------------
// CGroupContentSubscriptionItem::GroupMemberExistsL()
// ---------------------------------------------------------------------------
//
EXPORT_C TBool CGroupContentSubscriptionItem::GroupMemberExistsL(
                            CPresentityGroupMemberInfoImp& aMemberInfo ) const
    {
    TBool found( EFalse );

    CPresentityGroupMemberInfoImp* memberInfo =
        TXIMPObjectCloner< CPresentityGroupMemberInfoImp >::CloneL( aMemberInfo );
    CleanupStack::PushL( memberInfo );


    TLinearOrder<CPresentityGroupMemberInfoImp>
        linearOrder( CPresentityGroupMemberInfoImp::GroupIdLinearOrder );
    TInt pos = iCurrentMembers->FindInOrder( memberInfo, linearOrder );
    if ( pos != KErrNotFound )
        {
        found = ETrue;
        }

    CleanupStack::PopAndDestroy( memberInfo );
    TRACE_1( _L("CGroupContentSubscriptionItem::GroupMemberExistsL() returns=%d"), found );
    return found;
    }

// ---------------------------------------------------------------------------
// CGroupContentSubscriptionItem::IsGroupMemberUpdatedL()
// ---------------------------------------------------------------------------
//
EXPORT_C TBool CGroupContentSubscriptionItem::IsGroupMemberUpdatedL(
                            CPresentityGroupMemberInfoImp& aMemberInfo ) const
    {
    TBool updated( EFalse );

    if ( !GroupMemberExistsL( aMemberInfo ) )
        {
        return updated;
        }

    CPresentityGroupMemberInfoImp* memberInfo =
        TXIMPObjectCloner< CPresentityGroupMemberInfoImp >::CloneL( aMemberInfo );
    CleanupStack::PushL( memberInfo );


    TLinearOrder<CPresentityGroupMemberInfoImp>
        linearOrder( CPresentityGroupMemberInfoImp::GroupIdLinearOrder );
    TInt pos = iCurrentMembers->FindInOrder( memberInfo, linearOrder );
    // it exists, otherwise we would've returned after GroupMemberExistsL call

    CPresentityGroupMemberInfoImp* foundMember = (*iCurrentMembers)[ pos ];
    // Compare identity and displayname
    if ( memberInfo->GroupMemberId().Identity().Compare(
                            foundMember->GroupMemberId().Identity() ) != 0
                            ||
         memberInfo->GroupMemberDisplayName().Compare(
                            foundMember->GroupMemberDisplayName() ) != 0 )
        {
        updated = ETrue;
        }

    CleanupStack::PopAndDestroy( memberInfo );
    TRACE_1( _L("CGroupContentSubscriptionItem::IsGroupMemberUpdatedL() returns=%d"), updated );
    return updated;
    }

// ---------------------------------------------------------------------------
// CGroupContentSubscriptionItem::MemberCount()
// ---------------------------------------------------------------------------
//
TInt CGroupContentSubscriptionItem::MemberCount() const
    {
    TRACE_1( _L("CGroupContentSubscriptionItem::MemberCount() returns=%d"), iCurrentMembers->Count() );
    return iCurrentMembers->Count();
    }

// ---------------------------------------------------------------------------
// CGroupContentSubscriptionItem::MemberAt()
// ---------------------------------------------------------------------------
//
CPresentityGroupMemberInfoImp& CGroupContentSubscriptionItem::MemberAt( TInt aIndex )
    {
    return *( *iCurrentMembers )[ aIndex ];
    }

// ---------------------------------------------------------------------------
// CGroupContentSubscriptionItem::RegisterEventObserverL()
// ---------------------------------------------------------------------------
//
void CGroupContentSubscriptionItem::RegisterEventObserverL(
                        MGroupContentEventObserver* aEventObserver )
    {
    TInt indexOfObserver = iEventObservers.Find( aEventObserver );
    if( indexOfObserver == KErrNotFound )
        {
        iEventObservers.AppendL( aEventObserver );
        User::LeaveIfError( Open() );
        }
    }

// ---------------------------------------------------------------------------
// CGroupContentSubscriptionItem::UnregisterEventObserver()
// ---------------------------------------------------------------------------
//
void CGroupContentSubscriptionItem::UnregisterEventObserver(
                        MGroupContentEventObserver* aEventObserver )
    {
    TInt indexOfObserver = iEventObservers.Find( aEventObserver );
    if( indexOfObserver != KErrNotFound )
        {
        iEventObservers.Remove( indexOfObserver );
        Close();
        }
    }

// ---------------------------------------------------------------------------
// CGroupContentSubscriptionItem::CleanExpired()
// ---------------------------------------------------------------------------
//
void CGroupContentSubscriptionItem::CleanExpired()
    {
    }

// ---------------------------------------------------------------------------
// CGroupContentSubscriptionItem::InformObserversL()
// ---------------------------------------------------------------------------
//
void CGroupContentSubscriptionItem::InformObserversL()
    {
    TInt count = iEventObservers.Count();
    if( count && ( iAdded->Count() || iRemoved->Count() ) )
        {
        for( TInt a = 0; a < count; ++a )
            {
            MGroupContentEventObserver* observer = iEventObservers[ a ];
            observer->HandleChangeL( *iAdded, *iRemoved );
            }
        }
    }

// End of file

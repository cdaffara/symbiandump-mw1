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
* Description:  Presence Service Connection group list subscription implementation.
*
*/


#include <badesca.h>

#include "grouplistsubscriptionitem.h"
#include "presenceinfofilterimp.h"
#include "ximpitemparent.h"
#include "ximppanics.h"
#include "ximppsccontext.h"
#include "documentutils.h"
#include "presentitygrouplisteventimp.h"
#include "presencetypehelpers.h"
#include "presentitygroupinfoimp.h"
//#include "ximpapieventbase.h"

#include "ximptrace.h"
// ---------------------------------------------------------------------------
// CGroupListSubscriptionItem::CGroupListSubscriptionItem()
// ---------------------------------------------------------------------------
//
CGroupListSubscriptionItem::CGroupListSubscriptionItem( MXIMPItemParentBase& aParent )
: CXIMPSubscriptionItemBase( aParent )
    {
    }


// ---------------------------------------------------------------------------
// CGroupListSubscriptionItem::ConstructL()
// ---------------------------------------------------------------------------
//
void CGroupListSubscriptionItem::ConstructL()
    {
    BaseConstructL();
    // empty lists must always exist
    iCurrentList = new ( ELeave) RPrGrpInfoImpArray;

    iCreated = new ( ELeave ) RPrGrpInfoImpArray;
    iUpdated = new ( ELeave ) RPrGrpInfoImpArray;
    iDeleted = new ( ELeave ) RPrGrpInfoImpArray;
    }


// ---------------------------------------------------------------------------
// CGroupListSubscriptionItem::NewL()
// ---------------------------------------------------------------------------
//
CGroupListSubscriptionItem* CGroupListSubscriptionItem::NewLC( MXIMPItemParentBase& aParent )
    {
    CGroupListSubscriptionItem* self = new( ELeave ) CGroupListSubscriptionItem( aParent );
    CleanupClosePushL( *self );
    self->ConstructL();
    return self;
    }


// ---------------------------------------------------------------------------
// CGroupListSubscriptionItem::~CGroupListSubscriptionItem()
// ---------------------------------------------------------------------------
//
CGroupListSubscriptionItem::~CGroupListSubscriptionItem()
    {
        MXIMPItemParent* presCache = static_cast<MXIMPItemParent*>(iParent.GetInterface(PRESENCE_ITEM_PARENT));
    presCache->RemoveMe( this );

    if( iCurrentList )
        {
        iCurrentList->Close();
        }
    delete iCurrentList;

    Clean();
    // just in case
    delete iCreated;
    delete iDeleted;
    delete iUpdated;
    }

// ---------------------------------------------------------------------------
// CGroupListSubscriptionItem::SynthesiseSubscriptionEventTo()
// ---------------------------------------------------------------------------
//
void CGroupListSubscriptionItem::SynthesiseSubscriptionEventToL(
        MXIMPPscContext* aContext, TBool aForceEvent )
    {
    TRACE_1( _L("CGroupListSubscriptionItem::SynthesiseSubscriptionEventTo() aForce=%d"), aForceEvent );
    CXIMPDataSubscriptionStateImp* status = StatusLC( aContext );

    if( status->DataState() == MXIMPDataSubscriptionState::EDataAvailable || aForceEvent )
        {
        CPresentityGroupListEventImp* newEvent = CPresentityGroupListEventImp::NewL(
                iCreated,
                iUpdated,
                iDeleted,
                aForceEvent ? iCurrentList : NULL,
                status
                );
        CleanupStack::Pop( status );
        CleanupStack::PushL( newEvent );
        
        MXIMPItemParent* presCache = static_cast<MXIMPItemParent*>(iParent.GetInterface(PRESENCE_ITEM_PARENT));
        presCache->AddEventL( *newEvent, aContext );
        
        }
    CleanupStack::PopAndDestroy(); //status || newEvent. Depending on branch.
    }


// ---------------------------------------------------------------------------
// CGroupListSubscriptionItem::SetNewListL
// ---------------------------------------------------------------------------
//
EXPORT_C void CGroupListSubscriptionItem::SetNewListL(
        RPrGrpInfoImpArray* aGroupList )
    {
    TRACE_1( _L("CGroupListSubscriptionItem::SetNewListL() aGroupList Count=%d"), aGroupList->Count() );
    // we will eventually take ownership to this
    RPrGrpInfoImpArray* tmp = aGroupList;
    CleanupDeletePushL( tmp );

    // the list is already sorted. when a copy was made in datacacheimp, the
    // InsertInOrder method was used

    // The below algorithm has complexity of (roughly):
    //   - aGroupList length M
    //   - iCurrentList final length N
    //
    //   Final complexity:
    //     M*(    1     // indexed accessor
    //          + logN  // find (ordered)
    //          + logN  // insert in order (find+insert)
    //          + O(1)  // delete
    //          + O(1)  // remove
    //        ) + O(1)  // updating iDeleted and iCurrentList
    //     = M(2logN+3) + 2MlogN+3M = O(cMlogM).
    //
    // Should be fast enough. The complexity is not fully accurate because
    // array size grows as we insert into it.
    //
    TLinearOrder<CPresentityGroupInfoImp>
        linearOrder( CPresentityGroupInfoImp::GroupIdLinearOrder );

    for ( TInt i = 0; i < tmp->Count(); i++ )
        {
        // compare against current list
        CPresentityGroupInfoImp* info = (*tmp)[ i ];
        TInt pos = iCurrentList->FindInOrder( info, linearOrder );

        if ( pos == KErrNotFound )
            {
            // not found in current list
            // so must be a fresh created list name
            iCreated->InsertInOrderL( info, linearOrder );
            }
        else
            {
            // found in current list, so it must be an updated list name
            iUpdated->InsertInOrderL( info, linearOrder );

            delete (*iCurrentList)[ pos ];
            iCurrentList->Remove( pos );

            // we must remove the found ones from iCurrentList,
            // otherwise we will not know what was left. and finding out
            // the deleted items will be difficult.
            }
        }

    // what's left in iCurrentList contains the deleted ones.
    delete iDeleted;
    iDeleted = iCurrentList;

    // the given list becomes the new list
    iCurrentList = tmp;
    CleanupStack::Pop( tmp );
    iSubscriptionState->SetDataStateL( MXIMPDataSubscriptionState::EDataAvailable );
    }

// ---------------------------------------------------------------------------
// CGroupListSubscriptionItem::SetCreatedListL
// ---------------------------------------------------------------------------
//
EXPORT_C void CGroupListSubscriptionItem::SetCreatedListL(
        RPrGrpInfoImpArray* aGroupList )
    {
    TRACE_1( _L("CGroupListSubscriptionItem::SetCreatedListL() aGroupList Count=%d"), aGroupList->Count() );
    // we took ownership to the given list
    RPrGrpInfoImpArray* tmp = aGroupList;
    CleanupDeletePushL( tmp );

    // see also SetNewListL.

    // the list is already sorted. when a copy was made in datacacheimp, the
    // InsertInOrder method was used
    TLinearOrder<CPresentityGroupInfoImp>
        linearOrder( CPresentityGroupInfoImp::GroupIdLinearOrder );

    // update the list of created and updated groups
    for ( TInt i = 0; i < tmp->Count(); i++ )
        {
        // compare against current list
        CPresentityGroupInfoImp* info = (*tmp)[ i ];
        TInt pos = iCurrentList->FindInOrder( info, linearOrder );

        if ( pos == KErrNotFound )
            {
            // not found in current list
            // so must be a fresh created list name.
            // this cannot come when the HandleDisplayNameUpdatedListL method
            // is called.
            iCurrentList->InsertInOrderL( info, linearOrder );
            ( *tmp )[ i ] = NULL;
            iCreated->InsertInOrderL( info, linearOrder );
            }
        else
            {
            // This can be checked if really had changed if needed. Now
            // we trust server that changes are reasonable.

            // change the display name of the updated group in the current list
            (*iCurrentList)[ pos ]->SetGroupDisplayNameL( info->GroupDisplayName() );

            // found in current list, so it must be an updated list name
            iUpdated->InsertInOrderL( (*iCurrentList)[ pos ], linearOrder );
            }
        }

    // current list may get updated display name to the existing elements,
    // otherwise the list is unchanged. list of deleted groups stays empty.
    // updated and created lists were updated. the input parameter list can be
    // deleted
    CleanupStack::PopAndDestroy( tmp );
    iSubscriptionState->SetDataStateL( MXIMPDataSubscriptionState::EDataAvailable );
    }

// ---------------------------------------------------------------------------
// CGroupListSubscriptionItem::SetDeletedListL
// ---------------------------------------------------------------------------
//
EXPORT_C void CGroupListSubscriptionItem::SetDeletedListL(
        RPrGrpInfoImpArray* aGroupList )
    {
    TRACE_1( _L("CGroupListSubscriptionItem::SetDeletedListL() aGroupList Count=%d"), aGroupList->Count() );
    // we will eventually take ownership to this
    RPrGrpInfoImpArray* tmp = aGroupList;
    CleanupDeletePushL( tmp );

    // see also SetNewListL.

    // the list is already sorted. when a copy was made in datacacheimp, the
    // InsertInOrder method was used
    TLinearOrder<CPresentityGroupInfoImp>
        linearOrder( CPresentityGroupInfoImp::GroupIdLinearOrder );

    // remove the deleted ones from the current list
    for ( TInt i = 0; i < tmp->Count(); i++ )
        {
        // compare against current list
        CPresentityGroupInfoImp* info = (*tmp)[ i ];
        TInt pos = iCurrentList->FindInOrder( info, linearOrder );

        // found in current list, so it must be a deleted list name
        if ( pos != KErrNotFound )
            {
            // remove from current list
            delete (*iCurrentList)[ pos ];
            iCurrentList->Remove( pos );
            }
        }

    // the given list becomes the new list of
    // deleted ones
    delete iDeleted;
    iDeleted = tmp;
    CleanupStack::Pop( tmp );
    iSubscriptionState->SetDataStateL( MXIMPDataSubscriptionState::EDataAvailable );
    }

// ---------------------------------------------------------------------------
// CGroupListSubscriptionItem::Clean
// ---------------------------------------------------------------------------
//
EXPORT_C void CGroupListSubscriptionItem::Clean()
    {
    TRACE( _L("CGroupListSubscriptionItem::Clean()") );
    // empty the lists.
    // the lists must stay valid for use

    // iCreated and iUpdated are collection from currentlist. Do not delete items.
    // iDeleted is only place for items. Delete those.

    iCreated->Reset();
    iUpdated->Reset();
    iDeleted->Close();
    }

// ---------------------------------------------------------------------------
// CGroupListSubscriptionItem::CleanExpired()
// ---------------------------------------------------------------------------
//
void CGroupListSubscriptionItem::CleanExpired()
    {

    }
// End of file

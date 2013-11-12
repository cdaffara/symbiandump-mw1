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
* Description:  Presence Service Connection presence block list subscription implementation.
*
*/


#include <badesca.h>

#include "blocklistsubscriptionitem.h"
#include "presenceinfofilterimp.h"
#include "ximpitemparent.h"
#include "ximppanics.h"
#include "ximppsccontext.h"
#include "documentutils.h"
#include "ximpobjecthelpers.h"
#include "presencetypehelpers.h"
#include "presenceblockinfoimp.h"
#include "presenceblocklisteventimp.h"
#include "ximpidentityimp.h"
#include "ximpdatasubscriptionstateimp.h"

#include "ximptrace.h"

// ---------------------------------------------------------------------------
// CBlockListSubscriptionItem::CBlockListSubscriptionItem()
// ---------------------------------------------------------------------------
//
CBlockListSubscriptionItem::CBlockListSubscriptionItem( MXIMPItemParentBase& aParent )
: CXIMPSubscriptionItemBase( aParent )
    {
    }


// ---------------------------------------------------------------------------
// CBlockListSubscriptionItem::ConstructL()
// ---------------------------------------------------------------------------
//
void CBlockListSubscriptionItem::ConstructL()
    {
    BaseConstructL();
    // empty lists must always exist
    iCurrent = new ( ELeave) RPrBlockInfoImpArray;
    iAdded = new ( ELeave ) RPrBlockInfoImpArray;
    iRemoved = new ( ELeave ) RPrBlockInfoImpArray;
    iUpdated = new ( ELeave ) RPrBlockInfoImpArray;
    }


// ---------------------------------------------------------------------------
// CBlockListSubscriptionItem::NewL()
// ---------------------------------------------------------------------------
//
CBlockListSubscriptionItem* CBlockListSubscriptionItem::NewLC( MXIMPItemParentBase& aParent )
    {
    CBlockListSubscriptionItem* self = new( ELeave ) CBlockListSubscriptionItem( aParent );
    CleanupClosePushL( *self );
    self->ConstructL();
    return self;
    }


// ---------------------------------------------------------------------------
// CBlockListSubscriptionItem::~CBlockListSubscriptionItem()
// ---------------------------------------------------------------------------
//
CBlockListSubscriptionItem::~CBlockListSubscriptionItem()
    {
    MXIMPItemParent* presCache = static_cast<MXIMPItemParent*>(iParent.GetInterface(PRESENCE_ITEM_PARENT));
    presCache->RemoveMe( this );

    if( iCurrent )
        {
        iCurrent->Close();
        }
    delete iCurrent;

    Clean();
    // just in case
    delete iAdded;
    delete iRemoved;
    delete iUpdated;
    }

// ---------------------------------------------------------------------------
// From MXIMPSubscriptionItem class.
// CBlockListSubscriptionItem::SynthesiseSubscriptionEventTo()
// ---------------------------------------------------------------------------
//
void CBlockListSubscriptionItem::SynthesiseSubscriptionEventToL(
        MXIMPPscContext* aContext, TBool aForceEvent )
    {
    TRACE_1( _L("CBlockListSubscriptionItem::SynthesiseSubscriptionEventTo() aForce=%d"), aForceEvent );
    CXIMPDataSubscriptionStateImp* status = StatusLC( aContext );  // << status

    if( status->DataState() == MXIMPDataSubscriptionState::EDataAvailable || aForceEvent )
        {
        CleanupStack::Pop( status );    // >> status
        CPresenceBlockListEventImp* newEvent = CPresenceBlockListEventImp::NewLC(
                iAdded,
                aForceEvent ? iCurrent : NULL,
                iRemoved,
                iUpdated,
                status
                );
        MXIMPItemParent* presCache = static_cast<MXIMPItemParent*>(iParent.GetInterface(PRESENCE_ITEM_PARENT));
        presCache->AddEventL( *newEvent, aContext );
        CleanupStack::PopAndDestroy(); // >>> newEvent
        }
     else
        {
        CleanupStack::PopAndDestroy(); // >>> status
        }
    }


// ---------------------------------------------------------------------------
// CBlockListSubscriptionItem::SetNewListL()
// ---------------------------------------------------------------------------
//
EXPORT_C void CBlockListSubscriptionItem::SetNewListL(
                                                RPrBlockInfoImpArray* aList )
    {
    TRACE_1( _L("CBlockListSubscriptionItem::SetNewListL aList Count=%d"), aList->Count() );
    RPrBlockInfoImpArray* tmp = aList;
    CleanupDeletePushL( tmp );

    TLinearOrder<CPresenceBlockInfoImp>
        linearOrder( CPresenceBlockInfoImp::IdLinearOrder );

    for ( TInt i = 0; i < tmp->Count(); i++ )
        {
        // compare against current pending list
        CPresenceBlockInfoImp* info = (*tmp)[ i ];
        TInt pos = iCurrent->FindInOrder( info, linearOrder );

        if ( pos == KErrNotFound )
            {
            // not found in current list
            // so must be a fresh watcher
            iAdded->InsertInOrderL( info, linearOrder );
            }
        else
            {
            // found in current list, so it must be an updated list name
            ////iUpdated->InsertInOrderL( info, linearOrder );
            CPresenceBlockInfoImp* infoClone = NULL;
            infoClone = TXIMPObjectCloner< CPresenceBlockInfoImp >::CloneLC( *info );
            iUpdated->AppendL( infoClone ); // ownership is transferrred
            CleanupStack::Pop( infoClone );

            // remove it from the iCurrent list,
            // so it won't get on the iRemoved after this loop
            delete (*iCurrent)[ pos ];
            iCurrent->Remove( pos );
            }
        }

    // what's left in iCurrent contains the removed ones.
    delete iRemoved;
    iRemoved = iCurrent;

    // the given list becomes the new current list
    iCurrent = tmp;
    CleanupStack::Pop( tmp );
    iSubscriptionState->SetDataStateL( MXIMPDataSubscriptionState::EDataAvailable );
    }


// ---------------------------------------------------------------------------
// CBlockListSubscriptionItem::SetBlockedL()
// ---------------------------------------------------------------------------
//
EXPORT_C void CBlockListSubscriptionItem::SetBlockedL(
                                        CPresenceBlockInfoImp* aReq )
    {
    TRACE( _L("CBlockListSubscriptionItem::SetBlockedL") );
    TLinearOrder<CPresenceBlockInfoImp>
        linearOrder( CPresenceBlockInfoImp::IdLinearOrder );

    // compare against current list
    TInt pos = iCurrent->FindInOrder( aReq, linearOrder );

    if ( pos == KErrNotFound )
        {
        // not found in current list
        // so must be a fresh watcher
        iAdded->InsertInOrderL( aReq, linearOrder );
        // and also to the pending
        iCurrent->InsertInOrderL( aReq, linearOrder );
        // This looks like we take ownership two times, but the way we act
        // only iCurrent really owns it.
        // See the comments Clean() for more about this

        iSubscriptionState->SetDataStateL( MXIMPDataSubscriptionState::EDataAvailable );
        }
    else
        {
        // The request already exists in the pending list.
        // Ignore and delete the the object
        delete aReq;
        }
    }

// ---------------------------------------------------------------------------
// CBlockListSubscriptionItem::SetCanceledBlockIdL()
// ---------------------------------------------------------------------------
//
EXPORT_C void CBlockListSubscriptionItem::SetCanceledBlockIdL(
                                                CXIMPIdentityImp* aRemoved )
    {
    TRACE( _L("CBlockListSubscriptionItem::SetCanceledBlockIdL") );
    for ( TInt i = 0; i < iCurrent->Count(); i++ )
        {

        if ( aRemoved->Compare( (*iCurrent)[ i ]->IdentityImp() ) == 0 )
            {
            iRemoved->AppendL( (*iCurrent)[ i ] );
            iCurrent->Remove( i );
            break;
            }
        }


    delete aRemoved;
    }

// ---------------------------------------------------------------------------
// CBlockListSubscriptionItem::Clean()
// ---------------------------------------------------------------------------
//
EXPORT_C void CBlockListSubscriptionItem::Clean()
    {
    TRACE( _L("CBlockListSubscriptionItem::Clean") );
    // empty the lists.
    // the lists must stay valid for use

    // iAdded is a collection from the pending list.
    // Do not delete items.
    // iRemoved and iUpdated hold items. Delete those.


    iAdded->Reset();
    iRemoved->Close();
    iUpdated->Close();
    }

// ---------------------------------------------------------------------------
// From CXIMPSubscriptionItemBase class.
// CBlockListSubscriptionItem::CleanExpired()
// ---------------------------------------------------------------------------
//
void CBlockListSubscriptionItem::CleanExpired()
    {

    }

// End of file

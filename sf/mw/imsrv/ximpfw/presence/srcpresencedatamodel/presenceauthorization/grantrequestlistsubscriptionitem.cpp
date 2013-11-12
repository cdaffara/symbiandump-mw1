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
* Description:  Presence Service Connection presence watcher list subscription implementation.
*
*/


#include <badesca.h>

#include "grantrequestlistsubscriptionitem.h"
#include "presenceinfofilterimp.h"
#include "ximpitemparent.h"
#include "ximppanics.h"
#include "ximppsccontext.h"
#include "documentutils.h"
#include "ximpobjecthelpers.h"
#include "presencetypehelpers.h"
#include "presencegrantrequestinfoimp.h"
#include "presencegrantrequestlisteventimp.h"
#include "ximpidentityimp.h"
#include "ximpdatasubscriptionstateimp.h"

#include "ximptrace.h"


// ---------------------------------------------------------------------------
// CGrantRequestListSubscriptionItem::CGrantRequestListSubscriptionItem()
// ---------------------------------------------------------------------------
//
CGrantRequestListSubscriptionItem::CGrantRequestListSubscriptionItem( MXIMPItemParentBase& aParent )
: CXIMPSubscriptionItemBase( aParent )
    {
    }


// ---------------------------------------------------------------------------
// CGrantRequestListSubscriptionItem::ConstructL()
// ---------------------------------------------------------------------------
//
void CGrantRequestListSubscriptionItem::ConstructL()
    {
    BaseConstructL();
    // empty lists must always exist
    iPending = new ( ELeave) RPrGrntReqInfoImpArray;
    iNewRequests = new ( ELeave ) RPrGrntReqInfoImpArray;
    iObsoleted = new ( ELeave ) RPrGrntReqInfoImpArray;
    }


// ---------------------------------------------------------------------------
// CGrantRequestListSubscriptionItem::NewL()
// ---------------------------------------------------------------------------
//
CGrantRequestListSubscriptionItem* CGrantRequestListSubscriptionItem::NewLC( MXIMPItemParentBase& aParent )
    {
    CGrantRequestListSubscriptionItem* self = new( ELeave ) CGrantRequestListSubscriptionItem( aParent );
    CleanupClosePushL( *self );
    self->ConstructL();
    return self;
    }


// ---------------------------------------------------------------------------
// CGrantRequestListSubscriptionItem::~CGrantRequestListSubscriptionItem()
// ---------------------------------------------------------------------------
//
CGrantRequestListSubscriptionItem::~CGrantRequestListSubscriptionItem()
    {
    MXIMPItemParent* presCache = static_cast<MXIMPItemParent*>(iParent.GetInterface(PRESENCE_ITEM_PARENT));
    presCache->RemoveMe( this );


    if( iPending )
        {
        iPending->Close();
        }
    delete iPending;

    Clean();
    // just in case
    delete iNewRequests;
    delete iObsoleted;
    }

// ---------------------------------------------------------------------------
// From MXIMPSubscriptionItem class.
// CGrantRequestListSubscriptionItem::SynthesiseSubscriptionEventTo()
// ---------------------------------------------------------------------------
//
void CGrantRequestListSubscriptionItem::SynthesiseSubscriptionEventToL(
        MXIMPPscContext* aContext, TBool aForceEvent )
    {
    TRACE_1( _L("CGrantRequestListSubscriptionItem::SynthesiseSubscriptionEventTo() aForce=%d"), aForceEvent );
    CXIMPDataSubscriptionStateImp* status = StatusLC( aContext );  // << status

    if( status->DataState() == MXIMPDataSubscriptionState::EDataAvailable || aForceEvent )
        {
        CleanupStack::Pop( status );    // >> status
        CPresenceGrantRequestListEventImp* newEvent = CPresenceGrantRequestListEventImp::NewLC(
                iNewRequests,
                aForceEvent ? iPending : NULL,
                iObsoleted,
                status );
                
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
// CGrantRequestListSubscriptionItem::SetNewListL()
// ---------------------------------------------------------------------------
//
EXPORT_C void CGrantRequestListSubscriptionItem::SetNewListL(
                                                RPrGrntReqInfoImpArray* aList )
    {
    TRACE_1( _L("CGrantRequestListSubscriptionItem::SetNewListL aList Count=%d"), aList->Count() );
    RPrGrntReqInfoImpArray* tmp = aList;
    CleanupDeletePushL( tmp );

    TLinearOrder<CPresenceGrantRequestInfoImp>
        linearOrder( CPresenceGrantRequestInfoImp::IdLinearOrder );

    for ( TInt i = 0; i < tmp->Count(); i++ )
        {
        // compare against current pending list
        CPresenceGrantRequestInfoImp* info = (*tmp)[ i ];
        TInt pos = iPending->FindInOrder( info, linearOrder );

        if ( pos == KErrNotFound )
            {
            // not found in current list
            // so must be a fresh watcher
            iNewRequests->InsertInOrderL( info, linearOrder );
            }
        else
            {
            // otherwise remove it from the iPending list,
            // so it won't get on the iObsoleted after this loop
            delete (*iPending)[ pos ];
            iPending->Remove( pos );
            }
        }

    // what's left in iPending contains the obsoleted ones.
    delete iObsoleted;
    iObsoleted = iPending;

    // the given list becomes the new pending list
    iPending = tmp;
    CleanupStack::Pop( tmp );
    iSubscriptionState->SetDataStateL( MXIMPDataSubscriptionState::EDataAvailable );
    }


// ---------------------------------------------------------------------------
// CGrantRequestListSubscriptionItem::SetReceivedRequestL()
// ---------------------------------------------------------------------------
//
EXPORT_C void CGrantRequestListSubscriptionItem::SetReceivedRequestL(
                                        CPresenceGrantRequestInfoImp* aReq )
    {
    TRACE( _L("CGrantRequestListSubscriptionItem::SetReceivedRequestL") );
    TLinearOrder<CPresenceGrantRequestInfoImp>
        linearOrder( CPresenceGrantRequestInfoImp::IdLinearOrder );

    // compare against current list
    TInt pos = iPending->FindInOrder( aReq, linearOrder );

    if ( pos == KErrNotFound )
        {
        // not found in current list
        // so must be a fresh watcher
        iNewRequests->InsertInOrderL( aReq, linearOrder );
        // and also to the pending
        iPending->InsertInOrderL( aReq, linearOrder );
        // This looks like we take ownership two times, but the way we act
        // only iPending really owns it.
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
// CGrantRequestListSubscriptionItem::SetObsoletedRequestL()
// ---------------------------------------------------------------------------
//
EXPORT_C void CGrantRequestListSubscriptionItem::SetObsoletedRequestL(
                                                CXIMPIdentityImp* aObsoleted )
    {
    TRACE( _L("CGrantRequestListSubscriptionItem::SetObsoletedRequestL") );
    for ( TInt i = 0; i < iPending->Count(); i++ )
        {
        if ( aObsoleted->Compare( (*iPending)[ i ]->IdentityImp() ) == 0 )
            {
            iObsoleted->AppendL( (*iPending)[ i ] );
            iPending->Remove( i );
            break;
            }
        }


    delete aObsoleted;
    }

// ---------------------------------------------------------------------------
// CGrantRequestListSubscriptionItem::Clean()
// ---------------------------------------------------------------------------
//
EXPORT_C void CGrantRequestListSubscriptionItem::Clean()
    {
    TRACE( _L("CGrantRequestListSubscriptionItem::Clean") );
    // empty the lists.
    // the lists must stay valid for use

    // iNewRequests is a collection from the pending list.
    // Do not delete items.
    // iObsoleted is only place for items. Delete those.


    iNewRequests->Reset();
    iObsoleted->Close();
    }

// ---------------------------------------------------------------------------
// From CXIMPSubscriptionItemBase class.
// CGrantRequestListSubscriptionItem::CleanExpired()
// ---------------------------------------------------------------------------
//
void CGrantRequestListSubscriptionItem::CleanExpired()
    {

    }

// End of file

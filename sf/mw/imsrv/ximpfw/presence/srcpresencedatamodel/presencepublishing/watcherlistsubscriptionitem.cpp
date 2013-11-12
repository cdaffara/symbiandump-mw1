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

#include "watcherlistsubscriptionitem.h"
#include "presenceinfofilterimp.h"
#include "ximpitemparent.h"
#include "ximppanics.h"
#include "ximppsccontext.h"
#include "documentutils.h"
#include "presencewatcherlisteventimp.h"
#include "ximpobjecthelpers.h"
#include "presencetypehelpers.h"
#include "presencewatcherinfoimp.h"
#include "ximpidentityimp.h"
#include "ximpdatasubscriptionstateimp.h"

#include "ximptrace.h"

// ---------------------------------------------------------------------------
// CWatcherListSubscriptionItem::CWatcherListSubscriptionItem()
// ---------------------------------------------------------------------------
//
CWatcherListSubscriptionItem::CWatcherListSubscriptionItem( MXIMPItemParentBase& aParent )
: CXIMPSubscriptionItemBase( aParent )
    {
    }


// ---------------------------------------------------------------------------
// CWatcherListSubscriptionItem::ConstructL()
// ---------------------------------------------------------------------------
//
void CWatcherListSubscriptionItem::ConstructL()
    {
    BaseConstructL();
    // empty lists must always exist
    iCurrentList = new ( ELeave) RPrWatLstInfoImpArray;
    iNewWatchers = new ( ELeave ) RPrWatLstInfoImpArray;
    iDisappeared = new ( ELeave ) RPrWatLstInfoImpArray;
    }


// ---------------------------------------------------------------------------
// CWatcherListSubscriptionItem::NewL()
// ---------------------------------------------------------------------------
//
CWatcherListSubscriptionItem* CWatcherListSubscriptionItem::NewLC( MXIMPItemParentBase& aParent )
    {
    CWatcherListSubscriptionItem* self = new( ELeave ) CWatcherListSubscriptionItem( aParent );
    CleanupClosePushL( *self );
    self->ConstructL();
    return self;
    }


// ---------------------------------------------------------------------------
// CWatcherListSubscriptionItem::~CWatcherListSubscriptionItem()
// ---------------------------------------------------------------------------
//
CWatcherListSubscriptionItem::~CWatcherListSubscriptionItem()
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
    delete iNewWatchers;
    delete iDisappeared;
    }

// ---------------------------------------------------------------------------
// From MXIMPSubscriptionItem class.
// CWatcherListSubscriptionItem::SynthesiseSubscriptionEventTo()
// ---------------------------------------------------------------------------
//
void CWatcherListSubscriptionItem::SynthesiseSubscriptionEventToL(
        MXIMPPscContext* aContext, TBool aForceEvent )
    {
    TRACE_1( _L("CWatcherListSubscriptionItem::SynthesiseSubscriptionEventTo() aForce=%d"), aForceEvent );
    CXIMPDataSubscriptionStateImp* status = StatusLC( aContext );   /// << status

    if( status->DataState() == MXIMPDataSubscriptionState::EDataAvailable || aForceEvent )
        {
        CleanupStack::Pop( status );    // >> status
        CPresenceWatcherListEventImp* newEvent = CPresenceWatcherListEventImp::NewLC(
                iNewWatchers,
                aForceEvent ? iCurrentList : NULL,
                iDisappeared,
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
// CWatcherListSubscriptionItem::SetNewListL()
// ---------------------------------------------------------------------------
//
EXPORT_C void CWatcherListSubscriptionItem::SetNewListL(
                                                RPrWatLstInfoImpArray* aList )
    {
    TRACE_1( _L("CWatcherListSubscriptionItem::SetNewListL aList Count=%d"), aList->Count() );
    RPrWatLstInfoImpArray* tmp = aList;
    CleanupDeletePushL( tmp );

    TLinearOrder<CPresenceWatcherInfoImp>
        linearOrder( CPresenceWatcherInfoImp::IdLinearOrder );

    for ( TInt i = 0; i < tmp->Count(); i++ )
        {
        // compare against current list
        CPresenceWatcherInfoImp* info = (*tmp)[ i ];
        TInt pos = iCurrentList->FindInOrder( info, linearOrder );

        if ( pos == KErrNotFound )
            {
            // not found in current list
            // so must be a fresh watcher
            iNewWatchers->InsertInOrderL( info, linearOrder );
            }
        else
            {
            delete (*iCurrentList)[ pos ];
            iCurrentList->Remove( pos );

            // we must remove the found ones from iCurrentList,
            // otherwise we will not know what was left. and finding out
            // the deleted items will be difficult.
            }
        }

    // what's left in iCurrentList contains the deleted ones.
    delete iDisappeared;
    iDisappeared = iCurrentList;

    // the given list becomes the new list
    iCurrentList = tmp;
    CleanupStack::Pop( tmp );
    iSubscriptionState->SetDataStateL( MXIMPDataSubscriptionState::EDataAvailable );
    }

// ---------------------------------------------------------------------------
// CWatcherListSubscriptionItem::Clean()
// ---------------------------------------------------------------------------
//
EXPORT_C void CWatcherListSubscriptionItem::Clean()
    {
    TRACE( _L("CWatcherListSubscriptionItem::Clean") );
    // empty the lists.
    // the lists must stay valid for use

    // iNewWatchers and iUpdated are collection from currentlist. Do not delete items.
    // iDisappeared is only place for items. Delete those.

    iNewWatchers->Reset();
    iDisappeared->Close();
    }

// ---------------------------------------------------------------------------
// From CXIMPSubscriptionItemBase class.
// CWatcherListSubscriptionItem::CleanExpired()
// ---------------------------------------------------------------------------
//
void CWatcherListSubscriptionItem::CleanExpired()
    {

    }

// End of file

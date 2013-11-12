/*
 * Copyright (c) 2007 Nokia Corporation and/or its subsidiary(-ies).
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
 * Description:
 *
 */

#include "caclientsubsession.h"
#include "caclientnotifiersession.h"
#include "caclientnotifier.h"
#include "cainnernotifierfilter.h"
#include "casrvdef.h"
#include "cainnerentry.h"
#include "caclientproxy.h"
#include "caobserver.h"
#include "cautils.h"

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
RCaClientSubSession::RCaClientSubSession(
        const RCaClientNotifierSession* aSession,
        const IDataObserver* aObserver,
        const CCaInnerNotifierFilter *aInnerNotifierFilter ) :
    RSubSessionBase(), iSession( aSession ), iObserver( aObserver ),
    iInnerNotifierFilter( aInnerNotifierFilter ),
    iNotifier( NULL ), iMessageSize( NULL ), iChangedEntry( NULL ), 
    iChangedEntryType( EAddChangeType )
    {

    }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void RCaClientSubSession::CreateL()
    {
    if( iNotifier )
        {
        delete iNotifier;
        iNotifier = NULL;
        }
    if( iMessageSize )
        {
        delete iMessageSize;
        iMessageSize = NULL;
        }
    iMessageSize = new ( ELeave ) TPckgBuf<TInt> ();
    User::LeaveIfError( CreateSubSession( *iSession,
        static_cast<TInt>( EContentArsenalNotifierOpen ) ) );
    iNotifier = CCaClientNotifier::NewL( this );
    iChangedEntry = CCaInnerEntry::NewL();    
    }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void RCaClientSubSession::Close()
    {
    delete iChangedEntry;
    iChangedEntry = NULL;
    delete iMessageSize;
    iMessageSize = NULL;
    delete iNotifier;
    iNotifier = NULL;
    CloseSubSession( EContentArsenalNotifierClose );
    }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void RCaClientSubSession::RegisterForNotificationsL(
        TRequestStatus& aStatus ) const
    {
    TIpcArgs args;
    HBufC8* inbuf = MenuUtils::MarshalDataL( *iInnerNotifierFilter,
            KDefaultExpandSize );
    args.Set( KInputPosition1, inbuf );
    args.Set( KOutputPosition, iMessageSize );
    RSubSessionBase::SendReceive( EContentArsenalNotifierNotify, args,
            aStatus );
    }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void RCaClientSubSession::UnregisterForNotifications() const
    {
    RSubSessionBase::SendReceive( EContentArsenalNotifierCancel );
    }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void RCaClientSubSession::NotifyObserver() const
    {
    switch( iInnerNotifierFilter->GetNotifierType() )
        {
        case CCaInnerNotifierFilter::EEntryChangedWithId:
            {
            iObserver->entryChanged( iChangedEntry->GetId(), iChangedEntryType );
            break;
            }
        case CCaInnerNotifierFilter::EEntryChangedWithEntry:
            {
            iObserver->entryChanged( *iChangedEntry, iChangedEntryType );
            break;
            }
        case CCaInnerNotifierFilter::EEntryTouched:
            {
            iObserver->entryTouched( iChangedEntry->GetId() );
            break;
            }
        case CCaInnerNotifierFilter::EGroupContentChanged:
            {
            iObserver->groupContentChanged( iChangedEntry->GetId() );
            break;
            }
        default:
            {
            break;
            }
        }
    }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void RCaClientSubSession::GetChangeInfoL() const
    {
    TIpcArgs args;
    RBuf8 outbuf;
    outbuf.CleanupClosePushL();
    outbuf.CreateL( ( *iMessageSize )() );
    TPckg<TChangeType> changeTypePckg( EAddChangeType );
    args.Set( KInputPosition1, &outbuf );
    args.Set( KInputPosition2, &changeTypePckg );
    TInt error = RSubSessionBase::SendReceive(
            EContentArsenalGetChangeInfo, args );
    if( error == KErrNone )
        {
        RDesReadStream stream( outbuf );
        CleanupClosePushL( stream );
        iChangedEntry->InternalizeL( stream );
        CleanupStack::PopAndDestroy( &stream );
        iChangedEntryType = changeTypePckg();
        }
    CleanupStack::PopAndDestroy( &outbuf );
    }

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

#include "cautils.h"
#include "caclientnotifiersession.h"
#include "caclientsubsession.h"
#include "cainnerentry.h"
#include "cainnerquery.h"
#include "caidsarray.h"
#include "caentriesarray.h"
#include "caclientnotifierproxy.h"


// ======== MEMBER FUNCTIONS ========

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
TUint32 RCaClientNotifierSession::HashFunc(const TKey& key)
    {
    return reinterpret_cast<TUint>(key.iFilterAddress) + key.iNotifierType;
    }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
TBool RCaClientNotifierSession::IdentityFunc(const TKey& key1,
        const TKey& key2)
{
    return key1.iFilterAddress == key2.iFilterAddress &&
        key1.iNotifierType == key2.iNotifierType;
}

const THashFunction32<RCaClientNotifierSession::TKey>
        RCaClientNotifierSession::iHashFunction(
            RCaClientNotifierSession::HashFunc);

const TIdentityRelation<RCaClientNotifierSession::TKey>
        RCaClientNotifierSession::iIdentityRelation(
            RCaClientNotifierSession::IdentityFunc);

// -----------------------------------------------------------------------------
// RCaClientNotifierSession::RCaClientNotifierSession
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
RCaClientNotifierSession::RCaClientNotifierSession() :
    RSessionBase(), iSubsessions(iHashFunction, iIdentityRelation)
{
    // No implementation required
}

// -----------------------------------------------------------------------------
// RCAClientSession::Connect
// Connect to Content publisher server session.
// -----------------------------------------------------------------------------
//
TInt RCaClientNotifierSession::Connect()
{
    TInt error =
            CreateSession(KCaSrvName, Version(), KDefaultMessageSlots);
    if (error == KErrNone) {
        ShareAuto();
    }
    return error;
}

// -----------------------------------------------------------------------------
//
//
// -----------------------------------------------------------------------------
//
void RCaClientNotifierSession::Close()
{
    RHashMap<TKey, RCaClientSubSession*>::TIter iter(iSubsessions);
    while(iter.NextValue()) {
        (*iter.CurrentValue())->Close();
    }
    iSubsessions.Close();
    RSessionBase::Close();
}

// -----------------------------------------------------------------------------
//
//
// -----------------------------------------------------------------------------
//
void RCaClientNotifierSession::ConnectAllL()
{
    if( !CreateSession(KCaSrvName, Version(), KDefaultMessageSlots) )
        {
        RHashMap<TKey, RCaClientSubSession*>::TIter iter(iSubsessions);
        while(iter.NextValue()) 
            {
            (*iter.CurrentValue())->CreateL();
            }
        }
}

// -----------------------------------------------------------------------------
//
//
// -----------------------------------------------------------------------------
//
TInt RCaClientNotifierSession::RegisterNotifier(
        CCaInnerNotifierFilter* innerNotifierFilter,
        const TAny* notifierFilter,
        const IDataObserver* notifierProxy)
{
    TRAPD(error, RegisterNotifierL(innerNotifierFilter,
                    notifierFilter,
                    notifierProxy));
    return error;
}

// -----------------------------------------------------------------------------
//
//
// -----------------------------------------------------------------------------
//
void RCaClientNotifierSession::RegisterNotifierL(
        CCaInnerNotifierFilter* innerNotifierFilter,
        const TAny* notifierFilter,
        const IDataObserver *notifierProxy)
{
    RCaClientSubSession* subsession = new RCaClientSubSession(this,
            notifierProxy, innerNotifierFilter);
    CleanupClosePushL(*subsession);
    subsession->CreateL();
    TKey key(notifierFilter, innerNotifierFilter->GetNotifierType());
    iSubsessions.InsertL(key, subsession);
    CleanupStack::Pop(subsession);
}

// -----------------------------------------------------------------------------
//
//
// -----------------------------------------------------------------------------
//
void RCaClientNotifierSession::UnregisterNotifier(
        const TAny* notifierFilter,
        CCaInnerNotifierFilter::TNotifierType innerNotifierType)
{
    TKey key(notifierFilter, innerNotifierType);
    RCaClientSubSession** subsession = iSubsessions.Find(key);
    if (subsession) {
        (*subsession)->Close();
        delete (*subsession);
        iSubsessions.Remove(key);
    }
}

// -----------------------------------------------------------------------------
//
//
// -----------------------------------------------------------------------------
//
TInt RCaClientNotifierSession::SubsessionsCount() const
{
    return iSubsessions.Count();
}

// -----------------------------------------------------------------------------
//
//
// -----------------------------------------------------------------------------
//
TBool RCaClientNotifierSession::ContainsSubsession(
        const TAny* notifierFilter,
        CCaInnerNotifierFilter::TNotifierType innerNotifierType) const
{
    TKey key(notifierFilter, innerNotifierType);
    return iSubsessions.Find(key) != NULL;
}

// -----------------------------------------------------------------------------
// RCPServerClient::Version
// Version information.
// -----------------------------------------------------------------------------
//
TVersion RCaClientNotifierSession::Version() const
{
    return (TVersion(KCaMajorVersion, KCaMinorVersion, KCaBuild));
}

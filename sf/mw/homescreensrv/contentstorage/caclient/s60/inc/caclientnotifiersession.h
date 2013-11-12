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
 *  Description : Content Arsenal client session for notifications.
 *
 */

#ifndef CACLIENTNOTIFIERSESSION_H
#define CACLIENTNOTIFIERSESSION_H

// INCLUDES
#include <e32base.h>
#include <e32hashtab.h>

#include "caoperationparams.h"
#include "casrvdef.h"
#include "caclientsubsession.h"
#include "cainnernotifierfilter.h"

//FORWARD DECLARATIONS
class CCaInnerQuery;
class RCaEntriesArray;
class RCaIdsArray;
class CaClientProxy;
class CaClientNotifierProxy;

// Number of message slots to reserve for this client server session.
const TUint KDefaultMessageSlots(5);

// CLASS DECLARATION
/**
 * Client-side interface to Content Arsenal Server
 *
 * This class provides the client-side interface to the content arsenal server
 * session, it just passes requests to the server.
 *
 */
class RCaClientNotifierSession: public RSessionBase
    {
public:

    /**
     * Construct the object.
     */
    RCaClientNotifierSession();

    /**
     * Connects to the server and create a session.
     * @return error code
     */
    TInt Connect();

    /**
     * Create subsession.
     * @return error code
     */
    TInt Create(TInt function, const TIpcArgs& args);

    /**
     * Closes session
     *
     * @since S60 v 5.0
     */
    void Close();

    /**
     * Reconnect all subsession
     *
     * @since S60 v 5.0
     */
    void ConnectAllL();

    /**
     * Registers a notifier.
     * @param innerNotifierFilter pointer to inner (symbian style) filter.
     * @param notifierFilter notifier filter address
     * (used for identification purposes).
     * @param notifierProxy client notifier proxy.
     *
     * @since S60 v 5.0
     */
    TInt RegisterNotifier(CCaInnerNotifierFilter *innerNotifierFilter,
            const TAny* notifierFilter,
            const IDataObserver* notifierProxy);

    /**
     * Unregisters a notifier.
     * @param notifierFilter notifier filter address
     * (used for identification purposes).
     * @param innerNotifierType inner notifier type.
     *
     * @since S60 v 5.0
     */
    void UnregisterNotifier(const TAny* notifierFilter,
            CCaInnerNotifierFilter::TNotifierType innerNotifierType);

    /**
     * Returns a number of subsessions (which equals to a number
     * of registered notifiers).
     * @return a number of subsessions.
     *
     * @since S60 v 5.0
     */
    TInt SubsessionsCount() const;

    /**
     * Searches for a subsession.
     * @param notifierFilter notifier filter address
     * (used for identification purposes).
     * @param innerNotifierType inner notifier type.
     * @return true if session contains subsession for given parameters.
     *
     * @since S60 v 5.0
     */
    TBool ContainsSubsession(const TAny* notifierFilter,
            CCaInnerNotifierFilter::TNotifierType innerNotifierType) const;

    /**
     * Return version information
     */
    TVersion Version() const;

private:

    /**
     * Registers a notifier (leaving version).
     * @param innerNotifierFilter pointer to inner (symbian style) filter.
     * @param notifierFilter notifier filter address
     * @param notifierProxy client notifier proxy.
     *
     * @since S60 v 5.0
     */
    void RegisterNotifierL(CCaInnerNotifierFilter* innerNotifierFilter,
            const TAny* notifierFilter,
            const IDataObserver *notifierProxy);

    /*
     * GetSizeL method
     * Return error
     * @param query query
     * @param serverRequest
     * @param resultSize
     */
    TInt GetSizeL(const CCaInnerQuery& query,
            const TCaServerRequests serverRequest, TInt& resultSize);

private:

    /**
     * Class used as a key for a hash map iSubsessions.
     */
    class TKey
        {
    public:    
        TKey(): iFilterAddress(NULL), iNotifierType(0){}
        TKey(const TAny* const filterAddress, const TUint notifierType): 
            iFilterAddress(filterAddress), iNotifierType(notifierType){}
        /**
         * filter address, not own.
         */
        const TAny* const iFilterAddress;
        /**
         * notifier type.
         */
        const TUint iNotifierType;
        };

private:
    /**
     * Hash function used by hash map iSessions.
     * @param key a key (a pair: filter address, notifier type).
     * @return hash value.
     */
    static TUint32 HashFunc(const TKey& key);

    /**
     * Identification function used by hash map iSessions.
     * @param key1 the first key.
     * @param key1 the second key.
     * @return true if keys are equal.
     */
    static TBool IdentityFunc(const TKey& key1, const TKey& key2);

private:
    /**
     * Static member containing a hash function.
     */
    static const THashFunction32<TKey> iHashFunction;

    /**
     * Static member containing an identity function.
     */
    static const TIdentityRelation<TKey> iIdentityRelation;

    /**
     * Hash map containing all subsesions of this session.
     */
    RHashMap<TKey, RCaClientSubSession*> iSubsessions;

    };

#endif // CACLIENTNOTIFIERSESSION_H
// End of File

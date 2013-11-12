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
 *  Description : Content Arsenal client session
 *
 */

#ifndef CACLIENTSUBSESSION_H_
#define CACLIENTSUBSESSION_H_

#include <e32base.h>
#include "cadef.h"

class RCaClientNotifierSession;
class CCaClientNotifier;
class IDataObserver;
class CCaInnerNotifierFilter;
class CCaInnerEntry;

class RCaClientSubSession: public RSubSessionBase
    {
public:
    RCaClientSubSession( const RCaClientNotifierSession* aSession,
            const IDataObserver* aObserver,
            const CCaInnerNotifierFilter* aInnerNotifierFilter );

    /**
     * Create subsession.
     */
    void CreateL();

    /**
     * Close subsession
     */
    void Close();

    /**
     * Sends request for notification reqistering.
     * @param aStatus status.
     */
    void RegisterForNotificationsL( TRequestStatus& aStatus ) const;

    /**
     * Send request for notification unregistering.
     */
    void UnregisterForNotifications() const;

    /**
     * Sends request for getting change information.
     */
    void GetChangeInfoL() const;

    /**
     * Notify observer.
     */
    void NotifyObserver() const;

private:
    /**
     * Not own.
     */
    const RCaClientNotifierSession* iSession;

    /**
     * Not own.
     */
    const IDataObserver* iObserver;

    /**
     * Not own.
     */
    const CCaInnerNotifierFilter* iInnerNotifierFilter;

    /**
     * Own.
     */
    CCaClientNotifier* iNotifier;

    /**
     * Own.
     */
    TPckgBuf<TInt>* iMessageSize;

    /**
     * Own.
     */
    mutable CCaInnerEntry* iChangedEntry;
    
    /**
     * Own.
     */
    mutable TChangeType iChangedEntryType;
    };

#endif /* CACLIENTSUBSESSION_H_ */

/*
* Copyright (c) 2006, 2007 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Event interface for subscribed own presence information.
*
*/

#ifndef MXIMPFWOWNPRESENCEEVENT_H
#define MXIMPFWOWNPRESENCEEVENT_H

#include <e32std.h>
#include <ximpbase.h>
#include <presencemanagementifids.hrh>


class MXIMPIdentity;
class MPresenceInfo;
class MXIMPDataSubscriptionState;



/**
 * Event interface for subscribed own presence information.
 *
 * This interface is implemented by the XIMP Framework and
 * event object is signalled to registered client observers
 * through MXIMPContextObserver observer interface.
 * This event notifies clients about changes in subscribed
 * own presence information.
 *
 * Clients subscribe for this event with
 * MPresencePublishing::SubscribeOwnPresenceL().
 *
 * XIMP Framework signals this event when:
 * - New subscribed data is received from protocol.
 * - Data item data subscription status is updated by
 *   protocol adaptation.
 * - When client unsubscribes the data item.
 * - XIMP Framework generates initial data notification
 *   to client automatically after clients subscription,
 *   when the initial data comes available from protocol.
 *
 * @ingroup ximppresmanagementapievents
 * @since S60 v3.2
 */
class MOwnPresenceEvent : public MXIMPBase
    {
public:
    /** Interface ID for the MOwnPresenceEvent. */
    enum { KInterfaceId = PRES_IF_ID_OWN_PRESENCE_EVENT };


protected:

    /**
     * Protected destructor.
     * Object instancies can't be deleted via this interface.
     */
    virtual ~MOwnPresenceEvent() {}


public:


    /**
     * Returns presence info object holding
     * the subscribed user own presence information.
     *
     * @return The subscribed user own presence info.
     *         Object ownership is not returned.
     */
    virtual const MPresenceInfo& SubscribedPresence() const = 0;


    /**
     * Returns subscription state object holding
     * information data items data subscription state.
     *
     * @return The data items data subscription state.
     *         Object ownership is not returned.
     */
    virtual const MXIMPDataSubscriptionState& DataSubscriptionState() const = 0;


    };


#endif // MXIMPFWOWNPRESENCEEVENT_H




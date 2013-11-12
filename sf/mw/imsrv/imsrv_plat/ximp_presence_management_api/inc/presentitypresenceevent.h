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
* Description:  Event interface for subscribed presentity presence information.
*
*/

#ifndef MXIMPFWPRESENTITYPRESENCEEVENT_H
#define MXIMPFWPRESENTITYPRESENCEEVENT_H

#include <e32std.h>
#include <ximpbase.h>
#include <presencemanagementifids.hrh>


class MXIMPIdentity;
class MPresenceInfo;
class MXIMPDataSubscriptionState;


/**
 * Event interface for subscribed presentity presence information.
 *
 * This interface is implemented by the XIMP Framework and
 * event object is signalled to registered client observers
 * through MXIMPContextObserver observer interface.
 * This event notifies clients about changes in subscribed
 * presentitys presence information.
 *
 * Clients subscribe for this event with
 * MPresenceWatching::SubscribePresentityPresenceL().
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
class MPresentityPresenceEvent : public MXIMPBase
    {
public:
    /** Interface ID for the MPresentityPresenceEvent. */
    enum { KInterfaceId = PRES_IF_ID_PRESENTITY_PRESENCE_EVENT };


protected:

    /**
     * Protected destructor.
     * Object instancies can't be deleted via this interface.
     */
    virtual ~MPresentityPresenceEvent() {}


public:


    /**
     * Returns indetity of the presentity which presence is
     * notified here.
     *
     * @return The presentity identification.
     */
    virtual const MXIMPIdentity& PresentityId() const = 0;


    /**
     * Returns presence info object holding
     * the subscribed presentitys presence information.
     *
     * @return The subscribed presentitys presence info.
     *         Object ownership is not returned.
     */
    virtual const MPresenceInfo& SubscribedPresence() const = 0;


    /**
     * Returns subscription state object holding
     * information about presentity presence subscription
     * current state.
     *
     * @return The presentity presence subscription state object.
     *         Object ownership is not returned.
     */
    virtual const MXIMPDataSubscriptionState& DataSubscriptionState() const = 0;


    };


#endif // MXIMPFWPRESENTITYPRESENCEEVENT_H




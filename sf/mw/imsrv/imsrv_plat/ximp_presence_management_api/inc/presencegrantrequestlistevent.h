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
* Description:  Event interface for presence grant request list content.
*
*/

#ifndef MXIMPFWPRESENCEGRANTREQUESTLISTEVENT_H
#define MXIMPFWPRESENCEGRANTREQUESTLISTEVENT_H

#include <e32std.h>
#include <ximpbase.h>
#include <presencemanagementifids.hrh>


class MXIMPIdentity;
class MPresenceGrantRequestInfo;
class MXIMPDataSubscriptionState;


/**
 * Event interface for presence grant request list content.
 *
 * This interface is implemented by the XIMP Framework and
 * event object is signalled to registered client observers
 * through MXIMPContextObserver observer interface.
 * This event notifies clients about changes in
 * presence grant request list content.
 *
 * Clients subscribe for this event with
 * MPresenceAuthorization::SubscribePresenceGrantRequestListL().
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
class MPresenceGrantRequestListEvent : public MXIMPBase
    {
public:
    /** Interface ID for the MPresenceGrantRequestListEvent. */
    enum { KInterfaceId = PRES_IF_ID_PRESENCE_GRANT_REQUEST_LIST_EVENT };


protected:

    /**
     * Protected destructor.
     * Object instancies can't be deleted via this interface.
     */
    virtual ~MPresenceGrantRequestListEvent() {}



public: // Access to "delta information"


    /**
     * Gets count of new presence grant request entries.
     *
     * @return The count of new presence grant request entries.
     */
    virtual TInt NewRequestsCount() const = 0;


    /**
     * Gets presence grant request entry by index.
     *
     * @param [in] aIndex
     *        The index of the presence grant request entry to get.
     *
     * @return The presence grant request entry from given index.
     */
    virtual const MPresenceGrantRequestInfo& NewRequest(
                TInt aIndex ) const = 0;


    /**
     * Gets count of disappeared presence grant request entries.
     *
     * @return The count of disappeared presence grant request entries.
     */
    virtual TInt DisappearedRequestsCount() const = 0;


    /**
     * Gets presence grant request entry by index.
     *
     * @param [in] aIndex
     *        The index of the presence grant request entry to get.
     *
     * @return The presence grant request entry from given index.
     */
    virtual const MPresenceGrantRequestInfo& DisappearedRequest(
                TInt aIndex ) const = 0;



public: // Access to "current information"


    /**
     * Gets count of current presence grant request entries.
     *
     * Note that, valid "current information" is signalled only when
     * the client refreshes the event data from XIMP Framework cache,
     * by re-issuing the data subscription. In other cases, just plain
     * "delta information" is signalled to client.
     *
     * @return The count of current presence grant request entries.
     */
    virtual TInt CurrentRequestsCount() const = 0;


    /**
     * Gets presence grant request entry by index.
     *
     * Note that, valid "current information" is signalled only when
     * the client refreshes the event data from XIMP Framework cache,
     * by re-issuing the data subscription. In other cases, just plain
     * "delta information" is signalled to client.
     *
     * @param [in] aIndex
     *        The index of the presence grant request entry to get.
     *
     * @return The presence grant request entry from given index.
     */
    virtual const MPresenceGrantRequestInfo& CurrentRequest(
                TInt aIndex ) const = 0;



public: // Access to subscription state


    /**
     * Returns subscription state object holding
     * information about data items data subscription state.
     *
     * @return The data items data subscription state.
     *         Object ownership is not returned.
     */
    virtual const MXIMPDataSubscriptionState& DataSubscriptionState() const = 0;


    };


#endif // MXIMPFWPRESENCEGRANTREQUESTLISTEVENT_H




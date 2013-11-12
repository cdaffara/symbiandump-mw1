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
* Description:  Event interface for group content.
*
*/

#ifndef MIMGROUPEVENT_H
#define MIMGROUPEVENT_H

#include <e32std.h>
#include <ximpbase.h>
#include <immanagementifids.hrh>


class MXIMPIdentity;
class MImGroupInfo;
class MImDataSubscriptionState;


/**
 * Event interface for group content.
 *
 * This interface is implemented by the Im and
 * event object is signalled to registered client observers
 * through MImContextObserver observer interface.
 * This event notifies clients about changes in
 * group content.
 *
 * Clients subscribe for this event with
 * MImGroups::SubscribeImGroupChangeL().
 *
 * Im signals this event when:
 * - New subscribed data is received from protocol.
 * - Data item data subscription status is updated by
 *   protocol adaptation.
 * - When client unsubscribes the data item.
 * - Im generates initial data notification
 *   to client automatically after clients subscription,
 *   when the initial data comes available from protocol.
 *
 * @ingroup imftmanagementapievents
 * 
 */
class MImGroupEvent : public MXIMPBase
    {
public:
    /** Interface ID for the MImGroupEvent. */
    enum { KInterfaceId = IM_IF_ID_GROUP_EVENT };



protected:

    /**
     * Protected destructor.
     * Object instancies can't be deleted via this interface.
     */
    virtual ~MImGroupEvent() {}


public: // Access to "delta information"


    /**
     * Gets count of new groups.
     *
     * @return The count of new groups.
     */
    virtual TInt NewGroupsCount() const = 0;


    /**
     * Gets group entry by index.
     *
     * @param [in] aIndex
     *        The index of the group entry to get.
     *
     * @return The group entry from given index.
     */
    virtual const MImGroupInfo& NewGroup(
                TInt aIndex ) const = 0;


    /**
     * Gets count of updated groups.
     *
     * @return The count of updated groups.
     */
    virtual TInt UpdatedGroupsCount() const = 0;


    /**
     * Gets group entry by index.
     *
     * @param [in] aIndex
     *        The index of the group entry to get.
     *
     * @return The group entry from given index.
     */
    virtual const MImGroupInfo& UpdatedGroup(
                TInt aIndex ) const = 0;


    /**
     * Gets count of disappeared groups.
     *
     * @return The count of disappeared groups.
     */
    virtual TInt DisappearedGroupsCount() const = 0;


    /**
     * Gets group entry by index.
     *
     * @param [in] aIndex
     *        The index of the group entry to get.
     *
     * @return The group entry from given index.
     */
    virtual const MImGroupInfo& DisappearedGroup(
                TInt aIndex ) const = 0;



public: // Access to "current information"

    /**
     * Gets count of current groups.
     *
     * Note that, current information is signalled only when
     * the client refreshes the event data from Im cache,
     * by re-issuing the data subscription.
     *
     * @return The count of current groups.
     */
    virtual TInt CurrentGroupsCount() const = 0;


    /**
     * Gets group entry by index.
     *
     * Note that, current information is signalled only when
     * the client refreshes the event data from Im cache,
     * by re-issuing the data subscription.
     *
     * @param [in] aIndex
     *        The index of the group entry to get.
     *
     * @return The group entry from given index.
     */
    virtual const MImGroupInfo& CurrentGroup(
                TInt aIndex ) const = 0;



public: // Access to subscription state


    /**
     * Returns subscription state object holding
     * information about data items data subscription state.
     *
     * @return The data items data subscription state.
     *         Object ownership is not returned.
     */
    virtual const MImDataSubscriptionState& DataSubscriptionState() const = 0;


    };


#endif // MIMGROUPEVENT_H




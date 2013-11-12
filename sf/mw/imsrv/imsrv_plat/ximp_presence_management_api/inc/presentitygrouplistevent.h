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
* Description:  Event interface for presentity group list content.
*
*/

#ifndef MXIMPFWPRESENTITYGROUPLISTEVENT_H
#define MXIMPFWPRESENTITYGROUPLISTEVENT_H

#include <e32std.h>
#include <ximpbase.h>
#include <presencemanagementifids.hrh>     


class MXIMPIdentity;
class MPresentityGroupInfo;
class MXIMPDataSubscriptionState;


/**
 * Event interface for presentity group list content.
 *
 * This interface is implemented by the XIMP Framework and
 * event object is signalled to registered client observers
 * through MXIMPContextObserver observer interface.
 * This event notifies clients about changes in
 * presentity group list content.
 *
 * Clients subscribe for this event with
 * MPresentityGroups::SubscribePresentityGroupListL().
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
class MPresentityGroupListEvent : public MXIMPBase
    {
public:
    /** Interface ID for the MPresentityGroupListEvent. */
    enum { KInterfaceId = PRES_IF_ID_PRESENTITY_GROUP_LIST_EVENT };



protected:

    /**
     * Protected destructor.
     * Object instancies can't be deleted via this interface.
     */
    virtual ~MPresentityGroupListEvent() {}


public: // Access to "delta information"


    /**
     * Gets count of new presentity groups.
     *
     * @return The count of new presentity groups.
     */
    virtual TInt NewGroupsCount() const = 0;


    /**
     * Gets presentity group entry by index.
     *
     * @param [in] aIndex
     *        The index of the presentity group entry to get.
     *
     * @return The presentity group entry from given index.
     */
    virtual const MPresentityGroupInfo& NewGroup(
                TInt aIndex ) const = 0;


    /**
     * Gets count of updated presentity groups.
     *
     * @return The count of updated presentity groups.
     */
    virtual TInt UpdatedGroupsCount() const = 0;


    /**
     * Gets presentity group entry by index.
     *
     * @param [in] aIndex
     *        The index of the presentity group entry to get.
     *
     * @return The presentity group entry from given index.
     */
    virtual const MPresentityGroupInfo& UpdatedGroup(
                TInt aIndex ) const = 0;


    /**
     * Gets count of disappeared presentity groups.
     *
     * @return The count of disappeared presentity groups.
     */
    virtual TInt DisappearedGroupsCount() const = 0;


    /**
     * Gets presentity group entry by index.
     *
     * @param [in] aIndex
     *        The index of the presentity group entry to get.
     *
     * @return The presentity group entry from given index.
     */
    virtual const MPresentityGroupInfo& DisappearedGroup(
                TInt aIndex ) const = 0;



public: // Access to "current information"

    /**
     * Gets count of current presentity groups.
     *
     * Note that, current information is signalled only when
     * the client refreshes the event data from XIMP Framework cache,
     * by re-issuing the data subscription.
     *
     * @return The count of current presentity groups.
     */
    virtual TInt CurrentGroupsCount() const = 0;


    /**
     * Gets presentity group entry by index.
     *
     * Note that, current information is signalled only when
     * the client refreshes the event data from XIMP Framework cache,
     * by re-issuing the data subscription.
     *
     * @param [in] aIndex
     *        The index of the presentity group entry to get.
     *
     * @return The presentity group entry from given index.
     */
    virtual const MPresentityGroupInfo& CurrentGroup(
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


#endif // MXIMPFWPRESENTITYGROUPLISTEVENT_H




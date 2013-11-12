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
* Description:  Event interface for presentity group content.
*
*/

#ifndef MXIMPFWPRESENTITYGROUPCONTENTEVENT_H
#define MXIMPFWPRESENTITYGROUPCONTENTEVENT_H

#include <e32std.h>
#include <ximpbase.h>
#include <presencemanagementifids.hrh>


class MXIMPIdentity;
class MPresentityGroupMemberInfo;
class MXIMPDataSubscriptionState;



/**
 * Event interface for presentity group content event.
 *
 * This interface is implemented by the XIMP Framework and
 * event object is signalled to registered client observers
 * through MXIMPContextObserver observer interface.
 * This event notifies clients about changes in
 * presentity group content.
 *
 * Clients subscribe for this event with
 * MPresentityGroups::SubscribePresentityGroupContentL().
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
class MPresentityGroupContentEvent : public MXIMPBase
    {
public:
    /** Interface ID for the MPresentityGroupContentEvent. */
    enum { KInterfaceId = PRES_IF_ID_PRESENTITY_GROUP_CONTENT_EVENT };


protected:

    /**
     * Protected destructor.
     * Object instancies can't be deleted via this interface.
     */
    virtual ~MPresentityGroupContentEvent() {};


public: // Access to "delta information"


    /**
     * Presentity group identity information.
     */
    virtual const MXIMPIdentity& GroupId() const = 0;



    /**
     * Gets count of new presentity group members.
     *
     * @return The count of new presentity group members.
     */
    virtual TInt NewMembersCount() const = 0;


    /**
     * Gets presentity group member by index.
     *
     * @param [in] aIndex
     *        The index of the presentity group member to get.
     *
     * @return The presentity group member from given index.
     */
    virtual const MPresentityGroupMemberInfo& NewMember(
                TInt aIndex ) const = 0;


    /**
     * Gets count of updated presentity group members.
     *
     * @return The count of updated presentity group members.
     */
    virtual TInt UpdatedMembersCount() const = 0;


    /**
     * Gets presentity group member by index.
     *
     * @param [in] aIndex
     *        The index of the presentity group member to get.
     *
     * @return The presentity group member from given index.
     */
    virtual const MPresentityGroupMemberInfo& UpdatedMember(
                TInt aIndex ) const = 0;


    /**
     * Gets count of disappeared presentity group members.
     *
     * @return The count of disappeared presentity group members.
     */
    virtual TInt DisappearedMembersCount() const = 0;


    /**
     * Gets presentity group member by index.
     *
     * @param [in] aIndex
     *        The index of the presentity group member to get.
     *
     * @return The presentity group member from given index.
     */
    virtual const MPresentityGroupMemberInfo& DisappearedMember(
                TInt aIndex ) const = 0;



public: // Access to "current information"

    /**
     * Gets count of current presentity group members.
     *
     * Note that, current information is signalled only when
     * the client refreshes the event data from XIMP Framework cache,
     * by re-issuing the data subscription.
     *
     * @return The count of current presentity group members.
     */
    virtual TInt CurrentMembersCount() const = 0;



    /**
     * Gets presentity group member by index.
     *
     * @param [in] aIndex
     *        The index of the presentity group member to get.
     *
     * @return The presentity group member from given index.
     */
    virtual const MPresentityGroupMemberInfo& CurrentMember(
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


#endif // MXIMPFWPRESENTITYGROUPCONTENTEVENT_H




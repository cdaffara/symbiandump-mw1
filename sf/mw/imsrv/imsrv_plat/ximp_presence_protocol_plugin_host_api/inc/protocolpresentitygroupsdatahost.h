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
* Description:  Interface for managing cached presentity group information.
*
*/

#ifndef MPROTOCOLPRESENTITYGROUPSDATAHOST_H
#define MPROTOCOLPRESENTITYGROUPSDATAHOST_H


#include <e32base.h>
#include <presenceprotocolpluginhostifids.hrh>
#include <ximpbase.h>


class MXIMPIdentity;
class MXIMPObjectCollection;
class MPresentityGroupInfo;
class MPresentityGroupMemberInfo;
class MXIMPDataSubscriptionState;
class MXIMPStatus;



/**
 * Interface for managing cached presentity group information.
 *
 * Interface used by protocol adaptations to manage
 * manage presentity group information and presentity
 * group member information in XIMP Framework
 * internal data cache.
 *
 * This interface is implemented by XIMP Framework.
 * Protocol adaptation can obtain reference to this
 * interface from MXIMPProtocolConnectionHost interface.
 *
 * For each separated protocol connection, protocol
 * adaptation must use the specific
 * MProtocolPresentityGroupsDataHost instance,
 * what is received from the connection specific
 * MXIMPProtocolConnectionHost instance.
 *
 * @ingroup ximppresprotocolpluginhostapi
 * @since S60 v3.2
 */
class MProtocolPresentityGroupsDataHost : public MXIMPBase
    {
public:

    /** Interface ID for the MProtocolPresentityGroupsDataHost. */
    enum { KInterfaceId = PRES_IF_ID_PROTOCOL_PRESENTITY_GROUPS_DATAHOST };

protected:

    /**
     * Protected destructor. Object instancies
     * can't be deleted via this interface.
     */
    virtual ~MProtocolPresentityGroupsDataHost() {}



public: // Handling presentity group information


    /**
     * Notifies XIMP Framework from
     * presentity group list contents.
     *
     * Protocol adaptation implementation can call this
     * method to notify XIMP Framework from
     * complete presentity group list content.
     * XIMP Framework calculates the changes
     * (new and removed presentity groups) compared to previous
     * presence presentity group list content, and notifies
     * the subscribing clients from data changes.
     *
     * XIMP Framework takes ownership of the object
     * collection and all objects contained within, if
     * the method succeeds without a leave.
     *
     *
     * @param [in] aGroupList
     *        MXIMPObjectCollection object containing zero or
     *        more MPresentityGroupInfo objects. Each
     *        contained MPresentityGroupInfo shall present
     *        single presentity group within the presentity
     *        group list.
     *
     * @return The request ID identifying the issued request.
     *         Data cache request is processed asynchronously and
     *         its completion is signalled to adaptation direction
     *         through MXIMPProtocolConnectionHostObserver interface,
     *         with MXIMPProtocolRequestCompleteEvent event.
     *
     * @leave KErrNoMemory if failed to allocate memory.
     * @leave KErrArgument if NULL argument(s) were provided.
     */
    virtual TXIMPRequestId HandlePresentityGroupListL(
                MXIMPObjectCollection* aGroupList ) = 0;



    /**
     * Notifies XIMP Framework from new
     * presentity group list entry.
     *
     * Protocol adaptation implementation can call this
     * method to notify XIMP Framework from
     * new presentity group list entry. XIMP Framework
     * calculates the change, from previous presentity
     * group list content, and notifies the subscribing
     * clients from data changes.
     *
     * XIMP Framework takes ownership of the argument(s),
     * if method succeeds without a leave.
     *
     * @param [in] aGroupInfo
     *        MPresentityGroupInfo object holding
     *        information about new presentity group entry.
     *        New presentity group is added to the presentity
     *        group list.
     *
     * @return The request ID identifying the issued request.
     *         Data cache request is processed asynchronously and
     *         its completion is signalled to adaptation direction
     *         through MXIMPProtocolConnectionHostObserver interface,
     *         with MXIMPProtocolRequestCompleteEvent event.
     *
     * @leave KErrNoMemory if failed to allocate memory.
     * @leave KErrArgument if NULL argument(s) were provided.
     */
    virtual TXIMPRequestId HandlePresentityGroupCreatedL(
                MPresentityGroupInfo* aGroupInfo ) = 0;



    /**
     * Notifies XIMP Framework from deletion of
     * existing presentity group.
     *
     * Protocol adaptation implementation can call this
     * method to notify XIMP Framework from deletion
     * of existing presentity group. XIMP Framework
     * calculates the change, from previous presentity group
     * list content, and notifies the subscribing clients
     * from data changes.
     *
     * XIMP Framework takes ownership of the argument(s),
     * if method succeeds without a leave.
     *
     * @param [in] aGroupId
     *        MXIMPIdentity object identifying the
     *        presentity group which were deleted and thus
     *        needs to be removed from presentity group list.
     *
     * @return The request ID identifying the issued request.
     *         Data cache request is processed asynchronously and
     *         its completion is signalled to adaptation direction
     *         through MXIMPProtocolConnectionHostObserver interface,
     *         with MXIMPProtocolRequestCompleteEvent event.
     *
     * @leave KErrNoMemory if failed to allocate memory.
     * @leave KErrArgument if NULL argument(s) were provided.
     */
    virtual TXIMPRequestId HandlePresentityGroupDeletedL(
                MXIMPIdentity* aGroupId ) = 0;



    /**
     * Notifies XIMP Framework from update of
     * existing presentity group displayname.
     *
     * Protocol adaptation implementation can call this
     * method to notify XIMP Framework from update
     * of existing presentity group displayname. XIMP Framework
     * calculates the change, from previous presentity group
     * list content, and notifies the subscribing clients
     * from data changes.
     *
     * XIMP Framework takes ownership of the argument(s),
     * if method succeeds without a leave.
     *
     * @param [in] aGroupInfo
     *        MPresentityGroupInfo object identifying the
     *        presentity group and new displayname for the group.
     *
     * @return The request ID identifying the issued request.
     *         Data cache request is processed asynchronously and
     *         its completion is signalled to adaptation direction
     *         through MXIMPProtocolConnectionHostObserver interface,
     *         with MXIMPProtocolRequestCompleteEvent event.
     *
     * @leave KErrNoMemory if failed to allocate memory.
     * @leave KErrArgument if NULL argument(s) were provided.
     */
    virtual TXIMPRequestId HandlePresentityGroupDisplayNameUpdatedL(
                MPresentityGroupInfo* aGroupInfo ) = 0;



    /**
     * Gets data subscription state for presentity group list.
     *
     * Protocol adaptation can call this method to retrieve
     * presentity group lists data subscription state.
     * Protocol adaptation can use data subscription state
     * information, to optimize its own processing and
     * network trafic amount.
     *
     * @return Presentity group list data subscription state.
     *         Interface ownership is not returned to caller.
     *         Caller shouldn't cache the returned reference
     *         for later use.
     */
    virtual const MXIMPDataSubscriptionState&
                PresentityGroupListDataSubscriptionState() const = 0;



    /**
     * Updates data subscription state for presentity group list.
     *
     * Updates data subscription state for presentity group list.
     * XIMP Framework calculates the subscription state change,
     * from previous presentity group list, and
     * notifies the subscribing clients from data changes.
     *
     * XIMP Framework takes ownership of the argument(s),
     * if method succeeds without a leave.
     *
     * @param [in] aSubscriptionState
     *        New data subscription state for presentity group list.
     *
     * @param [in] aStatus
     *        Optional status object describing the
     *        the reason, why protocol adaptation updated
     *        data item subscription state. Given status object
     *        is passed to clients through data item specific
     *        data change event. NULL value is accepted.
     *
     * @return The request ID identifying the issued request.
     *         Data cache request is processed asynchronously and
     *         its completion is signalled to adaptation direction
     *         through MXIMPProtocolConnectionHostObserver interface,
     *         with MXIMPProtocolRequestCompleteEvent event.
     *
     * @leave KErrNoMemory if failed to allocate memory.
     * @leave KErrArgument if NULL subscription state argument was provided.
     */
    virtual TXIMPRequestId SetPresentityGroupListDataSubscriptionStateL(
                MXIMPDataSubscriptionState* aSubscriptionState,
                MXIMPStatus* aStatus ) = 0;



public: // Handling presentity group content


    /**
     * Notifies XIMP Framework from
     * presentity group contents.
     *
     * Protocol adaptation implementation can call this
     * method to notify XIMP Framework from presentity group
     * content. XIMP Framework calculates the changes
     * (added and removed group members) compared to previous
     * presentity group content, and notifies the subscribing
     * clients from data changes.
     *
     * XIMP Framework takes ownership of the object
     * collection and all objects contained within, if
     * the method succeeds without a leave.
     *
     * @param [in] aGroupId
     *        MXIMPIdentity object identifying the presentity group
     *        of which content is notified here.
     *
     * @param [in] aGroupMembers
     *        MXIMPObjectCollection object containing zero or
     *        more MPresentityGroupMemberInfo objects. Each
     *        contained MPresentityGroupMemberInfo shall
     *        present single presentity group member, within
     *        the presentity group.
     *
     * @return The request ID identifying the issued request.
     *         Data cache request is processed asynchronously and
     *         its completion is signalled to adaptation direction
     *         through MXIMPProtocolConnectionHostObserver interface,
     *         with MXIMPProtocolRequestCompleteEvent event.
     *
     * @leave KErrNoMemory if failed to allocate memory.
     * @leave KErrArgument if NULL argument(s) were provided.
     */
    virtual TXIMPRequestId HandlePresentityGroupContentL(
                MXIMPIdentity* aGroupId,
                MXIMPObjectCollection* aGroupMembers ) = 0;



    /**
     * Notifies XIMP Framework from new
     * presentity group member entry.
     *
     * Protocol adaptation implementation can call this
     * method to notify XIMP Framework from
     * new presentity group member entry. XIMP Framework
     * calculates the change, from previous presentity
     * group content, and notifies the subscribing
     * clients from data changes.
     *
     * XIMP Framework takes ownership of the argument(s),
     * if method succeeds without a leave.
     *
     * @param [in] aGroupId
     *        MXIMPIdentity object identifying the presentity
     *        group to where the group member is added.
     *
     * @param [in] aGroupMember
     *        MPresentityGroupMemberInfo object holding
     *        information about new presentity group member
     *        entry. New presentity group member is added
     *        to the presentity group content.
     *
     * @return The request ID identifying the issued request.
     *         Data cache request is processed asynchronously and
     *         its completion is signalled to adaptation direction
     *         through MXIMPProtocolConnectionHostObserver interface,
     *         with MXIMPProtocolRequestCompleteEvent event.
     *
     * @leave KErrNoMemory if failed to allocate memory.
     * @leave KErrArgument if NULL argument(s) were provided.
     */
    virtual TXIMPRequestId HandlePresentityGroupMemberAddedL(
                MXIMPIdentity* aGroupId,
                MPresentityGroupMemberInfo* aGroupMember ) = 0;



    /**
     * Notifies XIMP Framework from remove of
     * existing presentity group member.
     *
     * Protocol adaptation implementation can call this
     * method to notify XIMP Framework from remove
     * of existing presentity group member. XIMP Framework
     * calculates the change, from previous presentity group
     * content, and notifies the subscribing clients
     * from data changes.
     *
     * XIMP Framework takes ownership of the argument(s),
     * if method succeeds without a leave.
     *
     * @param [in] aGroupId
     *        MXIMPIdentity object identifying the presentity
     *        group from where to remove the group member.
     *
     * @param [in] aGroupMemberId
     *        MXIMPIdentity object identifying the presentity
     *        group member which to remove from the presentity
     *        group.
     *
     * @return The request ID identifying the issued request.
     *         Data cache request is processed asynchronously and
     *         its completion is signalled to adaptation direction
     *         through MXIMPProtocolConnectionHostObserver interface,
     *         with MXIMPProtocolRequestCompleteEvent event.
     *
     * @leave KErrNoMemory if failed to allocate memory.
     * @leave KErrArgument if NULL argument(s) were provided.
     */
    virtual TXIMPRequestId HandlePresentityGroupMemberRemovedL(
                MXIMPIdentity* aGroupId,
                MXIMPIdentity* aGroupMemberId ) = 0;



    /**
     * Notifies XIMP Framework from displayname update of
     * existing presentity group member.
     *
     * Protocol adaptation implementation can call this
     * method to notify XIMP Framework from displayname
     * update of existing presentity group member.
     * XIMP Framework calculates the change, from previous
     * presentity group content, and notifies the subscribing
     * clients from data changes.
     *
     * XIMP Framework takes ownership of the argument(s),
     * if method succeeds without a leave.
     *
     * @param [in] aGroupId
     *        MXIMPIdentity object identifying the presentity
     *        group which member displayname to update.
     *
     * @param [in] aGroupMember
     *        MPresentityGroupMemberInfo object identifying
     *        the presentity group member and its new displayname.
     *
     * @return The request ID identifying the issued request.
     *         Data cache request is processed asynchronously and
     *         its completion is signalled to adaptation direction
     *         through MXIMPProtocolConnectionHostObserver interface,
     *         with MXIMPProtocolRequestCompleteEvent event.
     *
     * @leave KErrNoMemory if failed to allocate memory.
     * @leave KErrArgument if NULL argument(s) were provided.
     */
    virtual TXIMPRequestId HandlePresentityGroupMemberDisplayNameUpdatedL(
                MXIMPIdentity* aGroupId,
                MPresentityGroupMemberInfo* aGroupMember ) = 0;



    /**
     * Gets data subscription state for presentity group.
     *
     * Protocol adaptation can call this method to retrieve
     * given presentity groups content subscription state.
     * Protocol adaptation can use data subscription state
     * information, to optimize its own processing and
     * network trafic amount.
     *
     * @return Presentity groups content subscription state.
     *         Interface ownership is not returned to caller.
     *         Caller shouldn't cache the returned reference
     *         for later use.
     */
    virtual const MXIMPDataSubscriptionState&
                PresentityGroupContentDataSubscriptionState(
                const MXIMPIdentity& aGroupId ) const = 0;



    /**
     * Updates data subscription state for
     * presentity group content.
     *
     * Updates data subscription state for presentity group content.
     * XIMP Framework calculates the subscription state change,
     * from previous presentity group content, and
     * notifies the subscribing clients from data changes.
     *
     * XIMP Framework takes ownership of the argument(s),
     * if method succeeds without a leave.
     *
     * @param [in] aGroupId
     *        MXIMPIdentity object identifying the presentity
     *        group of which data subscription state is
     *        updated here.
     *
     * @param [in] aSubscriptionState
     *        New data subscription state for presentity group .
     *
     * @param [in] aStatus
     *        Optional status object describing the
     *        the reason, why protocol adaptation updated
     *        data item subscription state. Given status object
     *        is passed to clients through data item specific
     *        data change event. NULL value is accepted.
     *
     * @return The request ID identifying the issued request.
     *         Data cache request is processed asynchronously and
     *         its completion is signalled to adaptation direction
     *         through MXIMPProtocolConnectionHostObserver interface,
     *         with MXIMPProtocolRequestCompleteEvent event.
     *
     * @leave KErrNoMemory if failed to allocate memory.
     * @leave KErrArgument if NULL subscription state argument was provided.
     */
    virtual TXIMPRequestId SetPresentityGroupContentDataSubscriptionStateL(
                MXIMPIdentity* aGroupId,
                MXIMPDataSubscriptionState* aSubscriptionState,
                MXIMPStatus* aStatus ) = 0;


    };


#endif // MPROTOCOLPRESENTITYGROUPSDATAHOST_H

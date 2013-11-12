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
* Description:  Interface for managing cached group information.
*
*/

#ifndef MPROTOCOLIMGROUPDATAHOST_H
#define MPROTOCOLIMGROUPDATAHOST_H

#include <e32base.h>
#include <protocolimimpluginhostifids.hrh>
#include <ximpbase.h>

class MXIMPIdentity;
class MImGroupInfo;
class MImGroupMemberInfo;

/**
 * Interface for managing cached group information.
 *
 * Interface used by protocol adaptations to manage
 * group information and group member information 
 * in IM internal data cache.
 *
 * This interface is implemented by IM.
 * Protocol adaptation can obtain reference to this
 * interface from MXIMPProtocolConnectionHost interface.
 *
 * For each separated protocol connection, protocol
 * adaptation must use the specific
 * MProtocolImuginroupDataHost instance,
 * what is received from the connection specific
 * MXIMPProtocolConnectionHost instance.
 *
 * @ingroup ProtocolImpluginhostapi
 * @since S60 
 */
class MProtocolImGroupDataHost : public MXIMPBase
    {
public:

    /** Interface ID for the MProtocolImuginroupDataHost. */
    enum { KInterfaceId = IM_IF_ID_PROTOCOL_GROUP_DATAHOST };

protected:

    /**
     * Protected destructor. Object instancies
     * can't be deleted via this interface.
     */
    virtual ~MProtocolImGroupDataHost() {}



public: // Handling group information
 /**
     * Notifies IM from
     * IM group list contents.
     *
     * Protocol adaptation implementation can call this
     * method to notify IM from
     * complete Im group list content.
     * IM calculates the changes
     * (new and removed Im groups) compared to previous
     * IM group list content, and notifies
     * the subscribing clients from data changes.
     *
     * IM takes ownership of the object
     * collection and all objects contained within, if
     * the method succeeds without a leave.
     *
     *
     * @param [in] aGroupList
     *        MImObjectCollection object containing zero or
     *        more MImGroupInfo objects. Each
     *        contained MImGroupInfo shall im
     *        single im group within the im
     *        group list.
     *
     * @return The request id identifying the issued request.
     *         Data cache request is processed asynchronously and
     *         its completion is signalled to adaptation direction
     *         through MXIMPProtocolConnectionHostObserver interface,
     *         with MXIMPProtocolRequestCompleteEvent event.
     *
     * @leave KErrNoMemory if failed to allocate memory.
     * @leave KErrArgument if NULL argument(s) were provided.
     */
    virtual TXIMPRequestId HandleImGroupListL(
                MImObjectCollection* aGroupList ) = 0;
    /**
     * Notifies IM from new
     * group created.
     *
     * Protocol adaptation implementation can call this
     * method to notify IM from
     * new group created. IM
     * calculates notifies the subscribing
     * clients from data changes.
     *
     * IM takes ownership of the argument(s),
     * if method succeeds without a leave.
     *
     * @param [in] aGroupInfo
     *        MImGroupInfo object holding
     *        information about new group entry.
     *
     * @return The request id identifying the issued request.
     *         Data cache request is processed asynchronously and
     *         its completion is signalled to adaptation direction
     *         through MXIMPProtocolConnectionHostObserver interface,
     *         with MXIMPProtocolRequestCompleteEvent event.
     *
     * @leave KErrNoMemory if failed to allocate memory.
     * @leave KErrArgument if NULL argument(s) were provided.
     */
    virtual TXIMPRequestId HandleGroupCreatedL(
                MImGroupInfo* aGroupInfo ) = 0;


	/**
     * Notifies IM from joined
     * group members.
     *
     * Protocol adaptation implementation can call this
     * method to notify IM from
     * Joined group members. IM
     * calculates notifies the subscribing
     * clients from data changes.
     *
     * IM takes ownership of the argument(s),
     * if method succeeds without a leave.
     *
     * @param [in] aGroupInfo
     *        MImGroupInfo object holding
     *        information about group members.
     *
     * @return The request id identifying the issued request.
     *         data cache request is processed asynchronously and
     *         its completion is signalled to adaptation direction
     *         through MXIMPProtocolConnectionHostObserver interface,
     *         with MXIMPProtocolRequestCompleteEvent event.
     *
     * @leave KErrNoMemory if failed to allocate memory.
     * @leave KErrArgument if NULL argument(s) were provided.
     */
    virtual TXIMPRequestId HandleJoinedGroupL(
                MImGroupInfo* aGroupInfo ) = 0;

    /**
     * Notifies IM from Left
     * group members.
     *
     * Protocol adaptation implementation can call this
     * method to notify IM from
     * left group. IM
     * calculates notifies the subscribing
     * clients from data changes.
     *
     * IM takes ownership of the argument(s),
     * if method succeeds without a leave.
     *
     * @param [in] aGroupId
     *        MXIMPIdentity object holding group id
     *
     * @return The request id identifying the issued request.
     *         data cache request is processed asynchronously and
     *         its completion is signalled to adaptation direction
     *         through MXIMPProtocolConnectionHostObserver interface,
     *         with MXIMPProtocolRequestCompleteEvent event.
     *
     * @leave KErrNoMemory if failed to allocate memory.
     * @leave KErrArgument if NULL argument(s) were provided.
     */
    virtual TXIMPRequestId HandleLeaveGroupL(MXIMPIdentity* aGroupId) = 0;

	/**
     * Notifies IM from 
     * deleted Group .
     *
     * Protocol adaptation implementation can call this
     * method to notify IM from
     * deleted group. IM
     * calculates notifies the subscribing
     * clients from data changes.
     *
     * IM takes ownership of the argument(s),
     * if method succeeds without a leave.
     *
     * @param [in] aGroupId
     *        MXIMPIdentity object holding group id
     *
     * @return The request id identifying the issued request.
     *         Data cache request is processed asynchronously and
     *         its completion is signalled to adaptation direction
     *         through MXIMPProtocolConnectionHostObserver interface,
     *         with MXIMPProtocolRequestCompleteEvent event.
     *
     * @leave KErrNoMemory if failed to allocate memory.
     * @leave KErrArgument if NULL argument(s) were provided.
     */
    virtual TXIMPRequestId HandleDeleteGroupL(MXIMPIdentity* aGroupId) = 0;

	/**
     * Notifies IM from 
     * group properties.
     *
     * Protocol adaptation implementation can call this
     * method to notify IM from
     * group properties. IM
     * calculates notifies the subscribing
     * clients from data changes.
     *
     * IM takes ownership of the argument(s),
     * if method succeeds without a leave.
     *
     * @param [in] aGroupInfo
     *        MImGroupInfo object holding
     *        information about group properties.
     *
     * @return The request id identifying the issued request.
     *         Data cache request is processed asynchronously and
     *         its completion is signalled to adaptation direction
     *         through MXIMPProtocolConnectionHostObserver interface,
     *         with MXIMPProtocolRequestCompleteEvent event.
     *
     * @leave KErrNoMemory if failed to allocate memory.
     * @leave KErrArgument if NULL argument(s) were provided.
     */
    virtual TXIMPRequestId HandleGroupPropertiesL(
                MImGroupInfo* aGroupInfo ) = 0;
                
                
     /**
     * Notifies IM from 
     * group subscription information.
     *
     * Protocol adaptation implementation can call this
     * method to notify IM from
     * group subscription information. IM
     * calculates notifies the subscribing
     * clients from data changes.
     *
     * IM takes ownership of the argument(s),
     * if method succeeds without a leave.
     *
     * @param [in] aSubscription
     *        Whether subscribed to group
     *
     * @return The request id identifying the issued request.
     *         Data cache request is processed asynchronously and
     *         its completion is signalled to adaptation direction
     *         through MXIMPProtocolConnectionHostObserver interface,
     *         with MXIMPProtocolRequestCompleteEvent event.
     *
     * @leave KErrNoMemory if failed to allocate memory.
     * @leave KErrArgument if NULL argument(s) were provided.
     */
    virtual TXIMPRequestId HandleGroupPropertiesL(
                TBool aSubscription ) = 0;
     /**
     * Notifies IM from
     * IM group access control list contents.
     *
     * Protocol adaptation implementation can call this
     * method to notify IM from
     * complete Im group access control list content.
     * IM calculates the changes
     * (new and removed Im groups) compared to previous
     * Im group list content, and notifies
     * the subscribing clients from data changes.
     *
     * IM takes ownership of the object
     * collection and all objects contained within, if
     * the method succeeds without a leave.
     *
     *
     * @param [in] accesscontrollist
     *        MImObjectCollection object containing zero or
     *        more MImAccessControlList objects. Each
     *        contained MImAccessControlList shall present
     *        single access control list for a  group within the im
     *        group list.
     *
     * @return The request id identifying the issued request.
     *         Data cache request is processed asynchronously and
     *         its completion is signalled to adaptation direction
     *         through MXIMPProtocolConnectionHostObserver interface,
     *         with MXIMPProtocolRequestCompleteEvent event.
     *
     * @leave KErrNoMemory if failed to allocate memory.
     * @leave KErrArgument if NULL argument(s) were provided.
     */
    virtual TXIMPRequestId HandleImAccessControlListL(
                MImObjectCollection* accesscontrollist ) = 0;
     /**
     * Notifies IM from
     * IM group members.
     *
     * Protocol adaptation implementation can call this
     * method to notify IM from IM group
     * members. IM calculates the changes
     * (added and removed group members) compared to previous
     * IM group content, and notifies the subscribing
     * clients from data changes.
     *
     * IM takes ownership of the object
     * collection and all objects contained within, if
     * the method succeeds without a leave.
     *
     * @param [in] aGroupId
     *        MXIMPIdentity object identifying the im group
     *        of which content is notified here.
     *
     * @param [in] aGroupMembers
     *        MImObjectCollection object containing zero or
     *        more MImGroupInfo objects. Each
     *        contained MImGroupInfo shall
     *        present single im group member, within
     *        the im group.
     *
     * @return The request id identifying the issued request.
     *         Data cache request is processed asynchronously and
     *         its completion is signalled to adaptation direction
     *         through MXIMPProtocolConnectionHostObserver interface,
     *         with MXIMPProtocolRequestCompleteEvent event.
     *
     * @leave KErrNoMemory if failed to allocate memory.
     * @leave KErrArgument if NULL argument(s) were provided.
     */
    virtual TPrFwRequestId HandleImGroupMembersL(
                MXIMPIdentity* aGroupId,
                MImObjectCollection* aGroupMembers ) = 0;
/**
     * Notifies IM from
     * im group joined users.
     *
     * Protocol adaptation implementation can call this
     * method to notify IM from im group
     * joined users. IM calculates the changes
     * (added and removed group members) compared to previous
     * Im group content, and notifies the subscribing
     * clients from data changes.
     *
     * IM takes ownership of the object
     * collection and all objects contained within, if
     * the method succeeds without a leave.
     *
     * @param [in] aGroupId
     *        MXIMPIdentity object identifying the im group
     *        of which content is notified here.
     * @param [in] aGroupJoinedUsers
     *        MImObjectCollection object containing zero or
     *        more MImGroupInfo objects. Each
     *        contained MImGroupInfo shall
     *        present single im group joined user, within
     *        the im group.
     *
     * @return The request id identifying the issued request.
     *         Data cache request is processed asynchronously and
     *         its completion is signalled to adaptation direction
     *         through MXIMPProtocolConnectionHostObserver interface,
     *         with MXIMPProtocolRequestCompleteEvent event.
     *
     * @leave KErrNoMemory if failed to allocate memory.
     * @leave KErrArgument if NULL argument(s) were provided.
     */
    virtual TPrFwRequestId HandleImGroupJoinedUsersListL(
                MXIMPIdentity* aGroupId,
                MImObjectCollection* aGroupJoinedUsers ) = 0;



    /**
     * Notifies IM from new
     * im group member entry.
     *
     * Protocol adaptation implementation can call this
     * method to notify IM from
     * new im group member entry. IM
     * calculates the change, from previous im
     * group content, and notifies the subscribing
     * clients from data changes.
     *
     * IM takes ownership of the argument(s),
     * if method succeeds without a leave.
     *
     * @param [in] aGroupId
     *        MPrFwIdentity object identifying the im
     *        group to where the group member is added.
     * @param [in] aGroupMember
     *        MImGroupInfo object holding
     *        information about new im group member
     *        entry. New im group member is added
     *        to the im group content.
     *
      * @return The request id identifying the issued request.
     *         Data cache request is processed asynchronously and
     *         its completion is signalled to adaptation direction
     *         through MXIMPProtocolConnectionHostObserver interface,
     *         with MXIMPProtocolRequestCompleteEvent event.
     *
     * @leave KErrNoMemory if failed to allocate memory.
     * @leave KErrArgument if NULL argument(s) were provided.
     */
    virtual TPrFwRequestId HandleImGroupMemberAddedL(
                MPrFwIdentity* aGroupId,
                MImGroupInfo* aGroupMember ) = 0;

     /**
     * Notifies IM from new
     * im group member entry.
     *
     * Protocol adaptation implementation can call this
     * method to notify IM from
     * remove of existing group member entry. IM
     * calculates the change, from previous im
     * group content, and notifies the subscribing
     * clients from data changes.
     *
     * IM takes ownership of the argument(s),
     * if method succeeds without a leave.
     *
     * @param [in] aGroupId
     *        MPrFwIdentity object identifying the im
     *        group to where the group member is added.
     *
     * @param [in] aGroupMember
     *        MPrFwIdentity object identifying the im
     *        group member which to remove from the im
     *        group.
     *
      * @return The request id identifying the issued request.
     *         Data cache request is processed asynchronously and
     *         its completion is signalled to adaptation direction
     *         through MXIMPProtocolConnectionHostObserver interface,
     *         with MXIMPProtocolRequestCompleteEvent event.
     *
     * @leave KErrNoMemory if failed to allocate memory.
     * @leave KErrArgument if NULL argument(s) were provided.
     */
    virtual TPrFwRequestId HandleImGroupMemberRemovedL(
                MPrFwIdentity* aGroupId,
                MImGroupInfo* aGroupMember ) = 0;

      

    };


#endif // MPROTOCOLIMGROUPDATAHOST_H

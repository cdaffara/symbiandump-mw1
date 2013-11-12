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
* Description:  Interface for managing cached presence authorization information.
*
*/

#ifndef MXIMPFWPROTOCOLPRESENCEAUTHORIZATIONDATAHOST_H
#define MXIMPFWPROTOCOLPRESENCEAUTHORIZATIONDATAHOST_H

#include <e32std.h>
#include <presenceprotocolpluginhostifids.hrh>
#include <ximpbase.h>


class MXIMPIdentity;
class MXIMPObjectCollection;
class MPresenceBlockInfo;
class MPresenceGrantRequestInfo;
class MXIMPDataSubscriptionState;
class MXIMPStatus;



/**
 * Interface for managing cached presence authorization information.
 *
 * Interface is used by protocol adaptations to manage
 * presence authorization related information in
 * XIMP Framework internal data cache.
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
class MProtocolPresenceAuthorizationDataHost : public MXIMPBase
    {
public:

    /** Interface ID for the MProtocolPresenceAuthorizationDataHost. */
    enum { KInterfaceId = PRES_IF_ID_PROTOCOL_PRESENCE_AUTHORIZATION_DATAHOST };


protected:


    /**
     * Protected destructor. Object instancies
     * can't be deleted via this interface.
     */
    virtual ~MProtocolPresenceAuthorizationDataHost() {}



public: // Handling presence grant requests


    /**
     * Notifies XIMP Framework from
     * presence grant request list contents.
     *
     * Protocol adaptation implementation can call this
     * method to notify XIMP Framework from
     * complete presence grant request list content.
     * XIMP Framework calculates the changes
     * (new and obsoleted requests) from previous
     * presence grant request list content, and notifies
     * the subscribing clients from data changes.
     *
     * XIMP Framework takes ownership of the object
     * collection and all objects contained within, if
     * the method succeeds without a leave.
     *
     * @param [in] aGrantRequestList
     *        MXIMPObjectCollection object containing zero or
     *        more MPresenceGrantRequestInfo objects. Each
     *        contained MPresenceGrantRequestInfo shall present
     *        single presence grant request entry in the list of
     *        presence grant requests.
     *
     * @return The request ID identifying the issued request.
     *         Data cache update request is processed
     *         asynchronously and operation completion result is
     *         signalled to adaptation direction through
     *         MXIMPProtocolConnectionHostObserver interface.
     *
     * @leave KErrNoMemory if failed to allocate memory.
     * @leave KErrArgument if NULL argument(s) were provided.
     */
    virtual TXIMPRequestId HandlePresenceGrantRequestListL(
                MXIMPObjectCollection* aGrantRequestList ) = 0;



    /**
     * Notifies XIMP Framework from new presence grant request.
     *
     * Protocol adaptation implementation can call this
     * method to notify XIMP Framework from
     * new presence grant request. XIMP Framework calculates
     * the change, from previous presence grant request list content,
     * and notifies the subscribing clients from data changes.
     *
     * XIMP Framework takes ownership of the argument(s),
     * if method succeeds without a leave.
     *
     * @param [in] aGrantRequest
     *        MPresenceGrantRequestInfo object holding
     *        information about new presence grant request
     *        entry.
     *
     * @return The request ID identifying the issued request.
     *         Data cache update request is processed
     *         asynchronously and operation completion result is
     *         signalled to adaptation direction through
     *         MXIMPProtocolConnectionHostObserver interface.
     *
     * @leave KErrNoMemory if failed to allocate memory.
     * @leave KErrArgument if NULL argument(s) were provided.
     */
    virtual TXIMPRequestId HandlePresenceGrantRequestReceivedL(
                MPresenceGrantRequestInfo* aGrantRequest ) = 0;



    /**
     * Notifies XIMP Framework that existing presence
     * grant request is obsoleted.
     *
     * Notifies XIMP Framework that existing presence
     * grant request is obsoleted and thus it needs be removed
     * from presence grant request list. XIMP Framework
     * calculates the change, from previous presence grant request
     * list content, and notifies the subscribing clients from
     * data changes.
     *
     * XIMP Framework takes ownership of the argument(s),
     * if method succeeds without a leave.
     *
     * @param [in] aGrantRequestor
     *        Identity of the presence grant requestor, which
     *        presence grant request needs to be removed
     *        from presence grant request list.
     *
     * @return The request ID identifying the issued request.
     *         Data cache update request is processed
     *         asynchronously and operation completion result is
     *         signalled to adaptation direction through
     *         MXIMPProtocolConnectionHostObserver interface.
     *
     * @leave KErrNoMemory if failed to allocate memory.
     * @leave KErrArgument if NULL argument(s) were provided.
     */
    virtual TXIMPRequestId HandlePresenceGrantRequestObsoletedL(
                MXIMPIdentity* aGrantRequestor ) = 0;



    /**
     * Gets data subscription state for presence grant request list.
     *
     * Protocol adaptation can call this method to retrieve
     * presence grant request list current data subscription state.
     * Protocol adaptation can use data subscription state
     * information, to optimize its own processing and
     * network trafic amount.
     *
     * @return Presence grant request list data subscription state.
     *         Interface ownership is not returned to caller.
     *         Caller shouldn't cache the returned reference
     *         for later use.
     */
    virtual const MXIMPDataSubscriptionState&
                PresenceGrantRequestDataSubscriptionState() const = 0;



    /**
     * Updates data subscription state for presence grant
     * request list.
     *
     * Updates data subscription state for presence grant
     * request list. XIMP Framework calculates the
     * subscription state change, from previous
     * presence grant request list content, and notifies the
     * subscribing clients from data changes.
     *
     * XIMP Framework takes ownership of the argument(s),
     * if method succeeds without a leave.
     *
     * @param [in] aSubscriptionState
     *        New data subscription state for presence grant
     *        request list.
     *
     * @param [in] aStatus
     *        Optional status object describing the
     *        the reason, why protocol adaptation updated
     *        data item subscription state. Given status object
     *        is passed to clients through data item specific
     *        data change event. NULL value is accepted.
     *
     * @return The request ID identifying the issued request.
     *         Data cache update request is processed
     *         asynchronously and operation completion result is
     *         signalled to adaptation direction through
     *         MXIMPProtocolConnectionHostObserver interface.
     *
     * @leave KErrNoMemory if failed to allocate memory.
     * @leave KErrArgument if NULL subscription state argument was provided.
     */
    virtual TXIMPRequestId SetPresenceGrantRequestDataSubscriptionStateL(
                MXIMPDataSubscriptionState* aSubscriptionState,
                MXIMPStatus* aStatus ) = 0;



public: // Handling presence blocking



    /**
     * Notifies XIMP Framework from presence block list
     * contents.
     *
     * Protocol adaptation implementation can call this
     * method to notify XIMP Framework from complete
     * presence block list content. XIMP Framework
     * calculates the changes (new and removed block entries)
     * from previous presence block list content, and notifies
     * the subscribing clients from data changes.
     *
     * XIMP Framework takes ownership of the object
     * collection and all objects contained within, if
     * the method succeeds without a leave.
     *
     * @param [in] aBlockList
     *        MXIMPObjectCollection object containing zero or
     *        more MPresenceBlockInfo objects. Each
     *        contained MPresenceBlockInfo shall present
     *        single presence block entry in the presence block list.
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
    virtual TXIMPRequestId HandlePresenceBlockListL(
                MXIMPObjectCollection* aBlockList ) = 0;



    /**
     * Notifies XIMP Framework from new
     * presence block entry.
     *
     * Protocol adaptation implementation can call this
     * method to notify XIMP Framework from
     * new presence block entry. XIMP Framework
     * calculates the change, from previous presence block
     * list content, and notifies the subscribing clients
     * from data changes.
     *
     * XIMP Framework takes ownership of the argument(s),
     * if method succeeds without a leave.
     *
     * @param [in] aBlockInfo
     *        MPresenceBlockInfo object containing
     *        presence block entrys information.
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
    virtual TXIMPRequestId HandlePresenceBlockedL(
                MPresenceBlockInfo* aBlockInfo ) = 0;



    /**
     * Notifies XIMP Framework from cancellation of
     * existing presence block entry.
     *
     * Protocol adaptation can call this method to notify
     * XIMP Framework from cancellation of existing
     * presence block entry. XIMP Framework calculates
     * the change, from previous presence block list content,
     * and notifies the subscribing clients from data changes.
     *
     * XIMP Framework takes ownership of the argument(s),
     * if method succeeds without a leave.
     *
     * @param [in] aBlockedEntityId
     *        Identity of the presence block entry which to
     *        remove from presence block list.
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
    virtual TXIMPRequestId HandlePresenceBlockCanceledL(
                MXIMPIdentity* aBlockedEntityId ) = 0;



    /**
     * Gets data subscription state for presence block list.
     *
     * Protocol adaptation can call this method to retrieve
     * presence block list current data subscription state.
     * Protocol adaptation can use data subscription state
     * information, to optimize its own processing and
     * network trafic amount.
     *
     * @return Presence block list data subscription state.
     *         Interface ownership is not returned to caller.
     *         Caller shouldn't cache the returned reference
     *         for later use.
     */
    virtual const MXIMPDataSubscriptionState&
                PresenceBlockDataSubscriptionState() const = 0;



    /**
     * Updates data subscription state for presence block list.
     *
     * Updates data subscription state for presence block list.
     * XIMP Framework calculates the subscription state change,
     * from previous presence block list content, and notifies the
     * subscribing clients from data changes.
     *
     * XIMP Framework takes ownership of the argument(s),
     * if method succeeds without a leave.
     *
     * @param [in] aSubscriptionState
     *        New data subscription state for presence block list.
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
    virtual TXIMPRequestId SetPresenceBlockDataSubscriptionStateL(
                MXIMPDataSubscriptionState* aSubscriptionState,
                MXIMPStatus* aStatus ) = 0;


    };


#endif // MXIMPFWPROTOCOLPRESENCEAUTHORIZATIONDATAHOST_H

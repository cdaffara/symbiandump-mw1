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
* Description:  Interface for managing cached presence publication information.
*
*/

#ifndef MXIMPFWPROTOCOLPRESENCEPUBLISHINGDATAHOST_H
#define MXIMPFWPROTOCOLPRESENCEPUBLISHINGDATAHOST_H


#include <e32std.h>
#include <presenceprotocolpluginhostifids.hrh>
#include <ximpbase.h>


class MXIMPIdentity;
class MPresenceInfo;
class MXIMPObjectCollection;
class MPresenceWatcherInfo;
class MXIMPDataSubscriptionState;
class MXIMPStatus;


/**
 * Interface for managing cached presence publication information.
 *
 * Interface is used by protocol adaptations to manage
 * presence publication related information in
 * XIMP Framework internal data cache
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
class MProtocolPresencePublishingDataHost : public MXIMPBase
    {
public:

    /** Interface ID for the MProtocolPresencePublishingDataHost. */
    enum { KInterfaceId = PRES_IF_ID_PROTOCOL_PRESENCE_PUBLISHING_DATAHOST };


protected:


    /**
     * Protected destructor. Object instancies
     * can't be deleted via this interface.
     */
    virtual ~MProtocolPresencePublishingDataHost() {}



public: // Handling user own presence information


    /**
     * Notifies XIMP Framework from subscribed
     * user own presence information.
     *
     * Protocol adaptation implementation can call this
     * method to notify XIMP Framework from user own
     * subscribed presence information. XIMP Framework
     * updates its cached user own presence status, and
     * notifies the subscribing clients from data changes.
     *
     * XIMP Framework takes ownership of the argument(s),
     * if method succeeds without a leave.
     *
     * @param [in] aPresenceInfo
     *        MPresenceInfo object containing new
     *        subscribed own presence information .
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
    virtual TXIMPRequestId HandleSubscribedOwnPresenceL(
                MPresenceInfo* aPresenceInfo ) = 0;



    /**
     * Gets data subscription state for own presence information.
     *
     * Protocol adaptation can call this method to retrieve
     * own presence information current data subscription state.
     * Protocol adaptation can use data subscription state
     * information, to optimize its own processing and
     * network trafic amount.
     *
     * @return Own presence information data subscription state.
     *         Interface ownership is not returned to caller.
     *         Caller shouldn't cache the returned reference
     *         for later use.
     */
    virtual const MXIMPDataSubscriptionState&
                OwnPresenceDataSubscriptionState() const = 0;



    /**
     * Updates data subscription state for presence block list.
     *
     * Updates data subscription state for own presence information.
     * XIMP Framework calculates the subscription state change,
     * from previous own presence information content, and notifies the
     * subscribing clients from data changes.
     *
     * XIMP Framework takes ownership of the argument(s),
     * if method succeeds without a leave.
     *
     * @param [in] aSubscriptionState
     *        New data subscription state for own presence information.
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
    virtual TXIMPRequestId SetOwnPresenceDataSubscriptionStateL(
                MXIMPDataSubscriptionState* aSubscriptionState,
                MXIMPStatus* aStatus ) = 0;



public: // Handling presence watcher list content



    /**
     * Notifies XIMP Framework from
     * presence watcher list contents.
     *
     * Protocol adaptation implementation can call this
     * method to notify XIMP Framework from
     * complete presence watcher list content.
     * XIMP Framework calculates the changes
     * (new and disappeared watcher entries) from previous
     * presence block list content, and notifies
     * the subscribing clients from data changes.
     *
     * XIMP Framework takes ownership of the object
     * collection and all objects contained within, if
     * the method succeeds without a leave.
     *
     * @param [in] aWatcherList
     *        MXIMPObjectCollection object containing zero or
     *        more MPresenceWatcherInfo objects. Each
     *        contained MPresenceWatcherInfo shall present
     *        single presence watcher entry in the list of
     *        presence watchers.
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
    virtual TXIMPRequestId HandlePresenceWatcherListL(
                MXIMPObjectCollection* aWatcherList ) = 0;



    /**
     * Gets data subscription state for presence watcher list.
     *
     * Protocol adaptation can call this method to retrieve
     * presence watcher list current data subscription state.
     * Protocol adaptation can use data subscription state
     * information, to optimize its own processing and
     * network trafic amount.
     *
     * @return Presence watcher list data subscription state.
     *         Interface ownership is not returned to caller.
     *         Caller shouldn't cache the returned reference
     *         for later use.
     */
    virtual const MXIMPDataSubscriptionState&
                PresenceWatcherListDataSubscriptionState() const = 0;



    /**
     * Updates data subscription state for presence watcher list.
     *
     * Updates data subscription state for presence watcher list.
     * XIMP Framework calculates the subscription state change,
     * from previous presence watcher list content, and notifies the
     * subscribing clients from data changes.
     *
     * XIMP Framework takes ownership of the argument(s),
     * if method succeeds without a leave.
     *
     * @param [in] aSubscriptionState
     *        New data subscription state for presence watcher list.
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
    virtual TXIMPRequestId SetPresenceWatcherListDataSubscriptionStateL(
                MXIMPDataSubscriptionState* aSubscriptionState,
                MXIMPStatus* aStatus ) = 0;


    };


#endif // MXIMPFWPROTOCOLPRESENCEPUBLISHINGDATAHOST_H

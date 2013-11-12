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
* Description:  Interface for managing cached subscribed presence information.
*
*/

#ifndef MXIMPFWPROTOCOLPRESENCEWATCHINGDATAHOST_H
#define MXIMPFWPROTOCOLPRESENCEWATCHINGDATAHOST_H

#include <e32std.h>
#include <presenceprotocolpluginhostifids.hrh>
#include <ximpbase.h>

class MPresenceInfo;
class MXIMPIdentity;
class MXIMPDataSubscriptionState;
class MXIMPStatus;



/**
 * Interface for managing cached subscribed presence information.
 *
 * Interface is used by protocol adaptations to manage
 * manage presentities presence information stored
 * in XIMP Framework internal data cache.
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
class MProtocolPresenceWatchingDataHost : public MXIMPBase
    {
public:

    /** Interface ID for the MProtocolPresenceWatchingDataHost. */
    enum { KInterfaceId = PRES_IF_ID_PROTOCOL_PRESENCE_WATCHING_DATAHOST };

protected:

    /**
     * Protected destructor. Object instancies
     * can't be deleted via this interface.
     */
    virtual ~MProtocolPresenceWatchingDataHost() {}




public: // Handling subscribed presence information


    /**
     * Notifies XIMP Framework from subscribed
     * presentitys presence information.
     *
     * Protocol adaptation implementation can call this
     * method to notify XIMP Framework from subscribed
     * presentitys presence information. XIMP Framework
     * updates its cached presentitys presence status, and
     * notifies the subscribing clients from data changes.
     *
     * XIMP Framework takes ownership of the argument(s),
     * if method succeeds without a leave.
     *
     * @param [in] aPresentityId
     *        MXIMPIdentity object identifying the presentity
     *        of which subscribed presence information
     *        is delivered here.
     *
     * @param [in] aPresenceInfo
     *        MPresenceInfo object containing new presence
     *        information for the identified presentity.
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
    virtual TXIMPRequestId HandleSubscribedPresentityPresenceL(
                MXIMPIdentity* aPresentityId,
                MPresenceInfo* aPresenceInfo ) = 0;



    /**
     * Gets data subscription state for presentitys presence
     * information.
     *
     * Protocol adaptation can call this method to retrieve
     * presentitys presence information data subscription state.
     * Protocol adaptation can use data subscription state
     * information, to optimize its own processing and
     * network trafic amount.
     *
     * @return Presentitys presence information data subscription state.
     *         Interface ownership is not returned to caller.
     *         Caller shouldn't cache the returned reference
     *         for later use.
     */
    virtual const MXIMPDataSubscriptionState&
                PresentityPresenceDataSubscriptionState(
                const MXIMPIdentity& aPresentityId ) const = 0;



    /**
     * Updates data subscription state for
     * presentitys presence information.
     *
     * Updates data subscription state for presentitys presence
     * subscription. XIMP Framework calculates the subscription
     * state change, from previous presentitys presence information
     * content, and notifies the subscribing clients from data
     * changes.
     *
     * XIMP Framework takes ownership of the argument(s),
     * if method succeeds without a leave.
     *
     * @param [in] aPresentityId
     *        MXIMPIdentity object identifying the presentity
     *        of which presence data subscription state is
     *        updated here.
     *
     * @param [in] aSubscriptionState
     *        New data subscription state for presentitys
     *        presence information.
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
    virtual TXIMPRequestId SetPresentityPresenceDataSubscriptionStateL(
                MXIMPIdentity* aPresentityId,
                MXIMPDataSubscriptionState* aSubscriptionState,
                MXIMPStatus* aStatus ) = 0;


    };


#endif // MXIMPFWPROTOCOLPRESENCEWATCHINGDATAHOST_H

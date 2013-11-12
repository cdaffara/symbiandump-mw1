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
* Description:  Interface for presence protocol publishing object.
*
*/

#ifndef MXIMPFWPROTOCOLPRESENCEPUBLISHING_H
#define MXIMPFWPROTOCOLPRESENCEPUBLISHING_H


#include <e32std.h>
#include <badesca.h>
#include <presenceprotocolpluginifids.hrh>
#include <ximpbase.h>


class MPresenceInfo;
class MPresenceInfoFilter;
class MXIMPIdentity;


/**
 * Interface for presence protocol publishing object.
 * Interface must be implemented by a XIMP Framework protocol
 * plug-ins.
 *
 * This interface defines actions that XIMP Framework
 * requests from a presence protocol connection to
 * execute presence publishing related tasks.
 *
 * Protocol implementation must route requests received through
 * this interface, to same remote presence service destination,
 * as where the parent MXIMPProtocolConnection is connected.
 * (Here the parent MXIMPProtocolConnection means the protocol
 * connection instance from where this interface instance
 * was retrieved.)
 *
 * @see MXIMPProtocolConnection
 * @ingroup ximppresprotocolpluginapi
 * @since S60 v3.2
 */
class MProtocolPresencePublishing : public MXIMPBase
    {
public:

    /** Interface ID for the MProtocolPresencePublishing. */
    enum { KInterfaceId = PRES_IF_ID_PROTOCOL_PRESENCE_PUBLISHING };


protected:

    /**
     * Protected destructor. MProtocolPresencePublishing
     * instancies can't be destroyed via this interface.
     */
    virtual inline ~MProtocolPresencePublishing() {};



public: // Publish user own presence information


    /**
     * Requests the protocol to publish user own
     * presence information to remote service.
     *
     *
     * @param [in] aOwnPresence
     *        Presence info indicating the own presence
     *        information to publish to remote service.
     *
     * @param [in] aReqId
     *        Request ID identifying the issued request.
     */
    virtual void DoPublishOwnPresenceL(
                    const MPresenceInfo& aOwnPresence,
                    TXIMPRequestId aReqId ) = 0;




public: // Subscribe user own presence information


    /**
     * Requests the protocol to subscribe user own
     * presence information from remote service.
     *
     * @param [in] aPif
     *        Presence information filter indicating the set
     *        of presence information to subscribe.
     *
     * @param [in] aReqId
     *        Request ID identifying the issued request.
     */
    virtual void DoSubscribeOwnPresenceL(
                    const MPresenceInfoFilter& aPif,
                    TXIMPRequestId aReqId ) = 0;


    /**
     * Requests the protocol to update presence information
     * filter (PIF) for already existing own presence subscription.
     *
     * @param [in] aPif
     *        Indicates the new set of presence information
     *        to subscribe. Given set may be "wider" or "narrower"
     *        than the already subscribed one.
     *
     * @param [in] aReqId
     *        Request ID identifying the issued request.
     */
    virtual void DoUpdateOwnPresenceSubscriptionPifL(
                    const MPresenceInfoFilter& aPif,
                    TXIMPRequestId aReqId ) = 0;


    /**
     * Requests the protocol to terminate own presence
     * information subscription.
     *
     * @param [in] aReqId
     *        Request ID identifying the issued request.
     */
    virtual void DoUnsubscribeOwnPresenceL(
                    TXIMPRequestId aReqId ) = 0;




public: // Subscribe presence watcher list content


    /**
     * Requests the protocol to subscribe presence
     * watcher list content from remote service.
     *
     * @param [in] aReqId
     *        Request ID identifying the issued request.
     */
    virtual void DoSubscribePresenceWatcherListL(
                    TXIMPRequestId aReqId ) = 0;


    /**
     * Requests the protocol to terminate presence
     * watcher list content subscription.
     *
     * @param [in] aReqId
     *        Request ID identifying the issued request.
     */
    virtual void DoUnsubscribePresenceWatcherListL(
                    TXIMPRequestId aReqId ) = 0;

    };



#endif // MXIMPFWPROTOCOLPRESENCEPUBLISHING_H

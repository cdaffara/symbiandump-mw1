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
* Description:  Interface for presence protocol watching object.
*
*/

#ifndef MXIMPFWPROTOCOLPRESENCEWATCHING_H
#define MXIMPFWPROTOCOLPRESENCEWATCHING_H

#include <e32std.h>
#include <badesca.h>
#include <presenceprotocolpluginifids.hrh>
#include <ximpbase.h>


class MPresenceInfo;
class MPresenceInfoFilter;
class MXIMPIdentity;


/**
 * Interface for presence protocol watching object.
 * Interface must be implemented by a XIMP Framework protocol
 * plug-ins.
 *
 * This interface defines actions that XIMP Framework
 * requests from a presence protocol connection to
 * execute presence watching related tasks.
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
class MProtocolPresenceWatching : public MXIMPBase
    {
public:

    /** Interface ID for the MProtocolPresenceWatching. */
    enum { KInterfaceId = PRES_IF_ID_PROTOCOL_PRESENCE_WATCHING };


protected:

    /**
     * Protected destructor. MProtocolPresenceWatching
     * instancies can't be destroyed via this interface.
     */
    virtual inline ~MProtocolPresenceWatching() {};



public: // Subscribe presence information from single presentities


    /**
     * Requests the protocol to subscribe presence information
     * for identified presentity.
     *
     * @param [in] aPresentityId
     *        Identification of the presentity which presence
     *        information subscribe to.
     *
     * @param [in] aPif
     *        Presence information filter indicating the set
     *        of presence information to request from the
     *        presentity.
     *
     * @param [in] aReqId
     *        Request ID identifying the issued request.
     */
    virtual void DoSubscribePresentityPresenceL(
                    const MXIMPIdentity& aPresentityId,
                    const MPresenceInfoFilter& aPif,
                    TXIMPRequestId aReqId ) = 0;


    /**
     * Requests the protocol to update presence information
     * filter (PIF) for already existing presence subscription.
     *
     * @param [in] aPresentityId
     *        Identification of the presentity which presence
     *        subscribtion PIF to update.
     *
     * @param [in] aPif
     *        Indicates the new set of presence information
     *        to request from the presentity. Given set
     *        may be "wider" or "narrower" than the already
     *        subscribed one.
     *
     * @param [in] aReqId
     *        Request ID identifying the issued request.
     */
    virtual void DoUpdatePresentityPresenceSubscriptionPifL(
                    const MXIMPIdentity& aPresentityId,
                    const MPresenceInfoFilter& aPif,
                    TXIMPRequestId aReqId ) = 0;


    /**
     * Requests the protocol to terminate presentity
     * presence information subscription.
     *
     * @param [in] aPresentityId
     *        Identification of the presentity which presence
     *        subscribtion to terminate.
     *
     * @param [in] aReqId
     *        Request ID identifying the issued request.
     */
    virtual void DoUnsubscribePresentityPresenceL(
                    const MXIMPIdentity& aPresentityId,
                    TXIMPRequestId aReqId ) = 0;




public: // Subscribe presence information from presentity group members


    /**
     * Requests the protocol to subscribe presence information
     * for all the presentities existing in the identified
     * presentity group.
     *
     * @param [in] aGroupId
     *        Identification of the presentity group for which
     *        members to request presence information.
     *
     * @param [in] aPif
     *        Presence information filter indicating the set
     *        of presence information to request from the
     *        presentity group members.
     *
     * @param [in] aReqId
     *        Request ID identifying the issued request.
     */
    virtual void DoSubscribePresentityGroupMembersPresenceL(
                    const MXIMPIdentity& aGroupId,
                    const MPresenceInfoFilter& aPif,
                    TXIMPRequestId aReqId ) = 0;


    /**
     * Requests the protocol to update presence information
     * filter (PIF) for already existing presentity group
     * members presence subscription.
     *
     * @param [in] aGroupId
     *        Identification of the presentity group for which
     *        members presence subscribtion PIF to update.
     *
     * @param [in] aPif
     *        Indicates the new set of presence information
     *        to request from the presentities. Given set
     *        may be "wider" or "narrower" than the already
     *        subscribed one.
     *
     * @param [in] aReqId
     *        Request ID identifying the issued request.
     */
    virtual void DoUpdatePresentityGroupMembersPresenceSubscriptionPifL(
                    const MXIMPIdentity& aGroupId,
                    const MPresenceInfoFilter& aPif,
                    TXIMPRequestId aReqId ) = 0;


    /**
     * Requests the protocol to terminate presentity group
     * members presence information subscription.
     *
     * @param [in] aGroupId
     *        Identification of the presentity group
     *        which members presence subscribtion to terminate.
     *
     * @param [in] aReqId
     *        Request ID identifying the issued request.
     */
    virtual void DoUnsubscribePresentityGroupMembersPresenceL(
                    const MXIMPIdentity& aGroupId,
                    TXIMPRequestId aReqId ) = 0;

    };



#endif // MXIMPFWPROTOCOLPRESENCEWATCHING_H

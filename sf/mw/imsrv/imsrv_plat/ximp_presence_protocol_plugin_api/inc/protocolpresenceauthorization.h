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
* Description:  Interface for presence protocol authorization object.
*
*/

#ifndef MXIMPFWPROTOCOLPRESENCEAUTHORIZATION_H
#define MXIMPFWPROTOCOLPRESENCEAUTHORIZATION_H


#include <e32std.h>
#include <badesca.h>
#include <presenceprotocolpluginifids.hrh>
#include <ximpbase.h>


class MXIMPIdentity;
class MPresenceInfoFilter;


/**
 * Interface for presence protocol authorization object.
 * Interface must be implemented by a XIMP Framework protocol
 * plug-ins.
 *
 * This interface defines actions that XIMP Framework
 * requests from a presence protocol connection to
 * execute presence authorization related tasks.
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
class MProtocolPresenceAuthorization : public MXIMPBase
    {
public:

    /** Interface ID for the MProtocolPresenceAuthorization. */
    enum { KInterfaceId = PRES_IF_ID_PROTOCOL_PRESENCE_AUTHORIZATION };


protected:

    /**
     * Protected destructor. MProtocolPresenceAuthorization
     * instancies can't be destroyed via this interface.
     */
    virtual inline ~MProtocolPresenceAuthorization() {};



public: // Subscribe presence grant request list


    /**
     * Requests the protocol to subscribe presence
     * grant request list from remote service.
     *
     * @param [in] aReqId
     *        Request ID identifying the issued request.
     */
    virtual void DoSubscribePresenceGrantRequestListL(
                    TXIMPRequestId aReqId ) = 0;


    /**
     * Requests the protocol to terminate presence
     * grant request list subscription.
     *
     * @param [in] aReqId
     *        Request ID identifying the issued request.
     */
    virtual void DoUnsubscribePresenceGrantRequestListL(
                    TXIMPRequestId aReqId ) = 0;




public: // Granting presence information access to single presentities


    /**
     * Requests the protocol to allow the identified presentity
     * to see specified presence information.
     *
     * @param [in] aPresentityId
     *        Identification of the presentity to which
     *        to allow presence information access.
     *
     * @param [in] aPif
     *        Presence information filter indicating the
     *        allowed presence information.
     *
     * @param [in] aReqId
     *        Request ID identifying the issued request.
     */
    virtual void DoGrantPresenceForPresentityL(
                    const MXIMPIdentity& aPresentityId,
                    const MPresenceInfoFilter& aPif,
                    TXIMPRequestId aReqId ) = 0;


    /**
     * Requests the protocol to update presence information
     * filter (PIF) for already existing presence grant.
     *
     * @param [in] aPresentityId
     *        Identification of the presentity which presence
     *        grant PIF to update.
     *
     * @param [in] aPif
     *        Indicates the new set of presence information
     *        to allow for the presentity. Given set
     *        may be "wider" or "narrower" than the already
     *        allowed one.
     *
     * @param [in] aReqId
     *        Request ID identifying the issued request.
     */
    virtual void DoUpdatePresenceGrantPifForPresentityL(
                    const MXIMPIdentity& aPresentityId,
                    const MPresenceInfoFilter& aPif,
                    TXIMPRequestId aReqId ) = 0;


    /**
     * Requests the protocol to withdraw existing
     * presence information grant.
     *
     * @param [in] aPresentityId
     *        Identification of the presentity which presence
     *        grant to withdraw.
     *
     * @param [in] aReqId
     *        Request ID identifying the issued request.
     */
    virtual void DoWithdrawPresenceGrantFromPresentityL(
                    const MXIMPIdentity& aPresentityId,
                    TXIMPRequestId aReqId ) = 0;




public: // Granting presence information access to presentity group members


    /**
     * Requests the protocol to allow the identified presentity
     * group members to see specified presence information.
     *
     *
     * @param [in] aGroupId
     *        Identification of the presentity group for which
     *        members to allow presence information.
     *
     * @param [in] aPif
     *        Presence information filter indicating the
     *        allowed presence information.
     *
     * @param [in] aReqId
     *        Request ID identifying the issued request.
     */
    virtual void DoGrantPresenceForPresentityGroupMembersL(
                    const MXIMPIdentity& aGroupId,
                    const MPresenceInfoFilter& aPif,
                    TXIMPRequestId aReqId ) = 0;


    /**
     * Requests the protocol to update presence information
     * filter (PIF) for already existing presentity group
     * members presence grant.
     *
     * @param [in] aGroupId
     *        Identification of the presentity group for which
     *        members presence grant PIF to update.
     *
     * @param [in] aPif
     *        Indicates the new set of presence information
     *        to allow for the presentities. Given set
     *        may be "wider" or "narrower" than the already
     *        allowed one.
     *
     * @param [in] aReqId
     *        Request ID identifying the issued request.
     */
    virtual void DoUpdatePresenceGrantPifForPresentityGroupMembersL(
                    const MXIMPIdentity& aGroupId,
                    const MPresenceInfoFilter& aPif,
                    TXIMPRequestId aReqId ) = 0;


    /**
     * Requests the protocol to withdraw existing
     * presentity group members presence information grant.
     *
     * @param [in] aGroupId
     *        Identification of the presentity group
     *        which members presence grant to terminate.
     *
     * @param [in] aReqId
     *        Request ID identifying the issued request.
     */
    virtual void DoWithdrawPresenceGrantFromPresentityGroupMembersL(
                    const MXIMPIdentity& aGroupId,
                    TXIMPRequestId aReqId ) = 0;



public: // Granting presence information access to everyone


    /**
     * Requests the protocol to allow everyone
     * to see specified presence information.
     *
     * @param [in] aPif
     *        Presence information filter indicating the
     *        allowed presence information.
     *
     * @param [in] aReqId
     *        Request ID identifying the issued request.
     */
    virtual void DoGrantPresenceForEveryoneL(
                    const MPresenceInfoFilter& aPif,
                    TXIMPRequestId aReqId ) = 0;


    /**
     * Requests the protocol to update presence information
     * filter (PIF) for already existing presence grant.
     *
     * @param [in] aPif
     *        Indicates the new set of presence information
     *        to allow for the presentity. Given set
     *        may be "wider" or "narrower" than the already
     *        allowed one.
     *
     * @param [in] aReqId
     *        Request ID identifying the issued request.
     */
    virtual void DoUpdatePresenceGrantPifForEveryoneL(
                    const MPresenceInfoFilter& aPif,
                    TXIMPRequestId aReqId ) = 0;


    /**
     * Requests the protocol to withdraw existing
     * presence information grant.
     *
     * @param [in] aReqId
     *        Request ID identifying the issued request.
     */
    virtual void DoWithdrawPresenceGrantFromEveryoneL(
                    TXIMPRequestId aReqId ) = 0;


public: // Subscribe presence block list


    /**
     * Requests the protocol to subscribe presence
     * block list from remote service.
     *
     * @param [in] aReqId
     *        Request ID identifying the issued request.
     */
    virtual void DoSubscribePresenceBlockListL(
                    TXIMPRequestId aReqId ) = 0;


    /**
     * Requests the protocol to terminate presence
     * block list subscription.
     *
     * @param [in] aReqId
     *        Request ID identifying the issued request.
     */
    virtual void DoUnsubscribePresenceBlockListL(
                    TXIMPRequestId aReqId ) = 0;



public: // Blocking presence information access from single presentities


    /**
     * Requests the protocol to block the identified presentity
     * from seeing presence information produced by this
     * presence connection.
     *
     * After blocking, identified presentity can't anymore
     * access the user own presence information.
     *
     * @param [in] aPresentityId
     *        Identification of the presentity for which
     *        presence information access to block.
     *
     * @param [in] aReqId
     *        Request ID identifying the issued request.
     */
    virtual void DoBlockPresenceForPresentityL(
                    const MXIMPIdentity& aPresentityId,
                    TXIMPRequestId aReqId ) = 0;


    /**
     * Requests the protocol to cancel existing presence
     * information block from the identified presentity.
     *
     * @param [in] aPresentityId
     *        Identification of the presentity of which
     *        presence information block to cancel.
     *
     * @param [in] aReqId
     *        Request ID identifying the issued request.
     */
    virtual void DoCancelPresenceBlockFromPresentityL(
                    const MXIMPIdentity& aPresentityId,
                    TXIMPRequestId aReqId ) = 0;

    };



#endif // MXIMPFWPROTOCOLPRESENCEAUTHORIZATION_H

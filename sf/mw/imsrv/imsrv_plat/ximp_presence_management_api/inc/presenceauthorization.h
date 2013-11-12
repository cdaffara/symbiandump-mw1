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
* Description:  Presence authorization control interface for XIMPFw clients use.
*
*/

#ifndef MXIMPFWPRESENCEAUTHORIZATION_H
#define MXIMPFWPRESENCEAUTHORIZATION_H

#include <e32std.h>
#include <ximpbase.h>
#include <presencemanagementifids.hrh>


class MPresenceInfoFilter;
class MXIMPIdentity;



/**
 * Presence authorization control interface for XIMPFw clients use.
 * Interface is implemented by the XIMP Framework.
 *
 * This interface defines actions that client can request from
 * XIMP Framework execute presence authorization related tasks.
 *
 * @ingroup ximppresmanagementapi
 * @since S60 v3.2
 */
class MPresenceAuthorization : public MXIMPBase
    {
public:

    /** Interface ID for the MPresenceAuthorization. */
    enum { KInterfaceId = PRES_IF_ID_PRESENCE_AUTHORIZATION };


protected:

    /**
     * Protected destructor.
     * Object instancies can't be deleted via this interface.
     */
    virtual ~MPresenceAuthorization() {}





public: // Presence grant request list access

    /**
     * Requests the XIMPFw to subscribe presence
     * grant request list from remote service.
     *
     * @return The request ID identifying the issued request.
     */
    virtual TXIMPRequestId SubscribePresenceGrantRequestListL() = 0;


    /**
     * Requests the XIMPFw to terminate presence
     * grant request list subscription.
     *
     * @return The request ID identifying the issued request.
     */
    virtual TXIMPRequestId UnsubscribePresenceGrantRequestListL() = 0;





public: // Granting presence information access to single presentities


    /**
     * Requests the XIMPFw to allow the identified presentity
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
     * @return The request ID identifying the issued request.
     */
    virtual TXIMPRequestId GrantPresenceForPresentityL(
                    const MXIMPIdentity& aPresentityId,
                    const MPresenceInfoFilter& aPif ) = 0;


    /**
     * Requests the XIMPFw to withdraw existing
     * presence information grant.
     *
     * @param [in] aPresentityId
     *        Identification of the presentity which presence
     *        grant to withdraw.
     *
     * @return The request ID identifying the issued request.
     */
    virtual TXIMPRequestId WithdrawPresenceGrantFromPresentityL(
                    const MXIMPIdentity& aPresentityId ) = 0;




public: // Granting presence information access to presentity group members


    /**
     * Requests the XIMPFw to allow the identified presentity
     * group members to see specified presence information.
     *
     * @param [in] aGroupId
     *        Identification of the presentity group for which
     *        members to allow presence information.
     *
     * @param [in] aPif
     *        Presence information filter indicating the
     *        allowed presence information.
     *
     * @return The request ID identifying the issued request.
     */
    virtual TXIMPRequestId GrantPresenceForPresentityGroupMembersL(
                    const MXIMPIdentity& aGroupId,
                    const MPresenceInfoFilter& aPif ) = 0;


    /**
     * Requests the XIMPFw to withdraw existing
     * presentity group members presence information grant.
     *
     * @param [in] aGroupId
     *        Identification of the presentity group
     *        which members presence grant to terminate.
     *
     * @return The request ID identifying the issued request.
     */
    virtual TXIMPRequestId WithdrawPresenceGrantFromPresentityGroupMembersL(
                    const MXIMPIdentity& aGroupId ) = 0;



public: // Granting presence information access to everyone


    /**
     * Requests the XIMPFw to allow everyone
     * to see specified presence information.
     *
     * @param [in] aPif
     *        Presence information filter indicating the
     *        allowed presence information.
     *
     * @return The request ID identifying the issued request.
     */
    virtual TXIMPRequestId GrantPresenceForEveryoneL(
                    const MPresenceInfoFilter& aPif ) = 0;


    /**
     * Requests the XIMPFw to withdraw existing
     * information grant for everyone.
     *
     * @return The request ID identifying the issued request.
     */
    virtual TXIMPRequestId WithdrawPresenceGrantFromEveryoneL() = 0;


public: // Presence block list access

    /**
     * Subscribe presence block information.
     *
     * @return The request ID identifying the issued request.
     */
    virtual TXIMPRequestId SubscribePresenceBlockListL() = 0;


    /**
     * Unsubscribe presence block information.
     *
     * @return The request ID identifying the issued request.
     */
    virtual TXIMPRequestId UnsubscribePresenceBlockListL() = 0;




public: // Blocking presence information access from single presentities


    /**
     * Requests the XIMPFw to block the identified presentity
     * from seeing presence information produced by this
     * presence context.
     *
     * After blocking, identified presentity can't anymore
     * access the user own presence information.
     *
     * @param [in] aPresentityId
     *        Identification of the presentity for which
     *        presence information access to block.
     *
     * @return The request ID identifying the issued request.
     */
    virtual TXIMPRequestId BlockPresenceForPresentityL(
                    const MXIMPIdentity& aPresentityId ) = 0;


    /**
     * Requests the protocol to cancel existing presence
     * information block from the identified presentity.
     *
     * @param [in] aPresentityId
     *        Identification of the presentity of which
     *        presence information block to cancel.
     *
     * @return The request ID identifying the issued request.
     */
    virtual TXIMPRequestId CancelPresenceBlockFromPresentityL(
                    const MXIMPIdentity& aPresentityId ) = 0;


    };

#endif // MXIMPFWPRESENCEAUTHORIZATION_H



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
* Description:  Presence subscription control interface for clients use.
*
*/

#ifndef MXIMPFWPRESENCEWATCHING_H
#define MXIMPFWPRESENCEWATCHING_H

#include <e32std.h>
#include <ximpbase.h>
#include <presencemanagementifids.hrh>


class MPresenceInfoFilter;
class MXIMPIdentity;



/**
 * Presence subscription control interface for clients use.
 * Interface is implemented by the XIMP Framework.
 *
 * @ingroup ximppresmanagementapi
 * @since S60 v3.2
 */
class MPresenceWatching : public MXIMPBase
    {
public:

    /** Interface ID for the MPresenceWatching. */
    enum { KInterfaceId = PRES_IF_ID_PRESENCE_WATCHING };


protected:

    /**
     * Protected destructor.
     * Object instancies can't be deleted via this interface.
     */
    virtual ~MPresenceWatching() {}



public: //Presentity presence subscription


    /**
     * Subscribe presentity presence.
     *
     * Subscribing presence is asynchronous operation and it
     * might evolve network operations.
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
     * @return The request ID identifying the issued request.
     */
    virtual TXIMPRequestId SubscribePresentityPresenceL(
                    const MXIMPIdentity& aPresentityId,
                    const MPresenceInfoFilter& aPif ) = 0;


    /**
     * Unsubscribe presentity presence.
     *
     * Unsubscribing presence is asynchronous operation and it
     * might evolve network operations.
     *
     * @param [in] aPresentityId
     *        Identification of the presentity which presence
     *        information subscribtion to end.
     *
     * @return The request ID identifying the issued request.
     */
    virtual TXIMPRequestId UnsubscribePresentityPresenceL(
                    const MXIMPIdentity& aPresentityId ) = 0;



public: //Presentity group members presence subscription


    /**
     * Subscribe presentity group members presence.
     *
     * Subscribing presence is asynchronous operation and it
     * might evolve network operations.
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
     * @return The request ID identifying the issued request.
     */
    virtual TXIMPRequestId SubscribePresentityGroupMembersPresenceL(
                    const MXIMPIdentity& aGroupId,
                    const MPresenceInfoFilter& aPif ) = 0;


    /**
     * Unsubscribe presentity group members presence.
     *
     * Unsubscribing presence is asynchronous operation and it
     * might evolve network operations.
     *
     * @param [in] aGroupId
     *        Identification of the presentity group
     *        which members presence subscribtion to terminate.
     *
     * @return The request ID identifying the issued request.
     */
    virtual TXIMPRequestId UnsubscribePresentityGroupMembersPresenceL(
                    const MXIMPIdentity& aGroupId ) = 0;

    };


#endif // MXIMPFWPRESENCEWATCHING_H



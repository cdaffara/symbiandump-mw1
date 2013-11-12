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
* Description:  Presence publication control interface for clients use.
*
*/

#ifndef MXIMPFWPRESENCEPUBLISHING_H
#define MXIMPFWPRESENCEPUBLISHING_H

#include <e32std.h>
#include <ximpbase.h>
#include <presencemanagementifids.hrh>


class MPresenceInfo;
class MPresenceInfoFilter;


/**
 * Presence publication control interface for clients use.
 * Interface is implemented by the XIMP Framework.
 *
 * MPresencePublishing provides services to publish
 * own presence infrmation to binded remote presence service.
 * It also provides way to subscribe notifications from own
 * presence changes
 *
 * MPresencePublishing interface can be obtained from
 * MXIMPContext interface.
 *
 * @ingroup ximppresmanagementapi
 * @since S60 v3.2
 */
class MPresencePublishing : public MXIMPBase
    {
public:

    /** Interface ID for the MPresencePublishing. */
    enum { KInterfaceId = PRES_IF_ID_PRESENCE_PUBLISHING };


protected:

    /**
     * Protected destructor.
     * Object instancies can't be deleted via this interface.
     */
    virtual ~MPresencePublishing() {}


public: //Presence publishing requests


    /**
     * Subscribes own presence information.
     *
     * Subscribes own presence information from
     * remote presence service.
     *
     * Own presence subscription is asynchronous operation and
     * it might evolve network operations.
     *
     * @param [in] aPif
     *        Presence info filter defining the presence
     *        information to subscribe.
     *
     * @return The request ID identifying the issued request.
     */
    virtual TXIMPRequestId SubscribeOwnPresenceL(
                const MPresenceInfoFilter& aPif ) = 0;


    /**
     * Unsubscribes own presence information.
     *
     * Unsubscribing own presence is asynchronous operation
     * and it might evolve network operations.
     *
     * @return The request ID identifying the issued request.
     */
    virtual TXIMPRequestId UnsubscribeOwnPresenceL() = 0;



    /**
     * Publishes own presence information.
     *
     * Publishes own presence information to remote presence service.
     *
     * Presence publication is asynchronous operation and
     * evolves network operations.
     *
     * @param [in] aPresence
     *        Presence info defining the presence information
     *        to publish.
     *
     * @return The request ID identifying the issued request.
     */
    virtual TXIMPRequestId PublishOwnPresenceL(
                const MPresenceInfo& aPresence ) = 0;




public: //Presence watcher list requests


    /**
     * Subscribes presence watcher list.
     *
     * @return The request ID identifying the issued request.
     */
    virtual TXIMPRequestId SubscribePresenceWatcherListL() = 0;


    /**
     * Unsubscribes presence watcher list.
     *
     * @return The request ID identifying the issued request.
     */
    virtual TXIMPRequestId UnsubscribePresenceWatcherListL() = 0;

    };



#endif // MXIMPFWPRESENCEPUBLISHING_H




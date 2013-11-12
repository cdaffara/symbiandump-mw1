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
* Description:  Interface for presence protocol connection host.
*
*/

#ifndef MPROTOCOLPRESENCEDATAHOST_H
#define MPROTOCOLPRESENCEDATAHOST_H


#include <ximpbase.h>
#include <presenceprotocolpluginhostifids.hrh>
#include <e32std.h>


class MXIMPStatus;
class MPresenceObjectFactory;
class MXIMPProtocolConnectionHostObserver;
class MProtocolPresenceWatchingDataHost;
class MProtocolPresentityGroupsDataHost;
class MProtocolPresencePublishingDataHost;
class MProtocolPresenceAuthorizationDataHost;



// CLASS DECLARATION

/**
 * Interface for presence protocol connection host.
 * This interface is implemented by XIMP Framework.
 *
 * Presence protocol connection host interface defines
 * requests that a presence protocol implementation
 * can request (callback) from XIMP Framework.
 *
 * @see MXIMPProtocolConnection
 * @ingroup ximppresprotocolpluginhostapi
 * @since S60 v3.2
 */
class MProtocolPresenceDataHost : public MXIMPBase
    {
public:

    /** Interface ID for the MXIMPProtocolConnectionHost. */
    enum { KInterfaceId = PRES_IF_ID_PROTOCOL_PRESENCE_DATAHOST };


protected:

    /**
     * Protected destructor. MXIMPProtocolConnectionHost instancies
     * can't be destroyed via this interface.
     */
    virtual inline ~MProtocolPresenceDataHost() {};



public: // Access to sub-interfaces


    /**
     * Gets reference to object factory interface.
     *
     * Presence protocol connection implementation
     * uses factory interface to instantiate presence
     * objects.
     *
     * @return Object factory interface.
     *         No ownership is returned caller.
     */
    virtual MPresenceObjectFactory& PresenceObjectFactory() = 0;



    /**
     * Gets reference to
     * MProtocolPresenceAuthorizationDataHost interface.
     *
     * Presence protocol connection implementation uses returned
     * data host interface to manage the presence connections
     * presence auhtorization data.
     *
     * @return MProtocolPresenceAuthorizationDataHost interface.
     *         Interface ownership is not returned caller.
     *         Returned interface is guaranteed to remain valid (usable)
     *         as long as this source interface.
     */
    virtual MProtocolPresenceAuthorizationDataHost& AuthorizationDataHost() = 0;



    /**
     * Gets reference to
     * MProtocolPresencePublishingDataHost interface.
     *
     * Presence protocol connection implementation uses returned
     * data host interface to manage the presence connections
     * presence publication data.
     *
     * @return MProtocolPresencePublishingDataHost interface.
     *         Interface ownership is not returned caller.
     *         Returned interface is guaranteed to remain valid (usable)
     *         as long as this source interface.
     */
    virtual MProtocolPresencePublishingDataHost& PublishingDataHost() = 0;



    /**
     * Gets reference to
     * MProtocolPresenceWatchingDataHost interface.
     *
     * Presence protocol connection implementation uses returned
     * data host interface to manage the presence connections
     * presentity presence data.
     *
     * @return MProtocolPresenceWatchingDataHost interface.
     *         Interface ownership is not returned caller.
     *         Returned interface is guaranteed to remain valid (usable)
     *         as long as this source interface.
     */
    virtual MProtocolPresenceWatchingDataHost& WatchingDataHost() = 0;



    /**
     * Gets reference to
     * MProtocolPresentityGroupsDataHost interface.
     *
     * Presence protocol connection implementation uses returned
     * data host interface to manage the presence connections
     * presentity groups data.
     *
     * @return MProtocolPresentityGroupsDataHost interface.
     *         Interface ownership is not returned caller.
     *         Returned interface is guaranteed to remain valid (usable)
     *         as long as this source interface.
     */
    virtual MProtocolPresentityGroupsDataHost& GroupsDataHost() = 0;


    };



#endif // MPROTOCOLPRESENCEDATAHOST_H

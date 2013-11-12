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
* Description:  Interface for presence protocol connection object.
*
*/

#ifndef MPRESENCEPROTOCOFEATURES_H
#define MPRESENCEPROTOCOFEATURES_H


#include <e32std.h>
#include <badesca.h>
#include <presenceprotocolpluginifids.hrh>
#include <ximpbase.h>


class MXIMPContextClientInfo;
class MXIMPProtocolConnectionHost;
class MProtocolPresenceWatching;
class MProtocolPresencePublishing;
class MProtocolPresentityGroups;
class MProtocolPresenceAuthorization;



/**
 * Interface for presence protocol connection object.
 * Interface must be implemented by a XIMP Framework protocol
 * plug-ins.
 *
 * This interface models a connection from XIMP Framework to
 * single remote presence service, over certain presence protocol
 * implementation. XIMP Framework calls methods on this interface
 * based on XIMP Framework client requests.
 *
 * XIMP Framework retrieves MXIMPProtocolConnection
 * instances through MXIMPProtocolPlugin::AcquireConnectionL().
 * Same MXIMPProtocolConnection instance may be returned by multiple
 * MXIMPProtocolPlugin::AcquireConnectionL() calls. In this case
 * XIMP Framework shares MXIMPProtocolConnection instance
 * by multiple clients, and MXIMPProtocolConnection lifetime is
 * extended over utilizing client sessions. When last utilizing
 * client session is succesfully closed with
 * MXIMPProtocolConnection::CloseSession(), XIMP Framework
 * disposes the MXIMPProtocolConnection instance with
 * MXIMPProtocolPlugin::ReleaseConnection().
 *
 *
 * MProtocolPresenceFeatures implementation must provide
 * access following feature specific sub-interfaces:
 * - MProtocolPresenceWatching
 * - MProtocolPresencePublishing
 * - MProtocolPresentityGroups
 * - MProtocolPresenceAuthorization
 *
 * Each provided sub-interface defines a set of its own
 * presence managements requests, relating to presence watching
 * and subscribing, presence publication, presentity groups and
 * presentity group member management and to presence authorizations.
 * XIMP Framework calls these sub-interfaces to request on
 * spesific presence requests from protocol connection.
 *
 * @see MXIMPProtocolConnectionHost
 * @ingroup ximppresprotocolpluginapi
 * @since S60 v3.2
 */
class MProtocolPresenceFeatures : public MXIMPBase
    {
public:

    /** Interface ID for the MXIMPProtocolConnection. */
    enum { KInterfaceId = PRES_IF_ID_PROTOCOL_PRESENCE_FEATURES };


protected:

    /**
     * Protected destructor. MXIMPProtocolConnection
     * objects can't be deleted through this interface.
     *
     * XIMP Framework disposes MXIMPProtocolConnection
     * instances through MXIMPProtocolPlugin::ReleaseConnection().
     */
    virtual inline ~MProtocolPresenceFeatures() {};



public: // Access to sub-interfaces


    /**
     * Gets reference to presence watching interface.
     *
     * XIMP Framework calls this method to retrieve a reference
     * to protocols MProtocolPresenceWatching interface
     * implementation.
     *
     * Call and interface lifetime convention:
     * - This method isn't called before protocol has signalled
     *   MXIMPProtocolConnection::OpenSessionL() succesfully completed.
     *
     * - XIMP Framework may call this method multiple times for
     *   for one MXIMPProtocolConnection instance.
     *
     * - Each call per one MXIMPProtocolConnection instance,
     *   must return reference to same interface instance.
     *
     * - Returned interface instance must remain valid (usable)
     *   untill last client session, using originating presence protocol
     *   connection is successfully closed with
     *   MXIMPProtocolConnection::CloseSession().
     *
     * @return Requested interface.
     *         interface ownership is not returned caller.
     */
    virtual MProtocolPresenceWatching& PresenceWatching() = 0;



    /**
     * Gets reference to presence publishing interface.
     *
     * XIMP Framework calls this method to retrieve a reference
     * to protocols MProtocolPresencePublishing interface
     * implementation.
     *
     * Call and interface lifetime convention:
     * - This method isn't called before protocol has signalled
     *   MXIMPProtocolConnection::OpenSessionL() succesfully completed.
     *
     * - XIMP Framework may call this method multiple times for
     *   for one MXIMPProtocolConnection instance.
     *
     * - Each call per one MXIMPProtocolConnection instance,
     *   must return reference to same interface instance.
     *
     * - Returned interface instance must remain valid (usable)
     *   untill last client session, using originating presence protocol
     *   connection is successfully closed with
     *   MXIMPProtocolConnection::CloseSession().
     *
     * @return Requested interface.
     *         interface ownership is not returned caller.
     */
    virtual MProtocolPresencePublishing& PresencePublishing() = 0;



    /**
     * Gets reference to presentity groups interface.
     *
     * XIMP Framework calls this method to retrieve a reference
     * to protocols MProtocolPresentityGroups interface
     * implementation.
     *
     * Call and interface lifetime convention:
     * - This method isn't called before protocol has signalled
     *   MXIMPProtocolConnection::OpenSessionL() succesfully completed.
     *
     * - XIMP Framework may call this method multiple times for
     *   for one MXIMPProtocolConnection instance.
     *
     * - Each call per one MXIMPProtocolConnection instance,
     *   must return reference to same interface instance.
     *
     * - Returned interface instance must remain valid (usable)
     *   untill last client session, using originating presence protocol
     *   connection is successfully closed with
     *   MXIMPProtocolConnection::CloseSession().
     *
     * @return Requested interface.
     *         interface ownership is not returned caller.
     */
    virtual MProtocolPresentityGroups& PresentityGroups() = 0;



    /**
     * Gets reference to presence authorization interface.
     *
     * XIMP Framework calls this method to retrieve a reference
     * to protocols MProtocolPresenceAuthorization interface
     * implementation.
     *
     * Call and interface lifetime convention:
     * - This method isn't called before protocol has signalled
     *   MXIMPProtocolConnection::OpenSessionL() succesfully completed.
     *
     * - XIMP Framework may call this method multiple times for
     *   for one MXIMPProtocolConnection instance.
     *
     * - Each call per one MXIMPProtocolConnection instance,
     *   must return reference to same interface instance.
     *
     * - Returned interface instance must remain valid (usable)
     *   untill last client session, using originating presence protocol
     *   connection is successfully closed with
     *   MXIMPProtocolConnection::CloseSession().
     *
     * @return Requested interface.
     *         interface ownership is not returned caller.
     */
    virtual MProtocolPresenceAuthorization& PresenceAuthorization() = 0;

    };



#endif // MPRESENCEPROTOCOFEATURES_H

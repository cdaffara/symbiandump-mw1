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
* Description:  Interface for presence protocol plug-in object.
*
*/

#ifndef MXIMPFWPROTOCOLPLUGIN_H
#define MXIMPFWPROTOCOLPLUGIN_H


#include <e32std.h>
#include <badesca.h>
#include <ximpprotocolpluginifids.hrh>
#include <ximpbase.h>

class MXIMPServiceInfo;
class MXIMPProtocolPluginHost;
class MXIMPProtocolConnection;
class MXIMPContextClientInfo;



/**
 * Interface for presence protocol plug-in object.
 * Interface must be implemented by a concrete
 * XIMP Framework protocol plug-ins.
 *
 * This interface models presence protocol plug-in.
 * XIMP Framework loads a MXIMPProtocolPlugin
 * instance for needed protocol, by instantiating
 * protocol specific CXimpFwProtocolPluginBase
 * object through ECom architecture.
 *
 * XIMP Framework calls methods defined in this
 * interface to manage protocol connections based
 * on clients requests.
 *
 * @ingroup ximpprotocolpluginapi
 * @since S60 v3.2
 */
class MXIMPProtocolPlugin : public MXIMPBase
    {
public:

    /** Interface ID for the MXIMPProtocolPlugin. */
    enum { KInterfaceId = XIMP_IF_ID_PROTOCOL_PLUGIN };


protected:

    /**
     * Protected destructor. MXIMPProtocolPlugin
     * objects can't be deleted through this interface.
     */
     virtual inline ~MXIMPProtocolPlugin() {};



public: // Common


    /**
     * Primes presence protocol plug-in with its callback interface.
     *
     * Primes the presence protocol plug-in instance with
     * its XIMP Framework side callback interface.
     * Here given MXIMPProtocolPluginHost interface is
     * implemented and owned by the XIMP Framework, and
     * presence protocol plug-in must use it to communicate back to
     * XIMP Framework direction.
     *
     * Call and interface lifetime convention:
     * - This method is called by XIMP Framework before requesting
     *   any other requests from presence protocol plug-in instance.
     *
     * - This method is called just once for one MXIMPProtocolPlugin,
     *   just after loading MXIMPProtocolPlugin instance through ECom
     *   architecture.
     *
     * - Presence protocol plug-in instance must use here
     *   given specific host interface instance to communicate
     *   back to XIMP Framework direction from asynchronous
     *   requests.
     *
     * - The presence protocol plug-in implementation must
     *   store the here given host reference internally, so
     *   protocol plug-in implementation can access the host
     *   when needed.
     *
     * - Here given MXIMPProtocolPluginHost instance, is guaranteed
     *   to remain valid (usable), untill the protocol plug-in
     *   destroyed by deleting. However, protocol plug-in isn't
     *   allowed to call MXIMPProtocolPluginHost from its
     *   MXIMPProtocolPlugin::~MXIMPProtocolPlugin() destructor.
     *
     *
     * @param [in] aHost
     *        Reference to host interface, what this MXIMPProtocolPlugin
     *        instance must use when communicating back to XIMP Framework
     *        direction. Object ownership is not trasfered.
     */
    virtual void PrimeHost(
                    MXIMPProtocolPluginHost& aHost ) = 0;



public: // Managing of protocol connections


    /**
     * Acquires a presence protocol connection.
     *
     * XIMP Framework calls this to acquire a presence protocol
     * connection object for identified remote service and client.
     * Protocol plug-in implementation can either:
     * -# Always instantiate a new MXIMPProtocolConnection
     *    object to manage each client session as separated
     *    entity.
     *
     * -# Try to locate suitable existing connection from
     *    list of currently existing remote connections,
     *    and thus allow two or more client applications to share
     *    the remote presence connection. Protocol plug-in
     *    implementation can itself select in which circumstance
     *    to share the connection. Usually  sharing should happen,
     *    when two remote connections are targeted to same remote
     *    service and to same user account.
     *
     * Presence Protocol plug-in must maintain internally list
     * of existing presence service connection objects.
     * Direct ownership of acquired presence service connection
     * objects remain in the presence protocol plug-in. When
     * XIMP Framework doesn't anymore need the certain
     * acquired presence service connection, XIMP Framework
     * disposes MXIMPProtocolConnection instance with
     * MXIMPProtocolPlugin::ReleaseConnection(). If the same
     * MXIMPProtocolConnection instance was returned multiple
     * times from MXIMPProtocolPlugin::AcquireConnectionL(),
     * it is still disposed only once with ReleaseConnection(),
     * after closing the last utilizing client session.
     *
     * @param [in] aService
     *        Target address, credentials etc.
     *        info for remote presence connection.
     *
     * @param [in] aContextClient
     *        Object describing the client requesting the
     *        remote presence connection.
     *
     * @return New or existing presence protocol connection.
     *         Object ownership not transfered. However,
     *         returned object must remain valid (usable)
     *         untill XIMP Framework calls
     *         MXIMPProtocolPlugin::ReleaseConnection() to dispose
     *         the MXIMPProtocolConnection.
     */
    virtual MXIMPProtocolConnection& AcquireConnectionL(
                const MXIMPServiceInfo& aService,
                const MXIMPContextClientInfo& aContextClient ) = 0;


    /**
     * Releases presence service connection.
     *
     * XIMP Framework calls this to releases previously acquired
     * MXIMPProtocolConnection instance. XIMP Framework calls this
     * method just once for each MXIMPProtocolConnection instance, event
     * same MXIMPProtocolConnection instance were returned in several
     * AcquireConnectionL() calls.
     *
     * @param [in] aConnection
     *        Connection object to release.
     */
    virtual void ReleaseConnection(
                    MXIMPProtocolConnection& aConnection ) = 0;

    };



#endif // MXIMPFWPROTOCOLPLUGIN_H

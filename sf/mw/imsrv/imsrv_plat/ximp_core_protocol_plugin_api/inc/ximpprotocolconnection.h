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

#ifndef MXIMPFWPROTOCOLCONNECTION_H
#define MXIMPFWPROTOCOLCONNECTION_H


#include <e32std.h>
#include <badesca.h>
#include <ximpprotocolpluginifids.hrh>
#include <ximpbase.h>


class MXIMPContextClientInfo;
class MXIMPProtocolConnectionHost;
class MProtocolPresenceFeatures;
class MProtocolImFeatures;



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
 * MXIMPProtocolConnection implementation must provide
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
 * @ingroup ximpprotocolpluginapi
 * @since S60 v3.2
 */
class MXIMPProtocolConnection : public MXIMPBase
    {
public:

    /** Interface ID for the MXIMPProtocolConnection. */
    enum { KInterfaceId = XIMP_IF_ID_PROTOCOL_CONNECTION };


protected:

    /**
     * Protected destructor. MXIMPProtocolConnection
     * objects can't be deleted through this interface.
     *
     * XIMP Framework disposes MXIMPProtocolConnection
     * instances through MXIMPProtocolPlugin::ReleaseConnection().
     */
    virtual inline ~MXIMPProtocolConnection() {};



public: // Common


    /**
     * Primes protocol connection with its callback interface.
     *
     * Primes the presence protocol connection instance with
     * its XIMP Framework side callback interface.
     * Here given MXIMPProtocolConnectionHost interface is
     * implemented and owned by the XIMP Framework, and
     * presence protocol connection must use it to communicate back to
     * XIMP Framework direction.
     *
     * Call and interface lifetime convention:
     * - This method is called by XIMP Framework before
     *   requesting any other requests from protocol connection
     *   instance.
     *
     * - This method is called just once for each MXIMPProtocolConnection,
     *   even the same MXIMPProtocolConnection instance is returned
     *   multiple times from MXIMPProtocolPlugin::AcquireConnectionL().
     *
     * - Each presence protocol connection instance must use here
     *   given specific host interface instance to communicate back
     *   to XIMP Framework direction
     *
     * - The presence protocol connection implementation must
     *   store the here given host reference internally, so
     *   protocol connection implementation can access the host
     *   when needed.
     *
     * - Here given MXIMPProtocolConnectionHost instance, is guaranteed
     *   to remain valid (usable), untill the presence protocol connection
     *   has completed the last MXIMPProtocolConnection::CloseSession()
     *   request.
     *
     *
     * @param [in] aHost
     *        Reference to host interface, what this MXIMPProtocolConnection
     *        instance must use to communicate back to XIMP Framework
     *        direction. Object ownership is not trasfered.
     */
    virtual void PrimeHost(
                    MXIMPProtocolConnectionHost& aHost ) = 0;



public: // Connection handling


    /**
     * Opens a session for the identified client through
     * the presence protocol connection.
     *
     * XIMP Framework calls this method to open a session to
     * remote presence service, for here specified client.
     * Remote presence service was identified when the
     * MXIMPProtocolConnection instance was retrieved through
     * MXIMPProtocolPlugin::AcquireConnectionL().
     *
     * @param [in] aContextClient
     *        Object describing the presence connection client.
     *
     * @param [in] aReqId
     *        Request ID identifying the request. Presence protocol
     *        connection implementation must cache here given ID and
     *        use it when completing the request later through the
     *        MXIMPProtocolConnectionHost::HandleRequestCompleted().
     */
    virtual void OpenSessionL(
                const MXIMPContextClientInfo& aContextClient,
                TXIMPRequestId aReqId ) = 0;


    /**
     * Opens a session for the identified client through
     * the presence protocol connection.
     *
     * XIMP Framework calls this method to open a session to
     * remote presence service, for here specified client.
     * Remote presence service was identified when the
     * MXIMPProtocolConnection instance was retrieved through
     * MXIMPProtocolPlugin::AcquireConnectionL().
     *
     * @param [in] aSettingsId
     *        SettingsId for the presence connection client.
     *
     * @param [in] aReqId
     *        Request ID identifying the request. Presence protocol
     *        connection implementation must cache here given ID and
     *        use it when completing the request later through the
     *        MXIMPProtocolConnectionHost::HandleRequestCompleted().
     */
    virtual void OpenSessionL(
                const TInt& aSettingsId,
                TXIMPRequestId aReqId ) = 0;
     
    
    /**
     * Closes the identified client session from the
     * presence connection.
     *
     * XIMP Framework calls this method to close
     * the remote presence service session, for here specified
     * client. When last using client session is closed,
     * XIMP Framework consideres the remote presence connection
     * as disconnect, and disposes MXIMPProtocolConnection instance
     * with MXIMPProtocolPlugin::ReleaseConnection().
     *
     * @param [in] aContextClient
     *        Client which session to close.
     *
     * @param [in] aReqId
     *        Request ID identifying the request. Presence protocol
     *        connection implementation must cache here given ID and
     *        use it when completing the request later through the
     *        MXIMPProtocolConnectionHost::HandleRequestCompleted().
     */
    virtual void CloseSession(
                const MXIMPContextClientInfo& aContextClient,
                TXIMPRequestId aReqId ) = 0;


    /**
     * Gets features that presence protocol connection supports.
     *
     * Presence protocol connection should negotiate
     * available features during the connection establish
     * and this method should return the supported features
     * from presence protocol connection internal cache.
     *
     * @param [out] aFeatures
     *        Return parameter where to append supported
     *        presence protocol connection features.
     *        Feature IDs are defined in NXIMPFeature namespace.
     */
    virtual void GetSupportedFeaturesL(
                CDesC8Array& aFeatures ) const = 0;



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
    virtual MProtocolPresenceFeatures& ProtocolPresenceFeatures() = 0;
    

    /**
     * Gets reference to protocol specific interface.
     *
     * XIMP Framework calls this method to retrieve a reference
     * to protocol interface implementation.
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
    virtual TAny* GetProtocolInterface(TInt aInterfaceId) = 0;

    };



#endif // MXIMPFWPROTOCOLCONNECTION_H

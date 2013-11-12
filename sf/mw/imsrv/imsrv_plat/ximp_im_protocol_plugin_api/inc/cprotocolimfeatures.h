/*
* Copyright (c) 2006-2009 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Interface for IM Protocol connection object.
*
*/

#ifndef CPROTOCOLIMFEATURES_H
#define CPROTOCOLIMFEATURES_H


#include <e32std.h>
#include <ximpbase.h>
#include <immanagementifids.hrh>
#include <ximpcontext.h>

class MProtocolImGroup;
class MProtocolImConversation;
class MProtocolImInvitation;
class MProtocolImSearch;
class MProtocolImDataHost;



/**
 * Interface for IM protocol connection object.
 * Interface must be implemented by a XIMP Framework
 * plug-ins.
 *
 * This interface models a connection from XIMP Framework to
 * single remote IM service, over certain IM protocol
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
 * CProtocolImFeatures implementation must provide
 * access following feature specific sub-interfaces:
 * - MProtocolImGroup
 * - MProtocolImConversation
 * - MProtocolImInvitation
 * - MProtocolImSearch
 *
 * Each provided sub-interface defines a set of its own
 * im managements requests, relating to im groups,search,im and
 * im invitation.
 * IM calls these sub-interfaces to request on
 * spesific im requests from protocol connection.
 *
 * @see MXIMPProtocolConnectionHost
 * @ingroup protocolimpluginapi
 * @since S60 
 */
class CProtocolImFeatures : public CBase, public MXIMPBase
    {
    
public:

    /** Interface ID for the CProtocolImFeatures. */
    enum { KInterfaceId = IM_IF_ID_PROTOCOL_IM_FEATURES };
    
    /** All possible subinterfaces plugin may support. */
    enum KImFeatureSubInterfaces 
        {
        EProtocolImGroup            = 1,
        EProtocolImConversation     = 2,
        EProtocolImInvitation       = 4,
        EProtocolImSearch           = 8
        };
    
	/**
     * Factory method to instantiate CProtocolImFeatures.
     *
     * Factory method to instantiate platform default
     * MXIMPClient implementation through the ECom.
     * If the default IM Feature isn't supported
     * in the platform, leaves with errorcode signalled
     * from ECom.
     *
     * @return The new Im client object. Object
     *         ownership is returned to caller.
     */
    static inline CProtocolImFeatures* NewL();
    
    /**
     * Factory method to instantiate CProtocolImFeatures with
     * initial parameters and specified implementation UID.
     *
     * If the specified IM Feature isn't supported
     * in the platform, leaves with errorcode signalled
     * from ECom.
     * 
     * @since S60 v5.0
     * @param aImplementationUid    Implementation UID of the plugin to be loaded
     * @param aInitParams           Initialization parameters
     * @return The new Im client object.
     */
    static inline CProtocolImFeatures* NewL( TUid aImplementationUid,
        TAny* aInitParams );

    /**
     * Protected destructor. MXIMPProtocolConnection
     * objects can't be deleted through this interface.
     *
     * XIMP Framework disposes MXIMPProtocolConnection
     * instances through MXIMPProtocolPlugin::ReleaseConnection().
     */
    virtual inline ~CProtocolImFeatures();
    
    /**
     * Returns bitfield representing supported sub-interfaces. Sub-interfaces
     * are defined with KImFeatureSubInterfaces. If client tries to access
     * non-supported sub-interface, panic will occur.
     *
     * @return  Supported sub-interfaces as a bitwield.
     */
    virtual TInt SupportedSubInterfaces() const = 0;

protected:

    /**
     * Default constructor to zero initialize
     * the iEcomDtorID member.
     */
    inline CProtocolImFeatures();


public: // Access to sub-interfaces


    /**
     * Gets reference to im group interface.
     *
     * IM calls this method to retrieve a reference
     * to protocols MProtocolImGroup interface
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
     *   untill last client session, using originating im protocol
     *   connection is successfully closed with
     *   MXIMPProtocolConnection::CloseSession().
     *
     * @return Requested interface.
     *         interface ownership is not returned caller.
     */
   virtual MProtocolImGroup& ImGroup() = 0;



    /**
     * Gets reference to Im Conversation interface.
     *
     * IM calls this method to retrieve a reference
     * to protocols MProtocolImConversation interface
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
     *   untill last client session, using originating im protocol
     *   connection is successfully closed with
     *   MXIMPProtocolConnection::CloseSession().
     *
     * @return Requested interface.
     *         interface ownership is not returned caller.
     */
    virtual MProtocolImConversation& ImConversation() = 0;



    /**
     * Gets reference to im groups interface.
     *
     * IM calls this method to retrieve a reference
     * to protocols MProtocolImInvitation interface
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
     *   untill last client session, using originating im protocol
     *   connection is successfully closed with
     *   MXIMPProtocolConnection::CloseSession().
     *
     * @return Requested interface.
     *         interface ownership is not returned caller.
     */
    virtual MProtocolImInvitation& ImInvitation() = 0;



    /**
     * Gets reference to im search interface.
     *
     * IM calls this method to retrieve a reference
     * to protocols MProtocolImSearch interface
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
     *   untill last client session, using originating im protocol
     *   connection is successfully closed with
     *   MXIMPProtocolConnection::CloseSession().
     *
     * @return Requested interface.
     *         interface ownership is not returned caller.
     */
    virtual MProtocolImSearch& ImSearch() = 0;

    
    /**
     * Sets data host.
     * 
     * IM calls this method to specify datahost.
     * 
     * @since   S60 v5.0
     * @param   aHost   data host
     * @return  void
     */    
    virtual void SetHost(MProtocolImDataHost& aHost) = 0;
  
private: // Data

    /**
     * Unique instance identifier key
     */
    TUid iEcomDtorID;
    };

#include <cprotocolimfeatures.inl>

#endif // CPROTOCOLIMFEATURES_H

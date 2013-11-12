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
* Description:  Interface for XIMP context.
*
*/

#ifndef MXIMPFWPRESENCECONTEXT_H
#define MXIMPFWPRESENCECONTEXT_H

#include <e32std.h>
#include <ximpbase.h>
#include <ximpmanagementifids.hrh>


class MXIMPClient;
class MXIMPContextObserver;
class MXIMPFeatureInfo;
class MXIMPObjectFactory;



/**
 * Interface for XIMP context.
 * This interface is implemented by the XIMP Framework.
 *
 * MXIMPContext is root interface for accessing
 * and updating XIMP data to remote XIMP service.
 * MXIMPContext client binds MXIMPContext
 * interface to desired remote XIMP service and uses
 * other MXIMPContext sub interfaces to publish
 * and access XIMP data.
 *
 * Following sub interfaces can be accessed from
 * from MXIMPContext:
 *   - MXIMPObjectFactory
 *
 * All Presence events (context state, XIMP data,
 * request completion etc. ones) are delivered through
 * MXIMPContextObserver interface.
 *
 * @ingroup ximpfwmanagementapi
 * @since S60 v3.2
 */
class MXIMPContext : public MXIMPBase
    {
public:

    /** Interface ID for the MXIMPContext. */
    enum { KInterfaceId = XIMP_IF_ID_CONTEXT };


public:

    /**
     * Public destructor.
     * Objects can be deleted through this interface.
     */
    virtual ~MXIMPContext() {}



public:

    /**
     * Reqisters XIMP context event observer.
     *
     * Reqisters observer is notified from XIMP context events.
     * XIMP Framework client side events are listed in
     * MXIMPContextObserver header.
     *
     * Event delivery uses client side active object to callback
     * the reqistered observer. If client thread is blocked with
     * User::WaitFor...() or other high priority active objects,
     * event notification is delayed until XIMP Framework
     * client side active objects get their execution turn.
     * When client is blocked, XIMP Framework queues events,
     * and delivers those later in FIFO order to observer. However,
     * the XIMP Framework limits the que size and thus
     * events might get dropped if the the client is blocked
     * too long.
     *
     * Even though client shouldn't perform any heavy
     * processing in the event callback, XIMP Framework
     * allows the client client to issue following kind of
     * requests to XIMP Framework, from event callback:
     * - Data management relatred asynchronous & synchronous requests
     * - Observer registeration and unregisteration
     * - Presence context undbind
     * - Presence contex destroy
     *
     * @param [in] aObserver
     *        The observer to be notified from context events.
     *        If the observer is already registered, event filter
     *        associated to it is updated.
     *
     * @param [in] aEventFilter
     *        List of event types that the observer accepts.
     *        Event types are identified with their event interface IDs.
     *        If NULL, all event types are delivered to observer.
     */
    virtual void RegisterObserverL(
            MXIMPContextObserver& aObserver,
            const TArray<TInt32>* aEventFilter = NULL ) = 0;


    /**
     * Unregisters context event observer.
     *
     * @param [in] aObserver
     *        The observer to unregister.
     */
    virtual void UnregisterObserver(
            MXIMPContextObserver& aObserver ) = 0;



    /**
     * Binds the XIMP context to remote XIMP service.
     *
     * Binds the XIMP context to given XIMP service,
     * with given credentials and by using defined protocol plug-in.
     * Binding is asynchronous operation and evolves network interaction.
     * Used protocol plug-in is identified explicitly with its
     * implementation UID.
     *
     * @param [in] aProtocolImpUid
     *        Protocol implementation UID identifying which
     *        PSC protocol implementation to use.
     *
     * @param [in] aServiceAddress
     *        Service address string identifying the remote
     *        XIMP service.
     *
     * @param [in] aIapId
     *        Internet Accesspoint for accessing the remote
     *        XIMP service connection.
     *
     * @param [in] aUsername
     *        Username (credential) for accessing the remote
     *        XIMP service.
     *
     * @param [in] aPassword
     *        Password (credential) for accessing the remote
     *        XIMP service.
     *
     * @return The request ID identifying the started binding request.
     *         Client can use request ID to find out correct
     *         completion event from the MXIMPContextObserver
     *         event stream.
     */
    virtual TXIMPRequestId BindToL(
                TUid aProtocolImpUid,
                const TDesC16& aServiceAddress,
                const TDesC16& aUsername,
                const TDesC16& aPassword,
                TInt32 aIapId ) = 0;


    virtual TXIMPRequestId BindToL(
                TUid aProtocolImpUid,
                const TDesC16& aServiceAddress,
                const TDesC16& aUsername,
                const TDesC16& aPassword,
                const TDesC16& aClientId,
                TInt32 aIapId ) = 0;

    /**
         * Binds the XIMP context to remote XIMP service.
         *
         * Binds the XIMP context to given XIMP service,
         * with given credentials and by using defined protocol plug-in.
         * Binding is asynchronous operation and evolves network interaction.
         * Used protocol plug-in is identified explicitly with its
         * implementation UID.
         *
         * @param [in] aProtocolImpUid
         *        Protocol implementation UID identifying which
         *        PSC protocol implementation to use.
         *
         * @param [in] aServiceAddress
         *        Service address string identifying the remote
         *        XIMP service.
         *
         * @param [in] aUsername
         *        Username (credential) for accessing the remote
         *        XIMP service.
         *
         * @param [in] aPassword
         *        Password (credential) for accessing the remote
         *        XIMP service.
         * @param [in] aServiceId  - Service Id
         * 
         * @return The request ID identifying the started binding request.
         *         Client can use request ID to find out correct
         *         completion event from the MXIMPContextObserver
         *         event stream.
         */
    virtual TXIMPRequestId BindToL( TUid aProtocolImpUid,
                            TInt aServiceId ) = 0;

    virtual TXIMPRequestId BindToL(
                TUid aProtocolImpUid,
                const TDesC16& aUsername,
                const TDesC16& aPassword,
                TInt aServiceId ) = 0;
    /**
     * Unbinds the XIMP context from the to currently binded
     * XIMP service. Unbinding is asynchronous operation and
     * might evolve network operations. Unbinding or waiting
     * unbinding completion isn't mandatory for the client.
     * XIMPFw performs unbinding implicitly when the bound
     * MXIMPContext object is destroyed.
     *
     * @return The request ID identifying the started unbinding
     *         request. Client can use request ID to find out correct
     *         completion event from the MXIMPContextObserver
     *         event stream.
     */
    virtual TXIMPRequestId UnbindL() = 0;



    /**
     * Retrieves Presence Contex features.
     *
     * Retrieves features supported by the Presence Contex.
     * Features returned here may be "less" than what the
     * protocol indicated, since connected remote service
     * may limit available features.
     *
     * @return Features supported in Presence Contex.
     *         Exact feature IDs are defined in NXIMPFeature
     *         namespace. Object ownership is returned
     *         to caller.
     */
    virtual MXIMPFeatureInfo* GetContextFeaturesLC() const = 0;



public: //Sub interfaces for XIMP context


    /**
     * Gets reference to object factory interface.
     *
     * Returned object factory interface is used
     * to instantiate XIMP objects.
     *
     * @return Object factory interface.
     *         No ownership is returned caller.
     */
    virtual MXIMPObjectFactory& ObjectFactory() const = 0;

    };


#endif // MXIMPFWPRESENCECONTEXT_H




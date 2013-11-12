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
* Description:  Interface for XIMP protocol connection host.
*
*/

#ifndef MXIMPFWPROTOCOLCONNECTIONHOST_H
#define MXIMPFWPROTOCOLCONNECTIONHOST_H


#include <ximpbase.h>
#include <ximpprotocolpluginhostifids.hrh>
#include <e32std.h>


class MXIMPStatus;
class MXIMPObjectFactory;
class MXIMPProtocolConnectionHostObserver;
class MProtocolPresenceDataHost;
class MProtocolImDataHost;


// CLASS DECLARATION

/**
 * Interface for XIMP protocol connection host.
 * This interface is implemented by XIMP Framework.
 *
 * XIMP protocol connection host interface defines
 * requests that a XIMP protocol implementation
 * can request (callback) from XIMP Framework.
 *
 * @see MXIMPProtocolConnection
 * @ingroup ximpprotocolpluginhostapi
 * @since S60 v3.2
 */
class MXIMPProtocolConnectionHost : public MXIMPBase
    {
public:

    /** Interface ID for the MXIMPProtocolConnectionHost. */
    enum { KInterfaceId = XIMP_IF_ID_PROTOCOL_CONNECTION_HOST };


protected:

    /**
     * Protected destructor. MXIMPProtocolConnectionHost instancies
     * can't be destroyed via this interface.
     */
    virtual inline ~MXIMPProtocolConnectionHost() {};



public: // Request completion handling


    /**
     * Notifies XIMP Framework that protocol connection has
     * completed (successfully or otherwise) the request handling.
     *
     * @param [in] aReqId
     *        Request ID identifying the request which handling
     *        is completed. This must equal to value given in
     *        the processing initiation. If no pending XIMP
     *        protocol connection request is found with this
     *        request ID, protocol connection is paniced.
     *
     * @param [in] aResultCode
     *        Result code from request handling.
     */
    virtual void HandleRequestCompleted(
                TXIMPRequestId aReqId,
                TInt aResultCode ) = 0;



    /**
     * Notifies XIMP Framework that protocol connection has
     * completed (successfully or otherwise) the request handling.
     *
     * @param [in] aReqId
     *        Request ID identifying the request which handling
     *        is completed. This must equal to value given in
     *        the processing initiation. If no pending XIMP
     *        protocol connection request is found with this
     *        request ID, protocol connection is paniced.
     *
     * @param [in] aResult
     *        Status object describing result from request handling.
     *        Object ownership is transfered always to XIMPFw.
     *        NULL value not allowed.
     */
    virtual void HandleRequestCompleted(
                TXIMPRequestId aReqId,
                MXIMPStatus* aResult ) = 0;



    /**
     * Notifies XIMP Framework that protocol connection has
     * completed (successfully or otherwise) the request handling.
     *
     * @param [in] aReqId
     *        Request ID identifying the request which handling
     *        is completed. This must equal to value given in
     *        the processing initiation. If no pending XIMP
     *        protocol connection request is found with this
     *        request ID, protocol connection is paniced.
     *
     * @param [in] aResult
     *        Status object describing result from request handling.
     *        Object ownership is transfered always to XIMPFw.
     *        NULL value not allowed.
     *
     * @param [in] aParameter
     *        Additional request complete parameters.
     *        NULL if no additional parameters to transfer XIMPFw.
     *        Parameter may be single MXIMPBase derived
     *        object or MXIMPRestrictedObjectCollection object
     *        containing multiple objects. Ownership of given
     *        parameter (also the container and contained objects)
     *        is transfered always to XIMPFw.
     */
    virtual void HandleRequestCompleted(
                TXIMPRequestId aReqId,
                MXIMPStatus* aResult,
                MXIMPBase* aParameter ) = 0;



public: // Connection status handling


    /**
     * Notifies XIMP Framework that protocol connection to
     * remote XIMP service is terminated due certain reason.
     *
     * In this case, XIMP Framework doesn't try to re-open
     * the client sessions.
     *
     * @param [in] aReason
     *        Status object describing more closely
     *        the protocol connection termination reason.
     *        Object ownership is transfered always to
     *        XIMP Framework. NULL value is allowed.
     *
     * @return The request id for the handle operation.
     */
    virtual TXIMPRequestId HandleConnectionTerminated(
                MXIMPStatus* aReason ) = 0;



public: // Access to sub-interfaces


    /**
     * Gets reference to object factory interface.
     *
     * XIMP protocol connection implementation
     * uses factory interface to instantiate XIMP
     * objects.
     *
     * @return Object factory interface.
     *         No ownership is returned caller.
     */
    virtual MXIMPObjectFactory& ObjectFactory() = 0;



    /**
     * Gets reference to
     * MProtocolPresenceDataHost interface.
     *
     * Presence protocol connection implementation uses returned
     * data host interface to manage the Presence connection's
     * presence auhtorization data.
     *
     * @return MProtocolPresenceAuthorizationDataHost interface.
     *         Interface ownership is not returned caller.
     *         Returned interface is guaranteed to remain valid (usable)
     *         as long as this source interface.
     */
    virtual MProtocolPresenceDataHost& ProtocolPresenceDataHost() = 0;
    

public: // Observer registration


    /**
     * Reqisters XIMP connection host event observer.
     *
     * Reqisters observer to be notified from XIMP connection
     * host events.
     *
     * @param [in] aObserver
     *        The observer to be notified from XIMP
     *        connection host events. If the observer is already
     *        registered, event filter associated to observer
     *        is updated.
     *
     * @param [in] aEventFilter
     *        List of event types that the observer accepts.
     *        Event types are identified with their interface IDs.
     *        If NULL, all event types are delivered to observer.
     *        In this case, observer must be prepared to handle
     *        gracefully unknown event types, which are added
     *        in future when extending XIMP Framework
     *        functionality.
     */
    virtual void RegisterObserverL(
                MXIMPProtocolConnectionHostObserver& aObserver,
                const TArray<TInt32>* aEventFilter = NULL ) = 0;


    /**
     * Unregisters XIMP connection host observer.
     *
     * @param [in] aObserver
     *        The observer to unregister.
     */
    virtual void UnregisterConnectionHostObserver(
                MXIMPProtocolConnectionHostObserver& aObserver ) = 0;


    };



#endif // MXIMPFWPROTOCOLCONNECTIONHOST_H

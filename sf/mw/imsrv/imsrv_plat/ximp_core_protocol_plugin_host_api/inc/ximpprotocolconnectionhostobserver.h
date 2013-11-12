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
* Description:  XIMP Protocol Connection Host interface.
*
*/

#ifndef MXIMPFWPROTOCOLCONNECTIONHOSTOBSERVER_H
#define MXIMPFWPROTOCOLCONNECTIONHOSTOBSERVER_H

#include <e32std.h>


class MXIMPBase;
class MXIMPProtocolConnectionHost;


// CLASS DECLARATION

/**
 * XIMP connection host observer interface.
 * All XIMP connection host events are delivered
 * through this interface back to protocol adaptation
 * implementation.
 *
 * @ingroup ximpprotocolpluginhostapi
 * @since S60 v3.2
 */
class MXIMPProtocolConnectionHostObserver
    {
protected:

    /**
     * Protected destructor. Observer instancies
     * can't be destroyed via this interface.
     */
    virtual ~MXIMPProtocolConnectionHostObserver() {}


public:


    /**
     * Event callback.
     *
     * Events are delivered as MXIMPBase typed objects.
     * Protocol adaptation can use MXIMPBase::InterfaceId()
     * to identify the real event interface type. Observer
     * can use cast methods provided in MXimpFw...Event
     * interfaces to cast to real event interface.
     *
     * @param [in] aHost
     *        XIMP host object from where this event is coming.
     *        Provided as reference for protocol adaptation, so
     *        observer can distinguish events coming from different
     *        hosts if listened in single observer.
     *
     * @param [in] aEvent
     *        The event object. Client can use MXIMPBase::InterfaceId()
     *        to determine correct event type and interface cast
     *        methods from MXimpFw...Event interfaces to cast object
     *        to real event interface.
     */
    virtual void HandleProtocolConnectionHostEvent(
                const MXIMPProtocolConnectionHost& aHost,
                const MXIMPBase& aEvent ) = 0;



    /**
     * Extension point for this observer interface.
     *
     * Returns an pointer to MXIMPProtocolConnectionHostObserver
     * extension interface or NULL if the concrete observer
     * implementation doesn't support requested extension.
     *
     * @param [in] aInterfaceId
     *        Interface ID identifying the requested extension
     *        interface.
     *
     * @return Pointer to requested extension interface or NULL.
     */
    virtual TAny* ProtocolConnectionHostObserverExtensionOrNull(
                TInt32 aInterfaceId  ) { ( void ) aInterfaceId; return NULL; }

    };



#endif // MXIMPFWPROTOCOLCONNECTIONHOSTOBSERVER_H

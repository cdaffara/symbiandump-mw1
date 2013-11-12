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
* Description:  Interface for XIMP context event observer.
*
*/

#ifndef XIMPFWPRESENCECONTEXTOBSERVER_H
#define XIMPFWPRESENCECONTEXTOBSERVER_H

#include <e32std.h>
#include <ximpbase.h>


class MXIMPContext;
class MXimpFwEventBase;



/**
 * Interface for XIMP context event observer.
 * This interface must be implemented by the client which
 * is using the XIMP Framework services, to receive status
 * and data notification callbacks from XIMP Framework.
 *
 * All callbacks from XIMP Framework XIMP context
 * to client direction happen through this interface
 * HandlePresenceContextEvent() method. Data content in callbacks
 * is presented to client with following event specific
 * interfaces:
 *  - MXIMPRequestCompleteEvent
 *  - MXIMPContextStateEvent
 *  - MOwnPresenceEvent
 *  - MPresentityPresenceEvent
 *  - MPresentityGroupListEvent
 *  - MPresentityGroupContentEvent
 *  - MPresenceWatcherListEvent
 *  - MPresenceGrantRequestListEvent
 *  - MPresenceBlockListEvent
 *
 * See event interface type specific documentation
 * for further information about different event types.
 *
 * Client can dispatch and cast event types on its own
 * event callback implementation in following way:
 * @code
 * void CMyObserverClass::HandlePresenceContextEvent(
 *      const MXIMPContext& aContext,
 *      const MXIMPBase& aEvent )
 *      {
 *      switch( aEvent.GetInterfaceId() )
 *          {
 *          case MXIMPRequestCompleteEvent::KInterfaceId:
 *              {
 *              const MXIMPRequestCompleteEvent* complEvent =
 *                  TXIMPGetInterface< const MXIMPRequestCompleteEvent >::From( aEvent,
 *                                                                              MXIMPBase::EPanicIfUnknown );
 *
 *              // Do here request complete event specific handling
 *              complEvent->RequestId();
 *
 *              break;
 *              }
 *
 *
 *          case MXIMPContextStateEvent::KInterfaceId:
 *              {
 *              const MXIMPContextStateEvent* ctxtStateEvent =
 *                  TXIMPGetInterface< const MXIMPContextStateEvent >::From( aEvent,
 *                                                                                   MXIMPBase::EPanicIfUnknown );
 *              // Do here context state event specific handling
 *              ctxtStateEvent->ContextState();
 *
 *              break;
 *              }
 *
 *
 *          default:
 *              {
 *              // New event types may be introduced in newer XIMP Framework
 *              // versions, so handle unknown event types gracefully.
 *              break;
 *              }
 *          }
 *      }
 *
 * @endcode
 *
 *
 * @ingroup ximpfwmanagementapi
 * @since S60 v3.2
 */
class MXIMPContextObserver
    {
protected:

    /**
     * Protected destructor. Observer instancies
     * can't be destroyed via this interface.
     */
    virtual ~MXIMPContextObserver() {}


public:

    /**
     * Event callback.
     *
     * Events are delivered as MXIMPBase typed objects to client.
     * Client can use MXIMPBase::GetInterfaceId() to determine
     * the real event interface type.
     *
     * @param [in] aContext
     *        Presence context object from where this event is coming.
     *        Provided as reference for client, so client can
     *        distinguish events from different contexts.
     *
     * @param [in] aEvent
     *        The event object. Client can use MXIMPBase::GetInterfaceId()
     *        to determine real event type and TXIMPGetInterface helper
     *        to cast base typed object to real event interface.
     *        Object ownership isn't transfered to client.
     */
    virtual void HandlePresenceContextEvent(
                const MXIMPContext& aContext,
                const MXIMPBase& aEvent ) = 0;


    /**
     * Extension point for this observer interface.
     *
     * Returns an pointer to MXIMPContextObserver
     * extension interface or NULL if the concrete observer
     * implementation doesn't support requested extension.
     *
     * @param [in] aInterfaceId
     *        Interface ID identifying the requested extension
     *        interface.
     *
     * @return Pointer to requested extension interface or NULL.
     */
    virtual TAny* PresenceContextObserverExtensionOrNull(
                TInt32 aInterfaceId ) { ( void ) aInterfaceId; return NULL; }

    };

#endif // XIMPFWPRESENCECONTEXTOBSERVER_H




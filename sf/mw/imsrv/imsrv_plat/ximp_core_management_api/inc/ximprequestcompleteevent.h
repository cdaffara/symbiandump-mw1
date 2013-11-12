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
* Description:  Event interface for request completion notification.
*
*/

#ifndef MXIMPFWREQUESTCOMPLETEEVENT_H
#define MXIMPFWREQUESTCOMPLETEEVENT_H

#include <e32std.h>
#include <ximpbase.h>
#include <ximpmanagementifids.hrh>


class MXIMPStatus;


/**
 * Event interface for request completion notification.
 *
 * This interface is implemented by the XIMP Framework,
 * and it is used to notify clients that XIMP Framework
 * has completed processing of certain client request.
 *
 * XIMP Framework signalls this event it to registered
 * client observers through MXIMPContextObserver
 * observer interface. Clients do need to perform any specific
 * subscribe request for this event type, just register a valid
 * observer with MXIMPPscContext::RegisterObserverL(),
 * with such event type filter, that this MXIMPRequestCompleteEvent
 * interface is accepted.
 *
 * @ingroup ximpfwmanagementapievents
 * @since S60 v3.2
 */
class MXIMPRequestCompleteEvent : public MXIMPBase
    {
public:

    /** Interface ID for the MXIMPRequestCompleteEvent. */
    enum { KInterfaceId = XIMP_IF_ID_REQUEST_COMPLETE_EVENT };


protected:

    /**
     * Protected destructor.
     * Event objects can't be deleted via this interface.
     */
    virtual ~MXIMPRequestCompleteEvent() {}


public:


    /**
     * Returns request id of the completed request.
     *
     * @return The request id.
     */
    virtual const TXIMPRequestId& RequestId() const = 0;


    /**
     * Returns request completion result.
     *
     * @return The request completion result object.
     *         Object ownership is not returned to caller.
     */
    virtual const MXIMPStatus& CompletionResult() const = 0;


    /**
     * Looks up the extension data object having given
     * interface type.
     *
     * Protocol adaptation or XIMP Framework itself
     * may signal request specific extra data to client
     * through request complete event. Client can use
     * this method to retrieve a pointer to extra data
     * object and later cast returned base object
     * to real object interface type with TXIMPGetInterface.
     *
     * @param [in] aInterfaceId
     *        The interface type of the extra data object
     *        to be looked up.
     *
     * @return Pointer to object of given interface type
     *         or NULL if no object having given interface
     *         was found from this request complete events
     *         extra data collection. Object ownership
     *         is not returned to caller.
     */
    virtual const MXIMPBase* LookupCompletionDataByType(
                TInt32 aInterfaceId ) const = 0;

    };


#endif // MXIMPFWREQUESTCOMPLETEEVENT_H




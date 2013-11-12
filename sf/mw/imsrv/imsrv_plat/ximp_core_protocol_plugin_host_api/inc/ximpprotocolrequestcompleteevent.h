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
* Description:  Interface for protocol request completion event.
*
*/

#ifndef MXIMPFWPROTOCOLREQUESTCOMPLETEEVENT_H
#define MXIMPFWPROTOCOLREQUESTCOMPLETEEVENT_H

#include <e32std.h>
#include <ximpbase.h>
#include <ximpprotocolpluginhostifids.hrh>



/**
 * Interface for protocol request completion event.
 * This interface is implemented by XIMP Framework.
 *
 * This event is signalled from XIMP Framework to
 * protocol adaptation to inform protocol implementation
 * about its request completion (succesfully or otherwise).
 *
 * @ingroup ximpprotocolpluginhostapi
 * @since S60 v3.2
 */
class MXIMPProtocolRequestCompleteEvent : public MXIMPBase
    {
public:

    /** Interface ID for the MXIMPProtocolRequestCompleteEvent. */
    enum { KInterfaceId = XIMP_IF_ID_PROTOCOL_REQUEST_COMPLETE_EVENT };


protected:

    /**
     * Protected destructor.
     * Event objects can't be deleted via this interface.
     */
    virtual ~MXIMPProtocolRequestCompleteEvent() {}


public:


    /**
     * Gets request id of the completed request.
     *
     * @return The request id.
     */
    virtual const TXIMPRequestId& RequestId() const = 0;


    /**
     * Gets request completion code.
     *
     * Result code can be:
     *  - one of the system wide errors
     *  - one of the XIMP specific errors defined in ximperrors.hrh and 
     *    in presenceerrors.hrh
     *
     * @return The result code.
     */
    virtual TInt ResultCode() const = 0;


    };


#endif // MXIMPFWPROTOCOLREQUESTCOMPLETEEVENT_H




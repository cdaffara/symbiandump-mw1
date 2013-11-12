/*
* Copyright (c) 2007 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:   Protocol IM Plugin host interface.
*
*/

#ifndef MPROTOCOLIMPLUGINHOST_H
#define MPROTOCOLIMPLUGINHOST_H

#include <ximpbase.h>
#include <protocolimpluginhostifids.hrh>
#include <e32std.h>


//class MImObjectFactory;
//class MImStatus;



// CLASS DECLARATION

/**
 * Interface for IM Protocol Plugin host.
 * This interface is implemented by IM.
 *
 * IM Protocol Plugin host interface defines
 * actions that a IM protocol plug-in can
 * request (callback) from IM.
 *
 * @see MProtocolImPlugin
 * @see CProtocolImPluginBase
 * @ingroup ProtocolImpluginhostapi
 * @since S60 
 */
class MProtocolImPluginHost : public MXIMPBase
    {
public:

    /** Interface ID for the MProtocolImPluginHost. */
    enum { KInterfaceId = IM_IF_ID_PROTOCOL_PLUGIN_HOST };


protected:

    /**
     * Protected destructor. IM Protocol Plugin host
     * instancies can't be destroyed via this interface.
     */
    virtual inline ~MProtocolImPluginHost() {};


public:


    /**
     * Gets reference to object factory interface.
     *
     * Returned object factory interface is used
     * to instantiate IM data objects.
     *
     * @return Object factory interface.
     *         No ownership is returned caller.
     */
    virtual MImObjectFactory& ObjectFactory() = 0;



    /**
     * Notifies IM that IM protocol
     * plug-in has completed (successfully or otherwise)
     * the request handling.
     *
     * /b NOTE: This method isn't yet supported by IM,
     *       since all MProtocolImPlugin methods are currently
     *       synchronous.
     *
     * @param [in] aReqId
     *        Request ID identifying the request which handling
     *        is completed. This must equal to value given in
     *        the processing initiation. If no pending IM
     *        protocol plug-in request is found with this id,
     *        plug-in is paniced.
     *
     * @param [in] aResultCode
     *        Result code from request handling.
     */
    virtual void HandleRequestCompleted( TXIMPRequestId aReqId,
                                         TInt aResultCode ) = 0;


    /**
     * Notifies IM that IM protocol
     * plug-in has completed (successfully or otherwise)
     * the request handling.
     *
     * /b NOTE: This method isn't yet supported by IM,
     *       since all MProtocolImPlugin methods are currently
     *       synchronous.
     *
     * @param [in] aReqId
     *        Request ID identifying the request which handling
     *        is completed. This must equal to value given in
     *        the processing initiation. If no pending IM
     *        protocol plug-in request is found with this id,
     *        plug-in is paniced.
     * @param [in] aResult
     *        Status object describing result from request handling.
     *        Object ownership is transfered always to IM.
     *        NULL value not allowed.
     */
    virtual void HandleRequestCompleted( TXIMPRequestId aReqId,
                                         MImStatus* aResult ) = 0;


    /**
     * Notifies IM that IM protocol
     * plug-in has completed (successfully or otherwise)
     * the request handling.
     *
     * /b NOTE: This method isn't yet supported by IM,
     *       since all MProtocolImPlugin methods are currently
     *       synchronous.
     *
	 * @param [in] aReqId
     *        Request ID identifying the request which handling
     *        is completed. This must equal to value given in
     *        the processing initiation. If no pending IM
     *        protocol plug-in request is found with this id,
     *        plug-in is paniced.
     * @param [in] aResult
     *        Status object describing result from request handling.
     *        Object ownership is transfered always to Im.
     *        NULL value not allowed.
     * @param [in] aParameter
     *        Additional request complete parameters.
     *        NULL if no additional parameters to transfer Im.
     *        Parameter may be single MXIMPBase derived
     *        object or MImRestrictedObjectCollection object
     *        containing multiple objects. Ownership of given
     *        parameter (also the container and contained objects)
     *        is transfered always to Im.
     */
    virtual void HandleRequestCompleted( TXIMPRequestId aReqId,
                                         MImStatus* aResult,
                                         MXIMPBase* aParameter ) = 0;

    };


#endif // MPROTOCOLIMPLUGINHOST_H

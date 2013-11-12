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
* Description:  XIMP Protocol Plugin host interface.
*
*/

#ifndef MXIMPFWPROTOCOLPLUGINHOST_H
#define MXIMPFWPROTOCOLPLUGINHOST_H

#include <ximpbase.h>
#include <ximpprotocolpluginhostifids.hrh>
#include <e32std.h>


class MXIMPObjectFactory;
class MXIMPStatus;



// CLASS DECLARATION

/**
 * Interface for XIMP Protocol Plugin host.
 * This interface is implemented by XIMP Framework.
 *
 * XIMP Protocol Plugin host interface defines
 * actions that a XIMP protocol plug-in can
 * request (callback) from XIMP Framework.
 *
 * @see MXIMPProtocolPlugin
 * @see CXIMPProtocolPluginBase
 * @ingroup ximpprotocolpluginhostapi
 * @since S60 v3.2
 */
class MXIMPProtocolPluginHost : public MXIMPBase
    {
public:

    /** Interface ID for the MXIMPProtocolPluginHost. */
    enum { KInterfaceId = XIMP_IF_ID_PROTOCOL_PLUGIN_HOST };


protected:

    /**
     * Protected destructor. XIMP Protocol Plugin host
     * instancies can't be destroyed via this interface.
     */
    virtual inline ~MXIMPProtocolPluginHost() {};


public:


    /**
     * Gets reference to object factory interface.
     *
     * Returned object factory interface is used
     * to instantiate XIMP data objects.
     *
     * @return Object factory interface.
     *         No ownership is returned caller.
     */
    virtual MXIMPObjectFactory& ObjectFactory() = 0;



    /**
     * Notifies XIMP Framework that XIMP protocol
     * plug-in has completed (successfully or otherwise)
     * the request handling.
     *
     * NOTE: This method isn't yet supported by XIMP Framework,
     *       since all MXIMPProtocolPlugin methods are currently
     *       synchronous.
     *
     *
     * @param [in] aReqId
     *        Request ID identifying the request which handling
     *        is completed. This must equal to value given in
     *        the processing initiation. If no pending XIMP
     *        protocol plug-in request is found with this ID,
     *        plug-in is paniced.
     *
     * @param [in] aResultCode
     *        Result code from request handling.
     */
    virtual void HandleRequestCompleted( TXIMPRequestId aReqId,
                                         TInt aResultCode ) = 0;


    /**
     * Notifies XIMP Framework that XIMP protocol
     * plug-in has completed (successfully or otherwise)
     * the request handling.
     *
     * NOTE: This method isn't yet supported by XIMP Framework,
     *       since all MXIMPProtocolPlugin methods are currently
     *       synchronous.
     *
     * @param [in] aReqId
     *        Request ID identifying the request which handling
     *        is completed. This must equal to value given in
     *        the processing initiation. If no pending XIMP
     *        protocol plug-in request is found with this ID,
     *        plug-in is paniced.
     *
     * @param [in] aResult
     *        Status object describing result from request handling.
     *        Object ownership is transfered always to XIMPFw.
     *        NULL value not allowed.
     */
    virtual void HandleRequestCompleted( TXIMPRequestId aReqId,
                                         MXIMPStatus* aResult ) = 0;


    /**
     * Notifies XIMP Framework that XIMP protocol
     * plug-in has completed (successfully or otherwise)
     * the request handling.
     *
     * NOTE: This method isn't yet supported by XIMP Framework,
     *       since all MXIMPProtocolPlugin methods are currently
     *       synchronous.
     *
     * @param [in] aReqId
     *        Request ID identifying the request which handling
     *        is completed. This must equal to value given in
     *        the processing initiation. If no pending XIMP
     *        protocol plug-in request is found with this ID,
     *        plug-in is paniced.
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
    virtual void HandleRequestCompleted( TXIMPRequestId aReqId,
                                         MXIMPStatus* aResult,
                                         MXIMPBase* aParameter ) = 0;

    };


#endif // MXIMPFWPROTOCOLPLUGINHOST_H

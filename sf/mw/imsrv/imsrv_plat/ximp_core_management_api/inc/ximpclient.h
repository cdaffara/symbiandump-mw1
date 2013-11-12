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
* Description:  Root interface for clients for XIMP Framework usage.
*
*/

#ifndef MXIMPFWCLIENT_H
#define MXIMPFWCLIENT_H

#include <e32std.h>
#include <ximpbase.h>
#include <ximpmanagementifids.hrh>


class MXIMPProtocolInfo;
class MXIMPFeatureInfo;
class MXIMPContext;


/**
 * Root interface for clients for XIMP Framework usage.
 *
 * MXIMPClient is main interface for XIMP Framework
 * clients, to access XIMP Framework services.
 * Interface is implemented by the XIMP Framework.
 * MXIMPClient instance is created with the
 * MXIMPClient::NewClientL() method.
 *
 * MXIMPClient provides methods to query available
 * presence protocols and their capabilities.
 *
 * MXIMPClient is also factory for instantiating
 * presence context objects.
 *
 * @ingroup ximpfwmanagementapi
 * @since S60 v3.2
 */
class MXIMPClient : public MXIMPBase
    {
public:

    /**
     * ECom interface UID for the MXIMPClient.
     *
     * This interface ID is used:
     * 1) In XIMPFw ECom registration file, to declare
     *    implemented interface. It is used also by
     *    the MXIMPClient::NewClientL() instantiation
     *    method, when creating MXIMPClient instance
     *    through the ECom.
     *
     * 2) By clients when resolving MXIMPClient interface
     *    through MXIMPBase::GetInterface().
     */
    enum { KInterfaceId = XIMP_ECOM_IF_UID_CLIENT_INTERFACE };


public:

    /**
     * Factory method to instantiate MXIMPClient.
     *
     * Factory method to instantiate platform default
     * MXIMPClient implementation through the ECom.
     * If the default presence framework isn't supported
     * in the platform, leaves with errorcode signalled
     * from ECom.
     *
     * @return The new presence client object. Object
     *         ownership is returned to caller.
     */
    static inline MXIMPClient* NewClientL();


    /**
     * Public destructor.
     * Object can be deleted through this interface.
     */
    virtual inline ~MXIMPClient();



protected:

    /**
     * Default constructor to zero initialize
     * the iEcomDtorID member.
     */
    inline MXIMPClient();


public:

    /**
     * Retrieves a list of presence protocols.
     *
     * @param [in, out] aProtocols
     *        The list of presence protocols, each protocol is
     *        presented with MXimpFwPresenceProtocolInfo object.
     *        Existing protocol info items are deleted from array
     *        before adding new entries. Ownership of created objects
     *        is returned to caller.
     */
    virtual void GetProtocolsL(
                RXIMPObjOwningPtrArray< MXIMPProtocolInfo >& aProtocols ) = 0;

    /**
     * Retrieves features supported by current
     * XIMP Framework implementation.
     *
     * @return Features supported by the XIMP Framework.
     *         Exact feature IDs are defined in NXIMPFeature
     *         namespace. Object ownership is returned to
     *         caller.
     */
    virtual MXIMPFeatureInfo* GetFrameworkFeaturesLC() = 0;


    /**
     * Instantiates new presence context object.
     *
     * @return New presence context instance.
     *         Object ownership is returned to caller.
     */
    virtual MXIMPContext* NewPresenceContextLC() = 0;



private: // Data

    //OWN: ECom destructor ID
    TUid iEcomDtorID;
    };

#include <ximpclient.inl>

#endif // MXIMPFWCLIENT_H




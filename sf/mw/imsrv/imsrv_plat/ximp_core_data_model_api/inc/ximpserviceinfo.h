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
* Description:  Interface for XIMP service info object.
*
*/

#ifndef MXIMPFWSERVICEINFO_H
#define MXIMPFWSERVICEINFO_H

#include <e32std.h>
#include <ximpbase.h>
#include <ximpdatamodelifids.hrh>

/**
 * Interface for XIMP service info object.
 *
 * Holds data that identifies the XIMP service.
 *
 * @ingroup ximpdatamodelapi
 * @since S60 v3.2
 */
class MXIMPServiceInfo : public MXIMPBase
    {
public:

    /** Interface ID for the MXIMPServiceInfo. */
    enum { KInterfaceId = XIMP_IF_ID_SERVICE_INFO };


public:

    /**
     * Public destructor.
     * Objects can be deleted through this interface.
     */
    virtual ~MXIMPServiceInfo() {}


public:

    /**
     * Gets service address.
     *
     * @return The service address.
     */
    virtual const TDesC16& ServiceAddress() const = 0;


    /**
     * Gets service user ID.
     *
     * @return The service user ID.
     */
    virtual const TDesC16& UserId() const = 0;


    /**
     * Gets service user password.
     *
     * @return The service user password.
     */
    virtual const TDesC16& Password() const = 0;


    /**
     * Gets Internet accesspoint ID.
     *
     * @return The Internet accesspoint ID.
     */
    virtual TInt32 IapId() const = 0;


    /**
     * Gets protocol implementation UID.
     *
     * @return The protocol implementation UID.
     */
    virtual TUid ProtocolImplUid() const = 0;

    };


#endif // MXIMPFWSERVICEINFO_H



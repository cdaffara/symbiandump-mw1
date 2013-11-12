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
* Description:  Interface for presence context client info object.
*
*/

#ifndef MXIMPFWCONTEXTCLIENTINFO_H
#define MXIMPFWCONTEXTCLIENTINFO_H

#include <e32std.h>
#include <ximpbase.h>
#include <ximpdatamodelifids.hrh>


class MXIMPIdentity;


/**
 * Interface for XIMP context client info object.
 *
 * XIMP context client info object presents information
 * about single XIMP context client.
 *
 * @ingroup ximpdatamodelapi
 * @since S60 v3.2
 */
class MXIMPContextClientInfo : public MXIMPBase
    {
public:

    /** Interface ID for the MXIMPContextClientInfo. */
    enum { KInterfaceId = XIMP_IF_ID_CONTEXT_CLIENT_INFO };


public:

    /**
     * Public destructor.
     * Objects can be deleted through this interface.
     */
    virtual ~MXIMPContextClientInfo() {}


public:


    /**
     * Returns XIMP context client identity value.
     *
     * @return The XIMP context client
     *         identity value.
     */
    virtual const MXIMPIdentity& ClientId() const = 0;



    /**
     * Sets XIMP context client identity value.
     *
     * @param [in] aIdentity
     *        New XIMP context client identity value.
     *        Ownership of the argument is transfered to
     *        callee, if method succeeds without a leave.
     *
     * @leave KErrNoMemory if failed to allocate memory.
     * @leave KErrArgument if NULL argument were provided.
     */
    virtual void SetClientIdL(
                MXIMPIdentity* aIdentity ) = 0;


    };



#endif // MXIMPFWCONTEXTCLIENTINFO_H



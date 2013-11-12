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
* Description:  Interface for presence grant request info object.
*
*/

#ifndef MXIMPFWPRESENCEGRANTREQUESTINFO_H
#define MXIMPFWPRESENCEGRANTREQUESTINFO_H

#include <e32std.h>
#include <ximpbase.h>
#include <presencedatamodelifids.hrh>


class MXIMPIdentity;


/**
 * Interface for presence grant request info object.
 *
 * Presence grant request info object presents single
 * presence grant request entry in the presence
 * grant request list. Presence grant request info
 * object holds the identity and display name information
 * for the presence grant request.
 *
 * @ingroup presencedatamodelapi
 * @since S60 v3.2
 */
class MPresenceGrantRequestInfo : public MXIMPBase
    {
public:

    /** Interface ID for the MPresenceGrantRequestInfo. */
    enum { KInterfaceId = PRES_IF_ID_PRESENCE_GRANT_REQUEST_INFO };


public:

    /**
     * Public destructor.
     * Objects can be deleted through this interface.
     */
    virtual ~MPresenceGrantRequestInfo() {}



public:


    /**
     * Returns presence requestor identity value.
     *
     * @return The presence requestor identity value.
     */
    virtual const MXIMPIdentity& RequestorId() const = 0;



    /**
     * Returns presence requestor displayname value.
     *
     * @return The presence requestor displayname value.
     */
    virtual const TDesC16& RequestorDisplayName() const = 0;



    /**
     * Sets presence requestor identity value.
     *
     * @param [in] aIdentity
     *        New presence requestor identity value.
     *        Ownership of the argument is transfered to
     *        callee, if method succeeds without a leave.
     *
     * @leave KErrNoMemory if failed to allocate memory.
     * @leave KErrArgument if NULL argument were provided.
     */
    virtual void SetRequestorIdL(
                MXIMPIdentity* aIdentity ) = 0;



    /**
     * Sets presence requestor display name value.
     *
     * @param [in] aDisplayName
     *        New display name value.
     *
     * @leave KErrNoMemory if failed to allocate memory.
     */
    virtual void SetRequestorDisplayNameL(
                const TDesC16& aDisplayName ) = 0;


    };


#endif // MXIMPFWPRESENCEGRANTREQUESTINFO_H




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
* Description:  Interface for presence block info object.
*
*/

#ifndef MXIMPFWPRESENCEBLOCKINFO_H
#define MXIMPFWPRESENCEBLOCKINFO_H

#include <e32std.h>
#include <ximpbase.h>
#include <presencedatamodelifids.hrh>


class MXIMPIdentity;


/**
 * Interface for presence block info object.
 *
 * Presence block info object presents single
 * presence block entry in the presence block list.
 * Presence block info object holds the identity
 * and display name information for the presence
 * block entry.
 *
 * @ingroup presencedatamodelapi
 * @since S60 v3.2
 */
class MPresenceBlockInfo : public MXIMPBase
    {
public:

    /** Interface ID for the MPresenceBlockInfo. */
    enum { KInterfaceId = PRES_IF_ID_PRESENCE_BLOCK_INFO };


public:


    /**
     * Public destructor.
     * Objects can be deleted through this interface.
     */
    virtual ~MPresenceBlockInfo() {}


public:

    /**
     * Returns identity of the blocked entity.
     *
     * @return The identity of the blocked entity.
     */
    virtual const MXIMPIdentity& BlockedEntityId() const = 0;



    /**
     * Returns displayname of the blocked entity.
     *
     * @return The displayname of the blocked entity.
     */
    virtual const TDesC16& BlockedEntityDisplayName() const = 0;



    /**
     * Sets identity of the blocked entity.
     *
     * @param [in] aIdentity
     *        Identity of the blocked entity.
     *        Ownership of the argument is transfered to
     *        callee, if method succeeds without a leave.
     *
     * @leave KErrNoMemory if failed to allocate memory.
     * @leave KErrArgument if NULL argument were provided.
     */
    virtual void SetBlockedEntityIdL(
                MXIMPIdentity* aIdentity ) = 0;


    /**
     * Sets displayname of the blocked entity.
     *
     * @param [in] aDisplayName
     *        New display name value.
     *
     * @leave KErrNoMemory if failed to allocate memory.
     */
    virtual void SetBlockedEntityDisplayNameL(
                const TDesC16& aDisplayName ) = 0;

    };


#endif // MXIMPFWPRESENCEBLOCKINFO_H




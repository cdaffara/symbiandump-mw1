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
* Description:  Interface for presence watcher info object.
*
*/

#ifndef MXIMPFWPRESENCEWATCHERINFO_H
#define MXIMPFWPRESENCEWATCHERINFO_H

#include <e32std.h>
#include <ximpbase.h>
#include <presencedatamodelifids.hrh>


class MXIMPIdentity;


/**
 * Interface for presence watcher info object.
 *
 * Presence watcher info object presents single presence watcher
 * entry in the presence watcher list. Presence watcher info
 * object holds the identity, display name and watcher type
 * information for the presence watcher.
 *
 * @ingroup presencedatamodelapi
 * @since S60 v3.2
 */
class MPresenceWatcherInfo : public MXIMPBase
    {
public:

    /** Interface ID for the MPresenceWatcherInfo. */
    enum { KInterfaceId = PRES_IF_ID_PRESENCE_WATCHER_INFO };


    /** Presence watcher types. */
    enum TWatcherType
        {
        /** Presence watcher type unknown. */
        EUnknown,

        /** Presence watcher is currently subscribing user presence. */
        EPresenceSubscriber
        };


public:

    /**
     * Public destructor.
     * Objects can be deleted through this interface.
     */
    virtual ~MPresenceWatcherInfo() {}



public:


    /**
     * Returns presence watcher identity value.
     *
     * @return The presence watcher identity value.
     */
    virtual const MXIMPIdentity& WatcherId() const = 0;



    /**
     * Returns presence watcher displayname value.
     *
     * @return The presence watcher displayname value.
     */
    virtual const TDesC16& WatcherDisplayName() const = 0;



    /**
     * Returns presence watcher type value.
     *
     * @return The presence watcher type value.
     */
    virtual TWatcherType WatcherType() const = 0;



    /**
     * Sets watcher identity value.
     *
     * @param [in] aIdentity
     *        New watcher identity value.
     *        Ownership of the argument is transfered to
     *        callee, if method succeeds without a leave.
     *
     * @leave KErrNoMemory if failed to allocate memory.
     * @leave KErrArgument if NULL argument were provided.
     */
    virtual void SetWatcherIdL(
                MXIMPIdentity* aIdentity ) = 0;


    /**
     * Sets presence watcher displayname value.
     *
     * @param [in] aDisplayName
     *        New display name value.
     *
     * @leave KErrNoMemory if failed to allocate memory.
     */
    virtual void SetWatcherDisplayNameL(
                const TDesC16& aDisplayName ) = 0;


    /**
     * Sets presence watcher type value.
     *
     * @param [in] aType
     *        New presence watcher type value.
     *
     * @leave KErrNoMemory if failed to allocate memory.
     */
    virtual void SetWatcherTypeL(
                TWatcherType aType ) = 0;

    };


#endif // MXIMPFWPRESENCEWATCHERINFO_H




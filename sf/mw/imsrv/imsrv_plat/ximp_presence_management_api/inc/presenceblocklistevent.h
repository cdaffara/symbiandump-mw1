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
* Description:  Event interface for presence block list content.
*
*/

#ifndef MXIMPFWPRESENCEBLOCKLISTEVENT_H
#define MXIMPFWPRESENCEBLOCKLISTEVENT_H

#include <e32std.h>
#include <ximpbase.h>
#include <presencemanagementifids.hrh>


class MXIMPIdentity;
class MPresenceBlockInfo;
class MXIMPDataSubscriptionState;



/**
 * Event interface for presence block list content.
 *
 * This interface is implemented by the XIMP Framework and
 * event object is signalled to registered client observers
 * through MXIMPContextObserver observer interface.
 * This event notifies clients about changes in
 * presence block list content.
 *
 * Clients subscribe for this event with
 * MPresenceAuthorization::SubscribePresenceBlockListL.
 *
 * XIMP Framework signals this event when:
 * - New subscribed data is received from protocol.
 * - Data item data subscription status is updated by
 *   protocol adaptation.
 * - When client unsubscribes the data item.
 * - XIMP Framework generates initial data notification
 *   to client automatically after clients subscription,
 *   when the initial data comes available from protocol.
 *
 * @ingroup ximppresmanagementapievents
 * @since S60 v3.2
 */
class MPresenceBlockListEvent : public MXIMPBase
    {
public:
    /** Interface ID for the MPresenceBlockListEvent. */
    enum { KInterfaceId = PRES_IF_ID_PRESENCE_BLOCK_LIST_EVENT };


protected:

    /**
     * Protected destructor.
     * Object instancies can't be deleted via this interface.
     */
    virtual ~MPresenceBlockListEvent() {}



public: // Access to "delta information"


    /**
     * Gets count of new presence block entries.
     *
     * @return The count of new presence block entries.
     */
    virtual TInt NewBlocksCount() const = 0;


    /**
     * Gets presence block entry by index.
     *
     * @param [in] aIndex
     *        The index of the presence block entry to get.
     *
     * @return The presence block entry from given index.
     */
    virtual const MPresenceBlockInfo& NewBlock(
                TInt aIndex ) const = 0;


    /**
     * Gets count of updated presence block entries.
     *
     * @return The count of updated presence block entries.
     */
    virtual TInt UpdatedBlocksCount() const = 0;


    /**
     * Gets presence block entry by index.
     *
     * @param [in] aIndex
     *        The index of the presence block entry to get.
     *
     * @return The presence block entry from given index.
     */
    virtual const MPresenceBlockInfo& UpdatedBlock(
                TInt aIndex ) const = 0;


    /**
     * Gets count of disappeared presence block entries.
     *
     * @return The count of disappeared presence block entries.
     */
    virtual TInt DisappearedBlocksCount() const = 0;


    /**
     * Gets presence block entry by index.
     *
     * @param [in] aIndex
     *        The index of the presence block entry to get.
     *
     * @return The presence block entry from given index.
     */
    virtual const MPresenceBlockInfo& DisappearedBlock(
                TInt aIndex ) const = 0;



public: // Access to "current information"


    /**
     * Gets count of current presence block entries.
     *
     * Note that, valid "current information" is signalled only when
     * the client refreshes the event data from XIMP Framework cache,
     * by re-issuing the data subscription. In other cases, just plain
     * "delta information" is signalled to client.
     *
     * @return The count of current presence block entries.
     */
    virtual TInt CurrentBlocksCount() const = 0;


    /**
     * Gets presence block entry by index.
     *
     * Note that, valid "current information" is signalled only when
     * the client refreshes the event data from XIMP Framework cache,
     * by re-issuing the data subscription. In other cases, just plain
     * "delta information" is signalled to client.
     *
     * @param [in] aIndex
     *        The index of the presence block entry to get.
     *
     * @return The presence block entry from given index.
     */
    virtual const MPresenceBlockInfo& CurrentBlock(
                TInt aIndex ) const = 0;



public: // Access to subscription state


    /**
     * Returns subscription state object holding
     * information data items data subscription state.
     *
     * @return The data items data subscription state.
     *         Object ownership is not returned.
     */
    virtual const MXIMPDataSubscriptionState& DataSubscriptionState() const = 0;

    };



#endif // MXIMPFWPRESENCEBLOCKLISTEVENT_H




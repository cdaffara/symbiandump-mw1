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
* Description:  Event interface for invitation content.
*
*/

#ifndef MIMINVITATIONEVENT_H
#define MIMINVITATIONEVENT_H

#include <e32std.h>
#include <ximpbase.h>
#include <immanagementifids.hrh>


class MXIMPIdentity;
class MImInvitationInfo;
class MImDataSubscriptionState;


/**
 * Event interface for invitation content.
 *
 * This interface is implemented by the IM and
 * event object is signalled to registered client observers
 * through MImContextObserver observer interface.
 * This event notifies clients about changes in
 * invitation content.
 *
 * IM signals this event when:
 * - New invitation for group is received from protocol.
 * - New invitation for conversation is received from protocol.
 * - Invitation response for group is received
 * - Invitation response for conversation is received
 * @ininvitation immanagementapievents
 * 
 */
class MImInvitationEvent : public MXIMPBase
    {
public:
    /** Interface ID for the MImInvitationEvent. */
    enum { KInterfaceId = IM_IF_ID_INVITATION_EVENT };



protected:

    /**
     * Protected destructor.
     * Object instancies can't be deleted via this interface.
     */
    virtual ~MImInvitationEvent() {}


public: // Access to "current information"


    /**
     * Gets count of invitations.
     *
     * @return The count of invitations.
     */
    virtual TInt InvitationCount() const = 0;


    /**
     * Gets invitation response by index.
     *
     * @param [in] aIndex
     *        The index of the invitation entry to get.
     *
     * @return The invitation response from given index.
     */
    virtual const MImInvitationInfo& InvitationResponse(
                TInt aIndex ) const = 0;





public: // Access to subscription state


    /**
     * Returns subscription state object holding
     * information about data items data subscription state.
     *
     * @return The data items data subscription state.
     *         Object ownership is not returned.
     */
    virtual const MImDataSubscriptionState& DataSubscriptionState() const = 0;


    };


#endif // MIMINVITATIONEVENT_H




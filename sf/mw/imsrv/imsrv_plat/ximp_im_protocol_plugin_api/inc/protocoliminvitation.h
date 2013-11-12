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
* Description:  Interface for IM protocol Invitation object.
*
*/

#ifndef MPROTOCOLIMINVITATION_H
#define MPROTOCOLIMINVITATION_H

#include <e32std.h>
#include <badesca.h>
#include <ximpbase.h>
#include "improtocolifids.hrh"


class MXIMPIdentity;
class MImInvitationInfo;
/**
 * Interface for IM protocol invitation object.
 * Interface must be implemented by a XIMP Framework protocol
 * plug-ins.
 *
 * This interface defines actions that XIMP Framework
 * requests from a IM protocol connection to
 * execute invitation related tasks.
 *
 * Protocol implementation must route requests received through
 * this interface, to same remote IM service destination,
 * as where the parent MXIMPProtocolConnection is connected.
 * (Here the parent MXIMPProtocolConnection means the protocol
 * connection instance from where this interface instance
 * was retrieved.)
 *
 * @see MProtocolImConnection
 * @ingroup ProtocolImpluginapi
 * 
 */
class MProtocolImInvitation : public MXIMPBase
    {
public:

    /** Interface ID for the MProtocolImInvitation. */
    enum { KInterfaceId = IM_IF_ID_PROTOCOL_INVITATION };


protected:

    /**
     * Protected destructor. MProtocolImInvitation
     * instancies can't be destroyed via this interface.
     */
    virtual inline ~MProtocolImInvitation() {}

public: 

     /** Requests the protocol to invite group of users to join in the mentioned group
     *
	 *  @param [in] aUserID           = IDs of the users to invite
     *  @param [in] InviteGroup      = ID of the group to which the users
     *                             			are invited to join
	 *  @param [in] aInviteInfo        = Identification and content of the invitation.
     *		MImInvitationInfo contains inviteId, contenttype - text/content, invitereason,validityperiod....... 
     *
	 */
                        
    virtual void DoImGroupInviteL( const MDesCArray* aUserID,
                                    const MXIMPIdentity& InviteGroup,
                                   const MImInvitationInfo& aInviteInfo ) = 0;
    /**  Request the protocol to invite user to IM conversation
     *
	 *  @param [in] aUserID           = IDs of the users to invite
     *  @param [in] aInviteInfo       = Identification and content of the invitation.
     *		MImInvitationInfo contains inviteid, contenttype - text/content, invitereason,validityperiod....... 
	 *
	 */
    virtual void DoImInviteL( const TDesC& aUserID,                              
                                 const MImInvitationInfo& aInviteInfo ) = 0;

     /** Request the protocol to Cancel an im group invite 
     *
	 *  @param [in] aUserID           = IDs of the users to invite
     *  @param [in] InviteGroup      = ID of the group to which the users
     *                             			are invited to join
	 *  @param [in] aInviteInfo        = Identification and content of the invitation.
     *		MImInvitationInfo contains inviteid, contenttype - text/content, invitereason,validityperiod....... 
     *
	 */

    virtual void DoCancelImgroupInviteL( const MDesCArray* aUserID,
                                    const MXIMPIdentity& InviteGroup,
                                   const MImInvitationInfo& aInviteInfo )= 0;
	/**  Request the protocol to cancel invitation for user to IM conversation
     
	 *   @param [in] aUserID           = IDs of the users to invite
     *   @param [in] aInviteInfo       = Identification and content of the invitation.
     *		MImInvitationInfo contains inviteid, contenttype - text/content, invitereason,validityperiod....... 
	 
	 *   @return The request id identifying the issued request.
     *
	 * /b Note: Possible error conditions
     *	/n		1. Invalid invite id
     *	/n		2. Invalid userid
     *  If the request is success it will send a group invitation to the mentioned users
     *
	 */
    virtual TXIMPRequestId DoCancelImInviteL( const TDesC& aUserID,                              
                                 const MImInvitationInfo& aInviteInfo ) = 0;
     /** Request the protocol to Send response to invite
     *  @param [in] aRecipientUserId  = Userid of the invitee
     *  @param [in] aInviteInfo       = Identification and content of the invitation.
     *		MImInvitationInfo contains inviteId, contenttype - text/content, inviteresponse,acceptance....... 
	 *
	 */
    virtual void DoInviteResponseL( const TDesC& aRecipientUserId,
										   const MImInvitationInfo& aInviteInfo ) = 0;
    };



#endif // MPROTOCOLIMINVITATION_H

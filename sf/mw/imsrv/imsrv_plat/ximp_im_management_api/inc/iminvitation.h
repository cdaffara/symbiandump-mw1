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
* Description:  IM Invitation handling interface for clients use.
*
*/

#ifndef MIMININVITATION_H
#define MIMININVITATION_H


#include <ximpbase.h>
#include <ximpidentity.h>
#include <bamdesca.h>
#include "immanagementifids.hrh"

class MImInvitationInfo;
class MXIMPIdentity;

/**
 * IM Invitation handling interface for clients use.
 * Interface is implemented by the IM.
 *
 * MImInvitation provides services for
 * IM clients to manage Invitationitaion
 * It provides also services to ???
 *
 * Client can retrieve MImInvitation interface from
 * MImContext interface.
 *
 * @ingroup Immanagementapi
 * 
 */
class MImInvitation : public MXIMPBase
    {
public:

    /** Interface ID for the MImInvitation. */
    enum { KInterfaceId = IM_IF_ID_INVITATION};


protected:

    /**
     * Protected destructor.
     * Object instancies can't be deleted via this interface.
     */
    virtual ~MImInvitation() {}


public: //Invitation Requests

     /** Inviting group of users to join in the mentioned group
     *
	 *  @param [in] aUserID           = IDs of the users to invite
     *  @param [in] InviteGroup      = ID of the group to which the users
     *                             			are invited to join
	 * @param [in] aInviteInfo        = Identification and content of the invitation.
     *		MImInvitationInfo contains InviteId, contenttype - text/content, invitereason,validityperiod....... 
     *
	 * @return The request id identifying the issued request.
     *
	 * /b Note: Possible error conditions
     *	/n		1. Invalid invite ID
     *	/n		2. Invalid userId/screenname
     *	/n		3. Group does't exists
     *  If the request is success it will send a group invitation to the mentioned users
	 *
     */
    virtual TXIMPRequestId ImGroupInviteL( const MDesCArray* aUserID,
                                    const MXIMPIdentity& InviteGroup,
                                   const MImInvitationInfo& aInviteInfo )= 0;
                                   
    /**  Invite user to IM conversation
     *
	 *  @param [in] aUserID           = ID of the user to invite
     *  @param [in] aInviteInfo       = Identification and content of the invitation.
     *		MImInvitationInfo contains InviteId, contenttype - text/content, invitereason,validityperiod....... 
	 *
	 *  @return The request ID identifying the issued request.
     *  
	 * /b Note: Possible error conditions
     *	/n		1. Invalid Invite ID
     *	/n		2. Invalid userId
     *  If the request is success it will send a group invitation to the mentioned users
     *
	 */
    virtual TXIMPRequestId ImInviteL( const TDesC& aUserID,                              
                                 const MImInvitationInfo& aInviteInfo ) = 0;
                                 
     /** Cancel an im group invite 
     *
	 *  @param [in] aUserID           = IDs of the users to cancel invititation
     *  @param [in] InviteGroup      = ID of the group to which the users
     *                             			are invited to join
	 * @param [in] aInviteInfo        = Identification and content of the invitation.
     *		MImInvitationInfo contains InviteId, contenttype - text/content, invitereason,validityperiod....... 
     * @return The request ID identifying the issued request.
     *
	 * /b Note: Possible error conditions
     *	/n		1. Invalid Invite ID
     *	/n		2. Invalid userId/Screenname
     *	/n		3. Group does't exists
     *  If the request is success it will send a group invitation to the mentioned users
     *
	 */
  virtual TXIMPRequestId CancelImgroupInviteL( const MDesCArray* aUserID,
                                    const MXIMPIdentity& InviteGroup,
                                   const MImInvitationInfo& aInviteInfo )= 0;
                                
     /**  Cancel an IM conversation Invite
     *
	 *  @param [in] aUserID           = ID of the user to cancel invitataion
     *  @param [in] aInviteInfo       = Identification and content of the invitation.
     *		MImInvitationInfo contains InviteId, contenttype - text/content, invitereason,validityperiod....... 
     *  @return The request ID identifying the issued request.
     *
	 * /b Note: Possible error conditions
     *	/n		1. Invalid Invite ID
     *	/n		2. Invalid userId
     *  If the request is success it will send a group invitation to the mentioned users
     *
	 */
    virtual TXIMPRequestId CancelImInviteL( const TDesC& aUserID,                              
                                 const MImInvitationInfo& aInviteInfo ) = 0;
                                 
     /** Send response to invite
	 *
     *  @param [in] aRecipientUserId  = UserID of the invitee
     *  @param [in] aInviteInfo       = Identification and content of the invitation.
     *		MImInvitationInfo contains InviteId, contenttype - text/content, inviteresponse,acceptance....... 
	 *  @return The request ID identifying the issued request.
     * 
	 * /b Note: Possible error conditions
     *	/n		1. Invalid invite id
     *	/n		2. Invalid userId/screenname/groupName
     *  If the request is success it will send a group invitation to the mentioned users
     *
	 */
		virtual TXIMPRequestId InviteResponseL( const TDesC& aRecipientUserId,
										   const MImInvitationInfo& aInviteInfo ) = 0;
    };
#endif // MIMININVITATION_H




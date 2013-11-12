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
* Description:  Interface for managing cached Invite information.
*
*/

#ifndef MPROTOCOLIMINVITATIONDATAHOST_H
#define MPROTOCOLIMINVITATIONDATAHOST_H

#include <e32std.h>
#include <improtocolpluginhostifids.hrh>
#include <ximpbase.h>

class MXIMPIdentity;
/**
 * Interface for managing cached Invitation information.
 *
 * Interface is used by protocol adaptations to manage
 * invitation related information in
 * IM internal data cache.
 *
 * This interface is implemented by IM.
 * Protocol adaptation can obtain reference to this
 * interface from MXIMPProtocolConnectionHost interface.
 *
 * For each separated protocol connection, protocol
 * adaptation must use the specific
 * MImProtocolInvitationDataHost instance,
 * what is received from the connection specific
 * MXIMPProtocolConnectionHost instance.
 *
 * @ingroup Improtocolpluginhostapi
 * @since S60
 */
class MImProtocolInvitationDataHost : public MXIMPBase
    {
public:

    /** Interface ID for the MImProtocolInvitationDataHost. */
    enum { KInterfaceId = IM_IF_ID_PROTOCOL_INVITATION_DATAHOST };


protected:


    /**
     * Protected destructor. Object instancies
     * can't be deleted via this interface.
     */
    virtual ~MImProtocolInvitationDataHost() {}



public: // Handling invitation requests

     
    /**
     * Notifies IM from
     * im group invitation .
     *
     * Protocol adaptation implementation can call this
     * method to notify IM from
     * im group invitation .
     * IM calculates the changes
     * notifies the subscribing clients from data changes.
     *
     * IM takes ownership of the argument(s),
     * if method succeeds without a leave.
     *
     *  @param [in] aUserID           = IDs of the users to invite
     *  @param [in] InviteGroup      = ID of the group to which the users
     *                             			are invited to join
	 * @param [in] aInviteInfo        = Identification and content of the invitation.
     *		MImInvitationInfo contains InviteId, contenttype - text/content, invitereason,validityperiod....... 
     *
	 * @return The request ID identifying the issued request.
     *         Data cache update request is processed
     *         asynchronously and operation completion result is
     *         signalled to adaptation direction through
     *         MXIMPProtocolConnectionHostObserver interface,
	 *         with MXIMPProtocolRequestCompleteEvent event.
     *
     * @leave KErrNoMemory if failed to allocate memory.
     * @leave KErrArgument if NULL argument(s) were provided.
     */
	virtual TXIMPRequestId HandleImGroupInviteL(
                 const MDesCArray* aUserID,
                                    const MXIMPIdentity& InviteGroup,
                                   const MImInvitationInfo& aInviteInfo ) = 0;
	 /**
     * Notifies IM from
     * im Invitation .
     *
     * Protocol adaptation implementation can call this
     * method to notify IM from
     * IM  invitation .
     * IM calculates the changes
     * notifies the subscribing clients from data changes.
     *
     * IM takes ownership of the argument(s),
     * if method succeeds without a leave.
     *
	 *  @param [in] aUserID           = ID of the user to invite
     *  @param [in] aInviteInfo       = Identification and content of the invitation.
     *		MImInvitationInfo contains InviteId, contenttype - text/content, invitereason,validityperiod....... 
	 *
	 * @return The request id identifying the issued request.
     *         Data cache update request is processed
     *         asynchronously and operation completion result is
     *         signalled to adaptation direction through
     *         MXIMPProtocolConnectionHostObserver interface,
	 *         with MXIMPProtocolRequestCompleteEvent event.
     *
     * @leave KErrNoMemory if failed to allocate memory.
     * @leave KErrArgument if NULL argument(s) were provided.
     */
	virtual TXIMPRequestId HandleImInviteL(
                 const TDesC& aUserID,                              
                                 const MImInvitationInfo& aInviteInfo ) = 0;
	


    /**
     * Notifies IM from
     * invitation response.
     *
     * Protocol adaptation implementation can call this
     * method to notify IM from
     * invitation response.
     * IM calculates the changes
     * notifies the subscribing clients from data changes.
     *
     * IM takes ownership of the argument(s),
     * if method succeeds without a leave.
     *
     *  @param [in] aRecipientUserId  = UserID of the invitee
     *  @param [in] aInviteInfo       = Identification and content of the invitation.
     *		MImInvitationInfo contains InviteId, contenttype - text/content, inviteresponse,acceptance....... 
	 *
	 * @return The request id identifying the issued request.
     *         Data cache update request is processed
     *         asynchronously and operation completion result is
     *         signalled to adaptation direction through
     *         MXIMPProtocolConnectionHostObserver interface,
	 *         with MXIMPProtocolRequestCompleteEvent event.
     *
     * @leave KErrNoMemory if failed to allocate memory.
     * @leave KErrArgument if NULL argument(s) were provided.
     */
	virtual TXIMPRequestId HandleInviteResponseL(
                const TDesC& aRecipientUserId,
										   const MImInvitationInfo& aInviteInfo ) = 0;
	
	
	/**
     * Notifies IM from
     * Cancelled im group Invitation.
     *
     * Protocol adaptation implementation can call this
     * method to notify IM from
     * cancelled im group invitation request.
     * IM calculates the changes
     * notifies the subscribing clients from data changes.
     *
     * IM takes ownership of the argument(s),
     * if method succeeds without a leave.
     *
	 *  @param [in] aUserID           = IDs of the users to cancel invititation
     *  @param [in] InviteGroup      = ID of the group to which the users
     *                             			are invited to join
	 * @param [in] aInviteInfo        = Identification and content of the invitation.
     *		MImInvitationInfo contains InviteId, contenttype - text/content, invitereason,validityperiod....... 
     * 
	 * @return The request id identifying the issued request.
     *         Data cache update request is processed
     *         asynchronously and operation completion result is
     *         signalled to adaptation direction through
     *         MXIMPProtocolConnectionHostObserver interface,
	 *         with MXIMPProtocolRequestCompleteEvent event.
     *
     * @leave KErrNoMemory if failed to allocate memory.
     * @leave KErrArgument if NULL argument(s) were provided.
     */
  
     virtual TXIMPRequestId HandleCancelImgroupInviteL( const MDesCArray* aUserID,
                                    const MXIMPIdentity& InviteGroup,
                                   const MImInvitationInfo& aInviteInfo )= 0;

/**
     * Notifies IM from
     * Cancelled Invitation.
     *
     * Protocol adaptation implementation can call this
     * method to notify IM from
     * cancelled invitation request.
     * IM calculates the changes
     * notifies the subscribing clients from data changes.
     *
     * IM takes ownership of the argument(s),
     * if method succeeds without a leave.
     *
	 *  @param [in] aUserID           = ID of the user to cancel invitataion
     *  @param [in] aInviteInfo       = Identification and content of the invitation.
     *		MImInvitationInfo contains InviteId, contenttype - text/content, invitereason,validityperiod....... 
     *
	 * @return The request ID identifying the issued request.
     *         Data cache update request is processed
     *         asynchronously and operation completion result is
     *         signalled to adaptation direction through
     *         MXIMPProtocolConnectionHostObserver interface,
	 *         with MXIMPProtocolRequestCompleteEvent event.
     *
     * @leave KErrNoMemory if failed to allocate memory.
     * @leave KErrArgument if NULL argument(s) were provided.
     */
  
     virtual TXIMPRequestId HandleCancelImInviteL( const TDesC& aUserID,                              
                                 const MImInvitationInfo& aInviteInfo ) = 0;
                                         


    };


#endif // MPROTOCOLIMINVITATIONDATAHOST_H

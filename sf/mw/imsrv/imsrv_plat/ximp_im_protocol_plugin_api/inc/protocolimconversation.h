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
* Description:  Interface for IM protocol messaging object.
*
*/

#ifndef MPROTOCOLIMCONVERSATION_H
#define MPROTOCOLIMCONVERSATION_H


#include <e32std.h>
#include <badesca.h>
#include <ximpbase.h>
#include <improtocolpluginifids.hrh>


class MXIMPIdentity;
class MImConversationInfo;
/**
 * Interface for IM protocol message handling object.
 * Interface must be implemented by a IM protocol
 * plug-ins.
 *
 * This interface defines actions that IM 
 * requests from a IM protocol connection to
 * execute conversation related tasks.
 *
 * Protocol implementation must route requests received through
 * this interface, to same remote IM service destination,
 * as where the parent MXIMPProtocolConnection is connected.
 * (Here the parent MXIMPProtocolConnection means the protocol
 * connection instance from where this interface instance
 * was retrieved.)
 *
 * @see MXIMPProtocolConnection
 * @ingroup Improtocolpluginapi
 *
 */
class MProtocolImConversation : public MXIMPBase
    {
public:

    /** Interface ID for the MProtocolImConversation. */
    enum { KInterfaceId = IM_IF_ID_PROTOCOL_CONVERSATION };


protected:

    /**
     * Protected destructor. MProtocolImConversation
     * instancies can't be destroyed via this interface.
     */
    virtual inline ~MProtocolImConversation() {};

public: 
    /**
     * Request the protocol to send a  message.
     *
	 * @param [in] aImMessage 	- Identification and content of the message.
     *		MImConversationInfo contains messageid, contenttype - text/content, content,.......  
     * @param [in] aRecipients 	- Recipients userid.
     * @param [in] aReqId
     *        Request id identifying the issued request.
	 *
	 * /b Note: Possible error conditions
     *	/n	 1. Invalid User-ID/Recipient user does not exist [KImApiErrInvalidUserId].
     *	/n	 2. Recipient user blocked the sender [KImApiErrSenderBlocked].
     *	/n	 3. Unsupported message context [KImErrUnsupportedContext].
     *	/n	 4. Message queue full [KImErrTooManyIM].
     */
    virtual void DoSendMessageL( const MImConversationInfo& aImMessage,TXIMPRequestId aReqId )= 0;

    /**
     * Request the protocol to forward a message with binary content.
     * Forward message to user(s)
     *
	 * @param [in] aImMessage 	- Identification and content of the message.
     *		MImConversationInfo contains messageid, contenttype - text/content, content,.......  
     * @param [in] aRecipients 	- Recipients userid.
	 * @param [in] aReqId
     *        Request id identifying the issued request.
	 *
	 * /b Note: Possible error conditions
     *	/n	 1. Invalid user-id/recipient user does not exist [KImApiErrInvalidUserId].
     *	/n	 2. Recipient user blocked the sender [KImApiErrSenderBlocked].
     *	/n	 3. Unsupported message context [KImErrUnsupportedContext].
     *	/n	 4. Message queue full [KImErrTooManyIM].
     */
   virtual void DoForwardMessageL( const MImConversationInfo& aImMessage,TXIMPRequestId aReqId) = 0;


    /**
     * Request the protocol to send message to group.
     *
	 * @param [in] aImMessage 	- Identification and content of the message.
     *		MImConversationInfo contains messageid, contenttype - text/content, content,.......  
     * @param [in] aUserIds 	- Recipients userid.
	 * @param [in] aReqId
     *        Request id identifying the issued request.
	 *
	 * /b Note: Possible error conditions
     *	/n	 1. Invalid user-id/recipient user does not exist [KImApiErrInvalidUserId].
     *	/n	 2. Recipient user blocked the sender [KImApiErrSenderBlocked].
     *	/n	 3. Unsupported message context [KImErrUnsupportedContext].
     *	/n	 4. Message queue full [KImErrTooManyIM].
     */
	virtual void DoSendMessageToGroupL( const MImConversationInfo& aImMessage,
			                                        const TDesC* aGroupId,TXIMPRequestId aReqId ) = 0;

/**
     * Request the protocol to block user(s).
     *
     * @param [in] aUserIds - User id's need to be blocked
     * @param [in] aReqId
     *        Request ID identifying the issued request.
     *    
	 * /b Note: Possible error conditions
     *	/n	 1. UserId doesn't exist [KImErrServiceGeneralError].
     *	/n	 2. The maximum number of users reached for the user in the list [KImErrTooManyUsers].
     */
	virtual TXIMPRequestId DoBlockUsersL(const MDesCArray* aUserIds ,TXIMPRequestId aReqId) = 0;
/**
     * Request the protocol to UnBlock User(s).
     *
     * @param [in] aUserIds - User id's need to be unblocked
     * @param [in] aReqId
     *        Request id identifying the issued request.
     *
	 * /b Note: Possible error conditions
     *	/n	 1. UserId doesn't exist [KImErrServiceGeneralError].
     *	/n	 2. The maximum number of users reached for the user in the list [KImErrTooManyUsers].
     */
	virtual TXIMPRequestId DoUnBlockUsersL(const MDesCArray* aUserIds,TXIMPRequestId aReqId ) = 0;

    /**
     * Request the protocol to get blocked users list from server
     *
     * @param [in] aImMessageId Identification for IM.
	   * @param [in] aReqId
     *        Request id identifying the issued request.
     *
	 * /b Note: Possible error conditions
     *	/n	 1. No block list in use [KImErrServiceGeneralError].
     */
	virtual void DoGetBlockedListRequestL( const MXIMPIdentity& aImMessageId,
											TXIMPRequestId aReqId ) = 0;

   
    /**
     * Request the protocol to retrieve the message-info structures of the undelivered instant messages. 
     * May use the retrieved Message-Info structures to inform the user about the new messages, or it May
	 * retrieve/reject/forward the instant messages automatically.
	 * 
     * @param [in] aImMessageId Identification for IM.
     * @param [in] aGroupId Identifies the group to retrieve history.
     * @param [in] aMessageCount The maximum number of message-info structures to be returned.
     * @param [in] aDeliveryReportWanted If the user wats to order delivery report.
	 * @param [in] aReqId
     *        Request id identifying the issued request.
    
	* /b Note: Possible error conditions
     *	/n	 1. Invalid group id [KImErrUnknownIMGroup].
     */
	virtual void DoGetMessageListL( const MXIMPIdentity& aImMessageId,
			    								   	   const TDesC* aGroupId,
			    								   	   const TInt aMessageCount,
			                                           TBool aDeliveryReportWanted,
			                                           TXIMPRequestId aReqId) = 0;
	
    /**
     * Request the protocol to reject the accumulated messages in the server.
     *
     * @param [in] aImMessageId Identification for IM.
     * @param [in] aMessageID Identifies the messages to be removed
	   * @param [in] aReqId
     *        Request id identifying the issued request.
     *
	 * /b Note: Possible error conditions
     *	/n	 1. Invalid message-id [KImErrFieldValueNotSupported].
     */
	virtual void DoRejectMessageL( const MXIMPIdentity& aImMessageId,
													const TDesC& aMessageID,
													TXIMPRequestId aReqId ) = 0;
	
    
    };



#endif // MPROTOCOLIMCONVERSATION_H

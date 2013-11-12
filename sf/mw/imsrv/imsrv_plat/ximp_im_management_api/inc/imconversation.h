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
* Description:  IM interface for clients use.
*
*/

#ifndef MIMCONVERSATIONIMP_H
#define MIMCONVERSATIONIMP_H

#include <e32std.h>
#include <badesca.h>
#include <bamdesca.h>
#include <ximpbase.h>
#include "immanagementifids.hrh"

class MImConversationInfo;
class MXIMPIdentity;
/**
 * Conversation interface for clients use.
 * Interface is implemented by the IM.
 *
 * @ingroup Immanagementapi
 * 
 */
class MImConversation : public MXIMPBase
    {
public:

    /** Interface ID for the MImConversation. */
    enum { KInterfaceId = IM_IF_ID_CONVERSATION };

protected:

    /**
     * Protected destructor.
     * Object instancies can't be deleted via this interface.
     */
    virtual ~MImConversation() {}

public: 
    /**
     * Send message to user(s)
     *
	 * @param [in] aImMessage 	- Identification and content of the message.
     *		MImConversationInfo contains messageid, contenttype - text/content, content,.......  
     * @param [in] aRecipients 	- Recipients userid.
	 *
	 *  /b Note: Possible error conditions
     * /n	 1. Invalid userid/recipient, user does not exist [KImApiErrInvalidUserId].
     * /n	 2. Recipient user blocked the sender [KImApiErrSenderBlocked].
     * /n	 3. Unsupported message context [KImErrUnsupportedContext].
     * /n	 4. Message queue full [KImErrTooManyIM].
     */
   virtual TXIMPRequestId SendMessageL( const MImConversationInfo& aImMessage) = 0;
     /**
     * Forward message to user(s)
     *
	 * @param [in] aImMessage 	- Identification and content of the message.
     *		MImConversationInfo contains messageid, contenttype - text/content, content,.......  
     * @param [in] aRecipients 	- Recipients userid.
	 *
	 * /b Note: Possible error conditions
     * /n	     1. Invalid userid/recipient, user does not exist [KImApiErrInvalidUserId].
     * /n		 2. Recipient user blocked the sender [KImApiErrSenderBlocked].
     * /n		 3. Unsupported message context [KImErrUnsupportedContext].
     * /n		 4. Message queue full [KImErrTooManyIM].
     */
    //virtual TXIMPRequestId ForwardMessageL( const MImConversationInfo& aImMessage) = 0;
	/**
     * Send message to group.
     *
	 * @param [in] aImMessage 	- Identification and content of the message.
     *		MImConversationInfo contains messageid, contenttype - text/content, content,.......  
     * @param [in] aUserIds 	- Recipients userid.
	 *
	 * /b Note: Possible error conditions
     * /n	     1. Invalid userid/recipient, user does not exist [KImApiErrInvalidUserId].
     * /n		 2. Recipient user blocked the sender [KImApiErrSenderBlocked].
     * /n		 3. Unsupported message context [KImErrUnsupportedContext].
     * /n		 4. Message queue full [KImErrTooManyIM].
     */


    //virtual TXIMPRequestId SendMessageToGroupL( const MImConversationInfo& aImMessage,
	//		                                        const TDesC* aGroupId ) = 0;

    /**
     * Block user(s).
	 *
     * @param [in] aUserIds - User id's need to be blocked
     *
     * /b Note: Possible error conditions
     * /n		 1. Userid doesn't exist [KImErrServiceGeneralError].
     * /n		 2. The maximum number of users reached for the user in the list [KImErrTooManyUsers].
     */
	//virtual TXIMPRequestId BlockUsersL(const MDesCArray* aUserIds ) = 0;

	/**
     * UnBlock User(s).
	 *
     * @param [in] aUserIds - User Id's need to be unblocked
     *
     * /b Note: Possible error conditions
     *	/n	 1. Userid doesn't exist [KImErrServiceGeneralError].
     *	/n	 2. The maximum number of users reached for the user in the list [KImErrTooManyUsers].
     */
	//virtual TXIMPRequestId UnBlockUsersL(const MDesCArray* aUserIds ) = 0;

    /**
     * Get Blocked usersid's list 
     *
     * /b Note: Possible error conditions
     *	/n	 1. No block list in use [KImErrServiceGeneralError].
     */
	//virtual TXIMPRequestId GetBlockedUsersListL(void ) = 0;

    /**
     * Retrieve the Message-Info structures of the undelivered instant messages. 
     * May use the retrieved message-info structures to inform the user about the new messages, or it may
	 * retrieve/reject/forward the instant messages automatically.
	 * 
     * @param [in] aImMessageId Identification for IM.
     * @param [in] aGroupId Identifies the group to retrieve history.
     * @param [in] aMessageCount The maximum number of message-info structures to be returned.
     * @param [in] aDeliveryReportWanted If the user wants to order delivery report.
	 *
     * /b Note: Possible error conditions.
     *	/n	 1. Invalid group id [KImErrUnknownIMGroup].
     */
	//virtual TXIMPRequestId GetMessageListL( const MXIMPIdentity& aImMessageId,
	//		    								   	   const TDesC* aGroupId,
	//		    								   	   const TInt aMessageCount,
	//		                                           TBool aDeliveryReportWanted) = 0;
	
    /**
     * Reject the accumulated messages in the server.
     *
     * @param [in] aImMessageId Identifies the messages to be removed
	 *
     * /b Note: Possible error conditions
     *	/n	 1. Invalid message-id [KImErrFieldValueNotSupported].
     */
	//virtual TXIMPRequestId RejectMessageL( const MXIMPIdentity& aImMessageId)= 0;
	
	
	private:
		// Message Info:
		// Identity  Messageid;
		// Message content text
		// Screenname
		// Deliver Report required
    };


#endif // MIMCONVERSATIONIMP_H



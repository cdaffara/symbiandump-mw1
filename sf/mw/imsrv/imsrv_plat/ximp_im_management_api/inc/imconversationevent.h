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
* Description:  Event interface for IM.
*
*/

#ifndef MIMCONVERSATIONEVENT_H
#define MIMCONVERSATIONEVENT_H

#include <e32std.h>
#include <ximpbase.h>
#include <immanagementifids.hrh>

class MImConversationInfo;
/**
 * Event interface for message content.
 *
 * This interface is implemented by the IM and
 * event object is signalled to registered client observers
 * through MPrFwIMContextObserver observer interface.
 * This event notifies clients about changes in
 * message content.
 * 
 * IM signals this event when:
 * - New message is received from protocol.
 * - Delivery report is received from the protocol
 *
 * @ingroup Immanagementapievents
 * 
 */
class MImConversationEvent : public MXIMPBase
    {
public:
    /** Interface ID for the MImConversationEvent. */
    enum { KInterfaceId = IM_IF_ID_CONVERSATION_EVENT };


protected:

    /**
     * Protected destructor.
     * Object instancies can't be deleted via this interface.
     */
    virtual ~MImConversationEvent() {}


public: // Access to "delta information"


    /**
     * Gets the count of newly received text messages.
     *
     * @return The count of newly received text messages.
     */
    virtual TInt NewTextMessageCount() const = 0;
    

    /**
     * Gets received text message by index.
     *
     * @param [in] aIndex
     *        The index of the text message to get.
     *
     * @return The received text message entry from given index.
     */
	virtual const MImConversationInfo& NewTextMessage( TInt aIndex ) const = 0;


    /**
     * Gets the count of newly received binary messages.
     *
     * @return The count of newly received binary messages.
     */
	//virtual TInt NewContentMessageCount() const = 0;
		

    /**
     * Gets received binary message by index.
     *
     * @param [in] aIndex
     *        The index of the binary message to get.
     *
     * @return The received binary message entry from given index.
     */
	//virtual const MImConversationInfo& NewContentMessage( TInt aIndex ) const = 0;


    /**
     * Gets the count of message delivery report.
     *
     * @return The count of message delivery report.
     */
	//virtual TInt DeliveryReportCount() const = 0;
	

    /**
     * Gets message delivery report by index.
     *
     * @param [in] aIndex
     *        The index of the message delivery report to get.
     *
     * @return The message delivery report entry from given index.
     */
	//virtual const MImConversationInfo& DeliveryReport( TInt aIndex ) const = 0;
	
 
    /**
     * Gets the count of blocked entity.
     *
     * @return The count of blocked entity.
     */
	//virtual TInt BlockedEntityCount( TInt aIndex ) const = 0;
	

    /**
     * Gets blocked entity by index.
     *
     * @param [in] aIndex
     *        The index of the blocked entity to get.
     *
     * @return The blocked entity entry from given index.
     */
	//virtual const MImConversationInfo& BlockedEntity( TInt aIndex ) const = 0;
	

    /**
     * Gets the count of undelivered messages.
     *
     * @return The count of undelivered messages.
     */
	//virtual TInt UndeliveredMessageCount() const = 0;
	

    /**
     * Gets undelivered message by index.
     *
     * @param [in] aIndex
     *        The index of the undelivered message to get.
     *
     * @return The undelivered message entry from given index.
     */
	//virtual const MImConversationInfo& UndeliveredMessage( TInt aIndex ) const = 0;
	

   /**
     * Gets the count of rejected messages.
     *
     * @return The count of rejected messages.
     */
	//virtual TInt RejectedMessageCount() const = 0;
	

    /**
     * Gets rejected message by index.
     *
     * @param [in] aIndex
     *        The index of the rejected message to get.
     *
     * @return The rejected message entry from given index.
     */
	//virtual const MImConversationInfo& RejectedMessage( TInt aIndex ) const = 0;
	
 
    /**
     * Gets the count of on going conversations.
     *
     * @return The count of on going conversations.
     */
	//virtual TInt ConversationCount() const = 0;
	

    /**
     * Gets on going conversation by index.
     *
     * @param [in] aIndex
     *        The index of the on going conversation to get.
     *
     * @return The on going conversation entry from given index.
     */
	//virtual const MImConversationInfo& Conversation( TInt aIndex ) const = 0;
	

    };


#endif // MIMCONVERSATIONEVENT_H



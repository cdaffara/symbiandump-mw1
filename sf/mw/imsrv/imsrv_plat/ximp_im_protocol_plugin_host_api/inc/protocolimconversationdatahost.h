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
* Description:  Interface for managing cached IM(Instant Message's) information.
*
*/

#ifndef MPROTOCOLCONVERSATIONDATAHOST_H
#define MPROTOCOLCONVERSATIONDATAHOST_H

#include <e32std.h>
#include <bamdesca.h>
#include <improtocolpluginhostifids.hrh>
#include <ximpbase.h>


class MXIMPIdentity;
class MXIMPObjectCollection;
class MXIMPDataSubscriptionState;
class MXIMPStatus;
class MImConversationInfo;


/**
 * Interface for managing cached conversation information.
 *
 * Interface is used by protocol adaptations to manage
 * conversation information stored
 * in IM internal data cache.
 *
 * This interface is implemented by IM.
 * Protocol adaptation can obtain reference to this
 * interface from MXIMPProtocolConnectionHost interface.
 *
 * For each separated protocol connection, protocol
 * adaptation must use the specific
 * MImProtocolConversationDataHost instance,
 * what is received from the connection specific
 * MXIMPProtocolConnectionHost instance.
 *
 * @ingroup Improtocolpluginhostapi
 * @since S60 
 */
class MProtocolImConversationDataHost : public MXIMPBase
    {
public:

    /** Interface ID for the MImProtocolConversationDataHost. */
    enum { KInterfaceId = IM_IF_ID_PROTOCOL_CONVERSATION_DATAHOST };

protected:

    /**
     * Protected destructor. Object instancies
     * can't be deleted via this interface.
     */
    virtual ~MProtocolImConversationDataHost() {}
    
public: // Handling IM information
	
	/**
     * Notifies IM from 
     * message sent completion information.
     *
     * Protocol adaptation implementation can call this
     * method to notify IM from message 
     * sent completion information. IM
     * updates its cached information and
     * notifies the subscribing clients from data changes.
     *
     * IM takes ownership of the argument(s),
     * if method succeeds without a leave.
     *
     * @param [in] aMessageId
     *        MXIMPIdentity object identifying the 
     *        Message sent information
     *
	 * @return The request id identifying the issued request.
     *         Data cache request is processed asynchronously and
     *         its completion is signalled to adaptation direction
     *         through MXIMPProtocolConnectionHostObserver interface,
     *         with MXIMPProtocolRequestCompleteEvent event.
     *
     * @leave KErrNoMemory if failed to allocate memory.
     * @leave KErrArgument if NULL argument(s) were provided.
     */
    //virtual TXIMPRequestId HandleSentMessageL(MXIMPIdentity* aMessageId) = 0;
	/**
     * Notifies IM from 
     * new text message received information.
     *
     * Protocol adaptation implementation can call this
     * method to notify IM from new
     * text message received information. IM
     * updates its cached new text message and
     * notifies the subscribing clients from data changes.
     *
     * IM takes ownership of the argument(s),
     * if method succeeds without a leave.
     *
     * @param [in] aMessageId
     *        MXIMPIdentity object identifying the new
     *        message information
     * @param [in] aSender
     *        message sender userid/groupId if the sender is group
     * @param [in] aScreenName
     *        ScreenName if the sender is groupid/NullDesc if the sender is userid
     * @param [in] aRecipients
     *        List of userid's / list of groupId's if receipients are groups.
     * @param [in] aScreenNames
     *        List of screennames if receipients are groups.
     * @param [in] aText
     *        Message text.
     *
	 * @return The request id identifying the issued request.
     *         Data cache request is processed asynchronously and
     *         its completion is signalled to adaptation direction
     *         through MXIMPProtocolConnectionHostObserver interface,
     *         with MXIMPProtocolRequestCompleteEvent event.
     *
     * @leave KErrNoMemory if failed to allocate memory.
     * @leave KErrArgument if NULL argument(s) were provided.
     */
    virtual TXIMPRequestId HandleNewTextMessageL( MImConversationInfo* aImMessage)  = 0;
	/**
     * Notifies IM from 
     * new content message received information.
     *
     * Protocol adaptation implementation can call this
     * method to notify IM from new
     * text message received information. IM
     * updates its cached new content message and
     * notifies the subscribing clients from data changes.
     *
     * IM takes ownership of the argument(s),
     * if method succeeds without a leave.
     *
     * @param [in] aMessageId
     *        MXIMPIdentity object identifying the new
     *        message information
     * @param [in] aSender
     *        message sender userid/groupid if the sender is group
     * @param [in] aScreenName
     *        ScreenName if the sender is groupid/NullDesc if the sender is userid
     * @param [in] aRecipients
     *        List of userid's / List of groupid's if receipients are groups.
     * @param [in] aScreenNames
     *        List of screennames if receipients are groups.
     * @param [in] aContentType
     *        Type of the message content.
     * @param [in] aContent
     *        Message content.
     *
	 * @return The request id identifying the issued request.
     *         Data cache request is processed asynchronously and
     *         its completion is signalled to adaptation direction
     *         through MXIMPProtocolConnectionHostObserver interface,
     *         with MXIMPProtocolRequestCompleteEvent event.
     *
     * @leave KErrNoMemory if failed to allocate memory.
     * @leave KErrArgument if NULL argument(s) were provided.
     */
    //virtual TXIMPRequestId HandleNewContentMessageL(  MXIMPIdentity* aMessageId,
    //                                       const TDesC& aSender,   
    //                                       const TDesC& aScreenName, 
    //                                       const MDesCArray& aRecipients,
    //                                       const MDesCArray& aScreenNames,                  
    //                                       const TDesC& aContentType, 
    //                                       const TDesC8& aContent)  = 0;
	/**
     * Notifies IM from 
     * blocked/granted List information.
     *
     * Protocol adaptation implementation can call this
     * method to notify IM from blocked/granted
     * list of information. IM
     * updates its cached blocked / granted list and
     * notifies the subscribing clients from data changes.
     *
     * IM takes ownership of the argument(s),
     * if method succeeds without a leave.
     *
     * @param [in] aBlockedList
     *        list of blocked users identified by userid
     * @param [in] aBlockedListInUse
     *        whether the blocked list is in use 
     * @param [in] aGrantedList
     *        list of granted users identified by userid
     * @param [in] aGrantedListInUse
     *        whether the granted list is in use
     *
	 * @return The request id identifying the issued request.
     *         Data cache request is processed asynchronously and
     *         its completion is signalled to adaptation direction
     *         through MXIMPProtocolConnectionHostObserver interface,
     *         with MXIMPProtocolRequestCompleteEvent event.
     *
     * @leave KErrNoMemory if failed to allocate memory.
     * @leave KErrArgument if NULL argument(s) were provided.
     */
     //virtual TXIMPRequestId HandleBlockedListResponseL( const MDesCArray* aBlockedList,
     //                                         TBool aBlockedListInUse,
     //                                         const MDesCArray* aGrantedList,
     //                                         TBool aGrantedListInUse)  = 0;
	/**
     * Notifies IM from 
     * message forward completion information.
     *
     * Protocol adaptation implementation can call this
     * method to notify IM from Message 
     * forward completion information. IM
     * updates its cached information and
     * notifies the subscribing clients from data changes.
     *
     * IM takes ownership of the argument(s),
     * if method succeeds without a leave.
     *
     * @param [in] aMessageId
     *        MXIMPIdentity object identifying the 
     *        message forward information
     *
	 * @return The request ID identifying the issued request.
     *         Data cache request is processed asynchronously and
     *         its completion is signalled to adaptation direction
     *         through MXIMPProtocolConnectionHostObserver interface,
     *         with MXIMPProtocolRequestCompleteEvent event.
     *
     * @leave KErrNoMemory if failed to allocate memory.
     * @leave KErrArgument if NULL argument(s) were provided.
     */
    //virtual TXIMPRequestId HandleForwardMessageCompletionL(MXIMPIdentity* aMessageId) = 0;



	/**
     * Notifies IM from 
     * received message list information.
     *
     * Protocol adaptation implementation can call this
     * method to notify IM from message 
     * forward completion information. IM
     * updates its cached information and
     * notifies the subscribing clients from data changes.
     *
     * IM takes ownership of the argument(s),
     * if method succeeds without a leave.
     *
     * @param [in] aMessageList
     *			MImFtpPluginObjectCollection object containing zero or
     *        	more /Message info class/ objects. Each
     *        	contained information about the message.
     *
	 * @return The request ID identifying the issued request.
     *         Data cache request is processed asynchronously and
     *         its completion is signalled to adaptation direction
     *         through MXIMPProtocolConnectionHostObserver interface,
     *         with MXIMPProtocolRequestCompleteEvent event.
     *
     * @leave KErrNoMemory if failed to allocate memory.
     * @leave KErrArgument if NULL argument(s) were provided.
     */
	//virtual TXIMPRequestId HandleGetMessageListL( /*MImFtpPluginObjectCollection* aMessageList */) = 0;
	/**
     * Notifies IM from 
     * reject message information.
     *
     * Protocol adaptation implementation can call this
     * reject message information.
     * method to notify IM from 
     * reject message  information. 
	 * IM updates its cached new text message and
     * notifies the subscribing clients from data changes.
     *
     * IM takes ownership of the argument(s),
     * if method succeeds without a leave.
     *
     * @param [in] aMessageId
     *        MXIMPIdentity object identifying the reject
     *        message information
     *
	 * @return The request id identifying the issued request.
     *         data cache request is processed asynchronously and
     *         its completion is signalled to adaptation direction
     *         through MXIMPProtocolConnectionHostObserver interface,
     *         with MXIMPProtocolRequestCompleteEvent event.
     *
     * @leave KErrNoMemory if failed to allocate memory.
     * @leave KErrArgument if NULL argument(s) were provided.
     */
    //virtual TXIMPRequestId HandleRejectMessageL( MXIMPIdentity* aMessageId) = 0;
                                        
   


    };


#endif // MPROTOCOLCONVERSATIONDATAHOST_H

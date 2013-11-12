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
* Description:  Interface for IM info object.
*
*/

#ifndef MIMCONVERSATIONINFO_H
#define MIMCONVERSATIONINFO_H

#include <e32std.h>
#include <bamdesca.h>
#include <ximpbase.h>
#include <imdatamodelifids.hrh>


class MXIMPIdentity;

 /**
     * Common enumerated message type values for "messages"
     * message type field.
     *
     * @ingroup imdatamodelapi
     *
     */
    enum TMessageTypes
        {

        /**
         * Message type is private.
         */
        EPrivateMessage = 0,

        };

/**
 * Interface for IM info object.
 *
 * IM info object presents single IM conversation
 * IM info object holds the identity, message content, delivery result, conversation ID
 * block entity display name
 *
 *
 * @ingroup Imdatamodelapi
 * 
 */
class MImConversationInfo : public MXIMPBase
    {
public:

    /** Interface ID for the MImConversationInfo. */
    enum { KInterfaceId = IM_IF_ID_CONVERSATION_INFO };


public:

    /**
     * Public destructor.
     * Objects can be deleted through this interface.
     */
    virtual ~MImConversationInfo() {}


public:

    /**
     * Returns identity of the message.
     *
     * @return The identity of the message.
     */
	virtual const MXIMPIdentity& MessageId() const = 0;

	virtual const TDesC16& TextMessage() const = 0;
	
	virtual const MDesCArray&  RecipientL() const = 0;
    /**
     * Sets identity of the message.
     *
     * @param [in] aIdentity
     *        Identity of the message.
     *        Ownership of the argument is transfered to
     *        callee, if method succeeds without a leave.
     *
     * @leave KErrNoMemory if failed to allocate memory.
     * @leave KErrArgument if NULL argument were provided.
     */
	virtual void SetMessageIdL(
                MXIMPIdentity* aIdentity ) = 0;


    virtual void SetTextMessageL(
                const TDesC16& aMessage ) = 0;   
                 
    virtual void SetRecipientL(const MDesCArray* aRecipients) = 0;

	
    };


#endif // MIMCONVERSATIONINFO_H




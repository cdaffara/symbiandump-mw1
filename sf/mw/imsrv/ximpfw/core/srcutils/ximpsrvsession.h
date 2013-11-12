/*
* Copyright (c) 2006 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Abstract server session interface
 *
*/

#ifndef MXIMPSRVSESSION_H
#define MXIMPSRVSESSION_H

#include <e32std.h>

class MXIMPSrvMessage;


/**
 * Abstract server session interface
 */
class MXIMPSrvSession
    {
public:

    /**
     * Public destructor.
     * Objects can be deleted through this interface.
     */
    virtual ~MXIMPSrvSession() { };


    /**
     * Message handling method.
     * Session adapters call this to mediate message handling
     * to concrete session objects.
     *
     * Rules for message handling:
     *
     * - If message is handled synchronously, handler must complete
     * the message when finished with it. Handler may not cause leave
     * exeptions after completing.
     *
     * - If message handling fails, it should be handled with a leave.
     * In this case Session::ServiceError() methods take care
     * of completing the message with leave code.
     *
     * - If handler wishes to complete the message asynchronously,
     * it must take message ownership to itself by using
     * MXIMPSrvMessage::PlaceOwnershipHere(). In this case handler
     * is responsible both to completing and deleting the message object.
     * After taking message ownership, message handler isn't anymore
     * allowed to cause leave exeptions during the same message
     * handling path.
     *
     * @param aMessage The message to handle.
     */
    virtual void TryHandleMessageL( MXIMPSrvMessage& aMessage ) = 0;
    };


#endif      //  MXIMPSRVSESSION_H



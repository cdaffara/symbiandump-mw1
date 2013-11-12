/*
* Copyright (c) 2006, 2007 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Event interface for XIMP context state notification.
*
*/

#ifndef MXIMPFWPRESENCECONTEXTSTATEEVENT_H
#define MXIMPFWPRESENCECONTEXTSTATEEVENT_H


#include <e32std.h>
#include <ximpbase.h>
#include <ximpmanagementifids.hrh>


class MXIMPStatus;
class MXIMPContextState;



/**
 * Event interface for XIMP context state notification.
 *
 * This interface is implemented by the XIMP Framework,
 * and it is used to notify clients about XIMP context
 * state changes.
 *
 * XIMP Framework signalls this event it to registered
 * client observers through MXIMPContextObserver
 * observer interface. Clients do need to perform any specific
 * subscribe request for this event type, just register a valid
 * observer with MXIMPPscContext::RegisterObserverL(),
 * with such event type filter, that this
 * MXIMPContextStateEvent interface is accepted.
 *
 * @ingroup ximpfwmanagementapievents
 * @since S60 v3.2
 */
class MXIMPContextStateEvent : public MXIMPBase
    {
public:

    /** Interface ID for the MXIMPContextStateEvent. */
    enum { KInterfaceId = XIMP_IF_ID_CONTEXT_STATE_EVENT };



protected:

    /**
     * Protected destructor.
     * Event objects can't be deleted via this interface.
     */
    virtual ~MXIMPContextStateEvent() {}



public:


    /**
     * Gets the XIMP context current state.
     *
     * @return The XIMP context current state.
     */
    virtual const MXIMPContextState& ContextState() const = 0;



    /**
     * Gets status object describing the XIMP contexts
     * state change reason.
     *
     * Status object is optional and it is present only in
     * those state change events which are originated from
     * underlying protocol. Protocol can fill in into status
     * object the details about the state reason.
     *
     * @return The MXIMPStatus object describing
     *         the XIMP context state change reason.
     *         NULL if no status object is present.
     *         Object ownersip is not returned.
     */
    virtual const MXIMPStatus* StateChangeReason() const = 0;

    };



#endif // MXIMPFWPRESENCECONTEXTSTATEEVENT_H




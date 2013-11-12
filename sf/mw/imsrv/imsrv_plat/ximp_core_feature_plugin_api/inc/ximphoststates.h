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
* Description:  States for PSC Host FSM
 *
*/

#ifndef PSCHOSTSTATES_H
#define PSCHOSTSTATES_H

#include <e32std.h>

/**
 * PSC host states.
 * Used as states for the finite state machine inside the host.
 */
namespace NXIMPHost
    {
    /**
     * States for the host (internal FSA)
     */
    enum THostStates
        {
        // actual states
        EXIMPNotConnected = 0,   // not connected
        EXIMPConnecting,         // connecting attempt ongoing
        EXIMPConnected,          // connected
        EXIMPReconnecting,       // attempting connection after nw suspend
        EXIMPSuspended,          // nw down
        EXIMPDisconnecting,      // disconnecting
        
        // helper states
        EXIMPHelperReconnectFailed // reconnecting failed
        };

    /**
     * Operation status, needed by host FSA state changes.
     * Called by operations. Used to update internal state
     * of host to determine whether network connection is up, down,
     * inactive or network suspended. See PSC Host-Plugin FSA in design
     * document.
     */
    enum THostOperationState
        {
        EXIMPOpNone,             // generic empty state
        EXIMPOpBindBegin,        // bind is beginning
        EXIMPOpBindEnd,          // bind has ended
        EXIMPOpShareBegin,       // share is beginning
        EXIMPOpShareEnd,         // share has ended
        EXIMPOpUnshareBegin,     // unshare is beginning
        EXIMPOpUnshareEnd,       // unshare has ended
        EXIMPOpReconnectBegin,   // reconnect is beginning
        EXIMPOpReconnectEnd,     // reconnect has ended
        EXIMPOpUnbindBegin,      // unbind is beginning
        EXIMPOpUnbindEnd,        // unbind has ended
        
        EXIMPOpSessionLostBegin,     // session lost handling is beginning
        EXIMPOpSessionLostEnd,       // session lost handling has ended
        EXIMPOpForcedTeardownBegin,  // forced teardown is beginning
        EXIMPOpForcedTeardownEnd,    // forced teardown has ended
        };
    }

#endif  // PSCHOSTSTATES_H






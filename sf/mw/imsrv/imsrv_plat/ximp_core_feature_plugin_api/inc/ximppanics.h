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
* Description:  XIMP Framework panic defines.
 *
*/

#ifndef XIMPPANICS_H
#define XIMPPANICS_H

#include <e32std.h>

/**
 * Panic category and reason codes for XIMP Framework client
 * failures.
 */
namespace NXIMPPanic
    {
    _LIT( KCategory, "XIMPClnt" );

    enum TReason
        {
        EOnSrvSessionImpAlreadyExists,
        EOnSrvSessionImpNotFound,
        EOnSrvRequestNotUnderstood,
        EOnSrvAlreadyListeningEvents,
        EOnSrvAlreadyHavingPrepearedData,
        EOnSrvNoPrepearedDataToCopy,
        EOnSrvUnknownCtxOperationType
        };
    }


/**
 * Panic category and reason codes for XIMP Framework server
 * failures.
 */
namespace NXIMPPrivPanic
    {
    _LIT( KCategory, "XIMPPriv" );
    enum TReason
        {
        ECurrentMsgAlreadyExists,                   //  0
        ECurrentMsgNotFoundInServiceError,          //  1
        ECurrentMsgNotValidInServiceError,          //  2
        EMessageStillPendingOnDestruction,          //  3
        EMessageSelfPtrAlreadyOccupied,             //  4
        EPscEnvGlobalsExistsAlready,                //  5
        EPscEnvGlobalsNotFound,                     //  6
        EFrontEndEventPumpNoServerClient,           //  7
        EPtrArrayEntryAddUndoInitialized,           //  8
        EPtrArrayEntryReplaceUndoInitialized,       //  9
        EInvalidOperationType,                      // 10
        EProtocolAlreadyExistsInHost,               // 11
        EHostMgrHostsHaveNotBeenDeleted,            // 12
        EContextAlreadyHasAHost,                    // 13
        EContextHasNoHost,                          // 14
        EHostStillReferenced,                       // 15
        EHostNegativeReferenceCount,                // 16
        EHostCurrentOperationStillActive,           // 17
        EInvalidSubscriptionStatus,                 // 18
        EFakeRequestCompleteNotReady,               // 19
        EOwnPresencePifNotSet,                      // 20
        EArrayCountNotValid,                        // 21
        EOwnPresencePresenceInfoDoesNotExist,       // 22
        EInvalidSubscriptionType,                   // 23
        ESubscriptionItemLogicError,                // 24
        EPresentityPresencePresenceInfoDoesNotExist,// 25
        EHostNotFound,                              // 26
        ENoCachedUnbindsFound,                      // 27
        EInvalidHostForProtocol,                    // 28
        ECurrentOperationNull,                      // 29
        EInvalidFilterType,                         // 30
        EMatchingClientSideReqNotFound,             // 31

        // invalid operation state transitions
        EInvalidStateForSessionLost = 40,           // 40
        EInvalidStateForOp,                         // 41
        EInvalidStateForOpBindBegin,                // 42
        EInvalidStateForOpBindEnd,                  // 43
        EInvalidStateForOpShareBoth,                // 44
        EInvalidStateForOpUnshareBoth,              // 45
        EInvalidStateForOpReconnectBegin,           // 46
        EInvalidStateForOpReconnectEnd,             // 47
        EInvalidStateForOpUnbindBegin,              // 48
        EInvalidStateForOpUnbindEnd,                // 49
        EInvalidStateForOpSessionLostBegin,         // 50
        EInvalidStateForOpSessionLostEnd,           // 51
        EInvalidStateForOpTearDownBoth,             // 52

        // invalid state transitions
        EInvalidStateTransUnknown = 60,             // 60
        EInvalidStateTransForNotConnected,          // 61
        EInvalidStateTransForConnecting,            // 62
        EInvalidStateTransForConnected,             // 63
        EInvalidStateTransForReconnecting,          // 64
        EInvalidStateTransForSuspended,             // 65
        EInvalidStateTransForDisconnecting,         // 66
        };
        
        
    /**
     * Panic function to trigger NXIMPPrivPanic
     * panics.
     */
    IMPORT_C void Panic( TReason aReason );
    }

namespace NXIMPPlugin
    {
    _LIT( KCategory, "XIMPPlugin" );
    enum TReason
        {
        EInvalidRequestId
        };

    }


#endif  //ximppanics_H






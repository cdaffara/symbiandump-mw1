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
* Description:  XIMP framework presence operation definitions.
 *
*/

#ifndef PRESENCEOPERATIONDEFS_H
#define PRESENCEOPERATIONDEFS_H

#include <e32std.h>

// TODO: use ranges for operations types, that will make sure that
// the core, presence and IM operations won't overlap

/**
 * PSC operations.
 */
namespace NPresenceOps
    {
    enum TPresenceOpTypes
        {
        // -----------------------------------------------
        // From client
        //
        EPrPublishOwnPresence = 10,                    // 10
        EPrSubscribeOwnPresence,                       // 11
        EPrUnsubscribeOwnPresence,                     // 12

        EPrSubscribeGroupList,                         // 13
        EPrUnsubscribeGroupList,                       // 14

        EPrCreatePresentityGroup,                      // 15

        EPrSubscribePresentityPresence,                // 16
        EPrUnsubscribePresentityPresence,              // 17

        EPrDeletePresentityGroup,                      // 18
        EPrUpdatePresentityGroupDisplayName,           // 19

        EPrSubscribePresentityGroupContent,            // 20
        EPrUnsubscribePresentityGroupContent,          // 21

        EPrAddPresentityGroupMember,                   // 22
        EPrRemovePresentityGroupMember,                // 23
        EPrUpdatePresentityGroupMemberDisplayName,     // 24
        EPrSynthesisePresenceSubscription,             // 25
        EPrSubscribeGroupContent,                      // 26
        EPrUnsubscribeGroupContent,                    // 27

        EPrPresentityGroupChange,                      // 28
        EPrPresentityGroupContentChange,               // 29

        EPrSubscribePresenceWatcherList,               // 30
        EPrUnsubscribePresenceWatcherList,             // 31

        EPrSubscribePresentityGroupMembersPresence,    // 32
        EPrUnsubscribePresentityGroupMembersPresence,  // 33

        EPrSubscribePresenceGrantRequestList,          // 34
        EPrUnsubscribePresenceGrantRequestList,        // 35

        EPrGrantPresenceToPresentity,                  // 36
        EPrWithdrawPresenceFromPresentity,             // 37
        EPrGrantPresenceToPresentityGroupMembers,      // 38
        EPrWithdrawPresenceFromPresentityGroupMembers, // 39
        EPrGrantPresenceToEveryone,                    // 40
        EPrWithdrawPresenceFromEveryone,               // 41

        EPrSubscribeBlockList,                         // 42
        EPrUnsubscribeBlockList,                       // 43
        EPrBlockPresenceForPresentity,                 // 44
        EPrCancelPresenceBlockFromPresentity,          // 45

        // -----------------------------------------------
        // From adaptation
        //
        EPrHandleOwnPresence = 50,                         // 50
        EPrSetOwnPresenceDataSubscriptionState,            // 51
        EPrHandlePresentityPresence,                       // 52
        EPrSetPresentityPresenceDataSubscriptionState,     // 53
        EPrHandlePresentityPresenceTerminated,             // 54
        EPrHandlePresenceWatcherList,                      // 55
        EPrSetPresenceWatcherListDataSubscriptionState,    // 56

        // Following map internally to the same operation
        EPrHandlePresenceGrantRequestList,                 // 57
        EPrHandlePresenceGrantRequestReceived,             // 58
        EPrHandlePresenceGrantRequestObsoleted,            // 59
        EPrSetPresenceGrantRequestDataSubscriptionState,   // 60

        // these map internally to the same operation
        EPrHandlePresenceBlockList,                // 61
        EPrHandlePresenceBlocked,                  // 62
        EPrHandlePresenceBlockCanceled,            // 63
        EPrSetPresenceBlockDataSubscriptionState,  // 64

        // these map internally to the same operation
        EPrHandlePresentityGroupList = 70,             // 70 - new list
        EPrHandlePresentityGroupCreated,               // 71 - created/updated groups
        EPrHandlePresentityGroupDeleted,               // 72 - deleted groups
        EPrHandlePresentityGroupDisplayNameUpdated,    // 73 - display name updated
        EPrSetPresentityGroupListDataSubscriptionState,// 74

        EPrHandlePresentityGroupContent = 80,              // 80
        EPrHandlePresentityGroupMemberAdded,               // 81
        EPrHandlePresentityGroupMemberRemoved,             // 82
        EPrHandlePresentityGroupMemberDisplayNameUpdated,  // 83
        EPrSetPresentityGroupContentDataSubscriptionState,  // 84
        
        EXIMPCleaner = 90,                       // 90 - Internal operation for cleaning.

        };
    }

#endif  //PRESENCEOPERATIONDEFS_H






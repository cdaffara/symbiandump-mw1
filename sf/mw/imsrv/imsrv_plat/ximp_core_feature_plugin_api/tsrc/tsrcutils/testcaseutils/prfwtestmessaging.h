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
* Description:
*
*/


#ifndef T_PRFWTESTMESSAGING_H_
#define T_PRFWTESTMESSAGING_H_

#include <e32base.h>
#include <e32property.h>

// FIXME change this?
// base UID for pubsub keys
const TUid KXIMPTestPropertyCatBase = { 0x10012345 };

enum TXIMPTestPropertyValues
    {
    // values for EXIMPPlgTestDeath
    EXIMPPrtValPluginDeathEmpty = 0,
    EXIMPPrtValPluginDeathDied = 1,

    // values for EXIMPTestPlgSessionLostStrategy
    EXIMPPrtValSessionLostEmpty = 0,
    EXIMPPrtValSessionLostReconnect,
    EXIMPPrtValSessionLostTeardown,
    EXIMPPrtValSessionLostReconnectExhaustAttempts,

    // values for EXIMPTestPlgBehaveGeneric
    EXIMPPrtValBehaveNothing = 0,           // do nothing
    };

// pubsub keys
enum TXIMPTestPropertyKeys
    {
    // meta things (message queue internal)
    EXIMPTestMetaPubSubDefined,             // pubsub stuff has been defined (1)
    EXIMPTestMetaPluginIndex,               // protocol plugin reads its index from this (-1=undefined)

    // p = plugin, h = host, t = test code.
    // direction also encoded into the property name

    // Instructions for plugin behaviour (t->p).
    // next call to plugin API method will cause the requested behavior (e.g. leave)
    EXIMPTestPlgBehaveLeave,            // leave with given code
    EXIMPTestPlgBehaveError,            // cause error with given code

    // boolean valued
    EXIMPTestPlgBehaveCloseHandle,      // abrupt close handle (1)
    EXIMPTestPlgBehaveInvalidReqId,     // complete next call with invalid req. id (1)

    // API method called -messages (p->t)
    // value is boolean (set or unset, 1 or 0)
    EXIMPPlgTestOpenSessionCalled,           // OpenSessionL
    EXIMPPlgTestShareSessionCalled,          // ShareSessionL
    EXIMPPlgTestUnshareSessionCalled,        // ShareSessionL
    EXIMPPlgTestCloseSessionCalled,          // CloseSessionL
    EXIMPPlgTestPublishPresenceCalled,       // PublishPresenceL
    EXIMPPlgTestUpdateInterestCalled,        // UpdatePresenceInterestL
    EXIMPPlgTestSubscribeOwnPresenceCalled,  // DoSubscribeOwnPresenceL
    EXIMPPlgTestUnsubscribeOwnPresenceCalled, // DoUnsubscribeOwnPresenceL
    EXIMPPlgTestUpdateOwnSubscriptionCalled, // DoUpdateOwnPresenceSubscriptionL
    EXIMPPlgTestSubscribeGroupListCalled,    // DoSubscribePresentityGroupListL
    EXIMPPlgTestUnsubscribeGroupListCalled,    // DoUnsubscribePresentityGroupListL
    EXIMPPlgTestCreatePresentityGroupCalled, // DoCreatePresentityGroupL
    EXIMPPlgTestDeletePresentityGroupCalled, // DoDeletePresentityGroupL
    EXIMPPlgTestUpdatePresentityGroupDisplayNameCalled, // DoUpdatePresentityGroupDisplayNameL
    EXIMPPlgTestSubscribePresentityPresesenceCalled,    // DoSubscribePresentityPresenceL
    EXIMPPlgTestUpdatePresentityPresesenceCalled,       // DoUpdatePresentityPresenceSubscriptionL
    EXIMPPlgTestUnsubscribePresentityPresesenceCalled,  // DoUnsubscribePresentityPresenceL
    EXIMPPlgTestSubscribePresentityGroupMembersPresesenceCalled,    // DoSubscribePresentityGroupMembersPresenceL
    EXIMPPlgTestUpdatePresentityGroupMembersPresesenceCalled,       // DoUpdatePresentityGroupMembersPresenceSubscriptionL
    EXIMPPlgTestUnsubscribePresentityGroupMembersPresesenceCalled,  // DoUnsubscribePresentityGroupMembersPresenceL

    EXIMPPlgTestSubscribePresentityGroupContentCalled,  // DoSubscribePresentityGroupContentL
    EXIMPPlgTestUnsubscribePresentityGroupContentCalled,  // DoUnsubscribePresentityGroupContentL
    EXIMPPlgTestAddPresentityGroupMemberCalled,  // DoAddPresentityGroupMemberL
    EXIMPPlgTestRemovePresentityGroupMemberCalled,  // DoRemovePresentityGroupMemberL
    EXIMPPlgTestUpdatePresentityGroupMemberDisplayNameCalled,  // DoUpdatePresentityGroupMemberDisplayNameL
    EXIMPPlgTestSubscribePresenceWatcherListCalled,     // DoSubscribePresenceWatcherListL
    EXIMPPlgTestUnsubscribePresenceWatcherListCalled,   // DoUnsubscribePresenceWatcherListL

    EXIMPPlgTestSubscribePresenceGrantRequestListCalled,    // DoSubscribePresenceGrantRequestListL
    EXIMPPlgTestUnsubscribePresenceGrantRequestListCalled,    // DoUnsubscribePresenceGrantRequestListL

    EXIMPPlgTestGrantPresenceForPresentityCalled,                   // EXIMPPlgTestGrantPresenceForPresentityCalled
    EXIMPPlgTestGrantPresenceForPresentityGroupMembersCalled,       // EXIMPPlgTestGrantPresenceForPresentityGroupMembersCalled
    EXIMPPlgTestGrantPresenceForEveryoneCalled,                   // EXIMPPlgTestGrantPresenceForEveryoneCalled
    EXIMPPlgTestUpdateGrantPresenceForPresentityCalled,             // EXIMPPlgTestUpdateGrantPresenceForPresentityCalled
    EXIMPPlgTestUpdateGrantPresenceForEveryoneCalled,             // EXIMPPlgTestUpdateGrantPresenceForEveryoneCalled
    EXIMPPlgTestUpdateGrantPresenceForPresentityGroupMembersCalled, // EXIMPPlgTestUpdateGrantPresenceForPresentityGroupMembersCalled
    EXIMPPlgTestWithdrawPresenceFromPresentityCalled,               // EXIMPPlgTestWithdrawPresenceFromPresentityCalled
    EXIMPPlgTestWithdrawPresenceFromPresentityGroupMembersCalled,   // EXIMPPlgTestWithdrawPresenceFromPresentityGroupMembersCalled
    EXIMPPlgTestWithdrawPresenceFromEveryoneCalled,               // EXIMPPlgTestWithdrawPresenceFromEveryoneCalled
    EXIMPPlgTestSubscribePresenceBlockListCalled,                   // EXIMPPlgTestSubscribePresenceBlockListCalled
    EXIMPPlgTestUnsubscribePresenceBlockListCalled,                  // EXIMPPlgTestUnubscribePresenceBlockListCalled
    EXIMPPlgTestBlockPresenceForPresentityCalled,                   // EXIMPPlgTestBlockPresenceForPresentityCalled
    EXIMPPlgTestCancelPresenceBlockFromPresentityCalled,            // EXIMPPlgTestCancelPresenceBlockFromPresentityCalled
    
    EXIMPPlgTestSubscribePresentityGroupListCalled,  // DoSubscribePresentityGroupListL
    EXIMPPlgTestUnsubscribePresentityGroupListCalled,  // DoUnsubscribePresentityGroupListL
    EXIMPPlgTestHandlePresentityGroupListCalled, //EXIMPPlgTestHandlePresentityGroupListCalled
    EXIMPPlgTestHandlePresentityPresenceCalled,   
    


    // For signaling some situations checked by adaptation
    EXIMPPlgTestExistsGroupContentInCache,              // Check if group content exists in cache.

    // signal Plg destruction (p->t)
    EXIMPPlgTestDeath,

    // simulate session lost with given strategy (t->p)
    EXIMPTestPlgSessionLostStrategy,
    };

// T-class for internal use
class TXIMPTestMsg
    {
public:
    TXIMPTestPropertyKeys iKey;     // pubsub key
    TInt iVal;                      // pubsub value
    };

    /**
     * @param aIndex The index for the testing context
     * @param aDescription Description for failed test.
     * @param aModifier If EFalse, negate the test, thus checking for
     * publish presence NOT being called.
     * @param aDescription2 Description for negate failed test.
     */
#define COMMONASSERT( aMsg, aKey, aDescription, aModifier, aDescription2 ) \
        { \
        TBool value = aMsg->GetBoolean( aKey ); \
        if( aModifier ) \
            { \
            EUNIT_ASSERT_DESC( value, aDescription ); \
            } \
        else if( !aDescription2 ) \
            { \
            EUNIT_ASSERT_DESC( value, "Description 2 not set." ); \
            } \
        else \
            { \
            EUNIT_ASSERT_DESC( !value, aDescription2 ); \
            } \
        }

#endif // T_PRFWTESTMESSAGING_H_



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
* Description: XIMP Framework Test Code 
*
*/

#ifndef T_PRESENCEWATCHING_H
#define T_PRESENCEWATCHING_H

#include <ceunittestsuiteclass.h>
#include <ximpbase.h>
#include "prfwteststatuseventlistener.h"
#include "prfwtestmessaging.h"

class CXIMPTestContextWrapper;
class CXIMPTestContextWrapperMgr;
class CXIMPTestMessenger;
class MPresenceCacheReader;

/**
 * XIMP Framework Eunit tests.
 *
 * Tests for XIMP session management services.
 *
 * @since S60 v4.0
 */
class T_PresenceWatching : public CEUnitTestSuiteClass
    {

public:
    static T_PresenceWatching* NewL();
    virtual ~T_PresenceWatching();


private:
    T_PresenceWatching();
    void ConstructL();



private:    // Test case functions

    void Setup_L();
    void SetupMultiple_L();
    void BindL();
    void BindAllL();
    void Teardown();
    void UnbindL();
    void UnbindAllL();

    void T_SubscribePresentityPresence_Single_L();
    void T_SubscribeHandlePresentityPresence_Single_L();
    void T_SubscribePresentityPresenceChangeId_Single_L();
    void T_UnsubscribeNotSubscribedPresentityPresence_Single_L();
    void T_UnsubscribeNotSubscribedPresentityPresence_Multiple_L();

    void T_SubscribePresentityPresence_Multiple_L();
    void T_SubscribePresentityGroupMembersPresence_Single_L();
    void T_UnsubscribeNotSubscribedPresentityGroupMembersPresence_Single_L();
    void T_UnsubscribeNotSubscribedPresentityGroupMembersPresence_Multiple_L();
    void T_ContentChangePresentityGroupMembersPresence_Single_L();
    
    void T_ErrFromPlg_LeaveOnSubscribePresentityPresence_L();
    void T_ErrFromPlg_LeaveOnSubscribePresentityGroupMembersPresence_L();
    void T_ErrFromPlg_LeaveOnUnsubscribePresentityPresence_L();
    void T_ErrFromPlg_LeaveOnUnsubscribePresentityGroupMembersPresence_L();
    
    void T_SubscribeGroupMembersPresence_Multiple_L();

private:    // Test helpers

    void SetupListenerReqCompleteL( CXIMPTestContextWrapper* aWrapper, TXIMPTestStatusEventTemplate aEventType );
    void SetupListenerReqCompleteAllowEventsL( CXIMPTestContextWrapper* aWrapper, TXIMPTestStatusEventTemplate aEventTypes );

private: // Test data

    /**
     * Eunit test case table declaration.
     */
    EUNIT_DECLARE_TEST_TABLE;

    // owned
    CXIMPTestContextWrapperMgr* iWrapperMgr;
    
    MPresenceCacheReader* iMPresenceCacheReader;
    };


#endif      //  T_PRESENCEWATCHING_H



// end of file





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

#ifndef T_PRESENCEAUTHORIZATION_H
#define T_PRESENCEAUTHORIZATION_H

#include <ceunittestsuiteclass.h>
#include <ximpbase.h>
#include "prfwteststatuseventlistener.h"
#include "prfwtestmessaging.h"

class CXIMPTestContextWrapper;
class CXIMPTestContextWrapperMgr;
class CXIMPTestMessenger;

/**
 * XIMP Framework Eunit tests.
 *
 * Tests for XIMP session management services.
 *
 * @since S60 v4.0
 */
class T_PresenceAuthorization : public CEUnitTestSuiteClass
    {

public:
    static T_PresenceAuthorization* NewL();
    virtual ~T_PresenceAuthorization();


private:
    T_PresenceAuthorization();
    void ConstructL();

private:    // Test case functions

    void Setup_L();
    void SetupMultiple_L();
    void BindL();
    void BindAllL();
    void Teardown();
    void UnbindL();
    void UnbindAllL();

    void T_GrantPresenceForPresentity_L();
    void T_GrantPresenceForPresentityChangeId_L();
    void T_WithdrawNotGrantedPresenceFromPresentity_L();
    void T_ErrFromPlg_LeaveOnGrantPresenceForPresentity_L();
    void T_ErrFromPlg_LeaveOnWithdrawPresenceFromPresentity_L();

    void T_GrantPresenceForPresentity_Multiple_L();
    void T_WithdrawNotGrantedPresenceFromPresentity_Multiple_L();
    
    void T_GrantPresenceForGroupMembers_L();
    void T_GrantPresenceForGroupMembersChangeId_L();
    void T_ErrFromPlg_LeaveOnGrantPresenceForGroupMembers_L();
    void T_ErrFromPlg_LeaveOnWithdrawPresenceFromGroupMembers_L();

    void T_GrantPresenceForGroupMembers_Multiple_L();
    void T_WithdrawNotGrantedPresenceFromGroupMembers_L();
    void T_WithdrawNotGrantedPresenceFromGroupMembers_Multiple_L();

    void T_GrantPresenceForEveryone_L();
    void T_ErrFromPlg_LeaveOnGrantPresenceForEveryone_L();
    void T_ErrFromPlg_LeaveOnWithdrawPresenceFromEveryone_L();

    void T_GrantPresenceForEveryone_Multiple_L();
    void T_WithdrawNotGrantedPresenceFromEveryone_L();
    void T_WithdrawNotGrantedPresenceFromEveryone_Multiple_L();

    void T_MakeAndGrantGroup_WithdrawOneID_L();

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
    };


#endif      //  T_PRESENCEAUTHORIZATION_H



// end of file





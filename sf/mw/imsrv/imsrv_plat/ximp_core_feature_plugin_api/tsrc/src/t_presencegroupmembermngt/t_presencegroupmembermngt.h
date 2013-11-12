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

#ifndef T_PRESENCEGROUPMEMBERMANAGEMENT_H
#define T_PRESENCEGROUPMEMBERMANAGEMENT_H

#include <ceunittestsuiteclass.h>
#include <ximpbase.h>
#include <presenceinfo.h>
#include "prfwteststatuseventlistener.h"
#include "prfwtestmessaging.h"

class CXIMPTestContextWrapper;
class CXIMPTestContextWrapperMgr;
class CXIMPTestMessenger;
class CPresentityGroupListEventImp;
class CPresentityGroupContentEventImp;

class T_PresenceGroupMemberManagement;

class CXIMPTestListener;

// function pointer to a method
typedef void (T_PresenceGroupMemberManagement::* TXIMPCallBack )();

/**
 * XIMP Framework Eunit tests.
 *
 * Tests for XIMP group member management services.
 *
 * @since S60 v4.0
 */
class T_PresenceGroupMemberManagement : public CEUnitTestSuiteClass
    {

public:
    static T_PresenceGroupMemberManagement* NewL();
    virtual ~T_PresenceGroupMemberManagement();


private:
    T_PresenceGroupMemberManagement();
    void ConstructL();

private:    // Test case functions

    void Setup_L();
    void SetupMultiple_L();
    void BindAllL();
    void Teardown();
    
    // destroy test case support things, which cannot
    // be known in Setup method.
    void TeardownTestSupport();
    
    void UnbindAllL();

    void T_SubUnsubGroupContent_Single_L();
    void T_SubUnsubGroupContent_Multi_L();
    void T_SubUnsubGroupContent_One_Multi_L();
    void T_SubUnsubGroupContent_All_Multi_L();
    void T_SubUnsubGroupContent_Altered_Single_L();
    void T_SubUnsubGroupContent_AlteredSame_Single_L();
    void T_UnsubscribeNonSubscribed_L();
    void T_AddGroupMembers_Single_L();
    void T_RemoveGroupMembers_Single_L();
    void T_RemoveGroupMembers_NonSub_Single_L();
    void T_RemoveGroupMembers_NonExist_Single_L();
    void T_UpdateGroupMembers_Single_L();
    void T_UpdateGroupMembers_NonSub_Single_L();
    void T_UpdateGroupMembers_NonExist_Single_L();
    void T_ExpireNonSubscribedData_L();
    void T_ExpireNonSubscribedDataUpdateMultiple_L();

    // variable test method support
    void T_GenericSubAndUnsubL( TXIMPCallBack& aCbFuncL );

    void T_EmptyFunc();
    void T_TestAddMembersL();
    void T_TestAddMembers_NonSub_L();
    void T_TestRemoveMembersL();
    void T_TestRemoveMembers_NonSub_L();
    void T_TestRemoveMembers_NonExisting_L();
    void T_TestUpdateMembers_L();
    void T_TestUpdateMembers_NonSub_L();
    void T_TestUpdateMembers_NonExisting_L();

private:    // Test helpers

    void SetupListenerReqCompleteL( CXIMPTestContextWrapper* aWrapper, TXIMPTestStatusEventTemplate aEventType );
    void SetupListenerReqCompleteAllowEventsL( CXIMPTestContextWrapper* aWrapper, TXIMPTestStatusEventTemplate aEventTypes );

    // sometimes EUnit might eat e.g. an Access violation.
    // Call this method in the test case to break when it happens,
    // hopefully you will find the cause.
    void UseMyOwnExceptionHandler();

private: // Test data

    /**
     * Eunit test case table declaration.
     */
    EUNIT_DECLARE_TEST_TABLE;

    // owned
    CXIMPTestContextWrapperMgr* iWrapperMgr;

    CActiveSchedulerWait* iASWait;

    // current group id
    // owned.
    MXIMPIdentity* iCurrentGrId;

    /**
     * Last received group content event.
     * Owned.
     */
    CPresentityGroupContentEventImp* iLastEvent;

    CXIMPTestListener* iListener2;

    // ETrue, if you want to skip group content subscription in generic
    // sub and unsub
    TBool iSkipSubscribe;

    // EFalse, if you want to skip group content unsubscription
    TBool iSkipUnsubscribe;
    };


#endif      //  T_PRESENCEGROUPMEMBERMANAGEMENT_H



// end of file





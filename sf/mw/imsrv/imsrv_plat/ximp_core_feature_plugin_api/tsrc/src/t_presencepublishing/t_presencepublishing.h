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

#ifndef T_PRESENCEPUBLISHING_H
#define T_PRESENCEPUBLISHING_H

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
class T_PresencePublishing : public CEUnitTestSuiteClass
    {

public:
    static T_PresencePublishing* NewL();
    virtual ~T_PresencePublishing();


private:
    T_PresencePublishing();
    void ConstructL();



private:    // Test case functions

    void Setup_L();
    void SetupMultiple_L();
    void BindL();
    void Bind2L();
    void BindAllL();
    void Teardown();
    void UnbindL();
    void UnbindAllL();


    //------------------------------------------
    //Presence Publish
    //
    void T_Publish_Single_L();

    // Maybe unneeded cases. All events should act likewise and these
    // kind of situations are tested in t_sessionmng already.
    void T_Publish_ErrFromPlg_Leave_L();
    void T_Publish_ErrFromPlg_CompleteWithError_L();


    //------------------------------------------
    //Presence interest registration
    //
    void T_SubscribeOwnPresence_Single_Empty_L();
    void T_SubscribeOwnPresence_Multiple_Empty_L();
    void T_SubscribeOwnPresence_Single_L();
    void T_SubscribeOwnPresence_Publish_Single_L();
    void T_SubscribeOwnPresence_Two_Clients_L();
    void T_ErrFromPlg_LeaveOnSubscribeOwnPresence_L();
    void T_NetworkUnplugError_L();
    void T_NetworkUnplugErrorMultiple_L();

    // Plugin leaves with error, returns error. Same as above.
    //void T_InterestRegistration_ErrorFromPlg_Leave_L();
    //void T_InterestRegistration_ErrorFromPlg_Error_L();

    //------------------------------------------
    // Input data verification tests
    // TODO when the mechanism for
    //
    //   Verify that plugin receives correct input data
    //     - single publish
    //     - single registration
    //     - aggregated registrations
    //   Verify that the test case received correct data (verify plugin output data)
    //     - own presence event
    //     - UpdatePresenceL / presentity event
    //     - filter registration verifications on presentity events

private:    // Test helpers

    void SetupListenerReqCompleteL( CXIMPTestContextWrapper* aWrapper, TXIMPTestStatusEventTemplate aEventType );
    void SetupListenerReqCompleteAllowEventsL( CXIMPTestContextWrapper* aWrapper, TXIMPTestStatusEventTemplate aEventTypes );
    // TODO move these to some TOOL part - could be common
    // among all components

    MPresenceInfo* CreateInfoLC();

private: // Test data

    /**
     * Eunit test case table declaration.
     */
    EUNIT_DECLARE_TEST_TABLE;

    // owned
    CXIMPTestContextWrapperMgr* iWrapperMgr;
    };

#endif      //  T_PRESENCEPUBLISHING_H



// end of file





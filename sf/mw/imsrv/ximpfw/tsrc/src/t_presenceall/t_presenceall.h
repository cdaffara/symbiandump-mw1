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

#ifndef T_PRESENCEALL_H
#define T_PRESENCEALL_H

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
class T_PresenceAll : public CEUnitTestSuiteClass
    {

public:
    static T_PresenceAll* NewL();
    virtual ~T_PresenceAll();


private:
    T_PresenceAll();
    void ConstructL();



private:    // Test case functions

    void Setup_L();
    void SetupMultiple_L();
    void BindL();
    void BindAllL();
    void Teardown();
    void UnbindL();
    void UnbindAllL();

    void T_TMO_demo_Startup_L();
    void T_GrantWithdrawGroup_L();
    
    void T_BindUnbindL();
 
private:    // Test helpers

void SendSrvMsgL(
        const TDesC& aUri, const TDesC& aDispName,
        TInt aMsgType );


// send a faked server-originated message with empty contents
//
void SendSrvMsgL( TInt aMsgType );

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


#endif      //  t_presenceall_H



// end of file





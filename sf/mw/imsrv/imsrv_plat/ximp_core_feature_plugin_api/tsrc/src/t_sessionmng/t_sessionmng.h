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

#ifndef T_SESSIONMNG_H__
#define T_SESSIONMNG_H__

#include <ceunittestsuiteclass.h>
#include <ximpbase.h>

#include "prfwteststatuseventlistener.h"


class CXIMPTestContextWrapperMgr;
class MXIMPContext;

/**
 * XIMP Framework Eunit tests.
 *
 * Tests for XIMP session management services.
 *
 * @since S60 v4.0
 */
class T_SessionMng : public CEUnitTestSuiteClass
    {

public:
    static T_SessionMng* NewL();
    virtual ~T_SessionMng();


private:
    T_SessionMng();
    void ConstructL();



private:    // Test case functions

    void Teardown();
    
    void Setup_L();
    void Setup_2_L();
    void Setup_3_L();

   
    void T_Simple_Bind_Wait_Unbind_L();
    void T_Settings_Bind_Wait_Unbind_L();
    void T_Simple_Bind_Wait_OOM_L();

    void T_Simple_Bind_Wait_Unbind_WithReqCompleteEventFilter_L();
    void T_Simple_Bind_Wait_CloseHandle_L();
    void T_Simple_Bind_Direct_Unbind_L();
    void T_Simple_Bind_Direct_CloseHandle_L();



    void T_ErrFromPlg_LeaveOnOpenSession_L();
    void T_ErrFromPlg_ErrorResultOnOpenSession_L();
    void T_ErrFromPlg_ErrorResultOnCloseSession_L();
    
    void T_ErrFromPlg_InvalidReqIdOnOpenSession_L();
    void T_Multi_Bind_Unbind_WithWaits_L();    
    


private:    // Test helpers

    TXIMPRequestId BindContextToDefaultL( MXIMPContext* aContext );





private: // Test data

    /**
     * Eunit test case table declaration.
     */
    EUNIT_DECLARE_TEST_TABLE;

    /**
     * Context Wrapper Manager
     */
    CXIMPTestContextWrapperMgr* iWrapperMgr;

    };

#endif      //  T_SESSIONMNG_H__



// end of file





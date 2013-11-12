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

#ifndef T_UTILS_H__
#define T_UTILS_H__

#include <ceunittestsuiteclass.h>
#include <ximpbase.h>

#include "prfwteststatuseventlistener.h"
#include "prfwwaitobserver.h"

class CXIMPTestContextWrapperMgr;

/**
 * XIMP Framework Eunit tests.
 *
 * Tests for XIMP session management services.
 *
 * @since S60 v4.0
 */
class T_Utils : public CEUnitTestSuiteClass
    {

public:
    static T_Utils* NewL();
    virtual ~T_Utils();

private:
    T_Utils();
    void ConstructL();

private:    // Test case functions

    void Setup_L();
    void Teardown();

    void T_FileToolTest_L();

    void T_KillServers_L();
    
    void T_ObjectCollection_Unnamed_L();
    void T_ObjectCollection_Named_L();
    void T_RestrictedObjectCollection_L();
    void T_DataSubscriptionState_L();
    
    void T_TXIMPRequestIdBuilder_Test_L();
    void T_TXIMPRequestId_Test_L();

private:    // Test helpers

private: // Test data

    /**
     * Eunit test case table declaration.
     */
    EUNIT_DECLARE_TEST_TABLE;
    };

#endif      //  T_UTILS_H__

// end of file





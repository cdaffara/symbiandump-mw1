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

#ifndef T_ROOTSERVICES_H__
#define T_ROOTSERVICES_H__

#include <ceunittestsuiteclass.h>
#include <ximpbase.h>

class MXIMPProtocolInfo;
class MXIMPClient;
class MXIMPContext;
class CUSContextObserver;


/**
 * XIMP Framework Eunit tests.
 *
 * Tests for XIMP root services.
 *
 * @since S60 v3.2
 */
class T_RootServices : public CEUnitTestSuiteClass
    {
public:
    static T_RootServices* NewL();
    virtual ~T_RootServices();


private:
    T_RootServices();
    void ConstructL();



private:    // Test case functions

    void Setup_L();
    void Teardown();

    void Test_ListProtocolsAndTheirCapabilities_L();


private:    // Test helpers

    void __DumpProtocolListL(
        RXIMPObjOwningPtrArray< MXIMPProtocolInfo >& aProtocolList );



private: // Test data


    /**
     * Eunit test case table declaration.
     */
    EUNIT_DECLARE_TEST_TABLE;


    };


#endif      //  T_ROOTSERVICES_H__



// end of file





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

#ifndef T_EUNITTEMPLATE_H
#define T_EUNITTEMPLATE_H

#include <ceunittestsuiteclass.h>





/**
 * XIMP Framework Eunit tests.
 *
 * TODO: Add test description here.
 *
 * @since S60 v4.0
 */
class T_EunitTemplate : public CEUnitTestSuiteClass
    {

public:

    /**  ?description */
    //enum ?declaration

    /**  ?description */
    //typedef ?declaration

    static T_EunitTemplate* NewL();
    virtual ~T_EunitTemplate();

private:

    T_EunitTemplate();
    void ConstructL();



private:    // Test case functions

    void Setup_L();
    void Teardown();

    void TestSomething_L();



private:    // Test helpers





private: // Test data


    /**
     * Eunit test case table declaration.
     */
    EUNIT_DECLARE_TEST_TABLE;


    /**
     *
     * Own. / Not own.
     */
    //?type* ?member_name;

    };


#endif      //  T_EUNITTEMPLATE_H



// end of file





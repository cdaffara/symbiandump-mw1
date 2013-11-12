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

#include "t_eunittemplate.h"
#include "eunittools.h"
#include <eunitmacros.h>



// ============================ MEMBER FUNCTIONS ===============================

// CONSTRUCTION
T_EunitTemplate* T_EunitTemplate::NewL()
    {
    T_EunitTemplate* self = new( ELeave ) T_EunitTemplate;
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );

    return self;
    }


T_EunitTemplate::~T_EunitTemplate()
    {
    }


void T_EunitTemplate::ConstructL()
    {
    CEUnitTestSuiteClass::ConstructL();
    }


T_EunitTemplate::T_EunitTemplate()
    {
    }



// ===========================================================================
// TEST CASE SETUP & TEARDOWN METHODS
// ===========================================================================
//
void T_EunitTemplate::Setup_L()
    {
    }



void T_EunitTemplate::Teardown()
    {
    }



// ===========================================================================
// TODO: Test case description & grouping
// ===========================================================================
//
void T_EunitTemplate::TestSomething_L()
    {
    EUNIT_ASSERT( EFalse );
    }




// ===========================================================================
// TEST CASE TABLE
// ===========================================================================
//
EUNIT_BEGIN_TEST_TABLE(
    T_EunitTemplate,
    "Add test suite description here.",
    "MODULE" )

    PRFW_NOT_DECORATED_TEST(
        "Empty test skeleton",
        "Add tested class name here",
        "Add tested function names here",
        "FUNCTIONALITY ERRORHANDLING BOUNDARY",
        Setup_L,
        TestSomething_L,
        Teardown )


EUNIT_END_TEST_TABLE





// ===========================================================================
// TEST SUITE CONSTRUCTION
// ===========================================================================
//
EXPORT_C MEUnitTest* CreateTestSuiteL()
    {
    return T_EunitTemplate::NewL();
    }


// end of file







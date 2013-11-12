/*
* Copyright (c) 2004 Nokia Corporation and/or its subsidiary(-ies). 
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


//  CLASS HEADER
#include "MT_BSUtil.h"

//  EXTERNAL INCLUDES
#include <EUnitMacros.h>
#include "bsutil.h"
#include <HWRMPowerStateSDKPSKeys.h>

//  INTERNAL INCLUDES

// ============================= LOCAL FUNCTIONS ===============================


// CONSTRUCTION
MT_BSUtil* MT_BSUtil::NewL()
    {
    MT_BSUtil* self = MT_BSUtil::NewLC();
    CleanupStack::Pop();

    return self;
    }

MT_BSUtil* MT_BSUtil::NewLC()
    {
    MT_BSUtil* self = new( ELeave ) MT_BSUtil();
    CleanupStack::PushL( self );

    self->ConstructL();

    return self;
    }

// Destructor (virtual by CBase)
MT_BSUtil::~MT_BSUtil()
    {
    }

// Default constructor
MT_BSUtil::MT_BSUtil()
    {
    }

// Second phase construct
void MT_BSUtil::ConstructL()
    {
    // The ConstructL from the base class CEUnitTestSuiteClass must be called.
    // It generates the test case table.
    CEUnitTestSuiteClass::ConstructL();
    }

//  METHODS



void MT_BSUtil::SetupL(  )
    {
    }

void MT_BSUtil::Teardown(  )
    {
    }

void MT_BSUtil::MT_CBSUtil_NewLL(  )
    {
    MT_BSUtil* util = MT_BSUtil::NewL();
    EUNIT_ASSERT_DESC(util, "MT_BSUtil::NewL - failed");
    delete util;
    }

void MT_BSUtil::MT_CBSUtil_NewLCL(  )
    {
    CBSUtil* util = CBSUtil::NewLC();
    EUNIT_ASSERT_DESC(util, "CBSUtil::NewLC - failed");
    CleanupStack::PopAndDestroy( util );
    }

void MT_BSUtil::MT_CBSUtil_GetBatteryCapacityL()
    {
    CBSUtil* util = CBSUtil::NewL();
    EUNIT_ASSERT_DESC(util, "CBSUtil::NewL - failed");
    TUint8 capacity( util->GetBatteryCapacity() );
    EUNIT_PRINT( _L("CBSUtil::GetBatteryCapacity returned %d"), capacity );
    delete util;
    }

//  TEST TABLE

EUNIT_BEGIN_TEST_TABLE(
    MT_BSUtil,
    "Add test suite description here.",
    "MODULE" )

EUNIT_TEST(
    "NewL - test ",
    "CBSUtil",
    "NewL",
    "FUNCTIONALITY",
    SetupL, MT_CBSUtil_NewLL, Teardown)

EUNIT_TEST(
    "NewLC - test ",
    "CBSUtil",
    "NewLC",
    "FUNCTIONALITY",
    SetupL, MT_CBSUtil_NewLCL, Teardown)

EUNIT_TEST(
    "GetBatteryCapacity - test ",
    "CBSUtil",
    "GetBatteryCapacity",
    "FUNCTIONALITY",
    SetupL, MT_CBSUtil_GetBatteryCapacityL, Teardown)


EUNIT_END_TEST_TABLE

//  END OF FILE

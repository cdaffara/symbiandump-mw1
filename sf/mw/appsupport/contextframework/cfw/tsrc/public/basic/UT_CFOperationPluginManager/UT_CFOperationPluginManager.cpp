/*
* Copyright (c) 2006-2010 Nokia Corporation and/or its subsidiary(-ies). 
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
#include "UT_CFOperationPluginManager.h"

//  EXTERNAL INCLUDES
#include <EUnitMacros.h>
#include <EUnitDecorators.h>


//  INTERNAL INCLUDES
#include "cfoperationpluginmanager.h"

// CONSTRUCTION
UT_CFOperationPluginManager* UT_CFOperationPluginManager::NewL()
    {
    UT_CFOperationPluginManager* self = UT_CFOperationPluginManager::NewLC();
    CleanupStack::Pop();

    return self;
    }

UT_CFOperationPluginManager* UT_CFOperationPluginManager::NewLC()
    {
    UT_CFOperationPluginManager* self = new( ELeave ) UT_CFOperationPluginManager();
    CleanupStack::PushL( self );

    self->ConstructL();

    return self;
    }

// Destructor (virtual by CBase)
UT_CFOperationPluginManager::~UT_CFOperationPluginManager()
    {
    }

// Default constructor
UT_CFOperationPluginManager::UT_CFOperationPluginManager()
    {
    }

// Second phase construct
void UT_CFOperationPluginManager::ConstructL()
    {
    // The ConstructL from the base class CEUnitTestSuiteClass must be called.
    // It generates the test case table.
    CEUnitTestSuiteClass::ConstructL();
    }

//  METHODS


void UT_CFOperationPluginManager::SetupL(  )
    {
    iCCFOperationPluginManager = NULL;
    }
    

void UT_CFOperationPluginManager::Teardown(  )
    {
    delete iCCFOperationPluginManager; 
    iCCFOperationPluginManager = NULL; 

    iArray.Close();
    }

//-----------------------------------------------------------------------------
// UT_CFOperationPluginManager class unit tests
//-----------------------------------------------------------------------------
//
void UT_CFOperationPluginManager::UT_CFOperationPluginManager_NewLL(  )
    {
    CCFOperationPluginManager* instance =
    CCFOperationPluginManager::NewL( *this );
    CleanupStack::PushL( instance );

    EUNIT_ASSERT_DESC( instance, "CCFOperationPluginManager instance not created!" );
    
    CleanupStack::PopAndDestroy( instance );
    }

void UT_CFOperationPluginManager::UT_CFOperationPluginManager_NewLCL(  )
    {
    CCFOperationPluginManager* instance =
    CCFOperationPluginManager::NewLC( *this );

    EUNIT_ASSERT_DESC( instance, "CCFOperationPluginManager instance not created!" );
    
    CleanupStack::PopAndDestroy( instance );
    }


//  TEST TABLE
EUNIT_BEGIN_TEST_TABLE(
    UT_CFOperationPluginManager,
    "Add test suite description here.",
    "UNIT" )

EUNIT_TEST(
	    "NewLC - test",
	    "CCFOperationPluginManager",
	    "NewLC",
	    "FUNCTIONALITY",
	    SetupL, UT_CFOperationPluginManager_NewLL, Teardown)    
    
EUNIT_TEST(
	    "NewL - test",
	    "CCFOperationPluginManager",
	    "NewL",
	    "FUNCTIONALITY",
	    SetupL, UT_CFOperationPluginManager_NewLCL, Teardown)    

EUNIT_END_TEST_TABLE

//  END OF FILE

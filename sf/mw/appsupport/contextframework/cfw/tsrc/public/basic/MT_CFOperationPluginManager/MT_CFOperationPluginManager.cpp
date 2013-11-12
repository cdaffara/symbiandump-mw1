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
#include "MT_CFOperationPluginManager.h"

//  EXTERNAL INCLUDES
#include <EUnitMacros.h>
#include <EUnitDecorators.h>
#include <ECom/ECom.h>
#include "cfoperationpluginmanager.h"

//  INTERNAL INCLUDES
#include "cfscripthandler.h"

// CONSTRUCTION
MT_CFOperationPluginManager* MT_CFOperationPluginManager::NewL()
    {
    MT_CFOperationPluginManager* self = MT_CFOperationPluginManager::NewLC();
    CleanupStack::Pop();

    return self;
    }

MT_CFOperationPluginManager* MT_CFOperationPluginManager::NewLC()
    {
    MT_CFOperationPluginManager* self = new( ELeave ) MT_CFOperationPluginManager();
    CleanupStack::PushL( self );

    self->ConstructL();

    return self;
    }

// Destructor (virtual by CBase)
MT_CFOperationPluginManager::~MT_CFOperationPluginManager()
    {
    }

// Default constructor
MT_CFOperationPluginManager::MT_CFOperationPluginManager()
    {
    }

// Second phase construct
void MT_CFOperationPluginManager::ConstructL()
    {
    // The ConstructL from the base class CEUnitTestSuiteClass must be called.
    // It generates the test case table.
    CEUnitTestSuiteClass::ConstructL();
    }

//  METHODS


void MT_CFOperationPluginManager::SetupL(  )
    {
    iCCFOperationPluginManager = CCFOperationPluginManager::NewL( *this );
    }

void MT_CFOperationPluginManager::SetupEmptyL(  )
	{
	
	}


void MT_CFOperationPluginManager::Teardown(  )
    {
    delete iCCFOperationPluginManager; 
    iCCFOperationPluginManager = NULL; 
    
    iArray.Close();

#ifdef __WINS__
     REComSession::FinalClose();
#endif
    }



//-----------------------------------------------------------------------------
// MT_CFOperationPluginManager::T_CCFOperationPluginManager_NewLL
//-----------------------------------------------------------------------------
//
void MT_CFOperationPluginManager::T_CCFOperationPluginManager_NewLL(  )
    {
    CCFOperationPluginManager* instance =
    CCFOperationPluginManager::NewL( *this );
    CleanupStack::PushL( instance );

    EUNIT_ASSERT_DESC( instance, "CCFOperationPluginManager instance not created!" );
    
    CleanupStack::PopAndDestroy( instance );
    }

//-----------------------------------------------------------------------------
// MT_CFOperationPluginManager::T_CCFOperationPluginManager_NewLL
//-----------------------------------------------------------------------------
//
void MT_CFOperationPluginManager::T_CCFOperationPluginManager_NewLCL(  )
    {
    CCFOperationPluginManager* instance =
    CCFOperationPluginManager::NewLC( *this );

    EUNIT_ASSERT_DESC( instance, "CCFOperationPluginManager instance not created!" );
    
    CleanupStack::PopAndDestroy( instance );
    }

//-----------------------------------------------------------------------------
// MT_CFOperationPluginManager::T_CCFOperationPluginManager_InitializePhaseL1L
//-----------------------------------------------------------------------------
//
void MT_CFOperationPluginManager::T_CCFOperationPluginManager_InitializePhaseL1L(  )
    {
    // Activate checks
    //TInt err = RProperty::Set( KContextSourceTestCategory, KDisableChecks, 1 );
    
    // Load only first phase plug-ins (TestContextSourcePlugin)
    MCFStarterObserver* instance =
        static_cast<MCFStarterObserver*>( iCCFOperationPluginManager );
    instance->InitializePhaseL( CCFPhaseBase::ECFDeviceStarting );
    }

//-----------------------------------------------------------------------------
// MT_CFOperationPluginManager::T_CCFOperationPluginManager_InitializePhaseL2L
//-----------------------------------------------------------------------------
//
void MT_CFOperationPluginManager::T_CCFOperationPluginManager_InitializePhaseL2L(  )
    {
    // Load all phases
    MCFStarterObserver* instance =
        static_cast<MCFStarterObserver*>( iCCFOperationPluginManager );
    instance->InitializePhaseL( CCFPhaseBase::ECFDeviceStarting );
    instance->InitializePhaseL( CCFPhaseBase::ECFDeviceStarted );
    instance->InitializePhaseL( CCFPhaseBase::ECFCfwReady );
    }


//  TEST TABLE
EUNIT_BEGIN_TEST_TABLE(
    MT_CFOperationPluginManager,
    "Add test suite description here.",
    "MODULE" )

EUNIT_TEST(
    "NewL - test ",
    "CCFOperationPluginManager",
    "NewL",
    "FUNCTIONALITY",
    SetupEmptyL, T_CCFOperationPluginManager_NewLL, Teardown)

EUNIT_TEST(
    "NewLC - test ",
    "CCFOperationPluginManager",
    "NewLC",
    "FUNCTIONALITY",
    SetupEmptyL, T_CCFOperationPluginManager_NewLCL, Teardown)

EUNIT_TEST(
    "InitializePhaseL 1 - test ",
    "CCFOperationPluginManager",
    "InitializePhaseL",
    "FUNCTIONALITY",
    SetupL, T_CCFOperationPluginManager_InitializePhaseL1L, Teardown)

EUNIT_TEST(
    "InitializePhaseL 2 - test ",
    "CCFOperationPluginManager",
    "InitializePhaseL",
    "FUNCTIONALITY",
    SetupL, T_CCFOperationPluginManager_InitializePhaseL2L, Teardown)    
    
EUNIT_END_TEST_TABLE

//  END OF FILE

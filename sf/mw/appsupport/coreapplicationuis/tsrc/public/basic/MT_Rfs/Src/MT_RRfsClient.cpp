/*
* Copyright (c) 2007 Nokia Corporation and/or its subsidiary(-ies). 
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


// CLASS HEADER
#include "MT_RRfsClient.h"

// EXTERNAL INCLUDES
#include <EUnitMacros.h>
#include <EUnitDecorators.h>

// INTERNAL INCLUDES
#include "rfsClient.h"

namespace
{
	// Filename of a test RFS script
	_LIT( KLitTestScriptFile, "z:\\data\\coreappuistestdata\\rfs_testscript0.txt" );

	_LIT( KLitTestScriptFileNotFound, "z:\\data\\coreappuistestdata\\thisfileisnotfound.yes");
	
	// Simple RFS script, no error
	_LIT( KLitTestScriptDesc, "CD C:\\system\\data\\");
	
	// Simple RFS script with syntax error
	_LIT( KLitTestScriptDescErr, "CerrorD C:\\system\\data\\");
}

// CONSTRUCTION
MT_RRfsClient* MT_RRfsClient::NewL()
    {
    MT_RRfsClient* self = MT_RRfsClient::NewLC();
    CleanupStack::Pop();

    return self;
    }

MT_RRfsClient* MT_RRfsClient::NewLC()
    {
    MT_RRfsClient* self = new( ELeave ) MT_RRfsClient();
    CleanupStack::PushL( self );

    self->ConstructL();

    return self;
    }

// Destructor (virtual by CBase)
MT_RRfsClient::~MT_RRfsClient()
    {
   	iRRfsClient.Close();
    }

// Default constructor
MT_RRfsClient::MT_RRfsClient()
    {
    }

// Second phase construct
void MT_RRfsClient::ConstructL()
    {
    // The ConstructL from the base class CEUnitTestSuiteClass must be called.
    // It generates the test case table.
    CEUnitTestSuiteClass::ConstructL();
    }

//  METHODS

void MT_RRfsClient::SetupEmptyL()
    {
    // Nothing to do
    }

void MT_RRfsClient::SetupL()
    {
    User::LeaveIfError( iRRfsClient.Connect() );
    }

void MT_RRfsClient::TeardownEmpty()
    {
    // Nothing to do
    }

void MT_RRfsClient::Teardown()
    {
    iRRfsClient.Close();
    }
    
void MT_RRfsClient::TestConnectAndCloseL()
    {
    RRfsClient client;    
	TInt err = client.Connect();
	EUNIT_ASSERT( err == KErrNone );
	client.Close();
    }
    
void MT_RRfsClient::TestPerformRfsNormalL()
    {
    
/*    TInt err = iRRfsClient.PerformRfs( ERfsNormal );
    EUNIT_ASSERT_EQUALS( err, KErrNone );
  */  
    }

void MT_RRfsClient::TestPerformRfsDeepL()
    {
    
/*    TInt err = iRRfsClient.PerformRfs( ERfsDeep );
    EUNIT_ASSERT_EQUALS( err, KErrNone );
  */  
    }

void MT_RRfsClient::TestPerformPostEikonRfsL()
    {
    
/*    TInt err = iRRfsClient.PerformPostEikonRfs( ERfsNormal );
    EUNIT_ASSERT_EQUALS( err, KErrNone );
  */	  
    }
    
void MT_RRfsClient::TestRunScriptInDescriptorNoErrorL()
    {
    TInt err = iRRfsClient.RunScriptInDescriptor( KLitTestScriptDesc );    
    EUNIT_ASSERT_EQUALS( err, KErrNone );    
    }

void MT_RRfsClient::TestRunScriptInDescriptorSyntaxErrorL()
    {
    TInt err = iRRfsClient.RunScriptInDescriptor( KLitTestScriptDescErr );    
    EUNIT_ASSERT_EQUALS( err, KErrGeneral );    
    }

void MT_RRfsClient::TestRunScriptInFileNoErrorL()
    {
    TInt err = iRRfsClient.RunScriptInFile( KLitTestScriptFile ); 
    EUNIT_ASSERT_EQUALS( err, KErrNone );    
    }

void MT_RRfsClient::TestRunScriptInFileNotFoundL()
    {
    TInt err = iRRfsClient.RunScriptInFile( KLitTestScriptFileNotFound ); 
    EUNIT_ASSERT_EQUALS( err, KErrNotFound );    
    }    

//  TEST TABLE
EUNIT_BEGIN_TEST_TABLE(
    MT_RRfsClient,
    "RRfsClient test suite",
    "MODULE" )

EUNIT_TEST(
    "Test Connect() and Close()",
    "RRfsClient",
    "Connect, Close",
    "FUNCTIONALITY",
    SetupEmptyL, TestConnectAndCloseL, TeardownEmpty)
    
EUNIT_TEST(
    "Test performing RFS (Normal)",
    "RRfsClient",
    "PerformRfs",
    "FUNCTIONALITY",
    SetupL, TestPerformRfsNormalL, Teardown)

EUNIT_TEST(
    "Test performing RFS (Deep)",
    "RRfsClient",
    "PerformRfs",
    "FUNCTIONALITY",
    SetupL, TestPerformRfsDeepL, Teardown)    
    
EUNIT_TEST(
    "PerformPostEikonRfs - test2",
    "RRfsClient",
    "PerformPostEikonRfs - test2",
    "FUNCTIONALITY",
    SetupL, TestPerformPostEikonRfsL, Teardown)
    
EUNIT_TEST(
    "Test RunScriptInDescriptor with valid script",
    "RRfsClient",
    "RunScriptInDescriptor",
    "FUNCTIONALITY",
    SetupL, TestRunScriptInDescriptorNoErrorL, Teardown)

EUNIT_TEST(
    "Test RunScriptInDescriptor with invalid script",
    "RRfsClient",
    "RunScriptInDescriptor",
    "ERRORHANDLING",
    SetupL, TestRunScriptInDescriptorSyntaxErrorL, Teardown)
    
EUNIT_TEST(
    "RunScriptInFile - test4",
    "RRfsClient",
    "RunScriptInFile - test4",
    "FUNCTIONALITY",
    SetupL, TestRunScriptInFileNoErrorL, Teardown)

EUNIT_TEST(
    "RunScriptInFile - test4",
    "RRfsClient",
    "RunScriptInFile - test4",
    "ERRORHANDLING",
    SetupL, TestRunScriptInFileNotFoundL, Teardown)       

EUNIT_END_TEST_TABLE

//  END OF FILE

/*
* Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description: This file contains testclass implementation.
*
*/

// INCLUDE FILES
#include <StifTestInterface.h>
#include "RfsTestModule.h"
#include <SettingServerClient.h>
#include <e32svr.h>
#include <StifParser.h>


// EXTERNAL DATA STRUCTURES
//extern  ?external_data;

// EXTERNAL FUNCTION PROTOTYPES  
//extern ?external_function( ?arg_type,?arg_type );

// CONSTANTS
//const ?type ?constant_var = ?constant;
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

// MACROS
//#define ?macro ?macro_def

// LOCAL CONSTANTS AND MACROS
//const ?type ?constant_var = ?constant;
//#define ?macro_name ?macro_def

// MODULE DATA STRUCTURES
//enum ?declaration
//typedef ?declaration

// LOCAL FUNCTION PROTOTYPES
//?type ?function_name( ?arg_type, ?arg_type );

// FORWARD DECLARATIONS
//class ?FORWARD_CLASSNAME;

// ============================= LOCAL FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// ?function_name ?description.
// ?description
// Returns: ?value_1: ?description
//          ?value_n: ?description_line1
//                    ?description_line2
// -----------------------------------------------------------------------------
//
/*
?type ?function_name(
    ?arg_type arg,  // ?description
    ?arg_type arg)  // ?description
    {

    ?code  // ?comment

    // ?comment
    ?code
    }
*/

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CRfsTestModule::CRfsTestModule
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
CRfsTestModule::CRfsTestModule( 
    CTestModuleIf& aTestModuleIf ):
        CScriptBase( aTestModuleIf )
    {
    }

// -----------------------------------------------------------------------------
// CRfsTestModule::ConstructL
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CRfsTestModule::ConstructL()
    {
    //Read logger settings to check whether test case name is to be
    //appended to log file name.
    RSettingServer settingServer;
    TInt ret = settingServer.Connect();
    if(ret != KErrNone)
        {
        User::Leave(ret);
        }
    // Struct to StifLogger settigs.
    TLoggerSettings loggerSettings; 
    // Parse StifLogger defaults from STIF initialization file.
    ret = settingServer.GetLoggerSettings(loggerSettings);
    if(ret != KErrNone)
        {
        User::Leave(ret);
        } 
    // Close Setting server session
    settingServer.Close();

    TFileName logFileName;
    
    if(loggerSettings.iAddTestCaseTitle)
        {
        TName title;
        TestModuleIf().GetTestCaseTitleL(title);
        logFileName.Format(KRfsTestModuleLogFileWithTitle, &title);
        }
    else
        {
        logFileName.Copy(KRfsTestModuleLogFile);
        }

    iLog = CStifLogger::NewL( KRfsTestModuleLogPath, 
                          logFileName,
                          CStifLogger::ETxt,
                          CStifLogger::EFile,
                          EFalse );
    
    SendTestClassVersion();
    }

// -----------------------------------------------------------------------------
// CRfsTestModule::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CRfsTestModule* CRfsTestModule::NewL( 
    CTestModuleIf& aTestModuleIf )
    {
    CRfsTestModule* self = new (ELeave) CRfsTestModule( aTestModuleIf );

    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop();

    return self;

    }

void CRfsTestModule::Delete() 
    {

    }

// Destructor
CRfsTestModule::~CRfsTestModule()
    { 

    // Delete resources allocated from test methods
    Delete();

    // Delete logger
    delete iLog; 

    }

//-----------------------------------------------------------------------------
// CRfsTestModule::SendTestClassVersion
// Method used to send version of test class
//-----------------------------------------------------------------------------
//
void CRfsTestModule::SendTestClassVersion()
	{
	TVersion moduleVersion;
	moduleVersion.iMajor = TEST_CLASS_VERSION_MAJOR;
	moduleVersion.iMinor = TEST_CLASS_VERSION_MINOR;
	moduleVersion.iBuild = TEST_CLASS_VERSION_BUILD;
	
	TFileName moduleName;
	moduleName = _L("RfsTestModule.dll");

	TBool newVersionOfMethod = ETrue;
	TestModuleIf().SendTestModuleVersion(moduleVersion, moduleName, newVersionOfMethod);
	}

// ========================== OTHER EXPORTED FUNCTIONS =========================

// -----------------------------------------------------------------------------
// LibEntryL is a polymorphic Dll entry point.
// Returns: CScriptBase: New CScriptBase derived object
// -----------------------------------------------------------------------------
//
EXPORT_C CScriptBase* LibEntryL( 
    CTestModuleIf& aTestModuleIf ) // Backpointer to STIF Test Framework
    {

    return ( CScriptBase* ) CRfsTestModule::NewL( aTestModuleIf );

    }



// -----------------------------------------------------------------------------
// CRfsTestModule::RunMethodL
// Run specified method. Contains also table of test mothods and their names.
// -----------------------------------------------------------------------------
//
TInt CRfsTestModule::RunMethodL( 
    CStifItemParser& aItem ) 
    {

    static TStifFunctionInfo const KFunctions[] =
        {  
        // Copy this line for every implemented function.
        // First string is the function name used in TestScripter script file.
        // Second is the actual implementation member function. 
        //ENTRY( "Example", CRfsTestModule::ExampleL ),
                
        // CRfsHandler test cases
        ENTRY( "TestCRfsHandlerL", CRfsTestModule::TestCRfsHandlerL ),
        ENTRY( "SetupL", CRfsTestModule::SetupL ),
        ENTRY( "Teardown", CRfsTestModule::Teardown ),
        ENTRY( "SpecialSetupL", CRfsTestModule::SpecialSetupL ),
        ENTRY( "SpecialTeardown", CRfsTestModule::SpecialTeardown ),
        ENTRY( "TestActivateRfsLL", CRfsTestModule::TestActivateRfsLL ),
        ENTRY( "TestCancelL", CRfsTestModule::TestCancelL ),
        
        // RRfsClient test cases
        ENTRY( "RRfsClientSetupL", CRfsTestModule::RRfsClientSetupL ),
        ENTRY( "RRfsClientTeardown", CRfsTestModule::RRfsClientTeardown ),
        ENTRY( "TestConnectAndCloseL", CRfsTestModule::TestConnectAndCloseL ),
        ENTRY( "TestPerformRfsNormalL", CRfsTestModule::TestPerformRfsNormalL ),
        ENTRY( "TestPerformRfsDeepL", CRfsTestModule::TestPerformRfsDeepL ),
        ENTRY( "TestPerformPostEikonRfsL", CRfsTestModule::TestPerformPostEikonRfsL ), 
        ENTRY( "TestPerformPostEikonRfsDeepL", CRfsTestModule::TestPerformPostEikonRfsDeepL ),
        ENTRY( "TestRunScriptInDescriptorNoErrorL", CRfsTestModule::TestRunScriptInDescriptorNoErrorL ),        
        ENTRY( "TestRunScriptInDescriptorSyntaxErrorL", CRfsTestModule::TestRunScriptInDescriptorSyntaxErrorL ),
        ENTRY( "TestRunScriptInFileNoErrorL", CRfsTestModule::TestRunScriptInFileNoErrorL ),
        ENTRY( "TestRunScriptInFileNotFoundL", CRfsTestModule::TestRunScriptInFileNotFoundL ),
        
        //ADD NEW ENTRY HERE
        // [test cases entries] - Do not remove

        };

    const TInt count = sizeof( KFunctions ) / 
                        sizeof( TStifFunctionInfo );

    return RunInternalL( KFunctions, count, aItem );

    }
// Test Methods
TInt CRfsTestModule::SetupL(CStifItemParser& /*aItem*/)
    {
    RDebug::Print(_L("Entered SetupL"));
    iCRfsHandler = new( ELeave ) CRfsHandler;
    STIF_ASSERT_NOT_NULL(iCRfsHandler);
    return KErrNone;
    }
    
TInt CRfsTestModule::Teardown(CStifItemParser& /*aItem*/)
    {
    RDebug::Print(_L("Entered Teardown"));
    delete iCRfsHandler; 
    iCRfsHandler = NULL; 
    return KErrNone;
    }

TInt CRfsTestModule::SpecialSetupL(CStifItemParser& /*aItem*/)
    {
    RDebug::Print(_L("Entered SpecialSetupL"));
    iPeriodic = CPeriodic::NewL( CActive::EPriorityStandard );
    STIF_ASSERT_NOT_NULL(iPeriodic);
    return KErrNone;
    }
    

TInt CRfsTestModule::SpecialTeardown( CStifItemParser& /*aItem*/)
    {
    RDebug::Print(_L("Entered SpecialTeardown"));
    if ( iPeriodic )
        {
        iPeriodic->Cancel();
        }
    delete iPeriodic;  
    return KErrNone;
    }    

TInt CRfsTestModule::TestCRfsHandlerL(CStifItemParser& /*aItem*/  )
    {
    RDebug::Print(_L("Entered TestCRfsHandlerL"));
    CRfsHandler* handler = new( ELeave ) CRfsHandler;
    STIF_ASSERT_NOT_NULL(handler);
    delete handler;
    return KErrNone;
    }
    
TInt CRfsTestModule::TestActivateRfsLL( CStifItemParser& /*aItem*/ )
    {
    RDebug::Print(_L("Entered TestActivateRfsLL"));
    if ( !iPeriodic->IsActive() )
        {
        iPeriodic->Start( 4000000, 1000000, TCallBack( SimulateRightSoftkey, static_cast<TAny*>( this ) ) );
        }

    TRAPD(err, iCRfsHandler->ActivateRfsL( ERfsNormal ));
    STIF_ASSERT_EQUALS( KErrNone, err );
    return KErrNone;
    }
    
TInt CRfsTestModule::TestCancelL( CStifItemParser& /*aItem*/ )
    {
    RDebug::Print(_L("Entered TestCancelL"));
    iCRfsHandler->Cancel( );
    return KErrNone;
    }

TInt CRfsTestModule::SimulateRightSoftkey( TAny* aPtr)
    {
    RDebug::Print(_L("Entered SimulateRightSoftkey"));
    CRfsTestModule* instL = static_cast<CRfsTestModule*>( aPtr );
    instL->DoSimulateRightL();
    return KErrNone;
    }
    
void CRfsTestModule::DoSimulateRightL()
    {
    RDebug::Print(_L("Entered DoSimulateRightL"));
    TKeyEvent event;
    event.iCode = EKeyCBA2;
    event.iScanCode = 0;
    event.iRepeats = 0;
    event.iModifiers = 0;

    TWsEvent wsEvent;
    *( wsEvent.Key() ) = event;
    wsEvent.SetType( EEventKey );
    wsEvent.SetTimeNow();

    RDebug::Print(_L("Entered DoSimulateRightL--2"));
    RWsSession* ws = &CCoeEnv::Static()->WsSession();
        
    TInt wgId = ws->GetFocusWindowGroup();
    RDebug::Print(_L("DoSimulateRightL: window focus group id = %d"),wgId);
    
    ws->SendEventToWindowGroup( wgId, wsEvent );
    
    iPeriodic->Cancel();
    RDebug::Print(_L("Leaving DoSimulateRightL"));
    }


//RRfsClient functions
TInt CRfsTestModule::RRfsClientSetupL( CStifItemParser& /*aItem*/ )
    {
    RDebug::Print(_L("Entered RRfsClientSetupL"));
    TInt err = iRRfsClient.Connect();
    STIF_ASSERT_EQUALS( KErrNone, err );
    return KErrNone;
    }

TInt CRfsTestModule::RRfsClientTeardown( CStifItemParser& /*aItem*/ )
    {
    RDebug::Print(_L("Entered RRfsClientTeardown"));
    iRRfsClient.Close();
    return KErrNone;
    }
    
TInt CRfsTestModule::TestConnectAndCloseL( CStifItemParser& /*aItem*/ )
    {
    RDebug::Print(_L("Entered TestConnectAndCloseL"));
    RRfsClient client;    
    TInt err = client.Connect();
    STIF_ASSERT_EQUALS( KErrNone, err );
    client.Close();
    return KErrNone;
    }
    
TInt CRfsTestModule::TestPerformRfsNormalL( CStifItemParser& /*aItem*/ )
    {
    RDebug::Print(_L("Entered TestPerformRfsNormalL"));
    TInt err = iRRfsClient.PerformRfs( ERfsNormal );
    RDebug::Print(_L("TestPerformRfsNormalL: iRRfsClient.PerformRfs returns with err = %d"), err);
    STIF_ASSERT_EQUALS( KErrNone, err );
    return KErrNone;
    }

TInt CRfsTestModule::TestPerformRfsDeepL( CStifItemParser& /*aItem*/ )
    {
    RDebug::Print(_L("Entered TestPerformRfsDeepL"));
    TInt err = iRRfsClient.PerformRfs( ERfsDeep );
    RDebug::Print(_L("TestPerformRfsDeepL: iRRfsClient.PerformRfs returns with err = %d"), err);
    STIF_ASSERT_EQUALS( KErrNone, err );
    return KErrNone;
    }

TInt CRfsTestModule::TestPerformPostEikonRfsL( CStifItemParser& /*aItem*/ )
    {
    RDebug::Print(_L("Entered TestPerformPostEikonRfsL"));
    TInt err = iRRfsClient.PerformPostEikonRfs( ERfsNormal );
    STIF_ASSERT_EQUALS( KErrNone, err );
    return KErrNone;
    }

TInt CRfsTestModule::TestPerformPostEikonRfsDeepL( CStifItemParser& /*aItem*/ )
    {
    RDebug::Print(_L("Entered TestPerformPostEikonRfsDeepL"));
    TInt err = iRRfsClient.PerformPostEikonRfs( ERfsDeep );
    STIF_ASSERT_EQUALS( KErrNone, err );
    return KErrNone;
    }
    
TInt CRfsTestModule::TestRunScriptInDescriptorNoErrorL( CStifItemParser& /*aItem*/ )
    {
    RDebug::Print(_L("Entered TestRunScriptInDescriptorNoErrorL"));
    TInt err = iRRfsClient.RunScriptInDescriptor( KLitTestScriptDesc ); 
    STIF_ASSERT_EQUALS( KErrNone, err );
    return KErrNone;
    }

TInt CRfsTestModule::TestRunScriptInDescriptorSyntaxErrorL( CStifItemParser& /*aItem*/ )
    {
    RDebug::Print(_L("Entered TestRunScriptInDescriptorSyntaxErrorL"));
    TInt err = iRRfsClient.RunScriptInDescriptor( KLitTestScriptDescErr );  
    STIF_ASSERT_EQUALS( KErrGeneral, err );
    return KErrNone;   
    }

TInt CRfsTestModule::TestRunScriptInFileNoErrorL( CStifItemParser& /*aItem*/ )
    {
    RDebug::Print(_L("Entered TestRunScriptInFileNoErrorL"));
    TInt err = iRRfsClient.RunScriptInFile( KLitTestScriptFile ); 
    RDebug::Print(_L("TestRunScriptInFileNoErrorL: iRRfsClient.RunScriptInFile returns with err = %d"), err);
    STIF_ASSERT_EQUALS( KErrNone, err );
    return KErrNone;   
    }

TInt CRfsTestModule::TestRunScriptInFileNotFoundL( CStifItemParser& /*aItem*/ )
    {
    RDebug::Print(_L("Entered TestRunScriptInFileNotFoundL"));
    TInt err = iRRfsClient.RunScriptInFile( KLitTestScriptFileNotFound );
    RDebug::Print(_L("TestRunScriptInFileNotFoundL: iRRfsClient.RunScriptInFile returns with err = %d"), err);
    STIF_ASSERT_EQUALS( KErrNotFound, err );
    return KErrNone;    
    }    

// End of File

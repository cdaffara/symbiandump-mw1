/*
* Copyright (c) 2010 Nokia Corporation and/or its subsidiary(-ies). 
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
#include "CertRFSPluginTestModule.h"
#include <SettingServerClient.h>
#include <rfsplugin.h>

// EXTERNAL DATA STRUCTURES
//extern  ?external_data;

// EXTERNAL FUNCTION PROTOTYPES  
//extern ?external_function( ?arg_type,?arg_type );

// CONSTANTS
//const ?type ?constant_var = ?constant;

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
// CCertRFSPluginTestModule::CCertRFSPluginTestModule
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
CCertRFSPluginTestModule::CCertRFSPluginTestModule( 
    CTestModuleIf& aTestModuleIf ):
        CScriptBase( aTestModuleIf )
    {
    }

// -----------------------------------------------------------------------------
// CCertRFSPluginTestModule::ConstructL
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CCertRFSPluginTestModule::ConstructL()
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
        logFileName.Format(KCertRFSPluginTestModuleLogFileWithTitle, &title);
        }
    else
        {
        logFileName.Copy(KCertRFSPluginTestModuleLogFile);
        }

    iLog = CStifLogger::NewL( KCertRFSPluginTestModuleLogPath, 
                          logFileName,
                          CStifLogger::ETxt,
                          CStifLogger::EFile,
                          EFalse );
    
    SendTestClassVersion();
    }

// -----------------------------------------------------------------------------
// CCertRFSPluginTestModule::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CCertRFSPluginTestModule* CCertRFSPluginTestModule::NewL( 
    CTestModuleIf& aTestModuleIf )
    {
    CCertRFSPluginTestModule* self = new (ELeave) CCertRFSPluginTestModule( aTestModuleIf );

    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop();

    return self;

    }

// -----------------------------------------------------------------------------
// CCertRFSPluginTestModule::Delete
// Delete here all resources allocated and opened from test methods. 
// Called from destructor. 
// -----------------------------------------------------------------------------
//
void CCertRFSPluginTestModule::Delete() 
    {

    }

// Destructor
CCertRFSPluginTestModule::~CCertRFSPluginTestModule()
    { 

    // Delete resources allocated from test methods
    Delete();

    delete iPlugin;
    REComSession::FinalClose();

    // Delete logger
    delete iLog; 

    }

// -----------------------------------------------------------------------------
// CCertRFSPluginTestModule::RunMethodL
// Run specified method. Contains also table of test mothods and their names.
// -----------------------------------------------------------------------------
//
TInt CCertRFSPluginTestModule::RunMethodL( 
    CStifItemParser& aItem ) 
    {

    static TStifFunctionInfo const KFunctions[] =
        {  
        // Copy this line for every implemented function.
        // First string is the function name used in TestScripter script file.
        // Second is the actual implementation member function. 
                ENTRY( "CreatePlugin", CCertRFSPluginTestModule::CreatePlugin ),
                ENTRY( "DestroyPlugin", CCertRFSPluginTestModule::DestroyPlugin ),
                ENTRY( "RestoreFactorySettings", CCertRFSPluginTestModule::RestoreFactorySettings ),
                ENTRY( "GetScript", CCertRFSPluginTestModule::GetScript ),
                ENTRY( "ExecuteCustomCommand", CCertRFSPluginTestModule::ExecuteCustomCommand ),
        //ADD NEW ENTRY HERE
        // [test cases entries] - Do not remove

        };

    const TInt count = sizeof( KFunctions ) / 
                        sizeof( TStifFunctionInfo );

    return RunInternalL( KFunctions, count, aItem );

    }

TInt CCertRFSPluginTestModule::CreatePlugin(CStifItemParser& /*aItem */)
    {
    STIF_ASSERT_NOT_LEAVES(iPlugin = CRFSPlugin::NewL( TUid::Uid( 0x2000F8AE ) ) );
    STIF_ASSERT_NOT_NULL( iPlugin );
    
    return KErrNone;
    
    }
TInt CCertRFSPluginTestModule::DestroyPlugin(CStifItemParser& /*aItem*/ )
    {
    delete iPlugin;
    iPlugin = NULL;
        
    return KErrNone;
    }

TInt CCertRFSPluginTestModule::RestoreFactorySettings(CStifItemParser& /*aItem*/ )
    {
    STIF_ASSERT_NOT_LEAVES( iPlugin->RestoreFactorySettingsL( ENormalRfs ) );
    STIF_ASSERT_NOT_LEAVES( iPlugin->RestoreFactorySettingsL( EDeepRfs ) );
    STIF_ASSERT_NOT_LEAVES( iPlugin->RestoreFactorySettingsL( EInitRfs ) );
    STIF_ASSERT_NOT_LEAVES( iPlugin->RestoreFactorySettingsL( static_cast<TRfsReason>( 100 ) ) );
    return KErrNone;
    }

TInt CCertRFSPluginTestModule::GetScript(CStifItemParser& /*aItem*/ )
    {
    TBuf<256> path;
     //TBuf<256> buf;
     //buf.Copy(KExpectedScript);
     STIF_ASSERT_NOT_LEAVES( iPlugin->GetScriptL( ENormalRfs, path ) );
     RDebug::Print(_L( "Script = '%S'"), &path );
     //STIF_ASSERT_EQUALS( buf, path );

     STIF_ASSERT_NOT_LEAVES( iPlugin->GetScriptL( EDeepRfs, path ) );
     RDebug::Print(_L( "Script = '%S'"), &path );
     //STIF_ASSERT_EQUALS( buf, path );

     STIF_ASSERT_NOT_LEAVES( iPlugin->GetScriptL( EInitRfs, path ) );
     RDebug::Print(_L( "Script = '%S'"), &path );
     //STIF_ASSERT_EQUALS( buf, path );

     STIF_ASSERT_NOT_LEAVES( iPlugin->GetScriptL( static_cast<TRfsReason>( 100 ), path ) );
     RDebug::Print(_L( "Script = '%S'"), &path );
     //STIF_ASSERT_EQUALS( buf, path );

    return KErrNone;
    }

TInt CCertRFSPluginTestModule::ExecuteCustomCommand(CStifItemParser& /*aItem*/ )
    {
    TBuf<256> param1( KNullDesC );
     STIF_ASSERT_NOT_LEAVES( iPlugin->ExecuteCustomCommandL( ENormalRfs, param1 ) );
     TBuf<1> param2( KNullDesC );
     STIF_ASSERT_NOT_LEAVES( iPlugin->ExecuteCustomCommandL( EDeepRfs, param2 ) );
     STIF_ASSERT_NOT_LEAVES( iPlugin->ExecuteCustomCommandL( EInitRfs, param2 ) );
     STIF_ASSERT_NOT_LEAVES( iPlugin->ExecuteCustomCommandL( static_cast<TRfsReason>( 100 ), param2 ) );
    return KErrNone;
    }

//-----------------------------------------------------------------------------
// CCertRFSPluginTestModule::SendTestClassVersion
// Method used to send version of test class
//-----------------------------------------------------------------------------
//
void CCertRFSPluginTestModule::SendTestClassVersion()
	{
	TVersion moduleVersion;
	moduleVersion.iMajor = TEST_CLASS_VERSION_MAJOR;
	moduleVersion.iMinor = TEST_CLASS_VERSION_MINOR;
	moduleVersion.iBuild = TEST_CLASS_VERSION_BUILD;
	
	TFileName moduleName;
	moduleName = _L("CertRFSPluginTestModule.dll");

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

    return ( CScriptBase* ) CCertRFSPluginTestModule::NewL( aTestModuleIf );

    }

// End of File

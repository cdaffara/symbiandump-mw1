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
#include "DatastoreRFSPluginTestModule.h"
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
// CDatastoreRFSPluginTestModule::CDatastoreRFSPluginTestModule
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
CDatastoreRFSPluginTestModule::CDatastoreRFSPluginTestModule( 
    CTestModuleIf& aTestModuleIf ):
        CScriptBase( aTestModuleIf )
    {
    }

// -----------------------------------------------------------------------------
// CDatastoreRFSPluginTestModule::ConstructL
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CDatastoreRFSPluginTestModule::ConstructL()
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
        logFileName.Format(KDatastoreRFSPluginTestModuleLogFileWithTitle, &title);
        }
    else
        {
        logFileName.Copy(KDatastoreRFSPluginTestModuleLogFile);
        }

    iLog = CStifLogger::NewL( KDatastoreRFSPluginTestModuleLogPath, 
                          logFileName,
                          CStifLogger::ETxt,
                          CStifLogger::EFile,
                          EFalse );
    
    SendTestClassVersion();
    }

// -----------------------------------------------------------------------------
// CDatastoreRFSPluginTestModule::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CDatastoreRFSPluginTestModule* CDatastoreRFSPluginTestModule::NewL( 
    CTestModuleIf& aTestModuleIf )
    {
    CDatastoreRFSPluginTestModule* self = new (ELeave) CDatastoreRFSPluginTestModule( aTestModuleIf );

    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop();

    return self;

    }

// -----------------------------------------------------------------------------
// CDatastoreRFSPluginTestModule::Delete
// Delete here all resources allocated and opened from test methods. 
// Called from destructor. 
// -----------------------------------------------------------------------------
//
void CDatastoreRFSPluginTestModule::Delete() 
    {

    }

// Destructor
CDatastoreRFSPluginTestModule::~CDatastoreRFSPluginTestModule()
    { 

    // Delete resources allocated from test methods
    Delete();

    delete iPlugin;
    REComSession::FinalClose();
    
    // Delete logger
    delete iLog; 

    }

//-----------------------------------------------------------------------------
// CDatastoreRFSPluginTestModule::SendTestClassVersion
// Method used to send version of test class
//-----------------------------------------------------------------------------
//
void CDatastoreRFSPluginTestModule::SendTestClassVersion()
	{
	TVersion moduleVersion;
	moduleVersion.iMajor = TEST_CLASS_VERSION_MAJOR;
	moduleVersion.iMinor = TEST_CLASS_VERSION_MINOR;
	moduleVersion.iBuild = TEST_CLASS_VERSION_BUILD;
	
	TFileName moduleName;
	moduleName = _L("DatastoreRFSPluginTestModule.dll");

	TBool newVersionOfMethod = ETrue;
	TestModuleIf().SendTestModuleVersion(moduleVersion, moduleName, newVersionOfMethod);
	}

// -----------------------------------------------------------------------------
// CDatastoreRFSPluginTestModule::RunMethodL
// Run specified method. Contains also table of test mothods and their names.
// -----------------------------------------------------------------------------
//
TInt CDatastoreRFSPluginTestModule::RunMethodL( 
    CStifItemParser& aItem ) 
    {

    static TStifFunctionInfo const KFunctions[] =
        {  
        // Copy this line for every implemented function.
        // First string is the function name used in TestScripter script file.
        // Second is the actual implementation member function. 
        ENTRY( "CreatePlugin", CDatastoreRFSPluginTestModule::CreatePlugin ),
        ENTRY( "DestroyPlugin", CDatastoreRFSPluginTestModule::DestroyPlugin ),
        ENTRY( "RestoreFactorySettings", CDatastoreRFSPluginTestModule::RestoreFactorySettings ),
        ENTRY( "GetScript", CDatastoreRFSPluginTestModule::GetScript ),
        ENTRY( "ExecuteCustomCommand", CDatastoreRFSPluginTestModule::ExecuteCustomCommand ),
        //ADD NEW ENTRY HERE
        // [test cases entries] - Do not remove

        };

    const TInt count = sizeof( KFunctions ) / 
                        sizeof( TStifFunctionInfo );

    return RunInternalL( KFunctions, count, aItem );

    }

TInt CDatastoreRFSPluginTestModule::CreatePlugin(CStifItemParser& /*aItem */)
    {
    STIF_ASSERT_NOT_LEAVES(iPlugin = CRFSPlugin::NewL( TUid::Uid( 0x2000F8B7 ) ) );
    STIF_ASSERT_NOT_NULL( iPlugin );
    
    return KErrNone;
    
    }
TInt CDatastoreRFSPluginTestModule::DestroyPlugin(CStifItemParser& /*aItem*/ )
    {
    delete iPlugin;
    iPlugin = NULL;
        
    return KErrNone;
    }

TInt CDatastoreRFSPluginTestModule::RestoreFactorySettings(CStifItemParser& /*aItem*/ )
    {
    STIF_ASSERT_NOT_LEAVES( iPlugin->RestoreFactorySettingsL( ENormalRfs ) );
    STIF_ASSERT_NOT_LEAVES( iPlugin->RestoreFactorySettingsL( EDeepRfs ) );
    STIF_ASSERT_NOT_LEAVES( iPlugin->RestoreFactorySettingsL( EInitRfs ) );
    STIF_ASSERT_NOT_LEAVES( iPlugin->RestoreFactorySettingsL( static_cast<TRfsReason>( 100 ) ) );
    return KErrNone;
    }

TInt CDatastoreRFSPluginTestModule::GetScript(CStifItemParser& /*aItem*/ )
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

TInt CDatastoreRFSPluginTestModule::ExecuteCustomCommand(CStifItemParser& /*aItem*/ )
    {
    TBuf<256> param1( KNullDesC );
     STIF_ASSERT_NOT_LEAVES( iPlugin->ExecuteCustomCommandL( ENormalRfs, param1 ) );
     TBuf<1> param2( KNullDesC );
     STIF_ASSERT_NOT_LEAVES( iPlugin->ExecuteCustomCommandL( EDeepRfs, param2 ) );
     STIF_ASSERT_NOT_LEAVES( iPlugin->ExecuteCustomCommandL( EInitRfs, param2 ) );
     STIF_ASSERT_NOT_LEAVES( iPlugin->ExecuteCustomCommandL( static_cast<TRfsReason>( 100 ), param2 ) );
    return KErrNone;
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

    return ( CScriptBase* ) CDatastoreRFSPluginTestModule::NewL( aTestModuleIf );

    }

// End of File

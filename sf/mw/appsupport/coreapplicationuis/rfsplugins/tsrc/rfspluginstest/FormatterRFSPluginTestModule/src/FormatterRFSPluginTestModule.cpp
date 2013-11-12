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
#include "FormatterRFSPluginTestModule.h"
#include <SettingServerClient.h>

#include <f32file.h>
#include <rfsplugin.h>

#include "formatterrfsplugincommon.h"
_LIT( KExcludeListPath,"c:\\private\\100059C9\\" );
_LIT( KExcludeListName, "c:\\private\\100059C9\\excludelist.txt" );
_LIT( KExcludeListAppSpecFolder, "c:\\private\\102073ea\\excludes\\" );
_LIT( KExcludeListAppSpecName, "c:\\private\\102073ea\\excludes\\12345678.exc" );
_LIT8( KDefaultExcludeList, "c:\\*.*\n" );
_LIT( KExpectedScript, "2000F8F4:00000001;" );

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
// ---------------------------------------------------------------------------
// CreateExcludeListFileL
// ---------------------------------------------------------------------------
//
static void CreateExcludeListFileL( const TDesC& aFile )
    {
    RFs fs;
    TInt err = fs.Connect();
    RDebug::Print(_L("Failed to connect to file server = %d"),err);
    User::LeaveIfError( err );
    CleanupClosePushL( fs );

    err = fs.MkDirAll(KExcludeListPath);
    if( err != KErrNone && err != KErrAlreadyExists )
        {
        User::Leave( err );
        }

    err = fs.MkDirAll( KExcludeListAppSpecFolder );
    if( err != KErrNone && err != KErrAlreadyExists )
        {
        User::Leave( err );
        }
    RFile file;
    err = file.Replace( fs, aFile, EFileWrite );
    RDebug::Print(_L("Failed to create file = %d"),err);
    User::LeaveIfError( err );
    err = file.Write( KDefaultExcludeList );
    RDebug::Print(_L("Failed to write exclude list = %d"),err);
    User::LeaveIfError( err );

    file.Close();

    CleanupStack::PopAndDestroy( &fs );
    }


// ---------------------------------------------------------------------------
// DeleteExcludeListFileL
// ---------------------------------------------------------------------------
//
static void DeleteExcludeListFileL( const TDesC& aFile )
    {
    RFs fs;
    TInt err = fs.Connect();
    RDebug::Print(_L("Failed to connect to file server = %d"),err);
    User::LeaveIfError( err );
    CleanupClosePushL( fs );

    err = fs.Delete( aFile );

    CleanupStack::PopAndDestroy( &fs );
    }


// ---------------------------------------------------------------------------
// RunSecureFormatterL
// ---------------------------------------------------------------------------
//
static void RunSecureFormatterL()
    {
    RProcess process;
    CleanupClosePushL( process );
    TInt err = process.Create( KSecureFormatter, KNullDesC );
    RDebug::Print(_L("Failed to create process = %d"),err);
    User::LeaveIfError( err );

    TRequestStatus status;
    process.Logon( status );
    process.Resume();
    User::WaitForRequest( status );
    RDebug::Print(_L("secure formatter failed = %d"),err);

    CleanupStack::PopAndDestroy( &process );
    }



// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CFormatterRFSPluginTestModule::CFormatterRFSPluginTestModule
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
CFormatterRFSPluginTestModule::CFormatterRFSPluginTestModule( 
    CTestModuleIf& aTestModuleIf ):
        CScriptBase( aTestModuleIf )
    {
    }

// -----------------------------------------------------------------------------
// CFormatterRFSPluginTestModule::ConstructL
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CFormatterRFSPluginTestModule::ConstructL()
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
        logFileName.Format(KFormatterRFSPluginTestModuleLogFileWithTitle, &title);
        }
    else
        {
        logFileName.Copy(KFormatterRFSPluginTestModuleLogFile);
        }

    iLog = CStifLogger::NewL( KFormatterRFSPluginTestModuleLogPath, 
                          logFileName,
                          CStifLogger::ETxt,
                          CStifLogger::EFile,
                          EFalse );
    
    SendTestClassVersion();
    }

// -----------------------------------------------------------------------------
// CFormatterRFSPluginTestModule::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CFormatterRFSPluginTestModule* CFormatterRFSPluginTestModule::NewL( 
    CTestModuleIf& aTestModuleIf )
    {
    CFormatterRFSPluginTestModule* self = new (ELeave) CFormatterRFSPluginTestModule( aTestModuleIf );

    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop();

    return self;

    }

// Destructor
CFormatterRFSPluginTestModule::~CFormatterRFSPluginTestModule()
    { 

    // Delete resources allocated from test methods
    Delete();

    // Delete logger
    delete iLog; 

    }


void CFormatterRFSPluginTestModule::Delete() 
    {

    }

//-----------------------------------------------------------------------------
// CFormatterRFSPluginTestModule::SendTestClassVersion
// Method used to send version of test class
//-----------------------------------------------------------------------------
//
void CFormatterRFSPluginTestModule::SendTestClassVersion()
	{
	TVersion moduleVersion;
	moduleVersion.iMajor = TEST_CLASS_VERSION_MAJOR;
	moduleVersion.iMinor = TEST_CLASS_VERSION_MINOR;
	moduleVersion.iBuild = TEST_CLASS_VERSION_BUILD;
	
	TFileName moduleName;
	moduleName = _L("FormatterRFSPluginTestModule.dll");

	TBool newVersionOfMethod = ETrue;
	TestModuleIf().SendTestModuleVersion(moduleVersion, moduleName, newVersionOfMethod);
	}


// -----------------------------------------------------------------------------
// CFormatterRFSPluginTestModule::RunMethodL
// Run specified method. Contains also table of test mothods and their names.
// -----------------------------------------------------------------------------
//
TInt CFormatterRFSPluginTestModule::RunMethodL( 
    CStifItemParser& aItem ) 
    {

    static TStifFunctionInfo const KFunctions[] =
        {  
        // Copy this line for every implemented function.
        // First string is the function name used in TestScripter script file.
        // Second is the actual implementation member function. 
        // ENTRY( "Example", CFormatterRFSPluginTestModule::ExampleL ),
        //ADD NEW ENTRY HERE
        ENTRY( "SetupEmptyL", CFormatterRFSPluginTestModule::SetupEmptyL ),
        ENTRY( "SetupConditionalL", CFormatterRFSPluginTestModule::SetupConditionalL ),
        ENTRY( "SetupL", CFormatterRFSPluginTestModule::SetupL ),
        ENTRY( "SetupExcludeListOnlyL", CFormatterRFSPluginTestModule::SetupExcludeListOnlyL ),
        ENTRY( "SetupExcludeListL", CFormatterRFSPluginTestModule::SetupExcludeListL ),
        ENTRY( "SetupAppSpecExcludeListOnlyL", CFormatterRFSPluginTestModule::SetupAppSpecExcludeListOnlyL ),
        ENTRY( "SetupAppSpecExcludeListL", CFormatterRFSPluginTestModule::SetupAppSpecExcludeListL ),
        ENTRY( "Teardown", CFormatterRFSPluginTestModule::Teardown ),
        ENTRY( "TeardownExcludeList", CFormatterRFSPluginTestModule::TeardownExcludeList ),
        ENTRY( "TeardownAppSpecExcludeList", CFormatterRFSPluginTestModule::TeardownAppSpecExcludeList ),
        ENTRY( "TestConstructorL", CFormatterRFSPluginTestModule::TestConstructorL ),
        ENTRY( "TestConstructorNoMemoryL", CFormatterRFSPluginTestModule::TestConstructorNoMemoryL ),
        ENTRY( "TestRestoreConditionalL", CFormatterRFSPluginTestModule::TestRestoreConditionalL ),
        ENTRY( "TestRestoreL", CFormatterRFSPluginTestModule::TestRestoreL ),
        ENTRY( "TestRestoreWithExcludeListL", CFormatterRFSPluginTestModule::TestRestoreWithExcludeListL ),
        ENTRY( "TestRestoreNoMemoryL", CFormatterRFSPluginTestModule::TestRestoreNoMemoryL ),
        ENTRY( "TestGetScriptConditionalL", CFormatterRFSPluginTestModule::TestGetScriptConditionalL ),
        ENTRY( "TestGetScriptL", CFormatterRFSPluginTestModule::TestGetScriptL ),
        ENTRY( "TestGetScriptEmptyL", CFormatterRFSPluginTestModule::TestGetScriptEmptyL ),
        ENTRY( "TestCustomCommandL", CFormatterRFSPluginTestModule::TestCustomCommandL ),
   
        // [test cases entries] - Do not remove

        };

    const TInt count = sizeof( KFunctions ) / 
                        sizeof( TStifFunctionInfo );

    return RunInternalL( KFunctions, count, aItem );

    }

// ==========================TEST METHODS=======================================
// ---------------------------------------------------------------------------
// CFormatterRfsPluginTestSuite::SetupEmptyL
// ---------------------------------------------------------------------------
//
TInt CFormatterRFSPluginTestModule::SetupEmptyL(CStifItemParser& /*aItem*/)
    {
    iTestObj = NULL;
    return KErrNone;
    }


// ---------------------------------------------------------------------------
// CFormatterRfsPluginTestSuite::SetupConditionalL
// ---------------------------------------------------------------------------
//
TInt CFormatterRFSPluginTestModule::SetupConditionalL(CStifItemParser& /*aItem*/)
    {
    AppExcludeListsExistL();
    iTestObj = CRFSPlugin::NewL( TUid::Uid( 0x2001CB74 ) );
    
    return KErrNone;
    }


// ---------------------------------------------------------------------------
// CFormatterRFSPluginTestModule::SetupL
// ---------------------------------------------------------------------------
//
TInt CFormatterRFSPluginTestModule::SetupL(CStifItemParser& aItem)
    {
    SetupEmptyL(aItem);
    iTestObj = CRFSPlugin::NewL( TUid::Uid( 0x2001CB74 ) );
    
    return KErrNone;
    }


// ---------------------------------------------------------------------------
// CFormatterRFSPluginTestModule::SetupExcludeListOnlyL
// ---------------------------------------------------------------------------
//
TInt CFormatterRFSPluginTestModule::SetupExcludeListOnlyL(CStifItemParser& /*aItem*/)
    {
    CreateExcludeListFileL( KExcludeListName );
    return KErrNone;
    }


// ---------------------------------------------------------------------------
// CFormatterRFSPluginTestModule::SetupExcludeListL
// ---------------------------------------------------------------------------
//
TInt CFormatterRFSPluginTestModule::SetupExcludeListL(CStifItemParser& aItem)
    {
    CreateExcludeListFileL( KExcludeListName );
    SetupL(aItem);
    return KErrNone;
    }


// ---------------------------------------------------------------------------
// CFormatterRFSPluginTestModule::SetupAppSpecExcludeListOnlyL
// ---------------------------------------------------------------------------
//
TInt CFormatterRFSPluginTestModule::SetupAppSpecExcludeListOnlyL(CStifItemParser& aItem)
    {
    CreateExcludeListFileL( KExcludeListAppSpecName );
    return KErrNone;
    }


// ---------------------------------------------------------------------------
// CFormatterRFSPluginTestModule::SetupAppSpecExcludeListL
// ---------------------------------------------------------------------------
//
TInt CFormatterRFSPluginTestModule::SetupAppSpecExcludeListL(CStifItemParser& aItem)
    {
    CreateExcludeListFileL( KExcludeListAppSpecName );
    SetupL(aItem);
    return KErrNone;
    }


// ---------------------------------------------------------------------------
// CFormatterRFSPluginTestModule::Teardown
// ---------------------------------------------------------------------------
//
TInt CFormatterRFSPluginTestModule::Teardown(CStifItemParser& /*aItem*/)
    {
    delete iTestObj;
    iTestObj = NULL;
    return KErrNone;
    }


// ---------------------------------------------------------------------------
// CFormatterRFSPluginTestModule::TeardownExcludeList
// ---------------------------------------------------------------------------
//
TInt CFormatterRFSPluginTestModule::TeardownExcludeList(CStifItemParser& aItem)
    {
    Teardown(aItem);
    TRAP_IGNORE( DeleteExcludeListFileL( KExcludeListName ) );
    return KErrNone;
    }


// ---------------------------------------------------------------------------
// CFormatterRFSPluginTestModule::TeardownAppSpecExcludeList
// ---------------------------------------------------------------------------
//
TInt CFormatterRFSPluginTestModule::TeardownAppSpecExcludeList(CStifItemParser& aItem)
    {
    Teardown(aItem);
    TRAP_IGNORE( DeleteExcludeListFileL( KExcludeListAppSpecName ) );
    
    return KErrNone;
    }


// ---------------------------------------------------------------------------
// CFormatterRFSPluginTestModule::TestConstructorL
// ---------------------------------------------------------------------------
//
TInt CFormatterRFSPluginTestModule::TestConstructorL(CStifItemParser& /*aItem*/)
    {
    STIF_ASSERT_NOT_LEAVES( iTestObj = CRFSPlugin::NewL( TUid::Uid( 0x2001CB74 ) ) );
    STIF_ASSERT_NOT_NULL( iTestObj );
    //delete iTestObj;
    //iTestObj = NULL;
    
    return KErrNone;
    }


// ---------------------------------------------------------------------------
// CFormatterRFSPluginTestModule::TestConstructorNoMemoryL
// ---------------------------------------------------------------------------
//
TInt CFormatterRFSPluginTestModule::TestConstructorNoMemoryL(CStifItemParser& /*aItem*/)
    {
    STIF_ASSERT_LEAVES_WITH( KErrNoMemory, 
        iTestObj = CRFSPlugin::NewL( TUid::Uid( 0x2001CB74 ) ));
    //EUNIT_ASSERT( !iTestObj );
    STIF_ASSERT_NULL(iTestObj);
    return KErrNone;
    }


// ---------------------------------------------------------------------------
// CFormatterRFSPluginTestModule::TestRestoreConditionalL
// ---------------------------------------------------------------------------
//
TInt CFormatterRFSPluginTestModule::TestRestoreConditionalL(CStifItemParser& aItem)
    {
    if( iAppSpecificFilesExist )
        {
        TestRestoreNoMemoryL(aItem);
        }
    else
        {
        TestRestoreL(aItem);
        }
    return KErrNone;
    }


// ---------------------------------------------------------------------------
// CFormatterRFSPluginTestModule::TestRestoreL
// ---------------------------------------------------------------------------
//
TInt CFormatterRFSPluginTestModule::TestRestoreL(CStifItemParser& /*aItem*/)
    {
    STIF_ASSERT_NOT_LEAVES( iTestObj->RestoreFactorySettingsL( ENormalRfs ) );
    STIF_ASSERT_NOT_LEAVES( iTestObj->RestoreFactorySettingsL( EDeepRfs ) );
    STIF_ASSERT_NOT_LEAVES( iTestObj->RestoreFactorySettingsL( EInitRfs ) );
    STIF_ASSERT_NOT_LEAVES( iTestObj->RestoreFactorySettingsL( static_cast<TRfsReason>( 100 ) ) );
    
    return KErrNone;
    }


// ---------------------------------------------------------------------------
// CFormatterRFSPluginTestModule::TestRestoreWithExcludeListL
// ---------------------------------------------------------------------------
//
TInt CFormatterRFSPluginTestModule::TestRestoreWithExcludeListL(CStifItemParser& /*aItem*/)
    {
    STIF_ASSERT_LEAVES_WITH( KErrPermissionDenied, iTestObj->RestoreFactorySettingsL( ENormalRfs ));
    STIF_ASSERT_LEAVES_WITH( KErrPermissionDenied, iTestObj->RestoreFactorySettingsL( EDeepRfs ));
    STIF_ASSERT_LEAVES_WITH( KErrPermissionDenied, iTestObj->RestoreFactorySettingsL( EInitRfs ));
    STIF_ASSERT_LEAVES_WITH( KErrPermissionDenied, iTestObj->RestoreFactorySettingsL( static_cast<TRfsReason>( 100 ) ));
    
    return KErrNone;
    }


// ---------------------------------------------------------------------------
// CFormatterRFSPluginTestModule::TestRestoreNoMemoryL
// ---------------------------------------------------------------------------
//
TInt CFormatterRFSPluginTestModule::TestRestoreNoMemoryL(CStifItemParser& /*aItem*/)
    {
    STIF_ASSERT_LEAVES_WITH( KErrNoMemory, iTestObj->RestoreFactorySettingsL( ENormalRfs ));
    STIF_ASSERT_LEAVES_WITH( KErrNoMemory, iTestObj->RestoreFactorySettingsL( EDeepRfs ));
    STIF_ASSERT_LEAVES_WITH( KErrNoMemory, iTestObj->RestoreFactorySettingsL( EInitRfs ));
    STIF_ASSERT_LEAVES_WITH( KErrNoMemory, iTestObj->RestoreFactorySettingsL( static_cast<TRfsReason>( 100 ) ));
    
    return KErrNone;
    }


// ---------------------------------------------------------------------------
// CFormatterRFSPluginTestModule::TestGetScriptConditionalL
// ---------------------------------------------------------------------------
//
TInt CFormatterRFSPluginTestModule::TestGetScriptConditionalL(CStifItemParser& aItem)
    {
    if( iAppSpecificFilesExist )
        {
        TestGetScriptEmptyL(aItem);
        }
    else
        {
        TestGetScriptL(aItem);
        }
    
    return KErrNone;
    }
    
    
// ---------------------------------------------------------------------------
// CFormatterRFSPluginTestModule::TestGetScriptL
// ---------------------------------------------------------------------------
//
TInt CFormatterRFSPluginTestModule::TestGetScriptL(CStifItemParser& /*aItem*/)
    {
    TBuf<256> path;
    TBuf<256> buf;
    buf.Copy(KExpectedScript);
    STIF_ASSERT_NOT_LEAVES( iTestObj->GetScriptL( ENormalRfs, path ) );
    RDebug::Print(_L( "Script = '%S'"), &path );
    STIF_ASSERT_EQUALS( buf, path );

    STIF_ASSERT_NOT_LEAVES( iTestObj->GetScriptL( EDeepRfs, path ) );
    RDebug::Print(_L( "Script = '%S'"), &path );
    STIF_ASSERT_EQUALS( buf, path );

    STIF_ASSERT_NOT_LEAVES( iTestObj->GetScriptL( EInitRfs, path ) );
    RDebug::Print(_L( "Script = '%S'"), &path );
    STIF_ASSERT_EQUALS( buf, path );

    STIF_ASSERT_NOT_LEAVES( iTestObj->GetScriptL( static_cast<TRfsReason>( 100 ), path ) );
    RDebug::Print(_L( "Script = '%S'"), &path );
    STIF_ASSERT_EQUALS( buf, path );
    
    return KErrNone;
    }


// ---------------------------------------------------------------------------
// CFormatterRFSPluginTestModule::TestGetScriptEmptyL
// ---------------------------------------------------------------------------
//
TInt CFormatterRFSPluginTestModule::TestGetScriptEmptyL(CStifItemParser& /*aItem*/)
    {
    TBuf<256> path;
    TBuf<256> buf;
    buf.Copy(KNullDesC);
    
    STIF_ASSERT_NOT_LEAVES( iTestObj->GetScriptL( ENormalRfs, path ) );
    RDebug::Print(_L( "Script = '%S'"), &path );
    STIF_ASSERT_EQUALS( buf, path);

    STIF_ASSERT_NOT_LEAVES( iTestObj->GetScriptL( EDeepRfs, path ) );
    RDebug::Print(_L( "Script = '%S'"), &path );
    STIF_ASSERT_EQUALS( buf, path);

    STIF_ASSERT_NOT_LEAVES( iTestObj->GetScriptL( EInitRfs, path ) );
    RDebug::Print(_L( "Script = '%S'"), &path );
    STIF_ASSERT_EQUALS( buf, path );

    STIF_ASSERT_NOT_LEAVES( iTestObj->GetScriptL( static_cast<TRfsReason>( 100 ), path ) );
    RDebug::Print(_L( "Script = '%S'"), &path );
    STIF_ASSERT_EQUALS( buf, path );
    
    return KErrNone;
    }


// ---------------------------------------------------------------------------
// CFormatterRFSPluginTestModule::TestCustomCommandL
// ---------------------------------------------------------------------------
//
TInt CFormatterRFSPluginTestModule::TestCustomCommandL(CStifItemParser& /*aItem*/)
    {
    TBuf<256> param1( KNullDesC );
    STIF_ASSERT_NOT_LEAVES( iTestObj->ExecuteCustomCommandL( ENormalRfs, param1 ) );
    TBuf<1> param2( KNullDesC );
    STIF_ASSERT_NOT_LEAVES( iTestObj->ExecuteCustomCommandL( EDeepRfs, param2 ) );
    STIF_ASSERT_NOT_LEAVES( iTestObj->ExecuteCustomCommandL( EInitRfs, param2 ) );
    STIF_ASSERT_NOT_LEAVES( iTestObj->ExecuteCustomCommandL( static_cast<TRfsReason>( 100 ), param2 ) );
    
    return KErrNone;
    }


// ---------------------------------------------------------------------------
// AppExcludeListsExistL
// ---------------------------------------------------------------------------
//
void CFormatterRFSPluginTestModule::AppExcludeListsExistL()
    {
    RFs fs;
    TInt err = fs.Connect();
    RDebug::Print(_L("Failed to connect to file server = %d"), err);
    User::LeaveIfError( err );
    CleanupClosePushL( fs );

    HBufC* buf = HBufC::NewLC( KMaxFileName );
    TPtr bufPtr = buf->Des();
    bufPtr.Append( KApplicationExcludeListPath );
    bufPtr.Append( KApplicationExcludeListFileSearchPattern );
    bufPtr[0] = fs.GetSystemDriveChar();

    CDir* dirC = NULL;
    TInt errC = fs.GetDir( bufPtr, KEntryAttNormal, ESortNone, dirC );
    CDir* dirZ = NULL;
    bufPtr[0] = KDefaultRom()[0];
    TInt errZ = fs.GetDir( bufPtr, KEntryAttNormal, ESortNone, dirZ );
    
    if( (errC == KErrNone && dirC->Count() ) || 
        (errZ == KErrNone && dirZ->Count() ) )
        {
        iAppSpecificFilesExist = ETrue;
        }
    else
        {
        iAppSpecificFilesExist = EFalse;
        }
    
    delete dirC;
    delete dirZ;
    CleanupStack::PopAndDestroy( buf );
    CleanupStack::PopAndDestroy( &fs );
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

    return ( CScriptBase* ) CFormatterRFSPluginTestModule::NewL( aTestModuleIf );

    }

// End of File

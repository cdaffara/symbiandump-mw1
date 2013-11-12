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
* Description:  Implementation of CFormatterRfsPluginTestSuite class
*
*/


#include <eunit/eunitmacros.h>
#include <eunit/ceunitalloctestcasedecorator.h>
#include <f32file.h>
#include <rfsplugin.h>

#include "formatterrfsplugintestsuite.h"
#include "trace.h"
#include "formatterrfsplugincommon.h"

_LIT( KExcludeListName, "c:\\private\\100059C9\\excludelist.txt" );
_LIT( KExcludeListAppSpecFolder, "c:\\private\\102073ea\\excludes\\" );
_LIT( KExcludeListAppSpecName, "c:\\private\\102073ea\\excludes\\12345678.exc" );
_LIT8( KDefaultExcludeList, "c:\\*.*\n" );
_LIT( KExpectedScript, "2000F8F4:00000001;" );

// ======== LOCAL FUNCTIONS ========

// ---------------------------------------------------------------------------
// CreateExcludeListFileL
// ---------------------------------------------------------------------------
//
static void CreateExcludeListFileL( const TDesC& aFile )
    {
    FUNC_LOG;

    RFs fs;
    TInt err = fs.Connect();
    ERROR( err, "Failed to connect to file server" );
    User::LeaveIfError( err );
    CleanupClosePushL( fs );

    err = fs.MkDirAll( KExcludeListAppSpecFolder );
    if( err != KErrNone && err != KErrAlreadyExists )
        {
        User::Leave( err );
        }
    RFile file;
    err = file.Replace( fs, aFile, EFileWrite );
    ERROR( err, "Failed to create file" );
    User::LeaveIfError( err );
    err = file.Write( KDefaultExcludeList );
    ERROR( err, "Failed to write exclude list" );
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
    FUNC_LOG;

    RFs fs;
    TInt err = fs.Connect();
    ERROR( err, "Failed to connect to file server" );
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
    FUNC_LOG;

    RProcess process;
    CleanupClosePushL( process );
    TInt err = process.Create( KSecureFormatter, KNullDesC );
    ERROR( err, "Failed to create process" );
    User::LeaveIfError( err );

    TRequestStatus status;
    process.Logon( status );
    process.Resume();
    User::WaitForRequest( status );
    ERROR( status.Int(), "secure formatter failed" );

    CleanupStack::PopAndDestroy( &process );
    }


// ======== MEMBER FUNCTIONS ========

// ---------------------------------------------------------------------------
// CFormatterRfsPluginTestSuite::NewL
// ---------------------------------------------------------------------------
//
CFormatterRfsPluginTestSuite* CFormatterRfsPluginTestSuite::NewL()
    {
    FUNC_LOG;

    CFormatterRfsPluginTestSuite* self = CFormatterRfsPluginTestSuite::NewLC();
    CleanupStack::Pop( self );
    return self;
    }


// ---------------------------------------------------------------------------
// CFormatterRfsPluginTestSuite::NewLC
// ---------------------------------------------------------------------------
//
CFormatterRfsPluginTestSuite* CFormatterRfsPluginTestSuite::NewLC()
    {
    FUNC_LOG;

    CFormatterRfsPluginTestSuite* self = new( ELeave ) CFormatterRfsPluginTestSuite;
    CleanupStack::PushL( self );
    self->ConstructL();
    return self;
    }


// ---------------------------------------------------------------------------
// CFormatterRfsPluginTestSuite::~CFormatterRfsPluginTestSuite
// ---------------------------------------------------------------------------
//
CFormatterRfsPluginTestSuite::~CFormatterRfsPluginTestSuite()
    {
    FUNC_LOG;
    }


// ---------------------------------------------------------------------------
// CFormatterRfsPluginTestSuite::CFormatterRfsPluginTestSuite
// ---------------------------------------------------------------------------
//
CFormatterRfsPluginTestSuite::CFormatterRfsPluginTestSuite()
    {
    FUNC_LOG;
    }


// ---------------------------------------------------------------------------
// CFormatterRfsPluginTestSuite::ConstructL
// ---------------------------------------------------------------------------
//
void CFormatterRfsPluginTestSuite::ConstructL()
    {
    FUNC_LOG;

    // The ConstructL from the base class CEUnitTestSuiteClass must be called.
    // It generates the test case table.
    CEUnitTestSuiteClass::ConstructL();

    TRAP_IGNORE( RunSecureFormatterL() ); // Run secure formatter once to get recource count initialised
    }


// ---------------------------------------------------------------------------
// CFormatterRfsPluginTestSuite::SetupEmptyL
// ---------------------------------------------------------------------------
//
void CFormatterRfsPluginTestSuite::SetupEmptyL()
    {
    FUNC_LOG;

    iTestObj = NULL;
    }


// ---------------------------------------------------------------------------
// CFormatterRfsPluginTestSuite::SetupConditionalL
// ---------------------------------------------------------------------------
//
void CFormatterRfsPluginTestSuite::SetupConditionalL()
    {
    FUNC_LOG;

    AppExcludeListsExistL();
    iTestObj = CRFSPlugin::NewL( TUid::Uid( 0x2001CB74 ) );
    }


// ---------------------------------------------------------------------------
// CFormatterRfsPluginTestSuite::SetupL
// ---------------------------------------------------------------------------
//
void CFormatterRfsPluginTestSuite::SetupL()
    {
    FUNC_LOG;

    SetupEmptyL();
    iTestObj = CRFSPlugin::NewL( TUid::Uid( 0x2001CB74 ) );
    }


// ---------------------------------------------------------------------------
// CFormatterRfsPluginTestSuite::SetupExcludeListOnlyL
// ---------------------------------------------------------------------------
//
void CFormatterRfsPluginTestSuite::SetupExcludeListOnlyL()
    {
    FUNC_LOG;

    CreateExcludeListFileL( KExcludeListName );
    }


// ---------------------------------------------------------------------------
// CFormatterRfsPluginTestSuite::SetupExcludeListL
// ---------------------------------------------------------------------------
//
void CFormatterRfsPluginTestSuite::SetupExcludeListL()
    {
    FUNC_LOG;

    CreateExcludeListFileL( KExcludeListName );
    SetupL();
    }


// ---------------------------------------------------------------------------
// CFormatterRfsPluginTestSuite::SetupAppSpecExcludeListOnlyL
// ---------------------------------------------------------------------------
//
void CFormatterRfsPluginTestSuite::SetupAppSpecExcludeListOnlyL()
    {
    FUNC_LOG;

    CreateExcludeListFileL( KExcludeListAppSpecName );
    }


// ---------------------------------------------------------------------------
// CFormatterRfsPluginTestSuite::SetupAppSpecExcludeListL
// ---------------------------------------------------------------------------
//
void CFormatterRfsPluginTestSuite::SetupAppSpecExcludeListL()
    {
    FUNC_LOG;

    CreateExcludeListFileL( KExcludeListAppSpecName );
    SetupL();
    }


// ---------------------------------------------------------------------------
// CFormatterRfsPluginTestSuite::Teardown
// ---------------------------------------------------------------------------
//
void CFormatterRfsPluginTestSuite::Teardown()
    {
    FUNC_LOG;

    delete iTestObj;
    iTestObj = NULL;
    }


// ---------------------------------------------------------------------------
// CFormatterRfsPluginTestSuite::TeardownExcludeList
// ---------------------------------------------------------------------------
//
void CFormatterRfsPluginTestSuite::TeardownExcludeList()
    {
    FUNC_LOG;

    Teardown();
    TRAP_IGNORE( DeleteExcludeListFileL( KExcludeListName ) );
    }


// ---------------------------------------------------------------------------
// CFormatterRfsPluginTestSuite::TeardownAppSpecExcludeList
// ---------------------------------------------------------------------------
//
void CFormatterRfsPluginTestSuite::TeardownAppSpecExcludeList()
    {
    FUNC_LOG;

    Teardown();
    TRAP_IGNORE( DeleteExcludeListFileL( KExcludeListAppSpecName ) );
    }


// ---------------------------------------------------------------------------
// CFormatterRfsPluginTestSuite::TestConstructorL
// ---------------------------------------------------------------------------
//
void CFormatterRfsPluginTestSuite::TestConstructorL()
    {
    FUNC_LOG;

    EUNIT_ASSERT_NO_LEAVE( iTestObj = CRFSPlugin::NewL( TUid::Uid( 0x2001CB74 ) ) );
    EUNIT_ASSERT( iTestObj );
    delete iTestObj;
    iTestObj = NULL;
    }


// ---------------------------------------------------------------------------
// CFormatterRfsPluginTestSuite::TestConstructorNoMemoryL
// ---------------------------------------------------------------------------
//
void CFormatterRfsPluginTestSuite::TestConstructorNoMemoryL()
    {
    FUNC_LOG;

    EUNIT_ASSERT_SPECIFIC_LEAVE(
        iTestObj = CRFSPlugin::NewL( TUid::Uid( 0x2001CB74 ) ), KErrNoMemory );
    EUNIT_ASSERT( !iTestObj );
    }


// ---------------------------------------------------------------------------
// CFormatterRfsPluginTestSuite::TestRestoreConditionalL
// ---------------------------------------------------------------------------
//
void CFormatterRfsPluginTestSuite::TestRestoreConditionalL()
    {
    FUNC_LOG;

    if( iAppSpecificFilesExist )
        {
        TestRestoreNoMemoryL();
        }
    else
        {
        TestRestoreL();
        }
    }


// ---------------------------------------------------------------------------
// CFormatterRfsPluginTestSuite::TestRestoreL
// ---------------------------------------------------------------------------
//
void CFormatterRfsPluginTestSuite::TestRestoreL()
    {
    FUNC_LOG;

    EUNIT_ASSERT_NO_LEAVE( iTestObj->RestoreFactorySettingsL( ENormalRfs ) );
    EUNIT_ASSERT_NO_LEAVE( iTestObj->RestoreFactorySettingsL( EDeepRfs ) );
    EUNIT_ASSERT_NO_LEAVE( iTestObj->RestoreFactorySettingsL( EInitRfs ) );
    EUNIT_ASSERT_NO_LEAVE( iTestObj->RestoreFactorySettingsL( static_cast<TRfsReason>( 100 ) ) );
    }


// ---------------------------------------------------------------------------
// CFormatterRfsPluginTestSuite::TestRestoreWithExcludeListL
// ---------------------------------------------------------------------------
//
void CFormatterRfsPluginTestSuite::TestRestoreWithExcludeListL()
    {
    FUNC_LOG;

    EUNIT_ASSERT_SPECIFIC_LEAVE( iTestObj->RestoreFactorySettingsL( ENormalRfs ), KErrPermissionDenied );
    EUNIT_ASSERT_SPECIFIC_LEAVE( iTestObj->RestoreFactorySettingsL( EDeepRfs ), KErrPermissionDenied );
    EUNIT_ASSERT_SPECIFIC_LEAVE( iTestObj->RestoreFactorySettingsL( EInitRfs ), KErrPermissionDenied );
    EUNIT_ASSERT_SPECIFIC_LEAVE( iTestObj->RestoreFactorySettingsL( static_cast<TRfsReason>( 100 ) ), KErrPermissionDenied );
    }


// ---------------------------------------------------------------------------
// CFormatterRfsPluginTestSuite::TestRestoreNoMemoryL
// ---------------------------------------------------------------------------
//
void CFormatterRfsPluginTestSuite::TestRestoreNoMemoryL()
    {
    FUNC_LOG;

    EUNIT_ASSERT_SPECIFIC_LEAVE( iTestObj->RestoreFactorySettingsL( ENormalRfs ), KErrNoMemory );
    EUNIT_ASSERT_SPECIFIC_LEAVE( iTestObj->RestoreFactorySettingsL( EDeepRfs ), KErrNoMemory );
    EUNIT_ASSERT_SPECIFIC_LEAVE( iTestObj->RestoreFactorySettingsL( EInitRfs ), KErrNoMemory );
    EUNIT_ASSERT_SPECIFIC_LEAVE( iTestObj->RestoreFactorySettingsL( static_cast<TRfsReason>( 100 ) ), KErrNoMemory );
    }


// ---------------------------------------------------------------------------
// CFormatterRfsPluginTestSuite::TestGetScriptConditionalL
// ---------------------------------------------------------------------------
//
void CFormatterRfsPluginTestSuite::TestGetScriptConditionalL()
    {
    FUNC_LOG;
    
    if( iAppSpecificFilesExist )
        {
        TestGetScriptEmptyL();
        }
    else
        {
        TestGetScriptL();
        }
    }
    
    
// ---------------------------------------------------------------------------
// CFormatterRfsPluginTestSuite::TestGetScriptL
// ---------------------------------------------------------------------------
//
void CFormatterRfsPluginTestSuite::TestGetScriptL()
    {
    FUNC_LOG;

    TBuf<256> path;
    EUNIT_ASSERT_NO_LEAVE( iTestObj->GetScriptL( ENormalRfs, path ) );
    INFO_1( "Script = '%S'", &path );
    EUNIT_ASSERT_EQUALS( path, KExpectedScript );

    EUNIT_ASSERT_NO_LEAVE( iTestObj->GetScriptL( EDeepRfs, path ) );
    INFO_1( "Script = '%S'", &path );
    EUNIT_ASSERT_EQUALS( path, KExpectedScript );

    EUNIT_ASSERT_NO_LEAVE( iTestObj->GetScriptL( EInitRfs, path ) );
    INFO_1( "Script = '%S'", &path );
    EUNIT_ASSERT_EQUALS( path, KExpectedScript );

    EUNIT_ASSERT_NO_LEAVE( iTestObj->GetScriptL( static_cast<TRfsReason>( 100 ), path ) );
    INFO_1( "Script = '%S'", &path );
    EUNIT_ASSERT_EQUALS( path, KExpectedScript );
    }


// ---------------------------------------------------------------------------
// CFormatterRfsPluginTestSuite::TestGetScriptEmptyL
// ---------------------------------------------------------------------------
//
void CFormatterRfsPluginTestSuite::TestGetScriptEmptyL()
    {
    FUNC_LOG;

    TBuf<256> path;
    EUNIT_ASSERT_NO_LEAVE( iTestObj->GetScriptL( ENormalRfs, path ) );
    INFO_1( "Script = '%S'", &path );
    EUNIT_ASSERT_EQUALS( path, KNullDesC );

    EUNIT_ASSERT_NO_LEAVE( iTestObj->GetScriptL( EDeepRfs, path ) );
    INFO_1( "Script = '%S'", &path );
    EUNIT_ASSERT_EQUALS( path, KNullDesC );

    EUNIT_ASSERT_NO_LEAVE( iTestObj->GetScriptL( EInitRfs, path ) );
    INFO_1( "Script = '%S'", &path );
    EUNIT_ASSERT_EQUALS( path, KNullDesC );

    EUNIT_ASSERT_NO_LEAVE( iTestObj->GetScriptL( static_cast<TRfsReason>( 100 ), path ) );
    INFO_1( "Script = '%S'", &path );
    EUNIT_ASSERT_EQUALS( path, KNullDesC );
    }


// ---------------------------------------------------------------------------
// CFormatterRfsPluginTestSuite::TestCustomCommandL
// ---------------------------------------------------------------------------
//
void CFormatterRfsPluginTestSuite::TestCustomCommandL()
    {
    FUNC_LOG;

    TBuf<256> param1( KNullDesC );
    EUNIT_ASSERT_NO_LEAVE( iTestObj->ExecuteCustomCommandL( ENormalRfs, param1 ) );
    TBuf<1> param2( KNullDesC );
    EUNIT_ASSERT_NO_LEAVE( iTestObj->ExecuteCustomCommandL( EDeepRfs, param2 ) );
    EUNIT_ASSERT_NO_LEAVE( iTestObj->ExecuteCustomCommandL( EInitRfs, param2 ) );
    EUNIT_ASSERT_NO_LEAVE( iTestObj->ExecuteCustomCommandL( static_cast<TRfsReason>( 100 ), param2 ) );
    }


// ---------------------------------------------------------------------------
// AppExcludeListsExistL
// ---------------------------------------------------------------------------
//
void CFormatterRfsPluginTestSuite::AppExcludeListsExistL()
    {
    FUNC_LOG;
    
    RFs fs;
    TInt err = fs.Connect();
    ERROR( err, "Failed to connect to file server" );
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


// ---------------------------------------------------------------------------
// Test table
// ---------------------------------------------------------------------------
//
EUNIT_BEGIN_TEST_TABLE(
    CFormatterRfsPluginTestSuite,
    "Unit tests for CRFSPlugin class",
    "UNIT" )

EUNIT_TEST(
    "Constructor",
    "CRFSPlugin",
    "NewL",
    "FUNCTIONALITY",
    SetupEmptyL, TestConstructorL, Teardown )

EUNIT_TEST(
    "Constructor (with exclude list)",
    "CRFSPlugin",
    "NewL",
    "FUNCTIONALITY",
    SetupExcludeListOnlyL, TestConstructorL, TeardownExcludeList )

EUNIT_TEST(
    "Constructor (with app specific exclude list)",
    "CRFSPlugin",
    "NewL",
    "FUNCTIONALITY",
    SetupAppSpecExcludeListOnlyL, TestConstructorL, TeardownAppSpecExcludeList )

#ifdef __WINS__ // EUNIT_ALLOC_TEST is not working on HW
EUNIT_ALLOC_TEST(
    "Constructor (no memory)",
    "CRFSPlugin",
    "NewL",
    "ERRORHANDLING",
    SetupEmptyL, TestConstructorNoMemoryL, Teardown )

EUNIT_ALLOC_TEST(
    "Constructor (with exclude list, no memory)",
    "CRFSPlugin",
    "NewL",
    "ERRORHANDLING",
    SetupExcludeListOnlyL, TestConstructorNoMemoryL, TeardownExcludeList )

EUNIT_ALLOC_TEST(
    "Constructor (with app specific exclude list, no memory)",
    "CRFSPlugin",
    "NewL",
    "ERRORHANDLING",
    SetupAppSpecExcludeListOnlyL, TestConstructorNoMemoryL, TeardownAppSpecExcludeList )
#endif // __WINS__

EUNIT_TEST(
    "Restore factory settings",
    "CRFSPlugin",
    "RestoreFactorySettingsL",
    "FUNCTIONALITY",
    SetupL, TestRestoreL, Teardown )

#ifdef __WINS__ // EUNIT_ALLOC_TEST is not working on HW
EUNIT_ALLOC_TEST(
    "Restore factory settings (no memory)",
    "CRFSPlugin",
    "RestoreFactorySettingsL",
    "ERRORHANDLING",
    SetupConditionalL, TestRestoreConditionalL, Teardown )
#endif // __WINS__

EUNIT_TEST(
    "Restore factory settings (with exclude list)",
    "CRFSPlugin",
    "RestoreFactorySettingsL",
    "FUNCTIONALITY",
    SetupExcludeListL, TestRestoreL, TeardownExcludeList )

EUNIT_TEST(
    "Restore factory settings (with app specific exclude list)",
    "CRFSPlugin",
    "RestoreFactorySettingsL",
    "FUNCTIONALITY",
    SetupAppSpecExcludeListL, TestRestoreL, TeardownAppSpecExcludeList )

#ifdef __WINS__ // EUNIT_ALLOC_TEST is not working on HW
EUNIT_ALLOC_TEST(
    "Restore factory settings (with exclude list, no memory)",
    "CRFSPlugin",
    "RestoreFactorySettingsL",
    "ERRORHANDLING",
    SetupExcludeListL, TestRestoreNoMemoryL, TeardownExcludeList )

EUNIT_ALLOC_TEST(
    "Restore factory settings (with app specific exclude list, no memory)",
    "CRFSPlugin",
    "RestoreFactorySettingsL",
    "ERRORHANDLING",
    SetupAppSpecExcludeListL, TestRestoreNoMemoryL, TeardownAppSpecExcludeList )
#endif // __WINS__

EUNIT_TEST(
    "Custom command",
    "CRFSPlugin",
    "ExecuteCustomCommandL",
    "FUNCTIONALITY",
    SetupL, TestCustomCommandL, Teardown )

EUNIT_TEST(
    "Custom command (with exclude list)",
    "CRFSPlugin",
    "ExecuteCustomCommandL",
    "FUNCTIONALITY",
    SetupExcludeListL, TestCustomCommandL, TeardownExcludeList )

EUNIT_TEST(
    "Custom command (with app specific exclude list)",
    "CRFSPlugin",
    "ExecuteCustomCommandL",
    "FUNCTIONALITY",
    SetupAppSpecExcludeListL, TestCustomCommandL, TeardownAppSpecExcludeList )

EUNIT_ALLOC_TEST(
    "Custom command (mem failure)",
    "CRFSPlugin",
    "ExecuteCustomCommandL",
    "ERRORHANDLING",
    SetupL, TestCustomCommandL, Teardown )

EUNIT_ALLOC_TEST(
    "Custom command (with exclude list, mem failure)",
    "CRFSPlugin",
    "ExecuteCustomCommandL",
    "ERRORHANDLING",
    SetupExcludeListL, TestCustomCommandL, TeardownExcludeList )

EUNIT_ALLOC_TEST(
    "Custom command (with app specific exclude list, mem failure)",
    "CRFSPlugin",
    "ExecuteCustomCommandL",
    "ERRORHANDLING",
    SetupAppSpecExcludeListL, TestCustomCommandL, TeardownAppSpecExcludeList )

EUNIT_TEST(
    "Script",
    "CRFSPlugin",
    "GetScriptL",
    "FUNCTIONALITY",
    SetupConditionalL, TestGetScriptConditionalL, Teardown )

EUNIT_TEST(
    "Script (with exclude list)",
    "CRFSPlugin",
    "GetScriptL",
    "FUNCTIONALITY",
    SetupExcludeListL, TestGetScriptEmptyL, TeardownExcludeList )

EUNIT_TEST(
    "Script (with app specific exclude list)",
    "CRFSPlugin",
    "GetScriptL",
    "FUNCTIONALITY",
    SetupAppSpecExcludeListL, TestGetScriptEmptyL, TeardownAppSpecExcludeList )

EUNIT_ALLOC_TEST(
    "Script (mem failure)",
    "CRFSPlugin",
    "GetScriptL",
    "ERRORHANDLING",
    SetupConditionalL, TestGetScriptConditionalL, Teardown )

EUNIT_ALLOC_TEST(
    "Script (with exclude list, mem failure)",
    "CRFSPlugin",
    "GetScriptL",
    "ERRORHANDLING",
    SetupExcludeListL, TestGetScriptEmptyL, TeardownExcludeList )

EUNIT_ALLOC_TEST(
    "Script (with app specific exclude list, mem failure)",
    "CRFSPlugin",
    "GetScriptL",
    "ERRORHANDLING",
    SetupAppSpecExcludeListL, TestGetScriptEmptyL, TeardownAppSpecExcludeList )

EUNIT_END_TEST_TABLE

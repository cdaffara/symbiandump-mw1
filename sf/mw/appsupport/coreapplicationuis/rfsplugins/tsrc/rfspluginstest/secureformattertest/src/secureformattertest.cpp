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
* Description:  Implementation of CSecureFormatterTest class
*
*/


#include <stifparser.h>
#include <stiftestinterface.h>

#include "secureformattertest.h"
#include "testdatacreatorapi.h"
#include "trace.h"

// Drive to be cleaned in the tests
_LIT( KRootDir, "d:\\" );
_LIT( KDefaultExcludeListPath,"d:\\private\\100059C9\\" );
// Default exclude list location
_LIT( KDefaultExcludeList, "d:\\private\\100059C9\\excludelist.txt" );
// String passed to secureformatter in case default exclude list not used
_LIT( KNoDefaultList, "-" );
// Application specific exclude list folder
_LIT( KAppSpecificExcludeListFolder, "c:\\private\\102073ea\\excludes\\" );
// Application specific exclude list file name pattern used in tests
_LIT( KAppSpecificExcludeListPattern, "c:\\private\\102073ea\\excludes\\app_spec_*.exc" );
// Application specific exclude list file name 1
_LIT( KAppSpecificExcludeList1, "app_spec_001.exc" );
// Application specific exclude list file name 2
_LIT( KAppSpecificExcludeList2, "app_spec_002.exc" );
// Secure formatter binary name
_LIT( KSecureFormatter, "secureformatter.exe");

// ======== LOCAL FUNCTIONS ========

// ---------------------------------------------------------------------------
// RunSecureFormatterL
// ---------------------------------------------------------------------------
//
static void RunSecureFormatterL( const TDesC& aExcludeList, const TDesC& aDriveToWipe )
    {
    FUNC_LOG;

    HBufC* arg = HBufC::NewLC( aExcludeList.Length() + aDriveToWipe.Length() + 1 );
    TPtr argPtr = arg->Des();
    argPtr.Format( _L( "%S %S" ), &aExcludeList, &aDriveToWipe );

    RProcess process;
    CleanupClosePushL( process );
    TInt err = process.Create( KSecureFormatter, *arg );
    ERROR( err, "Failed to create process" );
    User::LeaveIfError( err );

    TRequestStatus status;
    process.Logon( status );
    process.Resume();
    User::WaitForRequest( status );
    ERROR( status.Int(), "secure formatter failed" );
    //User::LeaveIfError( status.Int() );
    
    //RDebug::Print(_L("RunSecureFormatterL : before process.Kill(KErrNone)"));
    //process.Kill(KErrNone);
    CleanupStack::PopAndDestroy( &process );
    CleanupStack::PopAndDestroy( arg );
    }


// ---------------------------------------------------------------------------
// RunTestDataCreatorL
// ---------------------------------------------------------------------------
//
static void RunTestDataCreatorL(
    const TDesC& aArg,
    TInt aParamIdx1 = 0, const TDesC& aParam1 = KNullDesC,
    TInt aParamIdx2 = 0, const TDesC& aParam2 = KNullDesC )
    {
    FUNC_LOG;

    RProcess process;
    CleanupClosePushL( process );
    TInt err = process.Create( _L( "testdatacreator.exe" ), aArg );
    ERROR( err, "Failed to create process" );
    User::LeaveIfError( err );

    if ( aParam1 != KNullDesC )
        {
        err = process.SetParameter( aParamIdx1, aParam1 );
        ERROR_1( err, "Failed to set parameter 1 to index %d", aParamIdx1 );
        User::LeaveIfError( err );
        }

    if ( aParam2 != KNullDesC )
        {
        err = process.SetParameter( aParamIdx2, aParam2 );
        ERROR_1( err, "Failed to set parameter 2 to index %d", aParamIdx2 );
        User::LeaveIfError( err );
        }

    TRequestStatus status;
    process.Logon( status );
    process.Resume();
    User::WaitForRequest( status );
    ERROR( status.Int(), "testdatacreator.exe failed" );
    //User::LeaveIfError( status.Int() );

    //RDebug::Print(_L("RunTestDataCreatorL : before process.Kill(KErrNone)"));
    //process.Kill(KErrNone);
    CleanupStack::PopAndDestroy( &process );
    }


// ---------------------------------------------------------------------------
// SetupFilesL
// ---------------------------------------------------------------------------
//
static void SetupFilesL( const TDesC& aSpecFileName )
    {
    FUNC_LOG;

    TRAPD( err, RunTestDataCreatorL(
        KSetupFilesParam, KFileNameIndex, aSpecFileName ) );
    ERROR( err, "Failed to setup data" );
    User::LeaveIfError( err );
    }


// ---------------------------------------------------------------------------
// SetupExcludeListL
// ---------------------------------------------------------------------------
//
static void SetupExcludeListL(
    const TDesC& aSourceFileName,
    const TDesC& aTargetFileName )
    {
    FUNC_LOG;

    TRAPD( err, RunTestDataCreatorL(
        KSetupExcludeListParam,
        KFileNameIndex, aSourceFileName,
        KTargetFileNameIndex, aTargetFileName ) );
    ERROR( err, "Failed to setup exclude list" );
    User::LeaveIfError( err );
    }


// ---------------------------------------------------------------------------
// VerifyDataL
// ---------------------------------------------------------------------------
//
static void VerifyDataL( const TDesC& aSpecFileName )
    {
    FUNC_LOG;

    TRAPD( err, RunTestDataCreatorL(
        KVerifyFilesParam,
        KFileNameIndex, aSpecFileName,
        KRootDirNameIndex, KRootDir ) );
    ERROR( err, "Data verification failed" );
    User::LeaveIfError( err );
    }


// ---------------------------------------------------------------------------
// CleanupExcludeList
// ---------------------------------------------------------------------------
//
static void CleanupExcludeList( const TDesC& aListPattern )
    {
    FUNC_LOG;

    TRAPD_ERR( err, RunTestDataCreatorL(
        KCleanupExcludeListParam, KFileNameIndex, aListPattern ) );
    ERROR( err, "Failed to cleanup exclude list" );
    }


// ======== MEMBER FUNCTIONS ========

// ---------------------------------------------------------------------------
// CSecureFormatterTest::NewL
// ---------------------------------------------------------------------------
//
CSecureFormatterTest* CSecureFormatterTest::NewL( CTestModuleIf& aTestModuleIf )
    {
    FUNC_LOG;

    CSecureFormatterTest* self = new( ELeave ) CSecureFormatterTest( aTestModuleIf );
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );
    return self;
    }


// ---------------------------------------------------------------------------
// CSecureFormatterTest::~CSecureFormatterTest
// ---------------------------------------------------------------------------
//
CSecureFormatterTest::~CSecureFormatterTest()
    {
    FUNC_LOG;
    }


// ---------------------------------------------------------------------------
// CSecureFormatterTest::RunMethodL
// ---------------------------------------------------------------------------
//
TInt CSecureFormatterTest::RunMethodL( CStifItemParser& aItem )
    {
    FUNC_LOG;

    const TStifFunctionInfo KFunctions[] =
        {
        // Copy this line for every implemented function.
        // First string is the function name used in TestScripter script file.
        // Second is the actual implementation member function.
        ENTRY( "RunTestL", CSecureFormatterTest::RunTestL ),
        ENTRY( "RunSpecialTestFileOpenL", CSecureFormatterTest::RunSpecialTestFileOpenL ),
        ENTRY( "RunSpecialTestAttributesL", CSecureFormatterTest::RunSpecialTestAttributesL ),
        ENTRY( "RunAppSpecialTestL", CSecureFormatterTest::RunAppSpecialTestL ),
        ENTRY( "RunAppSpecialReplaceTestL", CSecureFormatterTest::RunAppSpecialReplaceTestL ),
        };
    const TInt count( sizeof( KFunctions ) / sizeof( TStifFunctionInfo ) );
    return RunInternalL( KFunctions, count, aItem );
    }


// ---------------------------------------------------------------------------
// CSecureFormatterTest::CSecureFormatterTest
// ---------------------------------------------------------------------------
//
CSecureFormatterTest::CSecureFormatterTest( CTestModuleIf& aTestModuleIf )
  : CScriptBase( aTestModuleIf )
    {
    FUNC_LOG;
    }


// ---------------------------------------------------------------------------
// CSecureFormatterTest::ConstructL
// ---------------------------------------------------------------------------
//
void CSecureFormatterTest::ConstructL()
    {
    FUNC_LOG;
    RFs fs;
    TInt err = fs.Connect();
    RDebug::Print(_L("Connect to file server  returned with = %d"),err);
    User::LeaveIfError( err );
    CleanupClosePushL( fs );

    err = fs.MkDirAll(KAppSpecificExcludeListFolder);
    RDebug::Print(_L("fs.MkDirAll(KAppSpecificExcludeListFolder) err = %d"), err);
    if( err != KErrNone && err != KErrAlreadyExists )
        {
        User::Leave( err );
        }

    err = fs.MkDirAll(KDefaultExcludeListPath);
    RDebug::Print(_L("fs.MkDirAll(KDefaultExcludeListPath)= %d"), err);
    if( err != KErrNone && err != KErrAlreadyExists )
        {
        User::Leave( err );
        }
    CleanupStack::PopAndDestroy( &fs );
    RunTestDataCreatorL( KNullDesC ); // Run one time to stabilize request count
    }


// ---------------------------------------------------------------------------
// CSecureFormatterTest::RunTestL
// ---------------------------------------------------------------------------
//
TInt CSecureFormatterTest::RunTestL( CStifItemParser& aItem )
    {
    FUNC_LOG;

    TFileName specName;
    SetupDataL( aItem, specName ); // Read specName
    RunAndVerifyL( specName, KDefaultExcludeList );

    return KErrNone;
    }


// ---------------------------------------------------------------------------
// CSecureFormatterTest::RunSpecialTestFileOpenL
// ---------------------------------------------------------------------------
//
TInt CSecureFormatterTest::RunSpecialTestFileOpenL( CStifItemParser& aItem )
    {
    FUNC_LOG;

    TFileName specName;
    SetupDataL( aItem, specName ); // Read specName

    TPtrC fileName;
    User::LeaveIfError( aItem.GetNextString( fileName ) );

    TInt mode;
    User::LeaveIfError( aItem.GetNextInt( mode ) );
    INFO_1( "File open mode = 0x%08x", mode );

    RFs fs;
    User::LeaveIfError( fs.Connect() );
    CleanupClosePushL( fs );

    RFile file;
    User::LeaveIfError( file.Open( fs, fileName, mode ) );
    CleanupClosePushL( file );

    RunAndVerifyL( specName, KDefaultExcludeList );

    CleanupStack::PopAndDestroy( &file );
    CleanupStack::PopAndDestroy( &fs );

    return KErrNone;
    }


// ---------------------------------------------------------------------------
// CSecureFormatterTest::RunSpecialTestAttributesL
// ---------------------------------------------------------------------------
//
TInt CSecureFormatterTest::RunSpecialTestAttributesL( CStifItemParser& aItem )
    {
    FUNC_LOG;

    TFileName specName;
    SetupDataL( aItem, specName ); // Read specName

    RFs fs;
    User::LeaveIfError( fs.Connect() );
    CleanupClosePushL( fs );

    TPtrC entryName;
    User::LeaveIfError( aItem.GetNextString( entryName ) );

    TInt att;
    User::LeaveIfError( aItem.GetNextInt( att ) );
    INFO_1( "File attributes = 0x%08x", att );

    User::LeaveIfError( fs.SetAtt( entryName, KEntryAttNormal,
        KEntryAttReadOnly | KEntryAttHidden | KEntryAttSystem | KEntryAttArchive ) );
    User::LeaveIfError( fs.SetAtt( entryName, att, 0 ) );

    RunAndVerifyL( specName, KDefaultExcludeList );

    // Cleanup file, ignore errors
    fs.SetAtt( entryName, KEntryAttNormal,
        KEntryAttReadOnly | KEntryAttHidden | KEntryAttSystem | KEntryAttArchive );
    fs.Delete( entryName );

    CleanupStack::PopAndDestroy( &fs );

    return KErrNone;
    }


// ---------------------------------------------------------------------------
// CSecureFormatterTest::SetupDataL
// ---------------------------------------------------------------------------
//
void CSecureFormatterTest::SetupDataL( CStifItemParser& aItem, TDes& aSpecFile )
    {
    FUNC_LOG;

    TPtrC name;
    User::LeaveIfError( aItem.GetNextString( name ) );
    aSpecFile = name;
    SetupFilesL( aSpecFile );

    TInt err = aItem.GetNextString( name );
    if ( err == KErrNone )
        {
        SetupExcludeListL( name, KDefaultExcludeList );
        }
    }


// ---------------------------------------------------------------------------
// CSecureFormatterTest::RunAndVerifyL
// ---------------------------------------------------------------------------
//
void CSecureFormatterTest::RunAndVerifyL( const TDesC& aSpecFile,
    const TDesC& aExcludeList )
    {
    FUNC_LOG;

    TInt errRun( KErrNone );
    TInt errVerify( KErrNone );
    TRAP( errRun, RunSecureFormatterL( aExcludeList, KRootDir ) ); // Trap this ...
    if( errRun == KErrNone )
        {
        // ...and this so that excludelists can be cleaned. Otherwise
        //  several test cases might be impacted.
        TRAP( errVerify, VerifyDataL( aSpecFile ) ); 
        }
    CleanupExcludeList( aExcludeList );
    CleanupExcludeList( KAppSpecificExcludeListPattern );
    User::LeaveIfError( errRun );
    User::LeaveIfError( errVerify );
    }


// ---------------------------------------------------------------------------
// CSecureFormatterTest::RunAppSpecialTestL
// ---------------------------------------------------------------------------
//
TInt CSecureFormatterTest::RunAppSpecialTestL( CStifItemParser& aItem )
    {
    FUNC_LOG;

    TFileName specName;
    SetupAppSpecialDataL( aItem, specName, EFalse ); // Read specName
    RunAndVerifyL( specName, KNoDefaultList );

    return KErrNone;
    }


// ---------------------------------------------------------------------------
// CSecureFormatterTest::RunAppSpecialReplaceTestL
// ---------------------------------------------------------------------------
//
TInt CSecureFormatterTest::RunAppSpecialReplaceTestL( CStifItemParser& aItem )
    {
    FUNC_LOG;

    TFileName specName;
    SetupAppSpecialDataL( aItem, specName, ETrue ); // Read specName
    RunAndVerifyL( specName, KNoDefaultList );

    return KErrNone;
    }


// ---------------------------------------------------------------------------
// CSecureFormatterTest::SetupAppSpecialDataL
// ---------------------------------------------------------------------------
//
void CSecureFormatterTest::SetupAppSpecialDataL( CStifItemParser& aItem, 
    TDes& aSpecFile, TBool aReplace )
    {
    FUNC_LOG;

    TPtrC name;
    User::LeaveIfError( aItem.GetNextString( name ) );
    aSpecFile = name;
    SetupFilesL( aSpecFile );

    TInt err( KErrNone );
    TInt cntr( 0 );
    
    do
        {
        
        err = aItem.GetNextString( name );
        if ( err == KErrNone )
            {
            TFileName path;
            TParsePtrC listName( name );
            path.Append( KAppSpecificExcludeListFolder );

            if( !KAppSpecificExcludeList1().Compare( listName.NameAndExt() ) )
                {
                path.Append( KAppSpecificExcludeList1 );
                }
            else if( !KAppSpecificExcludeList2().Compare( listName.NameAndExt() ) )
                {
                path.Append( KAppSpecificExcludeList2 );
                }
            else
                {
                // Append given file name or replace it with default exclude list name 
                if( !aReplace )
                    {
                    path.Append( listName.NameAndExt() );
                    }
                else if( !cntr )
                    {
                    path.Append( KAppSpecificExcludeList1 );
                    cntr++;
                    }
                else
                    {
                    path.Append( KAppSpecificExcludeList2 );
                    }
                }
            
            SetupExcludeListL( name, path );
            }
        }while( err == KErrNone );
    }


// ======== GLOBAL FUNCTIONS ========

// ---------------------------------------------------------------------------
// Entry point
// ---------------------------------------------------------------------------
//
EXPORT_C CScriptBase* LibEntryL( CTestModuleIf& aTestModuleIf )
    {
    FUNC_LOG;

    return CSecureFormatterTest::NewL( aTestModuleIf );
    }


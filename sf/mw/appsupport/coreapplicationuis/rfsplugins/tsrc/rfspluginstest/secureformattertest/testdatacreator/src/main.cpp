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
* Description:  Entry point for testdatacreator
*
*/


#include <bacline.h> // For CCommandLineArguments
#include <e32base.h>
#include <f32file.h>

#include "filespec.h"
#include "testdatacreatorapi.h"
#include "trace.h"

_LIT( KThreadName, "TestDataCreator" );

enum TCommand
    {
    ENoCmd = 0,
    ESetupData,
    ESetupList,
    EVerify,
    ECleanup
    };

// ======== LOCAL FUNCTIONS ========

TCommand CommandLineSwitch()
    {
    FUNC_LOG;

	// Get raw command line
    HBufC* rawCmdLine = HBufC::NewLC( User::CommandLineLength() );
	TPtr rawCmdLinePtr( rawCmdLine->Des() );
	User::CommandLine( rawCmdLinePtr );

    INFO_1( "Command line: '%S'", rawCmdLine );

	TLex lex( rawCmdLinePtr );
	lex.SkipSpace();
    TPtrC token = lex.NextToken();

    TCommand cmd( ENoCmd );
    if ( token == KSetupFilesParam )
        {
        cmd = ESetupData;
        }
    else if ( token == KSetupExcludeListParam )
        {
        cmd = ESetupList;
        }
    else if ( token == KVerifyFilesParam )
        {
        cmd = EVerify;
        }
    else if ( token == KCleanupExcludeListParam )
        {
        cmd = ECleanup;
        }

    CleanupStack::PopAndDestroy( rawCmdLine );
    return cmd;
	}


// ---------------------------------------------------------------------------
// ParamByIdxLC
// ---------------------------------------------------------------------------
//
static HBufC* ParamByIdxLC( TInt aIndex )
    {
    FUNC_LOG;

    TInt len = User::ParameterLength( aIndex );
    if ( len < 1 )
        {
        ERROR_1( KErrArgument, "Parameter at index %d not found", aIndex );
        User::Leave( KErrArgument );
        }

    HBufC* param = HBufC::NewLC( len );
    TPtr paramPtr = param->Des();
    User::LeaveIfError( User::GetDesParameter( aIndex, paramPtr ) );
    return param;
    }


// ---------------------------------------------------------------------------
// Create directory structure and files.
// ---------------------------------------------------------------------------
//
void CreateFilesL( RFs& aFs )
    {
    FUNC_LOG;

    HBufC* param = ParamByIdxLC( KFileNameIndex );
    INFO_1( "Setting up files - specification '%S'", param );

    CFileSpec* creator = CFileSpec::NewLC( aFs, *param );
    creator->CreateL();
    CleanupStack::PopAndDestroy( creator );

    CleanupStack::PopAndDestroy( param );
    }


// ---------------------------------------------------------------------------
// Create exclude list.
// ---------------------------------------------------------------------------
//
void CreateListL( RFs& aFs )
    {
    FUNC_LOG;

    HBufC* sourceFileName = ParamByIdxLC( KFileNameIndex );
    HBufC* targetFileName = ParamByIdxLC( KTargetFileNameIndex );
    INFO_2( "Setting up exclude list - copy file '%S' to '%S'", sourceFileName, targetFileName );

    aFs.MkDirAll( *targetFileName );

    CFileMan* fileMan = CFileMan::NewL( aFs );
    CleanupStack::PushL( fileMan );

    TInt err = fileMan->Copy( *sourceFileName, *targetFileName, CFileMan::EOverWrite );
    ERROR( err, "Failed to copy file" );
    User::LeaveIfError( err );

    CleanupStack::PopAndDestroy( fileMan );
    CleanupStack::PopAndDestroy( targetFileName );
    CleanupStack::PopAndDestroy( sourceFileName );
    }


// ---------------------------------------------------------------------------
// Verify directory structure and files.
// ---------------------------------------------------------------------------
//
void VerifyFilesL( RFs& aFs )
    {
    FUNC_LOG;

    HBufC* specFileName = ParamByIdxLC( KFileNameIndex );
    HBufC* rootDir = ParamByIdxLC( KRootDirNameIndex );
    INFO_2( "Verifying files - specification '%S', root directory '%S'", specFileName, rootDir );

    CFileSpec* verifier = CFileSpec::NewLC( aFs, *specFileName );
    verifier->VerifyL( *rootDir );
    CleanupStack::PopAndDestroy( verifier );

    CleanupStack::PopAndDestroy( rootDir );
    CleanupStack::PopAndDestroy( specFileName );
    }


// ---------------------------------------------------------------------------
// Clean up exclude list.
// ---------------------------------------------------------------------------
//
void CleanupListL( RFs& aFs )
    {
    FUNC_LOG;

    HBufC* param = ParamByIdxLC( KFileNameIndex );
    INFO_1( "Cleaning up exclude list - delete file(s) '%S'", param );

    CFileMan* fileMan = CFileMan::NewL( aFs );
    CleanupStack::PushL( fileMan );

    TInt err = fileMan->Delete( *param, CFileMan::ERecurse );
    if ( err == KErrNotFound || err == KErrPathNotFound )
        {
        err = KErrNone;
        }
    ERROR( err, "Failed to delete files" );
    User::LeaveIfError( err );

    CleanupStack::PopAndDestroy( fileMan );
    CleanupStack::PopAndDestroy( param );
    }


// ---------------------------------------------------------------------------
// Main function for the executable.
// ---------------------------------------------------------------------------
//
void MainL()
    {
    FUNC_LOG;

    RFs fs;
    User::LeaveIfError( fs.Connect() );
    CleanupClosePushL( fs );

    TCommand cmd = CommandLineSwitch();
    switch ( cmd )
        {
        case ESetupData:
            CreateFilesL( fs );
            break;
        case ESetupList:
            CreateListL( fs );
            break;
        case EVerify:
            VerifyFilesL( fs );
            break;
        case ECleanup:
            CleanupListL( fs );
            break;
        default:
            break;
        }

    CleanupStack::PopAndDestroy( &fs );
    }


// ======== GLOBAL FUNCTIONS ========

// ---------------------------------------------------------------------------
// Entry point for the executable.
// ---------------------------------------------------------------------------
//
TInt E32Main()
    {
    FUNC_LOG;
    __UHEAP_MARK;

    User::RenameThread( KThreadName );
    CTrapCleanup* cleanup = CTrapCleanup::New();

    TInt err = KErrNoMemory;
    if ( cleanup )
        {
        TRAP( err, MainL() );
        ERROR( err, "Leave in MainL" );
        delete cleanup;
        }

    __UHEAP_MARKEND;
    return err;
    }

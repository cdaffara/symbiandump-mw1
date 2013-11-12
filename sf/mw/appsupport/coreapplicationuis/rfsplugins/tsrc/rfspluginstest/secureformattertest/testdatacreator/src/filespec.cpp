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
* Description:  Implementation of CFileSpec class
*
*/


#include "filespec.h"
#include "filecreator.h"
#include "trace.h"

// TFileText can read at most 256 characters at a time
const TInt KMaxRecordLength( 256 );

// Granularity for the file verification array
const TInt KArrayGranularity( 16 );

// Character marking the place in file name to replace with number
const TChar KPatternChar( '?' );

// Format string used to construct new file names from a pattern
_LIT( KFileNameFmt, "%S%d%S" );

// Tags used in specification files
_LIT( KTagPath,  "[Path]" );
_LIT( KTagFile,  "[File]" );
_LIT( KTagFiles, "[Files]" );
_LIT( KTagSize,  "size" );
_LIT( KTagCount, "count" );
_LIT( KTagKeep, "keep" );

// ======== LOCAL FUNCTIONS ========

// ---------------------------------------------------------------------------
// PatternIdxL
// ---------------------------------------------------------------------------
//
static TInt PatternIdxL( const TDesC& aName )
    {
    TInt idx = aName.Locate( KPatternChar );
    if ( idx == KErrNotFound || aName.LocateReverse( KPatternChar ) != idx )
        {
        ERROR_1( KErrArgument, "Invalid file name specification '%S': there must be exactly one ? character in the file name", &aName );
        User::Leave( KErrArgument );
        }
    return idx;
    }


// ---------------------------------------------------------------------------
// LeftL
// ---------------------------------------------------------------------------
//
static TPtrC LeftL( const TDesC& aName )
    {
    return aName.Left( PatternIdxL( aName ) );
    }


// ---------------------------------------------------------------------------
// RightL
// ---------------------------------------------------------------------------
//
static TPtrC RightL( const TDesC& aName )
    {
    return aName.Right( aName.Length() - PatternIdxL( aName ) - 1 );
    }


// ---------------------------------------------------------------------------
// SortOrder
// ---------------------------------------------------------------------------
//
static TInt SortOrder( const HBufC& aLhs, const HBufC& aRhs )
    {
    return aLhs.CompareF( aRhs );
    }


// ======== MEMBER FUNCTIONS ========

// ---------------------------------------------------------------------------
// CFileSpec::NewLC
// ---------------------------------------------------------------------------
//
CFileSpec* CFileSpec::NewLC( RFs& aFs, const TDesC& aSpecFileName )
    {
    CFileSpec* self = new( ELeave ) CFileSpec( aFs );
    CleanupStack::PushL( self );
    self->ConstructL( aSpecFileName );
    return self;
    }


// ---------------------------------------------------------------------------
// CFileSpec::~CFileSpec
// ---------------------------------------------------------------------------
//
CFileSpec::~CFileSpec()
    {
    FUNC_LOG;

    delete iFileCreator;
    iKeptFiles.ResetAndDestroy();
    iKeptFiles.Close();
    iSpecFile.Close();
    }


// ---------------------------------------------------------------------------
// CFileSpec::CreateL
// ---------------------------------------------------------------------------
//
void CFileSpec::CreateL()
    {
    FUNC_LOG;
    ASSERT( !iFileCreator );

    iFileCreator = CFileCreator::NewL( iFs );

    TFileText fileText;
    fileText.Set( iSpecFile );
    TBuf<KMaxRecordLength> buf;
    TInt err = fileText.Read( buf );
    while ( err == KErrNone )
        {
        if ( buf == KTagPath )
            {
            CreateOneDirectoryL( fileText );
            }
        else if ( buf == KTagFile )
            {
            CreateOneFileL( fileText );
            }
        else if ( buf == KTagFiles )
            {
            CreateFilesL( fileText );
            }
        else if ( buf != KNullDesC )
            {
            INFO_1( "Unknown tag: %S", &buf );
            }

        err = fileText.Read( buf );
        }
    }


// ---------------------------------------------------------------------------
// CFileSpec::VerifyL
// ---------------------------------------------------------------------------
//
void CFileSpec::VerifyL( const TDesC& aRootDir )
    {
    FUNC_LOG;

    TFileText fileText;
    fileText.Set( iSpecFile );
    TBuf<KMaxRecordLength> buf;
    TInt err = fileText.Read( buf );
    while ( err == KErrNone )
        {
        if ( buf == KTagPath || buf == KTagFile )
            {
            ReadOneItemL( fileText );
            }
        else if ( buf == KTagFiles )
            {
            ReadManyItemsL( fileText );
            }

        err = fileText.Read( buf );
        }

    CheckAllKeptFilesExistL();
    iKeptFiles.Sort( TLinearOrder<HBufC>( SortOrder ) );
    CheckOnlyKeptFilesExistL( aRootDir );
    }


// ---------------------------------------------------------------------------
// CFileSpec::CFileSpec
// ---------------------------------------------------------------------------
//
CFileSpec::CFileSpec( RFs& aFs )
  : iFs( aFs ),
    iKeptFiles( KArrayGranularity )
    {
    FUNC_LOG;
    }


// ---------------------------------------------------------------------------
// CFileSpec::ConstructL
// ---------------------------------------------------------------------------
//
void CFileSpec::ConstructL( const TDesC& aSpecFileName )
    {
    FUNC_LOG;

    TInt err = iSpecFile.Open( iFs, aSpecFileName, EFileRead | EFileShareReadersOnly );
    ERROR_1( err, "Failed to open file %S", &aSpecFileName );
    User::LeaveIfError( err );
    }


// ---------------------------------------------------------------------------
// CFileSpec::CreateOneDirectoryL
// ---------------------------------------------------------------------------
//
void CFileSpec::CreateOneDirectoryL( TFileText& aSpecFile )
    {
    FUNC_LOG;

    TBuf<KMaxRecordLength> name;
    TInt err = aSpecFile.Read( name );
    ERROR( err, "Failed to read path name" );
    User::LeaveIfError( err );

    INFO_1( "Directory to create: %S", &name );
    err = iFs.MkDir( name );
    if ( err == KErrAlreadyExists )
        {
        err = KErrNone;
        }
    ERROR_1( err, "Failed to create directory %S", &name );
    User::LeaveIfError( err );
    }


// ---------------------------------------------------------------------------
// CFileSpec::CreateOneFileL
// ---------------------------------------------------------------------------
//
void CFileSpec::CreateOneFileL( TFileText& aSpecFile )
    {
    FUNC_LOG;

    TBuf<KMaxRecordLength> name;
    TInt err = aSpecFile.Read( name );
    ERROR( err, "Failed to read file name" );
    User::LeaveIfError( err );

    TInt size( 0 );
    TLex lex;
    TBuf<KMaxRecordLength> buf;
    err = aSpecFile.Read( buf );
    while ( err == KErrNone )
        {
        lex.Assign( buf );
        TPtrC token = lex.NextToken();
        if ( token == KTagSize )
            {
            lex.SkipSpace();
            err = lex.Val( size );
            ERROR_1( err, "Failed to determine file size for file %S", &name );
            }
        else if ( token == KNullDesC )
            {
            break;
            }

        err = aSpecFile.Read( buf );
        }

    iFileCreator->CreateOneFileL( name, size );
    }


// ---------------------------------------------------------------------------
// CFileSpec::CreateFilesL
// ---------------------------------------------------------------------------
//
void CFileSpec::CreateFilesL( TFileText& aSpecFile )
    {
    FUNC_LOG;

    TBuf<KMaxRecordLength> name;
    TInt err = aSpecFile.Read( name );
    ERROR( err, "Failed to read file name" );
    User::LeaveIfError( err );

    TInt size( 0 );
    TInt count( 0 );
    TLex lex;
    TBuf<KMaxRecordLength> buf;
    err = aSpecFile.Read( buf );
    while ( err == KErrNone )
        {
        lex.Assign( buf );
        TPtrC token = lex.NextToken();
        if ( token == KTagSize )
            {
            lex.SkipSpace();
            err = lex.Val( size );
            ERROR_1( err, "Failed to determine file size for file %S", &name );
            }
        else if ( token == KTagCount )
            {
            lex.SkipSpace();
            err = lex.Val( count );
            ERROR_1( err, "Failed to determine count for file %S", &name );
            }
        else if ( token == KNullDesC )
            {
            break;
            }

        err = aSpecFile.Read( buf );
        }

    CreateFilesL( name, size, count );
    }


// ---------------------------------------------------------------------------
// CFileSpec::CreateFilesL
// ---------------------------------------------------------------------------
//
void CFileSpec::CreateFilesL( const TDesC& aName, TInt aSizeInBytes, TInt aCount )
    {
    FUNC_LOG;
    INFO_3( "Files to create: %S (%d bytes), count: %d", &aName, aSizeInBytes, aCount );

    TPtrC left( LeftL( aName ) );
    TPtrC right( RightL( aName ) );
    TFileName name;
    for ( TInt i = 0; i < aCount; i++ )
        {
        name.Format( KFileNameFmt, &left, i + 1, &right );
        iFileCreator->CreateOneFileL( name, aSizeInBytes );
        }
    }


// ---------------------------------------------------------------------------
// CFileSpec::ReadOneItemL
// ---------------------------------------------------------------------------
//
void CFileSpec::ReadOneItemL( TFileText& aSpecFile )
    {
    FUNC_LOG;

    TBuf<KMaxRecordLength> name;
    TInt err = aSpecFile.Read( name );
    ERROR( err, "Failed to read file/path name" );
    User::LeaveIfError( err );

    TLex lex;
    TBuf<KMaxRecordLength> buf;
    err = aSpecFile.Read( buf );
    while ( err == KErrNone )
        {
        lex.Assign( buf );
        TPtrC token = lex.NextToken();
        if ( token == KTagKeep )
            {
            HBufC* nameBuf = name.AllocLC();
            INFO_1( "Keep '%S'", nameBuf );
            iKeptFiles.AppendL( nameBuf );
            CleanupStack::Pop( nameBuf );
            }
        else if ( token == KNullDesC )
            {
            break;
            }

        err = aSpecFile.Read( buf );
        }
    }


// ---------------------------------------------------------------------------
// CFileSpec::ReadManyItemsL
// ---------------------------------------------------------------------------
//
void CFileSpec::ReadManyItemsL( TFileText& aSpecFile )
    {
    FUNC_LOG;

    TBuf<KMaxRecordLength> pattern;
    TInt err = aSpecFile.Read( pattern );
    ERROR( err, "Failed to read file/path name" );
    User::LeaveIfError( err );

    TInt count( 0 );
    TBool keep( EFalse );
    TLex lex;
    TBuf<KMaxRecordLength> buf;
    err = aSpecFile.Read( buf );
    while ( err == KErrNone )
        {
        lex.Assign( buf );
        TPtrC token = lex.NextToken();
        if ( token == KTagKeep )
            {
            keep = ETrue;
            }
        else if ( token == KTagCount )
            {
            lex.SkipSpace();
            err = lex.Val( count );
            ERROR_1( err, "Failed to determine count for pattern %S", &pattern );
            }
        else if ( token == KNullDesC )
            {
            break;
            }

        err = aSpecFile.Read( buf );
        }

    if ( keep )
        {
        TPtrC left( LeftL( pattern ) );
        TPtrC right( RightL( pattern ) );
        TFileName name;
        for ( TInt i = 0; i < count; i++ )
            {
            name.Format( KFileNameFmt, &left, i + 1, &right );
            HBufC* nameBuf = name.AllocLC();
            INFO_1( "Keep '%S'", nameBuf );
            iKeptFiles.AppendL( nameBuf );
            CleanupStack::Pop( nameBuf );
            }
        }
    }


// ---------------------------------------------------------------------------
// CFileSpec::CheckAllKeptFilesExistL
// ---------------------------------------------------------------------------
//
void CFileSpec::CheckAllKeptFilesExistL()
    {
    FUNC_LOG;

    TInt count = iKeptFiles.Count();
    for ( TInt i = 0; i < count; i++ )
        {
        CheckExistsL( *( iKeptFiles[i] ) );
        }
    }


// ---------------------------------------------------------------------------
// CFileSpec::CheckExistsL
// ---------------------------------------------------------------------------
//
void CFileSpec::CheckExistsL( const TDesC& aName )
    {
    FUNC_LOG;
    INFO_1( "Verifying file '%S'", &aName );

    TFindFile finder( iFs );
    TInt err = finder.FindByPath( aName, NULL );
    ERROR( err, "Find file failed" );
    User::LeaveIfError( err );
    }


// ---------------------------------------------------------------------------
// CFileSpec::CheckOnlyKeptFilesExistL
// ---------------------------------------------------------------------------
//
void CFileSpec::CheckOnlyKeptFilesExistL( const TDesC& aRootDir )
    {
    FUNC_LOG;

    CDirScan* scan = CDirScan::NewLC( iFs );
    scan->SetScanDataL(
        aRootDir,
        KEntryAttDir | KEntryAttHidden | KEntryAttSystem,
        EDirsAnyOrder );

    TFileName fullName;
    CDir* dir( NULL );
    scan->NextL( dir );
    while( dir )
        {
        CleanupStack::PushL( dir );
        TPtrC path( scan->FullPath() );
        TInt count( dir->Count() );
        for ( TInt i = 0; i < count; i++ )
            {
            const TEntry& entry( ( *dir )[ i ] );
            fullName = path;
            fullName.Append( entry.iName );
            if ( entry.IsDir() )
                {
                fullName.Append( KPathDelimiter );
                }
            CheckFileMayExistL( fullName );
            }
        CleanupStack::PopAndDestroy( dir );
        dir = NULL;
        scan->NextL( dir );
        }

    CleanupStack::PopAndDestroy( scan );
    }


// ---------------------------------------------------------------------------
// CFileSpec::CheckFileMayExistL
// ---------------------------------------------------------------------------
//
void CFileSpec::CheckFileMayExistL( const TDesC& aName )
    {
    FUNC_LOG;
    INFO_1( "Checking if file/dir has right to exist '%S'", &aName );

    HBufC* nameBuf = aName.AllocLC();
    if ( iKeptFiles.FindInOrderL( nameBuf, TLinearOrder<HBufC>( SortOrder ) ) == KErrNotFound )
        {
        ERROR_1( KErrGeneral, "File/directory '%S' should have been cleaned away", &aName );
        User::Leave( KErrGeneral );
        }
    CleanupStack::PopAndDestroy( nameBuf );
    }

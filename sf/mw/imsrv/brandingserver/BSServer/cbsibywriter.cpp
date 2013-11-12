/*
* Copyright (c) 2006-2006 Nokia Corporation and/or its subsidiary(-ies).
* All rights reserved.
* This component and the accompanying materials are made available
* under the terms of the License "Eclipse Public License v1.0"
* which accompanies this distribution, and is available
* at the URL "http://www.eclipse.org/legal/epl-v10.html".
*
* Initial Contributors:
* Nokia Corporation - initial contribution.
*
* Contributors:
*
* Description:   Writes IBY files
*
*/



#include <f32file.h>
#include <bautils.h>
#include <utf.h>

#include "cbsibywriter.h"
#include "bsimportconstants.h"
//#include "importlogwriter.h"
#include "DebugTrace.h"

// CONSTANTS
_LIT8( KIBYHeader1,           "\n#ifndef __BRAND_" );
_LIT8( KIBYHeader2,           "\n#define __BRAND_" );
_LIT8( KIBYHeaderEnd,         "_IBY_" );
_LIT8( KIBYFileItem,          "\nfile=" );
_LIT8( KIBYEmptyLine,         "\n" );
_LIT8( KIBYSpace,             " \t\t " );
_LIT8( KIBYFooter,            "\n#endif //__BRAND_" );

// File & dir
_LIT8( KIBYBaseSource,        "\\epoc32\\winscw\\c" );

// ======== MEMBER FUNCTIONS ========

// ---------------------------------------------------------------------------
// CBSIBYWriter::NewL
// ---------------------------------------------------------------------------
//
CBSIBYWriter* CBSIBYWriter::NewL()
    {
    CBSIBYWriter* self = NewLC();
    CleanupStack::Pop();
    return self;
    }

// ---------------------------------------------------------------------------
// CBSIBYWriter::NewLC
// ---------------------------------------------------------------------------
//
CBSIBYWriter* CBSIBYWriter::NewLC()
    {
    CBSIBYWriter* self = new (ELeave) CBSIBYWriter();
    CleanupStack::PushL( self );
    self->ConstructL();
    return self;
    }

// ---------------------------------------------------------------------------
// CBSIBYWriter::ConstructL
// ---------------------------------------------------------------------------
//
void CBSIBYWriter::ConstructL()
    {
    }

// ---------------------------------------------------------------------------
// CBSIBYWriter::CBSIBYWriter
// ---------------------------------------------------------------------------
//
CBSIBYWriter::CBSIBYWriter()
    {
    }

// ---------------------------------------------------------------------------
// CBSIBYWriter::~CBSIBYWriter
// ---------------------------------------------------------------------------
//
CBSIBYWriter::~CBSIBYWriter()
    {
    iSourcePath.ResetAndDestroy();
    iSourceFiles.ResetAndDestroy();
    iDestinations.ResetAndDestroy();
    delete iIbyFile;
    }

// ---------------------------------------------------------------------------
// CBSIBYWriter::SetFileItemL
// ---------------------------------------------------------------------------
//
void CBSIBYWriter::SetFileItemL( const TDesC& aSrc, const TDesC& aDest )
    {
    TRACE( T_LIT( "CBSIBYWriter::SetFileItemL begin") );
    // Parse and construct filenames
    TParse srcparse;
    srcparse.Set( aSrc, NULL, NULL );

    TParse dstparse;
    dstparse.Set( aDest, NULL, NULL );

    // Path
    HBufC8* srcpath = HBufC8::NewLC( srcparse.Path().Length() );
    TPtr8 ptr( srcpath->Des() );
    CnvUtfConverter::ConvertFromUnicodeToUtf8( ptr, srcparse.Path() );

    HBufC8* src = HBufC8::NewLC( srcparse.NameAndExt().Length() );
    ptr.Set( src->Des() );
    CnvUtfConverter::ConvertFromUnicodeToUtf8( ptr, srcparse.NameAndExt() );

    HBufC8* dst = HBufC8::NewLC( aDest.Length() );
    ptr.Set( dst->Des() );
    
    HBufC* newDst = HBufC::NewLC( aDest.Length() );
    TPtr dstPtr( newDst->Des() );
    TPtrC name = dstparse.NameAndExt();
	TPtrC path = dstparse.Path();
	dstPtr.Append( KBSZDrive );
	dstPtr.Append( path );
	dstPtr.Append( name );
	
    CnvUtfConverter::ConvertFromUnicodeToUtf8( ptr, dstPtr );
    
    TRACE( T_LIT( "CBSIBYWriter::SetFileItemL - setting source file %S"), &aSrc );
    TRACE( T_LIT( "CBSIBYWriter::SetFileItemL - setting destination file %S"), newDst );
	CleanupStack::PopAndDestroy( newDst );
    CleanupStack::Pop( 3 );

    // Append filepair
    iSourcePath.Append( srcpath );
    iSourceFiles.Append( src );
    iDestinations.Append( dst );
    
    TRACE( T_LIT( "CBSIBYWriter::SetFileItemL end") );
    }

// ---------------------------------------------------------------------------
// CBSIBYWriter::WriteIBYFile()
// ---------------------------------------------------------------------------
//
void CBSIBYWriter::WriteIBYFileL( RFs& aFs, const TDesC& aFileName )
    {
	TRACE( T_LIT( "CBSIBYWriter::WriteIBYFileL begin") );
	
    TInt count = iSourceFiles.Count();
    if( count == 0 )
    	{
    	TRACE( T_LIT( "CBSIBYWriter::WriteIBYFileL - No file elements, nothing to do.") );
    	return;
    	}
    if( count != iDestinations.Count() )
        {
		TRACE( T_LIT("Internal error: IBY filename count mismatch ( %d != %d )"),
            count, iDestinations.Count() );
        User::Leave( KErrCorrupt );
        }

	RFile outfile;
	TInt err = outfile.Open( aFs, aFileName, EFileWrite );
	if( err == KErrNotFound )
		{
		TRACE( T_LIT( "CBSIBYWriter::WriteIBYFileL - IBY file not initialized!") );
		// if the file is not initialized -> not ready
		err = KErrNotReady;
		}
	User::LeaveIfError( err );

    // write data
    TInt size = -1;
    User::LeaveIfError( outfile.Size( size ) );

    outfile.Write( size, KIBYEmptyLine );
    
	TRACE( T_LIT( "CBSIBYWriter::WriteIBYFileL - start writing files to IBY") );
    
    for( TInt i = 0; i < count; i++ )
        {
        outfile.Write( KIBYFileItem );
        outfile.Write( KIBYBaseSource );
        outfile.Write( iSourcePath[i]->Des() );
        outfile.Write( iSourceFiles[i]->Des() );
        outfile.Write( KIBYSpace );
        outfile.Write( iDestinations[i]->Des() );
        TRACE( T_LIT( "CBSIBYWriter::WriteIBYFileL - %S"), iDestinations[i] );
        }

	TRACE( T_LIT( "CBSIBYWriter::WriteIBYFileL - writing IBY file footer") );
    outfile.Write( KIBYEmptyLine );
    outfile.Write( KIBYFooter );
    outfile.Write( KIBYHeaderEnd );

    // cleanup
    outfile.Close();
    TRACE( T_LIT( "CBSIBYWriter::WriteIBYFileL end") );
    }

// ---------------------------------------------------------------------------
// CBSIBYWriter::WriteIBYFile()
// ---------------------------------------------------------------------------
//
void CBSIBYWriter::InitIbyFileL( RFs& aFs, const TDesC& aFileName )
	{
	TRACE( T_LIT( "CBSIBYWriter::InitIbyFileL begin") );
	HBufC* temp = aFileName.AllocL();
	delete iIbyFile;
	iIbyFile = temp;

    TInt err = aFs.MkDir( KBSIbyDirectory );
    if( err == KErrAlreadyExists )
    	{
    	err = KErrNone;
    	}
    User::LeaveIfError( err );
	
	TRACE( T_LIT( "CBSIBYWriter::InitIbyFileL IBY directory (%S) created"), &KBSIbyDirectory() );
	
	// Open file for writing
	RFile outfile;
    User::LeaveIfError( outfile.Replace( aFs, aFileName, EFileWrite ) );
    
    TRACE( T_LIT( "CBSIBYWriter::InitIbyFileL IBY file (%S) created"), &aFileName );
    
    outfile.Write( KIBYHeader1 );
    outfile.Write( KIBYHeaderEnd );
    outfile.Write( KIBYHeader2 );
    outfile.Write( KIBYHeaderEnd );
    outfile.Close();
    TRACE( T_LIT( "CBSIBYWriter::InitIbyFileL end") );
	}
// End of file

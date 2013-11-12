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
* Description:   Global methods for brandimporter
*
*/



#include <e32base.h>
#include <e32cons.h>
#include <bacline.h>
#include <badesca.h>

#include "cbsimportmanager.h"
#include "importlogwriter.h"

namespace
{

// CONSTANTS
//
_LIT( KProcessName,         "BSImport" );
_LIT( KTxtTitle,            "BrandServer XML importer" );
_LIT( KTxtFormatFailed,     "\nimport failed: leave code = %d" );
_LIT( KTxtOK,               "\nok");
_LIT( KTxtPressAnyKey,      "\n [press any key]" );
_LIT( KInstallFiles,        "*.xml");
_LIT( KDirSeparator,        "\\" );

// SETTING SWITCHES (use uppercase)
//
_LIT( KInputDir,           "-D" );

}

// ======== GLOBAL FUNCTIONS ========

// ---------------------------------------------------------------------------
// DeleteRPtrArray
// ---------------------------------------------------------------------------
//
LOCAL_C void DeleteRPtrArray( TAny* aArray )
    {
    RPointerArray<HBufC>* array = static_cast< RPointerArray<HBufC>* >(aArray);
    TInt count = array->Count();
    for( TInt i = 0; i < count; i++ )
        {
        delete (*array)[i];
        }
    array->Reset();
    }
    
// ---------------------------------------------------------------------------
// ParseInstallableBrandsL
// ---------------------------------------------------------------------------
//
LOCAL_C void ParseInstallableBrandsL( RFs& aFs, 
                                      RPointerArray<HBufC>& aFiles, 
                                      const TDesC& aDirectory )
    {
    // list import content
    TParse file;
    file.Set( KInstallFiles, &aDirectory, NULL );
    
    CDir* dir = NULL;
    User::LeaveIfError( 
        aFs.GetDir( file.FullName(), KEntryAttNormal, ESortNone, dir ) );
    CleanupStack::PushL( dir );
    
    // create array of files (with full path)
    TInt count = dir->Count();
    for( TInt i = 0; i < count; i++ )
        {
        file.Set( (*dir)[i].iName, &aDirectory, NULL );
        aFiles.Append( file.FullName().AllocL() );
        }

    CleanupStack::PopAndDestroy( dir );
    }

// ---------------------------------------------------------------------------
// ParseCommandLineArgumentsL
// ---------------------------------------------------------------------------
//
LOCAL_C void ParseCommandLineArgumentsL( RPointerArray<HBufC>& aFiles, 
                                         CConsoleBase* aConsole )
    {
	// Get arguments
	CCommandLineArguments *pCmd = CCommandLineArguments::NewL();
    CleanupStack::PushL( pCmd );

	// check arguments
	if( pCmd->Count() == 1 )
	    {
    	aConsole->Printf( _L("Error: No files to import!\nGive import files as arguments") ); // CSI: 78 # See above
    	aConsole->Printf( KTxtPressAnyKey );
    	aConsole->Getch();
    	User::Leave( KErrArgument );
	    }

    // open file session
    // Codescanner warning: use of non-pointer/reference RFs
    // this code cannot get file server handle from anywhere so it has to be created here
    RFs fs; // CSI: 76 # See above
    User::LeaveIfError( fs.Connect() );
    CleanupClosePushL( fs );
	    
	// Check all arguments for switches
	// Skip first argument since it only contains the name of the executable
	TInt i = 0;
	while( ++i < pCmd->Count() )
	    {
        RBuf arg;
	    arg.CreateL( pCmd->Arg( i ) );
	    arg.CleanupClosePushL();
	    arg.UpperCase();
	    
		// Input directory: "-D [directory]"
		if ( arg.CompareF( KInputDir ) == 0 ) 
			{
            if( pCmd->Count() > i + 1 )
                {
                // get all files from given directory
                TPath path( pCmd->Arg( i + 1 ) );
                if( path.Right( 1 ).Compare( KDirSeparator ) != 0 )
                    {
                    path.Append( KDirSeparator );
                    }
                ParseInstallableBrandsL( fs, aFiles, path );
                i++; // skip to next argument
                }
            else
                {
                // no content after -d switch
    			aConsole->Printf( _L("Argument missing after '-d' switch") ); // CSI: 78 # debug print
    			aConsole->Printf( KTxtPressAnyKey );
    			aConsole->Getch();
    			User::Leave( KErrArgument );
                }
			}
	    else  // argument is just plain file - add it.
	        {
	        aFiles.Append( pCmd->Arg( i ).AllocL() );
	        }
	    
	    CleanupStack::PopAndDestroy(); // arg
	    }

    CleanupStack::PopAndDestroy( 2 ); // pCmd, fs
    }

// ---------------------------------------------------------------------------
// MainL
// ---------------------------------------------------------------------------
//
LOCAL_C void MainL( CConsoleBase* aConsole )
    {
    // Read settings
    RPointerArray<HBufC> files;
    CleanupStack::PushL( TCleanupItem( DeleteRPtrArray, &files ) );
    ParseCommandLineArgumentsL( files, aConsole );

    // convert PointerArray to MDesCArray
    TInt count = files.Count();
    CDesCArrayFlat* importArray = new (ELeave) CDesCArrayFlat( count );
    CleanupStack::PushL( importArray );
    for( TInt i = 0; i < count; i++ )
        {
        importArray->AppendL( files[i]->Des() );
        }
            
    // Do import
    CBSImportManager* importer = CBSImportManager::NewLC();
    importer->ImportFileL( *importArray );
    CleanupStack::PopAndDestroy( 3 ); //importer, importarray, files
	} 

// ---------------------------------------------------------------------------
// doMainL
// ---------------------------------------------------------------------------
//
LOCAL_C TInt doMainL()
    {
	CConsoleBase* console = Console::NewL( KTxtTitle, TSize( KConsFullScreen, KConsFullScreen ) );
	CleanupStack::PushL( console );
	TRAPD( error, MainL( console ) );
	if( error )
	    {
	    IMPORT_DP( D_IMPORT_LIT( "ERROR importing brand: %d" ), error );
		console->Printf( KTxtFormatFailed, error );
	    }
	else
        {
    	console->Printf( KTxtOK );
        }
	
	// Pause for development use. 
	// Disable this when releasing
	//-----------------------------------------------------
	console->Printf( KTxtPressAnyKey );
	//-----------------------------------------------------
	
	CleanupStack::PopAndDestroy(); // console
    return error;
    }

// ---------------------------------------------------------------------------
// Main function of the application executable.
// ---------------------------------------------------------------------------
//
GLDEF_C TInt E32Main()
    {
	__UHEAP_MARK;
    IMPORT_DP_TXT("----- NEW IMPORT SESSION ------( IMPORTER )---");
	CTrapCleanup* cleanup = CTrapCleanup::New();
	TInt returnCode( KErrNone );
    User::RenameThread( KProcessName );
	TRAPD( error, returnCode = doMainL() );
    IMPORT_DP_TXT("-----------------------------------------------");
	__ASSERT_ALWAYS( !error, User::Panic( KProcessName, error ) );
	delete cleanup;
	__UHEAP_MARKEND;
	return returnCode;
    }

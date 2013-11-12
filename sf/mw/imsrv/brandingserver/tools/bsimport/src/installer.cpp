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
* Description:   Global methods for brandinstaller
*
*/



#include <e32base.h>
#include <eikenv.h>
#include <f32file.h>
#include <e32property.h>

#include "cbsimportmanager.h"
#include "importlogwriter.h"
#include "bsbrandremover.h"

namespace
{

// CONSTANTS
//
_LIT( KProcessName,         "BSInstall" );

// Search these files for processing
_LIT( KInstallFiles,        "*.xml" );

// Path to installed xml files inside own private folder. 
_LIT( KInstallPath,         "import\\" );
_LIT( KInstallDrive,        "C:" );

// separators for uninstalled brands
_LIT( KLineFeed,            "\n" );
_LIT( KBrandSeparator,      "$" );

// Pub & sub keys for uninstall buffer
const TInt  KBrandingServerUid  = 0x102828DD;
const TUint KUninstallKey       = 0x01;
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
LOCAL_C void ParseInstallableBrandsL( RFs& aFs, RPointerArray<HBufC>& aFiles )
    {
    IMPORT_DP_TXT("ParseInstallableBrandsL begin");
    // list import content
    CDir* dir = NULL;
    TPath privatePath;
    aFs.PrivatePath( privatePath );
    privatePath.Insert( 0, KInstallDrive );
    privatePath.Append( KInstallPath );

    TFileName find( privatePath );
    find.Append( KInstallFiles );
    User::LeaveIfError( 
        aFs.GetDir( find, KEntryAttNormal, ESortNone, dir ) );
    CleanupStack::PushL( dir );
    
    // create array of files (as text)
    TInt count = dir->Count();
    for( TInt i = 0; i < count; i++ )
        {
        TFileName file( privatePath );
        file.Append( (*dir)[i].iName );
        aFiles.Append( file.AllocL() );
        }

    CleanupStack::PopAndDestroy( dir );
    IMPORT_DP_TXT("ParseInstallableBrandsL end");
    }

// ---------------------------------------------------------------------------
// Uninstall
// ---------------------------------------------------------------------------
//
LOCAL_C void UninstallL()
    {
    
    // Get uninstalled brands
    HBufC* dataBuf = HBufC::NewLC( RProperty::KMaxPropertySize );
    
    HBufC* newDataBuf = HBufC::NewLC( RProperty::KMaxPropertySize );
    TPtr newData( newDataBuf->Des() );
    TBool error( EFalse );
    
    TPtr data( dataBuf->Des() );
    RProperty::Get( TUid::Uid( KBrandingServerUid ), KUninstallKey, data );
    
    //IMPORT_DP( D_IMPORT_LIT( "Property data='%S'" ), &data );
    
    TPtrC processing( data );
    while( processing.Length() > 0 )
        {
        // get first brand from buffer
        TInt pos = processing.Find( KLineFeed );
        TPtrC brand( (pos >= 0) ? processing.Left( pos ) : processing );
        if( pos == KErrNotFound )
            {
            // this was last item. reset process.
            processing.Set( KNullDesC );
            }
        else
            {
            // cut out the brand part from processing
            processing.Set( processing.Mid( brand.Length() + 1 ) );
            }
        
        // divide brand
        TInt sepPos = brand.Find( KBrandSeparator );
        if( sepPos != KErrNotFound )
            {
            // remove brand
            TRAPD( err, BSBrandRemover::RemoveBrandL( 
                brand.Left( sepPos ), brand.Mid( sepPos + 1 ) ) );
            //If the brand removal failed and due to a KErrInUse errorcode, 
            //it must not be deleted from the property-->try to delete next
            //time again! in any other case, we simply delete the un-
            //uninstallation string from the RProperty.
            //NOTE also: Since we check here for an errorcode that is (probably)
            //raised in CBSSession.cpp by calling User::Leave, it has to be 
            //considered whether the condition of the following if statement 
            //has to be updated or not, if the errorcode of that User::Leave
            //in CBSSession.cpp is modified.
            if ( KErrInUse == err )
                {
                error = ETrue;
                if( newData.Length() > 0 )
                    {
                    newData.Append( KLineFeed );
                    }
                newData.Append( brand );                
                }
            }
        }
    
    // Reset uninstall buffer
    if ( error )
        {
        if( newData.Length() <= RProperty::KMaxPropertySize )
            {
            User::LeaveIfError( RProperty::Set( TUid::Uid( KBrandingServerUid ), 
                                                KUninstallKey, 
                                                newData ) );            
            }
        }
    else
        {
    	RProperty::Set( TUid::Uid( KBrandingServerUid ), 
                        KUninstallKey, 
                        KNullDesC );        
        }    
    CleanupStack::PopAndDestroy( newDataBuf );
    CleanupStack::PopAndDestroy( dataBuf );    
    
    IMPORT_DP_TXT("UninstallL end");
    }

// ---------------------------------------------------------------------------
// MainL
// ---------------------------------------------------------------------------
//
LOCAL_C void MainL()
    {
	IMPORT_DP_TXT("MainL begin");
	// Codescanner warning: User of non-pointer/refer3ence RFs (Id:76)
	// this code cannot get file server handle from anywhere, so it has to be
	// created here
    RFs fs; // CSI: 76 # See above
    User::LeaveIfError( fs.Connect() );
    CleanupClosePushL( fs );
    
    // Uninstall removed brands
    TRAP_IGNORE( UninstallL() );
    
    // find new xml files
    RPointerArray<HBufC> files;
    CleanupStack::PushL( TCleanupItem( DeleteRPtrArray, &files ) );
    ParseInstallableBrandsL( fs, files);
    
    // convert PointerArray to MDesCArray
	
    TInt count = files.Count();
    if(count)
    	{
    
	    // Do install
	    CBSImportManager* installer = CBSImportManager::NewLC();
	    
	    for( TInt i = 0; i < count; i++ )
	        {
	        TRAPD( err, installer->ImportFileL( *(files[i]) ) );
	        // only delete the xml file if the installation succeeded; ignore errors.
	        // If there is a reason to save these files, then change this to 
	        // move operation.
	        if ( KErrNone == err )
	            {
	            fs.Delete( *(files[i]) );
	            }	        
	        }
    
	    //User::LeaveIfError( err ); //TODO: should we still leave at this point? 
	    CleanupStack::PopAndDestroy( installer ); 
	    
		} 
	CleanupStack::PopAndDestroy( 2 ); // files,  fs
    IMPORT_DP_TXT("MainL end");
    }

// ---------------------------------------------------------------------------
// Main function of the application executable.
// ---------------------------------------------------------------------------
//
GLDEF_C TInt E32Main()
    {
	__UHEAP_MARK;
	CTrapCleanup* cleanup = CTrapCleanup::New();
	TInt returnCode( KErrNone );
    IMPORT_DP_TXT("----- NEW IMPORT SESSION ------( INSTALLER )---");
    User::RenameThread( KProcessName );
	TRAPD( error, MainL() );
	if( error )
	    {
	    IMPORT_DP( D_IMPORT_LIT( "ERROR installing brand: %d" ), error );
	    }
    IMPORT_DP_TXT("-----------------------------------------------");
	__ASSERT_ALWAYS( !error, User::Panic( KProcessName, error ) );
	delete cleanup;
	__UHEAP_MARKEND;
	return returnCode;
    }

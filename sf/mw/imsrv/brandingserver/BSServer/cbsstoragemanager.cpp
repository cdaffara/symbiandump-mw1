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
* Description:   Stores element data and writes it to stream
*
*/



#include <s32strm.h>
#include <s32file.h>
#include <f32file.h>

#include <bautils.h>
#include <utf.h>


#include "bsimportconstants.h"
#include "cbsstoragemanager.h"
#include "cbsstorage.h"
#ifdef __WINSCW__
#include "cbsibywriter.h"
#endif
#include "cbsserver.h"
#include "cbssession.h"
#include "mbsupdater.h"
//#include "importlogwriter.h"
#include "DebugTrace.h"


// ======== MEMBER FUNCTIONS ========

// ---------------------------------------------------------------------------
// CBSStorageManager::NewL
// ---------------------------------------------------------------------------
//
CBSStorageManager* CBSStorageManager::NewL( CBSSession* aSession, const TDesC& aAppId )
    {
    CBSStorageManager* self = NewLC( aSession, aAppId );
    CleanupStack::Pop();
    return self;
    }

// ---------------------------------------------------------------------------
// CBSStorageManager::NewLC
// ---------------------------------------------------------------------------
//
CBSStorageManager* CBSStorageManager::NewLC( CBSSession* aSession, const TDesC& aAppId )
    {
    CBSStorageManager* self =
        new (ELeave) CBSStorageManager();
    CleanupStack::PushL( self );
    self->ConstructL( aSession, aAppId );
    return self;
    }

// ---------------------------------------------------------------------------
// CBSStorageManager::ConstructL
// ---------------------------------------------------------------------------
//
void CBSStorageManager::ConstructL( CBSSession* aSession, const TDesC& aAppId )
    {
    TRACE( T_LIT( "CBSStorageManager::ConstructL begin aAppId[%S]"), &aAppId );
    User::LeaveIfError( iFs.Connect() );
 
// RFs::ShareProtected() needs to be called for the session 
// used to open the handle that is going to be shared.
// ShareProtected() was called to a wrong Fs-session in 
// CBSBrandHandler::GetFileL().
    User::LeaveIfError( iFs.ShareProtected() );
    
    iFs.CreatePrivatePath( EDriveC );
    
    iSession = aSession;
    
    
#ifdef __WINSCW__        
    if( !iWriter )
    	{
    	iWriter = CBSIBYWriter::NewL();
    	}
    if( aAppId.Compare( KNullDesC ) )
    	{
    	HBufC* temp = HBufC::NewL( KBSIbyDirectory().Length() +
    							   aAppId.Length() +
    							   KBSIbyExtension().Length() );
		TPtr ptr( temp->Des() );
		ptr.Append( KBSIbyDirectory );
		ptr.Append( aAppId );
		ptr.Append( KBSIbyExtension );
		delete iIbyFile;
		iIbyFile = temp;
	    iWriter->InitIbyFileL( iFs, *iIbyFile );
    	}
#endif
    TRACE( T_LIT( "CBSStorageManager::ConstructL end") );
    }

// ---------------------------------------------------------------------------
// CBSStorageManager::CBSStorageManager
// ---------------------------------------------------------------------------
//
CBSStorageManager::CBSStorageManager() :
    iCurrent( KErrNotFound )
    {
    }

// ---------------------------------------------------------------------------
// CBSStorageManager::~CBSStorageManager
// ---------------------------------------------------------------------------
//
CBSStorageManager::~CBSStorageManager()
    {
    TRACE( T_LIT( "CBSStorageManager::~CBSStorageManager begin") );
    iStorages.ResetAndDestroy();
    iFs.Close();

	delete iAppId;
	delete iBrandId;
	delete iLanguageId;
    delete iFilename;
    
    #ifdef __WINSCW__
    delete iWriter;
    #endif
    
    delete iIbyFile;
    TRACE( T_LIT( "CBSStorageManager::~CBSStorageManager end") );
    }

// ---------------------------------------------------------------------------
// CBSStorageManager::InitTransactionL
// ---------------------------------------------------------------------------
//
void CBSStorageManager::ConnectTransactionL(const TDesC& aAppId,
				    					  const TDesC& aBrandId,
				    					  TLanguage aLanguageId,
    					   				  TBool aAppending, /*= EFalse*/
    					   				  TInt aReserved /*=0*/)
	{
	TRACE( T_LIT( "CBSStorageManager::ConnectTransactionL begin AppId[%S],BrandId[%S]"),&aAppId, &aBrandId );
	if( iActive )
		{
		TBool transactionOk = ETrue;
		
		transactionOk = iReserved == aReserved;

		if( transactionOk )
			{
			transactionOk = iLanguageIdNum == aLanguageId;
			}

		if( transactionOk )
			{
			transactionOk != iAppId->Compare( aAppId );
			}
		
		if( transactionOk )
			{
			transactionOk != iBrandId->Compare( aBrandId );
			}
			
		if( !transactionOk )
			{
			User::Leave( KErrArgument );
			}
		// the same transaction as is ongoing
		return;
		}
	iActive = ETrue;
	
    HBufC* tmpAppId = aAppId.AllocL();
    delete iAppId;
    iAppId = tmpAppId;
    
    HBufC* tmpBrandId = aBrandId.AllocL();
    delete iBrandId;
    iBrandId = tmpBrandId;
    
    iLanguageIdNum = aLanguageId;
    
    TBuf<KLangBufLength> buffer;
    if( aLanguageId < 10 )
    	{
    	// we want two digit languageid
    	buffer.AppendNum( KLeadingZero );
    	}
    buffer.AppendNum( aLanguageId );
    HBufC* tmp = buffer.AllocL();
    delete iLanguageId;
    iLanguageId = tmp;
    
    iReserved = aReserved;
    
    HBufC* tempFile = ConstructFileNameL();
    delete iFilename;
    iFilename = tempFile;
    
    iAppending = aAppending;
	TRACE( T_LIT( "CBSStorageManager::ConnectTransactionL end") );
	}


// ---------------------------------------------------------------------------
// CBSStorageManager::CancelTransactionL
// ---------------------------------------------------------------------------
//
void CBSStorageManager::CancelTransactionL()
	{
	TRACE( T_LIT( "CBSStorageManager::CancelTransactionL begin") );
	if( !iActive )
		{
		User::Leave( KErrNotFound );
		}
	// this closes the transaction
	CleanupTransaction();
	TRACE( T_LIT( "CBSStorageManager::CancelTransactionL end") );
	}

// ---------------------------------------------------------------------------
// CBSStorageManager::ConstructFileNameL()
// ---------------------------------------------------------------------------
//
HBufC* CBSStorageManager::ConstructFileNameL( TInt aReserved /* = 0 */ )
    {
    HBufC* fileName = NULL;
    if( iAppId && iBrandId && iLanguageId )
        {
        fileName = ConstructFileNameL( *iAppId, *iBrandId, iLanguageIdNum, aReserved );
        }
    return fileName;
    }

// ---------------------------------------------------------------------------
// CBSStorageManager::ConstructFileNameL()
// ---------------------------------------------------------------------------
//
HBufC* CBSStorageManager::ConstructFileNameL( const TDesC& aAppId,
										      const TDesC& aBrandId,
										      TLanguage aLanguage,
										      TInt aReserved /* = 0 */ )
    {
	HBufC* fileName = NULL;

    TBuf<KLangBufLength> langBuf;
	if (aLanguage >= 100)
		User::LeaveIfError (KErrNotFound);
    if( aLanguage < 10 )
    	{
    	// we want two digit languageid
    	langBuf.AppendNum( KLeadingZero );
    	}
    langBuf.AppendNum( aLanguage );
    
    TInt length = aAppId.Length() + aBrandId.Length() +
                  KDefFileName().Length() + KDirSeparator().Length() * 2 +
                  langBuf.Length() + KDot().Length() * 3 + KMaxVersionLenght;
	fileName = HBufC::NewLC( length );
    
    if(fileName){
    TPtr file( fileName->Des() );

    // [application_id]\[brand_id]\[def_filename][language_id]
    //
    
    file.Append( aAppId );
    file.Append( KDirSeparator );
    file.Append( aBrandId );
    file.Append( KDirSeparator );
    iDir.Set( file.Left( file.Length() ) );
    file.Append( KDefFileName );
    file.Append( langBuf );
    if( aReserved > 0 )
    	{
	    TBuf<KMaxVersionLenght> versionBuffer;
	    versionBuffer.AppendNum( aReserved );
	    file.Append( KDot() );
    	file.Append( versionBuffer );
    	}
    	
    	CleanupStack::Pop( fileName );      
    }
    return fileName;
    }

// ---------------------------------------------------------------------------
// CBSStorageManager::CreateStorageL
// ---------------------------------------------------------------------------
//
void CBSStorageManager::CreateStorageL()
    {
    CBSStorage* tmp = CBSStorage::NewL();
    iStorages.Append( tmp );
    iCurrent = iStorages.Find( tmp );
    }

// ---------------------------------------------------------------------------
// CBSStorageManager::StorageCount
// ---------------------------------------------------------------------------
//
TInt CBSStorageManager::StorageCount()
    {
    return iStorages.Count();
    }

// ---------------------------------------------------------------------------
// CBSStorageManager::Storage
// ---------------------------------------------------------------------------
//
CBSStorage* CBSStorageManager::Storage( TInt aIndex )
    {
    return iStorages[ aIndex ];
    }

// ---------------------------------------------------------------------------
// CBSStorageManager::Storage
// ---------------------------------------------------------------------------
//
CBSStorage* CBSStorageManager::Storage()
    {
    if( iStorages.Count() > 0 )
    	{
    	return iStorages[ iCurrent ];	
    	}
    return NULL;
    }

// ---------------------------------------------------------------------------
// CBSStorageManager::WriteStorageFilesL
// ---------------------------------------------------------------------------
//
TInt CBSStorageManager::WriteStorageFilesL()
    {
    TRACE( T_LIT( "CBSStorageManager::WriteStorageFilesL begin") );
    if( !iActive )
    	{
    	User::Leave( KErrNotFound );
    	}
    TInt count = iStorages.Count();
    for( TInt i = 0; i < count; i++ )
        {
        // get the private path
        TBuf<KMaxPath> path;
        User::LeaveIfError( iFs.PrivatePath( path ) );
        
        // construct absolute filename
        HBufC* fullname = FullFileNameLC( *iFilename, ETrue );

        TParse filestorename;
        iFs.Parse( *fullname, filestorename );

	    RFileWriteStream outstream;

		TInt currentVersion = iReserved;
		TInt newVersion = iReserved;
		// check if the brand has any active clients
		TBool brandActive = ETrue;
		if( iSession )
			{
			CBSServer* server = iSession->Server();
			if( server )
				{
				brandActive = server->MatchSessionL( *iAppId, *iBrandId,
													 iLanguageIdNum, iSession, currentVersion );					
				}
			}

		if( BaflUtils::FileExists( iFs, filestorename.FullName() ) )
			{
			currentVersion = ReadVersionL( filestorename.FullName() );
			
			if( iReserved == KBSAutomaticVersionUpdate && brandActive )
				{

				// the brand has active clients
				// check if there is already a versioned file for this version
				HBufC* fileName = FindAvailableVersionL( currentVersion + 1, newVersion );
				CleanupStack::PushL( fileName);
				
				HBufC* newName = FullFileNameLC( *fileName, ETrue );

		        iFs.Parse( *newName, filestorename );
		        
				if( BaflUtils::FileExists( iFs, filestorename.FullName() ) )
					{
					// the file already exists
					User::Leave( KErrAlreadyExists );
					}
				CleanupStack::PopAndDestroy( 2, fileName ); // newName, fileName
				}
				
			else if( ( iReserved == currentVersion ) )
				{
				// this version is already installed
				User::Leave( KErrAlreadyExists );
				}
				
			else
				{
				// check if the current version brand is active
				brandActive = iSession->Server()->MatchSessionL( *iAppId, *iBrandId,
													 iLanguageIdNum, iSession, currentVersion );
				// new version
				if( brandActive )
					{
					// brand is active
					// check if there is already a versioned file for this version
					HBufC* fileName = ConstructFileNameL( iReserved );
					CleanupStack::PushL( fileName );
					
			        HBufC* newName = FullFileNameLC( *fileName, ETrue );
			        
			        iFs.Parse( *newName, filestorename );
			        			        
					if( BaflUtils::FileExists( iFs, filestorename.FullName() ) )
						{
						// the file already exists
						User::Leave( KErrAlreadyExists );
						}
					CleanupStack::PopAndDestroy( 2, fileName ); // newName, fileName
					}
				}
			}
			
 
		// write a new brand file
		
	    // Construct the output file & stream.
	    iFs.MkDirAll( filestorename.DriveAndPath() );

        TInt err = outstream.Replace( iFs, filestorename.FullName(), EFileWrite );


        if( err )
            {
			TRACE( T_LIT("ERROR: Cannot create branding file: '%S'"), fullname );

            User::Leave( err );
            }

        CleanupClosePushL( outstream );
		
		if( iReserved == KBSAutomaticVersionUpdate )
			{
			// set the next version
			iStorages[i]->SetVersion( newVersion );
			iReserved = newVersion;
			}
		
        // Write data to file
        TRAPD( extError, iStorages[i]->ExternalizeL( outstream ) );
		if( extError && !iAppending )
			{
			// there was an error externalizing the data and we are installing
			// delete the file, since it's not complete
			iFs.Delete( filestorename.DriveAndPath() );
			}
		User::LeaveIfError( extError );


#ifdef __WINSCW__
        // Add to IBY file
        iWriter->SetFileItemL( *fullname, *fullname );
#endif
            
        CleanupStack::PopAndDestroy( 2 ); // fullname, outstream


		// existing brand modified --> inform server 
		if( iSession )
			{
			CBSServer* server = iSession->Server();
			if( server )
				{
				server->BrandUpdatedL( *iAppId, *iBrandId,
									   iLanguageIdNum, iSession, iReserved );					
				}
			}
        }
    
    // this closes the transaction
	CleanupTransaction();
	TRACE( T_LIT( "CBSStorageManager::WriteStorageFilesL end") );
	return iReserved;
    }


// ---------------------------------------------------------------------------
// CBSStorageManager::ReadVersionL
// ---------------------------------------------------------------------------
//
TInt CBSStorageManager::ReadVersionL( const TDesC& aFilename )
	{
    RFile handle;
    User::LeaveIfError( handle.Open( iFs, aFilename, EFileRead ) );
    CleanupClosePushL( handle );
	RFileReadStream stream;
	stream.Attach( handle );
	CleanupClosePushL( stream );
	TInt version = stream.ReadInt16L();
	CleanupStack::PopAndDestroy( 2 ); // stream, handle
	return version;
	}

// ---------------------------------------------------------------------------
// CBSStorageManager::FindAvailableVersionL
// ---------------------------------------------------------------------------
//	
HBufC* CBSStorageManager::FindAvailableVersionL( TInt /*aCurrentVersion*/, TInt& aNewVersion )
	{
	HBufC* fileName =  NULL;
	TInt newVersion = GetNewestVersionL( *iAppId, *iBrandId, iLanguageIdNum );
	aNewVersion = newVersion++;
	fileName = ConstructFileNameL( aNewVersion );
	
	return fileName;
	}


// ---------------------------------------------------------------------------
// CBSStorageManager::CleanupTransaction
// ---------------------------------------------------------------------------
//
void CBSStorageManager::CleanupTransaction()
	{
	iActive = EFalse;
	delete iAppId;
	iAppId = NULL;
	delete iBrandId;
	iBrandId = NULL;
	delete iLanguageId;
	iLanguageId = NULL;
    delete iFilename;
    iFilename = NULL;

    iStorages.ResetAndDestroy();
	}


TInt CBSStorageManager :: isActualBrandInstalledL ( const TDesC& aAppId, const TDesC& aBrandId, TLanguage aLanguage)
{
	TInt actualBrandInstalled = -1;
	HBufC *actualBrandFileName = ConstructFileNameL(aAppId, aBrandId, aLanguage);
	CleanupStack::PushL( actualBrandFileName );
	HBufC* newName = FullFileNameLC( *actualBrandFileName , ETrue );
	if (newName)
		{
		RFile handle;
		TInt err = handle.Open( iFs, *newName, EFileShareAny );
		if (KErrNone == err)
			{
			actualBrandInstalled = 1;
			handle.Close ();
			}
		}
	
	CleanupStack::PopAndDestroy (2);
	return actualBrandInstalled;
}
// ---------------------------------------------------------------------------
// CBSStorageManager::BrandHandleL
// ---------------------------------------------------------------------------
//
void CBSStorageManager::BrandHandleL( const TDesC& aAppId,
									  const TDesC& aBrandId,
									  TLanguage aLanguageId,
									  RFile& aFile,
									  TInt aReserved )
	{
    TRACE( T_LIT( "CBSStorageManager::BrandHandleL begin AppId[%S],BrandId[%S]"),&aAppId, &aBrandId );
    TBuf<KLangBufLength> langBuf;
	if (aLanguageId >= 100)
		User::LeaveIfError( KErrNotFound );
    if( aLanguageId < 10 )
    	{
    	// we want two digit languageid
    	langBuf.AppendNum( KLeadingZero );
    	}
    langBuf.AppendNum( aLanguageId );
    
    TInt length = aAppId.Length() + aBrandId.Length() +
                  KDefFileName().Length() + KDirSeparator().Length() * 2 +
                  langBuf.Length();
                  
	HBufC* fileName = HBufC::NewLC( length );
	
    TPtr filePtr( fileName->Des() );

    // [application_id]\[brand_id]\[def_filename][language_id]
    //
    filePtr.Append( aAppId );
    filePtr.Append( KDirSeparator );
    filePtr.Append( aBrandId );
    filePtr.Append( KDirSeparator );

    TPtrC directory;
    directory.Set( filePtr.Left( filePtr.Length() ) );
    
    filePtr.Append( KDefFileName );
    filePtr.Append( langBuf );

	HBufC* fullname  = FullFileNameLC( filePtr );

    RFile handle;

	TBool versioned = EFalse;

	TParse findname;
	iFs.Parse( *fullname, findname );

	TFileName fullPtr = fullname->Des();


	// seems like nearestlanguagefile is only 
	// searching from z-drive by default, unless 
	// some other drive is explicitely defined
	// for that reason we first try from
	// default drive and then force c-drive if
	// not found.
	BaflUtils::NearestLanguageFile( iFs, fullPtr );

	TInt currentVersion = 0;
	TRAPD( err, currentVersion = ReadVersionL( fullPtr ) );
	if( ( err == KErrNotFound ) || ( err == KErrPathNotFound ) )
		{
		// not found, we try forcing c-drive
		HBufC* driveForced  = FullFileNameLC( filePtr, ETrue );
		TFileName forcedPtr = driveForced->Des();
		BaflUtils::NearestLanguageFile( iFs, forcedPtr );
		TRAP( err, currentVersion = ReadVersionL( forcedPtr ) );
		
		if( err == KErrPathNotFound )
			{
			// switching the error code to be more clear
			err = KErrNotFound;
			}
		User::LeaveIfError( err );
		
		// this file was found, we can use this
		CleanupStack::Pop( driveForced );
		CleanupStack::PopAndDestroy( fullname );
		fullname = driveForced;
		CleanupStack::PushL( fullname );
		}
	else
		{
		User::LeaveIfError( err );
		}
	err = KErrNone;
	if( currentVersion != aReserved ) 
		{
		// not the base version, check if we have a versioned file for this
		HBufC* versionedName = ConstructFileNameL( aAppId, aBrandId, 
												   aLanguageId, aReserved );
		CleanupStack::PushL( versionedName );
		HBufC* fullVersionedName  = FullFileNameLC( *versionedName, ETrue );
		
		err = handle.Open( iFs, *fullVersionedName, EFileShareAny );
		CleanupStack::Pop( fullVersionedName );
		CleanupStack::PopAndDestroy( versionedName );
		CleanupStack::PopAndDestroy( fullname );
		fullname = fullVersionedName;
		CleanupStack::PushL( fullname );
		versioned = ETrue;
		}
	else
		{
		err = handle.Open( iFs, *fullname, EFileShareAny );
		}

	if( err == KErrBadName )
		{
		// change bad name to not found
		err = KErrNotFound;
		}
	User::LeaveIfError( err );
    
    aFile = handle;
    
    if( iSession )
    	{
    	CBSServer* server = iSession->Server();
    	if( server )
    		{
    		server->RegisterFileForSessionL( iSession, *fullname, versioned );
    		}
    	}
    
    CleanupStack::PopAndDestroy( 2, fileName ); // fullname, fileName
	TRACE( T_LIT( "CBSStorageManager::BrandHandleL end") );
	}
	
// ---------------------------------------------------------------------------
// CBSStorageManager::FileElementHandleL
// ---------------------------------------------------------------------------
//
void CBSStorageManager::FileElementHandleL( const TDesC& aAppId,
						     const TDesC& aBrandId,
						     const TDesC& aFileName,
						     const TDesC& aLanguageBuf,
						     RFile& aFile )
	{
    TRACE( T_LIT( "CBSStorageManager::FileElementHandleL begin AppId[%S],BrandId[%S],Filename[%S]"),&aAppId, &aBrandId, &aFileName );
// Modified file name format: "r[n].filename.ext",
// where "n" is the language code.
// Format used to be "filename.ext.r[n]".
    HBufC* internalFn = HBufC::NewLC( aFileName.Length() 
        + KBSFileLangSuffix().Length()
        + aLanguageBuf.Length()
        + KDot().Length() );
    TPtr pInternalFn( internalFn->Des() );    
    pInternalFn.Append( KBSFileLangSuffix() );
    pInternalFn.Append( aLanguageBuf );	
    pInternalFn.Append( KDot() );
    pInternalFn.Append( aFileName );
    
    TInt length = aAppId.Length() 
        + aBrandId.Length() 
        + pInternalFn.Length()  
        + ( KDirSeparator().Length() * 3 )
        + KFileElementStore().Length();
                  
	HBufC* fileName = HBufC::NewL( length ); // replaced NewLC with NewL
	
    TPtr filePtr( fileName->Des() );

    // [application_id]\[brand_id]\files\[aFilename]
    //
    filePtr.Append( aAppId );
    filePtr.Append( KDirSeparator );
    filePtr.Append( aBrandId );
    filePtr.Append( KDirSeparator );
    filePtr.Append( KFileElementStore );
    filePtr.Append( KDirSeparator );
    filePtr.Append( pInternalFn ); 

    CleanupStack::PopAndDestroy( internalFn );
    internalFn = NULL;
    pInternalFn.Set( 0, 0, 0 );
    
    CleanupStack::PushL( fileName );
	HBufC* fullname = FullFileNameLC( filePtr );
    
// Make a TPtr instead of TFileName because after mod, "fullPtr"
// does not need to be passed to BaflUtils::NearestLanguageFile()
// old implementation:  TFileName fullPtr = fullname->Des();
    TPtr fullPtr( fullname->Des() );
    
	
//  A few thoughts about language suffix of files:
// NOTE: NOT about brand file (e.g. "brandfile.bin.r99" ),
// but about other files which belong to the branding package, 
// since language suffix added by branding server to a
// mif-file is not acceptable later when calling 
// AknIconUtils::CreateIconLC() on the mif-file.
// Skipped call to BaflUtils::NearestLanguageFile() because "XSP MOD" 
// moves language code from suffix to prefix.
// NearestLanguageFile()-call could be replaced by an op where 
// manager seeks a file only from c and z, or from all drives,
// but does so only for file matching to current or requested  
// language. Meaning will not try to get the nearest language 
// file but the matching one.
// If nearest language file is actually needed, then 
// manager could for example implement a seek-function similar to 
// BaflUtils::NearestLanguageFile(), but with the difference that
// it will inspect file prefix instead of suffix.


    RFile handle;
// RFs::ShareProtected() needs to be called for the Fs-session 
// used to open the handle that is about to be shared.
// ShareProtected() was called to a wrong Fs session in 
// CBSBrandHandler::GetFileL()!
	TInt err = handle.Open( iFs, fullPtr, EFileShareReadersOnly );
	if( err == KErrPathNotFound )
		{
		// not found, we try forcing c-drive
		HBufC* driveForced  = FullFileNameLC( filePtr, ETrue );
		
		TFileName forcedPtr = driveForced->Des();

		
		err = handle.Open( iFs, forcedPtr, EFileShareReadersOnly );
		User::LeaveIfError( err );
		// this file was found, we can use this
		CleanupStack::Pop( driveForced );
		CleanupStack::PopAndDestroy( fullname );
		fullname = driveForced;
		CleanupStack::PushL( fullname );
		}
	else
		{
		User::LeaveIfError( err );
		}

    CleanupStack::PopAndDestroy( 2, fileName ); // fullname, fileName
    aFile = handle;
	TRACE( T_LIT( "CBSStorageManager::FileElementHandleL end") );
	}
	
	
// ---------------------------------------------------------------------------
// CBSStorageManager::FullFileNameLC
// ---------------------------------------------------------------------------
//
HBufC* CBSStorageManager::FullFileNameLC( const TDesC& aFilename, TBool aForceCDrive /*= EFalse*/ )
	{
    // get the private path
    TBuf<KMaxPath> path;
    User::LeaveIfError( iFs.PrivatePath( path ) );

    // construct absolute filename
    HBufC* fullname = HBufC::NewLC( KBSCDrive().Length() + aFilename.Length() + path.Length() + 
    								KBSDataStore().Length() + KDirSeparator().Length() );
    TPtr fullPtr( fullname->Des() );
    fullPtr.Append( KBSCDrive );
    fullPtr.Append( path );
    fullPtr.Append( KBSDataStore );
    fullPtr.Append( KDirSeparator );
    fullPtr.Append( aFilename );
    
	TParse filename;
	iFs.Parse( fullPtr, filename );
	
	if( !aForceCDrive )
		{
		// C-drive not explicitely requested
		if( !BaflUtils::FileExists( iFs, fullPtr ) )
			{
			// the file does not exist in C-drive, so 
			// we have to return a path to Z-drive
			fullPtr.Zero();
		    fullPtr.Append( KBSZDrive );
		    fullPtr.Append( path );
		    fullPtr.Append( KBSDataStore );
		    fullPtr.Append( KDirSeparator );
		    fullPtr.Append( aFilename );
			}
		}
	
    return fullname;
	}
	
// ---------------------------------------------------------------------------
// CBSStorageManager::CleanupFileL
// ---------------------------------------------------------------------------
//
void CBSStorageManager::CleanupFileL( const TDesC& aFileName )
	{
	TRACE( T_LIT( "CBSStorageManager::CleanupFileL begin") );
	// we have a versioned file we need to clean up
	// get the file name without the version information
	if( !IsBaseFileL( aFileName) )
		{
		// this is not a basefile
		// we need to check if basefile should be replaced
		// with this one
		TPtrC baseFile = FilenameWithoutVersion( aFileName );
		TInt version = ReadVersionL( aFileName );
		TInt baseVersion = ReadVersionL( baseFile );
		if( version > baseVersion )
			{
			// newer version --> replace basefile
			TInt err = iFs.Replace( aFileName, baseFile);
			User::LeaveIfError( err );
			}
		else
			{
			// same or older version --> delete
			TInt err = iFs.Delete( aFileName );
			User::LeaveIfError( err );
			}
		}
	else
		{
		// this is a basefile
		// we need to check if there are any newer versioned files
		// that are not in use
		
		TParse filename;
		iFs.Parse( aFileName, filename );
		TPtrC driveAndPath = filename.DriveAndPath();
		
		HBufC* nameWithDrive = HBufC::NewLC( aFileName.Length() + KBSCDrive().Length() );
		TPtr nameWithDrivePtr = nameWithDrive->Des();

		nameWithDrivePtr.Append( aFileName );
		
		CDir* files = NULL;
		CDir* directories = NULL;
		
		User::LeaveIfError( iFs.GetDir( driveAndPath, KEntryAttNormal, ESortByName, files, directories ) );
		
		TInt count = files->Count();
		for( TInt i = 0; i < count; i++ )
			{
			const TEntry file = (*files)[ i ];
			TBufC<KMaxFileName> name = file.iName;
			
			HBufC* fullName = HBufC::NewLC( driveAndPath.Length() + name.Length() );
			TPtr fullPtr = fullName->Des();
			
			fullPtr.Append( driveAndPath );
			fullPtr.Append( name );
			
			TPtrC strippedName = FilenameWithoutVersion( *fullName );
			
			if( 0 == strippedName.Compare( nameWithDrivePtr ) )
				{
				// there is a versioned file, that might be newer
				// than the basefile
				TInt baseVersion = ReadVersionL( nameWithDrivePtr );
				
				TInt versionedVersion = ReadVersionL( fullPtr );
				if( versionedVersion > baseVersion )
					{
					// versioned file newer than base file
					// check if the versioned file is still in use
					if( !iSession->Server()->FileStillInUse( iSession, fullPtr ) )
						{
						// the file is not in use, we can replace the basefile
						// with the versioned file
						TInt err = iFs.Replace( fullPtr, nameWithDrivePtr );
						User::LeaveIfError( err );
						}
					}
				}
			CleanupStack::PopAndDestroy( fullName );
			}
		CleanupStack::PopAndDestroy( nameWithDrive );
		}
	TRACE( T_LIT( "CBSStorageManager::CleanupFileL end") );
	}

// ---------------------------------------------------------------------------
// CBSStorageManager::FilenameWithoutVersionL
// ---------------------------------------------------------------------------
//
TPtrC CBSStorageManager::FilenameWithoutVersion( const TDesC& aFileName )
	{
	TParse filename;
	iFs.Parse( aFileName, filename );
	TPtrC extension = filename.Ext();
	TPtrC strippedName = aFileName.Left( aFileName.Length() - ( extension.Length() ) );
	return strippedName;
	}


// ---------------------------------------------------------------------------
// CBSStorageManager::IsBaseFileL
// ---------------------------------------------------------------------------
//
TBool CBSStorageManager::IsBaseFileL( const TDesC& aFileName )
	{
	TBool returnValue = ETrue;
	TPtrC fileName = FilenameWithoutVersion( aFileName );
	if( BaflUtils::FileExists( iFs, fileName ) )
		{
		// file with one extension stripped out exists
		// --> the file was not a base file
		returnValue = EFalse;
		}
	return returnValue;
	}

// ---------------------------------------------------------------------------
// CBSStorageManager::InstallFile
// ---------------------------------------------------------------------------
//
HBufC* CBSStorageManager::InstallFileLC( const TDesC& aFileName )
	{
	TRACE( T_LIT( "CBSStorageManager::InstallFileLC begin") );
	if( !iAppId || !iBrandId )
		{
		User::Leave( KErrNotReady );
		}
	TParse filename;
	iFs.Parse( aFileName, filename );
	
    TInt length = iAppId->Length() + iBrandId->Length() +
                  aFileName.Length() + KDirSeparator().Length() * 3
                  + KFileElementStore().Length() + iLanguageId->Length()
                  + KDot().Length() + KBSFileLangSuffix().Length();
                  
	HBufC* fileName = HBufC::NewLC( length );
	
    TPtr filePtr( fileName->Des() );

    filePtr.Append( *iAppId );
    filePtr.Append( KDirSeparator );
    filePtr.Append( *iBrandId );
    filePtr.Append( KDirSeparator );
    filePtr.Append( KFileElementStore );
    filePtr.Append( KDirSeparator );
    filePtr.Append( KBSFileLangSuffix() );
	filePtr.Append( *iLanguageId );
    filePtr.Append( KDot() ); 
    filePtr.Append( filename.NameAndExt() ); 
	
	HBufC* fullname  = FullFileNameLC( filePtr, ETrue );
    
    TParse directory;
    iFs.Parse( *fullname, directory );
    TPtrC ptr = directory.DriveAndPath();
    TInt dirError = iFs.MkDirAll( directory.DriveAndPath() );
    if( dirError == KErrAlreadyExists )
    	{
    	// ignore already exists
    	dirError = KErrNone;
    	}
    User::LeaveIfError( dirError );
    
    CFileMan* fileMan = CFileMan::NewL( iFs );
    CleanupStack::PushL( fileMan );
    
    // copy the file, no overwriting
	TInt err = fileMan->Copy( aFileName, *fullname, 1 );
	User::LeaveIfError( err );
		
#ifdef __WINSCW__
        // Add to IBY file
        iWriter->SetFileItemL( *fullname, *fullname );
#endif

    CleanupStack::PopAndDestroy( 3, fileName ); // fileMan, fullname, fileName	


	HBufC* returnValue = filename.NameAndExt().AllocLC();
	TRACE( T_LIT( "CBSStorageManager::InstallFileLC end") );
	return returnValue;
	}


// ---------------------------------------------------------------------------
// CBSStorageManager::RemoveBrandL
// ---------------------------------------------------------------------------
//
void CBSStorageManager::RemoveBrandL(  const TDesC& aAppId,
									   const TDesC& aBrandId,
									   TLanguage aLanguageId,
									   TInt aReserved )
	{
    TRACE( T_LIT( "CBSStorageManager::RemoveBrandL begin") );
    HBufC* fileName = ConstructFileNameL( aAppId,
						       			  aBrandId,
						       			  aLanguageId,
						       			  aReserved );
	CleanupStack::PushL( fileName );
		
    HBufC* fullname = FullFileNameLC( *fileName, ETrue );

	TParse parseName;
	iFs.Parse( *fullname, parseName );
	
	TPtrC directory = parseName.DriveAndPath();

    CFileMan* fileMan = CFileMan::NewL( iFs );
    CleanupStack::PushL( fileMan );
    
    // delete all files under this brand and language
	TInt err = fileMan->RmDir( directory );
	TRACE( T_LIT( "CBSStorageManager::RemoveBrandL dir='%S' err=%d "), &directory, err );
	
	//in case the removal of the entire brand failed because the brand was 
	//in use, we set this brand as discarded (since parts of the package 
	//might have been removed)
    if ( KErrInUse  == err ) 
        {
        MarkBrandAsDiscardedL( aAppId, aBrandId );
        }
	
	User::LeaveIfError( err );

	CleanupStack::PopAndDestroy( 3, fileName );
	TRACE( T_LIT( "CBSStorageManager::RemoveBrandL end ") );
	}

// ---------------------------------------------------------------------------
// CBSStorageManager::RemoveApplicationL
// ---------------------------------------------------------------------------
//
void CBSStorageManager::RemoveApplicationL( const TDesC& aAppId )
	{
    TRACE( T_LIT( "CBSStorageManager::RemoveApplicationL begin") );
    TInt length = aAppId.Length() + 
                  KDefFileName().Length() + KDirSeparator().Length();
                  

	HBufC* fileName = HBufC::NewLC( length );
    
    TPtr file( fileName->Des() );

    file.Append( aAppId );
    file.Append( KDirSeparator );
    
    HBufC* fullName = FullFileNameLC( *fileName, ETrue );
    
    CFileMan* fileMan = CFileMan::NewL( iFs );
    CleanupStack::PushL( fileMan );
    
    // delete all files under this brand and language
	TInt err = fileMan->RmDir( *fullName );
	User::LeaveIfError( err ); 
    
    CleanupStack::PopAndDestroy( 3, fileName );
    TRACE( T_LIT( "CBSStorageManager::RemoveApplicationL end") );
	}


// ---------------------------------------------------------------------------
// CBSStorageManager::NextAvailableVersionL
// ---------------------------------------------------------------------------
//
TInt CBSStorageManager::NextAvailableVersionL()
	{
	TRACE( T_LIT( "CBSStorageManager::NextAvailableVersionL begin") );
	TInt returnValue = 0;
	
	if( iAppId && iBrandId )
		{
		TInt newVersion = 0;
		TRAPD( err, newVersion = GetNewestVersionL( *iAppId, *iBrandId, iLanguageIdNum ) );
		if( err == KErrNotFound )
			{
			// ignore not found -> we default to 1
			err = KErrNone;
			returnValue = iReserved;
			}
		else
			{
			returnValue = newVersion + 1;
			}
		User::LeaveIfError( err );
		}
    TRACE( T_LIT( "CBSStorageManager::NextAvailableVersionL end") );
	return returnValue;
	}


// ---------------------------------------------------------------------------
// CBSStorageManager::NextAvailableVersionL
// ---------------------------------------------------------------------------
//
void CBSStorageManager::SetVersion( TInt aReserved )
	{
	iReserved = aReserved;
	}


// ---------------------------------------------------------------------------
// CBSStorageManager::GetCurrentVersionL
// ---------------------------------------------------------------------------
//
TInt CBSStorageManager::GetNewestVersionL( const TDesC& aAppId,
										   const TDesC& aBrandId,
										   TLanguage aLanguageId,
										   TPtr* aDrive /*= KNullDesC()*/ )
	{
    TRACE( T_LIT( "CBSStorageManager::GetNewestVersionL begin") );
    HBufC* fileName = ConstructFileNameL( aAppId, aBrandId, aLanguageId );
	CleanupStack::PushL( fileName );


	// FullFileNameLC returns the file from C-drive if it exists there
	// and if not, then from ROM
    HBufC* fullFileName = FullFileNameLC( *fileName );

	TParse parse;
	iFs.Parse( *fullFileName, parse );
	TPtrC driveAndPath = parse.DriveAndPath();
	
	CDir* files = NULL;
	CDir* directories = NULL;
	
	TInt err = iFs.GetDir( driveAndPath, KEntryAttNormal, ESortByName, files, directories );
	if(KErrNone == err )
		{
		if(aDrive)
			{
			(*aDrive).Copy(KBSZDrive);	
			}
	    
		}
	if( err == KErrPathNotFound )
		{
		// not found from Z-drive, try from C-drive
		CleanupStack::PopAndDestroy( fullFileName );
		fullFileName = FullFileNameLC( *fileName, ETrue );
		driveAndPath.Set( parse.DriveAndPath() );
		err = iFs.GetDir( driveAndPath, KEntryAttNormal, ESortByName, files, directories );
		if(KErrNone == err )
			{
			if(aDrive)
				{
			    (*aDrive).Copy(KBSCDrive);
				}
			}
		if( err == KErrPathNotFound )
			{
			// switching error code to make it more clear
			err = KErrNotFound;
			}
		// as we are leaving here cleanup the stack
		CleanupStack::PopAndDestroy( 2, fileName ); // fullFileName, fileName
		User::LeaveIfError( err );
		}
	
	// get the base file version
	TInt returnValue = ReadVersionL( *fullFileName );
	
	TInt count = files->Count();
	for( TInt i = 0; i < count; i++ )
		{
		const TEntry file = (*files)[ i ];
		TBufC<KMaxFileName> name = file.iName;
		
		HBufC* fullName = HBufC::NewLC( driveAndPath.Length() + name.Length() );
		TPtr fullPtr = fullName->Des();
		
		fullPtr.Append( driveAndPath );
		fullPtr.Append( name );
		
		TPtrC strippedName = FilenameWithoutVersion( *fullName );
		
		if( 0 == strippedName.Compare( *fullFileName ) )
			{
			// there is a versioned file, that might be newer
			// than the basefile
			TInt versionedVersion = ReadVersionL( fullPtr );
			if( versionedVersion > returnValue )
				{
				// versioned file newer than base file
				returnValue = versionedVersion;
				}
			}
		CleanupStack::PopAndDestroy( fullName );
		}

	CleanupStack::PopAndDestroy( 2, fileName ); // fullFileName, fileName
    
    TRACE( T_LIT( "CBSStorageManager::GetNewestVersionL end") );
	return returnValue;
	}


// ---------------------------------------------------------------------------
// CBSStorageManager::WriteIbyFiles
// ---------------------------------------------------------------------------
//
TInt CBSStorageManager::WriteIbyFiles()
	{
	TInt err = KErrNone;
#ifdef __WINSCW__	
	if( !iWriter )
		{
		return KErrNone;
		}

    TRAP( err, iWriter->WriteIBYFileL( iFs, *iIbyFile ) );
    delete iIbyFile;
    iIbyFile = NULL;
    delete iWriter;
    iWriter = NULL;
	
#endif	
	return err;
	}

// ---------------------------------------------------------------------------
// CBSStorageManager::LockStorage
// ---------------------------------------------------------------------------
//
void CBSStorageManager::LockStorage()
    {
    TRACE( T_LIT( "CBSStorageManager::LockStorage begin") );
    // This will disable all filehandles, so that clients can't use them
    iFs.Close();
    TRACE( T_LIT( "CBSStorageManager::LockStorage end") );
    }
    
// ---------------------------------------------------------------------------
// CBSStorageManager::ReleaseLockL
// ---------------------------------------------------------------------------
//
void CBSStorageManager::ReleaseLockL()
    {
    TRACE( T_LIT( "CBSStorageManager::ReleaseLockL begin") );
    User::LeaveIfError( iFs.Connect() );
    TRACE( T_LIT( "CBSStorageManager::ReleaseLockL end") );
    }
    
// ---------------------------------------------------------------------------
// CBSStorageManager::CheckBrandDiscarded
// ---------------------------------------------------------------------------
//
TBool CBSStorageManager::CheckBrandDiscarded(const TDesC& aAppId,
							const TDesC& aBrandId)
{
	TBool flag = EFalse ;
	
	HBufC *fileName = ConstructDiscardBrandFileNameL(aAppId, aBrandId) ;
	CleanupStack::PushL(fileName);
	HBufC *fullPath = FullDiscardBrandFileNameLC( *fileName ) ;
	
	// crate a flag file
	RFile file;
	TInt err = file.Open(iFs, *fullPath, EFileWrite);
	if(err == KErrNone)
	{
		// file exists.
		file.Close() ;
		flag = ETrue ;
	}
	
	CleanupStack::PopAndDestroy(fullPath) ;
	CleanupStack::PopAndDestroy(fileName) ;
	return flag ;
}

// ---------------------------------------------------------------------------
// CBSStorageManager::ConstructDiscardBrandFileNameL
// ---------------------------------------------------------------------------
//
HBufC* CBSStorageManager::ConstructDiscardBrandFileNameL( const TDesC& aAppId,
										      const TDesC& aBrandId )
    {
	HBufC* fileName = NULL;

    TInt length = aAppId.Length() + aBrandId.Length() +
                  KDiscardBrandFileName().Length() + KDirSeparator().Length() * 2 ;
	fileName = HBufC::NewLC( length );
    
    TPtr file( fileName->Des() );

    // [application_id]\[brand_id]\discardedbrand.txt
    file.Append( aAppId );
    file.Append( KDirSeparator );
    file.Append( aBrandId );
    file.Append( KDirSeparator );
    file.Append( KDiscardBrandFileName );

    if( fileName )
    	{
    	CleanupStack::Pop( fileName );
    	}
    	
    return fileName;
    }
    
// ---------------------------------------------------------------------------
// CBSStorageManager::FullDiscardBrandFileNameLC
// ---------------------------------------------------------------------------
//
HBufC* CBSStorageManager::FullDiscardBrandFileNameLC( TDesC& aFilename )
	{
    // get the private path
    TBuf<KMaxPath> path;
    RFs fs ;
    User::LeaveIfError( fs.Connect() );
    CleanupClosePushL(fs);
    User::LeaveIfError( fs.PrivatePath( path ) );

    // construct absolute filename
    HBufC* fullname = HBufC::NewLC( KBSCDrive().Length() + aFilename.Length() + path.Length() + 
    								KBSDataStore().Length() + KDirSeparator().Length() );
    TPtr fullPtr( fullname->Des() );
    fullPtr.Append( KBSCDrive );
    fullPtr.Append( path );
    fullPtr.Append( KBSDataStore );
    fullPtr.Append( KDirSeparator );
    fullPtr.Append( aFilename );
    
	CleanupStack::Pop(); // fullname
	CleanupStack::PopAndDestroy() ; //fs
	CleanupStack::PushL(fullname) ;
	
    return fullname;
	}

// ---------------------------------------------------------------------------
// CBSStorageManager::MarkBrandAsDiscardedL
// ---------------------------------------------------------------------------
//
void CBSStorageManager::MarkBrandAsDiscardedL( const TDesC& aAppId,
			  			                       const TDesC& aBrandId )
    {
	TRACE( T_LIT( "CBSStorageManager::MarkBrandAsDiscardedL begin app='%S' brand='%S'"), &aAppId, &aBrandId );
	
	HBufC *fileName = ConstructDiscardBrandFileNameL( aAppId, aBrandId ) ;
	CleanupStack::PushL( fileName );
	HBufC *fullPath = FullDiscardBrandFileNameLC( *fileName ) ;
    TRACE( T_LIT( "CBSStorageManager::MarkBrandAsDiscardedL discardedfile='%S'"), &fullPath );
	
	// create flag file
	RFile file;	
	TInt err = file.Replace( iFs, *fullPath, EFileWrite );
	TRACE( T_LIT( "CBSStorageManager::MarkBrandAsDiscardedL discarded file creation result='%d'"), err );
	file.Close();
	
	CleanupStack::PopAndDestroy( fullPath );
	fullPath = NULL;
	CleanupStack::PopAndDestroy( fileName );
	fileName = NULL;
	
	TRACE( T_LIT( "CBSStorageManager::MarkBrandAsDiscardedL begin") );	
    }

// End of file

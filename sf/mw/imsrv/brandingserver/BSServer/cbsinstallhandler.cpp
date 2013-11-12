/*
* Copyright (c) 2006 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  CBSInstallhandler.cpp
*
*/



//  INCLUDE FILES
#include <apgcli.h>
#include <apacmdln.h>
#include <eikenv.h>
#include <bautils.h>
#include <utf.h>
#include <e32property.h>

#include "cbsinstallhandler.h"
#include "DebugTrace.h"
#include "bsimportconstants.h"
#include "cbsstoragemanager.h"


// CONSTANTS
// branding installer application
_LIT( KInstallerApp,            "bsinstall.exe" );


// wildcard for finding installed brand files
_LIT( KBrandWild,               "*" );

// Line feed separates uninstalled brands from each other
_LIT( KLineFeed,                "\n" );


// Pub&Sub Key for uninstalled brands
const TUint KUninstallKey       = 0x01;


// METHODS

// Two-phased constructor.
CBSInstallHandler* CBSInstallHandler::NewL( )
    {
    TRACE( T_LIT( "CBSInstallHandler::NewL begin") );
    CBSInstallHandler* self = new ( ELeave ) CBSInstallHandler() ;
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );
    TRACE( T_LIT( "CBSInstallHandler::NewL end") );
    return self;
    }

// Symbian OS default constructor can leave.
void CBSInstallHandler::ConstructL()
    {
    User::LeaveIfError( iFs.Connect() );
    CActiveScheduler::Add( this );
    }

// Destructor
CBSInstallHandler::~CBSInstallHandler()
    {
    Cancel();
    iFs.Close();
    }

// C++ default constructor can NOT contain any code, that
// might leave.
//
CBSInstallHandler::CBSInstallHandler() :
    CActive( EPriorityIdle )
	{
	}

// -----------------------------------------------------------------------------
// CBSInstallHandler::InstallNewFilesL()
// -----------------------------------------------------------------------------
//
void CBSInstallHandler::InstallNewFilesL()
    {
	TRACE( T_LIT( "CBSInstallHandler::InstallNewFilesL begin") );
    TInt needInstaller = 0;
    
        
    TRAP_IGNORE( needInstaller += CheckForDiscardedBrandsL( KBSDataStore ) );

    // uninstall removed brands
    needInstaller += 
        SyncFilesL( KInstallPath, KInstallObservePath, EInstallDeleteFromSrc );
    
    // install new brands
    needInstaller += 
        SyncFilesL( KInstallObservePath, KInstallPath, EInstallCopyNewToDest );
    
    if( needInstaller )
        {
        // something new was installed
        LaunchInstallerAppL();
        }
	TRACE( T_LIT( "CBSInstallHandler::InstallNewFilesL end") );
    }

// -----------------------------------------------------------------------------
// CBSInstallHandler::StartObservingL()
// -----------------------------------------------------------------------------
//
void CBSInstallHandler::StartObservingL()
	{
	TRACE( T_LIT( "CBSInstallHandler::StartObservingL begin") );
	if( IsActive() )
	    {
	    __ASSERT_DEBUG( EFalse, User::Leave( KErrAlreadyExists ) );
	    return;
	    }
	
	// observe path: (drive:)[private](/import/install)
	TPath path( KNullDesC );
	GetPrivateFolder( path, KInstallObservePath );
    
    iFs.NotifyChange( ENotifyEntry, iStatus, path );
    SetActive();
	TRACE( T_LIT( "CBSInstallHandler::StartObservingL end") );
	}

// -----------------------------------------------------------------------------
// CBSInstallHandler::StopObserving()
// -----------------------------------------------------------------------------
//
void CBSInstallHandler::StopObserving()
	{
	TRACE( T_LIT( "CBSInstallHandler::StopObserving begin") );
    Cancel();
	TRACE( T_LIT( "CBSInstallHandler::StopObserving end") );
	}

// -----------------------------------------------------------------------------
// CBSInstallHandler::DoCancel()
// -----------------------------------------------------------------------------
//
void CBSInstallHandler::DoCancel()
    {
    iFs.NotifyChangeCancel();
    }

// -----------------------------------------------------------------------------
// CBSInstallHandler::RunL()
// -----------------------------------------------------------------------------
//
void CBSInstallHandler::RunL()
    {
	TRACE( T_LIT( "CBSInstallHandler::RunL start") );
    if( iStatus == KErrNone )
        {
        TRACE( T_LIT( "CBSInstallHandler::RunL installing...") );
        TRAP_IGNORE( InstallNewFilesL() );
        StartObservingL();
        }
    else
        {
        TRACE( T_LIT( "CBSInstallHandler::RunL observing stopped") );
        }
	TRACE( T_LIT( "CBSInstallHandler::RunL end") );
    }

// -----------------------------------------------------------------------------
// CBSInstallHandler::LaunchInstallerAppL()
// -----------------------------------------------------------------------------
//
void CBSInstallHandler::LaunchInstallerAppL()
    {
	TRACE( T_LIT( "CBSInstallHandler::LaunchInstallerAppL start") );
    // initialize RApaLsSession
    RApaLsSession apas;
    User::LeaveIfError( apas.Connect() );
    CleanupClosePushL( apas );
    apas.GetAllApps();
    
    // start installer
    CApaCommandLine* command = CApaCommandLine::NewLC();
    command->SetExecutableNameL( KInstallerApp );
    User::LeaveIfError( apas.StartApp( *command ) );
    TRACE( T_LIT( "CBSInstallHandler::LaunchInstallerAppL bsinstall.exe launched OK") );

    CleanupStack::PopAndDestroy( 2 ); // apas, command
	TRACE( T_LIT( "CBSInstallHandler::LaunchInstallerAppL end") );
    }
    
// -----------------------------------------------------------------------------
// CBSInstallHandler::SyncFilesL()
// -----------------------------------------------------------------------------
//
TBool CBSInstallHandler::SyncFilesL( const TDesC& aSrcDir, const TDesC& aDestDir, 
                                     TInstallOperation aOperation )
    {
	TRACE( T_LIT( "CBSInstallHandler::SyncFilesL start") );

    // Check new install folder
    HBufC* fileBuf = HBufC::NewLC( KMaxFileName );
    TPtr file( fileBuf->Des() );
    GetPrivateFolder( file, aSrcDir );
    
    TBool ret = BaflUtils::PathExists( iFs, file );
    if( !ret )
        {
        // install folder doesn't exist.
    	TRACE( T_LIT( 
            "CBSInstallHandler::SyncFilesL no src folder!") );
        CleanupStack::PopAndDestroy( fileBuf );
        return EFalse;
        }

    // Apply extension filter
    file.Append( KBrandWild );       //
    file.Append( KBrandInstallExt ); // *.install

    // Get list of src dir files
    CDir* dir = NULL;
    User::LeaveIfError( iFs.GetDir( file, 
        KEntryAttNormal, ESortNone, dir ) );
    CleanupStack::PushL( dir );
    
    // Create destination directory
    GetPrivateFolder( file, aDestDir );
    BaflUtils::EnsurePathExistsL( iFs, file );
    
    // Compare source dir to destination
    TBool OperationExecuted = EFalse;
    TInt count = dir->Count();

    for( TInt i = 0; i < count; i++ )
        {
        GetPrivateFolder( file, aDestDir );
        file.Append( (*dir)[i].iName );
        if( !BaflUtils::FileExists( iFs, file ) )
            {
            // file does not exist in destionation dir
            // => react according to operation
            HBufC* fileSrcBuf = HBufC::NewLC( KMaxFileName );
            TPtr fileSrc( fileSrcBuf->Des() );
            GetPrivateFolder( fileSrc, aSrcDir );
            fileSrc.Append( (*dir)[i].iName );
            
            switch( aOperation )
                {
                case EInstallCopyNewToDest:
                    {
                    // copy new files from src to destination
                    TRACE( T_LIT( "CBSInstallHandler::SyncFilesL copy") );
                    OperationNotifyL( aOperation, file );
                    User::LeaveIfError( BaflUtils::CopyFile( 
                        iFs, fileSrc, file ) );
                    break;
                    }
                case EInstallDeleteFromSrc:
                    {
                    // delete files from src if they are not found from dest
                    TRACE( T_LIT( "CBSInstallHandler::SyncFilesL del") );
                    OperationNotifyL( aOperation, fileSrc );
                    User::LeaveIfError( BaflUtils::DeleteFile( iFs, fileSrc ) );
                    break;
                    }
                default:
                    {
                    // Every operation should have a case!
                    __ASSERT_DEBUG( EFalse, 
                        User::LeaveIfError( KErrArgument ) );
                    }
                }
            
            OperationExecuted = ETrue;
            CleanupStack::PopAndDestroy( fileSrcBuf );
            }
        }
    CleanupStack::PopAndDestroy( 2 ); // fileBuf, dir
    TRACE( T_LIT( "CBSInstallHandler::SyncFilesL end") );
    return OperationExecuted;
    }

// -----------------------------------------------------------------------------
// CBSInstallHandler::OperationNotifyL()
// -----------------------------------------------------------------------------
//
void CBSInstallHandler::OperationNotifyL( TInstallOperation aOperation, 
                                          const TDesC& aSrcFile )
    {
	TRACE( T_LIT( "CBSInstallHandler::OperationNotifyL start") );
    if( aOperation == EInstallDeleteFromSrc )
        {
        // parse brand id and application id from filename
        TParse parse;
        parse.Set( aSrcFile, NULL, NULL );
        
        // find ids from filename
        TInt firstSepar = parse.Name().Find( KInstallFileDataSeparator );
        TInt secondSepar = parse.Name().
            Mid( firstSepar + 1 ).Find( KInstallFileDataSeparator );
        TInt cutlen = 0;
        secondSepar == KErrNotFound ? cutlen = parse.Name().Length() :
                                      cutlen = firstSepar + secondSepar + 1;
        
        // brandString: [brandId]$[appId]
        TPtrC brandString ( parse.Name().Left( cutlen ) );
        if( IsBrandInstalled( brandString ) )
            {
            TRACE( T_LIT( "CBSInstallHandler::OperationNotifyL %S exists in import\\install =>RProperty NOT UPDATED!"),&aSrcFile );
            // brand still exists, so no need to uninstall.
            return;
            }

        // Create discardedbrand.txt file in the /appid/brandid path
        CreateFlagFile( aSrcFile );
                
        UpdateRPropertyL( brandString );
        
        TRACE( T_LIT( "CBSInstallHandler::OperationNotifyL uninstall string to pub&sub updated OK") );
        }
	TRACE( T_LIT( "CBSInstallHandler::OperationNotifyL end") );
    }
    
// -----------------------------------------------------------------------------
// CBSInstallHandler::IsBrandInstalled()
// -----------------------------------------------------------------------------
//
TBool CBSInstallHandler::IsBrandInstalled( const TDesC& aBrand )
    {
	TRACE( T_LIT( "CBSInstallHandler::IsBrandInstalled start") );
    TFileName file;
    GetPrivateFolder( file, KInstallObservePath );
    file.Append( aBrand );
    file.Append( KBrandInstallExt );
    
    // check file directly: 
    // aaa$bbb -> aaa$bbb.install
    CDir* dir = NULL;
    iFs.GetDir( file, KEntryAttNormal, ESortNone, dir );
    if( dir->Count() > 0 )
        {
        // brand is installed
        delete dir;
    	TRACE( T_LIT( "CBSInstallHandler::IsBrandInstalled true") );
        return ETrue;
        }
    delete dir;
    dir = NULL;

    GetPrivateFolder( file, KInstallObservePath );
    file.Append( aBrand );
    file.Append( KInstallFileDataSeparator );
    file.Append( KBrandWild );
    file.Append( KBrandInstallExt );
    
    // check file with different versions:
    // aaa$bbb -> aaa$bbb$*.install
    iFs.GetDir( file, KEntryAttNormal, ESortNone, dir );
    if( dir->Count() > 0 )
        {
        // brand is installed
        delete dir;
    	TRACE( T_LIT( "CBSInstallHandler::IsBrandInstalled true") );
        return ETrue;
        }
    delete dir;
    
    // brand is not installed
	TRACE( T_LIT( "CBSInstallHandler::IsBrandInstalled false") );
    return EFalse;
    }

// -----------------------------------------------------------------------------
// CBSInstallHandler::GetPrivateFolder()
// -----------------------------------------------------------------------------
//
void CBSInstallHandler::GetPrivateFolder( TDes& aPath, const TDesC& aAppend )
    {
    iFs.PrivatePath( aPath );
    aPath.Insert( 0, KInstallDrive );
    aPath.Append( aAppend );
    }

void CBSInstallHandler::CreateFlagFile(const TDesC& aSrcFile)
	{
	TRACE( T_LIT( "CBSInstallHandler::CreateFlagFile begin") );
	// parse brand id and application id from aSrcFile
	TParse parse;
	parse.Set( aSrcFile, NULL, NULL );

	// find ids from filename
	TInt firstSepar = parse.Name().Find( KInstallFileDataSeparator );

	// Get the brand Id
	HBufC* brandId = (parse.Name().Left(firstSepar)).AllocL();

	// to get application id
	TInt secondSepar = parse.Name().
	    Mid( firstSepar + 1 ).Find( KInstallFileDataSeparator );
	
	TInt cutlen = 0;
	secondSepar == KErrNotFound ? cutlen = parse.Name().Length() :
	                              cutlen = firstSepar + secondSepar + 1;


	TParse parseAgain;
	parseAgain.Set(parse.Name().Left( cutlen ), NULL, NULL );
	HBufC* applicationId = (parseAgain.Name().Right( cutlen - firstSepar - 1)).AllocL(); 
	

	HBufC *fileName = CBSStorageManager::ConstructDiscardBrandFileNameL(*applicationId, *brandId) ;
	CleanupStack::PushL(fileName);
	HBufC *fullPath = CBSStorageManager::FullDiscardBrandFileNameLC( *fileName ) ;
	
	// crate a flag file
	RFile file;
	file.Create(iFs, *fullPath, EFileWrite);
	file.Close() ;
	
	delete brandId ;
	delete applicationId ;
	
	CleanupStack::PopAndDestroy(fullPath) ;
	CleanupStack::PopAndDestroy(fileName) ;
	TRACE( T_LIT( "CBSInstallHandler::CreateFlagFile end") );
	}

// -----------------------------------------------------------------------------
// CBSInstallHandler::CheckForDiscardedBrandsL()
// -----------------------------------------------------------------------------
//
TBool CBSInstallHandler::CheckForDiscardedBrandsL( const TDesC& aDir )
    {
    TRACE( T_LIT( "CBSInstallHandler::CheckForDiscardedBrandsL begin") );
    TBool discardedBrandExists( EFalse );
       
    CDir* appDir = NULL; // will contain all application folders     
    
    TPath path;
    iFs.PrivatePath( path );
    path.Insert( 0, KInstallDrive );
    path.Append( aDir ); 
    path.Append( KDirSeparator ); // "C:\\private\\102828dd\\data\\"    

    User::LeaveIfError( 
        iFs.GetDir( path, KEntryAttDir, ESortNone, appDir ) );
    CleanupStack::PushL( appDir );

    /*TFileName find( path );
    find.Append( KInstallFiles );*/
    
    // create array of files (as text)
    TInt count = appDir->Count();
    //for each application do...
    for( TInt i = 0; i < count; i++ )
        { 
        TFileName file( path );
        file.Append( (*appDir)[i].iName );
        file.Append( KDirSeparator ); // "C:\\private\\102828dd\\data\\xsp\\"    
        
        TRACE( T_LIT( "CBSInstallHandler::CheckForDiscardedBrandsL AppDir='%S'"),&file );
        CDir* brandDir = NULL; // will contain all brand folder for each application 
        User::LeaveIfError( 
            iFs.GetDir( file, KEntryAttDir, ESortNone, brandDir ) );
        CleanupStack::PushL( brandDir );
        
        TInt countBrands = brandDir->Count();
        //for each brand of a certain application do...
        for( TInt j(0) ; j < countBrands ; j++ )
            {            
            TFileName discardedFile( file );
            discardedFile.Append( (*brandDir)[j].iName );
            discardedFile.Append( KDirSeparator );
            discardedFile.Append( KDiscardBrandFileName );   // "C:\\private\\102828dd\\data\\xsp\\branda\\discarded.txt"    
            
            TRACE( T_LIT( "CBSInstallHandler::CheckForDiscardedBrandsL BrandDir='%S'"),&file );
            
            // check for the existance of the 'discarded.txt' file  
            if ( BaflUtils::FileExists( iFs, discardedFile ) )
                {
                TRACE( T_LIT( "CBSInstallHandler::CheckForDiscardedBrandsL '%S' found!=>brand is discarded."),&discardedFile );
                discardedBrandExists = ETrue;
                
                //set RProperty for this brand                
                
                //the string written to RProperty: 
                //"[brandId]$[applicationId]"(e.g."branda$xsp")
                HBufC* writeBuf = HBufC::NewLC( RProperty::KMaxPropertySize );
                TPtr writeData( writeBuf->Des() );                
                writeData.Append( (*brandDir)[j].iName );
                writeData.Append( KInstallFileDataSeparator );
                writeData.Append( (*appDir)[i].iName );
                TRACE( T_LIT( "CBSInstallHandler::CheckForDiscardedBrandsL uninstallstring='%S'"),writeBuf );
                
                //UpdateProperty here!!!
                UpdateRPropertyL( writeData );
                
                CleanupStack::PopAndDestroy( writeBuf );
                }
            }        
        CleanupStack::PopAndDestroy( brandDir );
        }

    CleanupStack::PopAndDestroy( appDir );    
    TRACE( T_LIT( "CBSInstallHandler::CheckForDiscardedBrandsL end") );
    return discardedBrandExists;
    }


// -----------------------------------------------------------------------------
// CBSInstallHandler::UpdateRPropertyL 
// -----------------------------------------------------------------------------
//
void CBSInstallHandler::UpdateRPropertyL( const TDesC& aUninstallationString )
    {
    HBufC* dataBuf = HBufC::NewLC( RProperty::KMaxPropertySize );
    TPtr data( dataBuf->Des() );                
    
    // Update uninstall string to pub&sub (only if the RProperty does not 
    // contain the unistallation string yet)
    RProcess me;
    TUid uid = me.Identity();
    TInt ret = RProperty::Define( uid, KUninstallKey, RProperty::EText);
    if( ret != KErrAlreadyExists )
        {
        TRACE( T_LIT( "CBSInstallHandler::UpdateRPropertyL RProperty does not exist") );
        User::LeaveIfError( ret );
        }
    TRACE( T_LIT( "CBSInstallHandler::UpdateRPropertyL RProperty created/exists OK") );    
    User::LeaveIfError( RProperty::Get( uid, KUninstallKey, data ) );
    
    // don't append the uninstall string in case it already exists
    // in the RProperty
    if( KErrNotFound == data.Find( aUninstallationString ) )
        {
        if( data.Length() > 0 )
            {
            data.Append( KLineFeed );
            TRACE( T_LIT( "CBSInstallHandler::UpdateRPropertyL lineFeed appended to RProperty uninstallstring") );
            }
        data.Append( aUninstallationString );
        TRACE( T_LIT( "CBSInstallHandler::UpdateRPropertyL '%S' appended to RProperty uninstallstring"), &aUninstallationString );
        if( data.Length() <= RProperty::KMaxPropertySize )
            {
            User::LeaveIfError( RProperty::Set( uid, KUninstallKey, data ) );
            TRACE( T_LIT( "CBSInstallHandler::UpdateRPropertyL RProperty::Set OK") );
            }
        }
    else
        {
        TRACE( T_LIT( "CBSInstallHandler::UpdateRPropertyL '%S' already exists in RProperty"), &aUninstallationString );    
        }
        
    CleanupStack::PopAndDestroy( dataBuf );   
    
    }

//  END OF FILE

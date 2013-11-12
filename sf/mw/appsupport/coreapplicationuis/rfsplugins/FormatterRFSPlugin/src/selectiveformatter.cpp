/*
* Copyright (c) 2006-2010 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Implementation of CSelectiveFormatter class
*
*/


#include <f32file.h>
#include <swi/sisregistrysession.h>
#include <swi/sisregistryentry.h>
#include <swi/sisregistrypackage.h>

#include "dirstackentry.h"
#include "selectiveformatter.h"
#include "formatterrfsplugincommon.h"
#include "rfsfileman.h"
#include "trace.h"

_LIT(KZDrive,"z");
// ================= MEMBER FUNCTIONS =======================

// ---------------------------------------------------------------------------
// CSelectiveFormatter::NewL
// ---------------------------------------------------------------------------
//
CSelectiveFormatter* CSelectiveFormatter::NewLC( const TDesC& aExcludeListFile )
    {
    CSelectiveFormatter* self = new ( ELeave ) CSelectiveFormatter;
    CleanupStack::PushL( self );
    self->ConstructL( aExcludeListFile );
    return self;
    }


// ---------------------------------------------------------------------------
// CSelectiveFormatter::~CSelectiveFormatter
// ---------------------------------------------------------------------------
//
CSelectiveFormatter::~CSelectiveFormatter()
    {
    FUNC_LOG;

    delete iFileMan;

    iExcludeList.ResetAndDestroy();
    iExcludeList.Close();

    iDirStack.ResetAndDestroy();
    iDirStack.Close();

    iFs.Close();
    }


// ---------------------------------------------------------------------------
// CSelectiveFormatter::FindAndDeleteL
// ---------------------------------------------------------------------------
//
void CSelectiveFormatter::FindAndDeleteL( const TDesC& aFullPath )
    {
    FUNC_LOG;

    iDirStack.ResetAndDestroy();
    if ( PreserveType( aFullPath ) != CExcludeListEntry::EPreserveAll )
        {
        HBufC* buffer = HBufC::NewLC( KMaxPath );
        TPtr ptr = buffer->Des();
        CDirStackEntry::PushDirEntryL( iDirStack, aFullPath, iFs ); // Setup root dir
        while ( iDirStack.Count() > 0 )
            {
            CDirStackEntry& stackEntry = *( iDirStack[ iDirStack.Count() - 1 ] );
            const TEntry* dirEntry = stackEntry.GetNextEntry( ptr );
            if ( dirEntry )
                {
                HandleDirEntryL( ptr, *dirEntry );
                }
            else
                {
                // Dir has been processed. Pop it and continue.
                CDirStackEntry::PopAndDestroyDirEntry( iDirStack );
                }
            }
        CleanupStack::PopAndDestroy( buffer );
        }
    else
        {
        INFO_1( "Keep dir: '%S'", &aFullPath );
        }
    }


// ---------------------------------------------------------------------------
// CSelectiveFormatter::CSelectiveFormatter
// ---------------------------------------------------------------------------
//
CSelectiveFormatter::CSelectiveFormatter()
    {
    FUNC_LOG;
    }


// ---------------------------------------------------------------------------
// CSelectiveFormatter::ConstructL
// ---------------------------------------------------------------------------
//
void CSelectiveFormatter::ConstructL( const TDesC& aExcludeListFile )
    {
    FUNC_LOG;

    User::LeaveIfError( iFs.Connect() );
    iFileMan = CRfsFileMan::NewL( iFs );

    // Handle excludelist.txt
    if ( aExcludeListFile.Match( KEmptyParameter ) == KErrNotFound )
        {
        ReadExcludeListL( aExcludeListFile );
        }
    // Handle app specific files
    HandleAppExcludeListsL();
    
    // Handle NR-Applications
    //HandleNrExcludeListsL();
    
    if( !iValidExcludeListFound )
        {
        User::Leave( KErrInvalidExcList );
        }
    }


// ---------------------------------------------------------------------------
// CSelectiveFormatter::ReadExcludeListL
// ---------------------------------------------------------------------------
//
void CSelectiveFormatter::ReadExcludeListL( const TDesC& aExcludeListFile )
    {
    FUNC_LOG;

    RFile file;
    TInt err = file.Open( iFs, aExcludeListFile, EFileRead );
    ERROR_1( err, "Failed to open '%S'", &aExcludeListFile );
    if ( err == KErrNone )
        {
        // exclude list successfully opened -> Selective format.
        INFO_1( "Using exclude list '%S'", &aExcludeListFile );
        CleanupClosePushL( file );

        // Add exclude list file itself to exclude list
        CExcludeListEntry* entry = CExcludeListEntry::NewLC( aExcludeListFile ); // take ownership
        iExcludeList.AppendL( entry );
        CleanupStack::Pop( entry );

        TFileText text;
        text.Set( file );
        TFileName buffer;
        text.Read( buffer ); //carry on if read fails
        while ( buffer.Length() > 0 )
            {
            entry = CExcludeListEntry::NewL( buffer );
            err = iExcludeList.InsertInOrder( entry, CExcludeListEntry::Compare ); // take ownership
            if( err != KErrNone )
                {
                delete entry; // delete entry if ownership not transferred

                if( err != KErrAlreadyExists )
                    {
                    User::Leave( err );
                    }
                }            

            text.Read( buffer ); //carry on if read fails
            iValidExcludeListFound = ETrue;
            }

        CleanupStack::PopAndDestroy( &file );
        }
    }


// ---------------------------------------------------------------------------
// CSelectiveFormatter::PreserveType
// ---------------------------------------------------------------------------
//
CExcludeListEntry::TPreserveType
CSelectiveFormatter::PreserveType( const TDesC& aFullPath ) const
    {
    FUNC_LOG;

    CExcludeListEntry::TPreserveType ret = CExcludeListEntry::EPreserveNone;
    CExcludeListEntry::TMatchEntry entry( aFullPath );
    TInt pos = iExcludeList.Find( entry, CExcludeListEntry::CompareMatch );
    if ( pos >= 0 && pos < iExcludeList.Count() )
        {
        TPtrC matchPath = iExcludeList[pos]->Path();
        ret = iExcludeList[pos]->Type( entry );
        INFO_3( "'%S' matches '%S' type %d", &aFullPath, &matchPath, ret );
        }

    return ret;
    }


// ---------------------------------------------------------------------------
// CSelectiveFormatter::HandleDirEntryL
// ---------------------------------------------------------------------------
//
void CSelectiveFormatter::HandleDirEntryL(
    const TDesC& aFullPath,
    const TEntry& aEntry )
    {
    FUNC_LOG;

    CExcludeListEntry::TPreserveType preserve = PreserveType( aFullPath );

    if ( preserve == CExcludeListEntry::EPreserveNone )
        {
        TRAPD_ERR( err, iFileMan->DeleteL( aFullPath, aEntry ) );
        ERROR_1( err, "Failed to delete '%S'", &aFullPath );
        }
    else if ( aEntry.IsDir() && preserve != CExcludeListEntry::EPreserveAll )
        {
        INFO_1( "Keep matches from: '%S'", &aFullPath );
        CDirStackEntry::PushDirEntryL( iDirStack, aFullPath, iFs );
        }
    else
        {
        INFO_1( "Keep '%S'", &aFullPath );
        }
    }

// ---------------------------------------------------------------------------
// CSelectiveFormatter::HandleAppExcludeListsL
// ---------------------------------------------------------------------------
//
void CSelectiveFormatter::HandleAppExcludeListsL()
    {
    FUNC_LOG;
    HBufC* buf = HBufC::NewLC( KMaxPath );
    TPtr bufPtr = buf->Des();
    iSystemDrive = iFs.GetSystemDriveChar();
    
    // Search from rom and system drives.
    bufPtr.Append( KApplicationExcludeListPath );
    bufPtr.Append( KApplicationExcludeListFileSearchPattern );
    HandleAppExcludeListsOnDriveL( bufPtr, KDefaultRom()[0] );
    bufPtr.Zero();
    bufPtr.Append( KApplicationExcludeListPath );
    bufPtr.Append( KApplicationExcludeListFileSearchPattern );
    HandleAppExcludeListsOnDriveL( bufPtr, iSystemDrive );

    CleanupStack::PopAndDestroy( buf );
    }

// ---------------------------------------------------------------------------
// CSelectiveFormatter::HandleAppExcludeListsOnDriveL
// ---------------------------------------------------------------------------
//
void CSelectiveFormatter::HandleAppExcludeListsOnDriveL( TPtr aBuf, TChar aDrive )
    {
    FUNC_LOG;
    CDir* dir = NULL;
    
    aBuf[0] = aDrive;
    TInt err = iFs.GetDir( aBuf, KEntryAttNormal, ESortNone, dir );

    ERROR_1( err, "RFs::GetDir error while reading drive %c", (TUint)aDrive );
    
    if( err == KErrNone )
        {
        CleanupStack::PushL( dir );
        // Remove search pattern.
        aBuf.Delete( KApplicationExcludeListPath().Length(), 
            KApplicationExcludeListFileSearchPattern().Length() );
    
        for( TInt i = 0; i < dir->Count(); i++ )
            {
            TBool upgradeFound( EFalse );
            const TEntry& entry = ( *dir )[i];
            aBuf.Append( entry.iName );
    
            if( aDrive == KDefaultRom()[0] )
                {
                // Change to system drive for upgrade check
                aBuf[0] = iSystemDrive;
                RFile file;
                err = file.Open( iFs, aBuf, EFileRead );
                if( err == KErrNone )
                    {
                    // Upgrade found - rom file not used
                    upgradeFound = ETrue;
                    file.Close();
                    }
                else
                    {
                    // Upgrade not found - use rom file                
                    aBuf[0] = KDefaultRom()[0];
                    }
                }

            if( !upgradeFound )
                {
                RFile file;
                err = file.Open( iFs, aBuf, EFileRead );
                if ( err == KErrNone )
                    {
                    TInt fileSize( 0 );
                    err = file.Size( fileSize );
                    file.Close();
                    INFO_3( "Application exclude list '%S', err %d, size %d ", &aBuf, err, fileSize );
                    if ( err != KErrNone || fileSize == 0 )
                        {
                        // Empty file
                        INFO_1( "Application exclude list '%S' is empty", &aBuf );
                        }
                    else
                        {
                        ReadExcludeListL( aBuf );                    
                        }
                    }
                }
            // Remove file name.
            aBuf.Delete( KApplicationExcludeListPath().Length(), entry.iName.Length() );
            }
        CleanupStack::PopAndDestroy( dir );
        dir = NULL;
        }
    }

// ---------------------------------------------------------------------------
// CSelectiveFormatter::HandleNrExcludeListsL
// ---------------------------------------------------------------------------
//
void CSelectiveFormatter::HandleNrExcludeListsL()
    {
    INFO( "CSelectiveFormatter::HandleNrExcludeListsL() START ");
    
    Swi::RSisRegistrySession session;
    CleanupClosePushL(session);
    User::LeaveIfError(session.Connect());
    
    INFO( "In CSelectiveFormatter::HandleNrExcludeListsL() RSisRegistrySession::Connect() established");
    // Get the installed application UIDs
    RArray<TUid> uids;
    CleanupClosePushL(uids);
    session.InstalledUidsL(uids);
    TInt uidcount = uids.Count(); 
    
    Swi::RSisRegistryEntry regEntry;
    Swi::RSisRegistryEntry augmentForRegEntry;
    CleanupClosePushL(regEntry);
    CleanupClosePushL(augmentForRegEntry);
    
    // Array of registry files i.e., .reg and .ctl for the installed apps
    RPointerArray<HBufC> registryFiles;
    
    // Array of registry files i.e., .reg and .ctl for the augmented apps
    RPointerArray<HBufC> augmentedRegistryFiles;
    
    // Array of files installed through package.
    RPointerArray<HBufC> nonRemovableFiles;
    
    // Array of augmented files installed through package.
    RPointerArray<HBufC> nonRemovableAugmentedFiles;
    
    CleanupResetAndDestroyPushL(registryFiles);
    CleanupResetAndDestroyPushL(augmentedRegistryFiles);
    CleanupResetAndDestroyPushL(nonRemovableFiles);
    CleanupResetAndDestroyPushL(nonRemovableAugmentedFiles);
    
    TInt count;
    
    //Array of augmented packages
    RPointerArray<Swi::CSisRegistryPackage> augmentationPackages;
    CleanupResetAndDestroyPushL(augmentationPackages);
        
    for ( TInt iter=0; iter<uidcount; iter++)
     {
     User::LeaveIfError(regEntry.Open(session,uids[iter]));
     if(EFalse == regEntry.RemovableL())
         {
         INFO( "In CSelectiveFormatter::HandleNrExcludeListsL() get the nonRemovable and registry files");
         
         regEntry.FilesL(nonRemovableFiles);
         regEntry.RegistryFilesL(registryFiles);
         TInt fileCount = nonRemovableFiles.Count(); 
         for (TInt nonRemovableFilesCount=fileCount-1; nonRemovableFilesCount>=0;nonRemovableFilesCount--)
             {
             TPtr nrFileName(nonRemovableFiles[nonRemovableFilesCount]->Des());
             if(nrFileName.Left(1) == KZDrive )
                 {
                 delete nonRemovableFiles[nonRemovableFilesCount];
                 nonRemovableFiles.Remove(nonRemovableFilesCount);
                 }
             }
         // Look for augmentations.
         if(regEntry.IsAugmentationL())
             {
             regEntry.AugmentationsL(augmentationPackages);
             count = regEntry.AugmentationsNumberL();
             for (TInt augPkgCount=0; augPkgCount < count; ++augPkgCount)
                 {
                 User::LeaveIfError(augmentForRegEntry.OpenL(session,*augmentationPackages[augPkgCount]));
                 if(EFalse == augmentForRegEntry.RemovableL())
                     {
                     INFO( "In CSelectiveFormatter::HandleNrExcludeListsL() get the augmented nonRemovable and registry files");
                     augmentForRegEntry.FilesL(nonRemovableAugmentedFiles);
                     augmentForRegEntry.RegistryFilesL(augmentedRegistryFiles);
                     }
                 augmentForRegEntry.Close();
                 }
             }
         }
     AppendNrlisttoExcludeListL(nonRemovableFiles);
     nonRemovableFiles.ResetAndDestroy();
     regEntry.Close();
     }
    INFO( "In CSelectiveFormatter::HandleNrExcludeListsL() append the list of files to the excludelist ");
    
    AppendNrlisttoExcludeListL(nonRemovableAugmentedFiles);
    AppendNrlisttoExcludeListL(augmentedRegistryFiles);
    AppendNrlisttoExcludeListL(registryFiles);
    
    CleanupStack::PopAndDestroy(9,&session);
    INFO( "CSelectiveFormatter::HandleNrExcludeListsL() End");
    }

// ---------------------------------------------------------------------------
// CSelectiveFormatter::HandleNrExcludeListsL
// ---------------------------------------------------------------------------
//

void CSelectiveFormatter::AppendNrlisttoExcludeListL(RPointerArray<HBufC> &aFileNameArr)
    {
    INFO( "CSelectiveFormatter::AppendNrlisttoExcludeListL() START ");
    TInt size = aFileNameArr.Count();
    CExcludeListEntry* entry;
    TInt err;
    for ( TInt i=0; i < size; i++)
        {
        entry = CExcludeListEntry::NewL( aFileNameArr[i]->Des() );
        err = iExcludeList.InsertInOrder( entry, CExcludeListEntry::Compare ); // take ownership
        if( err != KErrNone )
            {
            delete entry; // delete entry if ownership not transferred
            
            if( err != KErrAlreadyExists )
                {
                INFO_1( "CSelectiveFormatter::AppendNrlisttoExcludeListL() leaves with error code %d",err);
                User::Leave( err );
                }
            }            
        }
    INFO( "CSelectiveFormatter::AppendNrlisttoExcludeListL() END ");
    }

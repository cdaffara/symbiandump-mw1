/*
* Copyright (c) 2002 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description: 
*       Implements the Server class for the rfs Server.
*
*
*/


// INCLUDE FILES

#include <e32svr.h>
#include <txtetext.h>

#include <commdb.h>
#include <cdbcols.h>

#include <pathinfo.h>

#include "rfsServer.h"
#include "rfsSession.h"
#include "rfsClient.h"
#include "rfsScript.h"
#include "rfsShutdown.h"
#include "rfsCompleter.h"
#include "RfsTraces.h"
#include "rfsCommand.h"

// CONSTANTS

_LIT( KRfsServerExe, "rfsServer.exe" );
#ifdef __WINS__
_LIT( KRfsLibName, "rfs.dll" );
#endif

const TInt KRfsShutdownTimeout = 4000000;   // 4 seconds

//Drives
_LIT(KInternalDrivesTag, "%INTERNALDRIVES%");
_LIT(KSystemDriveTag, "%SYSTEMDRIVE%");
_LIT(KRomDriveTag, "%ROMDRIVE%");
_LIT(KRamDriveTag, "%RAMDRIVE%");
_LIT(KMassStoragesTag, "%MASSSTORAGES%");

//Paths
_LIT(KRootPathTag, "%ROOTPATH%");
_LIT(KGamesPathTag, "%GAMESPATH%");
_LIT(KPicturesPathTag, "%PICTURESPATH%");
_LIT(KDigitalSoundsPathTag, "%DIGITALSOUNDSPATH%");
_LIT(KSimpleSoundsPathTag, "%SIMPLESOUNDSPATH%");
_LIT(KInstallsPathTag, "%INSTALLSPATH%");
_LIT(KOthersPathTag, "%OTHERSPATH%");
_LIT(KVideosPathTag, "%VIDEOSPATH%");

_LIT(KDoubleBackSlash, "\\\\");
_LIT(KInitScript, "Z:\\private\\100059C9\\ScriptInit.txt");
_LIT(KServerStartupSemaphoreName, "RfsStartupSem");

// ================= MEMBER FUNCTIONS =======================

CRfsServer::CRfsServer(): 
    CServer2(0),
    iRfsReason( ENormalRfs )
    {
    }

// Create and start a new rfs server.

CRfsServer* CRfsServer::New()
    {
    TRACES("CRfsServer::New()");
    CRfsServer* server = new CRfsServer();
    if ( server )
        {
        // This trap is needed because CFileMan only has a leaving constructor

        TRAPD( err, server->ConstructL() );
        if ( err != KErrNone )
            {
            delete server;
            server = NULL;
            }
        }       
    return server;
    }


void CRfsServer::ConstructL()
    {
    TRACES("CRfsServer::ConstructL()");
    iShutdown = CRfsShutdown::NewL( 0 );

    if ( iFs.Connect() != KErrNone || Start( KRfsServerName ) != KErrNone )
        {
        User::Leave( KErrGeneral );
        }

    iFileMan = CFileMan::NewL( iFs );

    TInt drive(NULL);
    User::LeaveIfError( DriveInfo::GetDefaultDrive( DriveInfo::EDefaultSystem, drive ));
    User::LeaveIfError( iFs.DriveToChar( drive, iSystemDriveChar ) );
    User::LeaveIfError( DriveInfo::GetDefaultDrive( DriveInfo::EDefaultRom, drive ));
    User::LeaveIfError( iFs.DriveToChar( drive, iRomDriveChar ) );
    User::LeaveIfError( DriveInfo::GetDefaultDrive( DriveInfo::EDefaultRam, drive ));
    User::LeaveIfError( iFs.DriveToChar( drive, iRamDriveChar ) );       
    }

CRfsServer::~CRfsServer()
    {
    TRACES("CRfsServer::~CRfsServer()");
    // deallocate all memory

    delete iFileMan;
    delete iShutdown;

    iPluginScriptList.ResetAndDestroy();
    iPluginScriptList.Close();

    // close server sessions

    iFs.Close();
    }

// -----------------------------------------------------------------------------
// CRfsServer::NewSessionL()
// -----------------------------------------------------------------------------

CSession2* CRfsServer::NewSessionL(const TVersion& /*aVersion*/, const RMessage2& /*aMessage*/) const
    {
    TRACES("CRfsServer::NewSessionL()");
    CRfsSession* session = CRfsSession::NewL();
    CONST_CAST( CRfsServer*, this )->IncrementSessions();
    return session;
    }

// -----------------------------------------------------------------------------
// CRfsServer::RunScriptInDescriptorL()
// -----------------------------------------------------------------------------

TInt CRfsServer::RunScriptInDescriptorL( const TDesC& aScript, 
                                         CRfsSession* aSession )
    {
    TRACES("CRfsServer::RunScriptInDescriptorL()");
    TInt errors( 0 );

    CRfsScript* script = CRfsScript::NewLC( aScript );

    // check the script syntax

    TInt err = script->CheckScriptSyntaxL();

    if (err)
        {
        TRACES("CRfsServer::RunScriptInDescriptorL(): ERROR: Illegal syntax in script. Whole RFS interrupted.");
        }

    // execute commands one by one

    if ( err == KErrNone )
        {
        TInt ret( KErrNone );

        while ( ret != KRfsEndOfScript )
            {
            TRAPD( err,
                {
                ret = script->ExecuteNextCommandL( this, aSession );
                } );

            if ( err != KErrNone ||
               ( ret != KErrNone && ret != KRfsEndOfScript ) )
                {
                errors++;
                }

#ifdef _DEBUG // ---------------------------------------------------------------
            TInt status = ( err == KErrNone ? ret : err );

            if ( ret != KRfsEndOfScript )
                {
                if (status) 
                    RDebug::Print( _L( "RFS: ERROR: Status %d" ), status);
                }
#endif // ----------------------------------------------------------------------

            }

        if ( !script->DosCallSent() )
            {
            aSession->DosCompleter()->Complete( KErrNone );
            }

        aSession->EpocCompleter()->Complete( errors );
        TRACES1("CRfsServer::RunScriptInDescriptorL(): %d errors while performing RFS", errors);
        err = KRfsRequestCompleted;
        }

    CleanupStack::PopAndDestroy();  // script

    return err;
    }

// -----------------------------------------------------------------------------
// CRfsServer::HandleScriptInFileL()
// -----------------------------------------------------------------------------

TInt CRfsServer::HandleScriptInFileL( const TDesC& aFileName,
                                      CRfsSession* aSession )
    {
    TRACES("CRfsServer::HandleScriptInFileL()");

    // try to open the file containing the script
    TInt compare = aFileName.Compare( KInitScript );

    if ( aFileName.Compare( KInitScript ) == 0)
        {
        TRACES("CRfsServer::HandleScriptInFileL(): Init script");
        iRfsReason = EInitRfs;
        }

    RFile file;
    TInt err = file.Open( iFs, aFileName, EFileShareReadersOnly );
    CleanupClosePushL( file );

    if ( err == KErrNone )
        {
        TInt size( 0 );
        err = file.Size( size );
        TInt buffer_size( size / sizeof(TText) );

        // allocate buffer for the script

        HBufC* buffer = HBufC::NewMaxLC( buffer_size );        
        TPtr8 bufferPtr( (TUint8*)buffer->Ptr(), size, size );

        if ( err == KErrNone )
            {
            __ASSERT_ALWAYS( size % 2 == 0,
            aSession->Message().Panic( KRfsPanicCategory, ERfsScriptFileNotUnicode ) );

            // read the script to the buffer from the file
            err = file.Read( bufferPtr );
            }

        if ( err == KErrNone )
            {
            // If deep RFS, peek for plug-ins command
            if ( iRfsReason == EDeepRfs &&
                 buffer->Find( KPluginsCommand ) >= 0 )
                {
                // Ensure that format plug-ins are loaded and executed first before 
                // loading other plug-ins.
                User::LeaveIfError( DoRunPlugins( aSession, KPluginFormat ) );
                }
            err = RunScriptL( buffer, aSession );
            }

        CleanupStack::PopAndDestroy( buffer );
        }

    CleanupStack::PopAndDestroy();          // file (closes it)
    return err;
    }

// -----------------------------------------------------------------------------
// CRfsServer::HandleScriptL()
// -----------------------------------------------------------------------------
TInt CRfsServer::HandleScriptL( const TDesC& aScript,
                                CRfsSession* aSession )
    {
    TRACES("CRfsServer::HandleScriptL()");
    TInt err( KErrNone );
    HBufC* buffer = HBufC::NewLC( aScript.Length() );
    TPtr bufferPtr(buffer->Des());
    bufferPtr.Copy( aScript );

    err = RunScriptL( buffer, aSession );

    CleanupStack::PopAndDestroy( buffer );
    return err;
    }

// -----------------------------------------------------------------------------
// CRfsServer::RunScriptL()
// -----------------------------------------------------------------------------
TInt CRfsServer::RunScriptL( HBufC* aBuffer,
                             CRfsSession* aSession )
    {
    TRACES("CRfsServer::RunScriptL()");
    TInt err( KErrNone );

    HandleRootpathTagsL( aBuffer );
    HandleGamesPathTagsL( aBuffer );
    HandlePicturesPathTagsL( aBuffer );
    HandleDigitalSoundsPathTagsL( aBuffer );
    HandleSimpleSoundsPathTagsL( aBuffer );
    HandleInstallsPathTagsL( aBuffer );
    HandleOthersPathTagsL( aBuffer );
    HandleVideosPathTagsL( aBuffer );
    HandleSystemDriveTagsL( aBuffer );
    HandleRomDriveTagsL( aBuffer );
    HandleRamDriveTagsL( aBuffer );
    HandleInternalDrivesTagsL( aBuffer );
    HandleMassStoragesTagsL( aBuffer );
    RemoveDoubleBackslashesL( aBuffer );

    TPtr script = aBuffer->Des();

    if ( aBuffer->Length() != 0 &&
       ( *aBuffer)[0] == CEditableText::EByteOrderMark )
        {
        script.Set( &script[1], script.Length() - 1,
                    script.Length() - 1 );
        }
    TRACES1("CRfsServer::RunScriptL(): Start to run script %S", &script);
    err = RunScriptInDescriptorL( script, aSession );

    TRACES1("CRfsServer::RunScriptL(): End: returns %d", err);
    return err;
    }

// -----------------------------------------------------------------------------
// CRfsServer::FindBoundariesOfCommand()
// -----------------------------------------------------------------------------

void CRfsServer::FindBoundariesOfCommand( TInt& aBegin, 
                                          TInt& aEnd, 
                                          TInt aOffset,
                                          TInt aScriptLength,
                                          const TDesC& aScript)
    {
    aEnd = aOffset;    
    while ( aEnd < aScriptLength )
        {
        TText t = aScript[ aEnd ];
    
        if ( t == EKeyLineFeed || t == EKeyEnter )
            {
            aEnd++;

            if(aEnd < aScriptLength)
	            {
	            // If reading from file, there might be second separator
	            t = aScript[ aEnd ];

	            if ( t == EKeyLineFeed || t == EKeyEnter )
	                {
	                aEnd++;
	                }
	            }
            break;
            }
        aEnd++;
        }

    aBegin = aOffset;
    while ( aBegin > 0 )
        {
        TText t = aScript[ aBegin ];
    
        if ( t == EKeyLineFeed || t == EKeyEnter )
            {
            aBegin++;
            break;
            }
        aBegin--;        
        }
    }

// -----------------------------------------------------------------------------
// CRfsServer::HandleRootpathTagsL()
// -----------------------------------------------------------------------------

void CRfsServer::HandleRootpathTagsL(HBufC* aBuffer)
    {
    TInt tagOffset( 0 );
    TFileName rootPath = PathInfo::PhoneMemoryRootPath();
    while (tagOffset >= 0)
        {
        tagOffset = aBuffer->Find(KRootPathTag);
        if (tagOffset >= 0)
        	{
            // If size of the aBuffer increases, alloc more space to it
            if (rootPath.Length() >  KRootPathTag.iTypeLength)
                {
                TInt size = aBuffer->Des().Length() + (rootPath.Length() -  
                        KRootPathTag.iTypeLength);
                aBuffer = aBuffer->ReAllocL( size );
                }
            
            // Delete %ROOTPATH% tag and insert rootPath
            aBuffer->Des().Delete(tagOffset, KRootPathTag.iTypeLength);
            aBuffer->Des().Insert(tagOffset, rootPath);
            }
        }
    aBuffer = aBuffer->ReAllocL(aBuffer->Des().Length());
    }

// -----------------------------------------------------------------------------
// CRfsServer::HandleInternalDrivesTagsL()
// -----------------------------------------------------------------------------

void CRfsServer::HandleInternalDrivesTagsL(HBufC* aBuffer)
    {
    TInt tagOffset( 0 );
#ifdef RD_MULTIPLE_DRIVE_ENABLERS
    TDriveList driveListInt;
    TInt driveCountInt(0);
    User::LeaveIfError( DriveInfo::GetUserVisibleDrives(iFs, driveListInt, driveCountInt, KDriveAttInternal | KDriveAttRemovable ));
    for( TInt i = EDriveA; i <= EDriveZ; i++ )
        {
        if (driveListInt[i])
        	{
        	TUint drvStatus( 0 );
        	User::LeaveIfError( DriveInfo::GetDriveStatus( iFs, i, drvStatus ) );
        	if ( !(drvStatus & DriveInfo::EDriveInternal) )        	
        		{
        		driveCountInt--;
        		driveListInt[ i ] = NULL;
        		}
        	}
        }
#endif //RD_MULTIPLE_DRIVE_ENABLERS

    tagOffset = 0;
    TInt commandBegin(0);
    TInt commandEnd(0);

    while (tagOffset >= 0)
        {
        tagOffset = aBuffer->Find(KInternalDrivesTag);
                      
        if (tagOffset >= 0)
            {
#ifdef RD_MULTIPLE_DRIVE_ENABLERS            
            if (driveCountInt > 1)
                {
                FindBoundariesOfCommand( commandBegin, 
                                         commandEnd, 
                                         tagOffset, 
                                         aBuffer->Des().Length(),
                                         aBuffer->Des());
                TBuf<KMaxPath*2> tempCommand;

                tempCommand.Append(
                    aBuffer->Des().MidTPtr(commandBegin, 
                                           commandEnd-commandBegin));
                for (TInt i = 2; i <= driveCountInt; i++)
                    {
                    aBuffer = aBuffer->ReAllocL(aBuffer->Des().Length() + 
                                     (commandEnd-commandBegin));
                    aBuffer->Des().Insert(commandEnd, tempCommand);
                    }
                }
            for( TInt i = EDriveA; i <= EDriveZ; i++ )
                {
                if (driveListInt[i])
                    {
                    tagOffset = aBuffer->Find(KInternalDrivesTag);

                    //Get drive letter
                    TChar driveChar;
                    User::LeaveIfError( iFs.DriveToChar( i, driveChar ) );

                    TBuf<1> driveLetter;
                    driveLetter.Append(driveChar);

                    // Delete %INTERNALDRIVES% tag and insert correct drive name
                    aBuffer->Des().Delete(tagOffset, KInternalDrivesTag.iTypeLength);
                    aBuffer->Des().Insert(tagOffset, KRfsDriveMarker);
                    aBuffer->Des().Insert(tagOffset, driveLetter);
                    }
                }
#else //!RD_MULTIPLE_DRIVE_ENABLERS
            // Delete %INTERNALDRIVES% tag and insert correct drive name
            aBuffer->Des().Delete(tagOffset, KInternalDrivesTag.iTypeLength);
            aBuffer->Des().Insert(tagOffset, _L("c:"));
#endif //RD_MULTIPLE_DRIVE_ENABLERS
            }
        }
    aBuffer = aBuffer->ReAllocL(aBuffer->Des().Length());
    }

// -----------------------------------------------------------------------------
// CRfsServer::HandleMassStoragesTagsL()
// -----------------------------------------------------------------------------

void CRfsServer::HandleMassStoragesTagsL(HBufC* aBuffer)
    {
    TInt tagOffset( 0 );
    TInt driveCountInt(0);    
#ifdef RD_MULTIPLE_DRIVE_ENABLERS
    TDriveList driveListInt;
    User::LeaveIfError( DriveInfo::GetUserVisibleDrives(iFs, driveListInt, driveCountInt, KDriveAttInternal | KDriveAttRemovable ));
    for( TInt i = EDriveA; i <= EDriveZ; i++ )
        {
        if (driveListInt[i])
        	{
        	TUint drvStatus( 0 );
        	User::LeaveIfError( DriveInfo::GetDriveStatus( iFs, i, drvStatus ) );
        	if ( !((drvStatus & DriveInfo::EDriveInternal)&&
                    (drvStatus & DriveInfo::EDriveExternallyMountable )))
        		{
        		driveCountInt--;
        		driveListInt[ i ] = NULL;
        		}
        	}
        }
#endif //RD_MULTIPLE_DRIVE_ENABLERS

    tagOffset = 0;
    TInt commandBegin(0);
    TInt commandEnd(0);

    while (tagOffset >= 0)
        {
        tagOffset = aBuffer->Find(KMassStoragesTag);
                      
        if (driveCountInt == 0 && tagOffset >= 0)
            {
            RemovePointedCommandL(aBuffer, tagOffset);
            }
#ifdef RD_MULTIPLE_DRIVE_ENABLERS
        else if (tagOffset >= 0)
            {
            if (driveCountInt > 1)
                {
                FindBoundariesOfCommand( commandBegin, 
                                         commandEnd, 
                                         tagOffset, 
                                         aBuffer->Des().Length(),
                                         aBuffer->Des());
                TBuf<KMaxPath*2> tempCommand;

                tempCommand.Append(
                    aBuffer->Des().MidTPtr(commandBegin, 
                                           commandEnd-commandBegin));
                for (TInt i = 2; i <= driveCountInt; i++)
                    {
                    aBuffer = aBuffer->ReAllocL(aBuffer->Des().Length() + 
                                     (commandEnd-commandBegin));
                    aBuffer->Des().Insert(commandEnd, tempCommand);
                    }
                }
            for( TInt i = EDriveA; i <= EDriveZ; i++ )
                {
                if (driveListInt[i])
                    {
                    tagOffset = aBuffer->Find(KMassStoragesTag);

                    //Get drive letter
                    TChar driveChar;
                    User::LeaveIfError( iFs.DriveToChar( i, driveChar ) );

                    TBuf<1> driveLetter;
                    driveLetter.Append(driveChar);

                    // Delete %MASSSTORAGES% tag and insert correct drive name
                    aBuffer->Des().Delete(tagOffset, KMassStoragesTag.iTypeLength);
                    aBuffer->Des().Insert(tagOffset, KRfsDriveMarker);
                    aBuffer->Des().Insert(tagOffset, driveLetter);
                    }
                }
            }
#endif //RD_MULTIPLE_DRIVE_ENABLERS
        }
    aBuffer = aBuffer->ReAllocL(aBuffer->Des().Length());
    }

// -----------------------------------------------------------------------------
// CRfsServer::HandleSystemDriveTagsL()
// -----------------------------------------------------------------------------

void CRfsServer::HandleSystemDriveTagsL(HBufC* aBuffer)
    {
    TInt tagOffset( 0 );

    while (tagOffset >= 0)
        {
        tagOffset = aBuffer->Find(KSystemDriveTag);
        if (tagOffset >= 0)
            {
#ifdef RD_MULTIPLE_DRIVE_ENABLERS            
            TBuf<1> driveLetter;
            driveLetter.Append(iSystemDriveChar);
            // Delete %SYSTEMDRIVE% tag and insert correct drive name
            aBuffer->Des().Delete(tagOffset, KSystemDriveTag.iTypeLength);
            aBuffer->Des().Insert(tagOffset, KRfsDriveMarker);
            aBuffer->Des().Insert(tagOffset, driveLetter);            
#else //!RD_MULTIPLE_DRIVE_ENABLERS
            // Delete %INTERNALDRIVES% tag and insert correct drive name
            aBuffer->Des().Delete(tagOffset, KSystemDriveTag.iTypeLength);
            aBuffer->Des().Insert(tagOffset, _L("c:"));
#endif //RD_MULTIPLE_DRIVE_ENABLERS
            }
        }
    aBuffer = aBuffer->ReAllocL(aBuffer->Des().Length());
    }

// -----------------------------------------------------------------------------
// CRfsServer::HandleRomDriveTagsL()
// -----------------------------------------------------------------------------

void CRfsServer::HandleRomDriveTagsL(HBufC* aBuffer)
    {
    TInt tagOffset( 0 );

    while (tagOffset >= 0)
        {
        tagOffset = aBuffer->Find(KRomDriveTag);
        if (tagOffset >= 0)
            {
#ifdef RD_MULTIPLE_DRIVE_ENABLERS            
            TBuf<1> driveLetter;
            driveLetter.Append(iRomDriveChar);
            // Delete %ROMDRIVE% tag and insert correct drive name
            aBuffer->Des().Delete(tagOffset, KRomDriveTag.iTypeLength);
            aBuffer->Des().Insert(tagOffset, KRfsDriveMarker);
            aBuffer->Des().Insert(tagOffset, driveLetter);            
#else //!RD_MULTIPLE_DRIVE_ENABLERS
            // Delete %INTERNALDRIVES% tag and insert correct drive name
            aBuffer->Des().Delete(tagOffset, KRomDriveTag.iTypeLength);
            aBuffer->Des().Insert(tagOffset, _L("z:"));
#endif //RD_MULTIPLE_DRIVE_ENABLERS
            }
        }
    aBuffer = aBuffer->ReAllocL(aBuffer->Des().Length());
    }

// -----------------------------------------------------------------------------
// CRfsServer::HandleRamDriveTagsL()
// -----------------------------------------------------------------------------

void CRfsServer::HandleRamDriveTagsL(HBufC* aBuffer)
    {
    TInt tagOffset( 0 );

    while (tagOffset >= 0)
        {
        tagOffset = aBuffer->Find(KRamDriveTag);
        if (tagOffset >= 0)
            {
#ifdef RD_MULTIPLE_DRIVE_ENABLERS
            TBuf<1> driveLetter;
            driveLetter.Append(iRamDriveChar);
            // Delete %RAMDRIVE% tag and insert correct drive name
            aBuffer->Des().Delete(tagOffset, KRomDriveTag.iTypeLength);
            aBuffer->Des().Insert(tagOffset, KRfsDriveMarker);
            aBuffer->Des().Insert(tagOffset, driveLetter);            
#else //!RD_MULTIPLE_DRIVE_ENABLERS
            // Delete %INTERNALDRIVES% tag and insert correct drive name
            aBuffer->Des().Delete(tagOffset, KRamDriveTag.iTypeLength);
            aBuffer->Des().Insert(tagOffset, _L("z:"));
#endif //RD_MULTIPLE_DRIVE_ENABLERS
            }
        }
    aBuffer = aBuffer->ReAllocL(aBuffer->Des().Length());
    }

// -----------------------------------------------------------------------------
// CRfsServer::HandleGamesPathTagsL()
// -----------------------------------------------------------------------------
    
void CRfsServer::HandleGamesPathTagsL(HBufC* aBuffer)
    {
    TInt tagOffset( 0 );

    while (tagOffset >= 0)
        {
        tagOffset = aBuffer->Find(KGamesPathTag);
        if (tagOffset >= 0)
            {
            TFileName path = PathInfo::GamesPath();
            // If size of the aBuffer increases, alloc more space to it
            if (path.Length() >  KGamesPathTag.iTypeLength)
                {
                aBuffer = aBuffer->ReAllocL( aBuffer->Des().Length() + 
                                   path.Length() - 
                                   KGamesPathTag.iTypeLength);
                }
            // Replace tag with path
            aBuffer->Des().Delete(tagOffset, KGamesPathTag.iTypeLength);
            aBuffer->Des().Insert(tagOffset, path);
            }
        }
    aBuffer = aBuffer->ReAllocL(aBuffer->Des().Length());
    }
    
// -----------------------------------------------------------------------------
// CRfsServer::HandlePicturesPathTagsL()
// -----------------------------------------------------------------------------
    
void CRfsServer::HandlePicturesPathTagsL(HBufC* aBuffer)
    {
    TInt tagOffset( 0 );

    while (tagOffset >= 0)
        {
        tagOffset = aBuffer->Find(KPicturesPathTag);
        if (tagOffset >= 0)
            {
            TFileName path = PathInfo::PicturesPath();
            // If size of the aBuffer increases, alloc more space to it
            if (path.Length() >  KPicturesPathTag.iTypeLength)
                {
                aBuffer = aBuffer->ReAllocL( aBuffer->Des().Length() + 
                                   path.Length() - 
                                   KPicturesPathTag.iTypeLength);
                }
            // Replace tag with path
            aBuffer->Des().Delete(tagOffset, KPicturesPathTag.iTypeLength);
            aBuffer->Des().Insert(tagOffset, path);
            }
        }
    aBuffer = aBuffer->ReAllocL(aBuffer->Des().Length());
    }
    
// -----------------------------------------------------------------------------
// CRfsServer::HandleDigitalSoundsPathTagsL()
// -----------------------------------------------------------------------------
    
void CRfsServer::HandleDigitalSoundsPathTagsL(HBufC* aBuffer)
    {
    TInt tagOffset( 0 );

    while (tagOffset >= 0)
        {
        tagOffset = aBuffer->Find(KDigitalSoundsPathTag);
        if (tagOffset >= 0)
            {
            TFileName path = PathInfo::DigitalSoundsPath();
            // If size of the aBuffer increases, alloc more space to it
            if (path.Length() >  KDigitalSoundsPathTag.iTypeLength)
                {
                aBuffer = aBuffer->ReAllocL( aBuffer->Des().Length() + 
                                   path.Length() - 
                                   KDigitalSoundsPathTag.iTypeLength);
                }
            // Replace tag with path
            aBuffer->Des().Delete(tagOffset, KDigitalSoundsPathTag.iTypeLength);
            aBuffer->Des().Insert(tagOffset, path);
            }
        }
    aBuffer = aBuffer->ReAllocL(aBuffer->Des().Length());
    }
    
// -----------------------------------------------------------------------------
// CRfsServer::HandleSimpleSoundsPathTagsL()
// -----------------------------------------------------------------------------
    
void CRfsServer::HandleSimpleSoundsPathTagsL(HBufC* aBuffer)
    {
    TInt tagOffset( 0 );

    while (tagOffset >= 0)
        {
        tagOffset = aBuffer->Find(KSimpleSoundsPathTag);
        if (tagOffset >= 0)
            {
            TFileName path = PathInfo::SimpleSoundsPath();
            // If size of the aBuffer increases, alloc more space to it
            if (path.Length() >  KSimpleSoundsPathTag.iTypeLength)
                {
                aBuffer = aBuffer->ReAllocL( aBuffer->Des().Length() + 
                                   path.Length() - 
                                   KSimpleSoundsPathTag.iTypeLength);
                }
            // Replace tag with path
            aBuffer->Des().Delete(tagOffset, KSimpleSoundsPathTag.iTypeLength);
            aBuffer->Des().Insert(tagOffset, path);
            }
        }
    aBuffer = aBuffer->ReAllocL(aBuffer->Des().Length());
    }
    
// -----------------------------------------------------------------------------
// CRfsServer::HandleInstallsPathTagsL()
// -----------------------------------------------------------------------------
    
void CRfsServer::HandleInstallsPathTagsL(HBufC* aBuffer)
    {
    TInt tagOffset( 0 );

    while (tagOffset >= 0)
        {
        tagOffset = aBuffer->Find(KInstallsPathTag);
        if (tagOffset >= 0)
            {
            TFileName path = PathInfo::InstallsPath();
            // If size of the aBuffer increases, alloc more space to it
            if (path.Length() >  KInstallsPathTag.iTypeLength)
                {
                aBuffer = aBuffer->ReAllocL( aBuffer->Des().Length() + 
                                   path.Length() - 
                                   KInstallsPathTag.iTypeLength);
                }
            // Replace tag with path
            aBuffer->Des().Delete(tagOffset, KInstallsPathTag.iTypeLength);
            aBuffer->Des().Insert(tagOffset, path);
            }
        }
    aBuffer = aBuffer->ReAllocL(aBuffer->Des().Length());
    }
    
// -----------------------------------------------------------------------------
// CRfsServer::HandleOthersPathTagsL()
// -----------------------------------------------------------------------------
    
void CRfsServer::HandleOthersPathTagsL(HBufC* aBuffer)
    {
    TInt tagOffset( 0 );

    while (tagOffset >= 0)
        {
        tagOffset = aBuffer->Find(KOthersPathTag);
        if (tagOffset >= 0)
            {
            TFileName path = PathInfo::OthersPath();
            // If size of the aBuffer increases, alloc more space to it
            if (path.Length() >  KOthersPathTag.iTypeLength)
                {
                aBuffer = aBuffer->ReAllocL( aBuffer->Des().Length() + 
                                   path.Length() - 
                                   KOthersPathTag.iTypeLength);
                }
            // Replace tag with path
            aBuffer->Des().Delete(tagOffset, KOthersPathTag.iTypeLength);
            aBuffer->Des().Insert(tagOffset, path);
            }
        }
    aBuffer = aBuffer->ReAllocL(aBuffer->Des().Length());
    }
    
// -----------------------------------------------------------------------------
// CRfsServer::HandleVideosPathTagsL()
// -----------------------------------------------------------------------------
    
void CRfsServer::HandleVideosPathTagsL(HBufC* aBuffer)
    {
    TInt tagOffset( 0 );

    while (tagOffset >= 0)
        {
        tagOffset = aBuffer->Find(KVideosPathTag);
        if (tagOffset >= 0)
            {
            TFileName path = PathInfo::VideosPath();
            // If size of the aBuffer increases, alloc more space to it
            if (path.Length() >  KVideosPathTag.iTypeLength)
                {
                aBuffer = aBuffer->ReAllocL( aBuffer->Des().Length() + 
                                   path.Length() - 
                                   KVideosPathTag.iTypeLength);
                }
            // Replace tag with path
            aBuffer->Des().Delete(tagOffset, KVideosPathTag.iTypeLength);
            aBuffer->Des().Insert(tagOffset, path);
            }
        }
    aBuffer = aBuffer->ReAllocL(aBuffer->Des().Length());
    }

// -----------------------------------------------------------------------------
// CRfsServer::RemoveDoubleBackslashesL()
// -----------------------------------------------------------------------------
    
void CRfsServer::RemoveDoubleBackslashesL(HBufC* aBuffer)
    {
    TInt offset( 0 );

    while (offset >= 0)
        {
        offset = aBuffer->Find(KDoubleBackSlash);
        if (offset >= 0)
            {
            aBuffer->Des().Delete(offset, 1);
            }
        }
    aBuffer = aBuffer->ReAllocL(aBuffer->Des().Length());
    }

// -----------------------------------------------------------------------------
// CRfsServer::PerformRfsL()
// Runs a script prepared for either normal or deep rfs
// -----------------------------------------------------------------------------

TInt CRfsServer::PerformRfsL( TRfsType aType, CRfsSession* aSession )
    {
    TRACES1("CRfsServer::PerformRfsL(type %d, session)", aType);
    if ( aType == ERfsDeep )
        {
        iRfsReason = EDeepRfs;
        }
    else
        {
        iRfsReason = ENormalRfs;
        }

    // 2 characters are allocated in filename for script number, so numbers
    // 0-99 are valid

    TInt err( KErrNone );

    if ( aType < 0 || aType >= 100 )
        {
        return KErrGeneral;
        }

    if (aType == ERfsNormal)
        {
        err = MergeCenRepScriptL();
        }

    TBuf<KMaxFileName> filename;

#ifdef RD_MULTIPLE_DRIVE_ENABLERS
    if ( err == KErrNone && aType == ERfsNormal )
        {
        filename.Append(iSystemDriveChar);
        }
    else
        {
        filename.Append(iRomDriveChar);
        }
    filename.Append(KRfsDriveMarker);
    filename.Append(KRfsScriptLocation);
    filename.Append(KRfsScriptFileBody);
#else
    if ( err == KErrNone && aType == ERfsNormal )
        {
        filename.Copy( KRfsScriptFileStartSys );
        }   
    else
        {
        filename.Copy( KRfsScriptFileStartRom );
        }
#endif //RD_MULTIPLE_DRIVE_ENABLERS

    filename.AppendNum( (TInt)aType );
    filename.Append( KRfsScriptFileExtension );

    TRACES1("CRfsServer::PerformRfsL(): Run script in file %S", &filename);
    err = HandleScriptInFileL( filename, aSession );

    PerformPluginScriptsL(aSession);

    TRACES1("CRfsServer::PerformRfsL(): End: returns %d",err);
    return err;
    }

// -----------------------------------------------------------------------------
// CRfsServer::PerformPluginScriptsL()
// -----------------------------------------------------------------------------
void CRfsServer::PerformPluginScriptsL( CRfsSession* aSession )
    {
    TRACES("CRfsServer::PerformPluginScriptsL()");

    if( iPluginScriptList.Count() )
        {
        for(TInt i = 0; i < iPluginScriptList.Count(); i++ )
            {
            TPtr script = iPluginScriptList[i]->Des();
            HandleScriptL( script, aSession );
    		}
    	}

    iPluginScriptList.ResetAndDestroy();

    TRACES("CRfsServer::PerformPluginScriptsL(): End");
    }

// -----------------------------------------------------------------------------
// CRfsServer::PerformPostEikonRfs()
// -----------------------------------------------------------------------------

TInt CRfsServer::PerformPostEikonRfs( TRfsType aType, CRfsSession* aSession )
    {
    TRACES("CRfsServer::PerformPostEikonRfs()");
    iRfsReason = aType == ERfsDeep ? EDeepRfs : ENormalRfs;
    TInt ret( DoRunPlugins( aSession, KPluginPostUI ) ); // Load and execute Post UI plug-ins
    TRACES1("CRfsServer::DoPerformPostEikonRfsL() End: returns %d", ret);
    return ret;
    }

// -----------------------------------------------------------------------------
// CRfsServer::IncrementSessions()
// -----------------------------------------------------------------------------

void CRfsServer::IncrementSessions()
    {
    TRACES("CRfsServer::IncrementSessions()");
    iSessionCount++;
    iShutdown->Cancel();
    }

// -----------------------------------------------------------------------------
// CRfsServer::DecrementSessions()
// -----------------------------------------------------------------------------

void CRfsServer::DecrementSessions()
    {
    TRACES("CRfsServer::DecrementSessions()");
    iSessionCount--;
    if ( iSessionCount <= 0 )
        {
        iShutdown->Start( KRfsShutdownTimeout );
        }
    }

// -----------------------------------------------------------------------------
// CRfsServer::MergeCenRepScriptL()
//
// -----------------------------------------------------------------------------

TInt CRfsServer::MergeCenRepScriptL()
    {
    TRACES("CRfsServer::MergeCenRepScriptL()");

    TInt err(KErrNone);

    RFile rfs_file;

#ifdef RD_MULTIPLE_DRIVE_ENABLERS
    TBuf<KMaxPath> script;
    script.Append(iRomDriveChar);
    script.Append(KRfsDriveMarker);
    script.Append(KRfsScriptLocation);    
    script.Append(KRfsRFSNormalScript);
    err = rfs_file.Open(iFs, script, EFileRead);
    TRACES2("CRfsServer::MergeCenRepScriptL(): %S file opened. Err = %d", &script ,err);    
#else
    err = rfs_file.Open(iFs, KRfsRFSScript, EFileRead);
    TRACES2("CRfsServer::MergeCenRepScriptL(): %S file opened. Err = %d", &KRfsRFSScript ,err);    
#endif //RD_MULTIPLE_DRIVE_ENABLERS
    if( err != KErrNone )
        {
        return err;
        }

    RFile cenrep_file;
    
#ifdef RD_MULTIPLE_DRIVE_ENABLERS
    TBuf<KMaxPath> cenRepScript;
    cenRepScript.Append(iRomDriveChar);        
    cenRepScript.Append(KRfsDriveMarker);
    cenRepScript.Append(KRfsScriptLocation);    
    cenRepScript.Append(KRfsCenRepScript);
    err = cenrep_file.Open(iFs, cenRepScript, EFileRead);
    TRACES2("CRfsServer::MergeCenRepScriptL(): %S file opened. Err = %d", &cenRepScript ,err);    
#else       
    err = cenrep_file.Open(iFs, KRfsCenRepScript, EFileRead);
    TRACES2("CRfsServer::MergeCenRepScriptL(): %S file opened. Err = %d", &KRfsCenRepScript ,err);
#endif //RD_MULTIPLE_DRIVE_ENABLERS
    if( err != KErrNone )
        {
        rfs_file.Close();
        return err;
        }

    RFile merged_file;
    
#ifdef RD_MULTIPLE_DRIVE_ENABLERS
    TBuf<KMaxPath> cenRepScript2;
    cenRepScript2.Append(iSystemDriveChar);        
    cenRepScript2.Append(KRfsDriveMarker);
    cenRepScript2.Append(KRfsScriptLocation);    
    cenRepScript2.Append(KRfsRFSNormalScript);
    iFs.MkDirAll( cenRepScript2 ); // Ignore error
    err = merged_file.Replace(iFs, cenRepScript2, EFileStreamText | EFileWrite);
    TRACES2("CRfsServer::MergeCenRepScriptL(): %S file replaced. Err = %d", &cenRepScript2 ,err);
#else       
    iFs.MkDirAll( KRfsMergedScript ); // Ignore error
    err = merged_file.Replace(iFs, KRfsMergedScript, EFileStreamText | EFileWrite);
    TRACES2("CRfsServer::MergeCenRepScriptL(): %S file replaced. Err = %d", &KRfsMergedScript ,err);
#endif //RD_MULTIPLE_DRIVE_ENABLERS    
    
    if( err != KErrNone )
        {
        rfs_file.Close();
        cenrep_file.Close();
        return err;
        }

    TInt size_of_script( 0 );
    TInt buffer_size( sizeof(TText) );
    err = rfs_file.Size( size_of_script );
    TInt number_of_chars( size_of_script / sizeof(TText) );

    // allocate buffer for the script

    HBufC* buffer = HBufC::NewMaxLC( buffer_size );        
    TPtr8 bufferPtr( (TUint8*)buffer->Ptr(), buffer_size);

    TInt i(0);

    for (i = 0; i < number_of_chars; i++)
       {
       if ( err == KErrNone )
            {
            err = rfs_file.Read( bufferPtr);                
            }
        merged_file.Write(bufferPtr);
        }

    err = cenrep_file.Size( size_of_script );
    number_of_chars = size_of_script / sizeof(TText);

    for (i = 0; i < number_of_chars; i++)
        {
        if ( err == KErrNone )
            {
            err = cenrep_file.Read( bufferPtr);
            }
        if (i)
            {
            err = merged_file.Write(bufferPtr);
            }
        else
            {
            // Add carriage return 0x000D and line feed 0x000A
            bufferPtr.Zero();
            TBuf<1> charAsDesc(1);
            charAsDesc[0] = TText(0x0D);
            bufferPtr.Append(charAsDesc[0]);
            charAsDesc[0] = TText(0x00);
            bufferPtr.Append(charAsDesc[0]);
            merged_file.Write(bufferPtr);
            bufferPtr.Zero();
            charAsDesc[0] = TText(0x0A);
            bufferPtr.Append(charAsDesc[0]);
            charAsDesc[0] = TText(0x00);
            bufferPtr.Append(charAsDesc[0]);
            merged_file.Write(bufferPtr);
            }
        }
    cenrep_file.Close();
    rfs_file.Close();
    merged_file.Close();
    CleanupStack::PopAndDestroy(); //buffer
    TRACES1("CRfsServer::MergeCenRepScriptL(): End: returns %d",err);
    return err;
    }

// -----------------------------------------------------------------------------
// CRfsServer::ThreadStart()
// The rfs server thread.
// -----------------------------------------------------------------------------
 
EXPORT_C TInt CRfsServer::ThreadStart( CRfsServer::TSignal& aSignal )
    {
    TRACES("CRfsServer::ThreadStart()");
    // Get thread that started us
    RThread starter;
    TInt err = starter.Open( aSignal.iId, EOwnerThread );

    if ( !err )
        {
        CTrapCleanup* cleanup = NULL;
        CActiveScheduler* scheduler = NULL;
        CRfsServer* server = NULL;

        cleanup = CTrapCleanup::New();
        scheduler = new CActiveScheduler;

        if ( cleanup && scheduler )
            {
            CActiveScheduler::Install( scheduler );
            server = CRfsServer::New();      // adds server in scheduler
            }
            
        if ( !cleanup || !scheduler || !server )
            {
            delete cleanup;
            delete scheduler;
            delete server;
            err = KErrNoMemory;
            }
                
        // Signal the client that server is started up. 
        CRfsServer::SignalClient();

        starter.Close();

        if ( err == KErrNone )
            {
            // start the shutdown timer ( shutdown if no client connection in 4 s )

            server->iShutdown->Start( KRfsShutdownTimeout );

            // start fielding requests from clients
            CActiveScheduler::Start();

            // comes here if server gets shut down

            delete scheduler;
            delete cleanup;
            delete server;
            }

        // thread/process exit
        }

    return err;
    }

// -----------------------------------------------------------------------------
// Simple UIKON env functions
// -----------------------------------------------------------------------------

void CRfsEikonEnv::ConstructL()
    {
    TRACES("CRfsEikonEnv::ConstructL()");
    CEikonEnv::ConstructL();
    }

void CRfsEikonEnv::DestroyEnvironment()
    {
    TRACES("CRfsEikonEnv::DestroyEnvironment()");
    CEikonEnv::DestroyEnvironment();
    }

void CRfsAppUi::ConstructL()
    {
    TRACES("CRfsAppUi::ConstructL()");
    CEikAppUi::BaseConstructL( ENonStandardResourceFile|ENoScreenFurniture);//|EAknEnableMSK  );
    }

#ifdef __WINS__

// -----------------------------------------------------------------------------
// ThreadFunction()
// -----------------------------------------------------------------------------

GLDEF_C TInt ThreadFunction( TAny* aThreadParams )
    {
    TRACES("ThreadFunction()");
    // increase dll's user count so it can't get unloaded when the client
    // application terminates

    RLibrary lib;
    lib.Load( KRfsLibName ); // rfs.dll

    return CRfsServer::ThreadStart( *(CRfsServer::TSignal*)aThreadParams );
    }

#endif

// -----------------------------------------------------------------------------
// StartServer()
// Creates the server thread/process
// -----------------------------------------------------------------------------

GLDEF_C TInt StartServer()
    {
    TRACES("StartServer()");
    TInt ret = KErrNone;
    TRequestStatus status;
    CRfsServer::TSignal signal( status );

    RSemaphore startupSemaphore;
    ret = startupSemaphore.CreateGlobal( KServerStartupSemaphoreName, 0 );

    if ( ret == KErrAlreadyExists )
        {
        // The server is starting up, but has not yet started 
        startupSemaphore.OpenGlobal( KServerStartupSemaphoreName );
        startupSemaphore.Wait(); // wait until the server has started up.
        startupSemaphore.Close();
        return ret;
        }

    // launch server thread (emulator) or process (hw)

    RProcess server;
    ret = server.Create( KRfsServerExe, signal.Get(),
                         TUidType( KNullUid, KNullUid, KNullUid ),
                         EOwnerThread );

    if ( ret != KErrNone )
        {
        startupSemaphore.Close();
        return ret;
        }
   
    server.Resume();
    server.Close();

    startupSemaphore.Wait();
    startupSemaphore.Close();

    return KErrNone;
    }

void CRfsServer::SignalClient()
    {
    RSemaphore startupSemaphore;
    if ( startupSemaphore.OpenGlobal( KServerStartupSemaphoreName ) == KErrNone 
       )
        {
        //Signal the client:The server might have started up successfully or not
        startupSemaphore.Signal();
        }

    startupSemaphore.Close();

    // No point to return any error.
    // if signal client fails, startup will freeze anyway.
    }

TRfsReason CRfsServer::GetRfsReason()
    {
    TRACES1("CRfsServer::GetRfsReason(): return %d", iRfsReason);
    return iRfsReason;
    }

void CRfsServer::RemovePointedCommandL(HBufC* aBuffer, TInt offset)
	{
	TInt start(0);
	TInt end(0);
	
    FindBoundariesOfCommand( start, 
    		end, 
    		offset, 
            aBuffer->Des().Length(),
            aBuffer->Des());
    aBuffer->Des().Delete(start, end - start);
    aBuffer = aBuffer->ReAllocL(aBuffer->Des().Length());
	}

// -----------------------------------------------------------------------------
// CRfsServer::DoRunPluginsL()
// -----------------------------------------------------------------------------
void CRfsServer::DoRunPluginsL( CRfsSession* aSession )
    {
    TRACES("CRfsServer::DoRunPluginsL()");

    CRfsCommand* command = CRfsCommand::NewLC( this, aSession );
    command->Set( ERfsCommandPlugins, 0, NULL );
    command->ExecuteL();
    CleanupStack::PopAndDestroy( command );
    PerformPluginScriptsL(aSession);

    TRACES("CRfsServer::DoRunPluginsL() End");
    }

// -----------------------------------------------------------------------------
// CRfsServer::DoRunPlugins()
// -----------------------------------------------------------------------------
TInt CRfsServer::DoRunPlugins( CRfsSession* aSession, const TDesC& aPluginMatch )
    {
    TRACES1("CRfsServer::DoRunPlugins() match: %S", &aPluginMatch);

    iPluginMatch.Set( aPluginMatch );
    TRAPD( ret, DoRunPluginsL( aSession ) );
    iPluginMatch.Set( KNullDesC );

    TRACES1("CRfsServer::DoRunPlugins() End: returns: %d", ret);
    return ret;
    }

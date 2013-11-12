/*
* Copyright (c) 2002-2010 Nokia Corporation and/or its subsidiary(-ies). 
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
*        Implements the Script's command execution.
*
*
*/


// SYSTEM INCLUDES
#include <e32base.h>
#include <centralrepository.h>
#include <badesca.h>
#include <commdb.h>
#include <cdbcols.h>
#include <hal.h>

// USER INCLUDES
#include "rfsCommand.h"
#include "rfsServer.h"
#include "rfsSession.h"
#include "rfsHandler.h"
#include "rfsPluginDef.h"
#include "sysutil.h"
#include "RfsTraces.h"

// CONSTANTS
_LIT( KPluginDeep, "D" );
_LIT( KPluginNormal, "N" );
_LIT( KPluginInit, "I" );
_LIT( KLinefeed, "\r\n" );
_LIT( KPathIdentifier, ":\\" );
_LIT( KColon, ":" );
_LIT( KSemiColon, ";" );
_LIT( KComma, "," );
_LIT( KSpace, " " );

const TUid KCRUidCommsDb = { 0xcccccc00 };
const TInt KMaxCenRepKeySize = 1024;
const TUint KDefaultPriority = 100;

// ================= MEMBER FUNCTIONS ==========================================

CRfsCommand::CRfsCommand( CRfsServer* aServer, CRfsSession* aSession ) : 
    iServer( aServer ),
    iSession( aSession ), 
    iCommandId( ERfsCommandUnknown )
    {
    }

CRfsCommand* CRfsCommand::NewLC( CRfsServer* aServer, CRfsSession* aSession )
    {
    CRfsCommand* command = new( ELeave ) CRfsCommand( aServer, aSession );
    CleanupStack::PushL( command );
    return command;
    }

CRfsCommand::~CRfsCommand()
    {
    delete [] iParams;
    }

// -----------------------------------------------------------------------------
// CRfsCommand::CommandId()
// -----------------------------------------------------------------------------

TRfsCommandId CRfsCommand::CommandId()
    {
    return iCommandId;
    }

// -----------------------------------------------------------------------------
// CRfsCommand::Set()
// -----------------------------------------------------------------------------

void CRfsCommand::Set( TRfsCommandId aCommandId, TInt aNumberOfParams,
                       TPtrC* aParams )
    {                       
    iCommandId = aCommandId;
    iNumberOfParams = aNumberOfParams;
    iParams = aParams;
    }

// -----------------------------------------------------------------------------
// CRfsCommand::ExecuteL()
// -----------------------------------------------------------------------------

TInt CRfsCommand::ExecuteL()
    {    
    TInt ret( KErrNone );
    TRACES("CRfsCommand::ExecuteL(): ******* Next command *******");

    switch ( iCommandId )
        {
        // DEL command

        case ERfsCommandDel:
            TRACES1("CRfsCommand::ExecuteL(): Command: DEL %S", &iTargetFile);

            FetchTargetFileL( iParams[ 0 ] );

            ret = iServer->iFileMan->Delete( iTargetFile );

            if ( ret == KErrNotFound || ret == KErrPathNotFound )
                {
                ret = KErrNone;
                }
            
            break;

        // CR command

        case ERfsCommandCr:
            TRACES("CRfsCommand::ExecuteL(): Command: CR");
            ret = ExecuteCrCommandL();
            break;

        // CD command

        case  ERfsCommandCd:
            TRACES("CRfsCommand::ExecuteL(): Command: CD");
            ret = ExecuteCdCommandL();
            iSession->iChangeDirStatus = ret;
            break;

        // RD command

        case ERfsCommandRd:
            TRACES("CRfsCommand::ExecuteL(): Command: RD");
            FetchTargetFileL( iParams[ 0 ] );

            ret = iServer->iFileMan->RmDir( iTargetFile );

            if ( ret == KErrNotFound || ret == KErrPathNotFound )
                {
                ret = KErrNone;
                }
                        
            break;

        // MD command

        case ERfsCommandMd:
            TRACES("CRfsCommand::ExecuteL(): Command: MD");
            FetchTargetFileL( iParams[ 0 ] );

            ret = iServer->iFs.MkDirAll( iTargetFile );
            TRACES1("CRfsCommand::ExecuteL(): Command: MD: err = %d", ret);

            if ( ret == KErrAlreadyExists )
                {
                TRACES1("CRfsCommand::ExecuteL(): Command: MD: Error %d is OK", KErrAlreadyExists);
                ret = KErrNone;
                }

            break;

        // CP command

        case ERfsCommandCp:
            TRACES("CRfsCommand::ExecuteL(): Command: CP");
            ret = ExecuteCpCommandL();
            break;

        // PLUGINS command

        case ERfsCommandPlugins:
            TRACES("CRfsCommand::ExecuteL(): Command: PLUGINS");
            ret = ExecutePluginsCommandL();
            break;

        // FORMAT command

        case ERfsCommandFormat:

        // skip FORMAT command in WINS
            TRACES("CRfsCommand::ExecuteL(): Command: FORMAT");
#ifndef __WINS__
            ret = ExecuteFormatCommand();
#endif
            break;

        default:
            break;
        };
    TRACES1("CRfsCommand::ExecuteL(): ******* Command completed. Status: %d *******", ret);
    return ret;
    }

// -----------------------------------------------------------------------------
// CRfsCommand::StringToNumberL()
// -----------------------------------------------------------------------------

void CRfsCommand::StringToNumberL( const TDesC& aString, TUint& aNumber,
                                   TRadix aRadix )
    {
    TLex lex( aString );

    User::LeaveIfError( lex.Val( aNumber, aRadix ) );

    if ( !lex.Eos() )
        {
        User::Leave( KErrGeneral );
        }
    }

// -----------------------------------------------------------------------------
// CRfsCommand::StringToNumberL()
// -----------------------------------------------------------------------------

void CRfsCommand::StringToNumberL( const TDesC& aString, TUid& aNumber,
                                   TRadix aRadix )
    {
  TLex lex(aString);
  
  TUint appId = 0;
  
  User::LeaveIfError( lex.Val( appId,aRadix ) );
  
  aNumber = TUid::Uid((TInt)appId);
  
    if ( !lex.Eos() )
        {
        User::Leave( KErrGeneral );
        }
    }

// -----------------------------------------------------------------------------
// CRfsCommand::StringToNumberL()
// -----------------------------------------------------------------------------

void CRfsCommand::StringToNumberL( const TDesC& aString, TUint32& aNumber,
                                   TRadix aRadix )
    {
  TLex lex(aString);

  User::LeaveIfError( lex.Val( aNumber,aRadix ) );
  
    if ( !lex.Eos() )
        {
        User::Leave( KErrGeneral );
        }
    }

// -----------------------------------------------------------------------------
// CRfsCommand::ExecuteCpCommandL()
// -----------------------------------------------------------------------------

TInt CRfsCommand::ExecuteCpCommandL()
    {
    TRACES("CRfsCommand::ExecuteCpCommandL()");

    TParsePtrC parse( iParams[ 0 ] );
    
    // if destination file is not given, it's same as source file but in C:

    if ( iNumberOfParams == 1 )
        {
#ifdef RD_MULTIPLE_DRIVE_ENABLERS
        TInt drive;
        User::LeaveIfError( DriveInfo::GetDefaultDrive( DriveInfo::EDefaultSystem, 
        		drive ));
        TChar driveChar;
        User::LeaveIfError( iServer->iFs.DriveToChar( drive, driveChar ) );
        iTargetFile.Append( driveChar );        
        iTargetFile.Append( KRfsDriveMarker );
#else
        _LIT( KTargetDrive, "C:" );
        iTargetFile.Copy( KTargetDrive );
#endif //RD_MULTIPLE_DRIVE_ENABLERS
        iTargetFile.Append( parse.Path() );        
        }

    // destination file is specified

    else
        {
        FetchTargetFileL( iParams[ 1 ] );
        }

    TRfsReason rfsReason = iServer->GetRfsReason();
    if ( rfsReason == EInitRfs)
        {
        TFileName fileName;
        fileName.Copy(iTargetFile);
        fileName.Append( parse.Name() );
        fileName.Append( parse.Ext() );

        TRACES1("CRfsCommand::ExecuteL(): Source file: %S", &iParams[ 0 ]);        
        TRACES1("CRfsCommand::ExecuteL(): Target file: %S", &fileName);
        
        TFindFile find( iServer->iFs );
        CDir* files (NULL);
      TInt ret = find.FindWildByPath( fileName, NULL, files );
        if ((ret == KErrNone) &&  files )
            {
            TRACES("CRfsCommand::ExecuteL(): Target file already exists ");
            return KErrNone;
            }
        }

    TInt ret = iServer->iFileMan->Copy( iParams[ 0 ], iTargetFile,
                                        CFileMan::EOverWrite |
                                        CFileMan::ERecurse );

    // remove the read-only flag from target since it will be set in a copy
    // operation from ROM.

    if ( ret == KErrNone )
        {
        ret = RemoveReadOnlyFlagFromTarget( iParams[0], iTargetFile );
        }

    // If the file to be copied was not found, return KErrNone

    if ( ret == KErrNotFound || ret == KErrPathNotFound )
        {
        ret = KErrNone;
        }

    TRACES1("CRfsCommand::ExecuteCpCommandL(): End: returns %d", ret);
    return ret;
    }

// -----------------------------------------------------------------------------
// CRfsCommand::ExecuteCrCommandL()
// -----------------------------------------------------------------------------

TInt CRfsCommand::ExecuteCrCommandL()
    {
    TRACES("CRfsCommand::ExecuteCrCommandL()");

    TInt err( KErrNone );

    TUint number;
    StringToNumberL( iParams[ 0 ], number, EHex );

    TUid uid = { number }; 

    // first parameter is the UID, the rest are keywords
    #ifdef _DEBUG
        TInt32 hexuid;
        hexuid = uid.iUid;
        HBufC* hbuf = HBufC::New( 80 );
        if ( hbuf )
            {
            TPtr buf = hbuf->Des();
            _LIT(KCenRepUID, "RFS: CRfsCommand::ExecuteCrCommandL(): CENREP UID = 0x");
            buf = KCenRepUID;
            buf.AppendNumFixedWidth( hexuid, EHex, 8 );
            RDebug::Print( buf );
            delete hbuf;
            }
    #endif

    CRepository* repository = CRepository::NewL(uid);
    CleanupStack::PushL( repository );

    if ( iNumberOfParams > 1 )
        {        
        for ( TInt i = 1 ; i < iNumberOfParams ; i++ )
            {
            TUint key;
            StringToNumberL( iParams[ i ], key, EHex );               
            TRACES2("CRfsCommand::ExecuteCrCommandL(): CR:  Param[%d] = %S", i, &iParams[ i ]);
            err = repository->Reset(key);
            }
        }
    else
        {
        err = repository->Reset();
        }
        
    CleanupStack::PopAndDestroy( repository );
    TRACES1("CRfsCommand::ExecuteCrCommandL(): End: returns %d", err);
    return err;
    }
// -----------------------------------------------------------------------------
// CRfsCommand::ExecuteFormatCommand()
// -----------------------------------------------------------------------------

TInt CRfsCommand::ExecuteFormatCommand()
    {
    TRACES("CRfsCommand::ExecuteFormatCommand()");
    RFormat formatter;
    TInt ret( KErrNone );

    TInt count;
    
    // Open formatter
    ret = formatter.Open( 
    iServer->iFs, iParams[ 0 ], EHighDensity | EFullFormat, count );
    TRACES1("CRfsCommand::ExecuteFormatCommand(): Formatter open returned : %d", ret);    
    if ( ret == KErrInUse ) // Locked files in drive
        {
        TInt driveNumber = EDriveC; 

        // Shouldn't assume it's the C drive, check the parameter 
        ret = iServer->iFs.CharToDrive( iParams[ 0 ][0], driveNumber ); 
        TRACES1("CRfsCommand::ExecuteFormatCommand(): drive numer %d", driveNumber);   

        // Get the file system name from the drive before dismounting it 
        TFullName name; 
        ret = iServer->iFs.FileSystemName(name, driveNumber); 
        TRACES2("CRfsCommand::ExecuteFormatCommand(): FileSystemName returned : %d, name %S", ret, &name); 

        // Force a dismount
        TRequestStatus dummyStat;
        TRACES("CRfsCommand::ExecuteFormatCommand(): Setting notifier");
        iServer->iFs.NotifyDismount( driveNumber, dummyStat, EFsDismountForceDismount );

        TRACES("CRfsCommand::ExecuteFormatCommand(): Waiting for request complete");
        User::WaitForRequest(dummyStat);
        TRACES("CRfsCommand::ExecuteFormatCommand(): Request completed");

        // Now mount the file system again
        ret = iServer->iFs.MountFileSystem(name, driveNumber);
        TRACES1("CRfsCommand::ExecuteFormatCommand(): MountFileSystem returned : %d", ret);

        // Try to open formatter second time
        ret = formatter.Open( 
            iServer->iFs, iParams[ 0 ], EHighDensity | EFullFormat, count );
        TRACES1("CRfsCommand::ExecuteFormatCommand(): Second try: Formatter open returned : %d", ret);
        }

    if ( ret == KErrNone )
        {
        while ( count > 0 )
            {
            ret = formatter.Next( count );
            TRACES2("CRfsCommand::ExecuteFormatCommand(): Format status: %d, tracks remaining %d", ret, count);
            }
        }
            
    formatter.Close();
    
    //Enforce Central Repository to re-read setting from .cre file…
    //In next phase try to modify CentRep setting. Because .cre file is locked by CRepositorySession
    //CentRep cannot make update and causes that repostiory goes to inconsistent state. Inconsistent
    //state ensures that new security settings are readed from drive before any other operation
    
    CRepository* repository( NULL );
    TRAPD( err, repository = CRepository::NewL( KCRUidCommsDb ) );
    if ( err == KErrNone )
        {
        err = repository->Reset();
        TRACES1("CRfsCommand::ExecuteFormatCommand(): CommsDb reset. Err %d",err);
        err = repository->Create( 0, 0 );
        TRACES1("CRfsCommand::ExecuteFormatCommand(): CommsDb create. Err %d",err);
        TRACES("CRfsCommand::ExecuteFormatCommand(): Request completed");
		if ( err == KErrAlreadyExists )
		    {
			err = repository->Set( 0, 0);		
			TRACES1("CRfsCommand::ExecuteFormatCommand(): CommsDb set. Err %d",err);
            }
        }
    delete repository;

    TRACES1("CRfsCommand::ExecuteFormatCommand(): End: returns %d", ret);
    return ret;
    }

// -----------------------------------------------------------------------------
// CRfsCommand::ExecuteCdCommandL()
// -----------------------------------------------------------------------------

TInt CRfsCommand::ExecuteCdCommandL()
    {
    TRACES("CRfsCommand::ExecuteCdCommandL()");
    TInt ret( KErrNone );
    FetchTargetFileL( iParams[ 0 ] );

    TParsePtrC parse( iTargetFile );

    if ( !parse.DrivePresent() || !parse.PathPresent() ||
         parse.NameOrExtPresent() )
        {
        ret = KErrGeneral;
        }

    else
        {
        ret = iServer->iFs.SetSessionPath( iTargetFile );
        }
    TRACES1("CRfsCommand::ExecuteCdCommandL(): End: returns %d", ret);
    return ret;
    }

// -----------------------------------------------------------------------------
// CRfsCommand::ExecutePluginsCommandL()
// -----------------------------------------------------------------------------

TInt CRfsCommand::ExecutePluginsCommandL()
    {
    TRACES("CRfsCommand::ExecutePluginsCommandL()");

    TInt ret( KErrNone );

    RImplInfoPtrArray implArray;

    CRFSPlugin::ListAllImplementationsL( implArray );
    CleanupClosePushL( implArray );
    ReOrderImplArrayL(implArray);

    TRfsReason rfsReason = iServer->GetRfsReason();
    TPtrC pluginMatch = iServer->PluginMatch();
    iNumberOfPlugins = implArray.Count();
    TRACES1("CRfsCommand::ExecutePluginsCommandL(): Number of plugins %d", iNumberOfPlugins);

    for( TInt i = 0; i < iNumberOfPlugins; i++ )
        {
        TRACES("CRfsCommand::ExecutePluginsCommandL(): ------- Next plugin -------");
        CImplementationInformation& info = *implArray[i];
        TUid implementation = info.ImplementationUid();

        TRACES1("CRfsCommand::ExecutePluginsCommandL(): Plugin name: %S", &info.DisplayName());
        HBufC* text = HBufC::NewMaxLC( implArray[i]->DataType().Length() );

        TPtr data( text->Des() );
        data.Copy(info.DataType());

        TRACES1("CRfsCommand::ExecutePluginsCommandL(): Data: %S", &data);

        if ( ( !pluginMatch.Length() &&
               (data.Find(KPluginNormal) >= 0 && rfsReason == ENormalRfs) ||
               (data.Find(KPluginInit) >= 0 && rfsReason == EInitRfs) ||
               (data.Find(KPluginDeep) >= 0 && rfsReason == EDeepRfs) ) ||
               (pluginMatch.Length() > 0 && data.Find(pluginMatch) >= 0) )
            {
            TRACES("CRfsCommand::ExecutePluginsCommandL(): Ok to execute");
            CRFSPlugin* plugin = CRFSPlugin::NewL( implementation );
            CleanupStack::PushL(plugin);
            TRACES("CRfsCommand::ExecutePluginsCommandL(): RestoreFactorySettingsL()");
            TRAPD( leaveCode, plugin->RestoreFactorySettingsL( rfsReason ));
            if (leaveCode != KErrNone )
                {
                TRACES1("CRfsCommand::ExecutePluginsCommandL(): RestoreFactorySettingsL leaved: %d", leaveCode);
                }
            TBuf<KMaxPath> script;
            plugin->GetScriptL( rfsReason, script );
            if (script.Compare(KNullDesC))
                {
                TRACES("CRfsCommand::ExecutePluginsCommandL(): Plugin script available()");
                HandlePluginScriptL( script, plugin, rfsReason );                   
                }
            CleanupStack::PopAndDestroy(plugin);
            }
        CleanupStack::PopAndDestroy(text);
        }
    CleanupStack::PopAndDestroy(&implArray);
    TRACES1("CRfsCommand::ExecutePluginsCommandL(): End: returns %d", ret);
    return ret;
    }

// -----------------------------------------------------------------------------
// CRfsCommand::IsCenRepKey()
// -----------------------------------------------------------------------------

TBool CRfsCommand::IsCenRepKey(TDes& aScript)
    {
    if (aScript.Find(KPathIdentifier) == 1)
        {
        return EFalse;
        }
    else
        {
        return ETrue;
        }
    }

// -----------------------------------------------------------------------------
// CRfsCommand::HandlePluginScriptL()
// -----------------------------------------------------------------------------

TInt CRfsCommand::HandlePluginScriptL( TDes& aScript, CRFSPlugin* aPlugin, const TRfsReason aType )
    {
    TInt err(KErrNone);

    RFile plgFile;
    if (!IsCenRepKey(aScript))
        {
        //Open script file
        err = plgFile.Open( iServer->iFs, aScript, EFileRead );
        TRACES2("CRfsCommand::HandlePluginScriptL(): Plugin script %S opened. Err = %d",&aScript, err);
        if( err != KErrNone )
            {
            return err;
            }
        CleanupClosePushL(plgFile);
        }

    TInt plgFileSize(NULL);
    TInt buffer_size(NULL);

    HBufC* buffer = NULL;

    if ( err == KErrNone )
        {
        if (IsCenRepKey(aScript))
            {
            TInt offset(NULL);

            buffer = HBufC::NewL( 1 );
            CleanupStack::PushL(buffer);
            buffer->Des().Append( KSpace );
        
            TBool endOfScript(EFalse);

            while (!endOfScript)
                {
                TUid uid;
                TInt nextUidEnd(aScript.Mid(offset).Find(KColon));
                TInt nextKeyEnd(aScript.Mid(offset).Find(KComma));
                TInt nextRepositoryEnd(aScript.Mid(offset).Find(KSemiColon));

                TInt lght = aScript.Length();
                TRACES1("CRfsCommand::HandlePluginScriptL(): offset: %d", offset);
                TRACES1("CRfsCommand::HandlePluginScriptL(): lght  : %d", lght);

                if (nextRepositoryEnd < 0 || 
                    offset + nextRepositoryEnd + 1 >= aScript.Length() )
                    {
                    endOfScript = ETrue;
                    }
                StringToNumberL( aScript.Mid(offset, nextUidEnd), uid, EHex );
                offset = offset + nextUidEnd+1;


                TBool endOfRepository(EFalse);
                while (!endOfRepository)
                    {
                    TRACES1("CRfsCommand::HandlePluginScriptL(): aScript: %S", &aScript);
                    nextKeyEnd = aScript.Mid(offset).Find(KComma);
                    nextRepositoryEnd = aScript.Mid(offset).Find(KSemiColon);

                    if (nextKeyEnd < 0 && nextRepositoryEnd < 0)
                        {
                        nextKeyEnd = aScript.Length() - offset;
                        endOfRepository = ETrue;              
                        }
                    else if (nextKeyEnd < 0 || nextKeyEnd > nextRepositoryEnd)
                        {
                        nextKeyEnd = nextRepositoryEnd;
                        endOfRepository = ETrue;
                        }
                    TUint32 key;
                    StringToNumberL( aScript.Mid(offset, nextKeyEnd), key, EHex );              
                    offset = offset + nextKeyEnd+1;
                    TBuf<KMaxCenRepKeySize> cenRepBuffer;
                    GetKeyL(uid, key, cenRepBuffer);
                    TRACES1("CRfsCommand::HandlePluginScriptL(): script size: %d", cenRepBuffer.Length() );
                    CleanupStack::Pop(buffer); // the address of buffer may change during the ReAlloc
                    buffer = buffer->ReAllocL( cenRepBuffer.Length() + buffer->Length() + sizeof(KLinefeed));
                                        
                    CleanupStack::PushL(buffer);
                    buffer->Des().Append( cenRepBuffer );
                    buffer->Des().Append( KLinefeed );
                    }
                }
            }
        else
            {
            plgFile.Size(plgFileSize);
            buffer_size = plgFileSize / sizeof(TText);
            buffer = HBufC::NewMaxLC( buffer_size );
            TPtr8 bufferPtr( (TUint8*)buffer->Ptr(), plgFileSize, plgFileSize );        
            err = plgFile.Read( bufferPtr );
            }
        }

    if ( buffer )
        {
        TPtr script = buffer->Des();
    
        TInt index(1);    
    
        TInt fileEnd(EFalse);
        while ( !fileEnd )
            {    
            TInt commandLength = GetCommandLength(script, index);
            if (commandLength)
                {
                HandleNextPluginCommandL(aPlugin, 
                                         script,
                                         index,
                               commandLength,
                               aType);
                index = index + commandLength + 2;
                }
            else
                {
                fileEnd = ETrue;
                }
            }
    
        CleanupStack::PopAndDestroy(buffer);
        }
    if (!IsCenRepKey(aScript))
        {
        CleanupStack::PopAndDestroy(&plgFile);
        }
    TRACES1("CRfsCommand::HandlePluginScriptL(): End: returns %d", err);
    return err;
    }

// -----------------------------------------------------------------------------
// CRfsCommand::HandleNextPluginCommandL()
// -----------------------------------------------------------------------------
void CRfsCommand::HandleNextPluginCommandL(CRFSPlugin* aPlugin,
                                           const TDesC& aScript, 
                                           TInt aIndex, 
                                           TInt aLength, 
                                           const TRfsReason aType)
    {
    HBufC* commandPtr = HBufC::NewLC(aLength + 1);
    TPtr command(commandPtr->Des());
    command.Copy(aScript.Mid(aIndex, aLength));
    TRACES1("CRfsCommand::HandleNextPluginCommandL(): Command: %S", &command);
    if (IsNextCommandValid(aScript, aIndex, aLength) == KErrNone)
        {
        TRACES("CRfsCommand::HandleNextPluginCommandL(): Valid command");
        command.Append( EKeyLineFeed );        
        iServer->iPluginScriptList.AppendL( commandPtr );
        CleanupStack::Pop(commandPtr);
        }
    else
        {
        CleanupStack::PopAndDestroy(commandPtr);
        TRACES("CRfsCommand::HandleNextPluginCommandL(): Unknown command");
        HBufC* ptr = HBufC::NewLC(aLength);
        TPtr customCommand(ptr->Des());
        customCommand.Copy(aScript.Mid(aIndex, aLength));
        aPlugin->ExecuteCustomCommandL(aType, customCommand);
        CleanupStack::PopAndDestroy(ptr);
        }
    }
    
// -----------------------------------------------------------------------------
// CRfsCommand::IsNextCommandValid()
// -----------------------------------------------------------------------------
TInt CRfsCommand::IsNextCommandValid(const TDesC& aScript, TInt aIndex, TInt aLength)
    {
    const TDesC* commands[ KRfsNumberOfCommands ] =
        {    
        &KRfsCommand1,
        &KRfsCommand2,
        &KRfsCommand3,
        &KRfsCommand4,
        &KRfsCommand5,
        &KRfsCommand6,
        &KRfsCommand7,
        &KRfsCommand8
        };
    
    TInt err( KErrNotSupported);
    
    for ( TInt i = 0; i < KRfsNumberOfCommands ; i++ )
        {
        if (!aScript.Mid(aIndex, aLength).Find(*( commands[ i ])))
            {
            if (!aScript.Mid(aIndex, aLength).Find(KPluginsCommand))
                {
                return KErrNotSupported;
                }
            else
                {
                return KErrNone;
                }
            }
        }    
    return err;
    }    

// -----------------------------------------------------------------------------
// CRfsCommand::GetCommandLength()
// -----------------------------------------------------------------------------
TInt CRfsCommand::GetCommandLength(const TDesC& aScript, TInt aIndex)
    {
    TInt length(0);
    TInt scriptLength = aScript.Length();

    FOREVER
        {
        if (aIndex + length >= scriptLength)
            {
            length = 0;
            break;
            }
        TText t = aScript[ length + aIndex ];
        if ( t == EKeyLineFeed || t == EKeyEnter )
            {
            break;
            }
        length++;
        }
    return length;
    }

// -----------------------------------------------------------------------------
// CRfsCommand::FetchTargetFileL()
// -----------------------------------------------------------------------------

void CRfsCommand::FetchTargetFileL( const TDesC& aFilename )
    {    
    TRACES("CRfsCommand::FetchTargetFileL()");
    TRACES1("CRfsCommand::FetchTargetFileL(): FileName: %S", &aFilename);

    // this is needed because Symbian OS file name parsers don't support file names
    // containing a relative directory path component (it has to be either
    // whole path or no path at all)

    // determine if a full file name is given

    if ( aFilename.Length() >= 2 && aFilename[ 1 ] == ':' )
        {
        iTargetFile.Copy( aFilename );
        }
    else
        {
        // if the last CD command failed, leave 
        User::LeaveIfError( iSession->iChangeDirStatus );

        iTargetFile.Zero();
        iServer->iFs.SessionPath( iTargetFile );
        iTargetFile.Append( aFilename );
        }
    TRACES("CRfsCommand::FetchTargetFileL(): End");
    }

// -----------------------------------------------------------------------------
// CRfsCommand::GetKeyL()
// -----------------------------------------------------------------------------

void CRfsCommand::GetKeyL( TUid aUid, TUint32 aKey, TDes& aBuf )
    {    
    CRepository* repository(NULL);
    
    TRAPD( err, repository = CRepository::NewL( aUid ) );
    TRACE_IF_ERROR_1("CRfsCommand::GetKeyL(): ERROR: If err is -1 CenRep ini file is missing: err = %d",err)
    CleanupStack::PushL( repository );
    User::LeaveIfError( err );
    err = repository->Get( aKey, aBuf );
    TRACE_IF_ERROR_1("CRfsCommand::GetKeyL(): ERROR: CRepository::Get returned err = %d", err );
    CleanupStack::PopAndDestroy( repository );

    //Replace semicolons with line feeds
    TBool moreSemicolons(ETrue);  
    while ( moreSemicolons)
        {
        TInt semic = aBuf.Find(KSemiColon);
        if (semic >= 0 )
            {
            aBuf.Replace(semic, 1, KLinefeed);
            }
        else
            {
            moreSemicolons = EFalse;
            }
        }
    }

// -----------------------------------------------------------------------------
// CRfsCommand::RemoveReadOnlyFlagFromTarget()
// -----------------------------------------------------------------------------

TInt CRfsCommand::RemoveReadOnlyFlagFromTarget( const TDesC& aSource,
                                                TDes& aTarget )
    {
    TRACES("CRfsCommand::RemoveReadOnlyFlagFromTarget()");
    TInt length = aTarget.Length();     // backup the old length

    // append the file name in the target if only path is specified

    if ( aTarget[ length-1 ] == '\\' )
        {
        TInt index = aSource.LocateReverse( '\\' );
        
        if ( index != KErrNotFound && index <= aSource.Length() - 2 )
            {
            aTarget.Append( aSource.Mid( index + 1 ) );
            }
        }

    TInt ret = iServer->iFileMan->Attribs( aTarget, 0, KEntryAttReadOnly,
                                           TTime( 0 ), CFileMan::ERecurse );

    aTarget.SetLength( length );
    TRACES1("CRfsCommand::RemoveReadOnlyFlagFromTarget(): End: returns %d", ret);
    return ret;
    }

// -----------------------------------------------------------------------------
// CRfsCommand::ReOrderImplArrayL()
// -----------------------------------------------------------------------------
void CRfsCommand::ReOrderImplArrayL(RImplInfoPtrArray& aImplInfoArray)
    {
    TInt numberOfPlugins = aImplInfoArray.Count();
    TInt offset(0);
    
#ifdef _DEBUG    
    TRACES("CRfsCommand::ReOrderImplArrayL(): Original order");
    for( TInt i = 0; i < numberOfPlugins; i++ )
        {
        CImplementationInformation& info = *aImplInfoArray[i];
        HBufC* text = HBufC::NewMaxLC( aImplInfoArray[i]->OpaqueData().Length() );
        TPtr opaqueData( text->Des() );
        opaqueData.Copy(info.OpaqueData());
        TRACES3("CRfsCommand::ReOrderImplArrayL(): Pos: %d prio: %S name: '%S'", i, &opaqueData, &info.DisplayName());
        CleanupStack::PopAndDestroy(text);
        }
#endif //_DEBUG      

    TBool unordered(ETrue);
    
    while (unordered)
        {
        TUint highPriorityPos(KMaxTInt32);
        TUint highPriorityPrio(KMaxTInt32);        
        unordered = EFalse;
        for( TInt i = offset; i < numberOfPlugins; i++ )
            {
            CImplementationInformation& info = *aImplInfoArray[i];
            HBufC* text = HBufC::NewMaxLC( aImplInfoArray[i]->OpaqueData().Length() );
            TPtr opaqueData( text->Des() );
            opaqueData.Copy(info.OpaqueData());
            TRACES3("CRfsCommand::ReOrderImplArrayL(): Pos: %d prio: %S name: '%S'", i, &opaqueData, &info.DisplayName());        

            TUint priority(KDefaultPriority);
            TInt err;
            TRAP( err, StringToNumberL( opaqueData, priority, EDecimal));
           
            if ( priority < highPriorityPrio )
                {
                unordered = ETrue;
                highPriorityPos = i;
                highPriorityPrio = priority;
                }
            CleanupStack::PopAndDestroy(text);
            }
        if (unordered)
            MoveImplementation(aImplInfoArray, highPriorityPos, offset);
        offset++;
        if (offset >= numberOfPlugins)
            unordered = EFalse;
        }
        
#ifdef _DEBUG    
    TRACES("CRfsCommand::ReOrderImplArrayL(): new order");
    for( TInt i = 0; i < numberOfPlugins; i++ )
        {
        CImplementationInformation& info = *aImplInfoArray[i];
        HBufC* text = HBufC::NewMaxLC( aImplInfoArray[i]->OpaqueData().Length() );
        TPtr opaqueData( text->Des() );
        opaqueData.Copy(info.OpaqueData());
        TRACES3("CRfsCommand::ReOrderImplArrayL(): Pos: %d prio: %S name: '%S'", i, &opaqueData, &info.DisplayName());
        CleanupStack::PopAndDestroy(text);
        }
#endif //_DEBUG    
    }

// -----------------------------------------------------------------------------
// CRfsCommand::MoveImplementation()
// -----------------------------------------------------------------------------
void CRfsCommand::MoveImplementation(RImplInfoPtrArray& aImplInfoArray, 
                                     TInt aSourcePos, 
                                     TInt aDestinationPos)
    {
    if (aDestinationPos >= aSourcePos) 
        return;
    CImplementationInformation& source = *aImplInfoArray[aSourcePos];
    
    for( TInt i = aSourcePos; i > aDestinationPos; i-- )
        {
        aImplInfoArray[i] = aImplInfoArray[i-1];
        CImplementationInformation& info = *aImplInfoArray[i];
        }
    aImplInfoArray[aDestinationPos] = &source;
    }
// End of File

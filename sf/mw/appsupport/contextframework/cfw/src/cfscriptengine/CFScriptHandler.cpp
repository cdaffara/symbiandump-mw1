/*
* Copyright (c) 2002-2008 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  The main interface class providing services of ScriptEngine module
*
*/


#include <e32math.h>
#include <e32svr.h>
#include <s32mem.h>
#include <bautils.h>
#include <s32file.h>
#include <centralrepository.h>
#include <gmxmldocument.h>
#include <gmxmlelement.h>

#ifdef RD_MULTIPLE_DRIVE
#include <driveinfo.h>
#endif

#include "CFScriptHandler.h"
#include "CFScript.h"
#include "ContextFrameworkPrivateCRKeys.h"
#include "cftrace.h"
#include "cfextendedcontextinterface.h"
#include "cfoperationpluginmanager.h"
#include "cfpersistentdata.h"
#include "cfcommon.h"
#include "cfscriptowner.h"

// CONSTANTS

#ifdef USE_TEMP_RULE_FOLDER
    _LIT( KDefaultRuleFilePath, "c:\\Silmaril\\Rules\\" );
#else
    _LIT( KDefaultRuleFilePath, "!:\\private\\10282BC4\\Rules\\" );
#endif

// Default script search pattern
_LIT( KDefaultRuleFileSearchPattern, "*.rul" );
_LIT( KWildCardChar, "*" );

const TInt KInitialScriptId = 1;
const TInt KUidLength = 8;
const TInt KTempScriptId = 0;

const TUint KLimit = 0xFFFFFFFF;

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CCFScriptHandler::CCFScriptHandler
// C++ default constructor can NOT contain any code, that might leave.
// -----------------------------------------------------------------------------
//
CCFScriptHandler::CCFScriptHandler(
	MCFExtendedContextInterface& aCF,
    RFs& aFs,
    MCFActionHandler& aScriptEventListener,
    MCFSecurityChecker& aSecurityChecker )
    :   iParserDataProvidingState( KError ),
        iNextId( KInitialScriptId ),
        iCF( aCF ),
        iFs( aFs ),
        iScriptEventListener( aScriptEventListener ),
        iSecurityChecker( aSecurityChecker )
    {
    FUNC_LOG;
    }

// -----------------------------------------------------------------------------
// CCFScriptHandler::ConstructL
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CCFScriptHandler::ConstructL()
    {
    FUNC_LOG;
    
    iParser = CMDXMLParser::NewL( this );
    iWaitParsing = new( ELeave ) CActiveSchedulerWait;
    iOperationPluginManager = CCFOperationPluginManager::NewL( *this );

#ifdef RD_MULTIPLE_DRIVE
    User::LeaveIfError( DriveInfo::GetDefaultDrive(
        DriveInfo::EDefaultRom, iDefaultRomDrive ) );
    User::LeaveIfError( DriveInfo::GetDefaultDrive(
        DriveInfo::EDefaultSystem, iDefaultSystemDrive ) );
#else
    iDefaultRomDrive = 'Z';
    iDefaultSystemDrive = 'C';
#endif
    }

// -----------------------------------------------------------------------------
// CCFScriptHandler::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
EXPORT_C CCFScriptHandler* CCFScriptHandler::NewL(
	MCFExtendedContextInterface& aCF,
    RFs& aFs,
    MCFActionHandler& aScriptEventListener,
    MCFSecurityChecker& aSecurityChecker )
    {
    FUNC_LOG;

    CCFScriptHandler* self = NewLC( aCF, aFs, aScriptEventListener,
    	aSecurityChecker );
    CleanupStack::Pop( self );
    return self;
    }

// -----------------------------------------------------------------------------
// CCFScriptHandler::NewLC
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
EXPORT_C CCFScriptHandler* CCFScriptHandler::NewLC(
	MCFExtendedContextInterface& aCF,
    RFs& aFs,
    MCFActionHandler& aScriptEventListener,
    MCFSecurityChecker& aSecurityChecker )
    {
    FUNC_LOG;

    CCFScriptHandler* self =
        new( ELeave ) CCFScriptHandler( aCF, aFs, aScriptEventListener,
        aSecurityChecker );
    CleanupStack::PushL( self );
    self->ConstructL();
    return self;
    }

// Destructor
EXPORT_C CCFScriptHandler::~CCFScriptHandler()
    {
    FUNC_LOG;

    iRollbackList.ResetAndDestroy();
    iLoadedScripts.Close();
    iScripts.ResetAndDestroy();
    iScriptIds.Close();
    delete iParser;
    delete iWaitParsing;
    delete iOperationPluginManager;
    }


// -----------------------------------------------------------------------------
// CCFScriptHandler::AddScript
// Adds a new script. Calls AddScriptL that does the actual work.
// -----------------------------------------------------------------------------
//
TInt CCFScriptHandler::AddScript( const TDesC& aName,
    const TDesC8& aScript,
    const TUid& aOwner,
    const RThread& aOwnerThread,
    MCFActionHandler& aActionHandler,
    MCFScriptOwner* aScriptOwner )
    {
    FUNC_LOG;
    
    TInt err = KErrNone;

    // Check that script with the name does not exist - notice script object
    // ownership is not transfered
    RPointerArray< CCFScript > scripts;
    GetScriptsByUid( aOwner, scripts );
    for( TInt i = 0; i < scripts.Count(); ++i )
        {
        // Compare names only if we are not updating existing script.
        if ( scripts[ i ]->ScriptId() != iNextId )
            {
            TPtrC scriptName( scripts[ i ]->Name() );
            if( scriptName.CompareF( aName ) == KErrNone )
                {
                err = KErrAlreadyExists;
                ERROR_2( err, "Script name %S already in use in client 0x%x",
                    &aName, aOwner );
                break;
                }
            }
        }
    scripts.Close();

    // Add script
    TInt ret = err;
    if( err == KErrNone )
        {
        TRAP( err, ret = AddScriptL( aName,
            aScript,
            aOwner,
            aOwnerThread,
            aActionHandler,
            ETrue,
            aScriptOwner ) );
        if ( err != KErrNone )
            {
            ERROR( ret, "Unable to add script" );
            ret = err;
            }
        }

    // return script Id ( >0 ) or error code if error ( <0 )
    return ret;
    }


// -----------------------------------------------------------------------------
// CCFScriptHandler::NumberOfScriptsByOwner
//
// -----------------------------------------------------------------------------
//
TInt CCFScriptHandler::NumberOfScriptsByOwner( const TUid& aOwner )
    {
    FUNC_LOG;

    TInt result( KErrNone );

    // iterate thru all the existing scripts
    for ( TInt i = 0; i < iScripts.Count(); i++  )
        {
        // check if the owner Uid matches
        if ( iScripts[ i ]->OwnerUid() == aOwner )
            {
            // increment script count
            result++;
            }
        }

    return result;
    }

// -----------------------------------------------------------------------------
// CCFScriptHandler::NumberOfScripts
//
// -----------------------------------------------------------------------------
//
TInt CCFScriptHandler::NumberOfScripts()
    {
    FUNC_LOG;

    return iScripts.Count();
    }

// -----------------------------------------------------------------------------
// CCFScriptHandler::ScriptLength
//
// -----------------------------------------------------------------------------
//
TInt CCFScriptHandler::ScriptLength( TInt aScriptId )
    {
    FUNC_LOG;

    TInt result ( KErrNotFound );

    // iterate thru all the existing scripts
    for ( TInt i = 0; i < iScripts.Count(); i++  )
        {
        // check if the script id matches
        if ( iScripts[ i ]->ScriptId() == aScriptId )
            {
            // get the descriptor length of the script
            result = iScripts[ i ]->Length();
            }
        }
    return result;
    }

// -----------------------------------------------------------------------------
// CCFScriptHandler::GetEveryScriptId
//
// -----------------------------------------------------------------------------
//
const RArray< TInt >& CCFScriptHandler::GetEveryScriptId()
    {
    FUNC_LOG;

    // local variable declarations
    TInt err( KErrNone );

    // reset array
    iScriptIds.Reset();

    // go thru all the scripts
    for ( TInt i = 0; i < iScripts.Count(); i++ )
        {
        // add Id to result array
        err = iScriptIds.Append( iScripts[ i ]->ScriptId() );
        // handle error
        if ( err != KErrNone )
            {
            // return what we have created so far
            return iScriptIds;
            }
        }
    return iScriptIds;
    }

// -----------------------------------------------------------------------------
// CCFScriptHandler::GetEveryScriptIdByOwner
//
// -----------------------------------------------------------------------------
//
const RArray< TInt >& CCFScriptHandler::GetEveryScriptIdByOwner( 
    const TUid& aScriptOwner )
    {
    FUNC_LOG;

    // local variable declarations
    TInt err( KErrNone );

    // reset array
    iScriptIds.Reset();

    // go thru all the scripts
    for ( TInt i = 0; i < iScripts.Count(); i++ )
        {
        // if the owner Uid matches
        if ( iScripts[ i ]->OwnerUid() == aScriptOwner )
            {
            // add Id to result array
            err = iScriptIds.Append( iScripts[ i ]->ScriptId() );
            // handle error
            if ( err != KErrNone )
                {
                // return what we have created so far
                return iScriptIds;
                }
            }
        }
    return iScriptIds;
    }

// -----------------------------------------------------------------------------
// CCFScriptHandler::UpdateScript
//
// -----------------------------------------------------------------------------
//
TInt CCFScriptHandler::UpdateScript( TInt aScriptID, 
	const RThread& aOwnerThread,
    const TDesC8& aUpdatedScript,
    MCFScriptOwner* aScriptOwner )
    {
    FUNC_LOG;

    // local variable declaration & initialization
    TInt err( KErrNotFound );
    TInt index = iScripts.Count() - 1;
    TBool found( EFalse );

    // search for the script
    while ( !found &&    // script not found yet
            index >= 0 ) // still scripts left
        {
        if ( iScripts[ index ]->ScriptId() == aScriptID )
            {
            found = ETrue;
            err = KErrNone;
            break; // skip to not found
            }
        index--;
        }
    // not found
    if ( !found )
        {
        // return immediately with error code
        return err;
        }
    // script found
    else
        {
        // store session and owner Uid temporarily
        MCFActionHandler& scriptSession 
            = ( iScripts[ index ]->ActionHandler() );

        // handle Script Ids correctly
        TInt nextAvailableScriptId = iNextId;
        iNextId = iScripts[ index ]->ScriptId();

        TUid scriptOwner = iScripts[ index ]->OwnerUid();

        // remove found script from array and delete it
        CCFScript* outdatedScript = iScripts [ index ];

        // add the new script; note that the existing script
        // file in filestore is removed automatically
        iUpdatedScript = outdatedScript;
        err = AddScript( outdatedScript->Name(),
            aUpdatedScript,
        	scriptOwner,
        	aOwnerThread,
        	scriptSession,
            aScriptOwner );
        iUpdatedScript = NULL;
        
        // remove the outdated script only if the new one is succesully added.
        if( err > 0 )
            {
            iScripts.Remove( index );
            delete outdatedScript;
            }
        // restore Next available Script Id
        iNextId = nextAvailableScriptId;
        }
    // script added successfully
    if ( err > 0 )
        {
        err = KErrNone;
        }

    return err;
    }

// -----------------------------------------------------------------------------
// CCFScriptHandler::SaveScript
// -----------------------------------------------------------------------------
//
TInt CCFScriptHandler::SaveScript( const TDesC8& aScript,
    TInt aScriptId,
    const TUid& aOwnerUid )
    {
    FUNC_LOG;
    
    TInt err = KErrNotFound;
    for( TInt i = 0; i < iScripts.Count(); i++ )
        {
        CCFScript* script = iScripts[i];
        // Check script ID
        if( script->ScriptId() == aScriptId )
            {
            // Check script owner
            if( script->OwnerUid() == aOwnerUid ||
                KCFServerSid == aOwnerUid )
                {
                // found matching uid - construct file path
                HBufC* name = ScriptFilePath( script->Info() );
                if( name )
                    {
                    err = DoSave( *name, aScript );
                    }
                else
                    {
                    err = KErrNoMemory;
                    }
                delete name;
                name = NULL;
                break;
                }
            else
                {
                err = KErrAccessDenied;
                }
            }
        }
    
    return err;
    }

// -----------------------------------------------------------------------------
// CCFScriptHandler::DeleteScriptByName
// -----------------------------------------------------------------------------
//
TInt CCFScriptHandler::DeleteScriptByName( const TDesC& aScriptName,
    const TUid& aOwnerUid )
    {
    FUNC_LOG;
    
    TInt err = KErrNotFound;

    // Get script name
    TParsePtrC parse( aScriptName );
    TPtrC scriptName( parse.Name() );
    
    // Get scripts added by the owner
    RPointerArray<CCFScript> scripts;
    GetScriptsByUid( aOwnerUid, scripts );
    
    // If the script being deleted has already been deregistered, try to resolve
    // the script path from the script name and owner uid
    if( scripts.Count() )
        {
        for( TInt i = 0; i < scripts.Count(); i++ )
            {
            CCFScript* script = scripts[i];
            if( script->Name().CompareF( scriptName ) == KErrNone )
                {
                // correct owner uid - delete script
                HBufC* name = ScriptFilePath( script->Info() );
                if( name )
                    {
                    // Delete script file
                    TPtrC namePtrC( *name );
                    err = DeleteScriptFile( namePtrC, aOwnerUid );
                    }
                else
                    {
                    err = KErrNoMemory;
                    }
                
                delete name;
                name = NULL;
                break;
                }
            }
        }
    
    // Check manually from the system drive if the script was not found
    if( err == KErrNotFound )
        {
        // Resolve script name from owner uid and script name
        HBufC* name = ScriptFilePath( scriptName, aOwnerUid );
        if( name )
            {
            // Delete script file
            TPtrC namePtrC( *name );
            err = DeleteScriptFile( namePtrC, aOwnerUid );
            if( err == KErrNone )
                {
                // Transfer err as KErrDeregisterNotNeeded since the script
                // has already been deregistered
                err = KErrDeregisterNotNeeded;
                }
            
            // Clean up
            delete name;
            name = NULL;
            }
        }
    
    // Clean up
    scripts.Close();
    
    return err;
    }

// -----------------------------------------------------------------------------
// CCFScriptHandler::DeleteScriptByUid
// -----------------------------------------------------------------------------
//
TInt CCFScriptHandler::DeleteScriptByUid( const TUid& aUid )
    {
    FUNC_LOG;
    
    // Delete all scripts registered and stored by the client
    TInt err = KErrNotFound;
    HBufC* name = HBufC::New( KMaxFileName );
    if( name )
        {
        RPointerArray<CCFScript> scripts;
        GetScriptsByUid( aUid, scripts );
        if( scripts.Count() )
            {
            for( TInt i = 0; i < scripts.Count(); i++ )
                {
                CCFScript* script = scripts[i];
                TPtr namePtr( name->Des() );
                ScriptFilePath( script->Info(), namePtr );
                err = BaflUtils::DeleteFile( iFs, namePtr );
                INFO_3( "Deleted script file %S from client %x with code %d",
                    &namePtr, aUid.iUid, err );
                }
            
            // Remove folder
            TParsePtrC parse( *name );
            TPtrC dir( parse.DriveAndPath() );
            
            // Check if not in rules base folder - delete this folder
            if( aUid != KCFServerSid )
                {
                err = iFs.RmDir( dir );
                INFO_2( "Removed directory %S with code %d", &dir, err );
                }
            }
        else
            {
            // Scripts not found (already deregistered), search the scripts
            // manually
            TPtr namePtr( name->Des() );
            namePtr.Format( KDefaultSystemRuleFileFormat, aUid.iUid, &KWildCardChar );
            namePtr[0] = iDefaultSystemDrive;
            
            // search all rule files from the particular client
            CDir* dir = NULL;
            err = iFs.GetDir( namePtr, KEntryAttNormal, ESortNone, dir );
            if( err == KErrNone && dir )
                {
                for( TInt i = 0; i < dir->Count(); i++ )
                    {
                    TParsePtrC entryNameParse( (*dir)[i].iName );
                    TPtrC entryName( entryNameParse.Name() );
                    namePtr.Format( KDefaultSystemRuleFileFormat,
                        aUid.iUid, &entryName );
                    namePtr[0] = iDefaultSystemDrive;
                    
                    // Possible error values can be ignored since deleting
                    // multiple files
                    DeleteScriptFile( namePtr, aUid );
                    }
                err = KErrDeregisterNotNeeded;
                }
            else
                {
                // Basically no scripts found, convert err to KErrNotFound
                err = KErrNotFound;
                }
            delete dir;
            dir = NULL;
            }
        
        // cleanup
        scripts.Close();
        delete name;
        name = NULL;
        }
    else
        {
        err = KErrNoMemory;
        }
    
    return err;
    }

// -----------------------------------------------------------------------------
// CCFScriptHandler::AlreadyExists
// -----------------------------------------------------------------------------
//
TBool CCFScriptHandler::AlreadyExists( const TDesC& aScriptName,
    const TUid& aOwnerUid,
    TInt& aScriptId ) const
    {
    FUNC_LOG;
    
    TParsePtrC parsePtrC( aScriptName );
    TPtrC scriptName( parsePtrC.Name() );
    TBool exists = EFalse;
    RPointerArray<CCFScript> scripts;
    GetScriptsByUid( aOwnerUid, scripts );
    for( TInt i = 0; i < scripts.Count(); i++ )
        {
        CCFScript* script = scripts[i];
        if( script->Name().CompareF( scriptName ) == KErrNone )
            {
            // Name already in use - store script id
            aScriptId = script->ScriptId();
            exists = ETrue;
            break;
            }
        }
    scripts.Close();
    
    return exists;
    }

// -----------------------------------------------------------------------------
// CCFScriptHandler::CleanupPersistentDataByName
// -----------------------------------------------------------------------------
//
void CCFScriptHandler::CleanupPersistentDataByName( const TDesC& aScriptName,
    const TUid& aOwnerUid )
    {
    FUNC_LOG;
    
    TParsePtrC parsePtrC( aScriptName );
    TPtrC scriptName( parsePtrC.Name() );
    RPointerArray<CCFScript> scripts;
    GetScriptsByUid( aOwnerUid, scripts );
    for( TInt i = 0; i < scripts.Count(); i++ )
        {
        CCFScript* script = scripts[i];
        if( script->Name().CompareF( scriptName ) == KErrNone )
            {
            script->CleanupPersistentData();
            }
        }
    scripts.Close();
    }

// -----------------------------------------------------------------------------
// CCFScriptHandler::CleanupPersistentDataByUid
// -----------------------------------------------------------------------------
//
void CCFScriptHandler::CleanupPersistentDataByUid( const TUid& aOwnerUid )
    {
    FUNC_LOG;
    
    RPointerArray<CCFScript> scripts;
    GetScriptsByUid( aOwnerUid, scripts );
    for( TInt i = 0; i < scripts.Count(); i++ )
        {
        CCFScript* script = scripts[i];
        script->CleanupPersistentData();
        }
    scripts.Close();
    }

// -----------------------------------------------------------------------------
// CCFScriptHandler::RestoreRomScript
// -----------------------------------------------------------------------------
//
TInt CCFScriptHandler::RestoreRomScript( TInt aScriptId,
    const TUid& /*aOwnerUid*/,
    const RThread& aClient )
    {
    FUNC_LOG;
    
    TInt err = KErrNotFound;
    
    // Check that the restored file actually exists
    RPointerArray<CCFScript> scripts;
    GetScriptsByUid( KCFServerSid, scripts );
    CCFScript* script = NULL;
    for( TInt i = 0; i < scripts.Count(); i++ )
        {
        script = scripts[i];
        if( script->ScriptId() == aScriptId )
            {
            // Script found - break from loop
            break;
            }
        script = NULL;
        }
    scripts.Close();
    
    // If the script was found
    if( script )
        {
        HBufC* name = ScriptFilePath( script->Info() );
        if( name )
            {
            // Resolve rom script name and replace system drive letter with rom
            TPtr namePtr( name->Des() );
            namePtr[0] = iDefaultRomDrive;
            
            TBool exists = BaflUtils::FileExists( iFs, namePtr );
            if( exists )
                {
                HBufC8* scriptBuf = LoadScriptFromFile( namePtr );
                if( scriptBuf )
                    {
                    MCFActionHandler& scriptSession =
                        script->ActionHandler();

                    // Check thath upgrade is allowed
                    err = IsUpgradeAllowed( script->Name(), *scriptBuf,
                        script->OwnerUid(), aClient, scriptSession );
                    if( err == KErrNone )
                        {
                        // Script data succesfully loaded - update script
                        err = UpdateScript( aScriptId, aClient, *scriptBuf, NULL );
                        }
                    }
                else
                    {
                    err = KErrCorrupt;
                    }
                delete scriptBuf;
                scriptBuf = NULL;
                }
            }

        // Clean up
        delete name;
        name = NULL;
        }
    
    return err;
    }

// -----------------------------------------------------------------------------
// CCFScriptHandler::IsUpgradeAllowed
// -----------------------------------------------------------------------------
//
TInt CCFScriptHandler::IsUpgradeAllowed( const TDesC& aName,
    const TDesC8& aScript,
    const TUid& aOwner,
    const RThread& aOwnerThread,
    MCFActionHandler& aActionHandler )
    {
    FUNC_LOG;
    
    TInt allowed( KErrNotSupported );
    
    // Since we are not creating a permanent script script id and owner are
    // temporary
    CCFScript* scriptUpgrade = NULL;
    TRAP( allowed, scriptUpgrade = CreateScriptL( aName, aScript, aOwner,
        aOwnerThread, aActionHandler, ETrue, KTempScriptId, NULL ) );
    if( allowed == KErrNone )
        {
        if( scriptUpgrade )
            {
            allowed = KErrNotFound;
            RPointerArray<CCFScript> scripts;
            
            // Get all rom scripts - this means all scripts with owner uid
            // of cfserver sid
            GetScriptsByUid( KCFServerSid, scripts );
            for( TInt i = 0; i < scripts.Count(); i++ )
                {
                CCFScript* script = scripts[i];
                if( script->Name().CompareF(
                    scriptUpgrade->Name() ) == KErrNone )
                    {
                    // Matching script found - check that script upgrade has at
                    // least the same capability level than the rom script
                    if( scriptUpgrade->UpgradeSecurity().HasCapabilities(
                        script->UpgradeSecurity() ) )
                        {
                        // Check that client has the needed upgrade capabilities
                        TSecurityInfo client( aOwnerThread );
                        if( client.iCaps.HasCapabilities(
                            scriptUpgrade->UpgradeSecurity() ) )
                            {
                            allowed = KErrNone;
                            }
                        else
                            {
                            allowed = KErrAccessDenied;
                            }
                        }
                    else
                        {
                        allowed = KErrAccessDenied;
                        }
                    }
                }
            scripts.Close();
            }
        else
            {
            allowed = KErrNotSupported;
            }
        }
        
    // clean up
    delete scriptUpgrade;
    scriptUpgrade = NULL;
    
    return allowed;
    }

//------------------------------------------------------------------------------
// CCFScriptHandler::RemoveScriptByProviderUid
//------------------------------------------------------------------------------
//
TInt CCFScriptHandler::RemoveScriptByProviderUid( const TUid& aProviderUid,
    TBool aRollback )
    {
    FUNC_LOG;
    
    TInt err = KErrNotFound;

    // Check all scripts which have dependency to the provider in question
    for( TInt i = iScripts.Count() - 1; i >= 0; i-- )
        {
        CCFScript* script = iScripts[i];
        if( script->HasDependency( aProviderUid ) )
            {
            // Script has dependency to the provider, remove script
            err = KErrNone;
            if( aRollback )
                {
                // Rollback needed, create a copy from info
                TRAPD( rollbackErr, AddRollbackInfoL( *script ) );
                if( rollbackErr != KErrNone )
                    {
                    TPtrC name( script->Name() );
                    ERROR_2( rollbackErr, "Failed to rollback script [%S] from client [%x]",
                        &name, script->OwnerUid().iUid );
                    }
                }
            iScripts.Remove( i );
            delete script;
            script = NULL;
            }
        }
    
    return err;
    }

//------------------------------------------------------------------------------
// CCFScriptHandler::RollbackScripts
//------------------------------------------------------------------------------
//
void CCFScriptHandler::RollbackScripts()
    {
    FUNC_LOG;
    
    HBufC* path = HBufC::New( KMaxFileName );
    if( path )
        {
        TPtr pathPtr( path->Des() );
        TBool rollback = EFalse;
        
        // Go through roll back list and automatically search for scripts in rom
        // or ram. If a script is not found, owner of the script will be notified
        // that an automatic script dereigstration has occured.
        for( TInt i = iRollbackList.Count() - 1; i >= 0; i-- )
            {
            // Format file path from script info
            CCFScriptInfo* info = iRollbackList[i];
            ScriptFilePath( *info, pathPtr );
            if( BaflUtils::FileExists( iFs, pathPtr ) )
                {
                // Script found from default system drive
                rollback = ETrue;
                }
            else
                {
                // Script not found from default system drive, check rom if the
                // owner if cfserver
                if( info->OwnerUid() == KCFServerSid )
                    {
                    // Check from rom
                    pathPtr[0] = iDefaultRomDrive;
                    if( BaflUtils::FileExists( iFs, pathPtr ) )
                        {
                        // Script found from default rom drive
                        rollback = ETrue;
                        }
                    }
                }
            
            // Rollback if possible
            if( rollback )
                {
                // Rollback
                TRAPD( err, RollbackScriptL( *info, pathPtr ) );
                TPtrC name( info->Name() );
                if( err == KErrNone )
                    {
                    INFO_2( "Script [%S] from owner [%x] succesfull restored",
                        &name, info->OwnerUid().iUid );
                    }
                else
                    {
                    INFO_2( "Script [%S] from owner [%x] failed to restore",
                        &name, info->OwnerUid().iUid );
                    }
                
                // Clean up rollback info
                iRollbackList.Remove( i );
                delete info;
                info = NULL;
                }
            }
        
        // Check scripts which could not be automatically activated
        while( iRollbackList.Count() )
            {
            // Get the first script info
            CCFScriptInfo* info = iRollbackList[0];

            // Check if the owner is set and check all the other scripts by the
            // same owner
            MCFScriptOwner* owner = info->OwnerSession();
            if( owner )
                {
                // Owner found, notify owner
                NotifyScriptIds( owner );
                }
            else
                {
                // Owner not set, delete info and update rollback list
                iRollbackList.Remove( 0 );
                delete info;
                info = NULL;
                }
            }
        
        // Clean up
        delete path;
        path = NULL;
        }
    else
        {
        ERROR( KErrNoMemory, "Failed to rollback scripts due OOM" );
        }
    iRollbackList.ResetAndDestroy();
    }

//------------------------------------------------------------------------------
// CCFScriptHandler::DeregisterScriptOwner
//------------------------------------------------------------------------------
//
void CCFScriptHandler::DeregisterScriptOwner( MCFScriptOwner* aScriptOwner )
    {
    FUNC_LOG;
    
    for( TInt i = 0; i < iScripts.Count(); i++ )
        {
        CCFScriptInfo& info = iScripts[i]->Info();
        if( info.OwnerSession() == aScriptOwner )
            {
            info.SetOwnerSession( NULL );
            }
        }
    }

// -----------------------------------------------------------------------------
// CCFScriptHandler::InitializePhaseL
// -----------------------------------------------------------------------------
//
void CCFScriptHandler::InitializePhaseL( CCFPhaseBase::TCFPhaseId aPhaseId )
    {
    FUNC_LOG;

    MCFStarterObserver* observer 
    	= (MCFStarterObserver*) iOperationPluginManager;
    observer->InitializePhaseL( aPhaseId );
    
    switch( aPhaseId )
        {
        case CCFPhaseBase::ECFDeviceStarting:
            {
            InitDeviceStartingPhaseL();
            break;
            }
        case CCFPhaseBase::ECFLoadingRules:
            {
            InitDeviceStartedPhaseL();
            break;
            }
        default:
            {
            break;
            }
        }
    }

//----------------------------------------------------------------------------
// CCFScriptHandler::SetEventHandler
//----------------------------------------------------------------------------
//
void CCFScriptHandler::SetEventHandler( MCFStarterEventHandler& /*aEventHandler*/ )
    {
	  FUNC_LOG;
    }

// -----------------------------------------------------------------------------
// CCFScriptHandler::UpdatePlugInsL
// -----------------------------------------------------------------------------
//
void CCFScriptHandler::UpdatePlugInsL()
    {
    FUNC_LOG;

    iOperationPluginManager->UpdatePlugInsL(); // Let the "wait starter" to proceed.
    }


// -----------------------------------------------------------------------------
// CCFScriptHandler::ParseFileCompleteL
// -----------------------------------------------------------------------------
//
void CCFScriptHandler::ParseFileCompleteL()
    {
    FUNC_LOG;

    iWaitParsing->AsyncStop(); // Let the "wait starter" to proceed.
    }

// -----------------------------------------------------------------------------
// CCFScriptHandler::GetData
// -----------------------------------------------------------------------------
//
void CCFScriptHandler::GetData( TPtrC8& aPtr, TRequestStatus& aStatus )
    {
    FUNC_LOG;

    TRequestStatus *requestStatus = &aStatus;
    switch( iParserDataProvidingState )
        {
        case KInit:
            aPtr.Set( iParserData );
            iParserDataProvidingState = KDataSent;
            User::RequestComplete( requestStatus, KMoreData );
            break;
        case KDataSent:
            iParserDataProvidingState = KDone;
            User::RequestComplete( requestStatus, KDataStreamEnd );
            break;
        case KDone:
            User::RequestComplete( requestStatus, KDataStreamEnd );
            break;
        default:
            User::RequestComplete( requestStatus, KDataStreamError );
            break;
        };
    }

// -----------------------------------------------------------------------------
// CCFScriptHandler::Disconnect
// -----------------------------------------------------------------------------
//
void CCFScriptHandler::Disconnect()
    {
    FUNC_LOG;

    iParserData.Set( 0, 0 ); // Parser data can be invalidated.
    iParserDataProvidingState = KDone;
    }

// -----------------------------------------------------------------------------
// CCFScriptHandler::AddScriptL
// Method that implements the adding operation of a new script.
// -----------------------------------------------------------------------------
//
TInt CCFScriptHandler::AddScriptL( const TDesC& aName,
    const TDesC8& aScript,
    const TUid& aOwner,
    const RThread& aOwnerThread,
    MCFActionHandler& aActionHandler,
    TBool aDoSecurityCheck,
    MCFScriptOwner* aScriptOwner,
    TInt aScriptId )
    {
    FUNC_LOG;
    
    CCFScript* script = CreateScriptL( aName, aScript, aOwner, aOwnerThread,
        aActionHandler, aDoSecurityCheck, aScriptOwner, aScriptId );
    
    // Leave if script is not created
    User::LeaveIfNull( script );

    CleanupStack::PushL( script );
    if( iUpdatedScript )
        {
        iUpdatedScript->CleanupPersistentData();
        }
    script->ActivateL();
    User::LeaveIfError( iScripts.Append( script ) );
    CleanupStack::Pop( script );

    return aScriptId != KErrNotFound ? aScriptId : iNextId++;
    }


// -----------------------------------------------------------------------------
// CCFScriptHandler::RemoveScriptsBySession
// Removes all scripts that are associated to the given aSession.
// -----------------------------------------------------------------------------
//
TInt CCFScriptHandler::RemoveScriptsBySession(
    const MCFActionHandler& aSession )
    {
    FUNC_LOG;

    // local variable declarations and initialization
    TInt count( KErrNone );             // init to zero
    TInt index = iScripts.Count() - 1;  // index of last script

    // iterate through the scripts array
    // and remove scripts if necessary
    while ( index >= 0 )
        {
        // get the currently last script in the array
        CCFScript* script = iScripts[ index ];

        // remove the script if the session matches
        if ( &( script->ActionHandler() ) == &aSession )
            {
            // remove from internal array
            iScripts.Remove( index );
            delete script;
            script = NULL;
            // increment removed scripts count
            count++;
            }
        // decrement index
        index--;
        }
    // compress the array
    iScripts.GranularCompress();
    // return # of scripts removed
    return count;
    }

// -----------------------------------------------------------------------------
// CCFScriptHandler::RemoveScriptById
// Removes a script according to its unique Id, if it exists.
// -----------------------------------------------------------------------------
//
TInt CCFScriptHandler::RemoveScriptById( TInt aScriptId, const RThread& aOwner )
    {
    FUNC_LOG;

    // make sure got valid Id
    if ( aScriptId <= 0 )
        {
        ERROR_GEN( "Got negative script Id parameter" );
        return KErrArgument;
        }

    // Iterate through the entire scripts array
    TInt err = KErrNotFound;
    for ( TInt i = iScripts.Count() - 1; i >= 0; i-- )
        {
        CCFScript* script = iScripts[ i ];
        if ( script->ScriptId() == aScriptId )
            {
            // script found
			if ( script->OwnerUid() == aOwner.SecureId() )
            	{
            	// remove from internal array
            	iScripts.Remove( i );
            	delete script;
            	script = NULL;
	            err = KErrNone;
            	}
    		else
    			{
    			err = KErrAccessDenied;
    			}        	
            break;
            }
        }
    // return whether or not we removed the script
    return err;
    }

// -----------------------------------------------------------------------------
// CCFScriptHandler::RemoveScriptByName
// -----------------------------------------------------------------------------
//
TInt CCFScriptHandler::RemoveScriptByName( const TDesC& aScriptName,
    const TUid& aOwnerUid )
    {
    FUNC_LOG;
    
    TParsePtrC parsePtrC( aScriptName );
    TPtrC scriptName( parsePtrC.Name() );
    
    TInt err = KErrNotFound;
    for( TInt i = 0; i < iScripts.Count(); ++i )
        {
        CCFScript* script = iScripts[i];
        
        // Check owner
        if( script->OwnerUid() == aOwnerUid )
            {
            // Check name
            if( script->Name().CompareF( scriptName ) == KErrNone )
                {
                // Owner uid and name matches - remove script
                iScripts.Remove( i );
                delete script;
                script = NULL;
                err = KErrNone;
                break;
                }
            }
        }
    iScripts.GranularCompress();
    
    return err;
    }

// -----------------------------------------------------------------------------
// CCFScriptHandler::RemoveScriptByUid
// -----------------------------------------------------------------------------
//
TInt CCFScriptHandler::RemoveScriptByUid( const TUid& aUid )
    {
    FUNC_LOG;
    
    TInt err = KErrNotFound;
    for( TInt i = iScripts.Count() - 1; i >= 0; i-- )
        {
        CCFScript* script = iScripts[i];
        if( script->OwnerUid() == aUid )
            {
            // remove from internal array
            iScripts.Remove( i );
            delete script;
            script = NULL;
            err = KErrNone;
            }
        }
    iScripts.GranularCompress();

    return err;
    }

// -----------------------------------------------------------------------------
// CCFScriptHandler::InitDeviceStartingPhaseL
// -----------------------------------------------------------------------------
//
void CCFScriptHandler::InitDeviceStartingPhaseL()
    {
    FUNC_LOG;

    // Load context source manager configuration cenrep
    CRepository* cenRep = CRepository::NewLC( KCRUidCFRuleScriptConf );
    TInt count = 0;
    TInt err = cenRep->Get( KCRuleScriptNumberOfMandatoryRules, count );
    if( err == KErrNone && count )
        {
        INFO_1( "Found %d scripts from cenrep", count );
        
        TUint32 key = KCRuleScriptNumberOfMandatoryRules + 1;
        HBufC* fileName = HBufC::NewLC( KMaxFileName );
        TPtr fileNamePtr = fileName->Des();
        for( TInt i = 0; i < count; i++ )
            {
            // Ignore first key (count)
            err = cenRep->Get( key + i, fileNamePtr );
            if( err == KErrNone && fileNamePtr != KNullDesC )
                {
                // Check if the script has an upgrade
                CompleteFilePath( fileNamePtr );
                fileNamePtr[0] = iDefaultSystemDrive;
                if( !BaflUtils::FileExists( iFs, fileNamePtr ) )
                    {
                    // Use default rom drive
                    fileNamePtr[0] = iDefaultRomDrive;
                    }
                err = DoLoad( fileNamePtr );
                if( err == KErrNone )
                    {
                    CCFScript* script = iScripts[iScripts.Count() - 1];
                    if( script )
                        {
                        TInt err = iLoadedScripts.Append( script );
                        ERROR_1( err, "Unable to append script %S in loaded scripts array",
                            &fileNamePtr );
                        }
                    }
                }
            else
                {
                INFO_2( "Script in file '%S' skipped, error code %d", &fileNamePtr, err );
                }

            fileNamePtr.Zero();
            }
        CleanupStack::PopAndDestroy( fileName ); // fileName
        }
    CleanupStack::PopAndDestroy( cenRep ); // cenRep
    }

// -----------------------------------------------------------------------------
// CCFScriptHandler::InitDeviceStartedPhaseL
// -----------------------------------------------------------------------------
//
void CCFScriptHandler::InitDeviceStartedPhaseL()
    {
    FUNC_LOG;

    // Create search pattern and dir scanner
    HBufC* filePath = HBufC::NewLC( KMaxFileName );
    TPtr filePathPtr = filePath->Des();
    filePathPtr.Append( KDefaultRuleFilePath );
    filePathPtr.Append( KDefaultRuleFileSearchPattern );
    filePathPtr[0] = iDefaultRomDrive;

    CDirScan* dirScan = CDirScan::NewLC( iFs );
    dirScan->SetScanDataL( filePathPtr, KEntryAttNormal, ESortNone );
    CDir* dir = NULL;
    
    // First scan rom
    dirScan->NextL( dir );
    while( dir )
        {
        for( TInt i = 0; i < dir->Count(); i++ )
            {
            filePathPtr.Copy( dirScan->FullPath() );
            const TEntry& entry = ( *dir )[i];
            filePathPtr.Append( entry.iName );
            
            // Check for upgrade
            filePathPtr[0] = iDefaultSystemDrive;
            if( !BaflUtils::FileExists( iFs, filePathPtr ) )
                {
                // Upgrade not found - use rom file
                filePathPtr[0] = iDefaultRomDrive;
                }
            DoLoad( filePathPtr );
            }
        delete dir;
        dir = NULL;
        dirScan->NextL( dir );
        }
    
    // Ensure that cfserver default rule folder exists
    filePathPtr.Copy( KDefaultRuleFilePath );
    filePathPtr[0] = iDefaultSystemDrive;
    BaflUtils::EnsurePathExistsL( iFs, filePathPtr );

    // Scan ram for *.rul files but exclude all upgrade files
    filePathPtr.Append( KDefaultRuleFileSearchPattern );
    dirScan->SetScanDataL( filePathPtr, KEntryAttNormal, ESortNone );
    dirScan->NextL( dir );
    while( dir )
        {
        // Exlude all rom upgrade files
        TPtrC fullPath( dirScan->FullPath() );
        if( fullPath.Length() > KDefaultRuleFilePath().Length() )
            {
            for( TInt i = 0; i < dir->Count(); i++ )
                {
                filePathPtr.Copy( dirScan->FullPath() );
                const TEntry& entry = ( *dir )[i];
                filePathPtr.Append( entry.iName );
                DoLoad( filePathPtr );
                }
            }
        delete dir;
        dir = NULL;
        dirScan->NextL( dir );
        }
    
    // Cleanup
    CleanupStack::PopAndDestroy( dirScan );  // dirScan
    CleanupStack::PopAndDestroy( filePath ); // filePath
    
    // Close loaded scripts array since it is not needed anymore
    iLoadedScripts.Close();
    }

// -----------------------------------------------------------------------------
// CCFScriptHandler::CompleteFilePath
// -----------------------------------------------------------------------------
//
void CCFScriptHandler::CompleteFilePath( TDes& aFileName )
    {
    FUNC_LOG;

    if( aFileName.MaxLength() >
        aFileName.Length() + KDefaultRuleFilePath().Length() )
        {
        aFileName.Insert( 0, KDefaultRuleFilePath );
        }
    }

// -----------------------------------------------------------------------------
// CCFScriptHandler::LoadScriptFromFile
// -----------------------------------------------------------------------------
//
HBufC8* CCFScriptHandler::LoadScriptFromFile( const TDesC& aFilePath )
    {
    FUNC_LOG;
    
    INFO_1( "Loading script: %S", &aFilePath );

    HBufC8* script = NULL;
    RFile file;
    TInt err = file.Open( iFs, aFilePath, EFileRead );
    if( err == KErrNone )
        {
        TInt size = 0;
        err = file.Size( size );
        if( err == KErrNone )
            {
            script = HBufC8::New( size );
            if( script )
                {
                TPtr8 scriptPtr = script->Des();
                err = file.Read( scriptPtr );
                if( err == KErrNone )
                    {
                    // Strip all unnecassary data from script
                    TInt pos = scriptPtr.FindF( KScriptStartTag );
                    if( pos != KErrNotFound )
                        {
                        scriptPtr.Copy( scriptPtr.MidTPtr( pos ) );
                        }
                    else
                        {
                        // Incorrect script
                        delete script;
                        script = NULL;
                        }
                    }
                else
                    {
                    delete script;
                    script = NULL;
                    }
                }
            }
        }
    
    // Cleanup
    file.Close();
    
    ERROR_1( err, "Failed loading script: %S", &aFilePath );
    
    return script;
    }

// CCFScriptHandler::GetScriptsByUid
// -----------------------------------------------------------------------------
//
void CCFScriptHandler::GetScriptsByUid( const TUid& aUid,
    RPointerArray<CCFScript>& aArray ) const
    {
    FUNC_LOG;

    TInt err = KErrNone;
    TInt count = iScripts.Count();
    for( TInt i = 0; i < count; i++ )
        {
        CCFScript* script = iScripts[i];
        if( script->OwnerUid() == aUid )
            {
            err = aArray.Append( script );
            if( err != KErrNone )
                {
                // Something went wrong
                break;
                }
            }
        }
    }

// -----------------------------------------------------------------------------
// CCFScriptHandler::ScriptFilePath
// -----------------------------------------------------------------------------
//
HBufC* CCFScriptHandler::ScriptFilePath( CCFScriptInfo& aInfo ) const
    {
    FUNC_LOG;
    
    HBufC* filepath = HBufC::New( KMaxFileName );
    if( filepath )
        {
        TPtr filepathPtr( filepath->Des() );
        ScriptFilePath( aInfo, filepathPtr );
        }

    return filepath;
    }

// -----------------------------------------------------------------------------
// CCFScriptHandler::ScriptFilePath
// -----------------------------------------------------------------------------
//
HBufC* CCFScriptHandler::ScriptFilePath( const TDesC& aName,
    const TUid& aOwnerUid ) const
    {
    FUNC_LOG;
    
    HBufC* filepath = HBufC::New( KMaxFileName );
    if( filepath )
        {
        TPtr filepathPtr( filepath->Des() );
        if( aOwnerUid == KCFServerSid )
            {
            filepathPtr.Format( KDefaultRomRuleFileUpgradeFormat, &aName );
            }
        else
            {
            filepathPtr.Format( KDefaultSystemRuleFileFormat,
                aOwnerUid.iUid, &aName );
            }
        filepathPtr[0] = iDefaultSystemDrive;
        }

    return filepath;
    }

// -----------------------------------------------------------------------------
// CCFScriptHandler::ScriptFilePath
// -----------------------------------------------------------------------------
//
void CCFScriptHandler::ScriptFilePath( CCFScriptInfo& aInfo, TDes& aFile ) const
    {
    FUNC_LOG;

    aFile.Zero();
    TPtrC name( aInfo.Name() );
    if( aInfo.OwnerUid() == KCFServerSid )
        {
        aFile.Format( KDefaultRomRuleFileUpgradeFormat, &name );
        }
    else
        {
        aFile.Format( KDefaultSystemRuleFileFormat,
            aInfo.OwnerUid().iUid, &name );
        }
    aFile[0] = iDefaultSystemDrive;
    }

// -----------------------------------------------------------------------------
// CCFScriptHandler::DoSave
// -----------------------------------------------------------------------------
//
TInt CCFScriptHandler::DoSave( const TDesC& aFileName, const TDesC8& aScript )
    {
    FUNC_LOG;

    TInt err = KErrNone;
    
    // Create the target folder if one needed
    TParsePtrC parse( aFileName );
    TPtrC path( parse.DriveAndPath() );
    if( !BaflUtils::FolderExists( iFs, path ) )
        {
        // Create the target folder
        TInt err = iFs.MkDirAll( path );
        INFO_2( "Created rule path %S with code %d", &path, err );
        }

    // Create a new file and write script - replace
    // existing file if one exists
    RFile file;
    err = file.Replace( iFs, aFileName, EFileWrite );
    if ( err == KErrNone )
        {
        err = file.Write ( aScript, aScript.Length( ) );
        }
    INFO_2( "Saved script file %S with code %d",
        &aFileName, err );
    file.Close();
    
    return err;
    }

// -----------------------------------------------------------------------------
// CCFScriptHandler::DoLoad
// -----------------------------------------------------------------------------
//
TInt CCFScriptHandler::DoLoad( const TDesC& aFilePath )
    {
    FUNC_LOG;
    
    // Check if the script has already been loaded
    TBool loaded = EFalse;
    TUid uid;
    TPtrC name( KNullDesC );
    ParseUidAndName( aFilePath, uid, name );
    TInt err = KErrNone;
    if ( uid.iUid != 0 ) // If Uid == 0, subfolder is not valid
        {
        for( TInt i = 0; i < iLoadedScripts.Count(); i++ )
            {
            CCFScript* script = iLoadedScripts[i];
            if( script->OwnerUid() == uid &&
                script->Name().CompareF( name ) == KErrNone )
                {
                // Script already loaded
                INFO_1( "Script %S already loaded",
                    &aFilePath );
                loaded = ETrue;
                iLoadedScripts.Remove( i );
                break;
                }
            }
    
        // Load script if it has not already been loaded
        if( !loaded )
            {
            // Load script from file and configure script
            HBufC8* script = LoadScriptFromFile( aFilePath );
            if( script )
                {
                INFO_1( "Script %S loaded", &aFilePath );
               
                // If script is faulty just ignore it
                RThread thread;
                TRAP( err, AddScriptL( name,
                    *script,
                    uid,
                    thread,
                    iScriptEventListener,
                    EFalse,
                    NULL ) );
 
                // Clean up
                thread.Close(); // just in case
                delete script;
                script = NULL;
            
                ERROR_1( err, "Script %S not created", &aFilePath );
                if( err >= KErrNone )
                    {
                    INFO_1( "Script %S succesfully created and initialized",
                        &aFilePath );                        
                    }
                }
            else
                {
                err = KErrNotFound;
                }
            }
        }   
        
    return err >= 0 ? KErrNone : err;
    }
 
// -----------------------------------------------------------------------------
// CCFScriptHandler::ParseUidAndName
// -----------------------------------------------------------------------------
//
void CCFScriptHandler::ParseUidAndName( const TDesC& aFilePath,
    TUid& aUid, TPtrC& aName ) const
    {
    FUNC_LOG;
    
    // Resolve script name
    TParsePtrC parse( aFilePath );
    aName.Set( parse.Name() );
    
    // Resolve uid
    TPtrC path( parse.DriveAndPath() );
    if( KDefaultRuleFilePath().Length() == path.Length() )
        {
        // Cfserver sid since no additional sid folder found
        aUid.iUid = KCFServerSid.iUid;
        }
    else
        {
        if( path.Length() == ( KDefaultRuleFilePath().Length() + KUidLength + 1 ) )
            {
            // Parse client sid from the folder
            TInt start = KDefaultRuleFilePath().Length();
            TPtrC uidPtrC( path.Mid( start, KUidLength ) );
            TLex lex( uidPtrC );
       
            // Convert descriptor to unsigned int
            TUint32 value = 0;
            TInt err = lex.BoundedVal( value, EHex, KLimit );
            if( err == KErrNone )
                {
                aUid.iUid = value;
                }
            ERROR_1( err, "Unable to parse uid from path %S", &aFilePath );
            }
        else
            {
            INFO_1( "The additional folder in path %S is not sid folder", &aFilePath );
            // Set Uid = 0 and none script will be loaded from unsuitable subfolder
            aUid.iUid = 0;
            }
        }
    }

// -----------------------------------------------------------------------------
// CCFScriptHandler::CreateScriptL
// -----------------------------------------------------------------------------
//
CCFScript* CCFScriptHandler::CreateScriptL( const TDesC& aName,
    const TDesC8& aScript,
    const TUid& aOwner,
    const RThread& aOwnerThread,
    MCFActionHandler& aActionHandler,
    TBool aDoSecurityCheck,
    MCFScriptOwner* aScriptOwner,
    TInt aScriptId )
    {
    FUNC_LOG;
    
    // check that we have a descriptor with content
    TInt len = aScript.Length();
    if ( len <= 0 )
        {
        ERROR_GEN( "Script length invalid" );
        User::Leave( KErrBadDescriptor );
        }

    // Init parser usage.
    iParserData.Set( aScript );
    iParserDataProvidingState = KInit;

    iParser->SetSourceCharacterWidth( CMDXMLParser::EAscii );
    iParser->ParseSource( this );
    iWaitParsing->Start();

    // Check errors.
    CCFScript* script = NULL;
    TXMLErrorCodeSeverity severity = iParser->ErrorSeverity();
    if ( severity == EXMLWorkable || severity == EXMLNone )
        {
        CMDXMLDocument* document = iParser->DetachXMLDoc();
        if ( document )
            {
            CleanupStack::PushL( document ); // CLEANUP<< document
            CMDXMLElement* documentElement = document->DocumentElement();
            if ( documentElement )
                {
                // Get script root
                CMDXMLNode* child = documentElement->FirstChild();
                while ( child )
                    {
                    if ( child->NodeType() != CMDXMLNode::EElementNode )
                        {
                        child = child->NextSibling();
                        continue;
                        }
                    else if ( script )
                        {
                        // One aScript is allowed to have one root (one tree).
                        User::Leave( KErrNotSupported );
                        }
                    
                    // Resolve script id
                    TInt scriptId = 0;
                    if( aScriptId != KErrNotFound )
                        {
                        scriptId = aScriptId;
                        }
                    else
                        {
                        scriptId = iNextId;
                        }
                    
                    // Create new script
                    TParsePtrC parse( aName );
                    script = CCFScript::NewLC( *child,
                        parse.Name(),
                        scriptId,
                        aOwner,
                        aActionHandler,
                        len,
                        iCF,
                        iSecurityChecker,
                        *iOperationPluginManager,
                        iFs,
                        aScriptOwner ); // CLEANUP<< script

                    // Do security check for the script.
                    if ( aDoSecurityCheck )
                        {    
                        User::LeaveIfError(
                            script->CheckSecurity( aOwnerThread ) );
                        }

                    child = child->NextSibling();
                    }
                if( script )
                    {
                    CleanupStack::Pop( script );                // CLEANUP>> script
                    }
                }
            CleanupStack::PopAndDestroy( document );    // CLEANUP>> document
            }
        }
    else
        {
        ERROR_GEN_1( "CCFScriptHandler::CreateScriptL - Parser error [%d]",
                iParser->Error() );
        User::LeaveIfError( iParser->Error() );
        }

    // Reset parser usage.
    iParserData.Set( 0, 0 ); // Parser data will not be valid anymore.
    iParserDataProvidingState = KError;
    
    return script;
    }

//------------------------------------------------------------------------------
// CCFScriptHandler::RollbackScriptL
//------------------------------------------------------------------------------
//
void CCFScriptHandler::RollbackScriptL( const CCFScriptInfo& aInfo,
    const TDesC& aFilePath )
    {
    FUNC_LOG;
    
    HBufC8* script = LoadScriptFromFile( aFilePath );
    User::LeaveIfNull( script );
    
    CleanupStack::PushL( script );
    RThread mainThread;
    CleanupClosePushL( mainThread );
    
    // Security check is not needed since the client has already
    // succesfully registered script, using cfsrver main thread as a temp
    // thread handle
    AddScriptL( aInfo.Name(), *script, aInfo.OwnerUid(), mainThread,
        iScriptEventListener, EFalse, aInfo.OwnerSession(), aInfo.Id() );

    // Clean up
    CleanupStack::PopAndDestroy( &mainThread );
    CleanupStack::PopAndDestroy( script );
    }

//------------------------------------------------------------------------------
// CCFScriptHandler::NotifyScriptIds
//------------------------------------------------------------------------------
//
void CCFScriptHandler::NotifyScriptIds( MCFScriptOwner* aScriptOwner )
    {
    FUNC_LOG;
    
    RArray<TInt> scriptIds;
    for( TInt i = iRollbackList.Count() - 1; i >= 0; i-- )
        {
        CCFScriptInfo* info = iRollbackList[i];
        if( aScriptOwner == info->OwnerSession() )
            {
            // Error is ignored. If there is no memory left then the client
            // will miss the deregister notification of some scripts
            scriptIds.Append( info->Id() );
            
            // Remove the info from rollback list
            iRollbackList.Remove( i );
            delete info;
            info = NULL;
            }
        }
    aScriptOwner->HandleScriptsRemoved( scriptIds );
    scriptIds.Close();
    }

//------------------------------------------------------------------------------
// CCFScriptHandler::AddRollbackInfoL
//------------------------------------------------------------------------------
//
void CCFScriptHandler::AddRollbackInfoL( CCFScript& aScript )
    {
    FUNC_LOG;

    CCFScriptInfo* info = aScript.CopyInfoLC();
    iRollbackList.AppendL( info );
    CleanupStack::Pop( info );
    }

//------------------------------------------------------------------------------
// CCFScriptHandler::DeleteScriptFile
//------------------------------------------------------------------------------
//
TInt CCFScriptHandler::DeleteScriptFile( const TDesC& aFilePath,
    const TUid& aOwnerUid )
    {
    FUNC_LOG;
    
    TInt err = KErrNotFound;
    if( BaflUtils::FileExists( iFs, aFilePath ) )
        {
        err = BaflUtils::DeleteFile( iFs, aFilePath );
        INFO_3( "Deleted script file %S from client %x with code %d",
            &aFilePath, aOwnerUid.iUid, err );
        if( err == KErrNone )
            {
            // All persistent data clean ups are not treated as errors
            CDir* dir = NULL;
            TParsePtrC parse( aFilePath );
            TPtrC driveAndPath( parse.DriveAndPath() );
            TPtrC name( parse.Name() );
            
            // Cleanup all persistent data files
            HBufC* persistentData = HBufC::New( KMaxFileName );
            if( persistentData )
                {
                TPtr persistentDataPtr( persistentData->Des() );
                persistentDataPtr.Format( KPersistenDataFormat,
                    aOwnerUid.iUid, &name, &KWildCardChar );
                TInt persistentErr = iFs.GetDir( persistentDataPtr, KEntryAttNormal,
                    ESortNone, dir );
                if( persistentErr == KErrNone && dir )
                    {
                    for( TInt i = 0; i < dir->Count(); i++ )
                        {
                        TParsePtrC entryNameParse( (*dir)[i].iName );
                        TPtrC entryName( entryNameParse.Name() );
                        persistentDataPtr.Format( KPersistenDataFormat2,
                            aOwnerUid.iUid, &entryName );
                        persistentDataPtr[0] = iDefaultSystemDrive;

                        persistentErr = BaflUtils::DeleteFile(
                            iFs, persistentDataPtr );
                        INFO_2( "Deleted persistent data file %S with code %d",
                            &persistentDataPtr, persistentErr );
                        }
                    }
                
                // Clean up
                delete dir;
                dir = NULL;

                delete persistentData;
                persistentData = NULL;
                }
            
            // Check if not in rules base folder - delete this folder
            // if the folder is empty
            if( aOwnerUid != KCFServerSid )
                {
                TInt rmDirErr = iFs.GetDir( driveAndPath, KEntryAttNormal,
                    ESortNone, dir );
                if( rmDirErr == KErrNone && dir )
                    {
                    if( dir->Count() == 0 )
                        {
                        rmDirErr = iFs.RmDir( driveAndPath );
                        INFO_2( "Removed directory %S with code %d",
                            &driveAndPath, rmDirErr );
                        }
                    }
                delete dir;
                dir = NULL;
                }
            }
        }
    return err;
    }

// End of file
